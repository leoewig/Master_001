#if !defined(VIO_TYPES_H)
#define      VIO_TYPES_H
/**
 *
 * Types/Macros header file for the VIO feature
 *
 * @file
 * @ingroup vio
 * $Id: vio_types.h 937 2015-04-24 15:43:18Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2013-2015 - All rights reserved */

#include "ts_types.h"


/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
typedef enum
{    
TMM_Normal,    
TMM_UME,
TMM_LME
} tmm_position_e;


typedef enum
{  
Rx_ECU_LimitInput,
Rx_ECU_RegelParaInput,
Rx_ECU_AngleInput,
Rx_ECU_SENTModeInput,
Rx_CAN2ANA_RX2,
Rx_CAN2ANA_RX1,
Rx_LIN_RSCV_Input_B,
Rx_LIN_RSCV_Input_A,
Rx_LIN_SEQ_Confg1,
Rx_LIN_SEQ_Confg2,
Rx_LIN_SEQ_SEL,
Rx_TMM_SEQ_Confg,
Tx_ECU_EndStopCounter,
Tx_ECU_SW_Version,
Tx_ECU_Modi,
Tx_ECU_AngleOutput,
Tx_ECU_SENT_ErrorCount,
Tx_ECU_LimitOutput,
Tx_ECU_RegelParaOutput,
Tx_ECU_RefPosOutput,
Tx_ECU_SENTModeOutput,
Tx_Module2_low,
Tx_Module2_high,
Tx_Module1_low,
Tx_Module1_high,
Tx_LIN_RSCV_Data_B,
Tx_LIN_RSCV_Output_B,
Tx_LIN_RSCV_Data_A,
Tx_LIN_RSCV_Output_A,
//Tx_ANA_A1A3,
Tx_ECU_SENT_B,
Tx_ECU_SENT_A,
TMM_CAN_MAX
} can_message_e;

#endif      /* VIO_TYPES_H */
