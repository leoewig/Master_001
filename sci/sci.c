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
#include "isr.h"
#include "Main_isr_types.h"
#include "watchdog.h"


/*****************************************************************************
 * MACROS
 *****************************************************************************/
#define OVERRUN_ERR      1             /* Overrun error flag bit   */
#define COMMON_ERR       2             /* Common error of RX       */
#define CHAR_IN_RX       4             /* Char is in the RX buffer     */
#define RUNINT_FROM_TX   8             /* Interrupt is in progress */
#define FULL_RX          16            /* Full receive buffer      */
/*****************************************************************************
 * CALIBRATABLE DATA
 *****************************************************************************/

 
/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

/******************************************************************************
 * Public DATA
 *****************************************************************************/ 
uint8_t SerFlag;                   
uint16_t SCI_InpLen = 0u;                      /* Length of the input buffer's content */
static uint8_t *InpPtrR;         /* Pointer for reading from the input buffer */
static uint8_t *InpPtrW;         /* Pointer for writing to the input buffer */
static uint8_t InpBuffer[SCI_INP_BUF_SIZE]; /* Input buffer for SCI commmunication */
uint16_t SCI_OutLen;                      /* Length of the output bufer's content */
static uint8_t *OutPtrR;         /* Pointer for reading from the output buffer */
static uint8_t *OutPtrW;         /* Pointer for writing to the output buffer */
uint8_t OutBuffer[SCI_OUT_BUF_SIZE]; /* Output buffer for SCI commmunication */
/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 *****************************************************************************/


/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
 
/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

void sci_init(void)
{	
//  uint16_t SCI_baud_rate;
//  
//  /*
//  SCI_baud_rate = 50000000/16/SCI_BAUD;
//  
//  SCI1BDH = (uint8_t)(SCI_baud_rate>>8);
//  SCI1BDL = (uint8_t)SCI_baud_rate;
//  SCI1CR1 = 0x00;        			//设置SCI0为正常模式，八位数据位，无奇偶校验
//  SCI1CR2 = 0x2c;         			//允许接收和发送数据，允许接收中断功能 
//  
//  (void)SCI1SR1;
//  (void)SCI1DRL;
//  
//  (void)set_interrupt_level(VECT_OFFSET_SCI1, INT_LVL_DEFAULT, INT_CPU_DEST);
//  */
//  
//  SCI_baud_rate = 50000000/16/SCI_BAUD;
//  
//  SCI1BDH = (uint8_t)(SCI_baud_rate>>8);
//  SCI1BDL = (uint8_t)SCI_baud_rate;
//  SCI1CR1 = 0x00;        			//设置SCI0为正常模式，八位数据位，无奇偶校验
//  SCI1CR2 = 0x2c;         			//允许接收和发送数据，允许接收中断功能 
//  
//  (void)SCI1SR1;
//  (void)SCI1DRL;
//  
//  (void)set_interrupt_level(VECT_OFFSET_SCI1, INT_LVL_DEFAULT, INT_CPU_DEST);
	
}

/*
** ===================================================================
**     Method      :  SCI0_RecvChar (bean AsynchroSerial)
**
**     Description :
**         If any data is received, this method returns one
**         character, otherwise it returns an error code (it does
**         not wait for data). This method is enabled only if the
**         receiver property is enabled.
**         [Note:] Because the preferred method to handle error and
**         break exception in the interrupt mode is to use events
**         <OnError> and <OnBreak> the return value ERR_RXEMPTY has
**         higher priority than other error codes. As a consequence
**         the information about an exception in interrupt mode is
**         returned only if there is a valid character ready to be
**         read.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - Pointer to a received character
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_RXEMPTY - No data in receiver
**                           ERR_BREAK - Break character is detected
**                           (only when the <Interrupt service>
**                           property is disabled and the <Break
**                           signal> property is enabled)
**                           ERR_COMMON - common error occurred (the
**                           <GetError> method can be used for error
**                           specification)
** ===================================================================
*/
uint8_t SCI1_RecvChar(uint8_t *Chr)
{
	uint8_t ccr;
  uint8_t Result = ERR_OK;                /* Return error code */

  if(SCI_InpLen > 0) {                /* Is number of received chars greater than 0? */
    //EnterCritical();                   /* Save the PS register */
	SAVE_INTERRUPTS(ccr);
    SCI_InpLen--;                     /* Decrease number of received chars */
    *Chr = *(InpPtrR++);               /* Received char */
    if(InpPtrR >= (InpBuffer + SCI_INP_BUF_SIZE)) { /* Is the pointer out of the receive buffer? */
      InpPtrR = InpBuffer;             /* Set pointer to the first item into the receive buffer */
    }
    Result = (uint8_t)((SerFlag & (OVERRUN_ERR|COMMON_ERR|FULL_RX))?ERR_COMMON:ERR_OK);
    SerFlag &= ~(OVERRUN_ERR|COMMON_ERR|FULL_RX|CHAR_IN_RX); /* Clear all errors in the status variable */
    //ExitCritical();                    /* Restore the PS register */
	RESTORE_INTERRUPTS(ccr);
  } else {
    return ERR_RXEMPTY;                /* Receiver is empty */
  }
  return Result;                       /* Return error code */
}

/*
** ===================================================================
**     Method      :  SCI0_SendChar (bean AsynchroSerial)
**
**     Description :
**         Sends one character to the channel. If the bean is
**         temporarily disabled (Disable method) SendChar method
**         only stores data into an output buffer. In case of a zero
**         output buffer size, only one character can be stored.
**         Enabling the bean (Enable method) starts the transmission
**         of the stored data. This method is available only if the
**         transmitter property is enabled.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_TXFULL - Transmitter is full
** ===================================================================
*/
uint8_t SCI1_SendChar(uint8_t Chr)
{
	uint8_t ccr;
	
  if(SCI_OutLen == SCI_OUT_BUF_SIZE) { /* Is number of chars in the buffer the same as the size of the transmit buffer? */
    return ERR_TXFULL;                 /* If yes then error */
  }
  //EnterCritical();                   /* Save the PS register */
	SAVE_INTERRUPTS(ccr);
  SCI_OutLen++;                       /* Increase number of uint8_ts in the transmit buffer */
  *(OutPtrW++) = Chr;                  /* Store char to the buffer */
  if(OutPtrW >= (OutBuffer + SCI_OUT_BUF_SIZE)) { /* Is the pointer out of the transmit buffer? */
    OutPtrW = OutBuffer;               /* Set the pointer to the first item in the transmit buffer */
  }
  //SCI0CR2_TIE = 1;                     /* Enable transmit interrupt */
  SCI1CR2 |= SCICR2_TIE_MASK;
  //ExitCritical();                    /* Restore the PS register */
	RESTORE_INTERRUPTS(ccr);
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  SCI0_RecvBlock (bean AsynchroSerial)
**
**     Description :
**         If any data is received, this method returns the block of
**         the data and its length (and incidental error), otherwise
**         it returns an error code (it does not wait for data).
**         This method is available only if non-zero length of the
**         input buffer is defined and the receiver property is
**         enabled.
**         If less than requested number of characters is received
**         only the available data is copied from the receive buffer
**         to the user specified destination. The value ERR_EXEMPTY
**         is returned and the value of variable pointed by the Rcv
**         parameter is set to the number of received characters.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - Pointer to the block of received data
**         Size            - Size of the block
**       * Rcv             - Pointer to real number of the received
**                           data
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_RXEMPTY - The receive buffer didn't
**                           contain the requested number of data.
**                           Only available data has been returned.
**                           ERR_COMMON - common error occurred (the
**                           GetError method can be used for error
**                           specification)
** ===================================================================
*/
uint8_t SCI1_RecvBlock(uint8_t *Ptr, uint16_t Size, uint16_t *Rcv)
{
  uint16_t count;                          /* Number of received chars */
  uint8_t result = ERR_OK;                /* Error code to return */

  for(count = 0; count < Size; count++) {
    switch ( SCI1_RecvChar(Ptr++)) {
    case ERR_RXEMPTY:
      if (result == ERR_OK) {
        result = ERR_RXEMPTY;
      }
     *Rcv = count;                     /* Return number of received chars */
      return result;
      break;
    case ERR_COMMON:
      result = ERR_COMMON;             /* Return the info that an error was detected */
      break;
    default:
      break;
    }
  }
  *Rcv = count;                        /* Return number of received chars */
  return result;                       /* Return last error code*/
}

/*
** ===================================================================
**     Method      :  SCI0_SendBlock (bean AsynchroSerial)
**
**     Description :
**         Sends a block of characters to the channel.
**         This method is available only if non-zero length of the
**         output buffer is defined and the transmitter property is
**         enabled.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Ptr             - Pointer to the block of data to send
**         Size            - Size of the block
**       * Snd             - Pointer to number of data that are sent
**                           (moved to buffer)
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_TXFULL - It was not possible to send
**                           requested number of uint8_ts
** ===================================================================
*/
uint8_t SCI1_SendBlock(uint8_t *Ptr, uint16_t Size, uint16_t *Snd)
{
  
  uint16_t count;                          /* Number of sent chars */
  uint8_t result = ERR_OK;                /* Last error */

  for(count = 0; count < Size; count++) {
    result = SCI1_SendChar(*Ptr++);
    if(result != ERR_OK) {             /* Sending given number of chars */
      break;                           /* Break data block sending */
    }
  }
  *Snd = count;                        /* Return number of sent chars */
  return result;                       /* Return last error code*/
}

/*
** ===================================================================
**     Method      :  SCI0_ClearRxBuf (bean AsynchroSerial)
**
**     Description :
**         Clears the receive buffer.
**         This method is available only if non-zero length of the
**         input buffer is defined and the receiver property is
**         enabled.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
uint8_t SCI1_ClearRxBuf(void)
{
	uint8_t ccr;
  //EnterCritical();                   /* Save the PS register */
	SAVE_INTERRUPTS(ccr);
  SCI_InpLen = 0;                     /* Set number of chars in the transmit buffer to 0 */
  InpPtrR = InpPtrW = InpBuffer;       /* Set pointers on the first item in the transmit buffer */
  SerFlag &= ~ (CHAR_IN_RX | FULL_RX); /* Clear the flags indicating a char in buffer */
  //ExitCritical();                    /* Restore the PS register */
	RESTORE_INTERRUPTS(ccr);
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  SCI0_ClearTxBuf (bean AsynchroSerial)
**
**     Description :
**         Clears the transmit buffer.
**         This method is available only if non-zero length of the
**         output buffer is defined and the receiver property is
**         enabled.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
uint8_t SCI1_ClearTxBuf(void)
{
  uint8_t ccr;
  //EnterCritical();                   /* Save the PS register */
	SAVE_INTERRUPTS(ccr);
  SCI_OutLen = 0;                     /* Set number of chars in the receive buffer to 0 */
  OutPtrR = OutPtrW = OutBuffer;       /* Set pointers on the first item in the receive buffer */
  //ExitCritical();                    /* Restore the PS register */
	RESTORE_INTERRUPTS(ccr);
  return ERR_OK;                       /* OK */
}


/*
** ===================================================================
**     Method      :  SCI0_InterruptRx (bean AsynchroSerial)
**
**     Description :
**         The method services the receive interrupt of the selected 
**         peripheral(s) and eventually invokes the bean's event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define ON_ERROR    1
#define ON_FULL_RX  2
#define ON_RX_CHAR  4

void SCI1_InterruptRx(void)
{
  uint8_t Data;                  /* Temporary variable for data */
  uint8_t OnFlags = 0;                    /* Temporary variable for flags */

  Data = SCI1DRL;                      /* Read data from the receiver */
  if(SCI_InpLen < SCI_INP_BUF_SIZE) { /* Is the number of uint8_ts in the receive buffer lower than the size of the buffer? */
    SCI_InpLen++;                     /* Increse number of chars in the receive buffer */
    *(InpPtrW++) = Data;               /* Save received char to the receive buffer */
    if(InpPtrW >= (InpBuffer + SCI_INP_BUF_SIZE)) { /* Is the pointer out of the receive buffer? */
      InpPtrW = InpBuffer;             /* Set pointer on the first item into the receive buffer */
    }
    OnFlags |= ON_RX_CHAR;             /* Set flag "OnRXChar" */
    if(SCI_InpLen== SCI_INP_BUF_SIZE) { /* Is number of uint8_ts in the receive buffer equal to the size of buffer? */
      OnFlags |= ON_FULL_RX;           /* If yes then set flag "OnFullRxBuff" */
    }
  } else {
    SerFlag |= FULL_RX;                /* If yes then set flag buffer overflow */
    OnFlags |= ON_ERROR;               /* Set flag "OnError" */
  }
  if(OnFlags & ON_ERROR) {             /* Was an error detected? */
    SCI1_OnError();                    /* If yes then invoke user event */
  }
  else {
    if(OnFlags & ON_RX_CHAR) {         /* Is OnRxChar flag set? */
      SCI1_OnRxChar();                 /* If yes then invoke user event */
    }
    if(OnFlags & ON_FULL_RX) {         /* Is OnFullRxBuf flag set? */
      SCI1_OnFullRxBuf();              /* If yes then invoke user event */
    }
  }
}

/*
** ===================================================================
**     Method      :  SCI0_InterruptTx (bean AsynchroSerial)
**
**     Description :
**         The method services the receive interrupt of the selected 
**         peripheral(s) and eventually invokes the bean's event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
#define ON_FREE_TX  1
#define ON_TX_CHAR  2

void SCI1_InterruptTx(void)
{
  uint8_t OnFlags = 0;                    /* Temporary variable for flags */

  if(SerFlag & RUNINT_FROM_TX) {       /* Is flag "running int from TX" set? */
    OnFlags |= ON_TX_CHAR;             /* Set flag "OnTxChar" */
  }
  SerFlag &= ~RUNINT_FROM_TX;          /* Reset flag "running int from TX" */
  if(SCI_OutLen) {                    /* Is number of uint8_ts in the transmit buffer greater then 0? */
    SCI_OutLen--;                     /* Decrease number of chars in the transmit buffer */
    SerFlag |= RUNINT_FROM_TX;         /* Set flag "running int from TX"? */
    (void) SCI1SR1;                    /* Reset interrupt request flag */
    SCI1DRL = (uint8_t)*(OutPtrR++);      /* Store char to transmitter register */
    if(OutPtrR >= (OutBuffer + SCI_OUT_BUF_SIZE)) { /* Is the pointer out of the transmit buffer? */
      OutPtrR = OutBuffer;             /* Set pointer on the first item into the transmit buffer */
    }
  }
  else {
    OnFlags |= ON_FREE_TX;             /* Set flag "OnFreeTxBuf" */
    //SCI1CR2_TIE = 0;                   /* Disable transmit interrupt */
	SCI1CR2 &= ~SCICR2_TIE_MASK;
  }
  if(OnFlags & ON_TX_CHAR) {           /* Is flag "OnTxChar" set? */
    SCI1_OnTxChar();                   /* If yes then invoke user event */
  }
  if(OnFlags & ON_FREE_TX) {           /* Is flag "OnFreeTxBuf" set? */
    SCI1_OnFreeTxBuf();                /* If yes then invoke user event */
  }
}

/*
** ===================================================================
**     Method      :  SCI0_Interrupt (bean AsynchroSerial)
**
**     Description :
**         The method services the receive interrupt of the selected 
**         peripheral(s) and eventually invokes the bean's event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
INTERRUPT void SCI1_Interrupt(void)
{
  uint8_t StatReg = SCI1SR1;

  if (StatReg & (SCISR1_OR_MASK | SCISR1_FE_MASK | SCISR1_NF_MASK)) { /* Is any error flag set? */
    SerFlag |= COMMON_ERR;             /* If yes then set an internal flag */
    (void) SCI1DRL;                    /* Dummy read of data register - clear error bits */
    StatReg &= ~SCISR1_RDRF_MASK;     /* Clear the receive data flag to discard the errorneous data */
    SCI1_OnError();                    /* If yes then invoke user event */
  }
  if (StatReg & SCISR1_RDRF_MASK) {   /* Is the receiver interrupt flag set? */
    SCI1_InterruptRx();                /* If yes, then invoke the internal service routine. This routine is inlined. */
  }
  if (SCI1CR2_TIE) {                   /* Is the transmitter interrupt enabled? */
    if (StatReg & SCISR1_TDRE_MASK) { /* Is the transmitter interrupt flag set? */
      SCI1_InterruptTx();              /* If yes, then invoke the internal service routine. This routine is inlined. */
    }
  }
}


