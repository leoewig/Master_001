/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   flash_eee_types.h
 *
 * description:  
 * Macros and types for Flash emulated EEPROM
 *
 * All fields stored in EEE are word-aligned because the Emulated EEPROM is
 * optimised for word accesses:
 * 1. Aligned word writes only affect flash when the data changes, 
 *    mis-aligned and byte writes always force a write through to flash
 * 2. Aligned word writes are done atomically, misaligned writes require two
 *    operations.
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#ifndef FLASH_EEE_TYPES_H
#define FLASH_EEE_TYPES_H
/**
 * Macros and types for Flash emulated EEPROM
 *
 * All fields stored in EEE are word-aligned because the Emulated EEPROM is
 * optimised for word accesses:
 * 1. Aligned word writes only affect flash when the data changes, 
 *    mis-aligned and byte writes always force a write through to flash
 * 2. Aligned word writes are done atomically, misaligned writes require two
 *    operations.
 *
 * @file
 * @ingroup flash_eee
 *
 */

#include "regs.h"       /* For FSTAT */

/**
 * bit mask of MGBUSY bit in #FSTAT
 */
#define         FSTAT_MGBUSY    0x08u

/**
 * Test whether the Emulated EEPROM has any outstanding writes.
 *
 * If FSTATS's MGBUSY bit is set, we cannot tell whether this is for a EEE
 * operation or some other flash command, alothough we're unlikely to be
 * perfroming another flash operation.
 *
 * If ETAG is non-zero, there are EEE writes still pending.
 *
 * Note that the order of the tests is important because ETAG is decremented
 * _before_ the word is written to the EEPROM.
 */
#define FLASH_EEE_BUSY()        ((ETAG > 0u) || (FSTAT & FSTAT_MGBUSY))

/**
 * Size of buffer in Emulated EEPROM for storing the location of an
 * internal error.  Must be small enough to ensure the whole structure
 * fits in a single 256-byte EEE page and should be even to keep everything
 * word-aligned.
 */
#define INTERNAL_ERROR_LOCATION_MAXLEN          100u

/**
 * Dual purpose enumeration used to define the value written by the
 * bootloader to #bootblock_eee .flash_eee_available and to define the value
 * returned by flash_eee_status()
 */
typedef enum {
    Flash_eee_unavailable,      /**< value written to
                                  #bootblock_eee .flash_eee_available
                                  by bootloader to indicate that EEE could not
                                  be partitioned. */
    Flash_eee_available,        /**< value written to
                                  #bootblock_eee .flash_eee_available
                                  by bootloader to indicate that EEE has been
                                  partitioned */
    Flash_eee_busy,             /**< Either there is outstanding EEE data
                                  to be written to flash, or the flash
                                  controller is busy doing something else so we
                                  can't check. */
    Flash_eee_idle              /**< The EEE has been written to flash so
                                  power can be safely removed. */
} flash_eee_status_e;

/**
 * Value stored in #bootblock_eee .last_reset to indicate the cause of a reset
 */
typedef enum {
    Reset_no_application,            /**< Bootloader was switched off
                                          while waiting to have an application
                                          programmed */
    Reset_uncontrolled,              /**< Bootloader passed control to the
                                          application, but a reset occured
                                          before the application updated
                                          @ref bootblock_eee .next_reset.
                                          This is expected behaviour for the
                                          VTBM */
    Reset_controlled,                /**< Application shut itself down in
                                          an orderly manner */
    Reset_internal_error,            /**< reset was caused by an
                                          INTERNAL_ERROR() macro being tripped
                                          */
    Reset_cop,                       /**< reset was caused by the COP which
                                          doesn't appear to have been triggered
                                          by the INTERNAL_ERROR() macro
                                          */
    Reset_clockmon,                  /**< reset was caused by the Clock
                                          Monitor */
    Reset_program_xcp_internal,      /**< Reset was due to an XCP
                                          reprogramming request on the internal
                                          bus */
    Reset_program_xcp_vehicle,       /**< Reset was due to an XCP 
                                          reprogramming request on the vehicle
                                          bus */
    Reset_program_iso,               /**< Reset was due to an ISO Diagnostics
                                          reprogramming request on the vehicle
                                          bus */
    Reset_program_complete,          /**< Reset followed programming */
    Reset_uninitialised=0xffffu     /**< EEE unprogrammed.
                                         Set to 16-bit value to ensure enum
                                         is word-sized to keep the EEE
                                         word-aligned */
} flash_eee_reset_type_e ;

typedef enum {
    Reprogram_iso = 1,          /**< Diagnostics reprogramming */
    Reprogram_iso_and_xcp = 2,  /**< here for completeness but unlikely to be
                                  implemented */
    Reprogram_vtbms = 3,        /**< needed to indicate that a request is for
                                  the MMC to reprogram the VTBMs, which doesn't
                                  go via the bootloader */
    Reprogram_xcp = 0xffff      /**< same as 'unprogrammed' because old XCP
                                    bootblocks don't have this field.
                                    Set to 16-bit value to keep EEE
                                    word-aligned */
} flash_eee_reprogramming_e;

/**
 * Structure used to hold the communication between the bootloader and the
 * application, stored in emulated EEPROM so that it can survive a reset or
 * power cycle
 */
typedef struct {
    /**
     * Word left for us by the bootloader to tell us if the emulated EEPROM is
     * partitioned and working.
     *
     * Will contain #Flash_eee_available if ok and something else otherwise
     *
     * A word is used to keep EEPROM accesses word aligned - which is how it
     * likes it.
     */
    uint16_t flash_eee_available;

    /**
     * Word used to signal the reason for a reset _to_ the bootloader.
     * Will contain one of the values from #flash_eee_reset_type_e.
     * @note the bootloader will always set this to #Reset_uncontrolled
     * before launching the application.
     *
     * Enum is word-sized
     */
    flash_eee_reset_type_e next_reset;

    /**
     * Value found by the bootloader in #bootblock_eee .next_reset
     * following a reset.
     *
     * Expected to contain one of the values from #flash_eee_reset_type_e
     *
     * Enum is word-sized
     */
    flash_eee_reset_type_e last_reset;

    /**
     * Location for string written by flash_eee_log_internal_error()
     */
    char_t last_internal_error_location[INTERNAL_ERROR_LOCATION_MAXLEN];

    /**
     * Enum stating the programming options available.
     *
     * A whole word is used to leep the EEPROM word-aligned
     */
    flash_eee_reprogramming_e reprogramming_options;

    uint8_t block_validity_flags;              /* FBL only */
    uint8_t reset_response_flag;               /* not used - our version of
                                                   the FBL doesn't support it */
    /** Indication whether security delay is active or not */
    uint8_t security_access_delay_flags;
    /** Number of invalid attempts to unlock security level 2 */
    uint8_t security_access_invalid_count;
    /** Indication whether a request to decrease #security_access_invalid_count
     * has been made */
    uint8_t security_access_eng_flags;
    uint8_t dummy;      /**< Dummy byte to ensure 16-bit alignment */
} flash_eee_boot_s;

#define ISO_BOOTBLOCK           \
        ((bootblock_eee.reprogramming_options == Reprogram_iso) \
         || (bootblock_eee.reprogramming_options == Reprogram_iso_and_xcp))

#define XCP_BOOTBLOCK           \
        ((bootblock_eee.reprogramming_options == Reprogram_xcp) \
         || (bootblock_eee.reprogramming_options == Reprogram_iso_and_xcp))

/**
 * Number of logical blocks that can be reprogrammed by the FBL
 */
#define FBL_N_LOGICAL_BLOCKS            5u
/** number of fingerprint bytes containing the reprogramming date */
#define FBL_FINGERPRINT_DATE_BYTES     3u
/** number of fingerprint bytes containing the reprogramming date */
#define FBL_FINGERPRINT_SERIAL_NUM_BYTES     6u
#define FBL_FINGERPRINT_BYTES   \
                (FBL_FINGERPRINT_DATE_BYTES + FBL_FINGERPRINT_SERIAL_NUM_BYTES)

typedef struct {
    uint8_t fingerprint[FBL_FINGERPRINT_BYTES];
    uint16_t programming_counter_inverted;
    uint16_t programming_attempts_inverted;
    uint32_t crc32;
    uint32_t crc_start;
    uint32_t crc_length;
} fbl_eee_meta_s;

/**
 * Test the emulated EEPROM status.
 *
 * Implemented as a macro so that it can be executed by functions stored
 * in either flash or RAM.
 *
 * @retval Flash_eee_unavailbale        Bootblock couldn't initialise EEPROM
 * @retval Flash_eee_busy               Either a flash operation is in progress
 *                                      or nto all EEPROM writes have been
 *                                      written through to flash.
 * @retval Flash_eee_idle               EEPROM is up to date.
 */
#define FLASH_EEE_STATUS()        \
    (flash_eee_status_e) \
    ((bootblock_eee.flash_eee_available != (uint16_t) Flash_eee_available) ? \
         Flash_eee_unavailable : \
                 ((FLASH_EEE_BUSY()) ? Flash_eee_busy : Flash_eee_idle))

/**
 * Record the cause of a shutdown to emulated EEPROM without waiting for the
 * write to complete
 */
#define FLASH_EEE_RECORD_SHUTDOWN(shutdown_type) do { \
        bootblock_eee.next_reset = (shutdown_type); \
    } while (0)

/**
 * Wait for emulated EEPROM to be written through to flash.
 *
 * If the EEPROM was not initialised by the bootblock (as shown
 * by #FLASH_EEE_STATUS giving Flash_eee_unavailable) will return immediately.
 */
#define FLASH_EEE_WAIT_FOR_WRITE()      do { \
        while (FLASH_EEE_STATUS() == Flash_eee_busy) \
        { \
            /* wait for EEPROM write so that caller */ \
            /* can reset or power off */ \
        } \
    } while (0)

/**
 * index of first application logical block in #fbl_meta_info
 */
#define APP_LOGICAL_BLOCK_NUMBER        0u
/**
 * index of cal logical block in #fbl_meta_info
 */
#define CAL_LOGICAL_BLOCK_NUMBER        2u
#endif /* #ifndef FLASH_EEE_TYPES_H */
