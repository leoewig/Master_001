/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    prodder_types.h
 *
 * description:  Prodder declarations
 *
 *
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
#ifndef PRODDER_TYPES_H
#define PRODDER_TYPES_H
/**
 * @file
 * @defgroup prodder Prodder
 *
 * Prodder declarations
 *
 * $Id: prodder_types.h 8740 2013-05-30 14:37:19Z pr4 $
 */

/**
 * PIT channel used for RTW code on XGATE
 *
 * must be manually kept consistent with location of xgpit_rtw_isr
 * in xvector[] array
 */
#define RTW_PIT   PitChannel1

/**
 * PIT channel used for RTC emulator
 */
#define RTC_EMU_LOOP_PIT     PitChannel2

/**
 * software trigger used to call RTW initialisation code
 *
 * must be manually kept consistent with location of xgswt_rtw_init
 * in xvector[] array
 */
#define   XGSWT_FOR_RTW_INIT    0u



#endif /* ifndef PRODDER_TYPES_H */
