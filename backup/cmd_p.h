#if !defined(CMD_P_H)
#define      CMD_P_H
/**
 *
 * This file defines all of the prototypes used internally within the
 * CGI feature
 *
 * @file
 * @ingroup cmd
 * $Id: cmd_p.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2012-2015 - All rights reserved */

#include "cmd_types.h"


/******************************************************************************
 * CALIBRATIONS
 *****************************************************************************/


 /******************************************************************************
 * PRIVATE MACROS
 *****************************************************************************/

/** Shorthand for determining whether we have just left the charge state.
 */
#define CMD_CHARGE_STATE_EXIT ((bool_t)((cmd_ms_state_prev == Ms_state_charger) \
                                   &&   (ms_sys_state != Ms_state_charger)))


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

typedef enum
{
    Cmd_s_chg_detect,
    Cmd_s_chg_enable,
    Cmd_s_chg_monitor,
    Cmd_s_shutdown
} cmd_sm_state_e;

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

#ifdef UNIT_TEST

#endif


/******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *****************************************************************************/

PRIVATE void cmd_update_cycle_pre_process(void);
PRIVATE void cmd_update_cycle_post_process(void);
PRIVATE void cmd_update_system_shutdown_state(void);
PRIVATE void cmd_update_state(void);
PRIVATE void cmd_sm_chg_detect_entry(void);
PRIVATE void cmd_sm_chg_detect(void);
PRIVATE void cmd_sm_chg_enable_entry(void);
PRIVATE void cmd_sm_chg_enable(void);
PRIVATE void cmd_sm_chg_monitor_entry(void);
PRIVATE void cmd_sm_chg_monitor(void);
PRIVATE void cmd_sm_shutdown_entry(void);
PRIVATE void cmd_sm_shutdown(void);
PRIVATE void cmd_set_mode_requests(void);
PRIVATE void cmd_set_ctc_demand(void);


#endif      /* CMD_P_H */

