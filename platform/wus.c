/**
 * @file
 * @ingroup wus
 *
 * Wake-up source latch implementation
 *
 * $Id: wus.c 9349 2013-07-09 08:43:23Z pr4 $
 */

/* Copyright (c) Linkedriving Tech, 2012 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#include "wus.h"
#include "wus_p.h"
#include "ts.h"
#include "dio_cfg.h"
#include "spi.h"

/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/

/**
 * Status of wake-up source latch feature
 */
enum wus_status_e wus_status = wus_status_not_read_yet;

/**
 * Original wake-up source(s)
 */
struct wus_s wus_original;
/**<
 * @a2l{.ignition, conversion, bool_t}
 * @a2l{.ignition, LongIdentifier, Ignition wake-up}
 * @a2l{.system, conversion, bool_t}
 * @a2l{.system, LongIdentifier, System wake-up}
 * @a2l{.rtc, conversion, bool_t}
 * @a2l{.rtc, LongIdentifier, RTC wake-up}
 * @a2l{.chg_control_pilot, conversion, bool_t}
 * @a2l{.chg_control_pilot, LongIdentifier, Charger control pilot wake-up}
 */

/**
 * Most recent wake-up source(s)
 */
struct wus_s wus_recent;
/**<
 * @a2l{.ignition, conversion, bool_t}
 * @a2l{.ignition, LongIdentifier, Ignition wake-up}
 * @a2l{.system, conversion, bool_t}
 * @a2l{.system, LongIdentifier, System wake-up}
 * @a2l{.rtc, conversion, bool_t}
 * @a2l{.rtc, LongIdentifier, RTC wake-up}
 * @a2l{.chg_control_pilot, conversion, bool_t}
 * @a2l{.chg_control_pilot, LongIdentifier, Charger control pilot wake-up}
 */

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

/**
 * State of wake up source latch state machine
 */
PRIVATE enum wus_state_e wus_state = wus_state_idle;


/**
 * Time stamp of the last wake-up source latch reading event
 */
PRIVATE ts_time_t wus_timestamp;

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/**
 * Returns wake-up source latch feature status
 *
 * @return wake-up source latch feature status
 */
enum wus_status_e wus_get_status(void)
{
    return wus_status;
}

/**
 * Wake up source latch state machine
 */
void wus_update(void)
{
    switch (wus_state)
    {
        case wus_state_idle:
            wus_st_idle();
            break;
        case wus_state_strobe_set:
            wus_st_strobe_set();
            break;
        case wus_state_strobe_reset:
            wus_st_strobe_reset();
            break;
        case wus_state_update:
            wus_st_update();
            break;
        default:
            /* this is rather unexpected */
            INTERNAL_ERROR();
            break;
    }
}

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/**
 * Parse data read from PSU latch and return it packed in #wus_s structure
 *
 * @param latch_data    is binary data byte as read from the PSU latch
 * @return              #wus_s type structure written with parsed
 *                      data from PSU latch
 */
PRIVATE struct wus_s wus_parse_psu_data(uint8_t latch_data)
{
    struct wus_s result;
    result.system = ((latch_data & WUS_SYSTEM_MASK)
        == WUS_SYSTEM_MASK) ? TRUE : FALSE;
    result.chg_control_pilot = ((latch_data & WUS_CHG_CONTROL_PILOT_MASK)
        == WUS_CHG_CONTROL_PILOT_MASK) ? TRUE : FALSE;
    result.ignition = ((latch_data & WUS_IGNITION_MASK)
        == WUS_IGNITION_MASK) ? TRUE : FALSE;
    result.rtc = ((latch_data & WUS_RTC_MASK)
        == WUS_RTC_MASK) ? TRUE : FALSE;
	result.cc_wkup_mmc = ((latch_data & WUS_CC_MASK)== WUS_CC_MASK) ? TRUE : FALSE;
    return result;
}

/**
 * Read serial PSU latch data and parse it
 *
 * @return      #wus_s type structure written with parsed
 *              data from PSU latch
 */
PRIVATE struct wus_s wus_read_sources(void)
{
 //   uint8_t latch_data;
 //   spi_transfer(Spi_psu_latch, (FPTR uint8_t *) &latch_data, FNULL, 1u);
 //   return wus_parse_psu_data(latch_data);
}

/**
 * Perform idle state of wake-up latch state machine. If data hasn't been read yet,
 * read it immediately by entering #wus_state_update state, otherwise wait
 * for #WUS_UPDATE_PERIOD time period to expire. If data has been read already,
 * manipulate strobe signal prior reading by entering #wus_state_strobe_set state.
 */
PRIVATE void wus_st_idle(void)
{
    if(wus_status == wus_status_not_read_yet)
    {
        wus_state = wus_state_update;
    }
    else if (ts_is_elapsed_ms(ts_get_ms(), wus_timestamp, WUS_UPDATE_PERIOD))
    {
        wus_state = wus_state_strobe_set;
    }
    else
    {
        /* MISRA */
    }
}

/**
 * Perform strobe set state of wake-up latch state machine. Set strobe signal
 * of power supply latch to the active level and enter #wus_state_strobe_reset state.
 */
PRIVATE void wus_st_strobe_set(void)
{
    do_psu_ltch_str = TRUE;
    wus_state = wus_state_strobe_reset;
}

/**
 * Perform strobe wait state of wake-up latch state machine. Set strobe signal
 * of power supply latch to the inactive level and enter #wus_st_update state.
 * @note There's no need to wait prior reseting the strobe signal, since minimal strobe
 * pulse duration is insignificant compared to foreground period.
 */
PRIVATE void wus_st_strobe_reset(void)
{
    do_psu_ltch_str = FALSE;
    wus_state = wus_state_update;
}

/**
 * Perform update state of wake-up latch state machine. Read the data from PSU latch
 * and store it either to the #wus_recent structure, if data has been read at least
 * once, or to the #wus_recent and #wus_original, if data hasn't been read yet.
 */
PRIVATE void wus_st_update(void)
{
    struct wus_s src;

    src = wus_read_sources();
    if(wus_status == wus_status_not_read_yet)
    {
        wus_original = src;
    }
    wus_recent = src;
    wus_status = wus_status_valid;
    wus_state = wus_state_idle;
    wus_timestamp = ts_get_ms();
}
