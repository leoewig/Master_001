/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    stm.c
 *
 * description:  Stack monitor implementation
 * 
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/


#include "basic_types.h"
#ifdef MMC
#include "fm.h"
#endif
#include "stm.h"     /* public header */
#include "stm_p.h"

/******************************************************************************
 * COMPILE-TIME CHECKS
 *****************************************************************************/

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

/** The number of bytes of free stack determined by probing */
PRIVATE int16_t stm_free_stack=0;

DEBUGVAR(stm_free_stack)

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

#ifndef HOSTED
/**
 * External assembler symbol pointing to the stop of the reserved stack area
 **/
extern uint8_t  _stack;
/*lint -esym(526,_stack) Symbol not defined */
/**
 * External assembler symbol pointing to the bottom of the reserved stack area
 **/
extern uint8_t  _stack_low;
/*lint -esym(526,_stack_low) Symbol not defined */
#endif

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/*****************************************************************************
 * Stack checking
 ****************************************************************************/

/**
 * Fill the S12 stack RAM with a known value, starting at the current stack
 * register and working downwards */
void stm_init(void)
{
    uint8_t * stackp;
    stackp=STM_GET_SP(); /* Store the stack pointer in D */
    while (stackp>STM_LOW_ADDR) /*lint !e946 Relational or subtract operator
                                  applied to pointers [MISRA 2004 Rule 17.3] */
    {
        /* decrement stackp first (so that it always points below SP) and write
         * the reserved value into the new location*/
        *(--stackp)=STM_FILL_VAL;
    }
}


/**
 * Walk upwards from the bottom of the stack, to find how much is still
 * available.
 * The result is placed in stm_free_stack  */
void stm_update(void)
{
    uint8_t * highp; /* even address at or below SP */
    uint8_t * checkp;   /* inspection pointer */
    uint8_t * basep;  /* the first even address above stack_low */

    /* Move the current value of the stack pointer to D */
    highp=STM_HIGH_ADDR;
    /* Set basep and lowp to point to the first even address above _stack_low */
    basep=STM_LOW_ADDR;
    checkp=basep;
    /* Move upwards from stack_low as long as the desired word pattern is
     * found. As belt and braces, stop at stackp */

    /*lint --e{946} Relational or subtract operator applied to pointers [MISRA
     * 2004 Rule 17.3] */

    while ( (STM_FILL_VAL == *checkp) && (checkp<highp) ) /*lint !e681 loop is not entered */
    {
        ++checkp;
    }

    /* Calculate the number of bytes difference */
    /*lint --e{960} Note 960: Violates MISRA 2004 Required Rule 10.3,
    Prohibited cast of complex integer expression: Casting to larger type
     */

    /* The above lint diagnostic seems to be innacurate, the pointer difference is */
    stm_free_stack =(ptrdiff_t)(checkp - basep);

#ifdef MMC
    if (stm_free_stack<=0) {
        /* The fault is updated only inside the condition, so that a good stack
         * will never down-count it */
        (void)fm_update_fault_filter(Fm_internal_error_unexpected,
                Fm_general_failure_information,TRUE,TRUE);
    }
#endif
}
