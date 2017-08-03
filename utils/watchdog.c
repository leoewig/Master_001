/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:       watchdog.c
 *
 * description:  
 * 
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/


/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "hardware.h"
#include "watchdog.h"
#if defined MMC
#include "flash_eee.h"
#include "fm.h"
#endif

/******************************************************************************
 * PRIVATE MACRO DEFINITIONS
 *****************************************************************************/
/** First key written to WDOG to avoid COP reset */
#define COP_KEY_0       0x55U
/** Second key written to WDOG to avoid COP reset */
#define COP_KEY_1       0xAAU
/** Hardware watchdog output direction register*/
#define WDG_HW_DDR      DDRT
/** Hardware watchdog output pin mask */
#define WDG_HW_PINMASK  0x01u
/** Hardware watchdog output port register*/
#define WDG_HW_PORT     PTT

/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/
#if defined VTBM_EOL_TEST || defined UNIT_TEST
/** Flag for hardware watchdog update disabling. Used only for vtbm eol test
 * and unit test build */
bool_t wdg_update_disable = FALSE;
#endif

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/**
 * Initialize hardware and software watchdogs
 */
void init_watchdog (void)
{
    /* initialize hardware watchdog output direction register */
    WDG_HW_DDR |= WDG_HW_PINMASK;

    /* initialize software watchdog */
    /*
    * Set RSBCK=1   : COP and RTI stop in BDM
    * CR[2:0]=3 : Wdog period = 2^18/OSCCLK seconds 
    * For BCM hardware, OSCCLK is 8MHz, giving timeout of 0.032s
    */
#ifdef EVB_4MHZ
    /*
     * Unmodified EVB has a 4MHz crystal so can't match MMC exactly, need to
     * choose between twice or half the period.  We'll have half the period
     * (0.016s) so keep COPCTL the same
     */
#endif
    //COPCTL = COP_PERIOD_32MS;

	/*
    * Set RSBCK=1   : COP and RTI stop in BDM    * CR[2:0]=4 : Wdog period = 2^20/OSCCLK seconds 
    * For BCM hardware, OSCCLK is 16MHz, giving timeout of 0.065s
    */
	COPCTL = 0x44;

    /* belt and braces watchdog kick, for when it was already initialized,
     * by the bootblock for instance */
    kick_watchdog();
}

/**
 * Kick hardware and software watchdogs
 */
void kick_watchdog (void)
{
    uint8_t ccr;

    /* Update software watchdog */
    ARMCOP = COP_KEY_0;
    ARMCOP = COP_KEY_1;

        SAVE_INTERRUPTS(ccr);
        /* toggle hardware watchdog output */
        WDG_HW_PORT ^= WDG_HW_PINMASK;
        RESTORE_INTERRUPTS(ccr);

}

