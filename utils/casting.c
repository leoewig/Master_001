/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:       casting.c
 *
 * description:  
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
 */

#include "basic_types.h"
#include "fixedpoint.h"		/* public header */

/* Global variables */
/* (none) */

/* Private variables */
/* (none) */

/* Public functions */

/******************************************************************************
 *
 * Sign conversions
 *
 *****************************************************************************/
/**
 * documentation with prototype in header file
*/
FCODE uint32 s_to_u32(sint32 b, binpt_t bpb, binpt_t bpa)
{
    uint32 a;

    if( b < 0L ) {
	a = 0uL;
    } else {
	a = (uint32)b;
    }

    a = shift_u32( a, bpb, bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE sint32 u_to_s32(uint32 b, binpt_t bpb, binpt_t bpa)
{
    sint32 a;

    b = shift_u32( b, bpb, bpa );
    if( b > (uint32)SINT32_MAX ) {
	a = SINT32_MAX;
    } else {
	a = (sint32)b;
    }
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE uint16 s_to_u16(sint16 b, binpt_t bpb, binpt_t bpa)
{
    uint16 a;

    if( b < 0 ) {
	a = 0u;
    } else {
	a = (uint16)b;
    }

    a = shift_u16( a, bpb, bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE sint16 u_to_s16(uint16 b, binpt_t bpb, binpt_t bpa)
{
    sint16 a;

    b = shift_u16( b, bpb, bpa );
    if( b > (uint16)SINT16_MAX ) {
	a = SINT16_MAX;
    } else {
	a = (sint16)b;
    }
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE uint8 s_to_u8(sint8 b, binpt_t bpb, binpt_t bpa)
{
    uint8 a;

    if( b < 0 ) {
	a = 0u;
    } else {
	a = (uint8)b;
    }

    a = shift_u8( a, bpb, bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE sint8 u_to_s8(uint8 b, binpt_t bpb, binpt_t bpa)
{
    sint8 a;

    b = shift_u8( b, bpb, bpa );
    if( b > (uint8)SINT8_MAX ) {
	a = SINT8_MAX;
    } else {
	a = (sint8)b;
    }
    return a;
}

/******************************************************************************
 *
 * Narrowing casts
 *
 *****************************************************************************/
/**
 * documentation with prototype in header file
*/
FCODE uint16 narrow_u16(uint32 b, binpt_t bpb, binpt_t bpa)
{
    uint16 a;

    b = shift_u32(b,bpb,bpa);
    if(((uint16)(b >> 16)) != 0U ) {
        /** @polyspace<MISRA-C:10.6:Not a defect:No action planned> limits.h is a Cosmic supplied file */
	a=UINT16_MAX;
    } else {
	a=(uint16)b;
    }
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE uint8  narrow_u8(uint16 b, binpt_t bpb, binpt_t bpa)
{
    uint8 a;

    b = shift_u16(b,bpb,bpa);
    if((b & 0xFF00U) != 0U ) {
	a=UINT8_MAX;
    } else {
	a=(uint8)b;
    }
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE sint16 narrow_s16(sint32 b, binpt_t bpb, binpt_t bpa)
{
    sint16 a;

    b = shift_s32(b,bpb,bpa);
    if( b > (sint32)SINT16_MAX) {
	a = SINT16_MAX;
    } else if( b < (sint32)SINT16_MIN ) {
	a = SINT16_MIN;
    } else {
	a=(sint16)b;
    }
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE sint8  narrow_s8(sint16 b, binpt_t bpb, binpt_t bpa)
{
    sint8 a;

    b = shift_s16(b,bpb,bpa);
    if( b > (sint16)SINT8_MAX) {
	a = SINT8_MAX;
    } else if( b < (sint16)SINT8_MIN ) {
	a = SINT8_MIN;
    } else {
	a=(sint8)b;
    }
    return a;
}
