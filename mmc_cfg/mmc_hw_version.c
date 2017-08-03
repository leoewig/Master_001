/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     mmc_hw_version.c
 *
 * description:  
 * HW Build Version utilities
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/

/**
 * @ingroup utils
 * @{
 * @file
 * @}
 */

#include "basic_types.h"
#include "mmc.h"
#include "mmc_hw_version.h"
#include "strings.h"      /* For access to string comparison utilities */
#include "mmc_eol.h"      /* For access to EOL data                    */

struct hw_versions_s
{
    char_t                    version_name[HW_VERSION_MAX_LEN];
    uint8_t                   string_length;
    enum  hw_build_versions_e build_version;
};

/** The number of hardware versions */
#define SUPPORTED_HW_VERSION_STRINGS 4u

/** The build versions */
PRIVATE CONST struct hw_versions_s hw_version_strings[] =
{
    { "RUB-010", 7u, Hw_rub_010, },
    { "RUB_010", 7u, Hw_rub_010, },
    { "RUB-011", 7u, Hw_rub_011, },
    { "RUB_011", 7u, Hw_rub_011, },
};

/**
 * @a2l{type,ASCII}
 * @a2l{longidentifier,Uppercase version of the hardware build information}
 */
PRIVATE FRAM char_t hw_version[HW_VERSION_MAX_LEN];

PRIVATE void hw_store_version(void);


/**
 * @a2l{conversion,hw_build_versions_e}
 * @a2l{longidentifier,The HW Version as determined from the stored string in the EOL data}
 */
PRIVATE enum hw_build_versions_e mmc_hw_version = Hw_rub_011;

/** Enumeration determing whether the HW Version has been initialised or not */
PRIVATE enum hw_build_versions_state_e mmc_hw_version_state = Hw_uninitialised;

/**
 * Check if given hardware version string matches supported ones & store if found
 */
void hw_store_version(void)
{
    uint8_t i;

    uppercase_string(mmc_eol.unit_hw_revision, hw_version, HW_VERSION_MAX_LEN);

    for (i = 0u; i < SUPPORTED_HW_VERSION_STRINGS; i++)
    {
        if (string_compare(hw_version, hw_version_strings[i].version_name, hw_version_strings[i].string_length) == TRUE)
        {
            mmc_hw_version       = hw_version_strings[i].build_version;
            mmc_hw_version_state = Hw_initialised;
        }
    }

    if (Hw_uninitialised == mmc_hw_version_state)
    {
        /* No valid string stored, prevent useless future calls */
        mmc_hw_version_state = Hw_default;
    }
}

/**
 * Check if given hardware version string matches supported ones
 *
 * @return      hw version enumeration if hardware string matches one of supported hw strings
 * @return      hw_rub_011 otherwise
 */
enum hw_build_versions_e mmc_build_version(void)
{
    /* Make sure that the hw version has been initialised once & only once */
    if (Hw_uninitialised == mmc_hw_version_state)
    {
        hw_store_version();
    }

    return mmc_hw_version;
}
