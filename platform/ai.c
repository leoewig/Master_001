/**
 * @file
 * @ingroup ai
 *
 * Analogue Interface functions
 *
 * Once started, A to D conversions run on their own so the only function
 * required is ai_init().  Conversion results are accessed through memory-
 * mapped variables defined in ai_cfg.h (which the build system will have
 * copied from a source such as mmc_ai_cfg.h)
 *
 * $Id: ai.c 8269 2013-04-22 14:06:47Z tg10 $
 */
#include "hardware.h"
#include "basic_types.h"
#include "ai.h"
#include "mmc_hw_version.h"
#include "tmi.h"
#include "LookUp_real32_T_real32_T_ZERO.h"
#include <math.h>


#define ATD_CHANNELS_PER_CONTROLLER     16u /**< number of Analogue
                              inputs per controller */

/**
 * layout of hardware registers for an ADC controller
 *
 * @lint Deviation Required : Not all structure members are referenced, so suppress\n
 * They must be defined because the structure maps over the hardware
 *      registers but they aren't required by the code \n
 * -esym(754,atd_s::stat0,atd_s::reserved,atd_s::cmpeh,atd_s::cmpeh) \n
 * -esym(754,atd_s::stat2,atd_s::dien,atd_s::cmpht,atd_s::dr)
 */
/*lint -esym(754,atd_s::stat0,atd_s::reserved,atd_s::cmpeh,atd_s::cmpel) */
/*lint -esym(754,atd_s::stat2,atd_s::dien,atd_s::cmpht,atd_s::dr) */
struct atd_s {
    uint8_t  ctl0;
    uint8_t  ctl1;
    uint8_t  ctl2;
    uint8_t  ctl3;
    uint8_t  ctl4;
    uint8_t  ctl5;
    uint8_t  stat0;
    uint8_t  reserved;
    uint8_t  cmpeh;
    uint8_t  cmpel;
    uint16_t stat2;
    uint16_t dien;
    uint16_t cmpht;
    uint16_t dr[16];
} ;

struct adc_multiplexed_var_s
{
    CONST volatile uint16_t        * source;
                   uint16_t        * destination;
                   data_validity_e * validity;
};

bool_t ai_all_multiplexed_inputs_read = FALSE;

/* current */
int16_t CURR_DCMR_RAW[sent_channel_max][Curr_RMS_Sameple_Cntr] = {0};
int32_t CURR_Integ_RAW[sent_channel_max][Curr_RMS_Sameple_Cntr] = {0};

int16_t CURR_DCMR_RMS_100ms[sent_channel_max] = {0};

int32_t Curr_Integ_prev[sent_channel_max] = {0};

int16_t CURR_DCMR_AVG_100ms[sent_channel_max] = {0};
uint8_t CURR_DCMR_CNT = 0u;

/* voltage */
uint16_t VOLT_DCMR = 0u;
uint16_t VOLT_BATT = 0u;
int16_t VOLT_AD_Input[2u] = {0u};

/**
 * @lint
 * .DEF files are not MISRA's favourite idea but we accept they are the best
 * approach to this sort of situation so do the following to keep lint quiet:\n
 * -esym(960,19.12)  Multiple use of '\#/##' operators in macro definition\n
 */
/*lint -esym(960,19.12) */
/**
 * @lint Deviation Required : Used to map structures over hardware registers
 * Lint does not like the use of '@' to locate registers, suppress with:\n
 * !e430\n
 * atd1 not referenced at the moment\n
 * -esym(528, atd1)
 */
/*lint -esym(528,atd1) */

/** This macro is only used in the header file, so define once & forget about it here */
#define NON_MULTIPLEXED_INPUT(ad_register,mmc_variable)

#define ADC_CONTROLLER(controller) \
struct atd_s ABS_LOCATE(atd ## controller ## , ATD ## controller ## CTL0);    /**< ATDx h/w registers */ /*lint !e430 */
#define MULTIPLEXED_INPUT_CH0(ad_register,mmc_variable)
#define MULTIPLEXED_INPUT_CH1(ad_register,mmc_variable)
#define MULTIPLEXED_INPUT_CH2(ad_register,mmc_variable)

/** @polyspace:begin<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes */
    #include "ai_cfg.h"
/** @polyspace:end<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes */

#undef MULTIPLEXED_INPUT_CH0
#undef MULTIPLEXED_INPUT_CH1
#undef MULTIPLEXED_INPUT_CH2
#undef ADC_CONTROLLER

#define ADC_CONTROLLER(_controller)     &atd ## _controller ## ,
#define MULTIPLEXED_INPUT_CH0(ad_register,mmc_variable)
#define MULTIPLEXED_INPUT_CH1(ad_register,mmc_variable)
#define MULTIPLEXED_INPUT_CH2(ad_register,mmc_variable)

PRIVATE struct atd_s * CONST ai_atd_controllers[Ai_max_controllers] =
{
    #include "ai_cfg.h"
};

#undef MULTIPLEXED_INPUT_CH0
#undef MULTIPLEXED_INPUT_CH1
#undef MULTIPLEXED_INPUT_CH2
#undef ADC_CONTROLLER

#if (ATD0_CH0_MAX_MULTIPLEXED_INPUT != 0u) || (ATD0_CH1_MAX_MULTIPLEXED_INPUT != 0u) || (ATD1_CH0_MAX_MULTIPLEXED_INPUT != 0u) || (ATD1_CH1_MAX_MULTIPLEXED_INPUT != 0u)
PRIVATE uint8_t multiplexed_channel = 0u;
#endif

/**
 * helper function to initialise one controller given a pointer to its
 * h/w registers.
 * See ai_init() for description of initialisation performed.
 */
PRIVATE void ai_init_controller(enum ai_controllers_e controller,uint8_t final_channel);

/**
 * helper function to select the correct multiplexer channel
 */
PRIVATE void ai_select_multiplexed_channel(uint8_t channel);

/*
 * sanity check our configuration
 */
#if ATD0_LAST_CHANNEL == 0u
#error feature does not support converting a single channel (see Reference Manual Table 13-3)
#endif

#define     ATDCTL1_12_BITS                     0x40u /**< 12 bit conversions */
#define     ATDCTL1_DISCHARGE                   0x10u /**< discharge between samples */
#define     ATDCTl2_DEFAULT                     0x00u /**< no changes required to ATDCTL2 */
#define     ATDCTL3_RIGHT_JUSTIFY               0x80u /**< right justify results */
/**
 * ATDCTL4 setting for optimal performance
 * @code
 * SMP = 0 (4 clocks per converions)
 * PRESCALER (PRS) = 15:
 *  Fatdclk =   Fbus / (2*(PRS+1))
 *      =   50 Mhz / 32
 *      =   1.5625 MHz
 *  -> ATDCTL4 = 0x0F
 * @endcode
 */
#define     ATDCTL4_OPTIMAL   0x0Fu

/** ATDCTL5.MULTI = 1 for multi-channel sequence */
#define     ATDCTL5_MULTI_CHANNEL   0x10u

#define     ATDSTAT0_SCF            0x80u

#define ADC_CONTROLLER(_controller)
#define MULTIPLEXED_INPUT_CH0(ad_register,mmc_variable) \
    CONST volatile uint16_t mmc_variable = 0u; \
            data_validity_e mmc_variable ## _validity = No_data_available;
#define MULTIPLEXED_INPUT_CH1(ad_register,mmc_variable) \
    CONST volatile uint16_t mmc_variable = 0u; \
            data_validity_e mmc_variable ## _validity = No_data_available;
#define MULTIPLEXED_INPUT_CH2(ad_register,mmc_variable) \
    CONST volatile uint16_t mmc_variable = 0u; \
            data_validity_e mmc_variable ## _validity = No_data_available;

#include "ai_cfg.h"

#undef MULTIPLEXED_INPUT_CH0
#undef MULTIPLEXED_INPUT_CH1
#undef MULTIPLEXED_INPUT_CH2
#undef ADC_CONTROLLER

#if (ATD0_CH0_MAX_MULTIPLEXED_INPUT != 0u) || (ATD1_CH0_MAX_MULTIPLEXED_INPUT != 0u)
#define ADC_CONTROLLER(_controller)
#define MULTIPLEXED_INPUT_CH0(ad_register,mmc_variable) { & ad_register ##, & mmc_variable ##, & mmc_variable ##_validity},
#define MULTIPLEXED_INPUT_CH1(ad_register,mmc_variable)
#define MULTIPLEXED_INPUT_CH2(ad_register,mmc_variable)

PRIVATE CONST struct adc_multiplexed_var_s adc_multiplexed_data_0[ATD0_CH0_MAX_MULTIPLEXED_INPUT] =
{
    #include "ai_cfg.h"
};

#undef MULTIPLEXED_INPUT_CH0
#undef MULTIPLEXED_INPUT_CH1
#undef MULTIPLEXED_INPUT_CH2
#undef ADC_CONTROLLER
#endif

#if (ATD0_CH1_MAX_MULTIPLEXED_INPUT != 0u) || (ATD1_CH1_MAX_MULTIPLEXED_INPUT != 0u)
#define ADC_CONTROLLER(_controller)
#define MULTIPLEXED_INPUT_CH0(ad_register,mmc_variable)
#define MULTIPLEXED_INPUT_CH1(ad_register,mmc_variable) { & ad_register ##, & mmc_variable ##, & mmc_variable ##_validity},
#define MULTIPLEXED_INPUT_CH2(ad_register,mmc_variable)

PRIVATE CONST struct adc_multiplexed_var_s adc_multiplexed_data_1[ATD0_CH1_MAX_MULTIPLEXED_INPUT] =
{
    #include "ai_cfg.h"
};

#undef MULTIPLEXED_INPUT_CH0
#undef MULTIPLEXED_INPUT_CH1
#undef MULTIPLEXED_INPUT_CH2
#undef ADC_CONTROLLER
#endif

#if (ATD0_CH1_MAX_MULTIPLEXED_INPUT != 0u) || (ATD1_CH1_MAX_MULTIPLEXED_INPUT != 0u)
#define ADC_CONTROLLER(_controller)
#define MULTIPLEXED_INPUT_CH0(ad_register,mmc_variable)
#define MULTIPLEXED_INPUT_CH1(ad_register,mmc_variable) 
#define MULTIPLEXED_INPUT_CH2(ad_register,mmc_variable)	{ & ad_register ##, & mmc_variable ##, & mmc_variable ##_validity},

PRIVATE CONST struct adc_multiplexed_var_s adc_multiplexed_data_2[ATD0_CH2_MAX_MULTIPLEXED_INPUT] =
{
    #include "ai_cfg.h"
};

#undef MULTIPLEXED_INPUT_CH0
#undef MULTIPLEXED_INPUT_CH1
#undef MULTIPLEXED_INPUT_CH2
#undef ADC_CONTROLLER
#endif

#undef NON_MULTIPLEXED_INPUT

/*lint +esym(960,19.12) */

void ai_initiate_read(enum ai_controllers_e controller)
{
    struct atd_s * CONST atdp = ai_atd_controllers[controller];

    /* Write to control register 5 starts a new sequence */
    atdp->ctl5 = ATDCTL5_MULTI_CHANNEL;
}

PRIVATE void ai_select_multiplexed_channel(uint8_t channel)
{
//#if (ATD0_CH0_MAX_MULTIPLEXED_INPUT != 0u) || (ATD0_CH1_MAX_MULTIPLEXED_INPUT != 0u) || (ATD1_CH0_MAX_MULTIPLEXED_INPUT != 0u) || (ATD1_CH1_MAX_MULTIPLEXED_INPUT != 0u)
//    uint8_t reg;
//    uint8_t ccr;
//
//    SAVE_INTERRUPTS(ccr);
//
//    reg = AI_SELECT_LINES_PORT;
//
//    /* Clear out the select line bits */
//    reg &= (uint8_t) ~AI_SELECT_LINES_MASK;
//
//    /* Mask in the new select line bits */
//    reg |= (channel & AI_SELECT_LINES_MASK);
//
//    AI_SELECT_LINES_PORT = reg;
//
//    RESTORE_INTERRUPTS(ccr);
//#endif
}

void ai_setup_next_multiplexed_channel()
{
//#if (ATD0_CH0_MAX_MULTIPLEXED_INPUT != 0u) || (ATD0_CH1_MAX_MULTIPLEXED_INPUT != 0u) || (ATD1_CH0_MAX_MULTIPLEXED_INPUT != 0u) || (ATD1_CH1_MAX_MULTIPLEXED_INPUT != 0u)
//    multiplexed_channel++;
//
//    if (multiplexed_channel == ATD0_CH0_MAX_MULTIPLEXED_INPUT)
//    {
//        multiplexed_channel = 0u;
//    }
//
//    ai_select_multiplexed_channel(multiplexed_channel);
//#endif
}

void ai_reselect_multiplexed_channel()
{
//#if (ATD0_CH0_MAX_MULTIPLEXED_INPUT != 0u) || (ATD0_CH1_MAX_MULTIPLEXED_INPUT != 0u) || (ATD1_CH0_MAX_MULTIPLEXED_INPUT != 0u) || (ATD1_CH1_MAX_MULTIPLEXED_INPUT != 0u)
//    ai_select_multiplexed_channel(multiplexed_channel);
//#endif
}

void ai_read_inputs(void)
{
    uint8_t i;
	int32_t sum_1 = 0;
    int32_t sum_2 = 0;
	int32_t curr_sqr;

    /* raw current */
    CURR_DCMR_RAW[sent_channel_1][CURR_DCMR_CNT] = (int16_t)((int32_t)hld_adc_motor_current_1*50000/4095 - 25000 - Curr_CAL_Offset_A);          /* mA */
    CURR_DCMR_RAW[sent_channel_2][CURR_DCMR_CNT] = (int16_t)((int32_t)hld_adc_motor_current_2*50000/4095 - 25000 - Curr_CAL_Offset_B);          /* mA */
	
    /* current integration raw */
    curr_sqr = (int32_t)CURR_DCMR_RAW[sent_channel_1][CURR_DCMR_CNT] * (int32_t)CURR_DCMR_RAW[sent_channel_1][CURR_DCMR_CNT];
    CURR_Integ_RAW[sent_channel_1][CURR_DCMR_CNT] = (curr_sqr + Curr_Integ_prev[sent_channel_1]) * 5 ;                              /* (I^2 + I1^2)/2* 10ms */
    Curr_Integ_prev[sent_channel_1] = curr_sqr;

    curr_sqr = (int32_t)CURR_DCMR_RAW[sent_channel_2][CURR_DCMR_CNT] * (int32_t)CURR_DCMR_RAW[sent_channel_2][CURR_DCMR_CNT];
    CURR_Integ_RAW[sent_channel_2][CURR_DCMR_CNT] = (curr_sqr + Curr_Integ_prev[sent_channel_2]) * 5 ;                              /* (I^2 + I1^2)/2* 10ms */
    Curr_Integ_prev[sent_channel_2] = curr_sqr;

    CURR_DCMR_CNT++;

    
    if(CURR_DCMR_CNT >= Curr_RMS_Sameple_Cntr)
    {
        /* calculate current RMS */
        for(i=0;i<Curr_RMS_Sameple_Cntr;i++)
        {
            sum_1 += CURR_Integ_RAW[sent_channel_1][i];
            sum_2 += CURR_Integ_RAW[sent_channel_2][i];
        }

        CURR_DCMR_RMS_100ms[sent_channel_1] = (int16_t)sqrt(sum_1/100);
        CURR_DCMR_RMS_100ms[sent_channel_2] = (int16_t)sqrt(sum_2/100);


        /* calculate current averagte */
        sum_1 = 0u;
        sum_2 = 0u;

        for(i=0;i<Curr_RMS_Sameple_Cntr;i++)
        {
            sum_1 += CURR_DCMR_RAW[sent_channel_1][i];
            sum_2 += CURR_DCMR_RAW[sent_channel_2][i];
        }

        CURR_DCMR_AVG_100ms[sent_channel_1] = (int16_t)(sum_1/Curr_RMS_Sameple_Cntr);
        CURR_DCMR_AVG_100ms[sent_channel_2] = (int16_t)(sum_2/Curr_RMS_Sameple_Cntr);

        CURR_DCMR_CNT = 0u;
    }


    /*  voltage */
    VOLT_DCMR = (uint16_t)((uint32_t)hld_adc_motor_voltage*20000/4095);                 /* 1mV */

    VOLT_BATT = (uint16_t)((uint32_t)hld_adc_batt_voltage*20000/4095);                 /* 1mV */

	VOLT_AD_Input[0u] = (int16_t)((((int32_t)hld_adc_ai_1 - 2048)*20000 + 2048)/4095);
    VOLT_AD_Input[1u] = (int16_t)((((int32_t)hld_adc_ai_2 - 2048)*20000 + 2048)/4095);

	//volt_AD_input_v = VOLT_AD_Input/1000;
//
	//VOLT_AD_Input = VOLT_AD_Input - (uint16_t)volt_AD_input_v*70;
}

/**
 * @par Implementation
 * ATD1 is not supported since its pins don't come out on the MMC, but a
 * framework is provided by having the call to ai_init_controller().  Adding a
 * second call for atd1 would be trivial.
 * NB This routine enables the analogue input multiplexer. These lines are under direct
 * control, hence the registers must be accessed directly & not using the dio feature.
 */
void ai_init(void)
{
    uint8_t CCR_reg;
    uint8_t reg;

    ai_init_controller(Ai_atd0,ATD0_LAST_CHANNEL);

    /* enable multiplexer outputs */
#if (ATD0_CH0_MAX_MULTIPLEXED_INPUT != 0u) || (ATD0_CH1_MAX_MULTIPLEXED_INPUT != 0u) || (ATD1_CH0_MAX_MULTIPLEXED_INPUT != 0u) || (ATD1_CH1_MAX_MULTIPLEXED_INPUT != 0u)
    /* Critical section - accessing ports directly */
    SAVE_INTERRUPTS( CCR_reg );

    if (mmc_build_version() == Hw_rub_010)
    {
        /* Enable lines are active high */
        reg   = AI_ENABLE_0_LINE_PORT;
        reg  |= AI_ENABLE_0_LINE_MASK;
        AI_ENABLE_0_LINE_PORT = reg;

        reg   = AI_ENABLE_1_LINE_PORT;
        reg  |= AI_ENABLE_1_LINE_MASK;
        AI_ENABLE_1_LINE_PORT   = reg;
    }
    else
    {
        /* Enable lines are active low */
        reg   = AI_ENABLE_0_LINE_PORT;
        reg  |= (uint8_t) ~AI_ENABLE_0_LINE_MASK;
        AI_ENABLE_0_LINE_PORT = reg;

        reg   = AI_ENABLE_1_LINE_PORT;
        reg  |= (uint8_t) ~AI_ENABLE_1_LINE_MASK;
        AI_ENABLE_1_LINE_PORT   = reg;
    }

    /* End critical section */
    RESTORE_INTERRUPTS( CCR_reg );
#endif
}

PRIVATE void ai_init_controller(enum ai_controllers_e controller,uint8_t final_channel)
{
   struct atd_s * CONST atdp = ai_atd_controllers[controller];
   uint8_t conversion_sequence_length ;

     /* This driver always starts converting from channel 0 up to final_channel,
      * so we can calculate the conversion sequence length as final_channel +1
      * (because channel numbers are from zero)
      */
    conversion_sequence_length = final_channel + 1u;

    if (conversion_sequence_length == ATD_CHANNELS_PER_CONTROLLER)
    {
      /* converting all 16 channels is a special case requiring '0' to be
       * stored in in ctl3 */
      conversion_sequence_length = 0u;
    }

    atdp->ctl0 = final_channel;     /* WRAP in ATDxCTL0 */
    atdp->ctl1 = (ATDCTL1_12_BITS | ATDCTL1_DISCHARGE);
    atdp->ctl2 = (ATDCTl2_DEFAULT);

    /*
     * right-justify the results and convert up to the final channel
     * (and FRZ[1,0]=0 means continue converting in debug)
     */
    atdp->ctl3 = (ATDCTL3_RIGHT_JUSTIFY
                        | (uint8_t) (conversion_sequence_length << 3u));

    atdp->ctl4 = ATDCTL4_OPTIMAL;

    /* Clear the Sequence Complete flag */
    atdp->stat0 = ATDSTAT0_SCF;

}

