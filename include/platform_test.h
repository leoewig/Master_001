/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    platform_test.h
 *
 * description:  platform_test Platform Test
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined (PLATFORM_TEST_H)
#define       PLATFORM_TEST_H
/**
 * @file
 *
 */
/**
 * Add feature documentation here!
 *
 * @defgroup platform_test Platform Test
 * @{
 * @file
 * @}
 */

#include "pit.h"

extern bool_t sw0;
extern bool_t sw1;
extern bool_t sw2;
extern bool_t sw3;

extern bool_t led0;
extern bool_t led1;
extern bool_t led2;
extern bool_t led3;

#pragma align on
struct xgpit_isr_s {
    PitChannelType channel;
};

/**
 * structure 'passed' to XGATE S/W trigger interrupt
 */
struct xgswt_isr_s {
    uint8_t channel; /**< set equal to channel causing the interrupt */
    uint16_t word;          /**< arbitrary argument to check arg passing and
                              alignment */
};

#pragma align off

/*
 * S12 interrupts
 */
extern INTERRUPT void isr_platform_test_pit1(void);
extern INTERRUPT void isr_platform_test_pit2(void);

/*
 * XGATE interrupts
 */
extern INTERRUPT void xgpit_isr(struct xgpit_isr_s args);
extern INTERRUPT void xgswt_isr(struct xgswt_isr_s args);
extern INTERRUPT void ECT_Channel1_Interrupt(void);
//extern INTERRUPT void ECT_Channel2_Interrupt(void);


/*
 * 'arguments' to the XGATE interrupts
 */
extern struct xgpit_isr_s xgpit1_args;
extern struct xgpit_isr_s xgpit2_args;
extern struct xgswt_isr_s xgswt0_args;

#endif /* PLATFORM_TEST_H */
