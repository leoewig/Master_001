/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      flash_check.c
 *
 * description:    Check P-flash for ECC errors
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li    Check P-flash for ECC errors
 *                       
 *
 *****************************************************************************/

/**
 * Check P-flash for ECC errors
 *
 * Provided we check flash is correct after re-programming, the only run-time
 * check required is that the flash ECC error correction is not reporting any
 * faults.
 *
 * Checking the entire 1Mbyte of flash at 50MHz with the functions below would
 * take 6.4ms.  We reduce the size of the check to 0.75Mbyte for the MMC and
 * 0.5Mbyte for the VTBM, giving times of 4.8ms and 3.2ms respectively.
 *
 * The flash ECC reports two kinds of error:
 * + single bit fault, which is automatically corrected,
 * + double-bit fault, which cannot be corrected
 *
 * flash_usable() will return TRUE unless a double-bit fault is detected.
 * Single-bit faults will result in the SFDIF flag being set in FERSTAT, where
 * it can be tested by the application.
 *
 * @file
 * @ingroup flash_bblk
 * @ingroup flash_fault
 * $Id: flash_check.c 9745 2013-07-31 11:11:40Z vs7 $
 */

#include "basic_types.h"
#include "hardware.h"
#include "flash_bblk.h"
#include "flash_bblk_p.h"
#include "flash_fault.h"       /* for FERSTAT_DFDIF */

#ifdef BBLK_VTBM
/** bottom half of flash is unused on VTBM so don't bother to check it */
#define PPAGE_FIRST_TO_CHECK       0xe0u  /**< PAGE representing start of used
                                          flash at 0x780000 */
#else   /* BBLK_MMC or FBL */
/*
 * We deliberately don't check the VTBM image - if a double-bit fault is present
 * in the image then the application will reset when it attempts to reprogram,
 * but that will only happen when the vehicle is in a safe condition and might
 * never happen in a vehicle's life.
 */
#define PPAGE_FIRST_TO_CHECK       0xd0u  /**< PAGE representing start of
                                          offlinecal, 0x740000, having skipped
                                          the vtbm_image */
#endif
#define PPAGE_LAST_TO_CHECK       0xffu


/**
 * Check all of P-flash for ECC errors.
 * If a single-bit fault is detected, the flag FERSTAT.SFDIF will be set 
 * when the function exits
 * @retval TRUE         no double-bit faults are present
 * @retval FALSE        double-bit faults were detected, indicating that not
 *                      all flash was read correctly
 *
 * @note the execution time for this function is 4.8ms on the MMC and 3.2ms on
 * the VTBM.  No watchdog servicing is carried out in this time.
 */
bool_t flash_usable(void)
{
    flash_check_ecc_phrases();
    return (FERSTAT & FERSTAT_DFDIF) ? FALSE : TRUE;
}

/**
 * Read a byte from every ECC phrase so that single- and double-fault errors
 * will be raised if they are lurking.
 *
 * @note This function must be in near memory because it uses PPAGE to
 * traverse all P-flash.
 *
 * How it works
 * ------------
 * The P-flash is ECC memory which works by storing an ECC byte for every
 * 8-byte flash phrase.  Whenever any memory in a phrase is read, the 8 byte
 * phrase is checked against the ECC byte.  If a single bit is wrong, it is
 * automatically corrected and the SFDIF flag is set.  If 2 bits are wrong,
 * they cannot be corrected - so the read may be incorrect - and the DFDIF flag
 * is set.  By reading the last byte of one phrase and the first of the next in
 * a single 16-bit read, two phrases can be checked simultaneously.  See 
 * AN3419: System Integrity Techniques for the S12XE.
 *
 * This function checks the range of memory between PPAGE_FIRST_TO_CHECK and
 * PPAGE_LAST_TO_CHECK by reading bytes from two phrases at a time.  At the end
 * of the sweep, any faults will be shown by the state of SFDIF and DFDIF.
 *
 * Assembly language is used in the inner loop of this function because it has
 * a direct effect on start-up time.
 */

PRIVATE NCODE void flash_check_ecc_phrases(void)
{
    uint8_t ppage_saved;

    ppage_saved = PPAGE;
    PPAGE = PPAGE_FIRST_TO_CHECK - 1u;     /* initialise taking account of
                                            increment at start of loop */
    do
    {
        PPAGE++;
#if !defined HOSTED && !defined _lint   /* hide assembler from PC and lint */
#pragma asm

LAST_BYTE_OF_FIRST_PHRASE_IN_WINDOW:       equ     $8007
END_OF_WINDOW:             equ     $bfff

        ldx     #LAST_BYTE_OF_FIRST_PHRASE_IN_WINDOW
inner_loop:
        ; Word reads at the address of the last byte of a phrase will test
        ; the ECC syndrome of two phrases at once.  We're not interested in
        ; the data actually read, just the act of reading 
        ; To reduce the looping overhead, test 16 phrases in total per
        ; loop.  We could do more per loop but with diminishing returns.
        ldy     0,x             ; test phrases 0,1
        ldy     16,x            ; 2,3
        ldy     32,x            ; 4,5
        ldy     48,x            ; 6,7
        ldy     64,x            ; 8,9
        ldy     80,x            ; 10,11
        ldy     96,x            ; 12,13
        ldy     112,x           ; 14,15
        leax    128,x           ; increase X to point at the next set of
                                ; 16 phrases (16*8 == 128)
        cpx     #END_OF_WINDOW
        bls     inner_loop      ; branch if x contains an address less than or
                                ; the same as END_OF_WINDOW
#pragma endasm
#endif
    } while (PPAGE < PPAGE_LAST_TO_CHECK);

    PPAGE = ppage_saved;
}
