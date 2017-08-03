/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     wus.h
 *
 * description:   Wake-up source latch header file
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Wake-up source latch header file
 *                       
 *
 *****************************************************************************/
#if !defined(WUS_H_)
#define WUS_H_

/**
 * @file
 * @ingroup wus
 *
 * Wake-up source latch header file
 *
 * $Id: wus.h 9349 2013-07-09 08:43:23Z pr4 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 * Wake-up source latch feature
 *
 * Overview
 * --------
 * Wake-up source latch feature identifies a signal, which has waken up the unit and provide
 * this information to the application. It also provides state of wake-up sources in the runtime.
 * Wake-up sources are read from PSU latch unit, which is based on 74hc165 IC.
 * It's a shift register with a latch, which may be read using SPI peripheral.
 *
 * Approach used
 * -------------
 * Wake-up source latch is based on single state machine performing following tasks:
 * - read the original wake-up source after startup and provide this information
 * - read state of wake-up sources in the runtime every update period
 *
 * Limitations
 * -----------
 * Data from PSU latch are read using SPI peripheral. Since clock signal is pulled-down
 * to the ground and low to high edge of the clock signal shifts the data in the latch,
 * it's not possible to set SPI clock polarity and phase in such a way to make reading
 * of whole byte from the latch possible and we're therefore losing the MSB bit.
 * Fortunately MSB bit is not used and it should stay like this in future hardware revisions.
 * Bit masks for each signal in PSU latch data byte are shifted left by one to deal with this,
 * e.g. WUS_IGNITION_MASK equals to 0x02, although the ignition signals is actually connected
 * to D0 input of PSU latch shift register.
 *
 * Configuration
 * -------------
 * - configure SPI peripheral, which is connected to the PSU latch as follows:
 *      - master mode
 *      - clock polarity - active high. An idle state is low (CPOL = 0)
 *      - clock phase - sampling of data occurs at even edges (CPHA = 1)
 *
 * Integration and Usage
 * ---------------------
 * - include wus.h
 * - call wus_update() periodically
 * - read #wus_status to get status of wus feature and since it
 *  equals to #wus_status_valid
 *      - read #wus_original to get original wake-up source(s)
 *      - read #wus_recent to get recent wake-up source(s)
 *
 * Requirement Satisfaction
 * ------------------------
 *
 * | Requirement          | Satisfied By            | Tested By     |
 * | ---------------------|-------------------------|---------------|
 * | REQ_BMS_SUP_WUS_001  | #spi_cfg[Spi_psu_latch], #dio_output_cfg[]| pat_wus_basic() |
 * | REQ_BMS_SUP_WUS_002  | drivers_read_inputs()   | pat_wus_basic() |
 * | REQ_BMS_SUP_WUS_003  | #wus_original           | pat_wus_basic() |
 * | REQ_BMS_SUP_WUS_004  | wus_update()            | pat_wus_basic() |
 * | REQ_BMS_SUP_WUS_005  | #wus_recent             | pat_wus_basic() |
 * | REQ_BMS_SUP_WUS_006  | wus_st_update()         | pat_wus_basic() |
 * | REQ_BMS_SUP_WUS_007  | #wus_status             | pat_wus_basic() |
 *
 *
 * @defgroup wus Wake-up source latch
 * @ingroup platform
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "wus_types.h"

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
extern struct wus_s wus_original;
extern struct wus_s wus_recent;
extern enum wus_status_e wus_status;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
enum wus_status_e wus_get_status(void);
void wus_update(void);

#endif /* WUS_H_ */
