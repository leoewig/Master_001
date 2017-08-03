/**
 *
 * Main state machine interface
 *
 * @file
 * @ingroup ms
 * $Id: ms_cust.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2015 - All rights reserved */

#if !defined(MS_CUST_H)
#define      MS_CUST_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "mmc.h"  /* mmc.h is a prerequisite */
#include "ms_types_cust.h"

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern enum ms_state_e       ms_sys_state;
extern enum Ms_wakeup_mode   ms_wakeup_type;
extern bool_t                ms_balancing_enabled;
extern bool_t                ms_shutdown_pending;
extern bool_t                ms_ecu_reset_pending;
extern bool_t                ms_fgloop_shutdown_complete;
extern ts_time_t             ms_left_initialisation;
extern enum Ms_wakeup_mode   ms_wakeup_type;

/**
 * This function allows an external feature to indicate to the MS
 * feature whether or not it believes that shutdown could be entered.
 */
void                     ms_update_shutdown_state     (enum ms_inhibits_e feature, enum ms_shutdown_state_e new_state);

/**
 * Query the shutdown request from a feature
 */
enum ms_shutdown_state_e ms_get_feature_shutdown_state(enum ms_inhibits_e feature);

#endif      /* MS_CUSTH */
