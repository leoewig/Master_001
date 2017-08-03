/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     vprg_types.h
 *
 * description:   VTBM re-programming macros and types
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           VTBM re-programming macros and types
 *                       
 *
 *****************************************************************************/
#if !defined(VPRG_TYPES_H)
#define      VPRG_TYPES_H
/**
 *
 * VTBM re-programming macros and types
 *
 * @file
 * @ingroup vprg
 * $Id: vprg_types.h 9609 2013-07-19 16:16:05Z gvs $
 */

/* Copyright (c) Linkedriving Tech 201vprg - All rights reserved */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "utils.h"      /* for STATIC_ASSERT */
#include "mdm_types.h"      /* for MAX_NUM_VTBM */

/* allow bypassing of internal error for unit testing */
#ifdef HOSTED
extern void vprg_internal_error(void);
#define VPRG_INTERNAL_ERROR()   vprg_internal_error()
#else
#define VPRG_INTERNAL_ERROR()   INTERNAL_ERROR()
#endif

#ifndef STRNCMP
#ifdef HOSTED
#define STRNCMP         strncmp
#else
#define STRNCMP         gstrncmp
#endif
#endif

/******************************************************************************
 * Image folder layout
 *****************************************************************************/

/*
 * Identifiaction strings and their lengths for image folders, images and
 * sectors
 */
#define FOLDER_HEADER_STRING            "FOLDER"
#define FOLDER_HEADER_STRING_LEN        7u
STATIC_ASSERT(sizeof(FOLDER_HEADER_STRING) == FOLDER_HEADER_STRING_LEN,
                                FOLDER_HEADER_STRING_LEN_wrong);

#define IMAGE_HEADER_STRING            "IMAGE"
#define IMAGE_HEADER_STRING_LEN        6u
STATIC_ASSERT(sizeof(IMAGE_HEADER_STRING) == IMAGE_HEADER_STRING_LEN,
                                IMAGE_HEADER_STRING_LEN_wrong);

#define SECTOR_HEADER_STRING            "SECTOR"
#define SECTOR_HEADER_STRING_LEN        7u
STATIC_ASSERT(sizeof(SECTOR_HEADER_STRING) == SECTOR_HEADER_STRING_LEN,
                                SECTER_HEADER_STRING_LEN_wrong);

/**
 * type used to represent an image index within a folder
 */ 
typedef uint16_t image_index_t;

/**
 * Structure representing the start of the folder header.  The actual
 * header has as many image_offsets as there are images, and these are followed
 * by image names and a CRC.  Note that the CRC will be located immediately
 * before the first image, so its address can be computed from the first image
 * offset
 */
typedef struct {
    char_t header_string[FOLDER_HEADER_STRING_LEN];
    image_index_t number_of_images;
    uint32_t image_offset[1];   /* '1' here is nominal, a folder can have
                                   multiple images and each will have an
                                   entry in this array */
} image_folder_header_s;

/**
 * dummy intialiser to keep lint happy
 */
#define IMAGE_FOLDER_LINT_INITIALISER           { "", 0u, { 0u } }

/**
 * image_offset is a 32-bit number and we use the first one to deduce the
 * size of a header.  The header is checked via its CRC but we want to sanity
 * check the offset before performing the CRC
 */
#define IMAGE_FOLDER_HEADER_MAX_LENGTH  0xffffu

/**
 * Limit on the number of images that can be stored
 */
#define IMAGE_FOLDER_MAX_IMAGES  1024u

/**
 * Magic number to indicate an invalid image index
 */
#define VPRG_NO_IMAGE           0xffffu

STATIC_ASSERT(VPRG_NO_IMAGE > IMAGE_FOLDER_MAX_IMAGES,MAX_images_too_big);

/** A P-flash sector is 1024 bytes (and D-flash is less) */
#define IMAGE_SECTOR_MAX_DATA_BYTES     1024u  

/**
 * Layout of an image header
 */
typedef struct {
    char_t header_string[IMAGE_HEADER_STRING_LEN];
    uint16_t number_of_sectors;
    uint16_t crc;
} image_header_s;

/**
 * Partial layout of a sector.  An actual sector will have as many bytes
 * as the given number_of_bytes and these will be followed by a CRC.
 */
typedef struct {
    char_t header_string[SECTOR_HEADER_STRING_LEN];
    uint16_t number_of_bytes;
    uint32_t address;
    uint8_t data[1];    /* nominal length: the actual length will be equal to
                           the number_of_bytes field */
} image_sector_header_s;

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/**
 * Requested mode for VTBM reprogramming
 */
typedef enum {
    Vprg_dmd_none,              /**< nothing requested */
    Vprg_dmd_prepare,           /**< check the desired image and get ready to
                                     program it */
    Vprg_dmd_program,           /**< get the VTBMs ready, erase and re-program
                                  */
    Vprg_dmd_reset,             /**< Reset the MMC (VTBMs will reset themselves
                                  after re-programming */
    Vprg_dmd_return_to_idle,    /**< Return state to idle */
    Vprg_dmd_inconsistent,       /**< The various sources of re-programming
                                  request are contradictory */
    Vprg_dmd_unknown            /**< Couldn't make sense of the input demand */
} vprg_demand_e;

/**
 * command byte from XCP user command
 */
typedef enum {
    Vprg_xcp_cmd_check = 1u,
    Vprg_xcp_cmd_reprogram = 2u,
    Vprg_xcp_cmd_return_to_idle = 3u,
    Vprg_xcp_cmd_reset = 4u
} vprg_xcp_cmd_e;

/**
 * VTBM programming main state
 */
typedef enum {
    Vprg_state_idle,                    /**< no task requested */
    Vprg_state_checking_image,          /**< VTBM image being checked for 
                                            consistency */
    Vprg_state_ready_to_program,        /**< Image is good, waiting for
                                            programming request */
    Vprg_state_shutdown_icm,            /**< halting regular VTBM comms */
    Vprg_state_cycle_vtbms,             /**< Power cycling VTBMs */
    Vprg_state_communicating,           /**< Communicating with a VTBM */
    Vprg_state_programming_complete,    /**< all requested VTBMs have been
                                            reprogrammed */
    Vprg_state_error                    /**< an error has been detected. Will
                                            remain in this state until a
                                            return-to-idle request is received
                                            */
} vprg_state_e;


/**
 * Reason for entering error state
 * NB changes to this enum must be mirrored in bms_prog error reporting
 */
typedef enum {
    Vprg_error_none,
    Vprg_error_unknown_demand,
    Vprg_error_inconsistent_demand,
    Vprg_error_wrong_demand_for_state,
    Vprg_error_image_check_failed,
    Vprg_error_conditions_not_correct,
    Vprg_error_image_unchecked,
    Vprg_error_invalid_vtbm_mask,
    Vprg_error_programming_failed
} vprg_error_e;

/**
 * Status of the image checker
 */
typedef enum {
    Vprg_ci_state_none,          /**< no request received */
    Vprg_ci_state_checking,      /**< check in progress */
    Vprg_ci_state_ok,            /**< last check completed successfully */
    Vprg_ci_state_error          /**< last check detected an error */
} vprg_ci_state_e ;

/**
 * Error detected by the image checker
 */
typedef enum {
    Vprg_image_error_none,
    Vprg_image_error_no_folder,
    Vprg_image_error_folder_corrupt,
    Vprg_image_error_bad_image_index,
    Vprg_image_error_bad_image_header,
    Vprg_image_error_bad_sector
} vprg_image_error_e;

/**
 * sub-status for cycle vtbms state
 */
typedef enum {
    Vprg_cycle_none,            /**< no request active */
    Vprg_cycle_waiting_vtbm_off,/**< waiting for VTBM switch off to
                                        take effect */
    Vprg_cycle_waiting_vtbm_on  /**<  waiting for VTBMs to power up before
                                  starting communication */
} vprg_cycle_state_e;

/**
 * sub-status of communicating state
 */
typedef enum {
    Vprg_comms_state_none,           /**< not communicating with VTBMs */
    Vprg_comms_state_connect,        /**< creating XCP connection */
    Vprg_comms_state_program_start,  /**< starting programming session */
    Vprg_comms_state_erase,          /**< erasing each sector */
    Vprg_comms_state_program,        /**< programming each sector */
    Vprg_comms_state_program_reset,  /**< resetting VTBM after programming */
    Vprg_comms_state_complete,       /**< all done for this VTBM */
    Vprg_comms_state_error           /**< something went wrong */
} vprg_comms_state_e;


/**
 * Error detected during communications
 */
typedef enum {
    Vprg_comms_error_none,              /**< no error detected */
    Vprg_comms_error_mask_internal,     /**< an internal error with the mask
                                             logic */
    Vprg_comms_error_xcp_timeout,       /**< didn't get a response to an XCP
                                             command request */
    Vprg_comms_error_xcp_err,           /**< repsonse to an XCP request 
                                             indicated an error */
    Vprg_comms_error_xcp_cmd_unknown,   /**< XCP reports an unknown command
                                             (probably from asking an old
                                              bootloader to compute a checksum)
                                             */
    Vprg_comms_error_crc_mismatch       /**< CRC seems wrong after
                                             reprogramming a sector (even after
                                             retrying) */

} vprg_comms_error_e;

/*
 * We need to store 1 << pl_num_vtbm in a variable with type vprg_vtbm_mask_t,
 * so we can use uint16_t provided MAX_NUM_VTBM is 15 or less
 */
//STATIC_ASSERT(MAX_NUM_VTBM <= 15,MAX_NUM_VTBM_too_big_for_vtbm_mask_type);
typedef uint16_t vprg_vtbm_mask_t;

/**
 * Number of bytes of data in a CC_PROGRAM_MAX message
 */
#define VPRG_PROGRAM_MAX_BYTES  (CAN_MAX_DATA - 1u)

#endif      /* VPRG_TYPES_H */

