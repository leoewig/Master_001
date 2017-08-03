/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      flash_eee_bblk.c
 *
 * description:   Bootblock source for Flash commands
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li    Bootblock source for Flash commands 
 *                       
 *
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#include "flash_commands.h"
#include "flash_eee.h"
#include "flash_bblk.h"
#include "flash_bblk_p.h"

#define DFPART_RESET_VALUE    0xffffu

/*
 * The D-flash uses all spare ROM to maximise the life of emulated EEPROM,
 * therefore we should just configure the space we need for user D-flash
 * and actual EEPROM.
 */
#define DFPART_MIN_VALUE      0x4u    /**< Minimum size (in 256 byte sectors)
                                       of the User D-flash partition.
                                       We need one page for EOL and 3 for
                                       sensor cal on an MMC.  VTBM doesn't need
                                       any User D-flash but we use the same 
                                       values so that EVBs can be programmed
                                       with either */
#define ERPART_MIN_VALUE      0x1u   /**< Just one 256 byte sector of
                                       Emulated EEPROM. */

PRIVATE uint16_t dfpart;
PRIVATE uint16_t erpart;

/**
 * @lint -esym(551,ecount,dead_and_readycount) debug variables not accessed
 */
/*lint -esym(551,ecount,dead_and_readycount) */
PRIVATE uint16_t ecount;
PRIVATE uint16_t dead_and_readycount;

void flash_eee_initialise(flash_eee_reprogramming_e reprogramming_options)
{
    bool_t eee_is_partitioned;
    bool_t eee_is_available;
    bool_t eee_error_during_reset;

    /*
     * if there is a problem with the D-flash partition, it will show up as
     * an error bit in FSTAT following reset.
     * We need to look for it before we query the eee because that will
     * clear the ACCERR bit
     */
    eee_error_during_reset = (FSTAT & FSTAT_ACCERR) ? TRUE : FALSE;

    flash_eee_query();

    if (eee_error_during_reset)
    {
        eee_is_partitioned = FALSE;
    }
    else if (dfpart == DFPART_RESET_VALUE)
    {
        /* brand new processor so partition it */
        eee_is_partitioned = flash_eee_partition();
    }
    else if ((dfpart < DFPART_MIN_VALUE) || (erpart < ERPART_MIN_VALUE))
    {
        /*
         * It's already got an unsuitable partition: can't do anything about it
         * now because repartitioning requires a BDM cable.
         */
        eee_is_partitioned = FALSE;
    }
    else
    {
        eee_is_partitioned = TRUE;
    }

    if (eee_is_partitioned)
    {
        eee_is_available = flash_eee_enable();
    }
    else
    {
        eee_is_available = FALSE;
    }

    /*
     * Note that if we haven't enabled eee, bootblock_eee will behave like
     * regular RAM
     */
    bootblock_eee.flash_eee_available = (uint16_t) ((eee_is_available) 
        ? Flash_eee_available : Flash_eee_unavailable) ;

    /*
     * If the reprogramming options have changed, reset FBL state
     */
    if (bootblock_eee.reprogramming_options != reprogramming_options)
    {
        bootblock_eee.block_validity_flags = 0xffu; /* 0xff -> all invalid */
        bootblock_eee.security_access_delay_flags = 0xffu;
        bootblock_eee.security_access_invalid_count = 0xffu;
        bootblock_eee.security_access_eng_flags = 0xffu;
    }

    /*
     * Store the ways that this bootloader knows how to reprogram flash.
     * will be Reprogram_iso for the fbl and Reprogram_xcp for the bblk
     */
    bootblock_eee.reprogramming_options = reprogramming_options;
}

PRIVATE void flash_eee_query(void)
{
    FLASH_CMD_PREPARE;
    FLASH_CMD_LOAD_PARAMETER(0u,FLASH_CMD_EEE_QUERY);
    FLASH_CMD_EXECUTE_AND_WAIT;
    dfpart = FLASH_CMD_READ_PARAMETER(1u);
    erpart = FLASH_CMD_READ_PARAMETER(2u);
    ecount = FLASH_CMD_READ_PARAMETER(3u);
    dead_and_readycount = FLASH_CMD_READ_PARAMETER(4u);
}

PRIVATE bool_t flash_eee_partition(void)
{
    bool_t no_errors;
    FLASH_CMD_PREPARE;
    FLASH_CMD_LOAD_PARAMETER(0u,FLASH_CMD_PARTITION_DFLASH);
    FLASH_CMD_LOAD_PARAMETER(1u,DFPART_MIN_VALUE);
    FLASH_CMD_LOAD_PARAMETER(2u,ERPART_MIN_VALUE);
    FLASH_CMD_EXECUTE_AND_WAIT;
    no_errors = !FLASH_CMD_ERRORS;

    return no_errors;
}

PRIVATE bool_t flash_eee_enable(void)
{
    bool_t no_errors;
    FLASH_CMD_PREPARE;
    FLASH_CMD_LOAD_PARAMETER(0u,FLASH_CMD_EEE_ENABLE);
    FLASH_CMD_EXECUTE_AND_WAIT;
    no_errors = !FLASH_CMD_ERRORS;

    return no_errors;
}
