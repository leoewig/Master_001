/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  gate_types.h
 *
 * description:  
 * Header file for S12 code which configures and interfaces with the XGATE
 * 
 * 
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
#ifndef GATE_TYPES_H
#define	GATE_TYPES_H
/**
 * @file
 *
 * Header file for S12 code which configures and interfaces with the XGATE
 *
 */

/**
 * macro to acknowledge an interrupt from the XGATE to the S12 for a given
 * XGATE channel number.
 *
 * This is done by writing a 1 to the channel's bit in the XGIF array.  XGIF
 * is an array of 8 16-bit words.  Integer division by 16 is used to get the
 * word number.  The bit number comes from the remainder
 */

#define	CLEAR_XGIF_FOR_CHANNEL(channel)                 \
                (XGIF[7u - (((uint8_t) (channel))/16u)]    \
                        = ((uint16_t) 1u << (((uint8_t) (channel)) % 16u)))
/**
 * macro to acknowledge an interrupt from the XGATE to the S12 for a given
 * S12 interrupt offset
 *
 * This is done by calling #CLEAR_XGIF_FOR_CHANNEL with the corresponding
 * channel number, which is half the offset
 */
#define	CLEAR_XGIF_FOR_OFFSET(offset)	(CLEAR_XGIF_FOR_CHANNEL((offset)/2u))

/**
 * XGATE channel hi-jacked to allow us to report any unexpected XGATE
 * interrupts to the S12.  Currently set to the SCI7 channel.
 *
 * @note value must be consistent with the vector offset of
 * isr_xgate_unexpected_interrupt() in #jtable[]
 */
#define	CHANNEL_FOR_UNHANDLED_XGATE_INTERRUPT	0x2Bu	/* SCI7 channel */

/**
 * XGATE channel hi-jacked to allow us to report any unexpected XGATE
 * interrupts to the S12.  Currently set to the SCI5 channel.
 *
 * @note value must be consistent with the vector offset of
 * isr_xgate_internal_error() in #jtable[]
 */
#define	CHANNEL_FOR_XGATE_INTERNAL_ERROR	0x42u	/* SCI5 channel */

/**
 * set an XGATE software trigger by writing the trigger number and mask to XGSWT
 * @param N	Trigger number to set (must be in range 0 to 7)
 */
#define		SET_XGSWT(N)	\
    			(XGSWT = (uint16_t)((uint16_t) 1u << ((N)+8u)) + ((uint16_t) 1u << (N)))
/**
 * clear an XGATE software trigge by writing just the trigger mask to XGSWT
 * @param N	Trigger number to clear (must be in range 0 to 7)
 */
#define		CLEAR_XGSWT(N)	(XGSWT = (uint16_t)((uint16_t) 1u << ((N)+8u)))

/*
 * XGMCTL bits
 */
#define XGIE	0x0001u	/**< XGMCTL.XGIE interrupt enable */
#define XGSWEF	0x0002u	/**< XGMCTL.XGSWEF software error flag */
#define XGFACT	0x0008u	/**< XGMCTL.XGFACT fake activity */
#define XGSS	0x0010u	/**< XGMCTL.XGSS single step */
#define XGDBG	0x0020u	/**< XGMCTL.XGDBG enable debug mode */
#define XGFRZ	0x0040u	/**< XGMCTL.XGFRZ stop in freeze mode */
#define XGE	0x0080u	/**< XGMCTL.XGE xgate enable */
#define XGIEM	0x0100u	/**< XGMCTL.XGIEM interrupt enable mask */
#define XGSWEFM 0x0200u	/**< XGMCTL.XGSWEFM software error flag mask */
#define XGFACTM	0x0800u	/**< XGMCTL.XGFACTM fake activity mask */
#define XGSSM	0x1000u	/**< XGMCTL.XGSSM single step mask */
#define XGDBGM	0x2000u	/**< XGMCTL.XGDBGM enable debug mode mask */
#define XGFRZM	0x4000u	/**< XGMCTL.XGFRZM stop in freeze mode mask */
#define XGEM	0x8000u	/**< XGMCTL.XGEM xgate enable mask */
#endif /* GATE_TYPES_H */
