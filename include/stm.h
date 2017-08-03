/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   stm.h
 *
 * description:  
 * External header file for stack monitor
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
#if !defined(STM_H_)
#define      STM_H_
/**
 *
 * External header file for stack monitor
 *
 * @file
 * @ingroup stm
 */

/* Copyright (c) Linkedriving Tech 2013 - All rights reserved */

/**
 * Stack usage monitor
 *
 * <h2> STM Overview </h2>
 *
 * <h2> Approach used </h2>
 * Start at the stack pointer and fill down to the bottom of the allocated
 * region with a known value.
 *
 * During run time,  scan up from the bottom until a value which does not match
 * the fill value is found. This determines the free space in the stack.
 *
 * <h2> How to use </h2>
 * Call stm_init() somewhere close to the start to fill the stack with a
 * given value.
 *
 * Periodically call stm_update() to update the monitor of how much is in
 * use.
 *
 * @defgroup stm Stack Monitor
 * @ingroup utils
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/


void stm_init(void);
void stm_update(void);

#endif      /* STM_H_ */

