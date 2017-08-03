/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    pit_types.h
 *
 * description:  Header file for Periodic Interval Timer
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#ifndef PIT_TYPES_H
#define PIT_TYPES_H
/**
 * @file
 *
 * Header file for Periodic Interval Timer
 *
 */

#define PIT_TICK_US       5u   /**< Length of one timer tick in micro seconds
				@note The period is hardcoded so changing
				this won't change the tick time! */
#define USEC_TO_TICKS(us)       ((us)/PIT_TICK_US) /**< Converts microseconds
                                                   to timer ticks */
#define TICKS_TO_USEC(ticks)       ((ticks)*PIT_TICK_US) /**< Converts ticks to
                                                   microseconds */

typedef enum{
  PitChannel0,
  PitChannel1,
  PitChannel2,
  PitChannel3,
  PitChannel4,
  PitChannel5,
  PitChannel6,
  PitChannel7,
  PitChannelCount
} PitChannelType;


#define PITCOUNTER2(n_)   ( PITCNT ## n_ )
#define PITCHANNEL2(n_)   (PitChannel ## n_ )

/** Macro to help keep calls to PIT API and access to registers consistent */
#define PITCOUNTER(n_)  PITCOUNTER2(n_)
/** Macro to help keep calls to PIT API and access to registers consistent */
#define PITCHAN(n_)  PITCHANNEL2(n_)

/* PitChannel4 is allocated to TICKS_PIT */

#endif /* PIT_TYPES_H */
