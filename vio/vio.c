/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:       vio.c
 *
 * description:  
 * Routines to control the formatting of transmitted CAN messages & the
 * the decoding of received CAN messages.
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
#include "vio_types_p.h" /* private macros */
#include "vio_p.h"      /* private header for this module */
#include "exc.h"
#include "exc_p.h"		/* for CAN_CYCLIC_STOP */
#include "tmm.h"
#include "smm.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/
 #define Clear_CAN_Buffer(buffname) do{ \
                                        buffname[0] = 0u;                \
                                        buffname[1] = 0u;                \
                                        buffname[2] = 0u;                \
                                        buffname[3] = 0u;                \
                                        buffname[4] = 0u;                \
                                        buffname[5] = 0u;                \
                                        buffname[6] = 0u;                \
                                        buffname[7] = 0u;                \
								} while(0)

/******************************************************************************
 * CALIBRATIONS
 *****************************************************************************/


/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/
uint8_t *p_CAN_MSG_BUFF[TMM_CAN_MAX] =
{
	/* Rx Buff */
	MCU_4._c,
	MCU_3._c,
	ECM_BMS_00._c,
	VIN._c,
	CarConfig._c,
	CCU_1._c,
	DCT_1._c,
	ECM_6._c,
	ISA_Result_msg1._c,
	ISA_Result_msg2._c,
	DMsut_rdreq._c,
	ISA_Result_msg3._c,
    BMS_00._c,
	BMS_01._c,
	BMS_02._c,
	BMS_03._c,
	BMS_04._c,
	BMS_05._c,
	BMS_06._c,
	BMS_07._c,
	BMS_08._c,
	BMS_09._c,
	BMS_0A._c,
	BMS_0B._c,
	BMS_0C._c,
	BMS_0D._c,
	BMS_0E._c,
	BMS_0F._c,
	BMS_10._c,
	//BMS_11._c,
	BMS_Diag_01._c,
	ISA_Command._c	
};

uint16_t TMM_CAN_ID_Default[TMM_CAN_MAX] = 
{
	0x601,				/* Rx_ECU_LimitInput */ 		
	0x600,				/* Rx_ECU_RegelParaInput */ 	
	0x192,				/* Rx_ECU_AngleInput */			
	0x590,				/* Rx_ECU_SENTModeInput */ 		
	0x3E9,				/* Rx_CAN2ANA_RX2 */ 			
	0x3E8,				/* Rx_CAN2ANA_RX1 */	
	0x66A,				/* Rx_LIN_RSCV_Input_B */
	0x666,				/* Rx_LIN_RSCV_Input_A */
	0x401,				/* Rx_LIN_SEQ_Confg1 */
	0x402,				/* Rx_LIN_SEQ_Confg2 */
	0x403,				/* Rx_LIN_SEQ_SEL */
	0x404,				/* Rx_TMM_SEQ_Confg */
	0x720,				/* Tx_ECU_EndStopCounter */		
	0x707,				/* Tx_ECU_SW_Version */			
	0x704,				/* Tx_ECU_Modi */				
	0x702,				/* Tx_ECU_AngleOutput */		
	0x604,				/* Tx_ECU_SENT_ErrorCount */	
	0x603,				/* Tx_ECU_LimitOutput */		
	0x602,				/* Tx_ECU_RegelParaOutput */	
	0x195,				/* Tx_ECU_RefPosOutput */		
	0x591,				/* Tx_ECU_SENTModeOutput */		
	0x580,				/* Tx_Module2_low */			
	0x581,				/* Tx_Module2_high */			
	0x57E,				/* Tx_Module1_low */			
	0x57F,				/* Tx_Module1_high */	
	0x62B,				/* Tx_LIN_RSCV_Data_B */
	0x62A,				/* Tx_LIN_RSCV_Output_B */
	0x627,				/* Tx_LIN_RSCV_Data_A */
	0x626,				/* Tx_LIN_RSCV_Output_A */	
	//0x405,				/* Tx_ANA_A1A3 */	
	0x211,				/* Tx_ECU_SENT_B （高速CAN） */
	0x210 				/* Tx_ECU_SENT_A （高速CAN） */
};
/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

PRIVATE ts_time_t       vio_startup_timestamp;
PRIVATE bool_t          vio_is_ready = FALSE;


uint8_t TMM_New_CANID[8]={0u};
data_validity_e TMM_New_CANID_valid = No_data_available;


/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
PRIVATE data_validity_e check_msg_validity(uint8_t *can_buf_p, uint8_t *target_buf_p)
{
	uint8_t i;
	uint16_t sum = 0u;
	uint8_t dif = 0u;
	data_validity_e res;

	for(i=0u; i<8u; i++)
	{
		sum += can_buf_p[i];

		if(can_buf_p[i] != target_buf_p[i])
		{
			dif++;
		}
	}

	if(sum==0u)
	{
		res = No_data_available;
	}
	else if(dif==0u)
	{
		res = Old_data_available;
	}
	else
	{
		res = New_data_available;
	}

	return res;	
}

/**
 *
 * motor should move to target angle when no operating mode.
 *
 */
PRIVATE void move_to_target_angle(void)
{

	if((TMM_Operating[sent_channel_1].target_angle!=TMM_Angle_Cur[sent_channel_1])&& \
	(TMM_Operating[sent_channel_1].tmm_operating_mode == No_Operating)  && \
	(TMM_Operating[sent_channel_1].tmm_operating_start == TRUE))
	{
		//TMM_Operating[sent_channel_1].tmm_operating_start = TRUE;
		TMM_Operating[sent_channel_1].tmm_operating_mode = To_Any_Target_Angle;
		TMM_Scaling_State[sent_channel_1] = scaling_s_detect;
	}

	if((TMM_Operating[sent_channel_2].target_angle!=TMM_Angle_Cur[sent_channel_2])&& \
	(TMM_Operating[sent_channel_2].tmm_operating_mode == No_Operating) && \
	(TMM_Operating[sent_channel_2].tmm_operating_start == TRUE))
	{
		//TMM_Operating[sent_channel_1].tmm_operating_start = TRUE;
		TMM_Operating[sent_channel_2].tmm_operating_mode = To_Any_Target_Angle;
		TMM_Scaling_State[sent_channel_2] = scaling_s_detect;
	}
}
/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/** This function will initialise the VIO feature and is called at startup. It
 *  will ensure that all output data is written at least once.
 */
void vio_init(void)
{
	can_message_e i;
    //vio_startup_timestamp = ts_get_ms();

	/* clear receive buffer */
    for(i=Rx_ECU_LimitInput; i< Tx_ECU_EndStopCounter; i++)
    {
    	Clear_CAN_Buffer(p_CAN_MSG_BUFF[i]);
    }

    vio_is_ready = TRUE;
}
/** This function will invoke all of the sub-functions that deformat the data
 *  received by the MMC. Each CAN message will be handled by a separate
 *  sub-function.
 */
void vio_read_inputs(void)
{
	can_message_e i;
	
	move_to_target_angle();

	if(check_msg_validity(p_CAN_MSG_BUFF[Rx_ECU_LimitInput], TMM_RX_CAN.ecu_limitinput) == New_data_available)
	{
		memcpy(TMM_RX_CAN.ecu_limitinput,p_CAN_MSG_BUFF[Rx_ECU_LimitInput], 8u);
		ECU_LimitInput();
	}

	if(check_msg_validity(p_CAN_MSG_BUFF[Rx_ECU_RegelParaInput], TMM_RX_CAN.ecu_regelparainput) == New_data_available)
	{
		memcpy(TMM_RX_CAN.ecu_regelparainput,p_CAN_MSG_BUFF[Rx_ECU_RegelParaInput], 8u);
		ECU_RegelParaInput();
	}

	if(check_msg_validity(p_CAN_MSG_BUFF[Rx_ECU_AngleInput], TMM_RX_CAN.ecu_angleinput) == New_data_available)
	{
		memcpy(TMM_RX_CAN.ecu_angleinput,p_CAN_MSG_BUFF[Rx_ECU_AngleInput], 8u);
		ECU_AngleInput();
	}

	if(check_msg_validity(p_CAN_MSG_BUFF[Rx_ECU_SENTModeInput], TMM_RX_CAN.ecu_sentmodeinput) == New_data_available)
	{
		memcpy(TMM_RX_CAN.ecu_sentmodeinput,p_CAN_MSG_BUFF[Rx_ECU_SENTModeInput], 8u);
		ECU_SENTModeInput();
	}

	if(check_msg_validity(p_CAN_MSG_BUFF[Rx_CAN2ANA_RX2], TMM_RX_CAN.can2ana_rx2) == New_data_available)
	{	
		if(p_CAN_MSG_BUFF[Rx_CAN2ANA_RX2][7] == 0x40)			// filter default value
		{
			p_CAN_MSG_BUFF[Rx_CAN2ANA_RX2][7] = 0x00;
		}
		memcpy(TMM_RX_CAN.can2ana_rx2,p_CAN_MSG_BUFF[Rx_CAN2ANA_RX2], 8u);
		CAN2ANA_RX2();
	}

	if(check_msg_validity(p_CAN_MSG_BUFF[Rx_CAN2ANA_RX1], TMM_RX_CAN.can2ana_rx1) == New_data_available)
	{
		if(p_CAN_MSG_BUFF[Rx_CAN2ANA_RX1][7] == 0x40)			// filter default value
		{
			p_CAN_MSG_BUFF[Rx_CAN2ANA_RX1][7] = 0x00;
		}
		memcpy(TMM_RX_CAN.can2ana_rx1,p_CAN_MSG_BUFF[Rx_CAN2ANA_RX1], 8u);
		CAN2ANA_RX1();
	}

	if((p_CAN_MSG_BUFF[Rx_LIN_RSCV_Input_B][0u] != 0u)&&(p_CAN_MSG_BUFF[Rx_LIN_RSCV_Input_B][1u] != 0u))
	{
		memcpy(TMM_RX_CAN.lin_rscv_input_b,p_CAN_MSG_BUFF[Rx_LIN_RSCV_Input_B], 4u);
		LIN_RSCV_Input_B();
		TMM_RX_CAN.lin_rscv_input_b_valid = New_data_available;
	}

	if((p_CAN_MSG_BUFF[Rx_LIN_RSCV_Input_A][0u] != 0u)&&(p_CAN_MSG_BUFF[Rx_LIN_RSCV_Input_A][1u] != 0u))
	{
		memcpy(TMM_RX_CAN.lin_rscv_input_a,p_CAN_MSG_BUFF[Rx_LIN_RSCV_Input_A], 4u);
		LIN_RSCV_Input_A();
		TMM_RX_CAN.lin_rscv_input_a_valid = New_data_available;
	}


	//if(check_msg_validity(p_CAN_MSG_BUFF[Rx_LIN_SEQ_Confg1], TMM_RX_CAN.rx_lin_seq_confg1) == New_data_available)
	//if((p_CAN_MSG_BUFF[Rx_LIN_SEQ_Confg1][0u]!= 0u)||(p_CAN_MSG_BUFF[Rx_LIN_SEQ_Confg1][1u]!= 0u))
	if((ISA_Result_msg1._c[0u] != 0u)||(ISA_Result_msg1._c[1u]!= 0u))
	{
		memcpy(TMM_RX_CAN.rx_lin_seq_confg1,p_CAN_MSG_BUFF[Rx_LIN_SEQ_Confg1], 8u);
		LIN_SEQ_Confg1();
	}


	//if(check_msg_validity(p_CAN_MSG_BUFF[Rx_LIN_SEQ_Confg2], TMM_RX_CAN.rx_lin_seq_confg2) == New_data_available)
	//if((p_CAN_MSG_BUFF[Rx_LIN_SEQ_Confg2][0u]!= 0u)||(p_CAN_MSG_BUFF[Rx_LIN_SEQ_Confg2][1u]!= 0u))
	if((ISA_Result_msg2._c[0u] != 0u)||(ISA_Result_msg2._c[1u]!= 0u))
	{
		memcpy(TMM_RX_CAN.rx_lin_seq_confg2,p_CAN_MSG_BUFF[Rx_LIN_SEQ_Confg2], 8u);
		LIN_SEQ_Confg2();
	}
	
	
	//if(check_msg_validity(p_CAN_MSG_BUFF[Rx_LIN_SEQ_SEL], TMM_RX_CAN.rx_lin_seq_sel) == New_data_available)
	//if((p_CAN_MSG_BUFF[Rx_LIN_SEQ_SEL][0u]!= 0u)||(p_CAN_MSG_BUFF[Rx_LIN_SEQ_SEL][1u]!= 0u))
	if((DMsut_rdreq._c[0u] != 0u)||(DMsut_rdreq._c[1u]!= 0u))
	{
		memcpy(TMM_RX_CAN.rx_lin_seq_sel,p_CAN_MSG_BUFF[Rx_LIN_SEQ_SEL], 4u);
		LIN_SEQ_SEL();
	}

	/* TMM Sequence configuration */
	//if((p_CAN_MSG_BUFF[Rx_TMM_SEQ_Confg][0u]!= 0u)||(p_CAN_MSG_BUFF[Rx_TMM_SEQ_Confg][1u]!= 0u))
	if((ISA_Result_msg3._c[0u] != 0u)||(ISA_Result_msg3._c[1u]!= 0u))
	{
		memcpy(TMM_RX_CAN.rx_tmm_seq_confg,p_CAN_MSG_BUFF[Rx_TMM_SEQ_Confg], 8u);
		TMM_SEQ_Confg();
	}



	/* clear receive buffer */
    for(i=Rx_ECU_LimitInput; i< Tx_ECU_EndStopCounter; i++)
    {
    	Clear_CAN_Buffer(p_CAN_MSG_BUFF[i]);
    }
	
	
}

/** This function will invoke all of the sub-functions that format the data
 *  transmitted by the MMC. Each CAN message will be handled by a separate
 *  sub-function.
 */
void vio_write_outputs(void)
{
	ECU_EndStopCounter();	
	ECU_SW_Version();	
	ECU_Modi();	
	ECU_AngleOutput();	
	ECU_SENT_ErrorCount();	
	ECU_LimitOutput();	
	ECU_RegelParaOutput();	
	ECU_RefPosOutput();	
	ECU_SENTModeOutput();	
	LIN_RSCV_Data_B();
	LIN_RSCV_Output_B();
	LIN_RSCV_Data_A();
	LIN_RSCV_Output_A();
	vio_write_ANA_A1A3();

	/* ANA to CAN */
	CAN_CYCLIC_STOP((IlTransmitHandle) exc_cyclic_control[9].tx_handle);
	CAN_CYCLIC_STOP((IlTransmitHandle) exc_cyclic_control[10].tx_handle);
	//Module2_low();	
	//Module2_high();	

	/* Temperature to CAN */
	CAN_CYCLIC_STOP((IlTransmitHandle) exc_cyclic_control[11].tx_handle);
	CAN_CYCLIC_STOP((IlTransmitHandle) exc_cyclic_control[12].tx_handle);
	//Module1_low();	
	//Module1_high();	

	/* CAN Fast, should put in interrupts */

	CAN_CYCLIC_STOP((IlTransmitHandle) exc_cyclic_control[13].tx_handle);
	CAN_CYCLIC_STOP((IlTransmitHandle) exc_cyclic_control[14].tx_handle);
	//ECU_SENT_B();	
	//CAN_CYCLIC_START((IlTransmitHandle) exc_cyclic_control[13].tx_handle);
	//ECU_SENT_A();		
	//CAN_CYCLIC_START((IlTransmitHandle) exc_cyclic_control[14].tx_handle);
}

