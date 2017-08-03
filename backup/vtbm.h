/** ###################################################################
**     Filename  : vtbm.H
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

#ifndef __vtbm_H
#define __vtbm_H
/* MODULE Events */

void vtbm_update(void);
void vtbm_init(void);
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



#endif /* __vtbm_H*/

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 3.00 [04.12]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
