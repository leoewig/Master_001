/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    tmm.c
 *
 * description:  
 *  			tmm management.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Mar-2  Jun Wu           initial version
 * 2017-Mar-27  Jun Wu          Control two TMM
 *
 *****************************************************************************/
#include "tmm.h"
#include "motor.h"
#include "smm.h"
#include "exc.h"
#include "mdm.h"
#include "mdm_ee.h"
#include "nvs.h"
#include "ai.h"
#include "pwm.h"
#include "vio.h"
#include "dio_cfg.h"



/*****************************************************************************
 * MACROS
 *****************************************************************************/
#define TMM_Stalling_Sample_CNT 50                   /* stailling time 500ms */

/*****************************************************************************
 * CALIBRATABLE DATA
 *****************************************************************************/

 
/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/
struct tmm_endstop_s TMM_Endstop_Def = {259,-4,3950,145};


/* tmm angle raw data for stalling judgement */
PRIVATE int16_t TMM_Angle_Value[sent_channel_max][TMM_Stalling_Sample_CNT+1];

/* tmm stalling counter */
PRIVATE uint8_t tmm_stalling_Count[sent_channel_max] = {0u};

/* Flag: if the motor stalled */
//PRIVATE bool_t Flag_Tmm_Stalling[sent_channel_max] = {FALSE, FALSE};

/* Flag: motor stalled conter */
//PRIVATE uint8_t Tmm_StallingCtr[sent_channel_max] = {0u};

/* counter to calculate the stop time in endstop or angle range */
PRIVATE uint16_t tmm_scaling_stop_ctr[sent_channel_max] = {0u};

/******************************************************************************
 * Public DATA
 *****************************************************************************/ 
/* tmm Rx buffer */
struct tmm_rx_s TMM_RX_CAN = 
{
	{0u},
	{0u},
	{0u},
	{0u},
	{0u},
	{0u},
	No_data_available,
	No_data_available,
	No_data_available,
	No_data_available,
	No_data_available,
	No_data_available
};



/* Flag: enable angle control */
bool_t Flag_TMM_Angle_Control[sent_channel_max] = {FALSE, FALSE};

/* Flag: succeed to target position */
bool_t Flag_Succeed_To_Target[sent_channel_max] = {FALSE, FALSE};

/* tmm target angle */
int16_t TMM_Angle_Tar[sent_channel_max]={0u};

/* TMM target speed */
int16_t TMM_Speed_Tar[sent_channel_max]={0u};

/* tmm angle deviation between target and current angle */
uint16_t TMM_Angle_Deviation[sent_channel_max]={0u};

/* tmm state machine */
scaling_state_e TMM_Scaling_State[sent_channel_max] = {scaling_s_detect};

/* TMM operating mode */
struct tmm_operating_s TMM_Operating[sent_channel_max] = 
{
	{
		FALSE,
		No_Operating,
		TMM_UME_Default,
		TMM_LME_Default,
		TMM_UAR_Default,
		TMM_LAR_Default,
		100u,                               /* 1s */
		0u,
		0u,
		0u,
		0u,
		300u,
		TMM_Speed_Default
	},
	{
		FALSE,
		No_Operating,
		TMM_UME_Default,
		TMM_LME_Default,
		TMM_UAR_Default,
		TMM_LAR_Default,
		100u,                               /* 1s */
		0u,
		0u,
		0u,
		0u,
		300u,
		TMM_Speed_Default
	}
};

/* TMM endstop angle */
struct tmm_endstop_s TMM_Endstop_Cur[sent_channel_max];

/* cyclic cnter */
uint16_t Cntr_Endpos_Mea[sent_channel_max] = {0u};	

/* Flag: if the motor stalled */
bool_t Flag_Tmm_Stalling[sent_channel_max] = {FALSE, FALSE};			

/* Flag: refence run status */
ref_run_status_e Flag_Ref_Run_Status[sent_channel_max] = {ref_unstart, ref_unstart};			  



uint8_t TMM_Matrix_Num = 0u;

//uint8_t PWM_To_ME_Ctr = 0u;

//int16_t CURR_DCMR_RMS[10] = {0};
//int16_t CURR_DCMR_RMS_100ms = 0;

/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 *****************************************************************************/
PRIVATE void tmm_PWM_To_ME_openloop(sent_channel_e sent_channel);
PRIVATE void tmm_scaling_detect(sent_channel_e sent_channel);
PRIVATE void tmm_scaling_to_UME(sent_channel_e sent_channel);
PRIVATE void tmm_scaling_stop_in_UE(sent_channel_e sent_channel);
PRIVATE void tmm_scaling_to_LME(sent_channel_e sent_channel);
PRIVATE void tmm_scaling_stop_in_LE(sent_channel_e sent_channel);
PRIVATE void tmm_scaling_to_UAR(sent_channel_e sent_channel);
PRIVATE void tmm_scaling_to_LAR(sent_channel_e sent_channel);
PRIVATE void tmm_scaling_to_Matrix(sent_channel_e sent_channel);
PRIVATE void tmm_scaling_to_Target(sent_channel_e sent_channel);
PRIVATE void tmm_stalling(sent_channel_e sent_channel);
PRIVATE void tmm_diviation(sent_channel_e sent_channel);
PRIVATE void tmm_operating(sent_channel_e sent_channel);
//PRIVATE void tmm_scaling_shutdown(sent_channel_e sent_channel);

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
/**
 *
 * PWM to mechanical endstop in open loop
 *
 */
PRIVATE void tmm_PWM_To_ME_openloop(sent_channel_e sent_channel)
{
	uint16_t pwm_duty_OLP;
	uint8_t pwm_dumr_olp = 0u;

	if(TMM_Operating[sent_channel].pwm_ducmr_openloop>0)
	{
		TMM_Scaling_State[sent_channel] = scaling_to_UME;
		
		pwm_duty_OLP = (uint16_t)(((uint32_t)pwm_data[PWM_Freq_Index].pwm_period * (uint32_t)TMM_Operating[sent_channel].pwm_ducmr_openloop + 50)/100);
		pwm_duty_OLP = pwm_data[PWM_Freq_Index].pwm_period - pwm_duty_OLP;
		pwm_duty_set(sent_channel, pwm_duty_OLP);
	}
	else if (TMM_Operating[sent_channel].pwm_ducmr_openloop<0)
	{
		TMM_Scaling_State[sent_channel] = scaling_to_LME;
		
		pwm_dumr_olp = (uint8_t)(-TMM_Operating[sent_channel].pwm_ducmr_openloop);
		pwm_duty_OLP = (uint16_t)(((uint32_t)pwm_data[PWM_Freq_Index].pwm_period * (uint32_t)pwm_dumr_olp + 50)/100);
		pwm_duty_OLP = pwm_data[PWM_Freq_Index].pwm_period - pwm_duty_OLP;
		pwm_duty_set(sent_channel, pwm_duty_OLP);
	}
}

/**
 *
 * detect TMM operating mode
 *
 */
PRIVATE void tmm_scaling_detect(sent_channel_e sent_channel)
{
	motor_stop(sent_channel);              /* stop the previous operating mode */
	Cntr_Endpos_Mea[sent_channel] = 0u;					/* cyclic counter */

//	if(TMM_Operating[sent_channel].tmm_operating_mode == To_Any_Target_Angle)
//	{
//		TMM_Speed_Tar[sent_channel] = Shift_Speed_Max;	
//	}
//	else
//	{
//		TMM_Speed_Tar[sent_channel] = TMM_Operating[sent_channel].target_speed;	
//	}
	
	switch(TMM_Operating[sent_channel].tmm_operating_mode)
	{
		case Refrence_Run:
			TMM_Scaling_State[sent_channel] = scaling_to_UME;
			TMM_Endstop_Cur[sent_channel].angle_upper_endstop = TMM_Operating[sent_channel].input_angle_ume;
			TMM_Endstop_Cur[sent_channel].angle_lower_endstop = TMM_Operating[sent_channel].input_angle_lme;

			Flag_Ref_Run_Status[sent_channel] = ref_start;
			//TMM_Speed_Tar[sent_channel] = TMM_Speed_Default;
			break;
		case Shift_Speed_AR:
			if(TMM_Speed_Tar[sent_channel] > 0u)
			{
				TMM_Scaling_State[sent_channel] = scaling_to_UAR;
			}
			else
			{
				TMM_Scaling_State[sent_channel] = scaling_to_LAR;
			}			
			break;
		case Shift_Speed_ME:
			if(TMM_Speed_Tar[sent_channel] > 0u)
			{
				TMM_Scaling_State[sent_channel] = scaling_to_UME;
			}
			else
			{
				TMM_Scaling_State[sent_channel] = scaling_to_LME;
			}
			break;
		case Matrix_Run:
			TMM_Scaling_State[sent_channel] = scaling_to_Matrix;
			break;
		case PWM_To_ME_closeloop:
			TMM_Scaling_State[sent_channel] = scaling_to_UME;
			TMM_Operating[sent_channel].pwm_dcmr_closeloop = PWM_DCMR_MAX;
			Flag_Cancel_PID[sent_channel] = TRUE;			
			break;
		case To_Any_Target_Angle:
			TMM_Scaling_State[sent_channel] = scaling_to_Target;
			TMM_Angle_Tar[sent_channel] = TMM_Operating[sent_channel].target_angle;
			pwm_duty_set(sent_channel, pwm_data[PWM_Freq_Index].pwm_min);
			break;
		case To_Any_Target_Sent:
			TMM_Scaling_State[sent_channel] = scaling_to_Target;
			break;
		case PWM_To_ME_openloop:
			Flag_Cancel_PID[sent_channel] = TRUE;	
			tmm_PWM_To_ME_openloop(sent_channel);
			break;
		case No_Operating:
			TMM_Scaling_State[sent_channel] = scaling_shutdown;
			break;
		default:
			TMM_Scaling_State[sent_channel] = scaling_shutdown;
			break;
	}

	//TMM_Speed_Tar[sent_channel] = (uint8_t)(((uint32_t)TMM_Speed_Tar[sent_channel]*10000 - 19375)/7656);				/* speed calibration */	
	
}

/**
 *
 * motor rotating to upper endstop (right endstop)
 *
 */
PRIVATE void tmm_scaling_to_UME(sent_channel_e sent_channel)
{
	uint16_t pwm_duty;
	
	TMM_Angle_Tar[sent_channel] = TMM_Endstop_Cur[sent_channel].angle_upper_endstop;
	Angle_Sent_Value_Tar[sent_channel] =(uint32_t)(((int32_t)TMM_Angle_Tar[sent_channel]*(int32_t)TMM_Angle_gain[sent_channel] + (int32_t)TMM_Angle_offset[sent_channel] + TMM_Calc_gain/2)/TMM_Calc_gain);
	
	
	if(TMM_Operating[sent_channel].tmm_operating_mode == PWM_To_ME_closeloop) 
	{
		pwm_duty = (uint16_t)(((uint32_t)pwm_data[PWM_Freq_Index].pwm_period * (uint32_t)TMM_Operating[sent_channel].pwm_dcmr_closeloop + 50)/100);
		pwm_duty = pwm_data[PWM_Freq_Index].pwm_period - pwm_duty;
		pwm_duty_set(sent_channel, pwm_duty);
	}
	
	motor_positive(sent_channel);             /* start with pwm max */		

	if(TMM_Operating[sent_channel].tmm_operating_mode != PWM_To_ME_openloop)
	{
		//if((Flag_Tmm_Stalling[sent_channel])&&(TMM_Angle_Deviation < AngleDevMax))
		if(Flag_Tmm_Stalling[sent_channel])                                  //TMM_Angle_Deviation in reference run is different from products
		{
			motor_stop(sent_channel);
			Flag_Tmm_Stalling[sent_channel] = FALSE;
				
			if(TMM_Operating[sent_channel].tmm_operating_mode == Refrence_Run)
			{
				TMM_Endstop_Cur[sent_channel].sent_upper_endstop = Angle_Sent_Value_Cur[sent_channel];
				//TMM_Position = TMM_UME;
			}

			TMM_Scaling_State[sent_channel] = scaling_stop_in_UE;	
		}
	}
}

/**
 *
 * motor stop in upper endstop/angle range
 *
 */
PRIVATE void tmm_scaling_stop_in_UE(sent_channel_e sent_channel)
{

	tmm_scaling_stop_ctr[sent_channel]++;
	
	if(tmm_scaling_stop_ctr[sent_channel] > TMM_Operating[sent_channel].stop_duration )
	{
		if((TMM_Operating[sent_channel].tmm_operating_mode == Refrence_Run) || \
			(TMM_Operating[sent_channel].tmm_operating_mode == PWM_To_ME_closeloop))
		{
			TMM_Scaling_State[sent_channel] = scaling_to_LME;
		}
		else if(TMM_Operating[sent_channel].tmm_operating_mode == Shift_Speed_ME)
		{
			if(TMM_Speed_Tar[sent_channel] < 0u)
			{
				Cntr_Endpos_Mea[sent_channel]++;

				if(Flag_Single_Cycle[sent_channel])
				{
					TMM_Scaling_State[sent_channel] = scaling_shutdown;
				}
				else
				{
					TMM_Scaling_State[sent_channel] = scaling_to_LME;
				}
			}
			else
			{
				TMM_Scaling_State[sent_channel] = scaling_to_LME;
			}
		}
		else if(TMM_Operating[sent_channel].tmm_operating_mode == Shift_Speed_AR)
		{
			if(TMM_Speed_Tar[sent_channel] < 0u)
			{
				Cntr_Endpos_Mea[sent_channel]++;

				if(Flag_Single_Cycle[sent_channel])
				{
					TMM_Scaling_State[sent_channel] = scaling_shutdown;
				}
				else
				{
					TMM_Scaling_State[sent_channel] = scaling_to_LAR;
				}
			}
			else
			{
				TMM_Scaling_State[sent_channel] = scaling_to_LAR;
			}
		}
		
		tmm_scaling_stop_ctr[sent_channel] = 0u;
		//TMM_Position = TMM_Normal;
	}	
}

/**
 *
 * motor rotating to lower endstop (left endstop)
 *
 */
PRIVATE void tmm_scaling_to_LME(sent_channel_e sent_channel)
{
	uint16_t pwm_duty;
	
	TMM_Angle_Tar[sent_channel] = TMM_Endstop_Cur[sent_channel].angle_lower_endstop;
	Angle_Sent_Value_Tar[sent_channel] =(uint32_t)(((int32_t)TMM_Angle_Tar[sent_channel]*(int32_t)TMM_Angle_gain[sent_channel] + (int32_t)TMM_Angle_offset[sent_channel] + TMM_Calc_gain/2)/TMM_Calc_gain);

	if(TMM_Operating[sent_channel].tmm_operating_mode == PWM_To_ME_closeloop) 
	{
		pwm_duty = (uint16_t)(((uint32_t)pwm_data[PWM_Freq_Index].pwm_period * (uint32_t)TMM_Operating[sent_channel].pwm_dcmr_closeloop + 50)/100);
		pwm_duty = pwm_data[PWM_Freq_Index].pwm_period - pwm_duty;
		pwm_duty_set(sent_channel, pwm_duty);
	}
	
	motor_negtive(sent_channel);
	
	if(TMM_Operating[sent_channel].tmm_operating_mode != PWM_To_ME_openloop)
	{
		//if((Flag_Tmm_Stalling[sent_channel])&&(TMM_Angle_Deviation < AngleDevMax))
		if(Flag_Tmm_Stalling[sent_channel])						//TMM_Angle_Deviation in reference run is different from products
		{
			motor_stop(sent_channel);
			Flag_Tmm_Stalling[sent_channel] = FALSE;
					
			if(TMM_Operating[sent_channel].tmm_operating_mode == Refrence_Run)
			{
				TMM_Endstop_Cur[sent_channel].sent_lower_endstop = Angle_Sent_Value_Cur[sent_channel];
				//TMM_Position = TMM_LME;
			}

			TMM_Scaling_State[sent_channel] = scaling_stop_in_LE;
		}
	}
	
}

/**
 *
 * motor stop in lower endstop/angle range
 *
 */
PRIVATE void tmm_scaling_stop_in_LE(sent_channel_e sent_channel)
{
	tmm_scaling_stop_ctr[sent_channel]++;
	
	if(tmm_scaling_stop_ctr[sent_channel] > TMM_Operating[sent_channel].stop_duration )
	{
		switch(TMM_Operating[sent_channel].tmm_operating_mode)
		{
			case Refrence_Run:
				TMM_Scaling_State[sent_channel] = scaling_shutdown;
				
				///* store to eeprom */
				//mdm_populate_image();
				//nvs_request_store(Nvs_Mdm_feature);
				//nvs_bg_write_no_reset(0u);
				
				//TMM_Angle_gain[sent_channel] = (uint32_t)(TMM_Endstop_Cur[sent_channel].sent_upper_endstop - TMM_Endstop_Cur[sent_channel].sent_lower_endstop)*TMM_Calc_gain;
				//TMM_Angle_gain[sent_channel] = TMM_Angle_gain[sent_channel]/(uint32_t)(TMM_Endstop_Cur[sent_channel].angle_upper_endstop - TMM_Endstop_Cur[sent_channel].angle_lower_endstop);
				//TMM_Angle_offset[sent_channel] = ((uint32_t)TMM_Endstop_Cur[sent_channel].sent_upper_endstop*TMM_Calc_gain) - ((uint32_t)TMM_Endstop_Cur[sent_channel].angle_upper_endstop*TMM_Angle_gain[sent_channel]);
				tmm_reset_factors(sent_channel);

				//Flag_Ref_Run_Status[sent_channel] = ref_succeed;
				if(TMM_Endstop_Cur[sent_channel].sent_upper_endstop < (TMM_Endstop_Cur[sent_channel].sent_lower_endstop + 3000))
				{
					Flag_Ref_Run_Status[sent_channel] = ref_failed;
					TMM_Endstop_Cur[sent_channel].sent_upper_endstop  = 2400u;
					TMM_Endstop_Cur[sent_channel].sent_lower_endstop  = 1700u;
				}
				else
				{
					Flag_Ref_Run_Status[sent_channel] = ref_succeed;
				}

				//TMM_Scaling_State[sent_channel] = scaling_to_Target;
				break;
			case Shift_Speed_AR:
				if(TMM_Speed_Tar[sent_channel] > 0u)
				{
					Cntr_Endpos_Mea[sent_channel]++;

					if(Flag_Single_Cycle[sent_channel])
					{
						TMM_Scaling_State[sent_channel] = scaling_shutdown;
					}
					else
					{
						TMM_Scaling_State[sent_channel] = scaling_to_UAR;
					}
				}
				else
				{
					TMM_Scaling_State[sent_channel] = scaling_to_UAR;
				}	
				break;
			case Shift_Speed_ME:
				if(TMM_Speed_Tar[sent_channel] > 0u)
				{
					Cntr_Endpos_Mea[sent_channel]++;

					if(Flag_Single_Cycle[sent_channel])
					{
						TMM_Scaling_State[sent_channel] = scaling_shutdown;
					}
					else
					{
						TMM_Scaling_State[sent_channel] = scaling_to_UME;
					}
				}
				else
				{
					TMM_Scaling_State[sent_channel] = scaling_to_UME;
				}
				break;
			case PWM_To_ME_closeloop:
				Cntr_Endpos_Mea[sent_channel]++;
				if(Flag_Single_Cycle[sent_channel])
				{
					TMM_Scaling_State[sent_channel] = scaling_shutdown;
				}
				else
				{
					TMM_Scaling_State[sent_channel] = scaling_to_UME;
				}
				break;
		}
		
		tmm_scaling_stop_ctr[sent_channel] = 0u;
		//TMM_Position = TMM_Normal;
	}	
}

/**
 *
 *  motor rotating to upper angle range
 *
 */
PRIVATE void tmm_scaling_to_UAR(sent_channel_e sent_channel)
{
	TMM_Angle_Tar[sent_channel] = TMM_Operating[sent_channel].input_angle_uar;
	Angle_Sent_Value_Tar[sent_channel] =(uint32_t)(((int32_t)TMM_Angle_Tar[sent_channel]*(int32_t)TMM_Angle_gain[sent_channel] + (int32_t)TMM_Angle_offset[sent_channel] + TMM_Calc_gain/2)/TMM_Calc_gain);
	
	Flag_TMM_Angle_Control[sent_channel] = TRUE;
	
	if(Flag_Succeed_To_Target[sent_channel])
	{
		Flag_TMM_Angle_Control[sent_channel] = FALSE;
		Flag_Succeed_To_Target[sent_channel] = FALSE;
		TMM_Scaling_State[sent_channel] = scaling_stop_in_UE;
	}
	
	
}

/**
 *
 *  motor rotating to lower angle range
 *
 */
PRIVATE void tmm_scaling_to_LAR(sent_channel_e sent_channel)
{
	TMM_Angle_Tar[sent_channel] = TMM_Operating[sent_channel].input_angle_lar;
	Angle_Sent_Value_Tar[sent_channel] =(uint32_t)(((int32_t)TMM_Angle_Tar[sent_channel]*(int32_t)TMM_Angle_gain[sent_channel] + (int32_t)TMM_Angle_offset[sent_channel] + TMM_Calc_gain/2)/TMM_Calc_gain);
	
	Flag_TMM_Angle_Control[sent_channel]  = TRUE;
	
	if(Flag_Succeed_To_Target[sent_channel] )
	{
		Flag_TMM_Angle_Control[sent_channel]  = FALSE;
		Flag_Succeed_To_Target[sent_channel]  = FALSE;
		TMM_Scaling_State[sent_channel] = scaling_stop_in_LE;
	}
	
}

PRIVATE void tmm_scaling_to_Matrix(sent_channel_e sent_channel)
{
	TMM_Matrix_Num = TMM_Operating[sent_channel].matrix_sel;
	Angle_Sent_Value_Tar[sent_channel] =(uint32_t)(((int32_t)TMM_Angle_Tar[sent_channel]*(int32_t)TMM_Angle_gain[sent_channel] + (int32_t)TMM_Angle_offset[sent_channel] + TMM_Calc_gain/2)/TMM_Calc_gain);
	//TMM_Speed_Tar[sent_channel] = (uint8_t)(((uint32_t)TMM_Speed_Tar[sent_channel]*10000 - 19375)/7656);				/* speed calibration */	
	
	if(Flag_Succeed_To_Target[sent_channel] )
	{
		Flag_TMM_Angle_Control[sent_channel]  = FALSE;
		Flag_Succeed_To_Target[sent_channel]  = FALSE;
	}
}

PRIVATE void tmm_scaling_to_Target(sent_channel_e sent_channel)
{

	if(TMM_Operating[sent_channel].tmm_operating_mode == To_Any_Target_Angle)
	{
		TMM_Angle_Tar[sent_channel] = TMM_Operating[sent_channel].target_angle;
		Angle_Sent_Value_Tar[sent_channel] =(uint32_t)(((int32_t)TMM_Angle_Tar[sent_channel]*(int32_t)TMM_Angle_gain[sent_channel] + (int32_t)TMM_Angle_offset[sent_channel] + TMM_Calc_gain/2)/TMM_Calc_gain);
	}
	else if (TMM_Operating[sent_channel].tmm_operating_mode == To_Any_Target_Sent)
	{
		Angle_Sent_Value_Tar[sent_channel] = TMM_Operating[sent_channel].target_sent;
		TMM_Angle_Tar[sent_channel] = (int16_t)((TMM_Calc_gain*(int32_t)Angle_Sent_Value_Tar[sent_channel] - (int32_t)TMM_Angle_offset[sent_channel] + (int32_t)TMM_Angle_gain[sent_channel]/2)/(int32_t)TMM_Angle_gain[sent_channel]);
	}
	//else if(TMM_Operating[sent_channel].tmm_operating_mode == Refrence_Run)
	//{
	//	TMM_Angle_Tar[sent_channel] = TMM_Safety_Angle;
	//	Angle_Sent_Value_Tar[sent_channel_1] =(uint32_t)(((int32_t)TMM_Angle_Tar[sent_channel]*(int32_t)TMM_Angle_gain[sent_channel] + (int32_t)TMM_Angle_offset[sent_channel] + TMM_Calc_gain/2)/TMM_Calc_gain);
	//}
	
	Flag_TMM_Angle_Control[sent_channel]  = TRUE;
	
	if(Flag_Succeed_To_Target[sent_channel] )
	{
		Flag_TMM_Angle_Control[sent_channel]  = FALSE;
		Flag_Succeed_To_Target[sent_channel]  = FALSE;
		TMM_Scaling_State[sent_channel] = scaling_shutdown;
	}
	
}


/**
 *
 * determine if the motor is stalled.
 *
 */
PRIVATE void tmm_stalling(sent_channel_e sent_channel)
{
	uint8_t i;
	//bool_t res = FALSE;
	int32_t sum = 0;

	TMM_Angle_Value[sent_channel][tmm_stalling_Count[sent_channel]] = TMM_Angle_Cur[sent_channel];
	
	if(Motor_rotating[sent_channel])
	{
		tmm_stalling_Count[sent_channel]++;
	}
	else
	{
		tmm_stalling_Count[sent_channel] = 0u;
	}

	if(tmm_stalling_Count[sent_channel] > (TMM_Stalling_Sample_CNT-1))                /* determine stalling every 500ms */
	{
		for(i=0;i<TMM_Stalling_Sample_CNT;i++)
		{
			sum += TMM_Angle_Value[sent_channel][i];
		}

		if((sum > ((TMM_Angle_Cur[sent_channel]-1)*TMM_Stalling_Sample_CNT))&&(sum < ((TMM_Angle_Cur[sent_channel]+1)*TMM_Stalling_Sample_CNT)))   
		//if(sum == ((int32_t)TMM_Angle_Cur[sent_channel_1]*TMM_Stalling_Sample_CNT))
		{
			Flag_Tmm_Stalling[sent_channel] = TRUE;
			
			//TMM_Angle_Sum=TMM_Angle_Lower_MIN*TMM_Stalling_Sample_CNT;                          //clear
		}
		
		tmm_stalling_Count[sent_channel]=0;
	}
	
}

/**
 *
 * calculate angle diviation between target and current angle.
 *
 */
PRIVATE void tmm_diviation(sent_channel_e sent_channel)
{
	if(TMM_Angle_Tar[sent_channel] > TMM_Angle_Cur[sent_channel])
	{
		TMM_Angle_Deviation[sent_channel] = TMM_Angle_Tar[sent_channel] - TMM_Angle_Cur[sent_channel];
	}
	else
	{
		TMM_Angle_Deviation[sent_channel] = TMM_Angle_Cur[sent_channel] - TMM_Angle_Tar[sent_channel];
	}
}

/**
 *
 * tmm operating state machine.
 *
 */
PRIVATE void tmm_operating(sent_channel_e sent_channel)
{	
	switch(TMM_Scaling_State[sent_channel])
	{
		case scaling_s_detect:
			tmm_scaling_detect(sent_channel);
			break;
		case scaling_to_UME:
			tmm_scaling_to_UME(sent_channel);
			break;
		case scaling_stop_in_UE:
			tmm_scaling_stop_in_UE(sent_channel);
			break;
		case scaling_to_LME:
			tmm_scaling_to_LME(sent_channel);
			break;
		case scaling_stop_in_LE:
			tmm_scaling_stop_in_LE(sent_channel);
			break;
		case scaling_to_UAR:
			tmm_scaling_to_UAR(sent_channel);
			break;
		case scaling_to_LAR:
			tmm_scaling_to_LAR(sent_channel);
			break;
		case scaling_to_Matrix:
			tmm_scaling_to_Matrix(sent_channel);
			break;
		case scaling_to_Target:
			tmm_scaling_to_Target(sent_channel);
			break;
		case scaling_shutdown:
			tmm_scaling_shutdown(sent_channel);
			break;			
	}

	if(TMM_Operating[sent_channel].tmm_operating_mode == To_Any_Target_Angle)
	{
		TMM_Speed_Tar[sent_channel] = Shift_Speed_Max;	
	}
	else
	{
		TMM_Speed_Tar[sent_channel] = TMM_Operating[sent_channel].target_speed;	
	}
	
	if(!Flag_Cancel_PID[sent_channel])
	{
		Speed_Sent_Value_Tar[sent_channel] =(uint32_t)(((uint32_t)TMM_Speed_Tar[sent_channel]*TMM_Angle_gain[sent_channel]  + TMM_Calc_gain/2)/TMM_Calc_gain);
	}
}

/**
 *
 * tmm operating shutdown.
 *
 */
void tmm_scaling_shutdown(sent_channel_e sent_channel)
{
	
	TMM_Scaling_State[sent_channel] = scaling_shutdown;
	TMM_Operating[sent_channel].tmm_operating_mode = No_Operating;
	motor_stop(sent_channel);

	Angle_Sent_Value_Tar[sent_channel] = 0u;                // clear angle sent target, and keep angle target

	tmm_scaling_stop_ctr[sent_channel] = 0u;
	Flag_Cancel_PID[sent_channel] = FALSE;
	Flag_TMM_Angle_Control[sent_channel]  = FALSE;
	Flag_Succeed_To_Target[sent_channel]  = FALSE;
	Flag_Single_Cycle[sent_channel] = FALSE;

	Flag_Tmm_Stalling[sent_channel] = FALSE;

	//if(Flag_Ref_Run_Status[sent_channel] == ref_start)
	//{
	//	Flag_Ref_Run_Status[sent_channel] = ref_failed;
//
	//	TMM_Endstop_Cur[sent_channel].sent_upper_endstop  = 2400u;
	//	TMM_Endstop_Cur[sent_channel].sent_lower_endstop  = 1700u;
	//	//tmm_reset_factors(sent_channel);
	//}
	
	//TMM_Angle_Tar[sent_channel] = 0u;
	
	//TMM_Position = TMM_Normal;
	TMM_Matrix_Num = 0u;
}


/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

void tmm_init(void)
{	
	sent_channel_e sent_channel;

	for(sent_channel = sent_channel_1; sent_channel < sent_channel_max; sent_channel++)
	{
		TMM_Endstop_Cur[sent_channel].angle_upper_endstop = TMM_UME_Default;
		TMM_Endstop_Cur[sent_channel].angle_lower_endstop = TMM_LME_Default;
		TMM_Endstop_Cur[sent_channel].sent_upper_endstop  = TMM_UME_SENT_Default;
		TMM_Endstop_Cur[sent_channel].sent_lower_endstop  = TMM_LME_SENT_Default;	
		
		//TMM_Angle_gain[sent_channel] = (uint32_t)(TMM_Endstop_Cur[sent_channel].sent_upper_endstop - TMM_Endstop_Cur[sent_channel].sent_lower_endstop)*TMM_Calc_gain;
		//TMM_Angle_gain[sent_channel] = TMM_Angle_gain[sent_channel]/(uint32_t)(TMM_Endstop_Cur[sent_channel].angle_upper_endstop - TMM_Endstop_Cur[sent_channel].angle_lower_endstop);
		//TMM_Angle_offset[sent_channel] = ((uint32_t)TMM_Endstop_Cur[sent_channel].sent_upper_endstop*TMM_Calc_gain) - ((uint32_t)TMM_Endstop_Cur[sent_channel].angle_upper_endstop*TMM_Angle_gain[sent_channel]);

		tmm_reset_factors(sent_channel);

		TMM_Operating[sent_channel].tmm_operating_mode = No_Operating;
		TMM_Scaling_State[sent_channel] = scaling_shutdown;
	}
	
	//PTT |= 0xc0;            /* pull pt6, pt7 */
	do_hbrige_1 = TRUE;
	do_hbrige_2 = TRUE;
	do_hbrige_3 = TRUE;
	do_hbrige_4 = TRUE;
}


/**
 *
 * tmm main interface.
 *
 */
void tmm_update(void)
{
	sent_channel_e sent_channel;

	for(sent_channel = sent_channel_1; sent_channel < sent_channel_max; sent_channel++)
	{
		if(TMM_Operating[sent_channel].tmm_operating_start)
		{
			tmm_diviation(sent_channel);
			tmm_stalling(sent_channel);
			tmm_operating(sent_channel);
			


			
		}
		else
		{
			motor_stop(sent_channel);
			tmm_scaling_shutdown(sent_channel);		
		}
	}
	
	//CAN_To_ANA();
}

/**
 *
 * tmm reset the factors between angle and sent
 *
 */
void tmm_reset_factors(sent_channel_e sent_channel)
{
	TMM_Angle_gain[sent_channel] = (uint32_t)(TMM_Endstop_Cur[sent_channel].sent_upper_endstop - TMM_Endstop_Cur[sent_channel].sent_lower_endstop)*TMM_Calc_gain;
	TMM_Angle_gain[sent_channel] = TMM_Angle_gain[sent_channel]/(uint32_t)(TMM_Endstop_Cur[sent_channel].angle_upper_endstop - TMM_Endstop_Cur[sent_channel].angle_lower_endstop);
	TMM_Angle_offset[sent_channel] = (uint32_t)TMM_Endstop_Cur[sent_channel].sent_upper_endstop*TMM_Calc_gain;
	TMM_Angle_offset[sent_channel] = TMM_Angle_offset[sent_channel] - ((uint32_t)TMM_Endstop_Cur[sent_channel].angle_upper_endstop*TMM_Angle_gain[sent_channel]);
}