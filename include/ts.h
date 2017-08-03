/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     ts.h
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
#if !defined(TS_H)
#define      TS_H
/**
 *
 * Header file for timer services feature
 *
 *
 * @file
 * @ingroup TimerServices
 * $Id: ts.h 11359 2013-11-21 13:57:27Z pr4 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 * Uses two PIT timer channels in order to provide timer services to other MMC
 * features. The first PIT timer is configured to request interrupt each millisecond,
 * in which ISR a 32-bit millisecond timer abstraction is calculated. The second
 * PIT timer is running in free running, non interrupt mode and provides 32-bit
 * tick counter abstraction for fine timings.
 *
 * Details
 * -------
 * The PIT timer #TS_MS_PIT_CHAN is set up by ts_init() such
 * that it will trigger an interrupt once a millisecond. The handler for this
 * interrupt will increment a 32-bit counter which is then accessed via
 * ts_get_ms() by other features to provide a millisecond timer. The PIT
 * timer #TS_TICKS_PIT_CHAN is configured to free running mode, not requesting
 * any interrupt and is directly read by ts_get_ticks() function providing
 * tick count and overflow (wrap around) count joined to a single 32-bit tick
 * count result. Additional helper macros provide ticks conversion to microseconds
 * for convenience.
 *
 * Using this mechanism in preference to using a variable as an extension of
 * the timer avoid the need to distinguish the situations where the
 * software-extended timer variable is read at the point where the PIT has
 * wrapped around, but the interrupt handler may or may not yet have serviced
 * it.
 *
 * Limitations
 * -----------
 * - It must be possible to service the #TS_MS_PIT_CHAN interrupt
 * within 1ms, a subsequent PIT interrupt for the same register will cause the
 * time to slip.
 * - Using a 32-bit counter, the 32-bit millisecond timestamp can go up to 24 days,
 * but is invalidated if the MMC is depowered.
 * - The resolution of millisecond timer is 1 millisecond, so comparisons using
 * it will have an error of up to one millisecond.
 * - Reading the timer is only supported from the S12. See the comment for
 * ts_pit_interrupt() if the XGATE needs to read it as well.
 * - The 32-bit ticks counter function #ts_get_ticks loses wrap, if it is not
 * called for a whole wrap-around period, which is approximately 327
 * milliseconds
 * - The resolution of tick / microsecond timer is 5 us, so comparisons using
 * it will have an error of up to 5 microseconds.
 *
 * How to use
 * ----------
 * - Call ts_get_ms() to obtain a millisecond counter
 * - Call ts_is_elapsed_ms() to determine whether a time interval has elapsed
 *
 * If you know that timerB and timerA are taken within the wrap-around limit of
 * each other, you can check whether timerB occurs before timerA using:
 *
 *  @code if ( (timerB - timerA) < 0 ) { Executed when timerB is before timerA
   } @endcode
 *
 * Note that the timers could be negative as well as positive.
 *
 * - Call ts_get_ticks() to obtain a tick counter value
 *
 * Checking for an expired millisecond timer
 * -----------------------------------------
 * Two's complement maths means that this will work even if one or both of
 * the timers has wrapped around the limit.
 *
 * To check that a given time interval in milliseconds has expired between
 * timerB and timerA use:
 *
 * @code if ((timerB - timerA) >= INTERVAL  )
  {
      Executed when INTERVAL has elapsed
  }
  @endcode
 *
 * The ts_is_elapsed_ms() function can be used to perform this check for you
 *
 * Conversion from ticks to microseconds and checking for an expired microsecond timer
 * -----------------------------------------------------------------------------------
 * To check that a given time interval in microseconds has expired
 * use #TS_IS_ELAPSED_US macro with former tick count reading as a parameter.
 * Note that former tick count reading must not be older than approximately 327
 * milliseconds to not lose the wrap
 *
 * To calculate a time difference in microseconds between two moments represented
 * by two tick count readings use #TS_DIFF_US macro.
 *
 * Requirement Satisfaction
 * ------------------------
 *
 * | Requirement        | Satisfied By                         | Tested By                    |
 * | -------------------|--------------------------------------|------------------------------|
 * | REQ_BMS_SUP_TS_001 | ts_start_timers() ts_get_ms()        | test_ts_increment_caller()   |
 * | REQ_BMS_SUP_TS_002 | #ts_time_t ts_start_timers()         | test_timersvc_range()        |
 * | REQ_BMS_SUP_TS_003 | #TS_TIMER_MS ts_pit_interrupt()      | test_ts_increment_caller()   |
 * | REQ_BMS_SUP_TS_004 | ts_pit_interrupt() SAVE_INTERRUPTS() | _code review_                |
 * | REQ_BMS_SUP_TS_005 | #ts_time_t ts_get_ms()               | test_timersvc_wrap()         |
 * | REQ_BMS_SUP_TS_006 | ts_get_ms()                          | test_timersvc_init()         |
 * | REQ_BMS_SUP_TS_007 | ts_get_ms()                          | test_ts_intoff_read_caller() |
 * | REQ_BMS_SUP_TS_008 | #ts_time_t                           | test_timersvc_negdelta()     |
 * | REQ_BMS_SUP_TS_009 | ts_start_timers()                    | test_timersvc_interval()     |
 * | REQ_BMS_SUP_TS_010 | ts_is_elapsed_ms()                   | test_timersvc_elapsed()      |
 *
 * - test_ts_increment_caller() calls the mmc_test_harness routine test_ts_increment()
 * - test_ts_intoff_read_caller() calls the mmc_test_harness routine test_ts_intoff_read()
 *
 * @untestable REQ_BMS_SUP_TS_004
 */

 /**
 * @defgroup TimerServices Timer Services
 * @ingroup platform
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "ts_types.h"

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 * Initialize the timer services feature.
 * Configure #TS_MS_PIT_CHAN such that it will generate an interrupt once a
 * millisecond.
 * Configure #TS_TICKS_PIT_CHAN such that it will run in free running mode,
 * not generating any interrupts
 * This makes use of PitStart() to configure the appropriate PITs.
 * The length of a PIT tick is defined in #PIT_TICK_US.
 * @pre The system must call PitInit() before calling this function.
 */
FCODE void ts_init(void);

/**
 *  Obtain the most recent millisecond timestamp
 * - Preserve the interrupt context
 * - disable interrupts
 * - copy #ts_time_ms into a stack variable.
 * - restore interrupt context.
 *  @returns ts_time_t giving  the timestamp in milliseconds */
FCODE ts_time_t ts_get_ms(void);

/**
 * Check for an elapsed timer
 * @retval TRUE if newval - oldval >= interval_ms
 * @retval FALSE otherwise */
FCODE bool_t ts_is_elapsed_ms(ts_time_t newval, ts_time_t oldval,
	ts_time_t interval_ms);

/** Start the timer services PITs */
FCODE void ts_start_timers(void);

/** Stop the timer services PITs */
FCODE void ts_stop_timers(void);

/**
 * Handler for the PIT ISR
 * - Interrupts have I mask set, so no need to  preserve interrupt context
 * - Increment the timestamp counter.
 * - This timestamp will intentionally wrap around when the maximum
 * representation is exceeded.
 *
 *   If we do want to use timer on the xgate, the following could be done in
 *   the ISR. This prevents the XGATE from locking out the S12 in an interrupt.
 *
 * On S12:
 * @code
 *   ts_updating = TRUE;
 *   ts_time_ms++;
 *   ts_updating = FALSE;
 * @endcode

 *   On XGATE:
 * @code
 *   do {
 *       t = ts_time_ms;
 *   } while (ts_updating || (t != ts_time_ms));
 * @endcode
 *
 * @note This ISR manipulates millisecond PIT timer #TS_MS_PIT_CHAN only, since
 * the #TS_TICKS_PIT_CHAN is used in free running, non interrupt request mode
 */
INTERRUPT void ts_pit_interrupt(void);

/**
 * Obtain ticks counter value. The scaling is as per pit.h.
 * - Preserve the interrupt context
 * - disable interrupts
 * - copy #TS_TICKS_PIT_COUNT 16-bit PIT timer value into a stack
 * variable.
 * - determine counter has wrapped condition and update
 * high order counter variable
 * - restore interrupt context.
 * @return  a 32-bit tick counter value
 *
 * @note  If it is not called for a whole wrap-around period, the wrap is
 * missed. The wrap around period is 327.68 milliseconds.
 */
FCODE ts_ticks_t ts_get_ticks(void);

/******************************************************************************
 * TEST HOOKS
 *****************************************************************************/

#if defined(UNIT_TEST)
/**
 * A function used to seed the timer value, used for testing the wrap-around
 * behavior
 * - Preserve and disable interrupts
 * - copy value into #ts_time_ms
 * - restore interrupts.
 */
FCODE void ts_set_ms(int32_t value);

/**
 * A function used to seed the ticks counter value, used for testing the wrap-around
 * behavior
 * - Preserve and disable interrupts
 * - copy high order 16-bit value into #ts_high_ticks
 * - copy low order 16-bit value into #ts_low_ticks and update
 * #TS_TICKS_PIT_COUNT accordingly
 * - restore interrupts.
 */
FCODE void ts_set_ticks(ts_ticks_t value);

#endif

#endif      /* TS_H */

