/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   interp2d.c
 *
 * description:  
 *
 * 2D interpolation lookup function
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/
/**
 * 2D interpolation lookup function
 *
 * @file
 * @ingroup utils
 */


#include "interp2d.h"

/*
 * Z
 *  + ---Y--->
 *  |
 *  X
 *  |
 *  V
 */

int16_t interp_i16_i16_u16(
   const TAB2D_I16_I16_U16_tp  * map	/* pointer to structure type */,
   int16_t	x, /* row */
   uint16_t	y) /* column */
{
    /** @lint checks 960 961 and 713 disabled, pre-existing code */
   /*lint --e{960} */
   /*lint --e{961} */
   /*lint --e{713} */
   const int16_t *	z_table;
   const uint16_t *	y_table;
   const int16_t *	x_table;

   int16_t Ny;	/* local copy of Ny                 */
   
   z_table = map->z_table;
   y_table = map->y_table;
   x_table = map->x_table;
   Ny = map->Ny;

   { /* begin search algorithms */
      
      /* X-vector: ascending linear search. */
      {
         int16_t low;	/* low table index.                 */

         if ( x < *x_table ) 
            x = *x_table; 
         if ( x > *(x_table + map->Nx-1) ) 
            x = *(x_table + map->Nx-1);
         
         low = 0;
         x_table++;
         while ( x > *(x_table++) ) low++;
         x_table -= 2;
         z_table += low * Ny;
      }
      
      /* Y-vector: ascending linear search. */
      if ( y < *y_table ) 
         y = *y_table; 
      if ( y > *(y_table + Ny-1) ) 
         y = *(y_table + Ny-1);
      
      
      y_table++;
      while ( y > *(y_table++) ) z_table++;
      y_table -= 2;
      
   } /* end search algorithms */

   { /* begin interpolation */
   
      int16_t d_0   ;	/* (x-x1)  or (y-y1)                */
      int16_t d_1   ;	/* (x2-x)  or (y2-y)                */
      int16_t d_01  ;	/* (x2-x1) or (y2-y1)               */
      int16_t z_x1_y;	/* 1. y-interpolation f(x1)         */
      int16_t z_x2_y;	/* 2. y-interpolation f(x2)         */

      /* Y-vector interpolation. */
      /* differences in y vector */
      d_0  =  y       - *(y_table++);
      d_1  = *y_table -   y;
      d_01 = d_0 + d_1;
      /* denominator == 0 ?      */
      if (d_01 == 0) 
      {
         z_x1_y = *z_table;
         z_x2_y = *(z_table + Ny);
      }
      else
      {
         /* 1. y interpolation      */
         z_x1_y   = (int16_t)(  (int32_t)
                  ( (int32_t)d_1 * (int32_t)(*z_table)
                  + (int32_t)d_0 * (int32_t)(*(z_table+1)) )
                  / (int16_t)d_01  );
         /* 2. y interpolation      */
         z_table += Ny;
         z_x2_y   = (int16_t)(  (int32_t)
                  ( (int32_t)d_1 * (int32_t)(*z_table)
                  + (int32_t)d_0 * (int32_t)(*(z_table+1)) )
                  / (int16_t)d_01  );
      }
      
      /* X-vector interpolation. */
      /* differences in x vector */
      d_0  =  x       - *(x_table++);
      d_1  = *x_table -   x;
      d_01 = d_0 + d_1;
      /* denominator != 0 ?      */
      if (d_01 != 0) 
      {
         z_x1_y = (int16_t)(  (int32_t)
                  ( (int32_t)d_1 * (int32_t)(z_x1_y)
                  + (int32_t)d_0 * (int32_t)(z_x2_y) )
                  / (int16_t)d_01  );
      }
      
      /* 'z_x1_y' contains the interpolated result or in the case of */
      /*  divison by zero the lower z-value.                         */
      return z_x1_y;
   } /* end interpolation */
}
