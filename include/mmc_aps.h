/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_aps.h
 *
 * description:  Application services public header file.
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
#if !defined(APS_H)
#define      APS_H
/**
 *
 * Application services public header file.
 *
 * @file
 * @ingroup APS
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 *
 * APS Overview
 *
 * ## How to use #
 * Call into this feature for the target-specific startup code for the portions
 * of functionality that are shared.
 *
 */

/**
 * @defgroup APS APS Application Services */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "scheduler.h"
#include "aps_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/**
 * The timestamp of the most recent foreground task start.
 * This is of general interest to most foreground tasks, so it is placed here*/
extern ts_time_t aps_fg_start_ms;

/**
 * The timestamp of the most recent background task start.
 * This is of general interest to most foreground tasks, so it is placed here*/
extern ts_time_t aps_bg_start_ms;

/**
 * The timestamp of the most recent background task start.
 * This is of general interest to most foreground tasks, so it is placed here*/
extern ts_time_t aps_nvs_start_ms;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/* Entry point is called from crt startup */
FCODE     void platform_main(void);
FCODE     void aps_run_bg_task(void);
bool_t    aps_is_bg_active(void);

INTERRUPT void isr_pit_fgloop(void);
void ad_pit_init(void);
INTERRUPT void ad_pit_interrupt(void);

#endif      /* APS_H */

