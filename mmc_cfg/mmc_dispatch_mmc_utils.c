/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_dispatch_mmc_utils.c
 *
 * description:  
 * This file contains the function to dispatch MMC Utility related test commands
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/
/**
 *
 * This file contains the function to dispatch MMC Utility related test commands
 *
 * @file
 * @ingroup platform_test
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#include "basic_types.h"
#include "fm.h"
#include "dispatch_mmc_utils.h"
#include "mmc.h"
#include "cal.h"
#include "log.h"

/**
 * Not static. as it is required by the MMC & test harness code
 * @lint -esym(552, MMC_B_retval)
 * @lint -esym(765, MMC_B_retval)
 */
/*lint -esym(552, MMC_B_retval) */
/*lint -esym(765, MMC_B_retval) */
              bool_t  MMC_B_retval;

/**
 * Not static. as it is required by the MMC & test harness code
 * @lint -esym(552, MMC_U16_retval)
 * @lint -esym(765, MMC_U16_retval)
 */
/*lint -esym(552, MMC_U16_retval) */
/*lint -esym(765, MMC_U16_retval) */
            uint16_t  MMC_U16_retval;


/**
 * Not static. as it is required by the MMC & test harness code
 * @lint -esym(552, MMC_aU8_active_faults_bitfield_array)
 * @lint -esym(765, MMC_aU8_active_faults_bitfield_array)
 */
/*lint -esym(552, MMC_aU8_active_faults_bitfield_array) */
/*lint -esym(765, MMC_aU8_active_faults_bitfield_array) */
uint8_t MMC_aU8_active_faults_bitfield_array[ACTIVE_FAULTS_BITFIELD_ARRAY_SIZE];

/**
 * Not static. as it is required by the MMC & test harness code
 * @lint -esym(552, MMC_E_fault_subtype_retval)
 * @lint -esym(765, MMC_E_fault_subtype_retval)
 */
/*lint -esym(552, MMC_E_fault_subtype_retval) */
/*lint -esym(765, MMC_E_fault_subtype_retval) */
    Fm_fault_subtype_list_e  MMC_E_fault_subtype_retval;

/**
 * Not static. as it is required by the MMC & test harness code
 * @lint -esym(552, MMC_E_param_fault_code)
 * @lint -esym(765, MMC_E_param_fault_code)
 */
/*lint -esym(552, MMC_E_param_fault_code) */
/*lint -esym(765, MMC_E_param_fault_code) */
     Fm_fault_list_e  MMC_E_param_fault_code = (Fm_fault_list_e) 0;

              bool_t MMC_B_enable_mmc_utils = FALSE;               /**< overall enable flag for MMC Utilities testing */

/**
 * Not static. as it is required by the MMC & test harness code
 * @lint -esym(552, MMC_E_param_fail_func_code)
 * @lint -esym(765, MMC_E_param_fail_func_code)
 */
/*lint -esym(552, MMC_E_param_fail_func_code) */
/*lint -esym(765, MMC_E_param_fail_func_code) */
     Fm_fail_func_list_e  MMC_E_param_fail_func_code = Fm_no_fail_fun;

/**
 * Not static. as it is required by the MMC & test harness code
 * @lint -esym(552, MMC_E_command_code_mmc_utils)
 * @lint -esym(765, MMC_E_command_code_mmc_utils)
 */
/*lint -esym(552, MMC_E_command_code_mmc_utils) */
/*lint -esym(765, MMC_E_command_code_mmc_utils) */
  MMC_E_CmdCode_MMC  MMC_E_command_code_mmc_utils   = MmcCmd_MMC_idle;  /**< command variable for invoking MMC Utilities tests */

/*lint -esym(551,MMC_DISP_*fault*) */
/*lint -esym(528,MMC_DISP_*fault*) */
 PRIVATE volatile const uint8_t  MMC_DISP_n_packed_fault = MMC_DISP_NUM_PACK_FAULT ;
 PRIVATE volatile uint8_t        MMC_DISP_packed_faults[ MMC_DISP_NUM_PACK_FAULT ];


 /****************************************************************************
  * PRIVATE Protos
  ***************************************************************************/

 PRIVATE void mmc_clear_encoded_faults(void);
 PRIVATE void mmc_disp_encode_faults(void);

 /****************************************************************************
  * FUNCTIONS
  ***************************************************************************/

FCODE void DispatchMmcUtils(void)
{
         uint16_t array_itr;
  Fm_fault_list_e fault_itr;

  switch ( MMC_E_command_code_mmc_utils )
  {
  default :
    /* clear invalid values */
    MMC_E_command_code_mmc_utils = MmcCmd_MMC_idle;
    /*lint -e(825) */
    /* fallthrough */

  case  MmcCmd_MMC_idle :
    /* nothing required */
    break;


  /* ===== PUBLIC Functions ===== */

  case  MmcCmd_MMC_EXEC_fm_reset_fault_filter :
    fm_reset_fault_filter(MMC_E_param_fault_code);
    MMC_E_command_code_mmc_utils = MmcCmd_MMC_idle;
    break;

  case  MmcCmd_MMC_EXEC_fm_clear_all_faults :
    fm_clear_faults_all();
    /* The faults from log seem to be almost impossible to clear by normal means
     * as they will instantly re-trigger the the fm fault even though no new
     * fault event has occured. They always re-trigger even if the eeprom for
     * log is written between tests. */
//    log_clear_data();
    MMC_E_command_code_mmc_utils = MmcCmd_MMC_idle;
    break;

  case  MmcCmd_MMC_EXEC_fm_clear_non_latching_faults :
    fm_clear_faults_non_latching();
    MMC_E_command_code_mmc_utils = MmcCmd_MMC_idle;
    break;

  case  MmcCmd_MMC_EXEC_fm_is_fault_present :
    MMC_B_retval = fm_is_fault_present(MMC_E_param_fault_code);
    MMC_E_command_code_mmc_utils = MmcCmd_MMC_idle;
    break;

  case  MmcCmd_MMC_EXEC_fm_is_fail_function_active :
    MMC_B_retval = fm_is_fail_function_active(MMC_E_param_fail_func_code);
    MMC_E_command_code_mmc_utils = MmcCmd_MMC_idle;
    break;

  case  MmcCmd_MMC_EXEC_fm_get_fault_subtype :
    MMC_E_fault_subtype_retval = fm_get_fault_subtype(MMC_E_param_fault_code);
    MMC_E_command_code_mmc_utils = MmcCmd_MMC_idle;
    break;

  case  MmcCmd_MMC_EXEC_fm_count_faults :
    MMC_U16_retval = 0u;

    for (fault_itr = (Fm_fault_list_e) 0; fault_itr < Fm_max_fault_types; fault_itr++)
    {
      if (fm_is_fault_present(fault_itr))
      {
        MMC_U16_retval++;
      }
    }
    MMC_E_command_code_mmc_utils = MmcCmd_MMC_idle;
    break;

  case  MmcCmd_MMC_EXEC_fm_fault_counter :
    MMC_U16_retval = fm_get_fault_counter(MMC_E_param_fault_code);
    MMC_E_command_code_mmc_utils = MmcCmd_MMC_idle;
    break;

  case MmcCmd_MMC_EXEC_fm_fetch_all_set_faults :
    /* first clear the whole array */
    for (array_itr = 0u; array_itr < ACTIVE_FAULTS_BITFIELD_ARRAY_SIZE; array_itr++)
    {
      MMC_aU8_active_faults_bitfield_array[array_itr] = 0u;
    }
    /* now set the bits for any faults that are set */
    for (fault_itr = (Fm_fault_list_e) 0; fault_itr < Fm_max_fault_types; fault_itr++)
    {
      if (fm_is_fault_present(fault_itr))
      {
        MMC_aU8_active_faults_bitfield_array[(((uint16_t)fault_itr) / ACTIVE_FAULTS_BITFIELD_ELEMENT_BITS)] |= (uint8_t)( ((uint8_t)(1u)) << (((uint16_t)fault_itr) % ACTIVE_FAULTS_BITFIELD_ELEMENT_BITS) );
      }
    }
    MMC_E_command_code_mmc_utils = MmcCmd_MMC_idle;
    break;

  case MmcCmd_MMC_EXEC_encode_faults:
      mmc_clear_encoded_faults();
      mmc_disp_encode_faults();
      MMC_E_command_code_mmc_utils = MmcCmd_MMC_idle;
      break;

  case MmcCmd_MMC_EXEC_set_base_cal:
      MMC_U16_retval = (uint16_t) cal_copy_flash_cal(Cal_slave_danomem);
      MMC_E_command_code_mmc_utils = MmcCmd_MMC_idle;
      break;



  /* ===== PRIVATE Functions ===== */



  /* ===== Special Tests ===== */


  }
}

/**
 * Clear out the contents of the encoded fault buffer to 0 */
PRIVATE void mmc_clear_encoded_faults(void)
{
    uint16_t j;
    for (j=0u;j<MMC_DISP_NUM_PACK_FAULT;j++)
    {
        MMC_DISP_packed_faults[j] =0u;
    }
}

/**
 * Pack the fault codes into a data structure, putting 8 fault flags into each
 * byte Start filling from least significant bit of byte 0*/
PRIVATE void mmc_disp_encode_faults(void)
{

    Fm_fault_list_e i;
    uint16_t bytepos; /* The index into the array we are writing */
    uint8_t bitpos;     /* The bit position we write to */
    uint8_t bitmask;    /* The mask to write */

    for (i = (Fm_fault_list_e) 0; i < Fm_max_fault_types; i++)
    {
        bytepos = ((uint16_t)i>>3);
        bitpos=   (uint8_t)(((uint16_t)i) & 0x7u);
        bitmask = (uint8_t)(1u<<bitpos);
        if (fm_is_fault_present(i))
        {
            MMC_DISP_packed_faults[ bytepos ] |= bitmask;
        }
    }
}


/* ////////////////////////////////////////////////////////////////////////// */
