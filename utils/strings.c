/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     strings.c
 *
 * description:  PIT peripheral device driver
 * 
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/
/**
 * @ingroup utils
 * @{
 * @file
 * @}
 */

#include "strings.h"

/**
 * Compares two strings
 *
 * @param[in]   string1 the first string to be compared
 * @param[in]   string2 the second string to be compared
 * @param[in]   max_len  maximal length to be compared
 * @return      TRUE    if string2 matches string1
 * @return      FALSE   if otherwise
 */
bool_t string_compare(CONST FPTR char_t * string1, CONST char_t * string2, uint8_t max_len)
{
    bool_t exit = FALSE;
    bool_t res = FALSE;
    uint8_t i = 0u;

    while (exit == FALSE)
    {
        if ((string2[i] == 0) || (max_len == 0u))
        {
            /* End of string or max characters compared */
            res = TRUE;
            exit = TRUE;
        }
        else
        {
            if (string2[i] != string1[i])
            {
                /* Mis-match found*/
                exit = TRUE;
            }
        }
        i++;
        max_len--;
    }
    return res;
}

/**
 * Returns the upper-case version of the string. This should
 * hopefully make checking of strings more resistant to operator error.
 *
 * @param[in]   string1 the string to be converted
 * @param[out]  string2 the converted string
 * @param[in]   max_len  maximal length to be converrted
 */
void uppercase_string(CONST FPTR char_t * string1, FPTR char_t * string2, uint8_t max_len)
{
    uint8_t i = 0u;
    uint8_t newchar;

    string2 [0] = string1 [0];

    if (max_len > 0u)
    {
        while (max_len > 0u)
        {
            newchar = (uint8_t) string1 [i];

            if ((string1 [i] >= 'a') && (string1 [i] <= 'z'))
            {
                newchar += (uint8_t) 'A';
                newchar -= (uint8_t) 'a';
            }

            string2 [i] = (char_t) newchar;

            i++;
            max_len--;
        }
    }
}

