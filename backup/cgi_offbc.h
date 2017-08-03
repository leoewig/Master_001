#if !defined(CGI_OFFBC_H)
#define      CGI_OFFBC_H
/**
 *
 * Public header for the off-board charger type
 *
 * @file
 * @ingroup cgi
 * $Id: cgi_offbc.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2012-2015 - All rights reserved */

#include "cgi_types.h"


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

extern bool_t           cgi_offbc_charger_enabled;
extern bool_t           cgi_offbc_tether_status_valid;
extern bool_t           cgi_offbc_is_tethered_to_nothing;
extern bool_t           cgi_offbc_is_tethered_to_something;
extern bool_t           cgi_offbc_is_tethered_to_charger;
extern bool_t           cgi_offbc_is_tethered_to_active;
extern bool_t           cgi_offbc_evt_charger_connected;
extern bool_t           cgi_offbc_evt_pilot_wakeup;
extern bool_t           cgi_offbc_request_chg_disable;
extern bool_t           cgi_offbc_request_chg_enable;
extern current_t        cgi_offbc_maximum_current;
extern current_t        cgi_offbc_charger_i_limit;
extern data_validity_e  cgi_offbc_charger_i_limit_valid;
extern pack_v_t         cgi_offbc_charger_v_limit;
extern data_validity_e  cgi_offbc_charger_v_limit_valid;
extern current_t        cgi_offbc_current_request;
extern data_validity_e  cgi_offbc_current_request_valid;
extern pack_v_t         cgi_offbc_voltage_request;
extern data_validity_e  cgi_offbc_voltage_request_valid;
extern bool_t           cgi_offbc_connection_error;
extern bool_t           cgi_offbc_comms_error;
extern bool_t           cgi_offbc_charger_error;
extern bool_t           cgi_offbc_is_capable_to_charge;
extern cgi_hv_isolation_state_e cgi_offbc_hv_isolation;
extern data_validity_e  cgi_offbc_hv_isolation_valid;
extern current_t        cgi_offbc_charger_measured_i;
extern data_validity_e  cgi_offbc_charger_measured_i_valid;
extern pack_v_t         cgi_offbc_charger_measured_v;
extern data_validity_e  cgi_offbc_charger_measured_v_valid;

/* 
bool_t cgi_offbc_charger_unable_to_charge;
bool_t cgi_offbc_request_hv_connect;
bool_t cgi_offbc_request_hv_isolate;
*/

/******************************************************************************
 * MODULE SHARED FUNCTION PROTOTYPES
 *****************************************************************************/

extern void cgi_offbc_init(void);
extern void cgi_offbc_read_inputs(void);
extern void cgi_offbc_write_outputs(void);


#endif      /* CGI_OFFBC_H */

