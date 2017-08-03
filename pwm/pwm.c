/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    pwm.c
 *
 * description:  
 *  			pwm configure.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Feb-2  Jun Wu           initial version
 *
 *****************************************************************************/
#include "pwm.h"


/*****************************************************************************
 * MACROS
 *****************************************************************************/

/*****************************************************************************
 * CALIBRATABLE DATA
 *****************************************************************************/

 
/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

/******************************************************************************
 * Public DATA
 *****************************************************************************/ 
struct pwm_data_s pwm_data[21] = {
	//{4998u,	4748u,	250u,  4248u,  25u},                 /*frequency:0KHz*/
	{4998u,	4748u,	250u,  3749u,  25u},                 /*frequency:0KHz*/		/* set limit speed to 25% */
	{4998u,	4748u,	250u,  4248u,  25u},                 /*frequency:1KHz*/
	{2500u,	2375u,	125u,  2125u,  13u},                 /*frequency:2KHz*/
	{1666u,	1583u,	83u,   1416u,  8u },                 /*frequency:3KHz*/
	{1250u,	1188u,	63u,   1063u,  6u },                 /*frequency:4KHz*/
	{1000u,	950u,	50u,   850u,  5u },                 /*frequency:5KHz*/
	{833u,	791u,	42u,   708u,  4u },                 /*frequency:6KHz*/
	{714u,	678u,	36u,   607u,  4u },                 /*frequency:7KHz*/
	{625u,	594u,	31u,   531u,  3u },                 /*frequency:8KHz*/
	{556u,	528u,	28u,   473u,  3u },                 /*frequency:9KHz*/
	{500u,	475u,	25u,   425u,  3u },                 /*frequency:10KHz*/
	{455u,	432u,	23u,   387u,  2u },                 /*frequency:11KHz*/
	{417u,	396u,	21u,   354u,  2u },                 /*frequency:12KHz*/
	{384u,	365u,	19u,   326u,  2u },                 /*frequency:13KHz*/
	{357u,	339u,	18u,   303u,  2u },                 /*frequency:14KHz*/
	{333u,	316u,	17u,   283u,  2u },                 /*frequency:15KHz*/
	{312u,	296u,	16u,   265u,  2u },                 /*frequency:16KHz*/
	{294u,	279u,	15u,   250u,  1u },                 /*frequency:17KHz*/
	{278u,	264u,	14u,   236u,  1u },                 /*frequency:18KHz*/
	{263u,	250u,	13u,   224u,  1u },                 /*frequency:19KHz*/
	{250u,	238u,	13u,   213u,  1u }                  /*frequency:20KHz*/
};

/* Maximal allowed PWM */
extern uint16_t PWM_DCMR_MAX;

/* Minimal allowed PWM */
extern uint16_t PWM_DCMR_MIN;
/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 *****************************************************************************/


/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
 
/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

void pwm_init(void)
{	
	//PWMPRCLK=0X00;                /* bus clock */
	//PWMSCLA=0X7D; 
	//PWMSCLB=0X7D;
	//PWMCLK|=PWM1_PIN_MASK; 
	//PWMCLK|=PWM2_PIN_MASK; 
	//PWMPOL|=PWM1_PIN_MASK;
	//PWMPOL|=PWM2_PIN_MASK;
	
	PWMPRCLK=0X00;                  /* bus clock */
	PWMSCLA=0X05; 
	PWMSCLB=0X05;
	PWMCLK=0xFF;					 /* use SA, SB for every channel */
	PWMCTL=0xF0;					 /* concatenate channel0,1 and channel2,3, and channel 4,5, and channel 6,7 */	
	PWMPOL=0xFF;
	
	PWMCAE=0x00; 

	/*
	PWMPER0=(uint8_t)((uint16_t)PWM_Period>>8); 
	PWMDTY0=PWM_Min;
	PWMPER1=(uint8_t)((uint16_t)PWM_Period); 
	PWMDTY1=PWM_Min;
	PWMPER2=(uint8_t)((uint16_t)PWM_Period>>8); 
	PWMDTY2=PWM_Min; 
	PWMPER3=(uint8_t)((uint16_t)PWM_Period); 
	PWMDTY3=PWM_Min; */
	
	//PWMPER0=0x01; 
	//PWMDTY0=0;
	//PWMPER1=0xF4; 
	//PWMDTY1=PWM_Min;
	//PWMPER2=0x01; 
	//PWMDTY2=0; 
	//PWMPER3=0xF4; 
	//PWMDTY3=PWM_Min; 
	
	pwm_period_set(0u);                               /*default 1KHz*/
	pwm_duty_set(sent_channel_1, pwm_data[0u].pwm_min);
	pwm_duty_set(sent_channel_2, pwm_data[0u].pwm_min);
	
	PWME_Disable(PWM1);
	PWME_Disable(PWM2);
	PWME_Disable(PWM3);
	PWME_Disable(PWM4);
	
}

void pwm_period_set(uint8_t freq_index)
{
	PWMPER0=(uint8_t)(pwm_data[freq_index].pwm_period>>8); 
	PWMPER1=(uint8_t)(pwm_data[freq_index].pwm_period); 
	PWMPER2=(uint8_t)(pwm_data[freq_index].pwm_period>>8); 
	PWMPER3=(uint8_t)(pwm_data[freq_index].pwm_period); 
	PWMPER4=(uint8_t)(pwm_data[freq_index].pwm_period>>8); 
	PWMPER5=(uint8_t)(pwm_data[freq_index].pwm_period); 
	PWMPER6=(uint8_t)(pwm_data[freq_index].pwm_period>>8); 
	PWMPER7=(uint8_t)(pwm_data[freq_index].pwm_period); 
}

void pwm_duty_set(sent_channel_e sent_channel, uint16_t duty)
{
	uint16_t pwm_max = 0u;
	uint16_t pwm_min = 0u;
	if(sent_channel==sent_channel_1)
	{
		PWMDTY0=(uint8_t)(duty>>8);
		PWMDTY1=(uint8_t)(duty);
		PWMDTY2=(uint8_t)(duty>>8);
		PWMDTY3=(uint8_t)(duty);
	}
	else if(sent_channel==sent_channel_2)
	{
		if(!Flag_Cancel_PID[sent_channel_2])
		{
			duty = pwm_data[0u].pwm_period - duty;
			
			pwm_max = PWM_DCMR_MAX*50u;
			pwm_min = PWM_DCMR_MIN*50u;
		
			if(duty > pwm_max)
  			{
  			    duty = pwm_max;
  			}
  			else if(duty < pwm_min)
  			{
  			    duty = pwm_min;
  			}
		
  			duty = pwm_data[0u].pwm_period - duty;
		}
		
		PWMDTY4=(uint8_t)(duty>>8);
		PWMDTY5=(uint8_t)(duty);
		PWMDTY6=(uint8_t)(duty>>8);
		PWMDTY7=(uint8_t)(duty);
	}
	else
	{
		;
	}
	
}

uint8_t pwm_duty_read(sent_channel_e sent_channel)
{
	uint16_t res_32;
	uint8_t res;

	if(sent_channel==sent_channel_1)
	{
		res_32 = ((uint16_t)PWMDTY0 << 8u) | (uint16_t)PWMDTY1;

		res_32 = ((uint32_t)res_32*100 + (uint32_t)pwm_data[0u].pwm_period/2u)/(uint32_t)pwm_data[0u].pwm_period;

	}
	else if(sent_channel==sent_channel_2)
	{
		res_32 = ((uint16_t)PWMDTY4 << 8u) | (uint16_t)PWMDTY5;

		res_32 = ((uint32_t)res_32*100 + (uint32_t)pwm_data[0u].pwm_period/2u)/(uint32_t)pwm_data[0u].pwm_period;
	}
	else
	{
		;
	}
	res = (uint8_t)res_32;

	return (uint8_t)res_32;
}

void pwm_ANA_period_set(void)
{
	PWMPER4=(uint8_t)(pwm_data[0].pwm_period>>8); 
	PWMPER5=(uint8_t)(pwm_data[0].pwm_period); 
	PWMPER6=(uint8_t)(pwm_data[0].pwm_period>>8); 
	PWMPER7=(uint8_t)(pwm_data[0].pwm_period); 
}

void pwm_ANA1_duty_set(uint16_t duty)
{
	PWMDTY4=(uint8_t)(duty>>8);
	PWMDTY5=(uint8_t)(duty);
}

void pwm_ANA2_duty_set(uint16_t duty)
{
	PWMDTY6=(uint8_t)(duty>>8);
	PWMDTY7=(uint8_t)(duty);
}