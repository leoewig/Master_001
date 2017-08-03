/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  cosmic_pointers_types.h
 *
 * description:  
 *    Cosmic pointer formats
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
#ifndef COSMIC_POINTERS_TYPES_H
#define COSMIC_POINTERS_TYPES_H
/**
 * Cosmic pointer formats
 *
 * @file
 * @ingroup utils
 *
 * $Id: cosmic_pointers_types.h 6174 2013-01-15 15:40:12Z gvs $
 */
/* Copyright (c) Linkedriving Tech 2015 - All rights resevered */
#include "basic_types.h"

/**
 * Union used to help generate a Cosmic \@gpage \@far pointer from a global
 * address
 */
typedef union {
    FPTR uint8_t *address_p;    /**< Cosmic \@gpage \@far pointer */
    struct {
        uint16_t address_word;  /**< 16-bit address in the first two bytes */
        uint8_t gpage;          /**< GPAGE in the next byte */
        uint8_t zero;           /**< followed by a zero byte */
    } address_struct;
} cosmic_gpage_address_u;

#endif /* #ifndef COSMIC_POINTERS_TYPES_H */
