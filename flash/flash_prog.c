/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      flash_prog.c
 *
 * description:    Flash programming routines
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li     Flash programming routines
 *
 *****************************************************************************/
#include "basic_types.h"
#include "cosmic_pointers.h"
#include "flash_prog.h"
#include "flash_prog_p.h"
#include "flash_commands.h"

#ifndef BBLK
#error Flash programmig commands for Bootblock only because they must be executed from RAM
#endif

/*
 * make sure our choice of FLASH_ADDRESS_ERROR doesn't clash with
 * other flash programming errors
 */
#if FLASH_ADDRESS_ERROR & FSTAT_ERROR_MASK
#error FLASH_ADDRESS_ERROR clashes with hardware errors defined in FLASH_ERROR_MASK
#endif

#pragma section (ramcode)

PRIVATE flash_parameters_s pflash_parameters = {
                            FLASH_CMD_PROGRAM_PFLASH, /* program_command */
                            FLASH_CMD_ERASE_PFLASH_SECTOR, /* clear_command */
                            0x700000u,  /* first_address */
                            0x100000u,  /* length */
                            0x7ffc00u,  /* sector_mask */
                               0x400u,  /* sector_size */
                    } ;

PRIVATE flash_parameters_s dflash_parameters = {
                            FLASH_CMD_PROGRAM_DFLASH, /* program_command */
                            FLASH_CMD_ERASE_DFLASH_SECTOR, /* clear_command */
                            0x100000u,  /* first_address */
                             0x30000u,  /* length */
                            0x1fff00u,  /* sector_mask */
                               0x100u,  /* sector_size */
                    } ;

PRIVATE flash_buffer_s flash_buffer = { TRUE,   /* empty */
                                        0u,     /* phrase address */
                                        0u,     /* program_command */
                                        /* data */
                                        {0xffu,0xffu,0xffu,0xffu,
                                            0xffu,0xffu,0xffu,0xffu},
                                        };
uint8_t flash_clear(FPTR const uint8_t *startp, uint32_t len)
{
    uint32_t start_address;
    flash_location_e location;
    uint8_t error_code;

    start_address = cosmic_global_address_from_fptr(startp);

    location = flash_location(start_address,len);

    switch (location)
    {
        case Flash_pflash:
            error_code = flash_clear_multiple_sectors(start_address,len,
                                                        &pflash_parameters);
            break;
        case Flash_dflash:
            error_code = flash_clear_multiple_sectors(start_address,len,
                                                        &dflash_parameters);
            break;
        case Flash_unknown:
        default:
            error_code = FLASH_ADDRESS_ERROR;
            break;
    }
    return error_code;
}

uint8_t flash_program(FPTR const uint8_t *startp, uint8_t len,const uint8_t *data)
{
    uint32_t start_address;
    flash_location_e location;
    uint8_t error_code;

    start_address = cosmic_global_address_from_fptr(startp);

    location = flash_location(start_address,(uint32_t) len);

    switch (location)
    {
        case Flash_pflash:
            error_code = flash_program_multiple(start_address,len,data,
                                        pflash_parameters.program_command);
            break;
        case Flash_dflash:
            error_code = flash_program_multiple(start_address,len,data,
                                        dflash_parameters.program_command);
            break;
        case Flash_unknown:
        default:
            error_code = FLASH_ADDRESS_ERROR;
            break;
    }
    return error_code;
}

PRIVATE flash_location_e flash_location(uint32_t start_address,uint32_t len)
{
    flash_location_e location;

    if ((start_address >= pflash_parameters.start_address)
            && ((start_address + len) <= (pflash_parameters.start_address
                                         + pflash_parameters.length)))
    {
        location = Flash_pflash;
    }
    else if ((start_address >= dflash_parameters.start_address)
            && ((start_address + len) <= (dflash_parameters.start_address
                                         + dflash_parameters.length)))
    {
        location = Flash_dflash;
    }
    else
    {
        location = Flash_unknown;
    }
    return location;
}


PRIVATE uint8_t flash_clear_multiple_sectors(uint32_t start,uint32_t len,
                                  const flash_parameters_s *flash_paramp)
{
    uint32_t sector_address;
    uint8_t flash_errors = 0u;

    for (sector_address = FLASH_SECTOR_START(start,flash_paramp->sector_mask);
            !flash_errors && (sector_address < (start + len)) ; 
            sector_address += flash_paramp->sector_size)
    {
        flash_errors = 
            flash_clear_sector(sector_address,flash_paramp->clear_command);
    }

    return flash_errors;
}


PRIVATE uint8_t flash_clear_sector(uint32_t address,uint16_t clear_command)
{
    FLASH_CMD_PREPARE;
    FLASH_CMD_LOAD_PARAMETER(0u,clear_command
                                | (uint16_t) ((address >> 16u) & 0xffu));
    FLASH_CMD_LOAD_PARAMETER(1u,(uint16_t) address);
    FLASH_CMD_EXECUTE_AND_WAIT;

    return FLASH_CMD_ERRORS;
}

PRIVATE uint8_t flash_program_multiple(uint32_t address,uint8_t len,
                                const uint8_t *data,uint16_t program_command)
{
    uint8_t i;
    uint8_t flash_errors = 0u;

    if (len == 0u)
    {
        /* this is a buffer flush request */
        flash_errors = flash_program_buffer();
    }

    for (i = 0u; !flash_errors && (i < len) ; i++)
    {
        flash_errors = flash_program_byte(address+i,data[i],program_command);
    }

    return flash_errors;
}

PRIVATE uint8_t flash_program_byte(uint32_t address,uint8_t byte,
                                    uint16_t program_command)
{
    uint8_t errors = 0u;
    uint8_t buffer_offset;

    if (!flash_buffer.empty && !FLASH_ADDRESS_IN_BUFFER_PHRASE(address))
    {
        errors = flash_program_buffer();
    }

    if (flash_buffer.empty)
    {
        flash_program_buffer_initialise(address,program_command);
    }

    buffer_offset = (uint8_t) (address - flash_buffer.phrase_address);
    flash_buffer.data[buffer_offset] = byte;

    return errors;
}

PRIVATE uint8_t flash_program_buffer(void)   
{
    uint8_t errors = 0u;
    uint16_t word;
    uint8_t i;
    uint8_t word_pos;

    if (!flash_buffer.empty)
    {
        FLASH_CMD_PREPARE;
        FLASH_CMD_LOAD_PARAMETER(0u,flash_buffer.program_command
                | (uint16_t) ((flash_buffer.phrase_address >> 16u) & 0xffu));
        FLASH_CMD_LOAD_PARAMETER(1u,(uint16_t) flash_buffer.phrase_address);
        /* there is data in the buffer, so program it */
        for (i = 0u ; i < FLASH_PHRASE_WORD_LENGTH ; i++)
        {
            word_pos = (uint8_t) (i * 2u);
            word = ((uint16_t) flash_buffer.data[word_pos] << 8u)
                        | flash_buffer.data[word_pos+1u];
            FLASH_CMD_LOAD_PARAMETER(i+2u,word);
        }
        FLASH_CMD_EXECUTE_AND_WAIT;
        errors = FLASH_CMD_ERRORS;
        flash_buffer.empty = TRUE;
    }

    return errors;
}

PRIVATE void flash_program_buffer_initialise(uint32_t address,
                                                uint16_t program_command)
{
    uint8_t i;

    flash_buffer.empty = FALSE;
    flash_buffer.phrase_address = FLASH_PHRASE_ADDRESS(address);
    flash_buffer.program_command = program_command;
    for (i = 0u ; i < FLASH_PHRASE_BYTE_LENGTH ; i++)
    {
        flash_buffer.data[i] = 0xffu;
    }
}
