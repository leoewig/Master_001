/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   i2c_types.h
 *
 * description:  Header file for internal CAN bus driver
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
#if !defined( I2C_TYPES_H )
#define       I2C_TYPES_H
/**
 * @file
 *
 * Header file for low-level I2C control
 *
 */

/**
 * Enumeration to denote the I2C specific cycle status in I2C communication.
 */
typedef enum
{
    I2c_not_complete,
    I2c_success,
    I2c_failure
} I2C_ProgressType;

/**
 * Enumeration to denote the I2C bus speed type
 */
typedef enum
{
    I2C_100,    /**< Standard Mode operating at 100Khz cycle rate. */
    I2C_400     /**< Fast Mode operating at 400Khz cycle rate. */
} I2C_SpeedType;

/**
 * Enumeration to provide a mechanism to identify the device which uses the I2C services.
 * Since only RTC is using the Services of the I2C driver, hence only the
 * RTC driver handle exists. Any other feature using the I2C services must first
 * register their name into this enum between the last enumerator value and
 * I2C_MAX_DEV.*/
typedef enum
{
    I2C_RTC_USER = 0,    /**< Device Index for RTC Device */
    I2C_MAX_DEV
} I2CDeviceIndex_t;

/**
 * Structure contining attributes to configure the I2C device.*/
typedef struct
{
    I2CDeviceIndex_t    dev_indx;  /**< Device index */
    uint16_t            address;   /**< Device address */
    I2C_SpeedType       bus_speed; /**< Device requested bus speed */
} I2CDevice_t;




#endif /* I2C_TYPES_H */
