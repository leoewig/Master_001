/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    mmc_aps.c
 *
 * description:  
 *  Contains main entry point and startup for MMC application.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Feb-2  Jun Wu           initial version
 *
 *****************************************************************************/

#include <mmc.h>
#include "build_idents.h"
#include "scheduler.h"
#include "mmc_aps.h"
#include "mmc_aps_p.h"  /* private header for this module */
#include <pit_cfg.h>
#include "dio_cfg.h"
#include "rel_id.h"

#include "utils.h"
/*****************************************************************************
 * Headers for drivers
 *****************************************************************************/
#include <hardware.h>   /* for ENABLE_INTERRUPTS */
#include <gate.h>       /* for XGSWT */

#include <isr.h>        /* for init_interrupts() */
#include <watchdog.h>
#include <platform.h>
#include <ai.h>
#include <timing_monitor.h>     /* for TIMING_MONITOR macro */
#include "stm.h"

/*****************************************************************************
 * Headers for features initialised from here
 *****************************************************************************/
#include <fm.h>
#include <om.h>
#include <smg.h>
#include <ts.h>
#include <ms.h>
#include <vio.h>
#include <pp.h>
#include <log.h>
#include <mmc_rtw.h>
#include <nvs.h>
#include <mdm.h>
#include <mdm_ee.h>
#include "dio.h"
#include <exc.h>
#include <pit.h>
#include <mmc_ican.h>
#include <mmc_danomem.h>
#include <dispatch_mmc_utils.h>
#include <cal.h>
#include <ixcp.h>
#include <flash_fault.h>
#include <flash_eee.h>
#include <vprg.h>
#include "pwm.h"
#include "ect.h"
#include "Main_isr_types.h"
#include "isr.h"
#include "smm.h"
#include "motor.h"
#include "lin.h"
#include "tmm.h"
//#include "sci.h"
#include "tmm_fm.h"
#include "rtc.h"
#include "tmm_seq.h"
#include <dac.h>
/*****************************************************************************
 * MACROS
 *****************************************************************************/

/**
 * @todo examine these PIT periods */
#define FGLOOP_PERIOD_US    (MMC_FG_PIT_MS * 1000uL)  /**< foreground loop period (us) */
/*
 * tasks
 */
#define     N_TASKS         3u  /**< total number of tasks */

#if N_TASKS>SCHEDULER_MAX_TASK_COUNT
#error Too many tasks defined
#endif

/**
 * @lint
 * macro not referenced
 * -esym(750,BG_TASK_PRIORITY) */

/**
 * priority of background calculation task
 * @note must be the same as the index of bg_task() in task_defs[]
 */

#define     BG_TASK_PRIORITY    ((task_priority_level_t) 1u)

/**
 * priority of foreground task
 * @note must be the same as the index of fg_task() in task_defs[]
 */
#define     FG_TASK_PRIORITY    ((task_priority_level_t) 2u)

/******************************************************************************
 * CALIBRATABLE PARAMETERS
 *****************************************************************************/

#undef MEMORY_LOCATION
#define MEMORY_LOCATION CALIBRATION_MEMORY
#include "memory_layout.h"

PRIVATE CAL bool_t aps_prevent_shutdown = FALSE;
/**<
 * @a2l{conversion,bool_t}
 * @a2l{longidentifier,Calibratable to prevent MMC shutting down. Only to be used in PATS }
 */

#undef MEMORY_LOCATION
#define MEMORY_LOCATION S12_MEMORY
#include "memory_layout.h"

/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/
ts_time_t aps_fg_start_ms;
/**<
 * @a2l{conversion, ts_time_t}
 * @a2l{LongIdentifier, The timestamp of the most recent foreground task start}
 * This is of general interest to most foreground tasks, so it is placed here
 * @note this variable is set in the foreground context
 */

ts_time_t aps_bg_start_ms;
/**<
 * @a2l{conversion, ts_time_t}
 * @a2l{LongIdentifier, The timestamp of the most recent background task start}
 * This is of general interest to most background tasks, so it is placed here
 * @note this variable is set in the background context
 */

/** @lint  lint -esym(551,aps_bg_delta_ms)
 * Symbol not accessed - provided as a diagnostic */
/*lint -esym(551,aps_bg_delta_ms) */
PRIVATE ts_time_t aps_bg_delta_ms;
/**
 * @a2l{conversion, ts_time_t}
 * @a2l{LongIdentifier, Maximum bg task execution time} */
PRIVATE ts_time_t aps_bg_max_ms;

/*------------------------------------------------------------------*/

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

PRIVATE bool_t aps_limited_mode =FALSE;

/**
 * Array of tasks present in the platform test application
 * priority is from lowest to highest.
 *
 * @note the order entries must be consistent with task priority definitions
 * above
 */
/** @polyspace<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes */
PRIVATE const struct task_definition_s task_defs[N_TASKS] = {
    { &idle_task, TASK_OPTIONS_NONE },
    { &bg_task,   TASK_OPTIONS_NONE },
    { &fg_task,   TASK_OPTIONS_NONE }
    };

/**
 * @lint esym(551,task_*_ctr)
 * The task counters are provided so that task invocations can be monitored in
 * a debugger etc. It is intentional that they are not accessed */
/*lint -esym(551,task_*_ctr) */
PRIVATE volatile uint32_t task_idle_ctr = 0u;
/**<
 * @a2l{conversion, uint32_t}
 * @polyspace<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes
 */

PRIVATE volatile uint32_t task_fg_ctr = 0u;
/**<
 * @a2l{conversion, uint32_t}
 * @polyspace<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes
 */

PRIVATE volatile uint32_t task_bg_ctr = 0u;
/**<
 * @a2l{conversion, uint32_t}
 * @polyspace<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes
 */

PRIVATE volatile uint32_t fg_pit_ctr = 0u;
/**<
 * @a2l{conversion, uint32_t}
 * @polyspace<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes
 */

/*lint -esym(551,fg_task_*_ticks) */
PRIVATE ts_ticks_t fg_task_start_ticks=0u;
PRIVATE ts_ticks_t fg_task_delta_ticks=0u;
PRIVATE ts_ticks_t fg_task_max_ticks=0u;

PRIVATE ts_ticks_t fg_ip_delta_ticks=0u;
PRIVATE ts_ticks_t fg_ip_max_ticks=0u;

PRIVATE ts_ticks_t fg_upd_start_ticks=0u;
PRIVATE ts_ticks_t fg_upd_delta_ticks=0u;
PRIVATE ts_ticks_t fg_upd_max_ticks=0u;

PRIVATE ts_ticks_t fg_op_start_ticks=0u;
PRIVATE ts_ticks_t fg_op_delta_ticks=0u;
PRIVATE ts_ticks_t fg_op_max_ticks=0u;

/**
 * @lint -esym(551,idle_pos_at_reset,bg_pos_at_reset,fg_pos_at_reset)
 * Debug symbols not accessed
 */
/*lint -esym(551,idle_pos_at_reset,bg_pos_at_reset,fg_pos_at_reset) */
#pragma section {uninit}
PRIVATE uint32_t idle_pos = 1u;
PRIVATE uint32_t bg_pos = 1u;
PRIVATE uint32_t fg_pos = 1u;
#pragma section {}
PRIVATE uint32_t idle_pos_at_reset;
PRIVATE uint32_t bg_pos_at_reset;
PRIVATE uint32_t fg_pos_at_reset;

#define MEMORY_LOCATION CALIBRATION_MEMORY_S12
#include "memory_layout.h"

/**
 * The length of time to wait for the PSU to depower the MMC before resetting.
 * This must be larger than TPSU_HOLD_TIME
 * @mantis{5090} APS requirement needed for this? */
PRIVATE CAL ts_time_t aps_shutdown_wait = 5000;

#undef  MEMORY_LOCATION
#define MEMORY_LOCATION S12_MEMORY
#include "memory_layout.h"


/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/


/**
 * @lint
 * platform_main is invoked from crsti2.s (assembler), hence will not be
 * recognised by Lint\n
 * -esym(765,platform_main) external could be static
 */
/*lint -esym(765,platform_main) */
FCODE void platform_main(void)
{
    /*
     * Reset was not from natural causes so log what we were doing when
     * the reset occurred
     */
    if (bootblock_eee.last_reset == Reset_uncontrolled)
    {
        idle_pos_at_reset = idle_pos;
        bg_pos_at_reset = bg_pos;
        fg_pos_at_reset = fg_pos;
    }
    /* Configure S12 interrupts to the default */
    init_interrupts();

    /* enable the flash ECC error interrupt */
    isr_flash_fault_init();

    /* initialize support for the fbl security */
    flash_fbl_sec_support_init();

    /* Start the watchdog going before init of other drivers.
     * Watchdog does not need DIO feature to manipulate its port.
     * DIO does not specify the watchdog port as either an output or
     * an input, so the DDR bit set in init_watchdog remains in place after
     * the call to dio_init(). The watchdog must be kickable in drivers_init()
     **/
    init_watchdog();

    /* Initialise shared platform layer */
    drivers_init();

    /* Defensively reconfigure the watchdog in case anything untoward happened
     * whilst the other I/O ports were configured. */
    init_watchdog();

    /* Initialise mmc-specific CAN interface */
    mmc_ican_init();

    /* Init the xgate driver (used by ICM) */
    gate_init();
	
	mdm_ee_init();

	ect_init();

	/*pwm*/
	pwm_init();
	
	motor_init();

	lin_init();

	tmm_init();

	//sci_init();

    dac_init();

    /* Initialise all features allocated to tasks */
    aps_features_init();

    /** @note This next function never returns */
    aps_run_scheduler();

    /* MS is shutting down the MMC, perform last few tidy-up actions */
    //aps_shutdown();

    /*
     * not expecting to get to here
     */
    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    INTERNAL_ERROR();
}

/**
 * Trigger a single execution invocation of the backround task.
 * @pre MUST NOT be called from an interrupt context */
FCODE void aps_run_bg_task(void)
{
    /** @todo the return value of this function is meaningless */
    (void)scheduler_task_set_event(BG_TASK_PRIORITY);
}

/*****************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/**
    arrange to send a S.U.T Reset Frame - either with SVN info or build time
 **/
PRIVATE void SendMmcIdFrame(void)
{
//FPTR const char_t* p = release_id;
//
//    CAN_TX_SIG(ID_year,   0xFF );  /* indicates this is not a date but an SVN stamp */
//    CAN_TX_SIG(ID_month,  ((p[0] == '_') ? 0u : 1u) + ((p[1] == '_') ? 0u : 2u) );
//    CAN_TX_SIG(ID_date,   0u);
//    CAN_TX_SIG(ID_hour,   ascii2bin(p[3], p[4]));
//    CAN_TX_SIG(ID_minute, ascii2bin(p[5], p[6]));
//    CAN_TX_SIG(ID_second, (uint8_t)(p[7]));
//    CAN_TX_SIG(ID_reason, 0u);
//    CAN_TX_SIG(ID_build,  BUILD_IDENT_MMC);
}


/*****************************************************************************
 * Feature initalisation
 ****************************************************************************/
/**
 * Initialisation function for all features which are invoked in tasks
 * Go through all tasks and initalise them in the appropriate order.
 * - restore NV data first, and optionally take action on failure
 * - initialise the rest of the interfaces layer
 * - initialise foreground tasks
 * - initialise background tasks
 *
 *   @pre This must be performed after platform initalisation and before the
 *   scheduler is started.
 *
 **/
PRIVATE FCODE void aps_features_init(void)
{
    /* Initialise the rest of the 'platform' level interfaces
     * specific to this target.
     **/
    aps_interface_init();

    /** Initialise foreground task items */
    aps_fg_init();
    aps_bg_init();

    /* Now that everything is initalised, start CANbedded running */
    exc_start();
    exc_update();
}


/**
 * Initialisation of interface-level components before the strategy proper is
 * kicked off.
 */
PRIVATE FCODE void aps_interface_init(void)
{
   /* external CAN */
    exc_init();
    /* Output monitoring - reads from imi, so must go after that */
    //om_init();
    /* Data manager */
    //mdm_init();
}

/**
 * Initialisation function for foreground task strategy features.
 * Go down the list of strategy features and invoke their init() functions
 * observing dependency order
 * */
PRIVATE FCODE void aps_fg_init(void)
{
    //pp_init();  /* pack protection */
    vio_init(); /* Vehicle interface */

    //log_init(); /* logging */
}

/**
 * Calls the init functions of all features allocated to the background task
 * */
PRIVATE FCODE void aps_bg_init(void)
{

}

/*****************************************************************************
 * Task running
 ****************************************************************************/

/**
 * Configure the scheduler with the MMC tasks, and start it running
 * Start the pre-emptive scheduler running */
PRIVATE FCODE void aps_run_scheduler(void)
{
    fn_scheduler_init_return_t sched_init_ret;
    fn_scheduler_start_return_t sched_start_ret;

    sched_init_ret = scheduler_init(task_defs,N_TASKS);

    if (SCHEDULER_INIT_OK != sched_init_ret )
    {
        /** @todo is any other action required?
         * Initialisation of scheduler has failed */
        INTERNAL_ERROR();
    }
    else
    {
        sched_start_ret = scheduler_start();
        if (SCHEDULER_START_OK != sched_start_ret )
        {
            INTERNAL_ERROR();
            /**
             * @todo scheduler failed to start */
        }
    }
}

/**
 * Shutdown the MMC
 * - store the current copy of the NVM image
 * - if ignition is low, then remove the power hold
 * - if ignition has been re-applied, then force a reset
 */
PRIVATE FCODE void aps_shutdown()
{
   
}

/*****************************************************************************
 * Scheduled task callbacks
 ****************************************************************************/

/**
 * @lint * Symbol not accessed\n * -esym(715,p_level)\n
 */
/*lint -esym(715,p_level) */

PRIVATE SCHEDULER_TASK_DECL(idle_task)
{
    bool_t nvs_complete;

    /* Other timer-based task interrupts must be kicked off in here in order to
     * allow the scheduler to have started up properly before any interrupts
     * for context switching occur */

   /* Clear the MS shutdown complete flag */
  // ms_fgloop_shutdown_complete = FALSE;

   stm_init();
   /* Configure the PIT allocated for the foreground task to be handled on the
     * S12*/
    PitEnableInterrupts(FGLOOP_PIT,INT_LVL_DEFAULT,INT_CPU_DEST);
    PitStart(FGLOOP_PIT,(uint16_t) (FGLOOP_PERIOD_US/PIT_TICK_US));
	//ad_pit_init();

    /* Once interrupts are enabled, the foreground task can break into this*/
    ENABLE_INTERRUPTS();

    /**
     * send a s/w trigger to the XGATE to do its intialisation check
     */
    SET_XGSWT(0u);

   // SendMmcIdFrame();

    /**
     * @lint
     * Infinite loop\n
     * !e716
     *
     * This is the 'idle' task, it should never return
     */
    while (1) /*lint !e716  while (1) */
    {
        TIMING_MONITOR_TOGGLE(Timing_monitor_idle_loop);
        idle_pos = 1u;
        task_idle_ctr++;
        exc_idle_tasks(); /* idle tasks like the CANbedded XCP checksum
         //                    calculation */
        idle_pos = 2u;
        //ixcp_idle_tasks(); /* idle tasks like the IXCP checksum calculation */
        idle_pos = 3u;
        cal_idle_tasks(); /* EEPROM writes to mirror calibration RAM */
        idle_pos = 4u;
        flash_eee_idle_tasks();       /* Emulated EEPROM housekeeping */
        idle_pos = 5u;
		//ect_read_inputs();
        idle_pos = 6u;
        idle_pos = 7u;
        idle_pos = 10u;
        stm_update();
        idle_pos = 0u;
    }
}


/* lint -esym(765,fg_task) */
PRIVATE SCHEDULER_TASK_DECL(fg_task)
{
    TIMING_MONITOR_ON(Timing_monitor_fg);
	
    fg_pos = 1u;
    task_fg_ctr++;
    fg_task_start_ticks=ts_get_ticks();
    fg_pos = 2u;
    aps_fg_start_ms=ts_get_ms();

    /* Read hardware inputs */
    fg_pos = 3u;
    drivers_read_inputs();
    /* Read inputs from comms layers */
    fg_pos = 4u;
    /* Read in hardware interface inputs */
    fg_pos = 5u;
	/* LED */
	if(task_fg_ctr%100==0)
	{
		PORTK|=0x01;	
	}
	else 
	{
	if (task_fg_ctr%50==0)
		{
		PORTK &= ~0x01;
		}	
	}

    fg_pos = 6u;
  
	fg_pos = 7u;
	//LIN_read_inputs();
	fg_pos = 8u;
	pwmin_read_inputs();
	fg_pos = 9u;

	fg_pos = 10u;
	ai_read_inputs();
	fg_pos = 11u;
	/* (mostly) CAN inputs */
	vio_read_inputs();
	fg_pos = 12u;
    tmm_seq_update();

	tmm_update();
	/* check for faults on the internal bus and process diagnostic messages */
	fg_pos = 13u;
	motor_rotating_update();
	fg_pos = 14u;
	UPDATE_MAX_TICKS(fg_ip_max_ticks,fg_ip_delta_ticks, fg_task_start_ticks);
	fg_pos = 15u;
	fg_upd_start_ticks=ts_get_ticks();
	fg_pos = 16u;
	//mdm_update(); /* XGATE->S12   and foreground to background data swap */
	fg_pos = 20u;
	tmm_fm_update();
	fg_pos = 21u;

	fg_pos = 22u;

	fg_pos = 23u;

	fg_pos = 24u;
    dac_update();
	fg_pos = 25u;

	fg_pos = 26u;

	fg_pos = 27u;

	fg_pos = 28u;
	UPDATE_MAX_TICKS(fg_upd_max_ticks,fg_upd_delta_ticks,fg_upd_start_ticks);
	fg_pos = 29u;
	fg_op_start_ticks=ts_get_ticks();
	fg_pos = 30u;
    lin_seq_update();

	fg_pos = 31u;

    //PORTK|=0x01;

        lin_update();
    
    
    //PORTK &= ~0x01;
	fg_pos = 32u;
	vio_write_outputs();    /* Calls CANbedded */
	fg_pos = 33u;

	
	fg_pos = 34u;
	//sci_write_outputs();
	fg_pos = 35u;

	exc_update();

	fg_pos = 36u;
	
    fg_pos = 37u;
    flash_nonfatal_fault_check();
    fg_pos = 38u;
    flash_fbl_sec_support_update();
    fg_pos = 39u;
    dio_write_outputs();        /* Write output ports */
    fg_pos = 40u;

    fg_pos = 41u;

    fg_pos = 43u;
    UPDATE_MAX_TICKS(fg_op_max_ticks,fg_op_delta_ticks,fg_op_start_ticks);
    fg_pos = 44u;
    UPDATE_MAX_TICKS(fg_task_max_ticks,fg_task_delta_ticks,fg_task_start_ticks);
    fg_pos = 45u;
    kick_watchdog();
    fg_pos = 0u;
    TIMING_MONITOR_OFF(Timing_monitor_fg);	
}

PRIVATE SCHEDULER_TASK_DECL(bg_task)
{
	PORTE |= (0x08);
	
    TIMING_MONITOR_ON(Timing_monitor_bg);
    bg_pos=1u;
    task_bg_ctr++;
    bg_pos=2u;
    aps_bg_start_ms=ts_get_ms();
    /* This function must be called to write the inputs of the bg features
     * and must be placed before calls to other bg features. */
    bg_pos=3u;

    /* Start of background Feature invocations*/
    bg_pos=4u;
    if (1) /* This calls TMM management */
    {
        bg_pos=5u;
		mdm_TMM_task();

    }

    bg_pos=6u;


    /*************************************************************************
     * Background task cleanup occurs below this point in the function.
     * DO NOT add other feature invocations below here
     *************************************************************************/

    /* This function reads the outputs of bg features and must be placed after
     * ALL other calls to background functionality. */
    bg_pos=7u;

    bg_pos=8u;
    aps_bg_delta_ms = ts_get_ms() -  aps_bg_start_ms;
    if (aps_bg_delta_ms>aps_bg_max_ms) {
        aps_bg_max_ms = aps_bg_delta_ms;
    }
    bg_pos=0u;
    TIMING_MONITOR_OFF(Timing_monitor_bg);
	
}

/**
 * A function to query whether the bg task is running or will start running
 * if higher priority tasks allow it to.
 * Attempting to start the bg task when it is already active will fail
 * because the task is configured with #TASK_OPTIONS_NONE
 *
 * @ref TASK_STATUS_READY means means the event to run it has not been
 * triggered. It should never be #TASK_STATUS_DISABLED after the scheduler
 * has started.
 *
 * @retval TRUE if the bg task is running waiting to run
 * @retval FALSE otherwise
 **/
bool_t aps_is_bg_active(void)
{
    task_status_t bg_status;
    bool_t bg_active;
    bg_status = scheduler_get_task_status( BG_TASK_PRIORITY );

    switch (bg_status)
    {
        case TASK_STATUS_WAITING:
        case TASK_STATUS_RUNNING:
            bg_active=TRUE;
            break;
        case TASK_STATUS_DISABLED:
        case TASK_STATUS_READY:
        case TASK_STATUS_UNKNOWN:
        default:
            bg_active=FALSE;
            break;
    }
    return bg_active;
}

/*****************************************************************************
 * Task Interrupt handlers
 ****************************************************************************/

/**
 * interrupt handler for #FGLOOP_PIT PIT channel
 *
 * Schedules fg_task() and clears the channel interrupt flag
 */
INTERRUPT void isr_pit_fgloop(void)
{	
    if ((fg_pit_ctr % AI_PIT_INTERRUPT_COUNT) == 1u)
    {
        /* Start next ADC sequence NB the multiplexer channel will have
         * been setup at the end of the previous interrupt */
        ai_initiate_read(Ai_atd0);
    }
    else if ((fg_pit_ctr % AI_PIT_INTERRUPT_COUNT) == 2u)
    {
        (void) scheduler_task_set_event_no_reschedule(FG_TASK_PRIORITY);
    }
    else
    {
        /* MISRA */
    }

    fg_pit_ctr++;	

   
    //PORTK|=0x01;
    //    LIN_Trans_Management();
    //    PORTK &= ~0x01;
    

	//if ((fg_pit_ctr % 100u) == 58u)
	//{
	//	aps_run_bg_task();
	//}
   

    PITCLRINTFLG(FGLOOP_PIT);
    SCHEDULER_DO_CONTEXT_SWITCH_ISR();
	
	
}

void ad_pit_init(void)
{
    /* set TimerServices PIT IRQ default priority */
//    PitEnableInterrupts(AD_PIT, INT_LVL_DEFAULT, INT_CPU_DEST);
//
//    /* and kick the timer off by default */
//     PitStart(AD_PIT, AD_PIT_TICKS);
}

INTERRUPT void ad_pit_interrupt(void)
{


//    PORTK|=0x01;
//        LIN_Trans_Management();
//        PORTK &= ~0x01;
//    /* Clear the timeout flag for the PIT */
//    PITCLRINTFLG(AD_PIT);
}
