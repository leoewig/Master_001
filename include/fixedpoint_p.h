/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  fixedpoint_p.h
 *
 * description:  
 *  fixedpoint_p
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
 * @file
 * @ingroup fixedpoint
 */

/**
 unsigned 64-bit integers for our private use. We don't allow them to
 * escape the fixedpoint library.
 */
typedef struct {
    uint32_t high, low;
} uint64;

/**
 * signed 64-bit integers for our private use. We don't allow them to
 * escape the fixedpoint library.
 */
typedef struct {
    int32_t high, low;
} sint64;

/**
 * fixedpoint_u16_overflow[n] is a mask of bits which will be lost when we
 * shift a 16-bit word left by n bits, but it's used by the 8- and 32-bit shift
 * routines too, as well as the 32-bit multiplies.
 *
 * it's defined for 0 <= n <= 16.
*/
#define FP_U16_OVERFLOW_SIZE 17
extern const uint16_t fixedpoint_u16_overflow[FP_U16_OVERFLOW_SIZE];

/**
 * @defgroup lsr Logical Shift Right
 * @ingroup fixedpoint
 * @{
 * logical shift right, with range check on the shift
 *
 * NB that, although lsr_* purport to have range checks, these are only
 * maximum checks - if you pass them a negative shift, anything could happen.
 */
FCODE uint32_t lsr_u32(uint32_t input, binpt_t shift );
FCODE uint16_t lsr_u16(uint16_t input, binpt_t shift );
FCODE uint8_t  lsr_u8 (uint8_t  input, binpt_t shift );
/**
 * @}
 */

/**
 * @defgroup asr Arithmetic Shift Right
 * @ingroup fixedpoint
 * @{
 * arithmetic shift right with range check on the shift
 *
 * NB that, although asr_* purport to have range checks, these are only
 * maximum checks - if you pass them a negative shift, anything could happen.
 */
FCODE int32_t asr_s32( int32_t input, binpt_t shift );
FCODE int16_t asr_s16( int16_t input, binpt_t shift );
FCODE int8_t  asr_s8 ( int8_t  input, binpt_t shift );
/**
 * @}
 */

/**
 * @defgroup shl Saturating Shift Left
 * @ingroup fixedpoint
 * @{
 * saturating left shift with range check on the shift
 *
 * NB that, although shl_* purport to have range checks, these are only
 * maximum checks - if you pass them a negative shift, anything could happen.
 */
FCODE  int32_t shl_s32( int32_t input, binpt_t shift );
FCODE uint32_t shl_u32(uint32_t input, binpt_t shift );
FCODE  int16_t shl_s16( int16_t input, binpt_t shift );
FCODE uint16_t shl_u16(uint16_t input, binpt_t shift );
FCODE  int8_t  shl_s8 ( int8_t  input, binpt_t shift );
FCODE uint8_t  shl_u8 (uint8_t  input, binpt_t shift );
/**
 * @}
 */
