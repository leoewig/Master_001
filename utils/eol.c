/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name: eol.c
 *
 * description:  
 *
 * End-of-line data module
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "crc.h"
#include "eol.h"
#include "eol_p.h"
#include "strings.h"

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/


/**
 * Check EOL data consistency and compatibility
 *
 * @param[in] eol   pointer to eol data structure to be checked
 * @param[in] supported_hw_versions   pointer to data structure to be checked against
 * @return  eol_data_ok    if EOL is compatible, consistent and ready for use
 * @return  eol_data_wrong_format   if one or more EOL data members are in wrong or
 *                          unsupported format
 * @return  eol_data_unsupported_hw_version if EOL hw string doesn't match any of
 *                          supported ones
 * @return  eol_data_invalid_crc if EOL CRC16-CITT checksum doesn't match calculated one
 *
 * Requirements covered
 * --------------------
 * - REQ_BMS_VTBM_EOL_002
 * - REQ_BMS_VTBM_EOL_003
 */
eol_data_status_t check_eol(FPTR const eol_t * eol,
                                const char_t * const supported_hw_versions[])
{
    eol_data_status_t res = eol_data_ok;
    uint8_t len;

    if ((eol->eol_validity_marker_1 != EOL_VALIDITY_MARKER)
        || (eol->eol_validity_marker_2 != EOL_VALIDITY_MARKER))
    {
        /* validity marked is of wrong value */
        res = eol_data_wrong_format;
    }

    if ((res == eol_data_ok)
        && (eol->data_layout_version != EOL_SUPPORTED_DATA_LAYOUT_VERSION))
    {
        /* unsupported data layout version */
        res = eol_data_wrong_format;
    }
    if ((res == eol_data_ok) && (eol->padding != EOL_PADDING))
    {
        /* padding is of wrong value */
        res = eol_data_wrong_format;
    }
    if (res == eol_data_ok)
    {
        len = is_ascii_string(eol->unit_serial_number, (uint8_t) sizeof(eol->unit_serial_number));
        if ((len == 0u) || (len >= sizeof(eol->unit_serial_number)))
        {
            /* string is either not ascii compatible, empty or not terminated */
            res = eol_data_wrong_format;
        }
    }
    if (res == eol_data_ok)
    {
        len = is_ascii_string(eol->unit_hw_revision, (uint8_t) sizeof(eol->unit_hw_revision));
        if ((len == 0u) || (len >= sizeof(eol->unit_hw_revision)))
        {
            /* string is either not ascii compatible, empty or not terminated */
            res = eol_data_wrong_format;
        }
    }
    if (res == eol_data_ok)
    {
        len = is_ascii_string(eol->year_of_manufacture, (uint8_t) sizeof(eol->year_of_manufacture));
        if (len != sizeof(eol->year_of_manufacture))
        {
            /* string is either not ascii compatible, empty or not correct size */
            res = eol_data_wrong_format;
        }
    }
    if (res == eol_data_ok)
    {
        len = is_ascii_string(eol->month_of_manufacture,
            (uint8_t) sizeof(eol->month_of_manufacture));
        if (len != sizeof(eol->month_of_manufacture))
        {
            /* string is either not ascii compatible, empty or not correct size */
            res = eol_data_wrong_format;
        }
    }
    if (res == eol_data_ok)
    {
        len = is_ascii_string(eol->day_of_manufacture, (uint8_t) sizeof(eol->day_of_manufacture));
        if (len != sizeof(eol->day_of_manufacture))
        {
            /* string is either not ascii compatible, empty or not correct size */
            res = eol_data_wrong_format;
        }
    }
    if (res == eol_data_ok)
    {
        /* Check CRC. Exclude validity markers and crc field itself. */
        /** @polyspace<MISRA-C:11.4:Not a defect:Justify with annotations> Casting is safe,
         * no alignment problem is expected, since eol object is aligned to uint32_t type */
        if (eol->checksum
            != crc16(CRC16_SEED, &(((CONST FPTR uint8_t *) eol)[sizeof(eol->eol_validity_marker_1)]),
                sizeof(eol_t)
                    - (sizeof(eol->eol_validity_marker_1) + sizeof(eol->eol_validity_marker_2)
                        + sizeof(eol->checksum))))
        {
            res = eol_data_invalid_crc;
        }
    }
    if (res == eol_data_ok)
    {
        if (check_hw_version(eol->unit_hw_revision,supported_hw_versions)
                != TRUE )
        {
            res = eol_data_unsupported_hw_version;
        }
    }
    return res;
}

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/**
 * Check that input character sequence makes ASCII compatible
 * string
 *
 * @param[in]   in      pointer to first character
 * @param[in]   max_len maximal length to be checked
 * @return      length of ascii compatible character sequence. If it matches max_len,
 *              string is not terminated, if it's 0, string is empty, or not ascii
 *              compatible
 */
PRIVATE uint8_t is_ascii_string(FPTR const char_t * in, uint8_t max_len)
{
    uint8_t len = 0u;
    while (max_len != 0u)
    {
        if (((uint8_t)in[len] < 32u) || ((uint8_t) in[len] > 127u))
        {
            if (in[len] != 0)
            {
                len = 0u;
            }
            break;
        }
        len++;
        max_len--;
    }
    return len;
}

/**
 * Check if given hardware version string matches supported ones
 *
 * @param[in]   hw      hardware string to be checked
 * @param[in] supported_hw_versions   pointer to data structure to be checked against
 * @return      TRUE if hardware string matches one of supported hw string
 * @return      FALSE if otherwise
 */
PRIVATE bool_t check_hw_version(FPTR const char_t * hw,
                                const char_t * const supported_hw_versions[])
{
    uint8_t i;
    bool_t res = FALSE;
    for (i = 0u;
            (res == FALSE) && (*supported_hw_versions[i] != '\0'); i++)
    {
        if (string_compare(hw, supported_hw_versions[i], HW_VERSION_MAX_LEN) == TRUE)
        {
            res = TRUE;
        }
    }
    return res;
}
