/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  danomem.h
 *
 * description:  
 * Dan-O-Mem internal header file
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
#ifndef DANOMEM_H
#define DANOMEM_H
/**
 * @file
 * @ingroup danomem
 *
 * Dan-O-Mem internal header file
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
danomem_res_t danomem_read_data(danomem_type_t type, volatile const FPTR void *addr_p,
    uint32_t *out);
danomem_res_t danomem_write_data(danomem_type_t type, volatile const FPTR void *addr_p,
    uint32_t data);

uint8_t ascii2bin(CONST char_t hi, CONST char_t lo);

#endif /* DANOMEM_H */
