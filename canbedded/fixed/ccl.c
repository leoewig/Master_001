/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     ccl.c 
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

#define CCL_MODULE

/***************************************************************************/
/* Include files                                                           */
/***************************************************************************/
#include "v_cfg.h"

#if defined( VGEN_GENY )
#include "v_inc.h"
#endif

#include "ccl.h"

#if defined(V_GEN_ENABLE_ECUM_SMSVE)
#  include "SmSve.h"
#endif

/***************************************************************************/
/* Version check                                                           */
/***************************************************************************/
#if( CCL__CORE_VERSION != 0x0229)
#  error "CCL core: incorrect main or sub version"
#endif

#if( CCL__CORE_RELEASE_VERSION != 0x00)
#  error "CCL core: incorrect release version"
#endif

#if defined( VGEN_GENY )
#  if( CCL__COREDLL_VERSION < 0x0206u )
#    error "Incompatible CCL DLL-Version with this CCL-Implementation!"
#  endif
#  if( CCL__COREDLL_VERSION == 0x0206u )
#    if( CCL__COREDLL_RELEASE_VERSION < 0x07u )
#      error "Incompatible CCL DLL-Version with this CCL-Implementation!"
#    endif
#  endif
#endif


/***************************************************************************/
/* Plausibility checks                                                     */
/***************************************************************************/
#if( CCL_DLL_VERSION < 0x0106 )
#  if((defined(CCL_USE_REQUEST_SETCOM_FCT) && !defined(CCL_USE_REQUEST_CLEARCOM_FCT)) || \
      (!defined(CCL_USE_REQUEST_SETCOM_FCT) && defined(CCL_USE_REQUEST_CLEARCOM_FCT)))
#    error "Not all CCL state notification functions are configured. If you use the state notification then you have to configure all state notification functions !!"
#  endif
#else
#  if((((defined(CCL_USE_REQUEST_SETINTCOM_FCT) || defined(CCL_USE_REQUEST_SETEXTCOM_FCT)) && !defined(CCL_USE_REQUEST_CLEARCOM_FCT)) || \
      (!defined(CCL_USE_REQUEST_SETINTCOM_FCT) || !defined(CCL_USE_REQUEST_SETEXTCOM_FCT)) && defined(CCL_USE_REQUEST_CLEARCOM_FCT)))
#    error "Not all CCL state notification functions are configured. If you use the state notification then you have to configure all state notification functions !!"
#  endif
#endif




/* ESCAN00026013 */
#if( kCclNrOfChannels > 8)
#  error "The CCL does not support more than 8 channels!"
#endif

#if (kCclNumberOfUser > 255)
#  error "The CCL does not support more than 255 user requests!"
#endif

/***************************************************************************/
/* Defines                                                                 */
/***************************************************************************/

#if defined(C_SINGLE_RECEIVE_BUFFER) || defined(C_MULTIPLE_RECEIVE_BUFFER)
#  if !defined(CAN_CHANNEL_CANTYPE_ONLY)
#    define CAN_CHANNEL_CANTYPE_ONLY      void
#  endif
#endif


#  define cclExtWakeUpReq     cclExtCanWakeUpReq
#  if(kCclNrOfChannels > 1)
#    define CclExtComReqFctTbl CclExtCanComReqFctTbl
#  else
#    define CclExtComReqFct    CclExtCanComReqFct
#  endif

#if( CCL_DLL_VERSION < 0x0106 )
#  define CCL_ENABLE_CANBEDDED_HANDLING
#endif

#  if defined ( C_VERSION_REF_IMPLEMENTATION )
#    if( C_VERSION_REF_IMPLEMENTATION < 0x130u )
#      define CanGlobalInterruptDisable  CanInterruptDisable
#      define CanGlobalInterruptRestore  CanInterruptRestore
#    endif
#  else
#    error "The CAN driver reference implementation define is missing!"
#  endif

/* ESCAN00028150 */
#if !defined ( DRV_API_CALL_TYPE )
#  define DRV_API_CALL_TYPE
#endif
#if !defined ( DRV_API_CALLBACK_TYPE )
#  define DRV_API_CALLBACK_TYPE
#endif

/***************************************************************************/
/* Macros                                                                  */
/***************************************************************************/
#if !defined(CAN_RESET_WAKEUP)
/* ESCAN00026778 */
#  if ( ( kCclNrOfSystemChannels > 1 ) || (defined (C_ENABLE_MULTICHANNEL_API) ) )
#        define CanResetWakeup(a) ((void)CanWakeUp((a)))
#  else
#        define CanResetWakeup() ((void)CanWakeUp())
#  endif
#endif

/***************************************************************************/
/* Constants                                                               */
/***************************************************************************/


/***************************************************************************/
/* Local data definitions                                                  */
/***************************************************************************/
#if defined( CCL_ENABLE_EXTERNAL_REQUEST ) 

V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 cclExtCanWakeUpReq[kCclNrOfChannels];
 /*
 * Description        : flag to handle external channel wake up by CAN
 * Access rights      : r/w
 * Val. range / Coding: kCclNoExtCanChWupReq / kCclExtCanChWupReq
 */




#  if defined(CCL_ENABLE_SLEEP_REPETITION)
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 cclCanSleepRepetition[kCclNrOfChannels];
 /*
 * Description        : flag to handle repetition of function "CanSleep"
 *                      due to failed transition from NORMAL to SLEEP mode
 * Access rights      : r/w
 * Val. range / Coding: kCclNoRepeatCanSleep / kCclRepeatCanSleep
 */
#  endif

V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM1 cclCanSleepReturnCode[kCclNrOfChannels];
 /*
 * Description        : return value of function "CanSleep" to ask
 *                      the application about the following handling
 * Access rights      : r/w
 * Val. range / Coding: kCclNoRepeatCanSleep / kCclRepeatCanSleep
 */

#endif

#if defined( CCL_ENABLE_INTERNAL_REQUEST )
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 cclIntNetReq[kCclNrOfNetworks];
 /*
 * Description        : flag to handle internal network request
 * Access rights      : r/w
 * Val. range / Coding: 0..kCclNrOfNetworks
 */

V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 cclIntNetRel[kCclNrOfNetworks];
 /*
 * Description        : flag to handle internal network release
 * Access rights      : r/w
 * Val. range / Coding: 0..kCclNrOfNetworks
 */

V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 cclIntNetRelHistory[kCclNrOfNetworks];
 /*
 * Description        : flag to handle internal network release
 * Access rights      : r/w
 * Val. range / Coding: 0..kCclNrOfNetworks
 */

#endif


V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 cclComHwState[kCclNrOfChannels];
 /*
 * Description        : state of HW to make communication possible
 *                      (PLL, CAN controller, CAN transceiver)
 * Access rights      : r/w
 * Val. range / Coding: 0: kCclComHwEnabled  (HW is enabled by power management)
 *                      1: kCclComHwDisabled (HW is disabled by power management)
 */

V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 cclBusState[kCclNrOfChannels];
 /*
 * Description        : Bus state 
 * Access rights      : r/w
 * Val. range / Coding: 0: kCclBusOn
 *                      1: kCclBusOff
 */

#if defined(CCL_ENABLE_TRCV_PORT_INT)
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 cclWakeUpIntState[kCclNrOfChannels];
 /*
 * Description        : state of wake up interrupt by IO port
 * Access rights      : r/w
 * Val. range / Coding: 0: kCclDisableWakeUpInt (port INT is disabled)
 *                      1: kCclEnableWakeUpInt  (port INT is enabled)
 */
#endif

#if( CCL_DLL_VERSION < 0x0106 )
#else
#  if defined( CCL_ENABLE_CANBEDDED_HANDLING ) 
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 cclStackInit;
 /*
 * Description        : flag to distinguish between the system specific and
 *                      channel specific CANbedded stack intialisation
 * Access rights      : r/w
 * Val. range / Coding: 0: system specific initialisation
 *                      1: channel specific initialisation
 */
#  endif
#endif























/***************************************************************************/
/* Global data definitions                                                 */
/***************************************************************************/
#if defined( CCL_ENABLE_ERROR_HOOK)
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 cclChannelNumber;
 /*
 * Description        : channel number to identify the occured assertion
 * Access rights      : r/w
 * Val. range / Coding: 0..kCclNrOfChannels
 */

V_MEMRAM0 V_MEMRAM1 vuint16 V_MEMRAM2 cclErrorCode;
 /*
 * Description        : error code to identify the occured assertion
 * Access rights      : r/w
 * Val. range / Coding: s. CCL_INC.H
 */

V_MEMRAM0 V_MEMRAM1 vuint16 V_MEMRAM2 cclErrorLine;
 /*
 * Description        : error line to identify the occured assertion
 * Access rights      : r/w
 * Val. range / Coding: s. DRV, IL, NM, TP or DIAG
 */

V_MEMRAM0 V_MEMRAM1 vuint16 V_MEMRAM2 cclComponentID;
 /*
 * Description        : component identifier to identify the occured assertion
 * Access rights      : r/w
 * Val. range / Coding: s. CCL_INC.H
 */
#endif

#if defined(CCL_ENABLE_INTERNAL_REQUEST)
V_MEMRAM0 volatile V_MEMRAM1 vuint8 V_MEMRAM2 cclIntNetState[kCclNetReqTableSize];
 /*
 * Description        : state of internal network request/release
 * Access rights      : r/w
 * Val. range / Coding: 0: network released
 *                      1: network requested
 */

#endif

V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 cclNmState[kCclNrOfChannels];
 /*
 * Description        : state of CCL depending
 *                      on external channel requests and
 *                      internal network requests or releases
 * Access rights      : r/w
 * Val. range / Coding: 0: kCclNmStateSleep (neither external nor internal request)
 *                      1: kCclNmStateGoToSleep (only external request)
 *                      2: kCclNmStateActive (at least internal request)
 *                      3: kCclNmPowerFail (power fail is active)
 */

#if defined( CCL_ENABLE_SW_COM_STATE )
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 cclComSwState[kCclNrOfChannels];
 /*
 * Description        : communication state of CANbedded stack (DRV, IL, NM)
 * Access rights      : r/w
 * Val. range / Coding: 0: kCclComSwOff  (communication is switched off by CCL)
 *                      1: kCclComSwOn   (communication is switched on by CCL)
 */
#endif





/***************************************************************************/
/* Local function prototypes                                               */
/***************************************************************************/
static void CCL_API_CALL_TYPE CclInitPorts(CCL_CHANNEL_CCLTYPE_ONLY);











/***************************************************************************/
/* Local functions                                                         */
/***************************************************************************/




#if defined(CCL_ENABLE_ERROR_HOOK)
/************************************************************************
| NAME:               CclFatalError
| PROTOTYPE:          void CCL_API_CALL_TYPE CclFatalError(vuint8 ChannelNumber, vuint16 ErrorCode, vuint16 ErrorLine, vuint8 ComponentID)
| CALLED BY:          assertions within the CANbedded stack and/or CCL
|                     re-directe the call of the "FatalError"-functions
|                     Application is NOT ALLOWED to call this function!
| PRECONDITIONS:      'use CCL ErrorHook' has to be activated in the used generation tool to notify the application
| INPUT PARAMETERS:   ChannelNumber: 0-255    (255 default for 'NO_CHANNEL_INFO_AVAILABLE')
|                     ErrorCode    : 0-65.535 (individual specified in each layer)
|                     ErrorLine    : __LINE__ (0 if no information is available)
|                     ComponentID  : s. CCL_INC.H
| RETURN VALUE:       void
| DESCRIPTION:        Convert the given error information to the global error handling
|                     variables and call a application function to handle the error.
|*************************************************************************/
void CCL_API_CALL_TYPE CclFatalError(CanChannelHandle ChannelNumber, vuint16 ErrorCode, vuint16 ErrorLine, vuint8 ComponentID)
 {
   /* pin assertion information */
   cclChannelNumber = (vuint8)ChannelNumber; /* channel number */
   cclErrorCode     = ErrorCode;     /* error code */
   cclErrorLine     = ErrorLine;     /* error line */
   cclComponentID   = ComponentID;   /* component ID */

   /* error hook (assertions) */
   ApplCclFatalError();
 }
#endif


/************************************************************************
| NAME:               CclInitPorts
| PROTOTYPE:          void CclInitPorts(CanChannelHandle channel)
|                     or
|                     void CclInitPorts(void)
| CALLED BY:          CclInitPowerOn
|                     Application is NOT ALLOWED to call this function!
| PRECONDITIONS:      to be called in loop for each channel
| INPUT PARAMETERS:   channel (multiple channel)
|                     or
|                     void (single channel)
| RETURN VALUE:       void
| DESCRIPTION:        calls function container to inizialise all HW ports
|                     (both the transceiver port register and the
|                     transceiver port interrupt) with the channel specific
|                     parameters.
|*************************************************************************/
static void CCL_API_CALL_TYPE CclInitPorts(CCL_CHANNEL_CCLTYPE_ONLY)
{
#if defined (CCL_ENABLE_MULTIPLE_CONFIG)
  if((cclIdChMap[V_ACTIVE_IDENTITY_LOG] & (vuint8)((vuint8)1 << (CCL_CHANNEL_CCLINDEX))) > 0x00)
#endif    
  {
#if defined( CCL_ENABLE_TRCV_TYPE_NONE )
   /* nothing to do */
#else
   /* init transceiver port register */
#  if(kCclNrOfChannels > 1)
   if( CclInitTrcvFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
   {
     /* By using the following table of function calls, code-doubled functions 
     * are called from the indexed CCL. This is done to simplify the called 
     * functions (no distinction of the parameter 'channel' is necessary). */
     (CclInitTrcvFctTbl[CCL_CHANNEL_CCLINDEX])();
   }
#  else
   CclInitTrcvFct();
#  endif
#endif

#if defined(CCL_ENABLE_TRCV_PORT_INT)
#  if defined(CCL_ENABLE_EXTERNAL_REQUEST)
   /* init transceiver port interrupt */
#    if(kCclNrOfChannels > 1)
   /* By using the following tables of function calls, code-doubled functions 
  * are called from the indexed CCL. This is done to simplify the called 
  * functions (no distinction of the parameter 'channel' is necessary). */
   if(CclConfigTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
   {
     (CclConfigTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX])();
   }
   if(CclDisableTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
   {
     (CclDisableTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX])();
   }
   if(CclClearTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
   {
     (CclClearTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX])();
   }
#    else
   CclConfigTrcvIntFct();
   CclDisableTrcvIntFct();
   CclClearTrcvIntFct();
#    endif
   /* set INT state to "disable" */
   cclWakeUpIntState[CCL_CHANNEL_CCLINDEX] = kCclDisableWakeUpInt;
#  endif
#endif
  }
 }

#  if defined(CCL_ENABLE_EXTERNAL_REQUEST)
 /************************************************************************
 | NAME:               ApplCanWakeUp
 | PROTOTYPE:          void ApplCanWakeUp(CanChannelHandle channel)
 |                     or
 |                     void ApplCanWakeUp(void)
 | CALLED BY:          CANdrv:  CanWakeUpInt / CanWakeUpTask (internal wake up INT)
 |                     CCL:     CclCanWakeUpInt (external wake up INT)
 |                     Application is NOT ALLOWED to call this function!
 | PRECONDITIONS:      -
 | INPUT PARAMETERS:   channel (multiple channel)
 |                     or
 |                     void (single channel)
 | RETURN VALUE:       void
 | DESCRIPTION:        Function notifies WakeUp over RX/INH transceiver port
 |                     (handled in CCL) or WakeUp over CAN controller
 |                     (handled in CAN driver), sets the external channel
 |                     wakeup request flag and requests the power manager state.
 |*************************************************************************/
void DRV_API_CALLBACK_TYPE ApplCanWakeUp(CAN_CHANNEL_CANTYPE_ONLY)
 {
#    if defined(C_MULTIPLE_RECEIVE_CHANNEL)
#      if( kCclNrOfChannels == 1)
   if(channel == (CanChannelHandle)kCclChannelIndex)
#      endif
#    endif
   {
     /* set external EMC or CAN wake up request */
     cclExtWakeUpReq[CCL_CHANNEL_CCLINDEX] = kCclExtChWupReq;

#    if defined( CCL_ENABLE_INTERNAL_REQUEST)
     if(cclIntNetReq[CCL_CHANNEL_CCLINDEX] == kCclNoIntNetReq) /* ESCAN00011217 and ESCAN00011218 */
#    endif
     {


#    if defined(CCL_USE_REQUEST_SETEXTCOM_FCT)
       /* external EMC or CAN communication request notification */
       if( cclNmState[CCL_CHANNEL_CCLINDEX] == kCclNmStateSleep )
       {
#      if(kCclNrOfChannels > 1)
         if (CclExtComReqFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
         {
           /* By using the following table of function calls, code-doubled functions 
            * are called from the indexed CCL. This is done to simplify the called 
            * functions (no distinction of the parameter 'channel' is necessary). */
           (CclExtComReqFctTbl[CCL_CHANNEL_CCLINDEX])();
         }
#      else
         CclExtComReqFct();
#      endif

#    if defined(V_GEN_ENABLE_ECUM_SMSVE)
         /* CCL is in sleep: check if RUN state of the ECU Manager is not yet requested (e.g. from an internal communication request before): */
         if( EcuM_ComM_HasRequestedRUN(CCL_CHANNEL_CCLPARA_ONLY) == FALSE )
         {
           (void)EcuM_ComM_RequestRUN(CCL_CHANNEL_CCLPARA_ONLY);
         }
#    endif
       }
#  endif

     }
   }
 }
#  endif








/***************************************************************************/
/* Global functions                                                        */
/***************************************************************************/

/************************************************************************
 | NAME:               CclCanNormal
 | PROTOTYPE:          void CclCanNormal(CanChannelHandle channel)
 |                     or
 |                     void CclCanNormal(void)
 | CALLED BY:          CCL
 |                     Application is NOT ALLOWED to call this function!
 | PRECONDITIONS:      -
 | INPUT PARAMETERS:   channel (multiple channel)
 |                     or
 |                     void (single channel)
 | RETURN VALUE:       void
 | DESCRIPTION:        set CAN transceiver and CAN controller to NORMAL mode
 |*************************************************************************/
void CCL_API_CALL_TYPE CclCanNormal(CCL_CHANNEL_CCLTYPE_ONLY)
{

#if defined(CCL_ENABLE_EXTERNAL_REQUEST)
#  if defined(CCL_ENABLE_SLEEP_REPETITION)
  /* stop sleep repetition handling */
  cclCanSleepRepetition[CCL_CHANNEL_CCLINDEX] = kCclNoRepeatCanSleep;
#  endif
#endif

  {
    /* check the actual bus state, to prevent multiple activations */
    if(cclBusState[CCL_CHANNEL_CCLINDEX] == kCclBusOff)
    {
      /* the bus state is off */
#if defined( CCL_ENABLE_TRCV_TYPE_NONE )
   /* nothing to do */
#else
       /* set transceiver to NORMAL mode by port register */
#  if(kCclNrOfChannels > 1)
       if( CclWakeUpTrcvFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
       {
         /* By using the following table of function calls, code-doubled functions 
          * are called from the indexed CCL. This is done to simplify the called 
          * functions (no distinction of the parameter 'channel' is necessary). */
         (CclWakeUpTrcvFctTbl[CCL_CHANNEL_CCLINDEX])();
       }
#  else
       CclWakeUpTrcvFct();
#  endif
#endif

#  if defined(CCL_ENABLE_EXTERNAL_REQUEST)
#    if(kCclNrOfChannels > 1)
       /* check wake up / sleep support */
       if((cclNmWakeUpAble & (vuint8)((vuint8)1 << (CCL_CHANNEL_CCLINDEX))) != kCclNoWakeUpAble)
#    endif
       {
         /* set CAN controller to NORMAL mode */
         /* ESCAN00026778 */
#    if ( ( kCclNrOfSystemChannels > 1 ) || (defined (C_ENABLE_MULTICHANNEL_API) ) )
        CanResetWakeup(CCL_CHANNEL_CANPARA_ONLY);
#    else
        CanResetWakeup();
#    endif

       }
#  endif
       /* set the CAN state to on */
       cclBusState[CCL_CHANNEL_CCLINDEX] = kCclBusOn;
     }
#  if defined(CCL_ENABLE_EXTERNAL_REQUEST)
    else if(cclCanSleepReturnCode[CCL_CHANNEL_CCLINDEX] == kCanFailed)
    {
#    if(kCclNrOfChannels > 1)
      /* check wake up / sleep support */
      if((cclNmWakeUpAble & (vuint8)((vuint8)1 << (CCL_CHANNEL_CCLINDEX))) != kCclNoWakeUpAble)
#    endif
      {
        /* set CAN controller to NORMAL mode */
        /* ESCAN00026778 */
#    if ( ( kCclNrOfSystemChannels > 1 ) || (defined (C_ENABLE_MULTICHANNEL_API) ) ) 
        CanResetWakeup(CCL_CHANNEL_CANPARA_ONLY);
#    else
        CanResetWakeup();
#    endif

      }
    }
#  endif

#if defined(CCL_ENABLE_EXTERNAL_REQUEST)
     /* reset the CanSleep return value */
    cclCanSleepReturnCode[CCL_CHANNEL_CCLINDEX] = kCanOk;
#endif



  }
}

 /************************************************************************
 | NAME:                CclCanStandby
 | PROTOTYPE:           void CclCanStandby(CanChannelHandle channel)
 |                      or
 |                      void CclCanStandby(void)
 | CALLED BY:           CCL
 |                      Application is NOT ALLOWED to call this function!
 | PRECONDITIONS:       -
 | INPUT PARAMETERS:    channel (multiple channel)
 |                      or
 |                      void (single channel)
 | RETURN VALUE:        void
 | DESCRIPTION:         set CAN controller and CAN transceiver to SLEEP mode,
 |                      release the power manager state and enable the
 |                      external wakeup port INT
 |*************************************************************************/
void CCL_API_CALL_TYPE CclCanStandby(CCL_CHANNEL_CCLTYPE_ONLY)
{
  CanGlobalInterruptDisable();

  {
    {
      {
#  if defined(CCL_ENABLE_EXTERNAL_REQUEST) 
#      if(kCclNrOfChannels > 1)
        /* check wake up / sleep support */
        if((cclNmWakeUpAble & (vuint8)((vuint8)1 << (CCL_CHANNEL_CCLINDEX))) != kCclNoWakeUpAble )
#      endif
        {
          /* set CAN controller to SLEEP mode */
          cclCanSleepReturnCode[CCL_CHANNEL_CCLINDEX] = CanSleep(CCL_CHANNEL_CANPARA_ONLY);
        }

         {          
           /* set CCL into wait bus sleep */
           cclNmState[CCL_CHANNEL_CCLINDEX] = kCclStateWaitBusSleep;
         }
         
       /* check "CanSleep" return code */
       if(cclCanSleepReturnCode[CCL_CHANNEL_CCLINDEX] != kCanFailed)
#  endif
       {

#if defined(CCL_ENABLE_EXTERNAL_REQUEST) 
#  if defined(CCL_ENABLE_SLEEP_REPETITION)
         /* stop sleep repetition handling */
         cclCanSleepRepetition[CCL_CHANNEL_CCLINDEX] = kCclNoRepeatCanSleep;
#  endif
#endif

#  if defined( CCL_ENABLE_TRCV_TYPE_NONE )
         /* nothing to do */
#  else
         /* set transceiver to SLEEP or STANDBY mode by port register */
#    if(kCclNrOfChannels > 1)
         if( CclSleepTrcvFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
         {
           /* By using the following table of function calls, code-doubled functions 
           * are called from the indexed CCL. This is done to simplify the called 
           * functions (no distinction of the parameter 'channel' is necessary). */
           (CclSleepTrcvFctTbl[CCL_CHANNEL_CCLINDEX])();
         }
#    else
         CclSleepTrcvFct();
#    endif
#  endif


         /* set the CCL state to SLEEP */
         cclNmState[CCL_CHANNEL_CCLINDEX] = kCclStateSleep;

         /* set the bus state to off */
         cclBusState[CCL_CHANNEL_CCLINDEX] = kCclBusOff;


#if defined(CCL_USE_REQUEST_CLEARCOM_FCT)
         {
           /* communication release notification */
#  if(kCclNrOfChannels > 1)
           if(CclComRelFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
           {
             /* By using the following table of function calls, code-doubled functions 
              * are called from the indexed CCL. This is done to simplify the called 
              * functions (no distinction of the parameter 'channel' is necessary). */
             (CclComRelFctTbl[CCL_CHANNEL_CCLINDEX])();
           }
#  else
         CclComRelFct();
#  endif
         }
#endif

#if defined(CCL_ENABLE_TRCV_PORT_INT)
#  if defined(CCL_ENABLE_INTERNAL_REQUEST)
         if(cclIntNetReq[CCL_CHANNEL_CCLINDEX] == kCclNoIntNetReq)
         {
           /* no internal request is pending */
#  endif
           /* enable transceiver port interrupt */
#  if( kCclNrOfChannels > 1 )
           /* By using the following table of function calls, code-doubled functions 
           * are called from the indexed CCL. This is done to simplify the called 
           * functions (no distinction of the parameter 'channel' is necessary). */
           /* Wtihin the following function calls a maybe pending 
           *  Transceiver interrupt will be cleared! */
           if(CclClearTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
           {
             (CclClearTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX])();
           }
           if(CclEnableTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
           {
             (CclEnableTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX])();
           }
#  else
           /* Within the following function calls a maybe pending 
           *  Transceiver interrupt will be cleared! */
           CclClearTrcvIntFct();
           CclEnableTrcvIntFct();
#  endif

           /* set INT state to "enable" */
           cclWakeUpIntState[CCL_CHANNEL_CCLINDEX] = kCclEnableWakeUpInt;

#  if defined(CCL_ENABLE_TRCV_PORT_EDGE_TRIGGER)
           /* check transceiver port register */
#    if( kCclNrOfChannels > 1 )
           if(CclCheckTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX]() == kCclTrcvWakeIntPending)
           {
             /* By using the following table of function calls, code-doubled functions 
             * are called from the indexed CCL. This is done to simplify the called 
             * functions (no distinction of the parameter 'channel' is necessary). */

             /* disable transceiver port interrupt */
             if(CclDisableTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
             {
               (CclDisableTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX])();
             }
#    else
             if(CclCheckTrcvIntFct() == kCclTrcvWakeIntPending)
             {
               /* disable transceiver port interrupt */
               CclDisableTrcvIntFct();
#    endif

               /* check INT state */
               if(cclWakeUpIntState[CCL_CHANNEL_CCLINDEX] == kCclEnableWakeUpInt)
               {
                 /* external EMC/CAN wake up notification */
                 CclCanWakeUpInt(CCL_CHANNEL_CCLPARA_ONLY);
               }
             }
#  endif
#  if defined(CCL_ENABLE_INTERNAL_REQUEST)
           }
#  endif
#endif

#if defined(V_GEN_ENABLE_ECUM_SMSVE)
         (void)EcuM_ComM_ReleaseRUN(CCL_CHANNEL_CCLPARA_ONLY);
#endif
       }

#if defined(CCL_ENABLE_EXTERNAL_REQUEST) 
#  if defined(CCL_ENABLE_SLEEP_REPETITION)
       cclCanSleepRepetition[CCL_CHANNEL_CCLINDEX] = ApplCclCanStandby(CCL_CHANNEL_CCLPARA_FIRST cclCanSleepReturnCode[CCL_CHANNEL_CCLINDEX]);

       CclAssert( ( ((cclCanSleepRepetition[CCL_CHANNEL_CCLINDEX] == kCclNoRepeatCanSleep) && (cclCanSleepReturnCode[CCL_CHANNEL_CCLINDEX] == kCanOk)) || \
                        ((cclCanSleepRepetition[CCL_CHANNEL_CCLINDEX] == kCclNoRepeatCanSleep) && (cclCanSleepReturnCode[CCL_CHANNEL_CCLINDEX] == kCanNotSupported)) || \
                        ((cclCanSleepRepetition[CCL_CHANNEL_CCLINDEX] == kCclRepeatCanSleep) && (cclCanSleepReturnCode[CCL_CHANNEL_CCLINDEX] == kCanFailed)) \
                       ), kWrongReturnValue)

#  else
       (void)ApplCclCanStandby(CCL_CHANNEL_CCLPARA_FIRST cclCanSleepReturnCode[CCL_CHANNEL_CCLINDEX]);
#  endif
#endif

#if defined(CCL_NM_TYPE_NONE)
#    if defined(CCL_ENABLE_EXTERNAL_REQUEST) 
       /* check "CanSleep" return code */
       if(cclCanSleepReturnCode[CCL_CHANNEL_CCLINDEX] != kCanFailed)
#    endif
       {
#    if(kCclNrOfChannels > 1)
         if((cclChannelWithoutNm & (vuint8)((vuint8)1 << (CCL_CHANNEL_CCLINDEX))) != kCclChWithNm )
#    endif
         {
           ApplCclNmStateChange(CCL_CHANNEL_CCLPARA_FIRST kCclStateWaitBusSleep, kCclStateSleep);
         }
       }
#endif


#if defined(CCL_ENABLE_INTERNAL_REQUEST)
#  if defined(CCL_USE_REQUEST_SETINTCOM_FCT) 
        if(cclIntNetReq[CCL_CHANNEL_CCLINDEX] != kCclNoIntNetReq)
       {
         /* min. one internal request is pending */
#    if defined( CCL_USE_REQUEST_SETINTCOM_FCT )
#      if defined(CCL_ENABLE_EXTERNAL_REQUEST) 
         /* check "CanSleep" return code */
         if(cclCanSleepReturnCode[CCL_CHANNEL_CCLINDEX] != kCanFailed) /* ESCAN00022602 */
#      endif
         {

           {
           
             /* internal communication request notification */
#      if ( kCclNrOfChannels > 1 )
             if (CclIntComReqFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
             {
               /* By using the following table of function calls, code-doubled functions 
                * are called from the indexed CCL. This is done to simplify the called 
                * functions (no distinction of the parameter 'channel' is necessary). */
               (CclIntComReqFctTbl[CCL_CHANNEL_CCLINDEX])();
             }
#      else
             CclIntComReqFct();
#      endif
           }
         }
#    endif

       }
#  endif
#endif
      }
    }
  }

  CanGlobalInterruptRestore();
 }


#if (defined( CCL_ENABLE_EXTERNAL_REQUEST) ||\
     defined( CCL_ENABLE_INTERNAL_REQUEST))
/************************************************************************
| NAME:               CclTask
| PROTOTYPE:          void CclTask( void )
| CALLED BY:          generated task container function
|                     Application is NOT ALLOWED to call this function!
| PRECONDITIONS:
| INPUT PARAMETERS:   void
| RETURN VALUE:       void
| DESCRIPTION:        internal network request handling
|                     internal network release handling
|                     external channel request handling
|                     EMC wake up timeout handling
|                     sleep repetition handling
|*************************************************************************/
void CCL_API_CALL_TYPE CclTask(void)
 {
#  if(kCclNrOfChannels > 1)
   CanChannelHandle channel; /* number of physical channels */
#  endif

#  if defined(CCL_NM_TYPE_NONE)
   vuint8 cclNmStateTmp[kCclNrOfChannels];
#  endif

#  if(kCclNrOfNetworks > 1)
   vuint8 network; /* number of logical networks */
#  endif


#  if(kCclNrOfChannels > 1)
   channel = 0;
#  endif


#  if(kCclNrOfNetworks > 1)
   network = 0;
#  endif



#  if(kCclNrOfNetworks > 1)
   for(network=0; network<kCclNrOfNetworks; network++)
   {
#  endif
     /* ESCAN00010661 
      *  Disable the global interrupt to guarantee data consistency 
      *  if this function is called from interrupt insted of task level. */

#    if(kCclNrOfChannels > 1)
     channel = (CanChannelHandle)network;
#    endif

#  if defined (CCL_ENABLE_MULTIPLE_CONFIG)
     if((cclIdChMap[V_ACTIVE_IDENTITY_LOG] & (vuint8)((vuint8)1 << (CCL_CHANNEL_CCLINDEX))) > 0x00)
     {
#  endif    
     
     /* check HW communication state */
     if(cclComHwState[CCL_CHANNEL_CCLINDEX] == kCclComHwEnabled)
     {
#  if defined( CCL_ENABLE_INTERNAL_REQUEST )
       


#  endif
         CanGlobalInterruptDisable();
#  if defined( CCL_ENABLE_INTERNAL_REQUEST )
/* ===================================================================== */
/* = begin "internal network request handling" ========================= */
/* ===================================================================== */
         
           /* pending internal network request? */
           if((cclIntNetReq[CCL_NET_CCLINDEX] != kCclNoIntNetReq))
           {
#    if defined(V_GEN_ENABLE_ECUM_SMSVE)
             if( EcuM_GetState() == ECUM_STATE_RUN )
             {
#    endif

               /* ESCAN00026441 */
               if(cclNmState[CCL_CHANNEL_CCLINDEX] < kCclNmStateActive)
               {

                   /* container function for transition from SLEEP or GO TO SLEEP to ACTIVE */


#      if(kCclNrOfChannels > 1)
                   /* By using the following table of function calls, code-doubled functions 
                    * are called from the indexed CCL. This is done to simplify the called 
                    * functions (no distinction of the parameter 'channel' is necessary). */
                  (void)CclNmActiveReqFctTbl[CCL_NET_CCLINDEX](cclIntNetReq[CCL_NET_CCLINDEX]);
#      else
                  (void)CclNmActiveReqFct(cclIntNetReq[CCL_NET_CCLINDEX]);
#      endif






#      if defined(CCL_NM_TYPE_NONE)
                 cclNmStateTmp[CCL_CHANNEL_CCLINDEX] = cclNmState[CCL_CHANNEL_CCLINDEX];
#      endif
                 /* set CCL state to ACTIVE */
                 cclNmState[CCL_CHANNEL_CCLINDEX] = kCclNmStateActive;
#      if defined(CCL_NM_TYPE_NONE)
#        if(kCclNrOfChannels > 1)
                 if((cclChannelWithoutNm & (vuint8)((vuint8)1 << (CCL_CHANNEL_CCLINDEX))) != kCclChWithNm )
#        endif
                 {
                   ApplCclNmStateChange(CCL_CHANNEL_CCLPARA_FIRST cclNmStateTmp[CCL_CHANNEL_CCLINDEX], kCclNmStateActive);
                 }
#      endif
             
                 /* ESCAN00026441 */
               }

                 
               /* clear internal network request flag */
               cclIntNetReq[CCL_NET_CCLINDEX] = kCclNoIntNetReq;

               /* clear internal network release history flag */
               cclIntNetRelHistory[CCL_NET_CCLINDEX] = kCclNoIntNetRel;

#    if defined( CCL_ENABLE_EXTERNAL_REQUEST )

               /* clear external CAN channel request flag */
               cclExtCanWakeUpReq[CCL_CHANNEL_CCLINDEX] = kCclNoExtChWupReq;

#      if defined(CCL_ENABLE_SLEEP_REPETITION)
               /* stop sleep repetition handling */
               cclCanSleepRepetition[CCL_CHANNEL_CCLINDEX] = kCclNoRepeatCanSleep;
#      endif
#    endif
#    if defined(V_GEN_ENABLE_ECUM_SMSVE)
             }
#    endif
           }

/* ===================================================================== */
/* = end "internal network request handling" =========================== */
/* ===================================================================== */

/* ===================================================================== */
/* = begin "internal network release handling" ========================= */
/* ===================================================================== */
         if( (cclIntNetRel[CCL_NET_CCLINDEX] != kCclNoIntNetRel) )
         {
           /* no communication request is pending,
            * set the CCL state to prepare sleep */

#    if defined(CCL_NM_TYPE_NONE)
           cclNmStateTmp[CCL_CHANNEL_CCLINDEX] = cclNmState[CCL_CHANNEL_CCLINDEX];
#    endif
           cclNmState[CCL_CHANNEL_CCLINDEX] = kCclNmStateGoToSleep;

           /* container function for transition from active to prepare sleep */
#    if ( kCclNrOfChannels > 1 )
           /* By using the following table of function calls, code-doubled functions 
            * are called from the indexed CCL. This is done to simplify the called 
            * functions (no distinction of the parameter 'channel' is necessary). */
           (CclNmSleepReqFctTbl[CCL_CHANNEL_CCLINDEX])(cclIntNetRel[CCL_NET_CCLINDEX]);
#    else
           CclNmSleepReqFct(cclIntNetRel[CCL_NET_CCLINDEX]);
#    endif
           

#    if defined(CCL_NM_TYPE_NONE)
#      if(kCclNrOfChannels > 1)
           if((cclChannelWithoutNm & (vuint8)((vuint8)1 << (CCL_CHANNEL_CCLINDEX))) != kCclChWithNm )
#      endif
           {
             ApplCclNmStateChange(CCL_CHANNEL_CCLPARA_FIRST cclNmStateTmp[CCL_CHANNEL_CCLINDEX], kCclNmStateGoToSleep);
           }
#    endif

           /* clear internal network release flag */
           cclIntNetRel[CCL_NET_CCLINDEX] = kCclNoIntNetRel;

           /* clear internal network release history flag */
           cclIntNetRelHistory[CCL_NET_CCLINDEX] = kCclNoIntNetRel;


         }
/* ===================================================================== */
/* = end "internal network release handling" =========================== */
/* ===================================================================== */
#  endif

#  if defined(CCL_ENABLE_EXTERNAL_REQUEST)
/* ===================================================================== */
/* = begin "external channel request handling" ========================= */
/* ===================================================================== */

         /* pending external CAN channel request? */
         if(cclExtCanWakeUpReq[CCL_CHANNEL_CCLINDEX] == kCclExtChWupReq)
         {
#    if defined(V_GEN_ENABLE_ECUM_SMSVE)
           if( EcuM_GetState() == ECUM_STATE_RUN )
#    endif
           {

             if( cclNmState[CCL_CHANNEL_CCLINDEX] < kCclNmStateGoToSleep )
             {
#      if defined(CCL_NM_TYPE_NONE)
               /* save old state */
               cclNmStateTmp[CCL_CHANNEL_CCLINDEX] = cclNmState[CCL_CHANNEL_CCLINDEX];
#      endif


               if( cclNmState[CCL_CHANNEL_CCLINDEX] == kCclNmStateSleep )
               {

                 /* set CCL state to prepare sleep */
                 cclNmState[CCL_CHANNEL_CCLINDEX] = kCclNmStateGoToSleep;

                 /* container function for transition from sleep to prepare sleep */
#      if(kCclNrOfChannels > 1)
                 /* By using the following table of function calls, code-doubled functions 
                  * are called from the indexed CCL. This is done to simplify the called 
                  * functions (no distinction of the parameter 'channel' is necessary). */
                 (CclNmPrepareSleepReqFctTbl[CCL_CHANNEL_CCLINDEX])();
#      else
                 CclNmPrepareSleepReqFct();
#      endif



               }
               else
               {
                 /* set CCL state to prepare sleep */
                 cclNmState[CCL_CHANNEL_CCLINDEX] = kCclNmStateGoToSleep;
               }

#      if defined(CCL_NM_TYPE_NONE)
#        if(kCclNrOfChannels > 1)
          
               if((cclChannelWithoutNm & (vuint8)((vuint8)1 << (CCL_CHANNEL_CCLINDEX))) != kCclChWithNm )
#        endif
               {
                 ApplCclNmStateChange(CCL_CHANNEL_CCLPARA_FIRST cclNmStateTmp[CCL_CHANNEL_CCLINDEX], kCclNmStateGoToSleep);
               }
#      endif

#      if defined(CCL_ENABLE_SLEEP_REPETITION)
               /* stop sleep repetition handling */
               cclCanSleepRepetition[CCL_CHANNEL_CCLINDEX] = kCclNoRepeatCanSleep;
#      endif





           }
             /* clear external CAN channel request flag */
             cclExtCanWakeUpReq[CCL_CHANNEL_CCLINDEX] = kCclNoExtChWupReq;
           }
         }
/* ===================================================================== */
/* = end "external channel request handling" =========================== */
/* ===================================================================== */



#    if defined(CCL_ENABLE_SLEEP_REPETITION)
/* ===================================================================== */
/* = begin "sleep repetition handling" =================================== */
/* ===================================================================== */
         /* check sleep repetition handling */
         if(cclCanSleepRepetition[CCL_CHANNEL_CCLINDEX] != kCclNoRepeatCanSleep)
         {
           /* sleep repetition is active,
            * set CAN communication (HW and SW) to SLEEP
            */
           CclCanStandby(CCL_CHANNEL_CCLPARA_ONLY);
         }
/* ===================================================================== */
/* = end "sleep repetition handling" =================================== */
/* ===================================================================== */
#    endif



#  endif
         CanGlobalInterruptRestore();
#  if defined( CCL_ENABLE_INTERNAL_REQUEST )
#  endif
     }
#  if defined (CCL_ENABLE_MULTIPLE_CONFIG)
    }
#  endif
#  if(kCclNrOfNetworks > 1)
  }
#  endif
}
#endif


/************************************************************************
| NAME:               CclInit
| PROTOTYPE:          void CclInit(CanChannelHandle channel)
|                     or
|                     void CclInit(void)
| CALLED BY:          Application during startup the system or while runtime
| PRECONDITIONS:      -
| INPUT PARAMETERS:   channel (multiple channel)
|                     or
|                     void (single channel)
| RETURN VALUE:       void
| DESCRIPTION:        channel specific initialisation of CCL and CANbedded stack
|*************************************************************************/
void CCL_API_CALL_TYPE CclInit(CCL_CHANNEL_CCLTYPE_ONLY)
 {

#if defined( CCL_ENABLE_INTERNAL_REQUEST)
   vuint8 count;
#endif

#if defined( CCL_ENABLE_ERROR_HOOK)
   cclChannelNumber = 0; /* init channel number */
   cclErrorCode     = 0; /* init error code */
   cclErrorLine     = 0; /* init error line */
   cclComponentID   = 0; /* init component ID */
#endif


#if defined( CCL_ENABLE_EXTERNAL_REQUEST )
   /* clear the external CAN channel request flag */
   cclExtCanWakeUpReq[CCL_CHANNEL_CCLINDEX] = kCclNoExtChWupReq;

#    if defined CCL_ENABLE_SLEEP_REPETITION
   /* deactivate sleep repetition */
   cclCanSleepRepetition[CCL_CHANNEL_CCLINDEX] = kCclNoRepeatCanSleep;
#    endif
   /* set the default value kCanOk of the CanSleep return value */
   cclCanSleepReturnCode[CCL_CHANNEL_CCLINDEX] = kCanOk;
#endif

#if defined( CCL_ENABLE_SW_COM_STATE )
   /* disable communication SW state */
   cclComSwState[CCL_CHANNEL_CCLINDEX] = kCclComSwOff;
#endif



#if defined (CCL_ENABLE_MULTIPLE_CONFIG)
   if((cclIdChMap[V_ACTIVE_IDENTITY_LOG] & (vuint8)((vuint8)1 << (CCL_CHANNEL_CCLINDEX))) > 0x00)
   {
#endif



     /* set CCL state to Active */
     cclNmState[CCL_CHANNEL_CCLINDEX] = kCclNmStateActive;


     /* enable communication HW state */
     cclComHwState[CCL_CHANNEL_CCLINDEX] = kCclComHwEnabled;

     /* set CCL bus state to on */
     cclBusState[CCL_CHANNEL_CCLINDEX] = kCclBusOn;

#if defined (CCL_ENABLE_MULTIPLE_CONFIG)
   }
   else
   {


     /* set CCL state to sleep */
     cclNmState[CCL_CHANNEL_CCLINDEX] = kCclNmStateSleep;


     /* disable communication HW state */
     cclComHwState[CCL_CHANNEL_CCLINDEX] = kCclComHwDisabled;

     /* set CCL bus state to of */
     cclBusState[CCL_CHANNEL_CCLINDEX] = kCclBusOff;
   }
#endif

   
#if defined( CCL_ENABLE_INTERNAL_REQUEST )

   {
     {
       for(count=cclStartIndex[CCL_NET_CCLINDEX]; count<=cclStopIndex[CCL_NET_CCLINDEX]; count++)
       {
         /* clear communication request field */
         cclIntNetState[count] = 0x00;
       }

       /* clear internal network request flag */
       cclIntNetReq[CCL_NET_CCLINDEX] = kCclNoIntNetReq;

       /* clear internal network release flag */
       cclIntNetRel[CCL_NET_CCLINDEX] = kCclNoIntNetRel;

       /* clear internal network release history flag */
       cclIntNetRelHistory[CCL_NET_CCLINDEX] = kCclNoIntNetRel;
     }
   }



#endif









   










#if( CCL_DLL_VERSION < 0x0106 )
#else
   /* check initialisation type flag */
#  if defined( CCL_ENABLE_CANBEDDED_HANDLING )
   if(cclStackInit == kCclChannelSpecificInit)
   {
     /* re-initialisation is active */
     /* call container function for CANbedded re-initialisation */
#    if(kCclNrOfChannels > 1)
     /* By using the following table of function calls, code-doubled functions 
     * are called from the indexed CCL. This is done to simplify the called 
     * functions (no distinction of the parameter 'channel' is necessary). */
     (CclInitFctTbl[CCL_CHANNEL_CCLINDEX])(CCL_NM_INITPOINTER_PARA);
#    else
     CclInitFct(CCL_NM_INITPOINTER_PARA);
#    endif
   }
#  endif
#endif
 }

/************************************************************************
| NAME:               CclInitPowerOn
| CALLED BY:          Application during startup the system or while runtime
| PRECONDITIONS:      global interrupts have to be disabled
| RETURN VALUE:       void
| DESCRIPTION:        system specific initialisation of CCL and CANbedded stack
|*************************************************************************/
 void CCL_API_CALL_TYPE CclInitPowerOn(CCL_ECUS_NODESTYPE_ONLY)
{
#if(kCclNrOfChannels > 1)
   CanChannelHandle channel;
#endif




#if( CCL_DLL_VERSION < 0x0106 )
#else
#  if defined( CCL_ENABLE_CANBEDDED_HANDLING ) 
   /* clear repeated initialisation flag */
   cclStackInit = kCclSystemSpecificInit;
#  endif
#endif



#if defined( CCL_ENABLE_CANBEDDED_HANDLING )
#  if defined(CCL_ENABLE_MULTIPLE_CONFIG)
   /* set active identity */
   VSetActiveIdentity(CCL_ECUS_NODESPARA_ONLY);
#  endif
#endif

#if(kCclNrOfChannels > 1)
   /* init channel parameters */
   for(channel=0; channel<(CanChannelHandle)kCclNrOfChannels; channel++)
#endif
   {
     /* initialize the transceiver ports */
     CclInitPorts(CCL_CHANNEL_CCLPARA_ONLY);

     /* inititialize the communication control layer */
     CclInit(CCL_CHANNEL_CCLPARA_ONLY);



   }


#if defined( CCL_ENABLE_CANBEDDED_HANDLING )
#  if defined(CCL_ENABLE_MULTIPLE_NODES)
#    if defined(VGEN_GENY)
   /* set active identity */
   VSetActiveIdentity(CCL_ECUS_NODESPARA_ONLY);
#    endif
#  endif

   /* container function for CANbedded component initialization */
   CclInitPowerOnFct(CCL_NM_INITPOINTER_PARA);
#endif





#if defined( CCL_ENABLE_TRCV_PORT_INT )
   CclAssert(cclNmWakeUpAble!=0,kExtAndIntWakeUp);
#endif

#if( CCL_DLL_VERSION < 0x0106 )
#else
#  if defined( CCL_ENABLE_CANBEDDED_HANDLING ) 
   /* set repeated initialisation flag */
   cclStackInit = kCclChannelSpecificInit;
#  endif
#endif

 }






#if defined(CCL_ENABLE_SYSTEM_STARTUP)
/************************************************************************
| NAME:               CclSystemStartup
| PROTOTYPE:          void CclSystemStartup(void)
| CALLED BY:          Application
| PRECONDITIONS:
| INPUT PARAMETERS:   void
| RETURN VALUE:       void
| DESCRIPTION:        startup handling of CANbedded stack
|*************************************************************************/
void CCL_API_CALL_TYPE CclSystemStartup(void)
 {
#if(kCclNrOfChannels > 1)
   CanChannelHandle channel;

   /* init channel parameters */
   for(channel=0; channel<(CanChannelHandle)kCclNrOfChannels; channel++)
   {
     /* By using the following table of function calls, code-doubled functions 
     * are called from the indexed CCL. This is done to simplify the called 
     * functions (no distinction of the parameter 'channel' is necessary). */

     /* system startup notification */
     (CclSystemStartupFctTbl[CCL_CHANNEL_CCLINDEX])();
   }
#else
   /* system startup notification */
   CclSystemStartupFct();
#endif
 }
#endif


#if defined(CCL_ENABLE_SYSTEM_SHUTDOWN)
/************************************************************************
| NAME:               CclSystemShutdown
| PROTOTYPE:          void CclSystemShutdown(void)
| CALLED BY:          Application
| PRECONDITIONS:      This function has to be called with disabled global
|                     interrupts.
| INPUT PARAMETERS:   void
| RETURN VALUE:       void
| DESCRIPTION:        shutdown handling of CANbedded stack
|*************************************************************************/
void CCL_API_CALL_TYPE CclSystemShutdown(void)
 {

#  if(kCclNrOfChannels > 1)
   CanChannelHandle channel;
#  endif


#  if(kCclNrOfChannels > 1)
   /* init channel parameters */
   for(channel=0; channel<(CanChannelHandle)kCclNrOfChannels; channel++)
#  endif
   {
#  if defined(CCL_ENABLE_MULTIPLE_CONFIG)
     if((cclIdChMap[V_ACTIVE_IDENTITY_LOG] & (vuint8)((vuint8)1 << (CCL_CHANNEL_CCLINDEX))) > 0x00)
     {
#  endif
     /* By using the following table of function calls, code-doubled functions 
     * are called from the indexed CCL. This is done to simplify the called 
     * functions (no distinction of the parameter 'channel' is necessary). */
#  if(kCclNrOfChannels > 1)
       /* system shutdown notification */
       (CclSystemShutdownFctTbl[CCL_CHANNEL_CCLINDEX])();
#  endif
#  if defined(CCL_ENABLE_MULTIPLE_CONFIG)
     }
#  endif
   }
#  if(kCclNrOfChannels > 1)
#  else
#  if defined(CCL_ENABLE_MULTIPLE_CONFIG)
   if((cclIdChMap[V_ACTIVE_IDENTITY_LOG] & (vuint8)((vuint8)1 << (CCL_CHANNEL_CCLINDEX))) > 0x00)
   {
#  endif

     CclSystemShutdownFct();
#    if defined(CCL_ENABLE_MULTIPLE_CONFIG)
   }
#    endif
#  endif

 }
#endif


#if defined(CCL_ENABLE_ERROR_PIN)
/************************************************************************
| NAME:               CclGetErrorPort
| PROTOTYPE:          vuint8 CclGetErrorPort(CanChannelHandle channel)
|                     or
|                     vuint8 CclGetErrorPort(void)
| CALLED BY:          Application
| PRECONDITIONS:
| INPUT PARAMETERS:   channel (multiple channel)
|                     or
|                     void (single channel)
| RETURN VALUE:       state of transceiver error port
| DESCRIPTION:        returns the current transceiver error port state
|*************************************************************************/
vuint8 CCL_API_CALL_TYPE CclGetErrorPort(CCL_CHANNEL_CCLTYPE_ONLY)
 {
   /* get transceiver error port value */
#if(kCclNrOfChannels > 1)
   /* By using the following table of function calls, code-doubled functions 
   * are called from the indexed CCL. This is done to simplify the called 
   * functions (no distinction of the parameter 'channel' is necessary). */
   if(CclGetTrcvErrorPortFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
   {
     return (CclGetTrcvErrorPortFctTbl[CCL_CHANNEL_CCLINDEX])();
   }
   else
   {
     /* no transceiver error port information available */
     return kCclNoErrPort;
   }
#  else
   return CclGetTrcvErrorPortFct();
#  endif
 }
#endif


/************************************************************************
| NAME:               CclGetChannelState
| PROTOTYPE:          vuint8 CclGetChannelState(CanChannelHandle channel)
|                     or
|                     vuint8 CclGetChannelState(void)
| CALLED BY:          Application
| PRECONDITIONS:
| INPUT PARAMETERS:   channel (multiple channel)
|                     or
|                     void (single channel)
| RETURN VALUE:       kCclNmStateSleep, sleep state 
|                     kCclNmStateGoToSleep, prepare sleep state
|                     kCclNmStateActive, normal state
|                     kCclNmPowerFail, power fail state
| DESCRIPTION:        returns the current CCL state
|*************************************************************************/
vuint8 CCL_API_CALL_TYPE CclGetChannelState(CCL_CHANNEL_CCLTYPE_ONLY)
{
  return cclNmState[CCL_CHANNEL_CCLINDEX];
}


#if (defined(CCL_ENABLE_EXTERNAL_REQUEST) &&\
     defined(CCL_ENABLE_TRCV_PORT_INT))
/************************************************************************
| NAME:               CclCanWakeUpInt
| PROTOTYPE:          void CclCanWakeUpInt(CanChannelHandle channel)
|                     or
|                     void CclCanWakeUpInt(void)
| CALLED BY:          application wakeup-ISR
| PRECONDITIONS:      external wakeup port INT via RX/INH of CAN transceiver
| INPUT PARAMETERS:   channel (multiple channel)
|                     or
|                     void (single channel)
| RETURN VALUE:       void
| DESCRIPTION:        disable the external wakeup port INT and notify CCL
|                     the wakeup event
|*************************************************************************/
void CCL_API_CALL_TYPE CclCanWakeUpInt(CCL_CHANNEL_CCLTYPE_ONLY)
{
   CclAssert(cclWakeUpIntState[CCL_CHANNEL_CCLINDEX]==kCclDisablePortIRQ, kAssertNotSleepAndTRCVINTactive);

   /* disable port interrupt */
#  if (kCclNrOfChannels > 1)
   /* By using the following table of function calls, code-doubled functions 
   * are called from the indexed CCL. This is done to simplify the called 
   * functions (no distinction of the parameter 'channel' is necessary). */
   if(CclDisableTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
   {
     (CclDisableTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX])();
   }
#  else
   CclDisableTrcvIntFct();
#  endif

   /* set INT state to "disable" */
   cclWakeUpIntState[CCL_CHANNEL_CCLINDEX] = kCclDisableWakeUpInt;

   ApplCanWakeUp(CCL_CHANNEL_CANPARA_ONLY);
}
#endif



#if defined( CCL_ENABLE_INTERNAL_REQUEST )
#  if defined( CCL_ENABLE_COM_REQ_HANDLING_API )
/************************************************************************
| NAME:               CclRequestCommunication
| PROTOTYPE:          vuint8 CCL_API_CALL_TYPE CclRequestCommunication(vuint8 cclComHandle)
| CALLED BY:          Application
| PRECONDITIONS:
| INPUT PARAMETERS:   cclComHandle: communication request handle
| RETURN VALUE:       kCclReqOk: the communication request is set correctly
|                     kCclReqFailed: the communication request is stored but
|                                    not handeled
| DESCRIPTION:        request the communication on the CAN channels which
|                     are configured for the cclComHandle
|*************************************************************************/
vuint8 CCL_API_CALL_TYPE CclRequestCommunication(vuint8 cclComHandle)
{
  vuint8 intNetState;
#      if(kCclNetReqTableSize > 1)
  vuint8 index;
#      endif


  vuint8 channel;
  vuint8 retValue;

  /* ESCAN00010661 
   *  Disable the global interrupt to guarantee data consistency 
   *  if this function is called from interrupt insted of task level. */
  CanGlobalInterruptDisable();
 
  /* to avoid compiler warnings */
  channel = 0;
   
  /* set the return value to ok */
  retValue = kCclReqOk;

  /* check for valid communication handle */
  CclAssert( (cclComHandle < kCclNumberOfUser), kCclErrInvalidComHandle);

  channel = cclUserChannel[cclComHandle]; /*lint !e661*/ /* cclComHandle range is checked by an assertion */


#    if defined (CCL_ENABLE_MULTIPLE_CONFIG)
  CclAssert( (cclIdChMap[V_ACTIVE_IDENTITY_LOG] & (vuint8)((vuint8)1 << (channel))), kCclWrongChannelHandle);
  
  if((cclIdChMap[V_ACTIVE_IDENTITY_LOG] & (vuint8)((vuint8)1 << (channel))) > 0x00)
  {
#    endif

#    if defined (CCL_ENABLE_NMPASSIVE_MODE) /* ESCAN00023408 */
  /* check for valid NM mode */
  CclAssert( (cclNmPassiveMode[channel] == kCclNmActiveMode ), kCclErrInvalidNmMode);
  
  if( cclNmPassiveMode[channel] == kCclNmActiveMode )
  {
#    endif
  

    intNetState = 0;

    {
      {
        /* the communication request is set the first time for the communication handle,
         * calculate if the request the first for this CAN channel
         */

     

#    if(kCclNetReqTableSize > 1)
          for(index=cclStartIndex[CCL_NET_CCLINDEX]; index<=cclStopIndex[CCL_NET_CCLINDEX]; index++)
          {
            intNetState |= cclIntNetState[index];
          }
#    else
          intNetState = cclIntNetState[0];
#    endif

          if((cclIntNetState[cclUserOffset[cclComHandle]] & cclUserMask[cclComHandle]) == 0)
          {
            /* store the communication request for this handle */
            cclIntNetState[cclUserOffset[cclComHandle]] |= cclUserMask[cclComHandle]; /*lint !e661*/ /* cclComHandle range is checked by an assertion */
            if( intNetState == kCclNoNetRequest )
            {
              /* the communication request is the first for this CAN channel */
              if(cclNmState[channel] == kCclNmStateSleep)
              {
                /* the CCL is in sleep */
                {
                  /* the communication request rrestriction is deactive */
#      if defined(CCL_ENABLE_TRCV_PORT_INT)
                  /* disable transceiver port interrupt */
#        if(kCclNrOfChannels > 1)
                  /* By using the following table of function calls, code-doubled functions 
                   * are called from the indexed CCL. This is done to simplify the called 
                   * functions (no distinction of the parameter 'channel' is necessary). */
                  if(CclDisableTrcvIntFctTbl[channel] != V_NULL)
                  {
                    (CclDisableTrcvIntFctTbl[channel])();
                  }
#        else
                  CclDisableTrcvIntFct();
#        endif
                  /* set the port interrupt state to deactive */
                  cclWakeUpIntState[channel] = kCclDisableWakeUpInt;
#      endif

#      if defined( CCL_USE_REQUEST_SETINTCOM_FCT )
                  /* inform the application about the first communication request */
#        if ( kCclNrOfChannels > 1 )
                  if (CclIntComReqFctTbl[channel] != V_NULL)
                  {
                    /* By using the following table of function calls, code-doubled functions 
                     * are called from the indexed CCL. This is done to simplify the called 
                     * functions (no distinction of the parameter 'channel' is necessary). */
                    (CclIntComReqFctTbl[channel])();
                  }
#        else
                  CclIntComReqFct();
#        endif
#    endif
#    if defined(V_GEN_ENABLE_ECUM_SMSVE)
                  /* CCL is in sleep: check if RUN state of the ECU Manager is not yet requested (e.g. from an external wake up event before): */
                  if( EcuM_ComM_HasRequestedRUN(CCL_CHANNEL_CCLPARA_ONLY) == FALSE )
                  {
                    (void)EcuM_ComM_RequestRUN(CCL_CHANNEL_CCLPARA_ONLY);
                  }
#    endif
                }
              }
              /* clear internal network release flag */
              cclIntNetRel[CCL_NET_CCLINDEX] = kCclNoIntNetReq;
              /* set internal network request flag */
              cclIntNetReq[CCL_NET_CCLINDEX] = CCL_CHANNEL_CCLINDEX;
            }
          }

      
      } /* ESCAN00022602 */
    }

#    if ( defined (CCL_ENABLE_NMPASSIVE_MODE) || defined (CCL_ENABLE_MULTIPLE_CONFIG) )
  }
  else
  {
    retValue = kCclReqFailed;
  }
#    endif
   
   /* ESCAN00010661 
    *  Enable the global interrupt before leaving the function. */   
   CanGlobalInterruptRestore();
     
   return retValue;
}
#  endif

#  if defined( CCL_ENABLE_COM_REQ_HANDLING_API )
/************************************************************************
| NAME:               CclReleaseCommunication
| PROTOTYPE:          void CCL_API_CALL_TYPE CclReleaseCommunication(vuint8 cclComHandle)
| CALLED BY:          Application
| PRECONDITIONS:
| INPUT PARAMETERS:   cclComHandle: communication request handle
| RETURN VALUE:       -
| DESCRIPTION:        release the communication request for CAN channels which
|                     are configured for the cclComHandle
|*************************************************************************/
void CCL_API_CALL_TYPE CclReleaseCommunication(vuint8 cclComHandle)
{
   vuint8 intNetState;
   vuint8 channel;
#    if (kCclNetReqTableSize > 1)
   vuint8 index;
#    endif



/* ESCAN00010661 
   *  Disable the global interrupt to guarantee data consistency 
   *  if this function is called from interrupt insted of task level. */
   CanGlobalInterruptDisable();

   /* to avoid compiler warnings */
   channel = 0;
   intNetState = 0;

   /* check for valid communication handle */
   CclAssert( (cclComHandle < kCclNumberOfUser), kCclErrInvalidComHandle);

   channel = cclUserChannel[cclComHandle]; /*lint !e661*/ /* cclComHandle range is checked by an assertion */

#    if defined(CCL_ENABLE_MULTIPLE_CONFIG)
   if((cclIdChMap[V_ACTIVE_IDENTITY_LOG] & (vuint8)((vuint8)1 << (CCL_CHANNEL_CCLINDEX))) > 0x00)   
   {
#    endif

#    if defined (CCL_ENABLE_NMPASSIVE_MODE) /* ESCAN00023408 */
   /* check for valid NM mode */
   CclAssert( (cclNmPassiveMode[channel] == kCclNmActiveMode ), kCclErrInvalidNmMode);
  
   if( cclNmPassiveMode[channel] == kCclNmActiveMode )
   {
#    endif
   
     
       
         if( (cclIntNetState[cclUserOffset[cclComHandle]] & cclUserMask[cclComHandle]) != kCclNoNetRequest ) /*lint !e661*/ /* cclComHandle range is checked by an assertion */
         {
           /* the communication request of the cclComHandle handle is released the first time,
            * clear communication request for the cclComHandle handle */
           cclIntNetState[cclUserOffset[cclComHandle]] &= (vuint8)(~cclUserMask[cclComHandle]); /*lint !e661*/ /* cclComHandle range is checked by an assertion */

           /* calculate if there are other communication requests pending */
  #    if(kCclNetReqTableSize > 1)
           for(index=cclStartIndex[CCL_NET_CCLINDEX]; index<=cclStopIndex[CCL_NET_CCLINDEX]; index++)
           {
             intNetState |= cclIntNetState[index];
           }
  #    else
           intNetState = cclIntNetState[0];
  #    endif

           if(intNetState == kCclNoNetRequest)
           {
             /* no communication requests are pending for the CAN channel */
               /* communication restriction is deactive,
                * check if an internal request is pending */
               if(cclIntNetReq[CCL_NET_CCLINDEX] == kCclNoIntNetReq)
               {
                 /* no internal network request is pending ,
                  * set the internal release network flag */
                 cclIntNetRel[CCL_NET_CCLINDEX] = channel;

                 /* set internal network release history flag */
                 cclIntNetRelHistory[CCL_NET_CCLINDEX] = channel;
               }
               else
               {
                 /* clear internal network request flag */
                 cclIntNetReq[CCL_NET_CCLINDEX] = kCclNoIntNetReq;

                 /* internal request is pending ,
                   * check the CCL state */
#  if defined(CCL_ENABLE_EXTERNAL_REQUEST)
                 /* and check if no external wake up request is pending: */
                 /* ESCAN00022666 */
                 if((cclNmState[channel] == kCclNmStateSleep) && (cclExtCanWakeUpReq[channel] == kCclNoExtChWupReq))
#  else
                 if(cclNmState[channel] == kCclNmStateSleep)
#  endif
                 {
                   {
                     /* inform the application about the state change to sleep */
#    if defined(CCL_USE_REQUEST_CLEARCOM_FCT)
#      if(kCclNrOfChannels > 1)
                     if(CclComRelFctTbl[channel] != V_NULL)
                     {
                       (CclComRelFctTbl[channel])();
                     }
#      else
                      
                     CclComRelFct();
#      endif
#    endif
                   }

#  if defined(V_GEN_ENABLE_ECUM_SMSVE)   
                   (void)EcuM_ComM_ReleaseRUN(CCL_CHANNEL_CCLPARA_ONLY);
#  endif

                 }
                  
                 else
                 {
                   if(cclIntNetRelHistory[CCL_NET_CCLINDEX] != kCclNoIntNetRel)
                   {
                     /* ESCAN00020286 */
                     /* internal release network */
                     cclIntNetRel[CCL_NET_CCLINDEX] = channel; /* ESCAN00021188 */
                   }
                 }
               }
#  if defined(CCL_ENABLE_TRCV_PORT_INT)            
               if(cclNmState[channel] == kCclNmStateSleep)
               {
#    if defined(CCL_ENABLE_EXTERNAL_REQUEST)
                 /* clear external CAN network request */
                 if(cclExtCanWakeUpReq[CCL_CHANNEL_CCLINDEX] == kCclNoExtChWupReq)
#    endif
                 {
                   if(cclWakeUpIntState[CCL_CHANNEL_CCLINDEX] == kCclDisableWakeUpInt)
                   {
                     /* enable transceiver port interrupt */
#    if( kCclNrOfChannels > 1 )
                     /* By using the following table of function calls, code-doubled functions 
                      * are called from the indexed CCL. This is done to simplify the called 
                      * functions (no distinction of the parameter 'channel' is necessary). */
            
                     /* Within the following function calls a maybe pending 
                      *  Transceiver interrupt will be cleared! */ 
                     if(CclClearTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
                     {                     
                       (CclClearTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX])();
                     }
                     if(CclEnableTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
                     {
                       (CclEnableTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX])();
                     }
  #      else
                     /* Wtihin the following function calls a maybe pending 
                      *  Transceiver interrupt will be cleared! */
                     CclClearTrcvIntFct();
                     CclEnableTrcvIntFct();
  #      endif

                     /* set INT state to "enable" */
                     cclWakeUpIntState[CCL_CHANNEL_CCLINDEX] = kCclEnableWakeUpInt;

  #      if defined(CCL_ENABLE_TRCV_PORT_EDGE_TRIGGER)
                     /* check transceiver port register */
  #        if( kCclNrOfChannels > 1 )
                     if(CclCheckTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX]() == kCclTrcvWakeIntPending)
                     {
                       /* By using the following table of function calls, code-doubled functions 
                        * are called from the indexed CCL. This is done to simplify the called 
                        * functions (no distinction of the parameter 'channel' is necessary). */

                       /* disable transceiver port interrupt */
                       if(CclDisableTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
                       {
                         (CclDisableTrcvIntFctTbl[CCL_CHANNEL_CCLINDEX])();
                       }
  #        else
                       if(CclCheckTrcvIntFct() == kCclTrcvWakeIntPending)
                       {
                         /* disable transceiver port interrupt */
                         CclDisableTrcvIntFct();
  #        endif
                     
                         /* check INT state */
                         if(cclWakeUpIntState[CCL_CHANNEL_CCLINDEX] == kCclEnableWakeUpInt)
                         {
                           /* external EMC/CAN wake up notification */
                           CclCanWakeUpInt(CCL_CHANNEL_CCLPARA_ONLY);
                         }
                       }
  #      endif
                     }
                   }
                 }
  #    endif
               /* clear internal network request flag */
               cclIntNetReq[CCL_NET_CCLINDEX] = kCclNoIntNetReq; /* ESCAN00011503 */
          }
        }
#    if defined (CCL_ENABLE_NMPASSIVE_MODE)
  }
#    endif

#    if defined(CCL_ENABLE_MULTIPLE_CONFIG)
  }
#    endif


   /* ESCAN00010661 
   *  Enable the global interrupt before leaving the function. */
   CanGlobalInterruptRestore();
 }
#  endif

#  if defined( CCL_ENABLE_COM_REQ_HANDLING_API )
/************************************************************************
| NAME:               CclGetComHandleState
| PROTOTYPE:          vuint8 CCL_API_CALL_TYPE CclGetComHandleState(vuint8 cclComHandle)
| CALLED BY:          Application
| PRECONDITIONS:
| INPUT PARAMETERS:   cclComHandle: communication request handle
| RETURN VALUE:       kCclComReqPending: the communication request is set
|                     kCclComReqNotPending: the communication request is not set
| DESCRIPTION:        Return the communication handle request state
|*************************************************************************/
vuint8 CCL_API_CALL_TYPE CclGetComHandleState(vuint8 cclComHandle)
{
  vuint8 retValue;

  retValue = kCclComReqNotPending;

  /* disable glaobal interrupts */
  CanGlobalInterruptDisable();

  /* check if the communication request is set */
  if( (cclIntNetState[cclUserOffset[cclComHandle]] & cclUserMask[cclComHandle]) != kCclNoNetRequest )
  {
    /* the communication request is not set */
    retValue = kCclComReqPending;
  }

  /* enable global interrupts */
  CanGlobalInterruptRestore();

  return retValue;
}


/************************************************************************
| NAME:               CclComRequestPending
| PROTOTYPE:          vuint8 CCL_API_CALL_TYPE CclComRequestPending(void) or
|                     vuint8 CCL_API_CALL_TYPE CclComRequestPending(CanChannelHandle channel)
| CALLED BY:          Application
| PRECONDITIONS:      
| INPUT PARAMETERS:   channel: communication request handle
| RETURN VALUE:       kCclComReqPending: minimum one communication request is set
|                                        for the channel
|                     kCclComReqNotPending: no communication request is set
|                                           for the channel
| DESCRIPTION:        Return the communication state for the CAN channel
|*************************************************************************/
vuint8 CCL_API_CALL_TYPE CclComRequestPending(CCL_CHANNEL_CCLTYPE_ONLY)
{
  vuint8 retValue;
  vuint8 intNetState;
#    if(kCclNetReqTableSize > 1)
  vuint8 index;
#    endif

  intNetState = 0;
  retValue = kCclComReqPending;

  /* disable global interrupts */
  CanGlobalInterruptDisable();

  /* calculate if a communication request is pending */
#    if(kCclNetReqTableSize > 1)
  for(index=cclStartIndex[CCL_CHANNEL_CCLINDEX]; index<=cclStopIndex[CCL_CHANNEL_CCLINDEX]; index++)
  {
    intNetState |= cclIntNetState[index];
  }
#    else
  intNetState = cclIntNetState[CCL_CHANNEL_CCLINDEX];
#    endif

  if( intNetState == kCclNoNetRequest )
  {
    /* no communication request is pending for the CAN channel */
    retValue = kCclComReqNotPending;
  }

  /* enable global interrupts */
  CanGlobalInterruptRestore();

  return retValue;
}
#  endif





#endif /* CCL_ENABLE_INTERNAL_REQUEST */

/************************************************************************
| NAME:               CclEnableCommunication
| PROTOTYPE:          void CclEnableCommunication(vuint8 channel)
|                     or
|                     void CclEnableCommunication(void)
| CALLED BY:          DPM or Application
| PRECONDITIONS:      Not to be called within ISR
|                     If the DPM component is configured than it is not allowed
|                     to call this function by the application.
| INPUT PARAMETERS:   channel (multiple channel)
|                     or
|                     void (single channel)
| RETURN VALUE:       -
| DESCRIPTION:        enable the HW communication state of CAN controller
|                     CAN transceiver and allow the communication (caused
|                     by external channel and internal network requests)
|*************************************************************************/
void CCL_API_CALL_TYPE CclEnableCommunication(CCL_CHANNEL_CCLTYPE_ONLY)
 {
   /* set HW communication state to enable */
   cclComHwState[CCL_CHANNEL_CCLINDEX] = kCclComHwEnabled;
 }

/************************************************************************
| NAME:               CclDisableCommunication
| PROTOTYPE:          void CclDisableCommunication(vuint8 channel)
|                     or
|                     void CclDisableCommunication(void)
| CALLED BY:          DPM or Application
| PRECONDITIONS:      Not to be called within ISR
|                     If the DPM component is configured than it is not allowed
|                     to call this function by the application.
| INPUT PARAMETERS:   channel (multiple channel)
|                     or
|                     void (single channel)
| RETURN VALUE:       -
| DESCRIPTION:        disable the HW communication state of CAN controller
|                     CAN transceiver and prevent the communication (caused
|                     by external channel and internal network requests)
|*************************************************************************/
void CCL_API_CALL_TYPE CclDisableCommunication(CCL_CHANNEL_CCLTYPE_ONLY)
 {
   /* set HW communication state to disable */
   cclComHwState[CCL_CHANNEL_CCLINDEX] = kCclComHwDisabled;
 }






#if defined (CCL_ENABLE_TRCV_TYPE_TLE6251G) /* ESCAN00022973 */
/************************************************************************
| NAME:               CclSetTrcvRxOnlyMode
| PROTOTYPE:          void CclSetTrcvRxOnlyMode(CanChannelHandle channel)
| CALLED BY:          Application from task level
| PRECONDITIONS:
| INPUT PARAMETERS:   channel (multiple receive channel)
|                     or
|                     void (single receive channel)
| RETURN VALUE:       kCclTrcvStateChangePerformed
|                     or
|                     kCclTrcvStateChangeNotPerformed
| DESCRIPTION:        informs the CCL to set the transceiver into the receive
|                     only mode
|*************************************************************************/
vuint8 CclSetTrcvRxOnlyMode(CCL_CHANNEL_CCLTYPE_ONLY)
{
  vuint8 retVal;

  retVal = kCclTrcvStateChangePerformed;
  
  /* CCL must be in state Active or GoToSleep: */
  CclAssert( (cclNmState[CCL_CHANNEL_CCLINDEX] > kCclStateWaitBusSleep), kCclErrWrongCclState);

  if(cclNmState[CCL_CHANNEL_CCLINDEX] > kCclStateWaitBusSleep)
  {
#  if(kCclNrOfChannels > 1)
    if (CclSetTrcvRxOnlyModeFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
    {
      /* By using the following table of function calls, code-doubled functions 
      * are called from the indexed CCL. This is done to simplify the called 
      * functions (no distinction of the parameter 'channel' is necessary). */
      (CclSetTrcvRxOnlyModeFctTbl[CCL_CHANNEL_CCLINDEX])();
    }
#  else
    CclSetTrcvRxOnlyModeFct();
#  endif
  }
  else
  {
    retVal = kCclTrcvStateChangeNotPerformed;
  }

  return retVal;
}

/************************************************************************
| NAME:               CclLeaveTrcvRxOnlyMode
| PROTOTYPE:          void CclLeaveTrcvRxOnlyMode(CanChannelHandle channel)
| CALLED BY:          Application from task level
| PRECONDITIONS:
| INPUT PARAMETERS:   channel (multiple receive channel)
|                     or
|                     void (single receive channel)
| RETURN VALUE:       kCclTrcvStateChangePerformed
|                     or
|                     kCclTrcvStateChangeNotPerformed
| DESCRIPTION:        informs the CCL to set the transceiver back to normal mode
|*************************************************************************/
vuint8 CclLeaveTrcvRxOnlyMode(CCL_CHANNEL_CCLTYPE_ONLY)
{ 
  vuint8 retVal;

  retVal = kCclTrcvStateChangePerformed;

  /* CCL must be in state Active or GoToSleep: */
  CclAssert( (cclNmState[CCL_CHANNEL_CCLINDEX] > kCclStateWaitBusSleep), kCclErrWrongCclState);

  if(cclNmState[CCL_CHANNEL_CCLINDEX] > kCclStateWaitBusSleep)
  {
#  if(kCclNrOfChannels > 1)
    if (CclLeaveTrcvRxOnlyModeFctTbl[CCL_CHANNEL_CCLINDEX] != V_NULL)
    {
      /* By using the following table of function calls, code-doubled functions 
      * are called from the indexed CCL. This is done to simplify the called 
      * functions (no distinction of the parameter 'channel' is necessary). */
      (CclLeaveTrcvRxOnlyModeFctTbl[CCL_CHANNEL_CCLINDEX])();
    }
#  else
    CclLeaveTrcvRxOnlyModeFct();
#  endif
  }
  else
  {
    retVal = kCclTrcvStateChangeNotPerformed;
  }

  return retVal;
}
#endif

#if defined(CCL_NM_TYPE_NONE)
/************************************************************************
| NAME:               CclNmWaitBusSleep
| PROTOTYPE:          void CclNmWaitBusSleep(CanChannelHandle channel)
| CALLED BY:          Application from task level
| PRECONDITIONS:
| INPUT PARAMETERS:   channel (multiple receive channel)
|                     or
|                     void (single receive channel)
| RETURN VALUE:       void
| DESCRIPTION:        informs the CCL to change into the wait bus sleep mode
|*************************************************************************/
void CCL_API_CALL_TYPE CclNmWaitBusSleep(CCL_CHANNEL_CCLTYPE_ONLY)
{
  CclAssert(cclNmState[CCL_CHANNEL_CCLINDEX] == kCclStateGoToSleep, kCclErrWrongStateTransition);

  CclAssert((cclChannelWithoutNm & (vuint8)((vuint8)1 << (CCL_CHANNEL_CCLINDEX))) != kCclChWithNm, kCclWrongChannelHandle );

  CanOffline(CCL_CHANNEL_CANPARA_ONLY);

  CclComWait(CCL_CHANNEL_CCLPARA_ONLY);

  cclNmState[CCL_CHANNEL_CCLINDEX] = kCclStateWaitBusSleep;

  ApplCclNmStateChange(CCL_CHANNEL_CCLPARA_FIRST kCclStateGoToSleep, kCclStateWaitBusSleep);
}

/************************************************************************
| NAME:               CclNmWaitBusSleepCancel
| PROTOTYPE:          void CclNmWaitBusSleepCancel(CanChannelHandle channel)
| CALLED BY:          Application from task level
| PRECONDITIONS:
| INPUT PARAMETERS:   channel (multiple receive channel)
|                     or
|                     void (single receive channel)
| RETURN VALUE:       void
| DESCRIPTION:        informs the CCL to cancel the the wait bus sleep mode
|*************************************************************************/
void CCL_API_CALL_TYPE CclNmWaitBusSleepCancel(CCL_CHANNEL_CCLTYPE_ONLY)
{
  CclAssert(cclNmState[CCL_CHANNEL_CCLINDEX] == kCclStateWaitBusSleep, kCclErrWrongStateTransition);

  CclAssert((cclChannelWithoutNm & (vuint8)((vuint8)1 << (CCL_CHANNEL_CCLINDEX))) != kCclChWithNm, kCclWrongChannelHandle );

  CanOnline(CCL_CHANNEL_CANPARA_ONLY);
  
  CclComResume(CCL_CHANNEL_CCLPARA_ONLY);

  cclNmState[CCL_CHANNEL_CCLINDEX] = kCclStateGoToSleep;
  
  ApplCclNmStateChange(CCL_CHANNEL_CCLPARA_FIRST kCclStateWaitBusSleep, kCclStateGoToSleep);
}


/************************************************************************
| NAME:               CclNmSleep
| PROTOTYPE:          void CclNmSleep(CanChannelHandle channel)
| CALLED BY:          Application from task level
| PRECONDITIONS:
| INPUT PARAMETERS:   channel (multiple receive channel)
|                     or
|                     void (single receive channel)
| RETURN VALUE:       void
| DESCRIPTION:        informs the CCL to change into the sleep mode
|*************************************************************************/
void CCL_API_CALL_TYPE CclNmSleep(CCL_CHANNEL_CCLTYPE_FIRST CanInitHandle initObject )
{
  CclAssert(cclNmState[CCL_CHANNEL_CCLINDEX] == kCclStateWaitBusSleep ,kCclErrWrongStateTransition);

  CclAssert((cclChannelWithoutNm & (vuint8)((vuint8)1 << (CCL_CHANNEL_CCLINDEX))) != kCclChWithNm, kCclWrongChannelHandle );

  CanOffline(CCL_CHANNEL_CANPARA_ONLY);

#  if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
  /* Macros must be encapsulated in this way */
  CanResetBusSleep( CCL_CHANNEL_CCLPARA_ONLY, initObject );
#  else
  CanResetBusSleep( initObject );
#  endif

  CclComStop(CCL_CHANNEL_CCLPARA_ONLY);
  CclCanStandby(CCL_CHANNEL_CCLPARA_ONLY);
}

/************************************************************************
| NAME:               CclComBusOffStart
| PROTOTYPE:          void CclBusOffStart(CanChannelHandle channel)
| CALLED BY:          Application from task level
| PRECONDITIONS:
| INPUT PARAMETERS:   channel (multiple receive channel)
|                     or
|                     void (single receive channel)
| RETURN VALUE:       void
| DESCRIPTION:        informs the CCL about the bus off recovery start phase
|*************************************************************************/
void CCL_API_CALL_TYPE CclComBusOffStart(CCL_CHANNEL_CCLTYPE_ONLY)
{
  CanOffline(CCL_CHANNEL_CANPARA_ONLY);
  CclBusOffStart(CCL_CHANNEL_CCLPARA_ONLY);
}


/************************************************************************
| NAME:               CclComBusOffEnd
| PROTOTYPE:          void CclBusOffEnd(CanChannelHandle channel)
| CALLED BY:          Application from task level
| PRECONDITIONS:
| INPUT PARAMETERS:   channel (multiple receive channel)
|                     or
|                     void (single receive channel)
| RETURN VALUE:       void
| DESCRIPTION:        informs the CCL about the bus off recovery end
|*************************************************************************/
void CCL_API_CALL_TYPE CclComBusOffEnd(CCL_CHANNEL_CCLTYPE_ONLY)
{
  if(cclNmState[CCL_CHANNEL_CCLINDEX] > kCclStateWaitBusSleep)
  {
    CanOnline(CCL_CHANNEL_CANPARA_ONLY);
  }
 
  CclBusOffEnd (CCL_CHANNEL_CCLPARA_ONLY);
}
#endif


/***************************************************************************/
/* Version check                                                           */
/***************************************************************************/

#if (CCL_VECTOR_VERSION != 0x0107)
  #error "CCL VECTOR OEM: incorrect main or sub version !"
#endif

#if (CCL_VECTOR_RELEASE_VERSION != 0x00)
  #error "CCL VECTOR OEM: incorrect release version!"
#endif


V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kCclMainVersion  =(vuint8)(CCL_VECTOR_VERSION >> 8);
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kCclSubVersion   =(vuint8)( (CCL_VECTOR_VERSION) & 0xFF);
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kCclReleaseVersion=(vuint8)( (CCL_VECTOR_RELEASE_VERSION) & 0xFF);

/***************************************************************************/ 
/* Plausibility checks                                                     */ 
/***************************************************************************/
#if defined(NM_TYPE_VECTOR_OSEK) 
#  if !defined(NM_ENABLE_EXTENDED_CALLBACK)
#  error "No extended callbacks activated for NM-OSEK !"
# endif
#endif

#if defined(NM_TYPE_VECTOR_OSEK ) 
#  if defined ( CCL_NM_TYPE_NONE )
#    error "NM type 'None' is not supported together with NM BASIC or NM OSEK!"
#  endif
#endif




/***************************************************************************/ 
/* callback functions                                                      */ 
/***************************************************************************/ 

#if defined(NM_TYPE_VECTOR_OSEK) 
/************************************************************************
| NAME:                ApplNmCanNormal
| PROTOTYPE:           void ApplNmCanNormal(void) or
|                      void ApplNmCanNormal(CanChannelHandle channel)
| CALLED BY:           OSEK-NM from NmOsekInit() and NmTask()
| PRECONDITIONS:
| INPUT PARAMETERS:    void (single channel) or channel (multiple channel)
| RETURN VALUE:        void
| DESCRIPTION:         Set the CAN controller and the CAN transceiver in NORMAL mode
|                      and disable the external wakeup port INT.
|*************************************************************************/
void NM_API_CALLBACK_TYPE ApplNmCanNormal(NM_CHANNEL_APPLTYPE_ONLY)
 {
#  if(kCclNrOfChannels > 1)
#    if defined(NM_ENABLE_INDEXED_NM)
  CclCanNormal(CCL_CHANNEL_CCLPARA_ONLY);
#    else
   CclCanNormal(NM_CAN_CHANNEL);
#    endif
#  else
   CclCanNormal(CCL_CHANNEL_CCLPARA_ONLY);
#  endif
}

/************************************************************************
| NAME:                ApplNmCanSleep
| PROTOTYPE:           void ApplNmCanSleep(void)
|                      void ApplNmCanSleep(CanChannelHandle channel)
| CALLED BY:           OSEK-NM from NmOsekInit() and NmTask()
| PRECONDITIONS:
| INPUT PARAMETERS:    void (single channel) or channel (multiple channel)
| RETURN VALUE:        void
| DESCRIPTION:         Set the CAN controller and the CAN transceiver in SLEEP mode
|                      and enables the external wakeup port INT.
|*************************************************************************/
void NM_API_CALLBACK_TYPE ApplNmCanSleep(NM_CHANNEL_APPLTYPE_ONLY)
 {
#  if(kCclNrOfChannels > 1)
#    if defined(NM_ENABLE_INDEXED_NM)
  CclComStop(CCL_CHANNEL_CCLPARA_ONLY);
   CclCanStandby(CCL_CHANNEL_CCLPARA_ONLY);
#    else
  CclComStop(NM_CAN_CHANNEL);
   CclCanStandby(NM_CAN_CHANNEL);
#    endif
#  else
  CclComStop(CCL_CHANNEL_CCLPARA_ONLY);
   CclCanStandby(CCL_CHANNEL_CCLPARA_ONLY);
#  endif
 }


/************************************************************************
| NAME:                ApplNmCanBusSleep
| PROTOTYPE:           void ApplNmCanBusSleep(void) or
|                      void ApplNmCanBusSleep(CanChannelHandle channel)
| CALLED BY:           OSEK-NM from NmOsekInit() and NmTask()
| PRECONDITIONS:
| INPUT PARAMETERS:    void (single channel) or channel (multiple channel)
| RETURN VALUE:        void
| DESCRIPTION:         s. ApplNmCanSleep
|*************************************************************************/
void NM_API_CALLBACK_TYPE ApplNmCanBusSleep(NM_CHANNEL_APPLTYPE_ONLY)
 {
 }


/************************************************************************
| NAME:               ApplNmBusOff
| PROTOTYPE:          void ApplNmBusOff(void) or 
|                     void ApplNmBusOff(CanChannelHandle channel)
| CALLED BY:          OSEK-NM from NmCanError() (CAN driver error interrupt)
| PRECONDITIONS:
| INPUT PARAMETERS:   void (single channel) or channel (multiple channel)
| RETURN VALUE:       void
| DESCRIPTION:        The CCL handles the configured interaction layer in
 |                    dependency of the CCL configuration.
|*************************************************************************/
void NM_API_CALLBACK_TYPE ApplNmBusOff(NM_CHANNEL_APPLTYPE_ONLY)
 {
#  if(kCclNrOfChannels > 1)
#    if defined(NM_ENABLE_INDEXED_NM)
   CclBusOffStart(CCL_CHANNEL_CCLPARA_ONLY);
#    else
   CclBusOffStart(NM_CAN_CHANNEL);
#    endif
#  else
   CclBusOffStart(CCL_CHANNEL_CCLPARA_ONLY);
#  endif
 }

/************************************************************************
| NAME:               ApplNmBusOffEnd
| PROTOTYPE:          void ApplNmBusOffEnd(void) or
|                     void ApplNmBusOffEnd(CanChannelHandle channel)
| CALLED BY:          OSEK-NM from NmTask()
| PRECONDITIONS:
| INPUT PARAMETERS:   void (single channel) or channel (multiple channel)
| RETURN VALUE:       void
| DESCRIPTION:        The CCL handles the configured interaction layer in
|                     dependency of the CCL configuration.
|*************************************************************************/
void NM_API_CALLBACK_TYPE ApplNmBusOffEnd(NM_CHANNEL_APPLTYPE_ONLY)
 {
#  if(kCclNrOfChannels > 1)
#    if defined(NM_ENABLE_INDEXED_NM)
   CclBusOffEnd(CCL_CHANNEL_CCLPARA_ONLY);
#    else
   CclBusOffEnd(NM_CAN_CHANNEL);
#    endif
#  else
   CclBusOffEnd(CCL_CHANNEL_CCLPARA_ONLY);
#  endif
 }

/************************************************************************
| NAME:               ApplNmBusStart
| PROTOTYPE:          void ApplNmBusStart(void) or
|                     void ApplNmBusStart(CanChannelHandle channel)
| CALLED BY :         OSEK-NM from NmOsekInit() and NmTask()
| PRECONDITIONS:
| INPUT PARAMETERS:   void (single channel) or channel (multiple channel)
| RETURN VALUE:       void
| DESCRIPTION:        Start the communication via interaction layer.
|*************************************************************************/
void NM_API_CALLBACK_TYPE ApplNmBusStart(NM_CHANNEL_APPLTYPE_ONLY)
 {
#  if(kCclNrOfChannels > 1)
#    if defined(NM_ENABLE_INDEXED_NM)
  if( cclNmState[NM_CHANNEL_APPLPARA_ONLY] < kCclStateGoToSleep )
#    else
  if( cclNmState[NM_CAN_CHANNEL] < kCclStateGoToSleep )
#    endif
#  else
  if( cclNmState[CCL_CHANNEL_CCLINDEX] < kCclStateGoToSleep )
#  endif
  {
#  if(kCclNrOfChannels > 1)
#    if defined(NM_ENABLE_INDEXED_NM)
    if( cclNmState[NM_CHANNEL_APPLPARA_ONLY] == kCclNmStateSleep )
#    else
    if( cclNmState[NM_CAN_CHANNEL] == kCclNmStateSleep )
#    endif
#  else
    if( cclNmState[CCL_CHANNEL_CCLINDEX] == kCclNmStateSleep )
#  endif
    {
#  if defined(CCL_USE_REQUEST_SETEXTCOM_FCT)
#    if(kCclNrOfChannels > 1)
#      if defined(NM_ENABLE_INDEXED_NM)
      if(cclExtCanWakeUpReq[NM_CHANNEL_APPLPARA_ONLY] == kCclNoExtChWupReq)
      {
        if (CclExtComReqFctTbl[NM_CHANNEL_APPLPARA_ONLY] != V_NULL)
        {
          /* By using the following table of function calls, code-doubled functions 
          * are called from the indexed CCL. This is done to simplify the called 
           * functions (no distinction of the parameter 'channel' is necessary). */
          (CclExtComReqFctTbl[NM_CHANNEL_APPLPARA_ONLY])();
        }
      }
#      else
      if(cclExtCanWakeUpReq[NM_CAN_CHANNEL] == kCclNoExtChWupReq)
      {
        if (CclExtComReqFctTbl[NM_CAN_CHANNEL] != V_NULL)
        {
          /* By using the following table of function calls, code-doubled functions 
          * are called from the indexed CCL. This is done to simplify the called 
          * functions (no distinction of the parameter 'channel' is necessary). */
          (CclExtComReqFctTbl[NM_CAN_CHANNEL])();
        }
      }
#      endif
#    else
      if(cclExtCanWakeUpReq[CCL_CHANNEL_CCLINDEX] == kCclNoExtChWupReq)
      {
        CclExtComReqFct();
      }
#    endif
#  endif
    }

    /* change CCL state to go to sleep */
#  if(kCclNrOfChannels > 1)
#    if defined(NM_ENABLE_INDEXED_NM)
    cclNmState[NM_CHANNEL_APPLPARA_ONLY] = kCclNmStateGoToSleep;
#    else
    cclNmState[NM_CAN_CHANNEL] = kCclNmStateGoToSleep;
#    endif
#  else
    cclNmState[CCL_CHANNEL_CCLINDEX] = kCclNmStateGoToSleep;
#  endif
  }
  
  /* start the communication */
#  if(kCclNrOfChannels > 1)
#    if defined(NM_ENABLE_INDEXED_NM)
  CclComStart(NM_CHANNEL_APPLPARA_ONLY);
#    else
  CclComStart(NM_CAN_CHANNEL);
#    endif
#  else  
  CclComStart(CCL_CHANNEL_CCLPARA_ONLY);
#  endif
}


/************************************************************************
| NAME:               ApplNmWaitBusSleep
| PROTOTYPE:          void ApplNmWaitBusSleep(void) or
|                     void ApplNmWaitBusSleep(CanChannelHandle channel)
| CALLED BY :         OSEK-NM from NmTask()
| PRECONDITIONS:
| INPUT PARAMETERS:   void (single channel) or channel (multiple channel)
| RETURN VALUE :      void
| DESCRIPTION :       Stop the communication via interaction layer.
|*************************************************************************/
void NM_API_CALLBACK_TYPE ApplNmWaitBusSleep(NM_CHANNEL_APPLTYPE_ONLY)
{
#  if(kCanNumberOfChannels > 1)
#    if defined(NM_ENABLE_INDEXED_NM)
  cclNmState[NM_CHANNEL_APPLPARA_ONLY] = kCclStateWaitBusSleep;
  CclComWait(NM_CHANNEL_APPLPARA_ONLY);
#    else
  cclNmState[NM_CAN_CHANNEL] = kCclStateWaitBusSleep;
  CclComWait(NM_CAN_CHANNEL);
#    endif
#  else
  cclNmState[CCL_CHANNEL_CCLINDEX] = kCclStateWaitBusSleep;
  CclComWait(CCL_CHANNEL_CCLPARA_ONLY);
#  endif
}

/************************************************************************
| NAME:               ApplNmWaitBusSleepCancel
| PROTOTYPE:          void ApplNmWaitBusSleepCancel(void) or
|                     void ApplNmWaitBusSleepCancel(CanChannelHandle channel)
| CALLED BY :         OSEK-NM from NmTask()
| PRECONDITIONS:
| INPUT PARAMETERS:   void (single channel) or channel (multiple channel)
| RETURN VALUE :      void
| DESCRIPTION :       Start the communication via interaction layer.
|*************************************************************************/
void NM_API_CALLBACK_TYPE ApplNmWaitBusSleepCancel(NM_CHANNEL_APPLTYPE_ONLY)
{
#  if(kCclNrOfChannels > 1)
#    if defined(NM_ENABLE_INDEXED_NM)
  if( cclNmState[NM_CHANNEL_APPLPARA_ONLY] == kCclStateWaitBusSleep )
  {
    cclNmState[NM_CHANNEL_APPLPARA_ONLY] = kCclStateGoToSleep;
  }
#    else
  if( cclNmState[NM_CAN_CHANNEL] == kCclStateWaitBusSleep )
  {
    cclNmState[NM_CAN_CHANNEL] = kCclStateGoToSleep;
  }
#    endif
#  else
  if( cclNmState[CCL_CHANNEL_CCLINDEX] == kCclStateWaitBusSleep )
  {
    cclNmState[CCL_CHANNEL_CCLINDEX] = kCclStateGoToSleep;
  }
#  endif
  
#  if(kCclNrOfChannels > 1)
#    if defined(NM_ENABLE_INDEXED_NM)
  CclComResume(NM_CHANNEL_APPLPARA_ONLY);
#    else
  CclComResume(NM_CAN_CHANNEL);
#    endif
#  else
  CclComResume(CCL_CHANNEL_CCLPARA_ONLY);
#  endif
}
#endif





/************   Organi, Version 3.7.0 Vector-Informatik GmbH  ************/
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
