/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_ai_cfg.h
 *
 * description:  
 * MMC ATD channel assignment
 *
 * Currently, only ATD0 is supported because ATD1 has no exposed pins on the
 * MMC processor.  However, extension to support ATD1 would be straightforward
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/

/** highest numbered channel in ATD0 conversion sequence  */
#define ATD0_LAST_CHANNEL       15u
#define ATD1_LAST_CHANNEL        0u

ADC_CONTROLLER(0)
ADC_CONTROLLER(1)

#define ATD0_CH0_MAX_MULTIPLEXED_INPUT  0u
#define ATD1_CH0_MAX_MULTIPLEXED_INPUT  0u

#define ATD0_CH1_MAX_MULTIPLEXED_INPUT  0u
#define ATD1_CH1_MAX_MULTIPLEXED_INPUT  0u

#define ATD0_CH2_MAX_MULTIPLEXED_INPUT  0u
#define ATD1_CH2_MAX_MULTIPLEXED_INPUT  0u

#define ATD0_LAST_NON_MULTIPLEXED_INPUT 15u
#define ATD1_LAST_NON_MULTIPLEXED_INPUT  0u

NON_MULTIPLEXED_INPUT(ATD0DR1,  hld_adc_motor_current_2)
NON_MULTIPLEXED_INPUT(ATD0DR6,  hld_adc_batt_voltage)
NON_MULTIPLEXED_INPUT(ATD0DR7,  hld_adc_ai_1)
NON_MULTIPLEXED_INPUT(ATD0DR9,  hld_adc_motor_current_1)
NON_MULTIPLEXED_INPUT(ATD0DR13, hld_adc_motor_voltage)
NON_MULTIPLEXED_INPUT(ATD0DR14, hld_adc_ai_2)

