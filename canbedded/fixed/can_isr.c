/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     can_isr.c 
 *
 * description:   can isr
 *
 *
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           can isr for S12XEP100
 *
 *****************************************************************************/

#include "can_inc.h"               /* dependend configuration of the driver.*/

#if defined(C_ENABLE_ISR_SPLIT)

# if !defined(C_ENABLE_RX_BASICCAN_POLLING)
void CanRxIrqHandler(CAN_CHANNEL_CANTYPE_ONLY);
# endif
# if !defined(C_ENABLE_TX_POLLING)
void CanTxIrqHandler(CAN_CHANNEL_CANTYPE_ONLY);
# endif
# if !defined(C_ENABLE_WAKEUP_POLLING)
#  if defined(C_ENABLE_SLEEP_WAKEUP)
void CanWakeUpIrqHandler(CAN_CHANNEL_CANTYPE_ONLY);
#  endif
# endif
# if !defined(C_ENABLE_ERROR_POLLING)
void CanErrorIrqHandler(CAN_CHANNEL_CANTYPE_ONLY);
# endif

# if defined(CAN_TX_IRQ_FUNC_DECL)
# else
#  if (defined(C_ENABLE_TX_POLLING)) && (!defined(C_ENABLE_INDIVIDUAL_POLLING))
#    define CAN_TX_IRQ_FUNC_DECL(Func)
#  else
#   if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#    define CAN_TX_IRQ_FUNC(Func)
#    define CAN_TX_IRQ_FUNC_DECL(Func)
#   else
#    define CAN_TX_IRQ_FUNC(Func)          CAN_IRQ_FUNC_QUAL1 void CAN_IRQ_FUNC_QUAL2 Func(void)
#    define CAN_TX_IRQ_FUNC_DECL(Func)     CAN_TX_IRQ_FUNC(Func);
#   endif
#  endif
# endif

# if defined(CAN_RX_IRQ_FUNC_DECL)
# else
#  if (defined(C_ENABLE_RX_BASICCAN_POLLING)) && (!defined(C_ENABLE_INDIVIDUAL_POLLING))
#    define CAN_RX_IRQ_FUNC_DECL(Func)
#  else
#   if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#    define CAN_RX_IRQ_FUNC(Func)
#    define CAN_RX_IRQ_FUNC_DECL(Func)
#   else
#    define CAN_RX_IRQ_FUNC(Func)          CAN_IRQ_FUNC_QUAL1 void CAN_IRQ_FUNC_QUAL2 Func(void)
#    define CAN_RX_IRQ_FUNC_DECL(Func)     CAN_RX_IRQ_FUNC(Func);
#   endif
#  endif
# endif

# if defined(CAN_ERROR_IRQ_FUNC_DECL)
# else
#  if defined(C_ENABLE_ERROR_POLLING)
#    define CAN_ERROR_IRQ_FUNC_DECL(Func)
#  else
#   if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#    define CAN_ERROR_IRQ_FUNC(Func)
#    define CAN_ERROR_IRQ_FUNC_DECL(Func)
#   else
#    define CAN_ERROR_IRQ_FUNC(Func)       CAN_IRQ_FUNC_QUAL1 void CAN_IRQ_FUNC_QUAL2 Func(void)
#    define CAN_ERROR_IRQ_FUNC_DECL(Func)  CAN_ERROR_IRQ_FUNC(Func);
#   endif
#  endif
# endif

# if defined(CAN_WAKEUP_IRQ_FUNC_DECL)
# else
#  if defined(C_ENABLE_WAKEUP_POLLING)
#   define CAN_WAKEUP_IRQ_FUNC_DECL(Func)
#   error "Wakeup Polling Is Not Allowed!"
#  else
#   if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#    define CAN_WAKEUP_IRQ_FUNC(Func)
#    define CAN_WAKEUP_IRQ_FUNC_DECL(Func)
#   else
#    define CAN_WAKEUP_IRQ_FUNC(Func)      CAN_IRQ_FUNC_QUAL1 void CAN_IRQ_FUNC_QUAL2 Func(void)
#    define CAN_WAKEUP_IRQ_FUNC_DECL(Func) CAN_WAKEUP_IRQ_FUNC(Func);
#   endif
#  endif
# endif

/****************************************************************************
| NAME:             CanIsr
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Interrupt service functions according to the CAN controller
|                   interrupt stucture
|                   - check for the interrupt reason ( interrupt source )
|                   - work appropriate interrupt:
|                     + status/error interrupt (busoff, wakeup, error warning)
|                     + basic can receive
|                     + full can receive
|                     + can transmit
|
|                   If an Rx-Interrupt occurs while the CAN controller is in Sleep mode, 
|                   a wakeup has to be generated. 
|
|                   If an Tx-Interrupt occurs while the CAN controller is in Sleep mode, 
|                   an assertion has to be called and the interrupt has to be ignored.
|
|                   The name of BrsTimeStrt...() and BrsTimeStop...() can be addapted to 
|                   realy used name of the interrupt functions.
|
|  ##Ht: description of actions for each kind of interrupt is missing
****************************************************************************/
# if ((defined( C_ENABLE_RX_BASICCAN_OBJECTS ) && !defined( C_ENABLE_RX_BASICCAN_POLLING )) || \
      !defined( C_ENABLE_TX_POLLING )        || \
       defined( C_ENABLE_INDIVIDUAL_POLLING ) || \
       !defined( C_ENABLE_ERROR_POLLING )     ||\
       !defined( C_ENABLE_WAKEUP_POLLING ))        /* ISR necessary; no pure polling configuration*/

#   if defined(kCanPhysToLogChannelIndex_0)


#    if (!defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS)) || (defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS))
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanRxInterrupt_0)
#     else
CAN_RX_IRQ_FUNC(CanRxInterrupt_0)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanRxIrqHandler();
#     else
  CanRxIrqHandler((CanChannelHandle)kCanPhysToLogChannelIndex_0);
#     endif
}
#    endif



#    if (!defined(C_ENABLE_TX_POLLING)) || (defined(C_ENABLE_TX_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING))
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanTxInterrupt_0)
#     else
CAN_TX_IRQ_FUNC(CanTxInterrupt_0)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanTxIrqHandler();
#     else
  CanChannelHandle channel;
  channel = (CanChannelHandle)kCanPhysToLogChannelIndex_0;
  CanTxIrqHandler(channel);
#     endif
}
#    endif



#    if defined(C_ENABLE_SLEEP_WAKEUP)
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanWakeUpInterrupt_0)
#     else
CAN_WAKEUP_IRQ_FUNC(CanWakeUpInterrupt_0)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanWakeUpIrqHandler();
#     else
  CanWakeUpIrqHandler((CanChannelHandle)kCanPhysToLogChannelIndex_0);
#     endif
}
#    endif



#    if !defined(C_ENABLE_ERROR_POLLING)
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanErrorInterrupt_0)
#     else
CAN_ERROR_IRQ_FUNC(CanErrorInterrupt_0)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanErrorIrqHandler();
#     else
  CanErrorIrqHandler((CanChannelHandle)kCanPhysToLogChannelIndex_0);
#     endif
}
#    endif


#   endif

#   if defined(kCanPhysToLogChannelIndex_1)


#    if (!defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS)) || (defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS))
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanRxInterrupt_1)
#     else
CAN_RX_IRQ_FUNC(CanRxInterrupt_1)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanRxIrqHandler();
#     else
  CanRxIrqHandler((CanChannelHandle)kCanPhysToLogChannelIndex_1);
#     endif
}
#    endif



#    if (!defined(C_ENABLE_TX_POLLING)) || (defined(C_ENABLE_TX_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING))
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanTxInterrupt_1)
#     else
CAN_TX_IRQ_FUNC(CanTxInterrupt_1)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanTxIrqHandler();
#     else
  CanChannelHandle channel;
  channel = (CanChannelHandle)kCanPhysToLogChannelIndex_1;
  CanTxIrqHandler(channel);
#     endif
}
#    endif



#    if defined(C_ENABLE_SLEEP_WAKEUP)
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanWakeUpInterrupt_1)
#     else
CAN_WAKEUP_IRQ_FUNC(CanWakeUpInterrupt_1)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanWakeUpIrqHandler();
#     else
  CanWakeUpIrqHandler((CanChannelHandle)kCanPhysToLogChannelIndex_1);
#     endif
}
#    endif



#    if !defined(C_ENABLE_ERROR_POLLING)
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanErrorInterrupt_1)
#     else
CAN_ERROR_IRQ_FUNC(CanErrorInterrupt_1)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanErrorIrqHandler();
#     else
  CanErrorIrqHandler((CanChannelHandle)kCanPhysToLogChannelIndex_1);
#     endif
}
#    endif


#   endif

#   if defined(kCanPhysToLogChannelIndex_2)


#    if (!defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS)) || (defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS))
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanRxInterrupt_2)
#     else
CAN_RX_IRQ_FUNC(CanRxInterrupt_2)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanRxIrqHandler();
#     else
  CanRxIrqHandler((CanChannelHandle)kCanPhysToLogChannelIndex_2);
#     endif
}
#    endif



#    if (!defined(C_ENABLE_TX_POLLING)) || (defined(C_ENABLE_TX_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING))
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanTxInterrupt_2)
#     else
CAN_TX_IRQ_FUNC(CanTxInterrupt_2)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanTxIrqHandler();
#     else
  CanChannelHandle channel;
  channel = (CanChannelHandle)kCanPhysToLogChannelIndex_2;
  CanTxIrqHandler(channel);
#     endif
}
#    endif



#    if defined(C_ENABLE_SLEEP_WAKEUP)
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanWakeUpInterrupt_2)
#     else
CAN_WAKEUP_IRQ_FUNC(CanWakeUpInterrupt_2)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanWakeUpIrqHandler();
#     else
  CanWakeUpIrqHandler((CanChannelHandle)kCanPhysToLogChannelIndex_2);
#     endif
}
#    endif



#    if !defined(C_ENABLE_ERROR_POLLING)
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanErrorInterrupt_2)
#     else
CAN_ERROR_IRQ_FUNC(CanErrorInterrupt_2)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanErrorIrqHandler();
#     else
  CanErrorIrqHandler((CanChannelHandle)kCanPhysToLogChannelIndex_2);
#     endif
}
#    endif


#   endif

#   if defined(kCanPhysToLogChannelIndex_3)


#    if (!defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS)) || (defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS))
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanRxInterrupt_3)
#     else
CAN_RX_IRQ_FUNC(CanRxInterrupt_3)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanRxIrqHandler();
#     else
  CanRxIrqHandler((CanChannelHandle)kCanPhysToLogChannelIndex_3);
#     endif
}
#    endif



#    if (!defined(C_ENABLE_TX_POLLING)) || (defined(C_ENABLE_TX_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING))
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanTxInterrupt_3)
#     else
CAN_TX_IRQ_FUNC(CanTxInterrupt_3)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanTxIrqHandler();
#     else
  CanChannelHandle channel;
  channel = (CanChannelHandle)kCanPhysToLogChannelIndex_3;
  CanTxIrqHandler(channel);
#     endif
}
#    endif



#    if defined(C_ENABLE_SLEEP_WAKEUP)
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanWakeUpInterrupt_3)
#     else
CAN_WAKEUP_IRQ_FUNC(CanWakeUpInterrupt_3)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanWakeUpIrqHandler();
#     else
  CanWakeUpIrqHandler((CanChannelHandle)kCanPhysToLogChannelIndex_3);
#     endif
}
#    endif



#    if !defined(C_ENABLE_ERROR_POLLING)
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanErrorInterrupt_3)
#     else
CAN_ERROR_IRQ_FUNC(CanErrorInterrupt_3)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanErrorIrqHandler();
#     else
  CanErrorIrqHandler((CanChannelHandle)kCanPhysToLogChannelIndex_3);
#     endif
}
#    endif


#   endif

#   if defined(kCanPhysToLogChannelIndex_4)


#    if (!defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS)) || (defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS))
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanRxInterrupt_4)
#     else
CAN_RX_IRQ_FUNC(CanRxInterrupt_4)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanRxIrqHandler();
#     else
  CanRxIrqHandler((CanChannelHandle)kCanPhysToLogChannelIndex_4);
#     endif
}
#    endif



#    if (!defined(C_ENABLE_TX_POLLING)) || (defined(C_ENABLE_TX_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING))
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanTxInterrupt_4)
#     else
CAN_TX_IRQ_FUNC(CanTxInterrupt_4)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanTxIrqHandler();
#     else
  CanChannelHandle channel;
  channel = (CanChannelHandle)kCanPhysToLogChannelIndex_4;
  CanTxIrqHandler(channel);
#     endif
}
#    endif



#    if defined(C_ENABLE_SLEEP_WAKEUP)
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanWakeUpInterrupt_4)
#     else
CAN_WAKEUP_IRQ_FUNC(CanWakeUpInterrupt_4)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanWakeUpIrqHandler();
#     else
  CanWakeUpIrqHandler((CanChannelHandle)kCanPhysToLogChannelIndex_4);
#     endif
}
#    endif



#    if !defined(C_ENABLE_ERROR_POLLING)
#     if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
ISR(CanErrorInterrupt_4)
#     else
CAN_ERROR_IRQ_FUNC(CanErrorInterrupt_4)
#     endif
{
#     if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanErrorIrqHandler();
#     else
  CanErrorIrqHandler((CanChannelHandle)kCanPhysToLogChannelIndex_4);
#     endif
}
#    endif


#   endif


# endif

#endif
/******   STOPSINGLE_OF_MULTIPLE    ******/
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
