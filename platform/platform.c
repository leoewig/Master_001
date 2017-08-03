/**
 *
 * Invoker for general-purpose platform functionality
 *
 * Additional file documentation if necessary
 *
 * @file
 * @ingroup platform
 * $Id: platform.c 10972 2013-10-17 11:00:55Z pr4 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#include <basic_types.h>
#include "platform.h"     /* public header for this module */

/*****************************************************************************
 * Headers for drivers
 *****************************************************************************/

#include "ai.h"
#include "spi.h"
#include "eeprom.h"
//#include "ican.h"
#include "dio.h"
#include "pit.h"
#include "nvs_cfg.h"
#include "nvs.h"
#include "pit.h"
#include "rtc.h"
//#include "isa.h"
#include "cal.h"
//#include "cal_offline.h"
#include "fm.h"
//#include "obd.h"
#include "mmc_eol.h"
#include "mmc_sensor_cal.h"
#include "wus.h"

/******************************************************************************
 * COMPILE-TIME CHECKS
 *****************************************************************************/


/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/


/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 *****************************************************************************/

PRIVATE FCODE void initialise_nvs_features(void);

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/** This function ensures that all of the features required by the NVS driver
 *  have been initialised prior to te initialisation of the NVS driver. If any
 *  of these features are not initialised at this point then the NVS driver will
 *  not have access to the EEPROM device and will cause the system to reset
 *  over and over again.
 */
PRIVATE FCODE void initialise_nvs_features()
{
    /* Periodic interval timer */
    PitInit();

    /* Initialize timer services */
    ts_init();

    /* Initialise serial peripheral interface */
    spi_init();

    (void) eeprom_init();
    nvs_init();
}

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/* documentation with prototype in header file */
FCODE void drivers_init(void)
{
    /* NVS related features */
    initialise_nvs_features();

    /* read a calibration out of EEPROM if it exists and matches the strategy */
    cal_init();

    fm_init();  /* Failure manager */

    /* Now that FM is running, we can log any NVS faults detected above */
    nvs_init_failure_management();
    cal_init_failure_management();

    //cal_offline_init();

    /* read the sesnor calibration data because all ADC calculatiobns need it */
    //mmc_init_sensor_cal();

    /* Digital I/O */
    dio_init();

    /* Analogue inputs */
    ai_init();

    /* Realtime clock, this initialises the I2C device itself, so do not
     * initialise I2C here. */
    rtc_init();

    /* NB As some of above need to update the outputs, we must actually write outputs,
     * not just update the variables */
    dio_write_outputs();
}

/**
 * Invoke all the driver level input functions that are shared across all
 * applications */

FCODE void drivers_read_inputs(void)
{
   /* Driver inputs */
    //dio_read_inputs();
    rtc_update(); /**< rtc_read_inputs is incorrectly named */
}

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

