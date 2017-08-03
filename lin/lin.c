/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    lin.c
 *
 * description:  
 *  			lin interface.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Mar-23  Jun Wu           initial version
 *
 *****************************************************************************/
#include "lin.h"
#include "vio.h"
#include "tmm.h"


/*****************************************************************************
 * MACROS
 *****************************************************************************/

/*****************************************************************************
 * CALIBRATABLE DATA
 *****************************************************************************/

 
/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/
uint32_t send_cnt =0u;
//uint32_t LIN_Receive_Cnt =0u;


PRIVATE bool_t send_res_28[lin_channel_max] = {FALSE,FALSE};
PRIVATE bool_t send_res_29[lin_channel_max] = {FALSE,FALSE};

PRIVATE bool_t Flag_Send_28_29[lin_channel_max] = {FALSE, FALSE};			//FALSE send 28 , TRUE send 29
PRIVATE	bool_t Flag_Send_CH1_CH2 = FALSE;									//FALSE send LIN1, TRUE send LIN2

uint16_t LIN_Seq_Step_ID = 0u;
uint16_t lin_step_ctr = 0u;
/******************************************************************************
 * Public DATA
 *****************************************************************************/ 
//uint8_t LINProtected_ID;
//uint8_t LINCheckSum;
//uint8_t MSG_Send_Length = 0u;
//uint8_t MSG_Receive_Length = 0u;
//bool_t LIN_Master_Send = FALSE;                  //master send data or receive data 

uint8_t RSCV_POS[lin_channel_max][2] = {0xFF, 0xFF};

uint8_t LIN_MSG_Receive_28[lin_channel_max][8u] = {0u};
uint8_t LIN_MSG_Receive_29[lin_channel_max][8u] = {0u};

bool_t Flag_LIN_Send_En[lin_channel_max] = {FALSE, FALSE};

lin_trans_state_e LIN_Trans_State = trans_detect;

lin_seq_data_s LIN_Seq_Data[LIN_STEPS_MAX];

/* LIN Sequence mode enable */
bool_t Flag_LIN_Seq_En = FALSE;

/* LIN normal mode enable, use 0x666 to control */
bool_t Flag_LIN_Nml_En = FALSE;

int16_t LIN_Temp_Raw = 0;
int8_t LIN_Volt_Raw = 0; 
/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 *****************************************************************************/


/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
PRIVATE void lin_seq_oprating(uint16_t step_id)
{
	uint16_t brv_pos = 0u;
	uint16_t next_step_id = 0u;

	next_step_id = step_id + 1u;

	if(next_step_id >= LIN_STEPS_MAX)
	{
		next_step_id = 0u;
	}

	brv_pos = (uint32_t)LIN_Seq_Data[step_id].BRV_POSI_SET*10000/978;

	switch(LIN_Seq_Data[step_id].LIN_SEQ_MODE)
	{
		case time_based:
				RSCV_POS[lin_channel_1][0u] = (uint8_t)brv_pos;
				RSCV_POS[lin_channel_1][1u] = (uint8_t)(brv_pos >> 8) & 0x03;
				break;
		case temp_based:
				if((LIN_Temp_Raw >= LIN_Seq_Data[step_id].LIN_TEMP_MIN) && (LIN_Temp_Raw <= LIN_Seq_Data[step_id].LIN_TEMP_MAX))
				{
					RSCV_POS[lin_channel_1][0u] = (uint8_t)brv_pos;
					RSCV_POS[lin_channel_1][1u] = (uint8_t)(brv_pos >> 8) & 0x03;
					LIN_Seq_Step_ID = LIN_Seq_Data[next_step_id].LIN_STEP_ID;
					lin_step_ctr = 0u;
				}		
				break;
		case voltage_based:
				if((LIN_Volt_Raw >= LIN_Seq_Data[step_id].LIN_ANA_MIN) && (LIN_Volt_Raw <= LIN_Seq_Data[step_id].LIN_ANA_MAX))
				{
					RSCV_POS[lin_channel_1][0u] = (uint8_t)brv_pos;
					RSCV_POS[lin_channel_1][1u] = (uint8_t)(brv_pos >> 8) & 0x03;
					LIN_Seq_Step_ID = LIN_Seq_Data[next_step_id].LIN_STEP_ID;
					lin_step_ctr = 0u;
				}
				break;
	}
	
	lin_step_ctr++;
	if(lin_step_ctr >= (LIN_Seq_Data[step_id].LIN_STEP_DURATION * 10))
	{
		LIN_Seq_Step_ID = LIN_Seq_Data[next_step_id].LIN_STEP_ID;
		lin_step_ctr = 0u;

		if(LIN_Seq_Data[step_id].FLAG_LIN_STEP_END)
		{
			LIN_Seq_Step_ID = 0u;
		}
	}
	

}
/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/
/* LIN Send 28, 29 with no interrupts */
void LIN_write_outputs(lin_channel_e lin_channel)
{	
	if(lin_channel == lin_channel_1)
	{
		if(TMM_RX_CAN.lin_rscv_input_a_valid == New_data_available)
		{
			RSCV_POS[lin_channel_1][0u] = TMM_RX_CAN.lin_rscv_input_a[0];
			RSCV_POS[lin_channel_1][1u] = TMM_RX_CAN.lin_rscv_input_a[1];
	
			TMM_RX_CAN.lin_rscv_input_a_valid = No_data_available; 
		}
	}
	else if(lin_channel == lin_channel_2)
	{
		if(TMM_RX_CAN.lin_rscv_input_b_valid == New_data_available)
		{
			RSCV_POS[lin_channel_2][0u] = TMM_RX_CAN.lin_rscv_input_b[0];
			RSCV_POS[lin_channel_2][1u] = TMM_RX_CAN.lin_rscv_input_b[1];
	
			TMM_RX_CAN.lin_rscv_input_b_valid = No_data_available; 
		}
	}

	/* send 0x28 */
//	LIN_Msg_Send[lin_channel].Flag_Master_Send_Data = TRUE;                      
//	LIN_Msg_Send[lin_channel].identifier = 0x28;
//	LIN_Msg_Send[lin_channel].send_data_length = 2;
//	LIN_Msg_Send[lin_channel].receive_data_length = 2;
//	
//	LIN_Msg_Send[lin_channel].data[0] = RSCV_POS[lin_channel][0u];
//	LIN_Msg_Send[lin_channel].data[1] = RSCV_POS[lin_channel][1u];
//
//	Flag_LIN_Send_En[lin_channel] = TRUE;
//	//send_res_28[lin_channel] = LINSendMsg(lin_channel);     				//master send header and data to slave
//	
//	LIN_MSG_Receive_28[lin_channel][0u] = frame_send[lin_channel].data[0u];
//	LIN_MSG_Receive_28[lin_channel][1u] = frame_send[lin_channel].data[1u];


//	/* send 0x29 */
//	LIN_Msg_Send[lin_channel].Flag_Master_Send_Data = FALSE;                     
//	LIN_Msg_Send[lin_channel].identifier = 0x29;
//	LIN_Msg_Send[lin_channel].send_data_length = 0;
//	LIN_Msg_Send[lin_channel].receive_data_length = 3;
//	//send_res_29[lin_channel]  = LINSendMsg(lin_channel);      				//master send header and receive data from slave
//
//	LIN_MSG_Receive_29[lin_channel][0u] = frame_send[lin_channel].data[0u];
//	LIN_MSG_Receive_29[lin_channel][1u] = frame_send[lin_channel].data[1u];	
//	LIN_MSG_Receive_29[lin_channel][2u] = frame_send[lin_channel].data[2u];	
}


void lin_update(void)
{	
	if((Flag_LIN_Nml_En) || (Flag_LIN_Seq_En))
	{
		if(TMM_RX_CAN.lin_rscv_input_a_valid == New_data_available)
		{
			RSCV_POS[lin_channel_1][0u] = TMM_RX_CAN.lin_rscv_input_a[0];
			RSCV_POS[lin_channel_1][1u] = TMM_RX_CAN.lin_rscv_input_a[1];
	
			TMM_RX_CAN.lin_rscv_input_a_valid = No_data_available; 
		}
	
		if(TMM_RX_CAN.lin_rscv_input_b_valid == New_data_available)
		{
			RSCV_POS[lin_channel_2][0u] = TMM_RX_CAN.lin_rscv_input_b[0];
			RSCV_POS[lin_channel_2][1u] = TMM_RX_CAN.lin_rscv_input_b[1];
	
			TMM_RX_CAN.lin_rscv_input_b_valid = No_data_available; 
		}
	
	
		switch(LIN_Trans_State)
		{
			case trans_detect:
					if(1)
					{
						LIN_Trans_State = lin1_28;
					}
					break;
			case lin1_28:
					LIN_Msg_Send[lin_channel_1].Flag_Master_Send_Data = TRUE;                      
					LIN_Msg_Send[lin_channel_1].identifier = 0x28;
					LIN_Msg_Send[lin_channel_1].send_data_length = 2;
					LIN_Msg_Send[lin_channel_1].receive_data_length = 2;
					
					LIN_Msg_Send[lin_channel_1].data[0] = RSCV_POS[lin_channel_1][0u];
					LIN_Msg_Send[lin_channel_1].data[1] = RSCV_POS[lin_channel_1][1u];
					//(void)LINSendMsg(lin_channel_1);
					LIN_Transmit_Enable(lin_channel_1);
	
					//if(frame_send[lin_channel_1].state == Complete)
					//{
					//	LIN_Trans_State = lin1_29;
//	
					//	LIN_MSG_Receive_28[lin_channel_1][0u] = frame_receive[lin_channel_1].data[0u];
					//	LIN_MSG_Receive_28[lin_channel_1][1u] = frame_receive[lin_channel_1].data[1u];
					//}
					break;
	
			case lin1_29:
					LIN_Msg_Send[lin_channel_1].Flag_Master_Send_Data = FALSE;                     
					LIN_Msg_Send[lin_channel_1].identifier = 0x29;
					LIN_Msg_Send[lin_channel_1].send_data_length = 0;
					LIN_Msg_Send[lin_channel_1].receive_data_length = 3;
					//(void)LINSendMsg(lin_channel_1);
					LIN_Transmit_Enable(lin_channel_1);
	
					//if(frame_send[lin_channel_1].state == Complete)
					//{
					//	LIN_Trans_State = lin2_28;
//	
					//	LIN_MSG_Receive_29[lin_channel_1][0u] = frame_receive[lin_channel_1].data[0u];
					//	LIN_MSG_Receive_29[lin_channel_1][1u] = frame_receive[lin_channel_1].data[1u];	
					//	LIN_MSG_Receive_29[lin_channel_1][2u] = frame_receive[lin_channel_1].data[2u];	
					//}
					break;
	
			case lin2_28:
					LIN_Msg_Send[lin_channel_2].Flag_Master_Send_Data = TRUE;                      
					LIN_Msg_Send[lin_channel_2].identifier = 0x28;
					LIN_Msg_Send[lin_channel_2].send_data_length = 2;
					LIN_Msg_Send[lin_channel_2].receive_data_length = 2;
					
					LIN_Msg_Send[lin_channel_2].data[0] = RSCV_POS[lin_channel_2][0u];
					LIN_Msg_Send[lin_channel_2].data[1] = RSCV_POS[lin_channel_2][1u];
					//(void)LINSendMsg(lin_channel_2);
					LIN_Transmit_Enable(lin_channel_2);
	
					//if(frame_send[lin_channel_2].state == Complete)
					//{
					//	LIN_Trans_State = lin2_29;
//	
					//	LIN_MSG_Receive_28[lin_channel_2][0u] = frame_receive[lin_channel_2].data[0u];
					//	LIN_MSG_Receive_28[lin_channel_2][1u] = frame_receive[lin_channel_2].data[1u];
					//}
					break;
	
			case lin2_29:
					LIN_Msg_Send[lin_channel_2].Flag_Master_Send_Data = FALSE;                     
					LIN_Msg_Send[lin_channel_2].identifier = 0x29;
					LIN_Msg_Send[lin_channel_2].send_data_length = 0;
					LIN_Msg_Send[lin_channel_2].receive_data_length = 3;
					//(void)LINSendMsg(lin_channel_2);
					LIN_Transmit_Enable(lin_channel_2);
	
					//if(frame_send[lin_channel_2].state == Complete)
					//{
					//	LIN_Trans_State = trans_detect;
//	
					//	LIN_MSG_Receive_29[lin_channel_2][0u] = frame_receive[lin_channel_2].data[0u];
					//	LIN_MSG_Receive_29[lin_channel_2][1u] = frame_receive[lin_channel_2].data[1u];	
					//	LIN_MSG_Receive_29[lin_channel_2][2u] = frame_receive[lin_channel_2].data[2u];	
					//}
					break;
	
			default:
					LIN_Trans_State = trans_detect;
					break;
		}
	}

	
}


/* LIN sequence mode */
void lin_seq_update(void)
{	

	if(Flag_LIN_Seq_En)
	{
		switch(LIN_Seq_Step_ID)
		{
			case 0u:
						lin_seq_oprating(0);
						break;
			case 1u:
						lin_seq_oprating(1);
						break;
			case 2u:
						lin_seq_oprating(2);
						break;
			case 3u:
						lin_seq_oprating(3);
						break;
			case 4u:
						lin_seq_oprating(4);
						break;
			case 5u:
						lin_seq_oprating(5);
						break;
			case 6u:
						lin_seq_oprating(6);
						break;
			case 7u:
						lin_seq_oprating(7);
						break;
			case 8u:
						lin_seq_oprating(8);
						break;
			case 9u:
						lin_seq_oprating(9);
						break;
			default:
					break;
		}
	}
	else
	{
		LIN_Seq_Step_ID = 0u;
	}
}




