/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_aps_p.h
 *
 * description:  
 *  Private header for the APS module
 *  Application-specific services.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/
#if !defined(MMC_APS_MAIN_P_H)
#define      MMC_APS_MAIN_P_H

/*
 * enumerations and macro definitioms made available must be documented here to
 * avoid duplication.
 *
 * PRIVATE functions are documented in the source file.
 * Non-PRIVATE variables and functions (i.e. those shared between files in the
 * feature) are documented here.
 */


/*
 * PRIVATE function prototypes (documented in source)
 */

/**
 * Initialisation of interface components, these are strategy-specific but
 * are kicked-off before the strategy proper starts */
PRIVATE FCODE void aps_interface_init(void);

/**
 * Initialisation of strategy features allocated to tasks */
PRIVATE FCODE void aps_features_init(void);

PRIVATE FCODE void aps_run_scheduler(void);

PRIVATE FCODE void aps_shutdown(void);

PRIVATE FCODE void aps_fg_init(void);

PRIVATE FCODE void aps_bg_init(void);

/**
 * foreground task
 *
 * Scheduled by the #FGLOOP_PIT channel interrupt
 */
PRIVATE SCHEDULER_TASK_DECL(fg_task);

/**
 * background task
 *
 * Scheduled by @todo
 */
PRIVATE SCHEDULER_TASK_DECL(bg_task);

/**
 *
 *  The idle task is the same for all targets. Every target should include this
 *  as their lowest-priority task.
 */

PRIVATE SCHEDULER_TASK_DECL(idle_task);


#endif      /* MMC_APS_MAIN_P_H */

