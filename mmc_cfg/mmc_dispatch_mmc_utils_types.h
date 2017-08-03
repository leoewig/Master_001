/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_dispatch_mmc_utils_types.h
 *
 * description:  
 * This file shares variables between the platform-test main and the MMC Utilities
 * command dispatcher function - mainly the general-purpose variables for
 * holding parameter and return values. It also holds the declarations of the
 * dispatcher functions and other shared gubbins
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/
#if ! defined DISPATCH_MMC_TYPES_H
#define DISPATCH_MMC_TYPES_H  "$Id: mmc_dispatch_mmc_utils_types.h 5857 2012-12-17 18:59:22Z js45 $"

/**
  Use the following macro to clear command codes, so that extra actions can be
  hooked in a single place if required. The implemention below captures the
  last actioned code, which makes operation visible to the host (which usually
  is too fast otherwise)
*/

/**
  enumeration for commands to MMC Utility test dispatcher - ref PLAT_E_command_code_mmc_utils
  - one entry per function to be tested, plus an idle state and end-of-list marker
*/

typedef enum
{
  MmcCmd_MMC_idle,                  /**< IDLE value - no-operation (& indicates any command has completed) */
  MmcCmd_MMC_EXEC_fm_reset_fault_filter,
  MmcCmd_MMC_EXEC_fm_clear_all_faults,
  MmcCmd_MMC_EXEC_fm_clear_non_latching_faults,
  MmcCmd_MMC_EXEC_fm_is_fault_present,
  MmcCmd_MMC_EXEC_fm_is_fail_function_active,
  MmcCmd_MMC_EXEC_fm_get_fault_subtype,
  MmcCmd_MMC_EXEC_fm_count_faults,
  MmcCmd_MMC_EXEC_fm_fault_counter,
  MmcCmd_MMC_EXEC_fm_fetch_all_set_faults,
  MmcCmd_MMC_EXEC_encode_faults,
  MmcCmd_MMC_EXEC_set_base_cal,
  /* Make MmcCmd_MMC_list_end always the last entry */
  MmcCmd_MMC_list_end               /**< standard list-end marker @lint not dispatched */ /*lint -esym(788,MmcCmd_MMC_list_end) -esym(769,MmcCmd_MMC_list_end) [enum not used in switch] */
} MMC_E_CmdCode_MMC;

/**
 * The size needed for the encoded fault array in MMC_U16_packed_faults in bytes */
#define MMC_DISP_NUM_PACK_FAULT ((((uint8_t)Fm_max_fault_types -1u )/8u)+1u)


#endif /* ! defined DISPATCH_MMC_TYPES_H */

/* ////////////////////////////////////////////////////////////////////////// */
