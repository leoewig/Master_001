/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    pit.h
 *
 * description:  Header file for Periodic Interval Timer
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
 * Header file for Periodic Interval Timer
 *
 */

/**
 * Functions and macros to control the PIT.
 * To use a PIT channel interrupt:
 * <ol><li>install the CPU or XGATE interrupt service routine in interrupts.c
 * The interrupt handler must clear the interrupt flag.  The #PITCLRINTFLG is
 * provided to do this.
 * <li>Call the following intiialisation code snippet:
 * @code
 * PitInit();
 * PitEnableInterrupts(chnl,destination);
 * PitStart(chnl,period);
 * @endcode
 * </ol>
 *
 * @par Conditional Compilation
 * @par ENABLE_DYNAMIC_CHECKING
 * Checks supplied arguments are within range and halts to the debugger if not
 *
 * @addtogroup pit Periodic Interval Timer
 * @ingroup platform
 * @{
 *  @file
 * @}
 *
 */


#ifndef PIT_H
#define PIT_H

#include "basic_types.h"
#include "pit_types.h"

#ifndef _XGATE_ /* hide S12-only functions from XGATE code */

/**
 * Initilaize PIT peripherial
 *
 * This function enables the PIT and sets up all channels to run from a single
 * micro-timer with a 5us tick, which gives a range of PIT periods from 5us
 * to 327ms.
 *
 * Must be called once before PIT channels are started.
 */
extern FCODE void PitInit(void);

/**
 * Starts a PIT channel counting with the supplied period
 *
 * If the channel is already counting, the channel counter will be
 * reloaded with the supplied period.
 * @param Channel selects PIT peripherial that shall be started
 * @param Period  number of 5us ticks to timeout
 *
 * PitInit() must have been called once before any calls to this function
 */
extern FCODE void PitStart(const PitChannelType Channel, const uint16_t Period);

/**
 * Stop PIT channel
 *
 * Stops counting of PIT and doesn't reset the clock.
 *
 * @param Channel selects PIT peripherial that shall be stopped.
 *
 */
extern FCODE void PitStop(const PitChannelType Channel);

/**
 * Enables interrupt and selects priority and interrupt-handling core for a
 * PIT channel
 *
 * The interrupt priority is set to INT_LVL_DEFAULT
 * @param Channel  channel to enable
 * @param IRQlevel interrupt priority.  1 is the lowest priority and 7 the
 *		   highest.  A priority level of 0 will disable the interrupt.
 * @param IRQDest  core to receive the interrupt,
 * 		   must be #INT_CPU_DEST or #INT_XGATE_DEST
 * The interrupt handler must have been installed at the correct vector
 * for the chosen core.
 */
extern FCODE void PitEnableInterrupts(const PitChannelType Channel,
					uint8_t IRQlevel, uint8_t IRQDest);
/**
 * Disables interrupts for the given PIT channel
 */
extern FCODE void PitDisableInterrupts(const PitChannelType Channel);

/**
 * ISR to schedule foreground loop
 */
extern INTERRUPT void isr_pit_fgloop(void);

#endif /* ifndef _XGATE_ */

/**
 * macro to clear a PIT channel interrupt flag
 * @param chnl PIT channel number to clear
 */
#define	PITCLRINTFLG(chnl)	(PITTF = (uint8_t)(1u << (uint8_t) (chnl)))


#endif /*PIT_H */
