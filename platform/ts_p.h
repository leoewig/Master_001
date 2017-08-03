#if !defined(TS_P_H)
#define      TS_P_H
/**
 *
 * Timer services private header
 *
 *
 * @file
 * @ingroup TimerServices
 * $Id: ts_p.h 3447 2012-08-28 14:49:24Z vs7 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/** The maximal value used for ticks PIT counter initialization and
 * ticks count calculation
 */
#define TS_TICKS_PIT_MAX_VALUE UINT16_MAX

/** The time interval required for the timer step */
#define TS_PIT_INTERVAL_US (UCONST(TS_TIMER_MS) * UCONST(US_IN_MS) )

/** The number of PIT ticks in a timer step */
#define TS_PIT_TICKS ( TS_PIT_INTERVAL_US / PIT_TICK_US)

/** The value that the timer is initalised to on init */
#define TS_MS_DEFAULT_VALUE 0

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

#if (defined (UNIT_TEST) && !defined(_lint))
extern volatile ts_time_t ts_time_ms;
#endif

#endif      /* TS_P_H */

