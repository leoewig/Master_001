/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    motor.c
 *
 * description:  
 *  			motor control.
 * 
 *
 * revision history:
 *
 * Date         Author           Description
 * 2016-Feb-4   Jun Wu           initial version
 * 2017-Mar-27  Jun Wu           Control two motors
 *
 *****************************************************************************/
#include "motor.h"
#include "smm.h"
#include "pwm.h"
#include "vio.h"
#include "pid.h"


/*****************************************************************************
 * MACROS
 *****************************************************************************/

//#define Limit_Speed_Angle 70              /*about 2 degrees*/
#define Limit_Speed 30


/*****************************************************************************
 * CALIBRATABLE DATA
 *****************************************************************************/

 
/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/
PRIVATE uint8_t OverRun_Cnt[sent_channel_max] = {0u};
/******************************************************************************
 * Public DATA
 *****************************************************************************/ 
#pragma align on

/* motor rotating status */
bool_t Motor_rotating[sent_channel_max] = {FALSE, FALSE};

bool_t Motor_Stoping_Flag_A = FALSE;

#pragma align off

/* motor rotating direction */
enum Rotating_Direction_e motor_direction[sent_channel_max] = {Unknown, Unknown};

PRIVATE uint8_t tmm_start_without_PID_ctr[sent_channel_max] = {0u};

/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 *****************************************************************************/
PRIVATE void motor_position_cntrl(void);
PRIVATE void delay_us(uint16_t d_us);
/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
PRIVATE void delay_us(uint16_t d_us)
{
	uint16_t i,j;
	for(i=0;i<d_us;i++)
	{
		for(j=0;j<2;j++)
		{

		}
	}
}

/**
 *
 * motor position PID control
 *
 */
PRIVATE void motor_position_cntrl(void)
{
	int16_t speed_result = 0u;
	sent_channel_e sent_channel;

	if(Flag_Pos_PID_En)
	{
		/* use postion PID control, when close to target angle */
		for(sent_channel = sent_channel_1; sent_channel < sent_channel_max; sent_channel++)
		{
    		if(delta_Position_Sent_value[sent_channel] < 200u)				// 5 degrees
    		{
    		    speed_result = Position_PIDAdjust(sent_channel, TMM_Angle_Cur[sent_channel], TMM_Angle_Tar[sent_channel]);
    		    TMM_Speed_Tar[sent_channel] = speed_result;
    		}
    	}
	}
	
}

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/
/**
 *
 * speed pid management
 *
 */
void speed_pid(sent_channel_e sent_channel)
{

//	uint16_t PID_result;
//	uint16_t PWM_DC;
//	
//	
//	if(!Flag_Cancel_PID[sent_channel]) 
//	{
//		
//		if(delta_Position_Sent_value[sent_channel] < Limit_Speed_Angle)
//		{
//			//PWM_DC = 170u;
//			PWM_DC = pwm_data[PWM_Freq_Index].pwm_limit_speed;
//		}
//		else if(delta_Speed_Sent_Value[sent_channel] > 10)
//		{
//			PID_result = Speed_PIDAdjust(sent_channel, Speed_Sent_Value_Cur[sent_channel], Speed_Sent_Value_Tar[sent_channel])*pwm_data[PWM_Freq_Index].pid_multiples;
//			if(PID_result > pwm_data[PWM_Freq_Index].pwm_max)
//			{
//				PID_result = pwm_data[PWM_Freq_Index].pwm_max;
//			}
//			else if(PID_result < pwm_data[PWM_Freq_Index].pwm_min)
//			{
//				PID_result = pwm_data[PWM_Freq_Index].pwm_min;
//			}
//			PWM_DC = pwm_data[PWM_Freq_Index].pwm_period - PID_result;
//		}
//		
//		pwm_duty_set(sent_channel, PWM_DC);
//	}
//	
	
}

/**
 *
 * motor init
 *
 */
void motor_init(void)
{	

	motor_stop(sent_channel_1);
	motor_stop(sent_channel_2);
	Speed_PIDInit();
	Position_PIDInit();
	
}

/**
 *
 * stop the motor
 *
 */
void motor_stop(sent_channel_e sent_channel)
{	
	if(sent_channel==sent_channel_1)
	{
		PWME_Disable(PWM1);
		PWME_Disable(PWM2);
	}
	else if(sent_channel==sent_channel_2)
	{
		PWME_Disable(PWM3);
		PWME_Disable(PWM4);
	}
	else
	{
		;
	}

	delay_us(1);                                         	/* delay 10us */
	
	Speed_Sent_Value_Cur[sent_channel]=0u;
	//Speed_Sent_Value_Avg=0u;
	TMM_Speed_Cur[sent_channel]=0u;
	
	Motor_rotating[sent_channel]=FALSE;
	if (sent_channel==sent_channel_1)
	{
		Motor_Stoping_Flag_A = TRUE;
	}
	motor_direction[sent_channel]=Unknown;
	//stalling_cnt=0u;

	//pwm_duty_set(pwm_data[PWM_Freq_Index].pwm_min);					/* make sure the rotary valve starting with max duty cycle */

	if(TMM_Operating[sent_channel].pwm_ducmr_openloop == 0)
	{
		pwm_duty_set(sent_channel, pwm_data[PWM_Freq_Index].pwm_max);
	}

}

/**
 *
 * motor rotating to positive direction (left endstop to right endstop)
 *
 */
void motor_positive(sent_channel_e sent_channel)
{		

	if(sent_channel==sent_channel_1)
	{
		PWME_Disable(PWM1);
		PWME_Enable(PWM2);
		Motor_Stoping_Flag_A = FALSE;
	}
	else if(sent_channel==sent_channel_2)
	{
		PWME_Disable(PWM3);
		PWME_Enable(PWM4);
	}
	else
	{
		;
	}
	delay_us(1);                                         	/* delay 1us */
	Motor_rotating[sent_channel] = TRUE;
	motor_direction[sent_channel] = Rotating_positve;
}

/**
 *
 * motor rotating to negative direction (right endstop to left endstop)
 *
 */
void motor_negtive(sent_channel_e sent_channel)
{	
	if(sent_channel==sent_channel_1)
	{
		PWME_Disable(PWM2);
		PWME_Enable(PWM1);
		Motor_Stoping_Flag_A = FALSE;
	}
	else if(sent_channel==sent_channel_2)
	{
		PWME_Disable(PWM4);
		PWME_Enable(PWM3);
	}
	else
	{
		;
	}
	delay_us(1);                                         	/* delay 1us */
	Motor_rotating[sent_channel] = TRUE;
	motor_direction[sent_channel] = Rotating_negative;
}

/**
 *
 * motor rotating to negative direction (right endstop to left endstop)
 *
 */
void motor_rotating_update(void)
{	
	sent_channel_e sent_channel;

	for(sent_channel = sent_channel_1; sent_channel < sent_channel_max; sent_channel++)
	{
		if(Flag_TMM_Angle_Control[sent_channel])	
		{		
			tmm_start_without_PID_ctr[sent_channel]++;

			if(tmm_start_without_PID_ctr[sent_channel] > 7)
			{
				Flag_Cancel_PID[sent_channel] = FALSE;
			}
			else
			{
				Flag_Cancel_PID[sent_channel] = TRUE;
			}

			if(Angle_Sent_Value_Cur[sent_channel] < (Angle_Sent_Value_Tar[sent_channel] - Dead_Band_AngCtrl))                   /* co-rotating */
			//if(TMM_Angle_Cur < (TMM_Angle_Tar - 0))  
			{
				if((Motor_rotating[sent_channel]==TRUE)&&(motor_direction[sent_channel]==Rotating_positve))
				{
					/* keep*/
					//motor_positive();
				}
				else if((Motor_rotating[sent_channel]==TRUE)&&(motor_direction[sent_channel]==Rotating_negative))
				{
					motor_stop(sent_channel);
					motor_positive(sent_channel);
				}
				else if (Motor_rotating[sent_channel]==FALSE)
				{
					motor_positive(sent_channel);
				}
			}
			else if (Angle_Sent_Value_Cur[sent_channel] > (Angle_Sent_Value_Tar[sent_channel] + Dead_Band_AngCtrl))              /* reverse */   
			//else if (TMM_Angle_Cur > (TMM_Angle_Tar + 0))              /* reverse */   
			{
				if((Motor_rotating[sent_channel]==TRUE)&&(motor_direction[sent_channel]==Rotating_positve))
				{
					motor_stop(sent_channel);
					motor_negtive(sent_channel);
				}
				else if((Motor_rotating[sent_channel]==TRUE)&&(motor_direction[sent_channel]==Rotating_negative))
				{			
					/* keep*/
				}
				else if (Motor_rotating[sent_channel]==FALSE)
				{
					motor_negtive(sent_channel);
				}
			}
			else
			{
				motor_stop(sent_channel);
				OverRun_Cnt[sent_channel]++;
				
				if(OverRun_Cnt[sent_channel] > 3u)
				{
					Flag_Succeed_To_Target[sent_channel] = TRUE;
					OverRun_Cnt[sent_channel] = 0u;

					tmm_start_without_PID_ctr[sent_channel] = 0u;
				}
				
			}
		}
		//else
		//	{
		//		motor_stop();
		//	}
	}

	PWM_PID_Max = PWM_DCMR_MAX*50u;
	PWM_PID_Min = PWM_DCMR_MIN*50u;

	motor_position_cntrl();
}

