/**
 * @file
 * @ingroup rtc
 *
 * Routines for initialising and controlling the Real Time Clock.
 *
 * $Id: rtc.c 11919 2014-03-25 16:17:37Z pr4 $
 * RTC Driver Implementation
 */
/* Copyright (c) Linkedriving Tech, 2012 */
#include "basic_types.h"
#include "hardware.h"

#if defined MMC
#include "fm.h"
#endif

#include "i2c.h"
#include "rtc.h"
#include "rtc_p.h"
#include "ms.h"
#include "wus.h"
//#include "umc.h"
#include "nvs.h"

/******************************************************************************
 * CALIBRATABLES
 *****************************************************************************/

#define MEMORY_LOCATION CALIBRATION_MEMORY_S12
#include "memory_layout.h"

PRIVATE CAL uint8_t rtc_number_of_retries = 5u;
/**<
 * @a2l{conversion,uint8_t}
 * @a2l{longidentifier,Number of I2C transaction retries to be attempted before
 * rtc feature gives up and raises fault}
 */

#define MEMORY_LOCATION GENERAL_MEMORY
#include "memory_layout.h"

/******************************************************************************
 * PRIVATE VARIABLE DEFINITIONS
 *****************************************************************************/
/**
 * constant data structure for each rtc command
 */
PRIVATE CONST i2c_req_t i2c_request[] = {
/*rtc command           txb                start address data          rxb             comment */
{Pca_read_device,       RTC_REQUEST_LEN,   PCA8565_DATA_START_ADDRESS, RTC_READ_LEN}, /* read device, obtain time and status info */
{Pca_set_time,          RTC_TIME_SET_LEN,  PCA8565_TIME_START,         0u},           /* write timer registers */
{Pca_set_alarm,         RTC_ALARM_SET_LEN, PCA_ALARM_MIN,              0u},           /* write alarm registers */
{Pca_disable_alarm,     RTC_ALARM_SET_LEN, PCA_ALARM_MIN,              0u},           /* write alarm registers */
{Pca_set_up,            RTC_RESET_INIT,    PCA_CLKOUT_CONTROL,         0u},           /* clk out and timer ctr registers */
{Pca_set_interrupt,     RTC_IRQ_SET_LEN,   PCA_CTR2,                   0u},           /* set irq request */
{Pca_clear_interrupt,   RTC_IRQ_CLEAR_LEN, PCA_CTR2,                   0u},           /* clear irq request and interrupt flag */
};

/**
 * Number of seconds in past months excluding the actual one. Index shall
 * be 1 - 12 (1 for January, 12 for December)
 */
PRIVATE CONST uint32_t total_seconds_to_month[] =
{
    0u, /* dummy so that jan starts at index 1 rather than 0 */
    0u,
    SECONDS_IN_JAN,
    SECONDS_IN_JAN + SECONDS_IN_FEB,
    SECONDS_IN_JAN + SECONDS_IN_FEB + SECONDS_IN_MAR,
    SECONDS_IN_JAN + SECONDS_IN_FEB + SECONDS_IN_MAR + SECONDS_IN_APR,
    SECONDS_IN_JAN + SECONDS_IN_FEB + SECONDS_IN_MAR + SECONDS_IN_APR + SECONDS_IN_MAY,
    SECONDS_IN_JAN + SECONDS_IN_FEB + SECONDS_IN_MAR + SECONDS_IN_APR + SECONDS_IN_MAY
        + SECONDS_IN_JUN,
    SECONDS_IN_JAN + SECONDS_IN_FEB + SECONDS_IN_MAR + SECONDS_IN_APR + SECONDS_IN_MAY
        + SECONDS_IN_JUN + SECONDS_IN_JUL,
    SECONDS_IN_JAN + SECONDS_IN_FEB + SECONDS_IN_MAR + SECONDS_IN_APR + SECONDS_IN_MAY
        + SECONDS_IN_JUN + SECONDS_IN_JUL + SECONDS_IN_AUG,
    SECONDS_IN_JAN + SECONDS_IN_FEB + SECONDS_IN_MAR + SECONDS_IN_APR + SECONDS_IN_MAY
        + SECONDS_IN_JUN + SECONDS_IN_JUL + SECONDS_IN_AUG + SECONDS_IN_SEP,
    SECONDS_IN_JAN + SECONDS_IN_FEB + SECONDS_IN_MAR + SECONDS_IN_APR + SECONDS_IN_MAY
        + SECONDS_IN_JUN + SECONDS_IN_JUL + SECONDS_IN_AUG + SECONDS_IN_SEP + SECONDS_IN_OCT,
    SECONDS_IN_JAN + SECONDS_IN_FEB + SECONDS_IN_MAR + SECONDS_IN_APR + SECONDS_IN_MAY
        + SECONDS_IN_JUN + SECONDS_IN_JUL + SECONDS_IN_AUG + SECONDS_IN_SEP + SECONDS_IN_OCT
        + SECONDS_IN_NOV };

/**
 * Number of seconds in each month. Index is be 1 - 12 (1 for January, 12 for December)
 */
PRIVATE CONST uint32_t seconds_in_month[] =
{
    0u, /* dummy so that jan starts at index 1 rather than 0 */
    SECONDS_IN_JAN, SECONDS_IN_FEB, SECONDS_IN_MAR, SECONDS_IN_APR,
    SECONDS_IN_MAY, SECONDS_IN_JUN, SECONDS_IN_JUL, SECONDS_IN_AUG,
    SECONDS_IN_SEP, SECONDS_IN_OCT, SECONDS_IN_NOV, SECONDS_IN_DEC
};

/**
 * Number of days in each month. Index is be 1 - 12 (1 for January, 12 for December)
 */
PRIVATE CONST uint8_t days_in_month[] =
{
    0u, /* dummy so that jan starts at index 1 rather than 0 */
    DAYS_IN_JAN, DAYS_IN_FEB, DAYS_IN_MAR, DAYS_IN_APR,
    DAYS_IN_MAY, DAYS_IN_JUN, DAYS_IN_JUL, DAYS_IN_AUG,
    DAYS_IN_SEP, DAYS_IN_OCT, DAYS_IN_NOV, DAYS_IN_DEC
};

/**
 * Reference RTC timestamp. This is the time from which the RTC starts counting,
 * corresponding to 0h 0min 0s 1/1/2012.
 */
//PRIVATE const rtc_time_stamp_t rtc_ref_time = { 0u, 0u, 0u, 1u, 1u, 12u };
const rtc_time_stamp_t rtc_ref_time = { 0u, 58u, 20u, 26u, 5u, 16u };


/******************************************************************************
 * VARIABLES DEFINITIONS
 *****************************************************************************/

/**
 * structure containing current parameters for the RTC, see type for more details
 */
PRIVATE rtc_data_t      rtc_data;

/**
 * state variable for main rtc state machine
 */
PRIVATE rtc_states_t    rtc_state;

/**
 * state variable for transactions with pcs8565 chip
 */
PRIVATE rtc_i2c_trans_state rtc_i2c_state = pca_idle;

/**
 * state variable for initialisation state machine
 */
PRIVATE rtc_init_state_t rtc_init_state = Rtc_init_i2c;

/**
 * Generic time stamp holder used among the feature
 */
PRIVATE ts_time_t rtc_ts;

/**
 * last recorded seconds calculation from rtc. */
PRIVATE rtc_time_t previous_second;

/**
 * number of msec since we detected seconds were updated */
PRIVATE uint16_t rtc_unchanging_ms;

/**
 * Buffer used to store responses from the rtc.
 * Note, time data received/contained here is in bcd format. */
PRIVATE uint8_t i2c_rx_buff[RTC_TRANS_BUFF_SIZE];

/**
 * Buffer used to store data sent to the rtc.
 * Note, time data received/containued here is in bcd format. */
PRIVATE uint8_t i2c_tx_buff[RTC_TRANS_BUFF_SIZE];

/**
 * Timestamp used for timing i2c operations
 */
PRIVATE ts_time_t rtc_i2c_ts;

/**
 * State holder to store a state at which the I2C fault occurred. Used
 * to differentiate if either Fm_plat_rtc or Fm_plat_rtc_config fault codes
 * to raise.
 */
PRIVATE rtc_states_t rtc_i2c_fault_occurred_state;

/**
 * Flag signaling that I2C has occurred during the last i2c transaction
 */
PRIVATE bool_t rtc_i2c_fault_occurred;

/**
 * Retries counter for repeated I2C operations
 */
PRIVATE uint8_t rtc_i2c_retries;

/**
 * Flag informing the application, that the wake up source feature
 * has detected a woken up by rtc condition, but rtc feature failed to
 * found itself requesting rtc alarm.
 */
/**@todo remove lint comment once it'll get used elsewhere */
/*lint -esym(551, rtc_unknown_alarm_wake_up) */
PRIVATE bool_t rtc_unknown_alarm_wake_up = FALSE;

/**
 * Flag signaling that the set alarm request has been placed
 */
PRIVATE bool_t rtc_alarms_process_requested = FALSE;

/**
 * Flag signaling rtc alarm has been successfully processed
 */
PRIVATE bool_t rtc_alarms_processed_success = FALSE;

/**
 * Timestamp structure holding most recent alarm setting
 */
PRIVATE rtc_time_stamp_t rtc_alarm_to_set;

/**
 * Array of inhibit enumerators for each registered alarm
 * requester from the rtc_alarm.def configuration file, where each
 * requester must be listed using RTC_ALARM_REQUESTER macro with second
 * parameter specifying an inhibit enumerator. Each enumerator
 * is used prior shutting down in order to detect, that all registered
 * alarm requesters (features) are ready to shutdown and hence won't
 * request any more alarms.
 */
#define RTC_ALARM_REQUESTER(src, inhib)    inhib,
PRIVATE CONST enum ms_inhibits_e rtc_alarm_req_inhibs[Rtc_alarm_req_max] =
{
#include "rtc_alarm.def"
};
#undef RTC_ALARM_REQUESTER

/**
 * Array of alarm active flags for each registered requester
 * from rtc_alarm_req_e enumeration.
 */
PRIVATE bool_t rtc_alarm_active[Rtc_alarm_req_max];

/**
 * Array of flags signaling whether a feature (requester) has set an alarm
 * which woke up the mmc.
 */
PRIVATE bool_t rtc_wake_up_sources[Rtc_alarm_req_max];

/**
 * Maximal timeout from ms shutdown requestperiod when rtc is about to shutdown and alarm processing
 */
PRIVATE CONST ts_time_t rtc_shutdown_timeout = TS_SECONDS(1);

/**
 * Timestamp of shutdwon request
 */
PRIVATE ts_time_t rtc_shutdown_pending_ts;

/**
 * Last state of ms_shutdown_pending used for shutdown request detection
 */
PRIVATE bool_t rtc_ms_shutdown_pending_last = FALSE;

/**
 * Flag signaling that wake up sources has already been detected
 */
bool_t rtc_wake_ups_sources_deduced = FALSE;

/******************************************************************************
 * NVS DATA
 *****************************************************************************/

/**
 * Extract alarm initialization for each registered alarm requester
 * for the initial rtc_ram_image
 */
#define RTC_ALARM_REQUESTER(src, inhib)    0u,

/**
 * NVS structure for the RTC feature
 */
FRAM rtc_nvs_wrapper_s rtc_ram_image =
{
    NVS_HEADER_INIT(RTC_NVS_LAYOUT_VERSION),
    {
#include "rtc_alarm.def"
    },
    NVS_RESERVED_BYTES_INIT
};
#undef RTC_ALARM_REQUESTER

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/**
 * Initiate one of the i2c_comms_id command I2C transaction
 *
 * @param cmd   one of the i2c_comms_id commands to be performed
 */
PRIVATE void rtc_initiate_command(i2c_comms_id cmd)
{
    rtc_i2c_retries = rtc_number_of_retries;
    rtc_data.command = cmd;
    rtc_state = Rtc_process_cmd_state;
    RTC_START_I2C_STATE_MACHINE;
}

/**
 * A bcd to binary converter with validity and range check.
 *
 * @param[in]   in    input binary number to be converted
 * @param[in]   min   minimal value the output must be greater or equal to
 * @param[in]   max   maximal value the output must be less or equal to
 * @param[out]  out   a pointer to an output buffer
 * @return      FALSE if an input value is not valid bcd number,
 *                    or out of range. Output wasn't written.
 * @return      TRUE if the input value is valid bcd number and within
 *                              range. Converted input was written to the
 *                              output.
 */
PRIVATE bool_t bcd_to_bin(uint8_t in, uint8_t min, uint8_t max, uint8_t *out)
{
    bool_t res = FALSE;
    uint8_t low = in & 0x0fu;
    uint8_t high = (uint8_t) ((in >> 4) & 0x0fu);
    uint8_t temp;
    /* valid bcd nibbles? */
    if ((low < 10u) && (high < 10u))
    {
        temp = (uint8_t) (high * 10u) + low;
        /* within the range? */
        if ((temp >= min) && (temp <= max))
        {
            *out = temp;
            res = TRUE;
        }
    }
    return res;
}

/**
 * Check validity of number of days in provided time stamp
 *
 * @param   time    pointer to time structure to be checked
 * @return  TRUE    number of days are valid
 * @return  FALSE   invalid number of days
 */
PRIVATE bool_t rtc_number_of_days_valid(rtc_time_stamp_t const * time)
{
    bool_t res = TRUE;
    bool_t is_leap = IS_LEAP_YEAR(time->byears);
    if (is_leap && (time->bmonths == 2u))
    {
        if (time->bdays > (DAYS_IN_FEB + 1u))
        {
            res = FALSE;
        }
    }
    else if (time->bdays > days_in_month[time->bmonths])
    {
        res = FALSE;
    }
    else
    {
        /* MISRA */
    }
    return res;
}

/******************************************************************************
 * TIME RELATED PRIVATE FUNCTIONS
 *****************************************************************************/

/**
 * Calculate the number of seconds since 1st Jan 2012.
 * This function assumes that the clock was re-set to that, as per the requirements.
 */
PRIVATE rtc_time_t rtc_calc_seconds(void)
{
    uint8_t year_count;
    uint32_t ober_seconds = 0u;

    /* add seconds for each year from 2012 to last year, accounting for leap years as we go */
    for (year_count = 12u;
         year_count < rtc_data.current.byears;
         year_count = ((year_count+1u) % RTC_YEAR_MOD))
    {
        ober_seconds += SECONDS_IN_YEAR;
        ober_seconds += ADD_LEAP_DAY(year_count);
    }
    ober_seconds += total_seconds_to_month[rtc_data.current.bmonths];
    /* if we are past feb account for possible leap day in this year */
    if (rtc_data.current.bmonths>2u) { ober_seconds += ADD_LEAP_DAY(year_count); }
    ober_seconds += ((uint32_t)rtc_data.current.bdays - 1uL) * SECONDS_IN_DAY;
    ober_seconds += rtc_data.current.bhours    * SECONDS_IN_HOUR;
    ober_seconds += rtc_data.current.bminutes  * SECONDS_IN_MIN;
    ober_seconds += rtc_data.current.bseconds;

    return(ober_seconds);
}

/**
 * Calculate difference in seconds from midnight 1st Jan to 'time_stamp' in SAME year
 * therefore calculations are all 'forward looking' and dont wrap in this function
 *
 * @param time_stamp    pointer to timestamp structure to be calculated
 * @return              number of seconds since 1.1 of this year
 */
PRIVATE rtc_time_t rtc_seconds_this_year(const rtc_time_stamp_t* time_stamp)
{
    uint8_t month_count;
    uint32_t ober_seconds = 0u;

    /* 1 - year calculation : none */
    /* 2 - month calculation: add sec in each month, account for diff in days in month */
    /* no need for wrap as we are in same year */
    for (month_count=1u; month_count < time_stamp->bmonths; month_count++)
    {
        ober_seconds += seconds_in_month[month_count];
        /* if we pass a feb, account for possible leap day in this year */
        if (month_count==2u) { ober_seconds += ADD_LEAP_DAY(time_stamp->byears); }
    }
    /* 3 - days calculation : add seconds for each day in this month */
    ober_seconds += ((uint32_t)time_stamp->bdays-1uL) * SECONDS_IN_DAY;
    ober_seconds += time_stamp->bhours   * SECONDS_IN_HOUR;
    ober_seconds += time_stamp->bminutes * SECONDS_IN_MIN;
    ober_seconds += time_stamp->bseconds;

    return(ober_seconds);
}

/**
 * Calculate seconds for intervening complete years, taking into account leap years
 */
PRIVATE rtc_time_t rtc_seconds_complete_years(const rtc_time_stamp_t* start, const rtc_time_stamp_t* end)
{
    uint8_t year_count;
    uint32_t ober_seconds = 0u;

    /* if there are complete years between the dates count them */
    year_count = ((start->byears+1u) % RTC_YEAR_MOD);
    if (year_count != end->byears)
    {
        for ( ; year_count < end->byears; year_count = ((year_count+1u) % RTC_YEAR_MOD))
        {
            ober_seconds += SECONDS_IN_YEAR;
            ober_seconds += ADD_LEAP_DAY(year_count);
        }
    }
    return (ober_seconds);
}

/**
 * This function calculates the number of seconds between two arbitary time-dates.
 */
PRIVATE rtc_time_t rtc_calc_time_difference(const rtc_time_stamp_t* start, const rtc_time_stamp_t* end)
{
    rtc_time_t ober_seconds = 0u;

    if (start->byears != end->byears)
    {
        /* year wrap-up; calc from 1st Jan to 'start' time-date*/
        ober_seconds = ((SECONDS_IN_YEAR + ADD_LEAP_DAY(start->byears))
            - rtc_seconds_this_year(start));

        /* calculate complete years between dates */
        ober_seconds += rtc_seconds_complete_years(start, end);

        /* calculate seconds in final year to 'end' time-date */
        ober_seconds += rtc_seconds_this_year(end);
    }
    else
    {
        ober_seconds = rtc_seconds_this_year(end) - rtc_seconds_this_year(start);
    }

    return (ober_seconds);
}

/**
 * Add the required alarm minutes to the current time and convert it
 * to the time stamp format to be transmitted to the rtc later with rtc_set_alarm
 * command
 */
PRIVATE void rtc_calc_alarm_time(uint32_t alarm_minutes)
{
    uint8_t temp;
    uint8_t days_this_month;
    uint16_t mins_to_future;

    /* get the offset in minutes */
    mins_to_future = (uint16_t) (alarm_minutes - (rtc_data.seconds / SECONDS_IN_MIN));
    /* calculate 'number' of days */
    rtc_alarm_to_set.bdays  = (uint8_t)(mins_to_future / MINUTES_IN_DAY);
    mins_to_future        = mins_to_future % MINUTES_IN_DAY;
    /* calculate 'number' of hours */
    rtc_alarm_to_set.bhours   = (uint8_t)(mins_to_future / MINUTES_IN_HOUR);
    /* calculate 'number' of remaining minutes */
    rtc_alarm_to_set.bminutes = (uint8_t)(mins_to_future % MINUTES_IN_HOUR);

    /* calculate where in the future to set alarm (add current time) */
    temp = rtc_alarm_to_set.bminutes + rtc_data.current.bminutes;
    rtc_alarm_to_set.bminutes = (temp % MINUTES_IN_HOUR);
    rtc_alarm_to_set.bhours   = rtc_alarm_to_set.bhours + (temp / MINUTES_IN_HOUR);

    /* calc hours */
    temp = rtc_alarm_to_set.bhours + rtc_data.current.bhours;
    rtc_alarm_to_set.bhours   = (temp % HOURS_IN_DAY);
    rtc_alarm_to_set.bdays    = rtc_alarm_to_set.bdays + (temp / HOURS_IN_DAY);

    /* calc days */
    days_this_month = days_in_month[rtc_data.current.bmonths];
    /* leap if necessary */
    if (((rtc_data.current.byears % 4u)==0u) && (rtc_data.current.bmonths == 2u))
    {
        days_this_month += 1u;
    }
    /* work out offset */
    temp = rtc_alarm_to_set.bdays + rtc_data.current.bdays;
    if (temp > days_this_month)
    {
        temp -= days_this_month;
    }
    rtc_alarm_to_set.bdays = (uint8_t)temp;
}

/******************************************************************************
 * FUNCTIONS FOR EXTRACTING STATUS FROM RTC DATA STREAM
 *****************************************************************************/

/**
 * Check whether rtc chip reports clock running
 */
void rtc_check_clock_running(void)
{
    if (TRUE == rtc_readable())
    {
        if (i2c_rx_buff[PCA_CTR1] & PCA_CTR1_CKL_STOP)
        {
            rtc_data.status = Rtc_clock_no_run;
        }
    }
}

/**
 * Function to check if the rtc battery is returning guaranteed coherent data.
 * It sets rtc_data_t->status to #Rtc_low_battery if not.
 */
PRIVATE void rtc_check_battery_ok(void)
{
    if (TRUE == rtc_readable())
    {
        /* (the data integrity flag is contained in the seconds register) */
        if (i2c_rx_buff[PCA_SEC] & PCA_CTR1_BAT_FAIL)
        {
            rtc_data.status = Rtc_low_battery;
        }
    }
}

/**
 * Take the time as provided in the rx buffer, check that the received bcd is valid.
 * Convert each section to binary then calculate the number of seconds since 1st Jan 2012.
 */
PRIVATE void rtc_check_and_update_time(void)
{
    bool_t time_ok = TRUE;

    /* store seconds temporarily so we dont do calculations during critical section */
    rtc_time_t   temp_seconds;
    Rtc_status_t temp_status;
    uint8_t      ccr;

    /* check there is not already a problem */
    if (TRUE == rtc_readable())
    {
        /* check and convert time fields */
        time_ok &= bcd_to_bin((i2c_rx_buff[PCA_SEC]  & BCD_SEC_MASK),  SEC_MIN,   SEC_MAX,  &rtc_data.current.bseconds);
        time_ok &= bcd_to_bin((i2c_rx_buff[PCA_MIN]  & BCD_MINU_MASK), MINU_MIN,  MINU_MAX, &rtc_data.current.bminutes);
        time_ok &= bcd_to_bin((i2c_rx_buff[PCA_HOUR] & BCD_HOUR_MASK), HOURS_MIN, HOURS_MAX,&rtc_data.current.bhours);
        time_ok &= bcd_to_bin((i2c_rx_buff[PCA_DAY]  & BCD_DAY_MASK),  DAYS_MIN,  DAYS_MAX, &rtc_data.current.bdays);
        time_ok &= bcd_to_bin((i2c_rx_buff[PCA_MONTH]& BCD_MNTH_MASK), MNTH_MIN,  MNTH_MAX, &rtc_data.current.bmonths);
        time_ok &= bcd_to_bin((i2c_rx_buff[PCA_YEAR] & BCD_YEAR_MASK), YEAR_MIN,  YEAR_MAX, &rtc_data.current.byears);
        time_ok &= rtc_number_of_days_valid(&rtc_data.current);

        if (TRUE == time_ok)
        {
            /* could replace this line with call to rtc_calc_time_difference(start, end)
             * where end == time just read and start == 1 jan 2012. But this is faster.
             */
            temp_seconds = rtc_calc_seconds();
            temp_status = Rtc_ok;
        }
        else
        {
            temp_seconds = 0xDEADBEEF;
            temp_status  = Rtc_invalid_data;
        }
        /* ensure atomic access */
        SAVE_INTERRUPTS(ccr);
        /* update value and its meaning together */
        rtc_data.status  = temp_status;
        rtc_data.seconds = temp_seconds;
        RESTORE_INTERRUPTS(ccr);
    }
}

/**
 * Update alarm active information for each registered requester (feature).
 * This must be called periodically to provide alarm active information to the
 * application during the runtime. This only sets flag in the rtc_alarm_active
 * for requester which alarm is actually active. A feature (requester)
 * has to clear its flag prior detecting a new one in order to prevent
 * missed alarm.
 */
PRIVATE void rtc_update_alarm_active(void)
{
    uint8_t i;
    if(rtc_data.status == Rtc_ok)
    {
        for (i = 0u; i < (uint8_t) Rtc_alarm_req_max; i++)
        {
            if(rtc_ram_image.data.requested_alarm_times[i] <= (rtc_data.seconds / SECONDS_IN_MIN))
            {
                rtc_alarm_active[i] = TRUE;
                /* alarm has expired during mmc operation and has been flagged. We don't need
                 * to set the hw alarm and hence we may clear the request. All features
                 * shall check and clear its rtc_alarm_active flag prior shutting down in order to
                 * not miss it when we're about shutting down! */
                rtc_ram_image.data.requested_alarm_times[(uint8_t) i] = 0u;
            }
        }
    }
}

/**
 * Reset all features (requesters) alarm active flags
 */
PRIVATE void rtc_reset_alarm_active_flags(void)
{
    uint8_t i;
    for (i = 0u; i < (uint8_t) Rtc_alarm_req_max; i++)
    {
        rtc_alarm_active[i] = FALSE;
    }
}

/**
 * Update rtc_data's alarm related fields. Extract data from the rtc chip,
 * check its validity and extract the alarm time.
 */
PRIVATE void rtc_check_and_update_chip_alarm(void)
{
    bool_t alarm_ok = TRUE;

    /* check there is not already a problem */
    if (TRUE == rtc_readable())
    {
        /* check if alarm is set */
        if ((i2c_rx_buff[PCA_ALARM_MIN]     & PCA_ALARM_DISABLE) &&
            (i2c_rx_buff[PCA_ALARM_HOUR]    & PCA_ALARM_DISABLE) &&
            (i2c_rx_buff[PCA_ALARM_DAY]     & PCA_ALARM_DISABLE) &&
            (i2c_rx_buff[PCA_ALARM_WEEKDAY] & PCA_ALARM_DISABLE))
        {
            rtc_data.alarm_set = FALSE;
        }
        else
        {
            rtc_data.alarm_set = TRUE;
        }
        /* get the current alarm active status */
        rtc_data.alarm_active = (i2c_rx_buff[PCA_CTR2] & PCA_CTR2_AF) ? TRUE : FALSE ;
        /* get the irq armed status */
        rtc_data.alarm_irq_armed = (i2c_rx_buff[PCA_CTR2] & PCA_CTR2_AIE) ? TRUE : FALSE ;

        /* check and convert min - only if alarm field enabled */
        alarm_ok &= bcd_to_bin((i2c_rx_buff[PCA_ALARM_MIN] & BCD_MINU_MASK), MINU_MIN, MINU_MAX,
            &rtc_data.alarm.bminutes);
        /* check and convert hours - only if alarm field enabled */
        alarm_ok &= bcd_to_bin((i2c_rx_buff[PCA_ALARM_HOUR] & BCD_HOUR_MASK), HOURS_MIN, HOURS_MAX,
            &rtc_data.alarm.bhours);
        /* check and convert hours - only if alarm field is enabled */
        alarm_ok &= bcd_to_bin((i2c_rx_buff[PCA_ALARM_DAY] & BCD_DAY_MASK), DAYS_MIN, DAYS_MAX,
            &rtc_data.alarm.bdays);

        if (TRUE == alarm_ok)
        {
            /* alarm is offset from current time, populate missing fields for arbitary date calculation routine */
            rtc_data.alarm.bseconds = 0u;
            /* check if the alarm spans a month boundary */
            if (rtc_data.alarm.bdays < rtc_data.current.bdays)
            {
                rtc_data.alarm.bmonths = (rtc_data.current.bmonths + 1u);
                /* additionally check for year wrap */
                if (rtc_data.alarm.bmonths > MNTH_MAX)
                {
                    rtc_data.alarm.bmonths = 1u;
                    rtc_data.alarm.byears  = rtc_data.current.byears+1u;
                }
            }
            else
            {
                rtc_data.alarm.bmonths = rtc_data.current.bmonths;
                rtc_data.alarm.byears  = rtc_data.current.byears;
            }
        }
        else
        {
            rtc_data.status  = Rtc_invalid_data;
        }
    }
}

/**
 * Determine whether rtc is stuck in the test mode or not
 */
PRIVATE void rtc_check_stuck_in_test(void)
{
    if (TRUE == rtc_readable())
    {
        if (i2c_rx_buff[PCA_CTR1] & PCA_CTR1_TEST1)
        {
            rtc_data.status = Rtc_stuck_in_test;
        }
    }
}

/**
 * Create a tx buffer to transfer data to the rtc.
 * Consists of a data pointer for the rtc followed but the data to be txed.
 * This function is specific to the PCA8565.
 */
PRIVATE FPTR uint8_t* rtc_create_tx_buffer(i2c_comms_id cmd)
{
    /* select data pointer for this command */
    i2c_tx_buff[0] = i2c_request[cmd].tx_addr;

    /* now add any command specific data for this cmd */
    switch(cmd)
    {
    case Pca_read_device:
        /* nothing more to tx in this case */
        break;
    case Pca_set_up:
        /* ensure clk out and timer ctl are kept set for low power */
        i2c_tx_buff[1u] = PCA_CLK_OUT_DISABLE;
        i2c_tx_buff[2u] = PCA_TIM_CTR_DISABLE;
        break;
    case Pca_set_time:
        /* collect new time left by the command interface */
        i2c_tx_buff[1u] = BIN_TO_BCD(rtc_data.current.bseconds);
        i2c_tx_buff[2u] = BIN_TO_BCD(rtc_data.current.bminutes);
        i2c_tx_buff[3u] = BIN_TO_BCD(rtc_data.current.bhours);
        i2c_tx_buff[4u] = BIN_TO_BCD(rtc_data.current.bdays);
        i2c_tx_buff[5u] = 0u; /* weekdays field (we dont care) */
        i2c_tx_buff[6u] = BIN_TO_BCD(rtc_data.current.bmonths);
        i2c_tx_buff[7u] = BIN_TO_BCD(rtc_data.current.byears);
        break;
    case Pca_set_alarm:
        /* Set the alarm time and enable the match bits */
        i2c_tx_buff[1] = PCA_ALARM_ENABLE + BIN_TO_BCD(rtc_alarm_to_set.bminutes);
        i2c_tx_buff[2] = PCA_ALARM_ENABLE + BIN_TO_BCD(rtc_alarm_to_set.bhours);
        i2c_tx_buff[3] = PCA_ALARM_ENABLE + BIN_TO_BCD(rtc_alarm_to_set.bdays);
        i2c_tx_buff[4] = PCA_ALARM_DISABLE; /* week day */
        break;
    case Pca_disable_alarm:
        /* clear all alarm flags */
        i2c_tx_buff[1] = PCA_ALARM_DISABLE; /* min      */
        i2c_tx_buff[2] = PCA_ALARM_DISABLE; /* hours    */
        i2c_tx_buff[3] = PCA_ALARM_DISABLE; /* days     */
        i2c_tx_buff[4] = PCA_ALARM_DISABLE; /* week day */
        break;
    case Pca_set_interrupt:
        i2c_tx_buff[1] = PCA_IRQ_ENABLE;
        break;
    case Pca_clear_interrupt:
        i2c_tx_buff[1] = PCA_IRQ_CLEAR;
        break;
    case Pca_max_cmds: /* deliberate fall through */
    default:
        break;
    }
    return (i2c_tx_buff);
}


/******************************************************************************
 * RTC STATE FUNCTIONS
 *****************************************************************************/

/**
 * Set up the I2C driver for our RTC device
 */
PRIVATE bool_t rtc_set_up_I2C(void)
{
    /** I2C configuration */
    I2CDevice_t reg_rtc_hndl;
    bool_t res = TRUE;

    /* Set the device configuration */
    reg_rtc_hndl.dev_indx  = PCA8565_DEVICE_ID;
    reg_rtc_hndl.address   = RTC_I2C_ADDRESS;
    reg_rtc_hndl.bus_speed = I2C_100;

    /* clear this to allow next attempt */
    rtc_i2c_fault_occurred = FALSE;

    if (I2c_success != I2C_Initialise(reg_rtc_hndl))
    {
        res = FALSE;
    }
    return res;
}

/**
 * CHeck whether the rtc state machine is blocked or not (ready to accept
 * a new command request)
 */
PRIVATE bool_t rtc_cmds_blocked(void)
{
    return ((bool_t)!(rtc_state == Rtc_idle_state));
}

/**
 * Periodically request an update of the time at a rate defined by #RTC_TIMER_UPDATE_RATE_MS
 * The function ensures that no other command are being processed by checking that
 * the #rtc_state is #Rtc_idle_state. If not it will delay issuing the update.
 */
PRIVATE void rtc_periodically_get_time(void)
{
    /* is the state machine idle? */
    //if ((rtc_state == Rtc_idle_state) && (!ms_shutdown_pending))
		if (rtc_state == Rtc_idle_state) 
    {
        if (ts_is_elapsed_ms(ts_get_ms(), rtc_ts, RTC_TIMER_UPDATE_RATE_MS))
        {
            rtc_initiate_command(Pca_read_device);
        }
    }
}

/**
 * Check whether there was an i2c error and repeat up to rtc_number_of_retries times, if so.
 * Raise an error flag and reinitialize whole rtc if failed for rtc_number_of_retries times.
 */
PRIVATE rtc_i2c_trans_state rtc_handle_i2c_error(I2C_ProgressType i2c_err,
    rtc_i2c_trans_state current_state)
{
    rtc_i2c_trans_state res = current_state;
    if (I2c_failure == i2c_err)
    {
        /* flag this to inform initialization (needed for recovery) */
        rtc_i2c_fault_occurred = TRUE;
        /* set this to prevent cannot update condition (next seconds reading
         * will be always different from RTC_INVALID_SECONDS)*/
        previous_second = RTC_INVALID_SECONDS;

        /* retry */
        I2C_ResetBusCycle();
        if(rtc_i2c_retries == 0u)
        {
            /* log there has been an i2c error */
            rtc_data.status = Rtc_i2c_fault;
            /* and a state it happened in (needed for fault filtering) */
            rtc_i2c_fault_occurred_state = rtc_state;

            rtc_state = Rtc_init_state;
            rtc_init_state = Rtc_init_i2c;
            res = pca_idle;
        }
        else
        {
            /* retry */
            rtc_i2c_retries--;
            RTC_START_I2C_STATE_MACHINE;
            res = pca_i2c_check;
        }
    }
    return res;
}

/**
 * This function selects the next i2c transaction state based on the i2c status.
 * If the i2c transaction is successful #I2c_success it moves the i2c state
 * machine on to the state specified.
 * However if an i2c error occurred this function calls a function that resets
 * the i2c state machine ready for another request. It also deduces times out
 * the maximal i2c operation time and eventually raises its own i2c failure flag.
 *
 * @param i2c_ret contains the status of the i2c system.
 * @param next_step contains the required next step.
 */
PRIVATE rtc_i2c_trans_state rtc_await_completion(I2C_ProgressType i2c_ret, rtc_i2c_trans_state next_step)
{
    rtc_i2c_trans_state result;
    /* this needs to differentiate whether we've just asked for a transaction and
     * it hasn't even started because the i2c is not at free cycle state, or
     * if we're waiting for a result and it didn't make it in time (or at all) */
    if(I2c_success == i2c_ret)
    {
        result = next_step;
        rtc_i2c_fault_occurred = FALSE;
        rtc_i2c_ts = ts_get_ms();
    }
    else
    {
        if(ts_is_elapsed_ms(ts_get_ms(), rtc_i2c_ts, RTC_I2C_STATE_TIMEOUT_MS))
        {
            i2c_ret = I2c_failure;
        }
        result = rtc_handle_i2c_error(i2c_ret, rtc_i2c_state);
    }
    return result;
}

/**
 * Check that it is possible to read from the rtc, i.e. that there wasn't
 * an i2c error since the last cycle.
 */
PRIVATE bool_t rtc_readable(void)
{
    return ((rtc_i2c_fault_occurred == FALSE) ? TRUE : FALSE);
}

/**
 * Check that the seconds field received from the rtc is updating.
 * If not, note that in the status field #rtc_data_t->status so a
 * fault will be raised.
 */
PRIVATE void rtc_seconds_updated(void)
{
    if (rtc_readable())
    {
        if (rtc_data.seconds != previous_second)
        {   /* seconds updating */
            previous_second   = rtc_data.seconds;
            rtc_unchanging_ms = 0u;
        }
        else
        {   /* count up to limit */
            rtc_unchanging_ms += (uint16_t)RTC_TIMER_UPDATE_RATE_MS;
            /* do not override Rtc_clock_no_run with Rtc_cannot_update*/
            if (rtc_unchanging_ms > RTC_UNCHANGING_LIMIT_MS)
            {
                rtc_data.status = Rtc_cannot_update;
            }
        }
    }
}

/**
 * Check that i2c initialization is continuing successfully. If not set up a
 * delay before re-trying.
 * If i2c initialization is progressing successfully this function
 * make transition to next_state
 */
PRIVATE void rtc_setup_init_ok(rtc_init_state_t next_state)
{
    if (rtc_i2c_fault_occurred)
    {   /* error during last i2c operation, set up a delay and try again */
        rtc_ts = ts_get_ms();
        rtc_init_state = Rtc_error_occurred;
        rtc_i2c_fault_occurred_state = rtc_state;
    }
    else
    {
        rtc_init_state = next_state;
        RTC_START_I2C_STATE_MACHINE;
    }
}

/**
 * This function handles the initialisation of the i2c and pcs chip. It also
 * performs an initial read of the device to set up the interface with the rtc
 * clock values.
 * As the device is read it also processes various other flags and finds
 * any rtc errors that are reported (low battery, clock stopped etc). There
 * are several initialisation steps required so this is handled outside
 * of the main rtc state machine.
 */
PRIVATE bool_t rtc_init_hardware(void)
{
    bool_t complete = FALSE;

    switch (rtc_init_state)
    {
    /* register with i2c module */
    case Rtc_init_i2c:
        if(rtc_set_up_I2C())
        {
            rtc_setup_init_ok(Rtc_setup_pca);
        }
        break;

    /* set up pc8565 */
    case Rtc_setup_pca:
        if(rtc_command(Pca_set_up))
        {
            rtc_setup_init_ok(Rtc_clear_alarm_ie_if);
        }
        break;

    /* check and update alarm configuration */
    case Rtc_clear_alarm_ie_if:
        if(rtc_command(Pca_clear_interrupt))
        {
            rtc_setup_init_ok(Rtc_read_check_device);
        }
        break;

    /* Read the rtc and see if rtc chip is reporting problems */
    case Rtc_read_check_device:
        if (rtc_command(Pca_read_device))
        {
            rtc_handle_response(Pca_read_device);
            if(rtc_data.status == Rtc_ok)
            {
                /* all initialization tasks successfully performed */
                complete = TRUE;
            }
            else
            {
                rtc_ts = ts_get_ms();
                rtc_init_state = Rtc_error_occurred;
            }
        }
        break;

    /* await a timeout on the re-try initialisation */
    case Rtc_error_occurred:
        if (ts_is_elapsed_ms(ts_get_ms(), rtc_ts, RTC_I2C_INITIALISATION_MS))
        {   /* re-try */
            rtc_init_state = Rtc_init_i2c;
        }
        break;

    default:
        /* badness has occure - start again */
        rtc_init();
        break;
    }

    return (complete);
}

/**
 * This is one of the main functions in rtc. We have been passed an rtc command
 * to process and will drive the i2c with the commands to get the rtc chip to
 * respond. The function will handle a generic i2c exchange, but the cmd table this
 * function uses is PCA8565 specific.
 *
 * @param cmd   command being performed
 * @return      TRUE when the command has finished
 * @return      FALSE when the command is in progress
 */
PRIVATE bool_t rtc_command(i2c_comms_id cmd)
{
    FPTR uint8_t* i2c_tx_buff_ptr = NULL;
    bool_t transaction_complete   = FALSE;
    I2C_ProgressType i2c_err      = I2c_failure;

    switch(rtc_i2c_state)
    {
    /* as long i2c is ok, continue with this transaction */
    case pca_i2c_check:
        i2c_err   = I2C_OpComplete();
        rtc_i2c_state = rtc_await_completion(i2c_err, pca_tx);
        break;

    /* tx a command and wait to complete */
    case pca_tx:
        i2c_tx_buff_ptr = rtc_create_tx_buffer(cmd);
        i2c_err   = I2C_Transmit(PCA8565_DEVICE_ID, i2c_tx_buff_ptr, (const size_t)(i2c_request[cmd].txb));
        rtc_i2c_state = rtc_await_completion(i2c_err, pca_await_tx);
        break;
    case pca_await_tx:
        i2c_err   = I2C_OpComplete();
        rtc_i2c_state = rtc_await_completion(i2c_err, pca_rx);
        break;

    /* check to see if there is anything to rx and wait to complete */
    case pca_rx:
        if (i2c_request[cmd].rxb > 0u)
        {
            i2c_err   = I2C_Receive(PCA8565_DEVICE_ID, (FPTR uint8_t*)i2c_rx_buff, (const size_t)i2c_request[cmd].rxb);
            rtc_i2c_state = rtc_await_completion(i2c_err, pca_await_rx);
        }
        else
        {   /* no rx bytes for this cmd */
            rtc_i2c_state = pca_idle;
        }
        break;
    case pca_await_rx:
        i2c_err   = I2C_OpComplete();
        rtc_i2c_state = rtc_await_completion(i2c_err, pca_idle);
        break;

    /* nothing left to do to - flag transaction as complete */
    case pca_idle:
        transaction_complete = TRUE;
        break;
    default:
    /* badness has occurred - reset this sub-system */
        rtc_init();
        break;
    }

    return (transaction_complete);
}

/**
 * This function handles the responses from the pca8565 chip that are specific
 * for each command. This stucture is generic enough to be used on
 * any rtc. Just need to re-write the called data processing functions
 * for each command/case. The returned data will have been placed in the
 * rx buffer #i2c_rx_buff.
 * Currently the only command obtained from the rtc is the read device command.
 * @param cmd contains the command type to be processed, type #i2c_comms_id.
 *
 */
PRIVATE void rtc_handle_response(i2c_comms_id cmd)
{
    switch (cmd)
    {
    case Pca_read_device:
        /* Reset status to default value. Following function will set it */
        rtc_data.status = Rtc_unknown;
        rtc_check_clock_running();
        rtc_check_stuck_in_test();
        if(rtc_data.status == Rtc_unknown)
        {
            rtc_check_and_update_chip_alarm();
            rtc_check_and_update_time();
            rtc_deduce_wake_up_sources();
            rtc_update_alarm_active();
            rtc_check_battery_ok();
            rtc_seconds_updated();
        }
        break;
    /* these command dont require any returned data to be processed */
    case Pca_set_up        :
    case Pca_set_time      :
    case Pca_set_alarm     :
    case Pca_disable_alarm :
    case Pca_set_interrupt :
    case Pca_clear_interrupt:
    case Pca_max_cmds      : /* deliberate fall through */
    default:
        /* dont understand what we have to process - do nothing */
        break;
    }
}

/**
 * Update fault filter related to the rtc
 *
 * Fault code Fm_plat_rtc is reported when any rtc related fault occurs with
 * following subtype:
 * Fm_invalid  - data read from the rtc are not valid
 * Fm_timeout  - an error has occurred while obtaining data from the rtc
 * Fm_min_v    - the battery powering the rtc is or has been detected low, therefore
 *               data validity is not guaranteed
 * Fault code Fm_plat_rtc_config is reported only when rtc fault occurs during
 * initialization stage. Following subtypes are distinguished:
 * Fm_bad -     either Rtc_stuck_in_test or Rtc_clock_no_run condition has been
 *              detected
 * Fm_timeout  - an I2C error has occurred during initialization stage
 */
PRIVATE void rtc_update_fault_filter(void)
{
    bool_t plat_rtc_fault_present = FALSE;
    Fm_fault_subtype_list_e plat_rtc_subtype = Fm_general_failure_information;
    bool_t plat_rtc_config_fault_present = FALSE;
    Fm_fault_subtype_list_e plat_rtc_config_subtype = Fm_general_failure_information;
    bool_t plat_rtc_monitored = TRUE;
    bool_t plat_rtc_config_monitored = TRUE;
    switch (rtc_data.status)
    {
        case Rtc_stuck_in_test:
        case Rtc_clock_no_run:
            /* these two errors might have occurred in rtc configuration stage only
             * and are ignored during data obtaining mode (which will never be entered
             * when rtc configuration stage won't end with ok state anyway) */
            plat_rtc_config_fault_present = TRUE;
            plat_rtc_config_subtype = Fm_bad;
            /* ignore plat_rtc fault*/
            plat_rtc_monitored = FALSE;
            break;
        case Rtc_i2c_fault:
            /* this fault might have occurred either during rtc configuration
             * or data processing stages. Determine which state it was
             * and set appropriate fault code.
             * If a fault has occurred during the normal operation, i.e. config
             * stage must have been passed, avoid monitoring of Fm_plat_rtc fault
             * in order to not remove it sooner than next config stage has been successfully
             * passed again, which is always entered after any fault occurs as a recovery
             * measure. */
            if(rtc_i2c_fault_occurred_state == Rtc_init_state)
            {
                if(fm_is_fault_present(Fm_plat_rtc))
                {
                    /* Normally, Fm_plat_rtc_config faults are reported during
                     * the initialization stage. When fault occurred during the normal
                     * operation, feature performs initialization stage again, but
                     * we need to tell, that subsequent faults didn't occurred during
                     * initial stage */
                    plat_rtc_config_monitored = FALSE;
                    plat_rtc_fault_present = TRUE;
                    plat_rtc_subtype = Fm_timeout;
                }
                else
                {
                    plat_rtc_monitored = FALSE;
                    plat_rtc_config_fault_present = TRUE;
                    plat_rtc_config_subtype = Fm_timeout;
                }
            }
            else
            {
                plat_rtc_config_monitored = FALSE;
                plat_rtc_fault_present = TRUE;
                plat_rtc_subtype = Fm_timeout;
            }
            break;
        /* rtc data faults */
        case Rtc_low_battery:
            /* this fault should have happened during obtaining data and is ignored
             * otherwise */
            plat_rtc_config_monitored = FALSE;
            plat_rtc_fault_present = TRUE;
            plat_rtc_subtype = Fm_min_v;
            break;
        case Rtc_cannot_update:
        case Rtc_invalid_data:
            /* this fault should have happened during obtaining data and is ignored
             * otherwise */
            plat_rtc_config_monitored = FALSE;
            plat_rtc_fault_present = TRUE;
            plat_rtc_subtype = Fm_invalid;
            break;
        case Rtc_ok:
            plat_rtc_fault_present = FALSE;
            plat_rtc_config_fault_present = FALSE;
            break;
        /* Inhibit fault updating when rtc hasn't been processed yet */
        default:
        case Rtc_unknown:
            plat_rtc_config_monitored = FALSE;
            plat_rtc_monitored = FALSE;
            break;
    }
    (void) fm_update_fault_filter(Fm_plat_rtc_config, plat_rtc_config_subtype,
        plat_rtc_config_fault_present, plat_rtc_config_monitored);
    (void) fm_update_fault_filter(Fm_plat_rtc, plat_rtc_subtype,
        plat_rtc_fault_present, plat_rtc_monitored);
}

/**
 * Check whether all features registered for using rtc alarms have requested
 * an alarm by checking its shutdown status.
 * Since all features are requesting alarms after ms initiates
 * shutdown, rtc must wait before it start to process these alarm requests.
 *
 * @return  TRUE    all registered features are ready to shutdown and hence
 *                  requested an alarm
 * @return  FALSE   at least one feature is not ready to shutdown
 */
PRIVATE bool_t rtc_all_alarm_features_allowed_to_shdn(void)
{
    uint8_t i;
    bool_t allowed = TRUE;
    //for (i = 0u; i < (uint8_t) Rtc_alarm_req_max; i++)
    //{
    //    if(ms_get_feature_shutdown_state(rtc_alarm_req_inhibs[i]) != Ms_shutdown_allowed)
    //    {
    //        allowed = FALSE;
    //    }
    //}
//	if(ms_get_feature_shutdown_state(ms_inhibit_log) != Ms_shutdown_allowed)
//        {
//            allowed = FALSE;
//        }
    return allowed;
}

/**
 * Update rtc shutdown signal to the MS feature
 * - if rtc state is idle, i.e. no command processing is taking place
 * or an error has occurred during performing rtc configuration stage,
 * allow shutdown by setting Ms_shutdown_allowed inhibit status to the MS feature
 * - if rtc state is not idle, e.g. command processing is taking place,
 * prevent shutdown by setting Ms_shutdown_prevented inhibit status to
 * the MS feature. Since RTC is secondary feature, inhibiting its
 * flag doesn't cause ms to wake up again once shutdown is pending
 */
PRIVATE void rtc_update_shutdown(void)
{
//   enum ms_shutdown_state_e allowed = Ms_shutdown_prevented;
//  if(ms_shutdown_pending)
//	   
//   {
//       /* detect shutdown request transition and store timestamp of the event */
//       if(rtc_ms_shutdown_pending_last != ms_shutdown_pending)
//       {
//           rtc_shutdown_pending_ts = ts_get_ms();
//       }
//       /* alarm processing needs valid rtc data */
//       if(rtc_all_alarm_features_allowed_to_shdn() && !rtc_alarms_process_requested)
//       {
//           if(rtc_process_alarms())
//           {
//               rtc_alarms_process_requested = TRUE;
//           }
//       }
//       /* allow shutdown only if alarms has been processed or there was an error, it
//        * was reported and a recovery attempt has been made */
//       if(ts_is_elapsed_ms(ts_get_ms(), rtc_shutdown_pending_ts, rtc_shutdown_timeout)
//           || rtc_alarms_processed_success)
//       {
//           allowed = Ms_shutdown_allowed;
//       }
//       ms_update_shutdown_state(ms_inhibit_rtc, allowed);
//   }
//   else
//   {
//       if((rtc_state == Rtc_idle_state) ||
//           ((rtc_state == Rtc_init_state) && (rtc_init_state == Rtc_error_occurred)))
//       {
//           ms_update_shutdown_state(ms_inhibit_rtc, Ms_shutdown_allowed);
//       }
//       else
//       {
//           ms_update_shutdown_state(ms_inhibit_rtc, Ms_shutdown_prevented);
//       }
//   }
//   rtc_ms_shutdown_pending_last = ms_shutdown_pending;
}

/**
 * Go through rtc_ram_image.data.requested_alarm_times, find the soonest, calculate
 * alarm time and configure rtc accordingly
 */
PRIVATE bool_t rtc_process_alarms(void)
{
    uint8_t i;
    uint32_t soonest_minute = UINT32_MAX;
    bool_t res = FALSE;

    if((rtc_cmds_blocked() == FALSE) && (rtc_data.status == Rtc_ok))
    {
        /* find the soonest alarm in the future. Ignore expired ones */
        for (i = 0u; i < (uint8_t) Rtc_alarm_req_max; i++)
        {
            /* check whether an alarm time hasn't expired yet since it was requested
             * (mmc has been on and hence no need to wake it up).
             * If so, remove it */
            if(rtc_ram_image.data.requested_alarm_times[i])
            {
                if(rtc_ram_image.data.requested_alarm_times[i] <= (rtc_data.seconds / SECONDS_IN_MIN))
                {
                    /* remove expired alarm */
                    rtc_ram_image.data.requested_alarm_times[i] = 0u;
                }
                else if(rtc_ram_image.data.requested_alarm_times[i] < soonest_minute)
                {
                    soonest_minute = rtc_ram_image.data.requested_alarm_times[i];
                }
                else
                {
                    /* MISRA */
                }
            }
        }
        if(soonest_minute != UINT32_MAX)
        {
            rtc_calc_alarm_time(soonest_minute);
            rtc_initiate_command(Pca_set_alarm);
        }
        else
        {
            rtc_alarms_processed_success = TRUE;
        }
        res = TRUE;
    }
    return res;
}

/**
 * Deduce wake-up sources and process expired alarms. This
 * is intended to be called just once upon startup.
 *
 * All feature's alarm requests are checked against actual time and
 * those responsible for the wake up are flagged in #rtc_wake_up_sources
 * and #rtc_alarm_active arrays.
 */
PRIVATE void rtc_deduce_wake_up_sources(void)
{
    uint8_t i;
    uint8_t num_of_sources = 0u;
    if(rtc_wake_ups_sources_deduced == FALSE)
    {
        for (i = 0u; i < (uint8_t) Rtc_alarm_req_max; i++)
        {
            rtc_wake_up_sources[i] = FALSE;
        }
        /* find all expired alarm requests up to now */
        for (i = 0u; i < (uint8_t) Rtc_alarm_req_max; i++)
        {
            if ((rtc_ram_image.data.requested_alarm_times[i] != 0u)
                && (rtc_ram_image.data.requested_alarm_times[i] <=
                    (rtc_data.seconds / SECONDS_IN_MIN)))
            {
                /* flag expired alarm request */
                rtc_wake_up_sources[i] = TRUE;
                rtc_alarm_active[i] = TRUE;
                /* clear the request */
                rtc_ram_image.data.requested_alarm_times[i] = 0u;
                num_of_sources++;
            }
        }

        if(wus_original.rtc && (num_of_sources == 0u))
        {
            /* WUS signals the wake-up from the RTC, but there's no alarm requester
             * found */
            rtc_unknown_alarm_wake_up = TRUE;
        }
        rtc_wake_ups_sources_deduced = TRUE;
    }
}

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/
/**
 * Initialization function for the RTC module.
 * The requirements call for initialization every 100 msec if not successful.
 * Therefore the initialization here is of data structures only that will allow
 * the command interface to behave until that time.
 */
void rtc_init(void)
{
    /* set up the structure for the rtc interface these will be updated again once
     * the device has been initialised. Block RTC requests until we are up and running*/
    rtc_data.seconds       = 0u;
    rtc_unchanging_ms      = 0u;
    rtc_i2c_retries        = 0u;
    rtc_data.alarm_set     = FALSE;
    rtc_data.command       = Pca_set_up;
    rtc_data.status        = Rtc_unknown;
    /* begin state machines */
    rtc_state         = Rtc_init_state;
    rtc_init_state    = Rtc_init_i2c;
    rtc_reset_alarm_active_flags();
}

/**
 * rtc_update function.
 * This function initializes the i2c sub-system and rtc chip and
 * checks the rtc chip for any errors. Periodically it updates the clock
 * and alarm remaining time (if there is one) and process any pending
 * external requests.
 */
void rtc_update(void)
{
    switch (rtc_state)
    {
    /* init i2c, rtc chip init and check for reported rtc errors */
    case Rtc_init_state:
        if (rtc_init_hardware())
        {
            /* ready to start- already read time once to get interface populated */
            rtc_data.command = Pca_max_cmds;
            rtc_ts = ts_get_ms();
            rtc_state = Rtc_idle_state;
        }
        break;

    /* await periodic read request or external command */
    case Rtc_idle_state:
        rtc_periodically_get_time();
        break;

    /* process any commands */
    case Rtc_process_cmd_state:
        if (rtc_command(rtc_data.command))
        {
            /* if it was set alarm command, enable interrupt as well */
            if(rtc_data.command == Pca_set_alarm)
            {
                rtc_initiate_command(Pca_set_interrupt);
            }
            else
            {
                if(rtc_data.command == Pca_set_interrupt)
                {
                    rtc_alarms_processed_success = TRUE;
                }
                rtc_ts = ts_get_ms();
                rtc_handle_response(rtc_data.command);
                if(rtc_data.status == Rtc_ok)
                {
                    rtc_data.command = Pca_max_cmds;
                    rtc_state        = Rtc_idle_state;
                }
                else
                {
                    rtc_state         = Rtc_init_state;
                    rtc_init_state    = Rtc_init_i2c;
                }
            }
        }
        break;

    default:
        /* badness has occurred - restart */
        rtc_init();
        break;
    }

#if defined MMC
    /* log any faults that have been noticed */
    rtc_update_fault_filter();
#endif
    rtc_update_shutdown();
}

/**
 * Get number of seconds passed since 1.1.2012
 *
 * @return number of seconds since 1.1.2012
 */
uint32_t rtc_get_seconds(void)
{
    return(rtc_data.seconds);
}

/**
 * Deduce data validity status from detailed rtc status
 *
 * @return  Rtc_data_unknown    rtc hasn't been read yet
 * @return  Rtc_data_invalid    rtc doesn't provide valid data or an error has occurred
 *                              during communicating with the rtc
 * @return  Rtc_data_valid      rtc provide valid data and may be obtained with
 *                              rtc_get_seconds()
 */
RTC_DataValidType rtc_get_status(void)
{
    switch(rtc_data.status)
    {
        case Rtc_unknown:
            return(Rtc_data_unknown);
        case Rtc_ok:
            if(rtc_state != Rtc_init_state)
            {
                return (Rtc_data_valid);
            }
            else
            {
                return (Rtc_data_unknown);
            }
        case Rtc_i2c_fault:
        case Rtc_stuck_in_test:
        case Rtc_clock_no_run:
        case Rtc_low_battery:
        case Rtc_cannot_update:
        case Rtc_invalid_data:
        default:
            return(Rtc_data_invalid);
    }
}

/**
 * Converts the provided date/time to the RTC seconds count by calculating the
 * number of seconds from the RTC reference time (0h 0min 0s 1/1/2012) to the 
 * specified date/time.
 */
rtc_time_t rtc_convert_date_to_rtc_time(const rtc_time_stamp_t* date_time)
{
    rtc_time_t retval = rtc_calc_time_difference(&rtc_ref_time, date_time);
    return retval;
}
/**
 * Request alarm
 *
 * This function stores alarm request independently from each registered
 * alarm requester (see rtc.def). This is then processed by the feature
 * prior to the shutdown.
 * Each requester may request just one rtc alarm. Further call by the same
 * requester simply resets previous request. If alarm_minutes argument equals
 * to zero, no alarm will be requested.
 *
 * @param requester             one of registered requesters
 * @param alarm_minutes         alarm time represented as a number of minutes
 *                              in the future counted from present time
 * @return Rtc_invalid_request  requested number of minutes at which alarm
 *                              shall be set exceeds maximal allowed number
 * @return Rtc_system_not_ready RTC feature hasn't initialized the chip yet
 *                              or the chip is not in operating condition and
 *                              hence can't accept any requests. This is
 *                              not the case when the feature performs routine time
 *                              reading from the chip, i.e. requests are accepted anytime
 *                              the time reading is valid.
 * @return Rtc_alarm_request_processed Alarm request has been successfully stored to
 *                                     be processed later (upon shutdown request)
 */
Rtc_set_alarm_res_e rtc_set_alarm(rtc_alarm_req_e requester, uint16_t alarm_minutes)
{
    Rtc_set_alarm_res_e res = Rtc_system_not_ready;
    /* calculate number of seconds since origin and store to nvs */
    if((rtc_data.status == Rtc_ok) && (rtc_state != Rtc_init_state))
    {
        if(alarm_minutes == 0u)
        {
            rtc_ram_image.data.requested_alarm_times[(uint8_t) requester] = 0u;
            res = Rtc_alarm_request_processed;
        }
        else if(alarm_minutes > MAX_FUTURE_ALARM)
        {
            res = Rtc_invalid_request;
        }
        else
        {
            /* Since rtc chip used provides minute granularity for the alarm,
             * calculate time in minutes from current time. Ceil or floor to
             * the closer minute boundary */
            rtc_ram_image.data.requested_alarm_times[(uint8_t) requester] = (uint32_t) alarm_minutes
                + (rtc_data.seconds / SECONDS_IN_MIN)
                + ((rtc_data.current.bseconds > 30u) ? 1u : 0u);
            res = Rtc_alarm_request_processed;
        }
    }
    return res;
}

/**
 * Returns number of seconds in future from now the alarm of given requester
 * is requested to
 *
 * @param requester     a requester to be queried
 * @return              number of minutes in future from now the alarm of a requester
 *                      is set to. Zero means no alarm has been requested
 */
uint32_t rtc_get_alarm(rtc_alarm_req_e requester)
{
    uint32_t seconds = 0u;
    if (rtc_ram_image.data.requested_alarm_times[(uint8_t) requester] > 0u)
    {
        seconds = (rtc_ram_image.data.requested_alarm_times[(uint8_t) requester] * SECONDS_IN_MIN)
            - rtc_data.seconds;
    }
    return seconds;
}

/**
 * Returns whether the mmc has been woken up by a requester
 *
 * @param   requester           a requester to be queried whether if it's
 *                              woken up the mmc
 * @return  TRUE                requester has woken up the mmc
 * @return  FALSE               requester hasn't woken up the mmc
 */
bool_t rtc_has_been_woken_by(rtc_alarm_req_e requester)
{
    return rtc_wake_up_sources[(uint8_t) requester];
}

/**
 * Returns whether alarm of a requester is active at the moment
 *
 * @param requester     a requester to be queried for alarm activity.
 *                      When alarm is active. User may clear it by
 *                      calling rtc_dismiss_alarm(). No new alarm event are
 *                      reported prior clearing (dismissing) the old one.
 * @return TRUE         Alarm of requester is active at the moment
 * @return FALSE        Alarm of requester is not active at the moment
 */
bool_t rtc_is_alarm_active(rtc_alarm_req_e requester)
{
    return rtc_alarm_active[(uint8_t) requester];
}

/**
 * Dismiss an alarm flag of a requester
 *
 * @param requester     a requester (owner) of alarm, which shall be dismissed
 */
void rtc_dismiss_alarm(rtc_alarm_req_e requester)
{
    rtc_alarm_active[(uint8_t) requester] = FALSE;
}

/**
 * Report the RTC status to OBD for snapshot records
 *
 * @return Current RTC status
 */
Rtc_status_t rtc_ss_data_status(void)
{
    return rtc_data.status;
}

/******************************************************************************
 * TEST HOOKS
 *****************************************************************************/

//#if defined(UNIT_TEST)
/**
 * Allows the application to set the current time.
 */
Rtc_set_time_res_e rtc_set_time(rtc_time_stamp_t * set)
{
   // if (!rtc_cmds_blocked())
   // {
        rtc_i2c_retries = rtc_number_of_retries;
        rtc_data.command = Pca_set_time;
        rtc_state = Rtc_process_cmd_state;
        RTC_START_I2C_STATE_MACHINE;
        /* capture time required */
        rtc_data.current.byears  = set->byears;
        rtc_data.current.bmonths = set->bmonths;
        rtc_data.current.bdays   = set->bdays;
        rtc_data.current.bhours  = set->bhours;
        rtc_data.current.bminutes= set->bminutes;
        rtc_data.current.bseconds= set->bseconds;
        return Rtc_requested;
    //}
    //else
    //{
    //    return Rtc_busy;
    //}
}

void rtc_set_seconds(uint32_t value)
{
    rtc_data.seconds = value;
}

void rtc_set_status(Rtc_status_t new_status)
{
    rtc_data.status = new_status;
}
//#endif

