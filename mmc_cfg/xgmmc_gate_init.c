/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    xgmmc_gate_init.c
 *
 * description:  
 * xgmmc_gate_init
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 * 2017-Mar_16 Jun Wu			Add ECT_Channel1_Interrupt, ECT_Channel2_Interrupt
 *
 *****************************************************************************/
/**
 * @file
 * Simple test function containing an XGATE interrupt handler that takes
 * arguments and calls the corresponding S12 interrupt handler
 */

#define _XGATE_
#include "basic_types.h"
#include "hardware.h"
#include "gate.h"
#include "pit.h"
#include "platform_test.h"
#include "smm.h"
#include <string.h>
#include "xgate_icm.h"

/*****************************************************************************
 * MACROS
 *****************************************************************************/
#define Unvalid_Count_MAX_A         100u
/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/
  PRIVATE const uint8_t CRC4Table_A[256] = {
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
 
//PRIVATE uint32_t sent_nibbles_duration_A[sent_channel_max][sent_state_unknown] = {(0,0)};
//PRIVATE uint32_t tick_duration = 0u;                    /*one sent tick time (us)*/

//only xgate
PRIVATE uint32_t pre_capture_A = 0u;
PRIVATE uint32_t cur_capture_A = 0u;
PRIVATE enum sent_state_e TMM_sent_state_A = sent_state_unknown;


//PRIVATE uint8_t sent_nibbles_value_prev[sent_channel_max][sent_state_unknown] = {0};

/* sent diagnos */
PRIVATE bool_t Err_SENT_ECUT_CRC_A = FALSE;
PRIVATE bool_t Err_SENT_ECUT_NibbleLength_A = FALSE;
PRIVATE bool_t Err_SENT_ECUT_SyncLength_A = FALSE;  /* the former message is not valid, if true */
PRIVATE bool_t Err_SENT_ECUT_RollingCounter_A = FALSE; 
PRIVATE bool_t Err_SENT_ECUT_InverseNibble_A = FALSE;
PRIVATE bool_t Err_SENT_ECUT_SensorDiag_A = FALSE;  /* sensor diagnos in status nibble */
PRIVATE bool_t FlagUnvalidSENT_A = FALSE;
//bool_t Err_SENT_ECUT_TimeOut = FALSE; 

PRIVATE uint8_t sent_sequence_num_A = 0u;
PRIVATE uint8_t sent_sequence_num_pre_A = 0u;
PRIVATE uint8_t delta_sequence_num_A = 0u;
PRIVATE uint8_t check_sum_value_A=0u;
PRIVATE uint32_t sequence_cnt_A = 0u;

//only xgate
PRIVATE uint32_t tick_duration_A = 0u;          /*one sent tick time (us)*/
PRIVATE uint32_t sent_nibbles_duration_A[sent_state_unknown] = {0u};
PRIVATE bool_t One_Sent_Cycle_Completed_A = FALSE;

//PRIVATE uint8_t sent_serial_cnter = 0u;
//PRIVATE uint8_t sent_serial_start_cnter = 0u;
//PRIVATE uint16_t sent_serial_suc_cnter = 0u;
//PRIVATE bool_t Serial_Data_Start = FALSE;
//PRIVATE bool_t EN_EnhSerMsg_16bit = FALSE;
//
//PRIVATE uint8_t Serial_Data_CRC = 0u;
//PRIVATE uint8_t Serial_Data_ID = 0u;
//PRIVATE uint16_t Serial_Data = 0u;
PRIVATE uint32_t Err_SENT_ECUT_NibbleLength_Count_A = 0u;

PRIVATE uint8_t Err_Too_Many_Nibbles_Ctr_A = 0u;

PRIVATE uint16_t Err_CRC_Ctr_A = 0u;
/******************************************************************************
 * Public DATA
 *****************************************************************************/

extern uint16_t ErrCount_SENT;

extern sent_CRC_version_e TMM_Sent_CRC_Version[sent_channel_max];
extern sent_pause_pulse_e TMM_EN_Pause_Pulse[sent_channel_max];
extern sent_serial_message_e TMM_SerMSg[sent_channel_max];
extern sent_A1A3_mode_e TMM_A1A3_Mode[sent_channel_max];
extern uint32_t Angle_Sent_Value_Cur[sent_channel_max];
extern uint32_t TMM_Angle_gain[sent_channel_max];
extern uint32_t TMM_Angle_offset[sent_channel_max];

/* sensor current angle */
extern int16_t  TMM_Angle_Cur[sent_channel_max];
//extern const uint8_t CRC4Table[256];


/******************************************************************************
 * PRIVATE FUNCTION
 *****************************************************************************/
PRIVATE void xcheck_initialisation(void);

PRIVATE void Calc_Nibbles_value_A(sent_channel_e sent_channel, uint32_t tick_duration, uint32_t *nibbles_duration);
PRIVATE uint8_t Sent_CRC_Check_A(sent_channel_e sent_channel, uint8_t *nibbles_value);
//PRIVATE void Calc_Serial_Data(sent_channel_e sent_channel)  ;
PRIVATE void Sent_Diagnose_A(sent_channel_e sent_channel);
PRIVATE void Store_Sent_Rawdata_A(void);
//PRIVATE void ECU_SENT_A(void);
PRIVATE void ECT_OnCapture_A(sent_channel_e sent_channel);


/*
 * Argument structures for the xvector array
 */
struct xgpit_isr_s xgpit1_args = { PitChannel1 };
struct xgpit_isr_s xgpit2_args = { PitChannel2 };

/**
 * @lint
 * xpit_ctr not accessed\n
 * -esym(551,xpit_ctr)
 */
/*lint -esym(551,xpit_ctr) */
/** @polyspace<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes */
static uint32_t xpit_ctr[PitChannelCount] = { 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u };

/**
 * @param args  structure containing the PIT channel that caused the interrupt
 */
INTERRUPT void xgpit_isr(struct xgpit_isr_s args)
{
//    xpit_ctr[args.channel]++;
//
//    PITCLRINTFLG(args.channel); /* clear the h/w interrupt */
//#if ! defined (POLYSPACE)
//    _sif();         /* call the corresponding S12 interrupt */
//#endif
}

struct xgswt_isr_s xgswt0_args = { 0u, 0x1234u};

/** @polyspace<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes */
static uint8_t xgate_byte = 0xf0u;
/** @polyspace<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes */
static uint16_t xgate_word = 0x12f1u;

/**
 * software interrupt to check argument passing to XGATE interrupts
 * and data intialisation.
 */
INTERRUPT void xgswt_isr(struct xgswt_isr_s args)
{
    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT(args.channel == 0u);
    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT(args.word == 0x1234u);
    xcheck_initialisation();

    /* finally clear the interrupt that triggered us */
    CLEAR_XGSWT(args.channel);
}

PRIVATE void xcheck_initialisation(void)
{
    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT(xgate_byte == 0xf0u);
    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
    ASSERT(xgate_word == 0x12f1u);
}


/**
 *
 * ECT Interrupt to XGATE
 *
 */

INTERRUPT void ECT_Channel1_Interrupt(void)
{
    /* ECT_TFLG1: C7F=0,C6F=0,C5F=0,C4F=0,C3F=0,C2F=0,C1F=1,C0F=0 */
    //TFLG1 |= ECT_Channel1_MASK;                       /* Reset interrupt request flag */
    //cur_capture[sent_channel_1] = TC1;            
    ECT_OnCapture_A(sent_channel_1);                        /* Invoke user event */

    if(One_Sent_Cycle_Completed_A)
    {
        One_Sent_Cycle_Completed_A = FALSE;
        _sif();         /* call the corresponding S12 interrupt */       
    }
    
}

//INTERRUPT void ECT_Channel2_Interrupt(void)
//{
//    /* ECT_TFLG2: C7F=0,C6F=0,C5F=0,C4F=0,C3F=0,C2F=0,C1F=1,C0F=0 */
//    TFLG1 |= ECT_Channel2_MASK;                       /* Reset interrupt request flag */
// 
//    ECT_OnCapture(sent_channel_2);                           /* Invoke user event */
//
//    if(One_Sent_Cycle_Completed[sent_channel_2])
//    {
//        One_Sent_Cycle_Completed[sent_channel_2] = FALSE;
//        _sif();         /* call the corresponding S12 interrupt */
//    }
//}

/**
 *
 * ECT capture sent signals, SENT1 is ECT1, SENT2 is ECT2
 *
 */

PRIVATE void ECT_OnCapture_A(sent_channel_e sent_channel)
{
    uint16_t capture=0u;
    uint32_t nibbles_duration = 0u;       /*us*/

    /* calculate nibbles duration time */
    switch(sent_channel)
    {
        case sent_channel_1:
                cur_capture_A = TC1;
                break;
        case sent_channel_2:
                cur_capture_A = TC2;
                break;
        default:
                break;
    }
    capture = cur_capture_A - pre_capture_A;
    //nibbles_duration = (uint32_t)capture*1000u/50u;     /* capture/50 us , multiply 1000 for accuracy */
    nibbles_duration = (uint32_t)capture*20u; 
            
    pre_capture_A = cur_capture_A;

    
    /* find the sync duration */
    if(TMM_sent_state_A == sent_state_unknown)                   /* when state is unknown, check sync */   
    {
        if ((nibbles_duration>=Sync_Duration_Min)&&(nibbles_duration<=Sync_Duration_Max))           
        {
            TMM_sent_state_A = sent_state_sync;
            tick_duration_A = nibbles_duration/Sync_Duration_Ticks;
            sent_nibbles_value[sent_channel][sent_state_sync] = Sync_Duration_Ticks;

            if(TMM_EN_Pause_Pulse[sent_channel] == Sent_En_Pause_Pulse)
            {
                if((Err_Too_Many_Nibbles_Ctr_A < 10u)&&(Err_Too_Many_Nibbles_Ctr_A > 0u))
                {
                    SENT_ECUT_A_Error_MissingNibble = TRUE;
                }
                else if( Err_Too_Many_Nibbles_Ctr_A > 10u)
                {
                    SENT_ECUT_A_Error_TooManyNibbles = TRUE;
                }
                else
                {
                    SENT_ECUT_A_Error_MissingNibble = FALSE;
                    SENT_ECUT_A_Error_TooManyNibbles = FALSE;
                }
            }
            else
            {
                if((Err_Too_Many_Nibbles_Ctr_A < 9u)&&(Err_Too_Many_Nibbles_Ctr_A > 0u))
                {
                    SENT_ECUT_A_Error_MissingNibble = TRUE;
                }
                else if( Err_Too_Many_Nibbles_Ctr_A > 9u)
                {
                    SENT_ECUT_A_Error_TooManyNibbles = TRUE;
                }
                else
                {
                    SENT_ECUT_A_Error_MissingNibble = FALSE;
                    SENT_ECUT_A_Error_TooManyNibbles = FALSE;
                }
            }    

            Err_Too_Many_Nibbles_Ctr_A = 0u;

        }
        else if (((nibbles_duration > Sync_Duration_ERR_Min)&&(nibbles_duration < Sync_Duration_Min)) || (nibbles_duration > Sync_Duration_Max))  /* sync length error */
        {
            Err_SENT_ECUT_SyncLength_A = TRUE;
        }
    }

    /* get other nibbles duration time */
    if(TMM_EN_Pause_Pulse[sent_channel] == Sent_En_Pause_Pulse)
    {
        switch(TMM_sent_state_A)
        {
            case sent_state_sync:
                sent_nibbles_duration_A[sent_state_sync] = nibbles_duration;
                TMM_sent_state_A = sent_state_status;           
                break;
            case sent_state_status:
                sent_nibbles_duration_A[sent_state_status] = nibbles_duration;
                TMM_sent_state_A = sent_state_data1;    
                break;
            case sent_state_data1:
                sent_nibbles_duration_A[sent_state_data1] = nibbles_duration;
                TMM_sent_state_A = sent_state_data2;    
                break;
            case sent_state_data2:
                sent_nibbles_duration_A[sent_state_data2] = nibbles_duration;
                TMM_sent_state_A = sent_state_data3;    
                break;
            case sent_state_data3:
                sent_nibbles_duration_A[sent_state_data3] = nibbles_duration;
                TMM_sent_state_A = sent_state_data4;    
                break;
            case sent_state_data4:
                sent_nibbles_duration_A[sent_state_data4] = nibbles_duration;
                TMM_sent_state_A = sent_state_data5;    
                break;
            case sent_state_data5:
                sent_nibbles_duration_A[sent_state_data5] = nibbles_duration;
                TMM_sent_state_A = sent_state_data6;    
                break;
            case sent_state_data6:
                sent_nibbles_duration_A[sent_state_data6] = nibbles_duration;
                TMM_sent_state_A = sent_state_checksum; 
                break;
            case sent_state_checksum:
                sent_nibbles_duration_A[sent_state_checksum] = nibbles_duration;
                TMM_sent_state_A = sent_state_pause;    
                break;
            case sent_state_pause:
                sent_nibbles_duration_A[sent_state_pause] = nibbles_duration;
                
                One_Sent_Cycle_Completed_A = TRUE;
                Calc_Nibbles_value_A(sent_channel, tick_duration_A, &sent_nibbles_duration_A[sent_state_sync]);                 
                //Calc_Serial_Data(sent_channel);
                Sent_Diagnose_A(sent_channel);
                Store_Sent_Rawdata_A();    
                sensor_speed_measure_A(sent_channel);    
                xgECU_SENT_A();                      
                
                //Sent_Rawdata_CNT[sent_channel]++;                         /* will be clear when raw data sent to can */         
                TMM_sent_state_A = sent_state_unknown;  
                break;
            default: 
                TMM_sent_state_A = sent_state_unknown;  
                break;
        }
    }
    else
    {
        switch(TMM_sent_state_A)
        {
            case sent_state_sync:
                sent_nibbles_duration_A[sent_state_sync] = nibbles_duration;
                TMM_sent_state_A = sent_state_status;           
                break;
            case sent_state_status:
                sent_nibbles_duration_A[sent_state_status] = nibbles_duration;
                TMM_sent_state_A = sent_state_data1;    
                break;
            case sent_state_data1:
                sent_nibbles_duration_A[sent_state_data1] = nibbles_duration;
                TMM_sent_state_A = sent_state_data2;    
                break;
            case sent_state_data2:
                sent_nibbles_duration_A[sent_state_data2] = nibbles_duration;
                TMM_sent_state_A = sent_state_data3;    
                break;
            case sent_state_data3:
                sent_nibbles_duration_A[sent_state_data3] = nibbles_duration;
                TMM_sent_state_A = sent_state_data4;    
                break;
            case sent_state_data4:
                sent_nibbles_duration_A[sent_state_data4] = nibbles_duration;
                TMM_sent_state_A = sent_state_data5;    
                break;
            case sent_state_data5:
                sent_nibbles_duration_A[sent_state_data5] = nibbles_duration;
                TMM_sent_state_A = sent_state_data6;    
                break;
            case sent_state_data6:
                sent_nibbles_duration_A[sent_state_data6] = nibbles_duration;
                TMM_sent_state_A = sent_state_checksum; 
                break;
            case sent_state_checksum:
                sent_nibbles_duration_A[sent_state_checksum] = nibbles_duration;
                
                //PORTK|=0x01;

                One_Sent_Cycle_Completed_A = TRUE;
                Calc_Nibbles_value_A(sent_channel, tick_duration_A, &sent_nibbles_duration_A[sent_state_sync]);                 
                //Calc_Serial_Data(sent_channel);
                Sent_Diagnose_A(sent_channel);
                Store_Sent_Rawdata_A();     
                sensor_speed_measure_A(sent_channel);     
                xgECU_SENT_A();  

                //PORTK &= ~0x01;  
                
                //Sent_Rawdata_CNT[sent_channel]++;                         /* will be clear when raw data sent to can */                                     
                TMM_sent_state_A = sent_state_unknown;  
                break;
            default: 
                TMM_sent_state_A = sent_state_unknown;  
                break;
        }
    }
    
    Err_Too_Many_Nibbles_Ctr_A++;
    //SENT_TimeOut_CNT[sent_channel]++;
}



/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
PRIVATE void Calc_Nibbles_value_A(sent_channel_e sent_channel, uint32_t tick_duration, uint32_t *nibbles_duration)
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
              Err_SENT_ECUT_NibbleLength_A = TRUE;
              Err_SENT_ECUT_NibbleLength_Count_A++;
            }
        }
    }   

}

/* Caculate CRC value */
PRIVATE uint8_t Sent_CRC_Check_A(sent_channel_e sent_channel, uint8_t *nibbles_value)
{   
    enum sent_state_e i;
    uint16_t tempCS;    
    uint8_t CheckSum = Init_CheckSum;
    
    for(i=sent_state_data1;i<sent_state_checksum;i++)
    {
        tempCS = nibbles_value[i] + CheckSum*16u;
        CheckSum = CRC4Table_A[tempCS];
    }
    
    if(TMM_Sent_CRC_Version[sent_channel] == Sent_CRC_2010)
    {
        tempCS = CheckSum*16u;
        CheckSum = CRC4Table_A[tempCS];
    }
        
    return CheckSum;
}


PRIVATE void Sent_Diagnose_A(sent_channel_e sent_channel)
{
    /* clear error flag */
    Err_SENT_ECUT_CRC_A = FALSE; 
    Err_SENT_ECUT_NibbleLength_A = FALSE; 
    Err_SENT_ECUT_SyncLength_A = FALSE;  
    Err_SENT_ECUT_RollingCounter_A = FALSE; 
    Err_SENT_ECUT_InverseNibble_A = FALSE; 
    Err_SENT_ECUT_SensorDiag_A = FALSE;
    FlagUnvalidSENT_A = FALSE;
    
    /* Err_SENT_ECUT_CRC_A */
    check_sum_value_A = Sent_CRC_Check_A(sent_channel, &sent_nibbles_value[sent_channel][0u]);
    
    Err_SENT_ECUT_CRC_A = (check_sum_value_A==sent_nibbles_value[sent_channel][sent_state_checksum])? FALSE: TRUE;

    SENT_ECUT_A_Error_CRC = Err_SENT_ECUT_CRC_A;
    SENT_ECUT_A_CRC = check_sum_value_A;

    //if(Err_SENT_ECUT_CRC_A)
    //{
    //    Err_CRC_Ctr_A++;
//
    //    if(Err_CRC_Ctr_A > CRC_ERR_MAX_A)
    //    {
    //        Err_CRC_Ctr_A = 0u;
    //    }
    //    else
    //    {
    //        SENT_ECUT_A_Error_CRC = FALSE;
    //        SENT_ECUT_A_CRC = sent_nibbles_value[sent_channel][sent_state_checksum];
    //    }
    //}
    
    
    /* Err_SENT_ECUT_NibbleLength_A */
    
    /* Err_SENT_ECUT_SyncLength_A */
    /* defined in Cap1_OnCapture() */
    
    /* Err_SENT_ECUT_RollingCounter_A */   
    //if(!TMM_Dis_Rolling_Cnter)                  //not clarify SENT1 or SENT2
    if(TMM_A1A3_Mode[sent_channel_1] == Sent_RollingCnter)
    {
        sent_sequence_num_A = ((uint8_t)sent_nibbles_value[sent_channel][sent_state_data4] << 4u) | (uint8_t)sent_nibbles_value[sent_channel][sent_state_data5];
            
        delta_sequence_num_A = sent_sequence_num_A - sent_sequence_num_pre_A;
    
        if(delta_sequence_num_A!=1)
        {
            sequence_cnt_A++;
            Err_SENT_ECUT_RollingCounter_A = TRUE;
        }
        else
        {
            Err_SENT_ECUT_RollingCounter_A = FALSE;
        }

         /* Err_SENT_ECUT_InverseNibble_A */
        Err_SENT_ECUT_InverseNibble_A = ((sent_nibbles_value[sent_channel][sent_state_data1] + sent_nibbles_value[sent_channel][sent_state_data6]) == 15)? FALSE: TRUE;

    }
    else 
    {
        Err_SENT_ECUT_RollingCounter_A = FALSE;
        Err_SENT_ECUT_InverseNibble_A = FALSE;
    }
    
        
     
    /* Err_SENT_ECUT_SensorDiag_A */
    Err_SENT_ECUT_SensorDiag_A = (sent_nibbles_value[sent_channel][sent_state_status]&0x02) ? TRUE : FALSE;
    
    /* FlagUnvalidSENT_A */
    FlagUnvalidSENT_A = Err_SENT_ECUT_CRC_A | Err_SENT_ECUT_NibbleLength_A | Err_SENT_ECUT_RollingCounter_A | Err_SENT_ECUT_InverseNibble_A | Err_SENT_ECUT_SensorDiag_A;
    FlagUnvalidSENT_A |= SENT_ECUT_A_Error_TooManyNibbles;
    

    Sent_Total_cnt[sent_channel]++; 

    /* ErrCount_SENT */     //not clarify SENT1 or SENT2
    //ErrCount_SENT += Err_SENT_ECUT_CRC_A + Err_SENT_ECUT_NibbleLength_A + Err_SENT_ECUT_SyncLength_A + Err_SENT_ECUT_RollingCounter_A + Err_SENT_ECUT_InverseNibble_A + Err_SENT_ECUT_SensorDiag_A; 
    
    sent_sequence_num_pre_A = sent_sequence_num_A;
}

/* store raw data */
PRIVATE void Store_Sent_Rawdata_A(void)
{   
    /* CAN Slow */
    //SENT_ErrCount_A = Sent_Unvalid_cnt[sent_channel_1];
    int32_t angle_value;

    /* CAN Fast */
    SENT_SENS_A_Nibble1 = sent_nibbles_value[sent_channel_1][sent_state_status];
    SENT_SENS_A_Nibble2 = sent_nibbles_value[sent_channel_1][sent_state_data1];
    SENT_SENS_A_Nibble3 = sent_nibbles_value[sent_channel_1][sent_state_data2];
    SENT_SENS_A_Nibble4 = sent_nibbles_value[sent_channel_1][sent_state_data3];
    SENT_SENS_A_Nibble5 = sent_nibbles_value[sent_channel_1][sent_state_data4];
    SENT_SENS_A_Nibble6 = sent_nibbles_value[sent_channel_1][sent_state_data5];
    SENT_SENS_A_Nibble7 = sent_nibbles_value[sent_channel_1][sent_state_data6];
    SENT_SENS_A_Nibble8 = sent_nibbles_value[sent_channel_1][sent_state_checksum];
    SENT_ECUT_A_Error_SensorDiag = Err_SENT_ECUT_SensorDiag_A;
    //RTOS_ErrDistRefPos_A          // in vio_write
    //RTOS_FlagValidSENT_A = !FlagUnvalidSENT_A;
    //RTOS_ErrGradBegr_A            // in vio_write
    //SENT_ECUT_A_CRC = check_sum_value_A;                  //in diagnose
    SENT_ECUT_A_Error_NibbleLength = Err_SENT_ECUT_NibbleLength_A;
    SENT_ECUT_A_Error_SyncLength = Err_SENT_ECUT_SyncLength_A;
    //SENT_ECUT_A_Error_MissingNibble
    //SENT_ECUT_A_Error_TooManyNibbles = (Err_Too_Many_Nibbles_Ctr_A > 0)? TRUE : FALSE;
    //SENT_ECUT_A_Error_CRC = Err_SENT_ECUT_CRC_A;          //in diagnose
    //SENT_ECUT_A_Error_TimeOut
    SENT_ECUT_A_Error_RollingCounter = Err_SENT_ECUT_RollingCounter_A;
    SENT_ECUT_A_Error_InverseNibble = Err_SENT_ECUT_InverseNibble_A;
    //ANGL_SENS_A_ANA_RAW = TMM_Angle_Cur[sent_channel];                                    //move to angle measure

    if(Err_SENT_ECUT_CRC_A)
    {
        memcpy(&sent_nibbles_value[sent_channel_1][0u], &sent_nibbles_value_prev[sent_channel_1][0u], sent_state_unknown);
    }
    
    memcpy(&sent_nibbles_value_prev[sent_channel_1][0u],&sent_nibbles_value[sent_channel_1][0u], sent_state_unknown);


    //Angle_Sent_Value_Cur[sent_channel_1] = ((uint16_t)SENT_SENS_A_Nibble2 << 8u) | \
    //                                        ((uint16_t)SENT_SENS_A_Nibble3 << 4u) | \
    //                                        ((uint16_t)SENT_SENS_A_Nibble4);

    Angle_Sent_Value_Cur[sent_channel_1] = ((uint16_t)sent_nibbles_value[sent_channel_1][sent_state_data1] << 8u) | \
                                            ((uint16_t)sent_nibbles_value[sent_channel_1][sent_state_data2] << 4u) | \
                                            ((uint16_t)sent_nibbles_value[sent_channel_1][sent_state_data3]); 

    angle_value = TMM_Calc_gain*(int32_t)Angle_Sent_Value_Cur[sent_channel_1]  - (int32_t)TMM_Angle_offset[sent_channel_1];
    angle_value = (angle_value + (int32_t)TMM_Angle_gain[sent_channel_1]/2)/(int32_t)TMM_Angle_gain[sent_channel_1];
    TMM_Angle_Cur[sent_channel_1] = (int16_t)angle_value;
    ANGL_SENS_A_ANA_RAW = TMM_Angle_Cur[sent_channel_1];   

    if(FlagUnvalidSENT_A)
    {
        Sent_Unvalid_cnt[sent_channel_1]++;                     //not clarify SENT1 or SENT2
        //memcpy(sent_nibbles_value,sent_nibbles_value_prev, sent_state_unknown);

        if(Sent_Unvalid_cnt[sent_channel_1] > Unvalid_Count_MAX_A)
        {
            Sent_Unvalid_cnt[sent_channel_1] = 0u;
        }
        else
        {    
            SENT_ECUT_A_Error_CRC = FALSE;
            SENT_ECUT_A_CRC = SENT_SENS_A_Nibble8;
            FlagUnvalidSENT_A = FALSE;
            SENT_ECUT_A_Error_NibbleLength = FALSE;
            SENT_ECUT_A_Error_SyncLength = FALSE;
            SENT_ECUT_A_Error_MissingNibble = FALSE;
            SENT_ECUT_A_Error_TooManyNibbles = FALSE;
            SENT_ECUT_A_Error_RollingCounter = FALSE;
            SENT_ECUT_A_Error_InverseNibble = FALSE;
        }
    }

    RTOS_FlagValidSENT_A = !FlagUnvalidSENT_A;  


    
}


//PRIVATE void ECU_SENT_A(void)
//{
//    uint8_t flags = 0u;
//    int16_t integer_value = 0u;
//
//    /* Byte Order: Intel */
//    Sent_Rawdata[sent_channel_1][0] = ((uint8_t)sent_nibbles_value[sent_channel_1][sent_state_status] << 4u) | (uint8_t)sent_nibbles_value[sent_channel_1][sent_state_data1];
//    Sent_Rawdata[sent_channel_1][1] = ((uint8_t)sent_nibbles_value[sent_channel_1][sent_state_data2] << 4u) | (uint8_t)sent_nibbles_value[sent_channel_1][sent_state_data3];
//    Sent_Rawdata[sent_channel_1][2] = ((uint8_t)sent_nibbles_value[sent_channel_1][sent_state_data4] << 4u) | (uint8_t)sent_nibbles_value[sent_channel_1][sent_state_data5];
//    Sent_Rawdata[sent_channel_1][3] = ((uint8_t)sent_nibbles_value[sent_channel_1][sent_state_data6] << 4u) | (uint8_t)sent_nibbles_value[sent_channel_1][sent_state_checksum];
//
//    flags = (Err_SENT_ECUT_SensorDiag_A)? 0x01 : 0x00;
//    //flags |= (RTOS_ErrDistRefPos_A)? 0x02 : 0x00;
//    flags |= (FlagUnvalidSENT_A)? 0x04 : 0x00;
//    //flags |= (RTOS_ErrGradBegr_A)? 0x08 : 0x00;
//    flags |= check_sum_value_A << 4u;
//    Sent_Rawdata[sent_channel_1][4] = flags;
//
//    flags = (Err_SENT_ECUT_NibbleLength_A)? 0x01 : 0x00;
//    flags |= (Err_SENT_ECUT_SyncLength_A)? 0x02 : 0x00;
//    //flags |= (SENT_ECUT_A_Error_MissingNibble)? 0x04 : 0x00;
//    //flags |= (SENT_ECUT_A_Error_TooManyNibbles)? 0x08 : 0x00;
//    flags |= (Err_SENT_ECUT_CRC_A)? 0x10 : 0x00;
//    //flags |= (SENT_ECUT_A_Error_TimeOut)? 0x20: 0x00;
//    flags |= (Err_SENT_ECUT_RollingCounter_A)? 0x40 : 0x00;
//    flags |= (Err_SENT_ECUT_InverseNibble_A)? 0x80 : 0x00;
//    Sent_Rawdata[sent_channel_1][5] = flags;
//
//    /* Byte Order: Motorola */
//    integer_value = ANGL_SENS_A_ANA_RAW * 16; 
//    Sent_Rawdata[sent_channel_1][6] = (uint8_t)(integer_value >> 8u);
//    Sent_Rawdata[sent_channel_1][7] = (uint8_t)integer_value; 
//    
//}


