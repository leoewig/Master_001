/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    interp.h
 *
 * description:  interp Linear Interpolation
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined (INTERP_H)
#define       INTERP_H
/*
/**
 * @file
 * @defgroup interp Linear Interpolation
 * @ingroup utils
 * $Id: interp.h 1944 2012-05-08 09:52:29Z tg10 $
 */

#include "basic_types.h"
#include "eq_types.h"

/**
 * @ingroup interp
 */
/**
 * Evaluate a function y=f(u) by performing a 1-D linear interpolation of
 * the table ('2DMAP') represented by uaxis[n_pts] and yaxis[n_pts].
 * Values in uaxis[] must be monotonically increasing. (1 <= n_pts <= 255)
 */
FCODE uint16 interp_1d_u16_u16 (uint16 u,
                               const FPTR uint16 *uaxis,
                               const FPTR uint16 *yaxis,
                               uint8 n_pts);

/**
 * @ingroup interp
 */
/** As with interp_1d_u16_u16, but using signed values. */
FCODE sint16 interp_1d_s16_s16 (sint16 u,
                               const FPTR sint16 *uaxis,
                               const FPTR sint16 *yaxis,
                               uint8 n_pts);

/**
 * @ingroup interp
 */
/** As with interp_1d_u16_u16, but using signed values. */
FCODE sint16 interp_1d_u16_s16 (uint16 u,
                               const FPTR uint16 *uaxis,
                               const FPTR sint16 *yaxis,
                               uint8 n_pts);
#endif /* INTERP_H */
