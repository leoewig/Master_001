/**
 *
 * MS Main state machine implentation source file
 *
 * @file
 * @ingroup ms
 * $Id: ms.c 12113 2014-06-02 11:01:20Z pr4 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#include "mmc.h"
#include "ms.h"
#include "ms_p.h"
#include "hardware.h"
//#include "ctc_p.h"
#include "fm.h"        /* For access to failure function status   */
//#include "cgi.h"       /* For access to charger present           */
//#include "cgs.h"       /* For access to charging state            */
//#include "icm.h"       /* For access to VTBM comms status         */
//#include "imi.h"       /* For access to initialisation of ISA/IMI */
#include "mmc_aps.h"   /* For access to foreground shutdown flag  */
#include "nvs.h"       /* For access to NVS update status         */
#include "pit.h"       /* For access to timer control routines    */
#include "pit_cfg.h"   /* For access to timer macros              */
#include "rtc.h"       /* For access to RTC state                 */
#include "scheduler.h" /* For access to scheduler stop            */
//#include "vmd.h"       /* For access to vehicle mode demand       */
//#include "cmd.h"       /* For access to charger mode demand       */
#include "wus.h"
#include "dio_cfg.h"
#include "ixcp.h"

//#include "umcl.h"       /* for umcl_is_disabled */
/******************************************************************************
 * COMPILE-TIME CHECKS
 *****************************************************************************/

/*
 * PUBLIC DATA (documented in header)
 */
enum ms_state_e       ms_sys_state        = Ms_state_init;
/**<
 * @a2l{conversion,ms_state_e}
 * @a2l{longidentifier,Main system state currently in force}
 */


bool_t   ms_balancing_enabled   = FALSE;
/**<
 * @a2l{conversion,bool_t}
 * @a2l{longidentifier,Conditions to be passed to the balancing based on the internal state}
 */

bool_t        ms_ecu_reset_pending = FALSE;
/**<
 * A reset is pending, typically other features should inspect
 * @ref ms_shutdown_pending instead.
 * @a2l{conversion,bool_t}
 * @a2l{longidentifier,Indicates that the MS intends to reset the MMC}
 */

bool_t                ms_shutdown_pending = FALSE;
/**<
 * @a2l{conversion,bool_t}
 * @a2l{longidentifier,Indicates that the MS feature would like to enter the shutdown state}
 */

bool_t                ms_fgloop_shutdown_complete;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Flag indicating whether the foreground tasks have shutdown or not}
 */

ts_time_t             ms_left_initialisation = TIME( 0 );

/** The determination of the wake-up mode */
enum Ms_wakeup_mode ms_wakeup_type;

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/



/** Reset type passed in with the request */
PRIVATE flash_eee_reset_type_e ms_reset_type;

/**
 * @lint
 * .DEF files are not MISRA's favourite idea but we accept they are the best
 * approach to this sort of situation so do the following to keep lint quiet:\n
 * -esym(960,19.12)  Multiple use of '\#/##' operators in macro definition\n
 */
/*lint -esym(960,19.12) */
#define MS_INHIBIT(_feature, _shutdown_level, _initial_state) _initial_state,

PRIVATE enum ms_shutdown_state_e ms_feature_shutdown_state [ms_last_inhibit] =
{
#include "ms.def"
};
/**<
 * @a2l{conversion,ms_shutdown_state_e}
 * @a2l{longidentifier,Flags indicating which of the features have completed their shutdown actions}
 */

#undef MS_INHIBIT
#define MS_INHIBIT(_feature, _shutdown_level, _initial_state) Ms_ ##_shutdown_level## _feature_shutdown,

PRIVATE CONST enum ms_inhibit_type_e ms_feature_shutdown_types [ms_last_inhibit] =
{
#include "ms.def"
};
/**<
 * @a2l{conversion,ms_inhibit_type_e}
 * @a2l{longidentifier,Flags indicating the required type of shutdown for each feature}
 */

#undef MS_INHIBIT


/*lint +esym(960,19.12) */


/** The number of attempts make to determine the cause of wake-up */
PRIVATE uint8_t ms_wakeup_src_tries=0u;

/** Millisecond timestamp when ms_init() was called */
PRIVATE ts_time_t ms_init_time=0;


/******************************************************************************
 * CALIBRATION
 *****************************************************************************/

#define MEMORY_LOCATION CALIBRATION_MEMORY_S12
#include "memory_layout.h"

/**
 * The maximum number of attempts to make to determine the source of wake-up
 */
PRIVATE CAL uint8_t ms_max_wakeup_src_tries=100u;

/** The minimum amount of time for the BMS to be awake if woken by the RTC.
 * This allows some time to establish what the BMS should do e.g. find the 
 * charger, decide it needs to log. */
PRIVATE CAL ts_time_t   ms_min_rtc_wakeup=5000;

#undef  MEMORY_LOCATION
#define MEMORY_LOCATION S12_MEMORY
#include "memory_layout.h"

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/* documentation with prototype in header file */
void ms_init(void)
{
    /* To prevent immediate transition to shutdown mode */
 //   ms_feature_shutdown_state [ms_inhibit_ms] = Ms_shutdown_prevented;
 //
 //   ms_sys_state =  Ms_state_init;
 //   ms_left_initialisation = TIME( 0 );
 //   ms_init_time = ts_get_ms();
 //   ms_wakeup_src_tries=0u;
 //   ms_wakeup_type=Ms_wakeup_unknown;
}

/* documentation with prototype in header file */
void ms_update( void )
{
 //   RTC_DataValidType    rtc_state;
 //
 //   if (vmd_error_mode || cmd_error_mode)
 //   {
 //       if ( (ms_sys_state != Ms_state_shutdown)
 //          )
 //       {
 //           /* Can be entered from any state (except shutdown & low power)
 //              and overrides all other transitions */
 //           ms_sys_state = Ms_state_error;
 //       }
 //   }
 //
 //   ms_wakeup_update();
 //
 //   switch (ms_sys_state)
 //   {
 //       case Ms_state_init:
 //           /** @todo Remove this transition when FM groups are available. This is only
 //            *  here to provide a belt & braces method of ensuring that we do not get
 //            *  trapped in the initialisation state during development */
 //           if (ms_check_for_shutdown_initiation())
 //           {
 //               if (ms_have_all_features_shutdown())
 //               {
 //                   ms_sys_state = Ms_state_shutdown;
 //               }
 //           }
 //           else
 //           {
 //               rtc_state = rtc_get_status();
 //
 //               if ( 
//				(Imi_state_running == imi_init_status)
 //               &&                                    
 //              ((Rtc_data_valid   == rtc_state) || (Rtc_data_invalid == rtc_state)))
 //               {
 //                   ms_sys_state = Ms_state_standby;
 //               }
 //           }
 //
 //           if( ms_sys_state != Ms_state_init ) {
 //               /* Capture timestamp if leaving initialisation mode */
 //               ms_left_initialisation = ts_get_ms();
 //           }
 //           break;
 //
 //       case Ms_state_standby:
 //           if (ms_check_for_shutdown_initiation())
 //           {
 //               if (ms_have_all_features_shutdown())
 //               {
 //                   ms_sys_state = Ms_state_shutdown;
 //               }
 //           }
 //           else
 //           {
 //               if ((!vmd_veh_mode_req)
 //               &&   (cmd_chg_mode_req))
 //               {
 //                   ms_sys_state = Ms_state_charger;
//					//cmd_dclink_ctc_command = Ctc_cont_closed;
 //               }
 //               else
 //               {
 //                   if ( (vmd_veh_mode_req)
 //                   &&  (!cmd_chg_mode_req)
 //                   &&  (ctc_dclink_contactors_closed == Ctc_cont_open)
//					&& 	(ctc_charger_contactors_closed == Ctc_cont_open))
 //                   {
 //                       ms_sys_state = Ms_state_veh_closing;
 //                   }
 //               }
 //           }
 //           break;
 //
 //       case Ms_state_veh_closing:
 //           if (!vmd_veh_mode_req)
 //           {
 //               ms_sys_state = Ms_state_veh_opening;
 //           }
 //           else
 //           {
 //               if (ctc_dclink_contactors_closed == Ctc_cont_closed)
 //               {
 //                   ms_sys_state = Ms_state_veh_active;
 //               }
 //           }
 //           break;
 //
 //       case Ms_state_veh_active:
 //           if (!vmd_veh_mode_req)
 //           {
 //               ms_sys_state = Ms_state_veh_opening;
 //           }
 //           break;
 //
 //       case Ms_state_veh_opening:
 //           if (ctc_dclink_contactors_closed == Ctc_cont_open)
 //           {
 //               ms_sys_state = Ms_state_standby;
 //           }
 //           break;
 //
 //       case Ms_state_charger:
 //           if ((!cmd_chg_mode_req)
 //           &&  (Cgs_s_not_poss == cgs_state))
 //           {
 //               ms_sys_state = Ms_state_standby;
//				//cmd_dclink_ctc_command = Ctc_cont_open;
 //           }
 //           break;
 //
 //       case Ms_state_shutdown:
 //           break;
 //
 //       case Ms_state_error:
 //           if (ms_check_for_shutdown_initiation())
 //           {
 //               if (ms_have_all_features_shutdown())
 //               {
 //                   ms_sys_state = Ms_state_shutdown;
 //               }
 //           } else if(( !vmd_error_mode ) && ( !cmd_error_mode )) {
 //               ms_sys_state = Ms_state_standby;
 //           } else {
 //               /* MISRA */
 //           }
 //          break;
 //
 //       case Ms_state_low_power:
 //       case Ms_max_states:
 //       default:
 //           INTERNAL_ERROR();
 //           break;
 //   }
 //
 //   ms_set_outputs();
 //
}

/**
 * Update information related to system wakeup source arbitrtation
 * This sets #ms_wakeup_type and ensures the BMS remains awake long enough
 * to determine what it needs to do based on wakeup source
 */
PRIVATE void   ms_wakeup_update(void)
{
 //   enum ms_shutdown_state_e shutdown_permitted=Ms_shutdown_allowed;
 //   if (Ms_wakeup_unknown == ms_wakeup_type)
 //   {
 //       /* Do not allow shutdown the MMC is still attempting to determine the 
 //       * wakeup reason */
 //       shutdown_permitted = Ms_shutdown_prevented;
 //       /* If we don't know why the MMC was woken up, keep looking until 
 //       * success or failure */
 //       ms_determine_wakeup();
 //   }
 //
 //   if (wus_original.rtc)
 //   {
 //       /** If BCM woke up on RTC and it is within the short window of 
 //       *startup, inhibit shutdown this allows the BCM a chance to establish
 //       *whether it needs to do anything */
 //       if (!IS_ELAPSED_MS(ts_get_ms(),ms_init_time,ms_min_rtc_wakeup))
 //       {
 //           shutdown_permitted = Ms_shutdown_prevented;
 //       }
 //   }
 //
 //   ms_update_shutdown_state(ms_inhibit_wake,shutdown_permitted);
}

/**
 * Make a determination of operation type depending on cause of wakeup.
 * This arbitrates the inputs into a single wakeup source to determine how
 * the BMS should behave at  wakeup. Be aware that this status will hide the 
 * (slim) possibility of two simultaneous wakeup sources.
 */
PRIVATE void ms_determine_wakeup(void)
{
//    if (Ms_wakeup_unknown == ms_wakeup_type)
//    {
//        ++ms_wakeup_src_tries;
//
//        if (di_ign_dbn)
//        {
//            /* If we detect ignition input, don't wait for WUS communication. 
//             * The BMS should start up as quickly as possible.*/
//            ms_wakeup_type = Ms_wakeup_ign;
//        }
//        else if (di_chg_control_pilot)
//        {
//            ms_wakeup_type = Ms_wakeup_chg_pilot;
//        }
//		else if (di_cc_wkup_mmc)
//        {
//            ms_wakeup_type = Ms_wakeup_cc;
//        }
//        else if(wus_status_valid == wus_status)
//        {
//            ms_wakeup_type = ms_wakeup_from_wus();
//        }
//        else
//        {
//            /* No obvious wakeup source, wait for timeout below 
//            */
//        }
//    }
//
//    if ( (Ms_wakeup_unknown == ms_wakeup_type)
//        && (ms_wakeup_src_tries>=ms_max_wakeup_src_tries) )
//    {
//        if (wus_original.rtc)
//        {
//             ms_wakeup_type = Ms_wakeup_timeout_timer;
//        }
//        else
//        {
//            /* No wakeup source could be found after maximum tries */
//            ms_wakeup_type = Ms_wakeup_none;
//        }
//    }
}

/**
 * Determine wakeup purely from wus source
 *
 * Caller is responsible for ensuring wus if valid before calling this function.
 * Caller is responsible for timeout mechanism if an answer is not found
 * @retval Ms_wakeup_normal if ignition or charger pilot
 * @retval Ms_wakeup_log_timer on log timer
 * @retval Ms_wakeup_chg_timer on charger timer
 * @retval Ms_wakeup_unknown if not determined yet
 * @retval Ms_wakeup_none if none of the above, this is a failure to determine the source
 * Ms_wakeup_none is a failure to determine the source
 */
PRIVATE enum Ms_wakeup_mode ms_wakeup_from_wus(void)
{
//    enum Ms_wakeup_mode wakeup_type = Ms_wakeup_unknown;
//    if (wus_original.rtc)
//    {
//        /** @todo it might be possible to use the RTC status here but I can't 
//         * tell if it is a reliable indication or not without further 
//         * investigation */
//        if (rtc_wake_ups_sources_deduced)
//        {
//            if (rtc_has_been_woken_by( Rtc_alarm_req_cmd))
//            {
//                /* Woke up for charging top-up */
//                wakeup_type =Ms_wakeup_chg_timer;
//            }
//            else if (rtc_has_been_woken_by( Rtc_alarm_req_log))
//            {
//                wakeup_type = Ms_wakeup_log_timer;
//            }
//            else
//            {
//                wakeup_type = Ms_wakeup_unk_timer;
//            }
//        }
//    }
//    else if (wus_original.ignition || wus_original.system)
//    {
//        wakeup_type = Ms_wakeup_ign;
//    }
//    else if ( wus_original.chg_control_pilot)
//    {
//        wakeup_type = Ms_wakeup_chg_pilot;
//    }
//	else if ( wus_original.cc_wkup_mmc)
//    {
//        wakeup_type = Ms_wakeup_cc;
//    }
//    else
//    {
//        wakeup_type = Ms_wakeup_none;
//    }
//    return wakeup_type;
}

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/**
 * Update outputs from MS which depend on the state transition, just performed.
 *
 * <b> Initialisation State </b>
 *
 * - The outputs shall be reset to their default states as follows
 *  - vehicle contactor destination to open
 *  - balancing shall be not permitted
 *
 * <b> Standby State </b>
 *
 * - The outputs shall be set as follows
 *  - vehicle contactor destination to open
 *  - balancing shall be permitted
 *
 * <b> Vehicle Closing State </b>
 *
 * - The outputs shall be set as follows
 *  - vehicle contactor destination to closed
 *  - balancing shall be permitted
 *
 * <b> Vehicle Active State </b>
 *
 * - The outputs shall be set as follows
 *  - vehicle contactor destination to closed
 *  - balancing shall be permitted
 *
 * <b> Vehicle Opening State </b>
 *
 * - The outputs shall be set as follows
 *  - vehicle contactor destination to open
 *  - balancing shall be permitted
 *
 * <b> Charger State </b>
 *
 * - The outputs shall be set as follows
 *  - vehicle contactor destination to open
 *  - balancing shall be set according to the indication from the charging strategy
 *
 * <b> Shutdown State </b>
 *
 * - On initial entry to the shutdown state, the NVS feature will be
 *   prompted to save the current strategy data to EEPROM
 *
 * - The outputs shall be set as follows
 *  - vehicle contactor destination to open
 *  - balancing shall be not permitted
 *  - ms_fgloop_shutdown_complete shall be set to TRUE
 *
 * <b> Error State </b>
 *
 * - The outputs shall be set as follows
 *  - vehicle contactor destination to open
 *  - balancing shall be permitted
 *
 * <b> Low Power State </b>
 *
 * - The outputs shall be reset to their default states as follows
 *  - vehicle contactor destination to open
 *  - balancing shall be not permitted
 */

PRIVATE void ms_set_outputs(void)
{
//    switch (ms_sys_state)
//    {
//        case Ms_state_standby:
//        case Ms_state_veh_opening:
//        case Ms_state_error:
//        case Ms_state_charger:
//            /* states requiring contactors open and balancing enabled */
//            /** @mantis{4503} How does charging strategy indicate whether
//             * balncing should be inhibited? */
//            ms_veh_cont_dest    = Ctc_cont_open;
//            ms_balancing_enabled = TRUE;
//            break;
//        case Ms_state_shutdown:
//            /* special-case state requiring contactors open and balancing
//             * disabled */
//            ms_veh_cont_dest    = Ctc_cont_open;
//            ms_balancing_enabled = FALSE;
//
//            /* Command the Scheduler to cease foreground processing */
//            ms_fgloop_shutdown_complete = TRUE;
//
//#if !defined (PLATFORM_TEST)
//            /* Stop the Foreground Scheduler timer */
//            PitStop(FGLOOP_PIT);
//#endif
//            break;
//        case Ms_state_veh_closing:
//        case Ms_state_veh_active:
//            /* states requiring contactors closed and balancing enabled */
//            ms_veh_cont_dest    = Ctc_cont_closed;
//            ms_balancing_enabled = TRUE;
//            break;
//        case Ms_state_low_power:
//        case Ms_max_states:
//        case Ms_state_init:
//        default:
//            /* states requiring contactors open and balancing disabled */
//            ms_veh_cont_dest    = Ctc_cont_open;
//            ms_balancing_enabled = FALSE;
//            break;
//    }
}

/**
 * This function allows an external feature to indicate to the MS
 * feature whether or not it believes that shutdown could be entered.
 */
void ms_update_shutdown_state(enum ms_inhibits_e feature, enum ms_shutdown_state_e new_state)
{
//    ms_feature_shutdown_state[feature] = new_state;
}

/**
 * Query the shutdown request from a feature
 */
enum ms_shutdown_state_e ms_get_feature_shutdown_state(enum ms_inhibits_e feature)
{
 //   return ms_feature_shutdown_state[feature];
}

/**
 * Determine whether the shutdown procedure should be started
 *  */
PRIVATE bool_t ms_is_shutdown_start_allowed(void)
{
//    bool_t             shutdown_permitted = TRUE;
//    enum ms_inhibits_e i;
//
//    for (i = (enum ms_inhibits_e) 0; i < ms_last_inhibit; i++)
//    {
//        if (Ms_primary_feature_shutdown == ms_feature_shutdown_types[i])
//        {
//            if (Ms_shutdown_prevented == ms_feature_shutdown_state[i])
//            {
//                /* Indicate that shutdown cannot be initiated */
//                shutdown_permitted = FALSE;
//            }
//        }
//    }
//
//    return shutdown_permitted;
}

/**
 * This function determines if ALL PRIMARY features are in a state
 * where they could be shutdown. When all are ready to shutdown, then
 * shutdown is initiated and all features are marked as "shutting down".
 */
PRIVATE bool_t ms_check_for_shutdown_initiation()
{
//    bool_t             shutdown_permitted = TRUE;
//    enum ms_inhibits_e i;
//
//    shutdown_permitted = ms_is_shutdown_start_allowed();
//
//    /* Process transitions to/from shutdown pending "mode" */
//    if (shutdown_permitted || ms_ecu_reset_pending)
//    {
//        if (!ms_shutdown_pending)
//        {
//            /* All primary features have stated that it is OK to shutdown,
//             * so reset all feature inhibits, to ensure that they have at
//             * least one chance to perform their shutdown tasks */
//            for (i = (enum ms_inhibits_e) 0; i < ms_last_inhibit; i++)
//            {
//                ms_feature_shutdown_state[i] = Ms_shutdown_commanded;
//            }
//        }
//        /* MS has no shutdown actions, so clear our own inhibit flag */
//        ms_update_shutdown_state(ms_inhibit_ms, Ms_shutdown_allowed);
//
//        /* Set the overall shutdown pending flag */
//        ms_shutdown_pending = TRUE;
//
//        ms_reset_if_required();
//    }
//    else
//    {
//        if (ms_shutdown_pending)
//        {
//            /* At least one primary feature wants to come out of
//             * shutdown pending "mode" so reset all feature inhibits
//             */
//            for (i = (enum ms_inhibits_e) 0; i < ms_last_inhibit; i++)
//            {
//                ms_feature_shutdown_state[i] = Ms_shutdown_indeterminate;
//            }
//        }
//
//        /* MS has no shutdown actions, so reset our own inhibit flag */
//        ms_update_shutdown_state(ms_inhibit_ms, Ms_shutdown_prevented);
//
//        /* Clear the overall shutdown pending flag */
//        ms_shutdown_pending = FALSE;
//    }
//    
//    /* Indicate to MS the current shutdown status */
//    return shutdown_permitted;
}

/**
 * This function determines if ALL features have completed their
 * shutdown actions. If so, then the transition to Shutdown can be made
 */
PRIVATE bool_t ms_have_all_features_shutdown()
{
 //   bool_t             answer = TRUE;
 //   enum ms_inhibits_e i;
 //
 //   for (i = (enum ms_inhibits_e) 0; i < ms_last_inhibit; i++)
 //   {
 //       if (Ms_shutdown_allowed != ms_feature_shutdown_state[i])
 //       {
 //           answer = FALSE;
 //       }
 //   }
 //
 //   return answer;
}

/**
 * Request that the MMC performs a (diagnostic commanded) reset, whilst still
 * shutting down with some grace. If the reset was accepted, the MMC will 
 * subsequently reset, but it will take a few hundred milliseconds.
 *
 * This will consider a subset of features that assert their keepalives
 *
 * @retval TRUE if reset request was accepted.
 * @retval FALSE request rejected, MMC will not reset
 */
enum ResetReqResult_e ms_request_reset(flash_eee_reset_type_e reset_type)
{
    enum ResetReqResult_e result=ResetReqAccepted;
    /* These are all treated as special cases for now */
    //if (Ms_shutdown_prevented
    //        ==ms_get_feature_shutdown_state(ms_inhibit_vprg))
    //{
    //    result=ResetReqRejected;
    //}

    if (!ixcp_reset_conditions_ok())
    {
        result=ResetReqRejected;
    }

    if (ResetReqAccepted==result)
    {
        ms_ecu_reset_pending = TRUE;
        ms_reset_type=reset_type;
		
		/*TMM*/
		nvs_request_all_and_reset(ms_reset_type);
		nvs_background(1u);
    }

    return result;
}

/**
 * Monitor conditions/pending requests and kick off a reset action when ready
 */
PRIVATE void ms_reset_if_required(void)
{
//    if (ms_ecu_reset_pending)
//    {
//        if (!ms_reset_action_inhibited())
//        {
//            nvs_request_all_and_reset(ms_reset_type);
//        }
//    }
}


/**
 * Determine whether nvs can carry on with its reset.
 * This function should be called before calling nvs_request_all_and_reset()
 * unless it is known that the system is already in a state to permit it.
 *
 * @retval TRUE if NVS is prevented from performing its reset cleanup actions
 * @retval FALSE if NVS can carry on and reset */
PRIVATE bool_t ms_reset_action_inhibited(void)
{
    bool_t inhibited=FALSE;
    /** Wait for ICM only, so that VTBMs can be gracefully shut down */
    //if (Ms_shutdown_prevented==ms_get_feature_shutdown_state(ms_inhibit_icm))
    //{
    //    inhibited =TRUE;
    //}
    return inhibited;
}
