/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   imi_types.h
 *
 * description: Header file for Current Measurement Interface
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
#if !defined( IMI_TYPES_H )
#define       IMI_TYPES_H
/**
 * Header file for Current Measurement Interface
 *
 * @file
 * @ingroup imi
 */

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
/**
 * Enumeration to denote the IMI module initilization state whether or not it is
 * initialised.
 * @note changes to this enum must be mirrored in the respective ASAP2 COMPU_VTAB
 * in imi.a2l
 */
enum Imi_state_e
{
    Imi_state_uninitialised, /**< IMI module is uninitialised. */
    Imi_state_initialising, /**< IMI module is waiting for ISA to initialise. */
    Imi_state_running /**< IMI module is initialised & running. */
};

/**
 * Enumeration to denote the Current sensor present in the battery pack.
 */
enum Imi_sensor_type_e
{
   Imi_lem_sensor,
   Imi_isa_sensor,
   Imi_multiple_sensor,
   Imi_last_sensor /* keep this as the last element */
};

/**
 * Enumeration to denote, whether the current on the HV bus is zero or not, and
 * its validity.
 * @note changes to this enum must be mirrored in the respective ASAP2 COMPU_VTAB
 * in imi.a2l
 */
enum imi_hv_bus_zero_curr_status_e
{
    Imi_hv_zero,     /**< current on the HV bus is zero */
    Imi_hv_non_zero, /**< current on the HV bus is non-zero */
    Imi_hv_invalid   /**< latest measured current on the HV bus is invalid */
};

/**
 * Enumeration to denote the validity of the integrated current
 * @note changes to this enum must be mirrored in the respective ASAP2 COMPU_VTAB
 * in imi.a2l
 */
enum imi_integral_validity_e
{
    Imi_all_data_valid,    /**< all sensor readings were valid in measurement cycle */
    Imi_some_data_invalid, /**< not all sensor readings were valid in measurement cycle */
    Imi_all_data_invalid   /**< all sensor readings invalid in measurement cycle (or invalid pointer) */
};

/**
 * Structure containing the most recent current read from the sensor, the average
 * of the previous and the most recent current, and data quality of the most
 * recent read current.
 */

struct Imi_latest_data_s
{
    current_t       latest_mA;          /**< Most recent current value in mA.*/
    data_validity_e latest_mA_validity; /**< Most recent current data quality indicator */
    current_t       latest_error_mA;    /**< Measurement error for the reading
                                             in latest_mA */
    bool_t          overflow;           /**< Flag to indicate that the sensor
                                             has overflown its measurement
                                             range */
};
#endif /* IMI_TYPES_H */
