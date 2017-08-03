/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   multiplication.c
 *
 * description: N/A
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


/******************************************************************************
 * INCLUDES
 *****************************************************************************/


#include "basic_types.h"
#include "hardware.h"
#include "fixedpoint.h"		/* public header */
#include "fixedpoint_p.h"	/* private header */

/* Global variables */
/* (none) */

/* Private variables */
/* (none) */

/* Private functions */

/* right-shifts a uint64 to give a uint32. Saturates if the result doesn't
   fit. */
/**
 * @lint Deviation Required : Storage class specified after a type, this is trusted code so suppress
 * lint -e(618)
 */
/*lint -e(618) */
FCODE static uint32 lsr_u64_u32( uint64 input, binpt_t shift );

/* widening u32*u32->u64 multiply */
FCODE static uint64 wmulu64( uint32 b, uint32 c );

/**
 * documentation with prototype in header file
*/
/**
 * @lint Deviation Required : Storage class specified after a type, this is trusted code so suppress
 * lint -e(618)
 */
/*lint -e(618) */
FCODE static uint32 lsr_u64_u32( uint64 input, binpt_t shift )
{
    uint32 result;

    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT( shift >= 0 );

    if( shift >= 64 ) {
	result = 0UL;
    } else {

	/* timing:
	   _lsr_u64_u32,L6: 132
	   L6 loop: 'n' iters: (n-1) * 8 = 8n-8  where 0 <= n <= 15
	   L4,L21: 20
	   L21 loop: 15-'n' iters: (15-n-1) * 4 = 56-4n
	   L01,L54: 37

	   total: 237+4n
	   worst case: n=15: 297 cycles
	*/

	if( shift >= 32 ) {
	    input.low = input.high;
	    input.high = 0UL;
	    shift -= 32;
	}
	if( shift >= 16 ) {
            /**
             * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
             * lint -esym(960, 10.3)
             */
            /*lint -esym(960, 10.3) */
            /** @polyspace<MISRA-C:10.3:Not a defect:No action planned> Trusted code, so suppress */
	    input.low  = (input.high << 16) | (uint32)((uint16)(input.low >> 16 ));
            /*lint +esym(960, 10.3) */
	    input.high >>= 16;
	    shift -= 16;
	}

	/* we now know shift < 16 */
	/* if any of the top 16 bits are still set, or some of the 2nd 16 bits
	   are set, we have an overflow */
	if((((uint16)( input.high >> 16 )) != 0U ) ||
	   ((((uint16)input.high) & fixedpoint_u16_overflow[16-shift]) != 0U )) {
            /** @polyspace<MISRA-C:10.6:Not a defect:No action planned> limits.h is a Cosmic supplied file */
	    result = UINT32_MAX;
	} else {
	    uint16 tmp;

	    result = input.low >> shift;

	    /* we have to do << (15-shift), << 1 rather than << (16-shift),
	       because shift might be 0 (violating MISRA) and a comparision
	       against zero costs more than an extra shift */
	    tmp = (((uint16)input.high) << (15-shift)) << 1;
	    result |= ((uint32)tmp)<<16;

	}
    }

    return result;
}

/**
 * documentation with prototype in header file
*/
/**
 * @lint Deviation Required : Storage class specified after a type, this is trusted code so suppress
 * lint -e(618)
 */
/*lint -e(618) */
FCODE static uint64 wmulu64( uint32 b, uint32 c )
{
    uint64 a;
    uint16 lb, hb, lc, hc;
    uint32 tmp;

    /* Split b and c into low and high words, and do a long multiplication.
    */
    lb = (uint16)b;
    hb = (uint16)(b>>16);

    lc = (uint16)c;
    hc = (uint16)(c>>16);

    /* a long multiplication works as follows:
       a = lb*lc + (hb*lc + lb*hc) << 16 + (hb*hc) << 32
    */

    /* Each multiply compiles to an EMUL */
    a.low  = ((uint32)lc)*((uint32)lb);
    a.high = ((uint32)hc)*((uint32)hb);

    /* mid result 1 */
    tmp    = ((uint32)hc)*((uint32)lb);

    /* add the top of the middle to the bottom of the top. Note that adding to
       the high word can't overflow due to the properties of multiplication */
    a.high += tmp >> 16;

    /* add the bottom of the middle to the top of the bottom, with carry into
       the high result if needed. */
    tmp <<= 16;
    a.low   += tmp;
    if( a.low < tmp ) {
	a.high++;
    }


    /* repeat for the other middle */
    tmp    = ((uint32)lc)*((uint32)hb);
    a.high += tmp >> 16;

    tmp <<= 16;
    a.low   += tmp;
    if( a.low < tmp ) {
	a.high++;
    }

    return a;
}


/***************************************************************************/

/* Public functions */
/**
 * documentation with prototype in header file
*/
FCODE uint32 mulu32( uint32 b, uint32 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    uint64 tmp;
    uint32 a;

    /* a widening multiplication is carried out to give an intermediate
     * result. The intermediate result has a binary point equal to the sum of
     * those of b and c */

    tmp = wmulu64( b, c );

    /* shift the intermediate result to the binary point of a, with saturation
     */
    bpa -= (bpb+bpc);

    if( bpa >= 0 ) {
	/* binary point of result is greater than intermediate: left shift
	   required. */

	/* if there's anything in the top word to start with, left-shifting
	   isn't going to help this, so the result will certainly overflow */
	if(tmp.high != 0UL) {
            /** @polyspace<MISRA-C:10.6:Not a defect:No action planned> limits.h is a Cosmic supplied file */
	    a = UINT32_MAX;
	} else {
	    /* otherwise, we can just treat it as a 32-bit shift. */
	    a = shl_u32( tmp.low, bpa );
	}
    } else {
	/* binary point of result is less than intermediate: right shift
	 * required. */
	a = lsr_u64_u32( tmp, (binpt_t)(-bpa) );
    }

    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE sint32  muls32( sint32 b, sint32 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    uint64 tmp;
    uint32 lng;
    sint32 a;
    bool neg;

    /* a widening multiplication is carried out to give an intermediate
     * result. The intermediate result has a binary point equal to the sum of
     * those of b and c */

	/* Do a long multiply. The problem is that long
	   multiplication doesn't work too well on signed nos, so we first sort
	   out the signs, and by that point we might as well do the shift on
	   the unsigned intermediate result, as well. */

	/* - "A minus times a minus is a plus!" - Mrs. Loch, maths teacher.

	   Although 'b=-b' can overflow (if b==-(2^31), we don't worry too much
	   about that. It all works out when we cast to an unsigned type.
	*/
	neg = FALSE;
	if( b < 0L ) {
            /**
             * @lint Deviation Required : Prohibited Implicit Conversion, this is trusted code so suppress
             * lint -esym(960, 10.1)
             */
            /*lint -esym(960, 10.1) */
            /** @polyspace<MISRA-C:12.6:Not a defect:No action planned> Construct is Misra compliant */
            /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
	    neg = !neg;
            /*lint +esym(960, 10.1) */
	    b = -b;
	}
	if( c < 0L ) {
            /**
             * @lint Deviation Required : Prohibited Implicit Conversion, this is trusted code so suppress
             * lint -esym(960, 10.1)
             */
            /*lint -esym(960, 10.1) */
            /** @polyspace<MISRA-C:12.6:Not a defect:No action planned> Construct is Misra compliant */
            /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
	    neg = !neg;
            /*lint +esym(960, 10.1) */
	    c = -c;
	}

	tmp = wmulu64((uint32)b,(uint32)c);

    /* shift the intermediate result to the binary point of a, with saturation
     */
    bpa -= (bpb+bpc);

    if( bpa >= 0 ) {
	/* binary point of result is greater than intermediate: left shift
	   required. */

	/* if there's anything in the top word to start with, left-shifting
	   isn't going to help this, so the result will certainly overflow */
	if(tmp.high != 0UL) {
	    /* UINT32_MAX is an arbitrary value to trigger the overflow logic
	       below. Anything with the top bit set would do. */
            /** @polyspace<MISRA-C:10.6:Not a defect:No action planned> limits.h is a Cosmic supplied file */
	    lng = UINT32_MAX;
	} else {
	    /* otherwise, we can just treat it as a 32-bit shift. */
	    lng = shl_u32( tmp.low, bpa );
	}
    } else {
	/* binary point of result is less than intermediate: right shift
	 * required. */
	lng = lsr_u64_u32( tmp, (binpt_t)(-bpa));
    }

    /* now restore the sign, and cast the intermediate result to the size of a,
     * with saturation */
    if(( lng & (1UL<<31)) != 0UL ) {
	/* either an overflow, or the result is supposed to be SINT32_MIN: clip
	 * to the right limit */
        /**
         * @lint Deviation Required : Expected signed type, this is trusted code so suppress
         * lint -esym(960, 12.9)
         */
        /*lint -esym(960, 12.9) */
        /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Trusted code so suppress */
	a = neg ? SINT32_MIN : SINT32_MAX;
        /*lint +esym(960, 12.9) */
    } else {
        /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Trusted code so suppress */
	a = neg ? - (sint32) lng : (sint32)lng;
    }

    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE uint32 wmulu32( uint16 b, uint16 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    uint32 a;

    /* this can't overflow as it is a widening multiply */
    a = ((uint32)b)*((uint32)c);

    /* shift the intermediate result to the binary point of a, with saturation
     */
    a = shift_u32( a, (binpt_t)(bpb+bpc), bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE sint32 wmuls32( sint16 b, sint16 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    sint32 a;

    /* this can't overflow as it is a widening multiply */
    a = ((sint32)b)*((sint32)c);

    /* shift the intermediate result to the binary point of a, with saturation
     */
    a = shift_s32( a, (binpt_t)(bpb+bpc), bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE uint16  mulu16( uint16 b, uint16 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    uint32 a_tmp;
    uint16 a;

    /* this can't overflow as it is a widening multiply */
    a_tmp = ((uint32)b)*((uint32)c);

    /* shift the intermediate result to the binary point of a, with saturation
     */
    a = narrow_u16( a_tmp, (binpt_t)(bpb+bpc), bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE sint16  muls16( sint16 b, sint16 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    sint32 a_tmp;
    sint16 a;

    /* this can't overflow as it is a widening multiply */
    a_tmp = ((sint32)b)*((sint32)c);

    /* shift the intermediate result to the binary point of a, with saturation
     */
    a = narrow_s16( a_tmp, (binpt_t)(bpb+bpc), bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE uint16 wmulu16( uint8  b, uint8  c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    uint16 a;

    /* this can't overflow as it is a widening multiply */
    a = ((uint16)b)*((uint16)c);

    /* shift the intermediate result to the binary point of a, with saturation
     */
    a = shift_u16( a, (binpt_t)(bpb+bpc), bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE sint16 wmuls16( sint8  b, sint8  c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    sint16 a;

    /* this can't overflow as it is a widening multiply */
    a = ((sint16)b)*((sint16)c);

    /* shift the intermediate result to the binary point of a, with saturation
     */
    a = shift_s16( a, (binpt_t)(bpb+bpc), bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE uint8   mulu8 ( uint8  b, uint8  c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    uint16 a_tmp;
    uint8 a;

    /* this can't overflow as it is a widening multiply */
    a_tmp = ((uint16)b)*((uint16)c);

    /* shift the intermediate result to the binary point of a, with saturation
     */
    a = narrow_u8( a_tmp, (binpt_t)(bpb+bpc), bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE sint8   muls8 ( sint8  b, sint8  c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    sint16 a_tmp;
    sint8 a;

    /* this can't overflow as it is a widening multiply */
    a_tmp = ((sint16)b)*((sint16)c);

    /* shift the intermediate result to the binary point of a, with saturation
     */
    a = narrow_s8( a_tmp, (binpt_t)(bpb+bpc), bpa );
    return a;
}
