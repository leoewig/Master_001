/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   shifts.c
 *
 * description:  shifts
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
/**
 * @file
 * @ingroup fixedpoint
 * $Id: shifts.c 1906 2012-05-03 12:50:46Z tg10 $
 */

#include "fixedpoint.h"		/* public header */
#include "fixedpoint_p.h"	/* private header */

/* Global variables */
/* (none) */

/* Private variables */
/* (none) */

/******************************************************************************
 *
 * shift_xxx: bidirectional shifts
 *
 *****************************************************************************/

/** @polyspace:begin<MISRA-C:19.4:Not a defect:No action planned> Trusted code, so suppress */
/** @polyspace:begin<MISRA-C:19.10:Not a defect:Justify with annotations> Necessary for correct macro expansion */

#define SHIFT( TYPE, SHL, SHR ) \
    TYPE result; \
    binpt_t bpdiff = bpa-bpb; \
    \
    if( bpdiff == 0 ) { \
	result = b; \
    } else if ( bpdiff < 0 ) { \
	bpdiff = -bpdiff; \
	result = SHR( b, bpdiff ); \
    } else { /* bpdiff > 0 */ \
        result = SHL( b, bpdiff ); \
    } \
    return result

/** @polyspace:end<MISRA-C:19.10:Not a defect:Justify with annotations> Necessary for correct macro expansion */
/** @polyspace:end<MISRA-C:19.4:Not a defect:No action planned> Trusted code, so suppress */


FCODE uint32 shift_u32( uint32 b, binpt_t bpb, binpt_t bpa)
{
    SHIFT( uint32, shl_u32, lsr_u32 );
}

FCODE sint32 shift_s32( sint32 b, binpt_t bpb, binpt_t bpa)
{
    SHIFT( sint32, shl_s32, asr_s32 );
}

FCODE uint16 shift_u16( uint16 b, binpt_t bpb, binpt_t bpa)
{
    SHIFT( uint16, shl_u16, lsr_u16 );
}

FCODE sint16 shift_s16( sint16 b, binpt_t bpb, binpt_t bpa)
{
    SHIFT( sint16, shl_s16, asr_s16 );
}

FCODE uint8 shift_u8(uint8 b, binpt_t bpb, binpt_t bpa)
{
    SHIFT( uint8, shl_u8, lsr_u8 );
}

FCODE sint8 shift_s8(sint8 b, binpt_t bpb, binpt_t bpa)
{
    SHIFT( sint8, shl_s8, asr_s8 );
}
