/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   smi_types.h
 *
 * description:  
 * Public header file for the smi feature
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
#if !defined(SMI_TYPES_H)
#define      SMI_TYPES_H
/**
 *
 * Public header file for the smi feature
 *
 * @file
 * @ingroup smi
 */

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/* No idea if these are the actual states, so ... */

enum smi_op_state_e
{
    Smi_bus_off,
    Smi_bus_on,
    Smi_open,
    Smi_precharging,
    Smi_veh_close,
    Smi_chg_close
};

enum smi_wake_src_e
{
    Smi_wake_ignition,
    Smi_wake_charger
};

#endif      /* SMI_TYPES_H */

