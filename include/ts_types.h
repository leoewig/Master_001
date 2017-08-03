/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    ts_types.h
 *
 * description:   Header file for timer services feature
 * 
 * 
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Header file for timer services feature
 *                       
 *
 *****************************************************************************/
#if !defined(TS_TYPES_H)
#define      TS_TYPES_H
/**
 *
 * Header file for timer services feature
 *
 *
 * @file
 * @ingroup TimerServices
 * $Id: ts_types.h 11919 2014-03-25 16:17:37Z pr4 $
 */

#include "pit_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/** Maximum value for ts_time_t */
#define TS_TIME_T_MAX    INT32_MAX

/** Minimum value for ts_time_t */
#define TS_TIME_T_MIN    INT32_MIN

 /* These are all defined as signed, to match ts_time_t */

 /** The number of microseconds in a millisecond */
 #define US_IN_MS 1000

 /** The number of milliseconds in a second */
 #define MS_IN_S 1000L

  /** The number of seconds in a minute */
 #define S_IN_MIN 60L

 /** The number of seconds in a minute */
 #define MIN_IN_HOUR 60L

 /** The number of hours in a day */
#define HOUR_IN_DAY 24L

/** The number of seconds in an hour */
#define S_IN_HOUR (S_IN_MIN * MIN_IN_HOUR)

 /** The number of minutes in a day */
#define MIN_IN_DAY (MIN_IN_HOUR * HOUR_IN_DAY)

/** The interval of the timestamp step in milliseconds */
#define TS_TIMER_MS 1

/* Macros to help features that are measuring time intervals */

/** Give a #ts_time_t value that represents a certain number of seconds */
#define TS_SECONDS(s)   ((s) * MS_IN_S)

/** Give a #ts_time_t value that represents a certain number of minutes */
#define TS_MINUTES(m)   ( TS_SECONDS( (m) * S_IN_MIN) )

/** Give a #ts_time_t value that represents a certain number of hours */
#define TS_HOURS(h)    ( TS_MINUTES( (h) * MIN_IN_HOUR) )

/**
 * Calculates time difference, in microseconds, between two timestamps
 * in ticks, as returned by ts_get_ticks()
 * @param now_ticks     later timestamp, in ticks
 * @param then_ticks    earlier timestamp, in ticks
 * @returns              time difference in microseconds
 */
#define TS_DIFF_US(now_ticks,then_ticks)              \
                                 (((now_ticks) - (then_ticks)) * PIT_TICK_US)
/**
 * Test whether the current time is more than a given period since an
 * earlier time.
 *
 * @param then_ticks        previous timestamp returned by ts_get_ticks()
 * @param period_usec       period, in microseconds
 * @retval      zero        `then_ticks` was less than `period_usec` in the past
 * @retval      non-zero    `then_ticks` was at least `period_usec` in the past
 *
 * @note See ts_get_ticks() for requirements on its accuracy.
 */
#define TS_IS_ELAPSED_US(then_ticks,period_usec)       \
                (TS_DIFF_US(ts_get_ticks(),(then_ticks)) >= (period_usec))

/**
 * Like ts_is_elapsed_ms, but doesn't yield a difficult to suppress complaint
 * whenever it is used in a conditional */
#define IS_ELAPSED_MS(newval, oldval, interval_ms) ( \
    (bool_t) ( ((newval) - (oldval))>= (interval_ms) ) \
        )

#if defined(UNIT_TEST)
/**
 * Set ticks counter to defined value.
 *
 * @param now_us    desired time in microseconds.
 */
#define TS_SET_US(now_us)    \
                (ts_set_ticks((now_us) / PIT_TICK_US))
#endif

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/**
 * Datatype representing PIT tick counts */
typedef uint32_t    ts_ticks_t;

/**
 * Datatype representing a timestamp in milliseconds.*/
typedef int32_t     ts_time_t;

#endif      /* TS_TYPES_H */

