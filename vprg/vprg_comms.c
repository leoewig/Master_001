/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     vprg_comms.c
 *
 * description:  
 * VTBM reprogramming erase/program routines
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/


#include "basic_types.h"
#include "hardware.h"
#include "ixcp.h"
#include "ts.h"
//#include "pl.h"
//#include "ican.h"
#include "crc.h"
#include "vprg.h"
#include "vprg_p.h"


/******************************************************************************
 * COMPILE-TIME CHECKS
 *****************************************************************************/

/*
 * PUBLIC DATA (documented in header)
 */

/**
 * Error detected while transmitting an image to a VTBM
 * @a2l{conversion,vprg_comms_error_e}
 */
vprg_comms_error_e vprg_comms_error = Vprg_comms_error_none;


/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/
#define MEMORY_LOCATION S12_MEMORY
#include "memory_layout.h"

/**
 * Maximum number of times to send a command request to a VTBM before giving up.
 * This is only used for commands which, if received, won't upset the MTA.
 * (CONNECT has its own value #vprg_connect_attempts)
 */
PRIVATE CAL uint8_t vprg_cro_attempts = 3u;
/**
 * Maximum time to wait for a response to a command request
 * (CONNECT has its own value #vprg_connect_timeout_ms)
 * @a2l{conversion,ts_time_t}
 */
PRIVATE CAL ts_time_t vprg_dto_timeout_ms = TIME(500);

/**
 * Maximum number of times to send a CONNECT request to a VTBM before giving up.
 * CONNECT has its own values because we want to connect to the bootloader after
 * power-cycling the VTBM but the VTBM power supply is a bit ponderous.
 * We will start connecting after #vprg_cycle_on_delay_ms and make an attempt
 * every #vprg_connect_timeout_ms until we have made this number of attempts.
 * It is essential that we hit the bootloader 50ms re-programming window
 * somewhere.
 */
PRIVATE CAL uint8_t vprg_connect_attempts = 30u;
/**
 * Maximum time to wait for a response to a CONNECT request before trying again.
 * This number nmust be smaller than the bootloader re-prgramming window to
 * ensure we connect to the bootloader XCP slave rather than the application.
 * @a2l{conversion,ts_time_t}
 */
PRIVATE CAL ts_time_t vprg_connect_timeout_ms = TIME(20);

/**
 * Maximum number of times to try to reprogram a sector
 */
PRIVATE CAL uint8_t vprg_prog_attempts = 5u;


/**
 * Maximum time to wait for a checksum to be calculated
 * (doesn't need to be too long because we only crunch 1024 bytes at a time)
 * @a2l{conversion,ts_time_t}
 */
PRIVATE CAL ts_time_t vprg_csum_timeout_ms = TIME(1000);

/**
 * XCP message to create an XCP connection
 */
PRIVATE FROM uint8_t vprg_connect_message[CAN_MAX_DATA] = { 
                    (uint8_t) Ixcp_cc_connect,0u,0u,0u, 0u,0u,0u,0u } ;
/**
 * XCP message to switch the slave into programming mode
 */
PRIVATE FROM uint8_t vprg_program_start_message[CAN_MAX_DATA] = { 
                    (uint8_t) Ixcp_cc_program_start,0u,0u,0u, 0u,0u,0u,0u } ;
/**
 * XCP message sent after reprogramming to force a reset on the VTBM
 */
PRIVATE FROM uint8_t vprg_program_reset_message[CAN_MAX_DATA] = { 
                    (uint8_t) Ixcp_cc_program_reset,0u,0u,0u, 0u,0u,0u,0u } ;

/**
 * XCP message to set the address for subsequent commands.
 * The address is written into the last four bytes of the message before sending
 */
PRIVATE FRAM uint8_t vprg_set_mta_message[CAN_MAX_DATA] = {
                    (uint8_t) Ixcp_cc_set_mta,0u,0u,0u, 0u,0u,0u,0u } ;
/**
 * XCP message to erase flash at the previously set address.
 * The number of bytes to erase is written into the message before sending.
 * The actual area erased will correspond to one or more entire flash sectors.
 */
PRIVATE FRAM uint8_t vprg_erase_message[CAN_MAX_DATA] = {
                    (uint8_t) Ixcp_cc_program_clear,0u,0u,0u, 0u,0u,0u,0u } ;
/**
 * XCP message to program 7 bytes starting at the previously sent address.
 * The 7 bytes are written into the message before sending.
 */
PRIVATE FRAM uint8_t vprg_program_max_message[CAN_MAX_DATA] = {
                    (uint8_t) Ixcp_cc_program_max,0u,0u,0u, 0u,0u,0u,0u } ;
/**
 * XCP message to program 0-6 bytes starting at the previously sent address.
 * The number of bytes and the data are written into the message before sending.
 * A length of 0 bytes tells the slave to write any unwritten data to flash.
 */
PRIVATE FRAM uint8_t vprg_program_message[CAN_MAX_DATA] = {
                    (uint8_t) Ixcp_cc_program,0u,0u,0u, 0u,0u,0u,0u } ;

/**
 * XCP message to build a checksum starting at the previously sent address
 * and wit the length written to the last 4 bytes
 */
PRIVATE FRAM uint8_t vprg_build_checksum_message[CAN_MAX_DATA] = {
                    (uint8_t) Ixcp_cc_build_checksum,0u,0u,0u, 0u,0u,0u,0u } ;
/**
 * index into the image folder of the image to be programmed
 */
PRIVATE image_index_t image_to_program = 0u;
/**
 * mask representing the VTBMs to be programmed. e.g. a mask of 0x0001 means
 * program VTBM 0.  A mask of 0x0fff means program VTBMs 0 to 11.
 */
PRIVATE vprg_vtbm_mask_t vprg_vtbms_to_program_mask = 0u;
/**
 * mask of VTBMs whch have been programmed in this operation.  Once it is equal
 * to #vprg_vtbms_to_program_mask, then reprogramming has finished.
 */
PRIVATE vprg_vtbm_mask_t vprg_vtbms_programmed_mask = 0u;
/**
 * State of the state machine contained in vprg_comms_idle_task()
 * @a2l{conversion,vprg_comms_state_e}
 */
PRIVATE vprg_comms_state_e vprg_comms_state = Vprg_comms_state_none;

/**
 * ID of the VTBM currently being programmed
 */
PRIVATE uint8_t vprg_vtbm_id = 0u;
/**
 * CAN ID of the DTO from the VTBM currently being programmed
 */
PRIVATE uint16_t vprg_dto_id = 0u;
/**
 * CAN ID of the CRO to the VTBM currently being programmed
 */
PRIVATE uint16_t vprg_cro_id = 0u;

/**
 * buffer into which the contents of a recieved DTO is copied
 */
PRIVATE uint8_t vprg_dto_data[CAN_MAX_DATA];

/**
 * Flag set if we encounter an old VTBM bootblock which can't calculate a CRC
 * @lint -esym(551,vprg_vtbm_bblk_has_no_crc) symbol not accessed
 * @todo have a latching DTC set when this flag sets and cleared when
 * this flag is clear after programming.
 */
/*lint -esym(551,vprg_vtbm_bblk_has_no_crc) */
PRIVATE FRAM bool_t vprg_vtbm_bblk_has_no_crc = FALSE;
/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/**
 * State machine to perform communication to a VTBM in the MMC idle loop.
 *
 * The state machine operates continuously until communication finishes.
 */
void vprg_comms_idle_task(void)
{
    bool_t finished = FALSE;

    while (!finished)
    {
        switch (vprg_comms_state)
        {
            case Vprg_comms_state_none:
            case Vprg_comms_state_complete:
            case Vprg_comms_state_error:
                /* nothing to do */
                finished = TRUE;
                break;
            case Vprg_comms_state_connect:
                vprg_comms_state = vprg_comms_connect();
                break;
            case Vprg_comms_state_program_start:
                vprg_comms_state = vprg_comms_program_start();
                break;
            case Vprg_comms_state_erase:
                vprg_comms_state = vprg_comms_erase();
                break;
            case Vprg_comms_state_program:
                vprg_comms_state = vprg_comms_program();
                break;
            case Vprg_comms_state_program_reset:
                vprg_comms_state = vprg_comms_program_reset();
                break;
            default:
                VPRG_INTERNAL_ERROR();
                break;
        }
    }
}

vprg_comms_state_e vprg_comms_status(void)
{
    return vprg_comms_state;
}

/**
 * Record the image to program and the vtbm mask so that they are available
 * when vprg_comms_idle_task() is next called
 */
void vprg_comms_set_demand(image_index_t image_index,vprg_vtbm_mask_t vtbm_mask)
{
    image_to_program = image_index;
    vprg_vtbms_to_program_mask = vtbm_mask;
    vprg_vtbms_programmed_mask = (vprg_vtbm_mask_t) 0u;
}

/**
 * Kick-off the state machine
 */
vprg_state_e vprg_comms_request(void)
{
    vprg_comms_state = Vprg_comms_state_connect;
    vprg_comms_error = Vprg_comms_error_none;
    vprg_vtbm_bblk_has_no_crc = FALSE;  /* reset flag */
    return Vprg_state_communicating;
}

/**
 * Report whether all required VTBMs have been reprogrammed by
 * comparing #vprg_vtbms_programmed_mask with #vprg_vtbms_to_program_mask
 */
bool_t vprg_comms_all_vtbms_programmed(void)
{
    bool_t all_programmed;

    all_programmed = (vprg_vtbms_to_program_mask == vprg_vtbms_programmed_mask);
    return all_programmed;
}

/**
 * Turn a VTBM ID into a bitmask. e.g. VTBM 0 -> mask 0x0001, VTBM 1 -> 0x0002
 */
vprg_vtbm_mask_t vprg_vtbm_id_to_mask(uint8_t i)
{
    return (vprg_vtbm_mask_t) 1u << i;
}

/**
 * Create an XCP connection to the next VTBM to be reporogrammed
 */
PRIVATE vprg_comms_state_e vprg_comms_connect(void)
{
    vprg_comms_state_e next_state;
    bool_t ok;

    ok = vprg_calc_next_vtbm_id();

    if (ok)
    {
        ok = (vprg_send_connect() == Vprg_comms_error_none);
    }

    if (ok)
    {
        next_state = Vprg_comms_state_program_start;
    }
    else
    {
        next_state = Vprg_comms_state_error;
    }

    return next_state;
}

/**
 * Send a PROGRAM_START message and cehck the response
 */
PRIVATE vprg_comms_state_e vprg_comms_program_start(void)
{
    vprg_comms_state_e next_state;
    bool_t ok;

    ok = (vprg_send_program_start() == Vprg_comms_error_none);

    if (ok)
    {
        next_state = Vprg_comms_state_erase;
    }
    else
    {
        next_state = Vprg_comms_state_error;
    }

    return next_state;
}
    
/**
 * Send an Erase message for each sector in the image
 */
PRIVATE vprg_comms_state_e vprg_comms_erase(void)
{
    FPTR image_header_s *image_p;
    FPTR image_sector_header_s *sector_p;
    uint16_t nsectors;
    bool_t ok;
    vprg_comms_state_e new_state;

    image_p = vprg_get_image_header(image_to_program);
    nsectors = image_p->number_of_sectors;
    sector_p = vprg_first_sector(image_p);

    ok = TRUE;
    do {
        ok = (vprg_erase_sector(sector_p) && ok);
        sector_p = vprg_next_sector(sector_p);
    } while (--nsectors);

    if (ok)
    {
        /* all sectors erased correctly */
        new_state = Vprg_comms_state_program; 
    }
    else
    {
        /* one or more failed to erase */
        new_state = Vprg_comms_state_error;
    }

    return new_state;
}

/**
 * Program the bytes in every sector in the image
 */
PRIVATE vprg_comms_state_e vprg_comms_program(void)
{
    FPTR image_header_s *image_p;
    FPTR image_sector_header_s *sector_p;
    uint16_t nsectors;
    bool_t ok;
    vprg_comms_state_e new_state;

    image_p = vprg_get_image_header(image_to_program);
    nsectors = image_p->number_of_sectors;
    sector_p = vprg_first_sector(image_p);

    do {
        ok = vprg_program_and_check_sector(sector_p);
        sector_p = vprg_next_sector(sector_p);
    } while (--nsectors && ok);

    if (ok)
    {
        vprg_vtbms_programmed_mask |= vprg_vtbm_id_to_mask(vprg_vtbm_id);
        new_state = Vprg_comms_state_program_reset; 
    }
    else
    {
        new_state = Vprg_comms_state_error;
    }

    return new_state;
}
    
/**
 * Reset a VTBM after it has been reprogrammed
 */
PRIVATE vprg_comms_state_e vprg_comms_program_reset(void)
{
    vprg_send_program_reset();

    return Vprg_comms_state_complete;
}

/**
 * Calculate the ID of the next VTBM to be reprogrammed, stores it 
 * in #vprg_vtbm_id and sets #vprg_cro_id and #vprg_dto_id to the correct
 * CAN IDs for the VTBM.
 * Works by finding the lowest bit in #vprg_vtbms_to_program_mask for which 
 * the corresponding bit in #vprg_vtbms_programmed_mask is clear
 */
PRIVATE bool_t vprg_calc_next_vtbm_id(void)
{
    bool_t found_next_in_mask = FALSE;
    vprg_vtbm_mask_t unprogrammed_mask;
    uint8_t i;
    vprg_vtbm_mask_t i_mask;

    unprogrammed_mask = vprg_vtbms_to_program_mask 
                            & (vprg_vtbm_mask_t) ~vprg_vtbms_programmed_mask;

    //for (i = 0u ; (i < pl_num_vtbms) && !found_next_in_mask ; i++)
		for (i = 0u ; (i < 8) && !found_next_in_mask ; i++)
    {
        i_mask = vprg_vtbm_id_to_mask(i);
        if (i_mask & unprogrammed_mask)
        {
            /* i is in the list of VTBMs still to program,
             * so it's our next one */
            vprg_vtbm_id = i;
            found_next_in_mask = TRUE;
        }
    }

    if (!found_next_in_mask)
    {
        /* there is nothing to program  - how did that happen? */
        vprg_comms_error = Vprg_comms_error_mask_internal;
    }
    vprg_cro_id = IXCP_CRO_ID_BASE + (uint16_t) vprg_vtbm_id;
    vprg_dto_id = IXCP_DTO_ID_BASE + (uint16_t) vprg_vtbm_id;

    return found_next_in_mask;
}

/**
 * Erase a single sector on the VTBM
 */
PRIVATE bool_t vprg_erase_sector(FPTR const image_sector_header_s *sector_p)
{
    uint32_t sector_address;
    uint32_t sector_length;
    bool_t ok;

    sector_address = sector_p->address;
    sector_length = sector_p->number_of_bytes;

    ok = (vprg_send_erase(sector_address,sector_length)
                 == Vprg_comms_error_none);
    return ok;
}

/**
 * Program a sector and verify its CRC.  If programming or the CRC check fails,
 * erase the sector and try again up to a limit of #vprg_prog_attempts times
 * @note the sector must have been erased prior to entry to this function.
 */
PRIVATE bool_t vprg_program_and_check_sector(
                                FPTR const image_sector_header_s *sector_p)
{
    uint8_t attempts;
    vprg_comms_error_e program_res;
    vprg_comms_error_e saved_error;
    bool_t program_ok;
    bool_t erase_ok;
    bool_t try_again;

    attempts = 0u;
    do
    {
        program_res = vprg_program_sector(sector_p);
        if (program_res == Vprg_comms_error_none)
        {
            program_ok = vprg_check_sector_programming(sector_p);
        }
        else
        { 
            program_ok = FALSE;
        }
        attempts++;
        if (program_ok)
        {
            /* programming was successful */
            try_again = FALSE;
        }
        else
        {
            /* 
             * either programming or the CRC check failed so erase the sector
             * again, taking care to preserve the error that caused the problem
             */
            saved_error = vprg_comms_error;
            erase_ok = vprg_erase_sector(sector_p);
            vprg_comms_error = saved_error;
            /* now decide whether it's worth trying again */
            try_again = (erase_ok && (attempts < vprg_prog_attempts));
        }
    } while (try_again);

    return program_ok;
}

/**
 * Program a single sector on the VTBM
 * @lint -e{662} Possible creation of out-of-bounds pointer.
 * The length of data varies from sector to sector so it is specified by the
 * number_of_bytes field, however the data array is defined
 * in #image_sector_header_s as containing only a single byte.  Lint notices
 * that we access the data array beyond the first byte so throws up a warning.
 * The convenience of getting the compiler to compute byte offsets for us
 * outweighs the cost of having to add lint suppression around accesses to the
 * data field.  This suppression is required both in this function and in
 * sub-functions called from here.  A manual inspection of this function shows
 * that nothing beyond sector_p->data[number_of_bytes-1] will be accessed.
 */

PRIVATE vprg_comms_error_e vprg_program_sector(
                                FPTR const image_sector_header_s *sector_p)
{
    uint32_t address;
    uint16_t bytes_left;
    FPTR const uint8_t *data;
    vprg_comms_error_e res;

    address = sector_p->address;
    bytes_left = sector_p->number_of_bytes;
    data = &sector_p->data[0];

    res = Vprg_comms_error_none;
    while ((bytes_left >= VPRG_PROGRAM_MAX_BYTES) 
            && (res == Vprg_comms_error_none))
    {
        /* enough bytes for a program_max */
        res = vprg_send_program_max(address,data);
        address += VPRG_PROGRAM_MAX_BYTES;
        /*lint -e{662} see note above */
        data += VPRG_PROGRAM_MAX_BYTES;
        bytes_left -= VPRG_PROGRAM_MAX_BYTES;
    }
    if (bytes_left && (res == Vprg_comms_error_none))
    {
        /*lint -e{662} see note above */
        res = vprg_send_program(address,data, (uint8_t) bytes_left);
    }
    /* finally send a request for zero bytes to flush the programming buffer
     * on the VTBM */
    if (res == Vprg_comms_error_none)
    {
       res = vprg_send_program(address+bytes_left,data,0u);
    }
    return res;
}

/**
 * Calculate the CRC for the data in the sector and ask the VTBM to do the
 * same, then compare the results
 * @retval TRUE CRCs match so VTBM has correctly programmed data
 * @retval FALSE CRCs differ
 * @note old VTBM bootblocks don't support checksum calculation so if the
 * BUILD_CHECKSUM command returns Command Unknown then we skip the CRC check
 * but set a flag indicating that the CRC could not be verified. 
 */
PRIVATE bool_t vprg_check_sector_programming(
                                FPTR const image_sector_header_s *sector_p)
{
    uint16_t expected_crc;
    uint16_t received_crc;
    uint16_t length;
    vprg_comms_error_e crc_res;
    bool_t ok;

    length = sector_p->number_of_bytes;

    expected_crc = crc16(IXCP_CSUM_SEED,sector_p->data,length);
    crc_res = vprg_send_build_checksum(sector_p->address,(uint32_t) length,
                                        &received_crc);

    if (crc_res == Vprg_comms_error_xcp_cmd_unknown)
    {
        /* 
         * it's an old VTBM bootloader which can't calculate CRCs:
         * assume it's ok but set a global flag - production modules will
         * use an up-to-date bootblock so stop worrying.
         */
        ok = TRUE;
        vprg_vtbm_bblk_has_no_crc = TRUE;
        /* clear the error set by vprg_cro_tx_dto_rx() */
        vprg_comms_error = Vprg_comms_error_none;
    }
    else if (crc_res == Vprg_comms_error_none)
    {
        ok = (expected_crc == received_crc);
        if (!ok)
        {
            vprg_comms_error = Vprg_comms_error_crc_mismatch;
        }
    }
    else
    {
        /* something went wrong with the CRC calculation */
        ok = FALSE;
    }
    return ok;
}

/**
 * Send an XCP connect message
 */
PRIVATE vprg_comms_error_e vprg_send_connect(void)
{
    return vprg_cro_tx_dto_rx_with_retries(vprg_connect_message,
                            vprg_connect_timeout_ms,vprg_connect_attempts);
}

/**
 * Send an XCP SET_MTA and ERASE messages
 */
PRIVATE vprg_comms_error_e vprg_send_erase(uint32_t address,uint32_t length)
{
    vprg_comms_error_e res;

    vprg_erase_message[4] = (uint8_t) (length >> 24u);
    vprg_erase_message[5] = (uint8_t) (length >> 16);
    vprg_erase_message[6] = (uint8_t) (length >> 8);
    vprg_erase_message[7] = (uint8_t) length;

    res = vprg_send_set_mta(address);

    if (res == Vprg_comms_error_none)
    {
        /* erase doesn't affect the mta, so might as well try with retries */
        res = vprg_cro_tx_dto_rx_with_retries(vprg_erase_message,
                                    vprg_dto_timeout_ms,vprg_cro_attempts);
    }
    return res;
}

/**
 * Send an XCP PROGRAM_START message
 */
PRIVATE vprg_comms_error_e vprg_send_program_start(void)
{
    return vprg_cro_tx_dto_rx_with_retries(vprg_program_start_message,
                vprg_dto_timeout_ms,vprg_cro_attempts);
}

/**
 * Send an XCP PROGRAM message
 */
PRIVATE vprg_comms_error_e vprg_send_program(uint32_t address,
                                    FPTR const uint8_t *data, uint8_t nbytes)
{
    vprg_comms_error_e res;
    uint8_t i ;

    if (nbytes >= VPRG_PROGRAM_MAX_BYTES)
    {
        VPRG_INTERNAL_ERROR();
    }
    vprg_program_message[1] = nbytes;

    for (i = 0u ; i < nbytes ; i++)
    {
        /* @lint -e{662} see note in vprg_program_sector() */
        /*lint -e{662} */
        vprg_program_message[i+2u] = data[i];
    }
    for ( ; i < (CAN_MAX_DATA - 2u) ; i++)
    {
        /* @lint -e{662} see note in vprg_program_sector() */
        /*lint -e{662} */
        vprg_program_message[i+2u] = 0u;
    }
    res = vprg_send_set_mta(address);
    if (res == Vprg_comms_error_none)
    {
        res = vprg_cro_tx_dto_rx(vprg_program_message,vprg_dto_timeout_ms);
    }
    return res;
}

/**
 * Send an XCP PROGRAM_MAX message
 */
PRIVATE vprg_comms_error_e vprg_send_program_max(uint32_t address,
                                        FPTR const uint8_t *data)
{
    vprg_comms_error_e res;
    uint8_t i ;

    for (i = 0u ; i < VPRG_PROGRAM_MAX_BYTES ; i++)
    {
        /* @lint -e{662} see note in vprg_program_sector() */
        /*lint -e{662} */
        vprg_program_max_message[i+1u] = data[i];
    }
    res = vprg_send_set_mta(address);
    if (res == Vprg_comms_error_none)
    {
        res = vprg_cro_tx_dto_rx(vprg_program_max_message,vprg_dto_timeout_ms);
    }
    return res;
}

/**
 * send a SET_MTA and build_checksum message
 */
PRIVATE vprg_comms_error_e vprg_send_build_checksum(uint32_t address,
                                    uint32_t length,uint16_t *crc_p)
{
    vprg_comms_error_e res;

    res = vprg_send_set_mta(address);

    vprg_build_checksum_message[4] = (uint8_t) (length >> 24u);
    vprg_build_checksum_message[5] = (uint8_t) (length >> 16);
    vprg_build_checksum_message[6] = (uint8_t) (length >> 8);
    vprg_build_checksum_message[7] = (uint8_t) length;

    if (res == Vprg_comms_error_none)
    {
        /* BUILD_CHECKSUM modifies MTA so we can't use retries */
        res = vprg_cro_tx_dto_rx(vprg_build_checksum_message,
                                        vprg_csum_timeout_ms);
    }

    if (res == Vprg_comms_error_none)
    {
        *crc_p = ((uint16_t) vprg_dto_data[6]*256u) + vprg_dto_data[7];
    }
    return res;
}
/**
 * Send an XCP SET MTA message
 */
PRIVATE vprg_comms_error_e vprg_send_set_mta(uint32_t address)
{
    vprg_set_mta_message[4] = (uint8_t) (address >> 24u);
    vprg_set_mta_message[5] = (uint8_t) (address >> 16);
    vprg_set_mta_message[6] = (uint8_t) (address >> 8);
    vprg_set_mta_message[7] = (uint8_t) address;

    return vprg_cro_tx_dto_rx_with_retries(vprg_set_mta_message,
                                vprg_dto_timeout_ms,vprg_cro_attempts);
}

/**
 * Send an XCP PROGRAM_RESET message and don't bother to wait for a response
 */
PRIVATE void vprg_send_program_reset(void)
{
    //(void) icantx(vprg_cro_id,Can_internal,CAN_MAX_DATA,
    //                    vprg_program_reset_message);
    //return;
}

/**
 * Attempt up to max_attempts times to get a response to a CRO message.
 * Other arguments and return values as for vprg_cro_tx_dto_rx()
 */ 
PRIVATE vprg_comms_error_e vprg_cro_tx_dto_rx_with_retries(
                                        FPTR const uint8_t *tx_data,
                                        ts_time_t timeout_ms,
                                        uint8_t max_attempts)
{
    vprg_comms_error_e result;
    uint8_t i;

    i = 0u;
    do {
        result = vprg_cro_tx_dto_rx(tx_data,timeout_ms);
        i++;
    } while ((result != Vprg_comms_error_none) && (i < max_attempts));

    return result;
}

/**
 * Send an XCP message and wait for the response.
 * @param[in] tx_data   points to #CAN_MAX_DATA bytes of data
 * @param timeout_ms    Maximum time to wait for a response
 * @retval Vprg_comms_error_none        reply received
 * @retval Vprg_comms_error_xcp_timeout No reply received within the given
 *      timeout
 * @retval Vprg_comms_error_xcp_unknown_cmd     Slave reported an unknown
 *      command error (differentiated from Vprg_comms_error_xcp_err so that
 *      we can handle special case of asking an old bblk for a checksum)
 * @retval Vprg_comms_error_xcp_err     Slave reported a different XCP error
 *
 * @note this function flushes the input buffer before sending to make sure
 * an old DTO reply isn't confused with a new CRO
 */
PRIVATE vprg_comms_error_e vprg_cro_tx_dto_rx(FPTR const uint8_t *tx_data,
                                        ts_time_t timeout_ms)
{
    bool_t reply_received = FALSE;
    bool_t timeout;
    vprg_comms_error_e result;
    ts_time_t send_time_ms;

    /* Flush receive buffer before sending CRO */
    (void) vprg_poll_for_dto();

    //(void) icantx(vprg_cro_id,Can_internal,CAN_MAX_DATA,tx_data);
    send_time_ms = ts_get_ms();

    do {
        TEST_HOOK(0);
        reply_received = vprg_poll_for_dto();
        timeout = ts_is_elapsed_ms(ts_get_ms(),send_time_ms,timeout_ms);
    } while (!reply_received && !timeout);

    if (!reply_received)
    {
        result = Vprg_comms_error_xcp_timeout;
    }
    else if (vprg_dto_data[0] == (uint8_t) Ixcp_dto_res)
    {
        /* command got a positive response */
        result = Vprg_comms_error_none;
    }
    else if ((vprg_dto_data[0] == (uint8_t) Ixcp_dto_err)
             && (vprg_dto_data[1] == (uint8_t) Ixcp_err_cmd_unknown))
    {
        result = Vprg_comms_error_xcp_cmd_unknown;
    }
    else
    {
        /* some other error with the response */
        result = Vprg_comms_error_xcp_err;
    }
    vprg_comms_error = result;
    return result;
}

/**
 * Goes through the internal bus CAN buffer looking for a VTBM DTO and
 * discarding everything else.
 * Copies the data from the most recently received DTO into #vprg_dto_data
 * @retval      TRUE    a DTO (with id #vprg_dto_id) has been recieved
 * @retval      FALSE   a DTO has not been received
 */
PRIVATE bool_t vprg_poll_for_dto(void)
{
  //  struct ican_rxtxq_s *rxqp = &ican_s12_rxq[Can_internal];
  //  struct ican_msg_s *msgp;
  //  bool_t message_received = FALSE;
  //  uint8_t i;
  //
  //  while (!QUEUE_EMPTY(rxqp))
  //  {
  //      msgp = OLDEST_MESSAGE(rxqp);
  //
  //      if ((msgp->id == vprg_dto_id) && (msgp->dlc >= 1u))
  //      {
  //          message_received = TRUE;
  //          for (i = 0u ; i < msgp->dlc ; i++)
  //          {
  //              vprg_dto_data[i] = msgp->data[i];
  //          }
  //          for ( ; i < CAN_MAX_DATA ; i++)
  //          {
  //              vprg_dto_data[i] = 0u;
  //          }
  //      }
  //      REMOVE_MESSAGE(rxqp);
  //  }
  //  return message_received;
}
