/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:       vio_read.c
 *
 * description:  
 *  Routines to decode received CAN messages.
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

/* Copyright (c) Ricardo plc 2013-2015 - All rights reserved */

#include "vio.h"        /* public header for this module */
#include "vio_p.h"      /* private header for this module */
//#include "nnm.h"
//#include "fm.h"
//#include "ts_types.h"
//#include "ts.h"
#include "basic_types.h"
#include "dio_cfg.h"
#include "exc.h"
#include "motor.h"
#include "pwm.h"
#include "tmm.h"
#include <string.h>
#include "mdm.h"
#include "mdm_ee.h"
#include "nvs.h"
#include "pid.h"
#include "lin.h"
#include "dac.h"
#include "tmm_seq.h"

/******************************************************************************
 * CALIBRATABLES
 *****************************************************************************/
/* Limit Config */
#define AngleDevMax_Def			10
#define ShiftSpeedDevMax_Def	180
#define TimeToDev_Max_Def		100             /* 100*10ms */
#define PWM_DCMR_MAX_Def        95              /* 95% */
#define PWM_DCMR_MIN_Def        5				/* 5% */
#define CURR_DCMR_MAX_Def		10				/* 10A */
#define Shift_Speed_Max_Def		180			/* ./s */


/* ECU_LimitInput */
#define RX_CCA_AngleDevMax_co_FACTOR		2
#define RX_CCA_AngleDevMax_co_OFFSET		0
#define RX_CCA_ShiftSpeedDevMax_co_FACTOR	2
#define RX_CCA_ShiftSpeedDevMax_co_OFFSET	0
#define RX_CCA_TimeTolDevMax_co_FACTOR		5		/* 1/0.2 */
#define RX_CCA_TimeTolDevMax_co_OFFSET		0
#define RX_EMC_ShiftSpeedCamDem_co_A_FACTOR	4
#define RX_EMC_ShiftSpeedCamDem_co_A_OFFSET 0
#define RX_EMC_ShiftSpeedCamDem_co_B_FACTOR	4
#define RX_EMC_ShiftSpeedCamDem_co_B_OFFSET	0

/* ECU_RegelParaInput */
#define RX_CCA_AngDeadBand_co_FACTOR		  8			/* 1/0.125 */
#define RX_CCA_AngDeadBand_co_OFFSET		  0
#define RX_CCA_KpAngleCtrl_co_FACTOR		  8			/* 1/0.125 */
#define RX_CCA_KpAngleCtrl_co_OFFSET		  0
#define RX_CCA_KiAngleCtrl_co_FACTOR		  32		/* 1/0.03125 */
#define RX_CCA_KiAngleCtrl_co_OFFSET		  0
#define RX_CCA_KdAngleCtrl_co_FACTOR		  32		/* 1/0.03125 */
#define RX_CCA_KdAngleCtrl_co_OFFSET		  0
#define RX_CCA_ShiftSpeedDeadBand_co_FACTOR   5
#define RX_CCA_ShiftSpeedDeadBand_co_OFFSET   0
#define RX_CCA_KpShiftSpeedCtrl_co_FACTOR	  8			/* 1/0.125 */
#define RX_CCA_KpShiftSpeedCtrl_co_OFFSET	  0
#define RX_CCA_KiShiftSpeedCtrl_co_FACTOR	  32		/* 1/0.03125 */
#define RX_CCA_KiShiftSpeedCtrl_co_OFFSET	  0
#define RX_CCA_MaxShiftSpeedAngCtrl_co_FACTOR 4
#define RX_CCA_MaxShiftSpeedAngCtrl_co_OFFSET 0

/* ECU_AngleInput */
#define ANGL_ECUT_X_ANA_CAN_FACTOR		64           /* 1/0.015625 */
#define ANGL_ECUT_X_ANA_CAN_OFFSET		0

/* ECU_SENTModeInput */
#define RX_INPUT_ANGL_UMA_FACTOR		1
#define RX_INPUT_ANGL_UMA_OFFSET		-90
#define RX_INPUT_ANGL_UVB_FACTOR		1
#define RX_INPUT_ANGL_UVB_OFFSET		-90
#define RX_INPUT_ANGL_OVB_FACTOR		1
#define RX_INPUT_ANGL_OVB_OFFSET		-90
#define RX_INPUT_ANGL_OMA_FACTOR		1
#define RX_INPUT_ANGL_OMA_OFFSET		-90

/* CAN2ANA_RX */
#define CTA_SET_VOLT_ECUT_X_FACTOR		158694			 /* 158694/100000000 */
#define CTA_SET_VOLT_ECUT_X_OFFSET		0
#define CTA_SET_VOLT_SENS_X_FACTOR		15259 			 /* 15259/100000000 */
#define CTA_SET_VOLT_SENS_X_OFFSET		0


#define Integer2Value_mul(type,integer,factor,offset) ((type)(integer)*factor + offset)
#define Integer2Value_div(type,integer,factor,offset) (((type)(integer) + factor/2)/factor + offset)


/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/
/* ECU_LimitInput */
PRIVATE uint16_t RX_CCA_AngleDevMax_co = 0u;
PRIVATE uint16_t RX_CCA_ShiftSpeedDevMax_co = 0u;
PRIVATE uint8_t RX_CCA_TimeTolDevMax_co = 0u;
PRIVATE uint8_t RX_TOP_PwmMax_co = 0u;
PRIVATE uint8_t RX_TOP_PwmMin_co = 0u;
PRIVATE uint8_t RX_TIC_CurEmMaxDiag_co = 0u;
PRIVATE int16_t RX_EMC_ShiftSpeedCamDem_co_A = 0u;
PRIVATE int16_t RX_EMC_ShiftSpeedCamDem_co_B = 0u;

/* ECU_RegelParaInput */
PRIVATE uint8_t RX_CCA_AngDeadBand_co = 0u;
PRIVATE uint8_t RX_CCA_KpAngleCtrl_co = 0u; 
PRIVATE uint8_t RX_CCA_KiAngleCtrl_co = 0u;
PRIVATE uint8_t RX_CCA_KdAngleCtrl_co = 0u;
PRIVATE uint16_t RX_CCA_ShiftSpeedDeadBand_co = 0u;
PRIVATE uint16_t RX_CCA_KpShiftSpeedCtrl_co = 0u;
PRIVATE uint8_t RX_CCA_KiShiftSpeedCtrl_co = 0u;
PRIVATE uint16_t RX_CCA_MaxShiftSpeedAngCtrl_co = 0u;

/* ECU_AngleInput */
PRIVATE uint8_t CYCLE_SEL_B = 0u;
PRIVATE uint8_t CYCLE_SEL_A = 0u;
PRIVATE int8_t PPWM_DCMR_B_EXT_INPUT = 0u;
PRIVATE int8_t PPWM_DCMR_A_EXT_INPUT = 0u;
bool_t ResetCutOff_B = FALSE;
bool_t ResetCutOff_A = FALSE;
bool_t Single_B = FALSE;
bool_t Single_A = FALSE;
bool_t Start_B = FALSE;
bool_t Start_A = FALSE;
bool_t Stop_B = FALSE;
bool_t Stop_A = FALSE;
PRIVATE int16_t ANGL_ECUT_B_ANA_CAN = 0u;
PRIVATE int16_t ANGL_ECUT_A_ANA_CAN = 0u;

/* ECU_SENTModeInput */
PRIVATE sensor_input_type_e RX_INPUT_TYPE_A = sensor_type_SENT;			
PRIVATE sent_CRC_version_e RX_INPUT_CRC_A = Sent_CRC_2008;
PRIVATE sent_pause_pulse_e RX_INPUT_PP_A = Sent_Dis_Pause_Pulse;
PRIVATE sent_serial_message_e RX_INPUT_SERMSG_A = Sent_No_SerMsg;
PRIVATE int16_t RX_INPUT_ANGL_UMA = 0u;
PRIVATE int16_t RX_INPUT_ANGL_UVB = 0u;
PRIVATE uint8_t RX_INPUT_A1A3_A = 0u;
PRIVATE uint8_t RX_INPUT_OPT6_A = 0u;

PRIVATE sensor_input_type_e RX_INPUT_TYPE_B = 0u;			/* 0 means SENT */
PRIVATE sent_CRC_version_e RX_INPUT_CRC_B = Sent_CRC_2008;
PRIVATE sent_pause_pulse_e RX_INPUT_PP_B = Sent_Dis_Pause_Pulse;
PRIVATE sent_serial_message_e RX_INPUT_SERMSG_B = Sent_No_SerMsg;
PRIVATE int16_t RX_INPUT_ANGL_OVB = 0u;
PRIVATE int16_t RX_INPUT_ANGL_OMA = 0u;
PRIVATE uint8_t RX_INPUT_A1A3_B = 0u;
PRIVATE uint8_t RX_INPUT_OPT6_B = 0u;


/* CAN2ANA_RX2 */
PRIVATE uint16_t CTA_SET_VOLT_ECUT_B = 0u;
PRIVATE uint16_t CTA_SET_VOLT_SENS_B = 0u;

/* CAN2ANA_RX2 */
PRIVATE uint16_t CTA_SET_VOLT_SENS_A = 0u;
PRIVATE uint16_t CTA_SET_VOLT_ECUT_A = 0u;

/* Flag to detect open loop mode */
PRIVATE bool_t Flag_OpenLoop_Ena[sent_channel_max] = {FALSE, FALSE};
PRIVATE bool_t Flag_OpenLoop_Ena_prev[sent_channel_max] = {FALSE, FALSE};

/* LIN */

/* Rx */
PRIVATE bool_t RX_RSCV_CALIBR_CMMD_B = FALSE;				
PRIVATE uint8_t RX_RSCV_POS_B = 0u;						/* uint : % */ 

PRIVATE bool_t RX_RSCV_CALIBR_CMMD_A = FALSE;				
PRIVATE uint8_t RX_RSCV_POS_A = 0u;						/* uint : % */ 

/* Tx */
PRIVATE uint16_t TX_RSCV_SENS_B	= 0u;							/* postion output : TX_RSCV_SENS_B/1024 */
PRIVATE bool_t TX_RSCV_CommErr_B = FALSE;
PRIVATE bool_t TX_RSCV_Operational_Fault_B = FALSE;
PRIVATE bool_t TX_RSCV_Internal_Fault_B = FALSE;
PRIVATE uint8_t TX_RSCV_Calibration_Status_B = 0u;
PRIVATE uint8_t TX_RSCV_RollingCounter_B = 0u;

PRIVATE bool_t TX_RSCV_CALIBR_CMMD_B = FALSE;				
PRIVATE uint8_t TX_RSCV_POS_B = 0u;	

PRIVATE uint16_t TX_RSCV_SENS_A	= 0u;							/* postion output : TX_RSCV_SENS_B/1024 */
PRIVATE bool_t TX_RSCV_CommErr_A = FALSE;
PRIVATE bool_t TX_RSCV_Operational_Fault_A = FALSE;
PRIVATE bool_t TX_RSCV_Internal_Fault_A = FALSE;
PRIVATE uint8_t TX_RSCV_Calibration_Status_A = 0u;
PRIVATE uint8_t TX_RSCV_RollingCounter_A = 0u;

PRIVATE bool_t TX_RSCV_CALIBR_CMMD_A = FALSE;				
PRIVATE uint8_t TX_RSCV_POS_A = 0u;	

PRIVATE uint16_t TMM_SEQ_STEP_ID_rx = 0u;

/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/
/* Flag just run one single cycle */
bool_t Flag_Single_Cycle[sent_channel_max] = {FALSE, FALSE};

/* Limit Config */
/* angle deviation which leads to a PWM shut off after a certain time */
uint16_t AngleDevMax = AngleDevMax_Def;

/* shift speed deviation which leads to a PWM shut off after a certain time */
uint16_t ShiftSpeedDevMax = ShiftSpeedDevMax_Def;

/* tolerance time for high deviations */
uint16_t TimeToDev_Max = TimeToDev_Max_Def;

/* Maximal allowed PWM */
uint16_t PWM_DCMR_MAX = PWM_DCMR_MAX_Def;

/* Minimal allowed PWM */
uint16_t PWM_DCMR_MIN = PWM_DCMR_MIN_Def;

/* Threshold to detect a too high e-motor current, which could lead to a thermal damage */
uint8_t CURR_DCMR_MAX = CURR_DCMR_MAX_Def;

/* maximum shift speed limit during angle control */
int16_t Shift_Speed_Max = Shift_Speed_Max_Def;

/* PID control */
//float KP_ShiftSpeed = 150u;
//float KI_ShiftSpeed = 30u;
//uint8_t KD_ShiftSpeed = 50u;
uint8_t Dead_Band_ShiftSpeed = 10u;       /* permitted speed inaccuracy */

uint8_t KP_AngCtrl = 0u;
uint8_t KI_AngCtrl = 0u;
uint8_t KD_AngCtrl = 0u;
uint8_t Dead_Band_AngCtrl = 3u;      	 /* permitted angle inaccuracy */

#pragma align on
/* SENT mode */
sent_CRC_version_e TMM_Sent_CRC_Version[sent_channel_max] = {Sent_CRC_2008, Sent_CRC_2010};
sent_pause_pulse_e TMM_EN_Pause_Pulse[sent_channel_max] = {Sent_Dis_Pause_Pulse, Sent_En_Pause_Pulse};
sent_serial_message_e TMM_SerMSg[sent_channel_max] = {Sent_No_SerMsg, Sent_No_SerMsg};
sent_A1A3_mode_e TMM_A1A3_Mode[sent_channel_max] = {Sent_RollingCnter, Sent_RollingCnter};
//bool_t TMM_Dis_Rolling_Cnter = FALSE;
#pragma align off




/* TMM1.0*/
uint8_t PWM_Freq_Index = 0u;
uint16_t Still_Time_OperMode = 10u;         /* 10*100ms */
bool_t  Flag_Reset_Cyclic_Cnter = FALSE;
int16_t TMM_Safety_Angle = 40u;

/* CAN ID*/
//bool_t Flag_New_RX_CANID = 0u;
//uint16_t TMM_Angle_NewID = 0u;
//uint16_t TMM_Sent_NewID = 0u;
//uint16_t TMM_Scaling_NewID = 0u;
//bool_t Flag_New_TX_CANID = 0u; 
//uint16_t Sent_Decode_NewID = 0u; 
//uint16_t TMM_Display1_NewID = 0u;
//uint16_t TMM_Display2_NewID = 0u;
//uint8_t TMM_CAN_BTR0 = 0x81u;
//bool_t Flag_Restore_Fcty_Defs = FALSE;

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
/**
 *
 * transfer users' signals to bsw's variables about TMM limit config.
 *
 */
PRIVATE void vio_read_TMM_Limit_Config(void)
{
	/* set speed */
	if(RX_EMC_ShiftSpeedCamDem_co_A > Shift_Speed_Max)
	{
		TMM_Operating[sent_channel_1].target_speed = Shift_Speed_Max;
	}
	else if(RX_EMC_ShiftSpeedCamDem_co_A < (-Shift_Speed_Max))
	{
		TMM_Operating[sent_channel_1].target_speed = -Shift_Speed_Max;
	}
	else
	{
		TMM_Operating[sent_channel_1].target_speed = RX_EMC_ShiftSpeedCamDem_co_A;
	}

	if(RX_EMC_ShiftSpeedCamDem_co_B > Shift_Speed_Max)
	{
		TMM_Operating[sent_channel_2].target_speed = Shift_Speed_Max;
	}
	else if(RX_EMC_ShiftSpeedCamDem_co_B < (-Shift_Speed_Max))
	{
		TMM_Operating[sent_channel_2].target_speed = -Shift_Speed_Max;
	}
	else
	{
		TMM_Operating[sent_channel_2].target_speed = RX_EMC_ShiftSpeedCamDem_co_B;
	}

	
	CURR_DCMR_MAX = RX_TIC_CurEmMaxDiag_co;
	AngleDevMax = RX_CCA_AngleDevMax_co;
	TimeToDev_Max = (uint16_t)RX_CCA_TimeTolDevMax_co * 100;			// s trans to 10ms
	ShiftSpeedDevMax = RX_CCA_ShiftSpeedDevMax_co;

	PWM_DCMR_MIN = RX_TOP_PwmMin_co;
	PWM_DCMR_MAX = RX_TOP_PwmMax_co;

	//PWM_PID_Max = PWM_DCMR_MAX*50u;
	//PWM_PID_Min = PWM_DCMR_MIN*50u;

}

/**
 *
 * transfer users' signals to bsw's variables about regel parameter.
 *
 */
PRIVATE void vio_read_TMM_Regel_Para(void)
{
	/* tbd */

	Shift_Speed_Max = (int16_t)RX_CCA_MaxShiftSpeedAngCtrl_co;

	TMM_Speed_KP = RX_CCA_KpShiftSpeedCtrl_co/100.0f;
	TMM_Speed_KI = RX_CCA_KiShiftSpeedCtrl_co/100.0f;

	//KD_ShiftSpeed;
	Dead_Band_ShiftSpeed = RX_CCA_ShiftSpeedDeadBand_co + 10u;


	if((RX_CCA_KpAngleCtrl_co > 50u)||(RX_CCA_KiAngleCtrl_co > 5u))				//Kp 0.5, Ki 5
	{
		Flag_Pos_PID_En = TRUE;

		TMM_Position_KP = RX_CCA_KpAngleCtrl_co/100.0f;
		TMM_Position_KI = RX_CCA_KiAngleCtrl_co/1000.0f;
		TMM_Position_KD = RX_CCA_KdAngleCtrl_co/100.0f;
	}
	else
	{
		Flag_Pos_PID_En = FALSE;
	}

	if(RX_CCA_AngDeadBand_co==1u)
	{
		Dead_Band_AngCtrl = 10u;				/* OMA : 100 */
	}
	else if(RX_CCA_AngDeadBand_co==2u)
	{
		Dead_Band_AngCtrl = 3u;				/* OMA : 259 */
	}
	else if(RX_CCA_AngDeadBand_co==3u)
	{
		Dead_Band_AngCtrl = 2u;				/* OMA : 300 */
	}
	else
	{
		Dead_Band_AngCtrl = 3u;
	}
}

/**
 *
 * transfer users' signals to bsw's variables about SENT mode.
 *
 */
PRIVATE void vio_read_TMM_SENT(void)
{
	TMM_Operating[sent_channel_1].input_angle_ume = RX_INPUT_ANGL_OMA;			/* upper endstop */
	TMM_Operating[sent_channel_2].input_angle_ume = RX_INPUT_ANGL_OMA;

	TMM_Operating[sent_channel_1].input_angle_lme = RX_INPUT_ANGL_UMA;			/* lower endstop */
	TMM_Operating[sent_channel_2].input_angle_lme = RX_INPUT_ANGL_UMA;

	TMM_Operating[sent_channel_1].input_angle_uar	= RX_INPUT_ANGL_OVB;			/* upper angle range */
	TMM_Operating[sent_channel_1].input_angle_lar	= RX_INPUT_ANGL_UVB;			/* lower angle range */

	TMM_Operating[sent_channel_2].input_angle_uar	= RX_INPUT_ANGL_OVB;			/* upper angle range */
	TMM_Operating[sent_channel_2].input_angle_lar	= RX_INPUT_ANGL_UVB;			/* lower angle range */

	TMM_SerMSg[sent_channel_2] = RX_INPUT_SERMSG_B;
	TMM_EN_Pause_Pulse[sent_channel_2] = RX_INPUT_PP_B;
	TMM_Sent_CRC_Version[sent_channel_2] = RX_INPUT_CRC_B;

	TMM_SerMSg[sent_channel_1] = RX_INPUT_SERMSG_A;
	TMM_EN_Pause_Pulse[sent_channel_1] = RX_INPUT_PP_A;
	TMM_Sent_CRC_Version[sent_channel_1] = RX_INPUT_CRC_A;

	/* not use */
	//RX_INPUT_TYPE_B
	//RX_INPUT_OPT6_B
	//RX_INPUT_TYPE_A
	//RX_INPUT_OPT6_A

	if(RX_INPUT_A1A3_A == 1u)
	{
		TMM_A1A3_Mode[sent_channel_1] = Sent_Temperature;
	}
	else
	{
		TMM_A1A3_Mode[sent_channel_1] = Sent_RollingCnter;
	}

	if(RX_INPUT_A1A3_B == 1u)
	{
		TMM_A1A3_Mode[sent_channel_2] = Sent_Temperature;
	}
	else
	{
		TMM_A1A3_Mode[sent_channel_2] = Sent_RollingCnter;
	}
	
}

/**
 *
 * transfer users' signals to bsw's variables about TMM angle control.
 *
 */
PRIVATE void vio_read_TMM_Angle(void)
{
	TMM_Operating[sent_channel_1].target_angle = ANGL_ECUT_A_ANA_CAN;
	TMM_Operating[sent_channel_2].target_angle = ANGL_ECUT_B_ANA_CAN;

	/* update TMM_A */
	if(PPWM_DCMR_A_EXT_INPUT == 0u)				// close loop
	{
		Flag_OpenLoop_Ena[sent_channel_1] = FALSE;
		TMM_Operating[sent_channel_1].tmm_operating_start = TRUE;

		if(Flag_OpenLoop_Ena_prev[sent_channel_1]==TRUE)
		{
			TMM_Scaling_State[sent_channel_1] = scaling_shutdown;
			TMM_Operating[sent_channel_1].tmm_operating_mode = No_Operating;
		}
		
		if(Start_A||Single_A)
		{
			TMM_Operating[sent_channel_1].stop_duration = 100;				//1s
			//Flag_TMM_Angle_Control[sent_channel_1] = FALSE;

			switch(CYCLE_SEL_A)
			{
				case 0u:												// refrence run
					

					//TMM_Endstop_Cur[sent_channel_1].angle_upper_endstop = TMM_UME_Default;
					//TMM_Endstop_Cur[sent_channel_1].angle_lower_endstop = TMM_LME_Default;
					TMM_Endstop_Cur[sent_channel_1].angle_upper_endstop = RX_INPUT_ANGL_OMA;
					TMM_Endstop_Cur[sent_channel_1].angle_lower_endstop = RX_INPUT_ANGL_UMA;
					TMM_Endstop_Cur[sent_channel_1].sent_upper_endstop  = TMM_UME_SENT_Default;
					TMM_Endstop_Cur[sent_channel_1].sent_lower_endstop  = TMM_LME_SENT_Default;
					tmm_reset_factors(sent_channel_1);

					tmm_scaling_shutdown(sent_channel_1);

					TMM_Operating[sent_channel_1].tmm_operating_mode = Refrence_Run;
					break;

				case 1u:	// function B
					TMM_Operating[sent_channel_1].tmm_operating_mode = To_Any_Target_Angle;
					TMM_Fun_Sel[sent_channel_1] = fun_A;
					break;

				case 2u:	// function B
					TMM_Operating[sent_channel_1].tmm_operating_mode = To_Any_Target_Angle;
					//Flag_TMM_Func_B_En[sent_channel_1] = TRUE;
					TMM_Fun_Sel[sent_channel_1] = fun_B;
					break;

				case 3u:	// function C
					TMM_Operating[sent_channel_1].tmm_operating_mode = To_Any_Target_Angle;
					TMM_Fun_Sel[sent_channel_1] = fun_C;
					break;


				case 10u:												// shift speed to angle range
					TMM_Operating[sent_channel_1].tmm_operating_mode = Shift_Speed_AR;
					break;
				case 12u:												// delay shift speed
					TMM_Operating[sent_channel_1].tmm_operating_mode = Shift_Speed_AR;
					TMM_Operating[sent_channel_1].stop_duration = 400;		//4s
					break;
//				case 13u:												// shift speed to end stop
//					TMM_Operating[sent_channel_1].tmm_operating_mode = To_Any_Target_Angle;
//					break;
				case 14u:												// shift speed to end stop
					TMM_Operating[sent_channel_1].tmm_operating_mode = Shift_Speed_ME;
					break;
				case 15u:												// PWM to endstop, max duty
					TMM_Operating[sent_channel_1].tmm_operating_mode = PWM_To_ME_closeloop;
					break;
				default:
					TMM_Operating[sent_channel_1].tmm_operating_mode = No_Operating;
					break;
			}

			if(Single_A)
			{
				Flag_Single_Cycle[sent_channel_1] = TRUE;					//shuold clear in shutdown function
				Flag_TMM_Seq_Single[sent_channel_1] = TRUE; 
			}
			
			TMM_Scaling_State[sent_channel_1] = scaling_s_detect;
		}

		if(Stop_A)
		{
			TMM_Scaling_State[sent_channel_1] = scaling_s_detect;
			TMM_Operating[sent_channel_1].tmm_operating_mode = No_Operating;

			//Flag_TMM_Func_B_En[sent_channel_1] = FALSE;	
			TMM_Fun_Sel[sent_channel_1] = no_fun;

		}

		if(ResetCutOff_A)
		{
			if((TMM_Operating[sent_channel_1].target_angle!=TMM_Angle_Cur[sent_channel_1])&& \
			(TMM_Operating[sent_channel_1].tmm_operating_mode == No_Operating))
			{
				//TMM_Operating[sent_channel_1].tmm_operating_start = TRUE;
				TMM_Operating[sent_channel_1].tmm_operating_mode = To_Any_Target_Angle;
				TMM_Scaling_State[sent_channel_1] = scaling_s_detect;
			}
		}

		//if((TMM_Operating[sent_channel_1].target_angle!=TMM_Angle_Cur[sent_channel_1])&& \
		//	((TMM_Operating[sent_channel_1].tmm_operating_mode == No_Operating) || \
		//	(TMM_Operating[sent_channel_1].tmm_operating_mode == PWM_To_ME_openloop)))
		//{
		//	TMM_Operating[sent_channel_1].tmm_operating_start = TRUE;
		//	TMM_Operating[sent_channel_1].tmm_operating_mode = To_Any_Target_Angle;
		//	//TMM_Operating[sent_channel_1].target_sent =(uint32_t)(((int32_t)TMM_Operating[sent_channel_1].target_angle*(int32_t)TMM_Angle_gain + (int32_t)TMM_Angle_offset + TMM_Calc_gain/2)/TMM_Calc_gain);
		//}
	}
	else 														// open loop
	{
		Flag_OpenLoop_Ena[sent_channel_1] = TRUE;
		//motor_stop(sent_channel_1);
		tmm_scaling_shutdown(sent_channel_1);
		TMM_Operating[sent_channel_1].tmm_operating_start = TRUE;
		TMM_Scaling_State[sent_channel_1] = scaling_s_detect;
		TMM_Operating[sent_channel_1].tmm_operating_mode = PWM_To_ME_openloop;
		TMM_Operating[sent_channel_1].pwm_ducmr_openloop = PPWM_DCMR_A_EXT_INPUT;
	}

	/* update TMM_B */
	if(PPWM_DCMR_B_EXT_INPUT == 0u)				// close loop
	{
		Flag_OpenLoop_Ena[sent_channel_2] = FALSE;
		TMM_Operating[sent_channel_2].tmm_operating_start = TRUE;

		if(Flag_OpenLoop_Ena_prev[sent_channel_2]==TRUE)
		{
			TMM_Scaling_State[sent_channel_2] = scaling_shutdown;
			TMM_Operating[sent_channel_2].tmm_operating_mode = No_Operating;
		}
		
		

		if(Start_B||Single_B)
		{
			TMM_Operating[sent_channel_2].stop_duration = 100;				//1s
			//Flag_TMM_Angle_Control[sent_channel_2] = FALSE;
			
			switch(CYCLE_SEL_B)
			{
				case 0u:												// refrence run
					

					//TMM_Endstop_Cur[sent_channel_2].angle_upper_endstop = TMM_UME_Default;
					//TMM_Endstop_Cur[sent_channel_2].angle_lower_endstop = TMM_LME_Default;
					TMM_Endstop_Cur[sent_channel_2].angle_upper_endstop = RX_INPUT_ANGL_OMA;
					TMM_Endstop_Cur[sent_channel_2].angle_lower_endstop = RX_INPUT_ANGL_UMA;
					TMM_Endstop_Cur[sent_channel_2].sent_upper_endstop  = TMM_UME_SENT_Default;
					TMM_Endstop_Cur[sent_channel_2].sent_lower_endstop  = TMM_LME_SENT_Default;
					tmm_reset_factors(sent_channel_2);

					tmm_scaling_shutdown(sent_channel_2);

					TMM_Operating[sent_channel_2].tmm_operating_mode = Refrence_Run;
					break;

				case 1u:	// function A
					TMM_Operating[sent_channel_2].tmm_operating_mode = To_Any_Target_Angle;
					TMM_Fun_Sel[sent_channel_2] = fun_A;
					break;

				case 2u:	// function B
					TMM_Operating[sent_channel_2].tmm_operating_mode = To_Any_Target_Angle;
					//Flag_TMM_Func_B_En[sent_channel_2] = TRUE;
					TMM_Fun_Sel[sent_channel_2] = fun_B;
					break;

				case 3u:	// function C
					TMM_Operating[sent_channel_2].tmm_operating_mode = To_Any_Target_Angle;
					TMM_Fun_Sel[sent_channel_2] = fun_C;
					break;


				case 10u:												// shift speed to angle range
					TMM_Operating[sent_channel_2].tmm_operating_mode = Shift_Speed_AR;
					break;
				case 12u:												// delay shift speed
					TMM_Operating[sent_channel_2].tmm_operating_mode = Shift_Speed_AR;
					TMM_Operating[sent_channel_2].stop_duration = 400;				//4s
					break;
//				case 13u:												// shift speed to end stop
//					TMM_Operating[sent_channel_2].tmm_operating_mode = To_Any_Target_Angle;
//					break;
				case 14u:												// shift speed to end stop
					TMM_Operating[sent_channel_2].tmm_operating_mode = Shift_Speed_ME;
					break;
				case 15u:												// PWM to endstop, max duty
					TMM_Operating[sent_channel_2].tmm_operating_mode = PWM_To_ME_closeloop;
					break;
				default:
					TMM_Operating[sent_channel_2].tmm_operating_mode = No_Operating;
					break;
			}

			if(Single_B)
			{
				Flag_Single_Cycle[sent_channel_2] = TRUE;					//shuold clear in shutdown function
				Flag_TMM_Seq_Single[sent_channel_2] = TRUE; 
			}

			TMM_Scaling_State[sent_channel_2] = scaling_s_detect;
			
		}

		if(Stop_B)
		{

			TMM_Scaling_State[sent_channel_2] = scaling_s_detect;
			TMM_Operating[sent_channel_2].tmm_operating_mode = No_Operating;	

			//Flag_TMM_Func_B_En[sent_channel_2] = FALSE;	
			TMM_Fun_Sel[sent_channel_2] = no_fun;

		}

		if(ResetCutOff_B)
		{
			if((TMM_Operating[sent_channel_2].target_angle!=TMM_Angle_Cur[sent_channel_2])&& \
			(TMM_Operating[sent_channel_2].tmm_operating_mode == No_Operating))
			{
				TMM_Operating[sent_channel_2].tmm_operating_mode = To_Any_Target_Angle;
				TMM_Scaling_State[sent_channel_2] = scaling_s_detect;
			}
		}

		//if((TMM_Operating[sent_channel_2].target_angle!=TMM_Angle_Cur[sent_channel_2])&& \
		//	((TMM_Operating[sent_channel_2].tmm_operating_mode == No_Operating) || \
		//	(TMM_Operating[sent_channel_2].tmm_operating_mode == PWM_To_ME_openloop)))
		//{
		//	TMM_Operating[sent_channel_2].tmm_operating_start = TRUE;
		//	TMM_Operating[sent_channel_2].tmm_operating_mode = To_Any_Target_Angle;
		//	//TMM_Operating[sent_channel_2].target_sent =(uint32_t)(((int32_t)TMM_Operating[sent_channel_2].target_angle*(int32_t)TMM_Angle_gain + (int32_t)TMM_Angle_offset + TMM_Calc_gain/2)/TMM_Calc_gain);
		//}
	}
	else 														// open loop
	{
		Flag_OpenLoop_Ena[sent_channel_2] = TRUE;
		//motor_stop(sent_channel_2);
		tmm_scaling_shutdown(sent_channel_2);
		TMM_Operating[sent_channel_2].tmm_operating_start = TRUE;
		TMM_Scaling_State[sent_channel_2] = scaling_s_detect;
		TMM_Operating[sent_channel_2].tmm_operating_mode = PWM_To_ME_openloop;
		TMM_Operating[sent_channel_2].pwm_ducmr_openloop = PPWM_DCMR_B_EXT_INPUT;
	}

	Flag_OpenLoop_Ena_prev[sent_channel_1] = Flag_OpenLoop_Ena[sent_channel_1];
	Flag_OpenLoop_Ena_prev[sent_channel_2] = Flag_OpenLoop_Ena[sent_channel_2];
}

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/
/**
 *
 * Limit settings.
 *
 */
void ECU_LimitInput(void)
{
	int8_t speed_value;

	RX_CCA_AngleDevMax_co = Integer2Value_mul(uint16_t, TMM_RX_CAN.ecu_limitinput[0], RX_CCA_AngleDevMax_co_FACTOR, RX_CCA_AngleDevMax_co_OFFSET);
	RX_CCA_ShiftSpeedDevMax_co = Integer2Value_mul(uint16_t, TMM_RX_CAN.ecu_limitinput[1], RX_CCA_ShiftSpeedDevMax_co_FACTOR, RX_CCA_ShiftSpeedDevMax_co_OFFSET);
	RX_CCA_TimeTolDevMax_co = Integer2Value_div(uint8_t, TMM_RX_CAN.ecu_limitinput[2], RX_CCA_TimeTolDevMax_co_FACTOR, RX_CCA_TimeTolDevMax_co_OFFSET);
	RX_TOP_PwmMax_co = TMM_RX_CAN.ecu_limitinput[3];
	RX_TOP_PwmMin_co = TMM_RX_CAN.ecu_limitinput[4];
	RX_TIC_CurEmMaxDiag_co = TMM_RX_CAN.ecu_limitinput[5];

	speed_value = (int8_t)TMM_RX_CAN.ecu_limitinput[6];
	RX_EMC_ShiftSpeedCamDem_co_A = Integer2Value_mul(int16_t, speed_value, RX_EMC_ShiftSpeedCamDem_co_A_FACTOR, RX_EMC_ShiftSpeedCamDem_co_A_OFFSET);
	speed_value = (int8_t)TMM_RX_CAN.ecu_limitinput[7];
	RX_EMC_ShiftSpeedCamDem_co_B = Integer2Value_mul(int16_t, speed_value, RX_EMC_ShiftSpeedCamDem_co_B_FACTOR, RX_EMC_ShiftSpeedCamDem_co_B_OFFSET);

	vio_read_TMM_Limit_Config();
}

/**
 *
 * PID settings.
 *
 */
void ECU_RegelParaInput(void)
{
	uint16_t kp_value = 0u;
	
	RX_CCA_AngDeadBand_co = Integer2Value_div(uint8_t, TMM_RX_CAN.ecu_regelparainput[0], RX_CCA_AngDeadBand_co_FACTOR, RX_CCA_AngDeadBand_co_OFFSET);
	
	kp_value = TMM_RX_CAN.ecu_regelparainput[1] * 100u;
	RX_CCA_KpAngleCtrl_co = Integer2Value_div(uint16_t, kp_value, RX_CCA_KpAngleCtrl_co_FACTOR, RX_CCA_KpAngleCtrl_co_OFFSET);
	RX_CCA_KiAngleCtrl_co = Integer2Value_div(uint8_t, TMM_RX_CAN.ecu_regelparainput[2], RX_CCA_KiAngleCtrl_co_FACTOR, RX_CCA_KiAngleCtrl_co_OFFSET);
	RX_CCA_KdAngleCtrl_co = Integer2Value_div(uint8_t, TMM_RX_CAN.ecu_regelparainput[3], RX_CCA_KdAngleCtrl_co_FACTOR, RX_CCA_KdAngleCtrl_co_OFFSET);
	RX_CCA_ShiftSpeedDeadBand_co = Integer2Value_mul(uint16_t, TMM_RX_CAN.ecu_regelparainput[4], RX_CCA_ShiftSpeedDeadBand_co_FACTOR, RX_CCA_ShiftSpeedDeadBand_co_OFFSET);
	
	kp_value = TMM_RX_CAN.ecu_regelparainput[5] * 100u;
	RX_CCA_KpShiftSpeedCtrl_co = Integer2Value_div(uint16_t, kp_value, RX_CCA_KpShiftSpeedCtrl_co_FACTOR, RX_CCA_KpShiftSpeedCtrl_co_OFFSET);
	RX_CCA_KiShiftSpeedCtrl_co = Integer2Value_div(uint8_t, TMM_RX_CAN.ecu_regelparainput[6], RX_CCA_KiShiftSpeedCtrl_co_FACTOR, RX_CCA_KiShiftSpeedCtrl_co_OFFSET);
	RX_CCA_MaxShiftSpeedAngCtrl_co = Integer2Value_mul(uint16_t, TMM_RX_CAN.ecu_regelparainput[7], RX_CCA_MaxShiftSpeedAngCtrl_co_FACTOR, RX_CCA_MaxShiftSpeedAngCtrl_co_OFFSET);

	vio_read_TMM_Regel_Para();
}

/**
 *
 * Angle settings.
 *
 */
void ECU_AngleInput(void)
{
	int16_t angle_integer_value  = 0u;

	angle_integer_value = ((int16_t)TMM_RX_CAN.ecu_angleinput[0] << 8u) | (int16_t)TMM_RX_CAN.ecu_angleinput[1];
	ANGL_ECUT_A_ANA_CAN = Integer2Value_div(int16_t, angle_integer_value, ANGL_ECUT_X_ANA_CAN_FACTOR, ANGL_ECUT_X_ANA_CAN_OFFSET);

	angle_integer_value = ((int16_t)TMM_RX_CAN.ecu_angleinput[2] << 8u) | (int16_t)TMM_RX_CAN.ecu_angleinput[3];
	ANGL_ECUT_B_ANA_CAN = Integer2Value_div(int16_t, angle_integer_value, ANGL_ECUT_X_ANA_CAN_FACTOR, ANGL_ECUT_X_ANA_CAN_OFFSET);
	
	Stop_B = (TMM_RX_CAN.ecu_angleinput[4] & 0x01u) ? TRUE : FALSE;
	Stop_A = (TMM_RX_CAN.ecu_angleinput[4] & 0x02u) ? TRUE : FALSE;
	Start_B = (TMM_RX_CAN.ecu_angleinput[4] & 0x04u) ? TRUE : FALSE;
	Start_A = (TMM_RX_CAN.ecu_angleinput[4] & 0x08u) ? TRUE : FALSE;
	Single_B = (TMM_RX_CAN.ecu_angleinput[4] & 0x10u) ? TRUE : FALSE;
	Single_A = (TMM_RX_CAN.ecu_angleinput[4] & 0x20u) ? TRUE : FALSE;
	ResetCutOff_A = (TMM_RX_CAN.ecu_angleinput[4] & 0x40u) ? TRUE : FALSE;
	ResetCutOff_B = (TMM_RX_CAN.ecu_angleinput[4] & 0x80u) ? TRUE : FALSE;

	PPWM_DCMR_A_EXT_INPUT = (int8_t)TMM_RX_CAN.ecu_angleinput[5];
	PPWM_DCMR_B_EXT_INPUT  = (int8_t)TMM_RX_CAN.ecu_angleinput[6];
	CYCLE_SEL_A = TMM_RX_CAN.ecu_angleinput[7]&0x0F;
	CYCLE_SEL_B = (TMM_RX_CAN.ecu_angleinput[7]&0xF0) >> 4u;

	vio_read_TMM_Angle();
}

/**
 *
 * SENT mode settings.
 *
 */
void ECU_SENTModeInput(void)
{
	uint16_t integer_value = 0u;
	
	switch(TMM_RX_CAN.ecu_sentmodeinput[0]&0x07)
	{
		case 0:
			RX_INPUT_TYPE_A = sensor_type_SENT;
			break;
		case 1:
			RX_INPUT_TYPE_A = sensor_type_ANALOG;
			break;
		case 2:
			RX_INPUT_TYPE_A = sensor_type_PWM;
			break;
		case 3:
			RX_INPUT_TYPE_A = sensor_type_LIN;
			break;
		case 4:
			RX_INPUT_TYPE_A = sensor_type_CAN;
			break;
		case 5:
			RX_INPUT_TYPE_A = sensor_type_Flexray;
			break;
		default:
			RX_INPUT_TYPE_A = sensor_type_SENT;
			break;
	}

	RX_INPUT_CRC_A = ((TMM_RX_CAN.ecu_sentmodeinput[0] & 0x08)==0x08)? Sent_CRC_2010 : Sent_CRC_2008;
	
	if((TMM_RX_CAN.ecu_sentmodeinput[0]&0x30) >> 4u)
	{
		RX_INPUT_PP_A = Sent_En_Pause_Pulse;
	}
	else
	{
		RX_INPUT_PP_A = Sent_Dis_Pause_Pulse;
	}

	switch((TMM_RX_CAN.ecu_sentmodeinput[0]&0xC0) >> 6u)
	{
		case 0:
			RX_INPUT_SERMSG_A = Sent_No_SerMsg;
			break;
		case 1:
			RX_INPUT_SERMSG_A = Sent_ShortSerMsg;
			break;
		case 2:
			RX_INPUT_SERMSG_A = Sent_EnhSerMsg_12bit;
			break;
		case 3:
			RX_INPUT_SERMSG_A = Sent_EnhSerMsg_16bit;
			break;
		default:
			RX_INPUT_SERMSG_A = Sent_No_SerMsg;
			break;
	}

	integer_value = ((uint16_t)(TMM_RX_CAN.ecu_sentmodeinput[2]&0x01) << 8u) | (uint16_t)TMM_RX_CAN.ecu_sentmodeinput[1];
	RX_INPUT_ANGL_UMA = Integer2Value_mul(int16_t, integer_value, RX_INPUT_ANGL_UMA_FACTOR, RX_INPUT_ANGL_UMA_OFFSET);

	integer_value = ((uint16_t)(TMM_RX_CAN.ecu_sentmodeinput[3]&0x03) << 7u) | (uint16_t)(TMM_RX_CAN.ecu_sentmodeinput[2] >> 1u);
	RX_INPUT_ANGL_UVB = Integer2Value_mul(int16_t, integer_value, RX_INPUT_ANGL_UVB_FACTOR, RX_INPUT_ANGL_UVB_OFFSET);

	RX_INPUT_A1A3_A = (TMM_RX_CAN.ecu_sentmodeinput[3]&0x0C) >> 2u;
	RX_INPUT_OPT6_A = TMM_RX_CAN.ecu_sentmodeinput[3] >> 4u;


	/* B module */
	switch(TMM_RX_CAN.ecu_sentmodeinput[4]&0x07)
	{
		case 0:
			RX_INPUT_TYPE_B = sensor_type_SENT;
			break;
		case 1:
			RX_INPUT_TYPE_B = sensor_type_ANALOG;
			break;
		case 2:
			RX_INPUT_TYPE_B = sensor_type_PWM;
			break;
		case 3:
			RX_INPUT_TYPE_B = sensor_type_LIN;
			break;
		case 4:
			RX_INPUT_TYPE_B = sensor_type_CAN;
			break;
		case 5:
			RX_INPUT_TYPE_B = sensor_type_Flexray;
			break;
		default:
			RX_INPUT_TYPE_B = sensor_type_SENT;
			break;
	}

	RX_INPUT_CRC_B = ((TMM_RX_CAN.ecu_sentmodeinput[4] & 0x08)==0x08)? Sent_CRC_2010 : Sent_CRC_2008;
	
	if((TMM_RX_CAN.ecu_sentmodeinput[4]&0x30) >> 4u)
	{
		RX_INPUT_PP_B = Sent_En_Pause_Pulse;
	}
	else
	{
		RX_INPUT_PP_B = Sent_Dis_Pause_Pulse;
	}

	switch((TMM_RX_CAN.ecu_sentmodeinput[4]&0xC0) >> 6u)
	{
		case 0:
			RX_INPUT_SERMSG_B = Sent_No_SerMsg;
			break;
		case 1:
			RX_INPUT_SERMSG_B = Sent_ShortSerMsg;
			break;
		case 2:
			RX_INPUT_SERMSG_B = Sent_EnhSerMsg_12bit;
			break;
		case 3:
			RX_INPUT_SERMSG_B = Sent_EnhSerMsg_16bit;
			break;
		default:
			RX_INPUT_SERMSG_B = Sent_No_SerMsg;
			break;
	}

	integer_value = ((uint16_t)(TMM_RX_CAN.ecu_sentmodeinput[6]&0x01) << 8u) | (uint16_t)TMM_RX_CAN.ecu_sentmodeinput[5];
	RX_INPUT_ANGL_OVB = Integer2Value_mul(int16_t, integer_value, RX_INPUT_ANGL_OVB_FACTOR, RX_INPUT_ANGL_OVB_OFFSET);

	integer_value = ((uint16_t)(TMM_RX_CAN.ecu_sentmodeinput[7]&0x03) << 7u) | (uint16_t)(TMM_RX_CAN.ecu_sentmodeinput[6] >> 1u);
	RX_INPUT_ANGL_OMA = Integer2Value_mul(int16_t, integer_value, RX_INPUT_ANGL_OMA_FACTOR, RX_INPUT_ANGL_OMA_OFFSET);

	RX_INPUT_A1A3_B = (TMM_RX_CAN.ecu_sentmodeinput[7]&0x0C) >> 2u;
	RX_INPUT_OPT6_B = TMM_RX_CAN.ecu_sentmodeinput[7] >> 4u;

	vio_read_TMM_SENT();
}

/**
 *
 * CAN2ANA settings.
 *
 */
void CAN2ANA_RX2(void)
{
	uint32_t integer_value = 0u;

	//	integer_value = (uint16_t)TMM_RX_CAN.can2ana_rx2[1] << 8u | (uint16_t)TMM_RX_CAN.can2ana_rx2[0];
	//	CTA_SET_VOLT_ECUT_B = ((uint32_t)integer_value * CTA_SET_VOLT_ECUT_X_FACTOR + 50000000u)/100000000u + CTA_SET_VOLT_ECUT_X_OFFSET;
//	
	//	integer_value = (uint16_t)TMM_RX_CAN.can2ana_rx2[3] << 8u | (uint16_t)TMM_RX_CAN.can2ana_rx2[2];
	//	CTA_SET_VOLT_SENS_B = ((uint32_t)integer_value * CTA_SET_VOLT_SENS_X_FACTOR + 50000000u)/100000000u + CTA_SET_VOLT_SENS_X_OFFSET;

	integer_value = (uint16_t)TMM_RX_CAN.can2ana_rx2[1] << 8u | (uint16_t)TMM_RX_CAN.can2ana_rx2[0];
	CAN2ANA_Vol[4u] = integer_value * 1526 /10000;

	integer_value = (uint16_t)TMM_RX_CAN.can2ana_rx2[3] << 8u | (uint16_t)TMM_RX_CAN.can2ana_rx2[2];
	CAN2ANA_Vol[5u] = integer_value * 10000/65535;



	//CAN2ANA_Vol[4u] = (uint16_t)TMM_RX_CAN.can2ana_rx2[1] << 8u | (uint16_t)TMM_RX_CAN.can2ana_rx2[0];
	//CAN2ANA_Vol[5u] = (uint16_t)TMM_RX_CAN.can2ana_rx2[3] << 8u | (uint16_t)TMM_RX_CAN.can2ana_rx2[2];
	CAN2ANA_Vol[6u] = (uint16_t)TMM_RX_CAN.can2ana_rx2[5] << 8u | (uint16_t)TMM_RX_CAN.can2ana_rx2[4];
	CAN2ANA_Vol[7u] = (uint16_t)TMM_RX_CAN.can2ana_rx2[7] << 8u | (uint16_t)TMM_RX_CAN.can2ana_rx2[6];

	Flag_CAN2ANA2_En = TRUE;
}

/**
 *
 * CAN2ANA settings.
 *
 */
void CAN2ANA_RX1(void)
{
	uint32_t integer_value = 0u;

	//	integer_value = (uint16_t)TMM_RX_CAN.can2ana_rx1[1] << 8u | (uint16_t)TMM_RX_CAN.can2ana_rx1[0];
	//	CTA_SET_VOLT_ECUT_A = ((uint32_t)integer_value * CTA_SET_VOLT_ECUT_X_FACTOR + 50000000u)/100000000u + CTA_SET_VOLT_ECUT_X_OFFSET;
//	
	//	integer_value = (uint16_t)TMM_RX_CAN.can2ana_rx1[3] << 8u | (uint16_t)TMM_RX_CAN.can2ana_rx1[2];
	//	CTA_SET_VOLT_SENS_A = ((uint32_t)integer_value * CTA_SET_VOLT_SENS_X_FACTOR + 50000000u)/100000000u + CTA_SET_VOLT_SENS_X_OFFSET;

	integer_value = (uint16_t)TMM_RX_CAN.can2ana_rx1[1] << 8u | (uint16_t)TMM_RX_CAN.can2ana_rx1[0];
	CAN2ANA_Vol[0u] = integer_value * 1526 /10000;

	integer_value = (uint16_t)TMM_RX_CAN.can2ana_rx1[3] << 8u | (uint16_t)TMM_RX_CAN.can2ana_rx1[2];
	CAN2ANA_Vol[1u] = integer_value * 10000/65535;

	//CAN2ANA_Vol[0u] = (uint16_t)TMM_RX_CAN.can2ana_rx1[1] << 8u | (uint16_t)TMM_RX_CAN.can2ana_rx1[0];
	//CAN2ANA_Vol[1u] = (uint16_t)TMM_RX_CAN.can2ana_rx1[3] << 8u | (uint16_t)TMM_RX_CAN.can2ana_rx1[2];
	CAN2ANA_Vol[2u] = (uint16_t)TMM_RX_CAN.can2ana_rx1[5] << 8u | (uint16_t)TMM_RX_CAN.can2ana_rx1[4];
	CAN2ANA_Vol[3u] = (uint16_t)TMM_RX_CAN.can2ana_rx1[7] << 8u | (uint16_t)TMM_RX_CAN.can2ana_rx1[6];

	Flag_CAN2ANA1_En = TRUE;
}

/**
 *
 * LIN command to RSCV B
 *
 */
void LIN_RSCV_Input_B(void)
{
	uint16_t integer_value = 0u;

	RX_RSCV_CALIBR_CMMD_B = (TMM_RX_CAN.lin_rscv_input_b[1] & 0x04u) ? TRUE : FALSE;

	integer_value = ((uint16_t)(TMM_RX_CAN.lin_rscv_input_b[1] & 0x03u) << 8u) | (uint16_t)TMM_RX_CAN.lin_rscv_input_b[0];
	RX_RSCV_POS_B = (uint32_t)integer_value * 100u /1022u;

	/* put into LIN buffer */
	//LIN_Msg_Send[lin_channel_2].data[0] = TMM_RX_CAN.lin_rscv_input_b[0];
	//LIN_Msg_Send[lin_channel_2].data[1] = TMM_RX_CAN.lin_rscv_input_b[1];

}


/**
 *
 * LIN command to RSCV A
 *
 */
void LIN_RSCV_Input_A(void)
{
	uint16_t integer_value = 0u;

	RX_RSCV_CALIBR_CMMD_A = (TMM_RX_CAN.lin_rscv_input_a[1] & 0x04u) ? TRUE : FALSE;

	integer_value = ((uint16_t)(TMM_RX_CAN.lin_rscv_input_a[1] & 0x03u) << 8u) | (uint16_t)TMM_RX_CAN.lin_rscv_input_a[0];
	RX_RSCV_POS_A = (uint32_t)integer_value * 100u /1022u;

	/* put into LIN buffer */
	LIN_Msg_Send[lin_channel_1].data[0] = TMM_RX_CAN.lin_rscv_input_a[0];
	LIN_Msg_Send[lin_channel_1].data[1] = TMM_RX_CAN.lin_rscv_input_a[1];

}


/* LIN sequence mode configuration 1 */
void LIN_SEQ_Confg1(void)
{
	uint16_t step_id = 0u;

	step_id = ((uint16_t)TMM_RX_CAN.rx_lin_seq_confg1[0u] << 8u) | (uint16_t)TMM_RX_CAN.rx_lin_seq_confg1[1u];
	step_id = step_id - 1u;

	LIN_Seq_Data[step_id].LIN_STEP_ID = step_id;
	LIN_Seq_Data[step_id].FLAG_LIN_STEP_END = (TMM_RX_CAN.rx_lin_seq_confg1[2u]&0x01) ? TRUE : FALSE;

	if(TMM_RX_CAN.rx_lin_seq_confg1[3u] == 0u)
	{
		LIN_Seq_Data[step_id].LIN_SEQ_MODE = time_based;
	}
	else if(TMM_RX_CAN.rx_lin_seq_confg1[3u] == 1u)
	{
		LIN_Seq_Data[step_id].LIN_SEQ_MODE = temp_based;
	}
	else if(TMM_RX_CAN.rx_lin_seq_confg1[3u] == 2u)
	{
		LIN_Seq_Data[step_id].LIN_SEQ_MODE = voltage_based;
	}
	else
	{
		LIN_Seq_Data[step_id].LIN_SEQ_MODE = time_based;
	}

	LIN_Seq_Data[step_id].LIN_POWER_SUPPLY = TMM_RX_CAN.rx_lin_seq_confg1[4u];			// 0.1 V
	LIN_Seq_Data[step_id].LIN_STEP_DURATION = TMM_RX_CAN.rx_lin_seq_confg1[5u];			//0.1 s
	LIN_Seq_Data[step_id].BRV_POSI_SET = TMM_RX_CAN.rx_lin_seq_confg1[6u];				// 1%


	if(LIN_Seq_Data[step_id].FLAG_LIN_STEP_END)
	{
		mdm_populate_image();
		nvs_request_all_and_reset(Reset_controlled);
		nvs_background(1u);
		RESET_S12(); 
	}
	      
}

/* LIN sequence mode configuration 2 */
void LIN_SEQ_Confg2(void)
{
	uint16_t step_id = 0u;

	step_id = ((uint16_t)TMM_RX_CAN.rx_lin_seq_confg2[0u] << 8u) | (uint16_t)TMM_RX_CAN.rx_lin_seq_confg2[1u];
	step_id = step_id - 1u;

	//LIN_Seq_Data[step_id].LIN_STEP_ID = step_id;

	LIN_Seq_Data[step_id].LIN_TEMP_MIN = (int16_t)(((uint16_t)TMM_RX_CAN.rx_lin_seq_confg2[2u] << 8u) | (uint16_t)TMM_RX_CAN.rx_lin_seq_confg2[3u]);		//0.1
	LIN_Seq_Data[step_id].LIN_TEMP_MAX = (int16_t)(((uint16_t)TMM_RX_CAN.rx_lin_seq_confg2[4u] << 8u) | (uint16_t)TMM_RX_CAN.rx_lin_seq_confg2[5u]);		//0.1
	LIN_Seq_Data[step_id].LIN_ANA_MIN = (int8_t)TMM_RX_CAN.rx_lin_seq_confg2[6u];			//0.1V, +/- 10v
	LIN_Seq_Data[step_id].LIN_ANA_MAX = (int8_t)TMM_RX_CAN.rx_lin_seq_confg2[7u];
}

/* LIN sequence mode select */
void LIN_SEQ_SEL(void)
{
	if(TMM_RX_CAN.rx_lin_seq_sel[0u] == 1u)
	{
		Flag_LIN_Nml_En = TRUE;
		Flag_LIN_Seq_En = FALSE;
	}
	else if(TMM_RX_CAN.rx_lin_seq_sel[0u] == 2u)
	{
		Flag_LIN_Seq_En = TRUE;
		Flag_LIN_Nml_En = FALSE;
	}
	else
	{
		Flag_LIN_Nml_En = FALSE;
		Flag_LIN_Seq_En = FALSE;
	}
}

/* TMM sequence mode configuration */
void TMM_SEQ_Confg(void)
{
	bool_t flag_seq_end = FALSE;
	uint16_t step_id;

		switch(TMM_RX_CAN.rx_tmm_seq_confg[5u])
		{
			case 0u:					// function A
				for(step_id = 0u; step_id < LIN_STEPS_MAX; step_id++)
   				{
					//TMM_Seq_Data_A[step_id].TMM_SEQ_TIME =  TMM_Seq_Data[step_id].TMM_SEQ_TIME;
        			//TMM_Seq_Data_A[step_id].TMM_SEQ_ANGLE = TMM_Seq_Data[step_id].TMM_SEQ_ANGLE; 
        			//TMM_Seq_Data_A[step_id].TMM_SEQ_SPEED = TMM_Seq_Data[step_id].TMM_SEQ_SPEED; 
        			//TMM_Seq_Data_A[step_id].FLAG_SEQ_END = TMM_Seq_Data[step_id].FLAG_SEQ_END; 

        			TMM_Seq_Data_A[TMM_SEQ_STEP_ID_rx].TMM_SEQ_TIME = ((uint16_t)TMM_RX_CAN.rx_tmm_seq_confg[0u] << 8u) | (uint16_t)TMM_RX_CAN.rx_tmm_seq_confg[1u];
					TMM_Seq_Data_A[TMM_SEQ_STEP_ID_rx].TMM_SEQ_ANGLE = (int16_t)(((uint16_t)TMM_RX_CAN.rx_tmm_seq_confg[2u] << 8u) | (uint16_t)TMM_RX_CAN.rx_tmm_seq_confg[3u]);
					flag_seq_end = (TMM_RX_CAN.rx_tmm_seq_confg[4u]&0x01) ? TRUE : FALSE;
					TMM_Seq_Data_A[TMM_SEQ_STEP_ID_rx].FLAG_SEQ_END = flag_seq_end;
				}
				break;
			case 1u:					// function A
				for(step_id = 0u; step_id < LIN_STEPS_MAX; step_id++)
   				{
					//TMM_Seq_Data_B[step_id].TMM_SEQ_TIME =  TMM_Seq_Data[step_id].TMM_SEQ_TIME;
        			//TMM_Seq_Data_B[step_id].TMM_SEQ_ANGLE = TMM_Seq_Data[step_id].TMM_SEQ_ANGLE; 
        			//TMM_Seq_Data_B[step_id].TMM_SEQ_SPEED = TMM_Seq_Data[step_id].TMM_SEQ_SPEED; 
        			//TMM_Seq_Data_B[step_id].FLAG_SEQ_END = TMM_Seq_Data[step_id].FLAG_SEQ_END; 

        			TMM_Seq_Data_B[TMM_SEQ_STEP_ID_rx].TMM_SEQ_TIME = ((uint16_t)TMM_RX_CAN.rx_tmm_seq_confg[0u] << 8u) | (uint16_t)TMM_RX_CAN.rx_tmm_seq_confg[1u];
					TMM_Seq_Data_B[TMM_SEQ_STEP_ID_rx].TMM_SEQ_ANGLE = (int16_t)(((uint16_t)TMM_RX_CAN.rx_tmm_seq_confg[2u] << 8u) | (uint16_t)TMM_RX_CAN.rx_tmm_seq_confg[3u]);
					flag_seq_end = (TMM_RX_CAN.rx_tmm_seq_confg[4u]&0x01) ? TRUE : FALSE;
					TMM_Seq_Data_B[TMM_SEQ_STEP_ID_rx].FLAG_SEQ_END = flag_seq_end;
				}
				break;
			case 2u:					// function A
				for(step_id = 0u; step_id < LIN_STEPS_MAX; step_id++)
   				{
					//TMM_Seq_Data_C[step_id].TMM_SEQ_TIME =  TMM_Seq_Data[step_id].TMM_SEQ_TIME;
        			//TMM_Seq_Data_C[step_id].TMM_SEQ_ANGLE = TMM_Seq_Data[step_id].TMM_SEQ_ANGLE; 
        			//TMM_Seq_Data_C[step_id].TMM_SEQ_SPEED = TMM_Seq_Data[step_id].TMM_SEQ_SPEED; 
        			//TMM_Seq_Data_C[step_id].FLAG_SEQ_END = TMM_Seq_Data[step_id].FLAG_SEQ_END; 

        			TMM_Seq_Data_C[TMM_SEQ_STEP_ID_rx].TMM_SEQ_TIME = ((uint16_t)TMM_RX_CAN.rx_tmm_seq_confg[0u] << 8u) | (uint16_t)TMM_RX_CAN.rx_tmm_seq_confg[1u];
					TMM_Seq_Data_C[TMM_SEQ_STEP_ID_rx].TMM_SEQ_ANGLE = (int16_t)(((uint16_t)TMM_RX_CAN.rx_tmm_seq_confg[2u] << 8u) | (uint16_t)TMM_RX_CAN.rx_tmm_seq_confg[3u]);
					flag_seq_end = (TMM_RX_CAN.rx_tmm_seq_confg[4u]&0x01) ? TRUE : FALSE;
					TMM_Seq_Data_C[TMM_SEQ_STEP_ID_rx].FLAG_SEQ_END = flag_seq_end;
				}
				break;
			default:
				break;


		}


	TMM_SEQ_STEP_ID_rx++;

	if(flag_seq_end)
	{
		TMM_SEQ_STEP_ID_rx = 0u;

		mdm_populate_image();
		nvs_request_all_and_reset(Reset_controlled);
		nvs_background(1u);

		RESET_S12(); 
	}
}


//	
///* can channel 3 */
//
//void vio_read_New_CANID(void)
//{	
//	uint16_t old_id, new_id;
//	can_message_e msg;
//	
//	if(TMM_New_CANID_valid)
//	{
//		old_id = ((uint16_t)TMM_New_CANID[1] << 8u) | (uint16_t)TMM_New_CANID[2];
//		new_id = ((uint16_t)TMM_New_CANID[3] << 8u) | (uint16_t)TMM_New_CANID[4];
//		Flag_Restore_Fcty_Defs = (TMM_New_CANID[5]&0x01) ? TRUE : FALSE;
//		
//		if(Flag_Restore_Fcty_Defs)
//		{
//			//memcpy(TMM_CAN_MSG,TMM_CAN_MSG_Default, TMM_CAN_MAX);                      /* too long */
//			
//			for(msg=Rx_TMM_Sent; msg < TMM_CAN_MAX; msg++)
//			{
//				TMM_CAN_MSG[msg] = TMM_CAN_MSG_Default[msg];
//			}
//		}
//		else
//		{
//			for(msg=Rx_TMM_Sent; msg < TMM_CAN_MAX; msg++)
//			{
//				if(TMM_CAN_MSG[msg] == old_id)
//				{
//					TMM_CAN_MSG[msg] = new_id;
//				}
//			}
//		}
//		
//		switch(TMM_New_CANID[6])
//		{
//			case 0 :
//				TMM_CAN_BTR0 = 0x81u;               /* 500k */
//				break;
//			case 1 :
//				TMM_CAN_BTR0 = 0x83u;               /* 250k */
//				break;
//			case 2 :
//				TMM_CAN_BTR0 = 0x87u;               /* 125k */
//				break;
//			default:
//				TMM_CAN_BTR0 = 0x81u;               /* 500k */
//				break;			
//		}
//			
//		mdm_populate_image();
//		nvs_request_all_and_reset(Reset_controlled);
//		nvs_background(1u);
//		RESET_S12();                    /* don't need to make TMM_New_CANID_valid to old data available, because we have reset. */
//	}
//	
//}
//

