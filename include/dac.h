/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2017 - All rights reserved 
 *
 * file name:    dac.h
 *
 * description:  
 *  			dac header.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2017-Mar_16 Jun Wu			init
 *
 *****************************************************************************/
#if !defined(DAC_H)
#define      DAC_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#include "regs.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/ 
 /******************************************************************************/
/* AD5724R                                                                     */
/******************************************************************************/
/* AD5724R GPIO */
//	#define AD5724R_LDAC_OUT    GPIO1_PIN_OUT
//	#define AD5724R_LDAC_LOW    GPIO1_LOW
//	#define AD5724R_LDAC_HIGH   GPIO1_HIGH
//	#define AD5724R_CLR_OUT     GPIO2_PIN_OUT
//	#define AD5724R_CLR_LOW     GPIO2_LOW
//	#define AD5724R_CLR_HIGH    GPIO2_HIGH

/* AD5724R Register Map */
#define AD5724R_REG_DAC             0x00 // DAC register
#define AD5724R_REG_RANGE_SELECT    0x01 // Output range select register
#define AD5724R_REG_POWER_CONTROL   0x02 // Power control register
#define AD5724R_REG_CONTROL         0x03 // Control register

/* AD5724R Channel Address */
#define AD5724R_DAC_A               0x00 // Address of channel A 
#define AD5724R_DAC_B               0x01 // Address of channel B
#define AD5724R_DAC_C               0x02 // Address of channel C
#define AD5724R_DAC_D               0x03 // Address of channel D
#define AD5724R_DAC_ALL             0x04 // All four DACs 

/* AD5724R Range Bits */
#define AD5724R_UNIPOLAR_5_RANGE    0x00 // 0..+5(V)        
#define AD5724R_UNIPOLAR_10_RANGE   0x01 // 0..+10(V)       
#define AD5724R_UNIPOLAR_10_8_RANGE 0x02 // 0..+10.8(V)     
#define AD5724R_BIPOLAR_5_RANGE     0x03 // -5..+5(V)       
#define AD5724R_BIPOLAR_10_RANGE    0x04 // -10...+10(V)    
#define AD5724R_BIPOLAR_10_8_RANGE  0x05 // -10.8...+10.8(V) 

/* Control Functions */
#define AD5724R_NOP_OPERATION       0x00 // No operation instruction used in readback operations.
#define AD5724R_SDO_DISABLE         0x11 // Set by the user to disable the SDO output.
#define AD5724R_CLR_SELECT          0x12 // Selects the clear code: 0V, Midscale or Negative full scale.
#define AD5724R_CLAMP_ENABLE        0x14 // Set by the user to enable the current-limit clamp.
#define AD5724R_TSD_ENABLE          0x18 // Set by the user to enable the thermal shutdown feature.
#define AD5724R_CLEAR               0x40 // Sets the DAC registers to the clear code and updates the outputs.
#define AD5724R_LOAD                0x50 // Updates the DAC registers and, consequently, the DAC outputs.
	
/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
typedef enum
{
    AD5724R_1 = 0u,
    AD5724R_2
} dac_cs_e;


typedef enum
{
    DAC_Power_OFF = 0u,
    DAC_Power_ON
} dac_pwr_onoff_e;
/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
extern bool_t Flag_CAN2ANA1_En;
extern bool_t Flag_CAN2ANA2_En;
extern int16_t CAN2ANA_Vol[8u];
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/ 
void dac_init(void);
void dac_update(void);


#endif /* DAC_H */
