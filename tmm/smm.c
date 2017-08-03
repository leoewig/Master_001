/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2017 - All rights reserved 
 *
 * file name:    smm.c
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

/* MODULE sent */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "smm.h"
#include "ect.h"
#include "dio_cfg.h"
#include <string.h>
#include "vio.h"
#include "motor.h"
#include "tmm.h"
#include "gate.h"
#include "exc.h"
#include "exc_p.h"      /* for CAN_CYCLIC_STOP */
#include "ican.h"       /* for icantx */
#include "xgate_icm.h" 
#include "pwm.h"
#include "pid.h"
#include "ts.h"


/*****************************************************************************
 * MACROS
 *****************************************************************************/
#define Angle_Noise_Threshold	100u
#define Angle_Noise_Cnt_Threshold	10u
#define Sample_Period 1000            //us
#define Angle_Sent_Sample_Times 10
#define Angle_Sent_Gain 100
#define Speed_Sent_Sample_Times 10

#define Limit_Speed_Angle 70              /*about 5 degrees*/

#define SENT_TIMEOUT_MAX_B              2000u       /* 2000 us */
#define Unvalid_Count_MAX_B             100u

#define PWM_PID_Max_B       980
#define PWM_PID_Min_B       100
/*****************************************************************************
 * CALIBRATABLE DATA
 *****************************************************************************/

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/
//PRIVATE uint32_t Angle_Sent_Value_Pre[sent_channel_max]={0u};
PRIVATE uint16_t Angle_Measure_Count_B=0u;
//PRIVATE uint16_t Angle_Sent_Noise_Count[sent_channel_max]={0u};
PRIVATE uint32_t Angle_Sent_Value_B[Angle_Sent_Sample_Times+1] = {0u};
//PRIVATE uint32_t Angle_Sent_Sum[sent_channel_max]={0u};
PRIVATE uint32_t angle_sent_avg_pre_B=0u;
PRIVATE uint32_t Speed_Sent_Value_B[Speed_Sent_Sample_Times+1] = {0u};
PRIVATE uint16_t Speed_Sent_Count_B=0u;
PRIVATE uint32_t Speed_Sent_Sum_B=0u;


PRIVATE const uint8_t CRC4Table[256] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
13,12,15,14,9,8,11,10,5,4,7,6,1,0,3,2,
7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8,
10,11,8,9,14,15,12,13,2,3,0,1,6,7,4,5,
14,15,12,13,10,11,8,9,6,7,4,5,2,3,0,1,
3,2,1,0,7,6,5,4,11,10,9,8,15,14,13,12,
9,8,11,10,13,12,15,14,1,0,3,2,5,4,7,6,
4,5,6,7,0,1,2,3,12,13,14,15,8,9,10,11,
1,0,3,2,5,4,7,6,9,8,11,10,13,12,15,14,
12,13,14,15,8,9,10,11,4,5,6,7,0,1,2,3,
6,7,4,5,2,3,0,1,14,15,12,13,10,11,8,9,
11,10,9,8,15,14,13,12,3,2,1,0,7,6,5,4,
15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,
2,3,0,1,6,7,4,5,10,11,8,9,14,15,12,13,
8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,
5,4,7,6,1,0,3,2,13,12,15,14,9,8,11,10
};


/* sent diagnos */
PRIVATE bool_t Err_SENT_ECUT_CRC_B= FALSE;
PRIVATE bool_t Err_SENT_ECUT_NibbleLength_B = FALSE;
PRIVATE bool_t Err_SENT_ECUT_SyncLength_B = FALSE;  /* the former message is not valid, if true */
PRIVATE bool_t Err_SENT_ECUT_RollingCounter_B = FALSE;
PRIVATE bool_t Err_SENT_ECUT_InverseNibble_B = FALSE;
PRIVATE bool_t Err_SENT_ECUT_SensorDiag_B = FALSE;  /* sensor diagnos in status nibble */
PRIVATE bool_t FlagUnvalidSENT_B = FALSE;
//bool_t Err_SENT_ECUT_TimeOut = FALSE; 

PRIVATE uint8_t sent_sequence_num_B = 0u;
PRIVATE uint8_t sent_sequence_num_pre_B = 0u;
PRIVATE uint8_t delta_sequence_num_B = 0u;
PRIVATE uint8_t check_sum_value_B =0u;
PRIVATE uint32_t sequence_cnt_B = 0u;

//PRIVATE uint8_t sent_nibbles_value_prev[sent_channel_max][sent_state_unknown] = {0};


PRIVATE uint8_t sent_serial_cnter = 0u;
PRIVATE uint8_t sent_serial_start_cnter = 0u;
PRIVATE uint16_t sent_serial_suc_cnter = 0u;
PRIVATE bool_t Serial_Data_Start = FALSE;
PRIVATE bool_t EN_EnhSerMsg_16bit = FALSE;

PRIVATE uint8_t Serial_Data_CRC = 0u;
PRIVATE uint8_t Serial_Data_ID = 0u;
PRIVATE uint16_t Serial_Data = 0u;

PRIVATE uint32_t pre_capture_B = 0u;
PRIVATE uint32_t cur_capture_B = 0u;
PRIVATE enum sent_state_e TMM_sent_state_B = sent_state_unknown;
PRIVATE uint32_t tick_duration_B = 0u;          /*one sent tick time (us)*/
PRIVATE uint32_t sent_nibbles_duration_B[sent_state_unknown] = {0u};
PRIVATE bool_t One_Sent_Cycle_Completed_B = FALSE;

uint32_t Err_SENT_ECUT_NibbleLength_Count_B = 0u;

PRIVATE uint32_t ts_cur_ticks_B = 0u;
PRIVATE uint32_t ts_pre_ticks_B = 0u;

PRIVATE uint32_t ts_cur_sent_ticks_B = 0u;
PRIVATE uint32_t ts_pre_sent_ticks_B = 0u;


PRIVATE uint8_t Err_Too_Many_Nibbles_Ctr_B = 0u;
PRIVATE uint16_t Err_CRC_Ctr_B = 0u;

PRIVATE PI Speed_PID_B = 
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
 * Public DATA
 *****************************************************************************/
#pragma align on

/* sensor current angle of sent */
uint32_t Angle_Sent_Value_Cur[sent_channel_max]={0u};

/* sensor current angle */
int16_t  TMM_Angle_Cur[sent_channel_max]={0u};

/* sensor current speed of sent */
uint32_t Speed_Sent_Value_Cur[sent_channel_max]={0u};

/* delta angle between target and current of sent */
uint32_t delta_Position_Sent_value[sent_channel_max]={0u};

/* sensor target angle of sent */
uint32_t Angle_Sent_Value_Tar[sent_channel_max]={0u};

/* sensor target speed of sent */
uint32_t Speed_Sent_Value_Tar[sent_channel_max]={0u};

/* delta speed between target and current of sent */
uint32_t delta_Speed_Sent_Value[sent_channel_max]={0u};

/* TMM current speed */
uint32_t TMM_Speed_Cur[sent_channel_max]={0u};

/* sensor nibbles value*/
uint8_t sent_nibbles_value[sent_channel_max][sent_state_unknown] = {0u};

/* previous sensor nibbles value*/
uint8_t sent_nibbles_value_prev[sent_channel_max][sent_state_unknown] = {0};

/* sensor time out counter */
//uint8_t SENT_TimeOut_CNT[sent_channel_max]= {0u};  

/* sensor raw data of sent */
uint8_t Sent_Rawdata[sent_channel_max][8u] = {0u};

/* sensor raw data counter of sent */
//uint8_t Sent_Rawdata_CNT[sent_channel_max] = {0u};

/* sensor unvalid message counter of sent */
uint32_t Sent_Unvalid_cnt[sent_channel_max]= {0u}; 

/* sensor total message counter of sent */
uint32_t Sent_Total_cnt[sent_channel_max] = {0u}; 

/* TMM factors between angle and SENT */
uint32_t TMM_Angle_gain[sent_channel_max] = {0u};
uint32_t TMM_Angle_offset[sent_channel_max] = {0u};

/* Flag: cancel PID for PWM control mode */
bool_t Flag_Cancel_PID[sent_channel_max] = {FALSE, FALSE};

//uint8_t PWM_PID_Min = 10u;
//uint8_t PWM_PID_Max = 190u;

uint16_t PWM_PID_Min = 250u;
uint16_t PWM_PID_Max = 4748u;

float TMM_Speed_KP = 2.0f;       //Proportional Const 
float TMM_Speed_KI = 0.06f;      //Integral Const 

/* Flag: enable position PID */
bool_t Flag_Pos_PID_En = FALSE;

/* ECU SENT A fault management */
bool_t RTOS_ErrDistRefPos_A = FALSE;    // vio write
bool_t RTOS_FlagValidSENT_A = FALSE;    // xgate
bool_t RTOS_ErrGradBegr_A = FALSE;

uint8_t SENT_ErrCount_A = 0u;

#pragma align off

/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 *****************************************************************************/
void Calc_Nibbles_value_B(sent_channel_e sent_channel, uint32_t tick_duration, uint32_t *nibbles_duration);
PRIVATE void ECT_OnCapture_B(sent_channel_e sent_channel);
//PRIVATE void ECU_SENT_A(void);
//PRIVATE void ECU_SENT_B(void);
//PRIVATE void sensor_angle_measure(sent_channel_e sent_channel);
//PRIVATE void sensor_speed_measure(sent_channel_e sent_channel);

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
PRIVATE void Speed_PID_Reset_B(void)
{
    Speed_PID_B.Uk = 0;
    Speed_PID_B.Udk = 0;
    Speed_PID_B.Uk_1 = 0;
    Speed_PID_B.ek_0 = 0;    //ek=0
    Speed_PID_B.ek_1 = 0;    //ek-1=0
    Speed_PID_B.ek_2 = 0;    //ek-2=0
    Speed_PID_B.b = 0;
}

PRIVATE int16_t Speed_PIDAdjust_B(sent_channel_e sent_channel, int16_t Next_Point, int16_t Target_Point) 
{

    int16_t P_Part;
    int16_t I_Part;
    int16_t D_Part;
    //int16_t ek_0_rawdate;
    
    //Speed_PID_B.Target = Speed_Sent_Value_Tar[sent_channel_1]; 
    Speed_PID_B.ek_0 =Target_Point - Next_Point;     //incremental
    if(((Speed_PID_B.Uk_1>=PWM_PID_Max)&&(Speed_PID_B.ek_0>=0))||((Speed_PID_B.Uk_1<=PWM_PID_Min)&&(Speed_PID_B.ek_0<=0)))
    {
        Speed_PID_B.b=0;
    } 
    else
    {
        Speed_PID_B.b=1;
    } 
    //Speed_PID_B.Udk=(Speed_PID_B.P*(Speed_PID_B.ek_0-Speed_PID_B.ek_1)+2000)/4000 + Speed_PID_B.b*Speed_PID_B.I*Speed_PID_B.ek_0 ;
    
    //only used to monitor the value.


    P_Part = Speed_PID_B.ek_0-Speed_PID_B.ek_1;
    I_Part = Speed_PID_B.b*Speed_PID_B.ek_0;
    D_Part = Speed_PID_B.ek_0 - 2*Speed_PID_B.ek_1 +Speed_PID_B.ek_2;
    
    //Speed_PID_B.Udk=KP*(Speed_PID_B.ek_0-Speed_PID_B.ek_1) + Speed_PID_B.b*KI*Speed_PID_B.ek_0 + KD*(Speed_PID_B.ek_0 - 2*Speed_PID_B.ek_1 +Speed_PID_B.ek_2) ;
    Speed_PID_B.Udk = TMM_Speed_KP*P_Part + TMM_Speed_KI*I_Part + Speed_PID_B.D*D_Part;

    Speed_PID_B.Uk = Speed_PID_B.Uk_1 + Speed_PID_B.Udk;

    value_to_watch_B[3] = Speed_PID_B.Uk;


    Speed_PID_B.ek_2 =   Speed_PID_B.ek_1;
    Speed_PID_B.ek_1 = Speed_PID_B.ek_0;
    Speed_PID_B.Uk_1 = Speed_PID_B.Uk; 
    //if(Speed_PID_B.Uk >= PWM_PID_Max)
    //{
    //  return PWM_PID_Max;
    //}
    //else if(Speed_PID_B.Uk <= PWM_PID_Min)
    //{
    //  return PWM_PID_Min;
    //} 
    //else
    //{
    //  return(Speed_PID_B.Uk);
    //}
//    if((Speed_PID_B.Uk < -10000) || Speed_PID_B.Uk > 5000)
//    {
//        Speed_PID_Reset_B(sent_channel);
//        return PWM_PID_Max_B / 2;
//    }
    if(Speed_PID_B.Uk >= PWM_PID_Max_B)
    {
        return PWM_PID_Max_B;
    }
    else if(Speed_PID_B.Uk <= PWM_PID_Min_B)
    {
        return PWM_PID_Min_B;
    } 
    else
    {
        return(Speed_PID_B.Uk);
    }
     
}


/* Caculate CRC value */
PRIVATE uint8_t Sent_CRC_Check(sent_channel_e sent_channel, uint8_t *nibbles_value)
{   
    enum sent_state_e i;
    uint16_t tempCS;    
    uint8_t CheckSum = Init_CheckSum;
    
    for(i=sent_state_data1;i<sent_state_checksum;i++)
    {
        tempCS = nibbles_value[i] + CheckSum*16u;
        CheckSum = CRC4Table[tempCS];
    }
    
    if(TMM_Sent_CRC_Version[sent_channel] == Sent_CRC_2010)
    {
        tempCS = CheckSum*16u;
        CheckSum = CRC4Table[tempCS];
    }
        
    return CheckSum;
}

/* Caculate Serial Data Message */
PRIVATE void Calc_Serial_Data(sent_channel_e sent_channel)                  //not clarify SENT1 or SENT2 for serial data
{   
    uint8_t tmp_Serial_Data_CRC = 0u;
    
    //switch(TMM_SerMSg[sent_channel])
    //{
    //    case Sent_No_SerMsg:
    //        Serial_Data = 0u;
    //        Serial_Data_ID = 0u;
    //        Serial_Data_CRC = 0u;
    //        sent_serial_cnter = 0u;
    //        Serial_Data_Start = FALSE;
    //        break;
    //    case Sent_ShortSerMsg:
    //            if(sent_nibbles_value[sent_channel][sent_state_status]&0x08)            /* Serial Message Start */
    //            {
    //                Serial_Data_Start = TRUE;
    //                sent_serial_cnter = 0u;
    //            }
    //            sent_serial_cnter++;
    //            
    //            if(Serial_Data_Start == TRUE)
    //            {
    //                if(sent_serial_cnter < 5u)
    //                {
    //                    Serial_Data_ID = (uint8_t)((sent_nibbles_value[sent_channel][sent_state_status]&0x04) >> 2) << (4 - sent_serial_cnter);
    //                }
    //                else if(sent_serial_cnter < 13u) 
    //                {
    //                    Serial_Data = (uint16_t)((sent_nibbles_value[sent_channel][sent_state_status]&0x04) >> 2) << (12 - sent_serial_cnter);
    //                }
    //                else if(sent_serial_cnter < 17u) 
    //                {
    //                    Serial_Data_CRC = (uint8_t)((sent_nibbles_value[sent_channel][sent_state_status]&0x04) >> 2) << (16 - sent_serial_cnter);
    //                    
    //                    if(sent_serial_cnter == 16u)
    //                    {
    //                        sent_serial_cnter = 0;
    //                        Serial_Data_Start = FALSE;
    //                    }
    //                }
    //                
    //            }                   
    //            break;
    //    case Sent_EnhSerMsg_12bit:
    //            if(sent_nibbles_value[sent_channel][sent_state_status]&0x08) 
    //            {
    //                sent_serial_start_cnter++;
    //                tmp_Serial_Data_CRC |= (uint8_t)((sent_nibbles_value[sent_channel][sent_state_status]&0x08) >> 3) << (6 - sent_serial_start_cnter);
    //            }
    //            else 
    //            {
    //                sent_serial_start_cnter = 0u;
    //            }
    //    
    //            if(sent_serial_start_cnter == 6u)
    //            {
    //                Serial_Data_Start = TRUE;
    //                Serial_Data_CRC = tmp_Serial_Data_CRC;
    //                sent_serial_cnter = 5u;
    //            }
    //            sent_serial_cnter ++;
    //            
    //            if((Serial_Data_Start == TRUE)&&(sent_serial_cnter>6))
    //            {
    //                Serial_Data |= (uint16_t)((sent_nibbles_value[sent_channel][sent_state_status]&0x04) >> 2) << (18 - sent_serial_cnter);
    //                
    //                if(sent_serial_cnter == 8)
    //                {
    //                    EN_EnhSerMsg_16bit = (bool_t)((sent_nibbles_value[sent_channel][sent_state_status]&0x08) >> 3);              /* Configuration bit */
    //                }
    //                else if((sent_serial_cnter > 8)&&(sent_serial_cnter < 13))
    //                {
    //                    Serial_Data_ID |= (uint8_t)((sent_nibbles_value[sent_channel][sent_state_status]&0x08) >> 3) << (16 - sent_serial_cnter);
    //                }
    //                else if((sent_serial_cnter > 13)&&(sent_serial_cnter < 18))
    //                {
    //                    Serial_Data_ID |= (uint8_t)((sent_nibbles_value[sent_channel][sent_state_status]&0x08) >> 3) << (17 - sent_serial_cnter);
    //                }
    //                else if(sent_serial_cnter == 18)
    //                {
    //                    sent_serial_cnter = 0;
    //                    Serial_Data_Start = FALSE;
    //                    
    //                    if(EN_EnhSerMsg_16bit)                /* Configuration bit is set to 16 bit */
    //                    {
    //                        Serial_Data = 0u;
    //                        Serial_Data_ID = 0u;
    //                        Serial_Data_CRC = 0u;
    //                    }
    //                }
    //            
    //            }
    //            break;
    //    case Sent_EnhSerMsg_16bit:
    //            if(sent_nibbles_value[sent_channel][sent_state_status]&0x08) 
    //            {
    //                sent_serial_start_cnter++;
    //                tmp_Serial_Data_CRC |= (uint8_t)((sent_nibbles_value[sent_channel][sent_state_status]&0x08) >> 3) << (6 - sent_serial_start_cnter);
    //            }
    //            else 
    //            {
    //                sent_serial_start_cnter = 0u;
    //            }
    //    
    //            if(sent_serial_start_cnter == 6u)
    //            {
    //                Serial_Data_Start = TRUE;
    //                Serial_Data_CRC = tmp_Serial_Data_CRC;
    //                sent_serial_cnter = 5u;
    //            }
    //            sent_serial_cnter ++;
    //            
    //            if((Serial_Data_Start == TRUE)&&(sent_serial_cnter>6))
    //            {
    //                Serial_Data |= (uint16_t)((sent_nibbles_value[sent_channel][sent_state_status]&0x04) >> 2) << (18 - sent_serial_cnter);
    //                
    //                if(sent_serial_cnter == 8)
    //                {
    //                    EN_EnhSerMsg_16bit = (bool_t)((sent_nibbles_value[sent_channel][sent_state_status]&0x08) >> 3);              /* Configuration bit */
    //                }
    //                else if((sent_serial_cnter > 8)&&(sent_serial_cnter < 13))
    //                {
    //                    Serial_Data_ID |= (uint8_t)((sent_nibbles_value[sent_channel][sent_state_status]&0x08) >> 3) << (12 - sent_serial_cnter);
    //                }
    //                else if((sent_serial_cnter > 13)&&(sent_serial_cnter < 18))
    //                {
    //                    Serial_Data |= (uint16_t)((sent_nibbles_value[sent_channel][sent_state_status]&0x08) >> 3) << (29 - sent_serial_cnter);
    //                }
    //                else if(sent_serial_cnter == 18)
    //                {
    //                    sent_serial_cnter = 0;
    //                    Serial_Data_Start = FALSE;
    //                    
    //                    if(EN_EnhSerMsg_16bit==0u)                /* Configuration bit is set to 12 bit */
    //                    {
    //                        Serial_Data = 0u;
    //                        Serial_Data_ID = 0u;
    //                        Serial_Data_CRC = 0u;
    //                    }
    //                }
    //            
    //            }
    //            break;      
    //}
}

/* diagnos sent message */
PRIVATE void Sent_Diagnose(sent_channel_e sent_channel)
{
    /* clear error flag */
    Err_SENT_ECUT_CRC_B = FALSE; 
    Err_SENT_ECUT_NibbleLength_B = FALSE; 
    Err_SENT_ECUT_SyncLength_B = FALSE;  
    Err_SENT_ECUT_RollingCounter_B = FALSE; 
    Err_SENT_ECUT_InverseNibble_B = FALSE; 
    Err_SENT_ECUT_SensorDiag_B = FALSE;
    FlagUnvalidSENT_B = FALSE;
    
    /* Err_SENT_ECUT_CRC_B */
    check_sum_value_B = Sent_CRC_Check(sent_channel, &sent_nibbles_value[sent_channel][0u]);
    
    Err_SENT_ECUT_CRC_B = (check_sum_value_B==sent_nibbles_value[sent_channel][sent_state_checksum])? FALSE: TRUE;

    SENT_ECUT_B_Error_CRC = Err_SENT_ECUT_CRC_B;
    SENT_ECUT_B_CRC = check_sum_value_B;

    //if(Err_SENT_ECUT_CRC_B)
    //{
    //    Err_CRC_Ctr_B++;
    //
    //    if(Err_CRC_Ctr_B > CRC_ERR_MAX_B)
    //    {
    //        Err_CRC_Ctr_B = 0u;
    //    }
    //    else
    //    {
    //        SENT_ECUT_B_Error_CRC = FALSE;
    //        SENT_ECUT_B_CRC = sent_nibbles_value[sent_channel][sent_state_checksum];
    //    }
    //}
    
    /* Err_SENT_ECUT_NibbleLength_B */
    
    /* Err_SENT_ECUT_SyncLength_B */
    /* defined in Cap1_OnCapture() */
    
    /* Err_SENT_ECUT_RollingCounter_B */   
    //if(TMM_A1A3_Mode[sent_channel] == Sent_RollingCnter)        // no A1A3 for TMM B now
    if(TMM_Sent_CRC_Version[sent_channel] == Sent_CRC_2008)
    {
        sent_sequence_num_B = ((uint8_t)sent_nibbles_value[sent_channel][sent_state_data4] << 4u) | (uint8_t)sent_nibbles_value[sent_channel][sent_state_data5];
            
        delta_sequence_num_B = sent_sequence_num_B - sent_sequence_num_pre_B;
    
        if(delta_sequence_num_B!=1)
        {
            sequence_cnt_B++;
            Err_SENT_ECUT_RollingCounter_B = TRUE;
        }
        else
        {
            Err_SENT_ECUT_RollingCounter_B = FALSE;
        }

         /* Err_SENT_ECUT_InverseNibble_B */
        Err_SENT_ECUT_InverseNibble_B = ((sent_nibbles_value[sent_channel][sent_state_data1] + sent_nibbles_value[sent_channel][sent_state_data6]) == 15)? FALSE: TRUE;

    }
    else 
    {
        Err_SENT_ECUT_RollingCounter_B = FALSE;
        Err_SENT_ECUT_InverseNibble_B = FALSE;
    }
    
        
     
    /* Err_SENT_ECUT_SensorDiag_B */
    Err_SENT_ECUT_SensorDiag_B = (sent_nibbles_value[sent_channel][sent_state_status]&0x02) ? TRUE : FALSE;
    
    /* FlagUnvalidSENT_B */
    FlagUnvalidSENT_B = Err_SENT_ECUT_CRC_B | Err_SENT_ECUT_NibbleLength_B | Err_SENT_ECUT_RollingCounter_B | Err_SENT_ECUT_InverseNibble_B | Err_SENT_ECUT_SensorDiag_B;
    FlagUnvalidSENT_B |= SENT_ECUT_B_Error_TooManyNibbles;

    if(FlagUnvalidSENT_B)
    {
        Sent_Unvalid_cnt[sent_channel]++;                     //not clarify SENT1 or SENT2
        //memcpy(sent_nibbles_value,sent_nibbles_value_prev, sent_state_unknown);

        if(Sent_Unvalid_cnt[sent_channel] > Unvalid_Count_MAX_B)
        {
            Sent_Unvalid_cnt[sent_channel] = 0u;
        }
        else
        {    
            SENT_ECUT_B_Error_CRC = FALSE;
            SENT_ECUT_B_CRC = sent_nibbles_value[sent_channel][sent_state_checksum];
            FlagUnvalidSENT_B = FALSE;
            Err_SENT_ECUT_NibbleLength_B = FALSE;
            Err_SENT_ECUT_SyncLength_B = FALSE;
            SENT_ECUT_B_Error_MissingNibble = FALSE;
            SENT_ECUT_B_Error_TooManyNibbles = FALSE;
            Err_SENT_ECUT_RollingCounter_B = FALSE;
            Err_SENT_ECUT_InverseNibble_B = FALSE;
        }
    }

    Sent_Total_cnt[sent_channel]++; 

    /* ErrCount_SENT */     //not clarify SENT1 or SENT2
    //ErrCount_SENT += Err_SENT_ECUT_CRC_B + Err_SENT_ECUT_NibbleLength_B + Err_SENT_ECUT_SyncLength_B + Err_SENT_ECUT_RollingCounter_B + Err_SENT_ECUT_InverseNibble_B + Err_SENT_ECUT_SensorDiag_B; 
    
    sent_sequence_num_pre_B = sent_sequence_num_B;
}

/* store raw data */
PRIVATE void Store_Sent_Rawdata(sent_channel_e sent_channel)
{   
    uint8_t flags = 0u;
    int16_t integer_value = 0u;
    uint32_t delta_time = 0u;


        /* CAN Slow */
        //SENT_ErrCount_B = Sent_Unvalid_cnt[sent_channel];             // in vio_write
    if(Sent_Unvalid_cnt[sent_channel_1] == 50u)
    {
        SENT_ErrCount_A++;
    }
    
    if(Sent_Unvalid_cnt[sent_channel_2] == 50u)
    {
        SENT_ErrCount_B++;
    }
    
        /* CAN Fast */
//        SENT_SENS_B_Nibble1 = sent_nibbles_value[sent_channel][sent_state_status];
//        SENT_SENS_B_Nibble2 = sent_nibbles_value[sent_channel][sent_state_data1];
//        SENT_SENS_B_Nibble3 = sent_nibbles_value[sent_channel][sent_state_data2];
//        SENT_SENS_B_Nibble4 = sent_nibbles_value[sent_channel][sent_state_data3];
//        SENT_SENS_B_Nibble5 = sent_nibbles_value[sent_channel][sent_state_data4];
//        SENT_SENS_B_Nibble6 = sent_nibbles_value[sent_channel][sent_state_data5];
//        SENT_SENS_B_Nibble7 = sent_nibbles_value[sent_channel][sent_state_data6];
//        SENT_SENS_B_Nibble8 = sent_nibbles_value[sent_channel][sent_state_checksum];
//        
//        SENT_ECUT_B_Error_SensorDiag = Err_SENT_ECUT_SensorDiag_B;
//        //RTOS_ErrDistRefPos_B                      // in vio_write
        
//        //RTOS_ErrGradBegr_B                        // in vio_write
//        SENT_ECUT_B_CRC = check_sum_value_B;
//        SENT_ECUT_B_Error_NibbleLength = Err_SENT_ECUT_NibbleLength_B;
//        SENT_ECUT_B_Error_SyncLength = Err_SENT_ECUT_SyncLength_B;
//        //SENT_ECUT_B_Error_MissingNibble
//        SENT_ECUT_B_Error_CRC = Err_SENT_ECUT_CRC_B;

        RTOS_FlagValidSENT_B = !FlagUnvalidSENT_B;

        ts_cur_sent_ticks_B = ts_get_ticks();
        
        delta_time = (ts_cur_sent_ticks_B - ts_pre_sent_ticks_B)*5u;                  /* 1 tick 5us */
        
        if(delta_time > SENT_TIMEOUT_MAX_B)
        {
            SENT_ECUT_B_Error_TimeOut = TRUE;
        }
        else
        {
            SENT_ECUT_B_Error_TimeOut = FALSE;
        }
        
        ts_pre_sent_ticks_B = ts_cur_sent_ticks_B;

        //SENT_ECUT_B_Error_TimeOut
 //       SENT_ECUT_B_Error_RollingCounter = Err_SENT_ECUT_RollingCounter_B;
 //       SENT_ECUT_B_Error_InverseNibble = Err_SENT_ECUT_InverseNibble_B;
        //ANGL_SENS_B_ANA_RAW = TMM_Angle_Cur[sent_channel];                                    //move to angle measure

 
        /* Byte Order: Intel */
        Sent_Rawdata[sent_channel_2][0] = ((uint8_t)sent_nibbles_value[sent_channel_2][sent_state_status] << 4u) | (uint8_t)sent_nibbles_value[sent_channel_2][sent_state_data1];
        Sent_Rawdata[sent_channel_2][1] = ((uint8_t)sent_nibbles_value[sent_channel_2][sent_state_data2] << 4u) | (uint8_t)sent_nibbles_value[sent_channel_2][sent_state_data3];
        Sent_Rawdata[sent_channel_2][2] = ((uint8_t)sent_nibbles_value[sent_channel_2][sent_state_data4] << 4u) | (uint8_t)sent_nibbles_value[sent_channel_2][sent_state_data5];
        Sent_Rawdata[sent_channel_2][3] = ((uint8_t)sent_nibbles_value[sent_channel_2][sent_state_data6] << 4u) | (uint8_t)sent_nibbles_value[sent_channel_2][sent_state_checksum];
    
        flags = (Err_SENT_ECUT_SensorDiag_B)? 0x01 : 0x00;
        flags |= (RTOS_ErrDistRefPos_B)? 0x02 : 0x00;
        flags |= (FlagUnvalidSENT_B)? 0x04 : 0x00;
        flags |= (RTOS_ErrGradBegr_B)? 0x08 : 0x00;
        flags |= SENT_ECUT_B_CRC<< 4u;
        Sent_Rawdata[sent_channel_2][4] = flags;
    
        flags = (Err_SENT_ECUT_NibbleLength_B)? 0x01 : 0x00;
        flags |= (Err_SENT_ECUT_SyncLength_B)? 0x02 : 0x00;
        flags |= (SENT_ECUT_B_Error_MissingNibble)? 0x04 : 0x00;
        flags |= (SENT_ECUT_B_Error_TooManyNibbles)? 0x08 : 0x00;
        flags |= (SENT_ECUT_B_Error_CRC)? 0x10 : 0x00;
        flags |= (SENT_ECUT_B_Error_TimeOut)? 0x20: 0x00;
        flags |= (Err_SENT_ECUT_RollingCounter_B)? 0x40 : 0x00;
        flags |= (Err_SENT_ECUT_InverseNibble_B)? 0x80 : 0x00;
        Sent_Rawdata[sent_channel_2][5] = flags;
    
        /* Byte Order: Motorola */
        integer_value = ANGL_SENS_B_ANA_RAW * 16; 
        Sent_Rawdata[sent_channel_2][6] = (uint8_t)(integer_value >> 8u);
        Sent_Rawdata[sent_channel_2][7] = (uint8_t)integer_value; 

    
    
    
    if(Err_SENT_ECUT_CRC_B)
    {
        memcpy(&sent_nibbles_value[sent_channel][0u], &sent_nibbles_value_prev[sent_channel][0u], sent_state_unknown);
    }
    
    memcpy(&sent_nibbles_value_prev[sent_channel][0u],&sent_nibbles_value[sent_channel][0u], sent_state_unknown);
        

}

/**
 *
 * Caculate nibbles value
 *
 */
void Calc_Nibbles_value_B(sent_channel_e sent_channel, uint32_t tick_duration, uint32_t *nibbles_duration)
{
    enum sent_state_e i;
    uint32_t temp_value;
    
    for(i=sent_state_status;i<sent_state_unknown;i++)
    {
        temp_value = (nibbles_duration[i]+tick_duration/2);
        temp_value = temp_value/tick_duration;
        sent_nibbles_value[sent_channel][i] = temp_value;
        
        if(i<sent_state_pause)
        {
            sent_nibbles_value[sent_channel][i] = sent_nibbles_value[sent_channel][i] - 12;
            
            if (sent_nibbles_value[sent_channel][i] > 15u)
            {
                Err_SENT_ECUT_NibbleLength_B = TRUE;
                Err_SENT_ECUT_NibbleLength_Count_B++;
            }
        }
    }   

}

/**
 *
 * ECT capture sent signals, SENT1 is ECT1, SENT2 is ECT2
 *
 */
PRIVATE void ECT_OnCapture_B(sent_channel_e sent_channel)
{
    uint16_t capture=0u;
    uint32_t nibbles_duration = 0u;       /*us*/

    /* calculate nibbles duration time */
    switch(sent_channel)
    {
        case sent_channel_1:
                cur_capture_B = TC1;
                break;
        case sent_channel_2:
                cur_capture_B = TC2;
                break;
        default:
                break;
    }
    capture = cur_capture_B - pre_capture_B;
    //nibbles_duration = (uint32_t)capture*1000u/50u;     /* capture/50 us , multiply 1000 for accuracy */
    nibbles_duration = (uint32_t)capture*20u;
            
    pre_capture_B = cur_capture_B;

    
    /* find the sync duration */
    if(TMM_sent_state_B == sent_state_unknown)                   /* when state is unknown, check sync */   
    {
        if ((nibbles_duration>=Sync_Duration_Min)&&(nibbles_duration<=Sync_Duration_Max))           
        {
            TMM_sent_state_B = sent_state_sync;
            tick_duration_B = nibbles_duration/Sync_Duration_Ticks;
            sent_nibbles_value[sent_channel][sent_state_sync] = Sync_Duration_Ticks;

            if(TMM_EN_Pause_Pulse[sent_channel] == Sent_En_Pause_Pulse)
            {
                if((Err_Too_Many_Nibbles_Ctr_B < 10u)&&(Err_Too_Many_Nibbles_Ctr_B > 0u))
                {
                    SENT_ECUT_B_Error_MissingNibble = TRUE;
                }
                else if( Err_Too_Many_Nibbles_Ctr_B > 10u)
                {
                    SENT_ECUT_B_Error_TooManyNibbles = TRUE;
                }
                else
                {
                    SENT_ECUT_B_Error_MissingNibble = FALSE;
                    SENT_ECUT_B_Error_TooManyNibbles = FALSE;
                }
            }
            else
            {
                if((Err_Too_Many_Nibbles_Ctr_B < 9u)&&(Err_Too_Many_Nibbles_Ctr_B > 0u))
                {
                    SENT_ECUT_B_Error_MissingNibble = TRUE;
                }
                else if( Err_Too_Many_Nibbles_Ctr_B > 9u)
                {
                    SENT_ECUT_B_Error_TooManyNibbles = TRUE;
                }
                else
                {
                    SENT_ECUT_B_Error_MissingNibble = FALSE;
                    SENT_ECUT_B_Error_TooManyNibbles = FALSE;
                }
            }
            
            Err_Too_Many_Nibbles_Ctr_B = 0u;

        }
        else if (((nibbles_duration > Sync_Duration_ERR_Min)&&(nibbles_duration < Sync_Duration_Min)) || (nibbles_duration > Sync_Duration_Max))  /* sync length error */
        {
            Err_SENT_ECUT_SyncLength_B = TRUE;
        }
    }

    /* get other nibbles duration time */
    if(TMM_EN_Pause_Pulse[sent_channel] == Sent_En_Pause_Pulse)
    {
        switch(TMM_sent_state_B)
        {
            case sent_state_sync:
                sent_nibbles_duration_B[sent_state_sync] = nibbles_duration;
                TMM_sent_state_B = sent_state_status;           
                break;
            case sent_state_status:
                sent_nibbles_duration_B[sent_state_status] = nibbles_duration;
                TMM_sent_state_B = sent_state_data1;    
                break;
            case sent_state_data1:
                sent_nibbles_duration_B[sent_state_data1] = nibbles_duration;
                TMM_sent_state_B = sent_state_data2;    
                break;
            case sent_state_data2:
                sent_nibbles_duration_B[sent_state_data2] = nibbles_duration;
                TMM_sent_state_B = sent_state_data3;    
                break;
            case sent_state_data3:
                sent_nibbles_duration_B[sent_state_data3] = nibbles_duration;
                TMM_sent_state_B = sent_state_data4;    
                break;
            case sent_state_data4:
                sent_nibbles_duration_B[sent_state_data4] = nibbles_duration;
                TMM_sent_state_B = sent_state_data5;    
                break;
            case sent_state_data5:
                sent_nibbles_duration_B[sent_state_data5] = nibbles_duration;
                TMM_sent_state_B = sent_state_data6;    
                break;
            case sent_state_data6:
                sent_nibbles_duration_B[sent_state_data6] = nibbles_duration;
                TMM_sent_state_B = sent_state_checksum; 
                break;
            case sent_state_checksum:
                sent_nibbles_duration_B[sent_state_checksum] = nibbles_duration;
                TMM_sent_state_B = sent_state_pause;    
                break;
            case sent_state_pause:
                sent_nibbles_duration_B[sent_state_pause] = nibbles_duration;
                
                //PORTK|=0x01;

                One_Sent_Cycle_Completed_B = TRUE;
                Calc_Nibbles_value_B(sent_channel, tick_duration_B, &sent_nibbles_duration_B[sent_state_sync]);                 
                //Calc_Serial_Data(sent_channel);
                Sent_Diagnose(sent_channel);
                Store_Sent_Rawdata(sent_channel);
                //ECU_SENT_B(); 
                sensor_angle_measure(sent_channel);  

                icantx(0x211, 0u, 8, (FPTR const uint8_t *)&Sent_Rawdata[sent_channel_2][0]);                   
                sensor_speed_measure(sent_channel);    

                 //PORTK &= ~0x01;        
                
                //Err_Too_Many_Nibbles_Ctr_B = 0u;                         
                TMM_sent_state_B = sent_state_unknown; 
                
                break;
            default: 
                TMM_sent_state_B = sent_state_unknown;  
                //Err_Too_Many_Nibbles_Ctr_B++;
                break;
        }
    }
    else
    {
       switch(TMM_sent_state_B)
       {
           case sent_state_sync:
               sent_nibbles_duration_B[sent_state_sync] = nibbles_duration;
               TMM_sent_state_B = sent_state_status;           
               break;
           case sent_state_status:
               sent_nibbles_duration_B[sent_state_status] = nibbles_duration;
               TMM_sent_state_B = sent_state_data1;    
               break;
           case sent_state_data1:
               sent_nibbles_duration_B[sent_state_data1] = nibbles_duration;
               TMM_sent_state_B = sent_state_data2;    
               break;
           case sent_state_data2:
               sent_nibbles_duration_B[sent_state_data2] = nibbles_duration;
               TMM_sent_state_B = sent_state_data3;    
               break;
           case sent_state_data3:
               sent_nibbles_duration_B[sent_state_data3] = nibbles_duration;
               TMM_sent_state_B = sent_state_data4;    
               break;
           case sent_state_data4:
               sent_nibbles_duration_B[sent_state_data4] = nibbles_duration;
               TMM_sent_state_B = sent_state_data5;    
               break;
           case sent_state_data5:
               sent_nibbles_duration_B[sent_state_data5] = nibbles_duration;
               TMM_sent_state_B = sent_state_data6;    
               break;
           case sent_state_data6:
               sent_nibbles_duration_B[sent_state_data6] = nibbles_duration;
               TMM_sent_state_B = sent_state_checksum; 
               break;
           case sent_state_checksum:
               sent_nibbles_duration_B[sent_state_checksum] = nibbles_duration;
               
               One_Sent_Cycle_Completed_B = TRUE;
               Calc_Nibbles_value_B(sent_channel, tick_duration_B, &sent_nibbles_duration_B[sent_state_sync]);                 
               //Calc_Serial_Data(sent_channel);
               Sent_Diagnose(sent_channel);
               Store_Sent_Rawdata(sent_channel);  
               //ECU_SENT_B();
               sensor_angle_measure(sent_channel);
               icantx(0x211, 0u, 8, (FPTR const uint8_t *)&Sent_Rawdata[sent_channel_2][0]);   
               sensor_speed_measure(sent_channel);           
               
               //Sent_Rawdata_CNT[sent_channel]++;                         /* will be clear when raw data sent to can */                                     
               TMM_sent_state_B = sent_state_unknown;  
               //Err_Too_Many_Nibbles_Ctr_B = 0u;
               break;
           default: 
               TMM_sent_state_B = sent_state_unknown;  
               //Err_Too_Many_Nibbles_Ctr_B++;
               break;
       }
    }
    
    //SENT_TimeOut_CNT[sent_channel]++;
    Err_Too_Many_Nibbles_Ctr_B++;

}


/**
 *
 * Sensor angle measurement:  ANGL_SENS_SENT_ACTUAL_MEA, ANGL_SENS_ACTUAL_MEA
 *
 */
void sensor_angle_measure(sent_channel_e sent_channel)					//only SENT1 just now
{
	
	//uint32_t delta_sent_value;

	
	Angle_Sent_Value_Cur[sent_channel] = ((uint16_t)sent_nibbles_value[sent_channel][sent_state_data1] << 8u) | \
					   ((uint16_t)sent_nibbles_value[sent_channel][sent_state_data2] << 4u) | \
					   ((uint16_t)sent_nibbles_value[sent_channel][sent_state_data3]);
		
	////not clarify SENT1 or SENT2
	TMM_Angle_Cur[sent_channel] = (int16_t)((TMM_Calc_gain*(int32_t)Angle_Sent_Value_Cur[sent_channel] - (int32_t)TMM_Angle_offset[sent_channel] + (int32_t)TMM_Angle_gain[sent_channel]/2)/(int32_t)TMM_Angle_gain[sent_channel]);      
	
	//Angle_Sent_Value_Pre[sent_channel] = Angle_Sent_Value_Cur[sent_channel];

    ANGL_SENS_B_ANA_RAW = TMM_Angle_Cur[sent_channel];
}

/**
 *
 * Sensor speed measurement
 *
 */
void sensor_speed_measure(sent_channel_e sent_channel)
{
	uint8_t i;	
	uint32_t delta_angle_sent = 0u;
	uint32_t angle_sent_avg= 0u;
	uint32_t angle_sent= 0u;	
	uint32_t speed_sent_avg= 0u;
    uint16_t PID_result;
    uint16_t PWM_DC;
    uint32_t sum = 0u;
    uint32_t delta_time = 0u;

	/* calculate angle sent average value for speed calculation */
	Angle_Sent_Value_B[Angle_Measure_Count_B] = Angle_Sent_Value_Cur[sent_channel] * Angle_Sent_Gain;
	
	//if(Angle_Measure_Count_B== (Angle_Sent_Sample_Times-1u))
	//{
	//	for(i=0u;i<Angle_Sent_Sample_Times;i++)
	//	{
	//		Angle_Sent_Sum[sent_channel] += Angle_Sent_Value_B[i];
	//	}			
	//}
	//else if (Angle_Measure_Count_B== Angle_Sent_Sample_Times)
	//{
	//	Angle_Sent_Sum[sent_channel] = Angle_Sent_Sum[sent_channel] - Angle_Sent_Value_B[0u] + Angle_Sent_Value_B[Speed_Sent_Count[sent_channel]];
	//	
	//	for(i=1u;i<=Angle_Sent_Sample_Times;i++)
	//	{
	//		Angle_Sent_Value_B[i-1u] = Angle_Sent_Value_B[i];
	//	}	
	//	
	//	Angle_Measure_Count_B= Angle_Sent_Sample_Times - 1u;
	//}
//
	//angle_sent_avg = Angle_Sent_Sum[sent_channel]/Angle_Sent_Sample_Times;      //still have Angle_Sent_Gain in the value

    Angle_Measure_Count_B++;

    if(Angle_Measure_Count_B>= Angle_Sent_Sample_Times)
    {


        for(i=0u; i<Angle_Sent_Sample_Times; i++)
        {
            sum += Angle_Sent_Value_B[i];
        }

        angle_sent_avg = sum/Angle_Sent_Sample_Times;      //still have Angle_Sent_Gain in the value

        ts_cur_ticks_B = ts_get_ticks();

    

	   /* speed calculate */
	   if(TMM_Operating[sent_channel].tmm_operating_start&&Motor_rotating[sent_channel])	////not clarify SENT1 or SENT2
	   {
    
	   	if(angle_sent_avg> angle_sent_avg_pre_B)
	   	{
	   		delta_angle_sent = angle_sent_avg - angle_sent_avg_pre_B;
	   	}
	   	else
	   	{
	   		delta_angle_sent = angle_sent_avg_pre_B - angle_sent_avg;
	   	}
    
            delta_time = (ts_cur_ticks_B - ts_pre_ticks_B)*5u;                  /* 1 tick 5us */
    
	   	//Speed_Sent_Value_Cur[sent_channel] = delta_angle_sent*1000000u/Angle_Sent_Gain + Sample_Period/2u;
	   	//Speed_Sent_Value_Cur[sent_channel] = Speed_Sent_Value_Cur[sent_channel]/Sample_Period;
    
            Speed_Sent_Value_Cur[sent_channel] = delta_angle_sent*1000000u/Angle_Sent_Gain + delta_time/2u;
            Speed_Sent_Value_Cur[sent_channel] = Speed_Sent_Value_Cur[sent_channel]/delta_time;
    
	   	angle_sent_avg_pre_B  = angle_sent_avg;
    
	   	
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
	       Speed_Sent_Value_B[Speed_Sent_Count_B] = Speed_Sent_Value_Cur[sent_channel];
	       
	    //   if(Speed_Sent_Count_B== (Speed_Sent_Sample_Times-1))
	    //   {
	    //   	for(i=0u;i<Speed_Sent_Sample_Times;i++)
	    //   	{
	    //   		Speed_Sent_Sum_B += Speed_Sent_Value_B[i];
	    //   	}			
	    //   }
	    //   else if (Speed_Sent_Count_B== Speed_Sent_Sample_Times)
	    //   {
	    //   	Speed_Sent_Sum_B = Speed_Sent_Sum_B - Speed_Sent_Value_B[0] + Speed_Sent_Value_B[Speed_Sent_Count_B];
	    //   	
	    //   	for(i=1u;i<=Speed_Sent_Sample_Times;i++)
	    //   	{
	    //   		Speed_Sent_Value_B[i-1] = Speed_Sent_Value_B[i];
	    //   	}	
	    //   	
	    //   	Speed_Sent_Count_B= Speed_Sent_Sample_Times - 1;
	    //   }
	    //   	
	    //   speed_sent_avg = Speed_Sent_Sum_B + Speed_Sent_Sample_Times/2;
	    //   speed_sent_avg = speed_sent_avg/Speed_Sent_Sample_Times;
	       
	       //if(Speed_Sent_Value_Tar[sent_channel] > speed_sent_avg )
	       //{
	       //    	delta_Speed_Sent_Value[sent_channel] = Speed_Sent_Value_Tar[sent_channel] - speed_sent_avg;
	       //}
	       //else
	       //{
	       //	   delta_Speed_Sent_Value[sent_channel] = speed_sent_avg - Speed_Sent_Value_Tar[sent_channel];
	       //}
    
            if(Speed_Sent_Value_Tar[sent_channel] > Speed_Sent_Value_Cur[sent_channel] )
            {
                delta_Speed_Sent_Value[sent_channel] = Speed_Sent_Value_Tar[sent_channel] - Speed_Sent_Value_Cur[sent_channel];
            }
            else
            {
                delta_Speed_Sent_Value[sent_channel] = Speed_Sent_Value_Cur[sent_channel] - Speed_Sent_Value_Tar[sent_channel];
            }
	   	
    	   	Speed_Sent_Count_B++;
	   }
    
	   //TMM_Speed_Cur[sent_channel] = (speed_sent_avg*TMM_Calc_gain + TMM_Angle_gain[sent_channel]/2)/TMM_Angle_gain[sent_channel];				////not clarify SENT1 or SENT2
	   TMM_Speed_Cur[sent_channel] = (Speed_Sent_Value_Cur[sent_channel]*TMM_Calc_gain + TMM_Angle_gain[sent_channel]/2)/TMM_Angle_gain[sent_channel];
    
        ts_pre_ticks_B = ts_cur_ticks_B;

         /* PID */
        if (!Motor_rotating[sent_channel]){
            Speed_PID_Reset_B();   
        }
        else if((TMM_Operating[sent_channel].tmm_operating_start)&&(Motor_rotating[sent_channel]))   
        {
            //speed_pid(sent_channel_2);
    
            if(!Flag_Cancel_PID[sent_channel]) 
            {
                /* use postion PID control, when close to target angle */
                if((delta_Position_Sent_value[sent_channel] < Limit_Speed_Angle)&&(!Flag_Pos_PID_En))
                {
                    //PWM_DC = 170u;
                    //PWM_DC = pwm_data[PWM_Freq_Index].pwm_period - pwm_data[PWM_Freq_Index].pwm_limit_speed;
                    PWM_DC = pwm_data[PWM_Freq_Index].pwm_limit_speed;
                }
                else if(delta_Speed_Sent_Value[sent_channel] > Dead_Band_ShiftSpeed)
                {
                    //PID_result = Speed_PIDAdjust(sent_channel, Speed_Sent_Value_Cur[sent_channel], Speed_Sent_Value_Tar[sent_channel])*pwm_data[PWM_Freq_Index].pid_multiples;
                    PID_result = Speed_PIDAdjust_B(sent_channel, Speed_Sent_Value_Cur[sent_channel], Speed_Sent_Value_Tar[sent_channel])*5;
                    if(PID_result > pwm_data[PWM_Freq_Index].pwm_max)
                    {
                        PID_result = pwm_data[PWM_Freq_Index].pwm_max;
                    }
                    else if(PID_result < pwm_data[PWM_Freq_Index].pwm_min)
                    {
                        PID_result = pwm_data[PWM_Freq_Index].pwm_min;
                    }

                    PWM_DC = pwm_data[PWM_Freq_Index].pwm_period - PID_result;
                    //PWM_DC = PID_result;
                }
                
                pwm_duty_set(sent_channel, PWM_DC);
            }
        }

        Angle_Measure_Count_B= 0u;
    }

}

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
/**
 *
 *	s12 ECT interrupts, jumped from xgate ECT interrupts
 *
 */

INTERRUPT void s12_ECT_Channel1_Interrupt(void)
{
    
    

    CLEAR_XGIF_FOR_CHANNEL(ECT_Channel1_Xg_Address_MASK);


    icantx(0x210, 0u, 8, (FPTR const uint8_t *)Sent_Rawdata_A);


}


INTERRUPT void ECT_Channel2_Interrupt(void)
{

    ECT_OnCapture_B(sent_channel_2);                           /* Invoke user event */

}


/* END */