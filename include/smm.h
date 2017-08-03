/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2017 - All rights reserved 
 *
 * file name:    smm.h
 *
 * description:  
 *  Sensors measurement management
 * 
 *
 * revision history:
 *
 * Date         Author           Description
 * 2017-Mar-21  Jun Wu           initial version
 *
 *****************************************************************************/
#if !defined(SMM_H)
#define      SMM_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#include "regs.h"
#include "smm_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/
#define Init_CheckSum 5u
#define  Sync_Duration_Ticks  56
#define  Sync_Duration_Max    201600             /* 3*(1+20%)*56 (us) */
#define  Sync_Duration_Min    134400             /* 3*(1-20%)*56 (us) */
#define  Sync_Duration_ERR_Min    108000             /* 3*(1+20%)*30 (us) */

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
//typedef enum
//{
//    sent_channel_1 = 0u,
//    sent_channel_2,
//    sent_channel_max
//} sent_channel_e;
//
//enum sent_state_e
//{
//    sent_state_sync = 0u,
//    sent_state_status,
//	sent_state_data1,
//	sent_state_data2,
//	sent_state_data3,
//	sent_state_data4,
//	sent_state_data5,
//	sent_state_data6,
//	sent_state_checksum,
//	sent_state_pause,            //for sent2010
//    sent_state_unknown
//};

typedef enum
{    
sensor_type_SENT,    
sensor_type_ANALOG,  			/* tbd */				
sensor_type_PWM, 				/* tbd */
sensor_type_LIN, 				/* tbd */
sensor_type_CAN, 				/* tbd */
sensor_type_Flexray				/* tbd */
} sensor_input_type_e;

typedef enum
{    
Sent_CRC_2008,    
Sent_CRC_2010
} sent_CRC_version_e;

typedef enum
{    
Sent_Dis_Pause_Pulse,    
Sent_En_Pause_Pulse
} sent_pause_pulse_e;

typedef enum
{    
Sent_No_SerMsg,    
Sent_ShortSerMsg,
Sent_EnhSerMsg_12bit,
Sent_EnhSerMsg_16bit
} sent_serial_message_e;

/* nibbler 4,5,6 */
typedef enum
{    
Sent_RollingCnter,    
Sent_Temperature
} sent_A1A3_mode_e;

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
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

/* previous sensor nibbles value*/
extern uint8_t sent_nibbles_value_prev[sent_channel_max][sent_state_unknown];

/* sensor time out counter */
//extern uint8_t SENT_TimeOut_CNT[sent_channel_max];  

/* sensor raw data of sent */
extern uint8_t Sent_Rawdata[sent_channel_max][8u];

/* sensor raw data counter of sent */
//extern uint8_t Sent_Rawdata_CNT[sent_channel_max];

/* sensor unvalid message counter of sent */
extern uint32_t Sent_Unvalid_cnt[sent_channel_max]; 

/* sensor total message counter of sent */
extern uint32_t Sent_Total_cnt[sent_channel_max]; 

/* TMM factors between angle and SENT */
extern uint32_t TMM_Angle_gain[sent_channel_max];
extern uint32_t TMM_Angle_offset[sent_channel_max];

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



/* ECU_SENT_A */
//extern uint8_t SENT_SENS_A_Nibble2;
//extern uint8_t SENT_SENS_A_Nibble1;
//extern uint8_t SENT_SENS_A_Nibble4;
//extern uint8_t SENT_SENS_A_Nibble3;
//extern uint8_t SENT_SENS_A_Nibble6;
//extern uint8_t SENT_SENS_A_Nibble5;
//extern uint8_t SENT_SENS_A_Nibble8;
//extern uint8_t SENT_SENS_A_Nibble7;
//extern bool_t SENT_ECUT_A_Error_SensorDiag;
//extern bool_t RTOS_ErrDistRefPos_A;
//extern bool_t RTOS_FlagValidSENT_A;
//extern bool_t RTOS_ErrGradBegr_A;
//extern uint8_t SENT_ECUT_A_CRC;
//extern bool_t SENT_ECUT_A_Error_NibbleLength;
//extern bool_t SENT_ECUT_A_Error_SyncLength;
//extern bool_t SENT_ECUT_A_Error_MissingNibble;
//extern bool_t SENT_ECUT_A_Error_TooManyNibbles;
//extern bool_t SENT_ECUT_A_Error_CRC;
//extern bool_t SENT_ECUT_A_Error_TimeOut;
//extern bool_t SENT_ECUT_A_Error_RollingCounter;
//extern bool_t SENT_ECUT_A_Error_InverseNibble;
//extern int16_t ANGL_SENS_A_ANA_RAW;


extern uint8_t SENT_ErrCount_A;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
INTERRUPT void s12_ECT_Channel1_Interrupt(void);
//INTERRUPT void s12_ECT_Channel2_Interrupt(void);
INTERRUPT void ECT_Channel2_Interrupt(void);
void Calc_Nibbles_value(sent_channel_e sent_channel, uint32_t tick_duration, uint32_t *nibbles_duration);
void sensor_speed_measure(sent_channel_e sent_channel);
void sensor_angle_measure(sent_channel_e sent_channel);
#endif /* SMM_H */
