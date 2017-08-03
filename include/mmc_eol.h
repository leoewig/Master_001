/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_eol.h
 *
 * description:  MMC End-of-line data module header
 *    
 *      
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(MMC_EOL_H_)
#define MMC_EOL_H_

/**
 * MMC End-of-line data module header
 *
 * @file
 * @ingroup mmc_eol
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 * EOL data module
 *
 * Overview
 * ========
 * This module checks for presence and validity of EOL data and provides it for application
 * use together with its status.
 *
 * Approach used
 * =============
 * EOL data are located at dedicated location in flash memory defined in chapter 4.3.1 of
 * MMC HWSW interface specification. This module checks for validity, consistency and
 * compatibility and publishes its status to be checked by an application. EOL data itself are
 * accessible by a pointer to eol_t structure also provided by this module. Data itself are
 * not copied out from original location. This module supports only EOL structure defined in chapter
 * 4.3.1.2 Version 2 of the VTBM HWSW Interface specification.
 *
 * Integration
 * ===========
 * -Include eol.h
 *
 * How to use
 * ==========
 * - call mmc_init_eol once
 * - check #mmc_eol_data_status
 *      - if it's equal to #eol_data_ok, EOL data is valid
 *      and ready to be accessed by an application
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "eol_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
extern eol_data_status_t mmc_eol_data_status;

extern FROM eol_t mmc_eol;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
void mmc_init_eol(void);

#endif /* MMC_EOL_H_ */
