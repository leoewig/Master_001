/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    pl_types.h
 *
 * description:  Header file for Pack Layout Feature
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined( PL_TYPES_H )
#define       PL_TYPES_H
/**
 * @file
 * @ingroup PL
 * Header file for Pack Layout Feature
 *
 */


/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/**
 * Enumeration of VTBM power supply configuration error codes.
 */
typedef enum
{
    Pl_vtbm_ps_cfg_ok,      /**< VTBM supply configuration OK */
    Pl_vtbm_ps_self_supp,   /**< A VTBM is supplying itself */
    Pl_vtbm_ps_invalid,     /**< VTBM supply number out of range */
    Pl_vtbm_ps_multiple     /**< A VTBM is supplying multiple VTBMs */
} pl_vtbm_ps_cfg_err_e;


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

#endif /* PL_TYPES_H */
