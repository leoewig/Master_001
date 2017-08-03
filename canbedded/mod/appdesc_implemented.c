/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     appdesc.c
 *
 * description:   Implementation example for the proper usage with CANdesc.
 *
 *
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Implementation example for the proper usage with CANdesc.
 *
 *****************************************************************************/

/* -----------------------------------------------------------------------------
  C O P Y R I G H T
 -------------------------------------------------------------------------------
  Copyright (c) 2001-2006 by Vector Informatik GmbH. All rights reserved.

  This software is copyright protected and proprietary to Vector Informatik
  GmbH.

  Vector Informatik GmbH grants to you only those rights as set out in the
  license conditions.

  All other rights remain with Vector Informatik GmbH.
 -------------------------------------------------------------------------------
 ----------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
    &&&~ History
 ----------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
  Please note, that the demo and example programs only show special aspects of
  the software. With regard to the fact that these programs are meant for
  demonstration purposes only, Vector Informatik´s liability shall be expressly
  excluded in cases of ordinary negligence, to the extent admissible by law or
  statute.
  ---------------------------------------------------------------------------- */

/*
------------------------------------------------------------------------------------------------------------------------
                                             A U T H O R   I D E N T I T Y
------------------------------------------------------------------------------------------------------------------------
Initials   Name                  Company
---------- --------------------- ---------------------------------------------------------------------------------------
Ade        Alexander Ditte       Vector Informatik GmbH
Her        Peter Herrmann        Vector Informatik GmbH
Ktw        Katrin Thurow         Vector Informatik GmbH
Mhe        Matthias Heil         Vector Informatik GmbH
Sa         Mishel Shishmanyan    Vector Informatik GmbH
------------------------------------------------------------------------------------------------------------------------
                                            R E V I S I O N   H I S T O R Y
------------------------------------------------------------------------------------------------------------------------
Date       Version  Author Modification type Issue Id      Affects                        Description
---------- -------- ------ ----------------- ------------- ------------------------------ -------------------------------
2006-10-20 05.00.00
                    Sa     Modified          ESCAN00016884 UDS                            ComControl service compliance with the latest ISO-UDS (2005-05-29).
                    Sa     Modified          ESCAN00018098 All                            Unified security access management and API.
                    Sa     Added             ESCAN00017752 All                            OBD II compliance with regard to the NRC usage and response behavior.
                    Sa     Added             ESCAN00018099 All                            Add support for CANdela format 5.0.
2007-02-16 05.01.00
                    Sa     Fixed             ESCAN00018114 Vector(UDS)                    CANdesc not compilable if permanent repeated call is turned on.
                    Sa     Fixed             ESCAN00018916 Vector(UDS)                    Compile error on multi-channel system and service $28 support.
                    Sa     Fixed             ESCAN00018576 All                            Service instance linear look-up routine fails finding a requested service.
                    Sa     Fixed             ESCAN00019220 All                            Compile error for wrong syntax after preprocesing defines: DescUsdtNetStateTask and DescUsdtNetTimerTask.
                    Sa     Fixed             ESCAN00019334 All                            Generator: Unable to generate CANdesc with multi channel TP and CANgen newer than 4.30.03.
                    Sa     Modified          ESCAN00018872 All                            Wrong main-handler comment for constants names kDescReqTypePhys (-Func).
                    Sa     Modified          ESCAN00019321 All                            Remove file-path from the #include "node.h"
                    Sa     Modified          ESCAN00019735 All                            Use CPU type optimized data definition types (uintx_least).
                    Sa     Added             ESCAN00018320 All                            Generate P2 and P2* task-call constants with a justification of one task call less.
                    Sa     Added             ESCAN00018592 All                            DescRingBufferCancel API added.
2007-04-13 05.02.00
                    Sa     Added             ESCAN00020069 UDS                            Support for unified PID application handling.
2007-05-30 05.03.00
                    Sa     Fixed             ESCAN00020760 UDS                            Service $22 responds with wrong NRC $13 instead of $31 in case single DID mode has been selected
                    Sa     Modified          ESCAN00020840 All                            Compiler error message on local variable pointer to ROM using V_MEMROM0 define
                    Sa     Modified          ESCAN00019437 All                            Compiler error in desc.c with mem-model huge
2007-07-23 05.04.00
                    Sa     Added             ESCAN00021529 Vector(UDS)                    Support for services $27, $2C and $2A
                    Sa     Added             ESCAN00021530 UDS                            Support for custom rx communication path handling on service $28
                    Sa     Fixed             ESCAN00021336 All                            use lower case characters for names of generated files (AppDesc.h => appdesc.h)
                    Sa     Modified          ESCAN00021523 All                            Removed the first frame length dependency on Vector TP for for ring-buffer usage
                    Sa     Added             ESCAN00021528 All                            Stopping RepeatedServiceCall in RingBufferCancel
                    Sa     Added             ESCAN00021567 All                            Use case catch on RingBuffer usage with SPRMIB=1
2007-09-07 05.05.00
                    Sa     Fixed             ESCAN00022280 Vector(UDS)                    Linker error on API DescOemProcessSecurityAccess
                    Sa     Fixed             ESCAN00021610 UDS                            Incorrect NRC on an invalidly formatted writeDataByIdentifier ($2e) request for non-writeable DID
                    Sa     Fixed             ESCAN00021615 UDS                            Incorrect NRC on service with parameter identifier not supported in active session
                    Sa     Fixed             ESCAN00021800 UDS                            Wrong response lenght on service $22 in single DID mode
                    Sa     Added             ESCAN00022074 UDS                            Unified PID support on service $2E
                    Sa     Added             ESCAN00022075 UDS                            Unified PID support on service $22 in single PID mode
                    Sa     Fixed             ESCAN00022068 All                            Include appDesc.h changed to appdesc.h
                    Sa     Modified          ESCAN00021654 All                            Explicit suppression of NRC $11 on functional request
                    Sa     Modified          ESCAN00021252 All                            Compiler warnings for unreferenced variable and function declaration
                    Sa     Modified          ESCAN00022254 All                            Adaptions for the interrupt support of the latest VStdLib version
                    Sa     Added             ESCAN00021847 All                            Access to the current communication parameters
2007-10-22 05.05.01
                    Sa     Fixed             ESCAN00022373 Vector(UDS)                    Service $2C processor deletes all DID definitions on command clear single DID definition
                    Sa     Fixed             ESCAN00022658 Vector(UDS)                    Compiler warning: undefined macro 'kDescSecureTimer' is taken as 0
                    Sa     Modified          ESCAN00022853 UDS                            Allow for service $10 the P2 time report not to be present
2007-10-26 05.05.02
                    -      -                 -             -                              No relevant changes available in this version.
2007-11-15 05.05.03
                    Sa     Added             ESCAN00023231 All                            Support multiple ECU numbers
2007-12-17 05.05.04
                    Sa     Added             ESCAN00023592 Vector(UDS)                    Support of Autosar DEM API
                    Sa     Added             ESCAN00023684 Vector(UDS)                    Automatic detection of P2/P2Ex parameters for service $10
2007-12-20 05.06.00
                    Sa     Fixed             ESCAN00022373 Vector(UDS)                    Service $2C processor deletes all DID definitions on command clear single DID definition
                    Sa     Fixed             ESCAN00022658 Vector(UDS)                    Compiler warning: undefined macro 'kDescSecureTimer' is taken as 0
                    Sa     Fixed             ESCAN00023846 Vector(UDS)                    Service $2C processor always deletes all DID definitions on command clear single DID definition
                    Sa     Added             ESCAN00023684 Vector(UDS)                    Automatic detection of P2/P2Ex parameters for service $10
                    Sa     Modified          ESCAN00022853 UDS                            Allow for service $10 the P2 time report not to be present
                    Sa     Modified          ESCAN00022868 UDS                            Consider CDD files having only SID based information (CANdescBasic like)
                    Sa     Added             ESCAN00023231 All                            Support multiple ECU numbers
2008-01-22 05.06.01
                    Sa     Fixed             ESCAN00023944 Vector(UDS)                    Invalid data responded with service $22 (ReadDataByIdentifier)
                    Sa     Modified          ESCAN00023488 Vector(UDS)                    The example code for SecurityAccess seed writes the SEED in intel instead of motorola format
                    Mhe    Added             ESCAN00024016 Vector(UDS)                    Support for Bosch UAcc interface generated main handlers
                    Sa     Modified          ESCAN00024147 UDS                            Specify suppressPosRes DescMsgContext member as read-only item and not read-write
                    Sa     Fixed             ESCAN00026046 All                            CANdesc responds on wrong CAN Id
                    Mhe    Added             ESCAN00024011 All                            Support dynamic normal addressing
                    Mhe    Added             ESCAN00024019 All                            Support far RAM Tp API
2008-02-04 05.06.02
                    -      -                 -             -                              No relevant changes available in this version.
2008-01-11 05.06.03
                    -      -                 -             -                              No relevant changes available in this version.
2008-03-14 05.06.04
                    Mhe    Added             ESCAN00024951 UDS                            Internal support for read/write memory by address
                    Sa     Added             ESCAN00024929 UDS                            Allow unified PID handling for service $22 wihtout multiple PID support
2008-04-03 05.06.05
                    Sa     Fixed             ESCAN00025410 Vector(UDS)                    Virtual requests, triggered by the RoE handler, restart the S1 timer
                    Sa     Fixed             ESCAN00025381 Vector(UDS)                    Context specific access to UACC functions
                    Sa     Fixed             ESCAN00025753 UDS                            Supported PIDs will not be recognized if service $2F is also available in the system
2008-04-21 05.06.06
                    Sa     Fixed             ESCAN00026438 Vector(UDS)                    Deadlock on RoE with a monitored DDDID
                    Her    Fixed             ESCAN00026454 Vector(UDS)                    RoE processing does not match the specification
                    Her    Added             ESCAN00026453 Vector(UDS)                    RoE event counter
                    Sa     Fixed             ESCAN00026335 All                            Compile error with DynamicTP
2008-05-23 05.06.07
                    Sa     Fixed             ESCAN00027056 Vector(UDS)                    Virtual request can overwrite ongoing RoE spontaneous response
                    Ade    Added             ESCAN00027064 Vector(UDS)                    ReadMemoryByAddress UACC ring-buffer/linear-buffer support
                    Sa     Added             ESCAN00027066 Vector(UDS)                    Additional notifications ApplDescOnBeginOfProc ApplDescOnEndOfProc
                    Sa     Added             ESCAN00027067 Vector(UDS)                    Added shared resource management in DanisDispatcher
                    Sa     Fixed             ESCAN00027058 UDS                            Compile error on generated main-handler with ring-buffer access an single PID mode
                    Sa     Fixed             ESCAN00026907 All                            On TMS320 wrong vstdlib memcopy API used
                    Sa     Fixed             ESCAN00027059 All                            Array out of boundary access error on generated main-handler with ring-buffer access
                    Sa     Modified          ESCAN00027061 All                            Minor DescICN driver optimization
                    Sa     Modified          ESCAN00027063 All                            MainHandler req-/resDataLen has misleading description for CANdescBasic
                    Ktw    Modified          ESCAN00027057 All                            Compile error on missing semicolon for DescSetNpmOnCanChannelActive macro expansion
                    Mhe    Added             ESCAN00026452 All                            Posthandler support for generated $23
2008-05-26 05.06.08
                    Sa     Fixed             ESCAN00027060 UDS                            Wrong assertion in case of negative response on service 0x22 and multiple PID mode
                    Sa     Fixed             ESCAN00027085 UDS                            Compile error on configurations without service $2A or $2C
                    Sa     Fixed             ESCAN00027084 All                            Compile error on non TMS320 platforms
2008-06-05 05.06.09
                    -      -                 -             -                              No relevant changes available in this version.
2008-06-16 05.06.10
                    -      -                 -             -                              No relevant changes available in this version.
------------------------------------------------------------------------------------------------------------------------
 */


/* -----------------------------------------------------------------------------
    &&&~ Includes
 ----------------------------------------------------------------------------- */
#include "basic_types.h"
#include "hardware.h"
#include "flash_eee.h"
#include "ts.h"
#include "sec.h"
#include "ixcp.h"
/* Include the interface to the OBD feature */
//#include "obd.h"
//#include "obd_ids.h"
#include "mdm.h"
#include "mdm_bg.h"
#include "mmc_eol.h"
#include "vio.h"
#include "vio_p.h"      /* for temperature scaling function */
#include "vio_types_p.h"
#include "log.h"
#include "log_p.h"
#include "log_types.h"
#include "fm.h"
//#include "cgi.h"
#include "vprg.h"
#include "dio_cfg.h"
#include "rtc.h"
//#include "vmd.h"
//#include "cal_offline.h"
//#include "cm_ref_types.h"   /* for SOH routine states */
//#include "vmi.h"
//#include "umc.h"
#include "ms.h"

/* Physical layer specific API */
#include "v_inc.h"
/* Check if CCL is available and take into account its configuration */
#if defined (VGEN_ENABLE_CCL)
# include "ccl_cfg.h"
#endif

/* Check if CCLcom is available and take into account its configuration */
#if defined (VGEN_ENABLE_CCLCOM)
# include "cclcom_cfg.h"
#endif

/* Make all CANdesc specific typedefs available */
#include "desc.h"
/* Include the implementation prototypes for prototype checks */
#include "appdesc.h"
/* Gives access to NM status */
#include "n_onmdef.h"


/* -----------------------------------------------------------------------------
    &&&~ Macros
 ----------------------------------------------------------------------------- */

/* BMS routine status record */
#define kRoutineStatusRecord_Success    0x00u
#define kRoutineStatusRecord_Failure    0x01u

/* BMS Routine completed flag */
#define kRoutineCompletedFlag_NotCompleted  0x00u
#define kRoutineCompletedFlag_Completed     0x01u

/* -----------------------------------------------------------------------------
    &&&~ Preprocessor constants (defines)
 ----------------------------------------------------------------------------- */

#define kDescEcuCryptoKey                                            0x12345678



/* -----------------------------------------------------------------------------
    &&&~ Function prototypes
 ----------------------------------------------------------------------------- */

static void appdesc_copy_data(uint8_t* dest, uint8_t* src, uint16_t len);
static vuint32 SecM_ComputeKey(void);
static void SecM_Serializer(DescMsg tgtPtr, vuint32 data);



/* -----------------------------------------------------------------------------
    &&&~ RAM definitions
 ----------------------------------------------------------------------------- */

/* 64Bit seed stored for the later key compare */
static vuint32 g_applDescSeed;

/**
 * Convenient structure to contain the communications status for
 * the Diagnostic State identifier
 */
static struct {
    bool_t normal_messages_enabled;
    bool_t nwm_messages_enabled;
} communications_status = { TRUE, TRUE } ;

/* -----------------------------------------------------------------------------
    &&&~ Implementation
 ----------------------------------------------------------------------------- */

#if defined (DESC_ENABLE_DEBUG_USER ) || defined (DESC_ENABLE_DEBUG_INTERNAL)
# if defined (CCL_ENABLE_ERROR_HOOK) || defined (CCLCOM_ENABLE_ERROR_HOOK)
/* CCL takes care about this function */
# else
/* ********************************************************************************
 * Function name:ApplDescFatalError
 * Description: This function will be called each time while the debug mode is active a
 * CANdesc fault has been detected. If you reach this function it makes no sence to continue the tests since CANdesc
 * will not operate properly until next start of the ECU.
 * Returns:  nothing
 * Parameter(s):
 *   - errorCode:
 *       - The assert code text equivalent can be found in desc.h (kDescAssert....).
 *       - Access type: read
 *   - lineNumber:
 *       - Since the same fault could be cales on many places the line number shows where exactly it occured.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - Set a break point at this place to know during the ECU development if you ran onto it.
 ******************************************************************************** */
void DESC_API_CALLBACK_TYPE ApplDescFatalError(vuint8 errorCode, vuint16 lineNumber)
{
  /*<<TBD>> Remove this comment once you have completely implemented this function!!!*/
  /* Avoid warnings */
  DESC_IGNORE_UNREF_PARAM(errorCode);
  DESC_IGNORE_UNREF_PARAM(lineNumber);

  /* When fatal error occurs, cause an ECU hang up at this point.
   * Please set break point at this line to investigate both parameter values. */
  for(;;);
}
# endif
#endif

/*  ********************************************************************************
 * Function name:ApplDescOnTransitionSession
 * Description:Notification function for state change of the given state group, defined by CANdelaStudio.
 * Returns:  nothing
 * Parameter(s):
 *   - newState:
 *       - The state which will be set.
 *       - Access type: read
 *   - formerState:
 *       - The current state of this state group.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" may not be called.
 *   - The function "DescSetNegResponse" may not be called.
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescOnTransitionSession(DescStateGroup newState, DescStateGroup formerState)
{
  /* DTC group corresponding to DTCs */
 //DescMsgItem request[3] = { 0xffu, 0xffu, 0xffu };
 //
 ///* Location for negative response code */
 //DescMsgItem neg_resp;
 //
 ///* Avoids warnings */
 //DESC_IGNORE_UNREF_PARAM(newState);
 //DESC_IGNORE_UNREF_PARAM(formerState);
 //
 ///*
 // * On any session transition, re-enable DTC setting for all DTCs.
 // * This callback is made on commanded transitions and also on timeout.
 // * The return value isn't checked as the request is hard-coded here and,
 // * in any event, there's not much can be done if it fails.
 // * As the return value isn't checked, neg_resp isn't used.
 // */
 //obd_control_dtc_setting(&request[0], TRUE, &neg_resp);
}


/*  ********************************************************************************
 * Function name:ApplDescOnTransitionSecurityAccess
 * Description:Notification function for state change of the given state group, defined by CANdelaStudio.
 * Returns:  nothing
 * Parameter(s):
 *   - newState:
 *       - The state which will be set.
 *       - Access type: read
 *   - formerState:
 *       - The current state of this state group.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" may not be called.
 *   - The function "DescSetNegResponse" may not be called.
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescOnTransitionSecurityAccess(DescStateGroup newState, DescStateGroup formerState)
{
  /* @todo: diagnostics implementation. Don't think we need to do anything on security level changes... */
  /* Avoids warnings */
  DESC_IGNORE_UNREF_PARAM(newState);
  DESC_IGNORE_UNREF_PARAM(formerState);
;
}


/*  ********************************************************************************
 * Function name:ApplDescRead_Diagnostic_DataEmissions (Service request header:$1 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescRead_Diagnostic_DataEmissions(DescMsgContext* pMsgContext)
{
 // /* request cannot contain more than 6 PIDs */
 // if((pMsgContext->reqDataLen > (DescMsgLen)6u)
 // || (pMsgContext->reqDataLen == (DescMsgLen)0u))
 // {
 //   /* Request length invalid - send negative response! */
 //   DescSetNegResponse(kDescNrcInvalidFormat);
 // }
 // else if(!obd_report_emission_diag_data(pMsgContext->reqData,
 //                                        pMsgContext->reqDataLen,
 //                                        &pMsgContext->resDataLen,
 //                                        pMsgContext->resData))
 // {
 //   /* Invalid request data */
 //   DescSetNegResponse(kDescNrcInvalidFormat);
 // }
 // else
 // {
 //   /* If the response length is 0 then none of the requested PIDs are supported.
 //    * In this case we shoudl suppress the positive response. */
 //   if (pMsgContext->resDataLen == 0u)
 //   {
 //       pMsgContext->msgAddInfo.resOnReq = 0u;
 //   }
 // }
 //
 // /* User service processing finished. */
 // DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescRead_Freeze_Frame_DataEmissions (Service request header:$2 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescRead_Freeze_Frame_DataEmissions(DescMsgContext* pMsgContext)
{
//  /* request cannot contain more than 3 pairs of PID + frame number */
//  if((pMsgContext->reqDataLen == (DescMsgLen)0u)
//  || (pMsgContext->reqDataLen > (DescMsgLen)6u)
//  || ((pMsgContext->reqDataLen & 0x01u) != 0u))
//  {
//    /* Request length invalid - send negative response! */
//    DescSetNegResponse(kDescNrcInvalidFormat);
//  }
//  else if(!obd_report_emission_ff(pMsgContext->reqData,
//                                  pMsgContext->reqDataLen,
//                                  &pMsgContext->resDataLen,
//                                  pMsgContext->resData))
//  {
//    /* Invalid request data */
//    DescSetNegResponse(kDescNrcInvalidFormat);
//  }
//  else
//  {
//    /* If the response length is 0 then none of the requested PIDs are supported.
//     * In this case we shoudl suppress the positive response. */
//    if (pMsgContext->resDataLen == 0u)
//    {
//        pMsgContext->msgAddInfo.resOnReq = 0u;
//    }
//  }
//
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescRead_Emissions_DTCsEmissions (Service request header:$3 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescRead_Emissions_DTCsEmissions(DescMsgContext* pMsgContext)
{
 // obd_report_emission_dtcs_confirmed(&pMsgContext->resDataLen,
 //                                    pMsgContext->resData);
 //
 // /* User service processing finished. */
 // DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescClear_Emissions_DTCsEmissions (Service request header:$4 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescClear_Emissions_DTCsEmissions(DescMsgContext* pMsgContext)
{
 // obd_clear_emission_dtcs();
 // /* User service processing finished. */
 // DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescRead_Current_Emissions_DTCsEmissions (Service request header:$7 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescRead_Current_Emissions_DTCsEmissions(DescMsgContext* pMsgContext)
{
 // obd_report_emission_dtcs_pending(&pMsgContext->resDataLen,
 //                                  pMsgContext->resData);
 // /* User service processing finished. */
 // DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescRead_Vehicle_InfoEmissions (Service request header:$9 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescRead_Vehicle_InfoEmissions(DescMsgContext* pMsgContext)
{
 // /* request cannot contain more than 6 support PIDs or 1 info PID */
 // if((pMsgContext->reqDataLen == (DescMsgLen)0u)
 // || (pMsgContext->reqDataLen > (DescMsgLen)6u))
 // {
 //   /* Request length invalid - send negative response! */
 //   DescSetNegResponse(kDescNrcInvalidFormat);
 // }
 // else if(!obd_report_emission_veh_info(pMsgContext->reqData,
 //                                       pMsgContext->reqDataLen,
 //                                       &pMsgContext->resDataLen,
 //                                       pMsgContext->resData))
 // {
 //   /* Invalid request data */
 //   DescSetNegResponse(kDescNrcInvalidFormat);
 // }
 // else
 // {
 //   /* If the response length is 0 then none of the requested PIDs are supported.
 //    * In this case we should suppress the positive response. */
 //   if (pMsgContext->resDataLen == 0u)
 //   {
 //       pMsgContext->msgAddInfo.resOnReq = 0u;
 //   }
 // }
 //
 // /* User service processing finished. */
 // DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescRead_Permanent_Emission_DTCsEmissions (Service request header:$A )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescRead_Permanent_Emission_DTCsEmissions(DescMsgContext* pMsgContext)
{
 // obd_report_emission_dtcs_permanent(&pMsgContext->resDataLen,
 //                                    pMsgContext->resData);
 // /* User service processing finished. */
 // DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescResetHard_Reset (Service request header:$11 $1 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescResetHard_Reset(DescMsgContext* pMsgContext)
{
    if (ResetReqAccepted!=ms_request_reset(Reset_controlled))
    {
        DescSetNegResponse(kDescNrcConditionsNotCorrect);
    }
    DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescResetSoft_Reset (Service request header:$11 $3 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescResetSoft_Reset(DescMsgContext* pMsgContext)
{
    if (ResetReqAccepted!=ms_request_reset(Reset_controlled))
    {
        /* MS says the reset should not be accepted at this time */
        DescSetNegResponse(kDescNrcConditionsNotCorrect);
    }
    DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescClearFaultMemory (Service request header:$14 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescClearFaultMemory(DescMsgContext* pMsgContext)
{
 // if(pMsgContext->reqDataLen != 3u)
 // {
 //   /* expecting a 3-byte request */
 //   DescSetNegResponse(kDescNrcInvalidFormat);
 // }
 // else if(!obd_clear_diagnostic_information(pMsgContext->reqData))
 // {
 //   DescSetNegResponse(kDescNrcRequestOutOfRange);
 // }
 // else
 // {
 //   /* Nothing further to do */
 // }
 // /* User service processing finished. */
 // DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadNumberFaultMemory (Service request header:$19 $1 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadNumberFaultMemory(DescMsgContext* pMsgContext)
{
//  if(pMsgContext->reqDataLen != 1u)
//  {
//    /* expecting a 1-byte request */
//    DescSetNegResponse(kDescNrcInvalidFormat);
//  }
//  else {
//    obd_report_number_of_dtc_by_status_mask(pMsgContext->reqData,
//                                            &pMsgContext->resDataLen,
//                                            pMsgContext->resData);
//  }
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadAllIdentifiedFaultMemory (Service request header:$19 $2 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadAllIdentifiedFaultMemory(DescMsgContext* pMsgContext)
{
 // if(pMsgContext->reqDataLen != 1u)
 // {
 //   /* expecting a 1-byte request */
 //   DescSetNegResponse(kDescNrcInvalidFormat);
 // }
 // else
 // {
 //   obd_report_dtc_by_status_mask(pMsgContext->reqData,
 //                                 &pMsgContext->resDataLen,
 //                                 pMsgContext->resData);
 // }
 // /* User service processing finished. */
 // DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadEnvironmentDataFaultMemory (Service request header:$19 $4 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadEnvironmentDataFaultMemory(DescMsgContext* pMsgContext)
{
//  if(pMsgContext->reqDataLen != 4u)
//  {
//    /* expecting a 4-byte request */
//    DescSetNegResponse(kDescNrcInvalidFormat);
//  }
//  //else if (!obd_report_dtc_snapshot_data(pMsgContext->reqData,
//  //                                       &pMsgContext->resDataLen,
//  //                                       pMsgContext->resData))
//  //{
//  //  DescSetNegResponse(kDescNrcRequestOutOfRange);
//  //}
//  else
//  {
//    /* Nothing further to do */
//  }
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescRead_extendedFaultMemory (Service request header:$19 $6 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescRead_extendedFaultMemory(DescMsgContext* pMsgContext)
{
 // if(pMsgContext->reqDataLen != 4u)
 // {
 //   /* expecting a 4-byte request */
 //   DescSetNegResponse(kDescNrcInvalidFormat);
 // }
 // else if (!obd_report_dtc_extended_data(pMsgContext->reqData,
 //                                        &pMsgContext->resDataLen,
 //                                        pMsgContext->resData))
 // {
 //   DescSetNegResponse(kDescNrcRequestOutOfRange);
 // }
 // else
 // {
 //   /* Nothing further to do */
 // }
 // DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadAllSupportedFaultMemory (Service request header:$19 $A )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadAllSupportedFaultMemory(DescMsgContext* pMsgContext)
{
//  /*
//   * The auto-generated skeleton file doesn't envisage the possibility that
//   * this callback might want to return a negative response on the grounds
//   * that there is no request data associated with it. Checking that there
//   * really is no request data might cause this callback to report failure
//   * when presented with a diagnostic tool that lazily doesn't supply zero
//   * request bytes. In the interests of supporting any such tools, no check
//   * is made here.
//   */
//  pMsgContext->resDataLen = obd_report_supported_dtc(pMsgContext->resData);
//
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescWriteFingerprint (Service request header:$2E $F1 $5A )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescWriteFingerprint(DescMsgContext* pMsgContext)
{
  /* This fingerprint is only written in reprogramming mode, return NRC */
  DescSetNegResponse(kDescNrcSubfunctionNotSupportedInActiveSession);
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescStartLearn_VIN (Service request header:$31 $1 $2 $0 )
 * Description:Learns the VIN transmitted in the Car Configuration Message on the CAN Bus
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescStartLearn_VIN(DescMsgContext* pMsgContext)
{
  uint8_t i;
  pMsgContext->resDataLen = 1;

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescStartCheck_Checksum (Service request header:$31 $1 $2 $2 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescStartCheck_Checksum(DescMsgContext* pMsgContext)
{
  /* this routine is only available in reprogramming mode */
  DescSetNegResponse(kDescNrcSubfunctionNotSupportedInActiveSession);
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescStartCheck_Programming_Preconditions (Service request header:$31 $1 $2 $3 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescStartCheck_Programming_Preconditions(DescMsgContext* pMsgContext)
{
    pMsgContext->resDataLen = 1;
    pMsgContext->resData[0] = (ixcp_prog_allowed(Reprogram_iso))
                                ? kTpSuccess : kTpFailed;
    /* User service processing finished. */
    DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescStartDecrease_Failed_Security_Access_Counter (Service request header:$31 $1 $2 $4 )
 * Description: Decrease number of failed attempts to unlock security level 2. This may be done
 * just once before a successful attempt to unlock security has been made, which is ensured
 * by asserting security engineering flag stored in non-volatile memory.
 * This request can't be made before maximal number of invalid attempts to unlock has been made.
 *
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext  -   not used
 * Negative responses:
 *   - kDescNrcExceedNumOfAttempts - request to decrease number of invalid attempts
 *                                      has already been made
 *   - kDescNrcConditionsNotCorrect - maximal number of allowed invalid attempts hasn't
 *                                      been reached yet
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescStartDecrease_Failed_Security_Access_Counter(DescMsgContext* pMsgContext)
{
  if(SEC_LVL2_PROT_GET_ENG_FLAG() == SEC_LVL2_PROT_ACCESS_ENG_ACTIVE)
  {
      /* request has already been made */
      DescSetNegResponse(kDescNrcExceedNumOfAttempts);
  }
  else if(SEC_LVL2_PROT_GET_INVALID_ATTEMPTS() < SEC_LVL2_PROT_MAX_INVALID_ATTEMPTS)
  {
      /* maximal allowed number of attempts hasn't been reached */
      DescSetNegResponse(kDescNrcConditionsNotCorrect);
  }
  else
  {
      /* Assert security access engineering flag*/
      SEC_LVL2_PROT_ASSERT_ENG_FLAG();
      /* Reduce invalid security access attempts by kEngAttempts */
      SEC_LVL2_PROT_SET_INVALID_ATTEMPTS(SEC_LVL2_PROT_MAX_INVALID_ATTEMPTS
          - SEC_LVL2_PROT_ENG_ADDITIONAL_ATTEMPTS);
  }
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescStartSOC_Learning (Service request header:$31 $1 $F0 $4 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescStartSOC_Learning(DescMsgContext* pMsgContext)
{
  //mdm_soc_learn_start();
  pMsgContext->resData[0] = kRoutineStatusRecord_Success;
  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescStartVTBM_Reprogramming (Service request header:$31 $1 $F0 $7 )
 * Description: Request VTBM reprogramming
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData: not used
 *   - pMsgContext->resData[0]:
 *       - kRoutineStatusRecord_Success if request has been successfully placed
 *       - kRoutineStatusRecord_Failure if request has been rejected
 *   - pMsgContext->reqDataLen: always written with 1
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescStartVTBM_Reprogramming(DescMsgContext* pMsgContext)
{
    /** place vtbm reprogramming request */
    if(vprg_uds_vtbm_reprogramming_req())
    {
        pMsgContext->resData[0] = kRoutineStatusRecord_Success;
    }
    else
    {
        pMsgContext->resData[0] = kRoutineStatusRecord_Failure;
    }
	
    pMsgContext->resDataLen = 1;
    /* User service processing finished. */
    DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescStartReset_Contactor_Wear (Service request header:$31 $1 $F0 $8 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescStartReset_Contactor_Wear(DescMsgContext* pMsgContext)
{
    /* clear contactor wear data */
//    LOG_INTEGRATOR(Log_integ_ctc_wear_prechg)  = 0UL;
//    LOG_INTEGRATOR(Log_integ_ctc_wear_veh_pos) = 0UL;
//    LOG_INTEGRATOR(Log_integ_ctc_wear_veh_neg) = 0UL;
//    LOG_INTEGRATOR(Log_integ_ctc_wear_chg_pos) = 0UL;
//    LOG_INTEGRATOR(Log_integ_ctc_wear_chg_neg) = 0UL;

    /* run the fault filters to clear any faults */
    (void) fm_update_fault_filter(Fm_pack_veh_pre_contactor_worn,
                                  Fm_general_failure_information, FALSE, TRUE);
    (void) fm_update_fault_filter(Fm_pack_veh_pos_contactor_worn,
                                  Fm_general_failure_information, FALSE, TRUE);
    /* common negative contactor related faults */
    (void) fm_update_fault_filter(Fm_pack_veh_neg_contactor_worn,
                                  Fm_general_failure_information, FALSE, TRUE);
    /* charger contactor related faults */
    (void) fm_update_fault_filter(Fm_pack_chg_pos_contactor_worn,
                                  Fm_general_failure_information, FALSE, TRUE);
    (void) fm_update_fault_filter(Fm_pack_chg_neg_contactor_worn,
                                  Fm_general_failure_information, FALSE, TRUE);

    pMsgContext->resData[0] = kRoutineStatusRecord_Success;
    pMsgContext->resDataLen = 1;

    /* User service processing finished. */
    DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescStartSOH_Learning_Phase_1 (Service request header:$31 $1 $F0 $9 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescStartSOH_Learning_Phase_1(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = mdm_bg_output_p->cm.sohStatusResp;
  //pMsgContext->resData[1] = mdm_bg_output_p->cm.sohStartResp[MdmSOHLearnPhase1];
  
  pMsgContext->resData[0] = 0;    //
  pMsgContext->resData[1] = 0;    //
  
  pMsgContext->resDataLen = 2;
 // mdm_soh_learn_start(MdmSOHLearnPhase1);

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescStartSOH_Learning_Phase_2 (Service request header:$31 $1 $F0 $A )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescStartSOH_Learning_Phase_2(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = mdm_bg_output_p->cm.sohStatusResp;
  //pMsgContext->resData[1] = mdm_bg_output_p->cm.sohStartResp[MdmSOHLearnPhase2];
  
  pMsgContext->resData[0] = 0;    //
  pMsgContext->resData[1] = 0;    //
  
  pMsgContext->resDataLen = 2;
 // mdm_soh_learn_start(MdmSOHLearnPhase2);

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescStartSOH_Learning_Phase_3 (Service request header:$31 $1 $F0 $B )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescStartSOH_Learning_Phase_3(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = mdm_bg_output_p->cm.sohStatusResp;
  //pMsgContext->resData[1] = mdm_bg_output_p->cm.sohStartResp[MdmSOHLearnPhase3];
  
  pMsgContext->resData[0] = 0;    //
  pMsgContext->resData[1] = 0;    //
  
  pMsgContext->resDataLen = 2;
 // mdm_soh_learn_start(MdmSOHLearnPhase3);

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescStartErase_Memory (Service request header:$31 $1 $FF $0 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescStartErase_Memory(DescMsgContext* pMsgContext)
{
  /* this routine is only available in reprogramming mode */
  DescSetNegResponse(kDescNrcSubfunctionNotSupportedInActiveSession);
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescStartCheck_Programming_Dependencies (Service request header:$31 $1 $FF $1 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescStartCheck_Programming_Dependencies(DescMsgContext* pMsgContext)
{
  /* this routine is only available in reprogramming mode */
  DescSetNegResponse(kDescNrcSubfunctionNotSupportedInActiveSession);
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescStopSOH_Learning_Phase_1 (Service request header:$31 $2 $F0 $9 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescStopSOH_Learning_Phase_1(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = mdm_bg_output_p->cm.sohStatusResp;
  //pMsgContext->resData[1] = mdm_bg_output_p->cm.sohStopResp[MdmSOHLearnPhase1];
  
  pMsgContext->resData[0] = 0;    //
  pMsgContext->resData[1] = 0;    //
  
  pMsgContext->resDataLen = 2;
 // mdm_soh_learn_stop(MdmSOHLearnPhase1);

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescStopSOH_Learning_Phase_2 (Service request header:$31 $2 $F0 $A )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescStopSOH_Learning_Phase_2(DescMsgContext* pMsgContext)
{
 // pMsgContext->resData[0] = mdm_bg_output_p->cm.sohStatusResp;
 // pMsgContext->resData[1] = mdm_bg_output_p->cm.sohStopResp[MdmSOHLearnPhase2];
 
 pMsgContext->resData[0] = 0;    //
 pMsgContext->resData[1] = 0;    //
 
  pMsgContext->resDataLen = 2;
 // mdm_soh_learn_stop(MdmSOHLearnPhase2);

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescStopSOH_Learning_Phase_3 (Service request header:$31 $2 $F0 $B )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescStopSOH_Learning_Phase_3(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = mdm_bg_output_p->cm.sohStatusResp;
 // pMsgContext->resData[1] = mdm_bg_output_p->cm.sohStopResp[MdmSOHLearnPhase3];
 
 pMsgContext->resData[0] = 0;    //
 pMsgContext->resData[1] = 0;    //
 
  pMsgContext->resDataLen = 2;
 // mdm_soh_learn_stop(MdmSOHLearnPhase3);

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescResultsVTBM_Reprogramming (Service request header:$31 $3 $F0 $7 )
 * Description: not available
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData: not used
 *   - pMsgContext->resData[0]: BMS Routine Completed Flag
 *   - pMsgContext->resData[1-2]: VTBM reprogramming mask
 *   - pMsgContext->resData[3]: VTBM image index number
 *   - pMsgContext->resData[4]: VTBM reprogramming latest error
 *   - pMsgContext->resData[5]: VTBM reprogramming image error
 *   - pMsgContext->resData[6]: VTBM reprogramming comms error
 *   - pMsgContext->resDataLen: 7
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescResultsVTBM_Reprogramming(DescMsgContext* pMsgContext)
{
    /* Collect data from vprg feature */
    bool_t finished;
    uint16_t mask;
    vprg_error_e latest_error;
    vprg_ci_state_e image_status;
    vprg_comms_error_e comms_error;
    uint16_t image;
   vprg_uds_get_status(&finished, &mask, &image, &latest_error, &image_status, &comms_error);
   pMsgContext->resData[0] = (finished == TRUE) ? kRoutineCompletedFlag_Completed :
       kRoutineCompletedFlag_NotCompleted;
   pMsgContext->resData[1] = (uint8_t) (mask >> 8);
   pMsgContext->resData[2] = (uint8_t) (mask & 0xff);
   pMsgContext->resData[3] = (uint8_t) image;
   pMsgContext->resData[4] = (uint8_t) latest_error;
   pMsgContext->resData[5] = (uint8_t) image_status;
   pMsgContext->resData[6] = (uint8_t) comms_error;
   
    /*pMsgContext->resDataLen:  Always set the correct length of the response data. */
    pMsgContext->resDataLen = 7;
    /* User service processing finished. */
    DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescResultsSOH_Learning_Phase_1 (Service request header:$31 $3 $F0 $9 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescResultsSOH_Learning_Phase_1(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = mdm_bg_output_p->cm.sohStatusResp;
  //pMsgContext->resData[1] = mdm_bg_output_p->cm.sohResultResp[MdmSOHLearnPhase1];
  
  pMsgContext->resData[0] = 0;    //
  pMsgContext->resData[1] = 0;    //
  
  pMsgContext->resDataLen = 2;
  
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescResultsSOH_Learning_Phase_2 (Service request header:$31 $3 $F0 $A )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescResultsSOH_Learning_Phase_2(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = mdm_bg_output_p->cm.sohStatusResp;
 // pMsgContext->resData[1] = mdm_bg_output_p->cm.sohResultResp[MdmSOHLearnPhase2];
 
 pMsgContext->resData[0] = 0;    //
 pMsgContext->resData[1] = 0;    //
 
  pMsgContext->resDataLen = 2;
  
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescResultsSOH_Learning_Phase_3 (Service request header:$31 $3 $F0 $B )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescResultsSOH_Learning_Phase_3(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = mdm_bg_output_p->cm.sohStatusResp;
  //pMsgContext->resData[1] = mdm_bg_output_p->cm.sohResultResp[MdmSOHLearnPhase3];
  
  pMsgContext->resData[0] = 0;    //
  pMsgContext->resData[1] = 0;    //
  
  pMsgContext->resDataLen = 2;
  
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescRequestDownload (Service request header:$34 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescRequestDownload(DescMsgContext* pMsgContext)
{
  /* this service is only available in reprogramming mode */
  DescSetNegResponse(kDescNrcSubfunctionNotSupportedInActiveSession);
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescTransmitDownload (Service request header:$36 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescTransmitDownload(DescMsgContext* pMsgContext)
{
  /* this service is only available in reprogramming mode */
  DescSetNegResponse(kDescNrcSubfunctionNotSupportedInActiveSession);
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescStopDownload (Service request header:$37 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescStopDownload(DescMsgContext* pMsgContext)
{
  /* this service is only available in reprogramming mode */
  DescSetNegResponse(kDescNrcSubfunctionNotSupportedInActiveSession);
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescSendTurn_DTC_Setting_On (Service request header:$85 $1 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescSendTurn_DTC_Setting_On(DescMsgContext* pMsgContext)
{
//  DescMsgItem neg_resp;
//
//  if(pMsgContext->reqDataLen != 3u)
//  {
//    /* expecting a 3-byte request */
//    DescSetNegResponse(kDescNrcInvalidFormat);
//  }
//  else if (!obd_control_dtc_setting(pMsgContext->reqData, TRUE, &neg_resp))
//  {
//    DescSetNegResponse(neg_resp);
//  }
//  else
//  {
//    /* Nothing further to do */
//  }
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescSendTurn_DTC_Setting_Off (Service request header:$85 $2 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescSendTurn_DTC_Setting_Off(DescMsgContext* pMsgContext)
{
 // DescMsgItem neg_resp;
 //
 // if(pMsgContext->reqDataLen != 3u)
 // {
 //   /* expecting a 3-byte request */
 //   DescSetNegResponse(kDescNrcInvalidFormat);
 // }
 // else if (!obd_control_dtc_setting(pMsgContext->reqData, FALSE, &neg_resp))
 // {
 //   DescSetNegResponse(neg_resp);
 // }
 // else
 // {
 //   /* Nothing further to do */
 // }
 // /* User service processing finished. */
 // DescProcessingDone();
}


/* ********************************************************************************
 * Function name:ApplDescOnCommunicationDisable
 * Description: Notification function that the communication has been disabled.
 * Returns:  none
 * Parameter(s):none
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" may not be called.
 *   - The function "DescSetNegResponse" may not be called.
 ******************************************************************************** */
void DESC_API_CALLBACK_TYPE ApplDescOnCommunicationDisable(void)
{
  /* @todo: don't think we need to do anything on comms state change */
  /* The requested group of messages will no more be sent onto the communication bus. */
}

/* ********************************************************************************
 * Function name:ApplDescOnCommunicationEnable
 * Description: Notification function that the communication has been restored.
 * Returns:  none
 * Parameter(s):none
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" may not be called.
 *   - The function "DescSetNegResponse" may not be called.
 ******************************************************************************** */
void DESC_API_CALLBACK_TYPE ApplDescOnCommunicationEnable(void)
{
  /* @todo: don't think we need to do anything on comms state change */
  /* The requested group of messages will be able to be sent onto the communication bus. */
}

/* Check the communication control activity */
/* ********************************************************************************
 * Function name:ApplDescCheckCommCtrl
 * Description:Check if the requested communication manipulation is possible to be performed by the ECU.
 * Returns:  nothing
 * Parameter(s):
 *   - iContext:
 *       - Diagnostic request handle used only in multi-context system (kDescNumContexts > 1).
 *       - Access type: read
 *   - commControlInfo->subNetTxNumber:
 *       - The application shall use this parameter to decied on which physical channels the communiaction will be manipulated.
 *       - Access type: read
 *   - commControlInfo->commCtrlChannel:
 *       - The application determines on which channel the communication will be manipulated (kDescCommControlCanCh01 - kDescCommControlCanCh14).
 *       - Access type: write
 *   - commControlInfo->rxPathState:
 *       - Activity type: kDescCommControlStateEnable - enables the communication; kDescCommControlStateDisable - disables it.
 *       - Access type: read
 *   - commControlInfo->txPathState:
 *       - Activity type: kDescCommControlStateEnable - enables the communication; kDescCommControlStateDisable - disables it.
 *       - Access type: read
 *   - commControlInfo->msgTypes:
 *       - Message group: kDescCommControlMsgAppl - application; kDescCommControlMsgNm - NM; 
 *       - Access type: read
 *   - commControlInfo->reqCommChannel:
 *       - The current CAN channel on which the CommControl request is received.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - Call "DescCommCtrlChecked" (here or later) to confirm the check.
 *   - Call "DescSetNegResponse" to reject the service.
 ******************************************************************************** */
void DESC_API_CALLBACK_TYPE ApplDescCheckCommCtrl(DESC_CONTEXT_FORMAL_PARAM_DEF_FIRST DescOemCommControlInfo *commControlInfo)
{
#if defined (DESC_ENABLE_COMM_CTRL_SUBNET_SUPPORT)
  switch(commControlInfo->subNetNumber)
  {
    /* kDescCommControlSubNetNum01 is vehicle bus, all other buses are prohibited to manipulate */
    case kDescCommControlSubNetNum01:
    # if !defined (DESC_ENABLE_MULTI_CHANNEL_SUPPORT)
          /* No sub-networks => no subnetwork number can be requested */
          errorCode = kDescNrcRequestOutOfRange;
    # endif
    /* since only vehicle bus is allowed to manipulate, change kDescCommControlSubNetNumAll and
     * kDescCommControlSubNetNumRx requests to kDescCommControlSubNetNum01 only */
    case kDescCommControlSubNetNumAll:/* Enable/Disable all comm channels */
    case kDescCommControlSubNetNumRx:/* Enable/Disable only the request RX comm channel (CAN)*/
      /* hack subNetNumber to always manipulate just kDescCommControlSubNetNum01 */
      commControlInfo->subNetNumber = kDescCommControlSubNetNum01;
      /* Assume 1:1 mapping subNetNumber:CAN (on multi channel system) */
      commControlInfo->commCtrlChannel = commControlInfo->subNetNumber;
      /* Convert subnetNum1 -> CAN0, etc.*/
      commControlInfo->commCtrlChannel--;
      /*
       * we need no special treatment: CANbedded will do the work for us but
       * we need to remember what's happened to report the Diagnostic State
       */
      if (commControlInfo->msgTypes & kDescCommControlMsgAppl)
      {
          communications_status.normal_messages_enabled
              = (commControlInfo->txPathState == kDescCommControlStateEnable)
              ? TRUE : FALSE;
      }
      if (commControlInfo->msgTypes & kDescCommControlMsgNm)
      {
          communications_status.nwm_messages_enabled
              = (commControlInfo->txPathState == kDescCommControlStateEnable)
              ? TRUE : FALSE;
      }
      break;
    default:
      /* unknown subnet */
      DescSetNegResponse(DESC_CONTEXT_PARAM_FIRST kDescNrcRequestOutOfRange);
  }
#endif
  /* Confirm the communication status will be accepted. */
  DescCommCtrlChecked(DESC_CONTEXT_PARAM_ONLY);
}

#if defined (DESC_ENABLE_COMM_CTRL_SUBNET_SUPPORT)
/* ********************************************************************************
 * Function name:ApplDescSetCommMode
 * Description:Manipulate application specific channels (LIN, MOST, etc.)
 * Returns:  nothing
 * Parameter(s):
 *   - commControlInfo->subNetTxNumber:
 *       - The application shall use this parameter to decied on which physical channels the communiaction will be manipulated.
 *       - Access type: read
 *   - commControlInfo->commCtrlChannel:
 *       - The application determines on which channel the communication will be manipulated (kDescCommControlCanCh01 - kDescCommControlCanCh14).
 *       - Access type: write
 *   - commControlInfo->rxPathState:
 *       - Activity type: kDescCommControlStateEnable - enables the communication; kDescCommControlStateDisable - disables it.
 *       - Access type: read
 *   - commControlInfo->txPathState:
 *       - Activity type: kDescCommControlStateEnable - enables the communication; kDescCommControlStateDisable - disables it.
 *       - Access type: read
 *   - commControlInfo->msgTypes:
 *       - Message group: kDescCommControlMsgAppl - application; kDescCommControlMsgNm - NM; 
 *       - Access type: read
 *   - commControlInfo->reqCommChannel:
 *       - The current CAN channel on which the CommControl request is received.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" may not be called.
 *   - The function "DescSetNegResponse" may not be called.
 ******************************************************************************** */
void DESC_API_CALLBACK_TYPE ApplDescSetCommMode(DescOemCommControlInfo *commControlInfo)
{
    /*
     * CANbedded does all the work for us.
     */
}
#endif

#if defined (DESC_ENABLE_RX_COMM_CONTROL)
/* ********************************************************************************
 * Function name:ApplDescSetCommModeOnRxPath
 * Description: Manipulates only the RX path on CAN. For the other networks (if any) such LIN, MOST, etc. reffer to the
 *              ApplDescSetCommMode API.
 * Returns:  nothing
 * Parameter(s):
 *   - commControlInfo->subNetTxNumber:
 *       - The application shall use this parameter to decied on which physical channels the communiaction will be manipulated.
 *       - Access type: read
 *   - commControlInfo->commCtrlChannel:
 *       - The application determines on which channel the communication will be manipulated (kDescCommControlCanCh01 - kDescCommControlCanCh14).
 *       - Access type: write
 *   - commControlInfo->rxPathState:
 *       - Activity type: kDescCommControlStateEnable - enables the communication; kDescCommControlStateDisable - disables it.
 *       - Access type: read
*   - commControlInfo->rxPathState: - irrelevant for this API since it processes only the RX path!!!
 *       - Activity type: kDescCommControlStateEnable - enables the communication; kDescCommControlStateDisable - disables it.
 *       - Access type: read
 *   - commControlInfo->msgTypes:
 *       - Message group: kDescCommControlMsgAppl - application; kDescCommControlMsgNm - NM; 
 *       - Access type: read
 *   - commControlInfo->reqCommChannel:
 *       - The current CAN channel on which the CommControl request is received.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" may not be called.
 *   - The function "DescSetNegResponse" may not be called.
 ******************************************************************************** */
void DESC_API_CALLBACK_TYPE ApplDescSetCommModeOnRxPath(DescOemCommControlInfo *commControlInfo)
{
  if((commControlInfo->rxPathState & kDescCommControlStateEnable) != 0)
  {
    /* _DrvCanSetRxOnlineMode(commControlInfo.commCtrlChannel, g_descCommControlInfo.msgTypes); */
  }
  else
  {
    /* _DrvCanSetRxOfflineMode(commControlInfo.commCtrlChannel, g_descCommControlInfo.msgTypes); */
  }
}
#endif

/*  ********************************************************************************
 * Function name:ApplDescCheckSessionTransition
 * Description:Check if the given session transition is allowed or your ECU is currently not
 * able to perform it.
 * Returns:  nothing
 * Parameter(s):
 *   - newState:
 *       - Contains the new state in which the state group will be set.
 *       - Access type: read
 *   - formerState:
 *       - Contains the current state of the state group.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescSessionTransitionChecked" may be called.
 *   - The function "DescSetNegResponse" may be called.
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescCheckSessionTransition(DescStateGroup newState, DescStateGroup formerState)
{
    /*
     * CANbedded enforces the security restrictions on transitions without
     * our help, so the only restriction we need to enforce here is that
     * transitions to a Programming Session are only allowed if
     * programming preconditions are satisfied.
     *
     * If we do want to switch to a programming session, we need to jump into
     * the FBL bootloader from here.
     *
     * Note: if we are running with a Ricardo bootblock instead of the
     * CANbedded FBL, ixcp_prog_allowed() will always return FALSE
     */

    bool_t respond_now=TRUE;
    if (newState == kDescStateSessionProgramming)
    {
        if (ixcp_prog_allowed(Reprogram_iso))
        {
            /* request all features to store its data, so NVS will store it next idle task
             * and performs reset immediately afterwards */
           if (ResetReqAccepted==ms_request_reset(Reset_program_iso))
           {
               /* If the reset request is accepted, the response will come
                * later on */
               respond_now=FALSE;
           }
        }

        /* If we reach here with respond_now set, it means the request to
         * program has been rejected */
        if (respond_now)
        {
            DescSetNegResponse(
                    DESC_CONTEXT_PARAM_FIRST kDescNrcConditionsNotCorrect);
        }
    }
    /* If respond_now is FALSE at this point, it means a request to enter
     * programming mode has been accepted, otherwise we are done with this
     * message so acknowledge it */
    if (respond_now)
    {
        DescSessionTransitionChecked(DESC_CONTEXT_PARAM_ONLY);
    }
}


/******************************************************************************
* Name         :  SecM_ComputeKey
* Called by    :  CheckKey
* Preconditions:  None
* Parameters   :  None
* Description  :
******************************************************************************/
static vuint32 SecM_ComputeKey(void)
{
//  return sec_key_gen_app(g_applDescSeed);
}

/******************************************************************************
* Name         :  SecM_Serializer
* Called by    :  CheckKey
* Preconditions:  None
* Parameters   :  None
* Description  :
******************************************************************************/
static void SecM_Serializer(DescMsg tgtPtr, vuint32 data)
{
  vuint8_least iter;
  iter = 4;
  while(iter != 0)
  {
    iter--;
    tgtPtr[iter] = (DescMsgItem)data;
    data >>= 8;
  }
}

/* ********************************************************************************
 * Function name:ApplDescSecurityAccessGetSeed
 * Description: Each time called must generate a different value (e.g. current free running timer value).
 * Returns:  Random value
 * Parameter(s):
 *   - iContext:(not available in single context systems)
 *       - Use this call-back handle for all API which need it.
 *       - Access type: read
 *   - securityLevel:
 *       - The current security level represented by the generated contsants (e.g kDescStateSecurityAccessLevel1e etc.).
 *       - Access type: read
 *   - seed:
 *       - A pointer where the generated seed must be written by the application.
 *       - Access type: write
 *   - seedLen:
 *       - The length of the written seed.
 *       - Access type: write
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" is NOT allowed to be called.
 *   - The function "DescSetNegResponse" is NOT allowed to be called.
 ******************************************************************************** */
void ApplDescSecurityAccessGetSeed(DescSecurityAccessContext* pDescSecurityAccessContext)
{
//  g_applDescSeed = sec_seed_gen_app((uint16_t) ts_get_ticks());

  SecM_Serializer(&(pDescSecurityAccessContext->dataPtr[0]), g_applDescSeed);

  /* Call this function to confirm the process end now or outside of this call-back */
  DescSecurityAccessSeedReady(DESC_CONTEXT_PARAM_WRAPPER_ONLY(pDescSecurityAccessContext->iContext));
}

/* ********************************************************************************
 * Function name:ApplDescSecurityAccessCheckKey
 * Description: The application must validate the received security key.
 * Returns:  Random value
 * Parameter(s):
 *   - iContext:(not available in single context systems)
 *       - Use this call-back handle for all API which need it.
 *       - Access type: read
 *   - securityLevel:
 *       - The current security level represented by the generated contsants (e.g kDescStateSecurityAccessLevel1e etc.).
 *       - Access type: read
 *   - key:
 *       - A pointer where the received key must be read from by the application.
 *       - Access type: read
 *   - keyLength:
 *       - The length of the received key.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" is NOT allowed be called.
 *   - The function "DescSetNegResponse" is NOT allowed be called. 
 ******************************************************************************** */
void ApplDescSecurityAccessCheckKey(DescSecurityAccessContext* pDescSecurityAccessContext)
{
   /* The size of this array may vary and depends on the security key definition in the CDD file. */
  vuint32 tmpKey;     /* Store Key on the stack, invisible for extern access */

  /* Key is OK */
  pDescSecurityAccessContext->status = kDescSaStatusOk;

  /* Calculate the Key */
  tmpKey = SecM_ComputeKey();

  /* Compare results with Key from tester */
  if (tmpKey != DescMake32Bit(pDescSecurityAccessContext->dataPtr[0],
                              pDescSecurityAccessContext->dataPtr[1],
                              pDescSecurityAccessContext->dataPtr[2],
                              pDescSecurityAccessContext->dataPtr[3])
      )
  {
    /* Key is INVALID */
    pDescSecurityAccessContext->status = kDescSaStatusNok;
  }
  /* Call this function to confirm the process end now or outside of this call-back */
  DescSecurityAccessKeyChecked(DESC_CONTEXT_PARAM_WRAPPER_ONLY(pDescSecurityAccessContext->iContext));
}

/*  ********************************************************************************
 * Function name:ApplDescReadFault_Memory_Status (Service request header:$22 $1 $0 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadFault_Memory_Status(DescMsgContext* pMsgContext)
{
 //pMsgContext->resData[0] = obd_num_active_dtcs;
 //pMsgContext->resData[1] = obd_max_active_dtcs;
 //pMsgContext->resDataLen = 2;
 ///* User service processing finished. */
 //DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadECU_Diagnostic_State (Service request header:$22 $3 $0 )
 * Description: Sends a byte encoded with the current diagnostic state
 *
 * bits 0-1:    Currently active session
 *              (unknown=0,default=01b, programming=10b, extended=11b)
 * bits 2-3:    Security level (locked=00b,level 1=01b,level2=10b,unknown=11b)
 * bits 4-5:    DTC setting (unknown=0b, on=01b, off=10b)
 * bit 6:       Network management transmissions enabled
 * bit 7:       Normal transmissions enabled
 *
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadECU_Diagnostic_State(DescMsgContext* pMsgContext)
{
//    DescStateGroup active_session;
//    DescStateGroup security_access;
//    uint8_t state_byte = 0;
//
//    active_session = DescGetStateSession();
//    security_access = DescGetStateSecurityAccess();
//
//    switch (active_session)
//    {
//        case kDescStateSessionDefault:
//            state_byte |= (uint8_t) 1u;
//            break;
//        case kDescStateSessionProgramming:
//            state_byte |= (uint8_t) 2u;
//            break;
//        case kDescStateSessionExtended:
//            state_byte |= (uint8_t) 3u;
//            break;
//        default:
//            /* unknown session type so leave bottom bits clear */
//            break;
//    }
//
//    switch (security_access)
//    {
//        case kDescStateSecurityAccessLocked:
//            /* leave bits clear */
//            break;
//        case kDescStateSecurityAccessUnlocked_Level_One:
//            state_byte |= (uint8_t) 4u;
//            break;
//        case kDescStateSecurityAccessUnlocked_Level_Two:
//            state_byte |= (uint8_t) 8u;
//            break;
//        default:
//            /* unknown security access */
//            state_byte |= (uint8_t) 0xc;
//            break;
//    }
//
//    state_byte |= obd_get_dtc_setting_status();
//    if (communications_status.nwm_messages_enabled)
//    {
//        state_byte |= 0x40;
//    }
//    if (communications_status.normal_messages_enabled)
//    {
//        state_byte |= 0x80;
//    }
//
//    pMsgContext->resData[0] = state_byte;
//    /* Always set the correct length of the response data. */
//    pMsgContext->resDataLen = 1;
//    /* User service processing finished. */
//    DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadVehicle_Positive_Contactor_Opening_Current_Table (Service request header:$22 $3 $1 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadVehicle_Positive_Contactor_Opening_Current_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_VEH_POS_CTC_OPEN_I
  uint32_t table_val;
  uint8_t  i;
  for (i = 0u; i < 12u; i++)
  {
    //table_val = log_get_table_bin(Log_table_veh_pos_ct_opening_i, i);
	table_val = 1;
    appdesc_copy_data(&(pMsgContext->resData[i*4u]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the cell temperature table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 48u;

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCharger_Positive_Contactor_Opening_Current_Table (Service request header:$22 $3 $2 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCharger_Positive_Contactor_Opening_Current_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_CHG_POS_CTC_OPEN_I
  uint32_t table_val;
  uint8_t i;
  for (i = 0u; i < 9u; i++)
  {
      //table_val = log_get_table_bin(Log_table_chg_pos_ct_opening_i, i);
	  table_val = 1;
      appdesc_copy_data(&(pMsgContext->resData[i*4u]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the cell temperature table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 36u;

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCell_Temperature_Table (Service request header:$22 $3 $3 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCell_Temperature_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_TEMPERATURE
  uint32_t table_val;
  uint8_t i;
  for (i = 0u; i < 10u; i++)
  {
      //table_val = log_get_table_bin(Log_table_cell_temperature, i);
	  table_val = 1;
      appdesc_copy_data(&(pMsgContext->resData[i*4u]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the cell temperature table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 40u;

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCurrent_SOC (Service request header:$22 $3 $6 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCurrent_SOC(DescMsgContext* pMsgContext)
{
  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadState_of_Balance (Service request header:$22 $3 $7 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadState_of_Balance(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = (uint8_t)mdm_bg_output_p->bal.is_balancing_requested;
  
  pMsgContext->resData[0] = 0;    //
  
  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCurrent_State_of_Health (Service request header:$22 $3 $8 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCurrent_State_of_Health(DescMsgContext* pMsgContext)
{
  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCurrent_Lowest_Cell_Temperature (Service request header:$22 $3 $9 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCurrent_Lowest_Cell_Temperature(DescMsgContext* pMsgContext)
{
  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCell_Number_with_Current_Lowest_Cell_Temperature (Service request header:$22 $3 $A )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCell_Number_with_Current_Lowest_Cell_Temperature(DescMsgContext* pMsgContext)
{
 // pMsgContext->resData[0] = mdm_cell_t_min_num;
 
 pMsgContext->resData[0] = 0;    //
 
  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCurrent_Highest_Cell_Temperature (Service request header:$22 $3 $B )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCurrent_Highest_Cell_Temperature(DescMsgContext* pMsgContext)
{
  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCell_Number_with_Current_Highest_Cell_Temperature (Service request header:$22 $3 $C )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCell_Number_with_Current_Highest_Cell_Temperature(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = mdm_cell_t_max_num;
  
  pMsgContext->resData[0] = 0;    //
  
  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCurrent_Lowest_Cell_Voltage (Service request header:$22 $3 $D )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCurrent_Lowest_Cell_Voltage(DescMsgContext* pMsgContext)
{
  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCell_Number_with_Current_Lowest_Cell_Voltage (Service request header:$22 $3 $E )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCell_Number_with_Current_Lowest_Cell_Voltage(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = mdm_cell_v_min_num;
  
  pMsgContext->resData[0] = 0;    //
  
  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCurrent_Highest_Cell_Voltage (Service request header:$22 $3 $F )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCurrent_Highest_Cell_Voltage(DescMsgContext* pMsgContext)
{
  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCell_Number_with_Current_Highest_Cell_Voltage (Service request header:$22 $3 $10 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCell_Number_with_Current_Highest_Cell_Voltage(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = mdm_cell_v_max_num;
  pMsgContext->resData[0] = 0;    //
  
  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadHighest_Cell_Temperature_Ever (Service request header:$22 $3 $11 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadHighest_Cell_Temperature_Ever(DescMsgContext* pMsgContext)
{
  //uint16_t log_temp = vio_scale_cell_temperature(log_ram_image_1.data.extremes.highest_cell_t);
  //appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&log_temp), 2);
  //pMsgContext->resDataLen = 2;
  ///* User service processing finished. */
  //DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCell_Number_with_Highest_Cell_Temperature_Ever (Service request header:$22 $3 $12 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCell_Number_with_Highest_Cell_Temperature_Ever(DescMsgContext* pMsgContext)
{
//  pMsgContext->resData[0] = log_ram_image_1.data.extremes.cell_num_hi_t;
//  pMsgContext->resDataLen = 1;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadLowest_Cell_Temperature_Ever (Service request header:$22 $3 $13 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadLowest_Cell_Temperature_Ever(DescMsgContext* pMsgContext)
{
  //uint16_t log_temp = vio_scale_cell_temperature(log_ram_image_1.data.extremes.lowest_cell_t);
  //appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&log_temp), 2);
  //pMsgContext->resDataLen = 2;
  ///* User service processing finished. */
  //DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCell_Number_with_Lowest_Cell_Temperature_Ever (Service request header:$22 $3 $14 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCell_Number_with_Lowest_Cell_Temperature_Ever(DescMsgContext* pMsgContext)
{
//  pMsgContext->resData[0] = log_ram_image_1.data.extremes.cell_num_lo_t;
//  pMsgContext->resDataLen = 1;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadOperating_Mode_Table (Service request header:$22 $3 $15 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadOperating_Mode_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_OP_MODE
  uint32_t table_val;
  uint8_t i;
  for (i = 0u; i < 7u; i++)
  {
      //table_val = log_get_table_bin(Log_table_operating_mode, i);
	  table_val = 1;
      appdesc_copy_data(&(pMsgContext->resData[i*4u]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the operating mode table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 16u;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadRegen_Energy_Wh_Table (Service request header:$22 $3 $16 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadRegen_Energy_Wh_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_REGEN
  uint32_t table_val;
  uint8_t i;
  for (i = 0u; i < 8u; i++)
  {   
      //table_val = log_get_table_bin(Log_table_regen_energy, i);
	  table_val = 1;
      appdesc_copy_data(&(pMsgContext->resData[i*4]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the cell temperature table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 32u;

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCharging_Energy_Wh_Table (Service request header:$22 $3 $17 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCharging_Energy_Wh_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_CHG_ENERGY
  uint32_t table_val;
  uint8_t i;

  for (i = 0u; i < 6u; i++)
  {
      //table_val = log_get_table_bin(Log_table_charger_energy, i);
	  table_val = 1;
      appdesc_copy_data(&(pMsgContext->resData[i*4]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the cell temperature table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 24u;

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadVehicle_Energy_Wh_Table (Service request header:$22 $3 $18 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadVehicle_Energy_Wh_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_VEH_ENERGY
  uint32_t table_val;
  uint8_t i;

  for (i = 0u; i < 8u; i++)
  {
      //table_val = log_get_table_bin(Log_table_veh_energy_usage, i);
	  table_val = 1;
      appdesc_copy_data(&(pMsgContext->resData[i*4]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the cell temperature table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 32u;

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadVTBM_Active_Balancing_Table (Service request header:$22 $3 $19 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadVTBM_Active_Balancing_Table(DescMsgContext* pMsgContext)
{
//#ifndef LOG_DIS_TABLE_VTBM_ACTV_BAL
//  uint32_t table_val;
//  uint8_t i;
//  for (i = 0u; i < LOG_MAX_NUM_OF_VTBM; i++)
//  {
//      //table_val = log_get_table_bin(Log_table_vtbm_n_actv_bal, i);
//	  table_val = 1;
//      appdesc_copy_data(&(pMsgContext->resData[i*4]), (uint8_t *)(&table_val), 4u);
//  }
//#else
//  /* the operating mode table is disabled hence there is no data to report */
//  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
//#endif

//  pMsgContext->resDataLen = 48u;
  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadVTBM_Passive_Balancing_Table (Service request header:$22 $3 $1A )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadVTBM_Passive_Balancing_Table(DescMsgContext* pMsgContext)
{
//#ifndef LOG_DIS_TABLE_VTBM_PASV_BAL
//  uint32_t table_val;
//  uint8_t i;
//  for (i = 0u; i < LOG_MAX_NUM_OF_VTBM; i++)
//  {
      //table_val = log_get_table_bin(Log_table_vtbm_n_pasv_bal, i);
//	  table_val = 1;
//      appdesc_copy_data(&(pMsgContext->resData[i*4]), (uint8_t *)(&table_val), 4u);
//  }
//#else
  /* the operating mode table is disabled hence there is no data to report */
//  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
//#endif

//  pMsgContext->resDataLen = 48u;
  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadNumber_of_Cells_Being_Actively_Balanced_Table (Service request header:$22 $3 $1B )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadNumber_of_Cells_Being_Actively_Balanced_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_N_ACTV_CELLS_BAL
  uint32_t table_val;
  uint8_t i;
  for (i = 0u; i < 6u; i++)
  {
      //table_val = log_get_table_bin(Log_table_n_active_cell_bal, i);
	  table_val = 1;
      appdesc_copy_data(&(pMsgContext->resData[i*4]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the operating mode table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 24u;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadNumber_of_Cells_Being_Passively_Balanced_Table (Service request header:$22 $3 $1C )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadNumber_of_Cells_Being_Passively_Balanced_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_N_PASV_CELLS_BAL
  uint32_t table_val;
  uint8_t i;
  for (i = 0u; i < 6u; i++)
  {
      //table_val = log_get_table_bin(Log_table_n_passive_cell_bal, i);
	  table_val = 1;
      appdesc_copy_data(&(pMsgContext->resData[i*4]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the operating mode table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 24u;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadHighest_Coolant_Inlet_Temperature_Ever (Service request header:$22 $3 $1D )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadHighest_Coolant_Inlet_Temperature_Ever(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = vio_scale_bms_temperature(log_ram_image_1.data.extremes.highest_inlet_t);
  //pMsgContext->resDataLen = 1;
  ///* User service processing finished. */
  //DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadLowest_Coolant_Inlet_Temperature_Ever (Service request header:$22 $3 $1E )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadLowest_Coolant_Inlet_Temperature_Ever(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = vio_scale_bms_temperature(log_ram_image_1.data.extremes.lowest_inlet_t);
  //pMsgContext->resDataLen = 1;
  ///* User service processing finished. */
  //DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCurrent_Inlet_Coolant_Temperature (Service request header:$22 $3 $1F )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCurrent_Inlet_Coolant_Temperature(DescMsgContext* pMsgContext)
{

  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadNumber_of_Charger_Contactor_Closing_Operations (Service request header:$22 $3 $21 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadNumber_of_Charger_Contactor_Closing_Operations(DescMsgContext* pMsgContext)
{
//  uint32_t counter_val = log_ram_image_1.data.event_counters[Log_event_cntr_chg_ctc_closed];
//  appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&counter_val), 4);
//  pMsgContext->resDataLen = 4;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadNumber_of_Vehicle_Contactor_Closing_Operations (Service request header:$22 $3 $22 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadNumber_of_Vehicle_Contactor_Closing_Operations(DescMsgContext* pMsgContext)
{
//  uint32_t counter_val = log_ram_image_1.data.event_counters[Log_event_cntr_veh_ctc_closed];
//  appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&counter_val), 4);
//  pMsgContext->resDataLen = 4;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadDelta_Cell_Temperature_Table (Service request header:$22 $3 $23 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadDelta_Cell_Temperature_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_DELTA_T
  uint32_t table_val;
  uint8_t i;
  for (i = 0u; i < 5u; i++)
  {
      //table_val = log_get_table_bin(Log_table_cell_delta_t, i);
	  table_val = 1;
      appdesc_copy_data(&(pMsgContext->resData[i*4u]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the cell temperature table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 20u;

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadDelta_Cell_Voltage_Table (Service request header:$22 $3 $24 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadDelta_Cell_Voltage_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_DELTA_V
  uint32_t table_val;
  uint8_t i;
  for (i = 0u; i < 5u; i++)
  {
      //table_val = log_get_table_bin(Log_table_cell_delta_v, i);
	  table_val = 1;
      appdesc_copy_data(&(pMsgContext->resData[i*4u]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the cell temperature table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 20u;

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadLowest_SOC_Ever (Service request header:$22 $3 $25 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadLowest_SOC_Ever(DescMsgContext* pMsgContext)
{
 // pMsgContext->resData[0] = vio_scale_percent_up(
 //         log_ram_image_1.data.extremes.lowest_soc, (uint16_t) SOC_FACTOR,
 //         PERCENT(PERCENT_OFFSET));
 // pMsgContext->resDataLen = 1;
 // /* User service processing finished. */
 // DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadNumber_of_Successful_Charge_Cycles (Service request header:$22 $3 $26 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadNumber_of_Successful_Charge_Cycles(DescMsgContext* pMsgContext)
{
//  uint32_t counter_val = log_ram_image_1.data.event_counters[Log_event_cntr_succ_chg_cycles];
//  appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&counter_val), 4);
//  pMsgContext->resDataLen = 4;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadNumber_of_HVIL_Broken_While_Contactors_Closed (Service request header:$22 $3 $27 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadNumber_of_HVIL_Broken_While_Contactors_Closed(DescMsgContext* pMsgContext)
{
//  uint32_t counter_val = log_ram_image_1.data.event_counters[Log_event_cntr_hvil_opened_ctc_closed];
//  appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&counter_val), 4);
//  pMsgContext->resDataLen = 4;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadNumber_of_HVIL_Broken_While_Contactors_Open (Service request header:$22 $3 $28 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadNumber_of_HVIL_Broken_While_Contactors_Open(DescMsgContext* pMsgContext)
{
//  uint32_t counter_val = log_ram_image_1.data.event_counters[Log_event_cntr_hvil_opened_ctc_open];
//  appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&counter_val), 4);
//  pMsgContext->resDataLen = 4;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadVHybridBattery (Service request header:$22 $3 $29 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadVHybridBattery(DescMsgContext* pMsgContext)
{
  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadIActual (Service request header:$22 $3 $2A )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadIActual(DescMsgContext* pMsgContext)
{

  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadPDischargeMaxShortTerm (Service request header:$22 $3 $2B )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadPDischargeMaxShortTerm(DescMsgContext* pMsgContext)
{

  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadIDischargeMaxShortTerm (Service request header:$22 $3 $2C )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadIDischargeMaxShortTerm(DescMsgContext* pMsgContext)
{

  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadIChargeMaxShortTerm (Service request header:$22 $3 $2D )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadIChargeMaxShortTerm(DescMsgContext* pMsgContext)
{

  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadPChargeMaxShortTerm (Service request header:$22 $3 $2E )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadPChargeMaxShortTerm(DescMsgContext* pMsgContext)
{

  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadNBatterySOCDesired (Service request header:$22 $3 $2F )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadNBatterySOCDesired(DescMsgContext* pMsgContext)
{

  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadPDischargeMaxMediumTerm (Service request header:$22 $3 $30 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadPDischargeMaxMediumTerm(DescMsgContext* pMsgContext)
{

  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadIDischargeMaxMediumTerm (Service request header:$22 $3 $31 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadIDischargeMaxMediumTerm(DescMsgContext* pMsgContext)
{

  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadIChargeMaxMediumTerm (Service request header:$22 $3 $32 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadIChargeMaxMediumTerm(DescMsgContext* pMsgContext)
{

  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadPChargeMaxMediumTerm (Service request header:$22 $3 $33 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadPChargeMaxMediumTerm(DescMsgContext* pMsgContext)
{

  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadIDischargeMaxLongTerm (Service request header:$22 $3 $34 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadIDischargeMaxLongTerm(DescMsgContext* pMsgContext)
{

  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadPDischargeMaxLongTerm (Service request header:$22 $3 $35 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadPDischargeMaxLongTerm(DescMsgContext* pMsgContext)
{

  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadIChargeMaxLongTerm (Service request header:$22 $3 $36 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadIChargeMaxLongTerm(DescMsgContext* pMsgContext)
{

  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadPChargeMaxLongTerm (Service request header:$22 $3 $37 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadPChargeMaxLongTerm(DescMsgContext* pMsgContext)
{

  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadVPackDCLink (Service request header:$22 $3 $38 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadVPackDCLink(DescMsgContext* pMsgContext)
{
 
  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadIRaw (Service request header:$22 $3 $39 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadIRaw(DescMsgContext* pMsgContext)
{
  /**
   * No need to worry about losing the sign information by ignoring the
   * the MSB. The IRaw signal range is limited to 3 bytes.
   */

  pMsgContext->resDataLen = 3;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadN_NMSleepBMS (Service request header:$22 $3 $3A )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadN_NMSleepBMS(DescMsgContext* pMsgContext)
{
  uint8_t nm_sleep;
  NmStatusType state;

  GetStatus(&state);
  /* set sleep ACK flag if NM in sleep or waiting to sleep state */
  nm_sleep = (NmStateBusSleep(state) || NmStateWaitBusSleep(state)) ? 2 : 0;
  /* set sleep IND flag if NM in sleep indication state */
  nm_sleep += NmStateBusSleepInd(state);

  pMsgContext->resData[0] = nm_sleep;
  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadDIDs (Service request header:$22 $3 $3B )
 * Description:DID required by MAL
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadDIDs(DescMsgContext* pMsgContext)
{
  uint32_t tmp_u32;

  /* DID Set 1 is packed as follows:
   * Bits 0-7  map to byte 1 bits 0-7
   * Bits 8-15 map to byte 0 bits 0-7
   *
   * BIT 8 is bit 0 of byte 0 and BIT 15 is bit 7 od byte 0
   * |<---    byte 1    --->||<---    byte 0    --->|
   *  7  6  5  4  3  2  1  0  15 14 13 12 11 10  9 8 
   */


  /* contactor close indicator (bit 2) */
  //tmp_u32 |= vmd_veh_close_allow ? 0x0400u : 0u;
  /* charger Pos contactor state (bit 3) */
  tmp_u32 |= (do_rb_chg_dc_pos_hs_mmc && do_rb_chg_dc_pos_ls_mmc) ? 0x0800u : 0u;
  /* charger Neg contactor state (bit 4) */
  tmp_u32 |= (do_rb_chg_dc_neg_hs_mmc && do_rb_chg_dc_neg_ls_mmc) ? 0x1000u : 0u;
  /* contactor open indicator (bit 5) */
  /* vehicle Neg contactor state (bit 6) */
  tmp_u32 |= (do_rb_veh_dc_neg_hs_mmc && do_rb_veh_dc_neg_ls_mmc) ? 0x4000u : 0u;
  /* vehicle Pos contactor state (bit 7) */
  tmp_u32 |= (do_rb_veh_dc_pos_hs_mmc && do_rb_veh_dc_pos_ls_mmc) ? 0x8000u : 0u;
  /* contactor stuck closed fault (bit 8) */

  /* contactor stuck open fault (bit 9) */
  /* BMS status (bits 10 - 14) */
  /* precharge contactor state (bit 15) */
  tmp_u32 |= (do_rb_pre_chg_hs_enbl && do_rb_pre_chg_ls_enbl) ? 0x80u : 0u;
  /* DID Set 1 signals */
  pMsgContext->resData[0] = (uint8_t)tmp_u32;
  pMsgContext->resData[1] = (uint8_t)(tmp_u32 >> 8u);

  /* inlet collant temperature */

  /* outlet collant temperature */
  
  /* DID Set 2 is packed as follows:
   * Bits 0-7   map to byte 7 bits 0-7
   * Bits 8-15  map to byte 6 bits 0-7
   * Bits 16-23 map to byte 5 bits 0-7
   * Bits 24-31 map to byte 4 bits 0-7
   *
   * BIT 8 is bit 0 of byte 0 and BIT 15 is bit 7 od byte 0
   * |<---    byte 7    --->||<---    byte  6    --->||<---    byte  5    --->||<---    byte  4    --->|
   *  7  6  5  4  3  2  1  0  15 14 13 12 11 10  9  8  23 22 21 20 19 18 17 16  31 30 29 28 27 26 25 24
   */

  /* charger tether status (bit 24) */
  //pMsgContext->resData[4] = vmd_imm_indicate_tethered ? 0x01u : 0u;
  pMsgContext->resData[4] = 0;    //


  pMsgContext->resDataLen = 10u;

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadVehicle_Negative_Contactor_Opening_Current_Table (Service request header:$22 $3 $3C )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadVehicle_Negative_Contactor_Opening_Current_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_VEH_NEG_CTC_OPEN_I
  uint32_t table_val;
  uint8_t  i;
  for (i = 0u; i < 12u; i++)
  {
    //table_val = log_get_table_bin(Log_table_veh_neg_ct_opening_i, i);
	table_val = 1;
    appdesc_copy_data(&(pMsgContext->resData[i*4u]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the cell temperature table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 48u;

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadVehicle_Pre_charge_Contactor_Opening_Current_Table (Service request header:$22 $3 $3D )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadVehicle_Pre_charge_Contactor_Opening_Current_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_PRECHG_CTC_OPEN_I
  uint32_t table_val;
  uint8_t i;
  for (i = 0u; i < 9u; i++)
  {
      //table_val = log_get_table_bin(Log_table_prechg_ct_opening_i, i);
	  table_val = 1;
      appdesc_copy_data(&(pMsgContext->resData[i*4u]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the cell temperature table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 36u;

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCharger_Negative_Contactor_Opening_Current_Table (Service request header:$22 $3 $3E )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCharger_Negative_Contactor_Opening_Current_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_CHG_NEG_CTC_OPEN_I
  uint32_t table_val;
  uint8_t i;
  for (i = 0u; i < 9u; i++)
  {
      //table_val = log_get_table_bin(Log_table_chg_neg_ct_opening_i, i);
	  table_val = 1;
      appdesc_copy_data(&(pMsgContext->resData[i*4u]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the cell temperature table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 36u;

  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadReference_State_of_Health (Service request header:$22 $3 $3F )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadReference_State_of_Health(DescMsgContext* pMsgContext)
{
 // pMsgContext->resData[0] = vio_scale_percent_up(
  //                              mdm_bg_output_p->cm.pack_capacity_ref,
  //                              (uint16_t) SOH_FACTOR, 
  //                              VIO_CAPACITY_MIN);
	pMsgContext->resData[0] = 0;    //
  /* Always set the correct length of the response data. */
  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadVehicle_Bus_Positive_Rail_Voltage (Service request header:$22 $3 $40 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadVehicle_Bus_Positive_Rail_Voltage(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = (uint8_t)((uint16_t)VMI_VEH_POS_V >> 8u);
  //pMsgContext->resData[1] = (uint8_t)VMI_VEH_POS_V;
  pMsgContext->resData[0] = 0;    //
  pMsgContext->resData[1] = 0;    //
  
  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadVehicle_Bus_Negative_Rail_Voltage (Service request header:$22 $3 $41 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadVehicle_Bus_Negative_Rail_Voltage(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = (uint8_t)((uint16_t)VMI_VEH_NEG_V >> 8u);
  //pMsgContext->resData[1] = (uint8_t)VMI_VEH_NEG_V;
  pMsgContext->resData[0] = 0;    //
  pMsgContext->resData[1] = 0;    //
  
  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCharger_Bus_Positive_Rail_Voltage (Service request header:$22 $3 $42 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCharger_Bus_Positive_Rail_Voltage(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = (uint8_t)((uint16_t)VMI_CHG_POS_V >> 8u);
  //pMsgContext->resData[1] = (uint8_t)VMI_CHG_POS_V;
  pMsgContext->resData[0] = 0;    //
  pMsgContext->resData[1] = 0;    //
  
  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCharger_Bus_Negative_Rail_Voltage (Service request header:$22 $3 $43 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCharger_Bus_Negative_Rail_Voltage(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = (uint8_t)((uint16_t)VMI_CHG_NEG_V >> 8u);
  //pMsgContext->resData[1] = (uint8_t)VMI_CHG_NEG_V;
  pMsgContext->resData[0] = 0;    //
  pMsgContext->resData[1] = 0;    //
  
  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadLast_Estimated_Balance_at_Top_of_Charge (Service request header:$22 $3 $44 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadLast_Estimated_Balance_at_Top_of_Charge(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = umc_ram_image.data.span_est[BalEstUpper];
  pMsgContext->resData[0] = 0;    //
  
  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadLast_Estimated_Balance_at_Bottom_of_Charge (Service request header:$22 $3 $45 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadLast_Estimated_Balance_at_Bottom_of_Charge(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = umc_ram_image.data.span_est[BalEstLower];
  pMsgContext->resData[0] = 0;    //
  
  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadLast_Estimated_Balance_at_Top_of_Charge_Timestamp (Service request header:$22 $3 $46 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadLast_Estimated_Balance_at_Top_of_Charge_Timestamp(DescMsgContext* pMsgContext)
{
  //uint32_t timestamp = umc_ram_image.data.span_timestamp[BalEstUpper];
  //appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&timestamp), 4);
  pMsgContext->resData[0] = 0;    //
  pMsgContext->resData[1] = 0;    //
  pMsgContext->resData[2] = 0;    //
  pMsgContext->resData[3] = 0;    //
  
  pMsgContext->resDataLen = 4;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadLast_Estimated_Balance_at_Bottom_of_Charge_Timestamp (Service request header:$22 $3 $47 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadLast_Estimated_Balance_at_Bottom_of_Charge_Timestamp(DescMsgContext* pMsgContext)
{
  //uint32_t timestamp = umc_ram_image.data.span_timestamp[BalEstLower];
  //appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&timestamp), 4);
  
  pMsgContext->resData[0] = 0;    //
  pMsgContext->resData[1] = 0;    //
  pMsgContext->resData[2] = 0;    //
  pMsgContext->resData[3] = 0;    //
  
  pMsgContext->resDataLen = 4;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadNumber_of_Ignition_Cycles (Service request header:$22 $3 $48 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadNumber_of_Ignition_Cycles(DescMsgContext* pMsgContext)
{
//  uint32_t counter_val = log_ram_image_1.data.event_counters[Log_event_cntr_ign_turned_on];
//  appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&counter_val), 4);
//  pMsgContext->resDataLen = 4;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadSOC_Table (Service request header:$22 $3 $49 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadSOC_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_SOC
  uint32_t table_val;
  uint8_t i;
  for (i = 0u; i < 10u; i++)
  {
      //table_val = log_get_table_bin(Log_table_soc, i);
	  table_val = 1;
      appdesc_copy_data(&(pMsgContext->resData[i*4u]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the cell temperature table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 40;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadVehicle_Bus_Current_Table (Service request header:$22 $3 $4A )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadVehicle_Bus_Current_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_VEH_I
  uint32_t table_val;
  uint8_t i;
  for (i = 0u; i < 10u; i++)
  {
      //table_val = log_get_table_bin(Log_table_veh_bus_i, i);
	  table_val = 1;
      appdesc_copy_data(&(pMsgContext->resData[i*4u]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the cell temperature table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 40;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCharger_Bus_Current_Table (Service request header:$22 $3 $4B )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCharger_Bus_Current_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_CHG_I
  uint32_t table_val;
  uint8_t i;
  for (i = 0u; i < 10u; i++)
  {
      //table_val = log_get_table_bin(Log_table_charger_bus_i, i);
	  table_val = 1;
      appdesc_copy_data(&(pMsgContext->resData[i*4u]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the cell temperature table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 40;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadPrecharge_Contactor_Wear_Counter (Service request header:$22 $3 $4C )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadPrecharge_Contactor_Wear_Counter(DescMsgContext* pMsgContext)
{
//  uint32_t counter_val = log_ram_image_1.data.data_integration[Log_integ_ctc_wear_prechg];
//  appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&counter_val), 4);
//  pMsgContext->resDataLen = 4;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadVehicle_Bus_Positive_Contactor_Wear_Counter (Service request header:$22 $3 $4D )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadVehicle_Bus_Positive_Contactor_Wear_Counter(DescMsgContext* pMsgContext)
{
//  uint32_t counter_val = log_ram_image_1.data.data_integration[Log_integ_ctc_wear_veh_pos];
//  appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&counter_val), 4);
//  pMsgContext->resDataLen = 4;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadVehicle_Bus_Negative_Contactor_Wear_Counter (Service request header:$22 $3 $4E )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadVehicle_Bus_Negative_Contactor_Wear_Counter(DescMsgContext* pMsgContext)
{
//  uint32_t counter_val = log_ram_image_1.data.data_integration[Log_integ_ctc_wear_veh_neg];
//  appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&counter_val), 4);
//  pMsgContext->resDataLen = 4;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCharger_Bus_Positive_Contactor_Wear_Counter (Service request header:$22 $3 $4F )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCharger_Bus_Positive_Contactor_Wear_Counter(DescMsgContext* pMsgContext)
{
//  uint32_t counter_val = log_ram_image_1.data.data_integration[Log_integ_ctc_wear_chg_pos];
//  appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&counter_val), 4);
//  pMsgContext->resDataLen = 4;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCharger_Bus_Negative_Contactor_Wear_Counter (Service request header:$22 $3 $50 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCharger_Bus_Negative_Contactor_Wear_Counter(DescMsgContext* pMsgContext)
{
//  uint32_t counter_val = log_ram_image_1.data.data_integration[Log_integ_ctc_wear_chg_neg];
//  appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&counter_val), 4);
//  pMsgContext->resDataLen = 4;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadHighest_SOC_Ever (Service request header:$22 $3 $51 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadHighest_SOC_Ever(DescMsgContext* pMsgContext)
{
//  pMsgContext->resData[0] = vio_scale_percent_up(
//          log_ram_image_1.data.extremes.highest_soc, (uint16_t) SOC_FACTOR,
//          PERCENT(PERCENT_OFFSET));
//  pMsgContext->resDataLen = 1;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadHighest_Cell_Voltage_Ever (Service request header:$22 $3 $52 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadHighest_Cell_Voltage_Ever(DescMsgContext* pMsgContext)
{
  //uint16_t scaled_v = vio_scale_cell_voltage(log_ram_image_1.data.extremes.highest_cell_v);
  //appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&scaled_v), 2);
  //pMsgContext->resDataLen = 2;
  ///* User service processing finished. */
  //DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCell_Number_with_Highest_Cell_Voltage_Ever (Service request header:$22 $3 $53 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCell_Number_with_Highest_Cell_Voltage_Ever(DescMsgContext* pMsgContext)
{
//  pMsgContext->resData[0] = log_ram_image_1.data.extremes.cell_num_hi_v;
//  pMsgContext->resDataLen = 1;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadLowest_Cell_Voltage_Ever (Service request header:$22 $3 $54 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadLowest_Cell_Voltage_Ever(DescMsgContext* pMsgContext)
{
  //uint16_t scaled_v = vio_scale_cell_voltage(log_ram_image_1.data.extremes.lowest_cell_v);
  //appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&scaled_v), 2);
  //pMsgContext->resDataLen = 2;
  ///* User service processing finished. */
  //DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCell_Number_with_Lowest_Cell_Voltage_Ever (Service request header:$22 $3 $55 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCell_Number_with_Lowest_Cell_Voltage_Ever(DescMsgContext* pMsgContext)
{
//  pMsgContext->resData[0] = log_ram_image_1.data.extremes.cell_num_lo_v;
//  pMsgContext->resDataLen = 1;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadHighest_Outlet_Coolant_Temperature_Ever (Service request header:$22 $3 $56 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadHighest_Outlet_Coolant_Temperature_Ever(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = vio_scale_bms_temperature(log_ram_image_1.data.extremes.highest_outlet_t);
  //pMsgContext->resDataLen = 1;
  ///* User service processing finished. */
  //DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadLowest_Outlet_Coolant_Temperature_Ever (Service request header:$22 $3 $57 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadLowest_Outlet_Coolant_Temperature_Ever(DescMsgContext* pMsgContext)
{
 // pMsgContext->resData[0] = vio_scale_bms_temperature(log_ram_image_1.data.extremes.lowest_outlet_t);
 // pMsgContext->resDataLen = 1;
 // /* User service processing finished. */
 // DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadHighest_Coolant_Temperature_Delta_Ever (Service request header:$22 $3 $58 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadHighest_Coolant_Temperature_Delta_Ever(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = vio_scale_bms_temperature(log_ram_image_1.data.extremes.highest_pack_delta_t);
  //pMsgContext->resDataLen = 1;
  ///* User service processing finished. */
  //DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadHighest_Charge_Current_Ever (Service request header:$22 $3 $59 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadHighest_Charge_Current_Ever(DescMsgContext* pMsgContext)
{
  //uint16_t scaled_i = vio_scale_i_charge(log_ram_image_1.data.extremes.highest_chg_i_ma);
  //appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&scaled_i), 2);
  //pMsgContext->resDataLen = 2;
  ///* User service processing finished. */
  //DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadHighest_Discharge_Current_Ever (Service request header:$22 $3 $5A )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadHighest_Discharge_Current_Ever(DescMsgContext* pMsgContext)
{
  //uint16_t scaled_i = vio_scale_i_charge(log_ram_image_1.data.extremes.highest_dis_i_ma);
  //appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&scaled_i), 2);
  //pMsgContext->resDataLen = 2;
  ///* User service processing finished. */
  //DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCurrent_Outlet_Coolant_Temperature (Service request header:$22 $3 $5B )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCurrent_Outlet_Coolant_Temperature(DescMsgContext* pMsgContext)
{
  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCurrent_BMS_Timestamp (Service request header:$22 $3 $5C )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCurrent_BMS_Timestamp(DescMsgContext* pMsgContext)
{
  uint32_t ts = rtc_get_seconds();
  appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&ts), 4);
  pMsgContext->resDataLen = 4;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCell_Storage_Temperature_Table (Service request header:$22 $3 $5D )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCell_Storage_Temperature_Table(DescMsgContext* pMsgContext)
{
#ifndef LOG_DIS_TABLE_TEMPERATURE
  uint32_t table_val;
  uint8_t i;
  for (i = 0u; i < 10u; i++)
  {
      //table_val = log_get_table_bin(Log_table_cell_strg_temperature, i);
	  table_val = 1;
      appdesc_copy_data(&(pMsgContext->resData[i*4u]), (uint8_t *)(&table_val), 4u);
  }
#else
  /* the cell temperature table is disabled hence there is no data to report */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
#endif

  pMsgContext->resDataLen = 40u;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadHighest_Cell_Storage_Temperature_Ever (Service request header:$22 $3 $5E )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadHighest_Cell_Storage_Temperature_Ever(DescMsgContext* pMsgContext)
{
  //uint16_t log_temp = vio_scale_cell_temperature(log_ram_image_1.data.extremes.highest_strg_cell_t);
  //appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&log_temp), 2);
  //pMsgContext->resDataLen = 2;
  ///* User service processing finished. */
  //DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCell_Number_with_Highest_Cell_Storage_Temperature_Ever (Service request header:$22 $3 $5F )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCell_Number_with_Highest_Cell_Storage_Temperature_Ever(DescMsgContext* pMsgContext)
{
//  pMsgContext->resData[0] = log_ram_image_1.data.extremes.cell_num_hi_strg_t;
//  pMsgContext->resDataLen = 1;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadLowest_Cell_Storage_Temperature_Ever (Service request header:$22 $3 $60 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadLowest_Cell_Storage_Temperature_Ever(DescMsgContext* pMsgContext)
{
  //uint16_t log_temp = vio_scale_cell_temperature(log_ram_image_1.data.extremes.lowest_strg_cell_t);
  //appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&log_temp), 2);
  //pMsgContext->resDataLen = 2;
  ///* User service processing finished. */
  //DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCell_Number_with_Lowest_Cell_Storage_Temperature_Ever (Service request header:$22 $3 $61 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCell_Number_with_Lowest_Cell_Storage_Temperature_Ever(DescMsgContext* pMsgContext)
{
//  pMsgContext->resData[0] = log_ram_image_1.data.extremes.cell_num_lo_strg_t;
//  pMsgContext->resDataLen = 1;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadTotal_Energy_Throughput (Service request header:$22 $3 $62 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadTotal_Energy_Throughput(DescMsgContext* pMsgContext)
{
  uint32_t tot_energy;
//  tot_energy = addu32(LOG_ENERGY_VEHICLE, LOG_ENERGY_REGEN, ENERGY_WH_BP, ENERGY_WH_BP, ENERGY_WH_BP);
//  tot_energy = addu32(tot_energy, LOG_ENERGY_CHARGER, ENERGY_WH_BP, ENERGY_WH_BP, ENERGY_WH_BP);
  appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&tot_energy), 4);
  pMsgContext->resDataLen = 4;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadHighest_Operational_Cell_Voltage_Ever (Service request header:$22 $3 $63 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadHighest_Operational_Cell_Voltage_Ever(DescMsgContext* pMsgContext)
{
  //uint16_t scaled_v = vio_scale_cell_voltage(log_ram_image_1.data.extremes.highest_op_cell_v);
  //appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&scaled_v), 2);
  //pMsgContext->resDataLen = 2;
  ///* User service processing finished. */
  //DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCell_Number_with_Highest_Operational_Cell_Voltage_Ever (Service request header:$22 $3 $64 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCell_Number_with_Highest_Operational_Cell_Voltage_Ever(DescMsgContext* pMsgContext)
{
//  pMsgContext->resData[0] = log_ram_image_1.data.extremes.cell_num_hi_op_v;
//  pMsgContext->resDataLen = 1;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadOperational_Lowest_Cell_Voltage_Ever (Service request header:$22 $3 $65 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadOperational_Lowest_Cell_Voltage_Ever(DescMsgContext* pMsgContext)
{
  //uint16_t scaled_v = vio_scale_cell_voltage(log_ram_image_1.data.extremes.lowest_op_cell_v);
  //appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&scaled_v), 2);
  //pMsgContext->resDataLen = 2;
  ///* User service processing finished. */
  //DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCell_Number_with_Lowest_Operational_Cell_Voltage_Ever (Service request header:$22 $3 $66 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCell_Number_with_Lowest_Operational_Cell_Voltage_Ever(DescMsgContext* pMsgContext)
{
//  pMsgContext->resData[0] = log_ram_image_1.data.extremes.cell_num_lo_op_v;
//  pMsgContext->resDataLen = 1;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadTotal_Vehicle_Energy (Service request header:$22 $3 $67 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadTotal_Vehicle_Energy(DescMsgContext* pMsgContext)
{
//  uint32_t energy = LOG_ENERGY_VEHICLE;
//  appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&energy), 4);
//  pMsgContext->resDataLen = 4;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadTotal_Regen_Energy (Service request header:$22 $3 $68 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadTotal_Regen_Energy(DescMsgContext* pMsgContext)
{
//  uint32_t energy = LOG_ENERGY_REGEN;
//  appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&energy), 4);
//  pMsgContext->resDataLen = 4;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadTotal_Charger_Energy (Service request header:$22 $3 $69 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadTotal_Charger_Energy(DescMsgContext* pMsgContext)
{
//  uint32_t energy = LOG_ENERGY_CHARGER;
//  appdesc_copy_data(pMsgContext->resData, (uint8_t *)(&energy), 4);
//  pMsgContext->resDataLen = 4;
//  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadSOCcHigh (Service request header:$22 $3 $6A )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadSOCcHigh(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = vio_scale_percent_up(mdm_cell_soc_comb_max, 
 //                                                (uint16_t) SOC_FACTOR,
 //                                                PERCENT(PERCENT_OFFSET));
 pMsgContext->resData[0] = 0;    //
 
  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadSOCcLow (Service request header:$22 $3 $6B )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadSOCcLow(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = vio_scale_percent_up(mdm_cell_soc_comb_min, 
  //                                               (uint16_t) SOC_FACTOR,
  //                                               PERCENT(PERCENT_OFFSET));
  pMsgContext->resData[0] = 0;    //
  
  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadSOC_Reset_State (Service request header:$22 $3 $6C )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadSOC_Reset_State(DescMsgContext* pMsgContext)
{
  //pMsgContext->resData[0] = (uint8_t)mdm_bg_write_p->cm.dcd_state;
  pMsgContext->resData[0] = 0;    //
  
  pMsgContext->resDataLen = 1;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadSOC_Reset_Timer (Service request header:$22 $3 $6D )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadSOC_Reset_Timer(DescMsgContext* pMsgContext)
{
  pMsgContext->resDataLen = 2;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadID_Block (Service request header:$22 $F1 $0 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadID_Block(DescMsgContext* pMsgContext)
{
//  obd_read_idblock(pMsgContext->resData,
//                                (FPTR uint16_t *)&pMsgContext->resDataLen);
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadFingerprint (Service request header:$22 $F1 $5A )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadFingerprint(DescMsgContext* pMsgContext)
{
  /* this fingerprint is only available in reprogramming mode */
  DescSetNegResponse(kDescNrcSubfunctionNotSupportedInActiveSession);
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadBoot_Software_ID (Service request header:$22 $F1 $80 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadBoot_Software_ID(DescMsgContext* pMsgContext)
{
//  obd_read_boot_sw_id(pMsgContext->resData,
//                                (FPTR uint16_t *)&pMsgContext->resDataLen);
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadApplication_Software_ID (Service request header:$22 $F1 $81 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadApplication_Software_ID(DescMsgContext* pMsgContext)
{
//  obd_read_app_sw_id(pMsgContext->resData,
//                                (FPTR uint16_t *)&pMsgContext->resDataLen);
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCalibration_Data_ID (Service request header:$22 $F1 $82 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCalibration_Data_ID(DescMsgContext* pMsgContext)
{
 // obd_read_cal_data_id(pMsgContext->resData,
 //                               (FPTR uint16_t *)&pMsgContext->resDataLen);
 // DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadBoot_Software_Fingerprint (Service request header:$22 $F1 $83 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadBoot_Software_Fingerprint(DescMsgContext* pMsgContext)
{
  /* Our bootloaders don't have a fingerprint! */
  DescSetNegResponse(kDescNrcSubfunctionNotSupported);
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadApplication_Software_Fingerprint (Service request header:$22 $F1 $84 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadApplication_Software_Fingerprint(DescMsgContext* pMsgContext)
{
 //   if (ISO_BOOTBLOCK)
 //   {
 //       obd_read_app_fingerprint(pMsgContext->resData,
 //                               (FPTR uint16_t *)&pMsgContext->resDataLen);
 //   }
 //   else
 //   {
 //       /* We're not running with an FBL so there aren't any fingerprints */
 //       DescSetNegResponse(kDescNrcSubfunctionNotSupported);
 //   }
 // DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadCalibration_Data_Fingerprint (Service request header:$22 $F1 $85 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadCalibration_Data_Fingerprint(DescMsgContext* pMsgContext)
{
  //  if (ISO_BOOTBLOCK)
  //  {
  //      obd_read_cal_fingerprint(pMsgContext->resData,
  //                              (FPTR uint16_t *)&pMsgContext->resDataLen);
  //  }
  //  else
  //  {
  //      /* We're not running with an FBL so there aren't any fingerprints */
  //      DescSetNegResponse(kDescNrcSubfunctionNotSupported);
  //  }
  //DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadECU_Manufacturing_Date (Service request header:$22 $F1 $8B )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadECU_Manufacturing_Date(DescMsgContext* pMsgContext)
{
  /* date is reported in 1 byte BCD format */
  /* year */
  pMsgContext->resData[0] = (mmc_eol.year_of_manufacture[2] - '0')*16u +
                            (mmc_eol.year_of_manufacture[3] - '0');
  /* month */
  pMsgContext->resData[1] = (mmc_eol.month_of_manufacture[0] - '0')*16u +
                            (mmc_eol.month_of_manufacture[1] - '0');
  /* day */
  pMsgContext->resData[2] = (mmc_eol.day_of_manufacture[0] - '0')*16u +
                            (mmc_eol.day_of_manufacture[1] - '0');
  pMsgContext->resDataLen = 3;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadECU_Serial_Number (Service request header:$22 $F1 $8C )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadECU_Serial_Number(DescMsgContext* pMsgContext)
{
  uint8_t i;
  for (i = 0; i < 12; i++)
  {
      pMsgContext->resData[i] = mmc_eol.unit_serial_number[i];
  }
  pMsgContext->resDataLen = 12;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadVIN (Service request header:$22 $F1 $90 )
 * Description: Read VIN number
 * Returns:  VIN number
 * Parameter(s):
 *   - pMsgContext->reqData: not used
 *   - pMsgContext->resData: VIN number
 *   - pMsgContext->reqDataLen: 17
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadVIN(DescMsgContext* pMsgContext)
{
 // uint8_t i;
 // for (i = 0; i < VIO_VIN_LEN; i++)
 // {
 //     pMsgContext->resData[i] = log_ram_image_1.data.vin[i];
 // }
 // pMsgContext->resDataLen = VIO_VIN_LEN;
 // /* User service processing finished. */
 // DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadHardware_ID (Service request header:$22 $F1 $91 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadHardware_ID(DescMsgContext* pMsgContext)
{
//  uint8_t i;
//  for (i = 0; i < OBD_HW_ID_LEN; i++)
//  {
//      pMsgContext->resData[i] = (uint8_t)obd_id_hw_id[i];
//  }
//  pMsgContext->resDataLen = OBD_HW_ID_LEN;
//  /* User service processing finished. */
//  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadSystem_Supplier_ECU_Hardware_Version_Number (Service request header:$22 $F1 $93 )
 * Description: not available 
 * Returns:  nothing
 * Parameter(s):
 *   - pMsgContext->reqData:
 *       - Points to the first service request data byte.
 *       - Access type: read
 *   - pMsgContext->resData:
 *       - Points to the first writeable byte for the service response data.
 *       - Access type: read/write
 *   - pMsgContext->reqDataLen:
 *       - Contains the count of the service request data bytes (Sid is excluded).
 *       - Access type: read
 *   - pMsgContext->resDataLen:
 *       - Must be initialized with the count of the service response data bytes (Sid is excluded).
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.reqType:
 *       - Shows the addressing type of the request (kDescPhysReq or kDescFuncReq).
 *       - Access type: read
 *   - pMsgContext->msgAddInfo.resOnReq:
 *       - Indicates if there will be response. Allowed only to write only 0 !!!
 *       - Access type: read/write
 *   - pMsgContext->msgAddInfo.suppPosRes:
 *       - UDS only!If set no positive response will be sent on this request.
 *       - Access type: read
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadSystem_Supplier_ECU_Hardware_Version_Number(DescMsgContext* pMsgContext)
{
  uint8_t i;
  for (i = 0; i < 13; i++)
  {
      pMsgContext->resData[i] = mmc_eol.unit_hw_revision[i];
  }
  pMsgContext->resDataLen = 13;
  /* User service processing finished. */
  DescProcessingDone();
}


/*  ********************************************************************************
 * Function name:ApplDescReadPack_Serial_Number (Service request header:$22 $FE $A0 )
 * Description: not available
 * Returns:  unit serial number
 * Parameter(s):
 *   - pMsgContext->reqData: not used
 *   - pMsgContext->resData: unit serial number
 *   - pMsgContext->resDataLen: 13
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" must be called from now on (within this
 * main-handler or later).
 *   - The function "DescSetNegResponse" can be called within this main-handler or
 * later but before calling "DescProcessingDone".
 ********************************************************************************  */
void DESC_API_CALLBACK_TYPE ApplDescReadPack_Serial_Number(DescMsgContext* pMsgContext)
{
  //uint8_t i;
  //for (i = 0u; i < PACK_OFFLINECAL_PACK_SN_SZ; i++)
  //{
  //    pMsgContext->resData[i] = cal_offline_data.cal_set2.data.pack_serial_number[i];
  //}
  //pMsgContext->resDataLen = PACK_OFFLINECAL_PACK_SN_SZ;
  ///* User service processing finished. */
  //DescProcessingDone();
}


#if defined (DESC_ENABLE_MULTI_CFG_SUPPORT)
/* ********************************************************************************
 * Function name:ApplDescIsDataIdSupported
 * Description: Additionaly reject a supported PID (multi ECU configuration)
 * Returns:  kDescTrue - if still supported, kDescFalse - if not supported
 * Parameter(s):The PID number
 * Particularitie(s) and limitation(s):
 *   - The function "DescProcessingDone" may not be called.
 *   - The function "DescSetNegResponse" may not be called.
 ******************************************************************************** */
DescBool ApplDescIsDataIdSupported(vuint16 pid)
{
  return kDescTrue;
}
#endif

/******************************************************************************
* Name         :  appdesc_copy_data
* Called by    :  ApplDescRead<xxx> callbacks
* Preconditions:  None
* Parameters   :  None
* Description  :  Custom memcpy version for copying data into diagnostics
*                 buffers.
******************************************************************************/
static void appdesc_copy_data(uint8_t* dest, uint8_t* src, uint16_t len)
{
    uint16_t i;
    for (i = 0u; i < len; i++)
    {
        *dest++ = *src++;
    }
}


