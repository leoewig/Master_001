/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     platform.h
 *
 * description:  Header file to draw together all platform definitions
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
/**
 * @file
 *
 * Header file to draw together all platform definitions
 *
 */
/**
 *
 * Interface to the microcontroller
 *
 * The platform feature consists of multiple subfeatures which each provide an
 * interface to a peripheral on the microcontroller.
 *
 * @defgroup platform Platform
 * @{
 * @file
 * @}
 */

#ifndef PLATFORM_H
#define PLATFORM_H

//#include "ican.h"
#include "pit.h"
#include "spi.h"
#include "platform_types.h"

/**
 * Initialise function for all platform-level functionality.
 * This will call the _init() functions of all drivers etc.
 *
 * @todo add the rest of the drivers here
 * Initialise general-purpose divers for various micro features. The following
 * features are intentionally omitted.:
 *    - Extenal CAN - handled by external Library
 *    - I2C implicitly called by RTC
 *    - ISA - not currently supported */

void FCODE drivers_init(void);
void FCODE drivers_read_inputs(void);

#endif /* PLATFORM_H */
