/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      flash_fault.c
 *
 * description:    Flash error interrupt service routine
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li     Flash error interrupt service routine
 *
 *****************************************************************************/

/**
 * Flash error interrupt service routine
 *
 * @file
 * @ingroup flash_fault
 */
#include "basic_types.h"
#include "hardware.h"
#include "main_isr.h"
#include "isr.h"
#include "flash_fault.h"
#ifndef VTBM
#include "fm.h"
#endif

#if defined VTBM
/**
 * Flag signaling presence of single bit flash fault
 */
bool_t vtbm_single_bit_fault_present = FALSE;
#endif

/**
 * Initialise the flash fault interrupt by enabling only the double-bit fault
 * error.
 */
void isr_flash_fault_init(void)
{
    /* enable the flash fault interrupt vector */
    (void)set_interrupt_level(VECT_OFFSET_FLASH_Fault,
                                INT_LVL_DEFAULT, INT_CPU_DEST);
    /* with only the double-bit fault interrupt */
    FERCNFG = FERCNFG_DFDIE;
}

/**
 * Flash error ISR
 * Ignores single bit faults (which are automatically corrected by the
 * hardware).
 * Treats double-bit faults as an internal error, which will cause a reset.
 * The bootblock scans flash for double-bit faults so - assuming the fault
 * persists - the application will not launch again.
 */
INTERRUPT void isr_flash_fault(void)
{
    if (FERSTAT & FERSTAT_DFDIF)
    {
        INTERNAL_ERROR();
    }
    else
    {
        /* 
         * It's not a double-bit fault but somehow another interrupt enable
         * bit has become set, so we have a different internal error!
         */
        INTERNAL_ERROR();
    }
}

#if defined VTBM
/**
 * Detect presence of flash single bit fault
 */
void vtbm_single_bit_fault_update(void)
{
    vtbm_single_bit_fault_present = FERSTAT & FERSTAT_SFDIF;
}
#endif

#if !defined VTBM
/**
 * Check for presence of flash single-bit faults occurring in mmc
 * and raise fault if found.
 *
 * @note: single-bit faults occurring in vtbm are reported using icm
 * and processed in icm_process_statuses()
 */
void flash_nonfatal_fault_check(void)
{
    (void) fm_update_fault_filter(Fm_flash_singlebit_fault_corrected,
                Fm_mmc,FERSTAT & FERSTAT_SFDIF,TRUE);
}
#endif
