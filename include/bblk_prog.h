/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    bblk_prog.h
 *
 * description:  Bootblock XCP slave header 
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Public header for ADC Conversion utility routines
 *                       
 *
 *****************************************************************************/
#ifndef BBLK_XCP_H
#define BBLK_XCP_H
/**
 * Bootblock XCP slave header 
 *
 * @file
 *
 * @ingroup bblk_xcp
 */
#include "ican_cfg.h"

void bblk_prog_prepare(void);
void bblk_prog_wait_forever(void);
void bblk_prog_wait(void);
void bblk_prog_session(enum can_handle_e bus);

void bblk_tx(uint16_t id,uint8_t dlc,const uint8_t *data);

#endif /* #ifndef BBLK_XCP_H */
