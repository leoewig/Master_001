/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     vio_types_p.h 
 *
 * description:   
 * Types/macros for vio that are internal to the c file
 * This prevents the canbedded files choking on and complex macro definitions,
 * but can still be shared with the unit tests.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Types/macros for vio that are internal to the c file
 *                       
 *
 *****************************************************************************/
#if !defined(VIO_TYPES_P_H)
#define      VIO_TYPES_P_H
/**
 *
 * Types/macros for vio that are internal to the c file
 * This prevents the canbedded files choking on and complex macro definitions,
 * but can still be shared with the unit tests.
 *
 * @file
 * @ingroup vio
 */

#define VIO_IMPEDANCE_MIN_PHYS 100.0f

#define VIO_CAPACITY_MIN_PHYS 50.0f

/** Minimum value for transmitted impedence, subtracted before output over
 * CAN */
#define VIO_IMPEDANCE_MIN   PERCENT(VIO_IMPEDANCE_MIN_PHYS)

/** Minimum value for transmitted capacity, subtracted before output over
 * CAN */
#define VIO_CAPACITY_MIN    PERCENT(VIO_CAPACITY_MIN_PHYS)

#endif      /* VIO_TYPES_P_H */
