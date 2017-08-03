/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   fixedpoint.c
 *
 * description:  
 *
 * fixedpoint
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/

#include "fixedpoint.h"		/* public header */
#include "fixedpoint_p.h"	/* private header */

/* Global variables */
/* (none) */

/* Private variables */
/* (none) */

/* because there's so much similarity between the six addxx functions, we
 * define a macro which expands to the function body. In an ideal world, we'd
 * have the whole function, including the 'signature', defined by the macro,
 * but that seems to upset LDRA.
 *
 * going still further, we could reduce duplication even more by, for example,
 * putting the function bodies in a separate file, and #including it once for
 * each type. But that upsets LDRA again, and MISRA is pretty unhappy with it
 * too.
 *
 * In practice, to stay within MISRA 52 "(Required: There shall be no
 * unreachable code)", and to get 100% coverage, we have to have separate
 * signed and unsigned versions of the ADD and SUB macros.
 *
 * Each function-body macro takes the following arguments. Not all of them are
 * used, but they're all the same for consistency.
 *
 *    TYPE: the type of the inputs and result of the functions
 *    SHR: a function which will do a right shift of the relevant integer type
 *    SHL: a function which will do a left shift of the relevant integer type
 *    CHANGE_BP: a function which will do a shift of the relevant type in
 *         either direction
 *    MAX_CLIP: the maximum clip for this type
 *    MIN_CLIP: the minimum clip for this type.
 *
 */

/** @polyspace:begin<MISRA-C:19.4:Not a defect:Justify with annotations> Trusted code, so suppress */
/** @polyspace:begin<MISRA-C:19.10:Not a defect:Justify with annotations> Trusted code, so suppress */

#define ADDS( TYPE, SHL, SHR, CHANGE_BP, MIN_CLIP, MAX_CLIP ) \
    TYPE a; \
    binpt_t bpdiff; \
    \
    /* if b and c have different binary points, the more precise is shifted to \
     *  the precision of the other. \
     */ \
    bpdiff = bpb-bpc; \
    if( bpdiff < 0 ) { \
	/* c is more precise than b; we need to shift right c. */ \
	bpdiff = -bpdiff; \
	c = SHR(c, bpdiff); \
	bpc = bpb; \
    } else { \
	/* b is more precise than c; we need to shift right b. */ \
	b = SHR(b, bpdiff); \
	bpb = bpc; \
    } \
    \
    /* the addition is performed with saturation (i.e. overflows are trapped \
     * and the result replaced with the most positive or negative result as \
     * appropriate) */ \
    a = b + c; \
    \
    if( c >= (TYPE)0 ) { \
        if( a < b ) { \
	    /* a should have been larger than b, but isn't: clip to max */ \
            a = MAX_CLIP; \
        } \
    } else { \
        if( a > b ) { \
            /* a should have been smaller than b, but isn't: clip to min */ \
            a = MIN_CLIP; \
        } \
    } \
    /* the result is shifted to the binary point of a with saturation as \
     * appropriate */ \
    a = CHANGE_BP( a, bpb, bpa ); \
    \
    return a


#define ADDU( TYPE, SHL, SHR, CHANGE_BP, MIN_CLIP, MAX_CLIP ) \
    TYPE a; \
    binpt_t bpdiff; \
    \
    /* if b and c have different binary points, the more precise is shifted to \
     *  the precision of the other. \
     */ \
    bpdiff = bpb-bpc; \
    if( bpdiff < 0 ) { \
	/* c is more precise than b; we need to shift right c. */ \
	bpdiff = -bpdiff; \
	c = SHR(c, bpdiff); \
	bpc = bpb; \
    } else { \
	/* b is more precise than c; we need to shift right b. */ \
	b = SHR(b, bpdiff); \
	bpb = bpc; \
    } \
    \
    /* the addition is performed with saturation (i.e. overflows are trapped \
     * and the result replaced with the most positive or negative result as \
     * appropriate) */ \
    a = b + c; \
    \
    if( a < b ) { \
	/* a should have been larger than b, but isn't: clip to max */ \
	a = MAX_CLIP; \
    } \
    \
    /* the result is shifted to the binary point of a with saturation as \
     * appropriate */ \
    a = CHANGE_BP( a, bpb, bpa ); \
    \
    return a


#define SUBS( TYPE, SHL, SHR, CHANGE_BP, MIN_CLIP, MAX_CLIP ) \
    TYPE a; \
    binpt_t bpdiff; \
    \
    /* if b and c have different binary points, the more precise is shifted to \
     *  the precision of the other. \
     */ \
    bpdiff = bpb-bpc; \
    if( bpdiff < 0 ) { \
	/* c is more precise than b; we need to shift right c. */ \
	bpdiff = -bpdiff; \
	c = SHR(c, bpdiff); \
	bpc = bpb; \
    } else { \
	/* b is more precise than c; we need to shift right b. */ \
	b = SHR(b, bpdiff); \
	bpb = bpc; \
    } \
    \
    /* the subtraction is performed with saturation (i.e. overflows are \
     * trapped and the result replaced with the most positive or negative \
     * result as appropriate) */ \
    a = b - c; \
    \
    if( c >= (TYPE)0 ) { \
	if( a > b ) { \
	    /* a should have been smaller than b, but isn't: clip to min */ \
            a = MIN_CLIP; \
	} \
    } else { \
	if( a < b ) { \
	    /* a should have been larger than b, but isn't: clip to max */ \
            a = MAX_CLIP; \
	} \
    } \
    \
    /* the result is shifted to the binary point of a with saturation as \
     * appropriate */ \
    a = CHANGE_BP( a, bpb, bpa ); \
    \
    return a

#define SUBU( TYPE, SHL, SHR, CHANGE_BP, MIN_CLIP, MAX_CLIP ) \
    TYPE a; \
    binpt_t bpdiff; \
    \
    /* if b and c have different binary points, the more precise is shifted to \
     *  the precision of the other. \
     */ \
    bpdiff = bpb-bpc; \
    if( bpdiff < 0 ) { \
	/* c is more precise than b; we need to shift right c. */ \
	bpdiff = -bpdiff; \
	c = SHR(c, bpdiff); \
	bpc = bpb; \
    } else { \
	/* b is more precise than c; we need to shift right b. */ \
	b = SHR(b, bpdiff); \
	bpb = bpc; \
    } \
    \
    /* the subtraction is performed with saturation (i.e. overflows are \
     * trapped and the result replaced with the most positive or negative \
     * result as appropriate) */ \
    a = b - c; \
    \
    if( a > b ) { \
	/* a should have been smaller than b, but isn't: clip to min */ \
        a = MIN_CLIP; \
    } \
    \
    /* the result is shifted to the binary point of a with saturation as \
     * appropriate */ \
    a = CHANGE_BP( a, bpb, bpa ); \
    \
    return a


#define MAX( TYPE, SHL, SHR, CHANGE_BP, MIN_CLIP, MAX_CLIP ) \
    TYPE a; \
    if( b > c ) { \
	a = b; \
    } else { \
	a = c; \
    } \
    return a


#define MIN( TYPE, SHL, SHR, CHANGE_BP, MIN_CLIP, MAX_CLIP ) \
    TYPE a; \
    if( b < c ) { \
	a = b; \
    } else { \
	a = c; \
    } \
    return a

#define CLIP( TYPE, SHL, SHR, CHANGE_BP, MIN_CLIP, MAX_CLIP ) \
    TYPE a; \
    if( b > upper ) { \
	a = upper; \
    } else if( b < lower ) { \
	a = lower; \
    } else { \
	a = b; \
    } \
    return a

/** @polyspace:end<MISRA-C:19.4:Not a defect:Justify with annotations> Trusted code, so suppress */
/** @polyspace:end<MISRA-C:19.10:Not a defect:Justify with annotations> Trusted code, so suppress */

/** @polyspace:begin<MISRA-C:10.3:Not a defect:Justify with annotations> Trusted code, so suppress */
/** @polyspace:begin<MISRA-C:10.6:Not a defect:Justify with annotations> Trusted code, so suppress */

/******************************************************************************
 *
 * uint32
 *
 *****************************************************************************/

/**
 * documentation with prototype in header file
*/
FCODE uint32 abs32( sint32 b )
{

    /* casting to uint32 ensures no overflow is possible. The only difficult
     * case is:
     *   b==SINT32_MIN, ie 0x80000000
     *     (-b) overflows to 0x80000000
     *     hence (uint32)(-b) is 0x80000000 = -(SINT32_MIN)
     *
     * hurrah. */

    /**
     * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
     * lint -esym(960, 10.3)
     */
    /*lint -esym(960, 10.3) */
    return ((b < 0L) ? (uint32)(-b) : (uint32)b );
    /*lint +esym(960, 10.3) */
}

/**
 * documentation with prototype in header file
*/
FCODE uint32 addu32( uint32 b, uint32 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    ADDU( uint32, shl_u32, lsr_u32, shift_u32, 0UL, UINT32_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE uint32 subu32( uint32 b, uint32 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    SUBU( uint32, shl_u32, lsr_u32, shift_u32, 0UL, UINT32_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE uint32 maxu32( uint32 b, uint32 c )
{
    MAX( uint32, shl_u32, lsr_u32, shift_u32, 0UL, UINT32_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE uint32 minu32( uint32 b, uint32 c )
{
    MIN( uint32, shl_u32, lsr_u32, shift_u32, 0UL, UINT32_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE uint32 clipu32( uint32 b, uint32 lower, uint32 upper )
{
    CLIP( uint32, shl_u32, lsr_u32, shift_u32, 0UL, UINT32_MAX );
}

/******************************************************************************
 *
 * sint32
 *
 *****************************************************************************/

/**
 * documentation with prototype in header file
*/
FCODE sint32 adds32( sint32 b, sint32 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    /**
     * @lint Deviation Required : Prohibited operator applied to unsigned underlying type, this is trusted code so suppress
     * lint -esym(960, 12.9)
     */
    /*lint -esym(960, 12.9) */
    ADDS( sint32, shl_s32, asr_s32, shift_s32, SINT32_MIN, SINT32_MAX );
    /*lint +esym(960, 12.9) */
}

/**
 * documentation with prototype in header file
*/
FCODE sint32 subs32( sint32 b, sint32 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    /**
     * @lint Deviation Required : Prohibited operator applied to unsigned underlying type, this is trusted code so suppress
     * lint -esym(960, 12.9)
     */
    /*lint -esym(960, 12.9) */
    SUBS( sint32, shl_s32, asr_s32, shift_s32, SINT32_MIN, SINT32_MAX );
   /*lint +esym(960, 12.9) */
}

/**
 * documentation with prototype in header file
*/
FCODE sint32 maxs32( sint32 b, sint32 c )
{
    MAX( sint32, shl_s32, asr_s32, shift_s32, SINT32_MIN, SINT32_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE sint32 mins32( sint32 b, sint32 c )
{
    MIN( sint32, shl_s32, asr_s32, shift_s32, SINT32_MIN, SINT32_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE sint32 clips32( sint32 b, sint32 lower, sint32 upper )
{
    CLIP( sint32, shl_s32, asr_s32, shift_s32, SINT32_MIN, SINT32_MAX );
}

/******************************************************************************
 *
 * uint16
 *
 *****************************************************************************/

/**
 * documentation with prototype in header file
*/
FCODE uint16 abs16( sint16 b )
{
    /* casting to uint16 ensures no overflow is possible. The only difficult
     * case is:
     *   b==SINT16_MIN, ie 0x8000
     *     b is implicitly cast to int; no change on a 16-bit processor
     *     (-b) overflows back to 0x8000
     *     hence (uint16)(-b) is 0x8000 = -(SINT16_MIN)
     *
     * hurrah.
     */

    /**
     * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
     * lint -esym(960, 10.3)
     */
    /*lint -esym(960, 10.3) */
    return ((b < 0) ? (uint16)(-b) : (uint16)b );
    /*lint +esym(960, 10.3) */
}

/**
 * documentation with prototype in header file
*/
FCODE uint16 addu16( uint16 b, uint16 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    ADDU( uint16, shl_u16, lsr_u16, shift_u16, 0U, UINT16_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE uint16 subu16( uint16 b, uint16 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    SUBU( uint16, shl_u16, lsr_u16, shift_u16, 0U, UINT16_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE uint16 maxu16( uint16 b, uint16 c )
{
    MAX( uint16, shl_u16, lsr_u16, shift_u16, 0U, UINT16_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE uint16 minu16( uint16 b, uint16 c )
{
    MIN( uint16, shl_u16, lsr_u16, shift_u16, 0U, UINT16_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE uint16 clipu16( uint16 b, uint16 lower, uint16 upper )
{
    CLIP( uint16, shl_u16, lsr_u16, shift_u16, 0U, UINT16_MAX );
}

/******************************************************************************
 *
 * sint16
 *
 *****************************************************************************/

/**
 * documentation with prototype in header file
*/
FCODE sint16 adds16( sint16 b, sint16 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    ADDS( sint16, shl_s16, asr_s16, shift_s16, SINT16_MIN, SINT16_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE sint16 subs16( sint16 b, sint16 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    SUBS( sint16, shl_s16, asr_s16, shift_s16, SINT16_MIN, SINT16_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE sint16 maxs16( sint16 b, sint16 c )
{
    MAX( sint16, shl_s16, asr_s16, shift_s16, SINT16_MIN, SINT16_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE sint16 mins16( sint16 b, sint16 c )
{
    MIN( sint16, shl_s16, asr_s16, shift_s16, SINT16_MIN, SINT16_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE sint16 clips16( sint16 b, sint16 lower, sint16 upper )
{
    CLIP( sint16, shl_s16, asr_s16, shift_s16, SINT16_MIN, SINT16_MAX );
}

/******************************************************************************
 *
 * uint8
 *
 *****************************************************************************/

/**
 * documentation with prototype in header file
*/
FCODE uint8 abs8( sint8 b )
{
    /* casting to uint8 ensures no overflow is possible. The only difficult
     * case is:
     *   b==SINT8_MIN, ie 0x80
     *     b is implicitly cast to int;
     *             ie 0xFF80 (representing -0x80) on a 16-bit processor
     *     (-b) is therefore 0x0080
     *     hence (uint8)(-b) is 0x80 = -(SINT8_MIN)
     *
     * hurrah.
     */
    /**
     * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
     * lint -esym(960, 10.3)
     */
    /*lint -esym(960, 10.3) */
    return ((b < 0) ? (uint8)(-b) : (uint8)b );
    /*lint +esym(960, 10.3) */
}

/**
 * documentation with prototype in header file
*/
FCODE uint8 addu8( uint8 b, uint8 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    ADDU( uint8, shl_u8, lsr_u8, shift_u8, 0U, UINT8_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE uint8 subu8( uint8 b, uint8 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    SUBU( uint8, shl_u8, lsr_u8, shift_u8, 0U, UINT8_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE uint8 maxu8( uint8 b, uint8 c )
{
    MAX( uint8, shl_u8, lsr_u8, shift_u8, 0U, UINT8_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE uint8 minu8( uint8 b, uint8 c )
{
    MIN( uint8, shl_u8, lsr_u8, shift_u8, 0U, UINT8_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE uint8 clipu8( uint8 b, uint8 lower, uint8 upper )
{
    CLIP( uint8, shl_u8, lsr_u8, shift_u8, 0U, UINT8_MAX );
}

/******************************************************************************
 *
 * sint8
 *
 *****************************************************************************/

/**
 * documentation with prototype in header file
*/
FCODE sint8 adds8( sint8 b, sint8 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    ADDS( sint8, shl_s8, asr_s8, shift_s8, SINT8_MIN, SINT8_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE sint8 subs8( sint8 b, sint8 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    SUBS( sint8, shl_s8, asr_s8, shift_s8, SINT8_MIN, SINT8_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE sint8 maxs8( sint8 b, sint8 c )
{
    MAX( sint8, shl_s8, asr_s8, shift_s8, SINT8_MIN, SINT8_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE sint8 mins8( sint8 b, sint8 c )
{
    MIN( sint8, shl_s8, asr_s8, shift_s8, SINT8_MIN, SINT8_MAX );
}

/**
 * documentation with prototype in header file
*/
FCODE sint8 clips8( sint8 b, sint8 lower, sint8 upper )
{
    CLIP( sint8, shl_s8, asr_s8, shift_s8, SINT8_MIN, SINT8_MAX );
}

/** @polyspace:end<MISRA-C:10.6:Not a defect:Justify with annotations> Trusted code, so suppress */
/** @polyspace:end<MISRA-C:10.3:Not a defect:Justify with annotations> Trusted code, so suppress */
