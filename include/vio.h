/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:       vio.h
 *
 * description:  
 *  Public header file for the VIO feature
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Feb-14  Jun Wu           initial version
 *
 *****************************************************************************/
#if !defined(VIO_H)
#define      VIO_H
/**
 *
 * Public header file for the VIO feature
 *
 * @file
 * @ingroup vio
 */


/**
 *
 * <h2> VIO Overview </h2>
 *
 * <h3> Message Reception </h3>
 * This feature decodes the CAN message data received from the vehicle into
 * the correct MMC variables. The MMC variables are scaled in engineering
 * units, hence  the VIO feature will apply the necessary scaling & offsets
 * to the input data before updating the MMC variables. This is the module
 * where invalid or inconsistent requests from the vehicle are detected. There
 * shall be a decode function for each individual input message. Each of these
 * may or may not call sub-function to process individual fields that require
 * more complex processing.
 *
 *
 * @defgroup vio VIO Vehicle Input/Output
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "mmc.h"
#include "vio_types.h"
#include "basic_types.h"
#include "smm.h"
#include "mdm_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

#define VIO_CAN_BUS Can_vehicle


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
extern bool_t ResetCutOff_B;
extern bool_t ResetCutOff_A;
extern bool_t Single_B;
extern bool_t Single_A;
extern bool_t Start_B;
extern bool_t Start_A;
extern bool_t Stop_B;
extern bool_t Stop_A;

/* Flag just run one single cycle */
extern bool_t Flag_Single_Cycle[sent_channel_max];

/* Limit Config */
/* angle deviation which leads to a PWM shut off after a certain time */
extern uint16_t AngleDevMax;

/* shift speed deviation which leads to a PWM shut off after a certain time */
extern uint16_t ShiftSpeedDevMax;

/* tolerance time for high deviations */
extern uint16_t TimeToDev_Max;

/* Maximal allowed PWM */
extern uint16_t PWM_DCMR_MAX;

/* Minimal allowed PWM */
extern uint16_t PWM_DCMR_MIN;

/* Threshold to detect a too high e-motor current, which could lead to a thermal damage */
extern uint8_t CURR_DCMR_MAX;

/* maximum shift speed limit during angle control */
extern int16_t Shift_Speed_Max;

/* PID control */
//extern uint8_t KP_ShiftSpeed;
//extern uint8_t KI_ShiftSpeed;
//extern uint8_t KD_ShiftSpeed;
extern uint8_t Dead_Band_ShiftSpeed;       /* permitted speed inaccuracy */

extern uint8_t KP_AngCtrl;
extern uint8_t KI_AngCtrl;
extern uint8_t KD_AngCtrl;
extern uint8_t Dead_Band_AngCtrl;      	 /* permitted angle inaccuracy */

/* SENT mode */
extern sent_CRC_version_e TMM_Sent_CRC_Version[sent_channel_max];
extern sent_pause_pulse_e TMM_EN_Pause_Pulse[sent_channel_max];
extern sent_serial_message_e TMM_SerMSg[sent_channel_max];
extern sent_A1A3_mode_e TMM_A1A3_Mode[sent_channel_max];
//bool_t TMM_Dis_Rolling_Cnter;


/* TMM1.0*/
extern uint8_t PWM_Freq_Index;
extern uint16_t Still_Time_OperMode;         /* 10*100ms */
extern bool_t  Flag_Reset_Cyclic_Cnter;
extern int16_t TMM_Safety_Angle;


/* TMM 2.0 */
extern uint8_t *p_CAN_MSG_BUFF[TMM_CAN_MAX];
extern uint16_t TMM_CAN_ID_Default[TMM_CAN_MAX];


/* vio_write */

/* ECU_SENT_B */
extern uint8_t SENT_SENS_B_Nibble2;
extern uint8_t SENT_SENS_B_Nibble1;
extern uint8_t SENT_SENS_B_Nibble4;
extern uint8_t SENT_SENS_B_Nibble3;
extern uint8_t SENT_SENS_B_Nibble6;
extern uint8_t SENT_SENS_B_Nibble5;
extern uint8_t SENT_SENS_B_Nibble8;
extern uint8_t SENT_SENS_B_Nibble7;
extern bool_t SENT_ECUT_B_Error_SensorDiag;
extern bool_t RTOS_ErrDistRefPos_B;
extern bool_t RTOS_FlagValidSENT_B;
extern bool_t RTOS_ErrGradBegr_B;
extern uint8_t SENT_ECUT_B_CRC;
extern bool_t SENT_ECUT_B_Error_NibbleLength;
extern bool_t SENT_ECUT_B_Error_SyncLength;
extern bool_t SENT_ECUT_B_Error_MissingNibble;
extern bool_t SENT_ECUT_B_Error_TooManyNibbles;
extern bool_t SENT_ECUT_B_Error_CRC;
extern bool_t SENT_ECUT_B_Error_TimeOut;
extern bool_t SENT_ECUT_B_Error_RollingCounter;
extern bool_t SENT_ECUT_B_Error_InverseNibble;
extern int16_t ANGL_SENS_B_ANA_RAW;





extern uint8_t SENT_ErrCount_B;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/* documentation in source file */
void vio_init(void);

/* documentation in source file */
void vio_read_inputs(void);

/* documentation in source file */
void vio_write_outputs(void);


/* vio read */
void ECU_LimitInput(void); 
void ECU_RegelParaInput(void); 
void ECU_AngleInput(void); 
void ECU_SENTModeInput(void); 
void CAN2ANA_RX2(void); 
void CAN2ANA_RX1(void);
void LIN_RSCV_Input_B(void);
void LIN_RSCV_Input_A(void); 
void LIN_SEQ_Confg1(void);
void LIN_SEQ_Confg2(void);
void LIN_SEQ_SEL(void);
void TMM_SEQ_Confg(void);

/* vio write */
void ECU_EndStopCounter(void); 
void ECU_SW_Version(void); 
void ECU_Modi(void); 
void ECU_AngleOutput(void); 
void ECU_SENT_ErrorCount(void); 
void ECU_LimitOutput(void); 
void ECU_RegelParaOutput(void); 
void ECU_RefPosOutput(void); 
void ECU_SENTModeOutput(void); 
void Module2_low(void); 
void Module2_high(void); 
void Module1_low(void); 
void Module1_high(void); 
void LIN_RSCV_Data_B(void);
void LIN_RSCV_Output_B(void);
void LIN_RSCV_Data_A(void);
void LIN_RSCV_Output_A(void);
void vio_write_ANA_A1A3(void); 


#endif      /* VIO_H */
