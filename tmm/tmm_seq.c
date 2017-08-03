/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2017 - All rights reserved 
 *
 * file name:    tmm_seq.c
 *
 * description:  
 *  			tmm sequence management.
 * 
 *
 * revision history:
 *
 * Date         Author           Description
 * 2017-Apr-21  Jun Wu           initial version
 *
 *****************************************************************************/
#include "tmm_seq.h"
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
FRAM PRIVATE uint16_t TMM_Seq_Step_ID[sent_channel_max] = {0u};
FRAM PRIVATE uint32_t tmm_seq_step_ctr[sent_channel_max] = {0u};


/******************************************************************************
 * Public DATA
 *****************************************************************************/ 
//tmm_seq_data_s TMM_Seq_Data[TMM_SEQ_STEP_MAX];



tmm_seq_data_s TMM_Seq_Data_A[TMM_SEQ_STEP_MAX];
tmm_seq_data_s TMM_Seq_Data_B[TMM_SEQ_STEP_MAX];
tmm_seq_data_s TMM_Seq_Data_C[TMM_SEQ_STEP_MAX];

bool_t Flag_TMM_Func_B_En[sent_channel_max] = {FALSE, FALSE};
bool_t Flag_TMM_Seq_Single[sent_channel_max] = {FALSE, FALSE};

tmm_seq_fun_sel_e TMM_Fun_Sel[sent_channel_max] = {no_fun, no_fun};
/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 *****************************************************************************/

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
 PRIVATE void tmm_seq_oprating(uint16_t step_id, sent_channel_e sent_channel)
{
	uint16_t brv_pos = 0u;
	uint16_t last_step_id = 0u;
	uint16_t step_duration = 0u;
	uint16_t tmm_seq_time_pre = 0u;

	tmm_seq_data_s tmm_seq_data;

	switch(TMM_Fun_Sel[sent_channel])
	{
		case fun_A:
				tmm_seq_data.TMM_SEQ_TIME = TMM_Seq_Data_A[step_id].TMM_SEQ_TIME; 
        		tmm_seq_data.TMM_SEQ_ANGLE = TMM_Seq_Data_A[step_id].TMM_SEQ_ANGLE;
        		tmm_seq_data.TMM_SEQ_SPEED = TMM_Seq_Data_A[step_id].TMM_SEQ_SPEED;
        		tmm_seq_data.FLAG_SEQ_END = TMM_Seq_Data_A[step_id].FLAG_SEQ_END; 

        		if(step_id > 0u)
        		{
        			tmm_seq_time_pre = TMM_Seq_Data_A[step_id - 1u].TMM_SEQ_TIME; 
        		}
        		break;
        case fun_B:
				tmm_seq_data.TMM_SEQ_TIME  = TMM_Seq_Data_B[step_id].TMM_SEQ_TIME; 
        		tmm_seq_data.TMM_SEQ_ANGLE = TMM_Seq_Data_B[step_id].TMM_SEQ_ANGLE;
        		tmm_seq_data.TMM_SEQ_SPEED = TMM_Seq_Data_B[step_id].TMM_SEQ_SPEED;
        		tmm_seq_data.FLAG_SEQ_END  = TMM_Seq_Data_B[step_id].FLAG_SEQ_END; 

        		if(step_id > 0u)
        		{
        			tmm_seq_time_pre = TMM_Seq_Data_B[step_id - 1u].TMM_SEQ_TIME; 
        		}
        		break;
        case fun_C:
				tmm_seq_data.TMM_SEQ_TIME  = TMM_Seq_Data_C[step_id].TMM_SEQ_TIME; 
        		tmm_seq_data.TMM_SEQ_ANGLE = TMM_Seq_Data_C[step_id].TMM_SEQ_ANGLE;
        		tmm_seq_data.TMM_SEQ_SPEED = TMM_Seq_Data_C[step_id].TMM_SEQ_SPEED;
        		tmm_seq_data.FLAG_SEQ_END  = TMM_Seq_Data_C[step_id].FLAG_SEQ_END; 

        		if(step_id > 0u)
        		{
        			tmm_seq_time_pre = TMM_Seq_Data_C[step_id - 1u].TMM_SEQ_TIME; 
        		}
        		break;
        default:
        		break;
	}

	if(step_id > 0u)
	{
		last_step_id = step_id - 1u;

		step_duration = tmm_seq_data.TMM_SEQ_TIME - tmm_seq_time_pre;
	}
	else
	{
		step_duration = tmm_seq_data.TMM_SEQ_TIME; 
	}

	step_duration = step_duration*10u;		// unit : 10ms

	TMM_Operating[sent_channel].target_angle = tmm_seq_data.TMM_SEQ_ANGLE;

	//TMM_Seq_Data[step_id].TMM_SEQ_SPEED;

	tmm_seq_step_ctr[sent_channel]++;
	if(tmm_seq_step_ctr[sent_channel] >= step_duration)
	{
		TMM_Seq_Step_ID[sent_channel]++;
		tmm_seq_step_ctr[sent_channel] = 0u;

		if(tmm_seq_data.FLAG_SEQ_END)
		{
			TMM_Seq_Step_ID[sent_channel] = 0u;

			if(Flag_TMM_Seq_Single[sent_channel])
			{
				//Flag_TMM_Func_B_En[sent_channel] = FALSE;

				TMM_Fun_Sel[sent_channel] = no_fun;
			}
		}
	}
	

}
/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/
void tmm_seq_update(void)
{
	sent_channel_e sent_channel;

	for(sent_channel = sent_channel_1; sent_channel < sent_channel_max; sent_channel++)
	{
		//if(Flag_TMM_Func_B_En[sent_channel])
		if(TMM_Fun_Sel[sent_channel] != no_fun)
		{
			switch(TMM_Seq_Step_ID[sent_channel])
			{
				case 0u:
					tmm_seq_oprating(0u, sent_channel);
				break;
	
				case 1u:
					tmm_seq_oprating(1u, sent_channel);
				break;
	
				case 2u:
					tmm_seq_oprating(2u, sent_channel);
				break;
	
				case 3u:
					tmm_seq_oprating(3u, sent_channel);
				break;
	
				case 4u:
					tmm_seq_oprating(4u, sent_channel);
				break;
	
				case 5u:
					tmm_seq_oprating(5u, sent_channel);
				break;
	
				case 6u:
					tmm_seq_oprating(6u, sent_channel);
				break;
	
				case 7u:
					tmm_seq_oprating(7u, sent_channel);
				break;
	
				case 8u:
					tmm_seq_oprating(8u, sent_channel);
				break;
	
				case 9u:
					tmm_seq_oprating(9u, sent_channel);
				break;
	
				case 10u:
					tmm_seq_oprating(10u, sent_channel);
				break;
	
				case 11u:
					tmm_seq_oprating(11u, sent_channel);
				break;
	
				case 12u:
					tmm_seq_oprating(12u, sent_channel);
				break;
	
				case 13u:
					tmm_seq_oprating(13u, sent_channel);
				break;
	
				case 14u:
					tmm_seq_oprating(14u, sent_channel);
				break;
	
				case 15u:
					tmm_seq_oprating(15u, sent_channel);
				break;
	
				case 16u:
					tmm_seq_oprating(16u, sent_channel);
				break;
	
				case 17u:
					tmm_seq_oprating(17u, sent_channel);
				break;
	
				case 18u:
					tmm_seq_oprating(18u, sent_channel);
				break;
	
				case 19u:
					tmm_seq_oprating(19u, sent_channel);
				break;
	
				case 20u:
					tmm_seq_oprating(20u, sent_channel);
				break;
	
				case 21u:
					tmm_seq_oprating(21u, sent_channel);
				break;
	
				case 22u:
					tmm_seq_oprating(22u, sent_channel);
				break;
	
				case 23u:
					tmm_seq_oprating(23u, sent_channel);
				break;
	
				case 24u:
					tmm_seq_oprating(24u, sent_channel);
				break;
	
				case 25u:
					tmm_seq_oprating(25u, sent_channel);
				break;
	
				case 26u:
					tmm_seq_oprating(26u, sent_channel);
				break;
	
				case 27u:
					tmm_seq_oprating(27u, sent_channel);
				break;
	
				case 28u:
					tmm_seq_oprating(28u, sent_channel);
				break;
	
				case 29u:
					tmm_seq_oprating(29u, sent_channel);
				break;
				
				case 30u:
					tmm_seq_oprating(30u, sent_channel);
				break;
	
				case 31u:
					tmm_seq_oprating(31u, sent_channel);
				break;
	
				case 32u:
					tmm_seq_oprating(32u, sent_channel);
				break;
	
				case 33u:
					tmm_seq_oprating(33u, sent_channel);
				break;
	
				case 34u:
					tmm_seq_oprating(34u, sent_channel);
				break;
	
				case 35u:
					tmm_seq_oprating(35u, sent_channel);
				break;
	
				case 36u:
					tmm_seq_oprating(36u, sent_channel);
				break;
	
				case 37u:
					tmm_seq_oprating(37u, sent_channel);
				break;
	
				case 38u:
					tmm_seq_oprating(38u, sent_channel);
				break;
	
				case 39u:
					tmm_seq_oprating(39u, sent_channel);
				break;
	
				case 40u:
					tmm_seq_oprating(40u, sent_channel);
				break;
	
				case 41u:
					tmm_seq_oprating(41u, sent_channel);
				break;
	
				case 42u:
					tmm_seq_oprating(42u, sent_channel);
				break;
	
				case 43u:
					tmm_seq_oprating(43u, sent_channel);
				break;
	
				case 44u:
					tmm_seq_oprating(44u, sent_channel);
				break;
	
				case 45u:
					tmm_seq_oprating(45u, sent_channel);
				break;
	
				case 46u:
					tmm_seq_oprating(46u, sent_channel);
				break;
	
				case 47u:
					tmm_seq_oprating(47u, sent_channel);
				break;
	
				case 48u:
					tmm_seq_oprating(48u, sent_channel);
				break;
	
				case 49u:
					tmm_seq_oprating(49u, sent_channel);
				break;
	
				case 50u:
					tmm_seq_oprating(50u, sent_channel);
				break;
	
				case 51u:
					tmm_seq_oprating(51u, sent_channel);
				break;
	
				case 52u:
					tmm_seq_oprating(52u, sent_channel);
				break;
	
				case 53u:
					tmm_seq_oprating(53u, sent_channel);
				break;
	
				case 54u:
					tmm_seq_oprating(54u, sent_channel);
				break;
	
				case 55u:
					tmm_seq_oprating(55u, sent_channel);
				break;
	
				case 56u:
					tmm_seq_oprating(56u, sent_channel);
				break;
	
				case 57u:
					tmm_seq_oprating(57u, sent_channel);
				break;
	
				case 58u:
					tmm_seq_oprating(58u, sent_channel);
				break;
	
				case 59u:
					tmm_seq_oprating(59u, sent_channel);
				break;

				case 60u:
					tmm_seq_oprating(60u, sent_channel);
				break;
	
				case 61u:
					tmm_seq_oprating(61u, sent_channel);
				break;
	
				case 62u:
					tmm_seq_oprating(62u, sent_channel);
				break;
	
				case 63u:
					tmm_seq_oprating(63u, sent_channel);
				break;
	
				case 64u:
					tmm_seq_oprating(64u, sent_channel);
				break;
	
				case 65u:
					tmm_seq_oprating(65u, sent_channel);
				break;
	
				case 66u:
					tmm_seq_oprating(66u, sent_channel);
				break;
	
				case 67u:
					tmm_seq_oprating(67u, sent_channel);
				break;
	
				case 68u:
					tmm_seq_oprating(68u, sent_channel);
				break;
	
				case 69u:
					tmm_seq_oprating(69u, sent_channel);
				break;
	
			}
		}
		else
		{
			TMM_Seq_Step_ID[sent_channel] = 0u;
		}
	}
	
}
