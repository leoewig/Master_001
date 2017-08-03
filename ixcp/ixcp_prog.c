/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    ixcp_prog_allowed.c
 *
 * description: 
 * XCP programming message routines
 *
 * This file contains the part of the XCP slave used by a bootblock to handle
 * XCP reprogramming commands.
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Temperature conversion module
 *
 *****************************************************************************/
/**
 * XCP programming message routines
 *
 * This file contains the part of the XCP slave used by a bootblock to handle
 * XCP reprogramming commands.
 *
 * @file
 * @ingroup ixcp
 *
 */

#if !defined BBLK
#error This file is for Bootblocks only
#endif

#include "basic_types.h"
#include "basic_types.h"
#include "flash_eee.h"
#include "flash_prog.h"
#include "ixcp.h"
#include "ixcp_p.h"

#ifdef BBLK_MMC
#define KEEP_ALIVE_PORT         PORTA
#define KEEP_ALIVE_DDR          DDRA
#define KEEP_ALIVE_PIN          7u
#define KEEP_ALIVE_MASK         ((uint8_t) (1u << KEEP_ALIVE_PIN))
#endif

/*
 * These routines all live in RAM
 */
#pragma section (ramcode)

extern FPTR uint8_t *ixcp_mta;

PRIVATE bool_t programming_started = FALSE;

/**
 * Respond to a program start request from a slave capable of reprogramming.
 *
 * On the MMC, assert the power hold so that we won't inadvertently switch off
 * if the ignition goes away mid-reprogram.
 *
 * Returns the following:
 * Byte            | Meaning        | Value
 * ----------------|----------------|-------
 * dto_payloadp[0] | Reserved       |
 * dto_payloadp[1] | COMM_MODE_PGM  | 0 (nothing fancy available)
 * dto_payloadp[2] | MAX_CTO_PGM    | 8 (CAN_MAX_DATA)
 * dto_payloadp[3] | MAX_BS_PGM     | 1 
 * dto_payloadp[4] | MIN_ST_PGM     | 0 (doesn't matter)
 * dto_payloadp[5] | QUEUE_SIZE_PGM | 0 (no queue)
 *
 * @retval #Ixcp_dto_res
 */
ixcp_dto_type_e ixcp_program_start_reprog(uint8_t *dto_payload)
{
    dto_payload[2] = CAN_MAX_DATA;
    programming_started = TRUE;
#ifdef BBLK_MMC
    KEEP_ALIVE_DDR |= KEEP_ALIVE_MASK;
    KEEP_ALIVE_PORT |= KEEP_ALIVE_MASK;
#endif

    return Ixcp_dto_res;
}

/**
 * Respond to a PROGRAM_CLEAR request.
 *
 * If something goes wrong, we return the error given by the flash controller
 * in byte 1.  see ixcp_flash_error()
 */
ixcp_dto_type_e ixcp_program_clear(const uint8_t *cro_data,uint8_t *dto_payload)
{
    ixcp_dto_type_e return_code;
    uint8_t flash_errors;
    uint32_t len;

    if (!programming_started)
    {
        return_code = ixcp_sequence_error(dto_payload);
    }
    else if (cro_data[1] != IXCP_PROG_CLEAR_MODE_ABS)
    {
        /* Requesting a functional mode PROGRAM_CLEAR, which isn't supported */
        return_code = ixcp_set_error(Ixcp_err_cmd_syntax,dto_payload);
    }
    else
    {
        /** @lint --e{826} suspicious pointer-to-pointer conversion
         * Lint doesn't like cast from byte to double-word but we specifically
         * arrange .a2l file to allow this
         * See note on lint suppression in ixcp_slave.c
         */
        /*lint --e{826} */
        len = *(const uint32_t *)&cro_data[4];
        flash_errors = flash_clear(ixcp_mta,len);
        if (flash_errors)
        {
            return_code = ixcp_flash_error(flash_errors,dto_payload);
        }
        else
        {
            return_code = Ixcp_dto_res;
        }
    }

    return return_code;
}

/**
 * Process a PROGRAM command
 */
ixcp_dto_type_e ixcp_program(const uint8_t *cro_data,uint8_t *dto_payload)
{
    ixcp_dto_type_e return_code;
    uint8_t len;

    len = cro_data[1];

    return_code = ixcp_do_program(cro_data+2u,dto_payload,len);
    return return_code;
}

/**
 * Process a PROGRAM_MAX command
 */
ixcp_dto_type_e ixcp_program_max(const uint8_t *cro_data,uint8_t *dto_payload)
{
    ixcp_dto_type_e return_code;
    uint8_t len;

    len = CAN_MAX_DATA - 1u;

    return_code = ixcp_do_program(cro_data+1u,dto_payload,len);
    return return_code;
}

/**
 * Process a PROGRAM_RESET command by making sure the flash buffers are
 * flushed, logging the successful reprogramming in emulated EE and
 * then resetting the processor.
 */
ixcp_dto_type_e ixcp_program_reset(uint8_t *dto_payload)
{
    ixcp_dto_type_e return_code;

    if (programming_started)
    {
        /* make sure programming buffer is flushed by sending zero-byte
         * request */
        (void) flash_program(ixcp_mta,0u,NULL);
        flash_eee_ramcode_record_shutdown_and_wait(Reset_program_complete);
        RESET_S12();
        return_code = Ixcp_dto_res;     /* assign for lint's benefit */
    }
    else
    {
        return_code = ixcp_sequence_error(dto_payload);
    }

    return return_code;
}

/**
 * Helper function for PROGRAM and PROGRAM_MAX commands.
 *
 * Errors from the flash controller are returned in byte 1 by ixcp_flash_error()
 */
PRIVATE ixcp_dto_type_e ixcp_do_program(const uint8_t *program_data,
                            uint8_t *dto_payload, uint8_t len)
{
    ixcp_dto_type_e return_code;
    uint8_t flash_errors;

    if (!programming_started)
    {
        return_code = ixcp_sequence_error(dto_payload);
    }
    else
    {
        flash_errors = flash_program(ixcp_mta,len,program_data);
        if (flash_errors)
        {
            return_code = ixcp_flash_error(flash_errors,dto_payload);
        }
        else
        {
            ixcp_mta += len;
            return_code = Ixcp_dto_res;
        }
    }

    return return_code;
}

/**
 * Return reason for a flash programming failure in an XCP error frame.
 *
 * Sadly, CANape ignores this byte so to see it you will have to log the bus
 */
PRIVATE ixcp_dto_type_e ixcp_flash_error(uint8_t flash_errors,
                                                uint8_t *dto_payload)
{
    ixcp_dto_type_e return_code;

    if (flash_errors == FLASH_ADDRESS_ERROR)
    {
        return_code = ixcp_set_error(Ixcp_err_out_of_range,dto_payload);
    }
    else
    {
        return_code = ixcp_set_error(Ixcp_err_generic,dto_payload);
        dto_payload[1] = flash_errors;
    }

    return return_code;
}
