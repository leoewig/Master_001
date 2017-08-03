/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    XCP definitions
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
#ifndef ISR_TYPES_H
#define ISR_TYPES_H
/**
 *
 * Header file for utilities for manipulating interrupts on the S12 CPU
 *
 * @file
 * @ingroup utils
 */

#define MAX_IVECT_ADDR   0xF2U /**< highest vector offset for I-maskable
				 interrupt */
#define MIN_IVECT_ADDR   0x3CU /**< lowest vector offset for I-maskable
				 interrupt */

#define INT_CPU_DEST    0x00U
#define INT_XGATE_DEST  0x80U

#define INT_LVL_MAX     7U
/** Default interrupt level to use for interrupts with no particular
    priority requirements */
#define INT_LVL_DEFAULT 3U

#define INT_E_OK        0x00U
#define INT_E_NOT_OK    0X01U

#endif /* ISR_TYPES_H */
