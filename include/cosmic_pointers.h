/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  cosmic_pointers.h
 *
 * description:  
 * Cosmic pointer manipulation
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
#ifndef COSMIC_POINTERS_H
#define COSMIC_POINTERS_H
/**
 * Cosmic pointer manipulation
 *
 * @file
 * @ingroup uitls
 *
 */
/* Copyright (c) Linkedriving Tech 2015 - All rights resevered */
#include "cosmic_pointers_types.h"

FPTR uint8_t *cosmic_fptr_from_global_address(uint32_t address);
uint32_t cosmic_global_address_from_fptr(FPTR const uint8_t *ptr_p);


#endif /* #ifndef CAL_H */
