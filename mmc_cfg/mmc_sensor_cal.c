/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_sensor_cal.c
 *
 * description:  
 * MMC Sensor calilbration data module
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
#include "fm.h"
#include "mmc_sensor_cal.h"
#include "sensor_cal.h"

/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/

/**
 * MMC End-of-line data structure mapped to absolute location.
 *
 */
/*lint -esym(728, mmc_sensor_cal )*/
#ifndef HOSTED
#pragma section @gpage @far const {sensor_cal}
#endif
PRIVATE FROM sensor_cal_t mmc_sensor_cal = SENSOR_CAL_DUMMY_INITIALISER ;
/**< NB This initialiser will not end up in the binary because the linker is
 * configured to leave the .sensor calibration section uninitialised.  However, if we leave it
 * out, the linker insists on ignoring the \#pragma section directive
 * regardless of whatever permutation of syntax I try
 */

#ifndef HOSTED
#pragma section @gpage @far const []
#endif

PRIVATE FROM sensor_cal_t mmc_default_sensor_cal = SENSOR_CAL_DEFAULT_INITIALISER ;

/**
 * Pointer to the Sensor Calibration data. If the unit has been correctly programmed then
 * this will point to #mmc_sensor_cal, otherwise it will point to #mmc_default_sensor_cal
 */
FROM sensor_cal_t * mmc_sensor_cal_p = &mmc_sensor_cal;

/**
 * Status of #mmc_sensor_cal. See definition of #sensor_cal_data_status_t for details.
 */
sensor_cal_data_status_t mmc_sensor_cal_data_status = sensor_cal_data_not_checked_yet;

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/**
 * Read sensor calibration data from dedicated memory location and check its consistency
 * and compatibility
 */
void mmc_init_sensor_cal(void)
{
    bool_t fault_present = FALSE;
    mmc_sensor_cal_data_status = check_sensor_cal((FPTR CONST sensor_cal_t * )&mmc_sensor_cal);

    if (sensor_cal_data_ok != mmc_sensor_cal_data_status)
    {
        /* Adjust the pointer to use the default calibration */
        mmc_sensor_cal_p = &mmc_default_sensor_cal;

        fault_present = TRUE;
    }

    (void) fm_update_fault_filter(Fm_plat_mmc_missing_sensor_cal_data,
                                Fm_general_failure_information,
                                fault_present,
                                TRUE);
}
