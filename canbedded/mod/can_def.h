/* Kernbauer Version: 1.16 Konfiguration: can_driver Erzeugungsgangnummer: 1 */

/* STARTSINGLE_OF_MULTIPLE */


/*****************************************************************************
| Project Name: DrvCan_Mcs12xMscanHll
|    File Name: CAN_DEF.H
|
|  Description: Application-Interface of the CAN-Driver
|               Declaration of functions, variables, and constants
|
|  Target and Compiler: see file can_drv.c
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 1996-2008 by Vector Informatik GmbH.       All rights reserved.
|
| This software is copyright protected and proprietary 
| to Vector Informatik GmbH. Vector Informatik GmbH 
| grants to you only those rights as set out in the 
| license conditions. All other rights remain with 
| Vector Informatik GmbH.
|
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     ------------------------------------
| Ml           Patrick Markl             Vector Informatik GmbH
| Ces          Senol Cendere             Vector Informatik GmbH 
| Ou           Mihai Olariu              Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date       Ver  Author  Description
| ---------  ---  ------ -- --------------------------------------------------
| 2004-05-27 00.01.00    Ml     Creation 
| 2004-10-14 01.00.00    Ml     Added support for Metrowerks
| 2004-11-21 01.01.00    Ml     ESCAN00010294: no changes
|                        Ml     ESCAN00010295: no line with single semicolon
| 2004-11-28 01.02.00    Ml     ESCAN00010461: no changes
|                        Ml     ESCAN00010572: Resolved corrupted Rx data
| 2005-05-26 01.03.00    Ml     ESCAN00013824: no changes
| 2005-08-26             Msr/Ml ESCAN00013351: no changes
| 2005-11-01 02.00.00    Ml     ESCAN00014262: no changes
| 2005-12-14 02.01.00    Ml     ESCAN00014687: resolved cat 2 wakeup macro
|                        Ml     ESCAN00014414: no changes
|                        Ml     ESCAN00014769: no changes
|                        Ml     ESCAN00014773: Corrected id type macro
|                        Ml     ESCAN00014791: Simplified MK_EXTID
| 2006-01-14 02.02.00    Ml     ESCAN00014907: Resolved computation of ECU number
| 2006-05-03 02.03.00    Ml     ESCAN00016237: Changed names of ISRs
| 2006-07-10 02.04.00    Ml     ESCAN00016840: Added support for cancel in hardware
| 2006-07-17 03.00.00    Ml     ESCAN00017189: Added support for Mcs08 family
|                        Ml     ESCAN00017134: no changes
|                        Ml     ESCAN00017257: Corrected CanTxWriteActId
|                        Ml     ESCAN00017166: no changes
| 2006-08-13 04.00.00    Ml     ESCAN00017700: Added Mpc5200 platform
| 2006-09-19 04.01.00    Ml     ESCAN00017435: no changes
| 2006-11-30 04.02.00    Ces    ESCAN00018590: Support Autosar
| 2006-12-06 04.03.00    Ml     ESCAN00018636: Corrected MK_EXTID macro
| 2006-12-19 04.03.00    Ml     ESCAN00018770: no changes
| 2006-01-08 04.03.00    Ces    ESCAN00018927: Support IAR compiler
|                        Ces    ESCAN00018928: Support Multi ECU configuration
|                        Ces    ESCAN00018930: Support RAM check
|                        Ces    ESCAN00018931: Wrong Tx data pointer used in pretransmit function
| 2007-02-13 04.04.00    Ml     ESCAN00019569: Changed macros of ISRs in order to work with Autosar OS
|                        Ml     ESCAN00019678: no changes
|                        Ml     ESCAN00019688: no changes
|                        Ces    ESCAN00019730: Adaptions for Autosar
| 2007-03-06 04.05.00    Ou     ESCAN00019728: Update for the multi ECU configuration support in XGate
| 2007-03-09 04.06.00    Ml     ESCAN00019894: Removed parts for XGate transmission
| 2007-03-15 04.07.00    Ml     ESCAN00019949: Change the handling of interrupts
| 2007-04-03 04.08.00    Ces    ESCAN00020177: Adaptions for Autosar
| 2007-04-09 05.00.00    Ou     ESCAN00020104: Upgrade to RI 1.5
|                        Ou     ESCAN00020163: Apply the erratum workaround regarding "SSEM instruction"
| 2007-08-10 05.01.00    Ces    Support Microsar Revision 2.1
| 2007-09-04 05.02.00    Ou     ESCAN00020861: no changes
|                        Ou     ESCAN00021695: Messages received in range(s) are not processed
|                        Ou     ESCAN00021724: no changes
| 2007-09-13 05.03.00    Ou     Add XGate support for Microsar
| 2008-01-09 05.04.00    Ou     ESCAN00022514: Interrupts don't work with OSEK OS
|                        Ou     ESCAN00022567: Message not yet sent is confirmed (CanRxInterrupt() interrupts CanTxTask())
|                        Ou     ESCAN00022855: Message not yet sent is confirmed ((CanTxTask() interrupts CanTransmit())
|                        Ou     ESCAN00022867: Message is corrupted when normal and low-level transmission interrupts each other
|                        Ou     ESCAN00023651: Message is corrupted when Tx confirmation interrupts either normal or low-level transmission (MCS12X HLL)
|                        Ou     ESCAN00023567: ISRs should not be mapped in the default section for non-banked routines
|                        Ou     ESCAN00025429: CAN interrupts are not correctly disabled
| 2008-04-15 05.05.00    Ou     ESCAN00026446: Support AUTOSAR multiplexed Tx feature
| 2008-08-22 05.06.00    Ou     ESCAN00029451: Support XGate for ASR3.0
| 2008-09-22 05.07.00    Ou     ESCAN00030206: Wrong Tx confirmation call after startup
|                        Ou     ESCAN00030207: Wrong hardware handle is passed to the CAN interface on indication
|                        Ou     ESCAN00030948: CAN interrupts are not correctly and fully disabled/ enabled
| 2008-12-16 05.07.01    Ou     No changes, just correct the fixed ESCAN in the history (ESCAN00030948 instead of ESCAN00030347)
|****************************************************************************/
/*****************************************************************************
|
|    ************    Version and change information of      **********        
|    ************    high level part only                   **********        
|
|    Please find the version number of the whole module in the previous 
|    File header.
|
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     ------------------------------------
| Ht           Heike Honert              Vector Informatik GmbH
| Pl           Georg Pfluegel            Vector Informatik GmbH
| Vg           Frank Voorburg            Vector CANtech, Inc.
| An           Ahmad Nasser              Vector CANtech, Inc.
| Ml           Patrick Markl             Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date       Ver  Author  Description
| ---------  ---  ------ -----------------------------------------------------
| ---------  ---  ------ -- --------------------------------------------------
| 24-Jan-01  0.02      Ht     - derived from C16x V3.3
| 18-Apr-01  1.00      Pl     - adaption for ARM7 TDMI
| 26-Apr-01  1.01      Ht     - adaption to LI 1.2
|                             - changed from code doupling to indexed
| 19-Jun-01            Ht     - define of CanSetActive/Passive changed
| 31-Oct-01  1.02      Ht     - support hash search
|                      Vg     - adaption for PowerPC
| 07-Nov-01  1.03      Ht     - remove some comments
| 12-Nov-01  1.04      Fz     - no changes
| 02-Jan-02  1.05      Ht     - ESCAN00002009: support of polling mode improved
| 12-Feb-02  1.06      Pl     - ESCAN00002279: - now it is possible to use only the error-task without the tx-task
|                                              - support of the makro  REENTRANT
|                                              - support of the makro C_HL_ENABLE_RX_INFO_STRUCT_PTR
|                                              - For better performance for the T89C51C there is a switch-case
|                                                instruction for direct call of the PreTransmitfunction
|                                              - insert Prototyp for CanErrorTask
| 18-Mai-02  1.07       Ht    - ESCAN....    : support Hash search with FullCAN controller
|                             - ESCAN00002707: Reception could went wrong if IL and Hash Search
|                             - ESCAN00002893: adaption to LI 1.3
| 29-Mai-02  1.08       Ht    - ESCAN00003028: Transmission could fail in Polling mode
|                             - ESCAN00003082: call Can_LL_TxEnd() in CanMsgTransmit()
|                             - ESCAN00003083: Hash search with extended ID
|                             - ESCAN00003084: Support C_COMP_METROWERKS_PPC
|                             - ESCAN00002164: Temporary vaiable "i" not defined in function CanBasicCanMsgReceived
|                             - ESCAN00003085: support C_HL_ENABLE_IDTYPE_IN_ID
| 25-Jun     1.08.01    Vg    - Declared localInterruptOldFlag in CanRxTask()
|                             - Corrected call to CanWakeUp for multichannel
| 11-Jul-02  1.08.02    Ht    - ESCAN00003203: Hash Search routine does not work will with extended IDs
|                             - ESCAN00003205: Support of ranges could went wrong on some platforms
| 08-Aug-02  1.08.03    Ht    - ESCAN00003447: Transmission without databuffer and pretransmit-function 
| 08-Aug-02  1.08.04    An      no changes
| 09-Sep-02  1.09       Ht    - ESCAN00003837: code optimication with KernelBuilder 
| 2002-12-12 1.10       Ht    -                support Dummy functions for indirect function call 
| 2003-02-04 1.11       Ht    -                optimization for polling mode
|                       Fz    - ESCAN00004600: Flags changed for V850 aFCAN only
| 2003-03-19 1.12       Ht    - ESCAN00005153: Wrong number of Indication bytes for V850 aFCAN only
|                             - ESCAN00005152: optimization of CanInit() in case of Direct Tx Objects
|                             - ESCAN00005143: CompilerWarning about function prototype 
|                                              CanHL_ReceivedRxHandle() and CanHL_IndRxHandle
|                             - ESCAN00005130: Wrong result of Heash Search on second or higher channel               
| 2003-05-12 1.13       Ht    - ESCAN00005624: support CantxMsgDestroyed for multichannel system
|                             - ESCAN00005209: Support confirmation and indication flags for EasyCAN4
|                             - ESCAN00004721: Change data type of Handle in CanRxInfoStruct
| 2003-06-18 1.20       Ht    - ESCAN00005908: support features of RI1.4
|                             - ESCAN: Support FJ16LX-Workaround for multichannel system
|                             - ESCAN00005671: Dynamic ID in extended ID system could fail
|                             - ESCAN00005863: Notification about cancelation failes in case of CanTxMsgDestroyed
| 2003-06-30 1.21       Ht   - ESCAN00006117: Common Confirmation Function: Write access to wrong memory location
|                            - ESCAN00006008: CanCanInterruptDisable in case of polling
|                            - ESCAN00006118: Optimization for Mixed ID and ID type in Own Register or ID type in ID Register
|                            - ESCAN00006063: Undesirable hardware dependency for 
|                                             CAN_HL (CanLL_RxBasicTxIdReceived)
| 2003-09-10 1.22       Ht   - ESCAN00006853: Support V_MEMROM0
|                            - ESCAN00006854: suppress some Compiler warnings
|                            - ESCAN00006856: support CanTask if only Wakeup in polling mode
|                            - ESCAN00006857: variable newDLC not defined in case of Variable DataLen
| 2003-10-14 1.23       Ht   - ESCAN00006858: support BrsTime for internal runtime measurement
|                            - ESCAN00006860: support Conditional Msg Receive
|                            - ESCAN00006865: support "Cancel in HW" with CanTask
|                            - ESCAN00006866: support Direct Tx Objects
|                            - ESCAN00007109: support new memory qualifier for const data and pointer to const
| 2004-01-05 1.24       Ml   - ESCAN00007206: no changes
|                       Ml   - ESCAN00007254: several changes
| 2004-02-06 1.25       Ml   - ESCAN00007281: no changes
|                       Ml   - removed compiler warnings
| 2004-02-21 1.26       Ml   - ESCAN00007670: CAN RAM check
|                       Ml   - ESCAN00007671: no changes
|                       Ml   - ESCAN00007764: no changes
|                       Ml   - ESCAN00007681: no changes
|                       Ml   - ESCAN00007272: no changes
|                       Ml   - ESCAN00008064: encapsulated CanRxHashId in case of array dimension == 0
| 2004-04-16 1.27       Ml   - ESCAN00008204: Optimized CanRxActualId for different ID modes
|                       Ml   - ESCAN00008160: encapsulated functions declared by tool in case of multichannel
|                       Ml   - ESCAN00008266: changed name of parameter of function CanTxGetActHandle
|                       Fz   - ESCAN00008272: Compiler error due to missing array canPollingTaskActive
| 2004-05-10 1.28       Fz   - ESCAN00008328: Compiler error if cancel in hardware is active
|                            - ESCAN00008363: Hole closed when TX in interrupt and cancel in HW is used
|                            - ESCAN00008365: Switch C_ENABLE_APPLCANPREWAKEUP_FCT added
|                            - ESCAN00008391: Wrong parameter macro used in call of 
|                                             CanLL_WakeUpHandling
| 2004-05-24 1.29       Ht   - ESCAN00008441: Interrupt not restored in case of internal error if TX Polling is used
| 2004-09-21 1.30       Ht   - ESCAN00008824: check of reference implementation version added
|                            - ESCAN00008825: No call of ApplCanMsgCancelNotification during CanInit()
|                            - ESCAN00008826: Support asssertions for "Conditional Message Received"  
|                       Ml   - ESCAN00008752: Added function qualifier macros
|                       Ht   - ESCAN00008823: compiler error due to array size 0
|                            - ESCAN00008977: label without instructions
|                            - ESCAN00009485: Message via Normal Tx Object will not be sent  
|                            - ESCAN00009497: support of CommonCAN and RX queue added
| 2004-09-28 1.31       Ht   - ESCAN00009703: unresolved functions CAN_POLLING_IRQ_DISABLE/RESTORE()
| 2004-11-25 1.32       Ht   - move fix for ESCAN00007671 to CAN-LL of DrvCan_MpcToucanHll
|                            - ESCAN00010350: Dynamic Tx messages are send always with Std. ID
|                            - ESCAN00010388: ApplCanMsgConfirmed will only be called if realy transmitted
|                       Ml   - ESCAN00009931: The HardwareLoopCheck should have a channelparameter in multichannel systems.
|                            - ESCAN00010093: lint warning: function type inconsistent "CanCheckMemory"
|                        Ht  - ESCAN00010811: remove Misra and compiler warnings
|                            - ESCAN00010812: support Multi ECU
|                            - ESCAN00010526: CAN interrupts will be disabled accidently
|                            - ESCAN00010584: ECU may crash or behave strange with Rx queue active
| 2005-01-20 1.33       Ht   - ESCAN00010877: ApplCanMsgTransmitConf() is called erronemous
| 2005-03-03 1.34       Ht   - ESCAN00011139: Improvement/Correction of C_ENABLE_MULTI_ECU_CONFIG
|                            - ESCAN00011511: avoid PC-Lint warnings
|                            - ESCAN00011512: copy DLC in case of variable Rx Datalen
|                            - ESCAN00010847: warning due to missing brakets in can_par.c at CanChannelObject
| 2005-05-23 1.35       Ht   - ESCAN00012445: compiler error "V_MEMROMO undefined"in case of multi ECU
|                            - ESCAN00012350: Compiler Error "Illegal token channel"
| 2005-07-06 1.36       Ht   - ESCAN00012153: Compile Error: missing declaration of variable i
|                            - ESCAN00012460: Confirmation of LowLevel message will run into assertion (C_ENABLE_MULTI_ECU_PHYS enabled)
|                            - support Testpoints for CanTestKit
| 2005-07-14 1.37       Ht   - ESCAN00012892: compile error due to missing logTxObjHandle
|                            - ESCAN00012998: Compile Error: missing declaration of txHandle in CanInit()
|                            - support Testpoints for CanTestKit for FullCAN controller
| 2005-10-05 1.38       Ht   - ESCAN00013597: Linker error: Undefined symbol 'CanHL_IndRxHandle'
| 2005-11-10 1.39.00    Ht   - ESCAN00014331: Compile error due to missing 'else' in function CanTransmit
| 2005-04-26 2.00.00    Ht   - ESCAN00016698: support RI1.5
|                            - ESCAN00014770: Cosmic compiler reports truncating assignement
|                            - ESCAN00016191: Compiler warning about unused variable in CanTxTask
| 2007-01-23 2.01.00    Ht   - ESCAN00017279: Usage of SingleGlobalInterruptDisable lead to assertion in OSEK
|                            - ESCAN00017148: Compile error in higher layer due to missing declaration of CanTxMsgHandleToChannel
| 2007-03-14 2.02.00   Ht    - ESCAN00019825: error directives added and misra changes
|                            - ESCAN00019827: adaption to never version of VStdLib.
|                            - ESCAN00019619: V_CALLBACK_1 and V_CALLBACK_2 not defined
|                            - ESCAN00019953: Handling of FullCAN reception in interrupt instead of polling or vice versa.
|                            - ESCAN00019958: CanDynTxObjSetId() or CanDynTxObjSetExtId() will run into assertion
| 2007-03-26 2.03.00  Ht     - ESCAN00019988: Compile warnings in can_drv.c
|                            - ESCAN00018831: polling mode: function prototype without function implemenation (CanRxFullCANTask + CanRxBasicCANTask)
| 2007-04-20 2.04.00  dH     - ESCAN00020299: user assertion fired irregularly due to unknown parameter (in case of CommonCAN)
| 2007-05-02 2.05.00  Ht     - ESCAN00021069: Handling of canStatus improved, usage of preprocessor defines unified
|                            - ESCAN00021070: support relocation of HW objects in case of Multiple configuration
|                            - ESCAN00021166: Compiler Warnings: canHwChannel & canReturnCode not used in CanGetStatus
|                            - ESCAN00021223: CanCanInterruptDisabled called during Sleepmode in CanWakeupTask
|                            - ESCAN00022048: Parameter of ApplCancorruptMailbox is hardware channel instead of logical channel - Error directive added
| 2007-11-12 2.06.00  Ht     - ESCAN00023188: support CAN Controller specific polling sequence for BasicCAN objects
|                            - ESCAN00023208: Compile issue about undefined variable kCanTxQueuePadBits in the CAN driver in Bit based Tx queue
|
|    ************    Version and change information of      **********        
|    ************    high level part only                   **********        
|
|    Please find the version number of the whole module in the previous 
|    File header.
|
|****************************************************************************/

#ifndef  CAN_HL_H
# define  CAN_HL_H


/***************************************************************************/
/* Version                  (abcd: Main version ab Sub Version cd )        */
/***************************************************************************/
/* ##V_CFG_MANAGEMENT ##CQProject : DrvCan_Mcs12xMscanHll CQComponent : Implementation */
# define DRVCAN_MCS12XMSCANHLL_VERSION         0x0507u
# define DRVCAN_MCS12XMSCANHLL_RELEASE_VERSION 0x01u
# define DRVCAN_VERSION                        DRVCAN_MCS12XMSCANHLL_VERSION
# define DRVCAN_RELEASE_VERSION                DRVCAN_MCS12XMSCANHLL_RELEASE_VERSION



/* ##V_CFG_MANAGEMENT ##CQProject : DrvCan__coreHll CQComponent : Implementation */
# define DRVCAN__COREHLL_VERSION                 0x0206
# define DRVCAN__COREHLL_RELEASE_VERSION         0x00

#define DRVCAN__HLLTXQUEUEBIT_VERSION 0x0104
#define DRVCAN__HLLTXQUEUEBIT_RELEASE_VERSION 0x00

/***************************************************************************/
/* Include                                                                 */
/***************************************************************************/

# include "v_def.h"

/***************************************************************************/
/* Defines                                                                 */
/***************************************************************************/

/***************************************************************************/
/* Default switches                                                        */
/* Automatic define settings, depending on user configuration in can_cfg.h */
/***************************************************************************/
# if defined( C_ENABLE_MULTICHANNEL_API )
#  define C_MULTIPLE_RECEIVE_CHANNEL
#  define MULTIPLE_RECEIVE_CHANNEL
# else
#  define C_SINGLE_RECEIVE_CHANNEL
# endif

# if ( kCanNumberOfTxObjects > 0 )
#  if !(defined( C_ENABLE_CAN_TRANSMIT ) || defined( C_DISABLE_CAN_TRANSMIT ))
#   define C_ENABLE_CAN_TRANSMIT
#  endif
# endif

# if !(defined( C_ENABLE_OFFLINE ) || defined( C_DISABLE_OFFLINE ))
#  define C_ENABLE_OFFLINE
# endif

# if !(defined( C_ENABLE_STOP ) || defined( C_DISABLE_STOP ))
#  define C_ENABLE_STOP
# endif

# if !defined( kCanNumberOfHwChannels )
/* compatibility for older tool versions */
#  define kCanNumberOfHwChannels                 kCanNumberOfChannels
# endif




#if !defined(VGEN_ENABLE_VSTDLIB)
# error "This driver needs the Vector Std library!"
#endif

# if defined(C_COMP_COSMIC_MCS12X_MSCAN12) 
#  if defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_SEARCH_XGATE)
#   error "XGate search and Rx polling cannot be combined - Please change search algorithm!"
#  endif
# endif


#define C_HL_ENABLE_OVERRUN_IN_STATUS /* S12 supports an overrun interrupt */

/* MCS12X is supporting a pseudo-hardware ranges filtering, this is performed by the XGate and is transparent for the CPU */
#if defined(C_COMP_COSMIC_MCS12X_MSCAN12) 
# if defined(C_ENABLE_XGATE_FULL_FILTERING)
#  define C_HL_ENABLE_HW_RANGES_FILTER
# else
#  if defined(C_ENABLE_XGATE_USED)
#   define C_HL_ENABLE_HW_RANGES_FILTER
#  else
#   define C_HL_DISABLE_HW_RANGES_FILTER
#  endif
# endif
#endif

#define C_HL_ENABLE_IDTYPE_IN_ID


#if defined C_HL_ENABLE_IDTYPE_IN_ID
/* Specify the ID tables which contain the ID type */

# if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
#  define C_LL_RX_IDTYPE_TABLE      CanRxId0
#  define C_LL_TX_IDTYPE_TABLE      CanTxId0
# endif
#endif

#define C_HL_DISABLE_DUMMY_FCT_CALL


#define C_HL_DISABLE_TX_MSG_DESTROYED


#define C_HL_DISABLE_CANCEL_IN_HW_TASK /* S12 fires an Tx interrupt in case of cancel */


#define C_HL_DISABLE_HW_EXIT_TRANSMIT

#define C_HL_ENABLE_LAST_INIT_OBJ /* S12 driver needs lastinitobject */

#define C_HL_ENABLE_CAN_IRQ_DISABLE

#define C_HL_DISABLE_ADJUST_RXHANDLE


#define C_HL_ENABLE_REJECT_UNWANTED_IDTYPE

#define C_HL_ENABLE_REJECT_REMOTE_FRAME

#if defined(C_COMP_COSMIC_MCS12X_MSCAN12) 
/* If the XGate is used, the canStatus variable is located in the shared RAM */
# if defined(VGEN_GENY)
#  if defined(C_ENABLE_XGATE_USED)
#   define C_HL_DISABLE_COPROCESSOR_SUPPORT
#  else
#   define C_HL_ENABLE_COPROCESSOR_SUPPORT
#  endif
# endif
#endif




# if !defined( C_CALLBACK_1 )
#  define C_CALLBACK_1
# endif

# if !defined( C_CALLBACK_2 )
#  define C_CALLBACK_2
# endif

# if !defined( C_API_1 )
#  define C_API_1
# endif

# if !defined( C_API_2 )
#  define C_API_2
# endif

# if !defined( C_API_3 )
#  define C_API_3                                REENTRANT
# endif

# if !defined( REENTRANT )
#  define REENTRANT
# endif

# if defined( C_ENABLE_TX_POLLING )
#  if !(defined( C_ENABLE_ERROR_POLLING )  || defined( C_DISABLE_ERROR_POLLING ))
#   define C_ENABLE_ERROR_POLLING 
#  endif
# endif

# if !(defined( C_ENABLE_TASK_RECURSION_CHECK ) || defined( C_DISABLE_TASK_RECURSION_CHECK ))
#  define C_ENABLE_TASK_RECURSION_CHECK
# endif


/***************************************************************************/
/* return values                                                           */
/***************************************************************************/

/* return values for precopy-routines */
# define kCanNoCopyData                          ((vuint8)0x00)
# define kCanCopyData                            ((vuint8)0x01)

/* Bitmask of return value of CanGetStatus() */
# define kCanTxOff                               ((vuint8)0x00)
# define kCanStatusInit                          ((vuint8)0x00)
# define kCanTxOn                                ((vuint8)0x01)
# define kCanTxNotOn                             ((vuint8)0xFE)              /* internal use only */
# define kCanHwIsStop                            ((vuint8)0x02)
# define kCanHwIsInit                            ((vuint8)0x04)
# define kCanHwIsInconsistent                    ((vuint8)0x08)              /* used of for common CAN */
# define kCanHwIsWarning                         ((vuint8)0x10)
# define kCanHwIsPassive                         ((vuint8)0x20)
# define kCanHwIsBusOff                          ((vuint8)0x40)
# define kCanHwIsSleep                           ((vuint8)0x80)

/* CanTransmit return values -----------------------------------------------*/
# define kCanTxFailed                            ((vuint8)0x00)  /* Tx path switched off or no sending possible */
# define kCanTxOk                                ((vuint8)0x01)  /* msg transmitted or in queue                 */
# define kCanTxPartOffline                       ((vuint8)0x02)  /* Tx path switched part off or           */
# define kCanCommunicationDisabled               ((vuint8)0x03)  /* if application has disabled com after memory check */

# if defined( C_DRV_INTERNAL )
#  define kCanTxNotify                           ((vuint8)0x05)  /* internal returncode only - not used for the API */
# endif

/* CanGetDynTxObj return values -----------------------------------------------*/
# define kCanNoTxDynObjAvailable                 ((CanTransmitHandle)0xFFFFFFFFU)

/* CanReleaseDynTxObj return values -------------------------------------------*/
# define kCanDynReleased                         ((vuint8)0x00)
# define kCanDynNotReleased                      ((vuint8)0x01)

/* generel return values */
# define kCanFailed                              ((vuint8)0x00)
# define kCanOk                                  ((vuint8)0x01)
# define kCanNotSupported                        ((vuint8)0x02)

# define kCanFalse                               ((vuint8)0x00)
# define kCanTrue                                ((vuint8)0x01)

/* CanTxGetActHandle return value - status of transmit objects */
# define kCanBufferMsgTransmit                   ((CanTransmitHandle)0xFFFFFFFCU)   /* mark a transmit object as used by CanMsgTransmit */

/* results of CAN RAM check */
# define kCanRamCheckFailed                      ((vuint8)0x00)
# define kCanRamCheckOk                          ((vuint8)0x01)
# define kCanDisableCommunication                ((vuint8)0x00)
# define kCanEnableCommunication                 ((vuint8)0x01)


/* return values of CanRxActualIdType */
#if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
# if defined( C_ENABLE_EXTENDED_ID )
#  define kCanIdTypeStd                          0x00000000
#  define kCanIdTypeExt                          0x00180000
# else
#  define kCanIdTypeStd                          0x0000
#  define kCanIdTypeExt                          0x0018
# endif
#endif



/***************************************************************************/
/* parameter values                                                        */
/***************************************************************************/
/* parameters for partial offline */
# define kCanTxPartInit                          ((vuint8)0x00)

/* paramater for assertions */
# define kCanAllChannels                         ((CanChannelHandle)0xFFU)
/* used for generated tables */
# define kCanChannelNotUsed                      ((CanChannelHandle)0xFFU)

/* canRxHandle does not contain a certain message handle or entry in table is not used */
# define kCanRxHandleNotUsed                     ((CanReceiveHandle)  0xFFFFFFFFU)
# define kCanTxHandleNotUsed                     ((CanTransmitHandle) 0xFFFFFFFFU)

# if defined( C_DRV_INTERNAL )
#  define kCanRxHandleRange0                     ((CanReceiveHandle)  0xFFFFFFF0U)
#  define kCanRxHandleRange1                     ((CanReceiveHandle)  0xFFFFFFF1U)
#  define kCanRxHandleRange2                     ((CanReceiveHandle)  0xFFFFFFF2U)
#  define kCanRxHandleRange3                     ((CanReceiveHandle)  0xFFFFFFF3U)

/* entries for generated table CanHwMsgTransmitIndex[], CanHwTxNormalIndex[] - unsed in case of CommonCan */
/* table entry does not contain a HW object assignment */
#  define kCanObjectHandleNotUsed                ((CanObjectHandle)0xFFU)
# endif
/* parameter for ApplCanTimerStart(),-End(), -Loop()*/
/* index has to start with 0, continues numbers have to be used. Gaps are not 
   allowed! */
#define kCanLoopInitReq         ((vuint8)0x01)
#define kCanLoopCanSleep        ((vuint8)0x02)
#define kCanLoopCanWakeup       ((vuint8)0x03)
#define kCanLoopExitInit        ((vuint8)0x04)

/***************************************************************************/
/* macros                                                                  */
/***************************************************************************/

# if defined( C_SINGLE_RECEIVE_CHANNEL ) 
#  define CAN_CHANNEL_CANTYPE_ONLY               void
#  define CAN_CHANNEL_CANTYPE_FIRST
#  define CAN_CHANNEL_CANTYPE_LOCAL         
#  define CAN_CHANNEL_CANPARA_ONLY
#  define CAN_CHANNEL_CANPARA_FIRST
#  define CAN_HW_CHANNEL_CANTYPE_ONLY            void
#  define CAN_HW_CHANNEL_CANTYPE_FIRST
#  define CAN_HW_CHANNEL_CANTYPE_LOCAL
#  define CAN_HW_CHANNEL_CANPARA_ONLY
#  define CAN_HW_CHANNEL_CANPARA_FIRST
# else
#  define CAN_CHANNEL_CANTYPE_ONLY               CanChannelHandle channel
#  define CAN_CHANNEL_CANTYPE_FIRST              CanChannelHandle channel,
#  define CAN_CHANNEL_CANTYPE_LOCAL              CanChannelHandle channel;
#  define CAN_CHANNEL_CANPARA_ONLY               channel
#  define CAN_CHANNEL_CANPARA_FIRST              channel,
#  define CAN_HW_CHANNEL_CANTYPE_ONLY            CanChannelHandle canHwChannel
#  define CAN_HW_CHANNEL_CANTYPE_FIRST           CanChannelHandle canHwChannel,
#  define CAN_HW_CHANNEL_CANTYPE_LOCAL           CanChannelHandle canHwChannel;
#  define CAN_HW_CHANNEL_CANPARA_ONLY            canHwChannel
#  define CAN_HW_CHANNEL_CANPARA_FIRST           canHwChannel,
# endif

# if defined( C_DRV_INTERNAL )
/* macros to fill struct elements RangeMask and RangeCode of type tCanChannelObject - used by generation tool only */
#  if (kCanNumberOfUsedCanRxIdTables == 1)
#   define MK_RX_RANGE_MASK_IDSTD(i)             {MK_RX_RANGE_MASK_IDSTD0(i)}
#   define MK_RX_RANGE_CODE_IDSTD(i)             {MK_RX_RANGE_CODE_IDSTD0(i)}
#  endif
#  if (kCanNumberOfUsedCanRxIdTables == 2)
#   define MK_RX_RANGE_MASK_IDSTD(i)             {MK_RX_RANGE_MASK_IDSTD0(i),MK_RX_RANGE_MASK_IDSTD1(i)}
#   define MK_RX_RANGE_CODE_IDSTD(i)             {MK_RX_RANGE_CODE_IDSTD0(i),MK_RX_RANGE_CODE_IDSTD1(i)}
#  endif
#  if (kCanNumberOfUsedCanRxIdTables == 3)
#   define MK_RX_RANGE_MASK_IDSTD(i)             {MK_RX_RANGE_MASK_IDSTD0(i),MK_RX_RANGE_MASK_IDSTD1(i),MK_RX_RANGE_MASK_IDSTD2(i)}
#   define MK_RX_RANGE_CODE_IDSTD(i)             {MK_RX_RANGE_CODE_IDSTD0(i),MK_RX_RANGE_CODE_IDSTD1(i),MK_RX_RANGE_CODE_IDSTD2(i)}
#  endif
#  if (kCanNumberOfUsedCanRxIdTables == 4)
#   define MK_RX_RANGE_MASK_IDSTD(i)             {MK_RX_RANGE_MASK_IDSTD0(i),MK_RX_RANGE_MASK_IDSTD1(i),MK_RX_RANGE_MASK_IDSTD2(i),MK_RX_RANGE_MASK_IDSTD3(i)}
#   define MK_RX_RANGE_CODE_IDSTD(i)             {MK_RX_RANGE_CODE_IDSTD0(i),MK_RX_RANGE_CODE_IDSTD1(i),MK_RX_RANGE_CODE_IDSTD2(i),MK_RX_RANGE_CODE_IDSTD3(i)}
#  endif
#  if (kCanNumberOfUsedCanRxIdTables == 5)
#   define MK_RX_RANGE_MASK_IDSTD(i)             {MK_RX_RANGE_MASK_IDSTD0(i),MK_RX_RANGE_MASK_IDSTD1(i),MK_RX_RANGE_MASK_IDSTD2(i),MK_RX_RANGE_MASK_IDSTD3(i),MK_RX_RANGE_MASK_IDSTD4(i)}
#   define MK_RX_RANGE_CODE_IDSTD(i)             {MK_RX_RANGE_CODE_IDSTD0(i),MK_RX_RANGE_CODE_IDSTD1(i),MK_RX_RANGE_CODE_IDSTD2(i),MK_RX_RANGE_CODE_IDSTD3(i),MK_RX_RANGE_CODE_IDSTD4(i)}
#  endif

#  if (kCanNumberOfUsedCanRxIdTables == 1)
#   define MK_RX_RANGE_MASK_IDEXT(i)             {MK_RX_RANGE_MASK_IDEXT0(i)}
#   define MK_RX_RANGE_CODE_IDEXT(i)             {MK_RX_RANGE_CODE_IDEXT0(i)}
#  endif
#  if (kCanNumberOfUsedCanRxIdTables == 2)
#   define MK_RX_RANGE_MASK_IDEXT(i)             {MK_RX_RANGE_MASK_IDEXT0(i),MK_RX_RANGE_MASK_IDEXT1(i)}
#   define MK_RX_RANGE_CODE_IDEXT(i)             {MK_RX_RANGE_CODE_IDEXT0(i),MK_RX_RANGE_CODE_IDEXT1(i)}
#  endif
#  if (kCanNumberOfUsedCanRxIdTables == 3)
#   define MK_RX_RANGE_MASK_IDEXT(i)             {MK_RX_RANGE_MASK_IDEXT0(i),MK_RX_RANGE_MASK_IDEXT1(i),MK_RX_RANGE_MASK_IDEXT2(i)}
#   define MK_RX_RANGE_CODE_IDEXT(i)             {MK_RX_RANGE_CODE_IDEXT0(i),MK_RX_RANGE_CODE_IDEXT1(i),MK_RX_RANGE_CODE_IDEXT2(i)}
#  endif
#  if (kCanNumberOfUsedCanRxIdTables == 4)
#   define MK_RX_RANGE_MASK_IDEXT(i)             {MK_RX_RANGE_MASK_IDEXT0(i),MK_RX_RANGE_MASK_IDEXT1(i),MK_RX_RANGE_MASK_IDEXT2(i),MK_RX_RANGE_MASK_IDEXT3(i)}
#   define MK_RX_RANGE_CODE_IDEXT(i)             {MK_RX_RANGE_CODE_IDEXT0(i),MK_RX_RANGE_CODE_IDEXT1(i),MK_RX_RANGE_CODE_IDEXT2(i),MK_RX_RANGE_CODE_IDEXT3(i)}
#  endif
#  if (kCanNumberOfUsedCanRxIdTables == 5)
#   define MK_RX_RANGE_MASK_IDEXT(i)             {MK_RX_RANGE_MASK_IDEXT0(i),MK_RX_RANGE_MASK_IDEXT1(i),MK_RX_RANGE_MASK_IDEXT2(i),MK_RX_RANGE_MASK_IDEXT3(i),MK_RX_RANGE_MASK_IDEXT4(i)}
#   define MK_RX_RANGE_CODE_IDEXT(i)             {MK_RX_RANGE_CODE_IDEXT0(i),MK_RX_RANGE_CODE_IDEXT1(i),MK_RX_RANGE_CODE_IDEXT2(i),MK_RX_RANGE_CODE_IDEXT3(i),MK_RX_RANGE_CODE_IDEXT4(i)}
#  endif
# endif

/* calculate size of cond/ind-flags */
# define kCanNumberOfConfBytes                   ((kCanNumberOfConfFlags + 7)/8)
# define kCanNumberOfConfWords                   ((kCanNumberOfConfFlags + 15)/16)
# define kCanNumberOfConfDWords                  ((kCanNumberOfConfFlags + 31)/32)
# define kCanNumberOfIndBytes                    ((kCanNumberOfIndFlags + 7)/8)
# define kCanNumberOfIndWords                    ((kCanNumberOfIndFlags + 15)/16)
# define kCanNumberOfIndDWords                   ((kCanNumberOfIndFlags + 31)/32)

/* Macros for CAN message access within ApplCanMsgReceived() or PreCopy() function */

# if defined( C_ENABLE_EXTENDED_ID )
#  if defined( C_ENABLE_MIXED_ID )
#   define CanRxActualId(rxStruct)               ((CanRxActualIdType(rxStruct) == kCanIdTypeExt) ?      \
                                                 CanRxActualExtId(rxStruct) : (vuint32)CanRxActualStdId(rxStruct))      /* returns vuint32 */
#  else
#   define CanRxActualId(rxStruct)               (CanRxActualExtId(rxStruct))             /* returns vuint32 */
#  endif
# else
#  define CanRxActualId(rxStruct)                (CanRxActualStdId(rxStruct))             /* returns vuint16 */
# endif

/* Macros for CAN Status */
# define CanHwIsOk(state)                        (((state) & (kCanHwIsWarning |     \
                                                    kCanHwIsPassive |     \
                                                    kCanHwIsBusOff) )    ? 0 : 1)
# define CanHwIsWarning(state)                   (((state) & kCanHwIsWarning)      ? 1 : 0)
# define CanHwIsPassive(state)                   (((state) & kCanHwIsPassive)      ? 1 : 0)
# define CanHwIsBusOff(state)                    (((state) & kCanHwIsBusOff)       ? 1 : 0)
# define CanHwIsWakeup(state)                    (((state) & kCanHwIsSleep)        ? 0 : 1)
# define CanHwIsSleep(state)                     (((state) & kCanHwIsSleep)        ? 1 : 0)
# define CanHwIsStop(state)                      (((state) & kCanHwIsStop)         ? 1 : 0)
# define CanHwIsStart(state)                     (((state) & kCanHwIsStop)         ? 0 : 1)
# define CanIsOnline(state)                      (((state) & kCanTxOn)             ? 1 : 0)
# define CanIsOffline(state)                     (((state) & kCanTxOn)             ? 0 : 1)
# define CanHwIsInconsistent(state)              (((state) & kCanHwIsInconsistent) ? 1 : 0)



#  if defined( C_DRV_INTERNAL )
#   define CanGetTxId0(i)                        (CanTxId0[i])
#   define CanGetTxId1(i)                        (CanTxId1[i])
#   define CanGetTxId2(i)                        (CanTxId2[i])
#   define CanGetTxId3(i)                        (CanTxId3[i])
#   define CanGetTxId4(i)                        (CanTxId4[i])
#   if defined( C_ENABLE_EXTENDED_ID )
#    if defined( C_ENABLE_MIXED_ID )
#     if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#      define CanGetTxIdType(i)                  (C_LL_TX_IDTYPE_TABLE[i] & kCanIdTypeExt)
#     else
#       define CanGetTxIdType(i)                 (CanTxIdType[i] & kCanIdTypeExt)
#     endif
#    else
#     define CanGetTxIdType(i)                   (kCanIdTypeExt)
#    endif
#   else
#    define CanGetTxIdType(i)                    (kCanIdTypeStd)
#   endif
#  endif /* C_DRV_INTERNAL */

#  define CanGetTxDlc(i)                         (CanTxDLC[i])
#  define CanGetTxDataPtr(i)                     (CanTxDataPtr[i])
#  define CanGetConfirmationOffset(i)            (CanConfirmationOffset[i])
#  define CanGetConfirmationMask(i)              (CanConfirmationMask[i])
#   define CanGetTxHwObj(i)                      (CanTxHwObj[i])
#  define CanGetTxSendMask(i)                    (CanTxSendMask[i])
#  define CanGetApplPreTransmitPtr(i)            (CanTxApplPreTransmitPtr[i])
#  define CanGetApplConfirmationPtr(i)           (CanTxApplConfirmationPtr[i])
#  define CanGetChannelOfTxObj(i)                (CanTxMsgHandleToChannel[i])

#  if defined( C_DRV_INTERNAL )
#   define CanGetRxId0(i)                        (CanRxId0[i])
#   define CanGetRxId1(i)                        (CanRxId1[i])
#   define CanGetRxId2(i)                        (CanRxId2[i])
#   define CanGetRxId3(i)                        (CanRxId3[i])
#   define CanGetRxId4(i)                        (CanRxId4[i])
#   if defined( C_ENABLE_EXTENDED_ID )
#    if defined( C_ENABLE_MIXED_ID )
#     if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#      define CanGetRxIdType(i)                  (C_LL_RX_IDTYPE_TABLE[i] & kCanIdTypeExt)
#     else
#      define CanGetRxIdType(i)                  (CanRxIdType[i] & kCanIdTypeExt)
#     endif
#    else
#     define CanGetRxIdType(i)                   (kCanIdTypeExt)
#    endif
#   else
#    define CanGetRxIdType(i)                    (kCanIdTypeStd)
#   endif
#  endif /* C_DRV_INTERNAL */

#  define CanGetRxDataLen(i)                     (CanRxDataLen[i])
#  define CanGetRxMinDataLen(i)                  (CanRxMinDataLen[i])
#  define CanGetRxDataPtr(i)                     (CanRxDataPtr[i])
#  define CanGetIndicationOffset(i)              (CanIndicationOffset[i])
#  define CanGetIndicationMask(i)                (CanIndicationMask[i])
#  define CanGetApplPrecopyPtr(i)                (CanRxApplPrecopyPtr[i])
#  define CanGetApplIndicationPtr(i)             (CanRxApplIndicationPtr[i])

# if defined( C_ENABLE_CONFIRMATION_FCT ) && \
    defined( C_ENABLE_DYN_TX_OBJECTS )  && \
    defined( C_ENABLE_TRANSMIT_QUEUE )
  /* set confirmation active for CanReleaseDynTxObj() */
#  if defined( C_SINGLE_RECEIVE_CHANNEL ) 
#   define CanConfirmStart(txHandle)             {confirmHandle[0] = (txHandle);}
#   define CanConfirmEnd()                       {confirmHandle[0] = kCanTxHandleNotUsed;}
#  else
#   define CanConfirmStart(channel, txHandle)    {confirmHandle[channel] = (txHandle);}
#   define CanConfirmEnd(channel)                {confirmHandle[channel] = kCanTxHandleNotUsed;}
#  endif
# endif

# if defined( C_ENABLE_PART_OFFLINE )
#  if defined( C_SINGLE_RECEIVE_CHANNEL ) 
#   define CanPartOffline(sendGroup)             (CanSetPartOffline( sendGroup))
#   define CanPartOnline(sendGroup)              (CanSetPartOnline( (vuint8)~(vuint8)(sendGroup)) )
#  else
#   define CanPartOffline(channel, sendGroup)    (CanSetPartOffline((channel), (sendGroup)))
#   define CanPartOnline(channel,  sendGroup)    (CanSetPartOnline((channel), (vuint8)~(vuint8)(sendGroup)) )
#  endif
# else
#  if defined( C_SINGLE_RECEIVE_CHANNEL ) 
#   define CanPartOffline(sendGroup)
#   define CanPartOnline(sendGroup)
#  else
#   define CanPartOffline(channel, sendGroup)
#   define CanPartOnline(channel, sendGroup)
#  endif
# endif


/* provide interface for interrupt disable and restore functions */
# define CanGlobalInterruptDisable               VStdSuspendAllInterrupts
# define CanGlobalInterruptRestore               VStdResumeAllInterrupts

# if ( C_SECURITY_LEVEL == 0 )
#  define CanDeclareGlobalInterruptOldStatus
#  define CanPutGlobalInterruptOldStatus(x)
#  define CanGetGlobalInterruptOldStatus(x)
#  define CanNestedGlobalInterruptDisable()
#  define CanNestedGlobalInterruptRestore()
# else  /* C_SECURITY_LEVEL == 0 */
#  define CanDeclareGlobalInterruptOldStatus     VStdDeclareGlobalInterruptOldStatus
#  define CanPutGlobalInterruptOldStatus(x)      VStdPutGlobalInterruptOldStatus(x)
#  define CanGetGlobalInterruptOldStatus(x)      VStdGetGlobalInterruptOldStatus(x)
#  if (VSTDLIB__COREHLL_VERSION  <  0x0216 )
#   define CanNestedGlobalInterruptDisable()      VStdNestedGlobalInterruptDisable()
#   define CanNestedGlobalInterruptRestore()      VStdNestedGlobalInterruptRestore()
#  else
#   define CanNestedGlobalInterruptDisable()      VStdGlobalInterruptDisable()
#   define CanNestedGlobalInterruptRestore()      VStdGlobalInterruptRestore()
#  endif
# endif /* C_SECURITY_LEVEL == 0 */


/* provide support for multiple callbacks */
#if !defined( APPL_CAN_GENERIC_PRECOPY )
# define APPL_CAN_GENERIC_PRECOPY                          ApplCanGenericPrecopy
#endif
#if !defined( APPL_CAN_TX_CONFIRMATION )
# define APPL_CAN_TX_CONFIRMATION                          ApplCanTxConfirmation
#endif
#if !defined( APPL_CAN_MSGTRANSMITCONF )
# define APPL_CAN_MSGTRANSMITCONF                          ApplCanMsgTransmitConf
#endif
#if !defined( APPL_CAN_MSGTRANSMITINIT )
# define APPL_CAN_MSGTRANSMITINIT                          ApplCanMsgTransmitInit
#endif
#if !defined( APPL_CAN_ONLINE )
# define APPL_CAN_ONLINE                                   ApplCanOnline
#endif
#if !defined( APPL_CAN_OFFLINE )
# define APPL_CAN_OFFLINE                                  ApplCanOffline
#endif
#if !defined( APPL_CAN_MSGRECEIVED )
# define APPL_CAN_MSGRECEIVED                              ApplCanMsgReceived
#endif
#if !defined( APPL_CAN_CANCELNOTIFICATION )
# define APPL_CAN_CANCELNOTIFICATION                       ApplCanCancelNotification
#endif
#if !defined( APPL_CAN_MSGCANCELNOTIFICATION )
# define APPL_CAN_MSGCANCELNOTIFICATION                    ApplCanMsgCancelNotification
#endif

# if defined( C_ENABLE_VARIABLE_DLC )
/* mask to keep some bits unchanged in the DLC for use with variable DLC */
/*  - bit remaining unchanged should be masked with 1 e.g. id type or direction */
#define CanLL_DlcMask            ((vuint8)0x00)  /* DLC register contains no more bits than the dlc itself */
#endif


/* macros to set DLC and ID in rx-,tx-structures ---------------------------*/
/* MK... macros are used by the generation tool to fill the rx and tx structures */
/* In case the IDE bit is located in the ID registers, this bit has to be handled as must match.
   In general, this is done by setting the IDE bit to 0 in the .._MASK_.. macros.
*/

/* RANGE macros might depend on C_xxx_RANGE_EXTENDED_ID */
# if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
#  define MK_RX_RANGE_MASK_IDSTD0(id)    (MK_STDID0(id))
#  define MK_RX_RANGE_CODE_IDSTD0(id)    (MK_STDID0(id))
#  define MK_RX_RANGE_MASK_IDEXT0(id)    (MK_EXTID0(id) & (~0x00180000))
#  define MK_RX_RANGE_CODE_IDEXT0(id)    (MK_EXTID0(id))
# endif





# if defined(C_ENABLE_EXTENDED_ID)
#  define MK_STDID(id)             ((vuint32)(((vuint32)(id)) << 21))
# else
#  define MK_STDID(id)             ((vuint16)((vuint16)(id) << 5))
# endif

#define MK_STDID_HI(id)          ((vuint16)(((vuint16)(id)) << 5))
#define MK_STDID_LO(id)          ((vuint16)0)


# if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
#  define MK_STDID0(id)            MK_STDID(id)
# endif



#define MK_EXTID(id)            ((vuint32)((((vuint32)(id) << 3) & (vuint32)0xFFE00000) | (((vuint32)(id) << 1) &  (vuint32)0x0007FFFE) | (vuint32)0x00180000))
#define MK_EXTID_HI(id)         ((vuint16)(MK_EXTID(id) >> 16))
#define MK_EXTID_LO(id)         ((vuint16)(MK_EXTID(id) & (vuint32)0x0000FFFF))


# if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
#  define MK_EXTID0(id)            MK_EXTID(id)
# endif



/* return lowest 8 bit of the logical ID */
/* depends on EXTENDED_ID and RANGE_EXTENDED_ID */
/* number of parameter dependes on kCanNumberOfTxIdTables */



#define MK_TX_DLC(dlc)       ((vuint8)((dlc) & (vuint8)0x0F))
#define MK_TX_DLC_EXT(dlc)   ((vuint8)((dlc) & (vuint8)0x0F))
#define XT_TX_DLC(dlc)       ((vuint8)((dlc) & (vuint8)0x0F))


#if defined( C_SINGLE_RECEIVE_CHANNEL )
/* Bus-Off functions */
# define CanResetBusOffStart(a)  (CanInit(a))
# define CanResetBusOffEnd(a)

/* Bus-Sleep functions */
# define CanResetBusSleep(a)     (CanInit(a))

#else
/* Bus-Off functions */
# define CanResetBusOffStart(ch,a)  (CanInit(ch,a))
# define CanResetBusOffEnd(ch,a)
  
/* Bus-Sleep functions */
# define CanResetBusSleep(ch,a)     (CanInit(ch,a))
#endif

/* Macros for CAN message access within ApplCanMsgReceived() or PreCopy() function */

/* The macros CanRxActualIdRawx() have to provide the register context of the ID registers.
   It is not necessary to mask any bits. These macros are for internal use only and masking
   will be done in can_hls.c
*/
#if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
    
# define CanIdDWord(rxStruct)             ((vuint32)*((vuint32*)((rxStruct) -> pChipMsgObj)))
# define CanHiIdWord(rxStruct)            ((vuint16)*((vuint16*)((rxStruct) -> pChipMsgObj)))
# define CanLoIdWord(rxStruct)            ((vuint16)*((vuint16*)(((rxStruct) -> pChipMsgObj) + 2)))
# define CanHiIdByte(rxStruct)            ((vuint8)*((vuint8*)((rxStruct) -> pChipMsgObj)))
# define CanMidHiIdByte(rxStruct)         ((vuint8)*((vuint8*)(((rxStruct) -> pChipMsgObj) + 1)))
# define CanMidLoIdByte(rxStruct)         ((vuint8)*((vuint8*)(((rxStruct) -> pChipMsgObj) + 2)))
# define CanLoIdByte(rxStruct)            ((vuint8)*((vuint8*)(((rxStruct) -> pChipMsgObj) + 3)))

# if defined( C_ENABLE_EXTENDED_ID )
#  define CanRxActualIdRaw0(rxStruct)     CanIdDWord(rxStruct)
#  define CanRxActualIdExtHi(rxStruct)    ((vuint8)  (CanHiIdByte(rxStruct) >> 3))
#  define CanRxActualIdExtMidHi(rxStruct) ((vuint8)(((CanHiIdWord(rxStruct) & 0x07E0) >> 3) | ((CanHiIdWord(rxStruct) & 0x0007) >> 1)))
#  define CanRxActualIdExtMidLo(rxStruct) ((vuint8) ((CanMidHiIdByte(rxStruct) << 7)        | (CanMidLoIdByte(rxStruct) >> 1)))
#  define CanRxActualIdExtLo(rxStruct)    ((vuint8) ((CanMidLoIdByte(rxStruct) << 7)        | (CanLoIdByte(rxStruct) >> 1)))
# else
#  define CanRxActualIdRaw0(rxStruct)     CanHiIdWord(rxStruct)
# endif

# define CanRxActualExtId(rxStruct)       ((vuint32)(((CanIdDWord(rxStruct) & 0xFFE00000) >> 3) | ((CanIdDWord(rxStruct) & 0x0007FFFE) >> 1)))
# define CanRxActualStdId(rxStruct)       ((vuint16)(CanHiIdWord(rxStruct) & 0xFFE0) >> 5)

# define CanRxActualData(rxStruct,i)      ((vuint8)*((vuint8*)(((rxStruct) -> pChipData) + (i))))
# define CanRxActualDLC(rxStruct)         ((*(vuint8*)((vuint8*)(rxStruct) -> pChipMsgObj + 0xC)) & 0x0F)

# if defined( C_ENABLE_EXTENDED_ID )
#  define CanRxActualIdType(rxStruct)     ((*(vuint32*)(rxStruct) -> pChipMsgObj) & kCanIdTypeExt)
# else
#  define CanRxActualIdType(rxStruct)     ((*(vuint16*)(rxStruct) -> pChipMsgObj) & kCanIdTypeExt)
# endif

# if defined (C_SINGLE_RECEIVE_CHANNEL )
#  define CanTxActualErrorCounter()       (*(vuint8*)(V_REG_BLOCK_ADR + kCanBasisAdr + (vuint8)0x0F))
#  define CanRxActualErrorCounter()       (*(vuint8*)(V_REG_BLOCK_ADR + kCanBasisAdr + (vuint8)0x0E))
# else
#  define CanTxActualErrorCounter(ch)     (*(vuint8*)(V_REG_BLOCK_ADR + CanBasisAdr[ch] + (vuint8)0x0F))
#  define CanRxActualErrorCounter(ch)     (*(vuint8*)(V_REG_BLOCK_ADR + CanBasisAdr[ch] + (vuint8)0x0E))
# endif
#endif







/* Macros for CAN message access within PreTransmit() function */
# if defined( C_ENABLE_EXTENDED_ID )
#  define CanTxWriteActId(txStruct, id)    *((vuint32 MEMORY_CAN*)(txStruct.pChipData - 0x04)) = MK_STDID(id)
#  define CanTxWriteActExtId(txStruct, id) *((vuint32 MEMORY_CAN*)(txStruct.pChipData - 0x04)) = MK_EXTID(id)
# else
#  define CanTxWriteActId(txStruct,id)     *((vuint16 MEMORY_CAN*)(txStruct.pChipData - 0x04)) = MK_STDID(id)
# endif
# define CanTxWriteActDLC(txStruct,dlc)    (*(vuint8 MEMORY_CAN*)((txStruct.pChipData) + 0x08) = MK_TX_DLC(dlc))

/* Macros to fill variables of the type tCanMsgTransmitStruct */
# define CanMsgTransmitSetStdId(pCanMsgTransmitStruct, id)               {(pCanMsgTransmitStruct)->IdRaw = MK_STDID_HI((id));  \
                                                                         (pCanMsgTransmitStruct)->IdRawExt = MK_STDID_LO((id));}
# define CanMsgTransmitSetExtId(pCanMsgTransmitStruct, id)               {(pCanMsgTransmitStruct)->IdRaw = MK_EXTID_HI((id));  \
                                                                         (pCanMsgTransmitStruct)->IdRawExt = MK_EXTID_LO((id));}
# define CanMsgTransmitSetDlc(pCanMsgTransmitStruct, dlc)                {(pCanMsgTransmitStruct)->DlcRaw = MK_TX_DLC((dlc));}
# define CanMsgTransmitSetData(pCanMsgTransmitStruct, dataByte, msgData) {(pCanMsgTransmitStruct)->DataFld[dataByte] = (msgData);}


/* macros for buffer access */
#define StartRxReadSync()          (VStdSuspendAllInterrupts())
#define EndRxReadSync()            (VStdResumeAllInterrupts())

#define StartRxWriteSync()         (VStdSuspendAllInterrupts())
#define EndRxWriteSync()           (VStdResumeAllInterrupts())

#define StartTxReadSync()          (VStdSuspendAllInterrupts())
#define EndTxReadSync()            (VStdResumeAllInterrupts())

#define StartTxWriteSync()         (VStdSuspendAllInterrupts())
#define EndTxWriteSync()           (VStdResumeAllInterrupts())

#define StartRxWriteCANSync()      (VStdSuspendAllInterrupts())
#define EndRxWriteCANSync()        (VStdResumeAllInterrupts())

#define StartRxReadCANSync()       (VStdSuspendAllInterrupts())
#define EndRxReadCANSync()         (VStdResumeAllInterrupts())

#define StartRxFullCANReadSync(Obj)        /* not used */
#define EndRxFullCANReadSync(Obj)          /* not used */

#define StartRxFullCANWriteSync(obj)       /* not used */
#define EndRxFullCANWriteSync(obj)         /* not used */

/* macros for flag access */
#define CanStartFlagWriteSync()    (VStdSuspendAllInterrupts())
#define CanEndFlagWriteSync()      (VStdResumeAllInterrupts())



/***************************************************************************/
/* error codes                                                             */
/***************************************************************************/
/* error numbers for User Assertions 0x00-0x1f - hardware independed */
# define kErrorTxDlcTooLarge                     ((vuint8)0x01)
# define kErrorTxHdlTooLarge                     ((vuint8)0x02)
# define kErrorIntRestoreTooOften                ((vuint8)0x03)
# define kErrorIntDisableTooOften                ((vuint8)0x04)
# define kErrorChannelHdlTooLarge                ((vuint8)0x05)
# define kErrorInitObjectHdlTooLarge             ((vuint8)0x06)
# define kErrorTxHwHdlTooLarge                   ((vuint8)0x07)
# define kErrorHwObjNotInPolling                 ((vuint8)0x08)
# define kErrorHwHdlTooSmall                     ((vuint8)0x09)
# define kErrorHwHdlTooLarge                     ((vuint8)0x0A)

# define kErrorAccessedInvalidDynObj             ((vuint8)0x0B)
# define kErrorAccessedStatObjAsDyn              ((vuint8)0x0C)
# define kErrorDynObjReleased                    ((vuint8)0x0D)

# define kErrorPollingTaskRecursion              ((vuint8)0x0E)
# define kErrorDisabledChannel                   ((vuint8)0x0F)
# define kErrorDisabledTxMessage                 ((vuint8)0x10)
# define kErrorDisabledCanInt                    ((vuint8)0x11)

# define kErrorCanSleep                          ((vuint8)0x12)
# define kErrorCanOnline                         ((vuint8)0x13)
# define kErrorCanStop                           ((vuint8)0x14)
# define kErrorWrongMask                         ((vuint8)0x15)
# define kErrorWrongId                           ((vuint8)0x16)
/* error numbers for User Assertions 0x20-0x3f - hardware depended */


/* error numbers for Gentool Assertions 0x40-0x5f */
# define kErrorTxROMDLCTooLarge                  ((vuint8)0x40)
# define kErrorWrongHwTxObjHandle                ((vuint8)0x41)

/* error numbers for Gentool Assertions 0x60-0x7f - hardware depended */


/* error numbers for Hardware Assertions 0x80-0x9f */
# define kErrorTxBufferBusy                      ((vuint8)0x80)

/* error numbers for Hardware Assertions 0xa0-0xbf - hardware depended */


/* error numbers for Internal Assertions 0xc0-0xdf */
# define kErrorTxHandleWrong                     ((vuint8)0xC0)
# define kErrorInternalTxHdlTooLarge             ((vuint8)0xC1)
# define kErrorRxHandleWrong                     ((vuint8)0xC2)        
# define kErrorTxObjHandleWrong                  ((vuint8)0xC3)
# define kErrorReleasedUnusedDynObj              ((vuint8)0xC4)
# define kErrorTxQueueTooManyHandle              ((vuint8)0xC5)
# define kErrorInternalChannelHdlTooLarge        ((vuint8)0xC6)
# define kErrorInternalDisabledChannel           ((vuint8)0xC7)
# define kErrorInternalDisabledTxMessage         ((vuint8)0xC8)

/* error numbers for Internal Assertions 0xe0-0xff - hardware depended */


/***************************************************************************/
/* DummyFunction                                                           */
/***************************************************************************/
# if defined( C_DRV_INTERNAL )
#  define CAN_RECEIVE_FCT_DUMMY                  ApplCanRxStructPtrDummy
#  define CAN_RANGE_FCT_DUMMY                    ApplCanRxStructPtrDummy
#  define CAN_BUSOFF_FCT_DUMMY                   ApplCanChannelDummy
#   define CAN_INDICATION_FCT_DUMMY              V_NULL
#   define CAN_PRECOPY_FCT_DUMMY                 V_NULL
#   define CAN_CONFIRMATION_FCT_DUMMY            V_NULL
#   define CAN_PRETRANSMIT_FCT_DUMMY             V_NULL
# endif

# if defined( C_NO_COMPATIBILITY )
# else
/***************************************************************************/
/* compatibility defines                                                   */
/***************************************************************************/

#  define CAN_TX_DLC(i)                          (CanGetTxDlc(i))            
#  define CAN_TX_DATA_PTR(i)                     (CanGetTxDataPtr(i))         
#  define CAN_CONFIRMATION_OFFSET(i)             (CanGetConfirmationOffset(i))
#  define CAN_CONFIRMATION_MASK(i)               (CanGetConfirmationMask(i))

#  define CAN_RX_DATA_LEN(i)                     (CanGetRxDataLen(i))
#  define CAN_RX_DATA_PTR(i)                     (CanGetRxDataPtr(i))
#  define CAN_INDICATION_OFFSET(i)               (CanGetIndicationOffset(i))
#  define CAN_INDICATION_MASK(i)                 (CanGetIndicationMask(i))

#  define CanInterruptDisable()                  (VStdSuspendAllInterrupts())
#  define CanInterruptRestore()                  (VStdResumeAllInterrupts())

#  define ApplCanReceiveDummy0                   ApplCanRxStructPtrDummy
#  define ApplCanReceiveDummy1                   ApplCanChannelDummy

#  define CanGetTxDirectMsg                      (CanGetTxHwObj)

#  define canRxDlcRam                            (canVariableRxDataLen)

#  define CAN_START_INDEX_INIT_OBJ(i)            (CanInitObjectStartIndex[i])
#  define CAN_START_INDEX_TX_OBJ(i)              (CanTxStartIndex[i])
#  define CAN_START_INDEX_RX_OBJ(i)              (CanRxStartIndex[i])

#  if !defined( VGEN_GENY )
#  endif


/***************************************************************************/
/* end of compatibility defines                                            */
/***************************************************************************/
# endif /* C_NO_COMPATIBILITY */

/***************************************************************************/
/* Memory qualifier                                                        */
/***************************************************************************/
/* memory qualifier for the CAN controller registers */
#define MEMORY_CAN /* empty */

/***************************************************************************/
/* data types                                                              */
/***************************************************************************/

/* data types for driver function parameters */
/* the types of CanInitHandle, CanChannelHandle and CanObjectHandle depends
   on the compiler and controller - what generates effective code */
typedef vuint16           CanInitHandle;
typedef vuint16           CanChannelHandle;
/* CanObjectHandle has to be unsigned! */
#if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
typedef vuint16           CanObjectHandle;
#endif



# if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
typedef vuint16           CanTransmitHandle;
typedef vuint16           CanReceiveHandle;
# endif


/* define datatype of local signed variables for message handles */
#if defined( C_CPUTYPE_8BIT ) && (kCanNumberOfTxObjects > 128)
typedef vsint16 CanSignedTxHandle;
#else
typedef vsintx CanSignedTxHandle;
#endif

#if defined( C_CPUTYPE_8BIT ) && (kCanNumberOfRxObjects > 128)
typedef vsint16 CanSignedRxHandle;
#else
# if defined( C_CPUTYPE_8BIT ) && defined ( C_SEARCH_HASH )
#  if (((kHashSearchListCountEx + kHashSearchListCountEx - 1) > 128) ||\
       ((kHashSearchListCount   + kHashSearchListCount   - 1) > 128) )
typedef vsint16 CanSignedRxHandle;
#  else
typedef vsintx CanSignedRxHandle;
#  endif
# else
typedef vsintx CanSignedRxHandle;
# endif
#endif

/* ptr to msgObj & data on chip used in precopy function */
typedef volatile vuint8       MEMORY_CAN *  CanChipDataPtr;
typedef volatile vuint8       MEMORY_CAN *  CanChipMsgPtr;

/* up to 5 tables can be defined for the transmission ID. The type and how 
   many tables are needed depends on the CPU bus type and on the ID registers 
   of the CAN controller. E.g. a CPU with a 16-Bit bus and 2 byte ID registers 
   for standard ID needs 1 table and tCanRxId0 of type vuint16. The same CPU
   with 5 byte ID registers for extended ID needs in this case 3 tables and
   tCanRxId0 = vuint16, tCanRxId1 = vuint16 and tCanRxId2 = vuint16. */

#if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
# if defined( C_ENABLE_EXTENDED_ID )
typedef vuint32   tCanTxId0;
# else
typedef vuint16   tCanTxId0;
# endif
#endif



/* type of variable for ID type kCanIdTypeExt or kCanIdTypeStd will be assigned to 
   a variable of this type */
#if defined( C_ENABLE_EXTENDED_ID )
typedef vuint32   tCanIdType;
#else
typedef vuint16   tCanIdType;
#endif


/* up to 3 tables can be defined for the reception ID. The type and how many
   tables are needed depends on the CPU bus type and on the ID registers of
   the CAN controller. E.g. a CPU with a 16-Bit bus and 2 byte ID registers 
   for standard ID needs 1 table and tCanRxId0 of type vuint16. The same CPU
   with 5 byte ID registers for extended ID needs in this case 3 tables and 
   tCanRxId0 = vuint16, tCanRxId1 = vuint16 and tCanRxId2 = vuint16. */

# if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
#  if defined C_ENABLE_EXTENDED_ID
typedef vuint32   tCanRxId0;
#  else
typedef vuint16   tCanRxId0;
#  endif
# endif


/* this type could also be a struct if more than one variable is necessary */
/* typedef vuint16         tCanLLCanIntOld; */

/* stucture of the CAN message object used by CanMsgTransmit() and RxQueue*/
# if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
typedef struct
{
  vuint16     IdRaw;
  vuint16     IdRawExt;
  vuint8      DataFld[8];
  vuint8      DlcRaw;
  vuint8      MsgPrio;
}
tCanMsgTransmitStruct;
#endif

typedef struct 
{
  vuint8 CanInitCTL0;       /* Control Register 0         */
  vuint8 CanInitCTL1;       /* Control Register 1         */
  vuint8 CanInitCBTR0;      /* Bus Timing 0               */
  vuint8 CanInitCBTR1;      /* Bus Timing 1               */
  vuint8 CanInitCRIER;      /* Receiver interrupt enable register */
  vuint8 CanInitCIDAC;      /* Acceptance Control Register  */
  vuint8 CanInitCIDAR0;     /* ID acceptance register     */
  vuint8 CanInitCIDAR1;     /* ID acceptance register     */
  vuint8 CanInitCIDAR2;     /* ID acceptance register     */
  vuint8 CanInitCIDAR3;     /* ID acceptance register     */
  vuint8 CanInitCIDMR0;     /* ID mask register           */
  vuint8 CanInitCIDMR1;     /* ID mask register           */
  vuint8 CanInitCIDMR2;     /* ID mask register           */
  vuint8 CanInitCIDMR3;     /* ID mask register           */
  vuint8 CanInitCIDAR4;     /* ID acceptance register     */
  vuint8 CanInitCIDAR5;     /* ID acceptance register     */
  vuint8 CanInitCIDAR6;     /* ID acceptance register     */  
  vuint8 CanInitCIDAR7;     /* ID acceptance register     */
  vuint8 CanInitCIDMR4;     /* ID mask register           */
  vuint8 CanInitCIDMR5;     /* ID mask register           */
  vuint8 CanInitCIDMR6;     /* ID mask register           */
  vuint8 CanInitCIDMR7;     /* ID mask register           */
} tCanInitObject;

# if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
typedef volatile struct
{
# if defined(C_ENABLE_EXTENDED_ID)
  vuint32 Id;             /* complete ID                                     */
# else
  vuint16 Id;             /* complete ID                                     */
  vuint16 IdEx;           /* Extended ID bytes; not used                     */
# endif
  vuint8  DataFld[8];     /* Data 0 .. 7                                     */
  vuint8  Dlc;            /* Data length reg.:  X X X X DLC3 DLC2 DLC1 DLC0  */
  vuint8  Prio;           /* TxBuf priority reg.                             */
  vuint8  Unused[2];
} 
tMsgObject;
# endif



#if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
typedef volatile struct
{
  vuint8      CanCTL0;         /* RXFRM RXACT  CSWAI   SYNCH   TIME    WUPE    SLPRQ  INITRQ  */
  vuint8      CanCTL1;         /* CANE  CLKSRC LOOPB   LISTEN  0       WUPM    SLPAK  INITAK  */
  vuint8      CanCBTR0;        /* SJW1  SJW0   BRP5    BRP4    BRP3    BRP2    BRP1   BRP0    */
  vuint8      CanCBTR1;        /* SAMP  TSEG22 TSEG21  TSEG20  TSEG13  TSEG12  TSEG11 TSEG10  */
  vuint8      CanCRFLG;        /* WUPIF CSCIF  RSTAT1  RSTAT0  TSTAT1  TSTAT0  OVRIF  RXF     */
  vuint8      CanCRIER;        /* WUPIE CSCIF  RSTATE1 RSTATE0 TSTATE1 TSTATE0 OVRIE  RXE     */
  vuint8      CanCTFLG;        /* 0     0     0       0       0       TXE2    TXE1   TXE0     */
  vuint8      CanCTIER;        /* 0     0     0       0       0       TXEIE2  TXEIE1 TXEIE0   */
  vuint8      CanCTARQ;        /* 0     0     0       0       0       ABTRQ2  ABTRQ1 ABTRQ0   */
  vuint8      CanCTAAK;        /* 0     0     0       0       0       ABTAK2  ABTAK1 ABTAK0   */
  vuint8      CanCTBSEL;       /* 0     0     0       0       0       TXE2    TXE1   TXE0     */
  vuint8      CanCIDAC;        /* 0     0     IDAM1   IDAM0   0       0       IDHIT1 IDHIT0   */
  vuint8      Reserved1;
  vuint8      CanCMISC;        /* 0     0     0       0       0       0       0      BOHOLD   */
  vuint8      CanCRXERR;       /* RXERR7 ... RXERR0                          */
  vuint8      CanCTXERR;       /* TXERR7 ... TXERR0                          */
  vuint8      CanCIDAR0;       /* Filter Masks 0..3                          */
  vuint8      CanCIDAR1;
  vuint8      CanCIDAR2;
  vuint8      CanCIDAR3;
  vuint8      CanCIDMR0;
  vuint8      CanCIDMR1;
  vuint8      CanCIDMR2;
  vuint8      CanCIDMR3;
  vuint8      CanCIDAR4;       /* Filter Masks 4..7 */
  vuint8      CanCIDAR5;
  vuint8      CanCIDAR6;
  vuint8      CanCIDAR7;
  vuint8      CanCIDMR4;
  vuint8      CanCIDMR5;
  vuint8      CanCIDMR6;
  vuint8      CanCIDMR7;
  tMsgObject  CanRxBuf;        /* Rx-Msg Object */
  tMsgObject  CanTxBuf;        /* Tx-Msg Objects */
} 
tCanCntrlRegBlock;
#endif



typedef struct 
{
  CanChipDataPtr     pChipData;
  CanTransmitHandle  Handle;
/* CPU-specific part */
} tCanTxInfoStruct;

/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
typedef struct 
{
  CanChannelHandle   Channel;
  CanTransmitHandle  Handle;
/* CPU-specific part */
  CanChipMsgPtr     pChipMsgObj;
  CanChipDataPtr    pChipData;
} tCanTxConfInfoStruct;

typedef struct 
{
  CanChannelHandle  Channel;
  CanChipMsgPtr     pChipMsgObj;
  CanChipDataPtr    pChipData;
  CanReceiveHandle  Handle;
/* CPU-specific part */
} tCanRxInfoStruct;

typedef tCanRxInfoStruct          *CanRxInfoStructPtr;
typedef tCanTxInfoStruct          CanTxInfoStruct;
/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
typedef tCanTxConfInfoStruct      *CanTxInfoStructPtr;

/* types of application functions called by driver ---------------------------*/
typedef C_CALLBACK_1 vuint8   (C_CALLBACK_2 *ApplCanMsgRcvFct)    (CanRxInfoStructPtr rxStruct);
typedef C_CALLBACK_1 vuint8   (C_CALLBACK_2 *ApplRangeFct)        (CanRxInfoStructPtr rxStruct);
typedef C_CALLBACK_1 vuint8   (C_CALLBACK_2 *ApplPrecopyFct)      (CanRxInfoStructPtr rxStruct);
typedef C_CALLBACK_1 void     (C_CALLBACK_2 *ApplIndicationFct)   (CanReceiveHandle rxObject);
typedef C_CALLBACK_1 void     (C_CALLBACK_2 *ApplConfirmationFct) (CanTransmitHandle txObject);
typedef C_CALLBACK_1 vuint8   (C_CALLBACK_2 *ApplPreTransmitFct)  (CanTxInfoStruct txStruct);
typedef C_CALLBACK_1 void     (C_CALLBACK_2 *ApplChannelFct)      (CAN_CHANNEL_CANTYPE_ONLY);

typedef struct
{
  tCanRxId0           Id0;
# if (kCanNumberOfUsedCanRxIdTables > 1)
  tCanRxId1           Id1;
# endif
# if (kCanNumberOfUsedCanRxIdTables > 2)
  tCanRxId2           Id2;
# endif     
# if (kCanNumberOfUsedCanRxIdTables > 3)
  tCanRxId3           Id3;
# endif
# if (kCanNumberOfUsedCanRxIdTables > 4)
  tCanRxId4           Id4;
# endif
} tCanStructRxIdType;

# if defined( C_DRV_INTERNAL )
typedef struct
{
  ApplChannelFct      ApplCanWakeUpFct; 
  ApplCanMsgRcvFct    ApplCanMsgReceivedFct;
  ApplRangeFct        ApplCanRangeFct[4];
  ApplChannelFct      ApplCanBusOffFct;
  ApplConfirmationFct ApplCanCancelNotificationFct;
  ApplChannelFct      ApplCanMsgTransmitCancelNotifyFct;
  tCanStructRxIdType  RangeMask[4];
  tCanStructRxIdType  RangeCode[4];
  vuint16             RangeActiveFlag;
  tCanIdType          RangeIdType[4];                    /*##Ht: change implemenation?*/
} tCanChannelObject;
# endif


/* datatypes for indication and confirmation flags */
# if defined( C_ENABLE_CONFIRMATION_FLAG )
union CanConfirmationBits 
{
  vuint8         _c[kCanNumberOfConfBytes];
#  if defined( V_CPUTYPE_BITARRAY_32BIT )
#   if kCanNumberOfConfFlags > 16
  struct _c_bits32 w[kCanNumberOfConfDWords];
#   elif kCanNumberOfConfFlags > 8
  struct _c_bits16 w[kCanNumberOfConfWords];
#   else
  struct _c_bits8  w[1];
#   endif
#  else
#   if kCanNumberOfConfFlags > 8
  struct _c_bits16 w[kCanNumberOfConfWords];
#   else
  struct _c_bits8  w[1];
#   endif
#  endif
}; 
# endif


# if defined( C_ENABLE_INDICATION_FLAG )
union CanIndicationBits 
{
  vuint8         _c[kCanNumberOfIndBytes];
#  if defined( V_CPUTYPE_BITARRAY_32BIT )
#   if kCanNumberOfIndFlags > 16
  struct _c_bits32 w[kCanNumberOfIndDWords];
#   elif kCanNumberOfIndFlags > 8
  struct _c_bits16 w[kCanNumberOfIndWords];
#   else
  struct _c_bits8  w[1];
#   endif
#  else
#   if kCanNumberOfIndFlags > 8
  struct _c_bits16 w[kCanNumberOfIndWords];
#   else
  struct _c_bits8 w[1];
#   endif
#  endif
};
# endif

# if defined( C_ENABLE_RX_QUEUE )
/* struct for receive-Queue. Should correspond to t_MsgObject */
typedef struct
{
  CanReceiveHandle      Handle;
  CanChannelHandle      Channel;
  tCanMsgTransmitStruct CanChipMsgObj;
}
tCanRxQueueObject;

/* the queue */
typedef struct 
{
  tCanRxQueueObject canRxQueueBuf[kCanRxQueueSize];  /* buffer for msg and handle */  
  volatile vuintx   canRxQueueWriteIndex;            /* index in canRxQueueBuf */
  volatile vuintx   canRxQueueReadIndex;             /* index in canRxQueueBuf */
  volatile vuintx   canRxQueueCount;                 /* count of messages in canRxQueueBuf  */   
}
tCanRxQueue;
# endif

/***************************************************************************/
/* External Declarations                                                   */
/***************************************************************************/

/*  CAN driver version */
/* ROM CATEGORY 4 START*/
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kCanMainVersion;
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kCanSubVersion;
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kCanBugFixVersion;
/* ROM CATEGORY 4 END*/



# if defined( C_ENABLE_PRECOPY_FCT )
/* RAM CATEGORY 1 START*/
extern volatile CanReceiveHandle canRxHandle[kCanNumberOfChannels];
/* RAM CATEGORY 1 END*/
# endif

# if defined( C_ENABLE_CONFIRMATION_FLAG )
/* RAM CATEGORY 1 START*/
V_MEMRAM0 extern volatile V_MEMRAM1_NEAR union CanConfirmationBits V_MEMRAM2_NEAR CanConfirmationFlags;
/* RAM CATEGORY 1 END*/
# endif

# if defined( C_ENABLE_INDICATION_FLAG )
/* RAM CATEGORY 1 START*/
V_MEMRAM0 extern volatile V_MEMRAM1_NEAR union CanIndicationBits   V_MEMRAM2_NEAR CanIndicationFlags;
/* RAM CATEGORY 1 END*/
# endif


# if defined( C_ENABLE_CONFIRMATION_FCT ) && \
    defined( C_ENABLE_DYN_TX_OBJECTS )  && \
    defined( C_ENABLE_TRANSMIT_QUEUE )
/* RAM CATEGORY 3 START*/
extern CanTransmitHandle          confirmHandle[kCanNumberOfChannels];
/* RAM CATEGORY 3 END*/
# endif

/* ##RI1.4 - 3.31: Dynamic Receive DLC */
# if defined( C_ENABLE_VARIABLE_RX_DATALEN )
#  if ( kCanNumberOfRxObjects > 0 )
/* RAM CATEGORY 1 START*/
extern volatile vuint8 canVariableRxDataLen[kCanNumberOfRxObjects];
/* RAM CATEGORY 1 END*/
#  endif
# endif

/* RAM CATEGORY 1 START*/
extern CanChipDataPtr canRDSRxPtr[kCanNumberOfChannels];
/* RAM CATEGORY 1 END*/

/* RAM CATEGORY 1 START*/
extern CanChipDataPtr canRDSTxPtr[kCanNumberOfUsedTxCANObjects];
/* RAM CATEGORY 1 END*/

/***************************************************************************/
/* Data structures filled with data by parameter generating tool           */
/***************************************************************************/

# if defined( C_DRV_INTERNAL )
/* structures for init and common objects -----------------------*/
#  if defined( C_SINGLE_RECEIVE_CHANNEL )
#  else
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 tCanChannelObject   V_MEMROM2 CanChannelObject[kCanNumberOfChannels];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 4 START*/
V_MEMROM0 extern V_MEMROM1 vuint8              V_MEMROM2 CanInitObjectStartIndex[kCanNumberOfChannels+1];
/* ROM CATEGORY 4 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanTransmitHandle   V_MEMROM2 CanTxDynRomStartIndex[kCanNumberOfChannels];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanTransmitHandle   V_MEMROM2 CanTxDynRamStartIndex[kCanNumberOfChannels+1];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanReceiveHandle    V_MEMROM2 CanRxBasicStartIndex[kCanNumberOfChannels+1];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanReceiveHandle    V_MEMROM2 CanRxFullStartIndex[kCanNumberOfHwChannels+1];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 2 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanLogHwTxStartIndex[kCanNumberOfHwChannels+1];
/* ROM CATEGORY 2 END*/

/* ROM CATEGORY 2 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwTxStartIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 2 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwRxFullStartIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwRxBasicStartIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 4 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwUnusedStartIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 4 END*/
/* ROM CATEGORY 2 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwTxStopIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 2 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwRxFullStopIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwRxBasicStopIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 4 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwUnusedStopIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 4 END*/

/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwMsgTransmitIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanObjectHandle     V_MEMROM2 CanHwTxNormalIndex[kCanNumberOfHwChannels];
/* ROM CATEGORY 1 END*/
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 vsintx              V_MEMROM2 CanTxOffsetHwToLog[kCanNumberOfHwChannels];
/* ROM CATEGORY 1 END*/

#  endif  /* C_SINGLE_RECEIVE_CHANNEL */




#  if defined( C_ENABLE_INDIVIDUAL_POLLING )
/* ROM CATEGORY 1 START*/
/* define first index to array CanHwObjIndivPolling[][] */
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2     CanHwObjIndivPolling[kCanNumberOfHwChannels][kCanNumberOfHwObjIndivPolling];
/* ROM CATEGORY 1 END*/
#  endif

# endif /* C_DRV_INTERNAL */

# if defined( C_SINGLE_RECEIVE_CHANNEL )
# else
/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 CanTransmitHandle   V_MEMROM2 CanTxStartIndex[kCanNumberOfChannels+1];
/* ROM CATEGORY 1 END*/ 
/* ROM CATEGORY 2 START*/ 
V_MEMROM0 extern V_MEMROM1 CanReceiveHandle    V_MEMROM2 CanRxStartIndex[kCanNumberOfChannels+1];
/* ROM CATEGORY 2 END*/
# endif

/* ROM generated */

# if ( kCanNumberOfTxObjects > 0 )
#   if defined( C_DRV_INTERNAL )
/* structures for transmit objects -----------------------*/
/* ROM CATEGORY 1 START*/ 
//V_MEMROM0 extern V_MEMROM1 tCanTxId0 V_MEMROM2       CanTxId0[kCanNumberOfTxObjects];  
V_MEMROM0 extern V_MEMROM1 tCanTxId0       CanTxId0[kCanNumberOfTxObjects];  
#    if (kCanNumberOfUsedCanTxIdTables > 1)
V_MEMROM0 extern V_MEMROM1 tCanTxId1 V_MEMROM2       CanTxId1[kCanNumberOfTxObjects];  
#    endif
#    if (kCanNumberOfUsedCanTxIdTables > 2)
V_MEMROM0 extern V_MEMROM1 tCanTxId2 V_MEMROM2       CanTxId2[kCanNumberOfTxObjects];  
#    endif
#    if (kCanNumberOfUsedCanTxIdTables > 3)
V_MEMROM0 extern V_MEMROM1 tCanTxId3 V_MEMROM2       CanTxId3[kCanNumberOfTxObjects];  
#    endif
#    if (kCanNumberOfUsedCanTxIdTables > 4)
V_MEMROM0 extern V_MEMROM1 tCanTxId4 V_MEMROM2       CanTxId4[kCanNumberOfTxObjects];  
#    endif

#    if defined( C_ENABLE_MIXED_ID )
#    endif
/* ROM CATEGORY 1 END*/
#   endif /* C_DRV_INTERNAL */

/* ROM CATEGORY 2 START*/
#   if defined( C_ENABLE_PART_OFFLINE )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2          CanTxSendMask[kCanNumberOfTxObjects];
#   endif
/* ROM CATEGORY 2 END*/

/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2          CanTxDLC[kCanNumberOfTxObjects];
V_MEMROM0 extern V_MEMROM1 TxDataPtr V_MEMROM2       CanTxDataPtr[kCanNumberOfTxObjects];

#   if defined( C_ENABLE_CONFIRMATION_FLAG )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2          CanConfirmationOffset[kCanNumberOfTxObjects];
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2          CanConfirmationMask[kCanNumberOfTxObjects];
#   endif
#   if defined( C_ENABLE_CONFIRMATION_FCT )
V_MEMROM0 extern V_MEMROM1 ApplConfirmationFct V_MEMROM2 CanTxApplConfirmationPtr[kCanNumberOfTxObjects];
#   endif
#   if defined( C_ENABLE_PRETRANSMIT_FCT )
V_MEMROM0 extern V_MEMROM1 ApplPreTransmitFct  V_MEMROM2 CanTxApplPreTransmitPtr[kCanNumberOfTxObjects];
#   endif
/* ROM CATEGORY 1 END*/


#   if defined( C_SINGLE_RECEIVE_CHANNEL )
#   else
#    if ( kCanNumberOfTxObjects > 0 )
/* ROM CATEGORY 2 START*/
V_MEMROM0 extern V_MEMROM1 CanChannelHandle    V_MEMROM2 CanTxMsgHandleToChannel[kCanNumberOfTxObjects];
/* ROM CATEGORY 2 END*/
#    endif
#   endif /* C_SINGLE_RECEIVE_CHANNEL */ 

#   if defined( C_ENABLE_MULTI_ECU_PHYS )
/* ROM CATEGORY 4 START*/
V_MEMROM0 extern V_MEMROM1 tVIdentityMsk V_MEMROM2      CanTxIdentityAssignment[kCanNumberOfTxObjects];
/* ROM CATEGORY 4 END*/
#   endif


#  if defined( C_ENABLE_TRANSMIT_QUEUE )
#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
/* ROM CATEGORY 1 START*/
/* pad bits added before the current channel position*/
V_MEMROM0 extern  V_MEMROM1 vuint8 V_MEMROM2 CanTxQueuePadBits[kCanNumberOfChannels];

/* start / stop indices for the element search*/
V_MEMROM0 extern V_MEMROM1 CanSignedTxHandle V_MEMROM2 CanTxQueueStartIndex[kCanNumberOfChannels + 1];
/* ROM CATEGORY 1 END*/
#endif
#  endif

# endif /* ( kCanNumberOfTxObjects > 0 ) */

/* structures for basic and full can receive objects -----------------------*/

# if ( kCanNumberOfRxObjects > 0 )
#  if defined( C_DRV_INTERNAL )
/* ROM CATEGORY 1 START*/
#   if defined( C_SEARCH_HASH ) 
#   else   /* C_SEARCH_HASH */
//V_MEMROM0 extern V_MEMROM1 tCanRxId0 V_MEMROM2       CanRxId0[kCanNumberOfRxObjects];  
V_MEMROM0 extern V_MEMROM1 tCanRxId0       CanRxId0[kCanNumberOfRxObjects];  
#    if (kCanNumberOfUsedCanRxIdTables > 1)
V_MEMROM0 extern V_MEMROM1 tCanRxId1 V_MEMROM2       CanRxId1[kCanNumberOfRxObjects];  
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 2)
V_MEMROM0 extern V_MEMROM1 tCanRxId2 V_MEMROM2       CanRxId2[kCanNumberOfRxObjects];  
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 3)
V_MEMROM0 extern V_MEMROM1 tCanRxId3 V_MEMROM2       CanRxId3[kCanNumberOfRxObjects];  
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 4)
V_MEMROM0 extern V_MEMROM1 tCanRxId4 V_MEMROM2       CanRxId4[kCanNumberOfRxObjects];  
#    endif

#    if defined( C_ENABLE_MIXED_ID )
#     if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#     else
V_MEMROM0 extern V_MEMROM1 tCanIdType V_MEMROM2      CanRxIdType[kCanNumberOfRxObjects];
#     endif
#    endif
#   endif  /* C_SEARCH_HASH */


#   if defined( C_SEARCH_HASH )
V_MEMROM0 extern V_MEMROM1 CanReceiveHandle V_MEMROM2     CanRxMsgIndirection[kHashSearchListCount + kHashSearchListCountEx + kCanNumberOfRxFullCANObjects];
#   else
#     if defined( C_ENABLE_RX_MSG_INDIRECTION )
V_MEMROM0 extern V_MEMROM1 CanReceiveHandle V_MEMROM2     CanRxMsgIndirection[kCanNumberOfRxObjects];
#     endif
#   endif

#   if defined( C_SEARCH_HASH )
#    if defined( C_ENABLE_EXTENDED_ID )
#     if( kHashSearchListCountEx > 0)
V_MEMROM0 extern V_MEMROM1 vuint32 V_MEMROM2              CanRxHashIdEx[kHashSearchListCountEx];
#     endif /* (kHashSearchListCountEx > 0) */
#     if defined( C_ENABLE_MIXED_ID )
#      if( kHashSearchListCount > 0)
V_MEMROM0 extern V_MEMROM1 vuint16 V_MEMROM2              CanRxHashId[kHashSearchListCount];
#      endif
#     endif
#    else
#     if( kHashSearchListCount > 0)
V_MEMROM0 extern V_MEMROM1 vuint16 V_MEMROM2              CanRxHashId[kHashSearchListCount];
#     endif 
#    endif
#   endif

#   if defined( C_ENABLE_MULTI_ECU_PHYS )
V_MEMROM0 extern V_MEMROM1 tVIdentityMsk V_MEMROM2           CanRxIdentityAssignment[kCanNumberOfRxObjects];
#   endif

#   if defined( C_ENABLE_RX_QUEUE_RANGE )
#    if defined( C_ENABLE_RANGE_0 )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2               CanRxQueueRange0[kCanNumberOfChannels];
#    endif
#    if defined( C_ENABLE_RANGE_1 )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2               CanRxQueueRange1[kCanNumberOfChannels];
#    endif
#    if defined( C_ENABLE_RANGE_2 )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2               CanRxQueueRange2[kCanNumberOfChannels];
#    endif
#    if defined( C_ENABLE_RANGE_3 )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2               CanRxQueueRange3[kCanNumberOfChannels];
#    endif
#   endif
/* ROM CATEGORY 1 END*/

/* ROM CATEGORY 4 START*/
#   if defined( C_ENABLE_MULTIPLE_BASICCAN ) && \
       defined( C_ENABLE_GEN_CHECK )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2               CanNumberOfBasicCAN[kCanNumberOfChannels];
#   endif
/* ROM CATEGORY 4 END*/

#  endif /* C_DRV_INTERNAL */

/* ROM CATEGORY 1 START*/
V_MEMROM0 extern V_MEMROM1 vuint8               V_MEMROM2 CanRxDataLen[kCanNumberOfRxObjects];
#   if defined( C_ENABLE_DLC_CHECK_MIN_DATALEN )
V_MEMROM0 extern V_MEMROM1 vuint8               V_MEMROM2 CanRxMinDataLen[kCanNumberOfRxObjects];
#   endif
V_MEMROM0 extern V_MEMROM1 RxDataPtr            V_MEMROM2 CanRxDataPtr[kCanNumberOfRxObjects];

#   if defined( C_ENABLE_PRECOPY_FCT )
V_MEMROM0 extern V_MEMROM1 ApplPrecopyFct       V_MEMROM2 CanRxApplPrecopyPtr[kCanNumberOfRxObjects];
#   endif
#   if defined( C_ENABLE_INDICATION_FLAG )
V_MEMROM0 extern V_MEMROM1 vuint8               V_MEMROM2 CanIndicationOffset[kCanNumberOfRxObjects];
V_MEMROM0 extern V_MEMROM1 vuint8               V_MEMROM2 CanIndicationMask[kCanNumberOfRxObjects];
#   endif
#   if defined( C_ENABLE_INDICATION_FCT )
V_MEMROM0 extern V_MEMROM1 ApplIndicationFct    V_MEMROM2 CanRxApplIndicationPtr[kCanNumberOfRxObjects];
#   endif
/* ROM CATEGORY 1 END*/
# endif /* ( kCanNumberOfRxObjects > 0 ) */


/* ROM CATEGORY 4 START*/

/* structure used on initialization CAN chip -------------------------------*/
//V_MEMROM0 extern V_MEMROM1 tCanInitObject V_MEMROM2 CanInitObject[];
V_MEMROM0 extern V_MEMROM1 tCanInitObject CanInitObject[];
#if defined (C_MULTIPLE_RECEIVE_CHANNEL )
V_MEMROM0 extern V_MEMROM1 vuint16        V_MEMROM2 CanBasisAdr[];
#endif
/* ROM CATEGORY 4 END*/

/***************************************************************************/
/* Callback functions                                                      */
/***************************************************************************/

# if defined( C_ENABLE_INTCTRL_ADD_CAN_FCT )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanAddCanInterruptRestore(CanChannelHandle channel);
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanAddCanInterruptDisable(CanChannelHandle channel);
/* CODE CATEGORY 1 END*/
# endif

# if defined( C_ENABLE_HARDWARE_CHECK ) || \
    defined( C_ENABLE_USER_CHECK )     || \
    defined( C_ENABLE_GEN_CHECK )      || \
    defined( C_ENABLE_INTERNAL_CHECK ) 
/* CODE CATEGORY 4 START*/ 
C_CALLBACK_1 void C_CALLBACK_2 ApplCanFatalError(CAN_CHANNEL_CANTYPE_FIRST vuint8 errorNumber);
/* CODE CATEGORY 4 END*/
# endif

# if defined( C_SINGLE_RECEIVE_CHANNEL )
/* these two functions are declared by the tool in case of multichannel */
/* CODE CATEGORY 4 START*/ 
C_CALLBACK_1 void C_CALLBACK_2 ApplCanBusOff(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/ 
C_CALLBACK_1 void C_CALLBACK_2 ApplCanWakeUp(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END*/
# endif
# if defined( C_ENABLE_APPLCANPREWAKEUP_FCT )
/* CODE CATEGORY 4 START*/ 
C_CALLBACK_1 void C_CALLBACK_2 ApplCanPreWakeUp(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END*/
# endif
# if defined( C_ENABLE_OVERRUN )
/* CODE CATEGORY 4 START*/ 
C_CALLBACK_1 void C_CALLBACK_2 ApplCanOverrun ( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
# endif

/* Range precopy/ generic precopy functions ********************************/
# if defined( C_SINGLE_RECEIVE_CHANNEL )
#  if defined( C_ENABLE_RANGE_0 )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 ApplCanRange0Precopy( CanRxInfoStructPtr rxStruct );
/* CODE CATEGORY 1 END*/
#  endif
#  if defined( C_ENABLE_RANGE_1 )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 ApplCanRange1Precopy( CanRxInfoStructPtr rxStruct );
/* CODE CATEGORY 1 END*/
#  endif
#  if defined( C_ENABLE_RANGE_2 )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 ApplCanRange2Precopy( CanRxInfoStructPtr rxStruct );
/* CODE CATEGORY 1 END*/
#  endif
#  if defined( C_ENABLE_RANGE_3 )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 ApplCanRange3Precopy( CanRxInfoStructPtr rxStruct );
/* CODE CATEGORY 1 END*/
#  endif
# endif

# if defined( C_ENABLE_COND_RECEIVE_FCT )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanMsgCondReceived  ( CanRxInfoStructPtr rxStruct );
/* CODE CATEGORY 1 END*/
# endif

# if defined( C_ENABLE_CAN_RAM_CHECK )
#  if defined( C_ENABLE_NOTIFY_CORRUPT_MAILBOX )
/* CODE CATEGORY 4 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanCorruptMailbox(CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle hwObjHandle);
/* CODE CATEGORY 4 END*/
#  endif
/* CODE CATEGORY 4 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 ApplCanMemCheckFailed(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END*/
# endif

# if defined( C_SINGLE_RECEIVE_CHANNEL )
#  if defined( C_ENABLE_RECEIVE_FCT )
/* in case of multiple CAN channels this function is declared by the tool */
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 APPL_CAN_MSGRECEIVED(CanRxInfoStructPtr rxStruct);
/* CODE CATEGORY 1 END*/
#  endif
# endif

/* ##RI1.4 - 2.7: Callbackfunction-DLC-Check */
# if defined( C_ENABLE_DLC_FAILED_FCT )
/* CODE CATEGORY 4 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanMsgDlcFailed( CanRxInfoStructPtr rxStruct ); 
/* CODE CATEGORY 4 END*/ 
# endif  /*C_ENABLE_DLC_FAILED_FCT */

# if defined( C_ENABLE_GENERIC_PRECOPY )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 APPL_CAN_GENERIC_PRECOPY( CanRxInfoStructPtr rxStruct );
/* CODE CATEGORY 1 END*/
# endif

# if defined( C_ENABLE_NOT_MATCHED_FCT )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanMsgNotMatched( CanRxInfoStructPtr rxStruct );
/* CODE CATEGORY 1 END*/
# endif

# if defined( C_ENABLE_CAN_TX_CONF_FCT )
/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 APPL_CAN_TX_CONFIRMATION(CanTxInfoStructPtr txStruct);
/* CODE CATEGORY 1 END*/
# endif

# if defined( C_ENABLE_TX_OBSERVE )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanTxObjStart( CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txHwObject );
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanTxObjConfirmed( CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txHwObject );
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 4 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanInit( CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txHwObjectFirstUsed, CanObjectHandle txHwObjectFirstUnused);
/* CODE CATEGORY 4 END*/
# endif

# if defined( C_ENABLE_HW_LOOP_TIMER )
/* CODE CATEGORY 2 START*/
C_CALLBACK_1 void     C_CALLBACK_2 ApplCanTimerStart(CAN_CHANNEL_CANTYPE_FIRST vuint8 source);
/* CODE CATEGORY 2 END*/
/* CODE CATEGORY 2 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 ApplCanTimerLoop (CAN_CHANNEL_CANTYPE_FIRST vuint8 source);
/* CODE CATEGORY 2 END*/
/* CODE CATEGORY 2 START*/
C_CALLBACK_1 void     C_CALLBACK_2 ApplCanTimerEnd  (CAN_CHANNEL_CANTYPE_FIRST vuint8 source);
/* CODE CATEGORY 2 END*/
# endif

# if defined( C_ENABLE_MSG_TRANSMIT_CONF_FCT )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 APPL_CAN_MSGTRANSMITCONF(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 4 START*/
C_CALLBACK_1 void C_CALLBACK_2 APPL_CAN_MSGTRANSMITINIT(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END*/
# endif

# if defined( C_SINGLE_RECEIVE_CHANNEL )
/* in case of multichannel these two functions are declared by the tool */
#  if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
/* CODE CATEGORY 3 START*/
C_CALLBACK_1 void C_CALLBACK_2 APPL_CAN_CANCELNOTIFICATION( CanTransmitHandle txHandle );
/* CODE CATEGORY 3 END*/
#  endif
#  if defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
/* CODE CATEGORY 3 START*/
C_CALLBACK_1 void C_CALLBACK_2 APPL_CAN_MSGCANCELNOTIFICATION( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 3 END*/
#  endif
# endif

# if defined( C_ENABLE_RX_QUEUE )
#  if defined( C_ENABLE_APPLCANPRERXQUEUE )
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 ApplCanPreRxQueue( CanRxInfoStructPtr pCanRxInfoStruct );
/* CODE CATEGORY 1 END*/
#  endif
#  if defined( C_ENABLE_RXQUEUE_OVERRUN_NOTIFY )
/* CODE CATEGORY 4 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanRxQueueOverrun( void );
/* CODE CATEGORY 4 END*/
#  endif
# endif

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
/* CODE CATEGORY 3 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanChannelDummy( CanChannelHandle channel );
/* CODE CATEGORY 3 END*/
# endif
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) 
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 void C_CALLBACK_2 ApplCanTxHandleDummy( CanTransmitHandle txHandle );
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 1 START*/
C_CALLBACK_1 vuint8 C_CALLBACK_2 ApplCanRxStructPtrDummy( CanRxInfoStructPtr rxStruct );
/* CODE CATEGORY 1 END*/
# endif
# if defined( C_ENABLE_ONLINE_OFFLINE_CALLBACK_FCT )
/* CODE CATEGORY 4 START*/
C_CALLBACK_1 void C_CALLBACK_2 APPL_CAN_OFFLINE( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_CALLBACK_1 void C_CALLBACK_2 APPL_CAN_ONLINE( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
# endif


/***************************************************************************/
/* function prototypes                                                     */
/***************************************************************************/

/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanInitPowerOn( void );
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanInit( CAN_CHANNEL_CANTYPE_FIRST CanInitHandle initObject );
/* CODE CATEGORY 4 END*/

# if defined( C_ENABLE_CAN_TRANSMIT )
/* CODE CATEGORY 2 START*/
C_API_1 vuint8 C_API_2 CanTransmit(CanTransmitHandle txHandle) C_API_3;
/* CODE CATEGORY 2 END*/
#  if defined( C_ENABLE_VARIABLE_DLC )
/* CODE CATEGORY 2 START*/
C_API_1 vuint8 C_API_2 CanTransmitVarDLC( CanTransmitHandle txHandle, vuint8 dlc);
/* CODE CATEGORY 2 END*/
#  endif
/* CODE CATEGORY 3 START*/
C_API_1 void C_API_2 CanCancelTransmit( CanTransmitHandle txHandle );
/* CODE CATEGORY 3 END*/
# endif

# if defined( C_ENABLE_MSG_TRANSMIT )
/* CODE CATEGORY 2 START*/
C_API_1 vuint8 C_API_2 CanMsgTransmit( CAN_CHANNEL_CANTYPE_FIRST tCanMsgTransmitStruct *txMsgStruct);
/* CODE CATEGORY 2 END*/
#  if defined( C_ENABLE_MSG_TRANSMIT_CONF_FCT )
/* CODE CATEGORY 3 START*/
C_API_1 void C_API_2 CanCancelMsgTransmit( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 3 END*/
#  endif
# endif

# if defined( C_ENABLE_OFFLINE )
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanOnline( CAN_CHANNEL_CANTYPE_ONLY );           /* switch CanStatus to On */
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanOffline( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
# endif

# if defined( C_ENABLE_PART_OFFLINE )
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetPartOffline( CAN_CHANNEL_CANTYPE_FIRST vuint8 sendGroup);
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetPartOnline( CAN_CHANNEL_CANTYPE_FIRST vuint8 invSendGroup);
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanGetPartMode( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
# endif

/* CODE CATEGORY 3 START*/
C_API_1 vuint8 C_API_2 CanGetStatus( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 3 END*/

/* CanSleep functions */
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanSleep( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanWakeUp( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/

# if defined( C_ENABLE_ECU_SWITCH_PASS )
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetActive(  CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetPassive( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
# else
#  if defined( C_MULTIPLE_RECEIVE_CHANNEL )
#   define    CanSetActive( channel )            /* remove calls to setActive/Passive*/
#   define    CanSetPassive( channel )
#  else
#   define    CanSetActive( )                    /* remove calls to setActive/Passive*/
#   define    CanSetPassive( )
#  endif
# endif

# if defined( C_ENABLE_STOP )
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanStart( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanStop( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
# endif

/* CODE CATEGORY 1 START*/
C_API_1 void C_API_2 CanCanInterruptDisable( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 1 START*/
C_API_1 void C_API_2 CanCanInterruptRestore( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 1 END*/

# if defined( C_ENABLE_TX_POLLING ) || \
    defined( C_ENABLE_RX_BASICCAN_POLLING ) || \
    defined( C_ENABLE_ERROR_POLLING ) || \
    defined( C_ENABLE_WAKEUP_POLLING ) 
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanTask(void);
/* CODE CATEGORY 2 END*/
# endif
# if defined( C_ENABLE_TX_POLLING ) 
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanTxTask( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 2 END*/
# endif
# if defined( C_ENABLE_TX_POLLING ) && \
     defined( C_ENABLE_INDIVIDUAL_POLLING )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanTxObjTask(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle);
/* CODE CATEGORY 2 END*/
# endif
# if defined( C_ENABLE_ERROR_POLLING )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanErrorTask( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 2 END*/
# endif
# if defined( C_ENABLE_SLEEP_WAKEUP )
#  if defined( C_ENABLE_WAKEUP_POLLING )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanWakeUpTask(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END*/
#  endif
# endif
#if defined( C_ENABLE_RX_BASICCAN_POLLING ) && \
    defined( C_ENABLE_RX_BASICCAN_OBJECTS )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanRxBasicCANTask(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 2 END*/
#  if defined( C_ENABLE_INDIVIDUAL_POLLING )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanRxBasicCANObjTask(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle);
/* CODE CATEGORY 2 END*/
#  endif
# endif


# if defined( C_ENABLE_DYN_TX_OBJECTS )
/* CODE CATEGORY 3 START*/
C_API_1 CanTransmitHandle C_API_2 CanGetDynTxObj(CanTransmitHandle txHandle );
/* CODE CATEGORY 3 END*/
/* CODE CATEGORY 3 START*/
C_API_1 vuint8          C_API_2 CanReleaseDynTxObj(CanTransmitHandle txHandle);
/* CODE CATEGORY 3 END*/

#  if defined( C_ENABLE_DYN_TX_ID )
#   if !defined( C_ENABLE_EXTENDED_ID ) ||\
     defined( C_ENABLE_MIXED_ID )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanDynTxObjSetId(CanTransmitHandle txHandle, vuint16 id);
/* CODE CATEGORY 2 END*/
#   endif
#   if defined( C_ENABLE_EXTENDED_ID )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanDynTxObjSetExtId(CanTransmitHandle txHandle, vuint16 idExtHi, vuint16 idExtLo);
/* CODE CATEGORY 2 END*/
#   endif
#  endif
#  if defined( C_ENABLE_DYN_TX_DLC )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanDynTxObjSetDlc(CanTransmitHandle txHandle, vuint8 dlc);
/* CODE CATEGORY 2 END*/
#  endif
#  if defined( C_ENABLE_DYN_TX_DATAPTR )
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanDynTxObjSetDataPtr(CanTransmitHandle txHandle, void* pData);
/* CODE CATEGORY 2 END*/
#  endif


# endif /* defined( C_ENABLE_DYN_TX_OBJECTS ) */

# if defined( C_ENABLE_TX_MASK_EXT_ID )
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetTxIdExtHi( CAN_CHANNEL_CANTYPE_FIRST  vuint8 mask );
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetTxIdExtMidHi( CAN_CHANNEL_CANTYPE_FIRST  vuint8 mask );
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetTxIdExtMidLo( CAN_CHANNEL_CANTYPE_FIRST  vuint8 mask );
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetTxIdExtLo( CAN_CHANNEL_CANTYPE_FIRST  vuint8 mask );
/* CODE CATEGORY 4 END*/
# endif

# if defined( C_ENABLE_TX_OBSERVE )
/* CODE CATEGORY 3 START*/
C_API_1 CanTransmitHandle C_API_2 CanTxGetActHandle( CanObjectHandle logicalTxHdl );
/* CODE CATEGORY 3 END*/
# endif

# if defined( C_ENABLE_COND_RECEIVE_FCT )
/* CODE CATEGORY 3 START*/
C_API_1 void     C_API_2 CanSetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 3 END*/
/* CODE CATEGORY 3 START*/
C_API_1 void     C_API_2 CanResetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 3 END*/
/* CODE CATEGORY 3 START*/
C_API_1 vuint8 C_API_2 CanGetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 3 END*/
# endif

# if defined( C_ENABLE_RX_QUEUE )
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanDeleteRxQueue(void);
/* CODE CATEGORY 4 END*/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanHandleRxMsg(void);
/* CODE CATEGORY 2 END*/
# endif

#if defined( C_ENABLE_MEMCOPY_SUPPORT )
/* CODE CATEGORY 1 START*/
void CanCopyToCan(CanChipDataPtr dst, void * src, vuint8 len);
/* CODE CATEGORY 1 END*/
/* CODE CATEGORY 1 START*/
void CanCopyFromCan(void *dst, CanChipDataPtr src, vuint8 len);
/* CODE CATEGORY 1 END*/
#endif

#if !(defined( C_ENABLE_ISR_PROTOTYPE) || defined( C_DISABLE_ISR_PROTOTYPE ))
# define C_ENABLE_ISR_PROTOTYPE
#endif

#if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
#ifdef INTERRUPT
/* use our own interrupt macro if we can so that lint doesn't see the @'s */
# define CAN_IRQ_FUNC_QUAL1    INTERRUPT
#else
/* CANbedded-only code doesn't see the definition of INTERRUPT so add it
 * explicitly here.  Any changes nmust be mirrored in the definition of
 * INTERRUPT
 */
# define CAN_IRQ_FUNC_QUAL1    @interrupt @svpage @near
#endif
# define CAN_IRQ_FUNC_QUAL2
#endif






/******************************************************************************
*  CANBEDDED                                                                  *
******************************************************************************/

#if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
# if defined(C_ENABLE_ISR_SPLIT)
C_API_1 void C_API_2 CanRxIrqHandler(CAN_CHANNEL_CANTYPE_ONLY);
C_API_1 void C_API_2 CanTxIrqHandler(CAN_CHANNEL_CANTYPE_ONLY);
C_API_1 void C_API_2 CanErrorIrqHandler(CAN_CHANNEL_CANTYPE_ONLY);
#  if defined(C_ENABLE_SLEEP_WAKEUP)
C_API_1 void C_API_2 CanWakeUpIrqHandler(CAN_CHANNEL_CANTYPE_ONLY);
#  endif
# endif
#endif

#if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  

# if (defined(C_ENABLE_TX_POLLING)) && (!defined(C_ENABLE_INDIVIDUAL_POLLING))
#  define CAN_TX_IRQ_FUNC_DECL(Func)
# else
#  if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#   define CAN_TX_IRQ_FUNC(Func)
#   define CAN_TX_IRQ_FUNC_DECL(Func)
#  else
#   define CAN_TX_IRQ_FUNC(Func)          CAN_IRQ_FUNC_QUAL1 void CAN_IRQ_FUNC_QUAL2 Func(void)
#   define CAN_TX_IRQ_FUNC_DECL(Func)     CAN_TX_IRQ_FUNC(Func);
#  endif
# endif

# if (defined(C_ENABLE_RX_BASICCAN_POLLING)) && (!defined(C_ENABLE_INDIVIDUAL_POLLING))
#  define CAN_RX_IRQ_FUNC_DECL(Func)
# else
#  if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#   define CAN_RX_IRQ_FUNC(Func)
#   define CAN_RX_IRQ_FUNC_DECL(Func)
#  else
#   define CAN_RX_IRQ_FUNC(Func)          CAN_IRQ_FUNC_QUAL1 void CAN_IRQ_FUNC_QUAL2 Func(void)
#   define CAN_RX_IRQ_FUNC_DECL(Func)     CAN_RX_IRQ_FUNC(Func);
#  endif
# endif

# if defined(C_ENABLE_ERROR_POLLING)
#  define CAN_ERROR_IRQ_FUNC_DECL(Func)
# else
#  if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#   define CAN_ERROR_IRQ_FUNC(Func)
#   define CAN_ERROR_IRQ_FUNC_DECL(Func)
#  else
#   define CAN_ERROR_IRQ_FUNC(Func)       CAN_IRQ_FUNC_QUAL1 void CAN_IRQ_FUNC_QUAL2 Func(void)
#   define CAN_ERROR_IRQ_FUNC_DECL(Func)  CAN_ERROR_IRQ_FUNC(Func);
#  endif
# endif

# if defined(C_ENABLE_WAKEUP_POLLING)
#  define CAN_WAKEUP_IRQ_FUNC_DECL(Func)
#  error "Wakeup Polling Is Not Allowed!"
# else
#  if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#   define CAN_WAKEUP_IRQ_FUNC(Func)
#   define CAN_WAKEUP_IRQ_FUNC_DECL(Func)
#  else
#   define CAN_WAKEUP_IRQ_FUNC(Func)      CAN_IRQ_FUNC_QUAL1 void CAN_IRQ_FUNC_QUAL2 Func(void)
#   define CAN_WAKEUP_IRQ_FUNC_DECL(Func) CAN_WAKEUP_IRQ_FUNC(Func);
#  endif
# endif
#endif


#if defined (C_ENABLE_ISR_PROTOTYPE)
# if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  

#  if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
/* ISR prototypes must be provided by OSEK header file */
#  else
#   if defined( kCanPhysToLogChannelIndex_0 )


CAN_RX_IRQ_FUNC_DECL(CanRxInterrupt_0)



CAN_TX_IRQ_FUNC_DECL(CanTxInterrupt_0)



CAN_WAKEUP_IRQ_FUNC_DECL(CanWakeUpInterrupt_0)



CAN_ERROR_IRQ_FUNC_DECL(CanErrorInterrupt_0)


#   endif
#   if defined( kCanPhysToLogChannelIndex_1 )


CAN_RX_IRQ_FUNC_DECL(CanRxInterrupt_1)



CAN_TX_IRQ_FUNC_DECL(CanTxInterrupt_1)



CAN_WAKEUP_IRQ_FUNC_DECL(CanWakeUpInterrupt_1)



CAN_ERROR_IRQ_FUNC_DECL(CanErrorInterrupt_1)


#   endif
#   if defined( kCanPhysToLogChannelIndex_2 )


CAN_RX_IRQ_FUNC_DECL(CanRxInterrupt_2)



CAN_TX_IRQ_FUNC_DECL(CanTxInterrupt_2)



CAN_WAKEUP_IRQ_FUNC_DECL(CanWakeUpInterrupt_2)



CAN_ERROR_IRQ_FUNC_DECL(CanErrorInterrupt_2)


#   endif
#   if defined( kCanPhysToLogChannelIndex_3 )


CAN_RX_IRQ_FUNC_DECL(CanRxInterrupt_3)



CAN_TX_IRQ_FUNC_DECL(CanTxInterrupt_3)



CAN_WAKEUP_IRQ_FUNC_DECL(CanWakeUpInterrupt_3)



CAN_ERROR_IRQ_FUNC_DECL(CanErrorInterrupt_3)


#   endif
#   if defined( kCanPhysToLogChannelIndex_4 )


CAN_RX_IRQ_FUNC_DECL(CanRxInterrupt_4)



CAN_TX_IRQ_FUNC_DECL(CanTxInterrupt_4)



CAN_WAKEUP_IRQ_FUNC_DECL(CanWakeUpInterrupt_4)



CAN_ERROR_IRQ_FUNC_DECL(CanErrorInterrupt_4)


#   endif
#  endif
# endif
#endif





#endif /* CAN_DEF_H */

/* End of channel */



/* Kernbauer Version: 1.16 Konfiguration: can_driver Erzeugungsgangnummer: 1 */

/* Kernbauer Version: 1.16 Konfiguration: can_driver Erzeugungsgangnummer: 1 */

/* Kernbauer Version: 1.16 Konfiguration: can_driver Erzeugungsgangnummer: 1 */

/* Kernbauer Version: 1.16 Konfiguration: can_driver Erzeugungsgangnummer: 1 */

/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
