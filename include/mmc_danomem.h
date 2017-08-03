/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_danomem.h
 *
 * description:  Dan-O-Mem public header for mmc, mmc_test_harness and prodder targets
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
#if !defined(MMC_DANOMEM_H_)
#define MMC_DANOMEM_H_

/**
 * Dan-O-Mem public header for mmc, mmc_test_harness and prodder targets
 *
 * @file
 * @ingroup danomem
 *
 */


/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "danomem.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

void danomem(void);
#if !defined PRODDER
void SendSutIdFrame(uint8_t p_U8_build_id, const char_t* p_pS_date_string,
    const char_t* p_pS_time_string, uint8_t p_U8_reset_reason);
#endif

void danomem_send_pending(void);

#endif /* MMC_DANOMEM_H_ */
