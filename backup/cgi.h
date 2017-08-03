/*************************************************************************//*!
 *
 * @brief
 * Public header for Charger Interface / Communications Feature [cgi]
 *
 * Copyright (c) Ricardo plc 2013-2015 - All rights reserved
 *
 * @file
 * $Id: cgi.h 873 2015-03-16 11:19:41Z j_bailey $
 *
 *****************************************************************************
 *
 * This header defines the public interface to the Charger Interface /
 * Communications Feature
 * [cgi]
 *
 *****************************************************************************/
/**
 *
 * <h1> Charger Interface </h1>
 *
 * <h2> Overview </h2>
 *
 * The Charger Interface feature implements the mechanism by which the charger
 * strategy communicates with a particular charger. It acts to de-couple
 * the generic process of charging the pack with the specifics of
 * communicating with a particular charger in order to achieve this.
 *
 * The inputs from the charger and the outputs to the charger will only be
 * updated when the cgi_update() is called, this is to ensure
 * concistency. It is expected that cgi_update() will be called
 * periodically from the scheduler.
 *
 * @defgroup cgi CGI Charger Interface
 * @ingroup chargingoverview
 * @{
 * @file
 * @}
 */

#if !defined(CGI_H)
#define      CGI_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "mmc.h"
#include "cgi_types.h"
#include "ctc_types.h"

/******************************************************************************
 * DATA DEFINITIONS
 *****************************************************************************/

extern bool_t                   cgi_charger_enabled;
extern bool_t                   cgi_is_capable_to_charge;
extern bool_t                   cgi_is_tethered_to_active;
extern bool_t                   cgi_is_tethered_to_charger;
extern bool_t                   cgi_is_tethered_to_something;
extern bool_t                   cgi_is_tethered_to_nothing;
extern bool_t                   cgi_event_charger_connected;
extern bool_t                   cgi_event_pilot_wakeup;
extern pack_v_t                 cgi_charger_voltage_limit;
extern data_validity_e          cgi_charger_voltage_limit_valid;
extern current_t                cgi_charger_current_max;
extern current_t                cgi_charger_current_limit;
extern data_validity_e          cgi_charger_current_limit_valid;
extern pack_v_t                 cgi_charger_measured_voltage;
extern data_validity_e          cgi_charger_measured_voltage_valid;
extern current_t                cgi_charger_measured_current;
extern data_validity_e          cgi_charger_measured_current_valid;
extern data_validity_e          cgi_hv_isolation_valid;
extern bool_t                   cgi_request_hv_connect;
extern bool_t                   cgi_request_hv_isolate;
extern pack_v_t                 cgi_voltage_request;
extern data_validity_e          cgi_voltage_request_valid;
extern current_t                cgi_current_request;
extern data_validity_e          cgi_current_request_valid;
extern bool_t                   cgi_request_chg_disable;
extern bool_t                   cgi_request_chg_enable;
extern cgi_hv_isolation_state_e cgi_hv_isolation;
extern bool_t                   cgi_charger_reported_fault;
extern enum Ctc_cont_state_e*   cgi_active_chg_ctc_demand;
/** Charger is unable to charge for some reason, though not due to a fault */
extern bool_t                   cgi_charger_unable_to_charge;
extern enum cgi_proximity_state_e   cgi_proximity_state;
extern bool_t cgi_proximity_state_valid;
extern supply_v_t cgi_prx_voltage;
 /** Charger status decoded from NChargerStatus signal */
extern enum cgi_charger_status_e       cgi_charger_status;

 /** Indicates charge status #cgi_charger_status is validity */
extern data_validity_e         cgi_charger_status_valid;
/** previous version of charger status valid used to determine outputs */
extern data_validity_e         cgi_charger_status_old_valid;

extern enum cgi_pilot_state_e   cgi_pilot_state;

extern bool_t                  cgi_request_inactive;
extern bool_t                  cgi_request_active;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/* public interface */

void cgi_init(void);
void cgi_read_inputs(void);
void cgi_write_outputs(void);


#endif  /* CGI_H */
