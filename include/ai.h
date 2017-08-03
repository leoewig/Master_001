/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    ai.h
 *
 * description:  Analogue Interface header file
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Analogue Interface header file
 *                       
 *
 *****************************************************************************/

#ifndef AI_H
#define AI_H
/**
 * @file
 * @ingroup ai
 *
 * Analogue Interface header file
 */
/**
 *
 * Analogue Digital Converter Interface
 *
 * The AI feature must handle both non-multiplexed and multiplexed analogue inputs. The way
 * that these inputs are handled is covered below.
 *
 * ### Requirements
 *
 * The requirements document for the feature can be found in
 * This design intends to cover all of the Analogue Inputs requirements specified in this
 * document.
 *
 * ### Analogue to Digital converter features
 *
 * The Analogue to Digital Converter has three features which make our life easy
 * -# You can set the converter to sample a sequence of channels
 * -# Conversions are fast: the worst case for converting all 16 channels
 *    is about 204.8us, even with capacitor discharge between each reading.
 * -# Each channel has its own register which contains the most recent
 *    conversion result. It can be read anytime.
 *
 * ### Design assumptions
 *
 * The design assumes the following:
 * -# it does not matter that a reading is up to 204.8us old.
 * -# it does not matter that you cannot establish the relative age of
 *    samples from different channels
 * -# sampling every channel from 0 up to a specified channel number is
 *    appropriate
 * -# it does not matter if the multiplexed data is upto 80ms old.
 * -# the positive & negative rails of a high voltage bus presented to the rest of the
 *    MMC software must be taken from the same scanning sequence, in order to avoid skew
 *    with the readings
 *
 * ### General Approach
 *
 * Given the above features and assumptions, the approach taken is as follows:
 * - The ADC is initialised to perform a single scan from channel 0 for
 *   a specified number of channels (defined by #ATD0_LAST_CHANNEL in the
 *   configuration header file).
 * - Once a scan is complete, the multiplexed data will be copied to the correct global
 *   variable for the rest of the MMC software to use.
 * - The scheduling of this feature will be controlled from the PIT interrupt ISR to ensure
 *   that all timing considerations are met.
 * - In the event that an ADC conversion does not complete, the data validity flag shall
 *   be set to indicate #Old_data_available, otherwise it shall be set to #New_data_available.
 *
 * ### Non-Multiplexed Inputs
 *
 * This interface is refreshingly straightforward.
 *
 * ### Approach used
 *
 * Given the above features and assumptions, the approach taken is as follows:
 * - intermediate ADC result variables, defined as const volatile uint16_t, are
 *   located at the hardware result registers.
 *
 * ## Multiplexed Inputs
 *
 * This interface is not so straightforward.
 *
 * ### Approach used
 *
 * Given the above features and assumptions, the approach taken is as follows:
 * - intermediate ADC result variables, defined as const volatile uint16_t, are
 *   declared and used to store the results of the mulitplexed A/D conversions.
 *
 * ### Hardware settings
 *
 * In addition to scanning from channel 0 to the channel given
 * by #ATD0_LAST_CHANNEL, the hardware is configured as follows:
 * - 12-bit A/D conversions
 * - Right-justified results (i.e. results range from 0 to 4095)
 * - The internal sample capacitor is disharged between each measurement.
 * - Samples are made at maximum speed, although this may be changed in the
 *   future if greater accuracy is required from the ADC readings.
 *
 * ### Integration
 *
 * To use this feature, you must:
 * -# Write a configuration header file, e.g. mmc_ai_cfg.h, which defines
 *    each of the analogue inputs, as follows :
 *  - which ADC (0 or 1) is used for the input
 *  - which input number on the ADC is used for the input
 *  - the destination variable name for the data
 *  - an indication of whether the data is multiplexed or not.
 * -# Configure the build system to copy the header file into the build
 *    directory with the name ai_cfg.h (see mmc.mk for an example of how this
 *    is done).
 * -# Include ai.h and call ai_init() from initialisation code.
 * -# Call ai_initiate_read() once every 10ms from the PIT interrupt. This kicks
 *    off the next A/D conversion sequence.
 * -# On the next PIT interrupt, call ai_read_inputs() to update the multiplexed data variables
 *    and the call ai_setup_next_multiplexed_channel() to select the next multiplexed channel
 *    to be read. This will give the multiplexer approximately 9ms to actually select the channel.
 *
 * | Requirement | Code | Tested By |
 * | ---------------------------|-------------------------|---------------------------------------------|
 * | REQ_BMS_SUP_AN_001 | ai_init() | |
 * | REQ_BMS_SUP_AN_002 | #adc_multiplexed_data_0, #adc_multiplexed_data_1 | |
 * | REQ_BMS_SUP_AN_003 | Timing is dependant upon the hardware | |
 * | REQ_BMS_SUP_AN_004 | isr_pit_fgloop() | |
 * | REQ_BMS_SUP_AN_005 | isr_pit_fgloop() | |
 * | REQ_BMS_SUP_AN_006 | #adc_multiplexed_data_0, #adc_multiplexed_data_1 | |
 * | REQ_BMS_SUP_AN_007 | #adc_multiplexed_data_0, #adc_multiplexed_data_1 | |
 * | REQ_BMS_SUP_AN_008 | ai_read_inputs() | |
 * | REQ_BMS_SUP_AN_009 | #adc_multiplexed_data_0, #adc_multiplexed_data_1 | |
 * | REQ_BMS_SUP_AN_010 | #adc_multiplexed_data_0, #adc_multiplexed_data_1, ai_setup_next_multiplexed_channel() | |
 * | REQ_BMS_SUP_AN_011 | ai_select_multiplexed_channel() | |
 *
 * @defgroup ai AI Analogue Interface
 * @ingroup platform
 */

#include "hardware.h"
#include "ai_types.h"
#include "smm_types.h"

#define Curr_RMS_Sameple_Cntr 	10

#define Curr_CAL_Offset_A 310             /* mA */
#define Curr_CAL_Offset_B 250             /* mA */


extern bool_t ai_all_multiplexed_inputs_read;

/* current */
extern int16_t CURR_DCMR_RAW[sent_channel_max][Curr_RMS_Sameple_Cntr];
extern int32_t CURR_Integ_RAW[sent_channel_max][Curr_RMS_Sameple_Cntr];

extern int16_t CURR_DCMR_RMS_100ms[sent_channel_max];

extern int32_t Curr_Integ_prev[sent_channel_max];

extern int16_t CURR_DCMR_AVG_100ms[sent_channel_max];
extern uint8_t CURR_DCMR_CNT;

/* voltage */
extern uint16_t VOLT_DCMR;
extern uint16_t VOLT_BATT;
extern int16_t VOLT_AD_Input[2u];
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

#include "ai_cfg.h"

#undef NON_MULTIPLEXED_INPUT
#undef MULTIPLEXED_INPUT_CH0
#undef MULTIPLEXED_INPUT_CH1
#undef MULTIPLEXED_INPUT_CH2
#undef ADC_CONTROLLER
/*lint +esym(960,19.12) */

/**
 * Analogue interface initialisation
 *
 * Function sets up ATD controller hardware as follows:
 * - 12-bit conversion
 * - scanning from channel 0
 * - discharge between each sample
 * - as fast as possible (although discharge does slow it down a little)
 * - continue converting in debug
 * - non-fifo mode (so channels have a fixed result register)
 *
 * The number of channels sampled per sequence is defined by #ATD0_LAST_CHANNEL
 *
 */
void ai_init(void);

/**
 * This function updates the global data with the data read from the analogue
 * input channels
 */
void ai_read_inputs(void);

/**
 * This function initiates a single read of all of the analogue input channels
 * @param[in] controller - The ADC controller that is to be read
 */
void   ai_initiate_read(enum ai_controllers_e controller);

/**
 * This function increments the channel number and outputs this to the
 * DIO select lines
 */
void   ai_setup_next_multiplexed_channel(void);

/**
 * This function re-write the current channel number to the
 * DIO select lines
 */
void ai_reselect_multiplexed_channel(void);

#endif /* AI_H*/
