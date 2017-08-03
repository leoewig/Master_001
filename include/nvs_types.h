/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    nvs_types.h
 *
 * description:  NVS types
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
#if !defined NVS_TYPES_H
/**
 * NVS types
 * @file
 * @ingroup nvs
 *
 */

#define NVS_TYPES_H

#include "basic_types.h"

/**
 * Header structure stored in EEPROM before each feature's data
 */
typedef struct {
#ifndef HOSTED
    data_validity_e restore_status; /**< Flag showing whether data was
                                          successfully read back from EEPROM.
                                          This byte is replacesd by the 
                                          NVS_HEADER_PATTERN when saving
                                          to EEPROM, which acts as a marker
                                          that we are looking at a header and
                                          not random data */
#else
    uint8_t restore_status;     /* enums are bigger on the host so force
                                   this to a single byte */
#endif
    uint8_t layout_version_number;     /**< Version number of this feature's
                                          EEPROM layout.  changing it will
                                          prevent old EEPROM data from being
                                          stored into the new layout */
    uint8_t write_count;                /**< counter incremented each time
                                          data is written to EEPROM */
    uint8_t last_read;                  /**< value of write_count the last time
                                          data was restored from EEPROM */
    uint16_t data_crc;                  /**< CRC of the data structure */
    uint16_t header_crc;                /**< CRC of the this structure up to,
                                          and including, the data_crc
                                          (so CRC of entire structure, including
                                          this will be zero) */
} nvs_header_s;

/**
 * Size of the header (which crops up often when reading and writing so
 * deserves a macro)
 */
#define         NVS_HEADER_SIZE         sizeof(nvs_header_s)

/**
 * Replace nvs_header_s.restore_status with 'E' when writing to make it as the
 * start of a header
 */
#define         NVS_HEADER_PATTERN      69u 

/**
 * macro to allow a feature to access the restore status of its RAM image
 * @retval      No_data_available       No data could be recovered from EEPROM
 * @retval      Old_data_available      Data restored might not be the most
 *                                      recently saved, but an older data set
 * @retval      New_data_available      The most recently stored data has been
 *                                      restored to the RAM image
 */
#define NVS_RESTORE_STATUS(ram_image)   ((ram_image).header.restore_status)

/**
 * Initialise NVS header with a given version number
 */
#define         NVS_HEADER_INIT(version_number)         \
                                            { \
                                                No_data_available, \
                                                version_number, \
                                                0u,     /* write_count */ \
                                                0u,     /* last_read */ \
                                                0u,     /* data_crc */ \
                                                0u      /* header_crc */ \
                                            } 
/**
 * suppress lint warning when intialising reserved_bytes[]
 * @lint --e{785} Too few initialisers: reserved_bytes[] does not need 
 *                      initialisation
 */
#define         NVS_RESERVED_BYTES_INIT     { \
                                                /*lint --e{785} */ \
                                                0u \
                                            }
/**
 * private struct describing layout of feature NVS data
 */
typedef struct {
    FRAM nvs_header_s *header_p ; /**< ptr to header at start of wrapper */
    uint16_t wrapper_size ;  /**< total structure including reserved_bytes[] */
    uint16_t data_size ;     /**< excludes header and reserved_bytes[] */
} nvs_feat_list_s ;

/**
 * private enum showing the possible combinations of EEPROM banks to be read
 */
typedef enum {
    NBRO_bank0_newer_than_bank1,
    NBRO_bank0_only,
    NBRO_bank1_newer_than_bank0,
    NBRO_bank1_only,
    NBRO_both_old,
    NBRO_none
} nvs_bank_read_order_e;

/**
 * private enum for naming NVS banks
 */
typedef enum {
    NVS_bank_0,
    NVS_bank_1,
    NVS_bank_none,      /**< used to show that neither bank was restored from
                                EEPROM */
    NVS_bank_both       /**< used to show that both banks require writing to
                                EEPROM */
} nvs_bank_e; 

/**
 * private structure containing distilled information about a header read
 * from the EEPROM
 */
typedef struct {
    bool_t valid;       /**< header pattern and CRCs all ok */
    bool_t unread_data; /**< latest saved data hasn't been read, i.e.
                         nvs_header_s.latest_read != nvs_header_s.write_count */
    uint8_t write_count;        /**< value of write_count in the header */
} nvs_header_status_s ;

/**
 * private structure drawing together information about the next write of a
 * feature to EEPROM
 */
typedef struct {
    uint8_t write_request;      /**< outstanding request from foreground to
                                        background */
    uint8_t next_write_count;   /**< write_count for the next data to be
                                        written */
    nvs_bank_e last_bank_restored;   /**< one of NVS_bank_0, NVS_bank_1 or,
                                          NVS_bank_none.
                                          Needed to track which bank requires
                                          an update to .last_read */
    nvs_bank_e next_bank_to_write_data;  /**< one of NVS_bank_0, NVS_bank_1 or,
                                          if neither bank currently contains
                                          valid data, NVS_bank_both */
} nvs_write_status_s;


typedef struct
{
    nvs_header_s header;        /**< Feature Information Structure.*/
    uint8_t data;          /**< Module NV data-set. */
    uint8_t reserved_bytes[10];
                                /**< Feature Reserved_bytes.*/
} ldt_mdm_nvs_wrapper_s;



/*
 * Set of flags for signalling information in #nvs_write_status between
 * foreground and background
 */
#define NVS_WRITE_IDLE                  0u /**< no outstanding request */
#define NVS_WRITE_HEADER_REQUEST        1u /**< header only needs writing to
                                                EEPROM */
#define NVS_WRITE_DATA_REQUEST          2u
#define NVS_WRITE_CHECKPOINT_REQUEST    4u /**< Combine with WRITE_DATA to make
                                                the written data appear 'old'*/

#endif /* #if !defined NVS_TYPES_H */
