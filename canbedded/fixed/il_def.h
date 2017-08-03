/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     il_def.h 
 *
 * description:   il_def
 *
 *
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           il for S12XEP100
 *
 *****************************************************************************/

#if !defined ( V_IL_DEF_COMPONENT_HEADER )
# define V_IL_DEF_COMPONENT_HEADER
/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/
/** \defgroup Il_VectorVersion Il_Vector version defines
    \brief The defines are used in the constant Table IlTxType and ilTxState to configure the send behaviour state of Tx messages. */
/**\{*/
/* ##V_CFG_MANAGEMENT ##CQProject : Il_Vector CQComponent : Implementation */
/** This is the Il_Vector bcd coded main- and subversion bcd. */
# define IL_VECTOR_VERSION          0x0502
/** This is the Il_Vector bcd coded releaseversion. */
# define IL_VECTOR_RELEASE_VERSION  0x01
/** This is the Il_Vector decimal Kernelbuilder build version. */
# define IL_VECTOR_BUILD_VERSION    50
/**\}*/

/** \defgroup CommonReturnValue Common Return Values
    \brief The defines are used to evaluate the return value of various Il_Vector functions. */
/**\{*/
/** The function returned successful. */
# define IL_OK                  0
/** The function returned, but the operation was interrupted.
    The function has been called either with and invalid argument or in an invalid call context. */
# define IL_ERROR              -3
/**\}*/

/** \defgroup IlStartCycleReturnValue IlStartCycle Return Values
    \brief The defines are used to evaluate the return value of IlStartCycle. */
/**\{*/
/** The message is already transmitted cyclically. */
# define IL_QUEUED             -2
/**\}*/

/** \defgroup IlBooleanReturnValue Boolean Return Value
    \brief The defines are used to evaluate the return value of the function with Il_Boolean return. */
/**\{*/
# define IL_FALSE            ((Il_Boolean)0)
# define IL_TRUE             ((Il_Boolean)1)
/**\}*/

/** \defgroup IlTxTypeValuesAndMasks IlTxType Values and Masks
    \brief The defines are used in the constant Table IlTxType and ilTxState to configure the send behaviour state of Tx messages. */
/**\{*/
/** This value is used, if nothing is configured for the message. */
# define kTxNoTxType           (vuint8)0x00
/** The message is transmitted cyclically. */
# define kTxSendCyclic         (vuint8)0x01
/** The message is currently transmitted fast (IfActive). */
# define kTxSendCyclicEvent    (vuint8)0x02
/** The timeout supervision for the message is activated. */
# define kTxCheckTimeout       (vuint8)0x04
/** The message is transmitted with repetition. */
# define kTxNSendRequest       (vuint8)0x08
/** The message is transmitted with the fast cycle rate if the start transition is performed for tx. */
# define kTxFastOnStart        (vuint8)0x10
/** The message will be transmitted if IlSendOnInitMsg is called. */
# define kTxQueueInit          (vuint8)0x20
/** The transmission request to the Data Link layer will be performed in the next IlTxTask call, if possible. */
# define kTxSendRequest        (vuint8)0x80

/** This mask is used to deactivate or decode kTxSendCyclic */
# define kTxNotSendCyclic      (vuint8)0xfe
/** This mask is used to deactivate or decode kTxSendCyclicEvent */
# define kTxNotSendCyclicEvent (vuint8)0xfd
/** This mask is used to deactivate or decode kTxCheckTimeout */
# define kTxNotCheckTimeout    (vuint8)0xfb
/** This mask is used to deactivate or decode kTxNSendRequest */
# define kTxNotNSendRequest    (vuint8)0xf7
/** This mask is used to deactivate or decode kTxFastOnStart */
# define kTxNotFastOnStart     (vuint8)0xef
/** This mask is used to deactivate or decode kTxQueueInit */
# define kTxNotQueueInit       (vuint8)0xdf
/** This mask is used to deactivate or decode kTxSendRequest */
# define kTxNotSendRequest     (vuint8)0x7f
/**\}*/

/** \defgroup ApplIlFatalErrorCodes ApplIlFatalError Error Codes
    \brief The defines are used as parameter for ApplIlFatalError. */
/**\{*/
/** The Tx message handle was not in a valid range. */
# define ILERR_ILLTXMSGHANDLE               (vuint8) 0x02
/** The Rx message handle was not in a valid range. */
# define ILERR_ILLRXMSGHANDLE               (vuint8) 0x03
/** The Tx timeout handle was not in a valid range. */
# define ILERR_ILLTXTIMEOUTINDEX            (vuint8) 0x12
/** The Rx timeout handle was not in a valid range. */
# define ILERR_ILLRXTIMEOUTINDEX            (vuint8) 0x13
/** The channel handle was not in a valid range. */
# define ILERR_ILLCHANNEL                   (vuint8) 0x17
/**\} */

/** \defgroup IL_ChannelStateValuesAndMasks IL_ChannelState Values and Masks
    \brief The defines are used to evaluate the current channel state. */
/**\{*/
/** The Rx and Tx state machine of this channel is in the suspended state. */
# define kIlIsSuspend     (vuint8)0x00
/** The Tx state machine of this channel is in the waiting state. */
# define kIlIsTxWait      (vuint8)0x01
/** The Tx state machine of this channel is in the running state. */
# define kIlIsTxRun       (vuint8)0x02
/** The Rx state machine of this channel is in the waiting state. */
# define kIlIsRxWait      (vuint8)0x04
/** The Rx state machine of this channel is in the running state. */
# define kIlIsRxRun       (vuint8)0x08
/** The Rx and Tx state machine of this channel is in the running state. */
# define kIlIsRxTxRun     (vuint8)0x0A

/** This mask is used to deactivate or decode kIlIsSuspend */
# define kIlIsNotSuspend     (vuint8)0xff
/** This mask is used to deactivate or decode kIlIsTxWait */
# define kIlIsNotTxWait      (vuint8)0xfe
/** This mask is used to deactivate or decode kIlIsTxRun */
# define kIlIsNotTxRun       (vuint8)0xfd
/** This mask is used to deactivate or decode kIlIsRxWait */
# define kIlIsNotRxWait      (vuint8)0xfb
/** This mask is used to deactivate or decode kIlIsRxRun */
# define kIlIsNotRxRun       (vuint8)0xf7
/** This mask is used to deactivate or decode kIlIsRxTxRun */
# define kIlIsNotRxTxRun     (vuint8)0xf5
/**\} */

/** \defgroup SpecialTableValues Reserved Array Constants
    \brief The defines are used to evaluate values in tables with reserved meanings. */
/**\{*/

/** If the IL Tx message handle has no Can Driver Tx handle, this value shall be used in the IlTxIndirection table.
    If the value is not defined by the Can Driver, the maximum value of the datatype CanTransmitHandle must be defined. */
# if !defined( kCanTxHandleNotUsed )
#  define kCanTxHandleNotUsed       ((CanTransmitHandle) 0xFFFFFFFF)
# endif

/** This value shall be used, if ilTxUpdateCounter of an IL Tx message handle is paused.
    The maximum value of the datatype IltTxUpdateCounter must be defined. */
# define kIlStopUpdateCounterValue  ((IltTxUpdateCounter)0xFFFFu)

# if defined ( VGEN_GENY )
/** This value shall be used, if the IL Tx handle has no IlTxTimeoutIndirection. */
#  define kIlNoTxToutIndirection               0x00u
/** This value shall be used, if the IL Tx handle has no IlTxFastOnStartDuration. */
#  define kIlNoFastOnStartDuration             0x01u
/** This value shall be used in IlTxStartCycles, if the IL Tx handle is not transmitted cyclically. */
#  define kIlNoCycleTime                       0x01u
/** This value shall be used in IlTxUpdateCycles, if the IL Tx handle has no delay time. */
#  define kIlNoDelayTime                       0x01u
/** This value shall be used in IlTxEventCycles, if the IL Tx handle cannot be transmitted with a fast cyclic period. */
#  define kIlNoCycleTimeFast                   0x01u
# endif/* VGEN_GENY */
/**\} */

/** \defgroup APIChannelAbstraction API Channel Abstraction
    \brief The defines are used to abstact single channel and multiple channel APIs. */
/**\{*/
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
#  define IL_CHANNEL_ILTYPE_ONLY          CanChannelHandle channel
#  define IL_CHANNEL_ILTYPE_FIRST         CanChannelHandle channel,
#  define IL_CHANNEL_ILTYPE_LOCAL         CanChannelHandle channel;
#  define IL_CHANNEL_ILPARA_ONLY          channel
#  define IL_CHANNEL_ILPARA_FIRST         channel,
#  define IL_CHANNEL_ILPARA_MACRO         channel
#  define IL_CHANNEL_ILPRECOPY_MACRO      rxStruct->Channel
# else
#  define IL_CHANNEL_ILTYPE_ONLY          void
#  define IL_CHANNEL_ILTYPE_FIRST
#  define IL_CHANNEL_ILTYPE_LOCAL
#  define IL_CHANNEL_ILPARA_ONLY
#  define IL_CHANNEL_ILPARA_FIRST
#  define IL_CHANNEL_ILPARA_MACRO         0
#  define IL_CHANNEL_ILPRECOPY_MACRO      0
# endif
/**\}*/

/** \defgroup CompatibilityMultipleNodes Multiple Nodes Compatibility
    \brief The defines are used to abstact the multiple nodes of CANGen and GENy. */
/**\{*/
# if defined ( VGEN_GENY )
#  if defined ( IL_ENABLE_SYS_MULTI_ECU_PHYS )
#   define IL_ENABLE_RX_MULTI_ECU_PHYS
#  else/* IL_ENABLE_SYS_MULTI_ECU_PHYS */
#   define IL_DISABLE_RX_MULTI_ECU_PHYS
#  endif
# endif
/**\} */

/** \defgroup CompatibilityAUTOSAR AUTOSAR Compatibility
    \brief The defines are used to abstact the IL, Can Driver interface to the AUTOSAR COM interface. */
/**\{*/
/**\} */

#  define IL_MEMROM1  V_MEMROM1
#  define IL_MEMROM2  V_MEMROM2
#  define IL_MEMROM3  V_MEMROM3

/**********************************************************************************************************************
  GLOBAL CHECK MACROS
**********************************************************************************************************************/
/* Compatibility Checks **********************************************************************************************/

# if defined ( VGEN_GENY )
/* Il_Vector - GenTool_Geny Compatibility Check */
#  if !defined( IL_VECTORDLL_VERSION )
#   error "Incompatible Il_Vector - GenTool_Geny with this Il_Vector-Implementation. IL_VECTORDLL_VERSION has to be defined!"
#  endif
#  if !defined( IL_VECTORDLL_RELEASE_VERSION )
#   error "Incompatible Il_Vector - GenTool_Geny with this Il_Vector-Implementation. IL_VECTORDLL_RELEASE_VERSION has to be defined!"
#  endif
#  if defined( IL_VECTORDLL_VERSION ) && defined( IL_VECTORDLL_RELEASE_VERSION )
#   if ( IL_VECTORDLL_VERSION < 0x0107u )
#    error "Incompatible Il_Vector - GenTool_Geny with this Il_Vector-Implementation!"
#   else
#   endif
#  endif

/* required to work around issue in MISRA checker tool */
#  define CANGENEXE_VERSION 0x0000u
#  define CANGENEXE_RELEASE_VERSION 0x00u

# endif

/* Il_Vector GenTool Compatibility Check */
# if ( IL_IMPLEMENTATION_VERSION != 0x0202u )
#  error "Incompatible IL implementation version!"
# endif


/* Common_Vdef-Implementation Compatibility Check */
# if !defined( COMMON_VDEF_VERSION )
#  error "Incompatible Common_Vdef-Implementation with this Il_Vector-Implementation. COMMON_VDEF_VERSION has to be defined!"
# endif
# if !defined( COMMON_VDEF_RELEASE_VERSION )
#  error "Incompatible Common_Vdef-Implementation with this Il_Vector-Implementation. COMMON_VDEF_RELEASE_VERSION has to be defined!"
# endif
# if defined( COMMON_VDEF_VERSION ) && defined( COMMON_VDEF_RELEASE_VERSION )
#  if ( COMMON_VDEF_VERSION < 0x0220u )
#   error "Incompatible Common_Vdef-Implementation with this Il_Vector-Implementation!"
#  else
#  endif
# endif

/* VStdLib Compatibility Check */
# if !defined( VGEN_ENABLE_VSTDLIB ) && !defined ( V_ENABLE_VSTDLIB )
#  error  "Error in File V_cfg.h: VGEN_ENABLE_VSTDLIB or V_ENABLE_VSTDLIB must be defined to use the Common_VStdLib-Implementation for this Il_Vector Implementation."
# endif
/* VStdLib__coreHll-Implementation Compatibility Check */
# if defined( VSTDLIB__COREHLL_VERSION )
#  if ( VSTDLIB__COREHLL_VERSION < 0x0203u )
#   error "Incompatible VStdLib__coreHll-Implementation with this Il_Vector-Implementation!"
#  endif
# else
#  error "Incompatible VStdLib__coreHll-Implementation with this Il_Vector-Implementation. VSTDLIB__COREHLL_VERSION has to be defined!"
# endif /* VSTDLIB__COREHLL_VERSION */
# if defined( VSTDLIB__COREHLL_RELEASE_VERSION )
# else
#  error "Incompatible VStdLib__coreHll-Implementation with this Il_Vector-Implementation. VSTDLIB__COREHLL_RELEASE_VERSION has to be defined!"
# endif /* VSTDLIB__COREHLL_RELEASE_VERSION */

/* Organi Check ******************************************************************************************************/

/* Oem Check *********************************************************************************************************/
# if defined ( VGEN_GENY )
# endif


# if defined( IL_ENABLE_SYS_GET_CONTEXT ) || defined( IL_ENABLE_SYS_SET_CONTEXT )

/* Check not supported Can Drivers */
#  if defined( C_SINGLE_RECEIVE_BUFFER ) || defined( C_MULTIPLE_RECEIVE_BUFFER )
#   error "The IlGetModuleContext/IlSetModuleContext interface is not supported code replicated Can Drivers!"
#  endif
#  if defined ( IL_ENABLE_TX_CYCLIC_EVENT )
#   error "The IlGetModuleContext/IlSetModuleContext Interface cannot be used with Messages and Signals, which have the sendtype IfActive!"
#  endif
/* Check not supported C_COMP switches */
# endif

/* Compatibility Check ***********************************************************************************************/


/* Vector CanDriver Interface Check Channel _Z *******************************************************************/
#  if !defined( XT_TX_DLC )
#   error  "Incompatible CanDriver: Missing Macro 'XT_TX_DLC'"
#  endif

#  if ( C_VERSION_REF_IMPLEMENTATION >= 0x120u )
#   if !defined( CanGetRxDataPtr )
#    error  "Incompatible CanDriver: Missing Macro 'CanGetRxDataPtr'"
#   endif
#   if !defined( CanGetRxDataLen )
#    error  "Incompatible CanDriver: Missing Macro 'CanGetRxDataLen'"
#   endif
#   if !defined( CanGetTxDlc )
#    error  "Incompatible CanDriver: Missing Macro 'CanGetTxDlc'"
#   endif
#   if !defined( CanGetTxDataPtr )
#    error  "Incompatible CanDriver: Missing Macro 'CanGetTxDataPtr'"
#   endif
#  endif
#  if !defined( CanGetConfirmationOffset )
#   error  "Incompatible CanDriver: Missing Macro 'CanGetConfirmationOffset'"
#  endif
#  if !defined( CanGetConfirmationMask )
#   error  "Incompatible CanDriver: Missing Macro 'CanGetConfirmationMask'"
#  endif
#  if !defined( CanGetIndicationOffset )
#   error  "Incompatible CanDriver: Missing Macro 'CanGetIndicationOffset'"
#  endif
#  if !defined( CanGetIndicationMask )
#   error  "Incompatible CanDriver: Missing Macro 'CanGetIndicationMask'"
#  endif
#  if defined ( C_ENABLE_CAN_CANCEL_NOTIFICATION ) && defined( C_MULTIPLE_RECEIVE_CHANNEL )
#   if !defined( CanGetChannelOfTxObj )
#    error  "Incompatible CanDriver: Missing Macro 'CanGetChannelOfTxObj'"
#   endif
#  endif

#  if defined( VGEN_GENY ) && ( C_VERSION_REF_IMPLEMENTATION < 0x130u )
#   error  "Incompatible CanDriver: The Reference Implementation must be higher than Version RI 1.3!"
#  endif

#  if !defined( VGEN_GENY )
#  endif

#  if ( C_VERSION_REF_IMPLEMENTATION >= 0x130u ) && !defined ( C_ENABLE_CAN_CANCEL_NOTIFICATION )
#   error  "Error in File Can_cfg.h: CAN Cancel Notification has to be enabled!"
#  endif

# if defined( VGEN_GENY )
/* Geny Compiler Switch - Constant Integrity Check *******************************************************************/


/* Compiler Switch - Constant Integrity Check */

/* Value Existance Check */
#  if !defined( kIlNumberOfChannels )
#   error  "Error in File Il_Cfg.h: kIlNumberOfChannels must be defined."
#  else
/* Value Range Check */
#   if ( kIlNumberOfChannels <= 0)
#    error  "Error in File Il_Cfg.h: kIlNumberOfChannels must be > 0."
#   endif
#  endif

/* Value Existance Check */
#  if !defined( kIlNumberOfIdentities )
#   error  "Error in File Il_Cfg.h: kIlNumberOfIdentities must be defined."
#  else
/* Value Range Check */
#   if ( kIlNumberOfIdentities <= 0)
#    error  "Error in File Il_Cfg.h: kIlNumberOfIdentities must be > 0."
#   endif
#  endif

#  if defined( IL_ENABLE_RX )
/* Value Existance Check */
#   if !defined( kIlNumberOfRxObjects )
#    error  "Error in File Il_Cfg.h: kIlNumberOfRxObjects must be defined if IL_ENABLE_RX is defined."
#   else
/* Value Range Check */
#    if ( kIlNumberOfRxObjects <= 0)
#     error  "Error in File Il_Cfg.h: kIlNumberOfRxObjects must be > 0 if IL_ENABLE_RX is defined."
#    endif
#   endif
/* Value Existance Check */
#   if !defined( kIlCanNumberOfRxObjects )
#    error  "Error in File Il_Cfg.h: kIlCanNumberOfRxObjects must be defined if IL_ENABLE_RX is defined."
#   else
/* Value Range Check */
#    if ( kIlCanNumberOfRxObjects <= 0)
#     error  "Error in File Il_Cfg.h: kIlCanNumberOfRxObjects must be > 0 if IL_ENABLE_RX is defined."
#    endif
#   endif
#  elif defined( IL_DISABLE_RX )
/* Value Existance Check */
#   if defined( kIlNumberOfRxObjects )
/* Value Range Check */
#    if ( kIlNumberOfRxObjects != 0 )
#     error  "Error in File Il_Cfg.h: kIlNumberOfRxObjects must be 0 if IL_DISABLE_RX is defined."
#    endif
#   endif
/* Value Existance Check */
#   if defined( kIlCanNumberOfRxObjects )
/* Value Range Check */
#    if ( kIlCanNumberOfRxObjects != 0 )
#     error  "Error in File Il_Cfg.h: kIlCanNumberOfRxObjects must be 0 if IL_DISABLE_RX is defined."
#    endif
#   endif
#  endif
       /* IL_ENABLE_RX */

#  if defined( IL_ENABLE_RX_INDICATION_FLAG )
/* Value Existance Check */
#   if !defined( kIlNumberOfRxIndicationFlags )
#    error  "Error in File Il_Cfg.h: kIlNumberOfRxIndicationFlags must be defined if IL_ENABLE_RX_INDICATION_FLAG is defined."
#   else
/* Value Range Check */
#    if ( kIlNumberOfRxIndicationFlags <= 0)
#     error  "Error in File Il_Cfg.h: kIlNumberOfRxIndicationFlags must be > 0 if IL_ENABLE_RX_INDICATION_FLAG is defined."
#    endif
#   endif
/* Value Existance Check */
#   if !defined( kIlNumberOfRxIndicationBits )
#    error  "Error in File Il_Cfg.h: kIlNumberOfRxIndicationBits must be defined if IL_ENABLE_RX_INDICATION_FLAG is defined."
#   else
/* Value Range Check */
#    if ( kIlNumberOfRxIndicationBits <= 0)
#     error  "Error in File Il_Cfg.h: kIlNumberOfRxIndicationBits must be > 0 if IL_ENABLE_RX_INDICATION_FLAG is defined."
#    endif
#   endif
#  elif defined( IL_DISABLE_RX_INDICATION_FLAG )
/* Value Existance Check */
#   if defined( kIlNumberOfRxIndicationFlags )
/* Value Range Check */
#    if ( kIlNumberOfRxIndicationFlags != 0 )
#     error  "Error in File Il_Cfg.h: kIlNumberOfRxIndicationFlags must be 0 if IL_DISABLE_RX_INDICATION_FLAG is defined."
#    endif
#   endif
/* Value Existance Check */
#   if defined( kIlNumberOfRxIndicationBits )
/* Value Range Check */
#    if ( kIlNumberOfRxIndicationBits != 0 )
#     error  "Error in File Il_Cfg.h: kIlNumberOfRxIndicationBits must be 0 if IL_DISABLE_RX_INDICATION_FLAG is defined."
#    endif
#   endif
#  endif
       /* IL_ENABLE_RX_INDICATION_FLAG */

#  if defined( IL_ENABLE_RX_FIRSTVALUE_FLAG )
/* Value Existance Check */
#   if !defined( kIlNumberOfRxFirstvalueFlags )
#    error  "Error in File Il_Cfg.h: kIlNumberOfRxFirstvalueFlags must be defined if IL_ENABLE_RX_FIRSTVALUE_FLAG is defined."
#   else
/* Value Range Check */
#    if ( kIlNumberOfRxFirstvalueFlags <= 0)
#     error  "Error in File Il_Cfg.h: kIlNumberOfRxFirstvalueFlags must be > 0 if IL_ENABLE_RX_FIRSTVALUE_FLAG is defined."
#    endif
#   endif
#  elif defined( IL_DISABLE_RX_FIRSTVALUE_FLAG )
/* Value Existance Check */
#   if defined( kIlNumberOfRxFirstvalueFlags )
/* Value Range Check */
#    if ( kIlNumberOfRxFirstvalueFlags != 0 )
#     error  "Error in File Il_Cfg.h: kIlNumberOfRxFirstvalueFlags must be 0 if IL_DISABLE_RX_FIRSTVALUE_FLAG is defined."
#    endif
#   endif
#  endif
       /* IL_ENABLE_RX_FIRSTVALUE_FLAG */

#  if defined( IL_ENABLE_RX_DATACHANGED_FLAG )
/* Value Existance Check */
#   if !defined( kIlNumberOfRxDataChangedFlags )
#    error  "Error in File Il_Cfg.h: kIlNumberOfRxDataChangedFlags must be defined if IL_ENABLE_RX_DATACHANGED_FLAG is defined."
#   else
/* Value Range Check */
#    if ( kIlNumberOfRxDataChangedFlags <= 0)
#     error  "Error in File Il_Cfg.h: kIlNumberOfRxDataChangedFlags must be > 0 if IL_ENABLE_RX_DATACHANGED_FLAG is defined."
#    endif
#   endif
#  elif defined( IL_DISABLE_RX_DATACHANGED_FLAG )
/* Value Existance Check */
#   if defined( kIlNumberOfRxDataChangedFlags )
/* Value Range Check */
#    if ( kIlNumberOfRxDataChangedFlags != 0 )
#     error  "Error in File Il_Cfg.h: kIlNumberOfRxDataChangedFlags must be 0 if IL_DISABLE_RX_DATACHANGED_FLAG is defined."
#    endif
#   endif
#  endif
       /* IL_ENABLE_RX_DATACHANGED_FLAG */

#  if defined( IL_ENABLE_RX_TIMEOUT )
/* Value Existance Check */
#   if !defined( kIlNumberOfRxTimeoutCounters )
#    error  "Error in File Il_Cfg.h: kIlNumberOfRxTimeoutCounters must be defined if IL_ENABLE_RX_TIMEOUT is defined."
#   else
/* Value Range Check */
#    if ( kIlNumberOfRxTimeoutCounters <= 0)
#     error  "Error in File Il_Cfg.h: kIlNumberOfRxTimeoutCounters must be > 0 if IL_ENABLE_RX_TIMEOUT is defined."
#    endif
#   endif
/* Value Existance Check */
#   if !defined( kIlNumberOfTimerFlagBytes )
#    error  "Error in File Il_Cfg.h: kIlNumberOfTimerFlagBytes must be defined if IL_ENABLE_RX_TIMEOUT is defined."
#   else
/* Value Range Check */
#    if ( kIlNumberOfTimerFlagBytes <= 0)
#     error  "Error in File Il_Cfg.h: kIlNumberOfTimerFlagBytes must be > 0 if IL_ENABLE_RX_TIMEOUT is defined."
#    endif
#   endif
#  elif defined( IL_DISABLE_RX_TIMEOUT )
/* Value Existance Check */
#   if defined( kIlNumberOfRxTimeoutCounters )
/* Value Range Check */
#    if ( kIlNumberOfRxTimeoutCounters != 0 )
#     error  "Error in File Il_Cfg.h: kIlNumberOfRxTimeoutCounters must be 0 if IL_DISABLE_RX_TIMEOUT is defined."
#    endif
#   endif
/* Value Existance Check */
#   if defined( kIlNumberOfTimerFlagBytes )
/* Value Range Check */
#    if ( kIlNumberOfTimerFlagBytes != 0 )
#     error  "Error in File Il_Cfg.h: kIlNumberOfTimerFlagBytes must be 0 if IL_DISABLE_RX_TIMEOUT is defined."
#    endif
#   endif
#  endif
       /* IL_ENABLE_RX_TIMEOUT */

#  if defined( IL_ENABLE_RX_TIMEOUT_FLAG )
/* Value Existance Check */
#   if !defined( kIlNumberOfRxTimeoutFlags )
#    error  "Error in File Il_Cfg.h: kIlNumberOfRxTimeoutFlags must be defined if IL_ENABLE_RX_TIMEOUT_FLAG is defined."
#   else
/* Value Range Check */
#    if ( kIlNumberOfRxTimeoutFlags <= 0)
#     error  "Error in File Il_Cfg.h: kIlNumberOfRxTimeoutFlags must be > 0 if IL_ENABLE_RX_TIMEOUT_FLAG is defined."
#    endif
#   endif
#  elif defined( IL_DISABLE_RX_TIMEOUT_FLAG )
/* Value Existance Check */
#   if defined( kIlNumberOfRxTimeoutFlags )
/* Value Range Check */
#    if ( kIlNumberOfRxTimeoutFlags != 0 )
#     error  "Error in File Il_Cfg.h: kIlNumberOfRxTimeoutFlags must be 0 if IL_DISABLE_RX_TIMEOUT_FLAG is defined."
#    endif
#   endif
#  endif
       /* IL_ENABLE_RX_TIMEOUT_FLAG */

#  if defined( IL_ENABLE_TX )
/* Value Existance Check */
#   if !defined( kIlNumberOfTxObjects )
#    error  "Error in File Il_Cfg.h: kIlNumberOfTxObjects must be defined if IL_ENABLE_TX is defined."
#   else
/* Value Range Check */
#    if ( kIlNumberOfTxObjects <= 0)
#     error  "Error in File Il_Cfg.h: kIlNumberOfTxObjects must be > 0 if IL_ENABLE_TX is defined."
#    endif
#   endif
#  elif defined( IL_DISABLE_TX )
/* Value Existance Check */
#   if defined( kIlNumberOfTxObjects )
/* Value Range Check */
#    if ( kIlNumberOfTxObjects != 0 )
#     error  "Error in File Il_Cfg.h: kIlNumberOfTxObjects must be 0 if IL_DISABLE_TX is defined."
#    endif
#   endif
#  endif
       /* IL_ENABLE_TX */

#  if defined( IL_ENABLE_TX_CONFIRMATION_FLAG )
/* Value Existance Check */
#   if !defined( kIlNumberOfTxConfirmationFlags )
#    error  "Error in File Il_Cfg.h: kIlNumberOfTxConfirmationFlags must be defined if IL_ENABLE_TX_CONFIRMATION_FLAG is defined."
#   else
/* Value Range Check */
#    if ( kIlNumberOfTxConfirmationFlags <= 0)
#     error  "Error in File Il_Cfg.h: kIlNumberOfTxConfirmationFlags must be > 0 if IL_ENABLE_TX_CONFIRMATION_FLAG is defined."
#    endif
#   endif
#  elif defined( IL_DISABLE_TX_CONFIRMATION_FLAG )
/* Value Existance Check */
#   if defined( kIlNumberOfTxConfirmationFlags )
/* Value Range Check */
#    if ( kIlNumberOfTxConfirmationFlags != 0 )
#     error  "Error in File Il_Cfg.h: kIlNumberOfTxConfirmationFlags must be 0 if IL_DISABLE_TX_CONFIRMATION_FLAG is defined."
#    endif
#   endif
#  endif
       /* IL_ENABLE_TX_CONFIRMATION_FLAG */

#  if defined( IL_DISABLE_TX_VARYING_REPETITION )
#  elif defined( IL_ENSABLE_TX_VARYING_REPETITION )
#   if defined( kIlNumberOfTxConfirmationFlags )
#    if ( kIlNumberOfTxConfirmationFlags != 0 )
#     error  "Error in File Il_Cfg.h: kIlNumberOfTxConfirmationFlags must be 0 if IL_ENSABLE_TX_VARYING_REPETITION is defined."
#    endif
#   endif
#  endif
       /* IL_ENABLE_TX_VARYING_REPETITION */

#  if defined( IL_ENABLE_TX_TIMEOUT )
/* Value Existance Check */
#   if !defined( kIlNumberOfTxTimeoutCounters )
#    error  "Error in File Il_Cfg.h: kIlNumberOfTxTimeoutCounters must be defined if IL_ENABLE_TX_TIMEOUT is defined."
#   else
/* Value Range Check */
#    if ( kIlNumberOfTxTimeoutCounters <= 0)
#     error  "Error in File Il_Cfg.h: kIlNumberOfTxTimeoutCounters must be > 0 if IL_ENABLE_TX_TIMEOUT is defined."
#    endif
#   endif
#  elif defined( IL_DISABLE_TX_TIMEOUT )
/* Value Existance Check */
#   if defined( kIlNumberOfTxTimeoutCounters )
/* Value Range Check */
#    if ( kIlNumberOfTxTimeoutCounters != 0 )
#     error  "Error in File Il_Cfg.h: kIlNumberOfTxTimeoutCounters must be 0 if IL_DISABLE_TX_TIMEOUT is defined."
#    endif
#   endif
#  endif
       /* IL_ENABLE_TX_TIMEOUT */

#  if defined( IL_ENABLE_TX_TIMEOUT_FLAG )
/* Value Existance Check */
#   if !defined( kIlNumberOfTxTimeoutFlags )
#    error  "Error in File Il_Cfg.h: kIlNumberOfTxTimeoutFlags must be defined if IL_ENABLE_TX_TIMEOUT_FLAG is defined."
#   else
/* Value Range Check */
#    if ( kIlNumberOfTxTimeoutFlags <= 0)
#     error  "Error in File Il_Cfg.h: kIlNumberOfTxTimeoutFlags must be > 0 if IL_ENABLE_TX_TIMEOUT_FLAG is defined."
#    endif
#   endif
#  elif defined( IL_DISABLE_TX_TIMEOUT_FLAG )
/* Value Existance Check */
#   if defined( kIlNumberOfTxTimeoutFlags )
/* Value Range Check */
#    if ( kIlNumberOfTxTimeoutFlags != 0 )
#     error  "Error in File Il_Cfg.h: kIlNumberOfTxTimeoutFlags must be 0 if IL_DISABLE_TX_TIMEOUT_FLAG is defined."
#    endif
#   endif
#  endif
       /* IL_ENABLE_TX_TIMEOUT_FLAG */


/* Compiler Switch Integrity Check */

#  if defined( IL_ENABLE_RX ) && defined( IL_DISABLE_RX )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_RX' and 'IL_DISABLE_RX' is defined."
#  endif
#  if defined( IL_ENABLE_RX_DATACHANGED_FLAG ) && defined( IL_DISABLE_RX_DATACHANGED_FLAG )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_RX_DATACHANGED_FLAG' and 'IL_DISABLE_RX_DATACHANGED_FLAG' is defined."
#  endif
#  if defined( IL_ENABLE_RX_DEFAULTVALUE ) && defined( IL_DISABLE_RX_DEFAULTVALUE )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_RX_DEFAULTVALUE' and 'IL_DISABLE_RX_DEFAULTVALUE' is defined."
#  endif
#  if defined( IL_ENABLE_RX_DYNAMIC_TIMEOUT ) && defined( IL_DISABLE_RX_DYNAMIC_TIMEOUT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_RX_DYNAMIC_TIMEOUT' and 'IL_DISABLE_RX_DYNAMIC_TIMEOUT' is defined."
#  endif
#  if defined( IL_ENABLE_RX_FIRSTVALUE_FLAG ) && defined( IL_DISABLE_RX_FIRSTVALUE_FLAG )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_RX_FIRSTVALUE_FLAG' and 'IL_DISABLE_RX_FIRSTVALUE_FLAG' is defined."
#  endif
#  if defined( IL_ENABLE_RX_INDICATION_FLAG ) && defined( IL_DISABLE_RX_INDICATION_FLAG )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_RX_INDICATION_FLAG' and 'IL_DISABLE_RX_INDICATION_FLAG' is defined."
#  endif
#  if defined( IL_ENABLE_RX_INTERRUPT ) && defined( IL_DISABLE_RX_INTERRUPT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_RX_INTERRUPT' and 'IL_DISABLE_RX_INTERRUPT' is defined."
#  endif
#  if defined( IL_ENABLE_RX_MODE_SIGNALS ) && defined( IL_DISABLE_RX_MODE_SIGNALS )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_RX_MODE_SIGNALS' and 'IL_DISABLE_RX_MODE_SIGNALS' is defined."
#  endif
#  if defined( IL_ENABLE_RX_POLLING ) && defined( IL_DISABLE_RX_POLLING )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_RX_POLLING' and 'IL_DISABLE_RX_POLLING' is defined."
#  endif
#  if defined( IL_ENABLE_RX_SIGNAL_START_FCT ) && defined( IL_DISABLE_RX_SIGNAL_START_FCT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_RX_SIGNAL_START_FCT' and 'IL_DISABLE_RX_SIGNAL_START_FCT' is defined."
#  endif
#  if defined( IL_ENABLE_RX_SIGNAL_STOP_FCT ) && defined( IL_DISABLE_RX_SIGNAL_STOP_FCT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_RX_SIGNAL_STOP_FCT' and 'IL_DISABLE_RX_SIGNAL_STOP_FCT' is defined."
#  endif
#  if defined( IL_ENABLE_RX_START_DEFAULTVALUE ) && defined( IL_DISABLE_RX_START_DEFAULTVALUE )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_RX_START_DEFAULTVALUE' and 'IL_DISABLE_RX_START_DEFAULTVALUE' is defined."
#  endif
#  if defined( IL_ENABLE_RX_STOP_DEFAULTVALUE ) && defined( IL_DISABLE_RX_STOP_DEFAULTVALUE )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_RX_STOP_DEFAULTVALUE' and 'IL_DISABLE_RX_STOP_DEFAULTVALUE' is defined."
#  endif
#  if defined( IL_ENABLE_RX_TIMEOUT ) && defined( IL_DISABLE_RX_TIMEOUT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_RX_TIMEOUT' and 'IL_DISABLE_RX_TIMEOUT' is defined."
#  endif
#  if defined( IL_ENABLE_RX_TIMEOUT_DELAY ) && defined( IL_DISABLE_RX_TIMEOUT_DELAY )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_RX_TIMEOUT_DELAY' and 'IL_DISABLE_RX_TIMEOUT_DELAY' is defined."
#  endif
#  if defined( IL_ENABLE_RX_TIMEOUT_FLAG ) && defined( IL_DISABLE_RX_TIMEOUT_FLAG )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_RX_TIMEOUT_FLAG' and 'IL_DISABLE_RX_TIMEOUT_FLAG' is defined."
#  endif
#  if defined( IL_ENABLE_SYS_ARGCHECK ) && defined( IL_DISABLE_SYS_ARGCHECK )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_SYS_ARGCHECK' and 'IL_DISABLE_SYS_ARGCHECK' is defined."
#  endif
#  if defined( IL_ENABLE_SYS_INIT_FCT ) && defined( IL_DISABLE_SYS_INIT_FCT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_SYS_INIT_FCT' and 'IL_DISABLE_SYS_INIT_FCT' is defined."
#  endif
#  if defined ( VGEN_GENY )
#   if defined( IL_ENABLE_SYS_MULTI_ECU_PHYS ) && defined( IL_DISABLE_SYS_MULTI_ECU_PHYS )
#    error "Error in File Il_Cfg.h: 'IL_ENABLE_SYS_MULTI_ECU_PHYS' and 'IL_DISABLE_SYS_MULTI_ECU_PHYS' is defined."
#   endif
#  endif
#  if defined( IL_ENABLE_SYS_RX_START_FCT ) && defined( IL_DISABLE_SYS_RX_START_FCT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_SYS_RX_START_FCT' and 'IL_DISABLE_SYS_RX_START_FCT' is defined."
#  endif
#  if defined( IL_ENABLE_SYS_RX_STOP_FCT ) && defined( IL_DISABLE_SYS_RX_STOP_FCT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_SYS_RX_STOP_FCT' and 'IL_DISABLE_SYS_RX_STOP_FCT' is defined."
#  endif
#  if defined( IL_ENABLE_SYS_SIGNAL_INIT_FCT ) && defined( IL_DISABLE_SYS_SIGNAL_INIT_FCT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_SYS_SIGNAL_INIT_FCT' and 'IL_DISABLE_SYS_SIGNAL_INIT_FCT' is defined."
#  endif
#  if defined( IL_ENABLE_SYS_TESTDEBUG ) && defined( IL_DISABLE_SYS_TESTDEBUG )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_SYS_TESTDEBUG' and 'IL_DISABLE_SYS_TESTDEBUG' is defined."
#  endif
#  if defined( IL_ENABLE_TX ) && defined( IL_DISABLE_TX )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX' and 'IL_DISABLE_TX' is defined."
#  endif
#  if defined( IL_ENABLE_TX_CONFIRMATION_FLAG ) && defined( IL_DISABLE_TX_CONFIRMATION_FLAG )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_CONFIRMATION_FLAG' and 'IL_DISABLE_TX_CONFIRMATION_FLAG' is defined."
#  endif
#  if defined( IL_ENABLE_TX_CYCLIC_EVENT ) && defined( IL_DISABLE_TX_CYCLIC_EVENT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_CYCLIC_EVENT' and 'IL_DISABLE_TX_CYCLIC_EVENT' is defined."
#  endif
#  if defined( IL_ENABLE_TX_DEFAULTVALUE ) && defined( IL_DISABLE_TX_DEFAULTVALUE )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_DEFAULTVALUE' and 'IL_DISABLE_TX_DEFAULTVALUE' is defined."
#  endif
#  if defined( IL_ENABLE_TX_DYNAMIC_CYCLETIME ) && defined( IL_DISABLE_TX_DYNAMIC_CYCLETIME )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_DYNAMIC_CYCLETIME' and 'IL_DISABLE_TX_DYNAMIC_CYCLETIME' is defined."
#  endif
#  if defined( IL_ENABLE_TX_FAST_ON_START ) && defined( IL_DISABLE_TX_FAST_ON_START )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_FAST_ON_START' and 'IL_DISABLE_TX_FAST_ON_START' is defined."
#  endif
#  if defined( IL_ENABLE_TX_INTERRUPT ) && defined( IL_DISABLE_TX_INTERRUPT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_INTERRUPT' and 'IL_DISABLE_TX_INTERRUPT' is defined."
#  endif
#  if defined( IL_ENABLE_TX_MODE_SIGNALS ) && defined( IL_DISABLE_TX_MODE_SIGNALS )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_MODE_SIGNALS' and 'IL_DISABLE_TX_MODE_SIGNALS' is defined."
#  endif
#  if defined( IL_ENABLE_TX_POLLING ) && defined( IL_DISABLE_TX_POLLING )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_POLLING' and 'IL_DISABLE_TX_POLLING' is defined."
#  endif
#  if defined( IL_ENABLE_TX_SECURE_EVENT ) && defined( IL_DISABLE_TX_SECURE_EVENT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_SECURE_EVENT' and 'IL_DISABLE_TX_SECURE_EVENT' is defined."
#  endif
#  if defined( IL_ENABLE_TX_SEND_ON_INIT ) && defined( IL_DISABLE_TX_SEND_ON_INIT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_SEND_ON_INIT' and 'IL_DISABLE_TX_SEND_ON_INIT' is defined."
#  endif
#  if defined( IL_ENABLE_TX_SIGNAL_START_FCT ) && defined( IL_DISABLE_TX_SIGNAL_START_FCT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_SIGNAL_START_FCT' and 'IL_DISABLE_TX_SIGNAL_START_FCT' is defined."
#  endif
#  if defined( IL_ENABLE_TX_SIGNAL_STOP_FCT ) && defined( IL_DISABLE_TX_SIGNAL_STOP_FCT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_SIGNAL_STOP_FCT' and 'IL_DISABLE_TX_SIGNAL_STOP_FCT' is defined."
#  endif
#  if defined( IL_ENABLE_TX_START_DEFAULTVALUE ) && defined( IL_DISABLE_TX_START_DEFAULTVALUE )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_START_DEFAULTVALUE' and 'IL_DISABLE_TX_START_DEFAULTVALUE' is defined."
#  endif
#  if defined( IL_ENABLE_TX_STARTSTOP_CYCLIC ) && defined( IL_DISABLE_TX_STARTSTOP_CYCLIC )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_STARTSTOP_CYCLIC' and 'IL_DISABLE_TX_STARTSTOP_CYCLIC' is defined."
#  endif
#  if defined( IL_ENABLE_TX_STOP_DEFAULTVALUE ) && defined( IL_DISABLE_TX_STOP_DEFAULTVALUE )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_STOP_DEFAULTVALUE' and 'IL_DISABLE_TX_STOP_DEFAULTVALUE' is defined."
#  endif
#  if defined( IL_ENABLE_TX_TIMEOUT ) && defined( IL_DISABLE_TX_TIMEOUT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_TIMEOUT' and 'IL_DISABLE_TX_TIMEOUT' is defined."
#  endif
#  if defined( IL_ENABLE_TX_TIMEOUT_FLAG ) && defined( IL_DISABLE_TX_TIMEOUT_FLAG )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_TIMEOUT_FLAG' and 'IL_DISABLE_TX_TIMEOUT_FLAG' is defined."
#  endif
#  if defined( IL_ENABLE_TX_VARYING_REPETITION ) && defined( IL_DISABLE_TX_VARYING_REPETITION )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_VARYING_REPETITION' and 'IL_DISABLE_TX_VARYING_REPETITION' is defined."
#  endif
#  if defined( IL_ENABLE_TX_VARYING_TIMEOUT ) && defined( IL_DISABLE_TX_VARYING_TIMEOUT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_TX_VARYING_TIMEOUT' and 'IL_DISABLE_TX_VARYING_TIMEOUT' is defined."
#  endif
#  if defined( IL_ENABLE_SYS_TX_START_FCT ) && defined( IL_DISABLE_SYS_TX_START_FCT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_SYS_TX_START_FCT' and 'IL_DISABLE_SYS_TX_START_FCT' is defined."
#  endif
#  if defined( IL_ENABLE_SYS_TX_STOP_FCT ) && defined( IL_DISABLE_SYS_TX_STOP_FCT )
#   error "Error in File Il_Cfg.h: 'IL_ENABLE_SYS_TX_STOP_FCT' and 'IL_DISABLE_SYS_TX_STOP_FCT' is defined."
#  endif

# endif/* VGEN_GENY */

/* Feature Compatibility *********************************************************************************************/
# if defined ( VGEN_GENY )
#  if defined ( IL_ENABLE_SYS_MULTIPLE_NODES )
#   error "The compiler switch IL_ENABLE_SYS_MULTIPLE_NODES is not used any more in Geny by Il_Vector!"
#  endif
# endif

/* Hardware Compiler Compatibility Check *****************************************************************************/
# if defined ( VGEN_GENY )

/* Check supported C_COMP switches for VGEN_GENY */

# endif/* VGEN_GENY */

# if !defined ( IL_MEMROM1 )
#  error "IL_MEMROM1 must be defined!"
# endif
# if !defined ( IL_MEMROM2 )
#  error "IL_MEMROM2 must be defined!"
# endif
# if !defined ( IL_MEMROM3 )
#  error "IL_MEMROM3 must be defined!"
# endif

/**********************************************************************************************************************
  GLOBAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  IlGetRxMsgStartIndex
**********************************************************************************************************************/
/** \brief    This method returns the first Il Rx message handle of the channel
              in a kIlNumberOfRxObjects sorted table.
    \param    channel      Handle of the CAN channel.
    \return   IlReceiveHandle  Il Rx message handle
**********************************************************************************************************************/
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
#  define IlGetRxMsgStartIndex(channel)      (IlRxMsgStartIndex[channel])
# else
#  define IlGetRxMsgStartIndex(channel)      0
# endif

/**********************************************************************************************************************
  IlGetTxMsgStartIndex
**********************************************************************************************************************/
/** \brief    This method returns the first IL Tx message handle of the channel
              in a kIlNumberOfTxObjects sorted table.
    \param    channel      Handle of the CAN channel.
    \return   IlTransmitHandle  Il Tx message handle
**********************************************************************************************************************/
# if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
#  define IlGetTxMsgStartIndex(channel)      (IlTxMsgStartIndex[channel])
# else
#  define IlGetTxMsgStartIndex(channel)      0
# endif

/**********************************************************************************************************************
  Il_ChannelState
**********************************************************************************************************************/
/** \brief    This method serves to access the ilChannelState variable of a channel.
    \param    channel      Handle of the CAN channel.
    \return   vuint8       This variable contains the current state of a channel bit coded.
                           Use the DecodeChannelState and ModifyChannelState macros to extract
                           the current state of the channel.
**********************************************************************************************************************/
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
#  define Il_ChannelState(channel)    (ilChannelState[(channel)])
# else
#  define Il_ChannelState(channel)    (ilChannelState)
# endif

# if defined( IL_ENABLE_TX ) && defined( IL_ENABLE_TX_SECURE_EVENT )
/**********************************************************************************************************************
  IlGetTxRepetitionCounter
**********************************************************************************************************************/
/** \brief    This method returns the number of repetitions for an IL Tx handle.
    \param    ilTxHnd      Handle of an IL Tx message.
    \return   IltTxRepetitionCounter  Number of Repetitions.
**********************************************************************************************************************/
#  if defined ( VGEN_GENY )
#   if defined( IL_ENABLE_TX_VARYING_REPETITION )
#    define IlGetTxRepetitionCounter(ilTxHnd)       (IlTxRepetitionCounters[ilTxHnd])
#   else
#    define IlGetTxRepetitionCounter(ilTxHnd)       ((IltTxRepetitionCounter) kIlNumberOfTxRepetitions)
#   endif
#  endif/* VGEN_GENY */
# endif/* IL_ENABLE_TX && IL_ENABLE_TX_SECURE_EVENT */

/**********************************************************************************************************************
  DecodeChannelState
**********************************************************************************************************************/
/** \defgroup DecodeChannelState Decode Il_ChannelState
    \brief    These macros shall be used to decode the return value of Il_ChannelState.
    \param    channel      Handle of the CAN channel.
    \return   (vuint8) 0 : The channel is not in the state.\n
              > 0        : The channel is in the state.
*/
/**\{*/
/** Rx is suspended  */
# define RxSuspended(channel) ((Il_ChannelState((channel)) & (kIlIsRxRun | kIlIsRxWait))==0)
/** Rx is running  */
# define RxEnabled(channel)   ((Il_ChannelState((channel)) & kIlIsRxRun) != 0)
/**\} */


/**********************************************************************************************************************
  IlGetStatusReturnValue
**********************************************************************************************************************/
/** \defgroup IlGetStatusReturnValue Decode IlGetStatus Return Value
    \brief    These macros shall be used to decode the return value of IlGetStatus.
    \param    state           Il_Status returned from IlGetStatus.
    \return   (Il_Status) 0 : The channel is not in the state.\n
              > 0           : The channel is in the state.
*/
/**\{*/
/** Tx is running */
# define IlIsTxRun(state)          (((state) & kIlIsTxRun)               ? 1 : 0)
/** Tx is waiting */
# define IlIsTxWait(state)         (((state) & kIlIsTxWait)              ? 1 : 0)
/** Tx is suspended */
# define IlIsTxSuspend(state)      (((state) & (kIlIsTxWait|kIlIsTxRun)) ? 0 : 1)
/** Rx is running */
# define IlIsRxRun(state)          (((state) & kIlIsRxRun)               ? 1 : 0)
/** Rx is waiting */
# define IlIsRxWait(state)         (((state) & kIlIsRxWait)              ? 1 : 0)
/** Rx is suspended */
# define IlIsRxSuspend(state)      (((state) & (kIlIsRxWait|kIlIsRxRun)) ? 0 : 1)
/**\} */

# if defined ( VGEN_GENY )
/** IL Abstraction of CanGlobalInterruptDisable. */
#  define IlEnterCriticalFlagAccess() CanGlobalInterruptDisable()
/** IL Abstraction of CanGlobalInterruptRestore. */
#  define IlLeaveCriticalFlagAccess() CanGlobalInterruptRestore()

#  if defined( IL_ENABLE_RX_TIMEOUT ) && defined( IL_ENABLE_RX_DYNAMIC_TIMEOUT )
/**********************************************************************************************************************
  IlConvertMs2RxToutCnt
**********************************************************************************************************************/
/** \brief    This method calculates the number of IlRxTask calls in a channel for a Rx timeout time.
    \param    ilChannelCycles  cycle time of IlRxTask calls in milliseconds
    \param    msTime           Rx timeout time time in milliseconds
    \return   IltRxTimeoutCounter   The Rx timeout counter in multiples of IlRxTask calls of this channel.
**********************************************************************************************************************/
#   define IlConvertMs2RxToutCnt(ilChannelCycles, msTime)         ((IltRxTimeoutCounter) ((msTime) / (ilChannelCycles)))
/**********************************************************************************************************************
  IlConvertRxToutCnt2Ms
**********************************************************************************************************************/
/** \brief    This method returns currently set ilRxDynamicTimeoutCounter in milliseconds.
    \param    ilChannelCycles  cycle time of IlRxTask calls in milliseconds
    \param    ilRxTimeoutHnd   Rx timeout handle
    \return   The current Rx timeout in milliseconds for the Rx timeout handle
**********************************************************************************************************************/
#   define IlConvertRxToutCnt2Ms(ilChannelCycles, ilRxTimeoutHnd) (ilRxDynamicTimeoutCounter[(ilRxTimeoutHnd)] * (ilChannelCycles))
#  endif

#  if defined( IL_ENABLE_TX_DYNAMIC_CYCLETIME )
/**********************************************************************************************************************
  IlConvertChannelCyclesAndMs2TxTicks
**********************************************************************************************************************/
/** \brief    This method returns currently set ilRxDynamicTimeoutCounter in milliseconds.
    \param    ilChannelCycles  cycle time of IlRxTask calls in milliseconds
    \param    ilMsTime         Rx timeout handle
    \return   IltTxCounter     The current Rx timeout in milliseconds for the Rx timeout handle
**********************************************************************************************************************/
#   define IlConvertChannelCyclesAndMs2TxTicks(ilChannelCycles, ilMsTime)    ((IltTxCounter) ((ilMsTime) / (ilChannelCycles)))

/**********************************************************************************************************************
  IlSetDynTxCycleTimeMs
**********************************************************************************************************************/
/** \brief    Sets the cycle time of an il message.

              The macro is called from the application. The function has no effect, if the il message handle is not 
              transmitted cyclically. This is defined in the dbc file with the attibutes GenMsgSendType and
              GenSigSendType. Please refer to the technical reference for details. To activate the cyclic transmission
              of an il message handle use the il API function IlStartCycle.
  
    \param_i ilChannelCycles : This value is the call cycle time of the IlTxTask in the current channel in
                               milliseconds. The value is generated to the file il_cfg.h as define
                               e.g. for channel 1 kIlTxCycleTime_1
    \param   ilTxHnd       This value is the handle of the Tx message. The generated file il_par.h contains generated
                           defines for the message handles.
    \param   ilMsTime      This value is the time in milliseconds, until the cyclic event is raised. The value has to
                           be smaller than 65534.
    \return  none
    \context The macro must be called within the Il callback function ApplIlInit or in the signal oriented init callback function.
    \warning Do not use the macro for handles containing multiplexed signals.
**********************************************************************************************************************/
#   if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
#    define IlSetDynTxCycleTimeMs(ilChannelCycles, ilTxHnd, ilMsTime)          IlSetDynTxCycleTimeRaw((ilTxHnd), IlConvertChannelCyclesAndMs2TxTicks((ilChannelCycles), (ilMsTime)))
#   else
#    define IlSetDynTxCycleTimeMs(ilTxHnd, ilMsTime)                           IlSetDynTxCycleTimeRaw((ilTxHnd), IlConvertChannelCyclesAndMs2TxTicks((kIlTxCycleTime), (ilMsTime)))
#   endif

#  endif/* IL_ENABLE_TX_DYNAMIC_CYCLETIME */
# endif/* VGEN_GENY */

/** \ingroup CompatibilityAUTOSAR */
/**\{*/
#  if defined ( IL_ENABLE_TX )
#   define IlGetTxConfirmationFlags(x)   (CanConfirmationFlags._c[x])
#   define IlGetTxConfirmationMask(x)    (CanGetConfirmationMask(x))
#   define IlGetTxConfirmationOffset(x)  (CanGetConfirmationOffset(x))
#  endif

#  if defined ( IL_ENABLE_RX ) && defined ( IL_ENABLE_RX_POLLING )
#   define IlGetRxIndicationFlags(x)     (CanIndicationFlags._c[x])
#   define IlGetRxIndicationMask(x)      (CanGetIndicationMask(x))
#   define IlGetRxIndicationOffset(x)    (CanGetIndicationOffset(x))
#  endif
/**\}*/

/**********************************************************************************************************************
  GLOBAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/
typedef void (*IlTxModeCopyFct) (void);
typedef void (*IlFaultRecoveryFct) (void);
typedef void (*IlIndicationFct) (void);
typedef void (*IlTimeoutIndicationFct) (void);
typedef void (*IlConfirmationFct) (void);
typedef vsint8 Il_Status;
typedef vsint8 Il_Boolean;

# if defined ( VGEN_GENY )
#  if ( kIlNumberOfTxObjects < 256 )
typedef vuint8 IlTransmitHandle;
#  endif
#  if ( kIlNumberOfTxObjects >= 256 ) && ( kIlNumberOfTxObjects < 65536 )
typedef vuint16 IlTransmitHandle;
#  endif
#  if ( kIlNumberOfTxObjects >= 65536 )
#   error "The Maximum Number of supported Tx Objects is exceeded"
#  endif

#  if ( kIlNumberOfRxObjects < 256 )
typedef vuint8 IlReceiveHandle;
#  endif
#  if ( kIlNumberOfRxObjects >= 256 ) && ( kIlNumberOfRxObjects < 65536 )
typedef vuint16 IlReceiveHandle;
#  endif
#  if ( kIlNumberOfRxObjects >= 65536 )
#   error "The Maximum Number of supported Rx Objects is exceeded"
#  endif

#  if defined ( IL_ENABLE_RX_INDICATION_FLAG )
#   if ( kIlNumberOfRxIndicationBits < 256 )
typedef vuint8 IlReceiveFlagHandle;
#   endif
#   if ( kIlNumberOfRxIndicationBits >= 256 ) && ( kIlNumberOfRxIndicationBits < 65536 )
typedef vuint16 IlReceiveFlagHandle;
#   endif
#   if ( kIlNumberOfRxIndicationBits >= 65536 )
#    error "The Maximum Number of supported Rx Objects is exceeded"
#   endif
#  endif/* IL_ENABLE_RX_INDICATION_FLAG */

# endif/* VGEN_GENY */

# if defined ( IL_ENABLE_RX_TIMEOUT )
#  if ( kIlNumberOfRxTimeoutCounters < 255 )
typedef vuint8 IlRxTimeoutHandle;
#  endif
#  if ( kIlNumberOfRxTimeoutCounters >= 255 ) && ( kIlNumberOfRxTimeoutCounters < 65535 )
typedef vuint16 IlRxTimeoutHandle;
#  endif
#  if ( kIlNumberOfRxTimeoutCounters >= 65535 )
#   error "The Maximum Number of supported Rx Timeout Counters is exceeded!"
#  endif
# endif/* IL_ENABLE_RX_TIMEOUT */


# if defined( IL_ENABLE_SYS_GET_CONTEXT ) || defined( IL_ENABLE_SYS_SET_CONTEXT )

/* Il_Vector context structure */
typedef struct
{
  vuint32 ilMagicNumber;

#  if defined ( IL_ENABLE_RX_MULTIPLE_NODES )
  vuintx ilECUMask;
#  endif

#  if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && ( kIlNumberOfChannels > 1 )
  vuint8 ilChannelState[kIlNumberOfChannels];
#   if defined( IL_ENABLE_RX_POLLING ) 
  vuint8 ilChannelReceive[kIlNumberOfChannels];
#   endif
#  else
  vuint8 ilChannelState;
#   if defined( IL_ENABLE_RX_POLLING ) 
  vuint8 ilChannelReceive;
#   endif
#  endif

#  if defined ( IL_ENABLE_TX )
  vuint8 ilTxState[kIlNumberOfTxObjects];
  IltTxCounter ilTxCyclicCounter[kIlNumberOfTxObjects];
  IltTxUpdateCounter ilTxUpdateCounter[kIlNumberOfTxObjects];
#   if defined( IL_ENABLE_TX_CYCLIC_EVENT )
#    if defined( VGEN_GENY )
  IlTIfActiveFlag ilIfActiveFlags;
#    endif
#   endif                       /* IL_ENABLE_TX_CYCLIC_EVENT */
#   if defined( VGEN_GENY ) && defined( IL_ENABLE_TX_DYNAMIC_CYCLETIME )
  IltTxCounter ilTxDynCyclicCycles[kIlNumberOfTxObjects];
#   endif
#   if defined( IL_ENABLE_TX_TIMEOUT_FLAG )
  vuint8 ilTxTimeoutFlags[kIlNumberOfTxTimeoutFlags];
#   endif
#   if defined( IL_ENABLE_TX_CONFIRMATION_FLAG )
  vuint8 ilTxConfirmationFlags[kIlNumberOfTxConfirmationFlags];
#   endif
#   if defined( IL_ENABLE_TX_SECURE_EVENT )
  IltTxRepetitionCounter ilTxNSendCounter[kIlNumberOfTxObjects];
#   endif
#   if defined( IL_ENABLE_TX_TIMEOUT )
  IltTxTimeoutCounter ilTxTimeoutCounter[kIlNumberOfTxTimeoutCounters];
#   endif
#   if defined ( IL_ENABLE_TX_CYCLIC_EVENT ) || defined ( IL_ENABLE_TX_SECURE_EVENT ) || defined ( IL_ENABLE_TX_FAST_ON_START )
  IltTxCounter ilTxEventCounter[kIlNumberOfTxObjects__Ch];
#   endif
#  endif                        /* IL_ENABLE_TX */

#  if defined ( IL_ENABLE_RX )
#   if defined( IL_ENABLE_RX_TIMEOUT )
  IltRxTimeoutCounter ilRxTimeoutCounter[kIlNumberOfRxTimeoutCounters];
  vuint8 ilRxTimerFlags[kIlNumberOfTimerFlagBytes];
#    if defined ( VGEN_GENY ) && defined( IL_ENABLE_RX_DYNAMIC_TIMEOUT )
  IltRxTimeoutCounter ilRxDynamicTimeoutCounter[kIlNumberOfRxTimeoutCounters];
#    endif
#   endif                       /* IL_ENABLE_RX_TIMEOUT */
#   if defined( IL_ENABLE_RX_DATACHANGED_FLAG )
  vuint8 ilRxDataChangedFlags[kIlNumberOfRxDataChangedFlags];
#   endif
#   if defined( IL_ENABLE_RX_INDICATION_FLAG )
  vuint8 ilRxIndicationFlags[kIlNumberOfRxIndicationFlags];
#   endif
#   if defined( IL_ENABLE_RX_FIRSTVALUE_FLAG )
  vuint8 ilRxFirstvalueFlags[kIlNumberOfRxFirstvalueFlags];
#   endif
#   if defined ( IL_ENABLE_RX_TIMEOUT_FLAG )
  vuint8 ilRxTimeoutFlags[kIlNumberOfRxTimeoutFlags];
#   endif
#   if !defined( VGEN_GENY ) && defined( IL_ENABLE_RX_MODE_SIGNALS )
#   endif
#  endif                        /* IL_ENABLE_RX */

}
tIlModuleContextStruct;

typedef tIlModuleContextStruct *tIlModuleContextStructPtr;

# endif/* IL_ENABLE_SYS_GET_CONTEXT || IL_ENABLE_SYS_SET_CONTEXT */

/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/
/** This variable contains the communication state of each channel. */
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 ilChannelState[kIlNumberOfChannels];
# else
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 ilChannelState;
# endif

# if defined ( IL_ENABLE_RX )


#  if defined( IL_ENABLE_RX_TIMEOUT )
/** This variable contains the Rx timeout counter. */
V_MEMRAM0 extern V_MEMRAM1 IltRxTimeoutCounter V_MEMRAM2 ilRxTimeoutCounter[kIlNumberOfRxTimeoutCounters];

#   if defined( VGEN_GENY ) && defined( IL_ENABLE_RX_DYNAMIC_TIMEOUT )
/** This variable contains the dynamic Rx timeout counter. */
V_MEMRAM0 extern V_MEMRAM1 IltRxTimeoutCounter V_MEMRAM2 ilRxDynamicTimeoutCounter[kIlNumberOfRxTimeoutCounters];
#   endif


/** This variable contains a Rx timeout flag for each Rx timeout counter to indicate the reception of the timeout supervised message. */
V_MEMRAM0 extern V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR ilRxTimerFlags[kIlNumberOfTimerFlagBytes];

#  endif/* IL_ENABLE_RX_TIMEOUT */

#  if defined ( IL_ENABLE_RX_INDICATION_FLAG )
/** This variable contains each configured Rx signal indication flag, to indicate the reception of a signal. */
V_MEMRAM0 extern V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR ilRxIndicationFlags[kIlNumberOfRxIndicationFlags];

#  endif

#  if defined ( IL_ENABLE_RX_FIRSTVALUE_FLAG )
/** This variable contains each configured Rx signal firstvalue flag, to indicate the first reception of a signal. */
V_MEMRAM0 extern V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR ilRxFirstvalueFlags[kIlNumberOfRxFirstvalueFlags];

#  endif

#  if defined ( IL_ENABLE_RX_TIMEOUT_FLAG )
/** This variable contains each configured Rx signal timeout flag. */
V_MEMRAM0 extern V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR ilRxTimeoutFlags[kIlNumberOfRxTimeoutFlags];

#  endif

#  if defined ( IL_ENABLE_RX_DATACHANGED_FLAG )
/** This variable contains each configured Rx signal datachanged flag, to indicate the change of the signal value. */
V_MEMRAM0 extern V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR ilRxDataChangedFlags[kIlNumberOfRxDataChangedFlags];

#  endif

#  if !defined (VGEN_GENY) && defined( IL_ENABLE_RX_MODE_SIGNALS )
#  endif

# endif/* IL_ENABLE_RX */

# if defined ( IL_ENABLE_TX )

/** This variable contains several states and functionalities bit coded for each Tx message.
  76543210
  |||||||+--- Send cyclic  is active
  ||||||+---- cyclic-event is active
  |||||+----- Tx-Timeout check active
  ||||+------ Send request for a single event
  |||+------- manuell cyclic-event is active
  ||+-------- Message is allowed for queueing on-init/reinit of VN
  |+--------- unused
  +---------- Internal send-request for IL-queue.
*/
V_MEMRAM0 extern V_MEMRAM1_NEAR volatile vuint8 V_MEMRAM2_NEAR ilTxState[kIlNumberOfTxObjects];

/** This variable contains the Tx updated counter. If the value is !=0, the transmission of messages is not allowed. */
V_MEMRAM0 extern V_MEMRAM1 IltTxUpdateCounter V_MEMRAM2 ilTxUpdateCounter[kIlNumberOfTxObjects];

#  if defined ( IL_ENABLE_TX_TIMEOUT )
/** This variable contains the Tx timeout counter. */
V_MEMRAM0 extern V_MEMRAM1 IltTxTimeoutCounter V_MEMRAM2 ilTxTimeoutCounter[kIlNumberOfTxTimeoutCounters];
#  endif
#  if defined( IL_ENABLE_TX_SECURE_EVENT )
/** This variable contains the number of transmit repetitions to be performed for each Tx message. */
V_MEMRAM0 extern V_MEMRAM1 IltTxRepetitionCounter V_MEMRAM2 ilTxNSendCounter[kIlNumberOfTxObjects];
#  endif


#  if defined( IL_ENABLE_TX_CYCLIC_EVENT )
#   if defined( IL_ENABLE_SYS_GET_CONTEXT ) || defined( IL_ENABLE_SYS_SET_CONTEXT )
#    if defined( VGEN_GENY )
/** This variable contains for each Tx ifactive signal an ifactive flag. */
V_MEMRAM0 extern V_MEMRAM1_NEAR IlTIfActiveFlag V_MEMRAM2_NEAR ilIfActiveFlags;
#    endif
#   endif
#  endif

#  if defined ( IL_ENABLE_TX_TIMEOUT_FLAG )
/** This variable contains each configured Tx signal timeout flag, to indicate the timeout of a signal. */
V_MEMRAM0 extern V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR ilTxTimeoutFlags[kIlNumberOfTxTimeoutFlags];
#  endif

#  if defined ( IL_ENABLE_TX_CONFIRMATION_FLAG )
/** This variable contains each configured Tx signal confirmation flag, to indicate the successful transmission of a signal. */
V_MEMRAM0 extern V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR ilTxConfirmationFlags[kIlNumberOfTxConfirmationFlags];
#  endif
# endif/* IL_ENABLE_TX */

/** This variable contains the bcd coded main version of the Il_Vector. */
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kIlMainVersion;
/** This variable contains the bcd coded sub version of the Il_Vector. */
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kIlSubVersion;
/** This variable contains the bcd coded release version of the Il_Vector. */
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kIlReleaseVersion;

# if defined ( IL_ENABLE_RX )

#  if defined( IL_ENABLE_RX_MODE_SIGNALS ) || defined( IL_ENABLE_SYS_MULTI_ECU_PHYS ) 
/** This constant contains the length of each Rx message. */
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 IlRxDataLen[kIlNumberOfRxObjects];
/** This constant references the data pointer of each Rx message buffer. */
V_MEMROM0 extern V_MEMROM1 RxDataPtr V_MEMROM2 IlRxDataPtr[kIlNumberOfRxObjects];
#  else
#   if !defined ( VGEN_GENY ) && ( C_VERSION_REF_IMPLEMENTATION < 0x120u )
#   endif
#  endif

#  if defined( IL_ENABLE_RX_DEFAULTVALUE )
/** This constant references the data pointer to an array with the length of the Rx message, which contains the default values for all signals. Undefined signals or gaps have the value 0. */
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 V_MEMROM3 *V_MEMROM1 V_MEMROM2
  IlRxDefaultInitValue[kIlNumberOfRxObjects];
#  endif
#  if defined ( IL_ENABLE_RX_START_DEFAULTVALUE )
/** This constant references the data pointer to an array with the length of the Rx message.
    The Value referenced by IlRxDefaultInitValue is valid at the transition start, if the referenced IlRxDefaultStartMask has at the bit position the value 1. */
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 V_MEMROM3 *V_MEMROM1 V_MEMROM2
  IlRxDefaultStartMask[kIlNumberOfRxObjects];
#  endif
#  if defined ( IL_ENABLE_RX_STOP_DEFAULTVALUE )
/** This constant references the data pointer to an array with the length of the Rx message.
    The Value referenced by IlRxDefaultInitValue is valid at the transition stop, if the referenced IlRxDefaultStopMask has at the bit position the value 1. */
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 V_MEMROM3 *V_MEMROM1 V_MEMROM2
  IlRxDefaultStopMask[kIlNumberOfRxObjects];
#  endif

#  if defined ( IL_ENABLE_RX_POLLING )
/** This constant references a IL internal indication function for each Rx messsage if the function is required and generated, else V_NULL. */
V_MEMROM0 extern V_MEMROM1 IlIndicationFct V_MEMROM2 IlCanRxIndicationFctPtr[kIlCanNumberOfRxObjects];
#  endif

#  if defined ( IL_ENABLE_RX_INDICATION_FLAG )
/** This constant contains the byte offset of the table ilRxIndicationFlags for each indication bit. */
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 IlIndicationOffset[kIlNumberOfRxIndicationBits];
/** This constant contains a mask for each indication bit, to decode the indication bit in the table ilRxIndicationFlags. */
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 IlIndicationMask[kIlNumberOfRxIndicationBits];
#  endif

#  if defined(IL_ENABLE_RX_TIMEOUT)
/** This constant references a IL internal timeout function for each Rx timeout counter if the function is required and generated, else V_NULL. */
V_MEMROM0 extern V_MEMROM1 IlTimeoutIndicationFct V_MEMROM2 IlRxTimeoutFctPtr[kIlNumberOfRxTimeoutCounters];
#   if defined(IL_ENABLE_SYS_MULTI_ECU_PHYS)
/** This constant contains Rx timeout counter for each Rx timeout counter in each identity of an ECU. */
V_MEMROM0 extern V_MEMROM1 IltRxTimeoutCounter V_MEMROM2
  IlRxTimeoutTbl[kIlNumberOfIdentities][kIlNumberOfRxTimeoutCounters];
#   else
/** This constant contains Rx timeout counter for each Rx timeout counter, if the ECU has one identity. */
V_MEMROM0 extern V_MEMROM1 IltRxTimeoutCounter V_MEMROM2 IlRxTimeoutTbl[kIlNumberOfRxTimeoutCounters];
#   endif
#  endif

#  if defined ( VGEN_GENY )
#   if defined( IL_ENABLE_RX_MULTI_ECU_PHYS )
/** This constant contains for each IL message in the CAN Driver table a mapping to the current active identity.
    If the LSB2MSB bitindex(IdentityIndex) is set to 1, the Rx message is active, else not. */
V_MEMROM0 extern V_MEMROM1 tVIdentityMsk V_MEMROM2 IlCanRxIdentityAssignment[kIlCanNumberOfRxObjects];
#   endif
#  endif

#  if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
/** This constant contains for each channel the startindex of the first message in
    kIlNumberOfRxObjcets ordered arrays. The last index is kIlNumberOfRxObjcets. */
V_MEMROM0 extern V_MEMROM1 IlReceiveHandle V_MEMROM2 IlRxMsgStartIndex[kIlNumberOfChannels + 1];
#   if defined ( IL_ENABLE_RX_POLLING ) 
/** This constant contains for each channel the startindex of the first Can Driver message in
    kIlCanNumberOfRxObjects ordered arrays. The last index is kIlCanNumberOfRxObjects. */
V_MEMROM0 extern V_MEMROM1 CanReceiveHandle V_MEMROM2 IlCanRxMsgStartIndex[kIlNumberOfChannels + 1];
#   endif
#   if defined( IL_ENABLE_RX_TIMEOUT )
/** This constant contains for each channel the startindex of the first timeout counter in
    kIlNumberOfRxTimeoutCounters ordered arrays. The last index is kIlNumberOfRxTimeoutCounters. */
V_MEMROM0 extern V_MEMROM1 IlReceiveHandle V_MEMROM2 IlRxTimeoutCntStartIndex[kIlNumberOfChannels + 1];
#   endif
#   if defined( IL_ENABLE_RX_TIMEOUT )
/** This constant contains for each channel the startindex of the first timer flag byte in
    kIlNumberOfTimerFlagBytes ordered arrays. The last index is kIlNumberOfTimerFlagBytes. */
V_MEMROM0 extern V_MEMROM1 IlReceiveHandle V_MEMROM2 IlRxTimerFlagsStartIndex[kIlNumberOfChannels + 1];
#   endif
#   if defined( IL_ENABLE_RX_TIMEOUT_FLAG )
/** This constant contains for each channel the startindex of the first timeout flag byte in
    kIlNumberOfRxTimeoutFlags ordered arrays. The last index is kIlNumberOfRxTimeoutFlags. */
V_MEMROM0 extern V_MEMROM1 IlReceiveHandle V_MEMROM2 IlRxTimeoutFlagsStartIndex[kIlNumberOfChannels + 1];
#   endif
#   if defined( IL_ENABLE_RX_FIRSTVALUE_FLAG )
/** This constant contains for each channel the startindex of the first firstvalue flag byte in
    kIlNumberOfRxFirstvalueFlags ordered arrays. The last index is kIlNumberOfRxFirstvalueFlags. */
V_MEMROM0 extern V_MEMROM1 IlReceiveHandle V_MEMROM2 IlRxFirstvalueFlagsStartIndex[kIlNumberOfChannels + 1];
#   endif
#   if defined( IL_ENABLE_RX_INDICATION_FLAG )
/** This constant contains for each channel the startindex of the first indication flag byte in
    kIlNumberOfRxIndicationFlags ordered arrays. The last index is kIlNumberOfRxIndicationFlags. */
V_MEMROM0 extern V_MEMROM1 IlReceiveHandle V_MEMROM2 IlRxIndicationFlagsStartIndex[kIlNumberOfChannels + 1];
#   endif
#   if defined( IL_ENABLE_RX_DATACHANGED_FLAG )
/** This constant contains for each channel the startindex of the first datachanged flag byte in
    kIlNumberOfRxDataChangedFlags ordered arrays. The last index is kIlNumberOfRxDataChangedFlags. */
V_MEMROM0 extern V_MEMROM1 IlReceiveHandle V_MEMROM2 IlRxDataChangedFlagsStartIndex[kIlNumberOfChannels + 1];
#   endif
#  endif

# endif/* defined ( IL_ENABLE_RX ) */

# if defined ( IL_ENABLE_TX )

#  if defined( IL_ENABLE_TX_MODE_SIGNALS ) 
/** This constant contains the length of each Tx message. */
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 IlTxDLC[kIlNumberOfTxObjects];
/** This constant references the data pointer of each Tx message buffer. */
V_MEMROM0 extern V_MEMROM1 TxDataPtr V_MEMROM2 IlTxDataPtr[kIlNumberOfTxObjects];
#  else
#   if !defined ( VGEN_GENY ) && ( C_VERSION_REF_IMPLEMENTATION < 0x120u )
#   endif
#  endif

#  if defined ( VGEN_GENY ) && defined( IL_ENABLE_TX_SECURE_EVENT ) && defined( IL_ENABLE_TX_VARYING_REPETITION )
V_MEMROM0 extern V_MEMROM1 IltTxRepetitionCounter V_MEMROM2 IlTxRepetitionCounters[kIlNumberOfTxObjects];
#  endif

#  if defined ( IL_ENABLE_TX_START_DEFAULTVALUE )
/** This constant references the data pointer to an array with the length of the Tx message, which contains the default values for all signals. Undefined signals or gaps have the value 0. */
V_MEMROM0 extern IL_MEMROM1 vuint8 IL_MEMROM2 IL_MEMROM3 *V_MEMROM1 V_MEMROM2
  IlTxDefaultStartMask[kIlNumberOfTxObjects];
#  endif
#  if defined( IL_ENABLE_TX_DEFAULTVALUE )
/** This constant references the data pointer to an array with the length of the Tx message.
    The Value referenced by IlTxDefaultInitValue is valid at the transition start, if the referenced IlTxDefaultStartMask has at the bit position the value 1. */
V_MEMROM0 extern IL_MEMROM1 vuint8 IL_MEMROM2 IL_MEMROM3 *V_MEMROM1 V_MEMROM2
  IlTxDefaultInitValue[kIlNumberOfTxObjects];
#  endif
#  if defined ( IL_ENABLE_TX_STOP_DEFAULTVALUE )
/** This constant references the data pointer to an array with the length of the Tx message.
    The Value referenced by IlTxDefaultInitValue is valid at the transition stop, if the referenced IlTxDefaultStopMask has at the bit position the value 1. */
V_MEMROM0 extern IL_MEMROM1 vuint8 IL_MEMROM2 IL_MEMROM3 *V_MEMROM1 V_MEMROM2
  IlTxDefaultStopMask[kIlNumberOfTxObjects];
#  endif

/** This constant contains the predefined sendtype of all Tx messages. */
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 IlTxType[kIlNumberOfTxObjects];


/** This constant contains the GenMsgCycleTime of all Tx messages in multiples of IlTxTask calls. */
V_MEMROM0 extern V_MEMROM1 IltTxCounter V_MEMROM2 IlTxCyclicCycles[kIlNumberOfTxObjects];

/** This constant contains the GenMsgStartDelayTime of all Tx messages in multiples of IlTxTask calls. */
V_MEMROM0 extern V_MEMROM1 IltTxCounter V_MEMROM2 IlTxStartCycles[kIlNumberOfTxObjects];

/** This constant contains the GenMsgDelayTime of all Tx messages in multiples of IlTxTask calls. */
V_MEMROM0 extern V_MEMROM1 IltTxUpdateCounter V_MEMROM2 IlTxUpdateCycles[kIlNumberOfTxObjects];

#   if defined ( IL_ENABLE_TX_CYCLIC_EVENT ) || defined ( IL_ENABLE_TX_SECURE_EVENT ) || defined ( IL_ENABLE_TX_FAST_ON_START )
/** This constant contains the GenMsgCycleTimeFast of all Tx messages in multiples of IlTxTask calls. */
V_MEMROM0 extern V_MEMROM1 IltTxCounter V_MEMROM2 IlTxEventCycles[kIlNumberOfTxObjects];
#   endif

#  if defined ( IL_ENABLE_TX_FAST_ON_START )
/** This constant contains the calculated GenMsgFastOnStart of all Tx messages in multiples of IlTxTask calls.
    The GenMsgStartDelayTime is taken additional into account. */
V_MEMROM0 extern V_MEMROM1 IltTxCounter V_MEMROM2 IlTxFastOnStartDuration[kIlNumberOfTxObjects];
#  endif

#  if defined ( VGEN_GENY ) && defined ( IL_ENABLE_TX_FAST_ON_START ) && defined( IL_ENABLE_TX_MODE_SIGNALS )
/** This constant contains calculated mode delay offset times for multiplexor value specific Tx handles of the IL in multiples of IlTxTask calls.
    The values are used to equalize the mode cycles in the transition from the GenMsgFastOnStart time to the normal cyclic transmission. */
V_MEMROM0 extern V_MEMROM1 IltTxCounter V_MEMROM2 IlTxFastOnStartMuxDelay[kIlNumberOfTxObjects];
#  endif

#  if defined ( IL_ENABLE_TX_POLLING )
/** This constant references a IL internal confirmation function for each Tx messsage if the function is required and generated, else V_NULL. */
V_MEMROM0 extern V_MEMROM1 IlConfirmationFct V_MEMROM2 IlTxConfirmationFctPtr[kIlNumberOfTxObjects];
#  endif

#  if defined( IL_ENABLE_TX_TIMEOUT )
/** This constant references for each Tx messsage the Tx timeout handle. The value in this table for the Tx message
    is only valid, if kTxCheckTimeout is set in the ilTxState array is set. */
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 IlTxTimeoutIndirection[kIlNumberOfTxObjects];
#  endif

#  if defined(IL_ENABLE_SYS_MULTI_ECU_PHYS)
/** This constant contains for each il Tx message the Can Driver Tx handle in each identity of an ECU. */
V_MEMROM0 extern V_MEMROM1 CanTransmitHandle V_MEMROM2
  IlTxIndirection[kIlNumberOfIdentities][kIlNumberOfTxObjects];
#  else
/** This constant contains for each il Tx message the Can Driver Tx handle, if the ECU has one identity. */
V_MEMROM0 extern V_MEMROM1 CanTransmitHandle V_MEMROM2 IlTxIndirection[kIlNumberOfTxObjects];
#  endif

#  if defined ( IL_ENABLE_TX_MODE_SIGNALS )
/** This constant references for multiplexor value specific Tx handles of the IL functions to copy the multiplexor value specific data. */
V_MEMROM0 extern V_MEMROM1 IlTxModeCopyFct V_MEMROM2 IlTxModeCopyFunctions[kIlNumberOfTxObjects];
#  endif

#  if defined( IL_ENABLE_SYS_MULTI_ECU_PHYS )
/** This constant contains for each IL message a mapping to the current active identity. 
    If the LSB2MSB bitindex(IdentityIndex) is set to 1, the Rx message is active, else not. */
V_MEMROM0 extern V_MEMROM1 tVIdentityMsk V_MEMROM2 IlTxIdentityAssignment[kIlNumberOfTxObjects];
#  endif

#  if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && (kIlNumberOfChannels > 1)
/** This constant contains for each channel the startindex of the first message in
    kIlNumberOfTxObjcets ordered arrays. The last index is kIlNumberOfTxObjcets. */
V_MEMROM0 extern V_MEMROM1 IlTransmitHandle V_MEMROM2 IlTxMsgStartIndex[kIlNumberOfChannels + 1];
#   if defined( IL_ENABLE_TX_TIMEOUT )
/** This constant contains for each channel the startindex of the first Tx timeout counter in
    kIlNumberOfTxTimeoutCounters ordered arrays. The last index is kIlNumberOfTxTimeoutCounters. */
V_MEMROM0 extern V_MEMROM1 IlTransmitHandle V_MEMROM2 IlTxTimeoutCntStartIndex[kIlNumberOfChannels + 1];
#   endif
#   if defined( IL_ENABLE_TX_CONFIRMATION_FLAG )
/** This constant contains for each channel the startindex of the first Tx confirmation flag in
    kIlNumberOfTxConfirmationFlags ordered arrays. The last index is kIlNumberOfTxConfirmationFlags. */
V_MEMROM0 extern V_MEMROM1 IlTransmitHandle V_MEMROM2 IlTxConfirmationFlagsStartIndex[kIlNumberOfChannels + 1];
#   endif
#   if defined( IL_ENABLE_TX_TIMEOUT_FLAG )
/** This constant contains for each channel the startindex of the first Tx timeout flag in
    kIlNumberOfTxTimeoutFlags ordered arrays. The last index is kIlNumberOfTxTimeoutFlags. */
V_MEMROM0 extern V_MEMROM1 IlTransmitHandle V_MEMROM2 IlTxTimeoutFlagsStartIndex[kIlNumberOfChannels + 1];
#   endif
#   if defined ( VGEN_GENY ) && defined( IL_ENABLE_TX_TIMEOUT ) && defined ( IL_ENABLE_TX_VARYING_TIMEOUT )
/** This constant contains the calculated ILTxTimeout of each channel in multiples of IlTxTask calls. */
V_MEMROM0 extern V_MEMROM1 IltTxTimeoutCounter V_MEMROM2 IlTxTimeout[kIlNumberOfChannels];
#   endif
#  endif

# endif/* defined ( IL_ENABLE_TX ) */




/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  IlInitPowerOn
**********************************************************************************************************************/
/** \brief    This method initializes the Il_Vector on all channels.\n
              IlInit is called for every channel.
    \return   none
    \context  The function must be called with disabled interrupts.\n
              The function must not interrupt IlRxTask, IlRxStateTask, IlTxTask, IlTxStateTask,
              IlInit, IlRxStart, IlTxStart, IlRxStop, IlTxStop.
    \note     The function is called by the Application or Ccl (Communication Control Layer).
**********************************************************************************************************************/
extern void IlInitPowerOn(void);

/**********************************************************************************************************************
  IlInit
**********************************************************************************************************************/
/** \brief    This method initializes the Il_Vector on a channel.\n
              Rx and Tx data buffers and flags are set to the initial state. If no default value for a message is
              defined, the data buffer is set to 0x00.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function must be called with disabled interrupts.\n
              The function must not interrupt IlRxTask, IlRxStateTask, IlTxTask, IlTxStateTask,
              IlInitPowerOn, IlRxStart, IlTxStart, IlRxStop, IlTxStop.
    \note     The function is called by the Application, Ccl (Communication Control Layer) or IlInitPowerOn.
**********************************************************************************************************************/
extern void IlInit(IL_CHANNEL_ILTYPE_ONLY);

/**********************************************************************************************************************
  IlRxStart
**********************************************************************************************************************/
/** \brief    This method enables the reception of messages.
              The transition "start" of the Rx state machine is performed.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function must be called on task level.\n
              The function must not interrupt IlRxTask, IlRxStateTask, IlTxTask, IlTxStateTask, 
              IlInitPowerOn, IlInit, IlTxStart, IlRxStop, IlTxStop.
    \note     The function is called by the Application or Nm (Network Management).
**********************************************************************************************************************/
extern void IlRxStart(IL_CHANNEL_ILTYPE_ONLY);

/**********************************************************************************************************************
  IlTxStart
**********************************************************************************************************************/
/** \brief    This method enables the transmission of messages and starts the transmission of periodic messages.
              The transition "start" of the Tx state machine is performed.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function must be called on task level.\n
              The function must not interrupt IlRxTask, IlRxStateTask, IlTxTask, IlTxStateTask, 
              IlInitPowerOn, IlInit, IlTxStart, IlRxStop, IlTxStop.
    \note     The function is called by the Application or Nm (Network Management).
**********************************************************************************************************************/
extern void IlTxStart(IL_CHANNEL_ILTYPE_ONLY);

/**********************************************************************************************************************
  IlRxStop
**********************************************************************************************************************/
/** \brief    This method disables the reception of messages.
              The transition "stop" of the Rx state machine is performed.
              The method is used for example to enter the Sleep Mode of an ECU.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function must be called on task level.\n
              The function must not interrupt IlRxTask, IlRxStateTask, IlTxTask, IlTxStateTask, 
              IlInitPowerOn, IlInit, IlTxStart, IlRxStop, IlTxStop.
    \note     The function is called by the Application or Nm (Network Management).
**********************************************************************************************************************/
extern void IlRxStop(IL_CHANNEL_ILTYPE_ONLY);

/**********************************************************************************************************************
  IlTxStop
**********************************************************************************************************************/
/** \brief    This method disables the transmission of messages (Sleep Mode).
              The transition "stop" of the Tx state machine is performed.
              The method is used for example to enter the Sleep Mode of an ECU.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function must be called on task level.\n
              The function must not interrupt IlInitPowerOn, IlInit, IlRxTask, IlRxStateTask, IlRxTimerTask, IlTxTask,
              IlTxStateTask, IlTxTimerTask, IlRxStart, IlTxStart, IlRxStop
    \note     The function is called by the Application or Nm (Network Management).
**********************************************************************************************************************/
extern void IlTxStop(IL_CHANNEL_ILTYPE_ONLY);

/**********************************************************************************************************************
  IlRxWait
**********************************************************************************************************************/
/** \brief    This method halts the timeout monitoring of reception messages.
              The transition "wait" of the Rx state machine is performed.
              The method is used for example when the bus-off mode of an ECU was entered.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function can be called on task and interrupt level.
    \note     The function is called by the Application or Nm (Network Management).
**********************************************************************************************************************/
extern void IlRxWait(IL_CHANNEL_ILTYPE_ONLY);

/**********************************************************************************************************************
  IlTxWait
**********************************************************************************************************************/
/** \brief    This method halts the transmission of messages.
              The transition "wait" of the Tx state machine is performed.
              The method is used for example when the bus-off mode of an ECU was entered.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function can be called on task and interrupt level.
    \note     The function is called by the Application or Nm (Network Management).
**********************************************************************************************************************/
extern void IlTxWait(IL_CHANNEL_ILTYPE_ONLY);

/**********************************************************************************************************************
  IlRxRelease
**********************************************************************************************************************/
/** \brief    The transition "release" of the Rx state machine is performed.\n
              -Restart the Rx Timeout Monitoring\n
              -Clear the timeout flags if IL_ENABLE_SYS_RX_RESET_TIMEOUT_FLAGS_ON_ILRXRELEASE is defined.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function can be called on task and interrupt level.
    \note     The function is called by the Application or Nm (Network Management).
**********************************************************************************************************************/
extern void IlRxRelease(IL_CHANNEL_ILTYPE_ONLY);

/**********************************************************************************************************************
  IlTxRelease
**********************************************************************************************************************/
/** \brief    This method resumes the transmission of messages from the "Waiting" state.
              The transition "release" of the Tx state machine is performed.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function can be called on task and interrupt level.
    \note     The function is called by the Application or Nm (Network Management).
**********************************************************************************************************************/
extern void IlTxRelease(IL_CHANNEL_ILTYPE_ONLY);

/**********************************************************************************************************************
  IlRxTask
**********************************************************************************************************************/
/** \brief    This method must be called periodically in the Rx task cycle time configured in the generation tool.
              The IlRxTimerTask and IlRxStateTask are called by this method.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function must be called on task level.\n
              The function must not interrupt IlRxStateTask, IlTxTask, IlTxStateTask,
              IlInitPowerOn, IlInit, IlRxStart, IlTxStart, IlRxStop, IlTxStop
    \note     The function is called by the Application or Ccl (Communication Control Layer).
**********************************************************************************************************************/
extern void IlRxTask(IL_CHANNEL_ILTYPE_ONLY);

/**********************************************************************************************************************
  IlTxTask
**********************************************************************************************************************/
/** \brief    This method must be called periodically in the Tx task cycle time configured in the generation tool.
              The IlTxTimerTask and IlTxStateTask are called by this method.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function must be called on task level.\n
              The function must not interrupt IlRxStateTask, IlTxTask, IlTxStateTask,
              IlInitPowerOn, IlInit, IlRxStart, IlTxStart, IlRxStop, IlTxStop
    \note     The function is called by the Application or Ccl (Communication Control Layer).
**********************************************************************************************************************/
extern void IlTxTask(IL_CHANNEL_ILTYPE_ONLY);

/**********************************************************************************************************************
  IlRxStateTask
**********************************************************************************************************************/
/** \brief    This method is called periodically by the IlRxTask. The function can be called in a faster rate 
              than the IlRxTask to check additionally for polled indication events.
              The usage of the IlRxTask shall be preferred.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function must be called on task level.\n
              The function must not interrupt IlRxStateTask, IlTxTask, IlTxStateTask,
              IlInitPowerOn, IlInit, IlRxStart, IlTxStart, IlRxStop, IlTxStop
    \note     The function is called by the Application or IlRxTask.
**********************************************************************************************************************/
extern void IlRxStateTask(IL_CHANNEL_ILTYPE_ONLY);

/**********************************************************************************************************************
  IlTxStateTask
**********************************************************************************************************************/
/** \brief    This method is called periodically by the IlTxTask. The function can be called in a faster rate,
              than the IlTxTask, to check additionally for polled confirmation events.
              The usage of the IlTxTask shall be preferred.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function must be called on task level.\n
              The function must not interrupt IlRxStateTask, IlTxTask, IlTxStateTask,
              IlInitPowerOn, IlInit, IlRxStart, IlTxStart, IlRxStop, IlTxStop
    \note     The function is called by the Application or IlTxTask.
**********************************************************************************************************************/
extern void IlTxStateTask(IL_CHANNEL_ILTYPE_ONLY);

/**********************************************************************************************************************
  IlRxTimerTask
**********************************************************************************************************************/
/** \brief    This method is called periodically by the IlRxTask. If the application separates the handling of
              notification and timing and the IlRxTask is not used, this function must be called periodically
              in the Rx task cycle time configured in the generation tool.
              The usage of the IlRxTask shall be preferred.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function must be called on task level.\n
              The function must not interrupt IlRxStateTask, IlTxTask, IlTxStateTask,
              IlInitPowerOn, IlInit, IlRxStart, IlTxStart, IlRxStop, IlTxStop
    \note     The function is called by the Application or IlRxTask.
**********************************************************************************************************************/
extern void IlRxTimerTask(IL_CHANNEL_ILTYPE_ONLY);

/**********************************************************************************************************************
  IlTxTimerTask
**********************************************************************************************************************/
/** \brief    This method is called periodically by the IlTxTask. If the application separates the handling of
              notification and timing and the IlTxTask is not used, this function must be called periodically
              in the Tx task cycle time configured in the generation tool.
              The usage of the IlTxTask shall be preferred.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function must be called on task level.\n
              The function must not interrupt IlRxStateTask, IlTxTask, IlTxStateTask,
              IlInitPowerOn, IlInit, IlRxStart, IlTxStart, IlRxStop, IlTxStop
    \note     The function is called by the Application or IlTxTask.
**********************************************************************************************************************/
extern void IlTxTimerTask(IL_CHANNEL_ILTYPE_ONLY);

# if defined ( IL_ENABLE_TX )
/**********************************************************************************************************************
  IlSetEvent
**********************************************************************************************************************/
/** \brief    This method serves to set a transmission request for a message.
    \param    ilTxHnd  Handle of the Tx message.
    \return   none
    \context  The function can be called on task and interrupt level.
    \note     The function is called by the Application or by IlPutTx method.
**********************************************************************************************************************/
extern void IlSetEvent(IlTransmitHandle ilTxHnd);
# endif

# if defined ( IL_ENABLE_TX ) && defined( IL_ENABLE_TX_SECURE_EVENT )
/**********************************************************************************************************************
  IlSecureEvent
**********************************************************************************************************************/
/** \brief    This method serves to set a transmission request for a message with repetitions.
    \param    ilTxHnd  Handle of the Tx message.
    \return   none
    \context  The function can be called on task and interrupt level.
    \note     The function is called by the IlPutTx method.
**********************************************************************************************************************/
extern void IlSecureEvent(IlTransmitHandle ilTxHnd);
# endif

# if defined ( IL_ENABLE_TX ) && defined ( IL_ENABLE_TX_CYCLIC_EVENT )
/**********************************************************************************************************************
  IlStartEvent
**********************************************************************************************************************/
/** \brief    This method starts the fast cyclic transmission of a message.
    \param    ilTxHnd         Handle of the Tx message.
    \return   (Il_Status) 1 : the fast transmission is started\n
              IL_ERROR      : the ilTxHnd is invalid
    \context  The function can be called on task and interrupt level.
    \note     The function is called by the IlPutTx method.
**********************************************************************************************************************/
extern Il_Status IlStartEvent(IlTransmitHandle ilTxHnd);
# endif

# if defined ( IL_ENABLE_TX ) && defined ( IL_ENABLE_TX_CYCLIC_EVENT )
/**********************************************************************************************************************
  IlStopEvent
**********************************************************************************************************************/
/** \brief    This method stops the fast cyclic transmission of a message.
    \param    ilTxHnd         Handle of the Tx message.
    \return   (Il_Status) 0 : the fast transmission is stopped\n
              IL_ERROR      : the ilTxHnd is invalid
    \context  The function can be called on task and interrupt level.
    \note     The function is called by the IlPutTx method.
**********************************************************************************************************************/
extern Il_Status IlStopEvent(IlTransmitHandle ilTxHnd);
# endif

# if defined ( IL_ENABLE_TX_STARTSTOP_CYCLIC )
/**********************************************************************************************************************
  IlStartCycle
**********************************************************************************************************************/
/** \brief    This method serves to start cyclic transmission requests of a message.
              Using this function can violate the predefined behavior of your module!
    \param    ilTxHnd     Handle of the Tx message.
    \return   IL_OK     : the cyclic transmission request is started\n
              IL_QUEUED : the cyclic transmission request is already enabled\n
              IL_ERROR  : the ilTxHnd is invalid
    \context  The function can be called on task and interrupt level.
    \note     The function is called by the Application.
**********************************************************************************************************************/
extern Il_Status IlStartCycle(IlTransmitHandle ilTxHnd);
# endif

# if defined ( IL_ENABLE_TX_STARTSTOP_CYCLIC )
/**********************************************************************************************************************
  IlStopCycle
**********************************************************************************************************************/
/** \brief    This method serves to stop the cyclic transmission request of a Tx message. Pending transmission
              requests are not taken into account.
              Using this function can violate the predefined behavior of your module!
    \param    ilTxHnd    Handle of the Tx message.
    \return   IL_OK    : the cyclic transmission request is removed\n
              IL_ERROR : the ilTxHnd is invalid
    \context  The function can be called on task and interrupt level.
    \note     The function is called by the Application.
**********************************************************************************************************************/
extern Il_Status IlStopCycle(IlTransmitHandle ilTxHnd);
# endif

# if defined ( IL_ENABLE_TX_SEND_ON_INIT )
/**********************************************************************************************************************
  IlSendOnInitMsg
**********************************************************************************************************************/
/** \brief    This method serves to set a transmission request flag for all messages configured as SendOnInit messages.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function must be called on task level.
    \note     The function is called by the Application.
**********************************************************************************************************************/
extern void IlSendOnInitMsg(IL_CHANNEL_ILTYPE_ONLY);
# endif

/**********************************************************************************************************************
  IlGetStatus
**********************************************************************************************************************/
/** \brief    Gets the current state of the Interaction Layer state machine.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   Il_Status : the value must be decoded with the following set of macros.\n
              The macros will return 0 (false) or 1 (true).
              \li IlIsTxRun(state)     : Tx is running
              \li IlIsTxWait(state)    : Tx is waiting
              \li IlIsTxSuspend(state) : Tx is suspended
              \li IlIsRxRun(state)     : Rx is running
              \li IlIsRxWait(state)    : Rx is waiting
              \li IlIsRxSuspend(state) : Rx is suspended
    \context  The function can be called on task and interrupt level.
    \note     The function is called by the Application.
**********************************************************************************************************************/
extern Il_Status IlGetStatus(IL_CHANNEL_ILTYPE_ONLY);

# if defined( IL_ENABLE_SEND_DIRECT )
/**********************************************************************************************************************
  IlSendDirect
**********************************************************************************************************************/
/** \brief    Refer to the Application Note AN-ISC-8-1045_IlSendDirect.pdf
    \param    ilTxHnd  Handle of the Tx message.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \warning  Do not use this API, without taking the application note into account.
**********************************************************************************************************************/
extern void IlSendDirect(IL_CHANNEL_ILTYPE_FIRST IlTransmitHandle ilTxHnd);
# endif

# if defined ( IL_ENABLE_SYS_GET_CONTEXT ) 
/**********************************************************************************************************************
  IlGetModuleContext
**********************************************************************************************************************/
/** \brief    This function copies the context of the component to a structure.
    \param    pContext  pointer to a tIlModuleContextStructPtr context structure.
    \return   none
    \pre      The component context must be saved
    \post     The structure contains the component state
    \context  The function must be called with disabled interrupts.
**********************************************************************************************************************/
extern void IlGetModuleContext(tIlModuleContextStructPtr pContext);
# endif

# if defined ( IL_ENABLE_SYS_SET_CONTEXT ) 
/**********************************************************************************************************************
  IlSetModuleContext
**********************************************************************************************************************/
/** \brief    This function copies a given context to a component.
    \param    pContext      pointer to a tIlModuleContextStructPtr context structure.
    \return   (vuint8) 0  : the magicNumber member of the tIlModuleContextStructPtr parameter is invalid.\n
              (vuint8) 1  : the magicNumber member of the tIlModuleContextStructPtr parameter is valid.
    \pre      The component context must be initialised.
    \post     The structure contains the component state.
    \context  The function must be called with disabled interrupts.
**********************************************************************************************************************/
extern vuint8 IlSetModuleContext(tIlModuleContextStructPtr pContext);
# endif



/* Callback functions ************************************************************************************************/

# if defined ( IL_ENABLE_SYS_INIT_FCT )
/**********************************************************************************************************************
  ApplIlInit
**********************************************************************************************************************/
/** \brief    This method is called to indicate the performed initialization.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function is called by the IL in the context of IlInit.
**********************************************************************************************************************/
extern void ApplIlInit(IL_CHANNEL_ILTYPE_ONLY);
# endif

# if defined ( IL_ENABLE_SYS_RX_START_FCT )
/**********************************************************************************************************************
  ApplIlRxStart
**********************************************************************************************************************/
/** \brief    This method is called to indicate the performed transition start for the Rx state machine.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function is called by the IL in the context of IlRxStart.
**********************************************************************************************************************/
extern void ApplIlRxStart(IL_CHANNEL_ILTYPE_ONLY);
# endif

# if defined ( IL_ENABLE_SYS_TX_START_FCT )
/**********************************************************************************************************************
  ApplIlTxStart
**********************************************************************************************************************/
/** \brief    This method is called to indicate the performed transition start for the Tx state machine.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function is called by the IL in the context of IlTxStart.
**********************************************************************************************************************/
extern void ApplIlTxStart(IL_CHANNEL_ILTYPE_ONLY);
# endif

# if defined ( IL_ENABLE_SYS_RX_STOP_FCT )
/**********************************************************************************************************************
  ApplIlRxStop
**********************************************************************************************************************/
/** \brief    This method is called to indicate the performed transition stop for the Rx state machine.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function is called by the IL in the context of IlRxStop.
**********************************************************************************************************************/
extern void ApplIlRxStop(IL_CHANNEL_ILTYPE_ONLY);
# endif

# if defined ( IL_ENABLE_SYS_TX_STOP_FCT )
/**********************************************************************************************************************
  ApplIlTxStop
**********************************************************************************************************************/
/** \brief    This method is called to indicate the performed transition stop for the Tx state machine.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function is called by the IL in the context of IlTxStop.
**********************************************************************************************************************/
extern void ApplIlTxStop(IL_CHANNEL_ILTYPE_ONLY);
# endif

# if defined ( IL_ENABLE_SYS_TESTDEBUG )
/**********************************************************************************************************************
  ApplIlFatalError
**********************************************************************************************************************/
/** \brief    If assertions are configured, this function is called to indicate invalid user conditions
              (API, reentrance), inconsistent generated data, hardware errors and internal errors.
    \param    errorNumber  numeric error code
    \return   none
    \context  The function is be called by the IL on task and interrupt level.
**********************************************************************************************************************/
extern void ApplIlFatalError(vuint8 errorNumber);
# endif

/* API Macro wrapped internal functions ******************************************************************************/

# if defined ( IL_ENABLE_RX ) && defined ( IL_ENABLE_RX_INDICATION_FLAG )
/**********************************************************************************************************************
  IlGetSignalIndicationFlag
**********************************************************************************************************************/
/** \brief    This method is used by the generated IlGetClr* macros to return and clear indication flags.
    \param    ilRxFlagHnd  Handle of the Rx flag.
    \return   (vuint8) 0 : the indication flag is not set.\n
                     > 0 : the indication flag was set.
    \context  The function can be called on task and interrupt level.
    \note     The function is called by the Application indirectly from the IlGetClr macros.
    \warning  Do not call this Il internal API from the application!
**********************************************************************************************************************/
extern vuint8 IlGetSignalIndicationFlag(IlReceiveFlagHandle ilRxFlagHnd);
# endif


# if defined( VGEN_GENY ) && defined( IL_ENABLE_TX_DYNAMIC_CYCLETIME )
/**********************************************************************************************************************
  IlSetDynTxCycleTimeRaw
**********************************************************************************************************************/
/** \brief    Sets the cycle time of an Il message.
              The macro is called from the application. The function has no effect, if the Il message handle is not
              transmitted cyclically. This is defined in the dbc database file with the attributes GenMsgSendType and
              GenSigSendType. Please refer to the technical reference for details. To activate the cyclic transmission
              of an Il message handle use the Il API function IlStartCycle.
    \param    ilTxHnd      This value is the handle of the Tx message. The generated file il_par.h contains generated
                           defines for the message handles.
    \param    ilTxTicks     This value is the number of calls of the IlTxTask, until the cyclic event is raised.
    \return   none
    \context  The function must be called within the Il callback function ApplIlInit or in the signal oriented init
              callback function.
    \note     The function is called by the Application.
    \warning  Do not use the function for handles containing multiplexed signals
**********************************************************************************************************************/
extern void IlSetDynTxCycleTimeRaw(IlTransmitHandle ilTxHnd, IltTxCounter ilTxTicks);
# endif/* VGEN_GENY && IL_ENABLE_TX_DYNAMIC_CYCLETIME */

# if defined( VGEN_GENY ) && defined( IL_ENABLE_SYS_TX_REPETITIONS_ARE_ACTIVE_FCT )
/**********************************************************************************************************************
  IlTxRepetitionsAreActive
**********************************************************************************************************************/
/** \brief    This method can be used to detect if messages with repetitions are queued for transmission on a channel.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   IL_TRUE   : Messages with repetitions are queued for transmission.\n
              IL_FALSE  : No message with repetitions is queued for transmission.
    \context  The function can be called on task and interrupt level.
    \note     The function is called by the Application.
    \warning  The function does not support Virtual Networks.
**********************************************************************************************************************/
Il_Boolean IlTxRepetitionsAreActive(IL_CHANNEL_ILTYPE_ONLY);
# endif/* VGEN_GENY && IL_ENABLE_TX_REPETITIONS_COMPLETED */

# if defined( VGEN_GENY ) && defined( IL_ENABLE_SYS_TX_SIGNALS_ARE_ACTIVE_FCT )
/**********************************************************************************************************************
  IlTxSignalsAreActive
**********************************************************************************************************************/
/** \brief    This method can be used to detect if signals are active on a channel.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   IL_TRUE   : Signals are in the active state.\n
              IL_FALSE  : No signal is in the active state.
    \context  The function can be called on task and interrupt level.
    \note     The function is called by the Application.
    \warning  The function does not support Virtual Networks.
**********************************************************************************************************************/
Il_Boolean IlTxSignalsAreActive(IL_CHANNEL_ILTYPE_ONLY);
# endif/* VGEN_GENY && IL_ENABLE_TX_REPETITIONS_COMPLETED */


/* Stack internal functions ******************************************************************************************/

/**********************************************************************************************************************
  IlResetRxTimeoutFlags
**********************************************************************************************************************/
/** \brief    This method clears Rx timeout flags of the Application and internal ilNodeCommActiveTimeoutFlags.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \context  The function can be called on task and interrupt level.
    \note     The function is called by the Application.
    \warning  Do not call this Il internal API from the application!
**********************************************************************************************************************/
extern void IlResetRxTimeoutFlags(IL_CHANNEL_ILTYPE_ONLY);

/* Il_Vector internal functions **************************************************************************************/

# if !defined( VGEN_GENY )
# endif/* VGEN_GENY */

# if defined( IL_ENABLE_TX ) && defined( IL_ENABLE_TX_CYCLIC_EVENT )
/**********************************************************************************************************************
  InitIfActiveFlags
**********************************************************************************************************************/
/** \brief    This method clears all if active flags of a channel.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \note     The function is called by the IlTxStart.
    \warning  Do not call this Il internal API from the application!
**********************************************************************************************************************/
extern void InitIfActiveFlags(IL_CHANNEL_ILTYPE_ONLY);
# endif

# if defined( IL_ENABLE_TX )
/**********************************************************************************************************************
  IlResetCanConfirmationFlags
**********************************************************************************************************************/
/** \brief    This method clears all Can Driver confirmation flags of IL Tx messages of a channel.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \note     The function is called by the IlTxStart.
    \warning  Do not call this Il internal API from the application!
**********************************************************************************************************************/
extern void IlResetCanConfirmationFlags(IL_CHANNEL_ILTYPE_ONLY);
# endif

# if defined( IL_ENABLE_RX ) && defined( IL_ENABLE_RX_POLLING )
/**********************************************************************************************************************
  IlResetCanIndicationFlags
**********************************************************************************************************************/
/** \brief    This method clears all Can Driver indication flags of IL Rx messages of a channel.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \note     The function is called by the IlRxStart.
    \warning  Do not call this Il internal API from the application!
**********************************************************************************************************************/
extern void IlResetCanIndicationFlags(IL_CHANNEL_ILTYPE_ONLY);
# endif

# if defined( IL_ENABLE_RX ) && !defined( VGEN_GENY ) && defined( IL_ENABLE_RX_MODE_SIGNALS )
# endif

# if defined( IL_ENABLE_TX ) && defined( IL_ENABLE_TX_TIMEOUT )
/**********************************************************************************************************************
  IlCheckTxTimeout
**********************************************************************************************************************/
/** \brief    This method checks the IL internal Tx timeout flags, decrements Tx timeout counters, calls Tx timeout
              callback functions and sets Tx timeout flags of a channel.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \note     The function is called by the IlTxTimerTask.
    \warning  Do not call this Il internal API from the application!
**********************************************************************************************************************/
extern void IlCheckTxTimeout(IL_CHANNEL_ILTYPE_ONLY);
# endif

# if defined( IL_ENABLE_SYS_SIGNAL_INIT_FCT )
/**********************************************************************************************************************
  IlSignalInit
**********************************************************************************************************************/
/** \brief    This method calls all configured init signal callback functions of a channel.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \note     The function is called by IlInit.
    \warning  Do not call this Il internal API from the application!
**********************************************************************************************************************/
extern void IlSignalInit(IL_CHANNEL_ILTYPE_ONLY);
# endif

# if defined( IL_ENABLE_RX ) && defined( IL_ENABLE_RX_SIGNAL_START_FCT )
/**********************************************************************************************************************
  IlSignalRxStart
**********************************************************************************************************************/
/** \brief    This method calls all configured Rx start signal callback functions of a channel.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \note     The function is called by IlRxStart.
    \warning  Do not call this Il internal API from the application!
**********************************************************************************************************************/
extern void IlSignalRxStart(IL_CHANNEL_ILTYPE_ONLY);
# endif

# if defined( IL_ENABLE_TX ) && defined( IL_ENABLE_TX_SIGNAL_START_FCT )
/**********************************************************************************************************************
  IlSignalTxStart
**********************************************************************************************************************/
/** \brief    This method calls all configured Tx start signal callback functions of a channel.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \note     The function is called by IlTxStart.
    \warning  Do not call this Il internal API from the application!
**********************************************************************************************************************/
extern void IlSignalTxStart(IL_CHANNEL_ILTYPE_ONLY);
# endif

# if defined( IL_ENABLE_RX ) && defined( IL_ENABLE_RX_SIGNAL_STOP_FCT )
/**********************************************************************************************************************
  IlSignalRxStop
**********************************************************************************************************************/
/** \brief    This method calls all configured Rx stop signal callback functions of a channel.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \note     The function is called by IlRxStop.
    \warning  Do not call this Il internal API from the application!
**********************************************************************************************************************/
extern void IlSignalRxStop(IL_CHANNEL_ILTYPE_ONLY);
# endif

# if defined( IL_ENABLE_TX ) && defined( IL_ENABLE_TX_SIGNAL_STOP_FCT )
/**********************************************************************************************************************
  IlSignalTxStop
**********************************************************************************************************************/
/** \brief    This method calls all configured Tx stop signal callback functions of a channel.
    \param_i  channel : Handle of the logical Can Driver channel.
    \return   none
    \note     The function is called by IlTxStop.
    \warning  Do not call this Il internal API from the application!
**********************************************************************************************************************/
extern void IlSignalTxStop(IL_CHANNEL_ILTYPE_ONLY);
# endif




/**********************************************************************************************************************
  END OF FILE: Il_Def.h
**********************************************************************************************************************/
#endif /* V_IL_DEF_COMPONENT_HEADER */
/* STOPSINGLE_OF_MULTIPLE */
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
