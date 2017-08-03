/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     watchdog.h
 *
 * description:    Hardware and software watchdog feature header
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li            Hardware and software watchdog feature header
 *                       
 *
 *****************************************************************************/
#if !defined(WATCHDOG_H)
#define WATCHDOG_H

/**
 *
 * @file
 * @ingroup wdg
 *
 * Hardware and software watchdog feature header
 *
 * $Id: $
 */

/**
 *
 * Hardware and software watchdog feature
 * ======================================
 *
 * This module implements API to handle internal software and external hardware
 * watchdogs. Internal software watchdog is configured to reset the system when
 * not updated for longer than 32ms, external hardware watchdog resets the system
 * when not updated for longer than 900ms.
 *
 * Approach used
 * -------------
 * Module implements only two API functions. One to initialize both watchdogs and second
 * to update both watchdogs to keep the system alive.
 *
 * How to use
 * ----------
 * - Call init_watchdog() once
 * - Call kick_watchdog() periodically. Could be called from the main loop or from some
 *   periodic task for the instance. Period between two calls to this function must not exceed
 *   shorter of both watchdogs update period.
 *
 * How to Integrate
 * ----------------
 * - Include watchdog.h file
 *
 * Configuration
 * -------------
 * - External watchdog output pin is same for both mmc and vtbm hardware and defined
 * by #WDG_HW_DDR, #WDG_HW_PINMASK and #WDG_HW_PORT macros
 * - Maximal software watchdog update period is defined by #COP_PERIOD_32MS and might
 * be eventually changed
 *
 * Requirement Satisfaction
 * ------------------------
 * The requirements document for the feature can be found
 * @todo add the link to BMS requirements document
 *
 * All requirements from chapter 3.5 are satisfied by this module.
 *
 * Requirements are satisfied as follows:
 *
 * | Requirement         | Satisfied By      | Unit Tested By | PAT Tested By |
 * |---------------------|------------------ |----------------|---------------|
 * | REQ_BMS_VTBM_WD_001 | init_watchdog(), kick_watchdog() | test_init_watchdog(), test_kick_watchdog() | |
 * | REQ_BMS_VTBM_WD_002 | kick_watchdog() | test_kick_watchdog() | |
 * | REQ_BMS_SUP_WDG_001 | init_watchdog()   | test_init_watchdog() | pat_wdg_internal_basic() |
 * | REQ_BMS_SUP_WDG_002 | fg_task()         | test_kick_watchdog() | pat_wdg_internal_basic() |
 * | REQ_BMS_SUP_WDG_003 | kick_watchdog()   | test_kick_watchdog() | pat_wdg_external_basic() |
 * | REQ_BMS_SUP_WDG_004 | init_watchdog()   | test_init_watchdog_reset_type() | pat_wdg_fault() |
 * | REQ_BMS_SUP_WDG_005 | platform_main()   | test_init_watchdog_reset_type() | pat_wdg_fault() |
 * | REQ_BMS_SUP_WDG_006 | platform_main()   | test_init_watchdog_reset_type() | pat_wdg_fault() |
 *
 * @defgroup wdg Hardware and software watchdog service
 * @ingroup utils
 */


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
#if defined VTBM_EOL_TEST || defined UNIT_TEST
extern bool_t wdg_update_disable;
#endif

#ifdef BBLK
/*
 * bootblocks need to know whether to kick the dog from flash or RAM
 */
extern bool_t ram_watchdog;

/* watchdog kicker in RAM */
extern void ram_kick_watchdog(void);
#endif
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
extern void kick_watchdog(void);
extern void init_watchdog(void);

#endif /* WATCHDOG_H */
