/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    utils_types.h
 *
 * description:  Public header for Utils
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Public header for Utils
 *                       
 *
 *****************************************************************************/
#ifndef UTILS_TYPES_H
#define UTILS_TYPES_H

/*************************************************************************//**
 *
 *
 *****************************************************************************/

 /**
 * A place to put useful macros
 *
 * @defgroup macros Useful Macros
 * @ingroup utils
 * @{
 * @file
 * @}
 */


/**************************************************************************//**
 *
 *  Check that a value is within a specified tolerance of another value
 *
 *
 @todo Add support for fixed point
 *****************************************************************************/
#define WITHIN_TOLERANCE(actual, target, tol) \
          (((int32_t)(actual) <= ((int32_t)(target) + (int32_t)(tol))) && ((int32_t)(actual) >= ((int32_t)(target) - (int32_t)(tol))))

/**
 * See if a value is above an allowed upper limit with a given tolerance
 *
 *  Works for 8 and 16 bit (signed or unsigned) types only.  Fixed point
 *  numbers can be used if all arguments have the same binary point.
 */
#define ABOVE_UPPER_LIMIT(value, upper_limit, tol) \
          ((int32_t)(value) > ((int32_t)(upper_limit) + (int32_t)(tol)))
/**
 * See if a value is below an allowed lower limit with a given tolerance
 *
 *  Works for 8 and 16 bit (signed or unsigned) types only.  Fixed point
 *  numbers can be used if all arguments have the same binary point.
 */
#define BELOW_LOWER_LIMIT(value, lower_limit, tol) \
          ((int32_t)(value) < ((int32_t)(lower_limit) - (int32_t)(tol)))

/******************************************************************************
 * MISRA disallows the mixing of signed and unsigned numbers, requiring a cast
 * however in a preprocessor expression casts aren't available so the result is
 * often a cut an paste of expressions and numbers to make unsigned versions.
 *****************************************************************************/

 /**
  * Support macro for UCONST()
  * @misra Rule 19.13 DeviationID:3
  **/
#define UCONST2(c) c ## u

 /** A macro that can be used to append a 'u' suffix to a signed number, so that
  * pp constants can be mixed without needing to violate MISRA rules on type
  * conversion but also without being defined twice. */
#define UCONST(c) UCONST2(c)

 /**
  * Support macro for LCONST()
  * @misra Rule 19.13 DeviationID:3
  **/
#define LCONST2(c) c ## L

 /** A macro that can be used to append a 'L' suffix to a signed number, so that
  * pp constants can be mixed without needing to violate MISRA rules on type
  * conversion but also without being defined twice. */
#define LCONST(c) LCONST2(c)

/**
  * Support macro for ULCONST()
  * @misra Rule 19.13 DeviationID:3
  **/
#define ULCONST2(c) c ## uL

 /** A macro that can be used to append a 'uL' suffix to a signed number, so that
  * pp constants can be mixed without needing to violate MISRA rules on type
  * conversion but also without being defined twice. */
#define ULCONST(c) ULCONST2(c)

/******************************************************************************
 *****************************************************************************/

 /**
 * Flip the bits in a variable and cast back down to the underlying type*/
#define INV_MASK(type,value) ( (type)~(value))

/**
 * Flip the bits in a variable and cast back down to a uint8_t*/
#define INV_U8(value)  INV_MASK(uint8_t,(value))
/**
 * Flip the bits in a variable and cast back down to a uint16_t*/
#define INV_U16(value) INV_MASK(uint16_t,(value))
/**
 * Flip the bits in a variable and cast back down to a uint32_t*/
#define INV_U32(value) INV_MASK(uint32_t,(value))

#define SET_MASK( var,type,value )    do { (var) |= (value); } while (0)

#define CLEAR_MASK( var,type,value )  do { (var) &= (type)~(value); } while (0)

#define SET_MASK8( var,value )          SET_MASK(var, uint8_t,value)
#define SET_MASK16( var,value )         SET_MASK(var,uint16_t,value)
#define SET_MASK32( var,value )         SET_MASK(var,uint32_t,value)

#define CLEAR_MASK8( var,value )        CLEAR_MASK(var, uint8_t,value)
#define CLEAR_MASK16( var,value )       CLEAR_MASK(var,uint16_t,value)
#define CLEAR_MASK32( var,value )       CLEAR_MASK(var,uint32_t,value)

/**
 * @lint -e940 Not a deviation - MISRA Rule 9.2 allows the use of a single 0
 * for the first element */
/*lint -emacro(940,ZERO_ARRAY) */

/**
 * Claimed rule Rule 10.1 deviation, it's not obvious that this is though,
 * given the way {0} works.
 * Prohibited Implicit Conversion: Signed versus Unsigned */

/*lint -emacro(960,ZERO_ARRAY) */

/**
 * Blank array initialisation
 * Lint complains about {0} (even though Lint permits it) so wrap the use in a
 * macro
 */
#define ZERO_ARRAY {0}

#endif /* UTILS_TYPES_H*/
