/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    ect.h
 *
 * description:  
 *  Contains ect initiate for sent.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Feb-2  Jun Wu           initial version
 * 2017-Mar_16 Jun Wu			Add ECT2
 *
 *****************************************************************************/
#if !defined(ECT_H)
#define      ECT_H


/******************************************************************************
 * INCLUDES
 *****************************************************************************/
 #define ECT_Channel1_MASK 0x02
 #define ECT_Channel2_MASK 0x04
 #define ECT_Channel3_MASK 0x08
 #define ECT_Overflow_MASK 0x80
 #define ECT_Channel1_Falling_Edge_MASK 0x08
 #define ECT_Channel2_Falling_Edge_MASK 0x20
 #define ECT_Channel3_Rising_Edge_MASK  0x40
 #define ECT_Channel3_Falling_Edge_MASK 0x80

 #define ECT_Channel1_Xg_Address_MASK 0x76
 #define ECT_Channel2_Xg_Address_MASK 0x75
/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
extern uint16_t PWM_In_Freq;
extern uint16_t PWM_In_Duty;
extern uint16_t PWM_In_Timeout_CNT;
extern uint8_t PWM_In_Freq_Cur;
extern uint8_t PWM_In_Freq_Pre;
extern uint8_t PWM_In_Freq_delta;
extern uint8_t PWM_In_Duty_Cur;
extern uint8_t PWM_In_Duty_Pre;
extern uint8_t PWM_In_Duty_delta;
extern uint8_t PWMIN_Freq_Unvalid_Cnt;
extern uint8_t PWMIN_Duty_Unvalid_Cnt;
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
INTERRUPT void ECT_Overflow_Interrupt(void);
INTERRUPT void ECT_Channel3_Interrupt(void);
void ect_init(void);
uint8_t Cap1_Enable(void);
uint8_t Cap1_Disable(void);
void pwmin_read_inputs(void);


#endif /* ECT_H */