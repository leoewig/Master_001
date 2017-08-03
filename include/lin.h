/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    lin.h
 *
 * description:  
 *  			lin configure header.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Mar-23  Jun Wu           initial version
 *
 *****************************************************************************/
#if !defined(LIN_H)
#define      LIN_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#include "regs.h"
#include "lin_types.h"


/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/ 
#define LIN_BAUD  		10417
#define LIN1_PIN_MASK 0x10                  //pm4
#define LIN2_PIN_MASK 0x20                  //pm5
//#define     LIN_ID         0x30   
#define     LIN_ID         0x31 
#define  SCI0SR1_TDRE (SCI0SR1&0x80)
#define  SCI1SR1_TDRE (SCI1SR1&0x80)
#define  SCI0SR1_RDRF (SCI0SR1&0x20)
#define  SCI1SR1_RDRF (SCI1SR1&0x20)
#define  SCI0CR2_TIE  (SCI0CR2&0x80)
#define  SCI1CR2_TIE  (SCI1CR2&0x80)
//#define     EN1         PTM4     //LIN??
//#define     EN1_dir     DDRM_DDRM4
//#define     EN2         PTM_PTM5     //LIN??
//#define     EN2_dir     DDRM_DDRM5
#define BIT(A,B)      ((A>>B)&0x01)   // A ??? 
                                      // B ??A?????

#define LIN_Transmit_Disable(channel) do{ switch(channel)                     \
                                            { case lin_channel_1:             \
                                                SCI0CR2&=(~0x80);             \
                                                break;                        \
                                              case lin_channel_2:             \
                                                SCI1CR2&=(~0x80);             \
                                                break;                        \
                                            }                                 \
                                        } while(0)
                  
#define LIN_Transmit_Enable(channel) do{ switch(channel)                        \
                                            { case lin_channel_1:               \
                                                SCI0CR2|=0x80;                  \
                                                break;                          \
                                              case lin_channel_2:               \
                                                SCI1CR2|=0x80;                  \
                                                break;                          \
                                            }                                   \
                                        } while(0)
/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
typedef enum
{
    lin_channel_1 = 0u,
    lin_channel_2,
    lin_channel_max
} lin_channel_e;

enum lin_state_e 
{ IDLE, 
  _BREAK, 
  SYNCH, 
  PROTECTED_IDENTIFIER, 
  DATA0, 
  DATA1, 
  DATA2, 
  DATA3, 
  DATA4, 
  DATA5, 
  DATA6, 
  DATA7, 
  CHECKSUM,
  Complete
};

typedef enum  
{ trans_detect, 
  lin1_28, 
  lin1_29, 
  lin2_28, 
  lin2_29
} lin_trans_state_e;

struct lin_frame_s {
  unsigned char protected_id;
  unsigned char data[8];
  unsigned char check;
  enum lin_state_e state;
  unsigned char error;
};

struct lin_message_s {
  unsigned char identifier;
  unsigned char data[8];
  bool_t Flag_Master_Send_Data;         //master send data field or slave send data to master
  uint8_t send_data_length;                // length of data field
  uint8_t receive_data_length;
};

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
//extern struct lin_message_s msg_send;
//extern struct lin_message_s msg_get;
//extern struct lin_frame_s frame_send;
//extern struct lin_frame_s frame_receive;
//extern uint8_t MSG_Send_Length;
//extern uint8_t MSG_Send_Length;
//extern uint8_t MSG_Receive_Length;
//extern bool_t LIN_Master_Send;                  //master send data or receive data 



extern struct lin_message_s LIN_Msg_Send[lin_channel_max];
extern struct lin_frame_s frame_send[lin_channel_max];
extern struct lin_frame_s frame_receive[lin_channel_max];

extern uint8_t LIN_MSG_Receive_28[lin_channel_max][8u];
extern uint8_t LIN_MSG_Receive_29[lin_channel_max][8u];
extern bool_t Flag_LIN_Send_En[lin_channel_max];
extern lin_trans_state_e LIN_Trans_State;
extern lin_seq_data_s LIN_Seq_Data[LIN_STEPS_MAX];
extern bool_t Flag_LIN_Seq_En;
extern bool_t Flag_LIN_Nml_En;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/ 
void lin_init(void);
INTERRUPT void LIN1_Interrupt(void);
INTERRUPT void LIN2_Interrupt(void);
bool_t LINSendMsg(lin_channel_e lin_channel);
void LIN_read_inputs(void);
void LIN_write_outputs(lin_channel_e lin_channel);
void lin_update(void);
void lin_seq_update(void);


#endif /* LIN_H */
