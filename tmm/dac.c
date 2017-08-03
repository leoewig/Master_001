/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2017 - All rights reserved 
 *
 * file name:    dac.c
 *
 * description:  
 *  			da output.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2017-Mar_16 Jun Wu			init
 *
 *****************************************************************************/
#include "dac.h"
#include "spi.h"
#include <string.h>
#include "dio_cfg.h"


/*****************************************************************************
 * MACROS
 *****************************************************************************/

/*****************************************************************************
 * CALIBRATABLE DATA
 *****************************************************************************/

 
/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/
uint8_t DAC_SPI_Tx_Buf[3u]  = {0u};
uint8_t DAC_SPI_Rx_Buf[3u]  = {0u};

uint8_t DAC_Power_Ctrl_Rx_Buf[3u]  = {0u};
uint8_t DAC_Output_Range_Rx_Buf[3u]  = {0u};
uint8_t DAC_Data_Rx_Buf[3u]  = {0u};

uint8_t DAC_Ctr = 0u;

/******************************************************************************
 * Public DATA
 *****************************************************************************/ 
bool_t Flag_CAN2ANA1_En = FALSE;
bool_t Flag_CAN2ANA2_En = FALSE;

int16_t CAN2ANA_Vol[8u] = {0};		/* mv */


/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 *****************************************************************************/


/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

 /***************************************************************************
 * @brief Writes data into a register.
 *
 * @param regBits - Address of the register.
 * @param adrBits - Address of a channel or of an option.
 * @param registerValue - Data value to write.
 *
 * @return None.
*******************************************************************************/
PRIVATE void AD5724R_SetRegisterValue(dac_cs_e dac_cs,
								uint8_t regBits,
                             	uint8_t adrBits,
                             	uint16_t registerValue)
{
    uint8_t registerWord[3] = {0};
	
    registerWord[0] = (regBits << 3) + adrBits;
    registerWord[1] = (registerValue & 0xFF00) >> 8;
    registerWord[2] = (registerValue & 0x00FF) >> 0;

     if(dac_cs == AD5724R_1)
    {
    	spi_transfer(SPI_DAC1, FNULL, registerWord, 3u);
    }
    else if(dac_cs == AD5724R_2)
    {
    	spi_transfer(SPI_DAC2, FNULL, registerWord, 3u);
    }
}

/***************************************************************************//**
 * @brief Reads the value of a register.
 *
 * @param registerAddress - Address of the register.
 *
 * @return *dataPointer - Value of the register.
*******************************************************************************/
PRIVATE uint16_t AD5724R_GetRegisterValue(dac_cs_e dac_cs, 
									uint8_t regBits,
                                       uint8_t adrBits)
{
    uint8_t registerWord[3]   = {0u};
    uint8_t registerWord_rb[3]   = {0u};
    uint16_t data             = 0u;
	
    registerWord[0] = (1 << 7) + (regBits << 3) + adrBits;
    registerWord[1] = 0;
    registerWord[2] = 0;

    if(dac_cs == AD5724R_1)
    {
    	spi_transfer(SPI_DAC1, FNULL, registerWord, 3u);
    }
    else if(dac_cs == AD5724R_2)
    {
    	spi_transfer(SPI_DAC2, FNULL, registerWord, 3u);
    }
    

    registerWord[0] = 0x18; // NOP instruction used in readback operations.
    registerWord[1] = 0;
    registerWord[2] = 0;

    if(dac_cs == AD5724R_1)
    {
    	spi_transfer(SPI_DAC1, registerWord_rb, registerWord, 3u);
    }
    else if(dac_cs == AD5724R_2)
    {
    	spi_transfer(SPI_DAC2, registerWord_rb, registerWord, 3u);
    }

    data = (registerWord_rb[1] << 8) + registerWord_rb[2];

    return(data);
}

/***************************************************************************//**
 * @brief Enables/disables indicated DAC channels.
 *
 * @param dacChannel - DAC channel or channels to be enabled/disabled.
 *                     Example: AD5724R_DAC_A - Channel A.
 *                              AD5724R_DAC_B - Channel B.
 *                              AD5724R_DAC_C - Channel C.
 *                              AD5724R_DAC_D - Channel D.
 *                              AD5724R_DAC_ALL - Channel ALL.
 * @param onOff - Enable or disable channel.
 *                Example: 0x0 - Off.
 *                         0x1 - On.
 *
 * @return None.
*******************************************************************************/
PRIVATE void AD5724R_SetPower(dac_cs_e dac_cs, uint8_t dacChannel, uint8_t onOff)
{
    uint16_t oldPowerCtl = 0;
	uint16_t newPowerCtl = 0;
	
    if(dacChannel == AD5724R_DAC_ALL)
    {
		dacChannel = 0x0F;
    }
    else
    {
		dacChannel = (1 << dacChannel);   
    }
    oldPowerCtl = AD5724R_GetRegisterValue(dac_cs, AD5724R_REG_POWER_CONTROL,0x00);
	newPowerCtl = oldPowerCtl & ~dacChannel;
	newPowerCtl = newPowerCtl | (onOff * dacChannel);
    AD5724R_SetRegisterValue(dac_cs, AD5724R_REG_POWER_CONTROL,0x00,newPowerCtl);
}

/***************************************************************************//**
 * @brief Writes data into a DAC channel register.
 *
 * @param dacChannel - DAC channel or channels to be written.
 * @param dacValue - Value written in DAC register.
 *
 * @return None.
*******************************************************************************/
PRIVATE void AD5724R_LoadDac(dac_cs_e dac_cs, uint8_t dacChannel, uint16_t dacValue)
{
    AD5724R_SetRegisterValue(dac_cs, AD5724R_REG_DAC, dacChannel, dacValue);
}

/***************************************************************************//**
 * @brief Selects the output range of a DAC channel.
 *
 * @param dacChannel - DAC channel.
 * @param outputRange - Output range of a DAC channel.
 *
 * @return None.
*******************************************************************************/
PRIVATE void AD5724R_RangeSelect(dac_cs_e dac_cs, uint8_t dacChannel, uint8_t outputRange)
{
	uint16_t registerData = 0;
	
	registerData |= outputRange;
	AD5724R_SetRegisterValue(dac_cs, AD5724R_REG_RANGE_SELECT,dacChannel,registerData);
}

/***************************************************************************//**
 * @brief Selects an option of Control Register.
 *
 * @param option - Control function selected.
 *                 Example: AD5724R_SDO_DISABLE
 *							AD5724R_CLR_SELECT
 *							AD5724R_CLAMP_ENABLE
 *							AD5724R_TSD_ENABLE
 * @param bitValue - Indicates a set or a clear of the bit. 
 *                   Example: 0 - bit is cleared;
 *                            1 - bit is set.
 *
 * @return None.
*******************************************************************************/
PRIVATE void AD5724R_ControlFunction(dac_cs_e dac_cs, uint8_t option, uint8_t bitValue)
{
    uint8_t adrBits  = option & 0xF0;
	uint8_t dataBits = option & 0X0F;
	uint16_t oldControl = 0;
	uint16_t newControl = 0;
	
	if(adrBits == 0x01)
	{
		oldControl = AD5724R_GetRegisterValue(dac_cs, AD5724R_REG_CONTROL,0x01); 
		newControl = oldControl & ~(dataBits); 
		newControl = newControl | (bitValue * dataBits);
	}
	AD5724R_SetRegisterValue(dac_cs, AD5724R_REG_CONTROL,adrBits,newControl);
}

/***************************************************************************//**
 * @brief Read alert bits form Power Control Register.
 *
 * @param None.
 *
 * @return powerCtlStatus - Result of the Power Control Register read.
 *                          Contains alert bits: OCd, OCc, OCb, OCa and TSD.
 *                          Example: 0x01 - TSD is set
 *                                   0x05 - OCa and TSD are set
 *                                   0x3C - OCd, OCc, OCb, OCa are set.                             
 *
*******************************************************************************/
PRIVATE uint8_t AD5724R_ReadAlertBits(dac_cs_e dac_cs)
{
	uint16_t powerCtlStatus = 0; 
	
	powerCtlStatus = AD5724R_GetRegisterValue(dac_cs, AD5724R_REG_POWER_CONTROL, 0x00);
	powerCtlStatus = (powerCtlStatus >> 5);
	powerCtlStatus &= 0x003F;	// Only bits 5 to 0 are required.

	return ((uint8_t) powerCtlStatus);
}

/***************************************************************************//**
 * 
 *	@brief Calculate DAC value with known voltage.
 * 	dac_voltage is mv.
 *
*******************************************************************************/
PRIVATE uint16_t dac_value_calc(int16_t dac_voltage)
{
	uint16_t res = 0; 
	int16_t dac_res = 0;

	dac_res = ((int32_t)dac_voltage * 2048 + 5000)/10000;

	if (dac_voltage < 0)
	{
		dac_res = dac_res + 4096;
	}
	else if(dac_voltage >= 10000)
	{
		dac_res = 2047;
	}

	res = (uint16_t)dac_res << 4;

	return res;
}


void dac_update_test(void);

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/
void dac_init(void)
{
	PORTE &= ~0x04; //LDAC always be low

	//AD5724R_CLR_HIGH; PB5, PB6
	do_dac_en_1 = TRUE;
	do_dac_en_2 = TRUE;

}

void dac_update(void)
{
	uint16_t dac_value_set;
	uint8_t i;

	if(Flag_CAN2ANA1_En)
	{
		AD5724R_SetPower(AD5724R_1, AD5724R_DAC_ALL, 0x1);
		AD5724R_RangeSelect(AD5724R_1, AD5724R_DAC_ALL, AD5724R_BIPOLAR_10_RANGE);

		for(i=0u; i<4u; i++)
		{
			dac_value_set = dac_value_calc(CAN2ANA_Vol[i]);
			AD5724R_LoadDac(AD5724R_1, i, dac_value_set);
		}

		Flag_CAN2ANA1_En = FALSE;
	}


	if(Flag_CAN2ANA2_En)
	{
		AD5724R_SetPower(AD5724R_2, AD5724R_DAC_ALL, 0x1);
		AD5724R_RangeSelect(AD5724R_2, AD5724R_DAC_ALL, AD5724R_BIPOLAR_10_RANGE);

		for(i=0u; i<4u; i++)
		{
			dac_value_set = dac_value_calc(CAN2ANA_Vol[i+4]);
			AD5724R_LoadDac(AD5724R_2, i, dac_value_set);
		}

		Flag_CAN2ANA2_En = FALSE;
	}

}



void dac_update_test(void)
{
	uint8_t i;

	DAC_Ctr++;

	

	if(DAC_Ctr == 1u)
	{
		//PORTE &= ~0x04;

		for(i=0; i<1u; i++)
		{
			/* set output range register */
			DAC_SPI_Tx_Buf[0u] = 0x0C;
			DAC_SPI_Tx_Buf[1u] = 0x00;
			DAC_SPI_Tx_Buf[2u] = 0x04;
		
			spi_transfer(SPI_DAC1, FNULL, DAC_SPI_Tx_Buf, 3u);
	
			/* set power control register */
			DAC_SPI_Tx_Buf[0u] = 0x10;
			DAC_SPI_Tx_Buf[1u] = 0x00;
			DAC_SPI_Tx_Buf[2u] = 0x0F;
			spi_transfer(SPI_DAC1, FNULL, DAC_SPI_Tx_Buf, 3u);

		}
		
		//PORTE |= 0x04;
	}
	else if (DAC_Ctr == 2u)
	{
		//PORTE &= ~0x04;
		
		/* set power control register */
			DAC_SPI_Tx_Buf[0u] = 0x10;
			DAC_SPI_Tx_Buf[1u] = 0x00;
			DAC_SPI_Tx_Buf[2u] = 0x0F;
			spi_transfer(SPI_DAC1, FNULL, DAC_SPI_Tx_Buf, 3u);


		//PORTE |= 0x04;


	}
	else if(DAC_Ctr == 3u)
	{
		//PORTE &= ~0x04;
		for(i=0; i<1u; i++)
		{
			/* set DAC register */
			DAC_SPI_Tx_Buf[0u] = 0x00;
			DAC_SPI_Tx_Buf[1u] = 0x80;
			DAC_SPI_Tx_Buf[2u] = 0x00;
		
			spi_transfer(SPI_DAC1, FNULL, DAC_SPI_Tx_Buf, 3u);
			//PORTE |= 0x04;
	
			//DAC_SPI_Tx_Buf[0u] = 0x1D;
			//DAC_SPI_Tx_Buf[1u] = 0x00;
			//DAC_SPI_Tx_Buf[2u] = 0x00;
			//
			//	spi_transfer(Spi_EEPROM, FNULL, DAC_SPI_Tx_Buf, 3u);
		}
	}
	else if(DAC_Ctr == 4u)          //read back power control register
	{
		/* read power control register */
		DAC_SPI_Tx_Buf[0u] = 0x90;
		DAC_SPI_Tx_Buf[1u] = 0x00;
		DAC_SPI_Tx_Buf[2u] = 0x00;
	
		spi_transfer(SPI_DAC1, FNULL, DAC_SPI_Tx_Buf, 3u);
	
		DAC_SPI_Tx_Buf[0u] = 0x18;
		DAC_SPI_Tx_Buf[1u] = 0x00;
		DAC_SPI_Tx_Buf[2u] = 0x00;
	
		spi_transfer(SPI_DAC1, DAC_SPI_Rx_Buf, DAC_SPI_Tx_Buf, 3u);
	
		memcpy(DAC_Power_Ctrl_Rx_Buf,DAC_SPI_Rx_Buf, 3u);
		
	}
	else if(DAC_Ctr == 5u)          //read back output range register
	{
		/* read power control register */
		DAC_SPI_Tx_Buf[0u] = 0x88;
		DAC_SPI_Tx_Buf[1u] = 0x00;
		DAC_SPI_Tx_Buf[2u] = 0x00;
	
		spi_transfer(SPI_DAC1, FNULL, DAC_SPI_Tx_Buf, 3u);
	
		DAC_SPI_Tx_Buf[0u] = 0x18;
		DAC_SPI_Tx_Buf[1u] = 0x00;
		DAC_SPI_Tx_Buf[2u] = 0x00;
	
		spi_transfer(SPI_DAC1, DAC_SPI_Rx_Buf, DAC_SPI_Tx_Buf, 3u);
	
		memcpy(DAC_Output_Range_Rx_Buf,DAC_SPI_Rx_Buf, 3u);
		
	}
	else if(DAC_Ctr == 6u)          //read back DAC register
	{
		/* read power control register */
		DAC_SPI_Tx_Buf[0u] = 0x80;
		DAC_SPI_Tx_Buf[1u] = 0x00;
		DAC_SPI_Tx_Buf[2u] = 0x00;
	
		spi_transfer(SPI_DAC1, FNULL, DAC_SPI_Tx_Buf, 3u);
	
		DAC_SPI_Tx_Buf[0u] = 0x18;
		DAC_SPI_Tx_Buf[1u] = 0x00;
		DAC_SPI_Tx_Buf[2u] = 0x00;
	
		spi_transfer(SPI_DAC1, DAC_SPI_Rx_Buf, DAC_SPI_Tx_Buf, 3u);
	
		memcpy(DAC_Data_Rx_Buf,DAC_SPI_Rx_Buf, 3u);
		
	}
	else if (DAC_Ctr > 10u)
	{
		DAC_Ctr = 0u;
	}



	

	//spi_transfer(Spi_EEPROM, SPI_Rx_Buf, SPI_Read_Back_Buf, 4);


}
