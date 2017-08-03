/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    ai_types.h
 *
 * description:  
 * Analogue Interface header file
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
#ifndef AI_TYPES_H
#define AI_TYPES_H
/**
 * @file
 * @ingroup ai
 *
 * Analogue Interface header file
 */
/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

#define AI_SELECT_LINES_MASK    0x07u
#define AI_SELECT_LINES_PORT    PORTK

#define AI_ENABLE_0_LINE_MASK   0x08u
#define AI_ENABLE_0_LINE_PORT   PORTK

#define AI_ENABLE_1_LINE_MASK   0x08u
#define AI_ENABLE_1_LINE_PORT   PTT

/**
 * @lint
 * .DEF files are not MISRA's favourite idea but we accept they are the best
 * approach to this sort of situation so do the following to keep lint quiet:\n
 * -esym(960,19.12)  Multiple use of '\#/##' operators in macro definition\n
 */
/*lint -esym(960,19.12) */
#define ADC_CONTROLLER(_controller) Ai_atd ##_controller ,
#define MULTIPLEXED_INPUT_CH0( ad_register, mmc_variable)
#define MULTIPLEXED_INPUT_CH1( ad_register, mmc_variable)
#define MULTIPLEXED_INPUT_CH2( ad_register, mmc_variable)
#define NON_MULTIPLEXED_INPUT( ad_register, mmc_variable)
enum ai_controllers_e
{
#include "ai_cfg.h"
    Ai_max_controllers
};
#undef NON_MULTIPLEXED_INPUT
#undef MULTIPLEXED_INPUT_CH0
#undef MULTIPLEXED_INPUT_CH1
#undef MULTIPLEXED_INPUT_CH2
#undef ADC_CONTROLLER
/*lint +esym(960,19.12) */

/**
 * Vref = 5V
 */
#define MAX_ADC_VOLTAGE (5u)

/**
 * Number of ADC counts associated with Vref
 */
#define MAX_ADC_COUNTS (0xFFFu)

/**
 * Macro to convert ADC counts into equivalent voltage using Equation 1 of
 * HW/SW Specification
 */
#define C_TO_V(c)  (((float32_t) MAX_ADC_VOLTAGE * (c))/(float32_t) MAX_ADC_COUNTS)

/**
 * Macro to convert voltage into equivalent ADC counts using Equation 1 of
 * HW/SW Specification
 */
/**
 * @lint -emacro((*),V_TO_C)
 */
/*lint -emacro((*),V_TO_C) */
#define V_TO_C(v)  ((uint16_t)(((float32_t) MAX_ADC_COUNTS * (v))/(float32_t) MAX_ADC_VOLTAGE))

#define MAX_MULTIPLEXED_INPUTS_PER_CHANNEL  8u

#endif /* AI_TYPES_H */
