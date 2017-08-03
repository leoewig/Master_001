/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    motor.h
 *
 * description:  
 *  			motor control header.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Feb-4  Jun Wu           initial version
 *
 *****************************************************************************/
#if !defined(MOTOR_H)
#define      MOTOR_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#include "regs.h"
#include "smm.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/ 


 #define HBrige1_PIN_MASK 0x40
 #define HBrige2_PIN_MASK 0x80
 #define HBrige_Disable(channel) do{ if(channel==HBrige1)                  \
									{                                \
										PTT&=(~HBrige1_PIN_MASK);     \
									}                                \
									else if (channel==HBrige2)          \
									{                                \
										PTT&=(~HBrige2_PIN_MASK);     \
									}                                \
								} while(0)
#define HBrige_Enable(channel) do{ if(channel==HBrige1)                   \
									{                                \
										PTT|=HBrige1_PIN_MASK;     	 \
									}                                \
									else if (channel==HBrige2)          \
									{                                \
										PTT|=HBrige2_PIN_MASK;   		 \
									}                                \
								} while(0)

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
 enum HBrige_channel_e
{
    HBrige1 = 0u,
    HBrige2,
    MAX_HBrige_NUM
};

 enum Rotating_Direction_e
{
    Rotating_positve = 0u,
    Rotating_negative,
    Unknown
};

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
/* motor rotating status */
extern bool_t Motor_rotating[sent_channel_max];

/* motor rotating direction */
extern enum Rotating_Direction_e motor_direction[sent_channel_max];



/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/ 
void speed_pid(sent_channel_e sent_channel);
void motor_init(void);
void motor_rotating_update(void);
void motor_stop(sent_channel_e sent_channel);
void motor_positive(sent_channel_e sent_channel);
void motor_negtive(sent_channel_e sent_channel);

#endif /* MOTOR_H */
