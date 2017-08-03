/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    sci_io.c
 *
 * description:  
 *  			sci IO.
 * 
 *
 * revision history:
 *
 * Date         Author           Description
 * 2016-Apr-15  Jun Wu           initial version
 *
 *****************************************************************************/
#include "sci.h"
#include "smm.h"
#include "motor.h"
#include <string.h>
#include "exc.h"
#include "exc_p.h"



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
bool_t SCI_rx_success = FALSE;
uint32_t SCI_rec_buf_number = 0u;
uint8_t SCI_rec_buf[8] = {0u};
uint8_t SCI_Send_Buffer[8] = {0u};
uint8_t SCI_response_buf[8] = {0u};
uint32_t SCI_Task_Cnter = 0u;

uint8_t Rolling_Cnter_10ms = 0u;
uint8_t Rolling_Cnter_100ms = 0u;

/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 *****************************************************************************/
PRIVATE void sci_write_TMM_Display1(void);
PRIVATE void sci_write_TMM_Display2(void);
PRIVATE void sci_write_TMM_Display3(void);
PRIVATE void sci_write_TMM_Display4(void);
PRIVATE void sci_write_TMM_Display5(void);

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
 PRIVATE void sci_write_TMM_Display1(void)
{
	uint8_t i;
	uint8_t data[8] = {0x00};
		
	//memcpy(data,BMS_CH1._c, 8);
	
	memcpy(data,BMS_CH1._c, 8);
	data[0] |= Rolling_Cnter_100ms << 4;
	
	for(i=0;i<SCI_OUT_BUF_SIZE;i++)
		{
		SCI1_SendChar(data[i]);
		}
}

 PRIVATE void sci_write_TMM_Display2(void)
{
	uint8_t i;
	uint8_t data[8] = {0x00};	
		
	memcpy(data,BMS_CH2._c, 8);
	data[0] |= Rolling_Cnter_100ms << 4;
	
	for(i=0;i<SCI_OUT_BUF_SIZE;i++)
		{
		SCI1_SendChar(data[i]);
		}
}

PRIVATE void sci_write_TMM_Display3(void)
{
	uint8_t i;
	uint8_t data[8] = {0x00};
		
	memcpy(data,BMS_Diag_14._c, 8);
	data[0] |= Rolling_Cnter_100ms << 4;
	
	for(i=0;i<SCI_OUT_BUF_SIZE;i++)
		{
		SCI1_SendChar(data[i]);
		}
}

PRIVATE void sci_write_TMM_Display4(void)
{
	uint8_t i;
	uint8_t data[8] = {0x00};
	uint8_t flags = 0u;	
		
	memcpy(data,BMS_Diag_15._c, 8);
	data[0] |= Rolling_Cnter_100ms << 4;
	
	for(i=0;i<SCI_OUT_BUF_SIZE;i++)
		{
		SCI1_SendChar(data[i]);
		}
}

PRIVATE void sci_write_TMM_Display5(void)
{
	uint8_t i;
	uint8_t data[8] = {0x00};
		
	memcpy(data,BMS_Diag_16._c, 8);
	data[0] |= Rolling_Cnter_100ms << 4;
	
	for(i=0;i<SCI_OUT_BUF_SIZE;i++)
		{
		SCI1_SendChar(data[i]);
		}
}
/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/
void sci_rx_response(void)
{
	uint8_t i;
	if(SCI_rx_success)
	{
		for(i=0;i<SCI_rec_buf_number;i++)
		{
		SCI1_SendChar(SCI_response_buf[i]);
		}
		
		SCI_rx_success = FALSE;
	}
	
}


void sci_write_outputs(void)
{
	if((SCI_Task_Cnter%10) == 0u)
	{
		sci_write_TMM_Display1();
	}
	else if((SCI_Task_Cnter%10) == 2u) 
	{
		sci_write_TMM_Display2();
	}
	else if((SCI_Task_Cnter%10) == 4u) 
	{
		sci_write_TMM_Display3();
	}
	else if((SCI_Task_Cnter%10) == 6u)
	{
		sci_write_TMM_Display4();
	}
	else if((SCI_Task_Cnter%10) == 8u)
	{
		sci_rx_response();
	}
	else if((SCI_Task_Cnter%10) == 9u)
	{
		sci_write_TMM_Display5();
		Rolling_Cnter_100ms++;
	}
		
	if(Rolling_Cnter_100ms > 15)
	{
		Rolling_Cnter_100ms = 0u;
	}
	
	SCI_Task_Cnter++;
}



