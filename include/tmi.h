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

/**
 *
 * <h1> Temperature Measurement Input Feature </h1>
 
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

extern FROM uint32_t             tmi_lookup_pt1000_counts[LOOKUP_SIZE_PT];
extern FROM int32_t        tmi_lookup_pt1000_temperature[LOOKUP_SIZE_PT];
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

