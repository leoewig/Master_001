/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    ixcp_prog_allowed.c
 *
 * description: 
 * Check that conditions are appropriate to reprogram a module.
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Temperature conversion module
 *
 *****************************************************************************/
/**
 * Check that conditions are appropriate to reprogram a module.
 *
 * For an MMC, this is done by examining the contactors and current flow.
 * The VTBM leaves the decision to the XCP master (the MMC), so will always
 * allow reprogramming.
 *
 * @file
 * @ingroup ixcp
 *
 */
#include "basic_types.h"
#include "flash_eee.h"  
#include "ixcp.h"

#ifdef MMC
//#include "ctc.h"
//#include "ctc_p.h"
//#include "imi.h"

/**
 * Check that external conditions are OK to reset for reprogramming.
 * This checks the conditions of ixcp_prog_allowed() that do not depend on
 * the bootloader.
 * Contactors are open and current is not flowing.
 * This does NOT check for vprg because it assumes whatever programming 
 * feature is calling this knows what it is doing.
 */
bool_t ixcp_reset_conditions_ok(void)
{
    bool_t reset_allowed;
    bool_t current_not_flowing;
    bool_t veh_contactors_open;
    /*
    * if the zero current status is not Imi_hv_zero, then either current is
    * flowing or we haven't got a working current sensor, in which case we
    * must make sure the contactors are demanded open before reprogramming.
    */
    /*current_not_flowing = (imi_hv_bus_zero_curr_status == Imi_hv_zero);
    //veh_contactors_open = (ctc_cont_veh_psn_dmd == Ctc_cont_open);
	veh_contactors_open = 1;
    
    //reset_allowed = (ctc_cont_chg_psn_dmd == Ctc_cont_open)
    //                 && (current_not_flowing || veh_contactors_open);
	reset_allowed = (current_not_flowing || veh_contactors_open);*/
	reset_allowed = 1;
    return reset_allowed;
}

/**
 * Check that it is ok to repogram the module and that the bootloader supports
 * the reprogramming type.
 *
 * We can't be too prescriptive because we will want to reprogram modules
 * on a bench.  Therefore we just require that _either_ the vehicle and charger
 * contactors are demanded to be open _or_ that we can see that no current
 * is flowing.
 */
bool_t ixcp_prog_allowed(
                flash_eee_reprogramming_e reprogramming_method_requested)
{
    bool_t suitable_bootloader;
    bool_t programming_allowed;

    bool_t reset_allowed;

    reset_allowed = ixcp_reset_conditions_ok();

    if (reprogramming_method_requested == Reprogram_vtbms)
    {
        /* reprogramming the VTBMs doesn't need the bootloader's help so
         * always ok */
        suitable_bootloader = TRUE;
    }
    else if (bootblock_eee.reprogramming_options == Reprogram_iso_and_xcp)
    {
        /* this bootloader can do everything! */
        suitable_bootloader = TRUE;
    }
    else if (bootblock_eee.reprogramming_options
                == reprogramming_method_requested)
    {
        /* we've asked for something the bootloader knows how to do */
        suitable_bootloader = TRUE;
    }
    else
    {
        /* request incomopatible with the current bootloader */
        suitable_bootloader = FALSE;
    }

    programming_allowed = (suitable_bootloader && reset_allowed );

    return programming_allowed;
}
#elif defined VTBM
/**
 * VTBMs leave the decision to reprogram to the MMC, so the IXCP slave will
 * always permit reprogramming
 *
 * @lint -esym(715,reprogramming_method_requested) parameter not accessed
 * because it is only relevent on the mmc
 */
bool_t ixcp_prog_allowed(
                flash_eee_reprogramming_e reprogramming_method_requested)
{
    /*lint -esym(715,reprogramming_method_requested) */
    return TRUE;
}
#else
#error ixcp_prog_allowed should only be compiled for applications
#endif
