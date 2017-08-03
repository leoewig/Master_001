/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  fixedpoint_p.h
 *
 * description:  
 *  Public header for fixed point utilities
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
 * Public header for fixed point utilities
 *
 *
 * @file
 * @ingroup fixedpoint
 *
 * @defgroup fixedpoint Fixed-Point Utilities
 * @ingroup utils
 * @{
 * @file
 * @}
 */

#if !defined (FIXEDPOINT_H)
#define       FIXEDPOINT_H

#include "basic_types.h"
#include "eq_types.h"
#include "fixedpoint_types.h"

/*
 * Public functions
 */

/**
 * @defgroup absolute Absolute Values
 * @ingroup fixedpoint
 * @{
 * absolute value
 *
 * a = abs(b) \n
 *
 * Algorithm: \n
 * <ol>
 *   <li>if b<0, return -b \n
 *   <li>otherwise return b \n
 * </ol>
 *
 * Notes: \n
 * <ol>
 * <li>if you wanted a signed result, you'll have to use u_to_snn() (which will
 *     saturate if b==SINTnn_MIN).
 * </ol>
 */
/**
 * @ingroup absolute
 */
/**
 * int32 - absolute value
 */
FCODE uint32_t abs32( int32_t b );

/**
 * @ingroup absolute
 */
/**
 * int16 - absolute value
 */
FCODE uint16_t abs16( int16_t b );

/**
 * @ingroup absolute
 */
/**
 * int8 - absolute value
 */
FCODE uint8_t  abs8 ( int8_t  b );
/**
 * @}
 */

/**
 * @defgroup addition Addition
 * @ingroup fixedpoint
 * @{
 * addition
 *
 * a = b+c \n
 *
 * bpb, bpc, bpa are the binary points of b, c and the result respectively. \n
 *
 * Algorithm: \n
 * <ol>
 *   <li>if b and c have different binary points, the more precise is shifted to
 *   the precision of the other
 *   <li>the addition is performed with saturation (i.e. overflows are trapped
 *   and the result replaced with the most positive or negative result as
 *   appropriate)
 *   <li>the result is shifted to the binary point of a with saturation as
 *   appropriate
 * </ol>
 *
 * Notes: \n
 * <ol>
 *   <li>saturation before the final shift can give an answer that appears
 *     wrong. This should have been considered at the design stage.
 *   <li>Use the function with the return type which matches that of a.
 *   <li>C's usual arithmetic conversions will implicitly widen the types of b
 *     and c if necessary.
 * </ol>
 * @}
 */
/**
 * @ingroup addition
 */
/**
 * uint32 - addition
 */
FCODE uint32_t addu32(uint32_t b, uint32_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup addition
 */
/**
 * int32 - addition
 */
FCODE  int32_t adds32( int32_t b,  int32_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup addition
 */
/**
 * uint16 - addition
 */
FCODE uint16_t addu16(uint16_t b, uint16_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup addition
 */
/**
 * int16 - addition
 */
FCODE  int16_t adds16( int16_t b,  int16_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup addition
 */
/**
 * uint8 - addition
 */
FCODE uint8_t  addu8 (uint8_t  b, uint8_t  c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup addition
 */
/**
 * int8 - addition
 */
FCODE  int8_t  adds8 ( int8_t  b,  int8_t  c, binpt_t bpb, binpt_t bpc, binpt_t bpa);
/**
 * @}
 */

/**
 * @defgroup subtraction Subtraction
 * @ingroup fixedpoint
 * @{
 * subtraction
 *
 * a = b-c
 *
 * bpb, bpc, bpa are the binary points of b, c and the result respectively.
 *
 * Algorithm: \n
 * <ol>
 *   <li>if b and c have different binary points, the more precise is shifted to
 *     the precision of the other
 *   <li>the subtraction is performed with saturation (i.e. overflows are trapped
 *     and the result replaced with the most positive or negative result as
 *     appropriate)
 *   <li>the result is shifted to the binary point of a with saturation as
 *     appropriate
 * </ol>
 *
 * Notes:
 * <ol>
 *   <li>saturation before the final shift can give an answer that appears
 *     wrong. This should have been considered at the design stage.
 *   <li>Use the function with the return type which matches that of a.
 *   <li>C's usual arithmetic conversions will implicitly widen the types of b
 *     and c if necessary.
 * </ol>
 * @}
 */
/**
 * @ingroup subtraction
 */
/**
 * uint32 - subtraction
 */
FCODE uint32_t subu32(uint32_t b, uint32_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup subtraction
 */
/**
 * int32 - subtraction
 */
FCODE  int32_t subs32( int32_t b,  int32_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup subtraction
 */
/**
 * uint16 - subtraction
 */
FCODE uint16_t subu16(uint16_t b, uint16_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup subtraction
 */
/**
 * int16 - subtraction
 */
FCODE  int16_t subs16( int16_t b,  int16_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup subtraction
 */
/**
 * uint8- subtraction
 */
FCODE uint8_t  subu8 (uint8_t  b, uint8_t c,  binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup subtraction
 */
/**
 * int8 - subtraction
 */
FCODE  int8_t  subs8 ( int8_t  b,  int8_t c,  binpt_t bpb, binpt_t bpc, binpt_t bpa);
/**
 * @}
 */

/**
 * @defgroup multiplication Multiplication
 * @ingroup fixedpoint
 * @{
 * multiplication
 *
 * a = b * c
 *
 * bpb, bpc, bpa are the binary points of b, c and the result respectively.
 *
 * Algorithm \n
 * <ol>
 *   <li>a widening multiplication is carried out to give an
 *     intermediate result. The intermediate result has a binary point equal to
 *     the sum of those of b and c
 *   <li>shift the intermediate result to the binary point of a, with saturation
 *   <li>cast the intermediate result to the width of a, with saturation.
 * </ol>
 *
 * Notes:
 * <ol>
 *   <li>Shifting before casting may give an answer that appears wrong if two
 *     uint8_ts or int8_ts are multiplied to give a uint32_t or int32_t. An explicit
 *     widening cast block can be used to acheive the correct result in this
 *     case.
 *   <li>Where a has at least twice as many bits as the longest of b or c, use
 *     the widening multiply function wmulxx with the same return type as that
 *     of a. This will be more efficient than the equivalent mulxx function.
 *   <li>Otherwise, use the function with the return type which matches that of
 *     a.
 *   <li>C's usual arithmetic conversions will implicitly widen the types of b
 *     and c if necessary.
 * </ol>
 * @}
 */
/**
 * @ingroup multiplication
 */
/**
 * uint32 - multiplication
 */
FCODE uint32_t  mulu32(uint32_t b, uint32_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup multiplication
 */
/**
 * int32 - multiplication
 */
FCODE  int32_t  muls32( int32_t b,  int32_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup multiplication
 */
/**
 * widening uint16 - multiplication
 */
FCODE uint32_t wmulu32(uint16_t b, uint16_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup multiplication
 */
/**
 * widening int16 - multiplication
 */
FCODE  int32_t wmuls32( int16_t b,  int16_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup multiplication
 */
/**
 * uint16 - multiplication
 */
FCODE uint16_t  mulu16(uint16_t b, uint16_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup multiplication
 */
/**
 * int16 - multiplication
 */
FCODE  int16_t  muls16( int16_t b,  int16_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup multiplication
 */
/**
 * widening uint8 - multiplication
 */
FCODE uint16_t wmulu16(uint8_t  b, uint8_t  c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup multiplication
 */
/**
 * widening int 8 - multiplication
 */
FCODE  int16_t wmuls16( int8_t  b,  int8_t  c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup multiplication
 */
/**
 * uint8 - multiplication
 */
FCODE uint8_t   mulu8 (uint8_t  b, uint8_t  c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup multiplication
 */
/**
 * int 8 -multiplication
 */
FCODE  int8_t   muls8 ( int8_t  b,  int8_t  c, binpt_t bpb, binpt_t bpc, binpt_t bpa);
/**
 * @}
 */

/**
 * @defgroup division Division
 * @ingroup fixedpoint
 * @{
 * division
 *
 * a = b / c
 *
 * bpb, bpc, bpa are the binary points of b, c and the result respectively.
 *
 * Algorithm \n
 * <ol>
 *   <li>An integer division, rounding towards zero, is performed on the
 *     underlying stored values, this intermediate result can be considered to
 *     have a binary point equal to the difference between the binary points of
 *     b and c. The result is undefined if c==0.
 *   <li>The intermediate result is cast to the same size as a, with saturation.
 *   <li>Finally the result is shifted to the binary point of a with saturation.
 * </ol>
 *
 * Notes
 * <ol>
 *   <li>saturation before the final shift can give an answer that appears
 *     wrong. This should have been considered at the design stage.
 *
 *   <li>If b has at least twice as many bits as a, use the narrowing division
 *     function ndivxx where xx is the type of b. This will be more efficient
 *     than the equivalent non-narrowing function divxx and avoids a narrowing
 *     cast on the return value.
 *
 *     ! Division is the only operator where we care more about the size of the
 *       input than the output.
 *
 *   <li>Otherwise, use the non-narrowing function divxx, where xx is the type of
 *     b.
 *
 *   <li>C's usual arithmetic conversions will implicitly widen the type of c if
 *     necessary.
 * </ol>
 * @}
 */
/**
 * @ingroup division
 */
/**
 * uint32 - division
 */
FCODE uint32_t  divu32( uint32_t b, uint32_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup division
 */
/**
 * int32 - division
 */
FCODE  int32_t  divs32(  int32_t b,  int32_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup division
 */
/**
 * narrowing uint32 - division
 */
FCODE uint16_t ndivu32( uint32_t b, uint16_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup division
 */
/**
 * narrowing int32 - division
 */
FCODE  int16_t ndivs32(  int32_t b,  int16_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup division
 */
/**
 * uint16 - division
 */
FCODE uint16_t  divu16( uint16_t b, uint16_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup division
 */
/**
 * int16 - division
 */
FCODE  int16_t  divs16(  int16_t b,  int16_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup division
 */
/**
 * narrowing uint16 - division
 */
FCODE uint8_t  ndivu16( uint16_t b, uint16_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup division
 */
/**
 * narrowing int16 - division
 */
FCODE  int8_t  ndivs16(  int16_t b,  int16_t c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup division
 */
/**
 * uint8 - division
 */
FCODE uint8_t   divu8 ( uint8_t  b, uint8_t  c, binpt_t bpb, binpt_t bpc, binpt_t bpa);

/**
 * @ingroup division
 */
/**
 * int8 - division
 */
FCODE  int8_t   divs8 (  int8_t  b,  int8_t  c, binpt_t bpb, binpt_t bpc, binpt_t bpa);
/**
 * @}
 */

/**
 * @defgroup shifting Simple Shifts
 * @ingroup fixedpoint
 * @{
 * Simple shifts
 * @}
 */
/**
 * @ingroup shifting
 */
/**
 * uint32 - Simple shift
 */
FCODE uint32_t shift_u32(uint32_t b, binpt_t bpb, binpt_t bpa);

/**
 * @ingroup shifting
 */
/**
 * int32 - Simple shift
 */
FCODE  int32_t shift_s32( int32_t b, binpt_t bpb, binpt_t bpa);

/**
 * @ingroup shifting
 */
/**
 * uint16 - Simple shift
 */
FCODE uint16_t shift_u16(uint16_t b, binpt_t bpb, binpt_t bpa);

/**
 * @ingroup shifting
 */
/**
 * int16 - Simple shift
 */
FCODE  int16_t shift_s16( int16_t b, binpt_t bpb, binpt_t bpa);

/**
 * @ingroup shifting
 */
/**
 * uint8 - Simple shift
 */
FCODE uint8_t  shift_u8 (uint8_t  b, binpt_t bpb, binpt_t bpa);

/**
 * @ingroup shifting
 */
/**
 * int 8 - Simple shift
 */
FCODE  int8_t  shift_s8 ( int8_t  b, binpt_t bpb, binpt_t bpa);
/**
 * @}
 */

/**
 * @defgroup signconversion Sign Conversion
 * @ingroup fixedpoint
 * @{
 * Sign conversion
 *
 * Algorithm \n
 * <ol>
 *   <li>If changing from signed to unsigned:
 *     <ol>
 *      <li>b is cast to the same integer type as a
 *      <li>b is shifted to the same binary point as a
 *     </ol>
 *
 *   <li>If changing from unsigned to signed:
 *     <ol>
 *      <li>b is shifted to the same binary point as a
 *      <li>b is cast to the same integer type as a
 *     </ol>
 * </ol>
 * @}
 */
/**
 * @ingroup signconversion
 */
/**
 * int32 - Sign conversion
 */
FCODE uint32_t s_to_u32( int32_t b, binpt_t bpb, binpt_t bpa);

/**
 * @ingroup signconversion
 */
/**
 * uint32 - Sign conversion
 */
FCODE  int32_t u_to_s32(uint32_t b, binpt_t bpb, binpt_t bpa);

/**
 * @ingroup signconversion
 */
/**
 * int16 - Sign conversion
 */
FCODE uint16_t s_to_u16( int16_t b, binpt_t bpb, binpt_t bpa);

/**
 * @ingroup signconversion
 */
/**
 * uint16 - Sign conversion
 */
FCODE  int16_t u_to_s16(uint16_t b, binpt_t bpb, binpt_t bpa);

/**
 * @ingroup signconversion
 */
/**
 * int8 - Sign conversion
 */
FCODE uint8_t s_to_u8  ( int8_t b,  binpt_t bpb, binpt_t bpa);

/**
 * @ingroup signconversion
 */
/**
 * uint8 - Sign conversion
 */
FCODE  int8_t u_to_s8  (uint8_t b,  binpt_t bpb, binpt_t bpa);
/**
 * @}
 */

/**
 * @defgroup narrowing Type Narrowing
 * @ingroup fixedpoint
 * @{
 * Narrowing cast
 *
 * Algorithm \n
 * <ol>
 *   <li>b is shifted to the same binary point as a
 *   <li>b is cast to the same integer type as a
 * </ol>
 * @}
 */
/**
 * @ingroup narrowing
 */
/**
 * uint32 - Narrowing cast
 */
FCODE uint16_t narrow_u16(uint32_t b, binpt_t bpb, binpt_t bpa);

/**
 * @ingroup narrowing
 */
/**
 * int32 - Narrowing cast
 */
FCODE  int16_t narrow_s16( int32_t b, binpt_t bpb, binpt_t bpa);

/**
 * @ingroup narrowing
 */
/**
 * uint16 - Narrowing cast
 */
FCODE uint8_t  narrow_u8 (uint16_t b, binpt_t bpb, binpt_t bpa);

/**
 * @ingroup narrowing
 */
/**
 * int16 - Narrowing cast
 */
FCODE  int8_t  narrow_s8 ( int16_t b, binpt_t bpb, binpt_t bpa);
/**
 * @}
 */

/**
 * @defgroup maximum Maximum
 * @ingroup fixedpoint
 * @{
 * Maximum
 * @}
 */
/**
 * @ingroup maximum
 */
/**
 * uint32 - Maximum
 */
FCODE uint32_t maxu32(uint32_t b, uint32_t c);

/**
 * @ingroup maximum
 */
/**
 * int32 - Maximum
 */
FCODE  int32_t maxs32( int32_t b,  int32_t c);

/**
 * @ingroup maximum
 */
/**
 * uint16 - Maximum
 */
FCODE uint16_t maxu16(uint16_t b, uint16_t c);

/**
 * @ingroup maximum
 */
/**
 * int16 - Maximum
 */
FCODE  int16_t maxs16( int16_t b,  int16_t c);

/**
 * @ingroup maximum
 */
/**
 * uint8 - Maximum
 */
FCODE uint8_t  maxu8 (uint8_t  b, uint8_t  c);

/**
 * @ingroup maximum
 */
/**
 * int8 - Maximum
 */
FCODE  int8_t  maxs8 ( int8_t  b,  int8_t  c);
/**
 * @}
 */

/**
 * @defgroup minimum Minimum
 * @ingroup fixedpoint
 * @{
 * Minimum
 * @}
 */
/**
 * @ingroup minimum
 */
/**
 * unit32 - Minimum
 */
FCODE uint32_t minu32( uint32_t b, uint32_t c);

/**
 * @ingroup minimum
 */
/**
 * int32 - Minimum
 */
FCODE  int32_t mins32(  int32_t b,  int32_t c);

/**
 * @ingroup minimum
 */
/**
 * uint16 - Minimum
 */
FCODE uint16_t minu16( uint16_t b, uint16_t c);

/**
 * @ingroup minimum
 */
/**
 * int16 - Minimum
 */
FCODE  int16_t mins16(  int16_t b,  int16_t c);

/**
 * @ingroup minimum
 */
/**
 * uint8 - Minimum
 */
FCODE uint8_t  minu8 ( uint8_t  b, uint8_t  c);

/**
 * @ingroup minimum
 */
/**
 * int8 - Minimum
 */
FCODE  int8_t  mins8 (  int8_t  b,  int8_t  c);
/**
 * @}
 */

/**
 * @defgroup clipping Clip
 * @ingroup fixedpoint
 * @{
 * Clip
 * @}
 */
/**
 * @ingroup clipping
 */
/**
 * uint32 - Clip
 */
FCODE uint32_t clipu32(uint32_t b, uint32_t lower, uint32_t upper);

/**
 * @ingroup clipping
 */
/**
 * int32 - Clip
 */
FCODE  int32_t clips32( int32_t b,  int32_t lower,  int32_t upper);

/**
 * @ingroup clipping
 */
/**
 * uint16 - Clip
 */
FCODE uint16_t clipu16(uint16_t b, uint16_t lower, uint16_t upper);

/**
 * @ingroup clipping
 */
/**
 * int16 - Clip
 */
FCODE  int16_t clips16( int16_t b,  int16_t lower,  int16_t upper);

/**
 * @ingroup clipping
 */
/**
 * uint8 - Clip
 */
FCODE uint8_t  clipu8 (uint8_t  b, uint8_t  lower, uint8_t  upper);

/**
 * @ingroup clipping
 */
/**
 * int8 - Clip
 */
FCODE  int8_t  clips8 ( int8_t  b,  int8_t  lower,  int8_t  upper);
/**
 * @}
 */
#endif /* FIXEDPOINT_H */
