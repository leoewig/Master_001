/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      mmc_ican_cfg.h 
 *
 * description:  
 * MMC Internal CAN configuration header
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/
#if !defined(MMC_ICAN_CFG_H)
#define      MMC_ICAN_CFG_H
/**
 *
 * MMC Internal CAN configuration header
 *
 * @file
 * @ingroup mmc_ican
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/
/**
 * Internal bus channel number
 *
 * This is the _physical_ controller number
 * @note if the channel is changed, the CAN interrupt handlers must also be
 * moved to new vectors
 */
#define         CAN_INTERNAL_BUS                2u      

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/**
 * List of _logical_ handles for buses using the ican driver
 */
enum can_handle_e {
	Can_internal,
    Can_vehicle,
    Can_charger,
    Can_current_sns,
    Can_n_channels
};



/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/


/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

#endif      /* MMC_ICAN_CFG_H */

