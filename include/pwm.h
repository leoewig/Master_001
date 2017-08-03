/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    pwm.h
 *
 * description:  
 *  			pwm configure header.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Feb-2  Jun Wu           initial version
 *
 *****************************************************************************/
#if !defined(PWM_H)
#define      PWM_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#include "regs.h"
#include "smm.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/ 
 #define PWM_FREQ 10000u               /* 50M/ 1(A) / (5*2) (SA) / (PWMPER) */
 #define PWM_Period 5000000/PWM_FREQ 

 #define PWM1_PIN_MASK 0x02
 #define PWM2_PIN_MASK 0x08
 #define PWM3_PIN_MASK 0x20
 #define PWM4_PIN_MASK 0x80
// #define PWM_Max	475                      // 475/500
// #define PWM_Min       25	                 // 10/200
/*
 #define PWME_Disable(channel) do{ if(channel==PWM1)                  \
									{                                \
										PWME&=(~PWM1_PIN_MASK);     \
									}                                \
									else if (channel==PWM2)          \
									{                                \
										PWME&=(~PWM2_PIN_MASK);     \
									}                                \
								} while(0)
#define PWME_Enable(channel) do{ if(channel==PWM1)                   \
									{                                \
										PWME|=PWM1_PIN_MASK;     	 \
									}                                \
									else if (channel==PWM2)          \
									{                                \
										PWME|=PWM2_PIN_MASK;   		 \
									}                                \
								} while(0)
*/

#define PWME_Disable(channel) do{ switch(channel)               			\
									{	case PWM1:							\
											PWME&=(~PWM1_PIN_MASK); 		\
											break;                          \
										case PWM2:                          \
											PWME&=(~PWM2_PIN_MASK);         \
											break;                          \
										case PWM3:                      \
											PWME&=(~PWM3_PIN_MASK);     \
											break;                          \
										case PWM4:                      \
											PWME&=(~PWM4_PIN_MASK);     \
											break;		                    \
									}							            \
								} while(0)
									
#define PWME_Enable(channel) do{ switch(channel)               		   		\
									{	case PWM1:							\
											PWME|=PWM1_PIN_MASK;  			\
											break;                          \
										case PWM2:                          \
											PWME|=PWM2_PIN_MASK;         	\
											break;                          \
										case PWM3:                      \
											PWME|=PWM3_PIN_MASK;    	\
											break;                          \
										case PWM4:                      \
											PWME|=PWM4_PIN_MASK;    	\
											break;		                    \
									}							            \
								} while(0)
	
/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
 enum pwm_channel_e
{
    PWM1 = 0u,
    PWM2,
	PWM3,
	PWM4,
    MAX_PWM_NUM
};

struct pwm_data_s
{
	uint16_t 			pwm_period;
	uint16_t 			pwm_max;                   /* 95% */
	uint16_t 			pwm_min;				   /* 5% */
	uint16_t 	  		pwm_limit_speed;		   /* 25% - speed 50, 15% - speed 30*/
	uint8_t 			pid_multiples;
};

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
extern struct pwm_data_s pwm_data[21];
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/ 
void pwm_init(void);
void pwm_period_set(uint8_t freq_index);
//void pwm_ANA_period_set(void);
void pwm_duty_set(sent_channel_e sent_channel, uint16_t duty);
//void pwm_ANA1_duty_set(uint16_t duty);
//void pwm_ANA2_duty_set(uint16_t duty);

uint8_t pwm_duty_read(sent_channel_e sent_channel);


#endif /* PWM_H */
