/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    basic_types.
 *
 * description:  
 * Defines the fundamental types and type qualifiers
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
#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H
/**
 *
 * @file
 * @ingroup utils
 *
 * Defines the fundamental types and type qualifiers
 *
 * $Id: basic_types.h 11339 2013-11-18 16:40:08Z pr4 $
 */
/* Copyright (c) Linkedriving Tech 2015 - All rights resevered */

/** @polyspace:begin<MISRA-C:19.4:Not a defect:Justify with annotations> Errors in Cosmic supplied files, so ignore */

#include <limits.h>
#include <stddef.h> /* include stddef to define NULL */

/**
 * Definition of types used instead of native C types
 *
 * @defgroup types Data types
 * @ingroup utils
 * @{
 */
/*
 * Check data sizes are what we expect and throw an error if we get a surprise
 */
/** @polyspace:begin<MISRA-C:10.6:Not a defect:No action planned> Error is in Cosmic supplied header file */
#if (UCHAR_MAX != 0xFFU) || ( (UINT_MAX != 0xFFFFU) && (UINT_MAX != 0xFFFFFFFFU)) || (ULONG_MAX != 0xFFFFFFFFUL)
/*
 * if you get this when porting, split the above expression to find where
 * the problem lies
 */
#error Type size mismatch
#endif
/** @polyspace:end<MISRA-C:10.6:Not a defect:No action planned> Error is in Cosmic supplied header file */

typedef unsigned char  uint8_t;          /**< unsigned byte */
#define UINT8_MAX (uint8_t) UCHAR_MAX    /**< max value storable in a #uint8_t */

#if defined(HOSTED)
/* The PC-hosted unit test target has a 32-bit "int".*/
typedef unsigned  short uint16_t;
#define UINT16_MAX (uint16_t) USHRT_MAX
#else   /* defined(HOSTED) */
/* For all other targets, uint16_t is an int */
typedef unsigned int uint16_t;           /**< unsigned 16-bit word */
#define UINT16_MAX (uint16_t) UINT_MAX   /**< max value storable in a #uint16_t */
#endif  /* defined(HOSTED) */

typedef unsigned long  uint32_t;         /**< unsigned 32-bit double-word */
#define UINT32_MAX (uint32_t) ULONG_MAX  /**< max value storable in a #uint32_t */

typedef signed char    int8_t;           /**< signed byte */
#define INT8_MAX SCHAR_MAX               /**< max value storable in a #int8_t */
#define INT8_MIN SCHAR_MIN               /**< min value storable in a #int8_t */

#if defined(HOSTED)
/* The PC-hosted unit test target has a 32-bit int.  Lint can have it's int type
 * set to 16 bit so it can keep the same definition as the target */
typedef short  int16_t;                   /**< signed 16-bit word */
#define INT16_MAX SHRT_MAX                /**< max value storable in a #int16_t */
#define INT16_MIN SHRT_MIN                /**< min value storable in a #int16_t */
#else   /* defined(HOSTED) */
typedef int   int16_t;                   /**< signed 16-bit word */
#define INT16_MAX INT_MAX                /**< max value storable in a #int16_t */
#define INT16_MIN INT_MIN                /**< min value storable in a #int16_t */
#endif  /* defined(HOSTED) */

typedef long    int32_t;                 /**< signed 32-bit double-word */
#define INT32_MAX LONG_MAX               /**< max value storable in a #int32_t */
#define INT32_MIN LONG_MIN               /**< min value storable in a #int32_t */

typedef uint8_t bool_t;                  /**< type for booleans */
#ifndef TRUE
#define TRUE  ((bool_t)1u)
#endif
#ifndef FALSE
#define FALSE ((bool_t)0u)
#endif

/* Float type. */
typedef float float32_t;

#define FNULL   (FPTR void *)0

/**
 * Corrigendum 1 from July 17 amends rule 6.3 so that plain char is allowed.
 * So use of plain char is not a deviation. However lint still complains. */
typedef char char_t;

/**
 * @}
 */

/**
 * @defgroup parsers Parser support
 * @ingroup utils
 *
 * Re-definition of function and memory location qualifiers for each code parser
 *
 * Qualifiers have to cope with all of the following:
 * <ul>
 * <li> Code _intended_ for S12 or XGATE
 * <li> Code parsed by any of the following:
 *      <ol>
 *      <li> Cosmic S12 compiler
 *      <li> Cosmic XGATE compiler
 *      <li> Microsoft Developer Studio (for hosted testing)
 *      <li> lint
 *      <li> Polyspace
 *      </ol>
 * </ul>
 *
 * Definitions used to distinguish them all:
 * <pre>
 * Intended target:
 *  XGATE:      \#ifdef _XGATE_             (defined by us)
 *  S12:        the default
 * Parser:
 *  Cosmic S12  \#ifdef __CSMC__        (defined by compiler)
 *  Cosmic XGATE    \#if defined(__CSMC__) && defined(_XGATE_)
 *  PC compiler \#ifdef HOSTED              (defined by us)
 *  lint        \#ifdef _lint               (defined by PC-lint)
 *  polyspace   \#ifdef POLYSPACE           (defined by polyspace?)
 *
 *
 * Qualifier             Parser                 Intended target  Expands to
 *---------------------+-----------------------+---------------+------------+
 * INLINE                Cosmic                  both            \@inline
 *                       PC,lint                 both            __inline
 *                       polyspace               both            -nothing-
 *---------------------+-----------------------+---------------+------------+
 * INTERRUPT             Cosmic                  S12 \@interrupt \@svpage \@near
 *                       Cosmic                  XGATE          \@interrupt
 *                       PC,lint,polyspace       both            -nothing-
 *---------------------+-----------------------+---------------+------------+
 * RAW_INTERRUPT         as for INTERRUPT for all cases except Cosmic S12
 *                       for which we remove the \@svpage
 *---------------------+-----------------------+---------------+------------+
 * NCODE                 Cosmic                  S12             \@near
 *                       Cosmic                  XGATE           -undefined-
 *                       PC,polyspace            S12             -nothing-
 *                       PC,polyspace            XGATE           -undefined-
 *                       lint                    S12             -nothing-
 *                       lint                    XGATE           -undefined-
 *---------------------+-----------------------+---------------+------------+
 * FCODE                 Cosmic                  S12             -nothing-
 *                       Cosmic                  XGATE           -undefined-
 *                       PC,polyspace            S12             -nothing-
 *                       PC,polyspace            XGATE           -undefined-
 *                       lint                    S12             -nothing-
 *                       lint                    XGATE           -undefined-
 *---------------------+-----------------------+---------------+------------+
 * PRIVATE               Cosmic,lint,polyspace   both            static
 *                       PC                      both            -nothing-
 *---------------------+-----------------------+---------------+------------+
 * CONST                 Cosmic,lint,polyspace   both            const
 *                       PC                      both            -nothing-
 *---------------------+-----------------------+---------------+------------+
 * FROM                  Cosmic                  S12         \@gpage \@far const
 *                       Cosmic                  XGATE           const
 *                       PC                      both            -nothing-
 *                       polyspace,lint          both            const
 *---------------------+-----------------------+---------------+------------+
 * FRAM                  Cosmic                  S12             \@gpage
 *                       Cosmic                  XGATE           -nothing-
 *                       PC,polyspace,lint       both            -nothing-
 *---------------------+-----------------------+---------------+------------+
 * CAL,CAL_CURVE,        Cosmic                  S12      \@gpage volatile const
 * CAL_MAP,CAL_STRING    Cosmic                  XGATE           volatile const
 *                       PC                      both            -nothing-
 *                       polyspace,lint          both            const
 *---------------------+-----------------------+---------------+------------+
 * FPTR                  Cosmic                S12               \@gpage \@far
 *                       Cosmic                XGATE             -nothing-
 *                       PC,polyspace,lint     both              -nothing-
 *---------------------+-----------------------+---------------+------------+
 * </pre>
 *
 * @{
 */

/**
 * sanity check processor definitions to ensure the location qualifier
 * logic is complete (using the fact that defined() expands to 1 or 0)
 */

#if defined __MWERKS__
/* CodeWarrior */
#error CodeWarrior not supported: rework qualifier logic
#elif (defined (__CSMC__) + defined (HOSTED) + defined (_lint) + defined (POLYSPACE)) == 0
/* it's a new parser so table above and logic below needs re-working */
#error  Unknown parser - rework qualifier logic
#elif (defined (__CSMC__) + defined (HOSTED) + defined (_lint) + defined (POLYSPACE)) > 1
#error  Multiple parsers defined simultaneously.  What's going on?
#endif

#if defined __CSMC__
/*****************************************************************************
 * Start of Cosmic S12 and XGATE definitions
 ****************************************************************************/

#if !defined _XGATE_
        /* Cosmic S12 compiler */
/** Cosmic extension */
#define         INLINE          @inline
/**
 * Cosmic extension to make function suitable for an interrupt service routine
 *
 * Must not be combined with #FCODE
 * @note if you change this, be sure that CANbedded interrupt declarations
 * are similarly declared.  See the definition of CAN_IRQ_FUNC_QUAL1
 */
#define         INTERRUPT       @interrupt @svpage @near
/**
 * Cosmic extension to make function near callable
 *
 * Interrupt handlers defined with #INTERRUPT will be near without having to
 * use this macro
 */
#define                 NCODE                   @near
/**
 * Cosmic extension to make function callable from a different flash bank
 * @note now redundant as code is compiled to be \@far by default
 *
 * Must not be combined with #INTERRUPT
 */
#define                 FCODE                   /* nothing - now the default */
/**
 * Wrap up static in a macro so that hosted code can gain access to statics by
 * redefining this to nothing.
 */
#if !defined (UNIT_TEST)
#define                 PRIVATE                 static
#else
#define                 PRIVATE
#endif
/**
 * Wrap up const in a macro so that hosted code can edit variables by
 * redefining this to nothing.
 */
#define                 CONST                   const
/**
 * constant located in the 64kbyte flash page addressed with GPAGE
 *
 * location <em>can</em> be modified by \#include'ing memory_location.h
 * with MEMORY_LOCATION set to GENERAL_MEMORY or S12_MEMORY
 * <!-- macros not tagged with a # because not all defined on VTBM -->
 */
#define                 FROM                    @gpage @far const

/**
 * variable located in paged RAM addessed with RPAGE
 *
 * location <em>can</em> be modified by \#include'ing memory_location.h
 * with MEMORY_LOCATION set to GENERAL_MEMORY or S12_MEMORY
 */
#define                 FRAM                    @gpage
/**
 * calibratable located (at run-time) in paged RAM addressed with RPAGE
 *
 * location <em>must</em> be modified by \#include'ing memory_location.h
 * with MEMORY_LOCATION set to CALIBRATION_MEMORY or CALIBRATION_MEMORY_S12
 */
#define                 CAL                     @gpage const volatile
/** generic pointer to any part of memory, addressed with GPAGE */
#define                 FPTR                    @gpage @far

#else   /* _XGATE_ */
        /* Cosmic XGATE */
#define         INLINE          @inline
#define         INTERRUPT       @interrupt
/*                      NCODE/FCODE not defined for XGATE */
#define                 PRIVATE                 static
#define                 CONST                   const
#define                 FROM                    const
#define                 FRAM                    /* nothing */
#define                 CAL                     volatile const
#define                 FPTR                    /* nothing */
#endif  /* !defined (_XGATE_) */
/*****************************************************************************
 * End of Cosmic S12 and XGATE definitions
 ****************************************************************************/

#elif defined HOSTED

/*****************************************************************************
 * Start of PC definitions
 ****************************************************************************/
#define         INLINE          __inline
#define         INTERRUPT       /* nothing */
#ifndef _XGATE_
#define                 NCODE                   /* nothing */
#define                 FCODE                   /* nothing */
#else
/*                      NCODE/FCODE not defined for XGATE */
#endif /* ndef _XGATE_ */
#define                 PRIVATE                 /* nothing */

/**
 * @warning
 * windef.h defines CONST as const, for the purposes of testing we want  to
 * override this in order to test with different values for 'CONST' items.
 * This means that if you include both this file and windows.h, windows.h
 * should be included first, otherwise system headers declared with CONST in
 * them will have it expand to a blank rather than const */

#undef          CONST

#define                 CONST                   /* nothing */
#define                 FROM                    /* nothing */
#define                 FRAM                    /* nothing */
#define                 CAL                     /* nothing */
#define                 FPTR                    /* nothing */
/*****************************************************************************
 * End of PC definitions
 ****************************************************************************/

#elif defined _lint
/*****************************************************************************
 * Start of lint definitions
 ****************************************************************************/
#define         INLINE          __inline
#define         INTERRUPT       /* nothing */
#ifndef _XGATE_
#define                 NCODE                   near
#define                 FCODE                   far
#else
/*                      NCODE/FCODE not defined for XGATE */
#endif /* ndef _XGATE_ */

/* Lint should have its PRIVATE defined in the same way as Cosmic i.e. PRIVATE
 * only expands to static for non-unit test */
#if !defined (UNIT_TEST)
#define                 PRIVATE                 static
#else
#define                 PRIVATE
#endif

#define                 CONST                   const
#define                 FROM                    far const
#define                 FRAM                    far
#define                 CAL                     far const volatile
#define                 FPTR                    far
/*****************************************************************************
 * End of lint definitions
 ****************************************************************************/
#elif defined POLYSPACE
/*****************************************************************************
 * Start of Polyspace definitions
 ****************************************************************************/
#define         INLINE          /* nothing */
#define         INTERRUPT       /* nothing */
#ifndef _XGATE_
#define                 NCODE                   /* nothing */
#define                 FCODE                   /* nothing */
#else
/*                      NCODE/FCODE not defined for XGATE */
#endif
#define                 PRIVATE                 static
#define                 CONST                   const
#define                 FROM                    const
#define                 FRAM                    /* nothing */
#define                 CAL                     const
#define                 FPTR                    /* nothing */
/*****************************************************************************
 * End of Polyspace definitions
 ****************************************************************************/
#else
        /* oops: should be nothing left */
#error qualifier logic broken
#endif

/*
 * all the CAL_* macros have the same definition as plain CAL
 */
#define         CAL_CURVE       CAL
#define         CAL_MAP         CAL
#define         CAL_STRING      CAL

/**
 * RAW_INTERRUPT is identical to INTERRUPT for for everything except
 * S12 Cosmic, for which we force it not to have \@svpage
 */
#if (defined(__CSMC__)) && (!defined(_XGATE_))
#define         RAW_INTERRUPT       @interrupt @near
#else
#define         RAW_INTERRUPT       INTERRUPT
#endif

/**
 * @}
 */

/**
 * General Purpose Enumerations
 */

/* Options for indicating the state of a data item */
typedef enum
{
    No_data_available,
    New_data_available,
    Old_data_available
} data_validity_e;

/* The following is used for returning the result (success/fail) from a function
   which has a single failure mode */
typedef enum
{
    Nw_sf_success,
    Nw_sf_fail
} nw_sf_e;

/** @polyspace:end<MISRA-C:19.4:Not a defect:Justify with annotations> Errors in Cosmic supplied files, so ignore */

/** Dummy enumeration to allow specification of variables marked as "enum" in
 * data dictionary"
 * @todo remove this */
typedef enum
{
    dummy_enum_0,
    dummy_enum_1,
    dummy_enum_2
} dummy_e;


/** Dummy enumeration to allow specification of variables marked as "blank" in
 * data dictionary"
 * @todo remove this */
typedef uint16_t dummy_t;

/*****************************************************************************
 * Macros shared between the MMC and the VTBM
 ****************************************************************************/

#define TEMPERATURE_MAX  INT16_MAX   /**< maximum value for temperature_t */
#define TEMPERATURE_MIN  INT16_MIN   /**< minimum value for temperature_t */
#define CELL_V_MAX       UINT16_MAX  /**< maximum value for cell_v_t */

#define TEMPERATURE_BP   8           /**< bin point for temperature_t */
#define ADC_V_BP         13          /**< bin point for adc_v_t */
#define CELL_V_BP        13          /**< bin point for cell_v_t */

#define TEMP(t)          BP_SCALE((t), temperature_t, TEMPERATURE_BP) /**< Convert a float into temperature_t */
#define ADC_V(v)         BP_SCALE((v), adc_v_t,       ADC_V_BP)       /**< Convert a float into adc_v_t */
//#define CELL_V(v)        BP_SCALE((v), cell_v_t,      CELL_V_BP)      /**< Convert a float into cell_v_t */
#define CELL_V(v)        (cell_v_t)(v*10000u)      /**< Convert a float into cell_v_t */

#if !defined _lint && !defined HOSTED
#define LINKER_SYMBOL(def,lint_or_hosted_initialiser)      extern def
#else
#define LINKER_SYMBOL(def,lint_or_hosted_initialiser)      \
                                    PRIVATE def = lint_or_hosted_initialiser
#endif

/** @lint -esym(551,x)  Symbol is provided as a diagnostic e.g. reading over
 * XCP, so don't give a lint message */
#define DEBUGVAR(x) /*lint -esym(551,x) */

/******************************************************************************
 * Types shared between the MMC and the VTBM
 *****************************************************************************/

/**
 * Data type used to represent cell temperature. Signed 16 bit binary point
 * #TEMPERATURE_BP */
typedef int16_t        temperature_t;

/**
 * Data type used to represent adc voltages
 * It has a binary point of #ADC_V_BP  */
typedef uint16_t       adc_v_t;

/**
 * Data type used to represent cell voltages */
typedef uint16_t       cell_v_t;

/**
 * Possible values for the balancing action.
 * This enum is written so as to match the values specified in the ICM protocol.
 * @warning if BalActionOff is changed from 0, the initialisation of
 * icm_bal_action MUST be changed. */
enum BalAction_e
{
    BalActionOff=0,
    BalActionPassive=1,
    BalActionActive=2
};

//LDT
typedef int16_t        low_res_temp_t;
typedef uint16_t        rpm_t;
//typedef uint32_t         energy_wh_t;
//typedef uint16_t         milliohm_t;
#define LOW_RES_TEMP_MAX INT16_MAX   /**< maximum value for low_res_temp_t */
#define LOW_RES_TEMP_MIN INT16_MIN   /**< minimum value for low_res_temp_t */
#define LOW_RES_TEMP_BP  7           /**< bin point for low_res_temp_t */
#define LOW_RES_TEMP(t)  BP_SCALE((t), low_res_temp_t, LOW_RES_TEMP_BP) /**< Convert a float into low_res_temp_t */

#endif  /* !defined  BASIC_TYPES_H  */
