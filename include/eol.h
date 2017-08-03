/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  eol.h
 *
 * description:  
 *  VTBM End-of-line data module header
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
#if !defined(EOL_H_)
#define EOL_H_

/**
 * VTBM End-of-line data module header
 *
 * @file
 * @ingroup vtbm_eol
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "eol_types.h"
#include "hardware.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
eol_data_status_t check_eol(FPTR const eol_t * eol,
                                const char_t * const supported_hw_versions[]);
#endif /* EOL_H_ */
