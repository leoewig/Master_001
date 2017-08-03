/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   mmc_sensor_cal.h
 *
 * description: Main state module header file
 *    
 *      
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(MMC_SENSOR_CAL_H_)
#define MMC_SENSOR_CAL_H_

/**
 * MMC Sensor Calibration data module header
 *
 * @file
 * @ingroup mmc_sensor_cal
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 * Sensor Calibration data module
 *
 * Overview
 * ========
 * This module provides access to the sensor calibration data.
 *
 * Approach used
 * =============
 * Sensor Calibration data is located at dedicated location in flash memory defined in chapter 3.3.2 of
 * MMC HWSW interface specification.
 *
 * Integration
 * ===========
 * -Include sensor_cal.h
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "sensor_cal_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern sensor_cal_data_status_t mmc_sensor_cal_data_status;

extern FROM sensor_cal_t * mmc_sensor_cal_p;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
void mmc_init_sensor_cal(void);

#endif /* MMC_SENSOR_CAL_H_ */
