/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    wus_types.h
 *
 * description:   Wake-up source latch types
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li            Wake-up source latch types
 *                       
 *
 *****************************************************************************/

#if !defined(WUS_TYPES_H_)
#define WUS_TYPES_H_

/**
 *
 * @file
 * @ingroup wus
 *
 * Wake-up source latch types
 *
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
/**
 * Wake-up source latch status enumerator
 */
enum wus_status_e
{
    wus_status_not_read_yet,   /**< Wake-up source latch data hasn't been
                                    read yet */
    wus_status_valid       /**< Wake-up source latch data has been
                                    read successfully and are ready to
                                    collect */
};

/**
 * Structure type containing states of all wake-up source signals
 */
struct wus_s
{
    bool_t ignition;            /**< Ignition wake-up source state */
    bool_t system;              /**< System wake-up source state */
    bool_t rtc;                 /**< RTC wake-up source state */
    bool_t chg_control_pilot;   /**< Charger control pilot wake-up source state */
	bool_t cc_wkup_mmc;         /**< CC wake-up */
};

#endif /* WUS_TYPES_H_ */
