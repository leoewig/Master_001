/**
 *
 * Timer Services
 *
 * Implementation file for the Timer services feature, providing a millisecond
 * counter.
 *
 * @file
 * @ingroup TimerServices
 * $Id: ts.c 4157 2012-10-05 09:06:07Z ae4 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#include "basic_types.h"
#include "hardware.h" /* Save and restore interrupts */

#include "ts.h"         /* public header for this module */
#include "pit_cfg.h"    /* The PIT API is used to control the timer */
#include "isr.h"        /* for INT_CPU_DEST */
#include "utils.h"      /* for UCONST */
#include "ts_p.h"

/******************************************************************************
 * COMPILE-TIME CHECKS
 *****************************************************************************/

/* Check that the PIT interval doesn't overflow a U16
 * This is supposed to be UINT16_MAX but it is defined in a way that
 * can't go through the PP */

#if    TS_PIT_INTERVAL_US > UCONST(USHRT_MAX)
#error Timer services PIT overflow
#endif

/* The number in the test below comes from REQ_BMS_SUP_TS_009 which
 * specifies a 0.5% resolution, this means 1000us needs to be divided
 * into 200 'ticks'. The PIT is implemented in such a manner that a
 * tick is exactly 5us so this resolution is somewhat superfluous
 * but serves as a check if the PIT frequencies are ever changed */

#if TS_PIT_TICKS < 200u
#error PIT resolution is too low
#endif

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/
PRIVATE volatile ts_time_t ts_time_ms = TS_MS_DEFAULT_VALUE;
/**<
 * @a2l{conversion, ts_time_t}
 * @a2l{LongIdentifier, A 32-bit millisecond timestamp volatile because it can
be modified through an interrupt handler and read back in a different context}
 */

PRIVATE uint16_t ts_high_ticks = 0u;
/**<
 * @a2l{conversion, uint16_t}
 * @a2l{LongIdentifier, The high part of the time-extended counter use
ts_get_ticks() for access}
 */

PRIVATE uint16_t ts_low_ticks = 0u;
/**<
 * @a2l{conversion, uint16_t}
 * @a2l{LongIdentifier, The low part of the extended ticks counter use
ts_get_ticks() for access}
 */

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

FCODE void ts_init(void)
{
    /* Initialise all private data */
    ts_time_ms = TS_MS_DEFAULT_VALUE;

    /* set TimerServices PIT IRQ default priority */
    PitEnableInterrupts(TS_MS_PIT_CHAN, INT_LVL_DEFAULT, INT_CPU_DEST);

    /* and kick the timer off by default */
    ts_start_timers();
}

FCODE ts_time_t ts_get_ms(void)
{
    ts_time_t time_val;
    uint8_t ccr;
    /* Preserve and disable interrupts SAVE_INTERRUPTS uses
     * DISABLE_INTERRUPTS()*/
    SAVE_INTERRUPTS(ccr);
    /* Store a coherently captured timestamp on the stack */
    time_val = ts_time_ms;
    /* Put back the interrupt context the way it was */
    RESTORE_INTERRUPTS(ccr);
    return time_val;
}

/* Start the timer services PITs */
FCODE void ts_start_timers(void)
{
    PitStart(TS_MS_PIT_CHAN, TS_PIT_TICKS);
    PitStart(TS_TICKS_PIT_CHAN, TS_TICKS_PIT_MAX_VALUE);
	PitStart(TS_TICKS_A_PIT_CHAN, TS_TICKS_PIT_MAX_VALUE);
}

/* Stop the timer services PITs */
FCODE void ts_stop_timers(void)
{
    PitStop(TS_MS_PIT_CHAN);
    PitStop(TS_TICKS_PIT_CHAN);
	PitStop(TS_TICKS_A_PIT_CHAN);
}

FCODE bool_t ts_is_elapsed_ms(ts_time_t newval, ts_time_t oldval, ts_time_t interval_ms)
{
    bool_t elapsed;
    ts_time_t time_delta;
    /* timers will wrap-around at the limit, overflow is intentional*/
    time_delta = (newval - oldval);
    /* Comparison is >=0 so if the interval is 0, a timestamp is always expired
     * compared to itself */
    elapsed = (bool_t) (time_delta >= interval_ms);

    return elapsed;
}

FCODE ts_ticks_t ts_get_ticks(void)
{
    uint8_t ccr;
    uint16_t newticks;
    uint32_t result;
    SAVE_INTERRUPTS(ccr);
    /* The PIT counts downwards from the upper limit, so switch it around */
    newticks = TS_TICKS_PIT_MAX_VALUE - TS_TICKS_PIT_COUNT;

    if (newticks < ts_low_ticks)
    {
        /* counter has wrapped */
        ++ts_high_ticks;
    }
    ts_low_ticks = newticks;
    result = ((uint32_t) ts_high_ticks << 16) | newticks;
    RESTORE_INTERRUPTS(ccr);
    return result;
}

INTERRUPT void ts_pit_interrupt(void)
{
    ts_time_ms += TS_TIMER_MS;
    /* Clear the timeout flag for the PIT */
    PITCLRINTFLG(TS_MS_PIT_CHAN);
}

/******************************************************************************
 * TEST HOOKS
 *****************************************************************************/

#if defined(UNIT_TEST)
FCODE void ts_set_ms(int32_t value)
{
    uint8_t ccr;
    SAVE_INTERRUPTS(ccr);
    ts_time_ms=value;
    RESTORE_INTERRUPTS(ccr);
}

FCODE void ts_set_ticks(ts_ticks_t value)
{
    uint8_t ccr;
    ts_high_ticks = (uint16_t)(value >> 16);
    ts_low_ticks = (uint16_t)(value & 0xFFFFu);
    SAVE_INTERRUPTS(ccr);
    TS_TICKS_PIT_COUNT = UINT16_MAX - ts_low_ticks;
    RESTORE_INTERRUPTS(ccr);
}

#endif

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
