/**
 *
 * Routines for reading the temperature measurement input signals
 *
 * @file
 * @ingroup tmi
 * $Id: tmi.c 11510 2013-12-18 17:37:30Z dc17 $
 */

/* Copyright (c) Linkedriving Tech 2011 - All rights reserved */

#include "basic_types.h"
#include "hardware.h"
#include "mmc.h"             /* access to the MMC specific data types     */
#include "tmi.h"             /* public header for this module             */
#include "tmi_p.h"           /* private header for this module            */
#include "ai.h"              /* access to the A/D conversion macros       */
#include "dio_cfg.h"         /* access to the digital output enable       */
#include "fixedpoint.h"      /* access to the fixedpoint utility routines */
#include "fm.h"              /* access to the fault monitoring routines   */
#include "interp.h"          /* access to the interpolation utilities     */
#include "ms.h"              /* For shutdown flags                        */
#include "ts.h"              /* For access to timestamp utilities         */
#include "mmc_hw_version.h"  /* For access to the hardware build version  */

/******************************************************************************
 * CALIBRATION DATA
 *****************************************************************************/

#define MEMORY_LOCATION CALIBRATION_MEMORY_S12
#include "memory_layout.h"

CAL temperature_t tmi_sensor_tolerance[Tmi_last_sensor]  =
{
    TEMP(1.0),
    TEMP(1.0),
    TEMP(1.0),
    TEMP(1.0),
};
/**<
 * @a2l{conversion,temperature_t}
 * @a2l{min,0}
 * @a2l{max,100}
 * @a2l{longidentifier,The tolerance to be applied to the temperature sensor readings.
                       Contributes to:
                           Fm_plat_temp1_input
                           Fm_plat_temp2_input
                           Fm_plat_temp3_input
                           Fm_plat_temp4_input. }
 * @a2l{annotation, REQ_BMS_SUP_MTM_024}
 */

CAL temperature_t tmi_sensor_upper_limit[Tmi_last_sensor] =
{
    TEMP(125.0),
    TEMP(125.0),
    TEMP(125.0),
    TEMP(125.0),
};
/**<
 * @a2l{conversion,temperature_t}
 * @a2l{min,-50}
 * @a2l{max,125}
 * @a2l{longidentifier,The temperature sensors may not exceed this upper limit.
                       Contributes to:
                           Fm_plat_temp1_input
                           Fm_plat_temp2_input
                           Fm_plat_temp3_input
                           Fm_plat_temp4_input. }
 * @a2l{annotation, REQ_BMS_SUP_MTM_021}
 */

CAL temperature_t tmi_sensor_lower_limit[Tmi_last_sensor] =
{
    TEMP(-50.0),
    TEMP(-50.0),
    TEMP(-50.0),
    TEMP(-50.0),
};
/**<
 * @a2l{conversion,temperature_t}
 * @a2l{min,-50}
 * @a2l{max,125}
 * @a2l{longidentifier,The temperature sensors may not fall below this lower limit}
 * @a2l{annotation, REQ_BMS_SUP_MTM_022}
 */

CAL enum tmi_sensor_brand_e tmi_sensor_brand[Tmi_last_sensor] =
{
    Tmi_sensor_Bosch,
    Tmi_sensor_Bosch,
    Tmi_sensor_none,
    Tmi_sensor_none
};
/**<
 * @a2l{conversion,tmi_sensor_brand_e}
 * @a2l{LongIdentifier,The temperature sensors may or may not be attached}
 * @a2l{annotation, REQ_BMS_SUP_MTM_003}
 */

CAL ts_time_t tmi_settle_delay = TIME(100);
/**<
 * @a2l{conversion,ts_time_t}
 * @a2l{LongIdentifier,The power-on settling time before the sensors are ready}
 * @a2l{annotation, REQ_BMS_SUP_MTM_041}
 */

CAL temperature_t tmi_inlet_outlet_t_delta_th = TEMP(40.0);
/**<
 * @a2l{conversion,temperature_t}
 * @a2l{min,0}
 * @a2l{max,175}
 * @a2l{longidentifier,The maximum absolute difference between the inlet and outlet  
 temperature measurements}
 */

#define MEMORY_LOCATION GENERAL_MEMORY
#include "memory_layout.h"

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

/** The raw ADC values used for the last temperature calculations */
uint16_t tmi_adc_values[Tmi_last_sensor];

/** @polyspace<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes */
PRIVATE const Fm_fault_list_e tmi_sensor_fault_types[Tmi_last_sensor] =
{
    Fm_plat_temp1_input,
    Fm_plat_temp2_input,
    Fm_plat_temp3_input,
    Fm_plat_temp4_input,
};
/**<
 * @a2l{LongIdentifier, Fault codes to be used in situations where the temperature
is below the minimum permitted}
 */

/**
 * Lookup table giving the addresses where the raw ADC values may be read for
 * each sensor
 */
/** @polyspace<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes */
PRIVATE CONST struct tmi_sensor_data_s tmi_raw_sensor_inputs[Tmi_last_sensor] =
{
    {
        &hld_adc_temp_in1,
        &hld_adc_temp_in1_validity
    },
    {
        &hld_adc_temp_in2,
        &hld_adc_temp_in2_validity
    },
    {
        &hld_adc_temp_in3,
        &hld_adc_temp_in3_validity
    },
    {
        &hld_adc_temp_in4,
        &hld_adc_temp_in4_validity
    }
};

/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/
temperature_t tmi_temperature[Tmi_last_sensor] =
{
    TEMP(0.0),
    TEMP(0.0),
    TEMP(0.0),
    TEMP(0.0),
};
/**<
 * @a2l{conversion, temperature_t}
 * @a2l{LongIdentifier, The last valid temperatures read for each sensor}
 * @a2l{annotation, REQ_BMS_SUP_MTM_002}
 */

data_validity_e tmi_validity[Tmi_last_sensor] =
{
    No_data_available,
    No_data_available,
    No_data_available,
    No_data_available,
};
/**<
 * @a2l{conversion, data_validity_e}
 * @a2l{LongIdentifier, Flags indicating the validity of the last reading of
each temperature sensor}
 * @a2l{annotation, REQ_BMS_SUP_MTM_023 REQ_BMS_SUP_MTM_024}
 */

ts_time_t tmi_settle_time;
/**<
 * @a2l{conversion, ts_time_t}
 * @a2l{LongIdentifier, The time that the temperature sensors were enabled}
 * @a2l{annotation, REQ_BMS_SUP_MTM_002}
 */

enum tmi_state_e tmi_state = Tmi_settling;
/**<
 * @a2l{conversion, tmi_state_e}
 * @a2l{LongIdentifier, The current operating state of the TMI feature}
 */

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/* documentation with prototype in header file */
FCODE void tmi_init(void)
{
    tmi_temperature[Tmi_sensor_1] = TEMP(0.0);
    tmi_temperature[Tmi_sensor_2] = TEMP(0.0);
    tmi_temperature[Tmi_sensor_3] = TEMP(0.0);
    tmi_temperature[Tmi_sensor_4] = TEMP(0.0);
    tmi_validity[Tmi_sensor_1] = No_data_available;
    tmi_validity[Tmi_sensor_2] = No_data_available;
    tmi_validity[Tmi_sensor_3] = No_data_available;
    tmi_validity[Tmi_sensor_4] = No_data_available;

    /* Enable the temperature sensors */
    do_5v_ref_buf_enb = TRUE;

    /* Capture the current time to allow the sensors to settle after power-on */
    tmi_settle_time = ts_get_ms();

    /* Set the state to settling */
    tmi_state = Tmi_settling;
}

/* documentation with prototype in header file */
FCODE void tmi_read_inputs(void)
{
//    enum tmi_sensor_no_e sensor;
//    bool_t fault_monitor;
//    bool_t fault_present;
//    int32_t t_delta;
//
//    /* TMI is neither a primary nor a secondary feature, so simply monitor
//     * for pending shutdowns and act accordingly
//     */
//    if (ms_shutdown_pending)
//    {
//        /* Disable the temperature sensors */
//        do_5v_ref_buf_enb = FALSE;
//    }
//    else
//    {
//        /* Enable the temperature sensors. NB This will ensure that the sensors
//         * are powered if the MMC has attempted to shutdown, but has since recovered.
//         */
//        do_5v_ref_buf_enb = TRUE;
//    }
//
//    switch (tmi_state)
//    {
//    case Tmi_settling:
//        tmi_settle();
//        break;
//
//    case Tmi_running:
//    default:
//        for (sensor = Tmi_sensor_1; sensor < Tmi_last_sensor; sensor++)
//        {
//            if (*tmi_raw_sensor_inputs[sensor].validity == New_data_available)
//            {
//                Fm_fault_subtype_list_e fault_subtype = Fm_general_failure_information;
//                fault_monitor = FALSE;
//                fault_present = FALSE;
//
//                if (tmi_sensor_brand[sensor] != Tmi_sensor_none)
//                {
//                    uint16_t adc_value = *tmi_raw_sensor_inputs[sensor].sensor_address;
//
//                    /* Store the ADC value for testing purposes */
//                    tmi_adc_values[sensor] = adc_value;
//
//                    /* Now validate the ADC reading */
//                    fault_subtype = tmi_validate_adc_reading(sensor, adc_value);
//
//                    if (fault_subtype == Fm_general_failure_information)
//                    {
//                        /* ADC reading is valid, validate the associated temperature */
//                        temperature_t temperature_in = tmi_convert_to_temperature(sensor, adc_value);
//
//                        fault_subtype = tmi_validate_temperature(sensor, temperature_in);
//
//                        if (fault_subtype != Fm_general_failure_information)
//                        {
//                            /* Invalid temperature */
//                            CLEAR_VALIDITY(tmi_validity[sensor]);
//
//                            fault_present = TRUE;
//                        }
//                        else
//                        {
//                            /* Valid temperature */
//                            SET_VALIDITY(tmi_validity[sensor]);
//
//                            fault_present = FALSE;
//
//                            tmi_temperature[sensor] = temperature_in;
//                        }
//                    }
//                    else
//                    {
//                        /* Invalid ADC reading */
//                        CLEAR_VALIDITY(tmi_validity[sensor]);
//
//                        fault_present = TRUE;
//                    }
//
//                    /* Now update fault monitoring condition, using the read-back version of the output */
//                    fault_monitor = do_rb_5v_ref_buf_enb == TRUE;
//                }
//                else
//                {
//                    /* No sensor connected */
//                    CLEAR_VALIDITY(tmi_validity[sensor]);
//                }
//
//                (void) fm_update_fault_filter(tmi_sensor_fault_types[sensor], fault_subtype, fault_present, fault_monitor);
//            }
//            else
//            {
//                /* No valid ADC reading */
//                CLEAR_VALIDITY(tmi_validity[sensor]);
//            }
//        }
//
//        /* Check plausibility of related measurements */
//        fault_monitor = (bool_t)((tmi_validity[Tmi_sensor_1] == New_data_available) &&
//                                 (tmi_validity[Tmi_sensor_2] == New_data_available));
//        t_delta = (tmi_temperature[Tmi_sensor_1] >= tmi_temperature[Tmi_sensor_2]) ?
//                  subs32((int32_t)tmi_temperature[Tmi_sensor_1], (int32_t)tmi_temperature[Tmi_sensor_2], TEMPERATURE_BP, TEMPERATURE_BP, TEMPERATURE_BP) :
//                  subs32((int32_t)tmi_temperature[Tmi_sensor_2], (int32_t)tmi_temperature[Tmi_sensor_1], TEMPERATURE_BP, TEMPERATURE_BP, TEMPERATURE_BP);
//        fault_present = (bool_t)(t_delta > tmi_inlet_outlet_t_delta_th);
//        (void) fm_update_fault_filter(Fm_plat_temp_out_of_range, Fm_posn_1, fault_present, fault_monitor);
//        break;
//    }
}

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/**
 * Private function to determine if a temperature is above an acceptable level
 *
 * @param[in] temperature_in - The sensor whose reading is to be converted
 * @param[in] limit          - The maximum allowed limit for the temperature
 * @param[in] tolerance      - The tolerance allowed above the specified limit
 * @return True, if temperature is above the allowed limit, False otherwise
 */
PRIVATE bool_t tmi_above_tolerance(
                   temperature_t temperature_in,
                   temperature_t limit,
                   temperature_t tolerance)
{
    bool_t   answer = FALSE;

    if ((temperature_t) (TEMPERATURE_MAX - limit) >= tolerance)
    {
        if (temperature_in > (temperature_t) (limit + tolerance))
        {
            answer = TRUE;
        }
    }

    return answer;
}

/**
 * Private function to determine if a temperature is below an acceptable level
 *
 * @param[in] temperature_in - The sensor whose reading is to be converted
 * @param[in] limit          - The maximum allowed limit for the temperature
 * @param[in] tolerance      - The tolerance allowed below the specified limit
 * @return True, if temperature is above the allowed limit, False otherwise
 */
PRIVATE bool_t tmi_below_tolerance(
                   temperature_t temperature_in,
                   temperature_t limit,
                   temperature_t tolerance)
{
    bool_t answer = FALSE;

    if ((limit >= (temperature_t) (TEMPERATURE_MIN + tolerance)))
    {
        if (temperature_in < (temperature_t) (limit - tolerance))
        {
            answer = TRUE;
        }
    }

    return answer;
}

/**
 * Private function to convert ADC counts into degrees C
 *
 * @param[in] sensor    - The sensor whose reading is to be converted.
 * @param[in] adc_value - The current ADC reading for the sensor
 * @return The temperature scaled in degrees C
 */
PRIVATE temperature_t tmi_convert_to_temperature(enum tmi_sensor_no_e sensor, uint16_t adc_value)
{
    temperature_t temp_bp = TEMP(0.0);

    switch (tmi_sensor_brand[sensor])
    {
    case Tmi_sensor_Bosch:
        /* Hardware revision RUB-010 uses the same Bosch sensor as the other builds,
         * however the scaling of the data is different. Hence make a decision based
         * upon the hardware revision stored in the EOL calibration data.
         */
        if (mmc_build_version() == Hw_rub_010)
        {
            temp_bp = interp_1d_u16_s16 (
                         adc_value,
                         (const FPTR uint16_t *) tmi_lookup_bosch_counts_rub010,
                         (const FPTR  int16_t *) tmi_lookup_bosch_temperature_rub010,
                         (uint8_t) LOOKUP_SIZE_BOSCH);
        }
        else
        {
            temp_bp = interp_1d_u16_s16 (
                         adc_value,
                         (const FPTR uint16_t *) tmi_lookup_bosch_counts,
                         (const FPTR  int16_t *) tmi_lookup_bosch_temperature,
                         (uint8_t) LOOKUP_SIZE_BOSCH);
        }
        break;

    case Tmi_sensor_PT1000:
        temp_bp = interp_1d_u16_s16 (
                     adc_value,
                     (const FPTR uint16_t *) tmi_lookup_pt1000_counts,
                     (const FPTR  int16_t *) tmi_lookup_pt1000_temperature,
                     (uint8_t) LOOKUP_SIZE_PT);
        break;

    case Tmi_sensor_NCP:
        temp_bp = interp_1d_u16_s16 (
                     adc_value,
                     (const FPTR uint16_t *) tmi_lookup_ncp_counts,
                     (const FPTR  int16_t *) tmi_lookup_ncp_temperature,
                     (uint8_t) LOOKUP_SIZE_NCP);
        break;

    case Tmi_sensor_none:
    default:
        /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only */
        INTERNAL_ERROR();
        break;
    }

    return temp_bp;
}

/**
 * Private function to validate a possible temperature reading
 *
 * @param[in] sensor      - The sensor whose reading is to be converted.
 * @param[in] temperature - The current temperature (converted from the ADC reading) for the sensor
 * @return The fault code associated with the reading, Fm_general_failure_information if no fault
 */
PRIVATE Fm_fault_subtype_list_e tmi_validate_temperature(enum tmi_sensor_no_e sensor, temperature_t temperature)
{
    Fm_fault_subtype_list_e fault_subtype = Fm_general_failure_information;

    /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
    if (tmi_below_tolerance(temperature, tmi_sensor_lower_limit[sensor], tmi_sensor_tolerance[sensor]))
    {
        fault_subtype = Fm_min_plaus;
    }
    else
    {
        /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
        if (tmi_above_tolerance(temperature, tmi_sensor_upper_limit[sensor], tmi_sensor_tolerance[sensor]))
        {
            fault_subtype = Fm_max_plaus;
        }
    }

    return fault_subtype;
}

/**
 * Private function to validate a temperature adc value
 *
 * @param[in] sensor    - The sensor whose reading is to be converted.
 * @param[in] adc_value - The current ADC reading for the sensor
 * @return The fault code associated with the reading, Fm_general_failure_information if no fault
 */
PRIVATE Fm_fault_subtype_list_e tmi_validate_adc_reading(enum tmi_sensor_no_e sensor, uint16_t adc_value)
{
    /* Set the default subtype to general failure i.e. an enumeration that's guaranteed to exist, but
     * isn't a TMI fault subtype */
    Fm_fault_subtype_list_e fault_subtype = Fm_general_failure_information;

    switch (tmi_sensor_brand[sensor])
    {
    case Tmi_sensor_Bosch:
        if (mmc_build_version() == Hw_rub_010)
        {
            /* If reading is off the bottom of the table, set subtype to low */
            if (adc_value < tmi_lookup_bosch_counts_rub010[0])
            {
                fault_subtype = Fm_v_low;
            }

            /* If reading is off the top of the table, set subtype to high */
            if (adc_value > tmi_lookup_bosch_counts_rub010[LOOKUP_SIZE_BOSCH - 1])
            {
                fault_subtype = Fm_v_high;
            }
        }
        else
        {
            /* If reading is off the bottom of the table, set subtype to low */
            if (adc_value < tmi_lookup_bosch_counts[0])
            {
                fault_subtype = Fm_v_low;
            }

            /* If reading is off the top of the table, set subtype to high */
            if (adc_value > tmi_lookup_bosch_counts[LOOKUP_SIZE_BOSCH - 1])
            {
                fault_subtype = Fm_v_high;
            }
        }
        break;

    case Tmi_sensor_PT1000:
        /* If reading is off the bottom of the table, set subtype to low */
        if (adc_value < tmi_lookup_pt1000_counts[0])
        {
            fault_subtype = Fm_v_low;
        }

        /* If reading is off the top of the table, set subtype to high */
        if (adc_value > tmi_lookup_pt1000_counts[LOOKUP_SIZE_PT - 1])
        {
            fault_subtype = Fm_v_high;
        }
        break;

    case Tmi_sensor_NCP:
        /* If reading is off the bottom of the table, set subtype to low */
        if (adc_value < tmi_lookup_ncp_counts[0])
        {
            fault_subtype = Fm_v_low;
        }

        /* If reading is off the top of the table, set subtype to high */
        if (adc_value > tmi_lookup_ncp_counts[LOOKUP_SIZE_NCP - 1])
        {
            fault_subtype = Fm_v_high;
        }
        break;

    case Tmi_sensor_none:
    default:
        break;
    }

    return fault_subtype;
}

/**
 * Private function to update the running state of the feature
 */
PRIVATE void tmi_settle()
{
    if (ts_is_elapsed_ms(ts_get_ms(), tmi_settle_time, tmi_settle_delay))
    {
        /* Calibratable delay has expired, enter running mode */
        tmi_state = Tmi_running;
    }
}

