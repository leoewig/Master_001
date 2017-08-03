/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    ixcp_slave.c
 *
 * description: 
 * XCP Slave
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Temperature conversion module
 *
 *****************************************************************************/
/**
 * XCP Slave
 *
 * @file
 * @ingroup ixcp
 *
 *
 * # A word about lint suppression
 *
 * This file has been written with the assumption that the byte order given
 * by the .a2l file and #IXCP_COMM_MODE_BASIC_BYTE matches the natural byte
 * order for the processor.  With this assumption, multi-byte reads and writes
 * into and from can messages can be managed by the compiler.
 *
 * Lint, quite rightly, objects to this so a number of 'suspicious pointer to
 * pointer conversions' have been suppressed.
 */
/* Copyright (c) Linkedriving Tech 2012 */

/*
 * set up configuration options according to the target
 */
#if defined VTBM || defined BBLK_VTBM
#define VTBM_IDS
#endif

#if defined VTBM_IDS || defined BBLK_MMC
#define RUNTIME_IDS
#endif

#ifdef BBLK
#define REPROGRAMMING_SLAVE
#if !defined HOSTED
#define CODE_IN_RAM
#endif
#endif

#ifdef MMC
#define CAL_ENABLED
#endif

#if defined REPROGRAMMING_SLAVE || defined CAL_ENABLED
#define CRC_ENABLED
#endif

#include "basic_types.h"
#include "hardware.h"           /* for SAVE/RESTORE_INTERRUPTS */
//#include "ican.h"
#ifdef CAL_ENABLED
#include "cal.h"
#include "ms.h"
#endif
#ifdef CRC_ENABLED
#include "crc.h"
#endif
#include "cosmic_pointers.h"
#include "ixcp.h" 
#include "ixcp_p.h" 

#ifdef VTBM_IDS
#include "vtbm_board_id.h"
#endif
#ifdef BBLK
#include "bblk_prog.h"  /* for bblk_tx() */
#include "watchdog.h"
#endif

#ifndef REPROGRAMMING_SLAVE
#include "flash_eee.h"
#endif

#ifdef REPROGRAMMING_SLAVE
FPTR uint8_t *ixcp_mta;                 /**< Pointer to memory to be read
                                          or written over XCP */
#else
PRIVATE FPTR uint8_t *ixcp_mta;         /**< Pointer to memory to be read
                                          or written over XCP,
                                          shared with ixcp_prog.c */
#endif

PRIVATE bool_t ixcp_connected = FALSE;  /**< State of XCP connection */

#ifdef CRC_ENABLED
/** 
 * Maximum number of bytes sent to crc16() in one go.
 * Bigger requests are split into multiple calls.  On a Ricardo bootloader,
 * times to calculate a CRC of this size _must_ be less than the watchdog
 * period.  Measurements show that a block of 0x4000 bytes takes about 21ms.
 * Also, must fit in a uint16_t.
 */
#define IXCP_CSUM_CHUNK         0x4000u

PRIVATE FPTR uint8_t *ixcp_checksum_start;      /**< Points to start of region
                                                  to be checksummed */
PRIVATE uint32_t ixcp_checksum_length;          /**< number of bytes ot be
                                                  checksumed */
#endif

#ifdef RUNTIME_IDS
PRIVATE char_t a2l_filename[A2L_FILENAME_LENGTH]; /**< A2L filename which is
                                                determined in ixcp_init_vtbm
                                                or ixcp_init_bblk_mmc */
uint16_t ixcp_cro_id;
PRIVATE uint16_t ixcp_dto_id;
#else
PRIVATE CONST char_t a2l_filename[] = A2L_FILENAME; /**< MMC has a fixed A2L
                                                       filename */
PRIVATE const uint16_t ixcp_dto_id = IXCP_DTO_ID;
#endif

#ifdef CODE_IN_RAM
#pragma section (ramcode)
#endif


#ifdef VTBM_IDS

#if A2L_MAX_ID_DIGITS != 2u
#error A2L filename generation assumes a two-digit VTBM id
#endif

/* A2L base filename, it is declared as a variable because we cannot rely on
 * the 'const' version of the string in flash when running out of RAM.
 * It is unclear whether this ever is accessed once the flash is erased, but
 * the function accessing it is running in RAM, so relying only on RAM data is
 * safer.
 **/
#ifdef CODE_IN_RAM
PRIVATE char_t a2l_base_filename[A2L_FILENAME_LENGTH] = A2L_FILENAME_BASE_STR; 
#else
PRIVATE CONST char_t a2l_base_filename[A2L_FILENAME_LENGTH] = A2L_FILENAME_BASE_STR; 
#endif

/**
 * function to calculate VTBM ID-specific XCP information.
 * Must be called after the board ID has been discovered
 */
void ixcp_init_vtbm(void)
{
//    char_t board_id_string[A2L_MAX_ID_DIGITS+1u];  /* +1 for trailing '\0' */
//    uint8_t id_tens;
//    uint8_t id_units;
//    char_t id_tens_char;
//    char_t id_units_char;
//
//    id_tens = vtbm_board_id / 10u;
//    id_tens_char = (char_t) '0' + (char_t) id_tens;
//    id_units = vtbm_board_id % 10u;
//    id_units_char = (char_t) '0' + (char_t) id_units;
//
//    ixcp_strcpy(a2l_filename, a2l_base_filename);
//    board_id_string[0] = id_tens_char;
//    board_id_string[1] = id_units_char;
//    board_id_string[2] = '\0';
//    ixcp_strcat(a2l_filename, board_id_string);
//
//    ixcp_cro_id = IXCP_CRO_ID_BASE + (uint16_t) vtbm_board_id;
//    ixcp_dto_id = IXCP_DTO_ID_BASE + (uint16_t) vtbm_board_id;
}
#elif defined BBLK_MMC
void ixcp_init_bblk_mmc(enum can_handle_e bus)
{
//    if (bus == Can_internal)
//    {
//        ixcp_strcpy(a2l_filename, A2L_INTERNAL_FILENAME);
//        ixcp_cro_id = IXCP_CRO_ID;
//        ixcp_dto_id = IXCP_DTO_ID;
//    }
//    else
//    {
//        ixcp_strcpy(a2l_filename, A2L_VEHICLE_FILENAME);
//        ixcp_cro_id = VEHICLE_XCP_CRO_ID;
//        ixcp_dto_id = VEHICLE_XCP_DTO_ID;
//    }
}
#endif

/**
 * Called whenever an XCP command request object (CRO) is received.
 *
 * If XCP is not connected and this is not a connection request, the message
 * is ignored.  Otheriwse, the data is passed to ixcp_read_cro()
 */
void ixcp_cro(const struct ican_msg_s *msgp)
{
  // if ((ixcp_connected == FALSE)
  //     && ((msgp->dlc < 1u) || (msgp->data[0] != (uint8_t) Ixcp_cc_connect)))
  // {
  //     /* 
  //      * we're not connected and this isn't a connection message,
  //      * so just ignore it
  //      */
  // }
  // else
  // {
  //     ixcp_read_cro(msgp->data);
  // }
}


/**
 * Send a data packet back to the XCP master.
 * @param dto_type      DTO message type, e.g. result message (Ixcp_dto_res)
 *                      or error message (Ixcp_dto_err)
 * @param dto_payload   Data for the message _excluding_ the message type.
 *                      Must point to an array of 7 bytes (#CAN_MAX_DATA-1).
 *
 * @note This function might be called from the idle task in response to an
 * EEPROM manipulation request, so the call to icantx is made atomically
 */
void ixcp_send_dto(ixcp_dto_type_e dto_type,const uint8_t *dto_payload)
{
//    uint8_t can_data[CAN_MAX_DATA];
//    uint8_t i;
//#ifndef BBLK
//    uint8_t ccr;
//#endif
//
//    can_data[0] = (uint8_t) dto_type;
//
//    for (i = 1u ; i < CAN_MAX_DATA ; i++)
//    {
//        can_data[i] = dto_payload[i-1u];
//    }
//
//#ifdef BBLK
//    bblk_tx(ixcp_dto_id,CAN_MAX_DATA,can_data);
//#else
//    SAVE_INTERRUPTS(ccr);
//    //(void) icantx(ixcp_dto_id,Can_internal,CAN_MAX_DATA,
//     //                   (FPTR const uint8_t *) can_data);
//    RESTORE_INTERRUPTS(ccr);
//#endif
}

/**
 * Writes the contents of an Error DTO _excluding_ the type byte into
 * the suplied data array.
 * @param error_code    XCP error to be reported
 * @param dto_payloadp  Pointer at which error_code is written
 *
 * Caller can write to remaining bytes in dto_payload to send back additional
 * information.
 *
 * @retval Ixcp_dto_err
 */
ixcp_dto_type_e ixcp_set_error(ixcp_error_code_e error_code,
                                        uint8_t *dto_payloadp)
{
//    dto_payloadp[0] = (uint8_t) error_code;
//
//    return Ixcp_dto_err;
}

#ifdef MMC
PRIVATE bool_t ixcp_calculate_checksum_request; /**< Set when foreground wants
                                                  the background to calculate
                                                  a checksum.  Cleared when the
                                                  calculation is complete */
/**
 * Called by the idle loop to calculate CRCs
 */
void ixcp_idle_tasks(void)
{
//    if (ixcp_calculate_checksum_request)
//    {
//        ixcp_calculate_checksum();
//        ixcp_calculate_checksum_request = FALSE;
//    }
}
#endif

#ifdef REPROGRAMMING_SLAVE
/**
 * Function to allow a bootblock to resume a session that was started by 
 * the application.
 *
 * All it needs to do is set the connection state to Connected.
 */
void ixcp_resume_connection(void)
{
//    ixcp_connected = TRUE;
}
#endif


/**
 * Generate a Sequence Error DTO because, for example, a programming command
 * was not preceded by a PROGRAM_START
 */
ixcp_dto_type_e ixcp_sequence_error(uint8_t *dto_payloadp)
{
//    return ixcp_set_error(Ixcp_err_sequence,dto_payloadp);
}

/**
 * Generate an Unknown Command error DTO
 */
ixcp_dto_type_e ixcp_unknown_command(uint8_t *dto_payloadp)
{
 //   return ixcp_set_error(Ixcp_err_cmd_unknown,dto_payloadp);
}


/**
 * Processes a CRO and sends a DTO unless it must be left to the background
 * to complete.
 *
 * @param cro_data      Contents of the CRO message including the command code
 */
PRIVATE void ixcp_read_cro(const uint8_t *cro_data)
{
//    ixcp_cc_e command_code;
//    ixcp_dto_type_e dto_type;
//    uint8_t i;
//    uint8_t dto_payload[CAN_MAX_DATA-1u];
//    command_code = (ixcp_cc_e) cro_data[0];
//
//    /*
//     * Initialise the response to all zeros.
//     * NB We have to use a loop here rather than initialising the definition
//     * because an initialiser will end up in ROM but we need this code to
//     * be executable from RAM
//     */
//    for (i = 0u ; i < (CAN_MAX_DATA - 1u) ; i++)
//    {
//        dto_payload[i] = 0u;
//    }
//
//    switch (command_code)
//    {
//        case Ixcp_cc_connect:
//            dto_type = ixcp_connect(dto_payload);
//            break;
//        case Ixcp_cc_disconnect:
//            dto_type = ixcp_disconnect();
//            break;
//        case Ixcp_cc_get_status:
//            dto_type = ixcp_get_status();
//            break;
//        case Ixcp_cc_get_comm_mode_info:
//            dto_type = ixcp_get_comm_mode_info(dto_payload);
//            break;
//        case Ixcp_cc_get_id:
//            dto_type = ixcp_get_id(cro_data,dto_payload);
//            break;
//        case Ixcp_cc_set_mta:
//            dto_type = ixcp_set_mta(cro_data,dto_payload);
//            break;
//        case Ixcp_cc_upload:
//            dto_type = ixcp_upload(cro_data,dto_payload);
//            break;
//        case Ixcp_cc_short_upload:
//            dto_type = ixcp_short_upload(cro_data,dto_payload);
//            break;
//#ifdef REPROGRAMMING_SLAVE
//        case Ixcp_cc_program_start:
//            dto_type = ixcp_program_start_reprog(dto_payload);
//            break;
//        case Ixcp_cc_program_clear:
//            dto_type = ixcp_program_clear(cro_data,dto_payload);
//            break;
//        case Ixcp_cc_program:
//            dto_type = ixcp_program(cro_data,dto_payload);
//            break;
//        case Ixcp_cc_program_max:
//            dto_type = ixcp_program_max(cro_data,dto_payload);
//            break;
//        case Ixcp_cc_program_reset:
//            dto_type = ixcp_program_reset(dto_payload);
//            break;
//#else
//        case Ixcp_cc_program_start:
//            dto_type = ixcp_program_start_app(dto_payload);
//            break;
//        case Ixcp_cc_program_clear:
//        case Ixcp_cc_program:
//        case Ixcp_cc_program_max:
//        case Ixcp_cc_program_reset:
//            /* programming commands received before a PROGRAM_START */
//            dto_type = ixcp_sequence_error(dto_payload);
//            break;
//#endif
//#ifdef CAL_ENABLED
//            /* calibration-related calls are MMC only */
//        case Ixcp_cc_get_cal_page:
//            dto_type = ixcp_get_cal_page(dto_payload);
//            break;
//        case Ixcp_cc_set_cal_page:
//            dto_type = ixcp_set_cal_page(cro_data,dto_payload);
//            break;
//        case Ixcp_cc_copy_cal_page:
//            dto_type = ixcp_copy_cal_page(cro_data,dto_payload);
//            break;
//        case Ixcp_cc_download:
//            dto_type = ixcp_download(cro_data,dto_payload);
//            break;
//        case Ixcp_cc_download_max:
//            dto_type = ixcp_download_max(cro_data,dto_payload);
//            break;
//#else
//        case Ixcp_cc_get_cal_page:
//        case Ixcp_cc_set_cal_page:
//        case Ixcp_cc_copy_cal_page:
//        case Ixcp_cc_download:
//        case Ixcp_cc_download_max:
//            dto_type = ixcp_unknown_command(dto_payload);
//            break;
//#endif
//#ifdef CRC_ENABLED
//        case Ixcp_cc_build_checksum:
//            dto_type = ixcp_build_checksum(cro_data);
//            break;
//#else
//        case Ixcp_cc_build_checksum:
//            dto_type = ixcp_unknown_command(dto_payload);
//            break;
//#endif
//        default:
//            dto_type = ixcp_unknown_command(dto_payload);
//            break;
//    }
//    if (dto_type != Ixcp_dto_pending)
//    {
//        /* 
//         * The command either succeeded, so we need to send back a result,
//         * or it failed so we need to send back an error.
//         * The third option is that the command will take a long time, so
//         * the idle tasks must make their own call to ixcp_send_dto()
//         */
//        ixcp_send_dto(dto_type,dto_payload);
//    }
}
/**
 * Respond to a connect request
 *
 * Returns the following:
 * Byte              | Meaning                         | Value
 * ------------------|---------------------------------|-------
 * dto_payloadp[0]   | Ressource (sic)                 | #IXCP_RESSOURCE_BYTE
 * dto_payloadp[1]   | COMM_MODE_BASIC                 | #IXCP_COMM_MODE_BASIC_BYTE
 * dto_payloadp[2]   | MAX_CTO                         | 8 (CAN_MAX_DATA)
 * dto_payloadp[3,4] | MAX_DTO                         | 8 (CAN_MAX_DATA)
 * dto_payloadp[5]   | XCP protocol layer version no.  | 1
 * dto_payloadp[6]   | XCP transport layer version no. | 1
 *
 * @retval #Ixcp_dto_res
 */
PRIVATE ixcp_dto_type_e ixcp_connect(uint8_t *dto_payloadp)
{
//    ixcp_connected = TRUE;
//    dto_payloadp[0] = IXCP_RESSOURCE_BYTE;
//    dto_payloadp[1] = IXCP_COMM_MODE_BASIC_BYTE;
//    dto_payloadp[2] = CAN_MAX_DATA;
//    dto_payloadp[3] = 0u;                  /* high byte of MAX_DTO */
//    dto_payloadp[4] = CAN_MAX_DATA;
//    dto_payloadp[5] = IXCP_PROTOCOL_LAYER_VERSION;
//    dto_payloadp[6] = IXCP_TRANSPORT_LAYER_VERSION;
//    return Ixcp_dto_res;
}

/**
 * Respond to a disconnection request
 */
PRIVATE ixcp_dto_type_e ixcp_disconnect(void)
{
//    ixcp_connected = FALSE;
//    return Ixcp_dto_res;
}

/**
 * Returns the following:
 * Byte              | Meaning                            | Value
 * ------------------|------------------------------------|-------
 * dto_payloadp[0]   | Current session status             | 0
 * dto_payloadp[1]   | Current resource protection status | 0
 * dto_payloadp[2]   | Reserved                           |
 * dto_payloadp[3,4] | Session configuration id           | 0
 *
 * Since we can rely on the caller having zeroed dto_payloadp, no action
 * is necessary.
 *
 * @retval #Ixcp_dto_res
 */
PRIVATE ixcp_dto_type_e ixcp_get_status(void)
{
 //   return Ixcp_dto_res;
}


/**
 * Returns the following:
 * Byte            | Meaning            | Value
 * ----------------|--------------------|-------
 * dto_payloadp[0] | Reserved           |
 * dto_payloadp[1] | COMM_MODE_OPTIONAL | 0 (no block mode, non-interleaved)
 * dto_payloadp[2] | Reserved           | 
 * dto_payloadp[3] | MAX_BS             | 0 (no block mode)
 * dto_payloadp[4] | MIN_ST             | 0 (no block mode)
 * dto_payloadp[5] | QUEUE_SIZE         | 0 (non-interleaved)
 * dto_payloadp[6] | XCP Driver version | #XCP_SLAVE_VERSION_NUMBER
 *
 * Since we can rely on dto_payloadp being zeroed by the caller, only the
 * slave version number needs writing.
 *
 * @retval #Ixcp_dto_res
 */
PRIVATE ixcp_dto_type_e ixcp_get_comm_mode_info(uint8_t *dto_payloadp)
{
    dto_payloadp[6] = XCP_SLAVE_VERSION_NUMBER;

    return Ixcp_dto_res;
}

/**
 * Gives XCP master access to the ASAP2 file name for this strategy.
 *
 * The requested identification type is passed in byte 1 of cro_datap[].  If
 * the type is #Ixcp_reqid_a2l_without_path_and_extension, the #ixcp_mta is set
 * to the address of #a2l_filename.  If the type is anything different, ixcp_mta
 * is not changed and the length bytes are set to zero.  This is as required
 * by the XCP standard.
 *
 * Byte              | Meaning      | Value
 * ------------------|--------------|-------
 * dto_payloadp[0]   | Mode         | 0 (mta is set to point to the answer)
 * dto_payloadp[1,2] | reserved     | 
 * dto_payloadp[3,6] | Length of ID | strlen(#a2l_filename)
 *
 * Since we can rely on dto_payloadp being zeroed by the caller, only the length
 * needs to be written.
 *
 * @retval #Ixcp_dto_res
 */
PRIVATE ixcp_dto_type_e ixcp_get_id(const uint8_t *cro_datap,
                                        uint8_t *dto_payloadp)
{
//    ixcp_reqid_e type;
//    uint16_t namelength;
//
//
//    type = (ixcp_reqid_e) cro_datap[1];
//
//    if (type == Ixcp_reqid_a2l_without_path_and_extension)
//    {
//        namelength = ixcp_strlen(a2l_filename);
//        ixcp_mta = (FPTR uint8_t *) a2l_filename;
//    }
//    else
//    {
//        /* unsupported request */
//        namelength = 0u;
//    }
//
//    /**
//     * @lint 826 suspicious pointer-to-pointer conversion
//     *            when writing a 2-byte length into four bytes in the DTO
//     */
//    /*lint -e{826} */
//    *(uint32_t *) &dto_payloadp[3] = namelength;
//
//    return Ixcp_dto_res;
}

/**
 * Sets #ixcp_mta to the requested value.
 *
 * Five byte address extension and address at cro_datap[3] is passed to
 * ixcp_set_mta_from_cro()
 * 
 * @returns return value from ixcp_set_mta_from_cro()
 */
PRIVATE ixcp_dto_type_e ixcp_set_mta(const uint8_t *cro_datap,uint8_t *dto_payloadp)
{
//    ixcp_dto_type_e return_code;
//
//    return_code = ixcp_set_mta_from_cro(cro_datap+3,dto_payloadp);
//
//    return return_code;
}

/**
 * Upload data from address previously stored in #ixcp_mta
 * 
 * @param[in] cro_datap Number of bytes to upload is stored in cro_datap[1]
 * @param[out] dto_payloadp Requested number of bytes, up to a maximum of 7
 *              (#CAN_MAX_DATA - 1) is written here
 *
 * A side effect is that #ixcp_mta is increased by the number of bytes read
 *
 * @retval Ixcp_dto_err More than 7 bytes was requested
 * @retval Ixcp_dto_res
 */
PRIVATE ixcp_dto_type_e ixcp_upload(const uint8_t *cro_datap,uint8_t *dto_payloadp)
{
//    uint8_t len;
//    ixcp_dto_type_e return_code;
//
//    len = cro_datap[1];
//
//    return_code = ixcp_do_upload(len,dto_payloadp);
//
//    return return_code;
}

/**
 * Upload data from given address
 * 
 * @param[in] cro_datap Input CAN message with the format in the table below
 * @param[out] dto_payloadp Requested number of bytes, up to a maximum of 7
 *              (#CAN_MAX_DATA - 1) is written here
 *
 * # Input
 * CRO Byte       | Meaning     
 * ---------------|--------------
 * cro_datap[0]   | Command code 
 * cro_datap[1]   | number of bytes to read
 * cro_datap[2]   | reserved 
 * cro_datap[3]   | address extension (must be zero)
 * cro_datap[4-7] | address of first byte to read
 *
 * # Response
 * Requested data in dto_payloadp.
 *
 * A side effect is that #ixcp_mta is set to the address following the data read
 *
 * @retval Ixcp_dto_err More than 7 bytes was requested or non-zero address
 *                      extension
 * @retval Ixcp_dto_res
 */
PRIVATE ixcp_dto_type_e ixcp_short_upload(const uint8_t *cro_datap,
                                                uint8_t *dto_payloadp)
{
//    uint8_t len;
//    ixcp_dto_type_e set_mta_code;
//    ixcp_dto_type_e return_code;
//
//
//    len = cro_datap[1];
//    set_mta_code = ixcp_set_mta_from_cro(cro_datap+3u,dto_payloadp);
//
//    if (set_mta_code == Ixcp_dto_res)
//    {
//        /* reading MTA went ok so now read the data */
//        return_code = ixcp_do_upload(len,dto_payloadp);
//    }
//    else
//    {
//        return_code = set_mta_code;
//    }
//
//    return return_code;
}

#ifdef CAL_ENABLED
/**
 * Get the current calibration page.
 *
 * The CRO will contain mode and segment information but since neither
 * will have any bearing on our answer, we ignore it.
 *
 * # Response
 * Byte            | Meaning                  | Value
 * ----------------|--------------------------|-------
 * dto_payloadp[2] | Logical data page number | 0: Flash page, 1: RAM page
 *
 */
PRIVATE ixcp_dto_type_e ixcp_get_cal_page(uint8_t *dto_payloadp)
{
//    cal_page_e cal_page;
//
//    cal_page = cal_get_cal_page();
//
//    dto_payloadp[2] = (cal_page == Cal_page_flash) ? 0u : 1u;
//    return Ixcp_dto_res;
}

/**
 * Set the current cal page
 *
 * # Input
 * CRO Byte       | Meaning     
 * ---------------|--------------
 * cro_datap[1]   | Mode
 * cro_datap[2]   | Segment number
 * cro_datap[3]   | Page number (0: flash, 1: RAM)
 *
 * # Response
 * No data in DTO if successful.  Errors set can be:
 * + #Ixcp_err_mode_not_valid
 * + #Ixcp_err_segment_not_valid
 * + #Ixcp_err_page_not_valid
 *
 * @retval Ixcp_dto_res Either page has switched or nothing needed doing
 * @retval Ixcp_dto_err Request is not supported (and appropriate error code
 *              written into dto_payloadp)
 * @retval Ixcp_dto_pending Request will take a long time so background will
 *              send the DTO itself.
 */
PRIVATE ixcp_dto_type_e ixcp_set_cal_page(const uint8_t *cro_datap,
                                                uint8_t *dto_payloadp)
{
//    cal_status_e cal_status;
//    cal_page_e cal_page;
//    ixcp_dto_type_e ixcp_status;
//    uint8_t mode;
//    uint8_t segment;
//    uint8_t page;
//
//    mode = cro_datap[1];
//    segment = cro_datap[2];
//    page = cro_datap[3];
//
//    if (!(mode & (IXCP_MODE_PAGE_XCP | IXCP_MODE_PAGE_ECU)))
//    {
//        /* we only support simultaneous access by both XCP and ECU so this is
//         * a bad mode */
//        ixcp_status = ixcp_set_error(Ixcp_err_mode_not_valid,dto_payloadp);
//    }
//    else if (!(mode & IXCP_MODE_ALL_SEGMENTS) && (segment != 0u))
//    {
//        /* Been asked for a single segment which isn't segment 0 */
//        ixcp_status = ixcp_set_error(Ixcp_err_segment_not_valid,dto_payloadp);
//    }
//    else if (page > 1u)
//    {
//        /* valid pages are 0: flash and 1: RAM */
//        ixcp_status = ixcp_set_error(Ixcp_err_page_not_valid,dto_payloadp);
//    }
//    else
//    {
//        cal_page = (page == 0u) ? Cal_page_flash : Cal_page_ram;
//        cal_status = cal_set_cal_page(Cal_slave_ixcp,cal_page);
//
//        switch (cal_status)
//        {
//            case Cal_success:
//                ixcp_status = Ixcp_dto_res;
//                break;
//            case Cal_pending:
//                /* request will take time so can't send DTO yet */
//                ixcp_status = Ixcp_dto_pending;
//                break;
//            case Cal_failure:
//            default:
//                /* something unobvious went wrong so send a generic error */
//                ixcp_status = ixcp_set_error(Ixcp_err_generic,dto_payloadp);
//                break;
//        }
//    }
//    return ixcp_status;
}

/**
 * Service request to copy a CAL page.
 *
 * # Input
 * CRO Byte       | Meaning     
 * ---------------|--------------
 * cro_datap[1]   | Source segment (only 0 is supported)
 * cro_datap[2]   | Source page
 * cro_datap[3]   | Destination segment
 * cro_datap[4]   | Destination page
 *
 * # Response
 * Nothing if successful.  Errors set can be:
 * + #Ixcp_err_segment_not_valid (any segment isn't 0)
 * + #Ixcp_err_page_not_valid (pages aren't 0 and 1 or aren't different)
 * + #Ixcp_err_write_protected (request to copy RAM over flash)
 *
 * @retval Ixcp_dto_res Either page has been copied or nothing needed doing
 * @retval Ixcp_dto_err Request is not supported (and appropriate error code
 *              written into dto_payloadp)
 * @retval Ixcp_dto_pending Request will take a long time so background will
 *              send the DTO itself.
 */
PRIVATE ixcp_dto_type_e ixcp_copy_cal_page(const uint8_t *cro_datap,
                                                uint8_t *dto_payloadp)
{
//    uint8_t source_segment;
//    uint8_t source_page;
//    uint8_t destination_segment;
//    uint8_t destination_page;
//    cal_status_e cal_status;
//    ixcp_dto_type_e ixcp_status;
//
//    source_segment = cro_datap[1];
//    source_page = cro_datap[2];
//    destination_segment = cro_datap[3];
//    destination_page = cro_datap[4];
//
//    if ((source_segment != 0u) || (destination_segment != 0u))
//    {
//        /* only segment 0 is valid */
//        ixcp_status = ixcp_set_error(Ixcp_err_segment_not_valid,dto_payloadp);
//    }
//    else if ((source_page > 1u) || (destination_page > 1u)
//             || (source_page == destination_page))
//    {
//        /* one or both pages invalid */
//        ixcp_status = ixcp_set_error(Ixcp_err_page_not_valid,dto_payloadp);
//    }
//    else if (destination_page == 0u)
//    {
//        /* asking for the RAM page to be copied over the flash page */
//        ixcp_status = ixcp_set_error(Ixcp_err_write_protected,dto_payloadp);
//    }
//    else
//    {
//        cal_status = cal_copy_flash_cal(Cal_slave_ixcp);
//        switch (cal_status)
//        {
//            case Cal_success:
//                /* all done */
//                ixcp_status = Ixcp_dto_res;
//                break;
//            case Cal_pending:
//                ixcp_status = Ixcp_dto_pending;
//                break;
//            case Cal_failure:
//            default:
//                /* something went wrong */
//                ixcp_status = ixcp_set_error(Ixcp_err_generic,dto_payloadp);
//                break;
//        }
//    }
//    return ixcp_status;
}
/**
 * Download variable length of calibration data to RAM
 *
 * # Input
 * CRO Byte       | Meaning     
 * ---------------|--------------
 * cro_datap[1]   | Number of bytes to write (between 1 and 6 bytes)
 * cro_datap[2..] | Data to write
 *
 * # Possible errors
 * + Ixcp_err_out_of_range Number of bytes specified lies outside range 1 to 6
 * + Ixcp_err_access_denied Area to be written does not lie wholly within a
 *              calibration section
 *
 * @retval Ixcp_dto_err one of above errors has been detected and appropriate
 *              error DTO has been generated.
 * @retval Ixcp_dto_pending Data is being written to EEPROM and DTO will be
 *                      sent when it's done
 */
PRIVATE ixcp_dto_type_e ixcp_download(const uint8_t *cro_datap,
                                        uint8_t *dto_payloadp)
{
//    ixcp_dto_type_e dto_type;
//    uint8_t length;
//
//    length = cro_datap[1];
//
//    if ((length >= 1u) && (length <= (CAN_MAX_DATA-2u)))
//    {
//        dto_type = ixcp_do_download(length,&cro_datap[2],dto_payloadp);
//    }
//    else
//    {
//        dto_type = ixcp_set_error(Ixcp_err_access_denied,dto_payloadp);
//    }
//    return dto_type;
}

/**
 * Download fixed length of calibration data to RAM
 *
 * # Input
 * CRO Byte       | Meaning     
 * ---------------|--------------
 * cro_datap[1-7] | 7 bytes of data to write
 *
 * # Possible errors
 * + Ixcp_err_access_denied Area to be written does not lie wholly within a
 *              calibration section
 *
 * @retval Ixcp_dto_err Above error has been detected and appropriate
 *              error DTO has been generated.
 * @retval Ixcp_dto_pending Data is being written to EEPROM and DTO will be
 *                      sent when it's done
 */
PRIVATE ixcp_dto_type_e ixcp_download_max(const uint8_t *cro_datap,
                                        uint8_t *dto_payloadp)
{
//    ixcp_dto_type_e dto_type;
//    dto_type = ixcp_do_download(CAN_MAX_DATA-1u,&cro_datap[1],dto_payloadp);
//    return dto_type;
}

#endif /* ifdef CAL_ENABLED */


/**
 * Reads the given 5-byte address which consists of an address extension and
 * a 32-bit address.
 * If the address extension is non-zero, returns Ixcp_dto_err and writes an
 * 'out-of-range' error into dto_payloadp.  Otherwise sets #ixcp_mta to the
 * requested address: we don't attempt to validate it.
 *
 * @retval Ixcp_dto_err address extension was non-zero
 * @retval Ixcp_dto_res #ixcp_mta has been set
 */
PRIVATE ixcp_dto_type_e ixcp_set_mta_from_cro(const uint8_t *five_byte_address,
                                               uint8_t *dto_payloadp)
{
 //   uint8_t address_extension;
 //   uint32_t address;
 //   ixcp_dto_type_e return_code;
 //
 //   address_extension = five_byte_address[0];
 //
 //   if (address_extension == 0u)
 //   {
 //       /**
 //        * @lint -e{826} suspicious pointer to pointer conversion
 //        *              when reading the four-byte address from bytes 1 to 4
 //        *              of a five byte array
 //        */
 //       /*lint -e{826} */
 //       address = *(uint32_t *) &five_byte_address[1];
 //       ixcp_mta = cosmic_fptr_from_global_address(address);
 //       return_code = Ixcp_dto_res;
 //   }
 //   else
 //   {
 //       /* non-zero address extension is not allowed so build an error
 //        * message to report the problem */
 //       return_code = ixcp_set_error(Ixcp_err_out_of_range,dto_payloadp);
 //   }
 //   return return_code;
}

/**
 * Helper funtion to do the work for both an UPLOAD and SHORT_UPLOAD
 * @param length        Number of bytes to upload
 * @param dto_payloadp  Pointer at which to write the data.
 *
 * If length is > 7 (i.e. CAN_MAX_DATA-1), will write an Ixcp_err_out_of_range
 * error into dto_payloadp.
 *
 * Data is read from the address pointed to by #ixcp_mta.  A side-effect is
 * that ixcp_mta is incrmented by length.
 *
 * @retval Ixcp_dto_err         Requested length was too long
 * @retval Ixcp_dto_res         Data has been written to dto_payloadp
 */
PRIVATE ixcp_dto_type_e ixcp_do_upload(uint8_t length,uint8_t *dto_payloadp)
{
 //   uint8_t i;
 //   ixcp_dto_type_e return_code;
 //
 //   if (length > (CAN_MAX_DATA - 1u))
 //   {
 //       /* asked for more data than will fit in dto_payloadp */
 //       return_code = ixcp_set_error(Ixcp_err_out_of_range,dto_payloadp);
 //   }
 //   else
 //   {
 //       for (i = 0u ; i < length ; i++)
 //       {
 //           dto_payloadp[i] = *ixcp_mta++;
 //       }
 //       return_code = Ixcp_dto_res;
 //   }
 //
 //   return return_code;
}

#ifdef CAL_ENABLED
/**
 * Helper funtion to do the work for both a DOWNLOAD and DOWNLOAD_MAX
 * @param length        Number of bytes to write
 * @param srcp          Pointer to data to write
 * @param dto_payloadp  Pointer for writing an error code if necessary
 *
 * Data is written to the address pointed to by #ixcp_mta.  A side-effect is
 * that ixcp_mta is incrmented by length.
 *
 * If the data cannot be written, because it does not lie wholly within
 * calibration memory, an Ixcp_err_access_denied error is produced.
 *
 * @retval Ixcp_dto_err         Target addresses do not lie wholly within
 *                              calibration memory
 * @retval Ixcp_dto_res         Data has been written to memory
 * @retval Ixcp_dto_pending     Write will be performed by a lower priority
 *                              task so no DTO should be sent yet.
 */
PRIVATE ixcp_dto_type_e ixcp_do_download(uint8_t length,const uint8_t *srcp,
                                        uint8_t *dto_payloadp)
{
 //   cal_status_e cal_status;
 //   ixcp_dto_type_e dto_type;
 //
 //   cal_status = cal_write(Cal_slave_ixcp,ixcp_mta,length,
 //                                       (FPTR const uint8_t *)srcp);
 //
 //   if (cal_status != Cal_failure)
 //   {
 //       ixcp_mta += length;
 //   }
 //
 //   switch (cal_status)
 //   {
 //       case Cal_success:
 //           /* 
 //            * not possible at time of writing because EEPROM writes always
 //            * deferred to the background, but theoretically possible in the
 //            * future
 //            */
 //           dto_type = Ixcp_dto_res;
 //           break;
 //       case Cal_pending:
 //           dto_type = Ixcp_dto_pending;
 //           break;
 //       case Cal_failure:
 //       default:
 //           dto_type = ixcp_set_error(Ixcp_err_access_denied,dto_payloadp);
 //           break;
 //   }
 //   return dto_type;
}
#endif

#ifdef CRC_ENABLED

PRIVATE ixcp_dto_type_e ixcp_build_checksum(const uint8_t *cro_datap)
{
//    ixcp_dto_type_e dto_type;
//
//    /**
//     * @lint --e{826} suspicious pointer to pointer conversion
//     *              when reading the four-byte address from bytes 3 to 6
//     *              of a seven byte array
//     */
//    /*lint -e{826} */
//    ixcp_checksum_length = *(uint32_t *)&cro_datap[4];
//
//    ixcp_checksum_start = ixcp_mta;
//    ixcp_mta += ixcp_checksum_length;
//#ifdef BBLK
//    /* bootblock calculates the checksum in line */
//    ixcp_calculate_checksum();
//#else
//    /* MMC leaves it to the background */
//    ixcp_calculate_checksum_request = TRUE;
//#endif
//    /*
//     * For the MMC, background will send the response when it's ready,
//     * For the BBLK, the response will already have been sent by
//     * ixcp_calculate_checksum(), but setting dto_type to Ixcp_dto_pending
//     * has the deisred effect of preventing the caller from sending a DTO.
//     */
//    dto_type = Ixcp_dto_pending;
//
//    return dto_type;
}

/**
 * Calculate a checksum following an XCP build_checksum request.  On the MMC
 * this is called from a background task.  On a Ricardo bootloader, this is
 * called in-line and so kicks the watchdog during the calculation.
 *
 * All the parameters for the calculation will have been set up by a
 * previous call to ixcp_build_checksum()
 */
PRIVATE void ixcp_calculate_checksum(void)
{
//    uint32_t bytes_remaining;
//    FPTR uint8_t *data_p;
//    uint16_t bytes_this_chunk;
//    uint16_t crc;
//    uint8_t dto_payload[CAN_MAX_DATA-1u];
//    uint8_t i;
//
//    /* dto_payload cannot be static initialised, because that generates an
//     * access to the flash const data segment. */
//    for (i = 0u ; i < (CAN_MAX_DATA-1u); i++)
//    {
//        dto_payload[i] = 0u;
//    }
//
//    bytes_remaining = ixcp_checksum_length;
//    data_p = ixcp_checksum_start;
//    crc = IXCP_CSUM_SEED;
//
//    while (bytes_remaining > 0uL)
//    {
//#ifdef BBLK
//        ram_kick_watchdog();
//#endif
//        bytes_this_chunk = (bytes_remaining > (uint32_t) IXCP_CSUM_CHUNK) ?
//                                IXCP_CSUM_CHUNK : (uint16_t) bytes_remaining;
//        crc = crc16(crc,data_p,bytes_this_chunk);
//
//        bytes_remaining -= bytes_this_chunk;
//        data_p += bytes_this_chunk;
//    }
//
//    dto_payload[0] = IXCP_CSUM_CRC16_CITT;
//    dto_payload[5] = (uint8_t) (crc >> 8u);
//    dto_payload[6] = (uint8_t) crc;
//
//    ixcp_send_dto(Ixcp_dto_res,dto_payload);
}
#endif

/**
 * Our own version of strlen, defined to allow the bootloader to be linked
 * without string library functions
 */
PRIVATE uint16_t ixcp_strlen(const char_t *string)
{
//    uint16_t len = 0u;
//
//    while (*string)
//    {
//        string++;
//        len++;
//    }
//    return len;
}

#ifdef RUNTIME_IDS
PRIVATE void ixcp_strcpy(char_t *dest, const char_t *src)
{
//    while (*src)
//    {
//        *dest++ = *src++;
//    }
}

#ifdef VTBM_IDS
PRIVATE void ixcp_strcat(char_t *dest, const char_t *src)
{
//
//    /* find the end of the destination string */
//    while (*dest)
//    {
//        dest++;
//    }
//    /* now copy the source string onto the end */
//    while (*src)
//    {
//        *dest++ = *src++;
//    }
//    *dest = '\0';       /* and add final NUL */
}
#endif /* ifdef VTBM_IDS */
#endif /* ifdef RUNTIME_IDS */

#ifndef REPROGRAMMING_SLAVE
/**
 * Process a reprogramming request in a slave which cannot itself reprogram
 *
 * If conditions are ok, as given by ixcp_prog_allowed(), then save programming
 * request to emulated EE and reset processor so that bootblock can reprogram.
 *
 * If conditions aren't ok, return an ERR_GENERIC (as required by the XCP
 * standard)
 */
PRIVATE ixcp_dto_type_e ixcp_program_start_app(uint8_t *dto_payloadp)
{
//    ixcp_dto_type_e dto_type = Ixcp_dto_pending;
//    bool_t programming_allowed;
//
//    programming_allowed = ixcp_prog_allowed(Reprogram_xcp);
//
//    if (programming_allowed)
//    {
//        /*
//         * need to reset to let the bootblock's prgramming slave take over
//         */
//#ifdef MMC
//        if (ResetReqAccepted!=ms_request_reset(Reset_program_xcp_internal))
//        {
//            programming_allowed =FALSE;
//        }
//#else
//        flash_eee_record_shutdown_and_wait(Reset_program_xcp_internal);
//        RESET_S12();        /* will not return... */
//#endif
//    }
//
//    if (!programming_allowed)
//    {
//        /* conditions aren't correct for reprogramming so report an error */
//        dto_type = ixcp_set_error(Ixcp_err_generic,dto_payloadp);
//    }
//    return dto_type;
}
#endif
