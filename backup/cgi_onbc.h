#if !defined(CGI_ONBC_H)
#define      CGI_ONBC_H
/**
 *
 * Public header for the on-board charger type
 *
 * @file
 * @ingroup cgi
 * $Id: cgi_onbc.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2012-2015 - All rights reserved */

#include "cgi_types.h"
#include "cgi_onbc_p.h"


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
 * CGI SHARED DATA
 *****************************************************************************/

extern bool_t           cgi_onbc_charger_enabled;
extern bool_t           cgi_onbc_tether_status_valid;
extern bool_t           cgi_onbc_is_tethered_to_nothing;
extern bool_t           cgi_onbc_is_tethered_to_something;
extern bool_t           cgi_onbc_is_tethered_to_charger;
extern bool_t           cgi_onbc_is_tethered_to_active;
extern bool_t           cgi_onbc_evt_charger_connected;
extern bool_t           cgi_onbc_evt_pilot_wakeup;
extern bool_t           cgi_onbc_request_chg_disable;
extern bool_t           cgi_onbc_request_chg_enable;
extern current_t        cgi_onbc_maximum_current;
extern current_t        cgi_onbc_charger_i_limit;
extern data_validity_e  cgi_onbc_charger_i_limit_valid;
extern pack_v_t         cgi_onbc_charger_v_limit;
extern data_validity_e  cgi_onbc_charger_v_limit_valid;
extern current_t        cgi_onbc_current_request;
extern data_validity_e  cgi_onbc_current_request_valid;
extern pack_v_t         cgi_onbc_voltage_request;
extern data_validity_e  cgi_onbc_voltage_request_valid;
extern bool_t           cgi_onbc_connection_error;
extern bool_t           cgi_onbc_comms_error;
extern bool_t           cgi_onbc_charger_error;
extern bool_t           cgi_onbc_is_capable_to_charge;
extern cgi_hv_isolation_state_e cgi_onbc_hv_isolation;
extern data_validity_e  cgi_onbc_hv_isolation_valid;

extern uint8_t cgi_onbc_is_capable_to_charge_monitoring;
extern uint8_t cgi_is_capable_to_charge_monitoring;
extern enum cgi_onbc_cc_state_e cgi_onbc_cc_state;
extern enum cgi_onbc_pilot_state_e cgi_onbc_pilot_state;
extern bool_t cgi_onbc_pilot_pwm_valid;


/******************************************************************************
 * MODULE SHARED FUNCTION PROTOTYPES
 *****************************************************************************/

extern void cgi_onbc_init(void);
extern void cgi_onbc_read_inputs(void);
extern void cgi_onbc_write_outputs(void);


#endif      /* CGI_ONBC_H */

