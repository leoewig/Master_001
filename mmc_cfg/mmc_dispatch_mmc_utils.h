/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_dispatch_mmc_utils.h
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
#if ! defined DISPATCH_MMC_H
#define DISPATCH_MMC_H "$Id: mmc_dispatch_mmc_utils.h 2777 2012-07-06 12:15:44Z tg10 $"

/**
 *
 * This file shares variables between the platform-test main and the MMC Utilities
 * command dispatcher function - mainly the general-purpose variables for
 * holding parameter and return values. It also holds the declarations of the
 * dispatcher functions and other shared gubbins
 *
 * @file
 * @ingroup prodder
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#include "dispatch_mmc_utils_types.h"

/**
 * Not static. as it is required by the MMC & test harness code
 * @lint -esym(714, MMC_B_enable_mmc_utils)
 * @lint -esym(759, MMC_B_enable_mmc_utils)
 * @lint -esym(765, MMC_B_enable_mmc_utils)
 */
/*lint -esym(714, MMC_B_enable_mmc_utils) */
/*lint -esym(759, MMC_B_enable_mmc_utils) */
/*lint -esym(765, MMC_B_enable_mmc_utils) */
extern       bool_t  MMC_B_enable_mmc_utils;

/**
 * Not static. as it is required by the MMC & test harness code
 * @lint -esym(714, DispatchMmcUtils)
 * @lint -esym(759, DispatchMmcUtils)
 * @lint -esym(765, DispatchMmcUtils)
 */
/*lint -esym(714, DispatchMmcUtils) */
/*lint -esym(759, DispatchMmcUtils) */
/*lint -esym(765, DispatchMmcUtils) */
extern FCODE void    DispatchMmcUtils(void);

#endif /* ! defined DISPATCH_MMC_H */

/* ////////////////////////////////////////////////////////////////////////// */
