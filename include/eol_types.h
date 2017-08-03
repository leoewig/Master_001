/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   eol_types.h
 *
 * description:  
 *  Types, enums and macros for VTBM EOL data module
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
#if !defined(EOL_TYPES_H_)
#define EOL_TYPES_H_

/**
 * Types, enums and macros for VTBM EOL data module
 *
 * @file
 * @ingroup vtbm_eol_types
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/**
 * Maximal length of hardware version string
 */
#define HW_VERSION_MAX_LEN      (16u)

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/**
 * EOL data structure taken from VTBM HWSW specification, Version 2,
 * chapter 4.3.1.2
 */
typedef struct
{
    /** Will be set to 0x9BA5EDFE only if the EOL data is loaded. Any other value
     * indicates invalid data */
    uint32_t eol_validity_marker_1;
    /** This field is used in order to check that the following data
     * is in the format expected by the VTBM software. The layout described
     * in this table corresponds to version 2. */
    uint8_t data_layout_version;
    /** Set to 0xFF */
    uint8_t padding;
    /** ASCII string including NULL terminator. */
    char_t unit_serial_number[16];
    /** ASCII string including NULL terminator. */
    char_t unit_hw_revision[16];
    /** ASCII string WITHOUT NULL terminator */
    char_t year_of_manufacture[4];
    /** ASCII string WITHOUT NULL terminator. Months less than 10 are zero-prefixed
     * (e.g. ?2?to represent February) */
    char_t month_of_manufacture[2];
    /** ASCII string WITHOUT NULL terminator. Day less than 10 are zero-prefixed
     * (e.g. ?2?to represent 2nd day of the month) */
    char_t day_of_manufacture[2];
    /** The checksum shall be calculated on all the data
     * between the validity markers (excluding the checksum itself) using the
     * CRC-15 methodology.*/
    uint16_t checksum;
    /** Will be set to 0x9BA5EDFE only if the EOL data is loaded. Any other value
     * indicates invalid data*/
    uint32_t eol_validity_marker_2;
} eol_t;

/**
 * Dummy initialiser for EOL data which will keep lint quiet and avoid
 * what seems to be a linker bug on the MMC
 */
#define EOL_DUMMY_INITIALISER   \
                                { 1u,0u,0u,"15 bytes + \0  ", \
                                    "15 bytes + \0  ","123","1","1",0u,0u}

/**
 * Enumerator denoting status of EOL data block
 */
typedef enum
{
    eol_data_ok, /**< EOL data has been read and found valid*/
    eol_data_unsupported_hw_version, /**< VTBM hardware is not supported */
    eol_data_invalid_crc, /**< EOL data hasn't passed CRC check */
    eol_data_wrong_format, /**< Other error has occurred during EOL data reading */
    eol_data_not_checked_yet /**< EOL data hasn't been read yet */
} eol_data_status_t;

#endif /* EOL_TYPES_H_ */
