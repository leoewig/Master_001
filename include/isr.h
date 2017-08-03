/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    isr.h
 *
 * description: Header file for utilities for manipulating interrupts on the S12 CPU
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
#ifndef ISR_H
#define ISR_H
/**
 *
 * Header file for utilities for manipulating interrupts on the S12 CPU
 *
 * @file
 * @ingroup interrupts
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/*******************************************************************************
 *
 * CONFIDENTIAL - Linkedriving Shanghai
 *
 * This is an unpublished work, which is a trade secret, created in 2006.
 * Linkedriving Shanghai owns all rights to this work and intends to maintain
 * it in confidence to preserve its trade secret status. Linkedriving Shanghai
 * reserves the right to protect this work as an unpublished copyrighted
 * work in the event of an inadvertent or deliberate unauthorized publication.
 * Linkedriving Shanghai also reserves its rights under the copyright laws to
 * protect this work as a published work. Those having access to this work
 * may not copy it, use it or disclose the information contained in it without
 * the written authorization of Linkedriving Shanghai.
 *
 *******************************************************************************
 */
/**
 * Interrupt service routines and utilities
 * @defgroup interrupts ISR Interrupt handling
 * @ingroup platform
 */

#include "isr_types.h"

extern void     init_interrupts(void);

extern uint8_t  set_interrupt_level(uint8_t vect_addr, uint8_t level,
	uint8_t dest);

extern FCODE void sch_isr_Unhandled(uint8_t vect_offset);
#endif
