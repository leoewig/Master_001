/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    xgate_icm.h
 *
 * description:  internal comms protocol
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           internal comms protocol
 *                       
 *
 *****************************************************************************/
#if !defined(XGATE_ICM_H)
#define      XGATE_ICM_H
/**
 *
 * Header file for the implementation of XGATe portion of the MMC's side of the
 * internal comms protocol
 *
 * @file
 * @ingroup xgate_icm
 */


/**
 *
 * <h2> XGATE internal comms overview Overview </h2>
 *
 * <h2> Approach used </h2>
 *
 * <h2> Something </h2>
 * Details of the design should go here, with headings as required.
 *
 * <h2> Limitations </h2>
 * Describe any limitations of the feature, if there are any.
 *
 * <h2> How to use </h2>
 * A description of exactly what the application and other features must do to 
 * use this feature.
 *
 * <h2> Configuration </h2>
 * If the feature has configuration items then point to where they are.
 *
 * <h2> Requirement Satisfaction </h2>
 * Requirements are located in:
 *
 * Requirements for the ICM feature are in:
 * http://viewvc/viewvc.cgi/trunk/docs/reqs/generic/RU_BMS_MeasDaq.doc?revision=988&root=Q52717_NightWing&view=co
 *
 * The protocol is defined in:
 * http://viewvc/viewvc.cgi/trunk/docs/des/sys/int_comms_spec.doc?revision=2759&root=Q52717_NightWing&view=co 
 *
 *
 * | Requirement         | Satisfied By | Unit Tested By | PATed by |
 * | --------------------|--------------|----------------|----------|
 * | REQ_BMS_XG_001      | | | |
 * | REQ_BMS_XG_002      | | | |
 * | REQ_BMS_XG_003      | | | |
 * | REQ_BMS_XG_004      | | | |
 * | REQ_BMS_XG_005      | | | |
 * | REQ_BMS_XG_006      | | | |
 * | REQ_BMS_XG_007      | | | |
 * | REQ_BMS_XG_008      | | | |
 * | REQ_BMS_XG_009      | | | |
 * | REQ_BMS_XG_010      | | | |
 * | REQ_BMS_XG_011      | | | |
 * | REQ_BMS_XG_012      | | | |
 * | REQ_BMS_XG_013      | | | |
 * | REQ_BMS_XG_014      | | | |
 * | REQ_BMS_XG_015      | | | |
 * | REQ_BMS_XG_016      | | | |
 * | REQ_BMS_XG_017      | | | |
 * | REQ_BMS_XG_018      | | | |
 * | REQ_BMS_XG_019      | | | |
 * | REQ_BMS_XG_020      | | | |
 * | REQ_BMS_XG_021      | | | |
 * | REQ_BMS_XG_022      | | | |
 * | REQ_BMS_XG_023      | | | |
 * | REQ_BMS_XG_024      | | | |

 *
 *
 * @defgroup xgate_icm XGATE internal comms
 * @ingroup icm
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "smm_types.h"
/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
typedef struct PID 
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
}PID;
/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
extern uint8_t SENT_SENS_A_Nibble2;
extern uint8_t SENT_SENS_A_Nibble1;
extern uint8_t SENT_SENS_A_Nibble4;
extern uint8_t SENT_SENS_A_Nibble3;
extern uint8_t SENT_SENS_A_Nibble6;
extern uint8_t SENT_SENS_A_Nibble5;
extern uint8_t SENT_SENS_A_Nibble8;
extern uint8_t SENT_SENS_A_Nibble7;
extern bool_t SENT_ECUT_A_Error_SensorDiag;
//extern bool_t RTOS_ErrDistRefPos_A;
//extern bool_t RTOS_FlagValidSENT_A;
//extern bool_t RTOS_ErrGradBegr_A;
extern uint8_t SENT_ECUT_A_CRC;
extern bool_t SENT_ECUT_A_Error_NibbleLength;
extern bool_t SENT_ECUT_A_Error_SyncLength;
extern bool_t SENT_ECUT_A_Error_MissingNibble;
extern bool_t SENT_ECUT_A_Error_TooManyNibbles;
extern bool_t SENT_ECUT_A_Error_CRC;
extern bool_t SENT_ECUT_A_Error_TimeOut;
extern bool_t SENT_ECUT_A_Error_RollingCounter;
extern bool_t SENT_ECUT_A_Error_InverseNibble;
extern int16_t ANGL_SENS_A_ANA_RAW;
extern int value_to_watch_A[9];

//extern PID Speed_PID_A;


/* These are shared with the S12 and so must be aligned */
#pragma align on

extern uint8_t Sent_Rawdata_A[8u];

#pragma align off
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
void xgECU_SENT_A(void);
void sensor_speed_measure_A(sent_channel_e sent_channel);
void sensor_angle_measure_A(sent_channel_e sent_channel);

#endif      /* XGATE_ICM_H */

