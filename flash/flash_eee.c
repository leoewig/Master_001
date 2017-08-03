/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      flash_eee.c
 *
 * description:   Source for Flash Emulated EEPROM
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li    Source for Flash Emulated EEPROM 
 *                       
 *
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#include "flash_eee.h"
#ifdef MMC
#include "ts.h"
#include "fm.h"
//#include "obd.h"
#endif
#ifdef BBLK
#include "flash_commands.h"
#endif

#if !defined BBLK && !defined FBL
/* string handling not required by bootblock or FBL */
#include "string.h"
#if defined HOSTED
#define STRNCPY strncpy
#define STRNCMP strncmp
#else
#define STRNCPY gstrncpy
#define STRNCMP gstrncmp
#endif
#endif

#ifdef MMC
/**
 * Timestamp of detection of fbl access delay flag active found active event.
 */
PRIVATE ts_time_t flash_fbl_sec_timestamp;
#endif

/**
 * Returns the current status of the Flash Emulated EEPROM.
 *
 * Implemented by #FLASH_EEE_STATUS
 *
 * @retval Flash_eee_unavailable    The EEE could not be partitioned by the
 *                                  bootloader.
 * @retval Flash_eee_busy           Either there are EEE writes outstanding or
 *                                  the flash controller is busy performing
 *                                  another operation.
 * @retval Flash_eee_idle           The EEE writing is up to date so power can
 *                                  safely be removed.
 */
flash_eee_status_e flash_eee_status(void)
{
    flash_eee_status_e status;

    status = FLASH_EEE_STATUS();
    return status;
}

/**
 * Writes the given shutdown_type into the EEPROM so that the bootloader
 * can detect the cause of a reset and waits for the write to be flushed
 * to NVRAM.
 *
 * Implemented by #FLASH_EEE_RECORD_SHUTDOWN and #FLASH_EEE_WAIT_FOR_WRITE.
 */
void flash_eee_record_shutdown_and_wait(flash_eee_reset_type_e shutdown_type)
{
    FLASH_EEE_RECORD_SHUTDOWN(shutdown_type);
    FLASH_EEE_WAIT_FOR_WRITE();
}

/**
 * Writes the given shutdown_type into the EEPROM so that the bootloader
 * can detect the cause of a reset but doesn't wait for the write to be flushed
 * to NVRAM.
 *
 * Before powering off or resetting, the caller must ensure that
 * flash_eee_status() is not #Flash_eee_busy
 *
 * Implemented by #FLASH_EEE_RECORD_SHUTDOWN
 */
void flash_eee_record_shutdown_dont_wait(flash_eee_reset_type_e shutdown_type)
{
    FLASH_EEE_RECORD_SHUTDOWN(shutdown_type);
}

#if !defined BBLK && !defined FBL
/**
 * Stores the location of an internal error in Emulated EEPROM and updates
 * the shutdown type.
 *
 * See #INTERNAL_ERROR
 */
void flash_eee_log_internal_error(FPTR const char_t *location)
{
    if (!STRNCMP((FPTR char_t *)bootblock_eee.last_internal_error_location,
                        (FPTR char_t *)location,
                        INTERNAL_ERROR_LOCATION_MAXLEN))
    {
        /*
         * latest internal error differs from what's stored there, so write it
         */
        (void) STRNCPY(
                (FPTR char_t *)bootblock_eee.last_internal_error_location,
                        (FPTR char_t *)location,
                        INTERNAL_ERROR_LOCATION_MAXLEN);
    }
    flash_eee_record_shutdown_and_wait(Reset_internal_error);

}

#endif

#ifdef MMC
#define MEMORY_LOCATION S12_MEMORY
#include "memory_layout.h"
PRIVATE CAL bool_t flash_eee_clr_internal_error = FALSE;
/**<
 * @a2l{LongIdentifier,Cal to allow stored internal error to be cleared}
 * @a2l{conversion,bool_t}
 */

void flash_eee_idle_tasks(void)
{
    if (flash_eee_clr_internal_error)
    {
        bootblock_eee.last_internal_error_location[0] = '\0';
    }
}

/**
 * Initialize security access delay support for the FBL
 *
 * When bootblock_eee.security_access_delay_flags is found active during
 * mmc initialization it's a result of FBL refused to unlock security level 2 due to the
 * invalid security authorization. Record the timestamp of this event detection to
 * be used for timing out the access delay period.
 */
void flash_fbl_sec_support_init(void)
{
    /*
     * If bootblock_eee.security_access_delay_flags was active, initialize
     * a security delay timer to count-down the security delay period
     */
    if(SEC_LVL2_PROT_GET_DELAY_FLAG() == SEC_LVL2_PROT_ACCESS_DELAY_ACTIVE)
    {
        flash_fbl_sec_timestamp = ts_get_ms();
    }
}

/**
 * Update function of security access delay support for the FBL
 *
 * When bootblock_eee.security_access_delay_flags is active, time out the security
 * access delay period and when expired, make the flag inactive, so the next attempt
 * to unlock security level 2 may be performed next time the FBL is asked to do so.
 */
void flash_fbl_sec_support_update(void)
{
    bool_t max_attempts_fault_present;
    bool_t unit_locked_fault_present;

    if(SEC_LVL2_PROT_GET_DELAY_FLAG() == SEC_LVL2_PROT_ACCESS_DELAY_ACTIVE)
    {
        if(ts_is_elapsed_ms(ts_get_ms(), flash_fbl_sec_timestamp, SEC_LVL2_PROT_ACCESS_DELAY_PERIOD_MS))
        {
            SEC_LVL2_PROT_CLEAR_DELAY_FLAG();
        }
    }
    /* Update fault filter */
    max_attempts_fault_present =
        (SEC_LVL2_PROT_GET_INVALID_ATTEMPTS() >= SEC_LVL2_PROT_MAX_INVALID_ATTEMPTS)
        && (SEC_LVL2_PROT_GET_ENG_FLAG() == SEC_LVL2_PROT_ACCESS_ENG_INACTIVE);
    unit_locked_fault_present =
        (SEC_LVL2_PROT_GET_INVALID_ATTEMPTS() >= SEC_LVL2_PROT_MAX_INVALID_ATTEMPTS)
            && (SEC_LVL2_PROT_GET_ENG_FLAG() == SEC_LVL2_PROT_ACCESS_ENG_ACTIVE);

//    (void) fm_update_fault_filter(Fm_security_lvl2_prot_max_attempts, Fm_general_failure_information,
//        max_attempts_fault_present, TRUE);
//    (void) fm_update_fault_filter(Fm_security_lvl2_prot_unit_locked, Fm_general_failure_information,
//        unit_locked_fault_present, TRUE);

    /* The security access DTCs follow a different lifecycle to all other DTCs.
     * If the fault conditions go away (i.e. as the result of a successful seed-key 
     * exchange), the DTCs must be cleared. */
//    if (!fm_is_fault_present(Fm_security_lvl2_prot_max_attempts))
//    {
//        obd_clear_active_dtc_and_env_data(Fm_security_lvl2_prot_max_attempts,
//                                          Fm_general_failure_information);
//    }
//    if (!fm_is_fault_present(Fm_security_lvl2_prot_unit_locked))
 //   {
 //       obd_clear_active_dtc_and_env_data(Fm_security_lvl2_prot_unit_locked,
 //                                         Fm_general_failure_information);
 //   }
}
#endif

#ifdef BBLK
#pragma section (ramcode)
void flash_eee_ramcode_record_shutdown_and_wait(
                                flash_eee_reset_type_e shutdown_type)
{
    FLASH_EEE_RECORD_SHUTDOWN(shutdown_type);
    FLASH_EEE_WAIT_FOR_WRITE();
}
#endif
