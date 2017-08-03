/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    isa_types.h
 *
 * description: Public header for Isabellenhuette feature
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
#ifndef ISA_TYPES_H
#define ISA_TYPES_H
/*************************************************************************//**
 *
 * Public header for Isabellenhuette feature
 *
 *
 * Copyright (c) Linkedriving Tech 2012 - All rights reserved
 *
 * @file
 *
 *****************************************************************************/

#define ISA_CURRENT_VALID   (isa_current_valid) /**< Indicates the validity of #isa_current_ma */
#define ISA_SERIAL_NUM_SIZE 4u                  /**< Number of bytes of sensor serial number */

/****************************************************************************
 * Options for faults while ISA sensor is running
 *****************************************************************************/
typedef enum
{
    Isa_fs_none = 0u, /**< No fault detected */
    Isa_fs_present,   /**< Fault present but not tripped */
    Isa_fs_timeout,   /**< Message timeout but not tripped*/
    Isa_fs_tripped    /**< Fault tripped */
} isa_fault_status_e;

/**************************************************************************//**
 * Options for #isa_init_state
 *****************************************************************************/
typedef enum {
    Isa_st_ver_check,          /**< ISA SW Version check */
    Isa_st_stop,               /**< Stopping the ISA sensor */
    Isa_st_config,             /**< Configuration check */
    Isa_st_correct_config,     /**< Correcting Configuration */
    Isa_st_serial_number,       /**< Obtaining serial number  */
    Isa_st_save_config,        /**< Saving new config */
    Isa_st_start,              /**< Starting the ISA sensor */
    Isa_st_calibration,        /**< Calibrating ISA sensor */
    Isa_st_init_complete       /**< Sensor initialization is complete */

} isa_init_state_e;

typedef enum
{
    Isa_st_power_on,    /**< Waiting for sensor to power up */
    Isa_st_sensor_init, /**< Initializing Sensor            */
    Isa_st_measuring,   /**< ISA Measuring Current          */
    Isa_st_power_off,   /**< ISA Powered off                */
    Isa_max_states      /**< Marker for max number of states*/
} isa_main_state_e;


#endif