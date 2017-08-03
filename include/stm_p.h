/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    stm_p.h
 *
 * description:  
 * Stack monitor private header
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
#if !defined(ABC_P_H)
#define      ABC_P_H
/**
 *
 * Stack monitor private header
 *
 * @file
 * @ingroup stm
 */

/* Copyright (c) Linkedriving Tech 2013 - All rights reserved */

/** The fill value used for stack checking */
#define STM_FILL_VAL ((uint8_t)'S')

#ifdef HOSTED

/*****************************************************************************
 * Definitions for hosted unit test target
 ****************************************************************************/

/* Size of stack allocated for testing */
#define STM_STACK_SIZE 2048

#define STM_GET_SP() (stm_test_sp)
#define STM_HIGH_ADDR (&stack_dummy[STM_STACK_SIZE])
#define STM_LOW_ADDR (&stack_dummy[0])

/* A dummy stack pointer used for unit testing */
extern uint8_t *stm_test_sp;
extern uint8_t stack_dummy[STM_STACK_SIZE+1];

#else /* def HOSTED */

/*****************************************************************************
 * Definitions for S12X target
 ****************************************************************************/

/*lint -emacro(718,STM_GET_SP) Symbol '_asm' undeclared  MISRA Rule 8.1 */
/*lint -emacro(746,STM_GET_SP) call to function '_asm()' not made in the
 * presence of a prototype MISRA Rule 8.1*/
/*lint -emacro(923,STM_GET_SP) cast from int to pointer [MISRA 2004 Rule 11.3] */


#define STM_GET_SP() ((uint8_t*)_asm("tfr sp,d"))
#define STM_HIGH_ADDR (&_stack)
#define STM_LOW_ADDR (&_stack_low)

#endif /* HOSTED */

#ifdef UNIT_TEST
extern int16_t stm_free_stack;
#endif

#endif      /* STM_P_H */

