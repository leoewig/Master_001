/** ###################################################################
**     Filename  : Events.H
**     Project   : Project
**     Processor : MC9S12XEP100MAL
**     Beantype  : Events
**     Version   : Driver 01.04
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 2015/4/2, 15:12
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         SM1_OnRxChar    - void SM1_OnRxChar(void);
**         SM1_OnRxCharExt - void SM1_OnRxCharExt(SM1_TComData Chr);
**         SM1_OnTxChar    - void SM1_OnTxChar(void);
**         SM1_OnFullRxBuf - void SM1_OnFullRxBuf(void);
**         SM1_OnFreeTxBuf - void SM1_OnFreeTxBuf(void);
**         SM1_OnError     - void SM1_OnError(void);
**
**     (c) Copyright UNIS, a.s. 1997-2008
**     UNIS, a.s.
**     Jundrovska 33
**     624 00 Brno
**     Czech Republic
**     http      : www.processorexpert.com
**     mail      : info@processorexpert.com
** ###################################################################*/

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Bit1.h"
#include "SM1.h"
#include "Bit2.h"
#include "VTBM_enable.h"


extern unsigned char rec_buf0[70];
extern unsigned char rec_buf1[70];
extern unsigned char rec_buf2[70];
extern unsigned char rec_buf3[70];
 
extern unsigned char rec_buf_tempa[70];
extern unsigned char rec_buf_tempb[70];
extern unsigned char rec_buf_tempc[70];
extern unsigned char rec_buf_tempd[70];
extern unsigned char rec_buf_tempe[70];
extern unsigned char rec_buf_tempf[70];

#pragma CODE_SEG DEFAULT

void SM1_OnFreeTxBuf(void);
/*
** ===================================================================
**     Event       :  SM1_OnFreeTxBuf (module Events)
**
**     From bean   :  SM1 [SynchroMaster]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**         This event is available only when the <Interrupt
**         service/event> property is enabled and the <Output buffer
**         size> property is set to non-zero value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SM1_OnError(void);
/*
** ===================================================================
**     Event       :  SM1_OnError (module Events)
**
**     From bean   :  SM1 [SynchroMaster]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/


void CAN1_OnFreeTxBuffer(word BufferMask);
/*
** ===================================================================
**     Event       :  CAN1_OnFreeTxBuffer (module Events)
**
**     From bean   :  CAN1 [FreescaleCAN]
**     Description :
**         This event is called after a successful transmission of a
**         message. The event is available only if Interrupt
**         service/event is enabled.
**     Parameters  :
**         NAME            - DESCRIPTION
**         BufferMask      - Transmit buffer mask. The
**                           mask can be used to check what message
**                           buffer caused the transmit interrupt.
**     Returns     : Nothing
** ===================================================================
*/

void CAN1_OnFullRxBuffer(void);
/*
** ===================================================================
**     Event       :  CAN1_OnFullRxBuffer (module Events)
**
**     From bean   :  CAN1 [FreescaleCAN]
**     Description :
**         This event is called when the receive buffer is full
**         after a successful reception of a message. The event is
**         available only if Interrupt service/event is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void CAN1_OnBusOff(void);
/*
** ===================================================================
**     Event       :  CAN1_OnBusOff (module Events)
**
**     From bean   :  CAN1 [FreescaleCAN]
**     Description :
**         This event is called when the node status becomes bus-off.
**         The event is available only if Interrupt service/event is
**         enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END Events */
#endif /* __Events_H*/

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 3.00 [04.12]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
