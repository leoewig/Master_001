/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    isa_p.h
 *
 * description:  
 * This file defines all of the prototypes used internally within the
 * ISA feature
 *  
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(ISA_P_H)
#define      ISA_P_H
/**
 *
 * This file defines all of the prototypes used internally within the
 * ISA feature
 *
 * @file
 * @ingroup isa
 */

/* Copyright (c) Linkedriving Tech 2011 - All rights reserved */

#include "isa_types.h"

/******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *****************************************************************************/
PRIVATE void isa_do_st_sensor_init(void);
PRIVATE void isa_do_st_sensor_init_entry(void);
PRIVATE bool_t isa_lower_faults_are_present(void);
PRIVATE void isa_cmd_set_config(void);
PRIVATE void isa_cmd_get_config(void);
PRIVATE void isa_cmd_version(void);
PRIVATE void isa_cmd_store_if_needed (void);
PRIVATE bool_t isa_is_version_valid(void);
PRIVATE bool_t isa_is_config_valid(void);
PRIVATE bool_t isa_is_mode_valid(enum isa_set_mode_e mode);
PRIVATE isa_fault_status_e isa_running_ff(ts_time_t result_wait_ms);
PRIVATE isa_fault_status_e isa_sw_ver_ff(void);
PRIVATE isa_fault_status_e isa_config_ff(void);
PRIVATE isa_fault_status_e isa_set_mode_ff(enum isa_set_mode_e mode);
PRIVATE void isa_do_st_power_on_entry(void);
PRIVATE void isa_do_st_power_on(void);
PRIVATE void isa_do_st_ver_check_entry(void);
PRIVATE void isa_do_st_ver_check(void);
PRIVATE void isa_do_st_stop_entry(void);
PRIVATE void isa_do_st_stop(void);
PRIVATE void isa_do_st_config_entry(void);
PRIVATE void isa_do_st_config(void);
PRIVATE void isa_do_st_start_entry(void);
PRIVATE void isa_do_st_start(void);
PRIVATE void isa_do_st_save_config(void);
PRIVATE void isa_do_st_save_config_entry(void);
PRIVATE void isa_do_st_serial_number(void);
PRIVATE void isa_do_st_serial_number_entry(void);
PRIVATE void isa_do_st_calibration_entry(void);
PRIVATE void isa_do_st_calibration(void);
PRIVATE void isa_do_st_sensor_measuring_entry(void);
PRIVATE void isa_do_st_sensor_measuring(void);
PRIVATE void isa_do_st_reset_power_down_entry(void);
PRIVATE void isa_do_st_reset_power_down(void);
PRIVATE nw_sf_e isa_calc_offset(void);
PRIVATE void isa_process_sw_ver_response(void);
PRIVATE void isa_process_config_response(void);
PRIVATE void isa_process_results(void);
PRIVATE void isa_process_set_mode_response(enum isa_set_mode_e mode);
PRIVATE void isa_cmd_mode(enum isa_set_mode_e mode);
PRIVATE void isa_calc_meas_error(void);
PRIVATE void isa_process_serial_number_response(void);
PRIVATE void isa_cmd_get_serial_number(void);
PRIVATE isa_fault_status_e isa_serial_number_ff(void);
PRIVATE Fm_fault_subtype_list_e isa_get_fault_subtype(void);
PRIVATE void isa_cmd_get_status(void);
PRIVATE void isa_process_status_msg(void);
PRIVATE void isa_process_measurment_msg(void);
PRIVATE isa_fault_status_e isa_internal_status_ff(ts_time_t result_wait_ms);
PRIVATE void isa_process_internal_status(void);
PRIVATE isa_fault_status_e isa_overall_fault_status(bool_t detected, bool_t flt1, bool_t flt2);
PRIVATE void isa_trans_st_calibration (isa_fault_status_e msg_result);

/******************************************************************************
 * CALIBRATIONS
 *****************************************************************************/
extern CAL uint8_t isa_error_mask_bd1;
extern CAL uint8_t isa_error_mask_bd2;
extern CAL ts_time_t isa_power_up_wait_ms;
extern CAL ts_time_t isa_power_down_wait_ms;
extern CAL ts_time_t isa_response_wait_ms;
extern CAL uint16_t isa_num_cal_measurements;
extern CAL current_t isa_max_offset_ma;
extern CAL ts_time_t isa_max_result_wait_ms;
extern CAL bool_t isa_invert_raw_current;
extern CAL bool_t isa_report_uncal_current;
extern CAL uint32_t isa_min_sw_version;
extern CAL uint32_t isa_max_sw_version;
extern CAL percent_t isa_error_percent;
extern CAL ts_time_t isa_power_down_wait_ms;
extern CAL ts_time_t isa_power_up_result_wait_ms;
extern CAL ts_time_t isa_power_down_cycle_ms;
extern CAL current_t isa_fixed_offset_ma;
extern CAL ts_time_t isa_status_poll_ms;
extern CAL ts_time_t isa_max_status_wait_ms;
extern CAL ts_time_t isa_power_down_cycle_ms;

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/
#if defined UNIT_TEST && !defined _lint
extern current_t isa_offset_ma; /**< Calibration offset applied to raw current */
extern isa_init_state_e isa_init_state; /**< status of the init state machine */
extern int32_t isa_calc_offset_sum; /**< Used for calculating #isa_offset_ma
                                          Note: with ISA_MAX_RAW_RESULT of
                                          1500000 it's possible to sum 1400
                                          readings before this variable breaks*/
extern ts_time_t isa_time_entered_state_ms;       /**< General purpose timestamp*/
extern uint16_t isa_meas_counter; /**< For counting the number of measurements
                                                     in the calibration stage */
extern ts_time_t isa_last_measurement_ms; /**< Time that the last good
                                                result was received. */
extern bool_t isa_valid_ver_received; /**< indicates if a valid sw
                                                    version has been received */
extern bool_t isa_sw_ver_received;  /**< indicates if a sw version
                                                    message has been received
                                                    (regardless of whether the
                                                    msg was valid or not) */
extern bool_t isa_valid_config_received; /**< indicates if a valid
                                                    config has been received */
extern bool_t isa_config_received;       /**< indicates if a config
                                                    message has been received
                                                    (regardless of whether the
                                                    msg was valid or not) */
extern bool_t isa_result_received;       /**< indicates if a result
                                                    message has been received
                                                    (regardless of whether the
                                                    msg was valid or not) */
extern bool_t isa_response_received;     /**< indicates if a response
                                                    message has been received
                                                    (regardless of whether the
                                                    msg was valid or not) */
extern bool_t isa_valid_response_received; /**< indicates if a valid
                                                   response has been received */
extern uint8_t isa_result_status;/**< Result status bits from result message */
extern uint8_t isa_result_type;  /**< Result types bits from result message */
extern bool_t isa_config_sent;   /**< "Set Config" command has been sent */
extern uint8_t isa_sensor_serial_num[ISA_SERIAL_NUM_SIZE];
extern bool_t    isa_indicates_overflow;
extern bool_t    isa_powered_on; 
extern bool_t    isa_current_valid; 
extern ts_time_t isa_last_power_down_ms;
extern bool_t    isa_internal_status_ok;
extern ts_time_t isa_last_internal_status_ms;
extern bool_t    isa_internal_status_msg_received;
//extern uint32_t  isa_sw_version;
#endif      /* UNIT_TEST */
#endif      /* ISA_P_H */

