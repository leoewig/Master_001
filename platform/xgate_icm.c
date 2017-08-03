/**
 *
 * Implementation of the XGATE portion of the internal comms protocol for the MMC
 *
 * @file
 * @ingroup xgate_icm
 * $Id: xgate_icm.c 2920 2012-07-18 15:55:00Z pr4 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#include "xgate_icm.h"     /* public header for this module */
#include "regs.h" 
//#include "xgate_icm_p.h"   /* private header for this module */

//#include "smm.h"

/*****************************************************************************
 * MACROS
 *****************************************************************************/
#define Limit_Speed_Angle_A			70u				/* about 5 degrees */
//#define Pwm_Limit_Speed_A			4248u			/* pwm_period 4998, (4998 - 4248)/4998 = 15%, so limit to 15% pwm */
#define Pwm_Limit_Speed_A			3749u			/* pwm_period 4998, (4998 - 3749u)/4998 = 15%, so limit to 15% pwm */
#define Pwm_Max_A					4748u			/* 95% */
#define Pwm_Min_A					250u			/* 5% */
#define Pwm_Period_A				4998u		
#define Pid_Factor_A				5u		
#define Pwm_PID_Max_A				980u			/* 95% */
#define Pwm_PID_Min_A				100u				/* 10% */

#define TS_TICKS_A_PIT_MAX_VALUE	65535
#define SENT_TIMEOUT_MAX_A			2000			/* 2000us*/
/******************************************************************************
 * COMPILE-TIME CHECKS
 *****************************************************************************/

/*
 * PUBLIC DATA (documented in header)
 */
/* ECU_SENT_A */
uint8_t SENT_SENS_A_Nibble2 = 0u;
uint8_t SENT_SENS_A_Nibble1 = 0u;
uint8_t SENT_SENS_A_Nibble4 = 0u;
uint8_t SENT_SENS_A_Nibble3 = 0u;
uint8_t SENT_SENS_A_Nibble6 = 0u;
uint8_t SENT_SENS_A_Nibble5 = 0u;
uint8_t SENT_SENS_A_Nibble8 = 0u;
uint8_t SENT_SENS_A_Nibble7 = 0u;
bool_t SENT_ECUT_A_Error_SensorDiag = FALSE;
//bool_t RTOS_ErrDistRefPos_A = FALSE;
//bool_t RTOS_FlagValidSENT_A = FALSE;
//bool_t RTOS_ErrGradBegr_A = FALSE;
uint8_t SENT_ECUT_A_CRC = 0u;
bool_t SENT_ECUT_A_Error_NibbleLength = FALSE;
bool_t SENT_ECUT_A_Error_SyncLength = FALSE;
bool_t SENT_ECUT_A_Error_MissingNibble = FALSE;
bool_t SENT_ECUT_A_Error_TooManyNibbles = FALSE;
bool_t SENT_ECUT_A_Error_CRC = FALSE;
bool_t SENT_ECUT_A_Error_TimeOut = FALSE;
bool_t SENT_ECUT_A_Error_RollingCounter = FALSE;
bool_t SENT_ECUT_A_Error_InverseNibble = FALSE;
int16_t ANGL_SENS_A_ANA_RAW = 0u;

//

int value_to_watch_A[9] = {0};





/* sensor raw data of sent */
uint8_t Sent_Rawdata_A[8u] = {0u};

///* sensor current angle of sent */
//uint32_t Angle_Sent_Value_Cur[sent_channel_max]={0u};
//
///* sensor current angle */
//int16_t  TMM_Angle_Cur[sent_channel_max]={0u};
//
///* sensor current speed of sent */
//uint32_t Speed_Sent_Value_Cur[sent_channel_max]={0u};
//
///* delta angle between target and current of sent */
//uint32_t delta_Position_Sent_value[sent_channel_max]={0u};
//
///* sensor target angle of sent */
//uint32_t Angle_Sent_Value_Tar[sent_channel_max]={0u};
//
///* sensor target speed of sent */
//uint32_t Speed_Sent_Value_Tar[sent_channel_max]={0u};
//
///* delta speed between target and current of sent */
//uint32_t delta_Speed_Sent_Value[sent_channel_max]={0u};
//
///* TMM current speed */
//uint32_t TMM_Speed_Cur[sent_channel_max]={0u};
//
///* sensor nibbles value*/
//uint8_t sent_nibbles_value[sent_channel_max][sent_state_unknown] = {0u};

/* sensor current angle of sent */
extern uint32_t Angle_Sent_Value_Cur[sent_channel_max];

/* sensor current angle */
extern int16_t  TMM_Angle_Cur[sent_channel_max];

/* sensor current speed of sent */
extern uint32_t Speed_Sent_Value_Cur[sent_channel_max];

/* delta angle between target and current of sent */
extern uint32_t delta_Position_Sent_value[sent_channel_max];

/* sensor target angle of sent */
extern uint32_t Angle_Sent_Value_Tar[sent_channel_max];

/* sensor target speed of sent */
extern uint32_t Speed_Sent_Value_Tar[sent_channel_max];

/* delta speed between target and current of sent */
extern uint32_t delta_Speed_Sent_Value[sent_channel_max];

/* TMM current speed */
extern uint32_t TMM_Speed_Cur[sent_channel_max];

/* sensor nibbles value*/
extern uint8_t sent_nibbles_value[sent_channel_max][sent_state_unknown];
extern uint32_t TMM_Angle_gain[sent_channel_max];
extern uint32_t TMM_Angle_offset[sent_channel_max];

extern bool_t Motor_rotating[sent_channel_max];

/* Flag: cancel PID for PWM control mode */
extern bool_t Flag_Cancel_PID[sent_channel_max];

extern uint16_t PWM_PID_Min;
extern uint16_t PWM_PID_Max;

extern float TMM_Speed_KP;       //Proportional Const 
extern float TMM_Speed_KI;      //Integral Const 

/* Flag: enable position PID */
extern bool_t Flag_Pos_PID_En;

/* ECU SENT A fault management */
extern bool_t RTOS_ErrDistRefPos_A;
extern bool_t RTOS_FlagValidSENT_A;
extern bool_t RTOS_ErrGradBegr_A;

extern bool_t Motor_Stoping_Flag_A;
/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

PRIVATE uint32_t Angle_Sent_Value_Pre_A=0u;
PRIVATE uint16_t Angle_Measure_Count_A=0u;
PRIVATE uint16_t Angle_Sent_Noise_Count_A=0u;
PRIVATE uint32_t Angle_Sent_Value_A[Angle_Sent_Sample_Times+1] = {0u};
PRIVATE uint32_t Angle_Sent_Sum_A=0u;
PRIVATE uint32_t angle_sent_avg_pre_A=0u;
PRIVATE uint32_t Speed_Sent_Value_A[Speed_Sent_Sample_Times+1] = {0u};
PRIVATE uint16_t Speed_Sent_Count_A=0u;
PRIVATE uint32_t Speed_Sent_Sum_A=0u;

PRIVATE uint16_t ts_low_ticks_A = 0u;
PRIVATE uint16_t ts_high_ticks_A = 0u;
PRIVATE uint32_t ts_cur_ticks_A = 0u;
PRIVATE uint32_t ts_pre_ticks_A = 0u;

PRIVATE uint32_t ts_cur_sent_ticks_A = 0u;
PRIVATE uint32_t ts_pre_sent_ticks_A = 0u;

PRIVATE PID Speed_PID_A=
{
	0u,
	0u,
	0u,
	0u,
	2.0f,
	0.06f,
	0.0f,
	0u,
	0u,
	0u,
	0u,
}; 


/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 *****************************************************************************/
PRIVATE int16_t Speed_PIDAdjust_A(sent_channel_e sent_channel, int16_t Next_Point, int16_t Target_Point);
PRIVATE uint32_t ts_get_ticks_A(void);
PRIVATE void Speed_PID_Resete(void);
/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/**
 * state variable
 */

void xgECU_SENT_A(void)
{
	uint8_t flags = 0u;
	int16_t integer_value = 0u;
	uint32_t delta_time = 0u;

    ts_cur_sent_ticks_A = ts_get_ticks_A();

    delta_time = (ts_cur_sent_ticks_A - ts_pre_sent_ticks_A)*5u;                  /* 1 tick 5us */

    if(delta_time > SENT_TIMEOUT_MAX_A)
    {
        SENT_ECUT_A_Error_TimeOut = TRUE;
    }
    else
    {
        SENT_ECUT_A_Error_TimeOut = FALSE;
    }

    ts_pre_sent_ticks_A = ts_cur_sent_ticks_A;

	
	/* Byte Order: Intel */
	Sent_Rawdata_A[0] = ((uint8_t)SENT_SENS_A_Nibble1 << 4u) | (uint8_t)SENT_SENS_A_Nibble2;
	Sent_Rawdata_A[1] = ((uint8_t)SENT_SENS_A_Nibble3 << 4u) | (uint8_t)SENT_SENS_A_Nibble4;
	Sent_Rawdata_A[2] = ((uint8_t)SENT_SENS_A_Nibble5 << 4u) | (uint8_t)SENT_SENS_A_Nibble6;
	Sent_Rawdata_A[3] = ((uint8_t)SENT_SENS_A_Nibble7 << 4u) | (uint8_t)SENT_SENS_A_Nibble8;

	flags = (SENT_ECUT_A_Error_SensorDiag)? 0x01 : 0x00;
	flags |= (RTOS_ErrDistRefPos_A)? 0x02 : 0x00;
	flags |= (RTOS_FlagValidSENT_A)? 0x04 : 0x00;
	flags |= (RTOS_ErrGradBegr_A)? 0x08 : 0x00;
	flags |= SENT_ECUT_A_CRC << 4u;
	Sent_Rawdata_A[4] = flags;
	
	flags = (SENT_ECUT_A_Error_NibbleLength)? 0x01 : 0x00;
	flags |= (SENT_ECUT_A_Error_SyncLength)? 0x02 : 0x00;
	flags |= (SENT_ECUT_A_Error_MissingNibble)? 0x04 : 0x00;
	flags |= (SENT_ECUT_A_Error_TooManyNibbles)? 0x08 : 0x00;
	flags |= (SENT_ECUT_A_Error_CRC)? 0x10 : 0x00;
	flags |= (SENT_ECUT_A_Error_TimeOut)? 0x20: 0x00;
	flags |= (SENT_ECUT_A_Error_RollingCounter)? 0x40 : 0x00;
	flags |= (SENT_ECUT_A_Error_InverseNibble)? 0x80 : 0x00;
	Sent_Rawdata_A[5] = flags;
	
	/* Byte Order: Motorola */
	integer_value = ANGL_SENS_A_ANA_RAW * 16; 
	Sent_Rawdata_A[6] = (uint8_t)(integer_value >> 8u);
	Sent_Rawdata_A[7] = (uint8_t)integer_value; 
	
  		//memcpy(p_CAN_MSG_BUFF[Tx_ECU_SENT_A],data, 8);
}


void sensor_angle_measure_A(sent_channel_e sent_channel)					//only SENT1 just now
{
	
//	uint32_t delta_sent_value;
//
//	
//	Angle_Sent_Value_Cur[sent_channel] = ((uint16_t)sent_nibbles_value[sent_channel][sent_state_data1] << 8u) | \
//					   ((uint16_t)sent_nibbles_value[sent_channel][sent_state_data2] << 4u) | \
//					   ((uint16_t)sent_nibbles_value[sent_channel][sent_state_data3]);
//	
//	if(Angle_Measure_Count_A > 1u)                                                         /*filter noise*/
//	{
//		if(Angle_Sent_Value_Cur[sent_channel] > Angle_Sent_Value_Pre_A )
//		{
//			delta_sent_value = Angle_Sent_Value_Cur[sent_channel] - Angle_Sent_Value_Pre_A;
//		}
//		else
//		{
//			delta_sent_value = Angle_Sent_Value_Pre_A - Angle_Sent_Value_Cur[sent_channel];
//		}
//		
//		if(delta_sent_value > Angle_Noise_Threshold)                                                 
//		{				
//			Angle_Sent_Noise_Count_A++;
//		}
//		else
//		{
//			Angle_Sent_Noise_Count_A = 0u;
//		}
//		
//		if((Angle_Sent_Noise_Count_A >0)&&(Angle_Sent_Noise_Count_A < Angle_Noise_Cnt_Threshold))
//		{
//			Angle_Sent_Value_Cur[sent_channel] = Angle_Sent_Value_Pre_A;
//		}	
//	}
//		
//	////not clarify SENT1 or SENT2
//	TMM_Angle_Cur[sent_channel] = (int16_t)((TMM_Calc_gain*(int32_t)Angle_Sent_Value_Cur[sent_channel] - (int32_t)TMM_Angle_offset[sent_channel] + (int32_t)TMM_Angle_gain[sent_channel]/2)/(int32_t)TMM_Angle_gain[sent_channel]);      
//	
//	Angle_Sent_Value_Pre_A = Angle_Sent_Value_Cur[sent_channel];
//
//    ANGL_SENS_A_ANA_RAW = TMM_Angle_Cur[sent_channel];
}


/**
 *
 * Sensor speed measurement
 *
 */
void sensor_speed_measure_A(sent_channel_e sent_channel)
{
	uint8_t i;	
	uint32_t delta_angle_sent = 0u;
	uint32_t angle_sent_avg= 0u;
	uint32_t angle_sent= 0u;	
	uint32_t speed_sent_avg= 0u;
	uint16_t PWM_DC;
	uint16_t PID_result;
	uint32_t delta_time = 0u;
	uint32_t sum = 0u;

	/* calculate angle sent average value for speed calculation */
	Angle_Sent_Value_A[Angle_Measure_Count_A] = Angle_Sent_Value_Cur[sent_channel] * Angle_Sent_Gain;
	
	//	if(Angle_Measure_Count_A == (Angle_Sent_Sample_Times-1u))
	//	{
	//		for(i=0u;i<Angle_Sent_Sample_Times;i++)
	//		{
	//			Angle_Sent_Sum_A += Angle_Sent_Value_A[i];
	//		}			
	//	}
	//	else if (Angle_Measure_Count_A == Angle_Sent_Sample_Times)
	//	{
	//		Angle_Sent_Sum_A = Angle_Sent_Sum_A - Angle_Sent_Value_A[0u] + Angle_Sent_Value_A[Angle_Measure_Count_A];
	//		
	//		for(i=1u;i<=Angle_Sent_Sample_Times;i++)
	//		{
	//			Angle_Sent_Value_A[i-1u] = Angle_Sent_Value_A[i];
	//		}	
	//		
	//		Angle_Measure_Count_A = Angle_Sent_Sample_Times - 1u;
	//	}
	//	angle_sent_avg = Angle_Sent_Sum_A/Angle_Sent_Sample_Times;      //still have Angle_Sent_Gain in the value

	Angle_Measure_Count_A++;
	if(Angle_Measure_Count_A >= Angle_Sent_Sample_Times)
	{
		for(i=0u; i<Angle_Sent_Sample_Times; i++)
		{
			sum += Angle_Sent_Value_A[i];
		}

		angle_sent_avg = sum/Angle_Sent_Sample_Times;      //still have Angle_Sent_Gain in the value

		ts_cur_ticks_A = ts_get_ticks_A();

		/* speed calculate */
		//if(TMM_Operating[sent_channel].tmm_operating_start&&Motor_rotating[sent_channel])	////not clarify SENT1 or SENT2
		if(Motor_rotating[sent_channel])	////not clarify SENT1 or SENT2
		//if(1)
		{
	
			if(angle_sent_avg> angle_sent_avg_pre_A)
			{
				delta_angle_sent = angle_sent_avg - angle_sent_avg_pre_A;
			}
			else
			{
				delta_angle_sent = angle_sent_avg_pre_A - angle_sent_avg;
			}
	
			delta_time = (ts_cur_ticks_A - ts_pre_ticks_A)*5u;					/* 1 tick 5us */
			//delta_time = 5000u;
	
			//Speed_Sent_Value_Cur[sent_channel] = delta_angle_sent*1000000u/Angle_Sent_Gain + Sample_Period/2u;
			//Speed_Sent_Value_Cur[sent_channel] = Speed_Sent_Value_Cur[sent_channel]/Sample_Period;
	
			Speed_Sent_Value_Cur[sent_channel] = delta_angle_sent*1000000u/Angle_Sent_Gain + delta_time/2u;
			Speed_Sent_Value_Cur[sent_channel] = Speed_Sent_Value_Cur[sent_channel]/delta_time;
	
			angle_sent_avg_pre_A  = angle_sent_avg;
	
			
			/*for limiting speed*/
			angle_sent = angle_sent_avg/Angle_Sent_Gain;
			if(angle_sent > Angle_Sent_Value_Tar[sent_channel])
			{
				delta_Position_Sent_value[sent_channel] = angle_sent - Angle_Sent_Value_Tar[sent_channel];
			}
			else
			{
				delta_Position_Sent_value[sent_channel] = Angle_Sent_Value_Tar[sent_channel] - angle_sent;
			}
					
	
			/*for speed average*/
				Speed_Sent_Value_A[Speed_Sent_Count_A] = Speed_Sent_Value_Cur[sent_channel];
				
				if(Speed_Sent_Count_A == (Speed_Sent_Sample_Times-1))
				{
					for(i=0u;i<Speed_Sent_Sample_Times;i++)
					{
						Speed_Sent_Sum_A += Speed_Sent_Value_A[i];
					}			
				}
				else if (Speed_Sent_Count_A == Speed_Sent_Sample_Times)
				{
					Speed_Sent_Sum_A = Speed_Sent_Sum_A - Speed_Sent_Value_A[0] + Speed_Sent_Value_A[Speed_Sent_Count_A];
					
					for(i=1u;i<=Speed_Sent_Sample_Times;i++)
					{
						Speed_Sent_Value_A[i-1] = Speed_Sent_Value_A[i];
					}	
					
					Speed_Sent_Count_A = Speed_Sent_Sample_Times - 1;
				}
					
				speed_sent_avg = Speed_Sent_Sum_A + Speed_Sent_Sample_Times/2;
				speed_sent_avg = speed_sent_avg/Speed_Sent_Sample_Times;

			if(Speed_Sent_Value_Tar[sent_channel] > speed_sent_avg )
			{
				delta_Speed_Sent_Value[sent_channel]  = Speed_Sent_Value_Tar[sent_channel] - speed_sent_avg;
			}
			else
			{
				delta_Speed_Sent_Value[sent_channel]  = speed_sent_avg - Speed_Sent_Value_Tar[sent_channel];
			}
			
			//if(Speed_Sent_Value_Tar[sent_channel] > Speed_Sent_Value_Cur[sent_channel] )
			//{
			//	delta_Speed_Sent_Value[sent_channel]  = Speed_Sent_Value_Tar[sent_channel] - Speed_Sent_Value_Cur[sent_channel];
			//}
			//else
			//{
			//	delta_Speed_Sent_Value[sent_channel]  = Speed_Sent_Value_Cur[sent_channel] - Speed_Sent_Value_Tar[sent_channel];
			//}
			
			
			Speed_Sent_Count_A++;
		}
	
		TMM_Speed_Cur[sent_channel] = (speed_sent_avg*TMM_Calc_gain + TMM_Angle_gain[sent_channel]/2)/TMM_Angle_gain[sent_channel];				////not clarify SENT1 or SENT2
		//TMM_Speed_Cur[sent_channel] = (uint8_t)(((uint32_t)TMM_Speed_Cur[sent_channel]*7656 + 8203) / 10000);
	
		//Angle_Measure_Count_A++;
	
		ts_pre_ticks_A = ts_cur_ticks_A;
	
		/* PID */
	
    		 if((Motor_rotating[sent_channel])&&(!Flag_Cancel_PID[sent_channel]))
    		{
	
    		    //if(!Flag_Cancel_PID[sent_channel]) 
    		    //{
    		        
    		        if((delta_Position_Sent_value[sent_channel] < Limit_Speed_Angle_A)&&(!Flag_Pos_PID_En))
    		        {
    		            //PWM_DC = 170u;
    		            PWM_DC = Pwm_Limit_Speed_A;
    		            if (Motor_Stoping_Flag_A)
    		            {
    		            	Speed_PID_Resete();
    		            }
    		        }
    		        else if(delta_Speed_Sent_Value[sent_channel] > 10)
    		        {
    		            PID_result = Speed_PIDAdjust_A(sent_channel, Speed_Sent_Value_Cur[sent_channel], Speed_Sent_Value_Tar[sent_channel])*Pid_Factor_A;
    		            //if(PID_result > Pwm_Max_A)
    		            //{
    		            //    PID_result = Pwm_Max_A;
    		            //}
    		            //else if(PID_result < Pwm_Min_A)
    		            //{
    		            //    PID_result = Pwm_Min_A;
    		            //}

    		            if(PID_result > PWM_PID_Max)
    		            {
    		                PID_result = PWM_PID_Max;
    		            }
    		            else if(PID_result < PWM_PID_Min)
    		            {
    		                PID_result = PWM_PID_Min;
    		            }
    		            PWM_DC = Pwm_Period_A - PID_result;
    		        }
    		        
    		        //pwm_duty_set(sent_channel, PWM_DC);
    		        PWMDTY0=(uint8_t)(PWM_DC>>8);
				PWMDTY1=(uint8_t)(PWM_DC);
				PWMDTY2=(uint8_t)(PWM_DC>>8);
				PWMDTY3=(uint8_t)(PWM_DC);
    		    //}
    	}

    	Angle_Measure_Count_A = 0u;
	}

}


/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
PRIVATE void Speed_PID_Resete(void)
{
	Speed_PID_A.Uk = 0;
	Speed_PID_A.Udk = 0;
	Speed_PID_A.Uk_1 = 0;
	Speed_PID_A.ek_0 = 0;	 //ek=0
	Speed_PID_A.ek_1 = 0;	 //ek-1=0
	Speed_PID_A.ek_2 = 0; 	 //ek-2=0
	Speed_PID_A.b = 0;
}

PRIVATE int16_t Speed_PIDAdjust_A(sent_channel_e sent_channel, int16_t Next_Point, int16_t Target_Point) 
{

	int16_t P_Part;
	int16_t I_Part;
	int16_t D_Part;

	
	//Speed_PID_A.Target = Speed_Sent_Value_Tar[sent_channel_1]; 
	Speed_PID_A.ek_0= Target_Point - Next_Point; 	//incremental
	if(((Speed_PID_A.Uk_1>=PWM_PID_Max)&&(Speed_PID_A.ek_0>=0))||((Speed_PID_A.Uk_1<=PWM_PID_Min)&&(Speed_PID_A.ek_0<=0)))
	{
	    Speed_PID_A.b=0;
	} 
	else
	{
		Speed_PID_A.b=1;
	} 
	//Speed_PID_A.Udk=(Speed_PID_A.P*(Speed_PID_A.ek_0-Speed_PID_A.ek_1)+2000)/4000 + Speed_PID_A.b*Speed_PID_A.I*Speed_PID_A.ek_0 ;
	
	value_to_watch_A[0] = Next_Point;
	value_to_watch_A[1] = Target_Point;
	value_to_watch_A[2] = Speed_PID_A.Target;
	value_to_watch_A[4] = Speed_PID_A.Udk;
	value_to_watch_A[5] = Speed_PID_A.Uk_1;
	value_to_watch_A[6] = Speed_PID_A.ek_0;
	value_to_watch_A[7] = Speed_PID_A.ek_1;
	value_to_watch_A[8] = Speed_PID_A.ek_2;

	P_Part = Speed_PID_A.ek_0-Speed_PID_A.ek_1;
	I_Part = Speed_PID_A.b*Speed_PID_A.ek_0;
	D_Part = Speed_PID_A.ek_0 - 2*Speed_PID_A.ek_1 +Speed_PID_A.ek_2;
	
	//Speed_PID_A.Udk = Speed_PID_A.P*P_Part + Speed_PID_A.I*I_Part + Speed_PID_A.D*D_Part;
	Speed_PID_A.Udk = TMM_Speed_KP*P_Part + TMM_Speed_KI*I_Part + Speed_PID_A.D*D_Part;
	

	Speed_PID_A.Uk = Speed_PID_A.Uk_1 + Speed_PID_A.Udk;

	value_to_watch_A[3] = Speed_PID_A.Uk;


	Speed_PID_A.ek_2 = Speed_PID_A.ek_1;
	Speed_PID_A.ek_1 = Speed_PID_A.ek_0;
	Speed_PID_A.Uk_1 = Speed_PID_A.Uk; 
	//if(Speed_PID_A.Uk >= PWM_PID_Max)
	//{
	//	return PWM_PID_Max;
	//}
	//else if(Speed_PID_A.Uk <= PWM_PID_Min)
	//{
	//	return PWM_PID_Min;
	//} 
	//else
	//{
	//	return(Speed_PID_A.Uk);
	//}
	if((Speed_PID_A.Uk < -5000) || Speed_PID_A.Uk > 5000)
	{
		Speed_PID_Resete();
		return Pwm_PID_Max_A / 2;
	}
	else if(Speed_PID_A.Uk >= Pwm_PID_Max_A)
	{
		return Pwm_PID_Max_A;
	}
	else if(Speed_PID_A.Uk <= Pwm_PID_Min_A)
	{
		return Pwm_PID_Min_A;
	} 
	else
	{
		return(Speed_PID_A.Uk);
	}

}

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
PRIVATE uint32_t ts_get_ticks_A(void)
{
    uint16_t newticks;
    uint32_t result;

    /* The PIT counts downwards from the upper limit, so switch it around */
    newticks = TS_TICKS_A_PIT_MAX_VALUE - PITCNT2;

    if (newticks < ts_low_ticks_A)
    {
        /* counter has wrapped */
        ++ts_high_ticks_A;
    }
    ts_low_ticks_A = newticks;
    result = ((uint32_t) ts_high_ticks_A << 16) | newticks;

    return result;
}