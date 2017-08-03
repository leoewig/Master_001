/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   sensor_cal.h
 *
 * description:  Sensor Calibration data module header
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
#if !defined(SENSOR_CAL_H_)
#define SENSOR_CAL_H_

/**
 * Sensor Calibration data module header
 *
 * @file
 * @ingroup mmc_sensor_cal
 * $Id: sensor_cal.h 8269 2013-04-22 14:06:47Z tg10 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "sensor_cal_types.h"
#include "hardware.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
sensor_cal_data_status_t check_sensor_cal(FPTR CONST sensor_cal_t * sensor_cal);

#endif /* SENSOR_CAL_H_ */
