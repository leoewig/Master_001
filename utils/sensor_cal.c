/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   sensor_cal.c
 *
 * description:  Sensor Calibration data module
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


/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "crc.h"
#include "sensor_cal.h"
#include "sensor_cal_p.h"
#include "strings.h"

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/


/**
 * Check MMC Sensor Calibration data consistency and compatibility
 *
 * @param[in] sensor_cal   pointer to data structure to be checked
 * @return  sensor_cal_data_ok    if compatible, consistent and ready for use
 * @return  sensor_cal_data_wrong_format   if one or more data members are in wrong or
 *                          unsupported format
 * @return  sensor_cal_data_invalid_crc if CRC16-CITT checksum doesn't match calculated one
 *
 */
sensor_cal_data_status_t check_sensor_cal(FPTR CONST sensor_cal_t * sensor_cal)
{
    sensor_cal_data_status_t res = sensor_cal_data_ok;

    if ((sensor_cal->calibration_validity_marker_1 != SENSOR_CAL_VALIDITY_MARKER)
        || (sensor_cal->calibration_validity_marker_2 != SENSOR_CAL_VALIDITY_MARKER))
    {
        /* validity marked is of wrong value */
        res = sensor_cal_data_wrong_format;
    }

    if ((res == sensor_cal_data_ok)
        && (sensor_cal->data_layout_version != SENSOR_CAL_SUPPORTED_DATA_LAYOUT_VERSION))
    {
        /* unsupported data layout version */
        res = sensor_cal_data_wrong_format;
    }
    if ((res == sensor_cal_data_ok) && (sensor_cal->padding != SENSOR_CAL_PADDING))
    {
        /* padding is of wrong value */
        res = sensor_cal_data_wrong_format;
    }

    if (res == sensor_cal_data_ok)
    {
        /* Check CRC. Exclude validity markers and crc field itself. */
        /** @polyspace<MISRA-C:11.4:Not a defect:Justify with annotations> Casting is safe,
         * no alignment problem is expected, since sensor_cal object is aligned to uint32_t type */
        if (sensor_cal->checksum
            != crc16(CRC16_SEED, &(((FPTR CONST uint8_t *) sensor_cal)[sizeof(sensor_cal->calibration_validity_marker_1)]),
                sizeof(sensor_cal_t)
                    - (sizeof(sensor_cal->calibration_validity_marker_1) + sizeof(sensor_cal->calibration_validity_marker_2)
                        + sizeof(sensor_cal->checksum))))
        {
            res = sensor_cal_data_invalid_crc;
        }
    }

    return res;
}

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
