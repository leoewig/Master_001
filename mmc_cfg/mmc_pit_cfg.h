/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_pit_cfg.h
 *
 * description:  
 * PIT channel position definition for MMC target.
 * The positions given in this file must match those specified in the vectors
 * for the target.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/
#if !defined(MMC_PIT_CFG_H)
#define      MMC_PIT_CFG_H


/******************************************************************************
 * INCLUDES
 *****************************************************************************/

/** 
 * If a module is using this header, it almost certainly wants the pit
 * definitions as well */
#include <pit.h>

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

  /** @lint -esym(755,FGLOOP_PIT) These are used for the MMC target, but the
 harness build complains */
 
 /*lint -esym(755,FGLOOP_PIT) */

#define FGLOOP_PIT   PITCHAN(0) /**< PIT used to schedule the FG loop */
#define AD_PIT   PITCHAN(1) /**< PIT used to schedule AD */
#define AD_PIT_TICKS 100	/* 500 us */

/** The PIT channel used for Timer services millisecond timer
 *  If this is changed, also check the vectors and settings in pit.h
 */
#define TS_MS_PIT_NUM       3
/** The PIT channel number used for the wrap-around ticks counter. */
#define TS_TICKS_PIT_NUM    4

/** PIT used by timer services */
#define TS_MS_PIT_CHAN           PITCHAN(TS_MS_PIT_NUM)

/** The channel number used for the timer services 'ticks' counter */
#define TS_TICKS_PIT_CHAN          PITCHAN(TS_TICKS_PIT_NUM)
/** The pit counter register used for the timer services 'ticks' counter */
#define TS_TICKS_PIT_COUNT         PITCOUNTER(TS_TICKS_PIT_NUM)



/** The PIT channel number used for the wrap-around ticks counter. */
#define TS_TICKS_A_PIT_NUM    2
/** The channel number used for the timer services 'ticks' counter */
#define TS_TICKS_A_PIT_CHAN          PITCHAN(TS_TICKS_A_PIT_NUM)
/** The pit counter register used for the timer services 'ticks' counter */
#define TS_TICKS_A_PIT_COUNT         PITCOUNTER(TS_TICKS_A_PIT_NUM)

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

#endif      /* MMC_PIT_CFG_H */

