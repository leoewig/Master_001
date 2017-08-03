/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  gate_types.h
 *
 * description:  
 * Header file for S12 code which configures and interfaces with the XGATE
 * 
 * 
 *  
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
/**
 * @file
 *
 * Header file for S12 code which configures and interfaces with the XGATE
 *
 * $Id: gate.h 2777 2012-07-06 12:15:44Z tg10 $
 */
/**
 *
 * <h1> XGATE overview </h1>
 *
 * The XGATE is intended to reduce the processor load on the S12 by providing
 * a convenient means of manipulating I/O data in interrupt service routines.
 * It runs at twice the frequency of the S12 and has a reasonably powerful
 * instruction set apart from the notable absence of a divide instruction.
 *
 * The XGATE can access 2k of registers, 30k of flash and 32k of RAM.  The
 * RAM can be accessed faster than flash so it is usual to copy XGATE code
 * into RAM and execute it from there.
 *
 * I-maskable interrupts (apart from IRQ and the high temperature interrupt)
 * can be routed to the XGATE instead of the S12.  Among the I-maskable
 * interrupts are eight XGATE software triggers which provide a means of
 * running an XGATE function independently of external I/O interrupts.
 *
 * The XGATE runs only in response to an interrupt and is idle at other times:
 * there is no XGATE background loop.
 *
 * Tasks must be configured as high or low priority.  A high priority task can
 * interrupt a low priority task.  High and low priority tasks run with a
 * separate stack and separate registers so there is no overhead to a context
 * switch.  The separate registers are hidden from the user, the context switch
 * just works by magic.
 *
 * <h1> XGATE and the build system </h1>
 *
 * The makefiles must be configured to treat C files starting with an 'x' as
 * XGATE source files.  Handwritten XGATE sources must \#define _XGATE_
 * [this is not done automatically to simplify the rules for statically
 * analysing XGATE and regular C files].
 *
 * XGATE code <em> must </em> by compiled with the <tt> -xe </tt> switch as
 * by default Cosmic will overwrite the stack-pointer and thus break the
 * high/low priority mechanism.
 *
 * <h1> Triggering an XGATE task from an interrupt </h1>
 *
 * To trigger an XGATE task from an interrupt, add it to the #xvector[] array
 * and enable it by calling set_interrupt_level() with the destination set to
 * #INT_XGATE_DEST.   Note that xvector[] elements contain both a function name
 * and a pointer to an arbitrary structure containing arguments for the task.
 * A typical use for the arguments structure would be to, for example, point to
 * the I/O channel causing the interrupt.  This allows the same task to be
 * specified for multiple channels of the same type.
 *
 * <h1> Triggering an S12 interrupt from the XGATE </h1>
 *
 * The SIF instruction (accessible through Cosmic's _sif() function) is used by
 * the XGATE to trigger an interrupt on the S12.  By default, the interrupt
 * triggered will be the one corresponding to the currently executing XGATE
 * task (e.g. the PIT 0 XGATE task will trigger the PIT 0 S12 interrupt).  If
 * _sif is called with a channel ID, then the interrupt corresponding to the ID
 * is triggered (provided that channel ID corresponds to a vector configured
 * by set_interrupt_level() with #INT_XGATE_DEST).
 *
 * An S12 interrupt service routines must be written with the knowledge of
 * whether it is triggered by hardware or the XGATE so that it can clear the
 * interrupt correctly.  An interrupt from the XGATE is cleared by writing to
 * the correpsonding bit in XGIF.
 *
 * <h1> S12/XGATE initialisation and halting </h1>
 *
 * XGATE code is copied into RAM by the processor start-up code.
 *
 * Two functions are required to initialise the XGATE: gate_init(), which can
 * be called at any time and gate_init_maskable_error_interrupts() which must be
 * called after init_interrupts().
 *
 * If required, gate_stop() can be called to halt the XGATE.  gate_stop()
 * blocks the S12 until it has succeeded in disabling the XGATE.
 *
 * @defgroup s12_xgate S12 XGATE Interface
 * @ingroup platform
 * @{
 * @file
 * @}
 */

#ifndef GATE_H
#define	GATE_H

#include "basic_types.h"
#include "gate_types.h"

/**
 * Initialise XGATE prior to use
 *
 * If the XGATE is idle and requests are disabled, performs the following:
 * <ul>
 * <li> Initialises vector base address to point to xvector[] array
 * <li> Initialises low and high priority initial stack pointers to addresses
 * 	specified in the .lkf file
 * <li> Zeros the .xbss memory segment
 * </ul>
 * Regardless of current XGATE state, does the following:
 * <ul>
 * <li> Configures XGATE to stop in Freeze (BDM) Mode
 * <li> Enables outgoing XGATE interrupts
 * <li> Enables the XGATE
 * </ul>
 *
 */
void gate_init(void);

/**
 * Stop the XGATE when the current executing thread (if any) finishes
 *
 * Clears the XGE bit in XGMCTL while waiting for the XGATE to become idle.
 *
 * @note This function will block until any currently executing XGATE thread
 * finishes.
 *
 * Blocking is necessary because after this function clears XGE, an executing
 * thread could re-set it.  This is plausible if the executing thread happens
 * to be in a critical section where it has temporarily disabled interrupts.
 */
void gate_stop(void);

/**
 * Initialise the S12 handlers which are called when the XGATE takes an
 * unexpected interrupt or executes the INTERNAL_ERROR() macro
 *
 * Marks the vectors as being handled by XGATE (seems perverse but necessary to
 * allow the XGATE to trigger the S12 interrupt via a SIF instruction) and
 * sets them to the highest priority.
 */
void gate_init_maskable_error_interrupts(void);

/**
 * <b> XGATE </b> interrupt installed as a handler for all unused
 * interrupts.
 * Uses the SIF mechanism to trigger isr_xgate_unexpected_interrupt() on
 * the S12.
 * Performs the following:
 *	<ol>
 *	<li> Saves the active channel number in xg_unhandled_chnl
 *	<li> Disables the interrupt (because we don't know how to acknowledge
 *	     it and otherwise we'll be called again as soon as we exit)
 *	<li> Calls the S12 interrupt handler for the channel specified
 *  	     by the macro CHANNEL_FOR_UNHANDLED_XGATE_INTERRUPT
 *	</ol>
 */
INTERRUPT void x_unhandled_isr(void);

/**
 * S12 interrupt triggered by the SIF mechanism when the XGATE handles an
 * interrupt it shouldn't.
 *
 * It will halt in the debugger if attached and then call sch_isr_Unhandled()
 * to disable the interrupt and log the fault.
 *
 * As it's called by the SIF mechanism, it can be installed in any unused
 * I-maskable interrupt vector as specified by
 * #CHANNEL_FOR_UNHANDLED_XGATE_INTERRUPT
 */
INTERRUPT void isr_xgate_unexpected_interrupt(void);


/**
 * S12 interrupt triggered by the SIF mechanism when the XGATE executes the
 * INTERNAL_ERROR macro.
 *
 * It will halt in the debugger if attached.
 *
 * As it's called by the SIF mechanism, it can be installed in any unused
 * I-maskable interrupt vector as specified by
 * #CHANNEL_FOR_XGATE_INTERNAL_ERROR
 */
INTERRUPT void isr_xgate_internal_error(void);

/**
 * XGATE channel on which an unexpected interrupt has occurred
 */
extern FRAM uint16_t xg_unhandled_chnl;

/**
 * XGATE program counter when an internal error occured
 */
extern FRAM uint16_t xg_internal_error_pc;

#endif
