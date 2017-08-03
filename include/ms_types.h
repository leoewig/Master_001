/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   obd_ids_types.h
 *
 * description: Main state module header file 
 *    
 *      
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(MS_TYPES_H)
#define      MS_TYPES_H
/**
 *
 * Main state module header file
 *
 * @file
 * @ingroup ms
 * $Id: ms_types.h 12113 2014-06-02 11:01:20Z pr4 $
 */

#define MS_INHIBIT(_feature, _shutdown_level, _initial_state) ms_inhibit_ ##_feature## ,

enum ms_inhibits_e
{
#include "ms.def"
    ms_last_inhibit
};

#undef MS_INHIBIT

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

enum ms_comms_state_e
{
    Ms_comms_inactive,
    Ms_comms_some_active,
    Ms_comms_all_active
};

/**
 * Features that affect the MMC shutdown can be either of the following
 * - Primary feature, all these must agree that they do wish the MMC to stay alive
 *   before a shutdown procedure can begin
 * - Secondary feature, once the MMC is shutting down, all these must agree that they
 *   have completed any necessary actions for the shutdown to be completed.
 */
enum ms_inhibit_type_e
{
    Ms_primary_feature_shutdown,
    Ms_secondary_feature_shutdown
};


/**
 * Response to an ECUReset request */
enum ResetReqResult_e
{
    ResetReqRejected,
    ResetReqAccepted
};

/**
 * Features that affect the MMC shutdown can be in any of the following states
 * - Allowed, all necessary conditions have been met for this feature to
 *   allow shutdown
 * - Prevented, all necessary conditions have NOT been met for this feature to
 *   allow shutdown
 * - In progress, the feature has been commanded to start to perform it's
 *   shutdown actions
 * - Indeterminate, the feature has not yet made any decision concerning the
 *   shutdown status
 */
enum ms_shutdown_state_e
{
    Ms_shutdown_prevented,
    Ms_shutdown_allowed,
    Ms_shutdown_commanded,
    Ms_shutdown_indeterminate
};

enum Ms_wakeup_mode
{
    /* Wakeup mode not yet determined */
    Ms_wakeup_unknown,
    /* Normal operation wakeup identified, either ignition or pilot */
    Ms_wakeup_ign,
    Ms_wakeup_chg_pilot,
	Ms_wakeup_cc,
    /* Woken up from internal source for the explicit purpose of logging */
    Ms_wakeup_log_timer,
    /* Woken up from internal source for charging top-up */
    Ms_wakeup_chg_timer,
    /* Woken up by RTC but no record of timer source */
    Ms_wakeup_unk_timer,
    /* Woken up by RTC but failed to get information before timeout */
    Ms_wakeup_timeout_timer,
    /* Could not determine cause of wakeup */
    Ms_wakeup_none
};

#endif      /* MS_TYPES_H */
