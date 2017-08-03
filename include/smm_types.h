/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2017 - All rights reserved 
 *
 * file name:   smm_types.h
 *
 * description:  
 * Public header file for the smm feature
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2017-Mar-21  Jun Wu           initial version
 *                       
 *
 *****************************************************************************/
#if !defined(SMM_TYPES_H)
#define      SMM_TYPES_H
/**
 *
 * Public header file for the smi feature
 *
 * @file
 * @ingroup smi
 */

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/
#define TMM_Calc_gain              1000
#define Angle_Noise_Threshold	100u
#define Angle_Noise_Cnt_Threshold	10u
#define Sample_Period 1000            //us
#define Angle_Sent_Sample_Times 10
#define Angle_Sent_Gain 100
#define Speed_Sent_Sample_Times 10
#define Init_CheckSum 5u
#define  Sync_Duration_Ticks  56
#define  Sync_Duration_Max    201600             /* 3*(1+20%)*56 (us) */
#define  Sync_Duration_Min    134400             /* 3*(1-20%)*56 (us) */
#define  Sync_Duration_ERR_Min    108000             /* 3*(1+20%)*30 (us) */

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/* No idea if these are the actual states, so ... */

typedef enum
{
    sent_channel_1 = 0u,
    sent_channel_2,
    sent_channel_max
} sent_channel_e;

enum sent_state_e
{
    sent_state_sync = 0u,
    sent_state_status,
	sent_state_data1,
	sent_state_data2,
	sent_state_data3,
	sent_state_data4,
	sent_state_data5,
	sent_state_data6,
	sent_state_checksum,
	sent_state_pause,            //for sent2010
    sent_state_unknown
};


#endif      /* SMM_TYPES_H */

