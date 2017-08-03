/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    lin_init.c
 *
 * description:  
 *  			lin 1.3 configure.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Mar-23  Jun Wu           initial version
 *
 *****************************************************************************/
#include "lin.h"
#include "isr.h"
#include "Main_isr_types.h"
#include "watchdog.h"
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
PRIVATE uint16_t Send_Timeout_Ctr[lin_channel_max] = {0u};
PRIVATE uint16_t Receive_Timeout_Ctr[lin_channel_max] = {0u};
PRIVATE uint8_t Receive_Data_Ctr[lin_channel_max] = {0u};
PRIVATE uint8_t Send_Data_Ctr[lin_channel_max] = {0u};

PRIVATE uint8_t LIN_Parity_Id[lin_channel_max] = {0u};


/******************************************************************************
 * Public DATA
 *****************************************************************************/ 
struct lin_frame_s frame_send[lin_channel_max] = 
{
  {
    0u,
    {0u},
    0u,
    IDLE,
    0u
  },
  {
    0u,
    {0u},
    0u,
    IDLE,
    0u
  }
};

struct lin_frame_s frame_receive[lin_channel_max] = 
{
  {
    0u,
    {0u},
    0u,
    IDLE,
    0u
  },
  {
    0u,
    {0u},
    0u,
    IDLE,
    0u
  }
};

struct lin_message_s LIN_Msg_Send[lin_channel_max] = 
{
  {
    0u,
    {0u},
    FALSE,
    8u
  },
  {
    0u,
    {0u},
    FALSE,
    8u
  },
}
;
//struct lin_message_s msg_get;
//struct lin_frame_s frame_send[lin_channel];
//struct lin_frame_s frame_receive;




extern bool_t do_en_LIN;
/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 *****************************************************************************/

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
/*************************************************************/
/*                     Calculate Parity                      */
/*************************************************************/
PRIVATE uint8_t LINCalcParity(uint8_t id)
{
  uint8_t parity, p0,p1;
  parity=id; 
  p0=(BIT(parity,0)^BIT(parity,1)^BIT(parity,2)^BIT(parity,4))<<6;     //even parity
  p1=(!(BIT(parity,1)^BIT(parity,3)^BIT(parity,4)^BIT(parity,5)))<<7;  //odd parity
  parity|=(p0|p1);
  return parity;
}

/*************************************************************/
/*                       Calculate Checksum                  */
/*************************************************************/
/*PRIVATE uint8_t LINCalcChecksum(uint8_t *data)                                    //LIN1.3
{
  unsigned int sum = 0;
  uint8_t i;

  for(i = 0; i < 8; i++)
  {
    sum += data[i];
    if(sum&0xFF00)
      sum = (sum&0x00FF) + 1;
  }
  sum ^= 0x00FF;         
  return (uint8_t )sum;
}*/

PRIVATE uint8_t LINCalcChecksum(uint8_t id, uint8_t *data, uint8_t data_len)                           //LIN2.0       
{
  unsigned int sum = id;
  uint8_t i;

  for(i = 0; i < data_len; i++)
  {
    sum += data[i];
    if(sum&0xFF00)
      sum = (sum&0x00FF) + 1;
  }
  sum ^= 0x00FF;         
  return (uint8_t )sum;
}

/*************************************************************/
/*                  Lin Check Send Function                  */
/*************************************************************/
PRIVATE bool_t LINCheckSend(lin_channel_e lin_channel, enum lin_state_e status, uint8_t val)
{
  /* wait for data received */
  while(frame_send[lin_channel].state < status)
  {
    Send_Timeout_Ctr[lin_channel]++;
    kick_watchdog();
    if((frame_send[lin_channel].error) || (Send_Timeout_Ctr[lin_channel] > 1000u))
    {
    Send_Timeout_Ctr[lin_channel] = 0u;
    return(FALSE);  
    }
  }
  
  Send_Timeout_Ctr[lin_channel] = 0u;  
    
  switch(status) 
  {
    case _BREAK:
    case SYNCH:
      break;
      
    case PROTECTED_IDENTIFIER:
      if(frame_send[lin_channel].protected_id != val)
        return(FALSE);
      break;
        
    case DATA0:
    case DATA1:
    case DATA2:
    case DATA3:
    case DATA4:
    case DATA5:
    case DATA6:
    case DATA7:
      if(frame_send[lin_channel].data[status-DATA0] != val)
        return(FALSE);
      break;
        
    case CHECKSUM:
      if(frame_send[lin_channel].check != val)
        return(FALSE);
      break;
  }
  return(TRUE);
}

/*************************************************************/
/*                     LIN Send Char                       */
/*************************************************************/
PRIVATE bool_t LINSendChar(lin_channel_e lin_channel, uint8_t ch)
{
  if(lin_channel == lin_channel_1)
  {
    //while(!SCI0SR1_TDRE);         //wait for transmit buffer to be empty
    SCI0DRL = ch;
    return(TRUE);
  }
  else if(lin_channel == lin_channel_2)
  {
    //while(!SCI1SR1_TDRE);         //wait for transmit buffer to be empty
    SCI1DRL = ch;
    return(TRUE);
  }
  else
  {

  }
}

/*************************************************************/
/*                      LIN Send break                       */
/*************************************************************/
PRIVATE bool_t LINSendbreak(lin_channel_e lin_channel)
{
  if(lin_channel == lin_channel_1)
  {
    //while(!SCI0SR1_TDRE);         //wait for transmit buffer to be empty         
    SCI0CR2 |= 0x01;        //break byte in queue
    SCI0CR2 &= ~0x01;       //back to normal
    return(TRUE);
  }
  else if(lin_channel == lin_channel_2)
  {
    //while(!SCI1SR1_TDRE);         //wait for transmit buffer to be empty         
    SCI1CR2 |= 0x01;        //break byte in queue
    SCI1CR2 &= ~0x01;       //back to normal
    return(TRUE);
  }
  else
  {

  }
  
}

 /*************************************************************/
/*                     LIN receive function                      */
/*************************************************************/
/* for master mode */

PRIVATE bool_t LIN1GetChar(lin_channel_e lin_channel)
{
  unsigned volatile char ch;
  
  // LIN接收通道状态
  switch(frame_receive[lin_channel].state)
  {
    case IDLE:
              if((!(SCI0SR1&0x22)) || (SCI0DRL))
            {
            return(FALSE);   
            }
                 
              frame_receive[lin_channel].state = _BREAK;
              break;
    case _BREAK:
              if((!(SCI0SR1&0x20)) || (SCI0DRL != 0x55))
            {
            return(FALSE);   
            }
                
              frame_receive[lin_channel].state = SYNCH;
              break;  
    case SYNCH:
              if(!(SCI0SR1&0x20))
            {
            return(FALSE);  
            }
                 
              ch = SCI0DRL;
              frame_receive[lin_channel].protected_id = ch;
            
            if(Receive_Data_Ctr[lin_channel] < LIN_Msg_Send[lin_channel].receive_data_length)
            {
              Receive_Data_Ctr[lin_channel]++;
              frame_receive[lin_channel].state = PROTECTED_IDENTIFIER;
            }
            else 
            {
              Receive_Data_Ctr[lin_channel] = 0u;
              frame_receive[lin_channel].state = DATA7;
            }   
              break;   
    case PROTECTED_IDENTIFIER:
    case DATA0:
    case DATA1:
    case DATA2:
    case DATA3:
    case DATA4:
    case DATA5:
    case DATA6:
              if(!(SCI0SR1&0x20))
              {
                return(FALSE);  
              }
              ch = SCI0DRL;
              frame_receive[lin_channel].data[frame_receive[lin_channel].state-PROTECTED_IDENTIFIER] = ch;
              
              if(Receive_Data_Ctr[lin_channel] < LIN_Msg_Send[lin_channel].receive_data_length)
              {
              Receive_Data_Ctr[lin_channel]++;
              frame_receive[lin_channel].state += 1;
              }
              else 
              {
                  Receive_Data_Ctr[lin_channel] = 0u;
                  frame_receive[lin_channel].state = DATA7;
              } 
              break;         
    case DATA7:
              if(!(SCI0SR1&0x20))
                return(FALSE); 
              ch = SCI0DRL;
              frame_receive[lin_channel].check = ch;
            
              frame_receive[lin_channel].state = CHECKSUM;
              break;
    case CHECKSUM:

              if(frame_receive[lin_channel].protected_id == LINCalcParity(0x28))
              {
                  LIN_MSG_Receive_28[lin_channel][0u] = frame_receive[lin_channel].data[0u];
                  LIN_MSG_Receive_28[lin_channel][1u] = frame_receive[lin_channel].data[1u];
              }
              else if(frame_receive[lin_channel].protected_id == LINCalcParity(0x29))
              {
                  LIN_MSG_Receive_29[lin_channel][0u] = frame_receive[lin_channel].data[0u];
                  LIN_MSG_Receive_29[lin_channel][1u] = frame_receive[lin_channel].data[1u];  
                  LIN_MSG_Receive_29[lin_channel][2u] = frame_receive[lin_channel].data[2u];  
              }

              frame_receive[lin_channel_1].state = IDLE;
              break;
              //return(FALSE);  
  }
  //frame_send[lin_channel].state+=1;
  return(TRUE);
}


PRIVATE bool_t LIN2GetChar(lin_channel_e lin_channel)
{
  unsigned volatile char ch;
  
  // LIN接收通道状态
  switch(frame_receive[lin_channel].state)
  {
    case IDLE:
      if((!(SCI1SR1&0x22)) || (SCI1DRL))
    {
    return(FALSE);   
    }
         
      frame_receive[lin_channel].state = _BREAK;
      break;
    case _BREAK:
      if((!(SCI1SR1&0x20)) || (SCI1DRL != 0x55))
    {
    return(FALSE);   
    }
        
      frame_receive[lin_channel].state = SYNCH;
      break;  
    case SYNCH:
      if(!(SCI1SR1&0x20))
    {
    return(FALSE);  
    }
         
      ch = SCI1DRL;
      frame_receive[lin_channel].protected_id = ch;
    
    if(Receive_Data_Ctr[lin_channel] < LIN_Msg_Send[lin_channel].receive_data_length)
    {
      Receive_Data_Ctr[lin_channel]++;
      frame_receive[lin_channel].state = PROTECTED_IDENTIFIER;
    }
    else 
    {
      Receive_Data_Ctr[lin_channel] = 0u;
      frame_receive[lin_channel].state = DATA7;
    }   
      break;   
    case PROTECTED_IDENTIFIER:
    case DATA0:
    case DATA1:
    case DATA2:
    case DATA3:
    case DATA4:
    case DATA5:
    case DATA6:
    if(!(SCI1SR1&0x20))
    {
      return(FALSE);  
    }
    ch = SCI1DRL;
    frame_receive[lin_channel].data[frame_receive[lin_channel].state-PROTECTED_IDENTIFIER] = ch;
    
    if(Receive_Data_Ctr[lin_channel] < LIN_Msg_Send[lin_channel].receive_data_length)
    {
    Receive_Data_Ctr[lin_channel]++;
    frame_receive[lin_channel].state += 1;
    }
    else 
    {
    Receive_Data_Ctr[lin_channel] = 0u;
    frame_receive[lin_channel].state = DATA7;
    } 
    break;         
    case DATA7:
      if(!(SCI1SR1&0x20))
        return(FALSE); 
      ch = SCI1DRL;
      frame_receive[lin_channel].check = ch;
    
    frame_receive[lin_channel].state = CHECKSUM;
      break;
    case CHECKSUM:

              if(frame_receive[lin_channel].protected_id == LINCalcParity(0x28))
              {
                  LIN_MSG_Receive_28[lin_channel][0u] = frame_receive[lin_channel].data[0u];
                  LIN_MSG_Receive_28[lin_channel][1u] = frame_receive[lin_channel].data[1u];
              }
              else if(frame_receive[lin_channel].protected_id == LINCalcParity(0x29))
              {
                  LIN_MSG_Receive_29[lin_channel][0u] = frame_receive[lin_channel].data[0u];
                  LIN_MSG_Receive_29[lin_channel][1u] = frame_receive[lin_channel].data[1u];  
                  LIN_MSG_Receive_29[lin_channel][2u] = frame_receive[lin_channel].data[2u];  
              }

              frame_receive[lin_channel_2].state = IDLE;
              break;
              //return(FALSE);  
  }
  //frame_send[lin_channel].state+=1;
  return(TRUE);
}

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

void lin_init(void)
{	
  uint8_t i;
  uint16_t LIN_baud_rate;
  
/*init LIN1*/
  LIN_baud_rate = 50000000/16/LIN_BAUD;                  //set baud rate 
  SCI0BDH = (uint8_t)(LIN_baud_rate>>8);
  SCI0BDL = (uint8_t)LIN_baud_rate;
  SCI0CR1 = 0x00;        			//normal operation, no even/odd parity check
  SCI0CR2 = 0x2c;         			//enable transmit and receive，enable receive interrupt
  SCI0SR2 |= 0x04;          		//SCI0SR2_BRK13 = 1;  
  
  (void)SCI0SR1;                    //clear registers
  (void)SCI0DRL;
  
  (void)set_interrupt_level(VECT_OFFSET_SCI0, INT_LVL_DEFAULT, INT_CPU_DEST);                    //set interrupt
  (void)set_interrupt_level(VECT_OFFSET_SCI0, INT_LVL_DEFAULT, INT_CPU_DEST);                    //set interrupt
  
 /*init LIN2*/
  //SCI1BD = 50000000/16/LIN_BAUD;    //设置SCI1波特率为9600  
  SCI1BDH = (uint8_t)(LIN_baud_rate>>8);
  SCI1BDL = (uint8_t)LIN_baud_rate;
  SCI1CR1 = 0x00;        			//设置SCI0为正常模式，八位数据位，无奇偶校验
  SCI1CR2 = 0x2c;        			//允许接收和发送数据，允许接收中断功能 
  SCI1SR2 |= 0x04;					//SCI1SR2_BRK13 = 1;  
  
  (void)SCI1SR1;
  (void)SCI1DRL;
 
  (void)set_interrupt_level(VECT_OFFSET_SCI1, INT_LVL_DEFAULT, INT_CPU_DEST);
  (void)set_interrupt_level(VECT_OFFSET_SCI0, INT_LVL_DEFAULT, INT_CPU_DEST);                    //set interrupt
  
  do_lin_en_1 = TRUE;
  do_lin_en_2 = TRUE;
	
}


/*************************************************************/
/*                     LIN Send Messsage                      */
/*************************************************************/
bool_t LINSendMsg(lin_channel_e lin_channel)
{
  uint8_t  check_sum, i;

    switch(frame_send[lin_channel].state)
    {
        case IDLE:
              if(1)
              {
                  frame_send[lin_channel].state = _BREAK;
              }
              break;

        case  _BREAK:
              //PORTK|=0x01;
              /* send break */
              if(!LINSendbreak(lin_channel))
              {
                return(FALSE);
              }
              //PORTK &= ~0x01;
          
              /* check send break */
              //if(!LINCheckSend(lin_channel, _BREAK, 0x00))
              //{
              //  return(FALSE); 
              //}

              frame_send[lin_channel].state = SYNCH;
              break;

        case  SYNCH:
              /* send sync */
              if(!LINSendChar(lin_channel, 0x55))
              {
                return(FALSE);
              }    
              /*check send sync */
              //if(!LINCheckSend(lin_channel, SYNCH,0x55))
              //{
              //  return(FALSE); 
              //}

              frame_send[lin_channel].state = PROTECTED_IDENTIFIER;
              break;

        case  PROTECTED_IDENTIFIER:
              /* calculate parity */
              LIN_Parity_Id[lin_channel]=LINCalcParity(LIN_Msg_Send[lin_channel].identifier);
              
              /* send identity */
              if(!LINSendChar(lin_channel, LIN_Parity_Id[lin_channel]))
              {
                return(FALSE);
              }
              /* check send identity */
              //if(!LINCheckSend(lin_channel, PROTECTED_IDENTIFIER, LIN_Parity_Id[lin_channel]))
              //{
              //  return(FALSE); 
              //}

              if(LIN_Msg_Send[lin_channel].Flag_Master_Send_Data)
              {
                  frame_send[lin_channel].state = DATA0;
              }
              else
              {
                  frame_send[lin_channel].state = Complete;
              }             
              break;

        case  DATA0:  
               /* send data */
              Send_Data_Ctr[lin_channel]++;

              if(Send_Data_Ctr[lin_channel] <= LIN_Msg_Send[lin_channel].send_data_length)
              {
                  if(!LINSendChar(lin_channel, LIN_Msg_Send[lin_channel].data[Send_Data_Ctr[lin_channel] - 1u]))
                  {
                    return(FALSE);
                  }
                  /* check send data */
                  //if(!LINCheckSend(lin_channel, DATA0+i, LIN_Msg_Send[lin_channel].data[i]))
                  //{
                  //  return(FALSE); 
                  //}
              }
              else
              {
                  Send_Data_Ctr[lin_channel] = 0u;

                  frame_send[lin_channel].state = CHECKSUM;
              }
              break;

        case  CHECKSUM:
              check_sum = LINCalcChecksum(LIN_Parity_Id[lin_channel], LIN_Msg_Send[lin_channel].data, LIN_Msg_Send[lin_channel].send_data_length);
              /* send checksum */
              if(!LINSendChar(lin_channel, check_sum))
              {
                return(FALSE);
              }
              /* check send checksum */
              //if(!LINCheckSend(lin_channel, CHECKSUM, check_sum))
              //{
              //  return(FALSE); 
              //}

              frame_send[lin_channel].state = Complete;
              Flag_LIN_Send_En[lin_channel] = FALSE;
              break;

        case  Complete:
              frame_send[lin_channel].state = IDLE;

              LIN_Transmit_Disable(lin_channel);

              switch(LIN_Trans_State)
                {
                    case lin1_28:
                          LIN_Trans_State = lin1_29;
                          //LIN_Trans_State = trans_detect;
                          break;

                    case lin1_29:
                          LIN_Trans_State = lin2_28;
                          break;

                    case lin2_28:
                          LIN_Trans_State = lin2_29;
                          break;
                    case lin2_29:
                          LIN_Trans_State = trans_detect;
                          break;
                }
              break;
              
        default:
              frame_send[lin_channel].state = IDLE;
              break;        
    }
}

//  bool_t LINSendMsg(lin_channel_e lin_channel)
//  {
//      uint8_t  check_sum, parity_id, i;
//  
//      frame_send[lin_channel].error = 0;
//      frame_send[lin_channel].state = IDLE;
//  
//      //PORTK|=0x01;
//      /* send break */
//      if(!LINSendbreak(lin_channel))
//      {
//        return(FALSE);
//      }
//      //PORTK &= ~0x01;
//  
//      /* check send break */
//      //if(!LINCheckSend(lin_channel, _BREAK, 0x00))
//      //{
//      //  return(FALSE); 
//      //}
//  
//      /* send sync */
//      if(!LINSendChar(lin_channel, 0x55))
//      {
//        return(FALSE);
//      }    
//      /*check send sync */
//      //if(!LINCheckSend(lin_channel, SYNCH,0x55))
//      //{
//      //  return(FALSE); 
//      //}
//      
//      /* calculate parity */
//      parity_id=LINCalcParity(LIN_Msg_Send[lin_channel].identifier);
//      
//      /* send identity */
//      if(!LINSendChar(lin_channel, parity_id))
//      {
//        return(FALSE);
//      }
//      /* check send identity */
//      //if(!LINCheckSend(lin_channel, PROTECTED_IDENTIFIER, parity_id))
//      //{
//      //  return(FALSE); 
//      //}
//  
//      /* if master send data or not */
//      if(LIN_Msg_Send[lin_channel].Flag_Master_Send_Data)
//      {
//          for(i=0; i < LIN_Msg_Send[lin_channel].send_data_length; i++) 
//          {
//            /* send data */
//            if(!LINSendChar(lin_channel, LIN_Msg_Send[lin_channel].data[i]))
//            {
//            	return(FALSE);
//            }
//            /* check send data */
//          	//if(!LINCheckSend(lin_channel, DATA0+i, LIN_Msg_Send[lin_channel].data[i]))
//            //{
//          	//	return(FALSE); 
//            //}
//          }
//  
//          check_sum = LINCalcChecksum(parity_id, LIN_Msg_Send[lin_channel].data, LIN_Msg_Send[lin_channel].send_data_length);
//          /* send checksum */
//          if(!LINSendChar(lin_channel, check_sum))
//          {
//          	return(FALSE);
//          }
//          /* check send checksum */
//          //if(!LINCheckSend(lin_channel, CHECKSUM, check_sum))
//          //{
//          //  return(FALSE); 
//          //}
//          	 
//          frame_send[lin_channel].state = IDLE;
//      }
//      else
//      {
//    //      while(frame_send[lin_channel].state < CHECKSUM)
//    //      {
//    //      	
//    //        Receive_Timeout_Ctr[lin_channel]++;
//    //        //kick_watchdog();
//    //        if((frame_send[lin_channel].error) || (Receive_Timeout_Ctr[lin_channel] > 100u))
//    //        {
//    //          Receive_Timeout_Ctr[lin_channel] = 0u;
//    //          frame_send[lin_channel].state = IDLE;
//    //          return(FALSE);  
//    //        }
//    //      }
//  //
//    //      Receive_Timeout_Ctr[lin_channel] = 0u;
//    //      frame_send[lin_channel].state = IDLE;
//      }
//  
//  
//      return(TRUE);
//  }



/*************************************************************/
/*                     LIN2接收字节函数                      */
/*************************************************************/
//bool_t LIN2GetChar(void)
//{
//  unsigned volatile char ch;
//  
//  // LIN接收通道状态
//  switch(frame_receive.state)
//  {
//    case IDLE:
//      if(!(SCI1SR1&0x22))
//        return(FALSE);  
//      if(SCI1DRL)
//        return(FALSE);
//      break;
//    case _BREAK:
//      if(!(SCI1SR1&0x20))
//        return(FALSE); 
//      if(SCI1DRL != 0x55)
//        return(FALSE);   
//      break;  
//    case SYNCH:
//      if(!(SCI1SR1&0x20))
//        return(FALSE); 
//      ch = SCI1DRL;
//      frame_receive.protected_id = ch;
//      break;   
//    case PROTECTED_IDENTIFIER:
//    case DATA0:
//    case DATA1:
//    case DATA2:
//    case DATA3:
//    case DATA4:
//    case DATA5:
//    case DATA6:
//      if(!(SCI1SR1&0x20))
//        return(FALSE); 
//      ch = SCI1DRL;
//      frame_receive.data[frame_receive.state-PROTECTED_IDENTIFIER] = ch;
//      break;
//    case DATA7:
//      if(!(SCI1SR1&0x20))
//        return(FALSE); 
//      ch = SCI1DRL;
//      frame_receive.check = ch;
//      break;
//    case CHECKSUM:
//      return(FALSE);  
//  }
//  frame_receive.state+=1;
//  return(TRUE);
//}

 /* for slave mode */
/*
bool_t LIN1GetChar(void)
{
  unsigned volatile char ch;
  
  // LIN接收通道状态
  switch(frame_receive.state)
  {
    case IDLE:
      if(!(SCI0SR1&0x22))
        return(FALSE);  
      if(SCI0DRL)
        return(FALSE);
      break;
    case _BREAK:
      if(!(SCI0SR1&0x20))
        return(FALSE); 
      if(SCI0DRL != 0x55)
        return(FALSE);   
      break;  
    case SYNCH:
      if(!(SCI0SR1&0x20))
        return(FALSE); 
      ch = SCI0DRL;
      frame_receive.protected_id = ch;
      break;   
    case PROTECTED_IDENTIFIER:
    case DATA0:
    case DATA1:
    case DATA2:
    case DATA3:
    case DATA4:
    case DATA5:
    case DATA6:
      if(!(SCI0SR1&0x20))
        return(FALSE); 
      ch = SCI0DRL;
      frame_receive.data[frame_receive.state-PROTECTED_IDENTIFIER] = ch;
      break;
    case DATA7:
      if(!(SCI0SR1&0x20))
        return(FALSE); 
      ch = SCI0DRL;
      frame_receive.check = ch;
      break;
    case CHECKSUM:
      return(FALSE);  
  }
  frame_receive.state+=1;
  return(TRUE);
}
*/
/*************************************************************/
/*                       结点1接收函数                       */
/*************************************************************/

INTERRUPT void LIN1_Interrupt(void)
{ 
    /* receive interrupt */
    if(SCI0SR1_RDRF)
    {
        if(!LIN1GetChar(lin_channel_1)) 
        {
          frame_receive[lin_channel_1].error = 1;
          frame_receive[lin_channel_1].state = IDLE;
        }
    }
    
    /* transmit interrupt */
    if(SCI0CR2_TIE)
    {
        if(SCI0SR1_TDRE)
        { 
            (void)LINSendMsg(lin_channel_1);
        }
    }
       
}

/*************************************************************/
/*                       结点2接收函数                       */
/*************************************************************/
INTERRUPT void LIN2_Interrupt(void)
{
    /* receive interrupt */
    if(SCI1SR1_RDRF)
    {
        if(!LIN2GetChar(lin_channel_2)) 
        {
            frame_receive[lin_channel_2].error = 1;
            frame_receive[lin_channel_2].state = IDLE;
        }
    }

    /* transmit interrupt */
    if(SCI1CR2_TIE)
    {
         if(SCI1SR1_TDRE)
        {
            (void)LINSendMsg(lin_channel_2);
        }
    }
    
}



