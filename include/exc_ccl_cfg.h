/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   flash_prog_types.h
 *
 * description:  
 *  Macros and types for flash programming
 * 
 * 
 *  
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
/**
 * Header to customise ccl_cfg.h
 *
 * This file is included by exc_ccl.cfg at the end of ccl_cfg.h.
 *
 * It is necessary because a bug/feature of CANbedded is that it insists on
 * defining CCL_NM_TYPE_NONE which is incompatible with the OSEK NM.
 *
 * @file
 * @ingroup exc
 *
 */

/**
 * CANbedded uses the OSEK NM so it will not compile if CCL_NM_TYPE_NONE
 * is defined
 */
#undef CCL_NM_TYPE_NONE
