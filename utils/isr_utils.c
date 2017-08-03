/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   isr_utils.c
 *
 * description:  
 *
 * Implementation for functions manipulating interrupt vector registers on the
 * S12 CPU
 *
 * #### Conditional compilation
 * uses `if !defined BBLK && !defined VTBM` to leave xgate header gate.h out 
 * bootblock and VTBM builds
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/


/******************************************************************************
 * INCLUDES
 *****************************************************************************/

/**
 *
 * Implementation for functions manipulating interrupt vector registers on the
 * S12 CPU
 *
 * #### Conditional compilation
 * uses `if !defined BBLK && !defined VTBM` to leave xgate header gate.h out 
 * bootblock and VTBM builds
 *
 * @file
 * @ingroup utils
 */


#include "basic_types.h"
#include "isr.h"        /* public header for this module */
#include "hardware.h"   /* For INT_CF */
#if !defined BBLK && !defined VTBM
#include "gate.h"       /* for xgate functions */
#endif

/**
 * @lint Improve : Symbol not referenced, but may be useful in  the future, so suppress for
 * now\n
 * lint -esym(528,get_interrupt_level)
 */
/*lint -esym(528,get_interrupt_level) */
static uint8_t get_interrupt_level(uint8_t vect_addr);

/******************************************************************************
 * COMPILE-TIME CHECKS
 *****************************************************************************/

/*
 * PUBLIC DATA (documented in header)
 */


/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/


/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/**
 * Route all I-maskable interrupts to the CPU and disable them
 */
void init_interrupts(void)
{
    uint8_t i;

    for (i = MIN_IVECT_ADDR; i <= MAX_IVECT_ADDR ; i++)
    {
        (void)set_interrupt_level(i, 0u, INT_CPU_DEST);
    }

    gate_init_maskable_error_interrupts();

}
/*!
 * @brief   Set level and destination of the interrupt.
 * @return  INT_E_OK or INT_E_NOT_OK
 */
uint8_t set_interrupt_level(uint8_t vect_addr, uint8_t level, uint8_t dest)
{
    uint8_t ret_val;

    if ((vect_addr <= MAX_IVECT_ADDR)
	    && (level <= INT_LVL_MAX)
	    && ((INT_CPU_DEST == dest) || (INT_XGATE_DEST == dest)))
    {
        INT_CFADDR = vect_addr & 0xF0u;
        INT_CFDATA[(vect_addr & 0x0Fu) >> 1] = level | dest;

        ret_val = INT_E_OK;
    }
    else
    {
        ret_val = INT_E_NOT_OK;
    }

    return ret_val;
}

/**
 * @todo these interrupt functions need documenting */

static uint8_t get_interrupt_level(uint8_t vect_addr)
{
    uint8_t ret_val = 0u;  /* set default if vect_addr out of range */

    if (vect_addr <= MAX_IVECT_ADDR)
    {
        INT_CFADDR = vect_addr & 0xF0u;
        ret_val = INT_CFDATA[(vect_addr & 0x0Fu) >> 1];
    }

    return ret_val;
}


/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

