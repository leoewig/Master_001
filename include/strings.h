/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    strings.h
 *
 * description:  
 * Public header for Strings Utility
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
#ifndef STRINGS_H
#define STRINGS_H
/*************************************************************************//**
 *
 * Public header for Strings Utility
 *
 *
 * @file
 * @ingroup utils
 *
 *
 *****************************************************************************/

#include "basic_types.h"
#include "utils_types.h"

bool_t string_compare  (CONST FPTR char_t * string1, CONST char_t * string2, uint8_t max_len);
void   uppercase_string(CONST FPTR char_t * string1, FPTR char_t * string2, uint8_t max_len);

#endif /* STRINGS_H */
