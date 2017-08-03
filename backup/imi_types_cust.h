/**
 *
 * Current measurement interface types
 *
 * @file
 * @ingroup imi
 * $Id: imi_types_cust.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2015 - All rights reserved */

#if !defined( IMI_TYPES_CUST_H )
#define       IMI_TYPES_CUST_H

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
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


#endif /* IMI_TYPES_CUST_H */
