/**
 *
 * Main state machine interface types
 *
 * @file
 * @ingroup ms
 * $Id: ms_types_cust.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2015 - All rights reserved */

#if !defined(MS_TYPES_CUST_H)
#define      MS_TYPES_CUST_H

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

#endif      /* MS_TYPES_CUST_H */
