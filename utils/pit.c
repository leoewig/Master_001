/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   pit.c 
 *
 * description: PIT peripheral device driver
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


/******************************************************************************
 * INCLUDES
 *****************************************************************************/


#include "hardware.h"
#include "pit.h"
#include "isr.h"
#include "main_isr.h"

/** @polyspace<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes */
static const uint8_t PitIntVectAddrs[PitChannelCount] =
{
  VECT_OFFSET_PIT0, VECT_OFFSET_PIT1, VECT_OFFSET_PIT2, VECT_OFFSET_PIT3,
  VECT_OFFSET_PIT4, VECT_OFFSET_PIT5, VECT_OFFSET_PIT6, VECT_OFFSET_PIT7
};

FCODE void PitInit(void)
{
  /* Enable pit module, don't stop in wait mode, stops in freeze mode */
  PITCFLMT = 0xA0u;

  /* Don't force loading couters now */
  PITFLT = 0x00u;

  /* Micro Timer Load Register 1
   * set 1 tick to be 5us on 50Mhz bus */
  PITMTLD1 = 249u ; /* 250 (i.e. 249+1) cycles per tick */

  /* Multiplex Register: All channels have source Micro Timer 1 */
  PITMUX = 0xFFu;

  return;
}

FCODE void PitEnableInterrupts(const PitChannelType Channel, uint8_t IRQlevel,
    uint8_t IRQDest)
{
#ifdef ENABLE_DYNAMIC_CHECKING
  if (Channel >= PitChannelCount)         /* Is the requested channel valid?        */
  {
    INTERNAL_ERROR;
  }
#endif
#if defined (ON_TARGET_TEST) || !defined(HOSTED)
  /** @todo Do we need this for hosted testing? */
  (void)set_interrupt_level(PitIntVectAddrs[Channel], IRQlevel, IRQDest);
#endif
  PITINTE |= (uint8_t)(0x01u << (uint8_t) Channel);

  return;
}

FCODE void PitDisableInterrupts(const PitChannelType Channel)
{
#ifdef ENABLE_DYNAMIC_CHECKING
  if (Channel >= PitChannelCount)         /* Is the requested channel valid?        */
  {
    INTERNAL_ERROR;
  }
#endif
  PITINTE &= (uint8_t) ~(uint8_t) (0x01u << (uint8_t) Channel);

  return;
}

/**
 * array of PIT channel h/w periods and counters
 *
 * The PIT hardware contains a period (PITLDx) and count (PITCNTx) register
 * for each channel, conveniently arranged sequentially so we can access
 * them as an array of structures.  This gives us a tidy method of addressing
 * the setting the period for a given channel
 *
 * @lint Deviation Required : !e430 use of @ in definition\n
 * lint -esym(551,pitchannel) not accessed\n
 * lint -esym(754,pitchannel_s::count) member not used\n
 * lint -esym(830,pitchannel) member not used
 */
/*lint -esym(551,pitchannel)  not accessed */
/*lint -esym(754,pitchannel_s::count) member not used */
/*lint -esym(830,pitchannel) member not used */
/** @polyspace:begin<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes */
static struct pitchannel_s {
    uint16_t period ; /**< channel period */
    /**
     * @lint Investigate :
     * lint !e754 not used
     * lint !e830 not used
     */
    uint16_t count ; /*lint !e754 not used *//*lint !e830 not used *//**< channel counter */
    /**
      * @lint Deviation Required : !e430 allow use of @
      * lint !e430 allow use of @
     */
} ABS_LOCATE(pitchannel[PitChannelCount],&PITLD0) ; /*lint !e430 allow use of @ */
/** @polyspace:end<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes */

/* main documentation in header */
/**
 * @par Implementation
 * The value stored in the period register is one less than the Period
 * argument to account for the PIT hardware characterstics
 */
FCODE void PitStart(const PitChannelType Channel, const uint16_t Period)
{
#ifdef ENABLE_DYNAMIC_CHECKING
    if (Channel >= PitChannelCount || Period == 0u)
    {
    INTERNAL_ERROR; /* input out of range */
    }
    else
    {
    /* do nothing */
    }
#endif
    /* Load Register */
    pitchannel[Channel].period = Period - 1u;

    /* Reload timer register */
    PITFLT = (uint8_t)(0x01u << (uint8_t) Channel);

    /* Enable channel */
    PITCE |= (uint8_t)(0x01u << (uint8_t) Channel);

    return;
}

FCODE void PitStop(const PitChannelType Channel)
{
#ifdef ENABLE_DYNAMIC_CHECKING
    if (Channel >= PitChannelCount)
    {
    INTERNAL_ERROR; /* input out of range */
    }
    else
    {
    /* do nothing */
    }
#endif
    /* Disable channel */
    PITCE &= (uint8_t) ~ (uint8_t) (0x01u << (uint8_t) Channel);

    return;
}
