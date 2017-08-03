/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      flash_bblk_p.h
 *
 * description:   Private headers for Flash Emulated EEPROM
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li    Private headers for Flash Emulated EEPROM
 *                       
 *
 *****************************************************************************/
#ifndef FLASH_BBLK_P_H
#define FLASH_BBLK_P_H


PRIVATE void flash_eee_query(void);
PRIVATE bool_t flash_eee_partition(void);
PRIVATE bool_t flash_eee_enable(void);

PRIVATE NCODE void flash_check_ecc_phrases(void);

#endif /* #ifndef FLASH_BBLK_P_H */
