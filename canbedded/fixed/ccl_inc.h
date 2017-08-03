/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     ccl_inc.h 
 *
 * description:   ccl
 *
 *
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           ccl for S12XEP100
 *
 *****************************************************************************/

/****************************************************************************/
/* one further header (the OEM specific one) is included                    */
/* at the end of this core header file                                      */
/****************************************************************************/

/* check multiple inclusion */
#ifndef CCL_INC_H
#define CCL_INC_H


/***************************************************************************/ 
/* Include files                                                           */ 
/***************************************************************************/ 
/* CCL configuration  */
#include "ccl_cfg.h"
#include "v_cfg.h"



/***************************************************************************/ 
/* Version  (core version)  (abcd: Main version ab Sub Version cd )        */ 
/***************************************************************************/ 
/* ##V_CFG_MANAGEMENT ##CQProject : Ccl__core CQComponent : Implementation */
#define CCL__CORE_VERSION          0x0229
#define CCL__CORE_RELEASE_VERSION  0x00

/***************************************************************************/
/* Version Defines to stay compatiple with older versions                  */
/***************************************************************************/
#define CCL_CORE_VERSION          CCL__CORE_VERSION
#define CCL_CORE_BUGFIX_VERSION   CCL__CORE_RELEASE_VERSION
#define kCclBugfixVersion         kCclReleaseVersion


/***************************************************************************/ 
/* Defines                                                                 */ 
/***************************************************************************/
#if defined(kCclNrOfSystemChannels)
#else
#  if defined(C_MULTIPLE_RECEIVE_CHANNEL)
#    define kCclNrOfSystemChannels 2
#  endif
#endif

#if defined( CCL_ENABLE_DEBUG)
#  if ( kCclNrOfSystemChannels > 1 )
#    if ( kCclNrOfChannels > 1 )
#      define CclAssert(p,e)  {if(!(p)){CclFatalError(channel, (e), (vuint16)(__LINE__),kModuleCcl);}}
#    else
#        define CclAssert(p,e)  {if(!(p)){CclFatalError((CanChannelHandle)kCclChannelIndex, (e), (vuint16)(__LINE__),kModuleCcl);}}
#    endif
#  else
#    if (kCclNrOfChannels > 1)
/* non-indexed systems are not supported by the CCL */
#    else
#      define CclAssert(p,e)  {if(!(p)){CclFatalError(0, (e), (vuint16)(__LINE__),kModuleCcl);}}
#    endif
#  endif
#else
#  define CclAssert(p,e)
#endif

/* defines for Paging-mode */
#ifndef CCL_API_CALL_TYPE
# define CCL_API_CALL_TYPE
#endif
#ifndef CCL_API_CALLBACK_TYPE
# define CCL_API_CALLBACK_TYPE
#endif
#ifndef CCL_INTERNAL_CALL_TYPE
# define CCL_INTERNAL_CALL_TYPE
#endif

/* downwards compatible defines */
#if( CCL_DLL_VERSION < 0x0106 )
#  define cclNetRequestField   cclIntNetState
#  define kCclNrOfNetworks     kCclNrOfNmNetworks
#  define cclStateCom          cclComSwState

#  define kCclComOn                kCclComSwOn
#  define kCclComOff               kCclComSwOff

#  if defined( CCL_USE_REQUEST_SETCOM_FCT )
#    define CCL_USE_REQUEST_SETINTCOM_FCT CCL_USE_REQUEST_SETCOM_FCT
#    define CCL_USE_REQUEST_SETEXTCOM_FCT CCL_USE_REQUEST_SETCOM_FCT
#  endif

#  if(kCclNrOfChannels > 1)
#    define CclInitTransFctTbl    CclInitTrcvFctTbl
#    define CclWakeUpTransFctTbl  CclWakeUpTrcvFctTbl
#    define CclSleepTransFctTbl   CclSleepTrcvFctTbl
#    define CclComRelFctTbl       CclClearComReqFctTbl
#    define CclExtCanComReqFctTbl CclSetComReqFctTbl
#    define CclIntComReqFctTbl    CclSetComReqFctTbl
#  else
#    define CclInitTransFct      CclInitTrcvFct
#    define CclWakeUpTransFct    CclWakeUpTrcvFct
#    define CclSleepTransFct     CclSleepTrcvFct
#    define CclComRelFct         CclClearComReqFct
#    define CclExtCanComReqFct   CclSetComReqFct
#    define CclIntComReqFct      CclSetComReqFct
#  endif
#endif

#if defined( VGEN_GENY )
/* enable commuincation request handling API */
#  if( CCL__COREDLL_VERSION >= 0x0295u )
#    define CCL_ENABLE_COM_REQ_HANDLING_API
#  endif
#endif

/* ESCAN00011723 */
#define CclGetNmState              CclGetChannelState

/* downwards compatible defines */
#define kModuleCanDrv            kComponentCanDrv
#define kModuleLinDrv            kComponentLinDrv
#define kModuleIl                kComponentIl
#define kModuleDbk               kComponentDbk
#define kModuleMm                kComponentMm
#define kModuleTp                kComponentTp
#define kModuleNm                kComponentNm
#define kModuleNmPt              kComponentNmPt
#define kModuleSm                kComponentSm
#define kModuleNmVagC            kComponentNmVagC
#define kModuleDiagx             kComponentDiagx
#define kModuleCANdesc           kComponentCANdesc
#define kModuleCcl               kComponentCcl

#define kCclNmStateSleep         kCclStateSleep
#define kCclNmStateGoToSleep     kCclStateGoToSleep
#define kCclNmStateActive        kCclStateActive
#define kCclNmPowerFail          kCclPowerFail


/* channel parameter and index replacement */ 
#if ( kCclNrOfSystemChannels > 1 )
#  if( kCclNrOfChannels > 1 )
#    define CCL_CHANNEL_CCLPARA_ONLY       channel
#    define CCL_CHANNEL_CCLPARA_FIRST      channel,
#    define CCL_CHANNEL_CCLTYPE_ONLY       CanChannelHandle channel
#    define CCL_CHANNEL_CCLTYPE_FIRST      CanChannelHandle channel,
#    define CCL_CHANNEL_CCLINDEX           (vuint8)channel
#    define CCL_CHANNEL_CANPARA_ONLY       channel
#    define CCL_CHANNEL_CCLHANDLE_FIRST   (vuint8)channel,
#  else
#    define CCL_CHANNEL_CCLPARA_ONLY
#    define CCL_CHANNEL_CCLPARA_FIRST
#    define CCL_CHANNEL_CCLTYPE_ONLY       void
#    define CCL_CHANNEL_CCLTYPE_FIRST
#    define CCL_CHANNEL_CCLINDEX           (vuint8)0
#    define CCL_CHANNEL_CANPARA_ONLY       (CanChannelHandle)kCclChannelIndex
#    define CCL_CHANNEL_CCLHANDLE_FIRST
#  endif
#else
#  define CCL_CHANNEL_CCLPARA_ONLY
#  define CCL_CHANNEL_CCLPARA_FIRST
#  define CCL_CHANNEL_CCLTYPE_ONLY       void
#  define CCL_CHANNEL_CCLTYPE_FIRST
#  define CCL_CHANNEL_CCLINDEX           (vuint8)0
/* ESCAN00026778 */
#  if defined C_ENABLE_MULTICHANNEL_API
#    define CCL_CHANNEL_CANPARA_ONLY     (CanChannelHandle)0
#  else
#    define CCL_CHANNEL_CANPARA_ONLY
#  endif
#  define CCL_CHANNEL_CCLHANDLE_FIRST
#endif

/* network parameter and index replacement */
#if ( kCclNrOfSystemChannels > 1 )
#    if(kCclNrOfChannels > 1)
#      define CCL_NET_CCLPARA_ONLY         channel
#      define CCL_NET_CCLPARA_FIRST        channel,
#      define CCL_NET_CCLINDEX             channel
#        define CCL_NET_CCLTYPE_ONLY         CanChannelHandle channel
#        define CCL_NET_CCLTYPE_FIRST        CanChannelHandle channel,
#    else
#      define CCL_NET_CCLPARA_ONLY
#      define CCL_NET_CCLPARA_FIRST
#      define CCL_NET_CCLTYPE_ONLY           void
#      define CCL_NET_CCLTYPE_FIRST
#      define CCL_NET_CCLINDEX               0
#    endif
#else
#    define CCL_NET_CCLPARA_ONLY
#    define CCL_NET_CCLPARA_FIRST
#    define CCL_NET_CCLTYPE_ONLY           void
#    define CCL_NET_CCLTYPE_FIRST
#    define CCL_NET_CCLINDEX               0
#endif

/* ECU handle replacement */
#if defined(CCL_ENABLE_MULTIPLE_NODES) 
#  if defined(CCL_ENABLE_CANBEDDED_HANDLING)
#    define CCL_ECUS_NODESTYPE_ONLY        cclECUHandle  CanEcuNr
#        define CCL_ECUS_NODESTYPE_FIRST       cclECUHandle  CanEcuNr  
#    define CCL_ECUS_NODESPARA_ONLY        (vuint8)CanEcuNr
#  else
#   define CCL_ECUS_NODESTYPE_ONLY         void
#       define CCL_ECUS_NODESTYPE_FIRST   void
#   define CCL_ECUS_NODESPARA_ONLY
#  endif
#else
#  if defined(CCL_ENABLE_MULTIPLE_CONFIG)
#    if defined(CCL_ENABLE_CANBEDDED_HANDLING)
#     define CCL_ECUS_NODESTYPE_ONLY        vuint8  IdentityNr
#     define CCL_ECUS_NODESTYPE_FIRST       vuint8  IdentityNr,
#     define CCL_ECUS_NODESPARA_ONLY        IdentityNr
#    else
#      define CCL_ECUS_NODESTYPE_ONLY        void
#      define CCL_ECUS_NODESTYPE_FIRST       
#      define CCL_ECUS_NODESPARA_ONLY
#    endif
#  else
#    define CCL_ECUS_NODESTYPE_ONLY        void
#    define CCL_ECUS_NODESTYPE_FIRST
#    define CCL_ECUS_NODESPARA_ONLY
#endif


#  define CCL_NM_INITPOINTER_TYPE      
#  define CCL_NM_INITPOINTER_PARA      

/* multiple configuration replacement */
#  define CCL_SET_MULTIPLE_CFG_HANDLE(i)



/* CCL states (active, go to sleep and sleep) */
#define kCclStateSleep                0x00    /* CCL state: sleep */
#define kCclStateWaitBusSleep         0x01    /* CCL state: wait bus sleep */
#define kCclStateGoToSleep            0x02    /* CCL state: go to sleep */
#define kCclStateActive               0x03    /* CCL state: active */
#define kCclPowerFail                 0x04    /* CCL state: power fail active */


#define kCclComHwDisabled               0x00    /* HW communication state: disabled */
#define kCclComHwEnabled                0x01    /* HW communication state: enabled */

#define kCclComSwOff                    0x00    /* SW communication state: off */
#define kCclComSwOn                     0x01    /* SW communication state: on */


#define kCclBusOn                        0x00     /* CAN hardware and transceiver are active */
#define kCclBusOff                       0x01     /* CAN hardware and transceiver are inactive */

#define kCclNoCanAvailable                0x00    /* CAN HW is not stable */
#define kCclCanAvailable                  0x01    /* CAN HW is stable */

#define kCclTrcvWakeIntPending    0x00
#define kCclTrcvWakeIntNoPending  0x01

#define CCL_NO_CHANNEL_INFO_AVAILABLE   0xFF

#define kCclNoErrPort                   0xFF
#define kCclTrcvErrPinActive            0x00

#define kCclNoRepeatCanSleep            0x00     /* no repeat of the function CanSleep */
#define kCclRepeatCanSleep              0x01     /* repeat the CanSleep function */

#define kCclSystemSpecificInit          0x00
#define kCclChannelSpecificInit         0x01

#define kCclNoNetRequest                0x00
#define kCclNetRequest                  0x01

#define kCclComReqNotPending               0x00
#define kCclComReqPending                  0x01

#define kCclNoWakeUpAble                0x00
#define kCclChWithNm                    0x00

#define kCclReqOk                       0x00
#define kCclReqFailed                   0xFF



#if defined(CCL_ENABLE_TRCV_PORT_INT)
#  define kCclDisableWakeUpInt          0x00     /* transceiver port interrupt is disabled */
#  define kCclEnableWakeUpInt           0x00     /* transceiver port interrupt is enabled */
#endif








#define kCclNoIlNiChannel             0x00

#define kCclOk                          0x01
#define kCclFailed                      0x00











#define kCclTrcvStateChangePerformed       0x01
#define kCclTrcvStateChangeNotPerformed    0x00

#if defined (CCL_ENABLE_NMPASSIVE_MODE) /* ESCAN00023408 */
# define kCclNmActiveMode   0x01
# define kCclNmPassiveMode  0x00
#endif

/***************************************************************************/

#  define kCclBusOffRecActive        0x01
#  define kCclBusOffRecNotActive     0x00
#endif

/***************************************************************************/ 
/* Default switches                                                        */ 
/***************************************************************************/ 


/***************************************************************************/ 
/* Memory qualifier                                                        */ 
/***************************************************************************/ 


/***************************************************************************/ 
/* return values                                                           */ 
/***************************************************************************/ 


/***************************************************************************/ 
/* Component Identifiers                                                   */ 
/***************************************************************************/ 
/* DRV component codes */ 
#define kComponentCanDrv                   0x01  /* CAN driver */
#define kComponentLinDrv                   0x02  /* LIN driver */

/* IL component codes */
#define kComponentIl                       0x10  /* Vector Interaction Layer */
#define kComponentDbk                      0x11  /* DBKOM Layer */
#define kComponentMm                       0x12  /* Message Manager */

/* NM component codes */
#define kComponentNm                       0x20  /* NM OSEK (direct)*/
#define kComponentNmPt                     0x21  /* NM Powertrain */
#define kComponentSm                       0x22  /* Station Manager for NM-OSEK (indirect) */
#define kComponentNmVagC                   0x23  /* NM VagC */
#define kComponentNmUData                  0x24  /* NM User Data */
#define kComponentNmHigh                   0x25  /* NM High */
#define kComponentNmBasic                  0x26  /* NM Powertrain Basic */
#define kComponentNmMazda                  0x27  /* NM Powertrain Mazda */
#define kComponentNmCbdWrp                 0x28  /* NM Autosar Wrapper */
#define kComponentNmAsrGeneric             0x29  /* NM Autosar Generic */
#define kComponentNmAsrCan                 0x2a  /* Nm Autosar CAN */
#define kComponentCwm                      0x2b  /* Car wake up manager */ /* ESCAN00022743 */
#define kComponentIndirectNm               0x2c  /* NM OSEK (indirect) */

/* TP component codes */
#define kComponentTp                       0x30  /* ISO TP */

/* DIAG component codes */
#define kComponentDiagx                    0x40  /* Diagnose: DiagX */
#define kComponentCANdesc                  0x41  /* Diagnose: CANdesc (Basic, UDS etc) */

/* CCL component codes */
#define kComponentCcl                      0x50  /* CCL */

/* DPM component code */
#define kComponentDpm                      0x60  /* DPM */

/* GW component code */
#define kComponentSigGW                    0x70  /* Signal Gateway */
#define kComponentAs                       0x71  /* Administrative Services */


/***************************************************************************/ 
/* CCL Error Codes                                                         */ 
/***************************************************************************/
#define kAssertCANandTRCVINT            0x00
#define kAssertNotSleepAndTRCVINTactive 0x01

#define kIllegalWakeUpInterrupt         0x03
#define kWrongReturnValue               0x04

#define kExtAndIntWakeUp                0x05

#define kCclErrEMCTimerHdlgFailed       0x06

#define kCclErrWakeFailed               0x07

#define kCclErrInvalidComHandle         0x08

#define kCclErrInvalidNmHInitValue      0x09

#define kCclErrWrongCclState            0x0A

#define kCclErrInvalidNmMode            0x0B

#define kCclErrNullPointer              0x10

#define kCclErrWrongWakeUpChannel       0x11

#define kCclWrongChannelHandle          0x12    /* ESCAN00022743 */

#define kCclErrWrongStateTransition     0x13

#define kCclErrComReqFailed             0x14   /* CCL communiation request is not acepted by the NM, not allowed */



/***************************************************************************/ 
/* macros                                                                  */ 
/***************************************************************************/ 


/***************************************************************************/ 
/* compatibility defines                                                   */ 
/***************************************************************************/ 


/***************************************************************************/ 
/* data types                                                              */ 
/***************************************************************************/
#if defined(CCL_ENABLE_MULTIPLE_NODES)
typedef vuint8 cclECUHandle; /* ECU handle */  /* ESCAN00011187 */
#endif






/* ESCAN00023724 */
#  if( C_VERSION_REF_IMPLEMENTATION < 0x130 )
typedef vuint8 CanChannelHandle;
#  endif

/***************************************************************************/ 
/* internal function prototypes                                            */ 
/***************************************************************************/ 
#if defined(CCL_ENABLE_ERROR_HOOK)
extern void CCL_API_CALL_TYPE CclFatalError(CanChannelHandle ChannelNumber, vuint16 ErrorCode, vuint16 ErrorLine, vuint8 ComponentID);
#endif

extern void CCL_API_CALL_TYPE CclCanNormal(CCL_CHANNEL_CCLTYPE_ONLY);
extern void CCL_API_CALL_TYPE CclCanStandby(CCL_CHANNEL_CCLTYPE_ONLY);

/***************************************************************************/ 
/* external function prototypes                                            */
/***************************************************************************/


extern void CCL_API_CALL_TYPE CclInitPowerOn(CCL_ECUS_NODESTYPE_ONLY);
extern void CCL_API_CALL_TYPE CclInit(CCL_CHANNEL_CCLTYPE_ONLY);

#if defined(CCL_ENABLE_SYSTEM_STARTUP)
extern void CCL_API_CALL_TYPE CclSystemStartup(void);
#endif

#if(defined( CCL_ENABLE_EXTERNAL_REQUEST ) || \
    defined( CCL_ENABLE_INTERNAL_REQUEST ))
extern void CCL_API_CALL_TYPE CclTask(void);
#endif

#if defined(CCL_ENABLE_SYSTEM_SHUTDOWN)
extern void CCL_API_CALL_TYPE CclSystemShutdown(void);
#endif

#if defined( CCL_ENABLE_INTERNAL_REQUEST )
#  if defined(CCL_ENABLE_COM_REQ_HANDLING_API)
extern vuint8 CCL_API_CALL_TYPE CclRequestCommunication(vuint8 cclComHandle);
extern void CCL_API_CALL_TYPE CclReleaseCommunication(vuint8 cclComHandle);
#  endif


#  if defined( CCL_ENABLE_COM_REQ_HANDLING_API )
extern vuint8 CCL_API_CALL_TYPE CclComRequestPending(CCL_CHANNEL_CCLTYPE_ONLY);
extern vuint8 CCL_API_CALL_TYPE CclGetComHandleState(vuint8 cclComHandle);
#  endif
#endif

extern void CCL_API_CALL_TYPE CclEnableCommunication(CCL_CHANNEL_CCLTYPE_ONLY);
extern void CCL_API_CALL_TYPE CclDisableCommunication(CCL_CHANNEL_CCLTYPE_ONLY);

extern vuint8 CCL_API_CALL_TYPE CclGetChannelState(CCL_CHANNEL_CCLTYPE_ONLY);

#if defined(CCL_ENABLE_TRCV_PORT_INT)
#  if defined(CCL_ENABLE_EXTERNAL_REQUEST)
extern void CCL_API_CALL_TYPE CclCanWakeUpInt(CCL_CHANNEL_CCLTYPE_ONLY);
#  endif
#endif

#if defined(CCL_NM_TYPE_NONE)
extern void CCL_API_CALL_TYPE CclNmWaitBusSleep(CCL_CHANNEL_CCLTYPE_ONLY);
extern void CCL_API_CALL_TYPE CclNmWaitBusSleepCancel(CCL_CHANNEL_CCLTYPE_ONLY);
extern void CCL_API_CALL_TYPE CclNmSleep(CCL_CHANNEL_CCLTYPE_FIRST CanInitHandle initObject);
extern void CCL_API_CALL_TYPE CclComBusOffStart(CCL_CHANNEL_CCLTYPE_ONLY);
extern void CCL_API_CALL_TYPE CclComBusOffEnd(CCL_CHANNEL_CCLTYPE_ONLY);
#endif

#if defined( CCL_ENABLE_ERROR_PIN )
extern vuint8 CCL_API_CALL_TYPE CclGetErrorPort(CCL_CHANNEL_CCLTYPE_ONLY);
#endif




#if defined (CCL_ENABLE_TRCV_TYPE_TLE6251G) /* ESCAN00022973 */
extern vuint8 CclSetTrcvRxOnlyMode(CCL_CHANNEL_CCLTYPE_ONLY);
extern vuint8 CclLeaveTrcvRxOnlyMode(CCL_CHANNEL_CCLTYPE_ONLY);
#endif
/***************************************************************************/ 
/* External Declarations                                                   */ 
/***************************************************************************/ 

#if defined( CCL_ENABLE_ERROR_HOOK)
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 cclChannelNumber;
V_MEMRAM0 extern V_MEMRAM1 vuint16 V_MEMRAM2 cclErrorCode;
V_MEMRAM0 extern V_MEMRAM1 vuint16 V_MEMRAM2 cclErrorLine;
V_MEMRAM0 extern V_MEMRAM1 vuint16 V_MEMRAM2 cclComponentID;
#endif



#if defined( CCL_ENABLE_SW_COM_STATE )
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 cclComSwState[kCclNrOfChannels];
#endif


#if defined(CCL_ENABLE_INTERNAL_REQUEST)
V_MEMRAM0 extern volatile V_MEMRAM1 vuint8 V_MEMRAM2 cclIntNetState[kCclNetReqTableSize];
#endif







V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 cclNmState[kCclNrOfChannels];

V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kCclMainVersion;
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kCclSubVersion;
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kCclReleaseVersion;

/***************************************************************************/ 
/* Callback functions                                                      */ 
/***************************************************************************/ 
extern vuint8 CCL_API_CALLBACK_TYPE ApplCclCanStandby(CCL_CHANNEL_CCLTYPE_FIRST vuint8 sleepResult);



#if defined(CCL_ENABLE_ERROR_HOOK)
extern void CCL_API_CALLBACK_TYPE ApplCclFatalError(void);
#endif








#if defined(CCL_NM_TYPE_NONE)
extern void ApplCclNmStateChange(CCL_CHANNEL_CCLTYPE_FIRST vuint8 activeState, vuint8 targetState);
#endif

/***************************************************************************/ 
/* OEM specific                                                            */ 
/***************************************************************************/ 

/*****************************************************************************
| Project Name: CCL Communication Control Layer
| File Name: 
|
|  Description: CCL OEM for VECTOR
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2008 by Vector Informatik GmbH.      All rights reserved.
|
|               This software is copyright protected and proprietary 
|               to Vector Informatik GmbH. Vector Informatik GmbH 
|               grants to you only those rights as set out in the 
|               license conditions. All other rights remain with 
|               Vector Informatik GmbH.
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     ------------------------------------
| Et           Thomas Ebert              Vector Informatik GmbH
| Pet          Thomas Petrus             Vector Informatik GmbH
| Dhf          Daniel Hof                Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date        Ver      Author  Description
| ---------   -------  ------  --------------------------------------------------
| 2003-03-31  1.00     Pet     intitial version
| 2004-10-04  1.01     Pet     ESCAN00009749: new VECTOR OEM version defines
|                              change core version to V1.17.00
| 2005-07-26  1.02.FF  Pet     change core version to V1.37.00
| 2006-03-14  1.03.FF  Pet     change core version to V2.00.00
|                              Add NmBasic support
| 2006-05-05  1.03.00  Pet     change core Version to V2.02.00
| 2006-10-27  1.04.00  Pet     change core Version to V2.08.00
| 2007-07-24  1.05.00  Pet     change core Version to V2.16.00
| 2007-12-06  1.06.00  Dhf     change core Version to V2.22.00
|                              ESCAN00021267: Support of INM OSEK and NM ASR added
|                              ESCAN00023349: extern declaration of ApplCclSmStatusIndication() encapsulated
|                              ESCAN00023372: adaption to support NM ASR for AUTOSAR version 2.1
|                              ESCAN00023403: added support of new API functions
| 2008-07-07  1.07.00  Pet     ESCAN00027041: Add support for new OEM McLaren
|                              change core version to V2.29.00
|****************************************************************************/


/***************************************************************************/
/* Version Defines                                                         */
/***************************************************************************/
/* ##V_CFG_MANAGEMENT ##CQProject : Ccl_Vector CQComponent : Implementation */
#define CCL_VECTOR_VERSION            0x0107
#define CCL_VECTOR_RELEASE_VERSION    0x00




#endif /* ifndef CCL_INC_H */
/************************** end of file ***************************************/ 
/************   Organi, Version 3.7.0 Vector-Informatik GmbH  ************/
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
