/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    interp2d.h
 *
 * description:  2D interpolation header
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
 * 2D interpolation header
 *
 * @file
 * @ingroup utils
 */


#if !defined (INTERP2D_H)
#define INTERP2D_H

/*****************************************************************************
 *  Headers
 ****************************************************************************/

#include "basic_types.h"

/*****************************************************************************
 *  Types
 ****************************************************************************/

typedef struct {
   const uint8_t Nx;          /* No of x axis points */
   const int16_t *x_table;      /* Pointer to start of x array */
   const uint8_t Ny;          /* No of y axis points */
   const uint16_t *y_table;      /* Pointer to start of y array */
   const int16_t *z_table;      /* Pointer to start of z array */
} TAB2D_I16_I16_U16_tp;

/*****************************************************************************
 *  Functions
 ****************************************************************************/

extern int16_t interp_i16_i16_u16(
   const TAB2D_I16_I16_U16_tp *map,  /* pointer to table struct */
   int16_t       x,                    /* x input */
   uint16_t       y                     /* y input */
);

#endif /* (INTERP2D_H) */
