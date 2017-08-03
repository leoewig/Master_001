/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    isa_p.h
 *
 * description:  Public header for Isabellenhuette feature
 * 
 * 
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
/*************************************************************************//**
 *
 * Public header for Isabellenhuette feature
 *
 *
 * Copyright (c) Linkedriving Tech 2012 - All rights reserved
 *
 * @file
 *
 *****************************************************************************
 *
 * This header defines the public interface to the ISA module
 *
 *
 *****************************************************************************/

 /**
 *
 * <h1> Isabellenhuette Driver </h1>
 *
 * <h2> Design goals </h2>
 *
 * The aim of the Isabellenhuette driver is to provide support for the
 * Isabellenhuette CAN current sensor.
 *
 * <h2> Requirements </h2>
 *
 * The requirements document for the feature can be found
 * <a href="http://viewvc/viewvc.cgi/trunk/docs/reqs/generic/RU_BMS_Peripherals.doc?root=Q52717_NightWing&view=co">here</a>.
 * This design intends to cover all of the requirements specified in the
 * ISA section of this document (section 3.1).
 *
 * <h2> Approach used </h2>
 *
 * The Isabellenhuette driver is based on a state machine which is managed in
 * isa_update() - The main ISA states are located here. These are;
 *  -# Power on - switch power supply on and delay until ISA sensor powers up,
 *  -# Sensor Initialization - Run through specific commands that ensure the
 *      sensor is working and configured correctly. It also calibrates the sensor
 *      in this state.
 *  -# Measurement state - measurements are provided to the rest of the system,
 *  -# Power Off - If failures are detected the ISA driver will attempt to reset
 *      the ISA sensor by cycling its power and re-initializing the sensor.
 *      Note that if successful calibration bas been undertaken previously in
 *      the current power cycle the sensor will not be recalibrated in the
 *      initialization phase. Note that the sensor will only be reset at a rate
 *      defined by #isa_power_down_cycle_ms. This allows failures time to heal
 *      if they have been previously detected but a sensor power cycle cured
 *      the issue.
 *
 *  The ISA driver maintains the following external interface;
 *      -# The current measurement #isa_current_ma
 *      -# The current measurement validity flag #isa_current_valid
 *      -# A flag indicating if the current reading indicates an
 *          overflow condition #isa_indicates_overflow,
 *      -# The calculated measurement error #isa_current_err_ma
 *      -# The power ISA power supply status #isa_powered_on
 *      -# The raw isa result without calibration
 *          applied #isa_raw_result
 *      -# The number of times the sensor has been reset #isa_num_sensor_resets
 *      -# The main state variable #isa_state.
 *
 *  Note that if the current reading is invalid the last good reading is maintained
 *  at the external interface.
 *
 * <h2> Limitations </h2>
 *
 * Only Isabellenhuette devices with software versions specified by
 * calibrations #isa_min_sw_version and #isa_max_sw_version are supported by
 * this driver.
 *
 * <h2> Configuration and Calibration </h2>

 * Each target must define an ISA configuration header and source file. The
 * build system copies the target-specific files to the build directory,
 * renaming them isa_cfg.h and isa_cfg.c. For the main application, these files
 * are called mmc_isa_cfg.h and mmc_isa_cfg.c.
 *
 * The header cfg file (e.g. mmc_isa_cfg.h) contains the configuration for the
 * ISA device.
 *
 * Calibrations for timings and device calibration are in isa.c.
 *
 * <h2> Integration </h2>
 *
 * To use this feature, you must:
 * <ol>
 * <li> Call isa_init() from initialisation code.
 * <li> Call isa_update() every 10ms from the main application task.
 * <li> The most recent valid current measurement is provided in #isa_current_ma.
 *      A positive current indicates flow into the pack.
 * <li> The present state of the driver is indicated using #isa_state.
 * </ol>
 *
 * <h2> Requirement Satisfaction </h2>
 *
 * | Requirement | Code | Tested By |
 * | --------------------------- | ------------------------- | --------------------------------------------- |
 * | REQ_BMS_ISA_CAL_001 | #isa_num_cal_measurements | |
 * | REQ_BMS_ISA_CAL_004 | #isa_max_offset_ma | |
 * | REQ_BMS_ISA_CAL_002 | #isa_offset_ma, isa_calc_offset() #isa_do_st_calibration | |
 * | REQ_BMS_ISA_CAL_005 | isa_calc_offset() | |
 * | REQ_BMS_ISA_CAL_003 | isa_do_st_calibration() | |
 * | REQ_BMS_ISA_CAL_006 | isa_do_st_calibration() | |
 * | REQ_BMS_ISA_CAL_007 | isa_calc_offset() | |
 * | REQ_BMS_ISA_CAL_008 | #isa_report_uncal_current | pat_isa_uncal_measurement() |
 * | REQ_BMS_ISA_CAL_009 | isa_do_st_calibration() | pat_isa_uncal_measurement() |
 * | REQ_BMS_ISA_CAN_001 | isa_process_results() etc. | |
 * | REQ_BMS_ISA_CFG_001 | #isa_response_wait_ms | pat_isa_config() |
 * | REQ_BMS_ISA_CFG_002 | isa_do_st_config_entry() | pat_isa_config() |
 * | REQ_BMS_ISA_CFG_003 | isa_do_st_config() | pat_isa_config() |
 * | REQ_BMS_ISA_CFG_004 | isa_config_ff() | pat_isa_config() |
 * | REQ_BMS_ISA_CFG_005 | isa_config_ff() | pat_isa_config() |
 * | REQ_BMS_ISA_CFG_006 | isa_config_ff() | pat_isa_config() |
 * | REQ_BMS_ISA_CFG_007 | isa_do_st_config() | pat_isa_config() |
 * | REQ_BMS_ISA_CFG_008 | isa_do_st_config() | pat_isa_config() |
 * | REQ_BMS_ISA_CFG_009 | isa_cmd_store_if_needed(), isa_do_st_config() | pat_isa_basic_config_correction(), pat_isa_invalid_config_uncorrectable() |
 * | REQ_BMS_ISA_CFG_010 | isa_cmd_get_config(), isa_do_st_config_entry() | pat_isa_basic_config_correction(), pat_isa_invalid_config_uncorrectable() |
 * | REQ_BMS_ISA_CFG_011 | isa_cmd_set_config(), isa_do_st_config() | pat_isa_basic_config_correction(), pat_isa_invalid_config_uncorrectable() |
 * | REQ_BMS_ISA_CFG_012 | | |
 * | REQ_BMS_ISA_CFG_013 | | |
 * | REQ_BMS_ISA_CFG_014 | | |
 * | REQ_BMS_ISA_IF_001 | #isa_state | |
 * | REQ_BMS_ISA_IF_002 | #isa_current_ma | |
 * | REQ_BMS_ISA_IF_003 | #isa_current_err_ma | |
 * | REQ_BMS_ISA_IF_004 | #isa_current_valid | |
 * | REQ_BMS_ISA_IF_005 | #isa_indicates_overflow | |
 * | REQ_BMS_ISA_IF_006 | #isa_powered_on | |
 * | REQ_BMS_ISA_PDN_001 | #isa_power_down_wait_ms | |
 * | REQ_BMS_ISA_PDN_002 | isa_do_st_reset_power_down_entry() | |
 * | REQ_BMS_ISA_PDN_003 | isa_do_st_reset_power_down() | |
 * | REQ_BMS_ISA_PDN_004 | #isa_num_sensor_resets | pat_isa_reset() |
 * | REQ_BMS_ISA_PWR_001 | #isa_power_up_wait_ms | |
 * | REQ_BMS_ISA_PWR_002 | isa_do_st_power_on_entry() | pat_isa_enable_line() |
 * | REQ_BMS_ISA_PWR_003 | isa_do_st_power_on() | pat_isa_power_on_wait() |
 * | REQ_BMS_ISA_RUN_001 | #isa_max_result_wait_ms | |
 * | REQ_BMS_ISA_RUN_002 | isa_process_results() | pat_isa_running() |
 * | REQ_BMS_ISA_RUN_005 | isa_running_ff() | pat_isa_running() |
 * | REQ_BMS_ISA_RUN_006 | isa_running_ff() | pat_isa_running() |
 * | REQ_BMS_ISA_RUN_007 | isa_running_ff() | pat_isa_running() |
 * | REQ_BMS_ISA_RUN_009 | isa_running_ff() | pat_isa_running() |
 * | REQ_BMS_ISA_RUN_010 | isa_running_ff() | pat_isa_running() |
 * | REQ_BMS_ISA_RUN_011 | #isa_invert_raw_current | |
 * | REQ_BMS_ISA_RUN_012 | #isa_error_percent | |
 * | REQ_BMS_ISA_RUN_013 | isa_calc_meas_error() | |
 * | REQ_BMS_ISA_RUN_016 | #isa_status_poll_ms | pat_isa_invalid_internal_status() |
 * | REQ_BMS_ISA_RUN_017 | | pat_isa_invalid_internal_status() |
 * | REQ_BMS_ISA_RUN_018 | isa_get_fault_subtype() | pat_isa_overflow() |
 * | REQ_BMS_ISA_RUN_019 | | pat_isa_invalid_internal_status() |
 * | REQ_BMS_ISA_RUN_020 | isa_running_ff() | pat_isa_stops_measurement() |
 * | REQ_BMS_ISA_RUN_021 | isa_running_ff() | pat_isa_stops_measurement() |
 * | REQ_BMS_ISA_RUN_022 | isa_running_ff() | pat_isa_stops_measurement() |
 * | REQ_BMS_ISA_RUN_023 | | pat_isa_invalid_config_uncorrectable() |
 * | REQ_BMS_ISA_RUN_024 | | pat_isa_invalid_config_uncorrectable() |
 * | REQ_BMS_ISA_RUN_025 | | pat_isa_stops_measurement() |
 * | REQ_BMS_ISA_SRT_001 | isa_do_st_start_entry(), isa_cmd_mode() | |
 * | REQ_BMS_ISA_SRT_002 | isa_is_mode_valid() isa_do_st_start() | |
 * | REQ_BMS_ISA_SRT_003 | isa_do_st_start() isa_set_mode_ff() | |
 * | REQ_BMS_ISA_SRT_004 | isa_set_mode_ff() | |
 * | REQ_BMS_ISA_SRT_005 | isa_set_mode_ff(), isa_do_st_stop() | |
 * | REQ_BMS_ISA_SRT_006 | isa_is_mode_valid() | |
 * | REQ_BMS_ISA_SRT_007 | isa_set_mode_ff() | |
 * | REQ_BMS_ISA_SRT_008 | isa_set_mode_ff() | |
 * | REQ_BMS_ISA_STP_001 | isa_do_st_stop_entry(), isa_cmd_mode() | |
 * | REQ_BMS_ISA_STP_002 | isa_is_mode_valid(), isa_do_st_stop() | |
 * | REQ_BMS_ISA_STP_003 | isa_do_st_stop(), isa_set_mode_ff() | |
 * | REQ_BMS_ISA_STP_004 | isa_set_mode_ff() | |
 * | REQ_BMS_ISA_STP_005 | isa_set_mode_ff(), isa_do_st_stop() | |
 * | REQ_BMS_ISA_STP_006 | isa_is_mode_valid() | |
 * | REQ_BMS_ISA_STP_007 | isa_set_mode_ff() | |
 * | REQ_BMS_ISA_STP_008 | isa_set_mode_ff() | |
 * | REQ_BMS_ISA_VER_001 | isa_do_st_ver_check_entry() | pat_isa_sw_version() |
 * | REQ_BMS_ISA_VER_002 | isa_do_st_ver_check() | pat_isa_sw_version() |
 * | REQ_BMS_ISA_VER_003 | isa_sw_ver_ff() | pat_isa_sw_version() |
 * | REQ_BMS_ISA_VER_004 | isa_sw_ver_ff() | pat_isa_sw_version() |
 * | REQ_BMS_ISA_VER_005 | isa_do_st_ver_check() | pat_isa_sw_version() |
 * | REQ_BMS_ISA_VER_006 | isa_do_st_ver_check() | pat_isa_sw_version() |
 * | REQ_BMS_ISA_VER_007 | isa_do_st_ver_check() | pat_isa_sw_version() |
 * | REQ_BMS_ISA_VER_008 | #isa_min_sw_version, isa_do_st_ver_check_entry() | pat_isa_sw_version() |
 * | REQ_BMS_ISA_VER_009 | #isa_max_sw_version, isa_do_st_ver_check_entry() | pat_isa_sw_version() |
 * | REQ_BMS_ISA_VER_010 | isa_do_st_ver_check_entry(), isa_do_st_ver_check() | pat_isa_bad_version() |
 *
 * @defgroup isa ISA Isabellenhuette Driver
 * @ingroup platform
 * @{
 * @file
 * @}
 */

#ifndef ISA_H
#define ISA_H

#include "mmc.h"
#include "isa_cfg.h"
#include "isa_types.h"



/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
extern isa_main_state_e isa_state;   /**< State of the ISA driver
                                   @a2l{conversion,isa_main_state_e} */
extern current_t isa_current_ma;     /**< Current reported by sensor with offset
                                      applied. Positive current is into the pack.
                                      @a2l{conversion,current_t} */
extern current_t isa_current_err_ma;     /**< The error applicable to the current
                                          measurement.
                                          @a2l{conversion,current_t} */
extern bool_t    isa_powered_on;        /**< ISA powersupply status */
extern bool_t    isa_current_valid;     /**< isa current validity flag  */
extern bool_t    isa_indicates_overflow;/**< ISA sensor flagging overflow condition */
extern current_t isa_raw_result;
/**< Raw result from ISA device
 * @a2l{conversion,current_t}
 */

extern uint16_t isa_num_sensor_resets; /**< Total number of sensor resets (stored in NV) */
/**< @a2l{LongIdentifier,Number of times the sensor has been reset} */

extern uint8_t isa_sensor_serial_num[ISA_SERIAL_NUM_SIZE]; /* ISA Sensor Number */

/** ISA fully configured, calibrated and reporting measurements */
extern bool_t  isa_ready;
/**************************************************************************//**
 *
 *  ISA driver initialisation
 *
 *  -# Reset #isa_current_ma and #isa_current_err_ma to zero.
 *  -# Reset the state machine by calling isa_do_st_power_on_entry().
 *
 *****************************************************************************/
void isa_init(void);

/**************************************************************************//**
 *
 *  ISA driver update
 *
 *  -# Runs the main state machine;
 *      -# Power on - enabling the ISA power supply and waiting a calibratable
 *          period for the ISA to power up.
 *      -# Sensor Initialization - running a separate state machine that controls
 *          the initialization, configuration and calibration of the sensor
 *      -# Measurement - receiving and processing current messurements. Polling
 *          and processing ISA status messages
 *      =# Power Off - Disabiling the ISA power supply and waiting a calibratable
 *          period.
 *****************************************************************************/
void isa_update(void);

/**************************************************************************//**
 *
 *  Collect ISA SW version data required by OBD snapshots
 *
 *  @param ver A pointer to the location into which the latest received ISA
 *             software version should be written
 *  @param ver_max A pointer to the location into which the maximum supported 
 *                 ISA software version should be written
 *  @param ver_min A pointer to the location into which the minimum supported 
 *                 ISA software version should be written
 *****************************************************************************/
extern void isa_ss_data_sw_ver(uint32_t *ver, uint32_t *ver_max, uint32_t *ver_min);

/**************************************************************************//**
 *
 *  Collect ISA initialisation status data required by OBD snapshots
 *
 *  @return Initialisation status
 *****************************************************************************/
extern isa_init_state_e isa_ss_data_init_state(void);

/**************************************************************************//**
 *
 *  Collect ISA result information data required by OBD snapshots
 *
 *  @param type A pointer to the location into which the result type should be
 *              written
 *
 *  @param status A pointer to the location into which the result status should 
 *                be written
 *****************************************************************************/
extern void isa_ss_data_result_info(uint8_t *type, uint8_t *status);

#endif /* ISA_H*/
