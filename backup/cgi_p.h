#if !defined(CGI_P_H)
#define      CGI_P_H
/**
 *
 * This file defines all of the prototypes used internally within the
 * CGI feature
 *
 * @file
 * @ingroup cgi
 * $Id: cgi_p.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2012-2015 - All rights reserved */

#include "basic_types.h"
#include "cgi_types.h"
#include "ctc_types.h"


/******************************************************************************
 * CALIBRATIONS
 *****************************************************************************/


 /******************************************************************************
 * PRIVATE MACROS
 *****************************************************************************/


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

extern bool_t                 cgi_dummy_request_flag;
extern bool_t                 cgi_dummy_flag_false;
extern enum Ctc_cont_state_e* cgi_contactor_cmd_p[Cgi_chg_count];
extern enum Ctc_cont_state_e* cgi_contactor_state_p[Cgi_chg_count];
extern bool_t*                cgi_charger_enabled_p[Cgi_chg_count];
extern bool_t*                cgi_tether_status_valid_p[Cgi_chg_count];
extern bool_t*                cgi_is_tethered_to_nothing_p[Cgi_chg_count];
extern bool_t*                cgi_is_tethered_to_something_p[Cgi_chg_count];
extern bool_t*                cgi_is_tethered_to_charger_p[Cgi_chg_count];
extern bool_t*                cgi_is_tethered_to_active_p[Cgi_chg_count];
extern bool_t*                cgi_evt_charger_connected_p[Cgi_chg_count];
extern bool_t*                cgi_evt_pilot_wakeup_p[Cgi_chg_count];
extern bool_t*                cgi_request_chg_disable_p[Cgi_chg_count];
extern bool_t*                cgi_request_chg_enable_p[Cgi_chg_count];
extern current_t*             cgi_maximum_current_p[Cgi_chg_count];
extern current_t*             cgi_charger_i_limit_p[Cgi_chg_count];
extern data_validity_e*       cgi_charger_i_limit_valid_p[Cgi_chg_count];
extern pack_v_t*              cgi_charger_v_limit_p[Cgi_chg_count];
extern data_validity_e*       cgi_charger_v_limit_valid_p[Cgi_chg_count];
extern current_t*             cgi_current_request_p[Cgi_chg_count];
extern data_validity_e*       cgi_current_request_valid_p[Cgi_chg_count];
extern pack_v_t*              cgi_voltage_request_p[Cgi_chg_count];
extern data_validity_e*       cgi_voltage_request_valid_p[Cgi_chg_count];
extern bool_t*                cgi_connection_error_p[Cgi_chg_count];
extern bool_t*                cgi_comms_error_p[Cgi_chg_count];
extern bool_t*                cgi_charger_error_p[Cgi_chg_count];
extern bool_t*                cgi_is_capable_to_charge_p[Cgi_chg_count];
extern bool_t*                cgi_charger_unable_to_charge_p[Cgi_chg_count];
extern bool_t*                cgi_request_hv_connect_p[Cgi_chg_count];
extern bool_t*                cgi_request_hv_isolate_p[Cgi_chg_count];
extern cgi_hv_isolation_state_e* cgi_hv_isolation_p[Cgi_chg_count];
extern data_validity_e*       cgi_hv_isolation_valid_p[Cgi_chg_count];
extern current_t*             cgi_charger_measured_i_p[Cgi_chg_count];
extern data_validity_e*       cgi_charger_measured_i_valid_p[Cgi_chg_count];
extern pack_v_t*              cgi_charger_measured_v_p[Cgi_chg_count];
extern data_validity_e*       cgi_charger_measured_v_valid_p[Cgi_chg_count];

/******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *****************************************************************************/

#if defined UNIT_TEST && !defined _lint
#endif      /* UNIT_TEST */

PRIVATE void    cgi_update_tethering_indicators(void);
PRIVATE void    cgi_set_active_charger(void);
PRIVATE void    cgi_update_charger_hv_status(void);
PRIVATE void    cgi_update_events(void);
PRIVATE void    cgi_update_active_charger_status_indicators(void);
PRIVATE void    cgi_update_charger_limits(void);
PRIVATE void    cgi_update_charger_measurements(void);
PRIVATE void    cgi_set_charger_requests(void);


/******************************************************************************
 * SHARED FUNCTION PROTOTYPES
 *****************************************************************************/

extern supply_v_t cgi_calc_signal_voltage(FPTR const cgi_signals_configuration_t* sig_cfg);


#endif      /* CGI_P_H */

