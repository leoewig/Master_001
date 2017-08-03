/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    utils.h
 *
 * description:   Public header for Utils
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li    Public header for Utils
 *                       
 *
 *****************************************************************************/
#ifndef UTILS_H
#define UTILS_H

#include "utils_types.h"

/* Measure a maximum time interval in ticks.
 * The time in ticks between the use of this macro and start_ticks
 * will be stored in delta_ticks
 * if delta_ticks is larger than max_ticks, then max_ticks will be set to
 * delta_ticks */
#define UPDATE_MAX_TICKS(max_ticks_,delta_ticks_,start_ticks_) do{\
    (delta_ticks_)= ts_get_ticks() - (start_ticks_); \
    if ( (delta_ticks_)>(max_ticks_) ) { \
        (max_ticks_)=(delta_ticks_); \
    } \
} while (0)


/** @lint esym(752) STAT_ASSERT__  declarator not referenced.
 * This is intentional, nothing should be referencing the symbols produced */
/*lint -esym(752,STAT_ASSERT__*) */

/** @lint redunantly declared symbol, this is intentional and part of the way
 * the macro works. */
/*lint -esym(762,STAT_ASSERT__*) */

/** A macro to cause an assertion at compile time on things that can't be
 * checked with the C preprocessor. This allows compile time checks that can
 * use sizeof, enums or constant macros that contain casts etc.
 * If the expression is false, the second declaration will result in a
 * different array size and produce a compiler error.
 *
 * The expression should be a constant expression that evaluates to true or
 * false and the message should have no spaces in it.
 *
 * As long as no  other attempt is made to  manipulate the variable produced
 *  (e.g. assign a value to it) Cosmic discards the object entirely and emits
 * no code or data for it.
 **/
#define STATIC_ASSERT(expr, msg)   \
    extern bool_t STAT_ASSERT__##msg[1]; \
    extern bool_t STAT_ASSERT__##msg[(expr)?1:2]

/*************************************************************************
 * Mask testing macros
 *************************************************************************/

#define TEST_MASK(value, mask) (bool_t)(0u!=((value) & (mask)))

/**
 * Test that value and-ed with mask equals mask
 * @param[in] value the value to test
 * @param[in] mask the bits that are being checked for
 * @retval TRUE if all the bits contained in mask are set in value */
#define TEST_MASK_EQ(value, mask) (bool_t)(( (value) & (mask) )==(mask))

/**
 * Test that value and-ed with mask equals mask
 * @param[in] variable the variable being tested
 * @param[in] value the value to test against
 * @param[in] mask the bits that are being checked for
 * @retval TRUE if all the bits contained in mask are set in value */
#define TEST_MASKED_VAL(variable, mask, value) \
    (bool_t)(( (variable) & (mask) )==(value))

/*************************************************************************
 * Fixed to floating conversions
 *************************************************************************/

/**
 * Convert a (positive) binary-pointed integer into a float */
#define BP_TO_FLOAT(input,bp) ((float32_t)(input)/(float32_t)(1L<<(bp)))

/**
 * Turn a pack_v_t into a float */
#define PACKV_TO_FLOAT(input) (BP_TO_FLOAT( (input) , PACK_V_BP) )
//#define PACKV_TO_FLOAT(input) ((float32_t)(input)*512/(float32_t)10000 )
/**
 * Turn a cell_v_t into a float */
//#define CELLV_TO_FLOAT(input) (BP_TO_FLOAT( (input) , CELL_V_BP) )
#define CELLV_TO_FLOAT(input) ((float32_t)(input)/(float32_t)10000)
/**
 * Turn a temperature_t into a float */
#define TEMP_TO_FLOAT(input)  (BP_TO_FLOAT( (input) , TEMPERATURE_BP) )

/* Coolant flow rate l/min to a float */
#define FLOW_TO_FLOAT(input)  (BP_TO_FLOAT( (input) , FLOW_LPM_BP) )

#define SPEED_KPH_TO_FLOAT(input)  (BP_TO_FLOAT( (input) , SPEED_BP) )

#endif /* UTILS_H*/
