/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  danomem_p.h
 *
 * description:  
 * Dan-O-Mem private header file
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
#ifndef DANOMEM_P_H
#define DANOMEM_P_H
/**
 * @file
 * @ingroup danomem
 *
 * Dan-O-Mem private header file
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "danomem_types.h"

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/* Documented in source */
PRIVATE danomem_res_t danomem_native_write(danomem_type_t type,
                            FPTR const void *addr_p, uint32_t data);
PRIVATE danomem_res_t danomem_cal_write(danomem_type_t type,
                            FPTR void *addr_p, uint32_t data);


#endif /* DANOMEM_P_H */
