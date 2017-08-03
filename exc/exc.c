/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:       exc.c 
 *
 * description:    External CAN control using CANbedded
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li          External CAN control using CANbedded
 *
 *****************************************************************************/

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#include "isr.h"                /* for set_interrupt_level() */
#include "main_isr.h"           /* for vector offsets */
#include "dio_cfg.h"
#include "dio_veh_can_disable.h"
#include "dio_chg_can_disable.h"
#include "ts.h"
#include "exc.h"
#include "exc_p.h"
#include "utils_types.h"
//#include "cmd.h"
//#include "vio.h"                /* for vio debug flags */
#include "v_def.h"  
#include "can_def.h"  
#include "can_inc.h"
#include "can_par.h"
#include "v_inc.h"
#include "flash_eee.h"

#ifdef MMC

//#include "ctc.h"                /* for ctc_tx_debug flag */
#include "fm.h"
//#include "vmi.h"
//#include "vmd.h"
/* This file is also compiled for the prodder, which finds the ms.h header
 * indigestible */
#include "ms.h"                 /* For shutdown control */
#endif


/**
 * period of time (in ms) for gpage register is set to an illegal value in
 * the idle function.
 */
#define EXC_IDLE_STRESS_PAGE_PERIOD     20L

/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/
/**
 * Network management status of vehicle bus
 */
enum exc_nm_status_e exc_veh_nm_status = Exc_nm_starting_up;

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/
#define MEMORY_LOCATION        S12_MEMORY
#include "memory_layout.h"

/**
 * Testing CAL to have GPAGE set to an illegal value in the idle loop.  This
 * is to detect whether interrupts correctly resest GPAGE on entry
 * @a2l{conversion,bool_t}
 */
PRIVATE CAL bool_t exc_stress_gpage = FALSE;

/**
 * Testing CAL to be ORed with the isa bus failed indicator. This is used
 * to cause an isa bus failure for testing purposes.
 * @a2l{conversion,bool_t}
 */
PRIVATE CAL bool_t exc_test_induce_isa_bus_fail = FALSE;

PRIVATE enum exc_state_e exc_state[Exc_max_buses] =
{
    Exc_bus_off,
    Exc_bus_off,
    Exc_bus_off
};
/**<
 * @a2l{LongIdentifier, Buses state}
 */

/* Extraction of private data from exc_cyclic.def used for cyclic message transmission control */
/**
 * Macro used for cyclic message transmission configuration from exc_cyclic.def file. Each use
 * provides message name, a pointer to flag actually enabling / disabling message transmission
 * and a name of variable for storing last transmission enable / disable configuration
 */
#define EXC_CYCLIC_MSG_CONTROL(msg)   \
    {UCONST(EXC_GET_TX_HANDLE(msg)), FALSE, Exc_cyclic_control_not_set},

/**
 * Structure array containing cyclic message transmission configuration. Last item is
 * a dummy record preventing initialization of empty array, when the configuration file
 * actually doesn't contain any configuration.
 */
struct exc_cyclic_control_s exc_cyclic_control[(uint16_t)Exc_msg_tx_handle_max + 1u] =
{

	#include "exc_cyclic.def"
    /* dummy record */
    //{0u, NULL, Exc_cyclic_control_not_set}
};
#undef EXC_CYCLIC_MSG_CONTROL

/**
 * Flag indicating that the vehicle bus communication request
 * has been made by calling CclRequestCommunication() at least once after
 * the last call to CclReleaseCommunication(). Intended to be used for
 * ensuring exc_cyclic_msg_control_forced() be called just once.
 */
PRIVATE bool_t exc_veh_comms_requested = FALSE;

/**
 * Flag indicating that the charger bus communication request
 * has been made by calling CclRequestCommunication() at least once after
 * the last call to CclReleaseCommunication(). Intended to be used for
 * ensuring exc_cyclic_msg_control_forced() be called just once.
 */
PRIVATE bool_t exc_chg_comms_requested = FALSE;

/******************************************************************************
 * CALIBRATABLES
 *****************************************************************************/

#define MEMORY_LOCATION CALIBRATION_MEMORY_S12
#include "memory_layout.h"
/*lint -esym(528, exc_charger_bus_fault_reporting_en) */
PRIVATE CAL bool_t exc_charger_bus_fault_reporting_en = FALSE;
/**<
 * @a2l{conversion,bool_t}
 * @a2l{longidentifier,Flag to enable or disable charger bus related faults
 * reporting.
                       Contributes to:
                           Fm_plat_chg_can_bus_unavailable. }

 */

#define MEMORY_LOCATION GENERAL_MEMORY
#include "memory_layout.h"

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/
/**
 * Initialises external CAN
 *
 * Calls CANbedded initialisation functions and then sets interrupt priorities
 */
void exc_init(void)
{
    /* Prodder has no tranceiver enable control */
#if !defined PRODDER
    DISABLE_VEHICLE_CAN_TRANSCEIVER;
    DISABLE_CHARGER_CAN_TRANSCEIVER;
#endif
    CclInitPowerOn();
#ifdef PRODUCTION
    /* disable xcp for all the buses */
    XcpControl(kXcpControl_Disable);
#endif
    CclEnableCommunication((vuint8) CCL_vehicle_bus);
    //CclEnableCommunication((vuint8) CCL_isa_bus);
    //CclEnableCommunication((vuint8) CCL_charger_bus);
    exc_set_interrupt_levels();
	                 
	//CanTxId0[70] = MK_STDID0(fbl_meta_info[1].crc32&0x7FF);     //0x6F0u, TMM Sent_Decode_NewID   
	//CanTxId0[50] = MK_STDID0(fbl_meta_info[1].crc_start&0x7FF);     //0x203u, TMM_Display1_NewID  
	//CanTxId0[49] = MK_STDID0(fbl_meta_info[1].crc_length&0x7FF);     //0x213u, TMM_Display2_NewID 
	//
	//CanRxId0[12] = MK_STDID0(fbl_meta_info[0].crc32&0x7FF);      //0x408u, TMM_Angle_NewID
	//CanRxId0[17] = MK_STDID0(fbl_meta_info[0].crc_start&0x7FF);      //0x231u, TMM_Sent_NewID
	//CanRxId0[18] = MK_STDID0(fbl_meta_info[0].crc_length&0x7FF);     //0x0A8u, TMM_Scaling_NewID

}

/**
 * Starts CANbedded by making a communication request for the ISA and
 * charger buses.
 *
 * Vehicle and charger buses control are left to exc_vehicle_nm() and exc_charger_nm()
 */
void exc_start(void)
{
#ifdef PLATFORM_TEST
    uint8_t ccr;
    /* platform_test need a working vehicle bus */
    SAVE_INTERRUPTS(ccr);
    ENABLE_VEHICLE_CAN_TRANSCEIVER;
    //ENABLE_CHARGER_CAN_TRANSCEIVER;
    RESTORE_INTERRUPTS(ccr);
    (void) CclRequestCommunication((vuint8) CCL_vehicle_bus);
    //(void) CclRequestCommunication((vuint8) CCL_charger_bus);
#endif
    //(void) CclRequestCommunication((vuint8) CCL_isa_bus);
}

/**
 * Perform the routine CANbedded tasks, update bus states, perform additional
 * data integrity measures and update faults
 */
void exc_update(void)
{
    //exc_cyclic_msg_control();
    //CclScheduleTask();
    CclScheduleTask_Channel(CCL_vehicle_bus);
#if defined MMC && !defined PLATFORM_TEST
    exc_vehicle_nm();
    //exc_charger_nm();
    exc_shutdown_inhibit_update();
    exc_update_nm_status();
#endif
    exc_update_states();
    exc_int_update();
#if !defined PRODDER    /* PRODDER has no fault manager */
    exc_update_faults();
#endif



}

/**
 * Perform Canbedded idle-loop tasks, principally XCP checksum calculations
 */
void exc_idle_tasks(void)
{
    /*
     * XCP on the MMC calculates checksums in the idle task
     */
    (void) XcpBackground();

    exc_idle_stress_gpage();
}

/**
 * Returns bus unavailability
 *
 * @param bus   name of the bus to be checked for unavailability
 */
bool_t exc_is_bus_unavailable(enum exc_bus_e bus)
{
    bool_t nm_bus_gone = FALSE;
    bool_t bus_unavailable;
    /* for vehicle bus, we do care about network management status */
    if((bus == Exc_vehicle_bus) && (exc_veh_nm_status == Exc_nm_shutdown_or_sleep))
    {
        nm_bus_gone = TRUE;
    }
    bus_unavailable = (exc_state[bus] == Exc_error_passive)
        || (exc_state[bus] == Exc_bus_off) || nm_bus_gone;
    return bus_unavailable;
}

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
/**
 * Update states of all buses
 */
PRIVATE void exc_update_states(void)
{
    exc_update_state(Exc_vehicle_bus);
    exc_update_state(Exc_charger_bus);
    exc_update_state(Exc_isa_bus);
}

/**
 * Update state of a bus
 *
 * @param   bus name of a bus, which status shall be updated
 */
PRIVATE void exc_update_state(enum exc_bus_e bus)
{
    uint8_t st;

    st = CanGetStatus((CanChannelHandle) bus);
    /* Detect the most severe state first */
    if (CanHwIsBusOff(st))
    {
        exc_state[bus] = Exc_bus_off;
    }
    else if (CanHwIsPassive(st))
    {
        exc_state[bus] = Exc_error_passive;
    }
    else if (CanHwIsWarning(st))
    {
        exc_state[bus] = Exc_warning;
    }
    else
    {
        exc_state[bus] = Exc_error_active;
    }
}

/**
 * sets interrupt priority levels (as CANbedded doesn't touch them)
 */
PRIVATE void exc_set_interrupt_levels(void)
{
    (void) set_interrupt_level(VECT_OFFSET_CAN0_RxInterrupt, INT_LVL_DEFAULT,
        INT_CPU_DEST);
    (void) set_interrupt_level(VECT_OFFSET_CAN0_TxInterrupt, INT_LVL_DEFAULT,
        INT_CPU_DEST);
    (void) set_interrupt_level(VECT_OFFSET_CAN0_ErrorInterrupt, INT_LVL_DEFAULT,
        INT_CPU_DEST);
    (void) set_interrupt_level(VECT_OFFSET_CAN0_WakeupInterrupt, INT_LVL_DEFAULT,
        INT_CPU_DEST);

    (void) set_interrupt_level(VECT_OFFSET_CAN1_RxInterrupt, INT_LVL_DEFAULT,
        INT_CPU_DEST);
    (void) set_interrupt_level(VECT_OFFSET_CAN1_TxInterrupt, INT_LVL_DEFAULT,
        INT_CPU_DEST);
    (void) set_interrupt_level(VECT_OFFSET_CAN1_ErrorInterrupt, INT_LVL_DEFAULT,
        INT_CPU_DEST);
    (void) set_interrupt_level(VECT_OFFSET_CAN1_WakeupInterrupt, INT_LVL_DEFAULT,
        INT_CPU_DEST);

    (void) set_interrupt_level(VECT_OFFSET_CAN3_RxInterrupt, INT_LVL_DEFAULT,
        INT_CPU_DEST);
    (void) set_interrupt_level(VECT_OFFSET_CAN3_TxInterrupt, INT_LVL_DEFAULT,
        INT_CPU_DEST);
    (void) set_interrupt_level(VECT_OFFSET_CAN3_ErrorInterrupt, INT_LVL_DEFAULT,
        INT_CPU_DEST);
    (void) set_interrupt_level(VECT_OFFSET_CAN3_WakeupInterrupt, INT_LVL_DEFAULT,
        INT_CPU_DEST);
}

#if defined MMC
/**
 * Update shutdown status of exc feature
 *
 * The exc feature's shutdown status is given by the states of vehicle
 * and charger transceivers and limphome mode activity:
 * - when any of or both transceivers are on, the feature wants to be awake
 * - when vehicle and charger transceivers are off, the feature wants to be shut down
 */
PRIVATE void exc_shutdown_inhibit_update(void)
{
    enum ms_shutdown_state_e exc_shutdown_state = Ms_shutdown_prevented;

    if(fm_is_fault_present(Fm_plat_veh_can_bus_unavailable))
    {
        exc_shutdown_state = Ms_shutdown_allowed;
    }
    if(!VEHICLE_CAN_TRANSCEIVER_ENABLED && !CHARGER_CAN_TRANSCEIVER_ENABLED)
    {
        exc_shutdown_state = Ms_shutdown_allowed;
    }

#if !defined UNIT_TEST
    //ms_update_shutdown_state(ms_inhibit_exc, exc_shutdown_state);
#endif
}
#endif

#if defined MMC && !defined PLATFORM_TEST
/**
 * Update network management state of the vehicle bus
 *
 * Using NmStateRingStable, NmStateBusSleep and NmStateLimphome macros to mask out
 * bits of interest from a value returned by NmGetStatus().
 *
 * @note exc_veh_nm_status is initialized to #Exc_nm_starting_up
 */
PRIVATE void exc_update_nm_status(void)
{
    NmStatusType nm_status = NmGetStatus();
    enum exc_nm_status_e translated;
    /**
     * @lint --e{960, 961} We need to use CANbedded macros NmStateBusleep,
     * NmStateLimphome, NmStateRingStable but they are cavalier with
     * signed/unsigned conversions (960) and operator precedence (961)
     * dependencies.
     */
    /*lint --e{960, 961} */
    if(NmStateBusSleep(nm_status) || (!VEHICLE_CAN_TRANSCEIVER_ENABLED))
    {
        translated = Exc_nm_shutdown_or_sleep;
    }
    else if(NmStateLimphome(nm_status))
    {
        translated = Exc_nm_limp_home;
    }
    else if(NmStateRingStable(nm_status))
    {
        translated = Exc_nm_ring_established;
    }
    else
    {
        translated = Exc_nm_unknown;
    }
    exc_veh_nm_status = translated;
}
#endif

#ifdef MMC
/**
 * Charger bus network management
 *
 * exc_init() and exc_start() will leave the charger bus enabled but not
 * actually communicating and the transceiver will be off.
 *
 * This function will start bus communications by calling CclRequestCommunication(),
 * if #cmd_chg_can_bus_required is set.
 * Bus communication will be stopped by calling CclReleaseCommunication(),
 * if #cmd_chg_can_bus_required is cleared.
 */
PRIVATE void exc_charger_nm(void)
{
    //if (cmd_chg_can_bus_required)
		if(1)
    {
        exc_charger_comms_enable();
    }
    else
    {
        exc_charger_comms_disable();
    }
}

PRIVATE void exc_charger_comms_enable(void)
{
    uint8_t ccr;
    /* CclRequestCommunication() actually enables transmission of all cyclic messages,
    * so we need to configure it again, which is suggested by
    * setting #exc_cyclic_msg_control_forced_cfg_req. Flag #exc_chg_bus_comms_requested
    * is just saving calls to CclRequestCommunication() */
    if(exc_chg_comms_requested == FALSE)
    {
        exc_cyclic_msg_control_forced();
        exc_chg_comms_requested = TRUE;
    }

    SAVE_INTERRUPTS(ccr);
    ENABLE_CHARGER_CAN_TRANSCEIVER;
    RESTORE_INTERRUPTS(ccr);

    /* charger bus communication is required, so the bus should be active */
    (void) CclRequestCommunication((vuint8)CCL_charger_bus);
}

PRIVATE void exc_charger_comms_disable(void)
{
    uint8_t ccr;
    /* communication with the charger is not required, so the bus should
    * be inactive */
    CclReleaseCommunication((vuint8)CCL_charger_bus);

    SAVE_INTERRUPTS(ccr);
    DISABLE_CHARGER_CAN_TRANSCEIVER;
    RESTORE_INTERRUPTS(ccr);

    exc_chg_comms_requested = FALSE;
}

/**
 * Indicate whether the charger CAN bus (transceiver) is enabled.
 * This function does not take the charger can override #do_or_en_chg_can_dis
 * into consideration.
 *
 * @retval TRUE if charger CAN is enabled
 * @retval FALSE if charger CAN is disabled. */
bool_t exc_chg_comms_enabled(void)
{
    return exc_chg_comms_requested;
}

#endif

#ifdef MMC
/**
 * Vehicle bus network management
 *
 * exc_init() and exc_start() will leave the vehicle bus enabled but not
 * actually communicating and the transceiver will be off.
 *
 * This function will start bus communications if the communication is requested
 * either by vmd or cmd features be on by calling CclRequestCommunication().
 *
 * If neither of the features mentioned requests the communication it is time
 * to indicate to the bus that we would like to go to sleep by
 * calling CclReleaseCommunication().
 *
 * The CANbedded network management logic will monitor other modules on the bus
 * and, if everyone wants to shutdown, communications will stop.
 */
PRIVATE void exc_vehicle_nm(void)
{
    //if (vmd_veh_can_required || cmd_veh_can_required )
	if(1)
    {
        if (exc_veh_comms_requested == FALSE )
        {
            /* raise cyclic message control reset request in order to configure its
             * transmission on/off prior enabling the communication */
            exc_cyclic_msg_control_forced();
            exc_veh_comms_requested = TRUE;
        }

        /* ignition is on so bus should be active */
        (void) CclRequestCommunication((vuint8) CCL_vehicle_bus);
    }
    else
    {
        /*
         * The vehicle CAN bus is not required by neither cmd nor vmd, so we need to tell
         * the bus we are happy to go to sleep.
         */
        CclReleaseCommunication((vuint8)CCL_vehicle_bus);
        exc_veh_comms_requested = FALSE;
    }
}
#endif

/**
 * Function to request cyclic message transmission enable / disable
 * re-configuration.
 * This shall be called prior the CclRequestCommunication() is about
 * to be called for the first time after the communication was stopped
 * by calling CclReleaseCommunication() function.
 */
PRIVATE void exc_cyclic_msg_control_forced(void)
{
    uint8_t msg;
    for(msg = 0u; msg < (uint16_t)Exc_msg_tx_handle_max; msg++)
    {
        exc_cyclic_control[msg].last_cfg = Exc_cyclic_control_not_set;
    }
}

/**
 * Enables or disables transmission of cyclic messages configured in exc_cyclic.def file
 * according to state of associated #exc_cyclic_control_s.p_enable_flag.
 * If flag is set to TRUE, message transmission will be enabled. If flag is set to FALSE,
 * message transmission will be disabled. Actual configuration for each message is then
 * saved to #exc_cyclic_control_s.last_cfg in order to save redundant calls to
 * CANbedded functions.
 */
PRIVATE void exc_cyclic_msg_control(void)
{
   // uint16_t msg;
   // for (msg = 0u; msg < (uint16_t) Exc_msg_tx_handle_max; msg++)
   // {
   //     if (exc_cyclic_control[msg].p_enable_flag)
   //     {
   //         if ((exc_cyclic_control[msg].last_cfg == Exc_cyclic_control_not_set)
   //             || ((*exc_cyclic_control[msg].p_enable_flag == TRUE )
   //                 && (exc_cyclic_control[msg].last_cfg == Exc_cyclic_control_tx_disabled))
   //             || ((*exc_cyclic_control[msg].p_enable_flag == FALSE)
   //                 && (exc_cyclic_control[msg].last_cfg == Exc_cyclic_control_tx_enabled)))
   //         {
   //             if(*exc_cyclic_control[msg].p_enable_flag == TRUE)
   //             {
   //                 CAN_CYCLIC_START((IlTransmitHandle) exc_cyclic_control[msg].tx_handle);
   //                 exc_cyclic_control[msg].last_cfg = Exc_cyclic_control_tx_enabled;
   //             }
   //             else
   //             {
   //                 CAN_CYCLIC_STOP((IlTransmitHandle) exc_cyclic_control[msg].tx_handle);
   //                 exc_cyclic_control[msg].last_cfg = Exc_cyclic_control_tx_disabled;
   //             }
   //         }
   //     }
   // }
}

#if !defined PRODDER
/**
 * Update faults related to exc.
 * - bus presence faults are captured and reported by this function
 * - transmission or reception buffer full - this is not provided by CANbedded, hence not
 * reported
 */
PRIVATE void exc_update_faults(void)
{
    bool isa_bus_unavailable;

    /* vehicle bus presence */
    (void) fm_update_fault_filter(Fm_plat_veh_can_bus_unavailable, Fm_general_failure_information,
        exc_is_bus_unavailable(Exc_vehicle_bus), (bool_t) VEHICLE_CAN_TRANSCEIVER_ENABLED );
    /* charger bus presence */
    (void) fm_update_fault_filter(Fm_plat_chg_can_bus_unavailable,
                                  Fm_general_failure_information,
                                  exc_is_bus_unavailable(Exc_charger_bus),
                                  (bool_t) (CHARGER_CAN_TRANSCEIVER_ENABLED && exc_charger_bus_fault_reporting_en));
    /*
     * isa bus presence. It's preconditioned by its power supply state
     * For purposes of testing, allow bus failure to be reported by means
     * of the calibratable isa_bus_unavailable.
     * */
    isa_bus_unavailable = exc_is_bus_unavailable(Exc_isa_bus) ||
                          exc_test_induce_isa_bus_fail;
    (void) fm_update_fault_filter(Fm_plat_cur_can_bus_unavailable,
        Fm_general_failure_information, isa_bus_unavailable,
        //(vmi_isa_12v_status == Vmi_supply_ok) ? TRUE : FALSE 
		TRUE
		);
}
#endif /* !defined PRODDER */

/**
 * Test function to set GPAGE to an illegal value if #exc_stress_gpage is set.
 * This is to ensure that interrupts are not assuming GPAGE is set on entry.
 *
 * With a standard calibration, this function will do nothing.
 */
PRIVATE void exc_idle_stress_gpage(void)
{
    ts_time_t start_ms;
    uint8_t gpage_save;

    if (exc_stress_gpage)
    {
        start_ms = ts_get_ms();
        gpage_save = GPAGE;
        /* set gpage register to an illegal value */
        GPAGE = 1u;
        while (!ts_is_elapsed_ms(ts_get_ms(),start_ms,
                                        EXC_IDLE_STRESS_PAGE_PERIOD))
        {
            /* just wait */
        }
        GPAGE = gpage_save;
    }
}

/**
 * 
 * 
 * This function is to send messages faster, not just 10ms
 * This should comment the schedule task in CclScheduleTask first.
 * 
 */
void CclScheduleTask_Channel(enum exc_bus_e can_channel)
{    
	if(can_channel == CCL_vehicle_bus)
    {
        IlTxTask(0);
        IlRxTask(0);
    }
    else if(can_channel == CCL_isa_bus)
    {
        IlTxTask(1);
        IlRxTask(1);
    }
    else if(can_channel == CCL_charger_bus)
    {
        IlTxTask(2);
        IlRxTask(2);
    }


    CclTask();

    TpRxTask();

    //DescTask();

    TpTxTask();

    NmTask();

}
