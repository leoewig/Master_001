/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    ai.h
 *
 * description:  
 * This feature reads the Temperature Measurement Inputs in raw ADC counts,
 * converts them into actual units (degrees C) and makes them available to
 * the rest of the system.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Temperature Measurement Inputs
 *                       
 *
 *****************************************************************************/
#if !defined(TMI_H)
#define      TMI_H
/**
 *
 * This feature reads the Temperature Measurement Inputs in raw ADC counts,
 * converts them into actual units (degrees C) and makes them available to
 * the rest of the system.
 *
 * @file
 * @ingroup tmi
 * $Id: tmi.h 11510 2013-12-18 17:37:30Z dc17 $
 */

/* Copyright (c) Linkedriving Tech 2011-2013 - All rights reserved */

/**
 *
 * <h1> Temperature Measurement Input Feature </h1>
 *
 * <h2> Design Goals </h2>
 *
 * Temperature Measurement Input module for reading raw temperature values and
 * converting to actual temperatures (degrees C).
 *
 * The raw ADC values are read from the hardware by the A/D Driver and are read
 * directly from the A/D result registers by this module.
 *
 * The requirements document for the feature can be found
 * <a href="http://viewvc/viewvc.cgi/trunk/docs/reqs/generic/RU_BMS_Support.doc?root=Q52717_NightWing&view=co">here</a>.
 * This design intends to cover all of the requirements specified in this
 * document.
 *
 * <h2> Approach Used </h2>
 *
 * The configuration of the sensors attached to the BMS is defined in the
 * Pack Configuration Data. This will allow various types of sensor (or indeed
 * no sensor) to be attached to each of the inputs.
 *
 * This module reads the temperature inputs from the hardware and providing
 * that the values are within bounds, converts the raw data to the actual
 * temperatures (degrees C). The conversion shall be performed using lookup
 * tables.
 *
 * If any of the temperature inputs are not within bounds (+/- a calibratable
 * tolerance) or the ADC reading could not possibly represent a valid temperature,
 * then reading is left at the last valid value.
 *
 * <h2> Fault Handling </h2>
 *
 * If any of the temperatures (+/- a calibratable tolerance) are out of bounds,
 * then a fault condition is present.
 *
 * The associated fault filter will be updated based upon the presence or
 * absence of the fault condition.
 *
 * <h2> Temperature Sensors Power Supply </h2>
 *
 * The power supply to the temperature sensors shall be enabled whenever the
 * MMC is not in the process of shutting down, and disabled when the MMC is
 * shutting down.
 *
 * <h2> External Access To Data </h2>
 *
 * Features that require the temperature measurement inputs can access the
 * temperature measurements directly using the appropriate index into the
 * #tmi_temperature array
 * Features that need to know the validity of the the temperature measurement
 * inputs can access this information directly using the appropriate index into
 * the #tmi_validity array
 *
 *  <h2> Feature Integration </h2>
 *
 * tmi_init() should be called once to initialise the TMI feature and then
 * tmi_read_inputs() should be called at regular intervals to maintain the
 * temperature readings.
 *
 * It is assumed that the main temperature measurement input reading routine
 * shall only be called from a single place and that multiple entries are not
 * possible. However multiple entries will be allowed to the access routines.
 *
 * <h2> Requirement Satisfaction </h2>
 * - The requirements document for the feature can be found
 * <a href="http://viewvc/viewvc.cgi/trunk/docs/reqs/generic/RU_BMS_Support.doc?root=Q52717_NightWing&view=co">in RU_BMS_Support.doc</a>.
 * - The Bosch ADC calculation can be found in [RU_BMS_Thermistor.xls](http://viewvc/viewvc.cgi/trunk/docs/reqs/project/p12/derived/RU_BMS_Thermistor.xls?root=Q52717_NightWing&view=log)
 * - The REQ_BMS_PP_PT requirements can be found in [RU_BMS_Fault_Diagnosis.doc](http://viewvc/viewvc.cgi/trunk/docs/reqs/generic/RU_BMS_Fault_Diagnosis.doc?root=Q52717_NightWing&view=log)
 *
 * All requirements are satisfied by this module.
 *
 * Requirements are satisfied as follows:
 *
 * | Requirement                |  Satisfied By           |              Tested By                      |
 * | ---------------------------|-------------------------|---------------------------------------------|
 * | REQ_BMS_PP_PT_001          | #pp_pack_max_t          | _code review_                               |
 * | REQ_BMS_PP_PT_002          | #pp_pack_min_t          | _code review_                               |
 * | REQ_BMS_SUP_MTM_001 | #Tmi_last_sensor, #tmi_temperature | TMI_pat_sensor_1_valid(), TMI_pat_sensor_2_valid(), TMI_pat_sensor_3_valid(), TMI_pat_sensor_4_valid() |
 * | REQ_BMS_SUP_MTM_002 | tmi_read_inputs(), #tmi_temperature, #tmi_settle_time | TMI_pat_sensor_1_valid() etc. |
 * | REQ_BMS_SUP_MTM_003 | #tmi_sensor_brand, #tmi_sensor_brand_e | TMI_pat_sensor_1_valid(), TMI_pat_sensor_2_valid(), TMI_pat_sensor_3_valid(), TMI_pat_sensor_4_valid() |
 * | REQ_BMS_SUP_MTM_004 | #tmi_sensor_brand | TMI_pat_sensor_1_valid(), TMI_pat_sensor_2_valid(), TMI_pat_sensor_3_valid(), TMI_pat_sensor_4_valid(), TMI_pat_sensor_1_disconnected(), TMI_pat_sensor_2_disconnected(), TMI_pat_sensor_3_disconnected(), TMI_pat_sensor_4_disconnected() |
 * | REQ_BMS_SUP_MTM_005 | tmi_validate_temperature(), #tmi_sensor_upper_limit | TMI_pat_sensor_1_invalid_high_temp() |
 * | REQ_BMS_SUP_MTM_006 | tmi_validate_temperature(), #tmi_sensor_lower_limit | TMI_pat_sensor_1_invalid_low_temp() |
 * | REQ_BMS_SUP_MTM_007 | tmi_validate_temperature(), #tmi_sensor_upper_limit | TMI_pat_sensor_2_invalid_high_temp() |
 * | REQ_BMS_SUP_MTM_008 | tmi_validate_temperature(), #tmi_sensor_lower_limit | TMI_pat_sensor_2_invalid_low_temp() |
 * | REQ_BMS_SUP_MTM_009 | tmi_validate_temperature(), #tmi_sensor_upper_limit | TMI_pat_sensor_3_invalid_high_temp() |
 * | REQ_BMS_SUP_MTM_010 | tmi_validate_temperature(), #tmi_sensor_lower_limit | TMI_pat_sensor_3_invalid_low_temp() |
 * | REQ_BMS_SUP_MTM_011 | tmi_validate_temperature(), #tmi_sensor_upper_limit | TMI_pat_sensor_4_invalid_high_temp() |
 * | REQ_BMS_SUP_MTM_012 | tmi_validate_temperature(), #tmi_sensor_lower_limit | TMI_pat_sensor_4_invalid_low_temp() |
 * | REQ_BMS_SUP_MTM_013 | #tmi_sensor_fault_types | TMI_pat_sensor_1_invalid_high_temp(), TMI_pat_sensor_1_invalid_low_temp(), TMI_pat_sensor_1_invalid_low_adc(), TMI_pat_sensor_1_invalid_high_adc() |
 * | REQ_BMS_SUP_MTM_014 | tmi_read_inputs() | TMI_pat_sensor_1_invalid_high_temp(), TMI_pat_sensor_1_invalid_low_temp() |
 * | REQ_BMS_SUP_MTM_015 | #tmi_sensor_fault_types | TMI_pat_sensor_1_invalid_high_temp(), TMI_pat_sensor_2_invalid_low_temp(), TMI_pat_sensor_2_invalid_low_adc(), TMI_pat_sensor_2_invalid_high_adc() |
 * | REQ_BMS_SUP_MTM_016 | tmi_read_inputs() | TMI_pat_sensor_2_invalid_high_temp(), TMI_pat_sensor_2_invalid_low_temp() |
 * | REQ_BMS_SUP_MTM_017 | #tmi_sensor_fault_types | TMI_pat_sensor_3_invalid_high_temp(), TMI_pat_sensor_3_invalid_low_temp(), TMI_pat_sensor_3_invalid_low_adc(), TMI_pat_sensor_3_invalid_high_adc() |
 * | REQ_BMS_SUP_MTM_018 | tmi_read_inputs() | TMI_pat_sensor_3_invalid_high_temp(), TMI_pat_sensor_3_invalid_low_temp() |
 * | REQ_BMS_SUP_MTM_019 | #tmi_sensor_fault_types | TMI_pat_sensor_4_invalid_high_temp(), TMI_pat_sensor_4_invalid_low_temp(), TMI_pat_sensor_4_invalid_low_adc(), TMI_pat_sensor_4_invalid_high_adc() |
 * | REQ_BMS_SUP_MTM_020 | tmi_read_inputs() | TMI_pat_sensor_4_invalid_high_temp(), TMI_pat_sensor_4_invalid_low_temp() |
 * | REQ_BMS_SUP_MTM_021 | #tmi_sensor_upper_limit          | _code review_  |
 * | REQ_BMS_SUP_MTM_022 | #tmi_sensor_lower_limit          | _code review_  |
 * | REQ_BMS_SUP_MTM_023 | tmi_read_inputs(), #tmi_validity | TMI_pat_sensor_1_invalid_low_adc() etc. |
 * | REQ_BMS_SUP_MTM_024 | #tmi_sensor_tolerance            | _code review_  |
 * | REQ_BMS_SUP_MTM_025 | #tmi_sensor_tolerance            | _code review_  |
 * | REQ_BMS_SUP_MTM_026 | #tmi_validity                    | _code review_  |
 * | REQ_BMS_SUP_MTM_027 | tmi_init(), tmi_read_inputs()    |  hut_tmi_supply_enable() |
 * | REQ_BMS_SUP_MTM_028 | tmi_read_inputs(), tmi_validate_adc_reading(), tmi_validate_temperature(), #tmi_validity | TMI_pat_sensor_1_invalid_low_adc() etc. |
 * | REQ_BMS_SUP_MTM_029 | tmi_validate_adc_reading() | TMI_pat_sensor_1_invalid_low_adc() |
 * | REQ_BMS_SUP_MTM_030 | tmi_validate_adc_reading() | TMI_pat_sensor_1_invalid_high_adc() |
 * | REQ_BMS_SUP_MTM_031 | tmi_validate_adc_reading() | TMI_pat_sensor_2_invalid_low_adc() |
 * | REQ_BMS_SUP_MTM_032 | tmi_validate_adc_reading() | TMI_pat_sensor_2_invalid_high_adc() |
 * | REQ_BMS_SUP_MTM_033 | tmi_validate_adc_reading() | TMI_pat_sensor_3_invalid_low_adc() |
 * | REQ_BMS_SUP_MTM_034 | tmi_validate_adc_reading() | TMI_pat_sensor_3_invalid_high_adc() |
 * | REQ_BMS_SUP_MTM_035 | tmi_validate_adc_reading() | TMI_pat_sensor_4_invalid_low_adc() |
 * | REQ_BMS_SUP_MTM_036 | tmi_validate_adc_reading() | TMI_pat_sensor_4_invalid_high_adc() |
 * | REQ_BMS_SUP_MTM_037 | tmi_read_inputs(),tmi_validate_adc_reading() | TMI_pat_sensor_1_invalid_low_adc(), TMI_pat_sensor_1_invalid_high_adc()|
 * | REQ_BMS_SUP_MTM_038 | tmi_read_inputs(),tmi_validate_adc_reading() | TMI_pat_sensor_2_invalid_low_adc(), TMI_pat_sensor_2_invalid_high_adc()|
 * | REQ_BMS_SUP_MTM_039 | tmi_read_inputs(),tmi_validate_adc_reading() | TMI_pat_sensor_3_invalid_low_adc(), TMI_pat_sensor_3_invalid_high_adc()|
 * | REQ_BMS_SUP_MTM_040 | tmi_read_inputs(),tmi_validate_adc_reading() | TMI_pat_sensor_4_invalid_low_adc(), TMI_pat_sensor_4_invalid_high_adc()|
 * | REQ_BMS_SUP_MTM_041 | #tmi_settle_delay                | _code review_ |
 *
 * @untestable REQ_BMS_SUP_MTM_021
 * @untestable REQ_BMS_SUP_MTM_022
 * @untestable REQ_BMS_SUP_MTM_024
 * @untestable REQ_BMS_SUP_MTM_025
 * @untestable REQ_BMS_SUP_MTM_026
 * @untestable REQ_BMS_SUP_MTM_041 
 *
 * @defgroup tmi Temperature Measurement Input
 * @ingroup platform
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "mmc.h"
#include "tmi_types.h"

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern uint16_t         tmi_adc_values     [Tmi_last_sensor];
extern temperature_t    tmi_temperature    [Tmi_last_sensor];
extern data_validity_e  tmi_validity       [Tmi_last_sensor];
extern uint16_t         tmi_raw_adc_values [Tmi_last_sensor];
extern enum tmi_state_e tmi_state;

extern FROM uint16_t             tmi_lookup_pt1000_counts[LOOKUP_SIZE_PT];
extern FROM temperature_t        tmi_lookup_pt1000_temperature[LOOKUP_SIZE_PT];
extern FROM uint16_t             tmi_lookup_ncp_counts[LOOKUP_SIZE_NCP];
extern FROM temperature_t        tmi_lookup_ncp_temperature[LOOKUP_SIZE_NCP];
extern FROM uint16_t             tmi_lookup_bosch_counts[LOOKUP_SIZE_BOSCH];
extern FROM temperature_t        tmi_lookup_bosch_temperature[LOOKUP_SIZE_BOSCH];
extern FROM uint16_t             tmi_lookup_bosch_counts_rub010[LOOKUP_SIZE_BOSCH];
extern FROM temperature_t        tmi_lookup_bosch_temperature_rub010[LOOKUP_SIZE_BOSCH];

extern CAL enum tmi_sensor_brand_e tmi_sensor_brand[Tmi_last_sensor];
extern CAL temperature_t           tmi_sensor_tolerance[Tmi_last_sensor];
extern CAL ts_time_t               tmi_settle_delay;
extern CAL temperature_t        tmi_inlet_outlet_t_delta_th;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 * This routine will initialise the temperature measurement input feature
 *
 * -# foreach sensor
 *  -# clear tmi_temperature[sensor]
 *  -# set the tmi_validity[sensor] to "No_data_available". */
FCODE void tmi_init(void);

/**
 *  This routine will read the raw temperature inputs (ADC counts) and, if
 *  valid, convert them into actual temperatures (degrees C), if not valid
 *  then the appropriate fault code will be raised
 *  -# Process a maximum of 4 temperature input sensors (allowing for the
 *     possibility that no sensor is attached to this input)
 *    -# Read the raw temperature sensor input and convert into an actual
 *       temperature
 *    -# Determine if the temperature is below the minimum acceptable level
 *      -# If so, raise an Out_Of_Range_Low fault and set tmi_validity[sensor]
 *         to "Old_data_available" if previously valid data was available
 *    -# Determine if the temperature is above the maximum acceptable level
 *      -# If so, raise an Out_Of_Range_High fault and clear the set
 *         tmi_validity[sensor] to "Old_data_available" if previously valid
 *         data was available
 *    -# Determine if the temperature is within acceptable limits
 *      -# If so then update the temperature sensor reading and set the
 *         tmi_validity[sensor] to "New_data_available".
 */
FCODE void tmi_read_inputs(void);

#endif      /* TMI_H */

