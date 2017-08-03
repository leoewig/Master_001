/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name: division.c
 *
 * description:  division
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
/**
 * documentation with prototype in header file
*/
FCODE uint32  divu32( uint32 b, uint32 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    uint32 a;

    /* An integer division, rounding towards zero, is performed on the
     * underlying stored values, this intermediate result can be considered to
     * have a binary point equal to the difference between the binary points of
     *  b and c. The result is undefined if c==0.
     */

    a = b / c;

    /* The intermediate result is cast to the same size as a, with saturation
       - a no-op here */

    /* Finally the result is shifted to the binary point of a with
     * saturation. */

    a = shift_u32( a, (binpt_t)(bpb-bpc), bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE sint32  divs32( sint32 b, sint32 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    sint32 a;

    /* An integer division, rounding towards zero, is performed on the
     * underlying stored values, this intermediate result can be considered to
     * have a binary point equal to the difference between the binary points of
     *  b and c. The result is undefined if c==0.
     */

    /* There's one potential overflow case here (assuming c != 0):
     * b = -(2^31), c = -1 => result = (2^31)
     */
    /**
     * @lint Investigate : Expected signed type, this is trusted code so suppress
     * Prohibited operator applied to unsigned underlying type, this is trusted code so suppress
     * lint -e(501)
     * lint -e(960)
     */
    /*lint -e(501) */
    /*lint -e(960) */
    if(( b == SINT32_MIN ) && ( c == -1L )) {
	a = SINT32_MAX;
    } else {
	a = b / c;
    }

    /* The intermediate result is cast to the same size as a, with saturation
       - a no-op here */

    /* Finally the result is shifted to the binary point of a with
     * saturation. */

    a = shift_s32( a, (binpt_t)(bpb-bpc), bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE uint16 ndivu32( uint32 b, uint16 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
   uint16 a;

    /* An integer division, rounding towards zero, is performed on the
     * underlying stored values, this intermediate result can be considered to
     * have a binary point equal to the difference between the binary points of
     *  b and c. The result is undefined if c==0.
     */

   /* The intermediate result is cast to the same size as a, with saturation */

   /* We want to use the efficient EDIV instruction, rather than calling out to
      the very complicated (and slow) ldiv routine, but that means casting back
      down to uint16 without checking the top 16 bits of the result. We
      therefore have to check for overflow first.

      The overflow case occurs where b/c >= 0x10000, ie b >= 0x10000 * c, ie
      b/0x10000 >= c. */
   if(((uint16)(b >> 16)) >= c ) {
       /** @polyspace<MISRA-C:10.6:Not a defect:No action planned> limits.h is a Cosmic supplied file */
       a = UINT16_MAX;
   } else {
       /* this gives a flexelint warning (implicit cast), but compiles to
	  EDIV. See deviation #45. */
       a = (uint16)(b / c);
   }

   /* Finally the result is shifted to the binary point of a with
    * saturation. */

    a = shift_u16( a, (binpt_t)(bpb-bpc), bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE sint16 ndivs32( sint32 b, sint16 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
   sint16 a;
   uint16 hb;
   bool neg;

    /* An integer division, rounding towards zero, is performed on the
     * underlying stored values, this intermediate result can be considered to
     * have a binary point equal to the difference between the binary points of
     *  b and c. The result is undefined if c==0.
     */

   /* The intermediate result is cast to the same size as a, with saturation */

   /* We want to use the efficient EDIV(S) instruction, rather than calling out to
      the very complicated (and slow) ldiv routine, but that means casting back
      down to sint16 without checking the top 16 bits of the result. We
      therefore have to check for overflow first.

      The chances are that we'll end up doing negations when we check for
      overflows, so we might as well sort the signs out in advance.

      "a minus over a minus is a plus, too" - Mrs. Loch again

      Although 'b=-b' can overflow (if b==-(2^31), we don't worry too much
      about that. It all works out when we cast to an unsigned type.
   */

   neg = FALSE;
   if( b < 0L ) {
       /**
        * @lint Investigate : Prohibited Implicit Conversion, this is trusted code so suppress
        * lint -esym(960, 10.1)
        */
       /*lint -esym(960, 10.1) */
       /** @polyspace<MISRA-C:12.6:Low:Investigate> Could code be changed to fix this? */
       /** @polyspace<MISRA-C:13.2:Low:Investigate> Could code be changed to fix this? */
       neg = !neg;
       /*lint +esym(960, 10.1) */
       b = -b;
   }
   if( c < 0 ) {
       /**
        * @lint Investigate : Prohibited Implicit Conversion, this is trusted code so suppress
        * lint -esym(960, 10.1)
        */
       /*lint -esym(960, 10.1) */
       /** @polyspace<MISRA-C:12.6:Low:Investigate> Could code be changed to fix this? */
       /** @polyspace<MISRA-C:13.2:Low:Investigate> Could code be changed to fix this? */
       neg = !neg;
       /*lint +esym(960, 10.1) */
       c = -c;
   }

   hb = (uint16)(((uint32)b) >> 16);

   /* check that the unsigned division isn't going to overflow */
   if(hb >= (uint16) c ) {
       /** @polyspace<MISRA-C:13.2:Low:Investigate> Could code be changed to fix this? */
       a = neg ? SINT16_MIN : SINT16_MAX;
   } else {
       uint16 tmp_a;

       /* this gives a flexelint warning (implicit cast), but compiles to
	  EDIV. See deviation #45. */
       tmp_a = (uint16)(((uint32)b) / (uint16)c);

       /* now check the result won't overflow when we cast back to signed.
	*/
       if( tmp_a > (uint16)SINT16_MAX ) {
           /** @polyspace<MISRA-C:13.2:Low:Investigate> Could code be changed to fix this? */
	   a = neg ? SINT16_MIN : SINT16_MAX;
       } else {
	   a = (sint16)tmp_a;
           /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
	   if( neg ) {
	       a = -a;
	   }
       }
   }

   /* Finally the result is shifted to the binary point of a with
    * saturation. */

   a = shift_s16( a, (binpt_t)(bpb-bpc), bpa );
   return a;
}

/**
 * documentation with prototype in header file
*/
FCODE uint16  divu16( uint16 b, uint16 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    uint16 a;

    /* An integer division, rounding towards zero, is performed on the
     * underlying stored values, this intermediate result can be considered to
     * have a binary point equal to the difference between the binary points of
     *  b and c. The result is undefined if c==0.
     */

    a = b / c;

    /* The intermediate result is cast to the same size as a, with saturation
       - a no-op here */

    /* Finally the result is shifted to the binary point of a with
     * saturation. */

    a = shift_u16( a, (binpt_t)(bpb-bpc), bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE sint16  divs16( sint16 b, sint16 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    sint16 a;

    /* An integer division, rounding towards zero, is performed on the
     * underlying stored values, this intermediate result can be considered to
     * have a binary point equal to the difference between the binary points of
     *  b and c. The result is undefined if c==0.
     */

    /* There's one potential overflow case here (assuming c != 0):
     * b = -(2^15), c = -1 => result = (2^15)
     */
    if(( b == SINT16_MIN ) && ( c == -1 )) {
	a = SINT16_MAX;
    } else {
	a = b / c;
    }

    /* The intermediate result is cast to the same size as a, with saturation
       - a no-op here */

    /* Finally the result is shifted to the binary point of a with
     * saturation. */

    a = shift_s16( a, (binpt_t)(bpb-bpc), bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE uint8  ndivu16( uint16 b, uint16 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
   uint8 a;
   uint16 a_tmp;

    /* An integer division, rounding towards zero, is performed on the
     * underlying stored values, this intermediate result can be considered to
     * have a binary point equal to the difference between the binary points of
     *  b and c. The result is undefined if c==0.
     */

   a_tmp = b/c;

   /* The intermediate result is cast to the same size as a, with saturation */
   if( a_tmp > UINT8_MAX ) {
       a = UINT8_MAX;
   } else {
       a = (uint8)a_tmp;
   }

   /* Finally the result is shifted to the binary point of a with
    * saturation. */

    a = shift_u8( a, (binpt_t)(bpb-bpc), bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE sint8  ndivs16( sint16 b, sint16 c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
   sint8 a;
   sint16 a_tmp;

    /* An integer division, rounding towards zero, is performed on the
     * underlying stored values, this intermediate result can be considered to
     * have a binary point equal to the difference between the binary points of
     *  b and c. The result is undefined if c==0.
     */

   /* There's one potential overflow case for the division (assuming c != 0):
    * b = -(2^15), c = -1 => result = (2^15).
    *
    * Plenty of other cases will overflow the cast down to sint8, but they're
    * much more easily checked for after the division. We don't incur a
    * performance penalty by doing a wider divide than truly necessary, because
    * the HC(S)12's narrowest divide is IDIV, which is 16 by 16.
    */
   if(( b == SINT16_MIN ) && ( c == -1 )) {
       a = SINT8_MAX;
   } else {
       a_tmp = b/c;

       /* The intermediate result is cast to the same size as a, with saturation */
       if( a_tmp > SINT8_MAX ) {
	   a = SINT8_MAX;
       } else if( a_tmp <= SINT8_MIN ) {
	   a = SINT8_MIN;
       } else {
	   a = (sint8)a_tmp;
       }
   }

   /* Finally the result is shifted to the binary point of a with
    * saturation. */

    a = shift_s8( a, (binpt_t)(bpb-bpc), bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE uint8   divu8 ( uint8  b, uint8  c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    uint8 a;

    /* An integer division, rounding towards zero, is performed on the
     * underlying stored values, this intermediate result can be considered to
     * have a binary point equal to the difference between the binary points of
     *  b and c. The result is undefined if c==0.
     */

    a = b / c;

    /* The intermediate result is cast to the same size as a, with saturation
       - a no-op here */

    /* Finally the result is shifted to the binary point of a with
     * saturation. */

    a = shift_u8( a, (binpt_t)(bpb-bpc), bpa );
    return a;
}

/**
 * documentation with prototype in header file
*/
FCODE sint8   divs8 ( sint8  b, sint8  c, binpt_t bpb, binpt_t bpc, binpt_t bpa)
{
    sint8 a;

    /* An integer division, rounding towards zero, is performed on the
     * underlying stored values, this intermediate result can be considered to
     * have a binary point equal to the difference between the binary points of
     *  b and c. The result is undefined if c==0.
     */

    /* There's one potential overflow case here (assuming c != 0):
     * b = -(2^7), c = -1 => result = (2^7)
     */
    if(( b == SINT8_MIN ) && ( c == -1 )) {
	a = SINT8_MAX;
    } else {
	a = b / c;
    }

    /* The intermediate result is cast to the same size as a, with saturation
       - a no-op here */

    /* Finally the result is shifted to the binary point of a with
     * saturation. */

    a = shift_s8( a, (binpt_t)(bpb-bpc), bpa );
    return a;
}
