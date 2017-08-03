/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   flash_prog.h
 *
 * description:  
 *  Macros and types for flash programming
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
#ifndef FLASH_PROG_H
#define FLASH_PROG_H
/**
 * Headers for Flash programming
 * @file
 * @ingroup flash_prog
 *
 */
/**
 * Flash programming
 *
 * @defgroup flash_prog Flash Programming
 * @ingroup flash
 */
#include "basic_types.h"
#include "flash_prog_types.h"

uint8_t flash_clear(FPTR const uint8_t *startp, uint32_t len);
uint8_t flash_program(FPTR const uint8_t *startp, uint8_t len,
                        const uint8_t *data);
        

#endif /* #ifndef FLASH_PROG_H */
