/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      flash_commands.h
 *
 * description:   Header for flash commands
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li    Header for flash commands
 *                       
 *
 *****************************************************************************/
#ifndef FLASH_COMMANDS_H
#define FLASH_COMMANDS_H

#include "basic_types.h"
#include "hardware.h"
#include "watchdog.h"

#ifdef EVB_4MHZ
#define FLASH_CMD_CLOCK_DIVIDER 3u
#else
/** Ref Manual Table 29-9 for 8MHz crystal on target hardware or modified EVB */
#define FLASH_CMD_CLOCK_DIVIDER 7u
#endif

/*
 * Implemented flash commands
 */
#define FLASH_CMD_EEE_QUERY             0x1500u
#define FLASH_CMD_EEE_ENABLE            0x1300u

#ifdef BBLK
/* only BBLK needs program/erase - FBL doesn't use these macros */
#define FLASH_CMD_PROGRAM_PFLASH        0x0600u
#define FLASH_CMD_ERASE_PFLASH_SECTOR   0x0a00u
#define FLASH_CMD_PROGRAM_DFLASH        0x1100u
#define FLASH_CMD_ERASE_DFLASH_SECTOR   0x1200u
#endif

/**
 * There are two EEE partitioning commands:
 * + Full Partition D-Flash which erases the D-flash, too, but can only be used
 *   with a BDM connected (i.e. in special mode)
 * + Partition D-Flash which will fail if the D-flash is not empty
 * We have to just use the regular partition command but if there becomes a
 * need to re-partition the D-flash you can rebuild with the full
 * partition command and run connected to a BDM.
 */
#define FLASH_CMD_PARTITION_DFLASH  0x2000u      /**< Only works if DFLASH is
                                                  empty */
/**
 * @lint -esym(755,FLASH_CMD_FULL_PARTITION_DFLASH_BDM_ONLY) macro not
 * referenced:  
 * Only works in special mode (i.e. running within a BDM session) so 
 * not called in regular code.  Left here for convenience, though.
 */
/*lint -esym(755,FLASH_CMD_FULL_PARTITION_DFLASH_BDM_ONLY) */ 
#define FLASH_CMD_FULL_PARTITION_DFLASH_BDM_ONLY  0x0F00u

/*
 * bits in registers
 */
#define FCLKDIV_FDIVLD  0x80u /**< FCLKDIV.FDIVLD indicates that FCLKDIV
                                    register has been written */
#define FSTAT_CCIF      0x80u /**< FSTAT.CCIF 0: flash controller busy,
                                              1: controller idle
                                   Writing 1 to this bit clears it and kicks
                                   off a command */
#define FSTAT_ACCERR    0x20u /**< FSTAT.ACCERR Access error, must be cleared
                                (by writing 1 to it) before another flash
                                command can be attempted */
#define FSTAT_FPVIOL    0x10u /**< FSTAT.FPVIOL Protection violation error,
                                must be cleared (by writing 1 to it) before
                                another flash command can be attempted 
                                @note this code does not attempt to set/clear
                                protections */
#define FSTAT_MGSTAT    3u      /**< FSTAT.MGSTAT[1:0] command completion
                                  status flag */
#define FSTAT_ERROR_MASK        (FSTAT_ACCERR | FSTAT_FPVIOL | FSTAT_MGSTAT)

#define FDIVLD_SET      (FCLKDIV & FCLKDIV_FDIVLD)
#define FLASH_CMD_BUSY  ((FSTAT & FSTAT_CCIF) == 0u)

#define FLASH_CMD_ERRORS    (FSTAT & FSTAT_ERROR_MASK)

#ifndef BBLK
#define FLASH_CMD_KICK_WATCHDOGS        kick_watchdog()
#else
/**
 * Bootblock code must decide between calling the RAM or flash
 * watchdog kicker based on the state of #ram_watchdog
 */
#define FLASH_CMD_KICK_WATCHDOGS        do { \
                                            if (ram_watchdog) \
                                            { \
                                                ram_kick_watchdog(); \
                                            } \
                                            else \
                                            { \
                                                kick_watchdog(); \
                                            } \
                                        } while (0)
#endif

#define FLASH_CMD_KICK_DOGS_TILL_READY  do { \
                                            while (FLASH_CMD_BUSY) \
                                            { \
                                                FLASH_CMD_KICK_WATCHDOGS; \
                                            } \
                                        } while (0)


#define FLASH_CMD_LATCHED_ERRORS        (FSTAT & (FSTAT_ACCERR | FSTAT_FPVIOL))
#define FLASH_CMD_CLEAR_ERRORS  do { \
                                    FSTAT = (FSTAT_ACCERR | FSTAT_FPVIOL); \
                                } while (0)

/**
 * load a parameter into the Flash Common Command Object
 */
#define FLASH_CMD_LOAD_PARAMETER(index,parameter) \
    do { \
        FCCOBIX = (index); \
        FCCOB = (parameter); \
    } while (0)

/**
 * Prepare a flash command by loading the divider if necessary and
 * clearing any errors
 */
#define FLASH_CMD_PREPARE \
    do { \
        if (!FDIVLD_SET) \
        { \
            FCLKDIV = FLASH_CMD_CLOCK_DIVIDER; \
        } \
        FLASH_CMD_KICK_DOGS_TILL_READY; \
        if (FLASH_CMD_LATCHED_ERRORS) \
        { \
            FLASH_CMD_CLEAR_ERRORS; \
        } \
    } while (0)

/**
 * Execute a Flash Command by clearing FSTAT.CCIF and wait for it to complete
 * using #FLASH_CMD_KICK_DOGS_TILL_READY
 * This macro must be preceded by #FLASH_CMD_PREPARE and as many 
 * #FLASH_CMD_LOAD_PARAMETER calls as required
 */
#define FLASH_CMD_EXECUTE_AND_WAIT \
    do { \
        FSTAT = FSTAT_CCIF; /* CCIF is set-to-clear, writing it starts cmd*/ \
        FLASH_CMD_KICK_DOGS_TILL_READY; \
    } while (0)

/**
 * Read data out of the Flash Common Command Object
 * This macro must only be called after #FLASH_CMD_BUSY returns FALSE
 * 
 * @lint -emacro(960,FLASH_CMD_READ_PARAMETER) comma operator used:
 * We need this functionality in both RAM and ROM-based code so we take
 * the complexity-hit of the comma operator in preference to the alternatives
 * of either passing an output parameter into a macro or having two functions
 */
/*lint -emacro(960,FLASH_CMD_READ_PARAMETER) */
#define FLASH_CMD_READ_PARAMETER(index) ((FCCOBIX = (index)), FCCOB)

#endif /* #ifndef FLASH_COMMANDS_H */
