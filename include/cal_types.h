/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   cal_types.h
 *
 * description:  
 *  XCP calibration types and macros
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
#ifndef CAL_TYPES_H
#define CAL_TYPES_H
/**
 * XCP calibration types and macros
 *
 * @file
 * @ingroup cal
 *
 * $Id: cal_types.h 9304 2013-07-05 00:59:16Z gvs $
 */
/* Copyright (c) Linkedriving Tech 2015 - All rights resevered */
#include "nvs_cfg.h"

/**
 * Starting address of calibration data in the EEPROM
 */
#define CAL_EEPROM_START_ADDRESS        8192u

/**
 * number of distinct calibration sections
 */
#define CAL_N_SECTIONS  2u

/**
 * bytes of NVS EEPROM used including the standard NVS header
 */
#define CAL_NVS_USED (sizeof(nvs_header_s) + sizeof(cal_nvs_data_s))

/**
 * Default value of test calibration #cal_test_cal which is assigned 
 * to #cal_test_var
 */
#define CAL_TEST_CAL_DEFAULT            29u

/**
 * Length of calibration_id array excluding trailing NULL
 */
#define CALIBRATION_ID_LENGTH   13u

/**
 * success/fail/busy as reported by cal functions
 */
typedef enum {
    Cal_success,
    Cal_failure,
    Cal_pending
} cal_status_e;

/**
 * List of supported XCP slaves
 */
typedef enum {
    Cal_slave_canbedded,
    Cal_slave_ixcp,
    Cal_slave_danomem
} cal_xcp_slave_e;

/**
 * Enumeration of calibration pages
 */
typedef enum {
    Cal_page_flash,
    Cal_page_ram
} cal_page_e;

/**
 * Version ID for CAL NVS data to ensure incompatible data isn't loaded after
 * reflash
 */
#define CAL_NVS_LAYOUT_VERSION          1u
/**
 * Non-volatile calibration support data - update #CAL_NVS_LAYOUT_VERSION
 * if changing
 */
typedef struct {
    bool_t eeprom_contains_cal;         /**< there is a valid calibration stored
                                             in the EEPROM */
    bool_t using_eeprom_cal;            /**< We are currently using a RAM mirror
                                             of the EEPROM calibration */
    uint16_t cal_checksum;              /**< simple 16-bit sum of all the bytes
                                          in the cal so that we can update it
                                          as we change bytes in the cal */
    uint16_t flash_crc[CAL_N_SECTIONS]; /**< CRCs of each cal section - used to
                                          see if a reflash has altered the base
                                          calibration */
} cal_nvs_data_s;

/**
 * Cal wrapper structure for NVS data
 * This structure contains some non-volatile support info which is
 * separate from the actual calibration data stored elsewhere on the EEPROM
 */
typedef struct {
    nvs_header_s header;        /**< standard NVS header */
    cal_nvs_data_s data;        /**< Cal NVS data */
#if 1 /* ndef _lint */
    uint8_t reserved_bytes[(int16_t)CAL_MAX_NVS_BYTES - (int16_t)CAL_NVS_USED];
                                                /**< Feature Reserved_bytes.*/
#else
    /* lint miscalculates the size of cal_nvs_data_s because it thinks the enum
     * is more than a single byte.  This leads to it thinking that
     * reserved_bytes has length zero (a 'vacuous' type).
     * Avoid this by fixing the array size for lint.
     */
    uint8_t reserved_bytes[1];
#endif
} cal_nvs_wrapper_s;

/**
 * structure with information about each calibration section
 */
typedef struct {
    FPTR uint8_t *ram_p;        /**< location of cal data in RAM */
    uint16_t length;            /**< number of bytes */
} cal_areas_s;

#define CAL_REQUEST_DISCARD_EEPROM_FLAG      1u
#define CAL_REQUEST_LOAD_EEPROM_FLAG         2u
#define CAL_REQUEST_WRITE_WHOLE_CAL_FLAG  4u
#define CAL_REQUEST_WRITE_CAL_BYTES_FLAG  8u

#define CAL_REQUEST_DISCARD_EEPROM(request_struct,slave)        \
    do { \
        (request_struct).request_byte |= CAL_REQUEST_DISCARD_EEPROM_FLAG; \
        (request_struct).xcp_slave = (slave); \
    } while (0)

#define CAL_REQUEST_LOAD_EEPROM(request_struct,slave) \
    do { \
        (request_struct).request_byte |= CAL_REQUEST_LOAD_EEPROM_FLAG; \
        (request_struct).xcp_slave = (slave); \
    } while (0)

#define CAL_REQUEST_WRITE_WHOLE_CAL(request_struct,slave) \
    do { \
        (request_struct).request_byte |= CAL_REQUEST_WRITE_WHOLE_CAL_FLAG; \
        (request_struct).xcp_slave = (slave); \
    } while (0)

#define CAL_CLEAR_REQUESTS(request_struct) \
        (request_struct).request_byte = 0u

#define CAL_REQUEST_WRITE_CAL_BYTES(request_struct,slave,src_p,size, \
                                        eeprom_address) \
    do { \
        (request_struct).request_byte |= CAL_REQUEST_WRITE_CAL_BYTES_FLAG; \
        (request_struct).xcp_slave = (slave); \
        (request_struct).write_src_p = (src_p); \
        (request_struct).write_size = (size); \
        (request_struct).eeprom_address = (eeprom_address); \
    } while (0)


typedef struct {
    uint8_t request_byte;
    cal_xcp_slave_e xcp_slave;
    FPTR const uint8_t *write_src_p;
    uint8_t write_size;
    uint16_t eeprom_address;
} cal_eeprom_request_s;


/**
 * structure representing a paged data entry in the Cosmic RAM initialisation
 * table
 */
typedef struct {
    uint8_t flag;               /**< flag indicating whether paged, whether
                                  movable code, and the first letter of the
                                  segment.  Zero indicates end of table */
    uint16_t ram_gpage;         /**< GPAGE of the RAM being intialised, stored
                                  in the lower byte of the word (so incompatible
                                  with a Cosmic C \@gpage \@far pointer!) */
    uint16_t ram_start;         /**< lower 16 bits of the global RAM address */
    uint16_t rom_end;           /**< _local_ ROM address of the end of the
                                    initalisation data for this entry/start of
                                    data for the next.  We need to use
                                    jiggery-pokery to infer the correct
                                    page for this */
} cosmic_init_entry_s;

/**
 * structure representing the whole initialisation table
 * Note that this will not match __idesc__ if the actual table contains
 * unpaged data or moveable code entries
 */
typedef struct {
    uint16_t rom_start; /**< first address of initalisation data in ROM */
    cosmic_init_entry_s table[1u];
                        /**< Array of entries.
                         @note the table length is arbitrary - we don't know
                         the actual number so we pick a suitably largish value.
                         The end of the table is indicated by a dummy entry
                         with a flag value of 0 at the end
                         */
} cosmic_idesc_s ;

#define CAL_ROM_OFFSET_FROM_ADDRESS_WORD(address)      ((address) & 0x3ffu)
#endif /* #ifndef CAL_TYPES_H */
