/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    tmm_fm.c
 *
 * description:  
 *  			tmm failure management.
 * 
 *
 * revision history:
 *
 * Date         Author           Description
 * 2016-May-18  Jun Wu           initial version
 *
 *****************************************************************************/
#include "tmm_fm.h"
#include "smm.h"
#include "motor.h"
#include "vio.h"
#include "ai.h"
#include "tmm.h"
#include "dio_cfg.h"




/*****************************************************************************
 * MACROS
 *****************************************************************************/
#define SENT_TIMEOUT_MAX        10            /* 10ms */

#define KickOff_Trigger(flag, tri_num)		do{ if(flag)				\
												{						\
													tri_num = FALSE;		\
												}						\
												else					\
												{						\
													tri_num = TRUE;	\
												}						\
											  } while(0)

/*****************************************************************************
 * CALIBRATABLE DATA
 *****************************************************************************/

 
/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/
 /* Flag: motor stalled conter */
PRIVATE uint8_t Tmm_StallingCtr[sent_channel_max] = {0u};

PRIVATE uint32_t TMM_Speed_Raw[sent_channel_max][10u] = {0u};
PRIVATE uint32_t TMM_Speed_Avg_Cur[sent_channel_max] = {0u};
PRIVATE uint32_t TMM_Speed_Avg_Pre[sent_channel_max] = {0u};
PRIVATE uint16_t ErrCount_SENT_pre[sent_channel_max] = {0u};
PRIVATE uint32_t tmm_fm_ctr = 0u;

PRIVATE uint16_t Over_Curr_Cntr[sent_channel_max] = {0u};

/******************************************************************************
 * Public DATA
 *****************************************************************************/ 
uint16_t Err_SENT_TimeOut_10ms = 0u;
uint16_t tmm_motor_rotating_ctr = 0u;              /* over current */


//extern uint32_t Sent_Unvalid_cnt;


bool_t FLAG_TOUT_DIG_RAW[sent_channel_max] = {FALSE, FALSE};

bool_t ErrGradBegr[sent_channel_max] = {FALSE, FALSE};

tmm_ecut_flag_s TMM_ECUT_FLAG[sent_channel_max] = {
	{FALSE, FALSE, FALSE, FALSE}, 
	{FALSE, FALSE, FALSE, FALSE}
};

#pragma align on

uint16_t ErrCount_SENT[sent_channel_max] = {0u};

#pragma align off

/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 *****************************************************************************/

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
PRIVATE void tmm_sent_err_management(sent_channel_e sent_channel)
{
	uint8_t i;
	uint32_t speed_sum=0u;
	uint32_t delta_speed = 0u;
	uint16_t ErrCount_SENT_1s = 0u;
	
		/* Err_SENT_TimeOut_10ms */
	
	/* ErrCount_SENT */	
	
	
	/* ErrGradBegr , speed exceed */
	//if(sent_channel == sent_channel_1)
	//{
	//	TMM_Speed_Cur[sent_channel_1] = TMM_Speed_Cur_A;
	//}
//
	TMM_Speed_Raw[sent_channel][tmm_fm_ctr%10u] = TMM_Speed_Cur[sent_channel];
	
	
	if(tmm_fm_ctr%10 == 9u)
	{
		for(i=0;i<10;i++)
		{
			speed_sum += TMM_Speed_Raw[sent_channel][i];
		}
		TMM_Speed_Avg_Cur[sent_channel] = speed_sum/10;
		
		if((Motor_rotating[sent_channel])&&(!Flag_Cancel_PID[sent_channel]))
		{
			
			if((TMM_Speed_Cur[sent_channel]>0)&&(TMM_Angle_Deviation[sent_channel]>AngleDevMax))
			{
				if(TMM_Speed_Avg_Cur[sent_channel] > TMM_Speed_Avg_Pre[sent_channel])
				{
					delta_speed = TMM_Speed_Avg_Cur[sent_channel] - TMM_Speed_Avg_Pre[sent_channel];			
				}
				else
				{
					delta_speed = TMM_Speed_Avg_Pre[sent_channel] - TMM_Speed_Avg_Cur[sent_channel];
				}
				
				if(delta_speed > ShiftSpeedDevMax)
				{
					ErrGradBegr[sent_channel] = TRUE;
					TMM_ECUT_FLAG[sent_channel].ECUT_FLAG_2= TRUE;
				}
				else{
						ErrGradBegr[sent_channel] = FALSE; 
						TMM_ECUT_FLAG[sent_channel].ECUT_FLAG_2 = FALSE;
					}
				
			}
			else
			{
				ErrGradBegr[sent_channel] = FALSE; 
				TMM_ECUT_FLAG[sent_channel].ECUT_FLAG_2 = FALSE; 
			}
			
			TMM_Speed_Avg_Pre[sent_channel] = TMM_Speed_Avg_Cur[sent_channel];
		}
		else
		{
			ErrGradBegr[sent_channel] = FALSE; 
			TMM_ECUT_FLAG[sent_channel].ECUT_FLAG_2 = FALSE; 
		}
	}
	
	
	/* ECUT_FLAG_1 */
	if((tmm_fm_ctr%100)==99)
	{
		ErrCount_SENT_1s = Sent_Unvalid_cnt[sent_channel] - ErrCount_SENT_pre[sent_channel];
		
		TMM_ECUT_FLAG[sent_channel].ECUT_FLAG_1 = (ErrCount_SENT_1s > 30)? TRUE : FALSE;
				
		ErrCount_SENT_pre[sent_channel] = Sent_Unvalid_cnt[sent_channel];
		
	}
	
	
	/* ECUT_FLAG_2, angle jump, speed exceed */
	
	/* ECUT_FLAG_3, angle deviation */
	
	//AngleDevMax , 在堵转超过TimeToDev_Max 后，stop， 此时再判断deviation
	
	/* ECUT_FLAG_4 */
	if(CURR_DCMR_RMS_100ms[sent_channel] > (CURR_DCMR_MAX*1000))
	{
		
		Over_Curr_Cntr[sent_channel]++;
		
		if(Over_Curr_Cntr[sent_channel] > Over_Current_Max)           /* current over 5A than 4s, then stop */
		{
			TMM_ECUT_FLAG[sent_channel].ECUT_FLAG_4 = TRUE;
			motor_stop(sent_channel);
			//tmm_scaling_shutdown(sent_channel_1);	
			Over_Curr_Cntr[sent_channel] = 0;
		}
	}
	else
	{
		Over_Curr_Cntr[sent_channel] = 0;
	}	
	/* clear the error every 10ms */
	//Err_SENT_TimeOut_10ms = 0u;
}

/* */
PRIVATE void tmm_trigger(void)
{
	KickOff_Trigger(TMM_ECUT_FLAG[sent_channel_1].ECUT_FLAG_1, do_tmm_trigger_0);
	KickOff_Trigger(TMM_ECUT_FLAG[sent_channel_1].ECUT_FLAG_2, do_tmm_trigger_1);
	KickOff_Trigger(TMM_ECUT_FLAG[sent_channel_1].ECUT_FLAG_3, do_tmm_trigger_2);
	KickOff_Trigger(TMM_ECUT_FLAG[sent_channel_1].ECUT_FLAG_4, do_tmm_trigger_3);
	KickOff_Trigger(TMM_ECUT_FLAG[sent_channel_2].ECUT_FLAG_1, do_tmm_trigger_4);
	KickOff_Trigger(TMM_ECUT_FLAG[sent_channel_2].ECUT_FLAG_2, do_tmm_trigger_5);
	KickOff_Trigger(TMM_ECUT_FLAG[sent_channel_2].ECUT_FLAG_3, do_tmm_trigger_6);
	KickOff_Trigger(TMM_ECUT_FLAG[sent_channel_2].ECUT_FLAG_4, do_tmm_trigger_7);
}

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

void tmm_fm_update(void)
{	
	sent_channel_e sent_channel;

	//tmm_sent_err_management();

	/* angle deviation which leads to a PWM shut off after a certain time */

	
		for(sent_channel = sent_channel_1; sent_channel < sent_channel_max; sent_channel++)
		{
			if((TMM_Operating[sent_channel].tmm_operating_mode != PWM_To_ME_openloop) && \
				(Motor_rotating[sent_channel]) && \
				(Flag_Tmm_Stalling[sent_channel]) && \
				(TMM_Angle_Deviation[sent_channel] > AngleDevMax))
			{
				
				Tmm_StallingCtr[sent_channel]++;
				
				if(Tmm_StallingCtr[sent_channel] > TimeToDev_Max)           /* stalling than 1s, then stop */
				{

					FLAG_TOUT_DIG_RAW[sent_channel] = TRUE;

					TMM_ECUT_FLAG[sent_channel].ECUT_FLAG_3 = TRUE;
	
					TMM_Scaling_State[sent_channel] = scaling_shutdown;
					TMM_Operating[sent_channel].tmm_operating_mode = No_Operating;
					motor_stop(sent_channel);
				
					Angle_Sent_Value_Tar[sent_channel] = 0u;                // clear angle sent target, and keep angle target			
					Flag_Cancel_PID[sent_channel] = FALSE;
					Flag_TMM_Angle_Control[sent_channel]  = FALSE;
					Flag_Succeed_To_Target[sent_channel]  = FALSE;
					Flag_Single_Cycle[sent_channel] = FALSE;
	
					Tmm_StallingCtr[sent_channel] = 0;
				}
				else
				{
					FLAG_TOUT_DIG_RAW[sent_channel] = FALSE;
					Tmm_StallingCtr[sent_channel] = 0;
				}
			}

			tmm_sent_err_management(sent_channel);
		}
	/* shift speed deviation which leads to a PWM shut off after a certain time */

	tmm_trigger();
	tmm_fm_ctr++;
}

