/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     tmi_p.h 
 *
 * description:  
 * This file defines all of the prototypes used internally within the
 * Temperature Measurement Input feature
 * 
 * 
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Temperature Measurement Input feature
 *                       
 *
 *****************************************************************************/
#if !defined(TMI_P_H)
#define      TMI_P_H


#include "tmi_types.h"
#include "fm.h"           /* access to the fault monitoring routines   */

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern CAL temperature_t          tmi_sensor_tolerance  [Tmi_last_sensor];
extern CAL temperature_t          tmi_sensor_upper_limit[Tmi_last_sensor];
extern CAL temperature_t          tmi_sensor_lower_limit[Tmi_last_sensor];

extern ts_time_t                  tmi_settle_time;

/*
 * PRIVATE function prototypes (documented in source)
 */
PRIVATE bool_t tmi_above_tolerance(
                   temperature_t   temperature_in,
                   temperature_t   limit,
                   temperature_t   tolerance);

PRIVATE bool_t tmi_below_tolerance(
                   temperature_t   temperature_in,
                   temperature_t   limit,
                   temperature_t   tolerance);

PRIVATE int16_t tmi_convert_to_temperature(
                   enum tmi_sensor_no_e sensor,
                   uint16_t             adc_value);

PRIVATE Fm_fault_subtype_list_e tmi_validate_temperature(
                   enum tmi_sensor_no_e sensor,
                   temperature_t        temperature);

PRIVATE Fm_fault_subtype_list_e tmi_validate_adc_reading(
                   enum tmi_sensor_no_e sensor,
                   uint16_t             adc_value);

PRIVATE bool_t tmi_check_hw_version(void);

PRIVATE void   tmi_settle(void);

#endif      /* TMI_P_H */

