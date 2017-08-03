/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     ts.h
 *
 * description:  
 * This file contains all of the macro definition and enumerated types used
 * by the Temperature Measurement Input feature
 * 
 * 
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Header file for timer services feature
 *                       
 *
 *****************************************************************************/
#if !defined(TMI_TYPES_H)
#define      TMI_TYPES_H
/**
 *
 * This file contains all of the macro definition and enumerated types used
 * by the Temperature Measurement Input feature
 *
 * @file
 * @ingroup tmi
 */


/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

#define LOOKUP_SIZE_BOSCH 176
//#define LOOKUP_SIZE_PT    126
#define LOOKUP_SIZE_PT    191
#define LOOKUP_SIZE_NCP   126

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

struct tmi_sensor_data_s
{
    CONST volatile uint16_t * sensor_address;
    data_validity_e         * validity;
};

/**
 * tmi_sensor_no_e enumeration to allow access for individual sensors into the
 * various data structures of this feature
 */
enum tmi_sensor_no_e
{
    Tmi_sensor_1 = 0,
    Tmi_sensor_2,
    Tmi_sensor_3,
    Tmi_sensor_4,
    Tmi_last_sensor
};

/**
 * tmi_sensor_brand_e enumeration of the various makes of temperature sensors
 * that can be attached to each of the possible inputs.
 * @note changes to this enum must be mirrored in the ASAP2 COMPU_VTAB in
 * tmi.a2l
 *
 */
enum tmi_sensor_brand_e
{
    Tmi_sensor_none = 0,
    Tmi_sensor_Bosch,
    Tmi_sensor_PT1000,
    Tmi_sensor_NCP
};

/**
 * tmi_state_e enumeration of the modes of operation of the TMI feature
 */
enum tmi_state_e
{
    Tmi_settling,
    Tmi_running
};

#endif      /* TMI_TYPES_H */

