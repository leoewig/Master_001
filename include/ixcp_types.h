/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     ixcp_types.h
 *
 * description: XCP definitions
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
#ifndef IXCP_TYPES_H
#define IXCP_TYPES_H
/**
 * @file
 * @ingroup ixcp
 *
 * XCP definitions
 */

#define XCP_SLAVE_VERSION_NUMBER        0x10u
                                        /**< byte reported in repsonse to
                                          GET_COMM_MODE_INFO command.  Format is
                                          Major nibble.minor nibble,
                                          e.g. version 1.0 is 0x10 */

#if defined MMC  || defined BBLK_MMC || defined MMC_TEST_HARNESS \
    || defined PRODDER
/*
 * MMC internal-bus specific definitions
 */
#define IXCP_CRO_ID     0x70du   /**< Internal bus CRO ID
                                   @note This value is also hardcoded
                                   in the Makefiles */
#define IXCP_DTO_ID     0x5fdu   /**< Internal bus DTO 
                                   @note This value is also hardcoded
                                   in the Makefiles */
#if defined BBLK_MMC
#define A2L_INTERNAL_FILENAME    "bblk_mmc_internal" 
                                /**< name of the internal bus ASAP2 file minus
                                  the .a2l suffix. 
                                  @note this value is also hardcoded
                                  in the Makefile */
#define A2L_VEHICLE_FILENAME    "bblk_mmc_vehicle" 
                                /**< name of the vehicle bus ASAP2 file minus
                                  the .a2l suffix. 
                                  @note this value is also hardcoded
                                  in the Makefile */
#define A2L_FILENAME_LENGTH     sizeof(A2L_INTERNAL_FILENAME) /**< 
                                   @note must be sizeof the longer of the two
                                   names */
#else   /* MMC */
/*
 * Vehicle bus ID is handled by CANbedded so not needed here
 */
#define A2L_FILENAME    "mmc_internal"  /**< name of the ASAP2 file minus the
                                          .a2l suffix. 
                                          @note this value is also hardcoded
                                          in the Makefile */
#endif
/*
 * MMC vehicle-bus specific definitions - only used by bootblock because
 * Application values are defined through CANbedded
 */
#if defined BBLK_MMC
#define VEHICLE_XCP_CRO_ID              0x43fu  /**< Vehicle bus CRO ID
                                                  @note This value is also
                                                  hardcoded in the
                                                  CANbedded configuration and
                                                  the Makefiles */
#define VEHICLE_XCP_DTO_ID              0x43eu  /**< Vehicle bus DTO ID
                                                  @note This value is also
                                                  hardcoded in the
                                                  CANbedded configuration and
                                                  the Makefiles */
#endif
#elif !defined PRODDER   /* not MMC or BBLK_MMC so VTBM-related */
/*
 * VTBM and VTBM bootblock specific definitions
 */

/*
 * Preprocessor symbol A2L_FILENAME_BASE must contain the prefix for the
 * A2L filename to which the ID string will be added,
 * e.g. vtbm -> vtbm00.a2l, vtbm01.a2l, etc.
 */
#ifndef A2L_FILENAME_BASE
#error VTBM-related applications must define A2L_FILENAME_BASE
#endif

/**
 * Convert the preprocessor symbol A2L_FILENAME_BASE into a string
 */
#define A2L_FILENAME_BASE_STR   A2L_FILENAME_BASE_TO_STRING1(A2L_FILENAME_BASE)
/**
 * Usual helper macro to expand argument before invoking the # stringification
 */
#define A2L_FILENAME_BASE_TO_STRING1(name)       \
                                        A2L_FILENAME_BASE_TO_STRING2(name)
/**
 * Usual helper macro to invoke the # stringification
 */
#define A2L_FILENAME_BASE_TO_STRING2(name)       #name
#define A2L_MAX_ID_DIGITS       2u      /**< enough for up to 100 VTBMs */
#define A2L_FILENAME_LENGTH     \
                        (sizeof(A2L_FILENAME_BASE_STR) + A2L_MAX_ID_DIGITS)
                        /**< size of buffer needed for the A2L filename
                         * once the ID string has been appended */
#endif

#define IXCP_CRO_ID_BASE     0x4e0u   /**< CRO ID of VTBM 0.  Other CROs are
                                        offset by the VTBM board ID.
                                        Required by VTBM xcp slaves and the
                                        MMC vprg feature.
                                        @note This value is also hardcoded
                                        in the vtbm Makefile */
#define IXCP_DTO_ID_BASE     0x680u   /**< DTO ID of VTBM 0.  Other DTOs are
                                        offset by the VTBM board ID
                                        Required by VTBM xcp slaves and the
                                        MMC vprg feature.
                                        @note This value is also hardcoded
                                        in the vtbm Makefile */

/**
 * XCP status for a command, Xcp_ok is used to flag success,
 * all other values indicate an error
 */
typedef enum {
    /*
     * defined XCP error codes
     */
    Ixcp_err_cmd_synch=0,
    Ixcp_err_cmd_busy=0x10,
    Ixcp_err_daq_active=0x11,
    Ixcp_err_pgm_active=0x12,
    Ixcp_err_cmd_unknown=0x20,
    Ixcp_err_cmd_syntax=0x21,
    Ixcp_err_out_of_range=0x22,
    Ixcp_err_write_protected=0x23,
    Ixcp_err_access_denied=0x24,
    Ixcp_err_access_locked=0x25,
    Ixcp_err_page_not_valid=0x26,
    Ixcp_err_mode_not_valid=0x27,
    Ixcp_err_segment_not_valid=0x28,
    Ixcp_err_sequence=0x29,
    Ixcp_err_daq_config=0x2a,
    Ixcp_err_memory_overflow=0x30,
    Ixcp_err_generic=0x31,
    Ixcp_err_verify=0x32,
    /**
     * a dummy value for convenience when the DTO identifier
     * is not #Ixcp_dto_err
     */
    Ixcp_err_none=0xff
} ixcp_error_code_e;

/**
 * Enum of values for the first byte of a DTO
 */
typedef enum {
    Ixcp_dto_res=0xff,          /**< DTO contains a positive response */
    Ixcp_dto_err=0xfe,          /**< DTO contains a negative response */
    /*
     * our own codes:
     */
    Ixcp_dto_pending=0       /**<  command will be processed by the idle loop
                                   so no response should be sent yet */
} ixcp_dto_type_e;

/**
 * DTO command codes
 */
typedef enum {
    Ixcp_cc_connect = 0xff,
    Ixcp_cc_disconnect = 0xfe,
    Ixcp_cc_get_status = 0xfd,
    Ixcp_cc_get_comm_mode_info = 0xfb,
    Ixcp_cc_get_id = 0xfa,
    Ixcp_cc_set_mta = 0xf6,
    Ixcp_cc_upload = 0xf5,
    Ixcp_cc_short_upload = 0xf4,
    Ixcp_cc_build_checksum = 0xf3,
    Ixcp_cc_download = 0xf0,
    Ixcp_cc_download_max = 0xee,
    Ixcp_cc_set_cal_page = 0xeb,
    Ixcp_cc_get_cal_page = 0xea,
    Ixcp_cc_copy_cal_page = 0xe4,
    Ixcp_cc_program_start = 0xd2,
    Ixcp_cc_program_clear = 0xd1,
    Ixcp_cc_program = 0xd0,
    Ixcp_cc_program_reset = 0xcf,
    Ixcp_cc_program_max = 0xc9
} ixcp_cc_e;

/**
 * Requested Indentification Type in GET_ID command
 */
typedef enum {
    Ixcp_reqid_ascii_text = 0,
    Ixcp_reqid_a2l_without_path_and_extension = 1,
    Ixcp_reqid_a2l_with_path_and_extension = 2,
    Ixcp_reqid_url = 3,
    Ixcp_a2l_upload = 4
} ixcp_reqid_e;

#define IXCP_RESSOURCE_CALPAG           1u      /**< Ressource bit indicating
                                                 Calibration/Paging available */
#define IXCP_RESSOURCE_DAQ              4u      /**< Ressource bit indicating
                                                  DAQ lists supported */
#define IXCP_RESSOURCE_STIM             8u      /**< Ressource bit indicating
                                                  Stimulation available */
#define IXCP_RESSOURCE_PGM              0x10u   /**< Ressource bit indicating
                                                  Flash programming available */

#define IXCP_MODE_ALL_SEGMENTS          0x80u   /**< bit in SET_CAL_PAGE mode
                                                  byte which indicates command
                                                  applies to all segments */
#define IXCP_MODE_PAGE_ECU              0x01u   /**< bit in SET_CAL_PAGE mode
                                                  byte which indicates page
                                                  is for ECU access */
#define IXCP_MODE_PAGE_XCP              0x02u   /**< bit in SET_CAL_PAGE mode
                                                  byte which indicates page
                                                  is for XCP access */

/**
 * Access mode for PROGRAM_CLEAR is absolute, i.e. normal
 */
#define IXCP_PROG_CLEAR_MODE_ABS   0u

#ifdef MMC
/**
 * MMC supports calibration and reprogramming
 */
#define IXCP_RESSOURCE_BYTE         (IXCP_RESSOURCE_CALPAG | IXCP_RESSOURCE_PGM)
#else
/**
 * Everything else (VTBM and both bootblocks) supports reprogramming only
 */
#define IXCP_RESSOURCE_BYTE         IXCP_RESSOURCE_PGM
#endif

#define IXCP_COMM_MODE_BYTE_ORDER_MOTOROLA  1u      /**< Motorola format */
#define IXCP_COMM_MODE_BYTE_GRANULARITY     0u      /**< Byte granularity */

/**
 * Value of COMM_MODE_BASIC in the response to a CONNECT request.
 * @note the declared byte order must match the natural byte order of the
 * processor.
 */
#define IXCP_COMM_MODE_BASIC_BYTE           \
        (IXCP_COMM_MODE_BYTE_ORDER_MOTOROLA | IXCP_COMM_MODE_BYTE_GRANULARITY)

#define IXCP_PROTOCOL_LAYER_VERSION     1u   /**< implemented protocol layer
                                      version as reported by CONNECT response */
#define IXCP_TRANSPORT_LAYER_VERSION    1u   /**< implemented transport layer
                                      version as reported by CONNECT response */

#define IXCP_CSUM_CRC16_CITT            8u      /**< only supported checksum
                                                  calculation */
#define IXCP_CSUM_SEED                  0xffffu /**< mandated seed for
                                                  CRC16_CITT checksum */
#endif /* ifndef IXCP_TYPES_H */
