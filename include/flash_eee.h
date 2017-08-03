/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   flash_eee.h
 *
 * description:  
 * Macros and types for Flash emulated EEPROM
 *
 * This feature provides application functions to support the use
 * of the Flash module's Emulated EEPROM features.
 *
 * For an application to use the EEPROM, it must first have been partitioned
 * by a bootloader.
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#ifndef FLASH_EEE_H
#define FLASH_EEE_H
/**
 * Headers for Flash Emulated EEPROM
 * @file
 * @ingroup flash_eee
 *
 */
/**
 * Flash Emulated EEPROM
 *
 * This feature provides application functions to support the use
 * of the Flash module's Emulated EEPROM features.
 *
 * For an application to use the EEPROM, it must first have been partitioned
 * by a bootloader.
 *
 * @defgroup flash_eee FLASH_EEE Flash Emulated EEPROM
 * @ingroup flash
 */
/**
 * Flash
 * @defgroup flash Flash
 * @ingroup platform
 */

#include "flash_eee_types.h"

/* Security level2 protection macros */

/** Security level2 protection access delay active */
#define SEC_LVL2_PROT_ACCESS_DELAY_ACTIVE       0xA7u
/** Security level2 protection access delay inactive */
#define SEC_LVL2_PROT_ACCESS_DELAY_INACTIVE     0xFFu
/** Security level2 protection access delay period in milliseconds */
#define SEC_LVL2_PROT_ACCESS_DELAY_PERIOD_MS    (ts_time_t) 600000u
/** Security level2 engineering access active */
#define SEC_LVL2_PROT_ACCESS_ENG_ACTIVE         0xA7u
/** Security level2 engineering access inactive */
#define SEC_LVL2_PROT_ACCESS_ENG_INACTIVE       0xFFu
/** Get the state of security level2 engineering access flag */
#define SEC_LVL2_PROT_GET_DELAY_FLAG()          (bootblock_eee.security_access_delay_flags)
/** De-assert security level2 delay flag */
#define SEC_LVL2_PROT_CLEAR_DELAY_FLAG()        (bootblock_eee.security_access_delay_flags = SEC_LVL2_PROT_ACCESS_DELAY_INACTIVE)
/** Get the state of security level2 engineering access flag */
#define SEC_LVL2_PROT_GET_ENG_FLAG()            (bootblock_eee.security_access_eng_flags)
/** Assert security level2 engineering access flag */
#define SEC_LVL2_PROT_ASSERT_ENG_FLAG()         (bootblock_eee.security_access_eng_flags = SEC_LVL2_PROT_ACCESS_ENG_ACTIVE)
/** Get the number of invalid attempts to unlock security level 2 */
#define SEC_LVL2_PROT_GET_INVALID_ATTEMPTS()    ((uint8_t) ~bootblock_eee.security_access_invalid_count)
/** Set the number of invalid attempts to unlock security level 2 */
#define SEC_LVL2_PROT_SET_INVALID_ATTEMPTS(x)   (bootblock_eee.security_access_invalid_count = (uint8_t)~(x))
/** Number of allowed invalid attempts to unlock security level 2*/
#define SEC_LVL2_PROT_MAX_INVALID_ATTEMPTS      0x0Fu
/** Number by which the number of invalid attempt to unlock security level 2 should be decreased
 * when engineering access is active */
#define SEC_LVL2_PROT_ENG_ADDITIONAL_ATTEMPTS   0x05u

flash_eee_status_e flash_eee_status(void);
void flash_eee_record_shutdown_and_wait(flash_eee_reset_type_e shutdown_type);
void flash_eee_record_shutdown_dont_wait(flash_eee_reset_type_e shutdown_type);

#ifndef BBLK
void flash_eee_log_internal_error(FPTR const char_t *location);
void flash_eee_idle_tasks(void);
#endif

#ifdef MMC
void flash_fbl_sec_support_init(void);
void flash_fbl_sec_support_update(void);
#endif

#ifdef BBLK
void flash_eee_ramcode_record_shutdown_and_wait(
                                flash_eee_reset_type_e shutdown_type);
#endif

extern flash_eee_boot_s bootblock_eee;
extern fbl_eee_meta_s fbl_meta_info[FBL_N_LOGICAL_BLOCKS];


#endif /* #ifndef FLASH_EEE_H */
