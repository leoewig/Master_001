/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   mmc_hw_version.h
 *
 * description: Public header for Utils
 *    
 *      
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#ifndef HW_VERSION_H
#define HW_VERSION_H

/*************************************************************************//*
 *
 * Public header for Utils
 *
 *****************************************************************************/

 /**
 * @ingroup utils
 * @{
 * @file
 * @}
 */

enum hw_build_versions_state_e
{
    Hw_uninitialised,
    Hw_initialised,
    Hw_default
};

enum hw_build_versions_e
{
    Hw_rub_010,
    Hw_rub_011,
    Hw_max_build_variants
};

enum hw_build_versions_e mmc_build_version(void);

#if defined(UNIT_TEST)

extern enum hw_build_versions_state_e mmc_hw_version_state;

#endif

#endif /* HW_VERSION_H */
