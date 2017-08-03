/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   fixedpoint_types.h
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
#if !defined (FIXEDPOINT_TYPES_H)
#define       FIXEDPOINT_TYPES_H
/**
 * Public header for fixed point utilities
 *
 *
 * Copyright (c) Linkedriving Tech 2012 - All rights reserved
 *
 * @file
 * @ingroup fixedpoint
 *
 * @ingroup utils
 * @{
 * @file
 * @}
 */

/**
 * binary point
 */
typedef int16_t binpt_t;

/**
 * The maximum representable by the binpt_t; note that this is different to the
 * range over which the fixedpoint routines are guaranteed to give correct
 * operation, which is defined to be [-128,127] by [Fixedpoint].
*/
#define BINPT_MAX (INT16_MAX)

/**
 * The minimum representable by the binpt_t; note that this is different to the
 * range over which the fixedpoint routines are guaranteed to give correct
 * operation, which is defined to be [-128,127] by [Fixedpoint].
*/
#define BINPT_MIN (INT16_MIN)

/*
 * Utility macros
 */

/**
 * @lint
 * This is a trusted macro and we do not want to change the original code, so
 * switch off all lint warnings/errors
 * -emacro((*),BP_TO_ACCURACY)
 */
/*lint -emacro((*),BP_TO_ACCURACY) */
/**
 * Convert binary point to accuracy. Returns a double (see deviation 192).
 * e.g. foo can take values of 0.25 [N m], 0.5, 0.75, etc.
 * It has a binary point of 2 and an accuracy of 0.25.
 * (and a resolution of 4 in cal.def terms)
 *
 * Only works on binary points between -63 and 63.
 *
 * NB this may produce coverage issues?
*/
#ifndef HOSTED
#define BP_TO_ACCURACY(bp) \
    ( \
     ((bp)<-31)  ? ((1.0/((double)(1UL<<(-32-(bp)))))/4294967296.0) : \
     (((bp)<0)    ? (1.0/((double)(1UL<<(-(bp))))) : \
     (((bp)<32)   ? ((double)(1UL<<((bp)))) : \
                    (((double)(1UL<<((bp)-32)))*4294967296.0))))
#else
/* Visual Studio complains about potential divides by zero in unreachable
   parts of the macro when binary points are negative or enormous.  We don't
   think we will need them, so we'll take them out of the HOSTED expansion.
   Ideally, we'd put an INTERNAL_ERROR trap but we can't because the macro
   must be evaluatable by the preprocessor for parameter intiialisation.
   */
#define BP_TO_ACCURACY(bp) \
    ((double)(1UL<<((bp))))
#endif

/**
 * Scale a floating-point value ready to assign to an integer of a given binary
 * point. Returns a value of type "type".
 *
 * Notes:
 * <ol>
 *  <li>val must be a "double": even if you want to assign an integer value, you
 *    must do (eg)
 *
 *        BP_SCALE( 100.0, uint16_t, -2 )
 *
 * <li>Uses BP_TO_ACCURACY, so will only work on binary points between +/- 63.
 *
 * Example: suppose you want to set a uint16_t, bin 7 value to "200.16":
 *
 *   uint16_t foo;
 *
 *   \#define BP_FOO ((binpt_t)7)
 *
 *   foo = BP_SCALE( 200.16, uint16_t, 7 );
 *
 * <li>that by requiring the type to be passed as an argument, we save forcing
 * the user to make an explicit cast.
 * </ol>
 */
#define BP_SCALE(val,type,bp) ((type)((val)*BP_TO_ACCURACY(bp)))

#endif /* FIXEDPOINT_TYPES_H */
