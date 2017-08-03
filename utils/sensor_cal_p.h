/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   shift_helpers.c
 *
 * description:  Sensor Calibration data module private header
 * 
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/
#if !defined(SENSOR_CAL_P_H_)
#define SENSOR_CAL_P_H_

/**
 * Sensor Calibration data module private header
 *
 * @file
 * @ingroup mmc_sensor_cal
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

/******************************************************************************
 * PRIVATE MACRO DEFINITIONS
 *****************************************************************************/

/**
 * EOL validity marker value
 */
#define SENSOR_CAL_VALIDITY_MARKER    (0x9BA5EDFEu)

/**
 * Supported sensor_cal data layout version
 */
#define SENSOR_CAL_SUPPORTED_DATA_LAYOUT_VERSION        (3u)

/**
 * Sensor_cal Padding value
 */
#define SENSOR_CAL_PADDING        (0xffu)

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *****************************************************************************/

#endif /* SENSOR_CAL_P_H_ */
