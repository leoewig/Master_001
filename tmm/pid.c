/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    pid.c
 *
 * description:  
 *  			pid control.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Feb-18  Jun Wu           initial version
 *
 *****************************************************************************/
#include "motor.h"
#include "vio.h"
#include "pid.h"
#include "pwm.h"
#include "smm.h"
#include "xgate_icm.h"

/*****************************************************************************
 * MACROS
 *****************************************************************************/
//#define PWM_PID_PER 200 
#define Position_PID_Speed_Max	20
#define Position_PID_Speed_Min	0

#define PWM_PID_Max_B		980
#define PWM_PID_Min_B		100

/*****************************************************************************
 * CALIBRATABLE DATA
 *****************************************************************************/

 
/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/


static PI Speed_PID[sent_channel_max]; 
static PI (*Speed_Point)[sent_channel_max]= &Speed_PID; 

static PI Position_PID[sent_channel_max];
static PI (*Position_Point)[sent_channel_max]= &Position_PID; 


/******************************************************************************
 * Public DATA
 *****************************************************************************/ 
 
// uint8_t PWM_PID_Min = 10u;
//uint8_t PWM_PID_Max = 190u;


//extern CAL float KP; 		//Proportional Const 
//extern CAL float KI; 		//Integral Const 
//extern CAL float KD; 			//Derivative Const 

//float TMM_Speed_KP = 0.15f;       //Proportional Const 
//float TMM_Speed_KI = 0.003f;      //Integral Const 
//float TMM_Speed_KD = 5.0f;            //Derivative Const 

float TMM_Position_KP = 2.0f;
float TMM_Position_KI = 0.06f;
float TMM_Position_KD = 5.0f;

//#pragma align on
//PID Speed_PID_A=
//{
//	0u,
//	0u,
//	0u,
//	0u,
//	2.0f,
//	0.06f,
//	0.0f,
//	0u,
//	0u,
//	0u,
//	0u,
//}; 
//#pragma align off

int value_to_watch_B[9] = {0};
//static PID *Speed_PID_Ptr_A = &Speed_PID_A;
/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 *****************************************************************************/



/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
void Speed_PID_Reset(sent_channel_e sent_channel)
{
	if (sent_channel == sent_channel_2)
	{
		Speed_Point[sent_channel]->Uk		= 0;
		Speed_Point[sent_channel]->Udk    = 0;
		Speed_Point[sent_channel]->Uk_1   = 0;
		Speed_Point[sent_channel]->ek_0 	= 0;	 //ek=0
		Speed_Point[sent_channel]->ek_1 	= 0;	 //ek-1=0
		Speed_Point[sent_channel]->ek_2 	= 0; 	 //ek-2=0
		Speed_Point[sent_channel]->b      = 0;
	}

	//value_to_watch_A[3] = Speed_PID_A.Uk;
	//value_to_watch_A[4] = Speed_PID_A.Udk;
	//value_to_watch_A[5] = Speed_PID_A.Uk_1;
	//value_to_watch_A[6] = Speed_PID_A.ek_0;
	//value_to_watch_A[7] = Speed_PID_A.ek_1;
	//value_to_watch_A[8] = Speed_PID_A.ek_2;
}

 
/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/
 void Speed_PIDInit(void) 
{ 
	sent_channel_e i;

	for(i=sent_channel_1; i<sent_channel_max; i++)
	{
		Speed_Point[i]->Target = 0;
		Speed_Point[i]->Uk		= 0;
		Speed_Point[i]->Udk    = 0;
		Speed_Point[i]->Uk_1   = 0;
		Speed_Point[i]->ek_0 	= 0;	 //ek=0
		Speed_Point[i]->ek_1 	= 0;	 //ek-1=0
		Speed_Point[i]->ek_2 	= 0; 	 //ek-2=0
		Speed_Point[i]->P 		= 2.0f;  //Proportional Const 
		Speed_Point[i]->I 		= 0.06f; 	 //Integral Const
		Speed_Point[i]->D 		= 0.0f; 	 //Derivative Const
		Speed_Point[i]->b      = 0; 
	}	
}

int16_t Speed_PIDAdjust(sent_channel_e sent_channel, int16_t Next_Point, int16_t Target_Point) 
{

	int16_t P_Part;
	int16_t I_Part;
	int16_t D_Part;
	//int16_t ek_0_rawdate;
	
	//Speed_Point[sent_channel]->Target = Speed_Sent_Value_Tar[sent_channel_1]; 
	Speed_Point[sent_channel]->ek_0 =Target_Point - Next_Point; 	//incremental
	if(((Speed_Point[sent_channel]->Uk_1>=PWM_PID_Max)&&(Speed_Point[sent_channel]->ek_0>=0))||((Speed_Point[sent_channel]->Uk_1<=PWM_PID_Min)&&(Speed_Point[sent_channel]->ek_0<=0)))
	{
	    Speed_Point[sent_channel]->b=0;
	} 
	else
	{
		Speed_Point[sent_channel]->b=1;
	} 
	//Speed_Point[sent_channel]->Udk=(Speed_Point[sent_channel]->P*(Speed_Point[sent_channel]->ek_0-Speed_Point[sent_channel]->ek_1)+2000)/4000 + Speed_Point[sent_channel]->b*Speed_Point[sent_channel]->I*Speed_Point[sent_channel]->ek_0 ;
	
	//only used to monitor the value.
	value_to_watch_B[0] = Next_Point;
	value_to_watch_B[1] = Target_Point;
	value_to_watch_B[2] = Speed_Point[sent_channel]->Target;
	value_to_watch_B[3] = Speed_Point[sent_channel]->Uk;
	value_to_watch_B[4] = Speed_Point[sent_channel]->Udk;
	value_to_watch_B[5] = Speed_Point[sent_channel]->Uk_1;
	value_to_watch_B[6] = Speed_Point[sent_channel]->ek_0;
	value_to_watch_B[7] = Speed_Point[sent_channel]->ek_1;
	value_to_watch_B[8] = Speed_Point[sent_channel]->ek_2;


	P_Part = Speed_Point[sent_channel]->ek_0-Speed_Point[sent_channel]->ek_1;
	I_Part = Speed_Point[sent_channel]->b*Speed_Point[sent_channel]->ek_0;
	D_Part = Speed_Point[sent_channel]->ek_0 - 2*Speed_Point[sent_channel]->ek_1 +Speed_Point[sent_channel]->ek_2;
	
	//Speed_Point[sent_channel]->Udk=KP*(Speed_Point[sent_channel]->ek_0-Speed_Point[sent_channel]->ek_1) + Speed_Point[sent_channel]->b*KI*Speed_Point[sent_channel]->ek_0 + KD*(Speed_Point[sent_channel]->ek_0 - 2*Speed_Point[sent_channel]->ek_1 +Speed_Point[sent_channel]->ek_2) ;
	Speed_Point[sent_channel]->Udk = TMM_Speed_KP*P_Part + TMM_Speed_KI*I_Part + Speed_Point[sent_channel]->D*D_Part;

	Speed_Point[sent_channel]->Uk = Speed_Point[sent_channel]->Uk_1 + Speed_Point[sent_channel]->Udk;

	value_to_watch_B[3] = Speed_Point[sent_channel]->Uk;


	Speed_Point[sent_channel]->ek_2 =	Speed_Point[sent_channel]->ek_1;
	Speed_Point[sent_channel]->ek_1 = Speed_Point[sent_channel]->ek_0;
	Speed_Point[sent_channel]->Uk_1 = Speed_Point[sent_channel]->Uk; 
    //if(Speed_Point[sent_channel]->Uk >= PWM_PID_Max)
	//{
	//	return PWM_PID_Max;
	//}
	//else if(Speed_Point[sent_channel]->Uk <= PWM_PID_Min)
	//{
	//	return PWM_PID_Min;
	//} 
	//else
	//{
	//	return(Speed_Point[sent_channel]->Uk);
	//}
	if((Speed_Point[sent_channel]->Uk < -10000) || Speed_Point[sent_channel]->Uk > 5000)
	{
		Speed_PID_Reset(sent_channel);
		return PWM_PID_Max_B / 2;
	}
	else if(Speed_Point[sent_channel]->Uk >= PWM_PID_Max_B)
	{
		return PWM_PID_Max_B;
	}
	else if(Speed_Point[sent_channel]->Uk <= PWM_PID_Min_B)
	{
		return PWM_PID_Min_B;
	} 
	else
	{
		return(Speed_Point[sent_channel]->Uk);
	}
	 
}

/* Position PID control */
 void Position_PIDInit(void) 
{ 
	sent_channel_e i;

	for(i=sent_channel_1; i<sent_channel_max; i++)
	{
		Position_Point[i]->Target = 0;
		Position_Point[i]->Uk		= 0;
		Position_Point[i]->Udk    = 0;
		Position_Point[i]->Uk_1   = PWM_PID_Max;
		Position_Point[i]->ek_0 	= 0;	 //ek=0
		Position_Point[i]->ek_1 	= 0;	 //ek-1=0
		Position_Point[i]->ek_2 	= 0; 	 //ek-2=0
		Position_Point[i]->P 		= 0.25f;  //Proportional Const 
		Position_Point[i]->I 		= 0.003f; 	 //Integral Const
		Position_Point[i]->D 		= 0.0f; 	 //Derivative Const
		Position_Point[i]->b      = 0; 
	}	
}

int16_t Position_PIDAdjust(sent_channel_e sent_channel, int16_t Next_Point, int16_t Target_Point) 
{

	int16_t P_Part;
	int16_t I_Part;
	int16_t D_Part;

	Position_Point[sent_channel]->ek_0= Target_Point - Next_Point; 	//incremental
	//if(((Position_Point[sent_channel]->Uk_1>=PWM_PID_Max)&&(Position_Point[sent_channel]->ek_0>=0))||((Position_Point[sent_channel]->Uk_1<=PWM_PID_Min)&&(Position_Point[sent_channel]->ek_0<=0)))
	//{
	//    Position_Point[sent_channel]->b=0;
	//} 
	//else
	//{
	//	Position_Point[sent_channel]->b=1;
	//} 
	
	P_Part = Position_Point[sent_channel]->ek_0-Position_Point[sent_channel]->ek_1;
	I_Part = Position_Point[sent_channel]->b*Position_Point[sent_channel]->ek_0;
	D_Part = Position_Point[sent_channel]->ek_0 - 2*Position_Point[sent_channel]->ek_1 +Position_Point[sent_channel]->ek_2;
	
	Position_Point[sent_channel]->Udk = TMM_Position_KP*P_Part + TMM_Position_KI*I_Part + TMM_Position_KD*D_Part;

	Position_Point[sent_channel]->Uk = Position_Point[sent_channel]->Uk_1 + Position_Point[sent_channel]->Udk;
	Position_Point[sent_channel]->ek_2 = Position_Point[sent_channel]->ek_1;
	Position_Point[sent_channel]->ek_1 = Position_Point[sent_channel]->ek_0;
	Position_Point[sent_channel]->Uk_1 = Position_Point[sent_channel]->Uk; 
    if(Position_Point[sent_channel]->Uk >= Position_PID_Speed_Max)
	{
		return Position_PID_Speed_Max;
	}
	else if(Position_Point[sent_channel]->Uk <= Position_PID_Speed_Min)
	{
		return Position_PID_Speed_Min;
	} 
	else
	{
		return(Position_Point[sent_channel]->Uk);
	}
	 
}