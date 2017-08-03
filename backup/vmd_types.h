/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     vmd_types.h
 *
 * description:   Private header for vmd
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Private header for vmd
 *                       
 *
 *****************************************************************************/

#if !defined(VMD_TYPES_H)
#define      VMD_TYPES_H

enum Vmd_override_type_e
{
    Vmd_override_type_tbd,
    Vmd_override_type_valid,
    Vmd_override_type_invalid_no_override,
    Vmd_override_type_invalid_allow_override
};

enum Vmd_override_status_e
{
    Vmd_override_status_not_overridden,
    Vmd_override_status_override_wait_for_epb_and_neutral,
    Vmd_override_status_override_wait_for_not_epb_and_not_neutral,
    Vmd_override_status_overridden
};

#endif /* VMD_TYPES_H */

