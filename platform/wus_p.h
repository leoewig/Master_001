#if !defined(WUS_P_H_)
#define WUS_P_H_

/**
 * @file
 * @ingroup wus
 *
 * Wake-up source latch private header
 *
 * $Id: wus_p.h 8829 2013-06-06 13:52:16Z vs7 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/** Wake-up source latch update period in milliseconds */
#define WUS_UPDATE_PERIOD  ((ts_time_t)300)
/** System wake up bit mask in byte read from PSU latch */
#define WUS_SYSTEM_MASK            (0x20u)
/** Charger control pilot bit mask in byte read from PSU latch */
#define WUS_CHG_CONTROL_PILOT_MASK (0x08u)
/** Ignition (BMS enable) bit mask in byte read from PSU latch */
#define WUS_IGNITION_MASK          (0x02u)
/** RTC wake up bit mask in byte read from PSU latch */
#define WUS_RTC_MASK               (0x04u)
/** CC wake up bit mask in byte read from PSU latch */
#define WUS_CC_MASK               (0x10u)

/******************************************************************************
 * PRIVATE TYPE DEFINITIONS
 *****************************************************************************/

/**
 * Enumeration of wake-up source latch state machine's states
 */
enum wus_state_e
{
    wus_state_idle,         /**< idle state, read immediately, if reading
                                for the first time, or wait until update period
                                expires */
    wus_state_update,       /**< read psu latch data */
    wus_state_strobe_set,   /**< enable strobe signal */
    wus_state_strobe_reset  /**< disable strobe signal */
};

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

PRIVATE struct wus_s wus_parse_psu_data(uint8_t latch_data);
PRIVATE struct wus_s wus_read_sources(void);
PRIVATE void wus_st_idle(void);
PRIVATE void wus_st_strobe_set(void);
PRIVATE void wus_st_strobe_reset(void);
PRIVATE void wus_st_update(void);

#endif /* WUS_P_H_ */
