/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_eol.c
 *
 * description:  MMC End-of-line data module
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
#include "basic_types.h"
#include "fm.h"
#include "mmc_eol.h"
#include "eol.h"

/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/

/**
 * MMC End-of-line data structure mapped to absolute location.
 *
 */
/*lint -esym(728, mmc_eol )*/
#ifndef HOSTED
#pragma section @gpage @far const {eol}
#endif
/**
 * @a2l{.unit_serial_number,type,ASCII}
 * @a2l{.unit_serial_number,longidentifier, Serial number of this unit. }
 * @a2l{.unit_hw_revision,type,ASCII}
 * @a2l{.unit_hw_revision,longidentifier, Hardware revision of this unit. }
 * @a2l{.year_of_manufacture,type,ASCII}
 * @a2l{.year_of_manufacture,longidentifier, Year that this unit was manufactured. }
 * @a2l{.month_of_manufacture,type,ASCII}
 * @a2l{.month_of_manufacture,longidentifier, Month that this unit was manufactured. }
 * @a2l{.day_of_manufacture,type,ASCII}
 * @a2l{.day_of_manufacture,longidentifier, Day that this unit was manufactured. }
 */
FROM eol_t mmc_eol = EOL_DUMMY_INITIALISER ;
/**< NB This initialiser will not end up in the binary because the linker is
 * configured leave the .eol section uninitialised.  However, if we leave it
 * out, the linker insists on ignoring the \#pragma section directive
 * regardless of whatever permutation of syntax I try
 */

#ifndef HOSTED
#pragma section @gpage @far const []
#endif
/**
 * Status of #mmc_eol. See definition of #eol_data_status_t for details.
 */
eol_data_status_t mmc_eol_data_status = eol_data_not_checked_yet;

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

/**
 * List of supported hardware version strings
 */
/** @polyspace<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit
 *  test purposes */
PRIVATE CONST char_t * mmc_eol_supported_hw_versions[] = {
    "RUB-010",
    "RUB_010",                  /* some discrepancy over whether to use
                                   '-' or '_' but since this has no
                                   material impact, we'll allow either */
    "RUB-011",
    "RUB_011",
    ""                          /* empty string to mark end of list */
};

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/**
 * Read EOL data from dedicated memory location and check its consistency
 * and compatibility
 */
void mmc_init_eol(void)
{
    bool_t fault_present;
    mmc_eol_data_status = check_eol((FPTR const eol_t * )&mmc_eol,
                                mmc_eol_supported_hw_versions);

    fault_present = (eol_data_ok != mmc_eol_data_status);
    (void) fm_update_fault_filter(Fm_plat_mmc_missing_eol_data,
                                Fm_general_failure_information,
                                fault_present,
                                TRUE);
}
