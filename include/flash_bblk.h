/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   flash_bblk.h
 *
 * description:  
 * Headers for Flash Emulated EEPROM
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#ifndef FLASH_BBLK_H
#define FLASH_BBLK_H
/**
 * Headers for Flash Emulated EEPROM
 * @file
 * @ingroup flash_bblk
 *
 */
#include "flash_eee_types.h"

void flash_eee_initialise(flash_eee_reprogramming_e reprogramming_options);
bool_t flash_usable(void);

#endif /* #ifndef FLASH_BBLK_H */
