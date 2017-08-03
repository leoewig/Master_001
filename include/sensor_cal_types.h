/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   sensor_cal_types.h
 *
 * description:  Types, enums and macros for Sensor Calibration data module
 *
 *
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
#if !defined(SENSOR_CAL_TYPES_H_)
#define SENSOR_CAL_TYPES_H_

/**
 * Types, enums and macros for Sensor Calibration data module
 *
 * @file
 * @ingroup sensor_cal_types
 * $Id: sensor_cal_types.h 10415 2013-09-10 13:45:26Z tg10 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "mmc.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * Low Voltage Calibration
 *****************************************************************************/
/**
 * Enumeration to allow access into the high resolution ADC calibration data
 */
enum sensor_cal_lv_adc_e
{
    Cal_lv_vtbm_can_gnd,
    Cal_lv_12v_power,
    Cal_lv_12v_isa,
    Cal_lv_vpos_12v,
    Cal_lv_vneg_12v,
    Cal_lv_fan1_diag,
    Cal_lv_fan2_diag,
    Cal_lv_spare_hs_diag,
    Cal_lv_charger_proximity,
    Cal_lv_12v_control,
    Cal_lv_temp_input1,
    Cal_lv_temp_input2,
    Cal_lv_temp_input3,
    Cal_lv_temp_input4,
    Cal_lv_vtbm_can_5v,
    Cal_lv_5v_crr_sensor,
    Cal_lv_crr_hi,
    Cal_lv_crr_lo,
    Cal_lv_pre_chrg_diag,
    Cal_lv_veh_hv_pos_diag,
    Cal_lv_chg_hv_pos_diag,
    Cal_lv_chg_hv_neg_diag,
    Cal_lv_veh_hv_neg_diag,
    Cal_lv_chg_control_pilot,
    Cal_lv_veh_lores_pos,
    Cal_lv_veh_lores_neg,
    Cal_max_lv_adcs
};

/**
 * Structure containing the calibratables for a single low voltage ADC
 */
typedef struct
{
    uint16_t k1;
    uint16_t k2;
    int16_t  q2;
} sensor_cal_lv_adc_t;

/**
 * Sensor Calibration data structure taken from HWSW specification
 */
typedef struct
{
    sensor_cal_lv_adc_t cal_lo_voltage[Cal_max_lv_adcs];

} sensor_cal_lv_t;

/******************************************************************************
 * High Voltage Calibration
 *****************************************************************************/
/**
 * Enumeration to allow access into the high resolution ADC calibration data
 */
enum sensor_cal_hv_adc_e
{
    Cal_hv_chg_pos,
    Cal_hv_chg_neg,
    Cal_hv_veh_pos,
    Cal_hv_veh_neg,
    Cal_max_hv_adcs
};

/**
 * Structure containing the calibratables for a single high voltage ADC
 */
typedef struct
{
    uint16_t k;
    int16_t q;
    int8_t  L[32];

} sensor_cal_hv_adc_t;

/**
 * Structure containing the calibratables for all high resolution ADCs
 */
typedef struct
{
    sensor_cal_hv_adc_t cal_hv_adcs[Cal_max_hv_adcs];

} sensor_cal_hv_adcs_t;

/**
 * Sensor Calibration data structure taken from HWSW specification
 */
typedef struct
{
    sensor_cal_hv_adcs_t cal_isol_inactive;
    sensor_cal_hv_adcs_t cal_isol_active;

} sensor_cal_hv_t;

/******************************************************************************
 * ADC Calibration
 *****************************************************************************/
/**
 * Sensor Calibration data structure taken from HWSW specification
 */
typedef struct
{
    /** Will be set to 0x9BA5EDFE only if the calibration data is loaded. Any other value
     * indicates invalid data */
    uint32_t calibration_validity_marker_1;
    /** This field is used in order to check that the following data
     * is in the format expected by the MMC software. The layout described
     * in this table corresponds to version 3. */
    uint8_t data_layout_version;
    /** Set to 0xFF */
    uint8_t padding;

    sensor_cal_lv_t cal_lo_voltage;
    sensor_cal_hv_t cal_hi_voltage;

    /** The checksum shall be calculated on all the data
     * between the validity markers (excluding the checksum itself) using the
     * CRC-15 methodology.*/
    uint16_t checksum;
    /** Will be set to 0x9BA5EDFE only if the calibration data is loaded. Any other value
     * indicates invalid data*/
    uint32_t calibration_validity_marker_2;
} sensor_cal_t;

/**
 * Dummy initialiser for sensor calibration data which will keep lint quiet and avoid
 * what seems to be a linker bug on the MMC
 */
#define SENSOR_CAL_DUMMY_INITIALISER   \
{                                      \
    1u,                                \
    0u,                                \
    0u,                                \
    {                                  \
        {                              \
            { 0u, 0u, 0 },    /* Cal_lv_vtbm_can_gnd      */           \
            { 0u, 0u, 0 },    /* Cal_lv_12v_power         */           \
            { 0u, 0u, 0 },    /* Cal_lv_12v_isa           */           \
            { 0u, 0u, 0 },    /* Cal_lv_vpos_12v          */           \
            { 0u, 0u, 0 },    /* Cal_lv_vneg_12v          */           \
            { 0u, 0u, 0 },    /* Cal_lv_fan1_diag         */           \
            { 0u, 0u, 0 },    /* Cal_lv_fan2_diag         */           \
            { 0u, 0u, 0 },    /* Cal_lv_spare_hs_diag     */           \
            { 0u, 0u, 0 },    /* Cal_lv_charger_proximity */           \
            { 0u, 0u, 0 },    /* Cal_lv_12v_control       */           \
            { 0u, 0u, 0 },    /* Cal_lv_temp_input1       */           \
            { 0u, 0u, 0 },    /* Cal_lv_temp_input2       */           \
            { 0u, 0u, 0 },    /* Cal_lv_temp_input3       */           \
            { 0u, 0u, 0 },    /* Cal_lv_temp_input4       */           \
            { 0u, 0u, 0 },    /* Cal_lv_vtbm_can_5v       */           \
            { 0u, 0u, 0 },    /* Cal_lv_5v_crr_sensor     */           \
            { 0u, 0u, 0 },    /* Cal_lv_crr_hi            */           \
            { 0u, 0u, 0 },    /* Cal_lv_crr_lo            */           \
            { 0u, 0u, 0 },    /* Cal_lv_pre_chrg_diag     */           \
            { 0u, 0u, 0 },    /* Cal_lv_veh_hv_pos_diag   */           \
            { 0u, 0u, 0 },    /* Cal_lv_chg_hv_pos_diag   */           \
            { 0u, 0u, 0 },    /* Cal_lv_chg_hv_neg_diag   */           \
            { 0u, 0u, 0 },    /* Cal_lv_veh_hv_neg_diag   */           \
            { 0u, 0u, 0 },    /* Cal_lv_chg_control_pilot */           \
            { 0u, 0u, 0 },    /* Cal_lv_veh_lores_pos     */           \
            { 0u, 0u, 0 },    /* Cal_lv_veh_lores_neg     */           \
        },                             \
    },                                 \
    {                                  \
        {                              \
            { /* Isolation Inactive */ \
                { 0u, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, \
                { 0u, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, \
                { 0u, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, \
                { 0u, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, \
            }                          \
        },                             \
        {                              \
            { /* Isolation Active   */ \
                { 0u, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, \
                { 0u, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, \
                { 0u, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, \
                { 0u, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, \
            }                          \
        }                              \
    },                                 \
    0u,                                \
    0u                                 \
}

#define SENSOR_CAL_DEFAULT_INITIALISER   \
{                                      \
    1u,                                \
    0u,                                \
    0u,                                \
    {                                  \
        {                              \
            { 0u, 0u, 0 },    /* Cal_lv_vtbm_can_gnd      */           \
            { 0u, 0u, 0 },    /* Cal_lv_12v_power         */           \
            { 0u, 0u, 0 },    /* Cal_lv_12v_isa           */           \
            { 0u, 0u, 0 },    /* Cal_lv_vpos_12v          */           \
            { 0u, 0u, 0 },    /* Cal_lv_vneg_12v          */           \
            { 0u, 0u, 0 },    /* Cal_lv_fan1_diag         */           \
            { 0u, 0u, 0 },    /* Cal_lv_fan2_diag         */           \
            { 0u, 0u, 0 },    /* Cal_lv_spare_hs_diag     */           \
            { 0u, 0u, 0 },    /* Cal_lv_charger_proximity */           \
            { 0u, 0u, 0 },    /* Cal_lv_12v_control       */           \
            { 0u, 0u, 0 },    /* Cal_lv_temp_input1       */           \
            { 0u, 0u, 0 },    /* Cal_lv_temp_input2       */           \
            { 0u, 0u, 0 },    /* Cal_lv_temp_input3       */           \
            { 0u, 0u, 0 },    /* Cal_lv_temp_input4       */           \
            { 0u, 0u, 0 },    /* Cal_lv_vtbm_can_5v       */           \
            { 0u, 0u, 0 },    /* Cal_lv_5v_crr_sensor     */           \
            { 0u, 0u, 0 },    /* Cal_lv_crr_hi            */           \
            { 0u, 0u, 0 },    /* Cal_lv_crr_lo            */           \
            { 0u, 0u, 0 },    /* Cal_lv_pre_chrg_diag     */           \
            { 0u, 0u, 0 },    /* Cal_lv_veh_hv_pos_diag   */           \
            { 0u, 0u, 0 },    /* Cal_lv_chg_hv_pos_diag   */           \
            { 0u, 0u, 0 },    /* Cal_lv_chg_hv_neg_diag   */           \
            { 0u, 0u, 0 },    /* Cal_lv_veh_hv_neg_diag   */           \
            { 0u, 0u, 0 },    /* Cal_lv_chg_control_pilot */           \
            { 0u, 0u, 0 },    /* Cal_lv_veh_lores_pos     */           \
            { 0u, 0u, 0 },    /* Cal_lv_veh_lores_neg     */           \
        },                             \
    },                                 \
    {                                  \
        {                              \
            { /* Isolation Inactive */ \
                { /* Cal_hv_chg_pos */  4416u,      0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, \
                { /* Cal_hv_chg_neg */  4801u, -16000, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, \
                { /* Cal_hv_veh_pos */  4801u,      0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, \
                { /* Cal_hv_veh_neg */  4801u, -16000, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, \
            }                          \
        },                             \
        {                              \
            { /* Isolation Active   */ \
                { /* Cal_hv_chg_pos */ 12185u,      0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, \
                { /* Cal_hv_chg_neg */  4801u, -16000, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, \
                { /* Cal_hv_veh_pos */ 12118u,      0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, \
                { /* Cal_hv_veh_neg */  4801u, -16000, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, \
            }                          \
        }                              \
    },                                 \
    0u,                                \
    0u                                 \
}

/**
 * Enumerator denoting status of Sensor Calibration data block
 */
typedef enum
{
    sensor_cal_data_ok, /**< data has been read and found valid*/
    sensor_cal_data_invalid_crc, /**< data hasn't passed CRC check */
    sensor_cal_data_wrong_format, /**< Other error has occurred during data reading */
    sensor_cal_data_not_checked_yet /**< data hasn't been read yet */
} sensor_cal_data_status_t;

#endif /* SENSOR_CAL_TYPES_H_ */
