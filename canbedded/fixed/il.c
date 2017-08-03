/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     il.c 
 *
 * description:   il
 *
 *
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           il for S12XEP100
 *
 *****************************************************************************/

/*lint -function(exit,ApplIlFatalError)*/

#define V_IL_VECTOR_COMPONENT_SOURCE

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "il_inc.h"

/**********************************************************************************************************************
  VERSION CHECK
**********************************************************************************************************************/
#if( IL_VECTOR_VERSION != 0x0502 )
# error "Source and Header file are inconsistent!"
#endif
#if( IL_VECTOR_RELEASE_VERSION != 0x01 )
# error "Source and Header file are inconsistent!"
#endif

/**********************************************************************************************************************
  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  ModifyChannelState
**********************************************************************************************************************/
/** \defgroup ModifyChannelState Modify Channel State
    \brief    These macros shall be used to modify ilChannelState.
    \param    channel      Handle of the CAN channel.
    \return   none */
/**\{*/
/** Set the rx state machine to running */
#define EnableRx(channel)    (Il_ChannelState((channel)) = (vuint8)(((vuint8)(Il_ChannelState((channel)) & kIlIsNotRxWait)) | kIlIsRxRun))
/** Set the tx state machine to running */
#define EnableTx(channel)    (Il_ChannelState((channel)) = (vuint8)(((vuint8)(Il_ChannelState((channel)) & kIlIsNotTxWait)) | kIlIsTxRun))
/** Set the rx state machine to waiting */
#define WaitRx(channel)      (Il_ChannelState((channel)) = (vuint8)(((vuint8)(Il_ChannelState((channel)) & kIlIsNotRxRun)) | kIlIsRxWait))
/** Set the tx state machine to waiting */
#define WaitTx(channel)      (Il_ChannelState((channel)) = (vuint8)(((vuint8)(Il_ChannelState((channel)) & kIlIsNotTxRun)) | kIlIsTxWait))
/** Set the rx state machine to suspended */
#define SuspendRx(channel)   (Il_ChannelState((channel)) = (vuint8)(((vuint8)(Il_ChannelState((channel)) & kIlIsNotRxRun)) & kIlIsNotRxWait))
/** Set the rx state machine to suspended */
#define SuspendTx(channel)   (Il_ChannelState((channel)) = (vuint8)(((vuint8)(Il_ChannelState((channel)) & kIlIsNotTxRun)) & kIlIsNotTxWait))
/**\}*/

/**********************************************************************************************************************
  DecodeChannelState
**********************************************************************************************************************/
/** \ingroup DecodeChannelState */
/**\{*/
/** Tx is running */
#define TxEnabled(channel)      ((Il_ChannelState((channel)) & kIlIsTxRun) != 0)
/** Rx is waiting */
#define RxWaiting(channel)      ((Il_ChannelState((channel)) & kIlIsRxWait) != 0)
/** Tx is waiting */
#define TxWaiting(channel)      ((Il_ChannelState((channel)) & kIlIsTxWait) != 0)
/** Tx is suspended */
#define TxSuspended(channel)    ((Il_ChannelState((channel)) & (kIlIsTxRun | kIlIsTxWait)) == 0)
/** Tx is not suspended */
#define TxNotSuspended(channel) ((Il_ChannelState((channel)) & (kIlIsTxRun | kIlIsTxWait)) != 0)
/** Rx is not suspended */
#define RxNotSuspended(channel) ((Il_ChannelState((channel)) & (kIlIsRxRun | kIlIsRxWait)) != 0)
/**\}*/

#if defined( C_ENABLE_GENERIC_PRECOPY )
/**********************************************************************************************************************
  ModifyChannelReceive
**********************************************************************************************************************/
/** \defgroup ModifyChannelReceive
    \brief    These macros shall be used to modify ilChannelReceive.
    \param    channel      Handle of the CAN channel.
    \return   IlGetChannelReceived : none\n
              IlGetChannelReceived : ilChannelReceive\n
              IlClrChannelReceived : none */
/**\{*/
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
#  define IlSetChannelReceived(channel)      (ilChannelReceive[(channel)] = 0x01)
#  define IlGetChannelReceived(channel)      (ilChannelReceive[(channel)])
#  define IlClrChannelReceived(channel)      (ilChannelReceive[(channel)] = 0x00)
# else
#  define IlSetChannelReceived(channel)      (ilChannelReceive =  0x01)
#  define IlGetChannelReceived(channel)      (ilChannelReceive)
#  define IlClrChannelReceived(channel)      (ilChannelReceive =  0   )
# endif
/**\}*/
#endif /* C_ENABLE_GENERIC_PRECOPY */

/**********************************************************************************************************************
  ClearStatusFlags
**********************************************************************************************************************/
/** \defgroup ClearStatusFlags Clear Status Flags
    \brief    These macros shall be used to clear status flags.
    \param    channel      Handle of the CAN channel.
    \return   none */
/**\{*/
#if defined IL_ENABLE_TX_TIMEOUT_FLAG
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
#  define ResetTxTimeoutFlags(channel)        (VStdMemNearClr(&ilTxTimeoutFlags[IlTxTimeoutFlagsStartIndex[(channel)]],(vuint8)(IlTxTimeoutFlagsStartIndex[(channel)+1]-IlTxTimeoutFlagsStartIndex[(channel)])))
# else
#  define ResetTxTimeoutFlags(channel)        (VStdMemNearClr(ilTxTimeoutFlags,(vuint8)kIlNumberOfTxTimeoutFlags))
# endif
#else
# define ResetTxTimeoutFlags(channel)
#endif

#if defined IL_ENABLE_TX_CONFIRMATION_FLAG
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
#  define ResetConfirmationFlags(channel)     (VStdMemNearClr(&ilTxConfirmationFlags[IlTxConfirmationFlagsStartIndex[(channel)]],(vuint8)(IlTxConfirmationFlagsStartIndex[(channel)+1]-IlTxConfirmationFlagsStartIndex[(channel)])))
# else
#  define ResetConfirmationFlags(channel)     (VStdMemNearClr(ilTxConfirmationFlags,(vuint8)kIlNumberOfTxConfirmationFlags))
# endif
#else
# define ResetConfirmationFlags(channel)
#endif

#if defined IL_ENABLE_RX_TIMEOUT_FLAG
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
#  define ResetRxTimeoutFlags(channel)        (VStdMemNearClr(&ilRxTimeoutFlags[IlRxTimeoutFlagsStartIndex[(channel)]],(vuint8)(IlRxTimeoutFlagsStartIndex[(channel)+1]-IlRxTimeoutFlagsStartIndex[(channel)])))
# else
#  define ResetRxTimeoutFlags(channel)        (VStdMemNearClr(ilRxTimeoutFlags,(vuint8)kIlNumberOfRxTimeoutFlags))
# endif
#else
# define ResetRxTimeoutFlags(channel)
#endif


#if defined( IL_ENABLE_RX_TIMEOUT )
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
#  define ResetRxTimerFlags(channel)          (VStdMemNearClr(&ilRxTimerFlags[IlRxTimerFlagsStartIndex[(channel)]],(vuint8)(IlRxTimerFlagsStartIndex[(channel)+1]-IlRxTimerFlagsStartIndex[(channel)])))
# else
#  define ResetRxTimerFlags(channel)          (VStdMemNearClr(ilRxTimerFlags,(vuint8)(kIlNumberOfTimerFlagBytes)))
# endif
#else
# define ResetRxTimerFlags(channel)
#endif


#if defined IL_ENABLE_RX_INDICATION_FLAG
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
#  define ResetIndicationFlags(channel)       (VStdMemNearClr(&ilRxIndicationFlags[IlRxIndicationFlagsStartIndex[(channel)]],(vuint8)(IlRxIndicationFlagsStartIndex[(channel)+1]-IlRxIndicationFlagsStartIndex[(channel)])))
# else
#  define ResetIndicationFlags(channel)       (VStdMemNearClr(ilRxIndicationFlags,(vuint8)kIlNumberOfRxIndicationFlags))
# endif
#else
# define ResetIndicationFlags(channel)
#endif

#if defined IL_ENABLE_RX_FIRSTVALUE_FLAG
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
#  define ResetFirstvalueFlags(channel)       (VStdMemNearClr(&ilRxFirstvalueFlags[IlRxFirstvalueFlagsStartIndex[(channel)]],(vuint8)(IlRxFirstvalueFlagsStartIndex[(channel)+1]-IlRxFirstvalueFlagsStartIndex[(channel)])))
# else
#  define ResetFirstvalueFlags(channel)       (VStdMemNearClr(ilRxFirstvalueFlags,(vuint8)kIlNumberOfRxFirstvalueFlags))
# endif
#else
# define ResetFirstvalueFlags(channel)
#endif

#if defined IL_ENABLE_RX_DATACHANGED_FLAG
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
#  define ResetDataChangedFlags(channel)      (VStdMemNearClr(&ilRxDataChangedFlags[IlRxDataChangedFlagsStartIndex[(channel)]],(vuint8)(IlRxDataChangedFlagsStartIndex[(channel)+1]-IlRxDataChangedFlagsStartIndex[(channel)])))
# else
#  define ResetDataChangedFlags(channel)      (VStdMemNearClr(ilRxDataChangedFlags,(vuint8)kIlNumberOfRxDataChangedFlags))
# endif
#else
# define ResetDataChangedFlags(channel)
#endif
/**\}*/

/**********************************************************************************************************************
  DataAbstraction
**********************************************************************************************************************/
/** \defgroup DataAbstraction Data Abstraction
    \brief    These macros shall be used to access tx and message buffers. */
/**\{*/

/**********************************************************************************************************************
  IlGetRxDataPtr
**********************************************************************************************************************/
/** \brief    This method returns the pointer to the buffer of a rx message.
    \param    ilRxHnd  Handle of an IL rx message.
    \return   vuint8*  Pointer to the databuffer.
**********************************************************************************************************************/
#if defined( IL_ENABLE_RX_MODE_SIGNALS ) || defined( IL_ENABLE_SYS_MULTI_ECU_PHYS ) 
# define IlGetRxDataPtr(ilRxHnd)                   (IlRxDataPtr[(ilRxHnd)])
#else
# define IlGetRxDataPtr(ilRxHnd)                   (CanGetRxDataPtr((ilRxHnd)))
#endif

/**********************************************************************************************************************
  IlGetRxDataLen
**********************************************************************************************************************/
/** \brief    This method returns the data length of a rx message.
    \param    ilRxHnd  Handle of an IL rx message.
    \return   vuint8   Length of the rx message in bytes.
**********************************************************************************************************************/
#if defined( IL_ENABLE_RX_MODE_SIGNALS ) || defined( IL_ENABLE_SYS_MULTI_ECU_PHYS ) 
# define IlGetRxDataLen(ilRxHnd)                   (IlRxDataLen[(ilRxHnd)])
#else
# define IlGetRxDataLen(ilRxHnd)                   (CanGetRxDataLen((ilRxHnd)))
#endif

/**********************************************************************************************************************
  IlGetTxDataPtr
**********************************************************************************************************************/
/** \brief    This method returns the pointer to the buffer of a tx message.
    \param    ilTxHnd  Handle of an IL tx message.
    \return   vuint8*  Pointer to the databuffer.
**********************************************************************************************************************/
#if defined( IL_ENABLE_TX_MODE_SIGNALS ) 
# define  IlGetTxDataPtr(ilTxHnd)                  (IlTxDataPtr[(ilTxHnd)])
#else
# define  IlGetTxDataPtr(ilTxHnd)                  (CanGetTxDataPtr(IlGetIlTxIndirection((ilTxHnd))))
#endif

/**********************************************************************************************************************
  IlGetTxDlc
**********************************************************************************************************************/
/** \brief    This method returns the data length code of a tx message.
    \param    ilTxHnd  Handle of an IL tx message.
    \return   vuint8   Dlc of the tx message in bytes.
**********************************************************************************************************************/
#if defined( IL_ENABLE_TX_MODE_SIGNALS ) 
# define  IlGetTxDlc(ilTxHnd)                      (IlTxDLC[(ilTxHnd)])
#else
# define  IlGetTxDlc(ilTxHnd)                      (XT_TX_DLC(CanGetTxDlc(IlGetIlTxIndirection((ilTxHnd)))))
#endif
/**\}*/

/**********************************************************************************************************************
  IlGetIlTxIndirection
**********************************************************************************************************************/
/** \brief    This method returns the Can Driver Handle of a IL tx message.
    \param    ilTxHnd  Handle of an IL tx message.
    \return   CanTransmitHandle  Can Driver tx message handle.
**********************************************************************************************************************/
#if defined( IL_ENABLE_SYS_MULTI_ECU_PHYS )
# define IlGetIlTxIndirection(ilTxHnd)             (IlTxIndirection[V_ACTIVE_IDENTITY_LOG][(ilTxHnd)])
#else
# define IlGetIlTxIndirection(ilTxHnd)             (IlTxIndirection[(ilTxHnd)])
#endif

/**********************************************************************************************************************
  IlGetStartRxTimeoutCounter
**********************************************************************************************************************/
/** \brief    This method returns the number of IlRxTask calls in a channel for the rx timeout time.
    \param    ilRxHnd  Handle of an IL rx message.
    \return   IltRxTimeoutCounter  Number of rx ticks until the rx timeout expires.
**********************************************************************************************************************/
#if defined ( VGEN_GENY ) && defined( IL_ENABLE_RX_DYNAMIC_TIMEOUT )
# define IlGetStartRxTimeoutCounter(ilRxHnd)     (ilRxDynamicTimeoutCounter[(ilRxHnd)])
#else
# if defined( IL_ENABLE_SYS_MULTI_ECU_PHYS )
#  define IlGetStartRxTimeoutCounter(ilRxHnd)    (IlRxTimeoutTbl[V_ACTIVE_IDENTITY_LOG][(ilRxHnd)])
# else
#  define IlGetStartRxTimeoutCounter(ilRxHnd)    (IlRxTimeoutTbl[(ilRxHnd)])
# endif
#endif

/**********************************************************************************************************************
  IlGetInitRxTimeoutCounter
**********************************************************************************************************************/
/** \brief    This method returns the number of IlRxTask calls in a channel for the initial rx timeout time.
    \param    ilRxHnd      Handle of an IL rx message.
    \return   IltRxTimeoutCounter  Number of rx ticks until the initial timeout expires.
**********************************************************************************************************************/
#if defined( IL_ENABLE_SYS_MULTI_ECU_PHYS )
# define IlGetInitRxTimeoutCounter(ilRxHnd)    (IlRxTimeoutTbl[V_ACTIVE_IDENTITY_LOG][(ilRxHnd)])
#else
# define IlGetInitRxTimeoutCounter(ilRxHnd)    (IlRxTimeoutTbl[(ilRxHnd)])
#endif

#if defined ( VGEN_GENY )
/**********************************************************************************************************************
  IlGetTxTimeout
**********************************************************************************************************************/
/** \brief    This method returns the number of IlTxTask calls in a channel for the tx timeout time.
    \param    channel      Handle of the CAN channel.
    \return   IltTxTimeoutCounter  Number of tx ticks until the tx timeout expires.
**********************************************************************************************************************/
# if defined( IL_ENABLE_TX ) && defined( IL_ENABLE_TX_TIMEOUT ) && defined ( IL_ENABLE_TX_VARYING_TIMEOUT )
#  define IlGetTxTimeout(channel)              (IlTxTimeout[channel])
# else
#  define IlGetTxTimeout(channel)              (kIlTxTimeout)
# endif
#endif /* VGEN_GENY */

/**********************************************************************************************************************
  IlGetTxCyclicCycles
**********************************************************************************************************************/
/** \brief    This method returns the number of IlTxTask calls for the cyclic transmission of a tx message.
    \param    ilTxHnd      Handle of an IL tx message.
    \return   IltTxCounter  Number of tx ticks until the next transmission request shall be performed.
**********************************************************************************************************************/
#if defined( IL_ENABLE_TX ) && defined( IL_ENABLE_TX_DYNAMIC_CYCLETIME )
# define  IlGetTxCyclicCycles(ilTxHnd)         (ilTxDynCyclicCycles[(ilTxHnd)])
#else
# define  IlGetTxCyclicCycles(ilTxHnd)         (IlTxCyclicCycles[(ilTxHnd)])
#endif

/**********************************************************************************************************************
  IlAssert
**********************************************************************************************************************/
/** \brief    This macro has to be used, if ApplIlFatalError needs to be called.
    \param    ilCondition      A boolean expression.
    \param    ilErrorCode      The error number to be submitted.
    \return   none
**********************************************************************************************************************/
#if defined( IL_ENABLE_SYS_TESTDEBUG )
# define IlAssert(ilCondition, ilErrorCode)        if ((ilCondition)==0) {(ApplIlFatalError((ilErrorCode)));}
#else
# define IlAssert(ilCondition, ilErrorCode)
#endif


/**********************************************************************************************************************
  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/


/**********************************************************************************************************************
  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/
#if defined( IL_ENABLE_RX_MULTIPLE_NODES )
/** This variable contains the ECU-mask for Multiple ECUs. The variable is replaced in GENy by V_ACTIVE_IDENTITY_MSK. */
V_MEMRAM0 static V_MEMRAM1 vuintx V_MEMRAM2 ilECUMask;
#endif

#if defined ( IL_ENABLE_RX )
# if defined( C_ENABLE_GENERIC_PRECOPY )
#  if defined( IL_ENABLE_RX_POLLING ) 
/** This variable indicates data reception of a channel */
#   if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 ilChannelReceive[kIlNumberOfChannels];
#   else
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 ilChannelReceive;
#   endif
#  endif /* IL_ENABLE_RX_POLLING || IL_ENABLE_RX_EXT_TIMEOUT */
# endif /* C_ENABLE_GENERIC_PRECOPY */
#endif /* IL_ENABLE_RX */

#if defined ( IL_ENABLE_TX )
/* variable definition for the send type CYCLIC-EVENT ***********************/
# if defined ( IL_ENABLE_TX_CYCLIC_EVENT ) || \
     defined ( IL_ENABLE_TX_SECURE_EVENT ) || \
     defined ( IL_ENABLE_TX_FAST_ON_START )
/** This variable contains the fast cycle time counter for every message. */
V_MEMRAM0 static V_MEMRAM1 IltTxCounter V_MEMRAM2 ilTxEventCounter[kIlNumberOfTxObjects];
# endif
/** This variable contains the standard cycle time and startdelaytime counter for every message. */
V_MEMRAM0 static V_MEMRAM1 IltTxCounter V_MEMRAM2 ilTxCyclicCounter[kIlNumberOfTxObjects];

# if defined( VGEN_GENY ) && defined( IL_ENABLE_TX_DYNAMIC_CYCLETIME )
/** This variable contains the cycle time for every message, if the dynamic cycle time API is used. */
V_MEMRAM0 static V_MEMRAM1 IltTxCounter V_MEMRAM2 ilTxDynCyclicCycles[kIlNumberOfTxObjects];
# endif
#endif /* IL_ENABLE_TX */


/**********************************************************************************************************************
  GLOBAL DATA
**********************************************************************************************************************/
#if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 ilChannelState[kIlNumberOfChannels];
#else
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 ilChannelState;
#endif

#if defined ( IL_ENABLE_RX )


# if defined( IL_ENABLE_RX_TIMEOUT )
V_MEMRAM0 V_MEMRAM1 IltRxTimeoutCounter V_MEMRAM2 ilRxTimeoutCounter[kIlNumberOfRxTimeoutCounters];
#  if defined( VGEN_GENY ) && defined( IL_ENABLE_RX_DYNAMIC_TIMEOUT )
V_MEMRAM0 V_MEMRAM1 IltRxTimeoutCounter V_MEMRAM2 ilRxDynamicTimeoutCounter[kIlNumberOfRxTimeoutCounters];
#  endif

V_MEMRAM0 V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR ilRxTimerFlags[kIlNumberOfTimerFlagBytes];

# endif /* IL_ENABLE_RX_TIMEOUT */

# if defined ( IL_ENABLE_RX_INDICATION_FLAG )
V_MEMRAM0 V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR ilRxIndicationFlags[kIlNumberOfRxIndicationFlags];

# endif

# if defined ( IL_ENABLE_RX_FIRSTVALUE_FLAG )
V_MEMRAM0 V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR ilRxFirstvalueFlags[kIlNumberOfRxFirstvalueFlags];

# endif

# if defined ( IL_ENABLE_RX_TIMEOUT_FLAG )
V_MEMRAM0 V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR ilRxTimeoutFlags[kIlNumberOfRxTimeoutFlags];

# endif

# if defined ( IL_ENABLE_RX_DATACHANGED_FLAG )
V_MEMRAM0 V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR ilRxDataChangedFlags[kIlNumberOfRxDataChangedFlags];
# endif

# if !defined (VGEN_GENY) && defined( IL_ENABLE_RX_MODE_SIGNALS )
# endif

#endif /* IL_ENABLE_RX */

#if defined ( IL_ENABLE_TX )

V_MEMRAM0 V_MEMRAM1_NEAR volatile vuint8 V_MEMRAM2_NEAR ilTxState[kIlNumberOfTxObjects];

V_MEMRAM0 V_MEMRAM1 IltTxUpdateCounter V_MEMRAM2 ilTxUpdateCounter[kIlNumberOfTxObjects];

# if defined ( IL_ENABLE_TX_TIMEOUT )
V_MEMRAM0 V_MEMRAM1 IltTxTimeoutCounter V_MEMRAM2 ilTxTimeoutCounter[kIlNumberOfTxTimeoutCounters];
# endif
# if defined( IL_ENABLE_TX_SECURE_EVENT )
V_MEMRAM0 V_MEMRAM1 IltTxRepetitionCounter V_MEMRAM2 ilTxNSendCounter[kIlNumberOfTxObjects];
# endif


# if defined ( IL_ENABLE_TX_TIMEOUT_FLAG )

V_MEMRAM0 V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR ilTxTimeoutFlags[kIlNumberOfTxTimeoutFlags];
# endif

# if defined ( IL_ENABLE_TX_CONFIRMATION_FLAG )

V_MEMRAM0 V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR ilTxConfirmationFlags[kIlNumberOfTxConfirmationFlags];
# endif
#endif /* IL_ENABLE_TX */


V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kIlMainVersion = (vuint8) (IL_VECTOR_VERSION >> 8);
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kIlSubVersion = (vuint8) (IL_VECTOR_VERSION & 0x00ff);
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kIlReleaseVersion = (vuint8) (IL_VECTOR_RELEASE_VERSION);


/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#if defined( IL_ENABLE_TX )
/**********************************************************************************************************************
  IlSendMessage
**********************************************************************************************************************/
/** \brief    This method performs the transmission request to the CAN Driver.
    \param    ilTxHnd  Handle of the tx message.
    \return   none
    \context  The function is called by IlTxTimerTask or IlSendDirect.
**********************************************************************************************************************/
static void IlSendMessage(IlTransmitHandle ilTxHnd);
#endif

#if defined( IL_ENABLE_RX_STOP_DEFAULTVALUE ) || defined( IL_ENABLE_RX_START_DEFAULTVALUE )
/**********************************************************************************************************************
  SetRxDefaultWithMask
**********************************************************************************************************************/
/** \brief    This method copies default values to the rx message buffer.
    \param    ilRxHnd  Handle of the rx message.
    \param    msk      pointer to a mask array of the rx message. At every position, where the mask bit is set,
                       the default value is set.
    \return   none
    \context  The function is called by IlRxStart or IlRxStop.
**********************************************************************************************************************/
static void SetRxDefaultWithMask(IlReceiveHandle ilRxHnd, V_MEMROM1 vuint8 V_MEMROM2 V_MEMROM3 * msk);
#endif

#if defined( IL_ENABLE_TX_STOP_DEFAULTVALUE ) || defined( IL_ENABLE_TX_START_DEFAULTVALUE )
/**********************************************************************************************************************
  SetTxDefaultWithMask
**********************************************************************************************************************/
/** \brief    This method copies default values to the tx message buffer.
    \param    ilTxHnd  Handle of the tx message.
    \param    msk      pointer to a mask array of the tx message. At every position, where the mask bit is set,
                       the default value is set.
    \return   none
    \context  The function is called by IlTxStart or IlTxStop.
**********************************************************************************************************************/
static void SetTxDefaultWithMask(IlTransmitHandle ilTxHnd, IL_MEMROM1 vuint8 IL_MEMROM2 IL_MEMROM3 * msk);
#endif


/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/
#if defined( IL_ENABLE_RX_STOP_DEFAULTVALUE ) || defined( IL_ENABLE_RX_START_DEFAULTVALUE )
/**********************************************************************************************************************
  SetRxDefaultWithMask
**********************************************************************************************************************/
static void SetRxDefaultWithMask(IlReceiveHandle ilRxHnd, V_MEMROM1 vuint8 V_MEMROM2 V_MEMROM3 * msk)
{
  vuint8 *dst = IlGetRxDataPtr(ilRxHnd);
  vuint8 len = IlGetRxDataLen(ilRxHnd);
  V_MEMROM1 vuint8 V_MEMROM2 V_MEMROM3 *src = IlRxDefaultInitValue[ilRxHnd];

  if (dst == V_NULL)
  {
    return;
  }

#  if defined IL_ENABLE_SYS_ARGCHECK

  if (len > 8)
  {
    return;
  }
#  endif

  while (len > 0)
  {
    len--;
    *dst = (*dst & (vuint8) ~ (*msk)) | ((*src) & (*msk));
    dst++;
    src++;
    msk++;
  }
}                               /* End of SetRxDefaultWithMask() */
#endif

#if defined( IL_ENABLE_TX_STOP_DEFAULTVALUE ) || defined( IL_ENABLE_TX_START_DEFAULTVALUE )
/**********************************************************************************************************************
  SetTxDefaultWithMask
**********************************************************************************************************************/
static void SetTxDefaultWithMask(IlTransmitHandle ilTxHnd, IL_MEMROM1 vuint8 IL_MEMROM2 IL_MEMROM3 * msk)
{
  vuint8 *dst = IlGetTxDataPtr(ilTxHnd);
  vuint8 len = IlGetTxDlc(ilTxHnd);
  IL_MEMROM1 vuint8 IL_MEMROM2 IL_MEMROM3 *src = IlTxDefaultInitValue[ilTxHnd];

  if (dst == V_NULL)
  {
    return;
  }
#  if defined IL_ENABLE_SYS_ARGCHECK

  if (len > 8)
  {
    return;
  }
#  endif

  while (len > 0)
  {
    len--;
    *dst = (*dst & (vuint8) ~ (*msk)) | ((*src) & (*msk));
    dst++;
    src++;
    msk++;
  }
}                               /* End of SetTxDefaultWithMask() */
#endif

#if defined ( IL_ENABLE_TX )
/**********************************************************************************************************************
  IlSendMessage
**********************************************************************************************************************/
static void IlSendMessage(IlTransmitHandle ilTxHnd)
{

  CanInterruptDisable();
  ilTxState[ilTxHnd] &= kTxNotSendRequest;

# if defined ( IL_ENABLE_TX_MODE_SIGNALS )
  /* copy mode depended signals to message buffer */
  if (IlTxModeCopyFunctions[ilTxHnd] != V_NULL)
  {
    IlTxModeCopyFunctions[ilTxHnd] ();
  }
# endif

  /* Counter for Update time must be reloaded twice, ------------ */
  /* here and in confirmation fct     --------------------------- */
# if defined ( C_ENABLE_CAN_CANCEL_NOTIFICATION )
  ilTxUpdateCounter[ilTxHnd] = kIlStopUpdateCounterValue;
# else
  ilTxUpdateCounter[ilTxHnd] = IlTxUpdateCycles[ilTxHnd];
# endif
  CanInterruptRestore();

  if (CanTransmit(IlGetIlTxIndirection(ilTxHnd)) != kCanTxOk)
  {
    CanInterruptDisable();
    {
      ilTxState[ilTxHnd] |= (kTxSendRequest);
    }
    ilTxUpdateCounter[ilTxHnd] = 0;
    CanInterruptRestore();
  }
# if defined( IL_ENABLE_TX_SECURE_EVENT )
  else
  {
    CanInterruptDisable();
    /* clear send request flag  ----------------------------------- */
    if ((ilTxNSendCounter[ilTxHnd] != 0) && ((ilTxState[ilTxHnd] & kTxNSendRequest) != 0))
    {
      ilTxNSendCounter[ilTxHnd]--;
    }
    ilTxState[ilTxHnd] &= kTxNotNSendRequest;
    CanInterruptRestore();
  }
# endif

}                               /* End of IlSendMessage() */
#endif /* IL_ENABLE_TX */


/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/
/**********************************************************************************************************************
  IlInitPowerOn
**********************************************************************************************************************/
void IlInitPowerOn(void)
{
#if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
  CanChannelHandle channel;
  for (channel = 0; channel < kIlNumberOfChannels; channel++)
#endif
  {
    IlInit(IL_CHANNEL_ILPARA_ONLY);
  }
}                               /* End of IlInitPowerOn() */

/**********************************************************************************************************************
  IlInit
**********************************************************************************************************************/
void IlInit(IL_CHANNEL_ILTYPE_ONLY)
{

#if defined ( IL_ENABLE_RX )
  IlReceiveHandle ilRxHnd;
#endif
#if defined ( IL_ENABLE_TX )
  IlTransmitHandle ilTxHnd;
#endif

  /* Reset internal state of IL    */
  Il_ChannelState(IL_CHANNEL_ILPARA_MACRO) = kIlIsSuspend;


#if defined( IL_ENABLE_RX_MULTIPLE_NODES )
  /* Setup the ECU mask for multiple ECUs */
  ilECUMask = (vuintx) (0x01 << V_ACTIVE_IDENTITY_LOG);
#endif


#if defined ( IL_ENABLE_RX )

  /***********************************************************
   **  Initialize receive data
   ***********************************************************/


   /*--- Loop for every receive object ---------------------------------------*/
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
  for (ilRxHnd = IlRxMsgStartIndex[channel]; ilRxHnd < IlRxMsgStartIndex[(channel + 1)]; ilRxHnd++)
# else
  for (ilRxHnd = 0; ilRxHnd < kIlNumberOfRxObjects; ilRxHnd++)
# endif
  {
    IlAssert(ilRxHnd < kIlNumberOfRxObjects, ILERR_ILLRXMSGHANDLE);

    if ((IlGetRxDataPtr(ilRxHnd)) != V_NULL)
    {
#  if defined IL_ENABLE_RX_DEFAULTVALUE
      if ((IlRxDefaultInitValue[ilRxHnd]) != V_NULL)
      {
        /*-- Set receive buffer to default values ---------------------------*/
        VStdMemCpyRomToRam(IlGetRxDataPtr(ilRxHnd), IlRxDefaultInitValue[ilRxHnd], IlGetRxDataLen(ilRxHnd));
      }
      else
#  endif /* IL_ENABLE_RX_DEFAULTVALUE */
      {
        /*-- No default value for this buffer. Set to '0' -------------------*/
        VStdMemClr(IlGetRxDataPtr(ilRxHnd), IlGetRxDataLen(ilRxHnd));
      }
    }
  }

   /*--- initialize dynamic Rx timeout objects ---------------------------------------*/
# if defined ( VGEN_GENY ) && defined( IL_ENABLE_RX_TIMEOUT ) && defined( IL_ENABLE_RX_DYNAMIC_TIMEOUT )
#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
  for (ilRxHnd = IlRxTimeoutCntStartIndex[channel]; ilRxHnd < IlRxTimeoutCntStartIndex[(channel + 1)]; ilRxHnd++)
#  else
  for (ilRxHnd = 0; ilRxHnd < kIlNumberOfRxTimeoutCounters; ilRxHnd++)
#  endif
  {
    IlAssert(ilRxHnd < kIlNumberOfRxTimeoutCounters, ILERR_ILLRXTIMEOUTINDEX);
    ilRxDynamicTimeoutCounter[ilRxHnd] = IlGetInitRxTimeoutCounter(ilRxHnd);
  }
# endif /* VGEN_GENY && IL_ENABLE_RX_TIMEOUT && IL_ENABLE_RX_DYNAMIC_TIMEOUT */

   /*---- Reset all flags for receive indication -----------------------------*/
  IlResetRxTimeoutFlags(IL_CHANNEL_ILPARA_ONLY);

  ResetRxTimerFlags(IL_CHANNEL_ILPARA_MACRO);
  ResetIndicationFlags(IL_CHANNEL_ILPARA_MACRO);
  ResetFirstvalueFlags(IL_CHANNEL_ILPARA_MACRO);
  ResetDataChangedFlags(IL_CHANNEL_ILPARA_MACRO);


#endif /* IL_ENABLE_RX */


#if defined ( IL_ENABLE_TX )
  /***********************************************************
   **  Initialize transmit data
   ***********************************************************/


  /*----- do the loop for every Tx object  -------------------------------*/
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
  for (ilTxHnd = IlTxMsgStartIndex[channel]; ilTxHnd < IlTxMsgStartIndex[(channel + 1)]; ilTxHnd++)
# else
  for (ilTxHnd = 0; ilTxHnd < kIlNumberOfTxObjects; ilTxHnd++)
# endif
  {
    IlAssert(ilTxHnd < kIlNumberOfTxObjects, ILERR_ILLTXMSGHANDLE);

# if defined( IL_ENABLE_SYS_MULTI_ECU_PHYS )
    if ((IlTxIdentityAssignment[ilTxHnd] & V_ACTIVE_IDENTITY_MSK) == (tVIdentityMsk) 0)
    {
      continue;
    }
# endif

    /************************************************************************/
    /**** Section: Clear Send-Type independent data *************************/
    /************************************************************************/
    if ((IlGetTxDataPtr(ilTxHnd)) != V_NULL)
    {
#  if defined IL_ENABLE_TX_DEFAULTVALUE
      if (IlTxDefaultInitValue[ilTxHnd] != V_NULL)
      {
        /*-- Set receive buffer to default values ---------------------------*/
        VStdMemCpyRomToRam(IlGetTxDataPtr(ilTxHnd), IlTxDefaultInitValue[ilTxHnd], IlGetTxDlc(ilTxHnd));
      }
      else
#  endif /* IL_ENABLE_TX_DEFAULTVALUE */
      {
        /*-- No default value for this buffer. Set to '0' -------------------*/
        /*** Clear message-transmit data-buffer *********************************/
        VStdMemClr(IlGetTxDataPtr(ilTxHnd), IlGetTxDlc(ilTxHnd));
      }
    }

    /*- Clear Tx-status variable. Message is inactive, no sendtype defined -*/
    ilTxState[ilTxHnd] = 0;

    /*--- Clear counter for update time ------------------------------------*/
    ilTxUpdateCounter[ilTxHnd] = 0; /*    */


    /************************************************************************/
    /**** Section: Clear data buffer for CYCLIC messages ********************/
    /************************************************************************/

    /* initialise the Dynamic cycle time counters with the initial defined rom values */
# if defined( VGEN_GENY ) && defined( IL_ENABLE_TX_DYNAMIC_CYCLETIME )
    ilTxDynCyclicCycles[ilTxHnd] = IlTxCyclicCycles[ilTxHnd];
# endif

    /*-- Counter for the cycletime -----------------------------------------*/
    ilTxCyclicCounter[ilTxHnd] = 0;


# if defined ( IL_ENABLE_TX_CYCLIC_EVENT ) || \
     defined ( IL_ENABLE_TX_SECURE_EVENT ) || \
     defined ( IL_ENABLE_TX_FAST_ON_START )
    /************************************************************************/
    /**** Section: Clear data buffer for CYCLIC- and SECURE-EVENT messages **/
    /************************************************************************/
    ilTxEventCounter[ilTxHnd] = 0;  /* Clear counter for event timer    */
# endif

  }

  /*---- Reset all flags for transmit path ----------------------------------*/
  ResetTxTimeoutFlags(IL_CHANNEL_ILPARA_MACRO);
  ResetConfirmationFlags(IL_CHANNEL_ILPARA_MACRO);


#endif /* IL_ENABLE_TX */


#if defined(IL_ENABLE_SYS_INIT_FCT)
  ApplIlInit(IL_CHANNEL_ILPARA_ONLY);
#endif
#if defined( IL_ENABLE_SYS_SIGNAL_INIT_FCT )
  IlSignalInit(IL_CHANNEL_ILPARA_ONLY);
#endif
}                               /* End of IlInit() */

/**********************************************************************************************************************
  IlRxStart
**********************************************************************************************************************/
void IlRxStart(IL_CHANNEL_ILTYPE_ONLY)
{
#if defined ( IL_ENABLE_RX )

# if defined ( IL_ENABLE_RX_START_DEFAULTVALUE )
  IlReceiveHandle ilRxHnd;
# endif
# if defined ( IL_ENABLE_RX_TIMEOUT )
  IlRxTimeoutHandle ilRxToHnd;
# endif

  /*** Receive already enabled. Do nothing! **********************************/
  if (RxEnabled(IL_CHANNEL_ILPARA_MACRO))
  {
    return;
  }

# if defined ( IL_ENABLE_RX_START_DEFAULTVALUE )
  /*--- Loop for every receive object ---------------------------------------*/
#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
  for (ilRxHnd = IlRxMsgStartIndex[channel]; ilRxHnd < IlRxMsgStartIndex[(channel + 1)]; ilRxHnd++)
#  else
  for (ilRxHnd = 0; ilRxHnd < kIlNumberOfRxObjects; ilRxHnd++)
#  endif
  {
    IlAssert(ilRxHnd < kIlNumberOfRxObjects, ILERR_ILLRXMSGHANDLE);
    /*-- Set receive buffer to default for some signals, if selected -------*/
    if ((IlRxDefaultInitValue[ilRxHnd] != V_NULL) && (IlRxDefaultStartMask[ilRxHnd] != V_NULL))
    {
      SetRxDefaultWithMask(ilRxHnd, IlRxDefaultStartMask[ilRxHnd]);
    }
  }
# endif /* IL_ENABLE_RX_START_DEFAULTVALUE */

# if defined( IL_ENABLE_RX_TIMEOUT )
#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
  for (ilRxToHnd = IlRxTimeoutCntStartIndex[channel]; ilRxToHnd < IlRxTimeoutCntStartIndex[(channel + 1)]; ilRxToHnd++)
#  else
  for (ilRxToHnd = 0; ilRxToHnd < kIlNumberOfRxTimeoutCounters; ilRxToHnd++)
#  endif
  {
    IlAssert(ilRxToHnd < kIlNumberOfRxTimeoutCounters, ILERR_ILLRXTIMEOUTINDEX);

    /*--- Reset timeout counter to its initial state --------------------*/
#  if defined ( IL_ENABLE_RX_TIMEOUT_DELAY )
    /* Timeout observation starts with first receive of a message */
    ilRxTimeoutCounter[ilRxToHnd] = 0;
#  else
    ilRxTimeoutCounter[ilRxToHnd] = IlGetStartRxTimeoutCounter(ilRxToHnd);
#  endif
  }
# endif /* IL_ENABLE_RX_TIMEOUT */

  /*---- Reset indication flags of the CAN driver ---------------------------*/
# if defined( IL_ENABLE_RX ) && defined( IL_ENABLE_RX_POLLING )
  IlResetCanIndicationFlags(IL_CHANNEL_ILPARA_ONLY);
# endif
# if !defined( VGEN_GENY ) && defined( IL_ENABLE_RX_MODE_SIGNALS )
# endif
  /*---- Reset all flags for reception path ---------------------------------*/
  IlResetRxTimeoutFlags(IL_CHANNEL_ILPARA_ONLY);

  ResetRxTimerFlags(IL_CHANNEL_ILPARA_MACRO);
  ResetIndicationFlags(IL_CHANNEL_ILPARA_MACRO);
  ResetFirstvalueFlags(IL_CHANNEL_ILPARA_MACRO);
  ResetDataChangedFlags(IL_CHANNEL_ILPARA_MACRO);


   /*---- call user defined function on start of reception path of IAL -------*/
# if defined IL_ENABLE_SYS_RX_START_FCT
  ApplIlRxStart(IL_CHANNEL_ILPARA_ONLY);
# endif
# if defined( IL_ENABLE_RX_SIGNAL_START_FCT )
  IlSignalRxStart(IL_CHANNEL_ILPARA_ONLY);
# endif

  EnableRx(IL_CHANNEL_ILPARA_MACRO);

#else /* IL_ENABLE_RX */
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1) && defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;            /* to avoid compiler warnings */
# endif
#endif /* IL_ENABLE_RX */
}                               /* End of IlRxStart() */

/**********************************************************************************************************************
  IlTxStart
**********************************************************************************************************************/
void IlTxStart(IL_CHANNEL_ILTYPE_ONLY)
{
#if defined ( IL_ENABLE_TX )
  IlTransmitHandle ilTxHnd;

  /*** Transmit already enabled. Do nothing! *********************************/
  if (TxEnabled(IL_CHANNEL_ILPARA_MACRO))
  {
    return;
  }

  /* load startup times into the counter */
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
  for (ilTxHnd = IlTxMsgStartIndex[channel]; ilTxHnd < IlTxMsgStartIndex[(channel + 1)]; ilTxHnd++)
# else
  for (ilTxHnd = 0; ilTxHnd < kIlNumberOfTxObjects; ilTxHnd++)
# endif
  {
    IlAssert(ilTxHnd < kIlNumberOfTxObjects, ILERR_ILLTXMSGHANDLE);

# if defined IL_ENABLE_TX_START_DEFAULTVALUE
    /*-- Set transmit buffer to default for some signals, if selected -------*/
    if ((IlTxDefaultInitValue[ilTxHnd] != V_NULL) && (IlTxDefaultStartMask[ilTxHnd] != V_NULL)
#  if defined( IL_ENABLE_SYS_MULTI_ECU_PHYS )
        && ((IlTxIdentityAssignment[ilTxHnd] & V_ACTIVE_IDENTITY_MSK) != (tVIdentityMsk) 0)
#  endif
      )
    {
      SetTxDefaultWithMask(ilTxHnd, IlTxDefaultStartMask[ilTxHnd]);
    }
# endif

    /* copy default status from ROM to RAM for later change ---------------- */
    ilTxState[ilTxHnd] = IlTxType[ilTxHnd];

    /*--- Clear counter for update time ------------------------------------*/
    ilTxUpdateCounter[ilTxHnd] = 0; /*    */

# if defined( IL_ENABLE_TX_SECURE_EVENT )
    ilTxNSendCounter[ilTxHnd] = 0;
# endif

# if  defined ( IL_ENABLE_TX_FAST_ON_START )
    if ((IlTxType[ilTxHnd] & kTxFastOnStart) != 0)
    {
      /*--- Set the Time for fast transmission after Start of the CYCLIC messages --------------------*/
      ilTxCyclicCounter[ilTxHnd] = IlTxFastOnStartDuration[ilTxHnd];    /* + 1 already done in the table */

      ilTxEventCounter[ilTxHnd] = IlTxStartCycles[ilTxHnd];     /* set start delay time for event timer *//* + 1 already done in the table */
    }
    else
# endif
    {
      /*--- Set the StartDelayTime of the CYCLIC messages --------------------*/
      ilTxCyclicCounter[ilTxHnd] = IlTxStartCycles[ilTxHnd];    /* + 1 already done in the table */

# if defined ( IL_ENABLE_TX_CYCLIC_EVENT ) || \
    defined ( IL_ENABLE_TX_SECURE_EVENT ) || \
    defined ( IL_ENABLE_TX_FAST_ON_START )
      ilTxEventCounter[ilTxHnd] = 0;        /* Clear counter for event timer    */
# endif
    }
  }

# if defined ( IL_ENABLE_TX_TIMEOUT )
#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
  for (ilTxHnd = IlTxTimeoutCntStartIndex[channel]; ilTxHnd < IlTxTimeoutCntStartIndex[(channel + 1)]; ilTxHnd++)
#  else
  for (ilTxHnd = 0; ilTxHnd < kIlNumberOfTxTimeoutCounters; ilTxHnd++)
#  endif
  {
    /*--- Stop timeout counter --------------------*/
    ilTxTimeoutCounter[ilTxHnd] = 0;
  }
# endif

  /*---- Reset all flags for transmission path ------------------------------*/
  ResetTxTimeoutFlags(IL_CHANNEL_ILPARA_MACRO);
  ResetConfirmationFlags(IL_CHANNEL_ILPARA_MACRO);

  /*---- Reset confirmation flags of the CAN driver ---------------------------*/
  IlResetCanConfirmationFlags(IL_CHANNEL_ILPARA_ONLY);
# if defined(IL_ENABLE_TX_CYCLIC_EVENT)
  InitIfActiveFlags(IL_CHANNEL_ILPARA_ONLY);
# endif


   /*---- call user defined function on start of transmission path of IAL ----*/
# if defined IL_ENABLE_SYS_TX_START_FCT
  ApplIlTxStart(IL_CHANNEL_ILPARA_ONLY);
# endif
# if defined( IL_ENABLE_TX_SIGNAL_START_FCT )
  IlSignalTxStart(IL_CHANNEL_ILPARA_ONLY);
# endif

  EnableTx(IL_CHANNEL_ILPARA_MACRO);

#else /* IL_ENABLE_TX */
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1) && defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;            /* to avoid compiler warnings */
# endif
#endif /* IL_ENABLE_TX */
}                               /* End of IlTxStart() */

/**********************************************************************************************************************
  IlRxStop
**********************************************************************************************************************/
void IlRxStop(IL_CHANNEL_ILTYPE_ONLY)
{
#if defined ( IL_ENABLE_RX )

# if defined IL_ENABLE_RX_STOP_DEFAULTVALUE
  IlReceiveHandle ilRxHnd;
# endif

  if (RxSuspended(IL_CHANNEL_ILPARA_MACRO)) /* Receive already disabled  */
  {
    return;
  }

  SuspendRx(IL_CHANNEL_ILPARA_MACRO);

# if defined IL_ENABLE_RX_STOP_DEFAULTVALUE
  /*--- Loop for every receive object ---------------------------------------*/
#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
  for (ilRxHnd = IlRxMsgStartIndex[channel]; ilRxHnd < IlRxMsgStartIndex[(channel + 1)]; ilRxHnd++)
#  else
  for (ilRxHnd = 0; ilRxHnd < kIlNumberOfRxObjects; ilRxHnd++)
#  endif
  {
    IlAssert(ilRxHnd < kIlNumberOfRxObjects, ILERR_ILLRXMSGHANDLE);

    /*-- Set receive buffer to default for some signals, if selected -------*/
    if ((IlRxDefaultInitValue[ilRxHnd] != V_NULL) && (IlRxDefaultStopMask[ilRxHnd] != V_NULL))
    {
      SetRxDefaultWithMask(ilRxHnd, IlRxDefaultStopMask[ilRxHnd]);
    }
  }
# endif


  /*---- call user defined function on stop of reception path of IAL --------*/
# if defined IL_ENABLE_SYS_RX_STOP_FCT
  ApplIlRxStop(IL_CHANNEL_ILPARA_ONLY);
# endif
# if defined( IL_ENABLE_RX_SIGNAL_STOP_FCT )
  IlSignalRxStop(IL_CHANNEL_ILPARA_ONLY);
# endif

#else /* IL_ENABLE_RX */
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1) && defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;            /* to avoid compiler warnings */
# endif
#endif /* IL_ENABLE_RX */
}                               /* End of IlRxStop() */

/**********************************************************************************************************************
  IlTxStop
**********************************************************************************************************************/
void IlTxStop(IL_CHANNEL_ILTYPE_ONLY)
{
#if defined ( IL_ENABLE_TX )

# if defined IL_ENABLE_TX_STOP_DEFAULTVALUE
  IlTransmitHandle ilTxHnd;
# endif

  if (TxSuspended(IL_CHANNEL_ILPARA_MACRO))
  {
    return;
  }
  SuspendTx(IL_CHANNEL_ILPARA_MACRO);

# if defined IL_ENABLE_TX_STOP_DEFAULTVALUE
  /*--- Loop for every receive object ---------------------------------------*/
#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
  for (ilTxHnd = IlTxMsgStartIndex[channel]; ilTxHnd < IlTxMsgStartIndex[(channel + 1)]; ilTxHnd++)
#  else
  for (ilTxHnd = 0; ilTxHnd < kIlNumberOfTxObjects; ilTxHnd++)
#  endif
  {
    IlAssert(ilTxHnd < kIlNumberOfTxObjects, ILERR_ILLTXMSGHANDLE);

    /*-- Set receive buffer to default for some signals, if selected -------*/
    if ((IlTxDefaultInitValue[ilTxHnd] != V_NULL) && (IlTxDefaultStopMask[ilTxHnd] != V_NULL)
#  if defined( IL_ENABLE_SYS_MULTI_ECU_PHYS )
        && ((IlTxIdentityAssignment[ilTxHnd] & V_ACTIVE_IDENTITY_MSK) != (tVIdentityMsk) 0)
#  endif
      )
    {
      SetTxDefaultWithMask(ilTxHnd, IlTxDefaultStopMask[ilTxHnd]);
    }
  }
# endif


  /*---- call user defined function on stop of transmission path of IAL -----*/
# if defined IL_ENABLE_SYS_TX_STOP_FCT
  ApplIlTxStop(IL_CHANNEL_ILPARA_ONLY);
# endif
# if defined( IL_ENABLE_TX_SIGNAL_STOP_FCT )
  IlSignalTxStop(IL_CHANNEL_ILPARA_ONLY);
# endif

#else /* IL_ENABLE_TX */
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1) && defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;            /* to avoid compiler warnings */
# endif
#endif /* IL_ENABLE_TX */
}                               /* End of IlTxStop() */

/**********************************************************************************************************************
  IlRxWait
**********************************************************************************************************************/
void IlRxWait(IL_CHANNEL_ILTYPE_ONLY)
{
#if defined ( IL_ENABLE_RX )
  if (RxEnabled(IL_CHANNEL_ILPARA_MACRO))
  {
    WaitRx(IL_CHANNEL_ILPARA_MACRO);
  }
#else /* IL_ENABLE_RX */
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1) && defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;            /* to avoid compiler warnings */
# endif
#endif /* IL_ENABLE_RX */
}                               /* End of IlRxWait() */

/**********************************************************************************************************************
  IlTxWait
**********************************************************************************************************************/
void IlTxWait(IL_CHANNEL_ILTYPE_ONLY)
{
#if defined ( IL_ENABLE_TX )
  if (TxEnabled(IL_CHANNEL_ILPARA_MACRO))
  {
    WaitTx(IL_CHANNEL_ILPARA_MACRO);
  }
#else /* IL_ENABLE_TX */
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1) && defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;            /* to avoid compiler warnings */
# endif
#endif /* IL_ENABLE_TX */
}                               /* End of IlTxWait() */

/**********************************************************************************************************************
  IlRxRelease
**********************************************************************************************************************/
void IlRxRelease(IL_CHANNEL_ILTYPE_ONLY)
{
#if defined ( IL_ENABLE_RX )

# if defined( IL_ENABLE_RX_TIMEOUT )
  IlRxTimeoutHandle ilRxToHnd;
# endif

  if (RxWaiting(IL_CHANNEL_ILPARA_MACRO))
  {
# if defined( IL_ENABLE_RX_TIMEOUT )

    /* reset old message indications for Timeout supervision */
    ResetRxTimerFlags(IL_CHANNEL_ILPARA_MACRO);

#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
    for (ilRxToHnd = IlRxTimeoutCntStartIndex[channel]; ilRxToHnd < IlRxTimeoutCntStartIndex[(channel + 1)];
         ilRxToHnd++)
#  else
    for (ilRxToHnd = 0; ilRxToHnd < kIlNumberOfRxTimeoutCounters; ilRxToHnd++)
#  endif
    {
      IlAssert(ilRxToHnd < kIlNumberOfRxTimeoutCounters, ILERR_ILLRXTIMEOUTINDEX);

      /*--- Reset timeout counter to its initial state --------------------*/
#  if defined ( IL_ENABLE_RX_TIMEOUT_DELAY )
      /* Timeout observation starts with first receive of a message */
      ilRxTimeoutCounter[ilRxToHnd] = 0;
#  else
      ilRxTimeoutCounter[ilRxToHnd] = IlGetStartRxTimeoutCounter(ilRxToHnd);
#  endif
    }
# endif

# if defined ( IL_ENABLE_SYS_RX_RESET_TIMEOUT_FLAGS_ON_ILRXRELEASE )
    IlResetRxTimeoutFlags(IL_CHANNEL_ILPARA_ONLY);
# endif


    EnableRx(IL_CHANNEL_ILPARA_MACRO);
  }

#else /* IL_ENABLE_RX */
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1) && defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;            /* to avoid compiler warnings */
# endif
#endif /* IL_ENABLE_RX */
}                               /* End of IlRxRelease() */

/**********************************************************************************************************************
  IlTxRelease
**********************************************************************************************************************/
void IlTxRelease(IL_CHANNEL_ILTYPE_ONLY)
{
#if defined ( IL_ENABLE_TX )
  if (TxWaiting(IL_CHANNEL_ILPARA_MACRO))
  {
    EnableTx(IL_CHANNEL_ILPARA_MACRO);
  }

#else /* IL_ENABLE_TX */
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1) && defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;            /* to avoid compiler warnings */
# endif
#endif /* IL_ENABLE_TX */
}                               /* End of IlTxRelease() */

/**********************************************************************************************************************
  IlRxTask
**********************************************************************************************************************/
void IlRxTask(IL_CHANNEL_ILTYPE_ONLY)
{
  IlRxStateTask(IL_CHANNEL_ILPARA_ONLY);
  IlRxTimerTask(IL_CHANNEL_ILPARA_ONLY);
}                               /* End of IlRxTask() */

/**********************************************************************************************************************
  IlRxStateTask
**********************************************************************************************************************/
void IlRxStateTask(IL_CHANNEL_ILTYPE_ONLY)
{
#if defined ( IL_ENABLE_RX )

# if defined( IL_ENABLE_RX_POLLING ) 
  /* This check shall avoid, that Signal Indication "hangs" without precopy */
#  if defined( C_ENABLE_GENERIC_PRECOPY )
  /* Check if any message for this channel has been received */
  if (IlGetChannelReceived(IL_CHANNEL_ILPARA_MACRO) == 0)
  {
    return;
  }
  /* Reset the channel flag */
  IlClrChannelReceived(IL_CHANNEL_ILPARA_MACRO);
#  endif /* C_ENABLE_GENERIC_PRECOPY */

  if (RxNotSuspended(IL_CHANNEL_ILPARA_MACRO))
  {
#  if defined( IL_ENABLE_RX_POLLING )
    CanReceiveHandle canRxHnd;

      /*--- Loop for every receive object ---------------------------------------*/
#   if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
    for (canRxHnd = IlCanRxMsgStartIndex[channel]; canRxHnd < IlCanRxMsgStartIndex[(channel + 1)]; canRxHnd++)
#   else
    for (canRxHnd = 0; canRxHnd < (CanReceiveHandle) kIlCanNumberOfRxObjects; canRxHnd++)
#   endif
    {
      /* Efficiency might be improved, but this is the way how the CAN-driver does it */
      if ((IlGetRxIndicationFlags(IlGetRxIndicationOffset(canRxHnd)) & IlGetRxIndicationMask(canRxHnd)) !=
          0)
      {
        /*  clear the Indication flags */
        CanInterruptDisable();
        IlGetRxIndicationFlags(IlGetRxIndicationOffset(canRxHnd)) &=
          (vuint8) ~ IlGetRxIndicationMask(canRxHnd);
        CanInterruptRestore();

        if (IlCanRxIndicationFctPtr[canRxHnd] != V_NULL)
        {
          IlCanRxIndicationFctPtr[canRxHnd] ();
        }
      }
    }
#  endif

  }
# else /* IL_ENABLE_RX_POLLING || IL_ENABLE_RX_EXT_TIMEOUT */
#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1) && defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;            /* to avoid compiler warnings */
#  endif
# endif /* IL_ENABLE_RX_POLLING || IL_ENABLE_RX_EXT_TIMEOUT */

#else /* IL_ENABLE_RX */
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1) && defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;            /* to avoid compiler warnings */
# endif
#endif /* IL_ENABLE_RX */
}                               /* End of IlRxStateTask() */

/**********************************************************************************************************************
  IlRxTimerTask
**********************************************************************************************************************/
void IlRxTimerTask(IL_CHANNEL_ILTYPE_ONLY)
{
#if defined ( IL_ENABLE_RX )

# if defined( IL_ENABLE_RX_TIMEOUT )
#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
  IlRxTimeoutHandle ilRxToHnd = IlRxTimeoutCntStartIndex[channel];
#  else
  IlRxTimeoutHandle ilRxToHnd = 0;
#  endif
  vuint8 ilRxPackedIndex;
  vuint8 ilRxPackedMask = 0x01;
# endif

  if (!RxEnabled(IL_CHANNEL_ILPARA_MACRO))
  {
    return;
  }

# if defined( IL_ENABLE_RX_TIMEOUT )

#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
  for (ilRxPackedIndex = IlRxTimerFlagsStartIndex[channel]; ilRxPackedIndex < IlRxTimerFlagsStartIndex[channel + 1];
       ilRxPackedIndex++)
#  else
  for (ilRxPackedIndex = 0; ilRxPackedIndex < kIlNumberOfTimerFlagBytes; ilRxPackedIndex++)
#  endif
  {
    vuint8 ilBitMask = 0xFF;
    vuint8 ilFlagMask;

    CanInterruptDisable();
    ilFlagMask = ilRxTimerFlags[ilRxPackedIndex];
    ilRxTimerFlags[ilRxPackedIndex] = 0;
    CanInterruptRestore();

    while (ilBitMask != 0x00)
    {
      ilBitMask &= (vuint8) ~ (ilRxPackedMask & 0xFFu);
      if ((ilFlagMask & ilRxPackedMask) != 0)
      {
        ilRxTimeoutCounter[ilRxToHnd] = IlGetStartRxTimeoutCounter(ilRxToHnd);
      }
      else
      {
        {
#  if defined( IL_ENABLE_RX_DYNAMIC_TIMEOUT )
          if (ilRxTimeoutCounter[ilRxToHnd] != kIlNoRxTimeoutSupervision)
          {
#  endif
            if (ilRxTimeoutCounter[ilRxToHnd] != 0)
            {
              ilRxTimeoutCounter[ilRxToHnd]--;
              if (ilRxTimeoutCounter[ilRxToHnd] == 0)
              {
                (*IlRxTimeoutFctPtr[ilRxToHnd]) ();
              }
            }
#  if defined( IL_ENABLE_RX_DYNAMIC_TIMEOUT )
          }
#  endif
        }
      }
      if (ilRxPackedMask == 0x80)
      {
        ilRxPackedMask = 0x01;
      }
      else
      {
        ilRxPackedMask <<= 1;
      }
      ilRxToHnd++;

#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
      if (ilRxToHnd >= IlRxTimeoutCntStartIndex[(channel + 1)])
#  else
      if (ilRxToHnd >= kIlNumberOfRxTimeoutCounters)
#  endif
      {
        ilBitMask = 0x00;
      }
    }
  }
# endif/* IL_ENABLE_RX_TIMEOUT */


#else /* IL_ENABLE_RX */
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1) && defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;            /* to avoid compiler warnings */
# endif

#endif /* IL_ENABLE_RX */
}                               /* End of IlRxTimerTask() */

/**********************************************************************************************************************
  IlInit
**********************************************************************************************************************/
void IlTxTask(IL_CHANNEL_ILTYPE_ONLY)
{
  IlTxStateTask(IL_CHANNEL_ILPARA_ONLY);
  IlTxTimerTask(IL_CHANNEL_ILPARA_ONLY);
}                               /* End of IlTxTask() */

/**********************************************************************************************************************
  IlTxStateTask
**********************************************************************************************************************/
void IlTxStateTask(IL_CHANNEL_ILTYPE_ONLY)
{
#if defined ( IL_ENABLE_TX )
  IlTransmitHandle ilTxHnd;
  CanTransmitHandle x;

  /* Check if any message for this channel has been recived */
  if (TxNotSuspended(IL_CHANNEL_ILPARA_MACRO))
  {
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
    ilTxHnd = IlTxMsgStartIndex[channel + 1];
    if (IlTxMsgStartIndex[channel] == ilTxHnd)
    {
      return;
    }
# else
    ilTxHnd = kIlNumberOfTxObjects;
# endif
    /*--- Loop for every transmit object ---------------------------------------*/
    do
    {
      /* Initial Decrement */
      ilTxHnd--;

      /* Go from IL handle to CAN handle */
      x = IlGetIlTxIndirection(ilTxHnd);

      if (x != kCanTxHandleNotUsed)
      {
        /* Efficiency might be improved, but this is the way how the CAN-driver does it */
        if ((IlGetTxConfirmationFlags(IlGetTxConfirmationOffset(x)) & IlGetTxConfirmationMask(x)) != 0)
        {
          /*  Clear the Confirmation flags */
          CanInterruptDisable();
          IlGetTxConfirmationFlags(IlGetTxConfirmationOffset(x)) &= (vuint8) ~ IlGetTxConfirmationMask(x);
          /* Counter for Update time must be reloaded twice, here and in confirmation fct */
          ilTxUpdateCounter[ilTxHnd] = IlTxUpdateCycles[ilTxHnd];
          CanInterruptRestore();

# if defined( IL_ENABLE_TX_POLLING )
          if (IlTxConfirmationFctPtr[ilTxHnd] != V_NULL)
          {
            IlTxConfirmationFctPtr[ilTxHnd] ();
          }
# endif
        }
      }
# if defined IL_ENABLE_SYS_TRANSMIT_IN_STATETASK

#  error "in case of multiple ECU ILSendMessage is only allowed to be called for Tx messages of the current ECU"

      /*********************************************************************/
      /*-- Send message if send request is set AND update time expired ----*/
      /*********************************************************************/
      if (((ilTxState[ilTxHnd] & kTxSendRequest) != 0) && (ilTxUpdateCounter[ilTxHnd] == 0))
      {
#  if defined ( IL_ENABLE_TX_TIMEOUT )
        if ((ilTxState[ilTxHnd] & kTxCheckTimeout) != 0)
        {
          /* start Timeout supervision for this message */
          IlAssert(IlTxTimeoutIndirection[ilTxHnd] < kIlNumberOfTxTimeoutCounters, ILERR_ILLTXTIMEOUTINDEX);
          CanInterruptDisable();
          if (ilTxTimeoutCounter[IlTxTimeoutIndirection[ilTxHnd]] == 0)
          {
            ilTxTimeoutCounter[IlTxTimeoutIndirection[ilTxHnd]] = IlGetTxTimeout(IL_CHANNEL_ILPARA_MACRO);
          }
          CanInterruptRestore();
        }
#  endif
        IlSendMessage(ilTxHnd);
      }
# endif
    }
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
    while (ilTxHnd != IlTxMsgStartIndex[channel]);
# else
    while (ilTxHnd != 0);
# endif
  }

#else /* IL_ENABLE_TX */
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1) && defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;            /* to avoid compiler warnings */
# endif
#endif /* IL_ENABLE_TX */
}                               /* End of IlTxStateTask() */

/**********************************************************************************************************************
  IlTxTimerTask
**********************************************************************************************************************/
void IlTxTimerTask(IL_CHANNEL_ILTYPE_ONLY)
{
#if defined ( IL_ENABLE_TX )
  IlTransmitHandle ilTxHnd;

  /*----- do Tx-task only if Tx is started ----------------------------------*/
  if (TxEnabled(IL_CHANNEL_ILPARA_MACRO))
  {
# if defined ( IL_ENABLE_TX_TIMEOUT )
    IlCheckTxTimeout(IL_CHANNEL_ILPARA_ONLY);
# endif

# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
    ilTxHnd = IlTxMsgStartIndex[channel + 1];
    if (IlTxMsgStartIndex[channel] == ilTxHnd)
    {
      return;
    }
# else
    ilTxHnd = kIlNumberOfTxObjects;
# endif
    /*--- Loop for every transmit object ---------------------------------------*/
    do
    {
      /* Initial Decrement */
      ilTxHnd--;
      IlAssert((ilTxHnd < kIlNumberOfTxObjects), ILERR_ILLTXMSGHANDLE);

# if defined( IL_ENABLE_SYS_MULTI_ECU_PHYS )
      /* Suppress  message that is not assigned for this Multi-ECU. */
      if ((IlTxIdentityAssignment[ilTxHnd] & V_ACTIVE_IDENTITY_MSK) == (tVIdentityMsk) 0)
      {
        continue;
      }
# endif

      /*********************************************************************/
      /**** Global section. ************************************************/
      /*********************************************************************/

      CanInterruptDisable();
      /*-- decrement counter for update time ------------------------------*/
      if ((ilTxUpdateCounter[ilTxHnd] != kIlStopUpdateCounterValue) && (ilTxUpdateCounter[ilTxHnd] > 0))
      {
        ilTxUpdateCounter[ilTxHnd]--;
      }
      CanInterruptRestore();

      {
        {
          /*********************************************************************/
          /**** Section for the CYCLIC messages ********************************/
          /*********************************************************************/

          /*-- Check if Cyclic send is active for this message ----------------*/
          if ((ilTxState[ilTxHnd] & kTxSendCyclic) != 0)
          {
            if (ilTxCyclicCounter[ilTxHnd] != 0)    /* decrement timer counter      */
            {
              ilTxCyclicCounter[ilTxHnd]--; /*   only if not 0              */
            }

            /*-- Tx timer expired -> request to send message -----------------*/
            if (ilTxCyclicCounter[ilTxHnd] == 0)
            {
# if  defined ( IL_ENABLE_TX_FAST_ON_START )
#  if defined ( VGEN_GENY ) && defined( IL_ENABLE_TX_MODE_SIGNALS )
              if ((ilTxState[ilTxHnd] & kTxFastOnStart) != 0)
              {
                if (IlTxFastOnStartMuxDelay[ilTxHnd] != 0)
                {
                  /*---- only multiplexed ilTxHnd ----*/
                  ilTxCyclicCounter[ilTxHnd] = IlTxFastOnStartMuxDelay[ilTxHnd];
                  ilTxEventCounter[ilTxHnd] = IlTxFastOnStartMuxDelay[ilTxHnd];
                }
                else
                {
                  /*-- Reload the cycle counter for the cyclic message ----------*/
                  ilTxCyclicCounter[ilTxHnd] = IlGetTxCyclicCycles(ilTxHnd);
                }
#  endif

                CanInterruptDisable();
                ilTxState[ilTxHnd] &= kTxNotFastOnStart;
                CanInterruptRestore();

#  if defined ( VGEN_GENY ) && defined( IL_ENABLE_TX_MODE_SIGNALS )
              }
              else
#  endif
# endif/* IL_ENABLE_TX_FAST_ON_START */
              {
                 /*-- Reload the cycle counter for the cyclic message ----------*/
                ilTxCyclicCounter[ilTxHnd] = IlGetTxCyclicCycles(ilTxHnd);
              }

# if defined ( IL_ENABLE_TX_CYCLIC_EVENT )  || \
    defined ( IL_ENABLE_TX_FAST_ON_START )
              if ((ilTxState[ilTxHnd] & (kTxSendCyclicEvent | kTxFastOnStart)) == 0)
# endif
              {
# if defined ( IL_ENABLE_TX_SECURE_EVENT )
                if ((ilTxNSendCounter[ilTxHnd]) == 0)
# endif
                {
                  /* only if nothing is send fast */
                    /*-- Set the send request bit in tx-status field --------------*/
                  CanInterruptDisable();
                  ilTxState[ilTxHnd] |= kTxSendRequest;
                  CanInterruptRestore();
                }
              }
            }
          }

# if defined ( IL_ENABLE_TX_CYCLIC_EVENT ) || defined ( IL_ENABLE_TX_SECURE_EVENT ) || \
    defined ( IL_ENABLE_TX_FAST_ON_START )

            /*********************************************************************/
            /**** Section for the CYCLIC- and SECURE-EVENT messages **************/
            /*********************************************************************/

            /*-- Check if Cyclic event send is active for this message ----------*/
#  if defined( IL_ENABLE_TX_SECURE_EVENT )
          if (((ilTxState[ilTxHnd] & (kTxSendCyclicEvent | kTxFastOnStart)) != 0)
              || (ilTxNSendCounter[ilTxHnd] != 0))
#  else
          if ((ilTxState[ilTxHnd] & (kTxSendCyclicEvent | kTxFastOnStart)) != 0)
#  endif
          {
            if (ilTxEventCounter[ilTxHnd] != 0)     /* decrement timer counter */
            {
              ilTxEventCounter[ilTxHnd]--;  /*   only if not 0        */
            }

            /*-- Tx EventTimer expired -> request to send message ------------*/
            if (ilTxEventCounter[ilTxHnd] == 0)
            {
              CanInterruptDisable();
#  if defined( IL_ENABLE_TX_SECURE_EVENT )
              if (ilTxNSendCounter[ilTxHnd] != 0)
              {
                ilTxState[ilTxHnd] |= (kTxSendRequest | kTxNSendRequest);
              }
              else
#  endif
              {
                ilTxState[ilTxHnd] |= kTxSendRequest;
              }
              CanInterruptRestore();

              ilTxEventCounter[ilTxHnd] = IlTxEventCycles[ilTxHnd];
            }
          }
# endif/* IL_ENABLE_TX_CYCLIC_EVENT || IL_ENABLE_TX_SECURE_EVENT || IL_ENABLE_TX_FAST_ON_START */
          /*********************************************************************/
          /*-- Send message if send request is set AND update time expired ----*/
          /*********************************************************************/
          if (((ilTxState[ilTxHnd] & kTxSendRequest) != 0) && (ilTxUpdateCounter[ilTxHnd] == 0))
          {
# if defined ( IL_ENABLE_TX_TIMEOUT )
            if ((ilTxState[ilTxHnd] & kTxCheckTimeout) != 0)
            {
              /* start Timeout supervision for this message */
              IlAssert(IlTxTimeoutIndirection[ilTxHnd] < kIlNumberOfTxTimeoutCounters,
                           ILERR_ILLTXTIMEOUTINDEX);
              CanInterruptDisable();
              if (ilTxTimeoutCounter[IlTxTimeoutIndirection[ilTxHnd]] == 0)
              {
                ilTxTimeoutCounter[IlTxTimeoutIndirection[ilTxHnd]] =
                  IlGetTxTimeout(IL_CHANNEL_ILPARA_MACRO);
              }
              CanInterruptRestore();
            }
# endif
            IlSendMessage(ilTxHnd);
          }
        }
      }
    }
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
    while (ilTxHnd != IlTxMsgStartIndex[channel]);
# else
    while (ilTxHnd != 0);
# endif
  }                             /* if TxEnabled */
#else /* IL_ENABLE_TX */
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1) && defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;            /* to avoid compiler warnings */
# endif
#endif /* IL_ENABLE_TX */
}                               /* End of IlTxTimerTask() */

#if defined ( C_ENABLE_CAN_CANCEL_NOTIFICATION )
/**********************************************************************************************************************
  IlCanCancelNotification
**********************************************************************************************************************/
/** \brief    This method is called if a transmit message is deleted (CanCancelTransmit, CanOffline or CanInit)
              and the update counter will be reset.
    \param    txHandle     Handle of the Can Driver tx message.
    \return   none
    \context  The function can be called on task and interupt level.
    \note     The function is called by the Can Driver. Either IlCanCancelNotification is exclusively called or
              the Can Driver sets a confirmation flag or calls a confirmation function.
**********************************************************************************************************************/
void IlCanCancelNotification(CanTransmitHandle txHandle)
{
# if defined ( IL_ENABLE_TX )

#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
  CanChannelHandle channel = CanGetChannelOfTxObj(txHandle);
#  endif
  IlTransmitHandle ilTxHnd;

   /*----- do the loop for every Tx object  -------------------------------*/
#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
  for (ilTxHnd = IlTxMsgStartIndex[channel]; ilTxHnd < IlTxMsgStartIndex[(channel + 1)]; ilTxHnd++)
#  else
  for (ilTxHnd = 0; ilTxHnd < kIlNumberOfTxObjects; ilTxHnd++)
#  endif
  {
    if (IlGetIlTxIndirection(ilTxHnd) == txHandle)
    {
      /* Reset the Update counter when message */
      /* is cancelled by the DLL.              */
      ilTxUpdateCounter[ilTxHnd] = 0;
    }
    /* loop over all objects in case of Multiplex signals */
  }


# else /* IL_ENABLE_TX */
#  if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  txHandle = txHandle;          /* to avoid compiler warnings */
#  endif
# endif /* IL_ENABLE_TX */
}                               /* End of IlCanCancelNotification() */
#endif /* ( C_ENABLE_CAN_CANCEL_NOTIFICATION ) */

#if defined ( C_ENABLE_GENERIC_PRECOPY )
/**********************************************************************************************************************
  IlCanGenericPrecopy
**********************************************************************************************************************/
/** \brief    This method is called immedialtely after the DLC-CHeck and notifies about the reception of a Can message.
    \param    rcvObject    Handle of the Can Driver tx message.
    \param    dptr         Pointer to the received data in the CAN Controller receive register
    \param    rxStruct     Pointer to the receive structure.
    \return   kCanCopyData, if the data processing shall be continued, else kCanNoCopyData.
    \context  The method can be called on task and interupt level.
    \note     The method is called by the Can Driver.
**********************************************************************************************************************/
# if defined( C_SINGLE_RECEIVE_BUFFER )
vuint8 IlCanGenericPrecopy(CanReceiveHandle rcvObject)
#  define canRxHnd  (rcvObject)
# endif
# if defined( C_MULTIPLE_RECEIVE_BUFFER )
     vuint8 IlCanGenericPrecopy(CanReceiveHandle rcvObject, pChipDataPtr dptr)
#  define canRxHnd  (rcvObject)
# endif
# if defined(C_SINGLE_RECEIVE_CHANNEL) || defined(C_MULTIPLE_RECEIVE_CHANNEL)
     vuint8 IlCanGenericPrecopy(CanRxInfoStructPtr rxStruct)
#  define canRxHnd  (rxStruct->Handle)
# endif
{

# if defined ( IL_ENABLE_RX )

#  if defined ( C_MULTIPLE_RECEIVE_BUFFER ) && defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  dptr = dptr;                  /* to avoid compiler warnings */
#  endif

  /* Not an IL message */
  if (canRxHnd >= kIlCanNumberOfRxObjects)
  {
    return kCanCopyData;
  }
  if (RxSuspended(IL_CHANNEL_ILPRECOPY_MACRO))
  {                             /* Receive is disabled  */
    return kCanNoCopyData;
  }
#  if defined( IL_ENABLE_RX_MULTI_ECU_PHYS )
  if ((IlCanRxIdentityAssignment[canRxHnd] & V_ACTIVE_IDENTITY_MSK) == (tVIdentityMsk) 0)
  {
    return kCanNoCopyData;
  }
#  endif

#  if defined( IL_ENABLE_RX_POLLING ) 
  IlSetChannelReceived(IL_CHANNEL_ILPRECOPY_MACRO);
#  endif


  return kCanCopyData;
# else/* IL_ENABLE_RX */
#  if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
#   if defined( C_SINGLE_RECEIVE_BUFFER )
  rcvObject = rcvObject;
#   endif
#   if defined( C_MULTIPLE_RECEIVE_BUFFER )
  rcvObject = rcvObject;
  dptr = dptr;
#   endif
#   if defined(C_SINGLE_RECEIVE_CHANNEL) || defined(C_MULTIPLE_RECEIVE_CHANNEL)
  rxStruct = rxStruct;
#   endif
#  endif/* V_ENABLE_USE_DUMMY_STATEMENT */
  return kCanCopyData;
# endif/* IL_ENABLE_RX */
}                               /* End of IlCanGenericPrecopy() */
#endif /* !IL_TYPE_AUTOSAR && C_ENABLE_GENERIC_PRECOPY */

/**********************************************************************************************************************
  IlGetStatus
**********************************************************************************************************************/
Il_Status IlGetStatus(IL_CHANNEL_ILTYPE_ONLY)
{
  return (Il_Status) Il_ChannelState(IL_CHANNEL_ILPARA_MACRO);
}                               /* End of IlGetStatus() */

#if defined( IL_ENABLE_TX_SEND_ON_INIT )
/**********************************************************************************************************************
  IlSendOnInitMsg
**********************************************************************************************************************/
void IlSendOnInitMsg(IL_CHANNEL_ILTYPE_ONLY)
{
# if defined ( IL_ENABLE_TX )
  IlTransmitHandle ilTxHnd;

#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
  IlAssert(channel < kIlNumberOfChannels, ILERR_ILLCHANNEL);

#   if defined IL_ENABLE_SYS_ARGCHECK
  if (channel >= kIlNumberOfChannels)
  {
    return;
  }
#   endif
  CanInterruptDisable();
  for (ilTxHnd = IlTxMsgStartIndex[channel]; ilTxHnd < IlTxMsgStartIndex[(channel + 1)]; ilTxHnd++)
#  else
  CanInterruptDisable();
  for (ilTxHnd = 0; ilTxHnd < kIlNumberOfTxObjects; ilTxHnd++)
#  endif
  {
    /*--- Queue message On-Init only if it is configured to be allowed -----*/
    if ((ilTxState[ilTxHnd] & kTxQueueInit) != 0)
    {
      ilTxState[ilTxHnd] |= kTxSendRequest;
    }
  }
  CanInterruptRestore();

# else/* IL_ENABLE_TX */
#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1) && defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;            /* to avoid compiler warnings */
#  endif
# endif/* IL_ENABLE_TX */
}                               /* End of IlSendOnInitMsg() */
#endif /* IL_ENABLE_TX_SEND_ON_INIT */

#if defined IL_ENABLE_TX_STARTSTOP_CYCLIC
/**********************************************************************************************************************
  IlStartCycle
**********************************************************************************************************************/
Il_Status IlStartCycle(IlTransmitHandle ilTxHnd)
{
# if defined ( IL_ENABLE_TX )
  /*----- Check for valid handle numbers ------------------------------------*/
  IlAssert(ilTxHnd < kIlNumberOfTxObjects, ILERR_ILLTXMSGHANDLE);

#  if defined IL_ENABLE_SYS_ARGCHECK
  if (ilTxHnd >= kIlNumberOfTxObjects)
  {
    return IL_ERROR;
  }
#  endif

  /*----- Check if SendState already active for this timebase ---------------*/
  if ((ilTxState[ilTxHnd] & kTxSendCyclic) != 0)
  {
    return IL_QUEUED;
  }

  CanInterruptDisable();
  /*--- Set the flag of the cyclic messages in the --------------------------*/
  ilTxState[ilTxHnd] |= kTxSendCyclic;

  /*--- Init cycle counter to the StartDelayTime (if actual time is greater) */
  if (ilTxCyclicCounter[ilTxHnd] > IlTxStartCycles[ilTxHnd])    /* + 1 already done in the table */
  {
    ilTxCyclicCounter[ilTxHnd] = IlTxStartCycles[ilTxHnd];      /* + 1 already done in the table */
  }
  CanInterruptRestore();

# else /* IL_ENABLE_TX */
#  if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  ilTxHnd = ilTxHnd;
#  endif
# endif /* IL_ENABLE_TX */
  return IL_OK;
}                               /* End of IlStartCycle(() */

/**********************************************************************************************************************
  IlStopCycle
**********************************************************************************************************************/
Il_Status IlStopCycle(IlTransmitHandle ilTxHnd)
{
# if defined ( IL_ENABLE_TX )
  /*----- Check for valid handle numbers ------------------------------------*/
  IlAssert(ilTxHnd < kIlNumberOfTxObjects, ILERR_ILLTXMSGHANDLE);

#  if defined IL_ENABLE_SYS_ARGCHECK
  if (ilTxHnd >= kIlNumberOfTxObjects)
  {
    return IL_ERROR;
  }
#  endif
  CanInterruptDisable();
  ilTxState[ilTxHnd] &= kTxNotSendCyclic;   /* Stop cyclic send        */
  CanInterruptRestore();

# else /* IL_ENABLE_TX */
#  if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  ilTxHnd = ilTxHnd;
#  endif
# endif /* IL_ENABLE_TX */
  return IL_OK;
}                               /* End of IlStopCycle() */
#endif /* IL_ENABLE_TX_STARTSTOP_CYCLIC */


#if defined(IL_ENABLE_TX_CYCLIC_EVENT)
/**********************************************************************************************************************
  IlStartEvent
**********************************************************************************************************************/
Il_Status IlStartEvent(IlTransmitHandle ilTxHnd)
{
  /*----- Check for valid handle numbers ------------------------------------*/
  IlAssert(ilTxHnd < kIlNumberOfTxObjects, ILERR_ILLTXMSGHANDLE);

# if defined IL_ENABLE_SYS_ARGCHECK
  if (ilTxHnd >= kIlNumberOfTxObjects)
  {
    return IL_ERROR;
  }
# endif

  /*--- Start immediately transmission of cyclic event messages -------------*/
  ilTxEventCounter[ilTxHnd] = 0;

  /*-- Set the status of the message for cyclic-event -----------------------*/
  ilTxState[ilTxHnd] |= kTxSendCyclicEvent;

  return (1);
}                               /* End of IlStartEvent() */

/**********************************************************************************************************************
  IlStopEvent
**********************************************************************************************************************/
Il_Status IlStopEvent(IlTransmitHandle ilTxHnd)
{
  /*----- Check for valid handle numbers ------------------------------------*/
  IlAssert(ilTxHnd < kIlNumberOfTxObjects, ILERR_ILLTXMSGHANDLE);

# if defined IL_ENABLE_SYS_ARGCHECK
  if (ilTxHnd >= kIlNumberOfTxObjects)
  {
    return IL_ERROR;
  }
# endif

  /* Stop cyclic send        */
  ilTxState[ilTxHnd] &= kTxNotSendCyclicEvent;

  return (0);
}                               /* End of IlStopEvent() */
#endif /* IL_TX_CYCLIC_EVENT */

#if defined ( IL_ENABLE_TX )
/**********************************************************************************************************************
  IlSetEvent
**********************************************************************************************************************/
void IlSetEvent(IlTransmitHandle ilTxHnd)
{
  /*----- Check for valid handle numbers ------------------------------------*/
  IlAssert(ilTxHnd < kIlNumberOfTxObjects, ILERR_ILLTXMSGHANDLE);

# if defined IL_ENABLE_SYS_ARGCHECK
  if (ilTxHnd >= kIlNumberOfTxObjects)
  {
    return;
  }
# endif

  ilTxState[ilTxHnd] |= kTxSendRequest;
}                               /* End of IlSetEvent() */
#endif /* IL_ENABLE_TX */

#if defined( IL_ENABLE_TX_SECURE_EVENT )
/**********************************************************************************************************************
  IlSecureEvent
**********************************************************************************************************************/
void IlSecureEvent(IlTransmitHandle ilTxHnd)
{
  /*----- Check for valid handle numbers ------------------------------------*/
  IlAssert(ilTxHnd < kIlNumberOfTxObjects, ILERR_ILLTXMSGHANDLE);

# if defined IL_ENABLE_SYS_ARGCHECK
  if (ilTxHnd >= kIlNumberOfTxObjects)
  {
    return;
  }
# endif

  /*--- load repeat counter ---*/
  ilTxNSendCounter[ilTxHnd] = IlGetTxRepetitionCounter(ilTxHnd);

  /*--- Start immediately transmission of cyclic event messages -------------*/
  ilTxEventCounter[ilTxHnd] = 0;
}                               /* End of IlSecureEvent() */
#endif

#if defined ( IL_ENABLE_RX )
# if defined ( IL_ENABLE_RX_INDICATION_FLAG )
/**********************************************************************************************************************
  IlGetSignalIndicationFlag
**********************************************************************************************************************/
vuint8 IlGetSignalIndicationFlag(IlReceiveFlagHandle ilRxFlagHnd)
{
  vuint8 rc;

  CanInterruptDisable();

  rc = ilRxIndicationFlags[IlIndicationOffset[ilRxFlagHnd]] & IlIndicationMask[ilRxFlagHnd];
  if (rc != 0)
  {
    /* reset indication flag */
    ilRxIndicationFlags[IlIndicationOffset[ilRxFlagHnd]] &= (vuint8) ~ (IlIndicationMask[ilRxFlagHnd]);
  }

  CanInterruptRestore();

  return (rc);
}                               /* End of IlGetSignalIndicationFlag() */
# endif /* ( IL_ENABLE_RX_INDICATION_FLAG ) */
#endif

/**********************************************************************************************************************
  IlResetRxTimeoutFlags
**********************************************************************************************************************/
void IlResetRxTimeoutFlags(IL_CHANNEL_ILTYPE_ONLY)
{
#if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1) && defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;            /* to avoid compiler warnings */
#endif

#if defined ( IL_ENABLE_RX )
  ResetRxTimeoutFlags(channel);
#endif /* IL_ENABLE_RX */

}                               /* End of IlResetRxTimeoutFlags() */

#if defined ( IL_ENABLE_TX ) && defined( IL_ENABLE_SEND_DIRECT )
/**********************************************************************************************************************
  IlSendDirect
**********************************************************************************************************************/
void IlSendDirect(IL_CHANNEL_ILTYPE_FIRST IlTransmitHandle ilTxHnd)
{
  IlAssert(ilTxHnd < kIlNumberOfTxObjects, ILERR_ILLTXMSGHANDLE);
# if defined ( IL_ENABLE_SYS_ARGCHECK )
  if (ilTxHnd >= kIlNumberOfTxObjects)
  {
    return;
  }
# endif

  if (TxEnabled(IL_CHANNEL_ILPARA_MACRO))
  {
    if ((ilTxState[ilTxHnd] & kTxSendRequest) != 0)
    {
# if defined ( IL_ENABLE_TX_TIMEOUT )
      if ((ilTxState[ilTxHnd] & kTxCheckTimeout) != 0)
      {
        /* start Timeout supervision for this message */
        IlAssert(IlTxTimeoutIndirection[ilTxHnd] < kIlNumberOfTxTimeoutCounters, ILERR_ILLTXTIMEOUTINDEX);
        CanInterruptDisable();
        if (ilTxTimeoutCounter[IlTxTimeoutIndirection[ilTxHnd]] == 0)
        {
          ilTxTimeoutCounter[IlTxTimeoutIndirection[ilTxHnd]] = IlGetTxTimeout(IL_CHANNEL_ILPARA_MACRO);
        }
        CanInterruptRestore();
      }
# endif
      IlSendMessage(ilTxHnd);
    }
  }
}                               /* End of IlSendDirect() */
#endif /* ( IL_ENABLE_TX ) && ( IL_ENABLE_SEND_DIRECT ) */


#if defined( VGEN_GENY ) && defined( IL_ENABLE_TX_DYNAMIC_CYCLETIME )
/**********************************************************************************************************************
  IlSetDynTxCycleTimeRaw
**********************************************************************************************************************/
void IlSetDynTxCycleTimeRaw(IlTransmitHandle ilTxHnd, IltTxCounter ilTxTicks)
{
  IlAssert(ilTxHnd < kIlNumberOfTxObjects, ILERR_ILLTXMSGHANDLE);
# if defined ( IL_ENABLE_SYS_ARGCHECK )
  if (ilTxHnd >= kIlNumberOfTxObjects)
  {
    return;
  }
# endif
  ilTxDynCyclicCycles[ilTxHnd] = ilTxTicks;
}                               /* End of IlSetDynTxCycleTimeRaw() */
#endif  /* VGEN_GENY && IL_ENABLE_TX_DYNAMIC_CYCLETIME */

#if defined( VGEN_GENY ) && defined( IL_ENABLE_SYS_TX_REPETITIONS_ARE_ACTIVE_FCT )
/**********************************************************************************************************************
  IlTxRepetitionsAreActive
**********************************************************************************************************************/
Il_Boolean IlTxRepetitionsAreActive(IL_CHANNEL_ILTYPE_ONLY)
{
# if defined( IL_ENABLE_TX_SECURE_EVENT )
  IlTransmitHandle ilTxHnd;
#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
  ilTxHnd = IlTxMsgStartIndex[channel + 1];
  if (IlTxMsgStartIndex[channel] != ilTxHnd)
#  else
  ilTxHnd = kIlNumberOfTxObjects;
#  endif
  {
    /*--- Loop for every transmit object ---------------------------------------*/
    do
    {
      /* Initial Decrement */
      ilTxHnd--;
      if (ilTxNSendCounter[ilTxHnd] != 0)
      {
        return IL_TRUE;
      }
    }
#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
    while (ilTxHnd != IlTxMsgStartIndex[channel]);
#  else
    while (ilTxHnd != 0);
#  endif
  }
# endif /* IL_ENABLE_TX_SECURE_EVENT */
  return IL_FALSE;
}
#endif /* VGEN_GENY && IL_ENABLE_SYS_TX_REPETITIONS_ARE_ACTIVE_FCT */


#if defined ( IL_ENABLE_SYS_GET_CONTEXT ) 
/**********************************************************************************************************************
  IlGetModuleContext
**********************************************************************************************************************/
void IlGetModuleContext(tIlModuleContextStructPtr pContext)
{
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
  CanChannelHandle channel;
# endif
# if defined ( IL_ENABLE_TX )
  IlTransmitHandle ilTxHnd;
# endif
# if defined( IL_ENABLE_RX_TIMEOUT ) || defined( IL_ENABLE_TX_TIMEOUT ) 
  vuintx i;
# endif


  pContext->ilMagicNumber = ((vuint32) ((vuint32) IL_VECTOR_VERSION << 16) | IL_VECTOR_RELEASE_VERSION);

# if defined ( IL_ENABLE_RX_MULTIPLE_NODES )
  pContext->ilECUMask = ilECUMask;
# endif

/* Copy Channel Dependent Context */
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
  for (channel = 0; channel < kIlNumberOfChannels; channel++)
# endif
  {
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
    pContext->ilChannelState[channel] = ilChannelState[channel];
#  if defined( IL_ENABLE_RX_POLLING ) 
    pContext->ilChannelReceive[channel] = ilChannelReceive[channel];
#  endif
# else
    pContext->ilChannelState = ilChannelState;
#  if defined( IL_ENABLE_RX_POLLING ) 
    pContext->ilChannelReceive = ilChannelReceive;
#  endif
# endif /* C_MULTIPLE_RECEIVE_CHANNEL */

  }

/* Copy Tx Dependent Context */
# if defined ( IL_ENABLE_TX )
  for (ilTxHnd = 0; ilTxHnd < kIlNumberOfTxObjects; ilTxHnd++)
  {
    pContext->ilTxState[ilTxHnd] = ilTxState[ilTxHnd];
    pContext->ilTxCyclicCounter[ilTxHnd] = ilTxCyclicCounter[ilTxHnd];
    pContext->ilTxUpdateCounter[ilTxHnd] = ilTxUpdateCounter[ilTxHnd];

#  if defined( VGEN_GENY ) && defined( IL_ENABLE_TX_DYNAMIC_CYCLETIME )
    pContext->ilTxDynCyclicCycles[ilTxHnd] = ilTxDynCyclicCycles[ilTxHnd];
#  endif
#  if defined( IL_ENABLE_TX_SECURE_EVENT )
    pContext->ilTxNSendCounter[ilTxHnd] = ilTxNSendCounter[ilTxHnd];
#  endif
#  if defined ( IL_ENABLE_TX_CYCLIC_EVENT ) || defined ( IL_ENABLE_TX_SECURE_EVENT ) || defined ( IL_ENABLE_TX_FAST_ON_START )
    pContext->ilTxEventCounter[ilTxHnd] = ilTxEventCounter[ilTxHnd];
#  endif
  }

#  if defined( IL_ENABLE_TX_CYCLIC_EVENT )
  pContext->ilIfActiveFlags = ilIfActiveFlags;
#  endif
#  if defined( IL_ENABLE_TX_TIMEOUT_FLAG )
  VStdMemCpyRamToRam(pContext->ilTxTimeoutFlags, ilTxTimeoutFlags, kIlNumberOfTxTimeoutFlags);
#  endif
#  if defined( IL_ENABLE_TX_CONFIRMATION_FLAG )
  VStdMemCpyRamToRam(pContext->ilTxConfirmationFlags, ilTxConfirmationFlags, kIlNumberOfTxConfirmationFlags);
#  endif
#  if defined( IL_ENABLE_TX_TIMEOUT )
  for (i = 0; i < (vuintx) kIlNumberOfTxTimeoutCounters; i++)
  {
    pContext->ilTxTimeoutCounter[i] = ilTxTimeoutCounter[i];
  }
#  endif
# endif /* IL_ENABLE_TX */

/* Copy Rx Dependent Context */
# if defined ( IL_ENABLE_RX )
#  if defined( IL_ENABLE_RX_TIMEOUT )
  for (i = 0; i < (vuintx) kIlNumberOfRxTimeoutCounters; i++)
  {
    pContext->ilRxTimeoutCounter[i] = ilRxTimeoutCounter[i];
#   if defined ( VGEN_GENY ) && defined( IL_ENABLE_RX_DYNAMIC_TIMEOUT )
    pContext->ilRxDynamicTimeoutCounter[i] = ilRxDynamicTimeoutCounter[i];
#   endif
  }
  VStdMemCpyRamToRam(pContext->ilRxTimerFlags, ilRxTimerFlags, kIlNumberOfTimerFlagBytes);
#  endif
#  if defined( IL_ENABLE_RX_DATACHANGED_FLAG )
  VStdMemCpyRamToRam(pContext->ilRxDataChangedFlags, ilRxDataChangedFlags, kIlNumberOfRxDataChangedFlags);
#  endif
#  if defined( IL_ENABLE_RX_INDICATION_FLAG )
  VStdMemCpyRamToRam(pContext->ilRxIndicationFlags, ilRxIndicationFlags, kIlNumberOfRxIndicationFlags);
#  endif
#  if defined( IL_ENABLE_RX_FIRSTVALUE_FLAG )
  VStdMemCpyRamToRam(pContext->ilRxFirstvalueFlags, ilRxFirstvalueFlags, kIlNumberOfRxFirstvalueFlags);
#  endif
#  if defined ( IL_ENABLE_RX_TIMEOUT_FLAG )
  VStdMemCpyRamToRam(pContext->ilRxTimeoutFlags, ilRxTimeoutFlags, kIlNumberOfRxTimeoutFlags);
#  endif
#  if !defined( VGEN_GENY ) && defined( IL_ENABLE_RX_MODE_SIGNALS )
#  endif
# endif /* IL_ENABLE_RX */

}                               /* End of IlGetModuleContext() */
#endif /* IL_ENABLE_SYS_GET_CONTEXT || VGEN_ENABLE_MDWRAP */

#if defined ( IL_ENABLE_SYS_SET_CONTEXT ) 
/**********************************************************************************************************************
  IlSetModuleContext
**********************************************************************************************************************/
vuint8 IlSetModuleContext(tIlModuleContextStructPtr pContext)
{
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
  CanChannelHandle channel;
# endif
# if defined ( IL_ENABLE_TX )
  IlTransmitHandle ilTxHnd;
# endif
# if defined( IL_ENABLE_RX_TIMEOUT ) || defined( IL_ENABLE_TX_TIMEOUT ) 
  vuintx i;
# endif

  if (pContext->ilMagicNumber == (vuint32) (((vuint32) IL_VECTOR_VERSION << 16) | IL_VECTOR_RELEASE_VERSION))
  {
# if defined ( IL_ENABLE_RX_MULTIPLE_NODES )
    ilECUMask = pContext->ilECUMask;
# endif

/* Copy Channel Dependent Context */
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
    for (channel = 0; channel < kIlNumberOfChannels; channel++)
# endif
    {
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
      ilChannelState[channel] = pContext->ilChannelState[channel];
#  if defined( IL_ENABLE_RX_POLLING ) 
      ilChannelReceive[channel] = pContext->ilChannelReceive[channel];
#  endif
# else
      ilChannelState = pContext->ilChannelState;
#  if defined( IL_ENABLE_RX_POLLING ) 
      ilChannelReceive = pContext->ilChannelReceive;
#  endif
# endif /* C_MULTIPLE_RECEIVE_CHANNEL */
    }

/* Copy Tx Dependent Context */
# if defined ( IL_ENABLE_TX )
    for (ilTxHnd = 0; ilTxHnd < kIlNumberOfTxObjects; ilTxHnd++)
    {
      ilTxState[ilTxHnd] = pContext->ilTxState[ilTxHnd];
      ilTxCyclicCounter[ilTxHnd] = pContext->ilTxCyclicCounter[ilTxHnd];
      ilTxUpdateCounter[ilTxHnd] = pContext->ilTxUpdateCounter[ilTxHnd];

#  if defined( VGEN_GENY ) && defined( IL_ENABLE_TX_DYNAMIC_CYCLETIME )
      ilTxDynCyclicCycles[ilTxHnd] = pContext->ilTxDynCyclicCycles[ilTxHnd];
#  endif
#  if defined( IL_ENABLE_TX_SECURE_EVENT )
      ilTxNSendCounter[ilTxHnd] = pContext->ilTxNSendCounter[ilTxHnd];
#  endif
#  if defined ( IL_ENABLE_TX_CYCLIC_EVENT ) || defined ( IL_ENABLE_TX_SECURE_EVENT ) || defined ( IL_ENABLE_TX_FAST_ON_START )
      ilTxEventCounter[ilTxHnd] = pContext->ilTxEventCounter[ilTxHnd];
#  endif
    }
#  if defined( IL_ENABLE_TX_CYCLIC_EVENT )
    ilIfActiveFlags = pContext->ilIfActiveFlags;
#  endif
#  if defined( IL_ENABLE_TX_TIMEOUT_FLAG )
    VStdMemCpyRamToNearRam(ilTxTimeoutFlags, pContext->ilTxTimeoutFlags, kIlNumberOfTxTimeoutFlags);
#  endif
#  if defined( IL_ENABLE_TX_CONFIRMATION_FLAG )
    VStdMemCpyRamToNearRam(ilTxConfirmationFlags, pContext->ilTxConfirmationFlags, kIlNumberOfTxConfirmationFlags);
#  endif
#  if defined( IL_ENABLE_TX_TIMEOUT )
    for (i = 0; i < (vuintx) kIlNumberOfTxTimeoutCounters; i++)
    {
      ilTxTimeoutCounter[i] = pContext->ilTxTimeoutCounter[i];
    }
#  endif
# endif /* IL_ENABLE_TX */

/* Copy Rx Dependent Context */
# if defined ( IL_ENABLE_RX )
#  if defined( IL_ENABLE_RX_TIMEOUT )
    for (i = 0; i < (vuintx) kIlNumberOfRxTimeoutCounters; i++)
    {
      ilRxTimeoutCounter[i] = pContext->ilRxTimeoutCounter[i];
#   if defined ( VGEN_GENY ) && defined( IL_ENABLE_RX_DYNAMIC_TIMEOUT )
      ilRxDynamicTimeoutCounter[i] = pContext->ilRxDynamicTimeoutCounter[i];
#   endif
    }
    VStdMemCpyRamToNearRam(ilRxTimerFlags, pContext->ilRxTimerFlags, kIlNumberOfTimerFlagBytes);
#  endif /* IL_ENABLE_RX_TIMEOUT */
#  if defined( IL_ENABLE_RX_DATACHANGED_FLAG )
    VStdMemCpyRamToNearRam(ilRxDataChangedFlags, pContext->ilRxDataChangedFlags, kIlNumberOfRxDataChangedFlags);
#  endif
#  if defined( IL_ENABLE_RX_INDICATION_FLAG )
    VStdMemCpyRamToNearRam(ilRxIndicationFlags, pContext->ilRxIndicationFlags, kIlNumberOfRxIndicationFlags);
#  endif
#  if defined( IL_ENABLE_RX_FIRSTVALUE_FLAG )
    VStdMemCpyRamToNearRam(ilRxFirstvalueFlags, pContext->ilRxFirstvalueFlags, kIlNumberOfRxFirstvalueFlags);
#  endif
#  if defined ( IL_ENABLE_RX_TIMEOUT_FLAG )
    VStdMemCpyRamToNearRam(ilRxTimeoutFlags, pContext->ilRxTimeoutFlags, kIlNumberOfRxTimeoutFlags);
#  endif
#  if !defined( VGEN_GENY ) && defined( IL_ENABLE_RX_MODE_SIGNALS )
#  endif
# endif /* IL_ENABLE_RX */


    return 1;
  }
  else
  {
    return 0;
  }
}                               /* End of IlSetModuleContext() */
#endif /* IL_ENABLE_SYS_SET_CONTEXT || VGEN_ENABLE_QWRAP */

/**********************************************************************************************************************
  CanCopyFromCan
**********************************************************************************************************************/
#if defined ( VGEN_CANGEN_VERSION ) && defined ( IL_ENABLE_RX_MODE_SIGNALS ) && !defined ( C_ENABLE_MEMCOPY_SUPPORT )
void CanCopyFromCan(void *dst, CanChipDataPtr src, vuint8 len)
{
  if (dst == V_NULL)
  {
    return;
  }

  while (len > 0)
  {
    len--;
    *((vuint8 *) dst) = *(src++);
    dst = ((vuint8 *) dst) + 1;
  }
}                               /* End of CanCopyFromCan() */
#endif


/**********************************************************************************************************************
  END OF FILE: Il.c
**********************************************************************************************************************/
/* STOPSINGLE_OF_MULTIPLE */
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
