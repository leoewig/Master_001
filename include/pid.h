/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    pid.h
 *
 * description:  
 *  			pid control header.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Feb-24  Jun Wu           initial version
 *
 *****************************************************************************/
#if !defined(PID_H)
#define      PID_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#include "regs.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/ 
#define PWM_PID_PER 200 

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
typedef struct PI 
{ 
	int Target;     //Desired Value
	int Uk;			//Uk
	int Udk;		//Udk
	int Uk_1; 		//Uk-1
	float P; 		//Proportional Const 
	float I; 		//Integral Const
	float D; 		//Integral Const
	int    b; 
	int ek_0;		//ek 
	int ek_1; 		//ek-1 
	int ek_2; 		//ek-2 
}PI;


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
//extern uint8_t PWM_PID_Min;
//extern uint8_t PWM_PID_Max;
/* speed PID */
extern float TMM_Position_KP;
extern float TMM_Position_KI;
extern float TMM_Position_KD;
extern int value_to_watch_B[9];
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/ 
void Speed_PIDInit(void) ;
int16_t Speed_PIDAdjust(sent_channel_e sent_channel, int16_t Next_Point, int16_t Target_Point);
void Position_PIDInit(void);
int16_t Position_PIDAdjust(sent_channel_e sent_channel, int16_t Next_Point, int16_t Target_Point);
void Speed_PID_Reset(sent_channel_e sent_channel);
#endif /* PWM_H */