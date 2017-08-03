/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    sci.c
 *
 * description:  
 *  			sci configure.
 * 
 *
 * revision history:
 *
 * Date         Author           Description
 * 2016-Apr-15  Jun Wu           initial version
 *
 *****************************************************************************/
#include "sci.h"
#include <string.h>
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

/******************************************************************************
 * Public DATA
 *****************************************************************************/ 

/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 *****************************************************************************/


/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
 
/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/
/*
** ===================================================================
**     Event       :  SCI1_OnError (module SCI_events)
**
**     From bean   :  SCI1 [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be
**         read using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SCI1_OnError(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SCI1_OnRxChar (module SCI_events)
**
**     From bean   :  SCI1 [AsynchroSerial]
**     Description :
**         This event is called after a correct character is
**         received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the
**         <Receiver> property is enabled or the <SCI output mode>
**         property (if supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SCI1_OnRxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SCI1_OnTxChar (module SCI_events)
**
**     From bean   :  SCI1 [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SCI1_OnTxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SCI1_OnFullRxBuf (module SCI_events)
**
**     From bean   :  SCI1 [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SCI1_OnFullRxBuf(void)
{
  /* Write your code here ... */
  
    unsigned char i;
  
  	SCI_rec_buf_number = SCI_InpLen;
          
	for(i=0; i<SCI_rec_buf_number;i++ )
	{
		 SCI1_RecvChar(&SCI_rec_buf[i]);
	} 

	//switch(SCI_rec_buf[0]&0xF0)
	//{
	//	case TMM_RX_SENT_MASK:
	//		memcpy(TMM_RX_SCI.tmm_sent_rx_buff,SCI_rec_buf, SCI_rec_buf_number);
	//		TMM_RX_SCI.tmm_sent_rx_valid = New_data_available;
	//		SCI_response_buf[0] = TMM_RX_SENT_MASK;
	//		break;
	//	case TMM_RX_Angle_MASK:
	//		//memcpy(TMM_Angle_Control_SCI,SCI_rec_buf, SCI_rec_buf_number);
	//		memcpy(TMM_RX_SCI.tmm_angle_rx_buff,SCI_rec_buf, SCI_rec_buf_number);
	//		TMM_RX_SCI.tmm_angle_rx_valid = New_data_available;
	//		SCI_response_buf[0] = TMM_RX_Angle_MASK;
	//		break;
	//	case TMM_RX_Config_MASK:
	//		memcpy(TMM_RX_SCI.tmm_limit_cfg_rx_buff,SCI_rec_buf, SCI_rec_buf_number);
	//		TMM_RX_SCI.tmm_limit_cfg_rx_valid = New_data_available;
	//		SCI_response_buf[0] = TMM_RX_Config_MASK;
	//		break;
	//	case TMM_RX_PID_MASK:
	//		memcpy(TMM_RX_SCI.tmm_PID_ctrl_rx_buff,SCI_rec_buf, SCI_rec_buf_number);
	//		TMM_RX_SCI.tmm_PID_ctrl_rx_valid = New_data_available;
	//		SCI_response_buf[0] = TMM_RX_PID_MASK;
	//		break;			
	//	default:
	//		SCI_response_buf[0] = 0x00;
	//		break;
	//}
		  
    SCI1_ClearRxBuf(); 
	SCI_rx_success = TRUE;
}

/*
** ===================================================================
**     Event       :  SCI1_OnFreeTxBuf (module SCI_events)
**
**     From bean   :  SCI1 [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SCI1_OnFreeTxBuf(void)
{
  /* Write your code here ... */
  SCI1_ClearTxBuf();
}


/*
** ===================================================================
**     Event       :  SCICalcChecksum 
**
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
uint8_t SCICalcChecksum(uint8_t *data)                               
{
  uint16_t sum = 0;
  uint8_t i;

  for(i = 0; i < 7; i++)
  {
    sum += data[i]&0x0F;

  }
  sum ^= 0x00FF;         
  return (uint8_t )sum;
}



