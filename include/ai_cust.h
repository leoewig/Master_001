/**
 *
 * Analogue Input interface
 *
 * @file
 * @ingroup ai
 * $Id: ai_cust.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2015 - All rights reserved */

#ifndef AI_CUST_H
#define AI_CUST_H

#include "hardware.h"
#include "ai_types.h"

/**
 * @lint
 * .DEF files are not MISRA's favourite idea but we accept they are the best
 * approach to this sort of situation so do the following to keep lint quiet:\n
 * -esym(960,19.12)  Multiple use of '\#/##' operators in macro definition\n
 */
/*lint -esym(960,19.12) */
#define ADC_CONTROLLER(_controller)
#define MULTIPLEXED_INPUT_CH0(ad_register,mmc_variable) \
    extern CONST volatile uint16_t mmc_variable; \
    extern         data_validity_e mmc_variable ## _validity;
#define MULTIPLEXED_INPUT_CH1(ad_register,mmc_variable) \
    extern CONST volatile uint16_t mmc_variable; \
    extern         data_validity_e mmc_variable ## _validity;
#define MULTIPLEXED_INPUT_CH2(ad_register,mmc_variable) \
    extern CONST volatile uint16_t mmc_variable; \
    extern         data_validity_e mmc_variable ## _validity;
#define NON_MULTIPLEXED_INPUT(ad_register,mmc_variable) \
extern CONST volatile uint16_t ABS_LOCATE( ## mmc_variable ## , ## ad_register ## );

//#include "ai_cfg.h"

#undef NON_MULTIPLEXED_INPUT
#undef MULTIPLEXED_INPUT_CH0
#undef MULTIPLEXED_INPUT_CH1
#undef MULTIPLEXED_INPUT_CH2
#undef ADC_CONTROLLER
/*lint +esym(960,19.12) */

#endif /* AI_H*/
