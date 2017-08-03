/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    icm_p.h 
 *
 * description:  
 * Internal Header for Internal comms feature, state machine component.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(ICM_P_H)
#define      ICM_P_H
/**
 *
 * Internal Header for Internal comms feature, state machine component.
 *
 * @file
 * @ingroup icm
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/*****************************************************************************
 * Variables
 *****************************************************************************/

#if defined( UNIT_TEST )

/************* CAL items ********************************************/

extern CAL ts_time_t icm_vtbm_startup_delay;

extern CAL ts_time_t icm_shutdown_interval;

extern CAL ts_time_t icm_refs_interval_ms;

/************* CAN Event counts ********************************************/
extern uint32_t     icm_invalid_dlc_count;

extern uint32_t     icm_invalid_checksum_count;

extern uint32_t     icm_invalid_counter_count;

extern uint32_t     icm_processed_frames_count;

extern ts_time_t    icm_can_enable_time;

extern uint8_t icm_shutdown_attempts;

/************* Event scheduling triggers ************************************/

extern uint8_t  icm_meas_trig_counter;

extern bool_t   icm_refs_update_due;

/************* State variables **********************************************/

extern enum IcmActive_e icm_active_state;

/* see definition for description */
extern enum IcmConnecting_e icm_connecting_state;

extern enum IcmShutdown_e  icm_shutdown_state ;

extern ts_time_t   icm_shutdown_time;

#endif /* defined( UNIT_TEST ) && !_lint */

/****************************************************************************
 * PRIVATE function prototypes (documented in source)
 ****************************************************************************/

PRIVATE void icm_update_state(void);

PRIVATE void icm_state_waiting_enter(void);
PRIVATE void icm_state_waiting(void);

PRIVATE void icm_state_off_enter(void);
PRIVATE void icm_state_off(void);

PRIVATE void icm_state_connecting_enter(void);
PRIVATE void icm_state_connecting(void);
PRIVATE void icm_schedule_connecting(void);
PRIVATE bool_t icm_is_connect_busy(void);

PRIVATE void icm_state_shutdown_enter(void);
PRIVATE void icm_state_shutdown(void);

PRIVATE void icm_state_reprog_enter(void);
PRIVATE void icm_state_reprog(void);

PRIVATE enum IcmTransmit_e icm_connecting_idle_enter(void);
PRIVATE enum IcmTransmit_e icm_connecting_idle(void);

PRIVATE enum IcmTransmit_e icm_connecting_version_enter(void);
PRIVATE enum IcmTransmit_e icm_connecting_version(void);

PRIVATE enum IcmTransmit_e icm_connecting_config_enter(void);
PRIVATE enum IcmTransmit_e icm_connecting_config(void);

PRIVATE enum IcmTransmit_e icm_connecting_cellmeas_enter(void);
PRIVATE enum IcmTransmit_e icm_connecting_cellmeas(void);

PRIVATE enum IcmTransmit_e icm_connecting_refs_enter(void);
PRIVATE enum IcmTransmit_e icm_connecting_refs(void);

PRIVATE void icm_state_active_enter(void);
PRIVATE void icm_state_active(void);
PRIVATE void icm_schedule_active(void);

PRIVATE void icm_active_idle_enter(void);
PRIVATE enum IcmTransmit_e icm_active_idle(void);

PRIVATE enum IcmTransmit_e icm_active_measure_enter(void);
PRIVATE enum IcmTransmit_e icm_active_measure(void);

PRIVATE enum IcmTransmit_e icm_active_refs_enter(void);
PRIVATE enum IcmTransmit_e icm_active_refs(void);

PRIVATE enum IcmTransmit_e icm_active_status_enter(void);
PRIVATE void icm_active_status(void);

PRIVATE enum IcmTransmit_e icm_schedule_balancing( enum IcmTransmit_e tx_state);

PRIVATE void icm_shutdown_stopbal_enter(void);
PRIVATE void icm_shutdown_stopbal(void);

PRIVATE void icm_shutdown_nomeas_enter(void);
PRIVATE void icm_shutdown_nomeas(void);

PRIVATE void icm_shutdown_confirm_enter(void);
PRIVATE void icm_shutdown_confirm(void);

PRIVATE void icm_shutdown_wait_enter(void);
PRIVATE void icm_shutdown_wait(void);

/*****************************************************************************
 * Actions, kick things off or mutate variables in a specific way
 ****************************************************************************/

PRIVATE void icm_update_action_triggers(void);
PRIVATE void icm_comms_required(void);
PRIVATE void icm_update_measure_trigger(void);
PRIVATE void icm_update_refs_trigger(void);
PRIVATE void icm_update_bal_trigger(void);
PRIVATE void icm_set_pwr_outputs(void);
PRIVATE void icm_mmc_keepalive(void);
PRIVATE void icm_clear_connection_state(void);

#endif      /* ICM_P_H */

