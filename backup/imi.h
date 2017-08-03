/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   imi.h
 *
 * description: 
 * Header file for Current Measurement feature
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
/**
 * @file
 *
 * Header file for Current Measurement feature
 *
 * $Id: imi.h 11312 2013-11-15 10:19:18Z tg10 $
 */

/**
 * Current Measurement Interface
 *
 * <h2> Requirements </h2>
 * The requirements for the feature can be found in the
 *
 * <h3> Requirement Traceability </h3>
 * | Requirement | Code | Tested By |
 * | ---------------------------|-------------------------|---------------------------------------------|
 * | REQ_BMS_MS_STANDBY_009 | #imi_init_status, imi_read_inputs() | |
 * | REQ_BMS_PP_I_001 | #imi_latest_data, imi_read_inputs() | |
 * | REQ_BMS_PP_PL_027 | #pp_max_discharge_i_ma | |
 * | REQ_BMS_SUP_CRM_001 | Requirement no longer valid | |
 * | REQ_BMS_SUP_CRM_002 | imi_read_sensor_current() | pat_imi_inst_curr() |
 * | REQ_BMS_SUP_CRM_004 | imi_read_inputs(), imi_exe_rectangle_rule(),imi_get_integ_cur_val() | |
 * | REQ_BMS_SUP_CRM_007 | #Fm_plat_all_i, imi_is_sensor_faulty(), imi_read_inputs() | pat_imi_inst_curr(), pat_imi_inst_curr_invalid() |
 * | REQ_BMS_SUP_CRM_008 | #Fm_plat_all_i, imi_is_sensor_faulty(), imi_read_inputs() | pat_imi_inst_curr(), pat_imi_inst_curr_invalid() |
 * | REQ_BMS_SUP_CRM_009 | #Fm_plat_all_i, imi_is_sensor_faulty(), imi_read_inputs() | pat_imi_inst_curr(), pat_imi_inst_curr_invalid() |
 * | REQ_BMS_SUP_CRM_012 | #imi_sensor_type | |
 * | REQ_BMS_SUP_CRM_013 | imi_get_integ_cur_val() | |
 * | REQ_BMS_SUP_CRM_014 | imi_get_integ_cur_val(), #imi_integral_data | |
 * | REQ_BMS_SUP_CRM_017 | imi_read_inputs() | pat_imi_inst_curr_invalid() |
 * | REQ_BMS_SUP_CRM_018 | imi_read_inputs() | |
 * | REQ_BMS_SUP_CRM_019 | #imi_latest_data | |
 * | REQ_BMS_SUP_CRM_020 | #imi_integral_data | |
 * | REQ_BMS_SUP_CRM_023 | #imi_zero_curr_tol_mA | |
 * | REQ_BMS_SUP_CRM_024 | imi_check_zero_curr_status() | pat_imi_curr_zero_positive(), pat_imi_curr_zero_negative(), pat_imi_curr_non_zero_charge(), pat_imi_curr_non_zero_discharge() |
 * | REQ_BMS_SUP_CRM_025 | imi_check_zero_curr_status() | pat_imi_inst_curr() |
 * | REQ_BMS_SUP_CRM_026 | imi_check_zero_curr_status() | pat_imi_inst_curr_invalid() |
 * | REQ_BMS_SUP_CRM_027 | #Imi_latest_data_s, #imi_latest_data, imi_read_inputs() | |
 * | REQ_BMS_SUP_CRM_028 | #imi_sensor_measure_err_ma | |
 * | REQ_BMS_SUP_CRM_029 | imi_exe_rectangle_rule() | |
 * | REQ_BMS_SUP_CRM_030 | #imi_hv_bus_zero_curr_status | |
 * | REQ_BMS_SUP_CRM_031 | imi_exe_rectangle_rule() | |
 * | REQ_BMS_SUP_CRM_032 | #imi_hv_bus_zero_curr_charge_status | pat_imi_curr_zero_positive(), pat_imi_curr_zero_negative(), pat_imi_curr_non_zero_charge() |
 * | REQ_BMS_SUP_CRM_033 | imi_check_zero_curr_status() | pat_imi_curr_zero_positive(), pat_imi_curr_zero_negative(), pat_imi_curr_non_zero_charge() |
 * | REQ_BMS_SUP_CRM_034 | imi_check_zero_curr_status() | pat_imi_curr_zero_positive(), pat_imi_curr_zero_negative(), pat_imi_curr_non_zero_charge() |
 * | REQ_BMS_SUP_CRM_035 | imi_check_zero_curr_status() | pat_imi_curr_zero_positive(), pat_imi_curr_zero_negative(), pat_imi_curr_non_zero_charge() |
 * | REQ_BMS_SUP_CRM_036 | #imi_hv_bus_zero_curr_discharge_status | pat_imi_curr_zero_positive(), pat_imi_curr_zero_negative(), pat_imi_curr_non_zero_discharge() |
 * | REQ_BMS_SUP_CRM_037 | #Imi_latest_data_s, #imi_latest_data | |
 * | REQ_BMS_SUP_CRM_037 | #imi_hv_bus_zero_curr_discharge_status | pat_imi_curr_zero_positive(), pat_imi_curr_zero_negative(), pat_imi_curr_non_zero_discharge() |
 * | REQ_BMS_SUP_CRM_038 | #imi_sensor_overflow | |
 * | REQ_BMS_SUP_CRM_038 | #imi_hv_bus_zero_curr_discharge_status | pat_imi_curr_zero_positive(), pat_imi_curr_zero_negative(), pat_imi_curr_non_zero_discharge() |
 * | REQ_BMS_SUP_CRM_039 | #imi_latest_data | pat_imi_curr_zero_positive(), pat_imi_curr_zero_negative(), pat_imi_curr_non_zero_discharge() |
 *
 * <h2> Brief Information on the Current Measurement interface Module </h2>
 * The Current Measurement module is one of the several interface feature modules.
 * It provides access to current measurement data (scaled in physical units: mA)
 * for the rest of the strategy.
 *
 * In the P12 BMS system, the Current measurement is performed using the CAN
 * based Isabellenhutte current sensor. The current design specifically uses
 * only the ISA driver services. All other configurations shall effectively disable
 * the IMI feature, but shall otherwise affect the operation of the MMC software.
 *
 * In other BMS systems the current measurement could also be done using multiple
 * sensors and employing arbitration strategies etc, depending upon the system configuration.
 *
 * <h2> Initialization </h2>
 * The Current Measurement module is initialized using the function imi_init().
 * Before this module can be used it is necessary to have the underlying
 * current sensor driver initialised and ready for use. Since the underlying
 * current sensor driver initialization may consume time, the initialisation of
 * the imi module is completed in the imi_read_inputs() and the flag to indicate
 * the IMI init status is updated accordingly.
 *
 * <h2> Design Details </h2>
 * The IMI module provides access to the current measurement data. It basically
 * provides 4 types of current values along with their individual data quality
 * indicator. The respective data quality indicators should be checked before
 * considering the current measurement data. The types of current measurement
 * data provided are:
 * -# Integral value of measured current since its last reset.
 * -# The instantaneous current value, read from the sensor.
 *
 * The imi_init() function determines the type of current sensor used in the
 * system. Based on the type of sensor used, the imi_read_inputs() function
 * calls the appropriate current sensor interface. In the P12 BMS program since
 * the only current sensor used is the ISA current sensor, the function
 * imi_read_inputs() uses only the ISA driver interface to get the current
 * readings (which is done inside the function imi_read_sensor_curr()).
 *
 * The imi_read_inputs() function is called periodically to fetch the latest
 * value of the measured current from the underlying current sensor. The data
 * read from the sensor is only processed if it is valid. If the data read from
 * the sensor is invalid, the read data is discarded and the associated data
 * quality indicators for all data items are updated to indicate that is
 * currently invalid (#Old_data_available). In this case the integrated current
 * value retains its most recent valid value.
 *
 * If the currently used current sensor reports any fault, the
 * fault-type #Fm_plat_all_i is used, to represent it.
 *
 * The imi_read_inputs() invokes the private function imi_compute_current_data()
 * to compute the integral value of the current, during the present measurement cycle.
 * It also calls imi_check_zero_curr_status() to determine whether the current on the
 * HV bus is effectively zero or not. This is indicated by the flag
 * #imi_hv_bus_zero_curr_status. If the instantaneous current is valid and its
 * "magnitude" is less than or equal
 * to #imi_zero_curr_tol_mA the flag #imi_hv_bus_zero_curr_status is set
 * to #Imi_hv_zero. If the instantaneous current is valid and its
 * "magnitude" is greater than #imi_zero_curr_tol_mA, the
 * flag #imi_hv_bus_zero_curr_status is set to #Imi_hv_non_zero. If the
 * instantaneous current is invalid, #imi_hv_bus_zero_curr_status is set
 * to #Imi_hv_invalid.
 *
 * The integral value of current is computed in imi_exe_rectangle_rule(), using
 * the rectangle rule with mid-point approximation.
 *
 * Before using any of the current data items, the respective data quality
 * indicator must be checked for validity. Please refer to the table below, for
 * the current data items and the respective data quality indicator.
 *
 * | Data | Data Quality Indicator |
 * | -----|------------------------|
 * | Integral value of measured current | returned by imi_get_integ_cur_val() |
 * | Instantaneous current value        | imi_latest_data.latest_mA_validity |
 * | #imi_hv_bus_zero_curr_status       | check against the value #Imi_hv_invalid |
 *
 * The instantaneous current value is available through the structure
 * variable #imi_latest_data. The integrated current value
 * can be obtained, by calling the function imi_get_integ_cur_val(). This function
 * will also reset the requested data atomically.
 *
 * The information about the type of sensor being used currently for measurement
 * and its tolerance could be obtained by calling the function imi_get_sensor_data().
 *
 * This module consists of S12 code called from a single task.
 *
 * @defgroup imi IMI Current Measurement
 * @ingroup platform
 * @{
 * @file
 * @}
 */

#if !defined( IMI_H )
#define       IMI_H

#include "basic_types.h"
#include "vmi.h"    /* For reading the voltage at new Min & Max current.*/
#include "mmc.h"    /* For pack_v_t type. */
#include "imi_types.h"

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/* Documented in source */
extern CAL enum Imi_sensor_type_e imi_sensor_type;

/* Documented in source */
extern bool_t imi_sensor_overflow;

/* Documented in source */
extern current_t                  imi_sensor_measure_err_ma;

/* Documented in source */
extern struct Imi_latest_data_s imi_latest_data;

/* Documented in source */
extern enum imi_hv_bus_zero_curr_status_e imi_hv_bus_zero_curr_status;

/* Documented in source */
extern enum imi_hv_bus_zero_curr_status_e imi_hv_bus_zero_curr_charge_status;

/* Documented in source */
extern enum imi_hv_bus_zero_curr_status_e imi_hv_bus_zero_curr_discharge_status;

/* Documented in source */
extern enum Imi_state_e imi_init_status;

/* Documented in source */
extern CAL current_t imi_zero_curr_tol_mA;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
/* Documented in source */
void imi_init(void);

/* Documented in source */
void imi_read_inputs(void);

/* Documented in source */
enum imi_integral_validity_e imi_get_integ_cur_val(CONST bool_t reset,
                                         FPTR high_res_charge_t *CONST data_p);

#endif
