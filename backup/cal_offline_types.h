/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   cal_offline_types.h
 *
 * description:  
 * Types, enums and macros for MMC pack-wide end-of-line calibration
 * This calibration is stored in flash, and can not be adjusted online
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
#if !defined(PACK_OFFLINECAL_TYPES_H)
#define PACK_OFFLINECAL_TYPES_H

/**
 * Types, enums and macros for MMC pack-wide end-of-line calibration
 * This calibration is stored in flash, and can not be adjusted online
 *
 * @file
 * @ingroup pack_eolcal
 * $Id: cal_offline_types.h 11014 2013-10-23 07:53:19Z dc17 $
 */

/* Copyright (c) Linkedriving Tech 2015 - All rights resevered */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
//#include "basic_types.h"

#include "mdm_types.h" /* for MAX_NUM_VTBM */
#include "icm_types.h" /* For MAX_VTBM_CELLS */

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

#define PACK_OFFLINECAL1_VALIDITY1      0xE07B5EBAUL
#define PACK_OFFLINECAL1_VALIDITY2      0x68253DBAUL
#define PACK_OFFLINECAL1_VERSION        1u
#define PACK_OFFLINECAL2_VALIDITY1      0x02176597UL
#define PACK_OFFLINECAL2_VALIDITY2      0x23100228UL
#define PACK_OFFLINECAL2_VERSION        1u
#define CAL_PADDING                     (0xffu)

#define PACK_OFFLINECAL_PACK_SN_SZ      (16u)
/** Maximum size of the offline calibration data set 2. Unused data is 
 *  automatically assigned to the 'reserved' parameter. */
#define OFFLINECAL_DATA2_MAX_BYTES      (1024u)

/** Size of the offline calibration data set 2 overhead. The overhead is made
 *  up of the validation markers and the checksum */
#define OFFLINECAL_OVERHEAD_SZ          (4u+2u+4u)
#define OFFLINECAL_DATA2_BYTES_USED     (OFFLINECAL_OVERHEAD_SZ + sizeof(cal_off_data2_t))

/**
 * The checksum must be explicitly in big-endian byte order, so for portability
 * this access macro is provided. Note that the data payload itself is not
 * portable. The different endianess will give different checksums on different
 * platforms */
#define SET_OFFLINE_CAL_CS(calp,cs) do {\
    (calp)->checksum = (cs);\
    } while (0)

#define GET_OFFLINE_CAL_CS(calp)   ((calp)->checksum)

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/**
 * Enumerator denoting status of EOL data block
 */
typedef enum
{
    cal_off_data_ok,             /**< EOL data has been read and found valid */
    cal_off_data_bad_range,      /**< VTBM temperatures are out of range */
    cal_off_data_invalid_crc,    /**< EOL data hasn't passed CRC check */
    cal_off_data_wrong_format,   /**< Other error has occurred during EOL data
                                  *   reading */
    cal_off_data_not_checked_yet /**< EOL data hasn't been read yet */
} cal_off_data_status_t;

/** Structure containing the offline calibration data set 1. Data set 1 
 *  is kept separate for backward compatibility with packs already in the 
 *  field.
 */
//typedef struct tag_cal_pack_offline_set1
//{
//    uint32_t validity_marker_1;
//    struct {
//        uint8_t layout_version;
//        uint8_t padding;                /** set to 0xFF */
//
//        /** offset for every cell in the pack, the structure should be 16-bit
//         *  aligned at the start of this field */
//        temperature_t cell_t_offset[MAX_NUM_VTBM][MAX_VTBM_CELLS];
//    } data;
//    uint16_t checksum;
//    uint32_t validity_marker_2;
//} cal_pack_offline_set1_t;

/** Structure containing the offline calibration data set 2. Data set 2
 *  is an extension to the initial offline calibration data.
 */
typedef struct 
{
    uint8_t  layout_version;
    uint8_t  padding;                   /** set to 0xFF */
    /** Pack serial number, ASCII string */
    char_t   pack_serial_number[16];
    /** Cell manufacture date */
    uint16_t yr_of_cell_manufacture;    /** range 2000 to 2099 */
    uint8_t  mth_of_cell_manufacture;   /** range 1 to 12 */
    uint8_t  day_of_cell_manufacture;   /** range 1 to 31 */
} cal_off_data2_t;

/** Container for offline calibration data set 2. Includes overhead 
 *  parameters used for validation as well as a reserved area for future
 *  expansion. The reserved area size is automatically calculated but
 *  relies on the accuracy of the overhead size OFFLINECAL_OVERHEAD_SZ.
 *  NOTE: should the overhead parameters (i.e. any element excluding 'data'
 *  or 'reserved_bytes') be modified, OFFLINECAL_OVERHEAD_SZ must be 
 *  manually updated.
 */
typedef struct 
{
    uint32_t validity_marker_1;
    cal_off_data2_t data;
    uint8_t reserved_bytes[(int16_t) OFFLINECAL_DATA2_MAX_BYTES
                           - (int16_t) OFFLINECAL_DATA2_BYTES_USED];
    uint16_t checksum;
    uint32_t validity_marker_2;
} cal_pack_offline_set2_t;

/** Offline calibration data container.
 */
typedef struct
{
    //cal_pack_offline_set1_t cal_set1;
    cal_pack_offline_set2_t cal_set2;
} cal_pack_offline_t;

#ifdef CAL_OFFLINE_VALID_DUMMY
/* An offline calibration initialiser that is valid */
#define CAL_OFFLINE_DUMMY_INITIALISER   \
{ \
    { \
        PACK_OFFLINECAL1_VALIDITY1, \
        {\
            PACK_OFFLINECAL1_VERSION,\
            CAL_PADDING,\
            { \
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW\
            }, \
        },\
        CAL_OFFLINE_DUMMY_CSUM, \
        PACK_OFFLINECAL1_VALIDITY2 \
    }, \
    { \
        PACK_OFFLINECAL2_VALIDITY1, \
        { \
            PACK_OFFLINECAL2_VERSION, \
            CAL_PADDING, \
            { 0u }, \
            12u, \
            1u, \
            1u \
        }, \
        { 0u }, \
        0xDBC7u, \
        PACK_OFFLINECAL2_VALIDITY2 \
    } \
}

#ifdef CAL_OFFLINE_DUMMY_VALUES

/**
 * A mix of temperature offsets, so that it is possible to see the temperature
 *  cal having an effect */

/** A row of VTBM cell temperature offsets */
#define VTBM_TEMP_ROW  {TEMP(-2.00),TEMP(-1.75),TEMP(-1.50),TEMP(-1),\
                        TEMP(-0.75),TEMP(-0.50),TEMP(-0.25),TEMP( 0),\
                        TEMP( 0.25),TEMP( 0.50),TEMP(0.75), TEMP( 1),\
                        TEMP( 1.25),TEMP( 1.50),TEMP( 1.75),TEMP( 2)}

#define CAL_OFFLINE_DUMMY_CSUM  0x6D65u

#else /* CAL_OFFLINE_DUMMY_VALUES */

/* A valid dummy calibration which gives every cell a zero offset */
#define VTBM_TEMP_ROW  {0,0,0,0,0,0,0,0,\
                        0,0,0,0,0,0,0,0}

#define CAL_OFFLINE_DUMMY_CSUM  0x42B0u
#endif /* CAL_OFFLINE_DUMMY_VALUES */

#else /* CAL_OFFLINE_VALID_DUMMY */

/* Standard invalid initialiser, all values cause the value FF to be placed in
 * the flash */

#define CAL_OFFLINE_DUMMY_INITIALISER   \
{ \
    { \
        0xFFFFFFFFUL, \
        { \
            0xFFu, \
            0xFFu, \
            { \
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW,\
                VTBM_TEMP_ROW\
            }, \
        },\
        0xFFFFu,\
        0xFFFFFFFFUL \
    }, \
    { \
        0xFFFFFFFFUL, \
        { \
            0xFFu, \
            0xFFu, \
            { 0 }, \
            0xFFFFu, \
            0xFFu, \
            0xFFu \
        }, \
        { 0u }, \
        0xFFFFu, \
        0xFFFFFFFFUL \
    } \
}

#define VTBM_TEMP_ROW  {-1,-1,-1,-1,\
                        -1,-1,-1,-1,\
                        -1,-1,-1,-1,\
                        -1,-1,-1,-1}
#endif


#endif /* PACK_OFFLINECAL_TYPES_H */
