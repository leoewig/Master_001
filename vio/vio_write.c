/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:       vio_write.c
 *
 * description:  
 *  Routines to format transmitted CAN messages.
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Feb-14  Jun Wu          initial version
 * 2017-Mar-23  Jun Wu 			update protocal for TMM2.0
 *
 *****************************************************************************/


#include <mmc.h>
#include "vio.h"        /* public header for this module */
#include "vio_p.h"      /* private header for this module */
#include "exc.h"
#include "exc_p.h"
#include "fm.h"         /* Failure Management */
#include "ms.h"         /* Main State Machine */
#include "fm.h"         /* Fault Management */
#include "dio_cfg.h"    /* To read Pin state for contactors */
#include "mdm.h"
#include "mdm_bg.h"
#include "pp_p.h"
#include "smm.h"
#include "sci.h"
#include "motor.h"
#include <string.h>
#include "ai.h"
#include "ect.h"
#include "lin.h"
#include "pwm.h"
#include "tmm.h"
#include "tmm_fm.h"

/*****************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/* ECU_SW_Version */
#define GLO_EngineCode			0
#define GLO_SampleVersion		0
#define GLO_SoftwareVersion		0
#define GLO_Hardware			0
#define GLO_ParameterSet		0

/* ECU_Modi */
#define CURR_DCMR_X_ANA_RMS_FACTOR		16				/* 1/0.0625 */
#define SPAN_SENS_X_ANA_MEA_FACTOR		8
#define VOLT_BATT_M_ANA_RAW_FACTOR		10				/* 1/0.1 */

/* ECU_AngleOutput */
#define ANGL_ECUT_X_ANA_MEA_FACTOR		16				/* 1/0.0625 */
#define ANGL_SENS_X_ANA_MEA_FACTOR		64				/* 0.015625 */

/* Module2 ANA2CAN */
#define PRES_C704_FACTOR 		0.000213623
#define PRES_C704_OFFSET 		-7
#define HUMI_C704_FACTOR 		0.00305176
#define HUMI_C704_OFFSET 		-100
#define CURR_C704_FACTOR 		0.00183105
#define CURR_C704_OFFSET		-60

/* Module1 TEMP2CAN */
#define TEMP_C703_FACTOR 		 0.0152588
#define TEMP_C703_OFFSET 		-200

/* ECU SENT */
#define ANGL_SENS_X_ANA_RAWFACTOR 			16			/* 1/0.0625 */

/******************************************************************************
 * CALIBRATABLE PARAMETERS
 *****************************************************************************/

/*****************************************************************************
 * PRIVATE DATA
 *****************************************************************************/
uint8_t PWM_In_Freq_TX = 0u;
uint8_t PWM_In_Duty_TX = 0u;



/* ECU_EndStopCounter */
uint16_t ANGL_SENS_B_ANA_LSB = 0u;
uint16_t ANGL_SENS_A_ANA_LSB = 0u;
uint16_t CNTR_ECUT_B_ENDPOS_RAW = 0u;
uint16_t CNTR_ECUT_A_ENDPOS_RAW = 0u;

/* ECU_Modi */
int16_t SPAN_SENS_A_ANA_MEA = 0u;
int16_t SPAN_SENS_B_ANA_MEA = 0u;
uint16_t CURR_DCMR_A_ANA_RMS = 0u;
uint16_t CURR_DCMR_B_ANA_RMS = 0u;
bool_t SENT_ECUT_A_GradientBegr = FALSE;
bool_t SENT_ECUT_B_GradientBegr = FALSE;
bool_t SENT_ECUT_A_FlagValidSent = FALSE;
bool_t SENT_ECUT_B_FlagValidSent = FALSE;
uint8_t EMC_ModeCtrl_A = 0u;
uint8_t EMC_ModeCtrl_B = 0u;
bool_t FLAG_TOUT_A_DIG_RAW = FALSE;
bool_t FLAG_TOUT_B_DIG_RAW = FALSE;
bool_t SENT_ECUT_A_Error_DistRefPos = FALSE;
bool_t SENT_ECUT_B_Error_DistRefPos = FALSE;
bool_t TII_FlagIgnSig = FALSE;
bool_t TOA_FlagAmpEn_A = FALSE;
bool_t TOA_FlagAmpEn_B = FALSE;
uint8_t VOLT_BATT_M_ANA_RAW = 0u;

/* ECU_AngleOutput */
int16_t ANGL_ECUT_A_ANA_MEA = 0u;
int16_t ANGL_ECUT_B_ANA_MEA = 0u;
int16_t ANGL_SENS_A_ANA_MEA = 0u;
int16_t ANGL_SENS_B_ANA_MEA = 0u;

/* ECU_SENT_ErrorCount */
uint8_t TX_SENT_TEMP_B = 0u;
uint8_t TX_SENT_TEMP_A = 0u;
uint8_t SENT_ErrCount_B = 0u;


/* ECU_LimitOutput */
uint16_t TX_CCA_AngleDevMax_co = 0u;
uint16_t TX_CCA_ShiftSpeedDevMax_co = 0u;
uint8_t TX_CCA_TimeTolDevMax_co = 0u;
uint8_t TX_TOP_PwmMax_co = 0u;
uint8_t TX_TOP_PwmMin_co = 0u;
uint8_t TX_TIC_CurEmMaxDiag_co = 0u;
int16_t TX_EMC_ShiftSpeedCamDem_co_A = 0u;
int16_t TX_EMC_ShiftSpeedCamDem_co_B = 0u;

/* ECU_RefPosOutput */
uint16_t ANGL_ECUT_A_REFPOSLEFT_RAW = 0u;
uint16_t ANGL_ECUT_B_REFPOSLEFT_RAW = 0u;
uint16_t ANGL_ECUT_A_REFPOSRIGHT_RAW = 0u;
uint16_t ANGL_ECUT_B_REFPOSRIGHT_RAW = 0u;
int8_t PPWM_DCMR_B_ANA_RAW = 0u;
int8_t PPWM_DCMR_A_ANA_RAW = 0u;

/* Module2_low */
uint16_t PRES_C704_CH00 = 0u;
uint16_t PRES_C704_CH01 = 0u;
uint16_t PRES_C704_CH02 = 0u;
uint16_t PRES_C704_CH03 = 0u;

/* Module2_high */
uint16_t CURR_C704_CH07 = 0u;
uint16_t HUMI_C704_CH06 = 0u;
uint16_t PRES_C704_CH04 = 0u;
uint16_t PRES_C704_CH05 = 0u;

/* Module1_low */
int16_t TEMP_C703_CH00 = 0u;
int16_t TEMP_C703_CH01 = 0u;
int16_t TEMP_C703_CH02 = 0u;
int16_t TEMP_C703_CH03 = 0u;

/* Module1_high */
int16_t TEMP_C703_CH04 = 0u;
int16_t TEMP_C703_CH05 = 0u;
int16_t TEMP_C703_CH06 = 0u;
int16_t TEMP_C703_CH07 = 0u;


/* ECU_SENT_B */
uint8_t SENT_SENS_B_Nibble2 = 0u;
uint8_t SENT_SENS_B_Nibble1 = 0u;
uint8_t SENT_SENS_B_Nibble4 = 0u;
uint8_t SENT_SENS_B_Nibble3 = 0u;
uint8_t SENT_SENS_B_Nibble6 = 0u;
uint8_t SENT_SENS_B_Nibble5 = 0u;
uint8_t SENT_SENS_B_Nibble8 = 0u;
uint8_t SENT_SENS_B_Nibble7 = 0u;
bool_t SENT_ECUT_B_Error_SensorDiag = FALSE;
bool_t RTOS_ErrDistRefPos_B = FALSE;
bool_t RTOS_FlagValidSENT_B = FALSE;
bool_t RTOS_ErrGradBegr_B = FALSE;
uint8_t SENT_ECUT_B_CRC = 0u;
bool_t SENT_ECUT_B_Error_NibbleLength = FALSE;
bool_t SENT_ECUT_B_Error_SyncLength = FALSE;
bool_t SENT_ECUT_B_Error_MissingNibble = FALSE;
bool_t SENT_ECUT_B_Error_TooManyNibbles = FALSE;
bool_t SENT_ECUT_B_Error_CRC = FALSE;
bool_t SENT_ECUT_B_Error_TimeOut = FALSE;
bool_t SENT_ECUT_B_Error_RollingCounter = FALSE;
bool_t SENT_ECUT_B_Error_InverseNibble = FALSE;
int16_t ANGL_SENS_B_ANA_RAW = 0u;



PRIVATE uint32_t TMM_Speed_Pre[sent_channel_max] = {0u};

PRIVATE uint8_t SENT_ERR_Cnt[sent_channel_max] = {0u};


/*****************************************************************************
 * PUBLIC DATA
 *****************************************************************************/

uint16_t valid_sent_cnt_10ms = 0u;
bool_t SEQ_MEAS_START = FALSE;                         /* a trigger to upper PC */
uint16_t pwmdty_100 = 0u;
bool_t Err_SENT_TimeOut_Valid = FALSE;

tmm_position_e TMM_Position = TMM_Normal;


/*****************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/
void ECU_EndStopCounter(void)
{
	uint8_t data[8];

	CNTR_ECUT_A_ENDPOS_RAW = Cntr_Endpos_Mea[sent_channel_1];
	CNTR_ECUT_B_ENDPOS_RAW = Cntr_Endpos_Mea[sent_channel_2];

	ANGL_SENS_A_ANA_LSB = Angle_Sent_Value_Cur[sent_channel_1];
	ANGL_SENS_B_ANA_LSB = Angle_Sent_Value_Cur[sent_channel_2];

	/* Byte Order: Intel */
	data[0] = (uint8_t)CNTR_ECUT_A_ENDPOS_RAW; 
	data[1] = (uint8_t)(CNTR_ECUT_A_ENDPOS_RAW >> 8u);
	data[2] = (uint8_t)CNTR_ECUT_B_ENDPOS_RAW; 
	data[3] = (uint8_t)(CNTR_ECUT_B_ENDPOS_RAW >> 8u);

	/* Byte Order: Motorola */
	data[4] = (uint8_t)(ANGL_SENS_A_ANA_LSB >> 8u);
	data[5] = (uint8_t)ANGL_SENS_A_ANA_LSB; 
	data[6] = (uint8_t)(ANGL_SENS_B_ANA_LSB >> 8u);
	data[7] = (uint8_t)ANGL_SENS_B_ANA_LSB; 
	
	memcpy(p_CAN_MSG_BUFF[Tx_ECU_EndStopCounter],data, 8);
}

void ECU_SW_Version(void)
{
	uint8_t data[8];

	/* Byte Order: Intel */
	data[0] = (uint8_t)GLO_EngineCode; 
	data[1] = (uint8_t)GLO_SampleVersion;
	data[2] = (uint8_t)GLO_SoftwareVersion; 
	data[3] = (uint8_t)GLO_Hardware; 
	data[4] = (uint8_t)GLO_ParameterSet; 
	data[5] = (uint8_t)(GLO_ParameterSet >> 8u);
	data[6] = 0u;
	data[7] = 0u; 
	
	memcpy(p_CAN_MSG_BUFF[Tx_ECU_SW_Version],data, 8);
}

void ECU_Modi(void)
{
	uint8_t data[8];
	uint8_t flags=0u;
	int16_t speed_value;
	uint32_t delta_speed = 0u;
	bool_t err_grad_begr;

	speed_value = (int16_t)TMM_Speed_Cur[sent_channel_1];
	if(motor_direction[sent_channel_1] == Rotating_positve)
	{
		if(speed_value > Shift_Speed_Max)
		{
			SPAN_SENS_A_ANA_MEA = Shift_Speed_Max;
		}
		else
		{
			SPAN_SENS_A_ANA_MEA = speed_value;
		}
	}
	else if(motor_direction[sent_channel_1] == Rotating_negative)
	{
		if(speed_value > Shift_Speed_Max)
		{
			SPAN_SENS_A_ANA_MEA = -Shift_Speed_Max;
		}
		else
		{
			SPAN_SENS_A_ANA_MEA = -speed_value;
		}
	}
	else
	{
		SPAN_SENS_A_ANA_MEA = 0u;
	}

	speed_value = (int16_t)TMM_Speed_Cur[sent_channel_2];
	if(motor_direction[sent_channel_2] == Rotating_positve)
	{
		if(speed_value > Shift_Speed_Max)
		{
			SPAN_SENS_B_ANA_MEA = Shift_Speed_Max;
		}
		else
		{
			SPAN_SENS_B_ANA_MEA = speed_value;
		}
	}
	else if(motor_direction[sent_channel_2] == Rotating_negative)
	{
		if(speed_value > Shift_Speed_Max)
		{
			SPAN_SENS_B_ANA_MEA = -Shift_Speed_Max;
		}
		else
		{
			SPAN_SENS_B_ANA_MEA = -speed_value;
		}
	}
	else
	{
		SPAN_SENS_B_ANA_MEA = 0u;
	}

	
	CURR_DCMR_A_ANA_RMS = ((int32_t)CURR_DCMR_RMS_100ms[sent_channel_1] * CURR_DCMR_X_ANA_RMS_FACTOR + 500u) /1000u;		/* 1A */
	CURR_DCMR_B_ANA_RMS = ((int32_t)CURR_DCMR_RMS_100ms[sent_channel_2] * CURR_DCMR_X_ANA_RMS_FACTOR + 500u) /1000u;		/* 1A */

	/* Byte Order: Intel */

	data[0] = (uint8_t)((SPAN_SENS_A_ANA_MEA + SPAN_SENS_X_ANA_MEA_FACTOR/2)/SPAN_SENS_X_ANA_MEA_FACTOR);
	data[1] = (uint8_t)((SPAN_SENS_B_ANA_MEA + SPAN_SENS_X_ANA_MEA_FACTOR/2)/SPAN_SENS_X_ANA_MEA_FACTOR);
	data[2] = (uint8_t)CURR_DCMR_A_ANA_RMS;
	data[3] = (uint8_t)CURR_DCMR_B_ANA_RMS;

	SENT_ECUT_A_FlagValidSent = RTOS_FlagValidSENT_A;
	SENT_ECUT_B_FlagValidSent = RTOS_FlagValidSENT_B;


	if((TMM_Speed_Cur[sent_channel_1]>0)&&(TMM_Angle_Deviation[sent_channel_1]>AngleDevMax))
	{
		if(TMM_Speed_Cur[sent_channel_1] > TMM_Speed_Pre[sent_channel_1])
		{
			delta_speed = TMM_Speed_Cur[sent_channel_1] - TMM_Speed_Pre[sent_channel_1];			
		}
		else
		{
			delta_speed = TMM_Speed_Pre[sent_channel_1] - TMM_Speed_Cur[sent_channel_1];
		}
		
		if(delta_speed > ShiftSpeedDevMax)
		{
			err_grad_begr = TRUE;

		}
		else{
				err_grad_begr = FALSE; 
			}
		
	}
	else
	{
		err_grad_begr = FALSE; 
	}

	TMM_Speed_Pre[sent_channel_1] = TMM_Speed_Cur[sent_channel_1];
	//SENT_ECUT_A_GradientBegr = ErrGradBegr[sent_channel_1];
	//RTOS_ErrGradBegr_A = ErrGradBegr[sent_channel_1];

	SENT_ECUT_A_GradientBegr = err_grad_begr;
	RTOS_ErrGradBegr_A = err_grad_begr;

	SENT_ECUT_B_GradientBegr = ErrGradBegr[sent_channel_2];
	RTOS_ErrGradBegr_B = ErrGradBegr[sent_channel_2];


	flags = (SENT_ECUT_A_GradientBegr)? 0x01 : 0x00;
	flags |= (SENT_ECUT_B_GradientBegr)? 0x02 : 0x00;
	flags |= (SENT_ECUT_A_FlagValidSent)? 0x04 : 0x00;
	flags |= (SENT_ECUT_B_FlagValidSent)? 0x08 : 0x00;

	if(Motor_rotating[sent_channel_1]==TRUE)
	{
		EMC_ModeCtrl_A = 3u;
	}
	else
	{
		EMC_ModeCtrl_A = 1u;
	}

	if(Motor_rotating[sent_channel_2]==TRUE)
	{
		EMC_ModeCtrl_B = 3u;
	}
	else
	{
		EMC_ModeCtrl_B = 1u;
	}


	flags |= (EMC_ModeCtrl_A << 4u)&0x30;

	flags |= (EMC_ModeCtrl_B << 6u)&0xC0;

	data[4] = flags;

	data[5] = 0u;

	
	FLAG_TOUT_A_DIG_RAW = FLAG_TOUT_DIG_RAW[sent_channel_1];
	FLAG_TOUT_B_DIG_RAW	= FLAG_TOUT_DIG_RAW[sent_channel_2];

	if(Flag_Ref_Run_Status[sent_channel_1] == ref_failed)
	{
		SENT_ECUT_A_Error_DistRefPos = TRUE;
		RTOS_ErrDistRefPos_A = TRUE;
	}
	else
	{
		SENT_ECUT_A_Error_DistRefPos = FALSE;
		RTOS_ErrDistRefPos_A = FALSE;
	}

	if(Flag_Ref_Run_Status[sent_channel_2] == ref_failed)
	{
		SENT_ECUT_B_Error_DistRefPos = TRUE;
		RTOS_ErrDistRefPos_B = TRUE;
	}
	else
	{
		SENT_ECUT_B_Error_DistRefPos = FALSE;
		RTOS_ErrDistRefPos_B = FALSE;
	}

	TII_FlagIgnSig = TRUE;					/* when the TCU is power on , then true */

	if(Motor_rotating[sent_channel_1]==TRUE)
	{
		TOA_FlagAmpEn_A = TRUE;
	}
	else
	{
		TOA_FlagAmpEn_A = FALSE;
	}

	if(Motor_rotating[sent_channel_2]==TRUE)
	{
		TOA_FlagAmpEn_B = TRUE;
	}
	else
	{
		TOA_FlagAmpEn_B = FALSE;
	}
	
	flags = (FLAG_TOUT_A_DIG_RAW)? 0x01 : 0x00;
	flags |= (FLAG_TOUT_B_DIG_RAW)? 0x02 : 0x00;
	flags |= (SENT_ECUT_A_Error_DistRefPos)? 0x04 : 0x00;
	flags |= (SENT_ECUT_B_Error_DistRefPos)? 0x08 : 0x00;
	flags |= (TII_FlagIgnSig)? 0x20 : 0x00;
	flags |= (TOA_FlagAmpEn_A)? 0x40 : 0x00;
	flags |= (TOA_FlagAmpEn_B)? 0x80 : 0x00;

	data[6] = flags;
	//data[7] = (uint8_t)(VOLT_BATT_M_ANA_RAW * VOLT_BATT_M_ANA_RAW_FACTOR); 

	VOLT_BATT_M_ANA_RAW = VOLT_BATT/100u;
	data[7] = VOLT_BATT_M_ANA_RAW; 

	
	memcpy(p_CAN_MSG_BUFF[Tx_ECU_Modi],data, 8);
}

void ECU_AngleOutput(void)
{
	uint8_t data[8];

	ANGL_ECUT_A_ANA_MEA = TMM_Operating[sent_channel_1].target_angle;
	ANGL_SENS_A_ANA_MEA = TMM_Angle_Cur[sent_channel_1];

	ANGL_ECUT_B_ANA_MEA = TMM_Operating[sent_channel_2].target_angle;
	ANGL_SENS_B_ANA_MEA = TMM_Angle_Cur[sent_channel_2];

	/* Byte Order: Motorola */
	data[0] = (uint8_t)(ANGL_ECUT_A_ANA_MEA * ANGL_ECUT_X_ANA_MEA_FACTOR >> 8u);
	data[1] = (uint8_t)ANGL_ECUT_A_ANA_MEA * ANGL_ECUT_X_ANA_MEA_FACTOR; 
	data[2] = (uint8_t)(ANGL_ECUT_B_ANA_MEA * ANGL_ECUT_X_ANA_MEA_FACTOR >> 8u);
	data[3] = (uint8_t)ANGL_ECUT_B_ANA_MEA * ANGL_ECUT_X_ANA_MEA_FACTOR; 
	data[4] = (uint8_t)(ANGL_SENS_A_ANA_MEA * ANGL_SENS_X_ANA_MEA_FACTOR >> 8u);
	data[5] = (uint8_t)ANGL_SENS_A_ANA_MEA * ANGL_SENS_X_ANA_MEA_FACTOR; 
	data[6] = (uint8_t)(ANGL_SENS_B_ANA_MEA * ANGL_SENS_X_ANA_MEA_FACTOR >> 8u);
	data[7] = (uint8_t)ANGL_SENS_B_ANA_MEA * ANGL_SENS_X_ANA_MEA_FACTOR; 
	
	memcpy(p_CAN_MSG_BUFF[Tx_ECU_AngleOutput],data, 8);
}

void ECU_SENT_ErrorCount(void)
{
	/* 4 bytes */
	uint8_t data[4];
	uint8_t err_cnt_A = 0u;	
	uint8_t err_cnt_B = 0u;	

	//SENT_ErrCount_A = Sent_Unvalid_cnt[sent_channel_1];
	//SENT_ErrCount_B = Sent_Unvalid_cnt[sent_channel_2];				

	/* Byte Order: Intel */
	//if(SENT_ErrCount_A == 50u)
	//{
	//	SENT_ERR_Cnt[sent_channel_1]++;
	//}
	//
	//if(SENT_ErrCount_B == 50u)
	//{
	//	SENT_ERR_Cnt[sent_channel_2]++;
	//}
	

	data[0] = SENT_ErrCount_A; 
	data[1] = SENT_ErrCount_B;
	data[2] = TX_SENT_TEMP_A; 
	data[3] = TX_SENT_TEMP_B;
	
	memcpy(p_CAN_MSG_BUFF[Tx_ECU_SENT_ErrorCount],data, 4);
}

void ECU_LimitOutput(void)
{
	memcpy(p_CAN_MSG_BUFF[Tx_ECU_LimitOutput],TMM_RX_CAN.ecu_limitinput, 8);
}

void ECU_RegelParaOutput(void)
{
	memcpy(p_CAN_MSG_BUFF[Tx_ECU_RegelParaOutput],TMM_RX_CAN.ecu_regelparainput, 8);
}

void ECU_RefPosOutput(void)
{
	uint8_t data[8];
	uint8_t val=0u;
	uint8_t pwm_duty;
	
	/* TMM A reference position */
	if(TMM_Operating[sent_channel_1].tmm_operating_mode == Refrence_Run)
	{
		if(motor_direction[sent_channel_1] == Rotating_positve)
		{
			if(Angle_Sent_Value_Cur[sent_channel_1] > 2400u)
			{
				ANGL_ECUT_A_REFPOSRIGHT_RAW = Angle_Sent_Value_Cur[sent_channel_1];
			}
			else
			{
				ANGL_ECUT_A_REFPOSRIGHT_RAW = 2400u;
			}

			ANGL_ECUT_A_REFPOSLEFT_RAW = 1700u;
		}
		else if(motor_direction[sent_channel_1] == Rotating_negative)
		{
			if(Angle_Sent_Value_Cur[sent_channel_1] < 1700u)
			{
				ANGL_ECUT_A_REFPOSLEFT_RAW = Angle_Sent_Value_Cur[sent_channel_1];
			}
			else
			{
				ANGL_ECUT_A_REFPOSLEFT_RAW = 1700u;
			}

			//ANGL_ECUT_A_REFPOSRIGHT_RAW = 2400u;
		}
		else
		{
			//ANGL_ECUT_A_REFPOSLEFT_RAW = TMM_Endstop_Cur[sent_channel_1].sent_lower_endstop;
			//ANGL_ECUT_A_REFPOSRIGHT_RAW = TMM_Endstop_Cur[sent_channel_1].sent_upper_endstop;
		}
	}
	else
	{
		ANGL_ECUT_A_REFPOSLEFT_RAW = TMM_Endstop_Cur[sent_channel_1].sent_lower_endstop;
		ANGL_ECUT_A_REFPOSRIGHT_RAW = TMM_Endstop_Cur[sent_channel_1].sent_upper_endstop;
	}

	/* TMM B reference position */
	if(TMM_Operating[sent_channel_2].tmm_operating_mode == Refrence_Run)
	{
		if(motor_direction[sent_channel_2] == Rotating_positve)
		{
			if(Angle_Sent_Value_Cur[sent_channel_2] > 2400u)
			{
				ANGL_ECUT_B_REFPOSRIGHT_RAW = Angle_Sent_Value_Cur[sent_channel_2];
			}
			else
			{
				ANGL_ECUT_B_REFPOSRIGHT_RAW = 2400u;
			}
			ANGL_ECUT_B_REFPOSLEFT_RAW = 1700u;
		}
		else if(motor_direction[sent_channel_2] == Rotating_negative)
		{
			if(Angle_Sent_Value_Cur[sent_channel_2] < 1700u)
			{
				ANGL_ECUT_B_REFPOSLEFT_RAW = Angle_Sent_Value_Cur[sent_channel_2];
			}
			else
			{
				ANGL_ECUT_B_REFPOSLEFT_RAW = 1700u;
			}
			//ANGL_ECUT_B_REFPOSRIGHT_RAW = 2400u;
		}
		else
		{
			//ANGL_ECUT_B_REFPOSLEFT_RAW = TMM_Endstop_Cur[sent_channel_2].sent_lower_endstop;
			//ANGL_ECUT_B_REFPOSRIGHT_RAW = TMM_Endstop_Cur[sent_channel_2].sent_upper_endstop;
		}
	}
	else
	{
		ANGL_ECUT_B_REFPOSLEFT_RAW = TMM_Endstop_Cur[sent_channel_2].sent_lower_endstop;
		ANGL_ECUT_B_REFPOSRIGHT_RAW = TMM_Endstop_Cur[sent_channel_2].sent_upper_endstop;
	}


	pwm_duty = 100u - pwm_duty_read(sent_channel_1);

	if(pwm_duty > PWM_DCMR_MAX)
	{
		pwm_duty = PWM_DCMR_MAX;
	}
	else if(pwm_duty < PWM_DCMR_MIN )
	{
		pwm_duty = PWM_DCMR_MIN;
	}

	if(motor_direction[sent_channel_1] == Rotating_positve)
	{
		PPWM_DCMR_A_ANA_RAW = pwm_duty;
	}
	else if(motor_direction[sent_channel_1] == Rotating_negative)
	{
		PPWM_DCMR_A_ANA_RAW = -(int8_t)pwm_duty;
	}
	else
	{
		PPWM_DCMR_A_ANA_RAW = 0u;
	}


	pwm_duty = 100u - pwm_duty_read(sent_channel_2);

	if(pwm_duty > PWM_DCMR_MAX)
	{
		pwm_duty = PWM_DCMR_MAX;
	}
	else if(pwm_duty < PWM_DCMR_MIN )
	{
		pwm_duty = PWM_DCMR_MIN;
	}

	if(motor_direction[sent_channel_2] == Rotating_positve)
	{
		PPWM_DCMR_B_ANA_RAW = pwm_duty;
	}
	else if(motor_direction[sent_channel_2] == Rotating_negative)
	{
		PPWM_DCMR_B_ANA_RAW = -(int8_t)pwm_duty;
	}
	else
	{
		PPWM_DCMR_B_ANA_RAW = 0u;
	}
	
	/* Byte Order: Intel */
	data[0] = (uint8_t)ANGL_ECUT_A_REFPOSLEFT_RAW; 
	val = (uint8_t)(ANGL_ECUT_A_REFPOSLEFT_RAW >> 8u) & 0x0F;
	val |= ((uint8_t)ANGL_ECUT_B_REFPOSLEFT_RAW & 0x0F) << 4u;
	data[1] = val;
	data[2] = (uint8_t)(ANGL_ECUT_B_REFPOSLEFT_RAW >> 4u); 

	data[3] = (uint8_t)ANGL_ECUT_A_REFPOSRIGHT_RAW; 
	val = (uint8_t)(ANGL_ECUT_A_REFPOSRIGHT_RAW >> 8u) & 0x0F;
	val |= ((uint8_t)ANGL_ECUT_B_REFPOSRIGHT_RAW & 0xF0) << 4u;
	data[4] = val;
	data[5] = (uint8_t)(ANGL_ECUT_B_REFPOSRIGHT_RAW >> 4u); 

	data[6] = PPWM_DCMR_B_ANA_RAW;
	data[7] = PPWM_DCMR_A_ANA_RAW;

	memcpy(p_CAN_MSG_BUFF[Tx_ECU_RefPosOutput],data, 8);
}

void ECU_SENTModeOutput(void)
{
	memcpy(p_CAN_MSG_BUFF[Tx_ECU_SENTModeOutput],TMM_RX_CAN.ecu_sentmodeinput, 8);
}

/* ANA to CAN */
void Module2_low(void)
{
	uint8_t data[8];

	/* need multi factor ? */

	/* Byte Order: Intel */
	data[0] = (uint8_t)PRES_C704_CH00; 
	data[1] = (uint8_t)(PRES_C704_CH00 >> 8u);
	data[2] = (uint8_t)PRES_C704_CH01; 
	data[3] = (uint8_t)(PRES_C704_CH01 >> 8u);
	data[4] = (uint8_t)PRES_C704_CH02; 
	data[5] = (uint8_t)(PRES_C704_CH02 >> 8u);
	data[6] = (uint8_t)PRES_C704_CH03; 
	data[7] = (uint8_t)(PRES_C704_CH03 >> 8u);

	memcpy(p_CAN_MSG_BUFF[Tx_Module2_low],data, 8);
}

/* ANA to CAN */
void Module2_high(void)
{
	uint8_t data[8];

	/* need multi factor ? */

	/* Byte Order: Intel */
	data[0] = (uint8_t)PRES_C704_CH04; 
	data[1] = (uint8_t)(PRES_C704_CH04 >> 8u);
	data[2] = (uint8_t)PRES_C704_CH05; 
	data[3] = (uint8_t)(PRES_C704_CH05 >> 8u);
	data[4] = (uint8_t)HUMI_C704_CH06; 
	data[5] = (uint8_t)(HUMI_C704_CH06 >> 8u);
	data[6] = (uint8_t)CURR_C704_CH07; 
	data[7] = (uint8_t)(CURR_C704_CH07 >> 8u);

	memcpy(p_CAN_MSG_BUFF[Tx_Module2_high],data, 8);
}

/* TEMP to CAN */
void Module1_low(void)
{
	uint8_t data[8];

	/* need multi factor ? */

	/* Byte Order: Intel */
	data[0] = (uint8_t)TEMP_C703_CH00; 
	data[1] = (uint8_t)(TEMP_C703_CH00 >> 8u);
	data[2] = (uint8_t)TEMP_C703_CH01; 
	data[3] = (uint8_t)(TEMP_C703_CH01 >> 8u);
	data[4] = (uint8_t)TEMP_C703_CH02; 
	data[5] = (uint8_t)(TEMP_C703_CH02 >> 8u);
	data[6] = (uint8_t)TEMP_C703_CH03; 
	data[7] = (uint8_t)(TEMP_C703_CH03 >> 8u);

	memcpy(p_CAN_MSG_BUFF[Tx_Module1_low],data, 8);
}

/* TEMP to CAN */
void Module1_high(void)
{
	uint8_t data[8];

	/* need multi factor ? */

	/* Byte Order: Intel */
	data[0] = (uint8_t)TEMP_C703_CH04; 
	data[1] = (uint8_t)(TEMP_C703_CH04 >> 8u);
	data[2] = (uint8_t)TEMP_C703_CH05; 
	data[3] = (uint8_t)(TEMP_C703_CH05 >> 8u);
	data[4] = (uint8_t)TEMP_C703_CH06; 
	data[5] = (uint8_t)(TEMP_C703_CH06 >> 8u);
	data[6] = (uint8_t)TEMP_C703_CH07; 
	data[7] = (uint8_t)(TEMP_C703_CH07 >> 8u);

	memcpy(p_CAN_MSG_BUFF[Tx_Module1_high],data, 8);
}

/* LIN_RSCV_Data_B */
void LIN_RSCV_Data_B(void)
{
	uint8_t data[8];

	data[0] = LIN_MSG_Receive_29[lin_channel_2][0]; 
	data[1] = LIN_MSG_Receive_29[lin_channel_2][1]; 
	data[2] = LIN_MSG_Receive_29[lin_channel_2][2]; 
	data[3] = 0u;
	data[4] = 0u;
	data[5] = 0u;
	data[6] = 0u;
	data[7] = 0u;

	memcpy(p_CAN_MSG_BUFF[Tx_LIN_RSCV_Data_B],data, 8);
}

/* LIN_RSCV_Output_B */
void LIN_RSCV_Output_B(void)
{
	uint8_t data[8];

	data[0] = LIN_MSG_Receive_28[lin_channel_2][0]; 
	data[1] = LIN_MSG_Receive_28[lin_channel_2][1]; 
	data[2] = 0u; 
	data[3] = 0u;
	data[4] = 0u;
	data[5] = 0u;
	data[6] = 0u;
	data[7] = 0u;

	memcpy(p_CAN_MSG_BUFF[Tx_LIN_RSCV_Output_B],data, 8);
}

/* LIN_RSCV_Data_A */
void LIN_RSCV_Data_A(void)
{
	uint8_t data[8];

	data[0] = LIN_MSG_Receive_29[lin_channel_1][0]; 
	data[1] = LIN_MSG_Receive_29[lin_channel_1][1]; 
	data[2] = LIN_MSG_Receive_29[lin_channel_1][2]; 
	data[3] = 0u;
	data[4] = 0u;
	data[5] = 0u;
	data[6] = 0u;
	data[7] = 0u;

	memcpy(p_CAN_MSG_BUFF[Tx_LIN_RSCV_Data_A],data, 8);
}

/* LIN_RSCV_Output_A */
void LIN_RSCV_Output_A(void)
{
	uint8_t data[8];

	data[0] = LIN_MSG_Receive_28[lin_channel_1][0]; 
	data[1] = LIN_MSG_Receive_28[lin_channel_1][1]; 
	data[2] = 0u; 
	data[3] = 0u;
	data[4] = 0u;
	data[5] = 0u;
	data[6] = 0u;
	data[7] = 0u;

	memcpy(p_CAN_MSG_BUFF[Tx_LIN_RSCV_Output_A],data, 8);
}

/* CAN FAST */

void vio_write_ANA_A1A3(void)
{
	uint8_t data[8];
	uint8_t flags = 0u;
	int16_t integer_value = 0u;

	/* A1A3 : temperature */
	if(TMM_A1A3_Mode[sent_channel_1] == Sent_Temperature)
	{
		integer_value = (uint16_t)sent_nibbles_value[sent_channel_1][sent_state_data5] << 8u;
		integer_value |= (uint16_t)sent_nibbles_value[sent_channel_1][sent_state_data4] << 4u;
		integer_value |= (uint16_t)sent_nibbles_value[sent_channel_1][sent_state_data3];

		// byte order: motorola
		data[0] = (uint8_t)(integer_value >> 8u);
		data[1] = (uint8_t)integer_value;
	}
	else
	{
		data[0] = 0u;
		data[1] = 0u;
	}

	if(TMM_A1A3_Mode[sent_channel_2] == Sent_Temperature)
	{
		integer_value = (uint16_t)sent_nibbles_value[sent_channel_2][sent_state_data5] << 8u;
		integer_value |= (uint16_t)sent_nibbles_value[sent_channel_2][sent_state_data4] << 4u;
		integer_value |= (uint16_t)sent_nibbles_value[sent_channel_2][sent_state_data3];

		// byte order: motorola
		data[2] = (uint8_t)(integer_value >> 8u);
		data[3] = (uint8_t)integer_value;
	}
	else
	{
		data[2] = 0u;
		data[3] = 0u;
	}
	
	/* two ANA input, byte order: motorola */
	data[4] = (uint8_t)(VOLT_AD_Input[0u] >> 8u);
	data[5] = (uint8_t)VOLT_AD_Input[0u]; 
	data[6] = (uint8_t)(VOLT_AD_Input[1u] >> 8u);
	data[7] = (uint8_t)VOLT_AD_Input[1u]; 

	memcpy(BMS_11._c,data, 8);
 
}



//void ECU_SENT_A(void)
//{
//	uint8_t data[8];
//	uint8_t flags = 0u;
//	int16_t integer_value = 0u;
//
//	/* Byte Order: Intel */
//	data[0] = ((uint8_t)SENT_SENS_A_Nibble1 << 4u) | (uint8_t)SENT_SENS_A_Nibble2;
//	data[1] = ((uint8_t)SENT_SENS_A_Nibble3 << 4u) | (uint8_t)SENT_SENS_A_Nibble4;
//	data[2] = ((uint8_t)SENT_SENS_A_Nibble5 << 4u) | (uint8_t)SENT_SENS_A_Nibble6;
//	data[3] = ((uint8_t)SENT_SENS_A_Nibble7 << 4u) | (uint8_t)SENT_SENS_A_Nibble8;
//
//	flags = (SENT_ECUT_A_Error_SensorDiag)? 0x01 : 0x00;
//	flags |= (RTOS_ErrDistRefPos_A)? 0x02 : 0x00;
//	flags |= (RTOS_FlagValidSENT_A)? 0x04 : 0x00;
//	flags |= (RTOS_ErrGradBegr_A)? 0x08 : 0x00;
//	flags |= SENT_ECUT_A_CRC << 4u;
//	data[4] = flags;
//
//	flags = (SENT_ECUT_A_Error_NibbleLength)? 0x01 : 0x00;
//	flags |= (SENT_ECUT_A_Error_SyncLength)? 0x02 : 0x00;
//	flags |= (SENT_ECUT_A_Error_MissingNibble)? 0x04 : 0x00;
//	flags |= (SENT_ECUT_A_Error_TooManyNibbles)? 0x08 : 0x00;
//	flags |= (SENT_ECUT_A_Error_CRC)? 0x10 : 0x00;
//	flags |= (SENT_ECUT_A_Error_TimeOut)? 0x20: 0x00;
//	flags |= (SENT_ECUT_A_Error_RollingCounter)? 0x40 : 0x00;
//	flags |= (SENT_ECUT_A_Error_InverseNibble)? 0x80 : 0x00;
//	data[5] = flags;
//
//	/* Byte Order: Motorola */
//	integer_value = ANGL_SENS_A_ANA_RAW * ANGL_SENS_X_ANA_RAWFACTOR; 
//	data[6] = (uint8_t)(integer_value >> 8u);
//	data[7] = (uint8_t)integer_value; 
//	
//	memcpy(p_CAN_MSG_BUFF[Tx_ECU_SENT_A],data, 8);
//}






//void vio_write_TMM_Sent_Raw(void)
//{
//	uint8_t msg;
//	uint8_t Sent_Cyclic_CNT = Sent_Rawdata_CNT[sent_channel_1];
//	
//	for(msg=0;msg<1;msg++)
//	{
//		CAN_CYCLIC_STOP((IlTransmitHandle) exc_cyclic_control[msg].tx_handle);
//		
//		if(msg<Sent_Cyclic_CNT)
//		{
//			memcpy(p_Sent_Cyclic_Message[msg],&Sent_Rawdata[sent_channel_1][msg][0], 8);
//			
//			if(msg < 1)
//			{
//				Sent_Rawdata_CNT[sent_channel_1] = 0u;
//			}
//			
//			CAN_CYCLIC_START((IlTransmitHandle) exc_cyclic_control[msg].tx_handle);
//			
//		}
//		exc_cyclic_control[msg].enable_flag = FALSE;
//	}
//}
//
//void vio_write_TMM_Current_Raw(void)
//{
//	uint8_t msg;
//	uint8_t data[8];
//	
//	for(msg=0;msg<1;msg++)
//	{		
//		data[0] = (uint8_t)(CURR_DCMR_RAW[msg*4 + 0] >> 8);
//		data[1] = (uint8_t)(CURR_DCMR_RAW[msg*4 + 0]);
//		data[2] = (uint8_t)(CURR_DCMR_RAW[msg*4 + 1] >> 8);
//		data[3] = (uint8_t)(CURR_DCMR_RAW[msg*4 + 1]);
//		
//		if(msg<12)
//		{
//			data[4] = (uint8_t)(CURR_DCMR_RAW[msg*4 + 2] >> 8);
//			data[5] = (uint8_t)(CURR_DCMR_RAW[msg*4 + 2]);
//			data[6] = (uint8_t)(CURR_DCMR_RAW[msg*4 + 3] >> 8);
//			data[7] = (uint8_t)(CURR_DCMR_RAW[msg*4 + 3]);
//		}
//		else
//		{
//			data[4] = PWM_In_Freq_Cur;
//			data[5] = PWM_In_Duty_Cur;
//			data[6] = (uint8_t)(VOLT_AD_Input>>8);
//			data[7] = (uint8_t)VOLT_AD_Input;
//		}
//		
//		memcpy(p_Current_Cyclic_Message[msg],data, 8);
//
//	}
//	
//}
//
//
//
//void vio_write_TMM_Display1(void)
//{	
//	uint8_t i;
//	uint8_t data[8] = {0x00};
//	uint8_t flags = 0u;	
//		
//	data[0] = TMM_Display1_MASK;
//	data[1] = (uint8_t)(TMM_Angle_Tar>>8); 
//	data[2] = (uint8_t)TMM_Angle_Tar;
//	data[3] = (uint8_t)(TMM_Angle_Cur[sent_channel_1]>>8);
//	data[4] = (uint8_t)TMM_Angle_Cur[sent_channel_1];
//
//	flags = Motor_rotating ? Motor_Rotating_MASK : 0x00u;
//	if(motor_direction==Rotating_positve)
//	{
//		flags &= ~Motor_Direction_MASK;
//	}
//	else if (motor_direction==Rotating_negative)
//	{
//		flags |= Motor_Direction_MASK;
//	}
//	//flags |= SEQ_MEAS_START ? SEQ_MEAS_START_MASK : 0x00u;
//	if(TMM_Position==TMM_UME)
//	{
//		flags |= TMM_UME_MASK;
//	}
//	else if(TMM_Position==TMM_LME)
//	{
//		flags |= TMM_LME_MASK;
//	}
//	else
//	{
//		flags &= 0x3F;
//	}
//	
//	if(Angle_Sent_Value_Cur[sent_channel_1] > TMM_Sent_Upper_MAX)
//	{
//		Angle_Sent_Value_Cur[sent_channel_1] = TMM_Sent_Upper_MAX;
//	}
//	else if (Angle_Sent_Value_Cur[sent_channel_1] < TMM_Sent_Lower_MIN)
//	{
//		Angle_Sent_Value_Cur[sent_channel_1] = TMM_Sent_Lower_MIN;
//	}
//	data[5] = flags | ((uint8_t)(Angle_Sent_Value_Cur[sent_channel_1]>>8) & 0x0F);
//	data[6] = (uint8_t)Angle_Sent_Value_Cur[sent_channel_1];
//	
//	if(Motor_rotating)
//	{
//		data[7] = (uint8_t)TMM_Speed_Cur[sent_channel_1];
//	}
//	else
//	{
//		data[7] = 0x00;
//	}
//	
//	
//	BMS_CH1._c[0] = data[0];
//	BMS_CH1._c[1] = data[1];
//	BMS_CH1._c[2] = data[2];
//	BMS_CH1._c[3] = data[3];
//	BMS_CH1._c[4] = data[4];
//	BMS_CH1._c[5] = data[5];
//	BMS_CH1._c[6] = data[6];
//	BMS_CH1._c[7] = data[7];
//
//}
//
//void vio_write_TMM_Display2(void)
//{
//	uint8_t data[8] = {0x00};	
//	uint16_t pwmdty, pwmperiod;
//	uint8_t flags = 0u;	
//		
//	data[0] = TMM_Display2_MASK;
//	
//	if(Motor_rotating)
//	{
//		pwmperiod = ((uint16_t)PWMPER0 << 8) | (uint16_t)PWMPER1;
//		pwmdty = pwmperiod - (((uint16_t)PWMDTY0 << 8) | (uint16_t)PWMDTY1);
//				
//		pwmdty_100 = (uint8_t)(((uint32_t)pwmdty*100 + (uint32_t)pwmperiod/2) / (uint32_t)pwmperiod);
//		
//		if(pwmdty_100 > PWM_DCMR_MAX)
//		{
//			pwmdty_100 = PWM_DCMR_MAX;
//		}
//		else if(pwmdty_100 < PWM_DCMR_MIN)
//		{
//			pwmdty_100 = PWM_DCMR_MIN;
//		}
//		 
//		 data[1] = pwmdty_100;
//	}
//	else
//	{
//		data[1] = 0u;
//	}
//	data[2] = (uint8_t)(CURR_DCMR_AVG_10ms>>8); 
//	data[3] = (uint8_t)CURR_DCMR_AVG_10ms;
//	data[4] = (uint8_t)(CURR_SENS_A_AVG_10ms>>8);                  /* 0.1 mA */	
//	data[5] = (uint8_t)CURR_SENS_A_AVG_10ms;
//	data[6] = (uint8_t)(CNTR_ENDPOS_MEA>>8); 
//	data[7] = (uint8_t)CNTR_ENDPOS_MEA;
//	
//	BMS_CH2._c[0] = data[0];
//	BMS_CH2._c[1] = data[1];
//	BMS_CH2._c[2] = data[2];
//	BMS_CH2._c[3] = data[3];
//	BMS_CH2._c[4] = data[4];
//	BMS_CH2._c[5] = data[5];
//	BMS_CH2._c[6] = data[6];
//	BMS_CH2._c[7] = data[7];
//
//}
//
//void vio_write_TMM_Display3(void)
//{
//	uint8_t data[8] = {0x00};	
//		
//	data[0] = TMM_Display3_MASK;
//	data[1] = (uint8_t)(VOLT_DCMR>>8); 
//	data[2] = (uint8_t)VOLT_DCMR;
//	data[3] = (uint8_t)(VOLT_SENS/100);  
//	data[4] = (uint8_t)(Temp_Pt100>>8);             
//	data[5] = (uint8_t)Temp_Pt100;                                        /* temperature */
//	data[6] = (uint8_t)(Temp_Ktype>>8); 
//	data[7] = (uint8_t)Temp_Ktype;;
//	
//	BMS_Diag_14._c[0] = data[0];
//	BMS_Diag_14._c[1] = data[1];
//	BMS_Diag_14._c[2] = data[2];
//	BMS_Diag_14._c[3] = data[3];
//	BMS_Diag_14._c[4] = data[4];
//	BMS_Diag_14._c[5] = data[5];
//	BMS_Diag_14._c[6] = data[6];
//	BMS_Diag_14._c[7] = data[7];
//}
//
//void vio_write_TMM_Display4(void)
//{
//	uint8_t data[8] = {0x00};
//	uint8_t flags = 0u;	
//		
//	data[0] = TMM_Display4_MASK;
//	
//	if(Err_SENT_TimeOut_Valid)
//	{
//		data[1] = (uint8_t)(Err_SENT_TimeOut_10ms>>8); 
//		data[2] = (uint8_t)Err_SENT_TimeOut_10ms;
//		
//		Err_SENT_TimeOut_Valid = FALSE;                    /* clear Err_SENT_TimeOut_10ms */
//	}
//	else
//	{
//		data[1] = 0u; 
//		data[2] = 0u;
//	}
//	
//	data[3] = (uint8_t)(ErrCount_SENT>>8);                                     
//	data[4] = (uint8_t)ErrCount_SENT;              	
//	flags |= ErrGradBegr ? 0x01u : 0x00u;
//	flags |= ECUT_FLAG_1 ? 0x02u : 0x00u;
//	flags |= ECUT_FLAG_2 ? 0x04u : 0x00u;
//	flags |= ECUT_FLAG_3 ? 0x08u : 0x00u;
//	flags |= ECUT_FLAG_4 ? 0x10u : 0x00u;
//	data[5] = flags;
//	data[6] = (uint8_t)(TMM_Operating.input_angle_lar>>8); 
//	data[7] = (uint8_t)TMM_Operating.input_angle_lar;
//
//	BMS_Diag_15._c[0] = data[0];
//	BMS_Diag_15._c[1] = data[1];
//	BMS_Diag_15._c[2] = data[2];
//	BMS_Diag_15._c[3] = data[3];
//	BMS_Diag_15._c[4] = data[4];
//	BMS_Diag_15._c[5] = data[5];
//	BMS_Diag_15._c[6] = data[6];
//	BMS_Diag_15._c[7] = data[7];
//	
//	//BMS_Diag_15._c[0] = frame_send.data[0];
//	//BMS_Diag_15._c[1] = frame_send.data[1];
//	//BMS_Diag_15._c[2] = frame_send.data[2];
//	//BMS_Diag_15._c[3] = frame_send.data[3];
//	//BMS_Diag_15._c[4] = frame_send.state;
//	//BMS_Diag_15._c[5] = frame_send.protected_id;
//	//BMS_Diag_15._c[6] = 0;
//	//BMS_Diag_15._c[7] = 0;
//}
//
//void vio_write_TMM_Display5(void)
//{
//	uint8_t data[8] = {0x00};
//		
//	data[0] = TMM_Display5_MASK;
//	data[1] = (uint8_t)(TMM_Endstop_Cur.angle_upper_endstop>>8); 
//	data[2] = (uint8_t)TMM_Endstop_Cur.angle_upper_endstop;
//	data[3] = (uint8_t)(TMM_Endstop_Cur.angle_lower_endstop>>8); 
//	data[4] = (uint8_t)TMM_Endstop_Cur.angle_lower_endstop;                
//	data[5] = (uint8_t)(TMM_Operating.input_angle_uar>>8);                                        /* temperature */
//	data[6] = (uint8_t)TMM_Operating.input_angle_uar;
//	data[7] = 0u;
//	
//	BMS_Diag_16._c[0] = data[0];
//	BMS_Diag_16._c[1] = data[1];
//	BMS_Diag_16._c[2] = data[2];
//	BMS_Diag_16._c[3] = data[3];
//	BMS_Diag_16._c[4] = data[4];
//	BMS_Diag_16._c[5] = data[5];
//	BMS_Diag_16._c[6] = data[6];
//	BMS_Diag_16._c[7] = data[7];
//	
//	//BMS_Diag_16._c[0] = frame_send.data[0];
//	//BMS_Diag_16._c[1] = frame_send.data[1];
//	//BMS_Diag_16._c[2] = frame_send.data[2];
//	//BMS_Diag_16._c[3] = frame_send.data[3];
//	//BMS_Diag_16._c[4] = frame_send.data[4];
//	//BMS_Diag_16._c[5] = frame_send.data[5];
//	//BMS_Diag_16._c[6] = frame_send.data[6];
//	//BMS_Diag_16._c[7] = frame_send.data[7];
//}

/*****************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

