/* STARTSINGLE_OF_MULTIPLE */
/*******************************************************************************
| Project Name: Tp_Iso15765
|               OSEK single-/multiconnection transport layer
|    File Name: tpmc.h
|
|  Description: This version supports the specification for 
|               the ISO 15765-2 transport protocol
|
|-------------------------------------------------------------------------------
|               C O P Y R I G H T
|-------------------------------------------------------------------------------
| Copyright (c) 2000-2007 Vector Informatik GmbH            All rights reserved.
|
|     This software is copyright protected and proprietary 
|     to Vector Informatik GmbH. Vector Informatik GmbH 
|     grants to you only those rights as set out in the 
|     license conditions. All other rights remain with 
|     Vector Informatik GmbH.
|
|     REMOVAL OF THESE COMMENTS IS A VIOLATION OF THE LICENSE AGREEMENT.
|
|-------------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-------------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     ---------------------------------------
| Gz           Oliver Garnatz            Vector Informatik GmbH
| Js           Bernd Jesse               Vector Informatik GmbH
| Ap           Andreas Pick              Vector Informatik GmbH
| Her          Peter Herrmann            Vector Informatik GmbH
| EJW          Eric J. Winder            Vector CANtech, Inc.
|-------------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-------------------------------------------------------------------------------
| Date         Ver   Author  Description
| ---------    ----  ------  ---------------------------------------------------
| 16-Jun-00    2.00    Gz    Creation
| 26-Jul-00    2.01    Gz    Compatibility for DBKOMGen (files needed: tp_mch.h & tpmc.cfg)
| 31-Jul-00    2.02    Gz    Old Switches deleted/renamed
| 31-Aug-00    2.03    Gz    support of normal_addressing including dynamic_ids
| 12-Oct-00    2.05    Gz    support of normal_fixed_addressing (ext.ID) format
|                            added Channel-Lock
|                            added Reception Break while sending Wait-Frame 
|                            added BugFix Version
|                            added more Assertions
|                            added indicated CAN-Driver support
|                            added/modified some API-functions
|                            modified Custom TX/RX CopyFunctions 
|                            modified concerning DBKOMGen Tool
|                            deleted TransmitSF
|                            deleted compatibility Mode
| 24-Nov-00    2.06    Gz    no changes
| 05-Mar-01    2.07    Gz    added connections for rx-side; adjustable stmin-time
|                      Gz    added Review changes
| 14-Mar-01    2.08    Gz    corrections for Busoff handling
| 09-Apr-01    2.09    Gz    extension of the Gateway-API
| 20-Apr-01    2.10    Gz    added missing lines
| 11-May-01    2.11    Gz    enlarged support for multiple CAN-channel ECUs
| 05-Jun-01    2.12    Gz    support of extended addressing
| 11-Jun-01            Gz    support of single channel TP
| 15-Jun-01    2.13    Gz    no changes
| 19-Jun-01    2.14    Js    pChipDataPtr substituted with CanChipDataPtr
| 27-Jul-01    2.15    Gz    no changes
| 07-Sep-01    2.16    Gz    ESCAN00000819: better support for singlechannel-TP
|                            ESCAN00001096: Support of Tp-Connections without FC
|                            ESCAN00001097: TP is always in compatibility mode 
|                                           while reading Source-/TargetAddress
|                            ESCAN00001101: early termination of the 
|                                           queueing mechanism (only if CanChannels > 1)
|                            ESCAN00001149: New TP-class: Dynamic Multiple Addressing
| 07-Sep-01    2.17    Gz    no changes
| 20-Sep-01    2.18    Gz    ESCAN00001482: 'Connection specific timings' is not 
|                                           working correctly
|                            ESCAN00001526: Timings should be normalized by the GenTool
|                            ESCAN00001537: TransmitHandles should be changeable 
|                                           while runtime
| 26-Oct-01    2.19    VC    Reserved (only VectorCantech)
| 26-Oct-01    2.20    VC    Reserved (only VectorCantech)
| 26-Oct-01    2.21    VC    Reserved (only VectorCantech)
| 26-Oct-01    2.22    Gz    General improvement
| 06-Nov-01            Rn    ESCAN00001724: delete used in TpTxHoldChannel
| 06-Nov-01            Gz    ESCAN00001744: Combination of 'Use STMin from flow control 
|                                           frame' together with a 'connection without FC' 
|                                           will not work
| 06-Nov-01            Gz    ESCAN00001746: (only DC) complementary to 'connecntions 
|                                           without FC' an additional BIT is set in the 
|                                           TPCI-Byte to differ between
| 08-Nov-01    2.23    Gz    ESCAN00001764: In case of single optimized version some 
|                                           macros have still parameter channel.
| 27-Nov-01    2.24    Gz    no changes
| 2001-Dec-06  2.25.00 Gz    ESCAN00001891: The limitation having an unique CAN-ID over 
|                                           all CAN-Channels is eliminated
| 2001-Dec-12  2.26.00 Gz    ESCAN00001662: It should be possible to have more than one Base-Address
| 2001-Dec-18          Gz    ESCAN00001798: The TPCI-Byte for FlowControl's is not checked 
|                                           for valid values
| 2002-Jan-07          Gz    ESCAN00001853: insert keyword for far APIm and far callback-functions
| 2002-Jan-30          Gz    ESCAN00002185: for multiple CAN-systems and dynamicTP: 
|                                           TpPrecopy-function is not reentrant 
|                            ESCAN00002175: Illegal SingleFrame lengths will be shorten 
|                                           to a legal SingleFrame length.
|                            ESCAN00002190: ApplTpRxErrorIndication is called after ApplTpRxIndication
| 2002-Jan-14  2.27.00 Gz    ESCAN00002323: No transmission or reception possible in case of enable debug mode
|                            ESCAN00002324: support indexed CAN-driver for all dynamic TP-classes
| 2002-Feb-28  2.28.00 Gz    ESCAN00002374: Limiting the STmin time from FC to a minimum
| 2002-Mar-11          Gz    ESCAN00002392: Observing CAN frame DLC (Data Length Code)
| 2002-Mar-11          Gz    ESCAN00002379: Illegal reception, if a CAN-message with DLC=0 is received
| 2002-Mar-11          Gz    ESCAN00002380: The TP can't receive a FirstFrame with a dataLength 
|                                           smaller six(6)/seven(7) bytes successfully
| 2002-Mar-11          Gz    ESCAN00002459: enhance the fault tolerance 
| 2002-Mar-14          Gz    ESCAN00002490: A possibility should be provided to pre-check 
|                                           each CAN-message of a TP-frame
| 2002-Mar-15          Gz    ESCAN00002496: Syntax error in definition of function TpRxGetBS
| 2002-Mar-20  2.29.00 Gz    ESCAN00002534: Tp is blocked after TpTransmit returns 'kTpFailed'
| 2002-Mar-22          Gz    ESCAN00002554: Tp stays blocked after receiving a TP-frame with 
|                                           wrong DLC or incorrect frame-length 
|                      Gz    ESCAN00002555: AppltpTxErrorFunction() will be called 
|                                           after TpTransmit returns 'kTpFailed'
|                      Gz    ESCAN00002534: TP is locked after wakeup 
| 2002-Mar-27  2.30.00 Gz    ESCAN00002565: Possible under-run of the requested STmin time
|                      Gz    ESCAN00002558: Only Cosmic HC12 with option -pck : 
|                                           Compiler reports truncating assignment when TpTxQueueCheck() is called
| 2002-Apr-03          Gz    ESCAN00002578: It should be not possible to change communication parameters 
|                                           while a communication is running
| 2002-Apr-05          Gz    ESCAN00002632: compatibility mode for old style 
|                                           TpTxErrorIndication function is deleted 
|                      Gz    ESCAN00002636: the possibility to switch off the feature 
|                                           'using connections' are not supported any more
| 2002-Apr-26  2.31.00 Gz    ESCAN00002642: Partially written frames are transmitted by the TP 
|                                           (only if 'ApplTpTxCopyToCAN' is used)
| 2002-May-07  2.32.00 Gz    ESCAN00002798: connections will terminate very often and run into a 
|                                           confirmation timeout (only with disabled can transmit queue)
| 2002-Jun-05  2.33.00 Gz    ESCAN00003065: It should be possible to deactivate the TP separately on each CAN-channel 
|                      Gz    ESCAN00002652: no changes
|                      Gz    ESCAN00003077: no changes
|                      Gz    ESCAN00003076: no changes
| 2002-Jul-11  2.34.00 Gz    ESCAN00003204: no changes
| 2002-Jul-22          Gz    ESCAN00003135: no changes
| 2002-Jul-22          Gz    ESCAN00003081: Fully compliance to the ISO-specification
| 2002-Jul-22          Gz    ESCAN00003344: no changes
| 2002-Jul-22          Gz    ESCAN00003345: no changes
| 2002-Jul-24          Gz    ESCAN00003342: Change in observing the minimum sending time foy dyn.TP
| 2002-Jul-24          Gz    ESCAN00003374: Calculate STmin and BS only from the first FlowControl 
| 2002-Aug-22  2.35.00 Gz    ESCAN00003584: no changes
| 2002-Sep-02          Gz    ESCAN00003617: no changes
| 2002-Sep-04          Gz    ESCAN00003586: no changes
| 2002-Sep-04          Gz    ESCAN00003655: no changes
| 2002-Sep-05          Gz    ESCAN00003375: Separation of TimerTask and TransmissionTask (StateTask)
| 2002-Sep-05          Gz    ESCAN00003376: CF Transmission faster than callycyle of TpTxTimerTask()
| 2002-Okt-07  2.36.00 Ap    ESCAN00003809: While polling the CAN-driver slowly the connection can be lost-> define added
| 2002-Okt-09          Gz    ESCAN00003520: Change hook-functions (delete 'ApplTpRxCanMessageReceived' / 
|                                           add 'ApplTpTxFC' / add 'TpPreCopyCheckFunction')
| 2002-Okt-09          Gz    ESCAN00003750: prototype for ApplTpFataleError missing
| 2002-Okt-09          Gz    ESCAN00003751: no changes
| 2002-12-06   2.37.00 Gz    ESCAN00004216: no changes
|                      Gz    ESCAN00004340: no changes
|                      Gz    ESCAN00004373: no changes
| 2002-12-09   2.38.00 Gz    ESCAN00004399: no changes
| 2003-01-11   2.39.00 Gz    ESCAN00004587: Compatibility mode for older DBKOMgen versions 
|                                           won't work, if DBKOM is deativated 
| 2003-02-03   2.40.00 Gz    ESCAN00004757: no changes
| 2003-11-13   2.41.00 Gz    ESCAN00005041: no changes
|                      Gz    ESCAN00004199: Conditional Pre-Processor directives misses parantheses
|                      Gz    ESCAN00003891: Add a CANchannel-specific re-initialization function.
|                      Gz    ESCAN00005100: no changes
|                      Gz    ESCAN00005221: no changes
|                      Gz    ESCAN00005257: no changes
|                      Gz    ESCAN00005539: TpRxSetBufferOverrun is provided: Do not copy all frames if a buffer overrun occured
|                      Gz    ESCAN00005541: Call ApplXxErrorIndication() if for a running connection TpXxResetChannel() is called.
|                      !!! Warning this version has a change in its behaviour !!!
|                      Gz    ESCAN00006155: no changes
|                      Gz    ESCAN00006159: Add TpTask() function
|                      Gz    ESCAN00006156: Add new requirements from ISO 15765-2.2
|                      !!! Warning if these changes are active there will be changes in the behaviour !!!
|                      Gz    ESCAN00006158: Add a separat channel for functional reception (N_TAtype = functional)
|                      Gz    ESCAN00006040: An API should be added which results the current CAN-channel 
|                                           of the received frame (only multiple CAN-channel systems)
|                      Gz    ESCAN00006014: no changes
|                      Gz    ESCAN00006341: no changes
|                      Gz    ESCAN00006354: C-Library functions not longer used
|                      Gz    ESCAN00006465: no changes
|                      Gz    ESCAN00006379: no changes
|                      Gz    ESCAN00006850: no changes
|                      Gz    ESCAN00007005: no changes
|                      Gz    ESCAN00007006: Support for GENy
| 2003-11-17  2.42.00  Ap    ESCAN00007029: channel specific minimum sending distance
|                      Gz    ESCAN00007005: no changes
| 2004-01-09  2.43.00  Gz    ESCAN00007108: no changes
|                      Gz    ESCAN00007107: no changes
|                      Gz    ESCAN00007005: no changes
| 2004-01-15  2.44.00  Gz    ESCAN00007173: no changes
|                      Gz    ESCAN00007005: no changes
|                      Ap    ESCAN00007229: tTpCopyToCanInfoStruct_s not used with GENy
|                      Gz    ESCAN00007193: no changes
|                      Gz    ESCAN00007192: OEM specific preselection for ISO 15765_2_2 is wrong
|                      Gz    ESCAN00007191: no changes
|                      Gz    ESCAN00007098: no changes
|                      Ap    ESCAN00007257: no changes
|                      Gz    ESCAN00005679: The memory qualifier V_MEMROM0 has to be implemented in this module
|                      Gz    ESCAN00007273: Assertions should be able to configure independent from the CANdriver assertions
| 2004-01-30  2.45.00  Gz    ESCAN00007293: Additional memory qualifier should be added
|                      Ap    ESCAN00007349: no changes
|                      Gz    ESCAN00007318: In each transmission wrong data will be sent. (only 8bit controller)
| 2004-01-30  2.50.00  Ap    ESCAN00007341: ApplTpCopyToCAN parameter harmonisation for 8-bit controllers
|                      Ap    ESCAN00007392: no changes 
| 2004-02-05  2.51.00  Ap    ESCAN00007426: redefinition of macro __ApplTpTxFC
|                      Ap    ESCAN00007441: no changes
|                      Ap    ESCAN00007471: no changes
| 2004-03-02  2.52.00  Gz    ESCAN00007778: Compile error while using TP handle changeable
| 2004-03-12  2.53.00  Ap    ESCAN00007864: no changes
| 2004-03-19  2.54.00  Ap    ESCAN00007932: no changes
| 2004-03-23  2.55.00  Ap    ESCAN00007519: Support for CAN buffer access in ApplTpRxGetBuffer callback function
| 2004-04-02  2.56.00  Ap    ESCAN00007675: no changes
|                      Gz    ESCAN00008089: no changes
| 2004-04-06  2.57.00  Gz    ESCAN00008142: no changes
|                      Gz    ESCAN00008143: no changes
|                      Gz    ESCAN00008091: Diagnostic requests with DLC < 8 bytes are considered invalid
| 2004-04-23  2.58.00  Gz    ESCAN00008246: no changes
|                      Gz    ESCAN00007675: no changes
|                      Ap    ESCAN00008259: no changes
| 2004-05-13  2.60.00  Ap    ESCAN00007624: Support for mixed addressing 
|                      Ap    ESCAN00007831: MISRA: tpmc.h: Macro body not enclosed in ()
|                      Ap    ESCAN00008170: no changes
|                      Ap    ESCAN00008037: Variable DLC support
|                      Ap    ESCAN00008390: no changes
| 2004-05-25  2.61.00  Ap    ESCAN00008474: tTpCopyToCanInfoStruct defined twice for 8bit controllers
| 2004-05-26  2.62.00  Ap    ESCAN00008488: no changes
|                      Gz    ESCAN00008520: Add special routines for testing purpose
|                      Ap    ESCAN00008629: Preprocessor comparison with = instead of ==
|                      Ap    ESCAN00008630: no changes
| 2004-08-06  2.63.00  Gz    ESCAN00008520: Add special routines for testing purpose (2.nd)
|                      Gz    ESCAN00008778: no changes
|                      Gz    ESCAN00008983: no changes
|                      Gz    ESCAN00007440: Rx-Tx-inversion should be handled in each configurations
|                      Ap    ESCAN00009095: no changes
| 2004-06-18  2.64.00  Ap    ESCAN00009112: no changes
|                      Ap    ESCAN00009111: New default settings for TP_USE_PADDING
| 2004-08-23  2.65.00  Gz    ESCAN00009502: Remove of error directives due to new organi structure
|                      Ap    ESCAN00009153: no changes   
|                      Ap    ESCAN00009375: no changes   
|                      Ap    ESCAN00009511: Naming Conventions: New version defines
|                      Ap    ESCAN00004702: BS and STmin shall be ignored
| 2004-10-15  2.66.00  Ap    ESCAN00009865: no changes
| 2004-10-22  2.67.00  Ap    ESCAN00009979: no changes
|                      Ap    ESCAN00009797: Old naming of compiler switch "MULTIPLE_RECEIVE_BUFFER"
| 2005-01-11  2.68.00  Ap    ESCAN00010029: no changes
|                      Ap    ESCAN00010186: No libary functions should be used - VSTDLIB used (mendatory) instead  
|                      Ap    ESCAN00010770: Use of official CAN Driver API CanGetTxDataPtr
|                      Ap    ESCAN00010773: Support for multiple ECUs in combination with single optimization and Mixed29/Extended/NormalFixed
|                      Ap    ESCAN00010774: Support for Multiple Addressing with GENy
|                      Ap    ESCAN00010775: MISRA and pclint status and improvement request
|                      Ap    ESCAN00010776: no changes
|                      Ap    ESCAN00010777: Support for AddressingType specific PGN and Priority
|                      Ap    ESCAN00010781: no changes
| 2005-02-08  2.69.00  Ap    ESCAN00010829: Support for non ISO feature AcknowledgedConnection
|                      Ap    ESCAN00010831: Support for GatewayAPI on diagnostic functional requests
|                      Ap    ESCAN00011025: Availability of API function TpRxGetStatus extended (as documented in TechRef)
|                      Gz    ESCAN00011041: Extra OBD reception path has to be added
|                      Gz    ESCAN00011040: TpTransmit has to be accepted while TP is in minimum sending distance state
| 2005-02-11  2.70.00  Ap    ESCAN00010186: Compliency to CANGen and DBKOMGen  
| 2005-02-25  2.71.00  Ap    ESCAN00011124: no changes
|                      Ap    ESCAN00010186: No libary functions should be used - include of string.h removed
|                      Ap    ESCAN00011285: No changes
| 2005-03-16  2.72.00  Ap    ESCAN00011292: No changes
|                      Ap    ESCAN00011443: No changes
|                      Ap    ESCAN00011415: Support for IAR Compiler required
| 2005-08-11  2.73.00  Ap    ESCAN00011729: Undefined preprocessor define compared to value leading to compile error
|                      Ap    ESCAN00011738: No changes
|                      Ap    ESCAN00011897: Transmissions of FlowControl.Wait / suspend of reception (interface behaviour change required!)
|                      Ap    ESCAN00011924: No changes
|                      Ap    ESCAN00012015: Possible overlap of bitfield over word boundary
|                      Her   ESCAN00012778: Added dynamic handling of SN and FC strict checking
|                                           (TP_ENABLE_FC_MSG_FLOW_DYN_CHECK, TP_ENABLE_DYN_AWAIT_CORRECT_SN)
|                      Gz    ESCAN00012915: Change the functionality of the ApplTpCheckTA() callback function 
|                                           !!! Warning this version has a change in its behaviour !!!
|                      Ap    ESCAN00011586: No changes
|                      Ap    ESCAN00011117: No changes
|                      Her   ESCAN00013192: added dynamic handling of timeout values (N_As,N_Ar,N_Bs,N_Cr) 
|                                           (TP_ENABLE_DYN_CHANNEL_TIMING)
| 2005-08-22  2.74.00  Her   ESCAN00013222: MISRA compliancy (cancelled context switch)
| 2005-09-12  2.75.00  Ap    ESCAN00013489: Acceptance of functional FirstFrame
| 2005-09-25           Her   ESCAN00013590: no changes
| 2005-10-17  2.76.00  Her   ESCAN00013654: FirstFrame / SingleFrame acceptance to wrong parallel connection in multi addressing TP class
|                            ESCAN00013914: cleanup of compiler warnings 
|                            ESCAN00013995: added single channel multi connection multi CAN channels
|                                           (TP_ENABLE_SINGLE_CHAN_MULTICONN)
| 2005-11-10  2.77.00  Gz    ESCAN00014224: FC.overflow is not automatically supported
|                      Gz    ESCAN00013335: Wrong calculation of data length when receiving a first frame (only PIC18)
|                      Gz    ESCAN00014153: Compiler warning W1353C "assignment incompatible pointer types" occurs
|                      Gz    ESCAN00014000: CAN driver assertion is called in TpInitPowerOn()
|                      Gz    ESCAN00014015: Multiple ECU numbers in functional requests
|                      Gz    ESCAN00013054: memcpy() should used instead of VStdRamMemCpy()
| 2005-11-10  2.78.00  Gz    ESCAN00014444: Compile error: kTpFCSuppressFrame is undefined
| 2005-12-19  2.79.00  Gz    ESCAN00014547: Invalid call of "dynamic tx object set data pointer" macro under certain configuration.
| 2006-01-10  2.80.00  Gz    ESCAN00014790: no changes
|                      Gz    ESCAN00014838: no changes
|                      Gz    ESCAN00014839: no changes
| 2006-01-18  2.81.00  Her   ESCAN00014937: no changes
| 2006-01-31  2.82.00  Her   ESCAN00014888: Tx/Rx interrupt priority inversion re-released
| 2006-02-06           Her   ESCAN00015232: TP_ENABLE_FC_SUP[P]RESS backward compatibility
| 2006-02-15  2.83.00  Her   ESCAN00015346: no changes
| 2006-02-18  2.84.00  Her   ESCAN00013656: no changes
|                      Her   ESCAN00013658: disable functional addressing support (multiple addressing)
|                      Her   ESCAN00015368: replace string.h
|                      Her   ESCAN00015427: no changes
| 2006-02-27  2.85.00  Her   ESCAN00015472: no changes
| 2006-03-06  2.86.00  Her   ESCAN00015521: cleanup of compiler warnings, additional consistency check
|                      Her   ESCAN00015595: no changes
| 2006-04-06  2.87.00  Her   ESCAN00015988: no changes
|                      Her   ESCAN00016008: no changes
|                      Her   ESCAN00016050: TP is not configured on all CAN channels (kTpNumberOfCanChannels != kCanNumberOfChannel)
|                      Her   ESCAN00016055: no changes
| 2006-04-24  2.88.00  Her   ESCAN00016125: set TP_ENABLE_CHECKTA_COMPATIBILITY as default for old GenTool,
|                                           Misra compliancy adaptations
|                      Her   ESCAN00016214: no changes
|                      Her   ESCAN00016229: no changes
| 2006-06-16  2.89.00  Her   ESCAN00016608: no changes
|                      Her   ESCAN00016610: no changes
|                      Her   ESCAN00016611: no changes
|                      Her   ESCAN00016632: no changes
|                      Her   ESCAN00016639: no changes
|                      Her   ESCAN00016658: no changes
|                      Her   ESCAN00016678: no changes
|                      Her   ESCAN00016581: Support max. number of wait frames (N_WFTmax)
|                      Her   ESCAN00016821: no changes
|                      Her   ESCAN00016920: use macro VStdMemCpyRamToRam instead of function VStdRamMemCpy
| 2006-07-27  2.89.01  Her   ESCAN00017063: no changes
| 2006-08-10  2.89.02  Her   ESCAN00017206: no changes
|                      Her   ESCAN00017305: no changes
| 2006-10-12  2.89.03  EJW   ESCAN00018009: changes to support data buffers in far RAM
| 2006-10-18  2.89.04  Her   ESCAN00018058: no changes
|                      Her   ESCAN00018299: no changes
|                      Her   ESCAN00018508: Extend ApplTpCopyFromCan API with rxStruct for usage by GW 
|                      Her   ESCAN00018791: no changes
| 2007-01-04  2.90.00  Her   ESCAN00018236: separate SF/MF acknowledged connection
|                      Her   ESCAN00019126: no changes
|                      Her   ESCAN00019137: no changes
| 2007-02-19  2.91.00  Her   ESCAN00019597: Disable MF reception
| 2007-03-15  2.92.00  Her   ESCAN00019850: no changes
|                      Her   ESCAN00019928: no changes
|                      Her   ESCAN00020136: Add ASR CanIf interface for Static SingleTP use case
|                      Her   ESCAN00020427: no changes
| 2007-05-10  2.93.00  Her   ESCAN00020926: no changes
|                      Her   ESCAN00021048: Add ASR CanIf interface for Static SingleTP use case (functional connection group)
|                      Her   ESCAN00021049: no changes
| 2007-08-28  2.94.00  Her   ESCAN00021597: Redefinitions in mixed configurations (ASR/nonASR) solved 
|                      Her   ESCAN00022111: Support AUTOSAR post built functionality for transmitting PDU-IDs
|                      Her   ESCAN00022166: Set default value of TP_DISABLE_FC_WAIT
|                      Her   ESCAN00022281: no changes
|                      Her   ESCAN00022424: no changes
|                      Her   ESCAN00022582: no changes
| 2007-10-30  3.00.00  Her   ESCAN00023023: Dispatched Multi TP classes
|                      Her   ESCAN00022661: no changes
|                      Her   ESCAN00022683: no changes
|                      Her   ESCAN00023030: ASR - Single Channel TP used with multiple channels (TP_ENABLE_SINGLE_CHAN_MULTICONN)
| 2007-11-08  3.00.01  Her   ESCAN00023135: no changes
| 2007-11-20  3.01.00  Her   ESCAN00023262: no changes
|                      Her   ESCAN00022704: no changes
|                      Her   ESCAN00023482: no changes
| 2007-12-07  3.02.00  Her   ESCAN00023568: no changes
|                      Her   ESCAN00023572: API function TpTxGetTargetAddress for DISPATCHED Multi TP classes
| 2008-01-08  3.03.00  Her   ESCAN00023881: no changes
| 2008-01-09  3.04.00  Her   ESCAN00023959: Application buffer resides in paged (far)  memory
|                      Her   ESCAN00023972: added extended API for ApplTpGetRxBufferFct 
|                      Her   ESCAN00023991: API function TpFuncGetReceiveCanID returns a wrong type.
|                      Her   ESCAN00013328: no changes
|                      Her   ESCAN00024096: no changes
|                      Her   ESCAN00024024: TP messages sent on another CAN than expected
| 2008-01-25  3.04.01  Her   ESCAN00024227: new API function for functional requested multiframe responses 
| 2008-02-08  3.04.02  Her   ESCAN00024290: no changes
|                      Her   ESCAN00024448: no changes
|                      Her   ESCAN00024771: Wrong memory qualifier used
| 2008-02-28  3.04.03  Her   ESCAN00024960: no changes
|                      Her   ESCAN00024963: no changes
|                      Her   ESCAN00024845: no changes
|                      Her   ESCAN00025819: no changes
|                      Her   ESCAN00026052: no changes
******************************************************************************/
#ifndef OSEKTPMC_H
#define OSEKTPMC_H

#if defined (__cplusplus) /* enable C++ access to TPMC API */
extern "C" {
#endif
/* ##V_CFG_MANAGEMENT ##CQProject : Tp_Iso15765 CQComponent : Implementation */
#define TP_ISO15765_VERSION           0x0304
#define TP_ISO15765_RELEASE_VERSION   0x03
/* Old TPMC version defines */
#define OSEK_TRANSPORT_LAYER_VERSION         TP_ISO15765_VERSION
#define OSEK_TRANSPORT_LAYER_BUGFIX_VERSION  TP_ISO15765_RELEASE_VERSION
/* Set this value according to the actual version number of this module. */
/* The value is used to generate the Main- and Subversion number for diagnosis.*/

#define OSEKTP_C_MODULE /* Only for compatibility with tp_cfg.h */
/*Tool configurated header file for global and local usage*/
#include "tp_cfg.h" 



/******************************************************************************
* compatibility to older generation tool versions
******************************************************************************/
#if defined (TP_ENABLE_FC_SUPRESS)
# define TP_ENABLE_FC_SUPPRESS
#endif
#if defined (V_ENABLE_VSTDLIB)
# if !defined (VGEN_ENABLE_VSTDLIB)
#  define VGEN_ENABLE_VSTDLIB
# endif
#endif

#if defined (TP_USE_NORMAL_ADDRESSING)
# if (TP_USE_NORMAL_ADDRESSING == kTpOn)
#  ifndef TP_ENABLE_NORMAL_ADDRESSING
 #define TP_ENABLE_NORMAL_ADDRESSING
#  endif
# else /* (TP_USE_NORMAL_ADDRESSING == kTpOff) */
#  ifndef TP_DISABLE_NORMAL_ADDRESSING
 #define TP_DISABLE_NORMAL_ADDRESSING
#  endif
# endif
#else 
# if (!defined(TP_ENABLE_NORMAL_ADDRESSING) && !defined(TP_DISABLE_NORMAL_ADDRESSING))
 #define TP_DISABLE_NORMAL_ADDRESSING
# endif
#endif
#if defined (TP_USE_NORMAL_FIXED_ADDRESSING)
# if (TP_USE_NORMAL_FIXED_ADDRESSING == kTpOn)
 #define TP_ENABLE_NORMAL_FIXED_ADDRESSING
# else /* (TP_USE_NORMAL_FIXED_ADDRESSING == kTpOff) */
#  ifndef TP_DISABLE_NORMAL_FIXED_ADDRESSING
 #define TP_DISABLE_NORMAL_FIXED_ADDRESSING
#  endif
# endif
#else 
# if (!defined(TP_DISABLE_NORMAL_FIXED_ADDRESSING))
 #define TP_DISABLE_NORMAL_FIXED_ADDRESSING
# endif
#endif
#if defined (TP_USE_EXTENDED_ADDRESSING)
# if (TP_USE_EXTENDED_ADDRESSING == kTpOn)
 #define TP_ENABLE_EXTENDED_ADDRESSING
# else /* (TP_USE_EXTENDED_ADDRESSING == kTpOff) */
#  ifndef TP_DISABLE_EXTENDED_ADDRESSING
 #define TP_DISABLE_EXTENDED_ADDRESSING
#  endif
# endif
#else 
# if (!defined(TP_DISABLE_EXTENDED_ADDRESSING))
 #define TP_DISABLE_EXTENDED_ADDRESSING
# endif
#endif
#if defined (TP_USE_MIXED_29_ADDRESSING)
# if (TP_USE_MIXED_29_ADDRESSING == kTpOn)
 #define TP_ENABLE_MIXED_29_ADDRESSING
# else /* (TP_USE_MIXED_29_ADDRESSING == kTpOff) */
#  ifndef TP_DISABLE_MIXED_29_ADDRESSING
 #define TP_DISABLE_MIXED_29_ADDRESSING
#  endif
# endif
#else 
# if (!defined(TP_DISABLE_MIXED_29_ADDRESSING))
 #define TP_DISABLE_MIXED_29_ADDRESSING
# endif
#endif
#if defined (TP_USE_MIXED_11_ADDRESSING)
# if (TP_USE_MIXED_11_ADDRESSING == kTpOn)
 #define TP_ENABLE_MIXED_11_ADDRESSING
# else /* (TP_USE_MIXED_11_ADDRESSING == kTpOff) */
#  ifndef TP_DISABLE_MIXED_11_ADDRESSING
 #define TP_DISABLE_MIXED_11_ADDRESSING
#  endif
# endif
#else 
# if (!defined(TP_DISABLE_MIXED_11_ADDRESSING))
 #define TP_DISABLE_MIXED_11_ADDRESSING
# endif
#endif
#if defined (TP_USE_DYN_ID)
# if (TP_USE_DYN_ID == kTpOn)
#   define TP_ENABLE_DYNAMIC_CHANNELS
# else /* (TP_USE_DYN_ID == kTpOff) */
#  ifndef TP_DISABLE_DYNAMIC_CHANNELS
#   define TP_DISABLE_DYNAMIC_CHANNELS
#  endif
# endif
#else 
# if (!defined(TP_DISABLE_DYNAMIC_CHANNELS))
# error Static/Dynamic channel not specified
# endif
#endif

#if defined (TP_USE_VARIABLE_DLC)
# if (TP_USE_VARIABLE_DLC == kTpOn)
#  ifndef TP_ENABLE_VARIABLE_DLC
#   define TP_ENABLE_VARIABLE_DLC
#  endif
# else /* (TP_USE_VARIABLE_DLC == kTpOff) */
#  ifndef TP_DISABLE_VARIABLE_DLC
#   define TP_DISABLE_VARIABLE_DLC
#  endif
# endif
#endif

#if !defined( TP_USE_WAIT_FOR_CORRECT_SN )
# define TP_USE_WAIT_FOR_CORRECT_SN kTpOff
#else
# if (TP_USE_WAIT_FOR_CORRECT_SN == kTpOn)
#  if !defined (TP_ENABLE_WAIT_FOR_CORRECT_SN)
#   define TP_ENABLE_WAIT_FOR_CORRECT_SN
#  endif
# endif
#endif

#if !defined (TP_ENABLE_SINGLE_MSG_OBJ)
#else
# if (!defined (TP_ENABLE_CHECKTA_COMPATIBILITY) && !defined (TP_DISABLE_CHECKTA_COMPATIBILITY))
  /* check for GENy compatible version */
#  if ((TP_ISO15765DLL_VERSION == 0x0202) && (TP_ISO15765DLL_RELEASE_VERSION > 0x01)) || (TP_ISO15765DLL_VERSION > 0x0202)
   /* use new prototype version of ApplTpCheckTA */
#   define TP_DISABLE_CHECKTA_COMPATIBILITY
#  else
   /* no compatible GENy version detected - use compatibility mode */
#   define TP_ENABLE_CHECKTA_COMPATIBILITY
#  endif
# endif
#endif

/*******************************************************************************
* set switches to default
*******************************************************************************/
#ifndef kTpNumberOfCanChannels
# define kTpNumberOfCanChannels kCanNumberOfChannels
#endif
#if ( (defined (C_MULTIPLE_RECEIVE_CHANNEL)) )
#else
# if ( kTpNumberOfCanChannels > 1 ) /* more than one CanChannel ? */
#  define TP_CAN_CODEDOUBLED    
# endif
#endif
#if ((!defined (TP_DISABLE_DYNAMIC_CHANNELS)))
# define TP_DISABLE_DYNAMIC_CHANNELS
#endif

#if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
#else
# ifndef TP_ENABLE_MULTI_CHANNEL_TP
#  define TP_ENABLE_MULTI_CHANNEL_TP
# endif
#endif
# ifndef TP_DISABLE_MULTIPLE_ADDRESSING
#  define TP_DISABLE_MULTIPLE_ADDRESSING
# endif

#if defined (TP_ENABLE_ISO_15765_2_2)
#else
# ifndef TP_DISABLE_ISO_15765_2_2
#  define TP_DISABLE_ISO_15765_2_2
# endif
#endif

#ifndef TP_USE_PRE_DISPATCHED_MODE
# define TP_USE_PRE_DISPATCHED_MODE           kTpOff
#endif
#if (TP_USE_PRE_DISPATCHED_MODE == kTpOn)
# define TP_ENABLE_PRE_DISPATCHED_MODE
#else
# define TP_DISABLE_PRE_DISPATCHED_MODE
#endif

#ifndef TP_USE_CONNECTIONS 
 #define TP_USE_CONNECTIONS                   kTpOn
#endif
#ifndef TP_USE_FAST_PRECOPY
 #define TP_USE_FAST_PRECOPY                  kTpOff
#endif
#ifndef TP_USE_DIAGPRECOPY
 #define TP_USE_DIAGPRECOPY                   kTpOff
#endif
#ifndef TP_USE_CHANNEL_0_FOR_SPECIAL_PURPOSE
 #define TP_USE_CHANNEL_0_FOR_SPECIAL_PURPOSE kTpOff
#endif
#ifndef TP_USE_WAIT_FRAMES
 #define TP_USE_WAIT_FRAMES                   kTpOff
#endif

#if defined (TP_USE_EXTENDED_FIXED_ADDRESSING)
# if (TP_USE_EXTENDED_FIXED_ADDRESSING == kTpOn)
#  ifndef TP_ENABLE_EXTENDED_FIXED_ADDRESSING
 #define TP_ENABLE_EXTENDED_FIXED_ADDRESSING
#  endif
# else /* (TP_USE_EXTENDED_FIXED_ADDRESSING == kTpOff) */
#  ifndef TP_DISABLE_EXTENDED_FIXED_ADDRESSING
 #define TP_DISABLE_EXTENDED_FIXED_ADDRESSING
#  endif
# endif
#else
 #define TP_DISABLE_EXTENDED_FIXED_ADDRESSING
#endif

#if ( (!defined(TP_ENABLE_MCAN)) && (!defined(TP_DISABLE_MCAN)) )
# define TP_DISABLE_MCAN
#endif
#if ( (!defined(TP_ENABLE_MF_RECEPTION)) && (!defined(TP_DISABLE_MF_RECEPTION)) )
# define TP_ENABLE_MF_RECEPTION
#endif
#ifndef TP_USE_APPL_PRECOPY
 #define TP_USE_APPL_PRECOPY                  kTpOff
#endif
#ifndef TP_USE_GATEWAY_API
 #define TP_USE_GATEWAY_API                   kTpOff
#endif
#ifndef TP_USE_EXTENDED_API_STMIN
 #define TP_USE_EXTENDED_API_STMIN            kTpOff
#endif
#ifndef TP_USE_RX_CHANNEL_WITHOUT_FC
# define TP_USE_RX_CHANNEL_WITHOUT_FC         kTpOff
#endif
#ifndef TP_USE_TX_CHANNEL_WITHOUT_FC
# define TP_USE_TX_CHANNEL_WITHOUT_FC         kTpOff
#endif
#ifndef TP_USE_MULTIPLE_ECU_NR
# define TP_USE_MULTIPLE_ECU_NR               kTpOff
#endif
#ifndef TP_USE_MULTIPLE_ECU
# define TP_USE_MULTIPLE_ECU                  kTpOff
#endif
#ifndef TP_USE_MULTIPLE_BASEADDRESS
# define TP_USE_MULTIPLE_BASEADDRESS          kTpOff
#endif
#ifndef TP_USE_TX_ERROR_IND_COMPATIBILITY
# define TP_USE_TX_ERROR_IND_COMPATIBILITY    kTpOff
#endif
#ifndef TP_USE_TX_HANDLE_CHANGEABLE
# define TP_USE_TX_HANDLE_CHANGEABLE          kTpOff
#endif
#ifndef TP_USE_EXT_IDS_FOR_NORMAL
# define TP_USE_EXT_IDS_FOR_NORMAL            kTpOff
#endif

#ifndef TP_USE_STRICT_MSG_FLOW_CHECKING
/* 7.4.4.2  FlowStatus error handling
If a FlowControl N_PDU message is received with an invalid FlowStatus parameter value then
proper error handling shall take place in the network layer. The message transmission shall
be aborted, and the network layer shall make a N_USData.indication service call with
the parameter <N_Result> = N_INVALID_FS to the adjacent upper layer. */
# define TP_USE_STRICT_MSG_FLOW_CHECKING     kTpOn
#endif

#if (!defined (TP_ENABLE_FC_MSG_FLOW_DYN_CHECK) && !defined(TP_DISABLE_FC_MSG_FLOW_DYN_CHECK))
# define TP_DISABLE_FC_MSG_FLOW_DYN_CHECK
#endif

#if (!defined(TP_ENABLE_VARIABLE_DLC) && !defined(TP_DISABLE_VARIABLE_DLC))
# define TP_DISABLE_VARIABLE_DLC
#endif

#if (!defined(TP_DISABLE_IGNORE_CONTENT_FC))
# define TP_DISABLE_IGNORE_CONTENT_FC
#endif

#if !defined (TP_ENABLE_WAIT_FOR_CORRECT_SN) && !defined (TP_DISABLE_WAIT_FOR_CORRECT_SN)
# define TP_DISABLE_WAIT_FOR_CORRECT_SN
#endif

#if (!defined (TP_DISABLE_DYN_AWAIT_CORRECT_SN) && !defined(TP_ENABLE_DYN_AWAIT_CORRECT_SN))
# define TP_DISABLE_DYN_AWAIT_CORRECT_SN
#endif

#if !defined (TP_ENABLE_DYN_CHANNEL_TIMING) &&  !defined(TP_DISABLE_DYN_CHANNEL_TIMING)
#define TP_DISABLE_DYN_CHANNEL_TIMING
#endif

#if (!defined (TP_ENABLE_SINGLE_CHAN_MULTICONN) && !defined(TP_DISABLE_SINGLE_CHAN_MULTICONN))
# define TP_DISABLE_SINGLE_CHAN_MULTICONN
#endif

#if (!defined (TP_ENABLE_EXT_COPYFROMCAN_API) && !defined(TP_DISABLE_EXT_COPYFROMCAN_API))
# define TP_DISABLE_EXT_COPYFROMCAN_API
#endif

#ifndef TP_USE_OLD_STMIN_CALCULATION
# define TP_USE_OLD_STMIN_CALCULATION         kTpOff
#endif
#ifndef TP_USE_PRE_COPY_CHECK 
# define TP_USE_PRE_COPY_CHECK                kTpOff
#endif
#ifndef TP_USE_FAST_TX_TRANSMISSION
# define TP_USE_FAST_TX_TRANSMISSION          kTpOff
#endif
#ifndef TP_USE_ISO_COMPLIANCE
# define TP_USE_ISO_COMPLIANCE                kTpOff
#endif
#ifndef TP_USE_OVERRUN_INDICATION
# define TP_USE_OVERRUN_INDICATION            kTpOff
#endif
#ifndef TP_USE_TX_OF_FC_IN_ISR
# define TP_USE_TX_OF_FC_IN_ISR              kTpOn 
#endif
#ifndef TP_USE_QUEUE_IN_ISR
# define TP_USE_QUEUE_IN_ISR                  kTpOn 
#endif
#ifndef TP_USE_NO_STMIN_AFTER_FC
# define TP_USE_NO_STMIN_AFTER_FC             kTpOff
#endif
#ifndef TP_USE_VARIABLE_RX_DLC_CHECK 
# define TP_USE_VARIABLE_RX_DLC_CHECK         kTpOn
#endif
#ifndef TP_USE_FIX_RX_DLC_CHECK
# define TP_USE_FIX_RX_DLC_CHECK              kTpOff
#endif
#ifndef TP_USE_EXTENDED_API_BS
# define TP_USE_EXTENDED_API_BS               kTpOff
#endif
#ifndef TP_USE_ONLY_FIRST_FC
# define TP_USE_ONLY_FIRST_FC                 kTpOff
#endif

#ifndef TP_USE_WITHOUT_FC_TPCI_ADDON
# define TP_USE_WITHOUT_FC_TPCI_ADDON         kTpOff
#endif
#ifndef TP_USE_TP_TX_FC
# define TP_USE_TP_TX_FC                      kTpOff
#endif

#ifndef TP_HIGH_RX_LOW_TX_PRIORITY
# define TP_HIGH_RX_LOW_TX_PRIORITY           kTpOn
#endif


#if !defined (TP_USE_STMIN_OF_FC)
# define TP_USE_STMIN_OF_FC                   kTpOn
#endif

#if !defined (TP_USE_TP_INDICATION)
# define TP_USE_TP_INDICATION                 kTpOff
#endif

#if !defined (TP_USE_TP_RX_SF)
# define TP_USE_TP_RX_SF                      kTpOff
#endif

#if !defined (TP_USE_TP_RX_FF)
# define TP_USE_TP_RX_FF                      kTpOff
#endif

#if !defined (TP_USE_TP_RX_CF)
# define TP_USE_TP_RX_CF                      kTpOff
#endif

#if !defined (TP_USE_TP_RX_GET_BUFFER)
# define TP_USE_TP_RX_GET_BUFFER              kTpOff
#endif

#if !defined (TP_USE_TX_ERROR_INDICATION)
# define TP_USE_TX_ERROR_INDICATION           kTpOff
#endif

#if !defined (TP_USE_CUSTOM_TX_MEMCPY)
# define TP_USE_CUSTOM_TX_MEMCPY              kTpOff
#endif

#if !defined (TP_USE_CUSTOM_RX_MEMCPY)
# define TP_USE_CUSTOM_RX_MEMCPY              kTpOff
#endif

#if ( TP_USE_CUSTOM_RX_MEMCPY == kTpOn )
# if defined( TP_ENABLE_EXT_COPYFROMCAN_API )
#  if !defined (TP_ENABLE_SINGLE_MSG_OBJ )
#  endif
# endif
#endif

#if !defined (TP_USE_TP_CONFIRMATION)
# define TP_USE_TP_CONFIRMATION               kTpOff
#endif

#if !defined (TP_USE_TP_NOTIFY_TX)
# define TP_USE_TP_NOTIFY_TX                  kTpOff
#endif

#if !defined (TP_USE_TP_CAN_MESSAGE_TRANSMITTED)
# define TP_USE_TP_CAN_MESSAGE_TRANSMITTED    kTpOff
#endif

#if !defined (TP_USE_TP_CAN_MESSAGE_RECEIVED)
# define TP_USE_TP_CAN_MESSAGE_RECEIVED       kTpOff
#endif

#if ! ( defined(TP_ENABLE_FC_WAIT) || defined(TP_DISABLE_FC_WAIT) )
# define TP_DISABLE_FC_WAIT
#endif

#if ! ( defined(TP_ENABLE_FC_SUPPRESS) || defined(TP_DISABLE_FC_SUPPRESS) )
# define TP_ENABLE_FC_SUPPRESS
#endif

#if ! ( defined(TP_ENABLE_FC_OVERFLOW) || defined(TP_DISABLE_FC_OVERFLOW) )
#if defined (TP_ENABLE_ISO_15765_2_2)
#  define TP_ENABLE_FC_OVERFLOW
# else
#  define TP_DISABLE_FC_OVERFLOW
# endif
#endif

#if ! ( defined (TP_ENABLE_REQUEST_QUEUE) || defined(TP_DISABLE_REQUEST_QUEUE) )
# if defined (TP_ENABLE_MIN_TIMER)
#  define TP_ENABLE_REQUEST_QUEUE
# else
#  define TP_DISABLE_REQUEST_QUEUE
# endif
#endif

#if ! (defined (TP_ENABLE_OBD_PRECOPY) || defined(TP_DISABLE_OBD_PRECOPY) )
# define TP_DISABLE_OBD_PRECOPY
#endif


#if (TP_USE_PRE_COPY_CHECK == kTpOn)
# ifndef __ApplTpPreCopyCheckFunction
# define __ApplTpPreCopyCheckFunction(x)      1
# endif
#endif

#ifndef __ApplTpTxDelayFinished
# if defined (TP_ENABLE_SINGLE_MSG_OBJ)
#define __ApplTpTxDelayFinished(tpChannel, state)
# endif
#endif

#ifndef __ApplTpRxCanMessageTransmitted
#define __ApplTpRxCanMessageTransmitted(tpChannel)
#endif

#if defined (TP_DISABLE_DYNAMIC_CHANNELS)         && \
    (defined (TP_ENABLE_NORMAL_ADDRESSING)        )
#else
# ifndef __ApplTpTxFC
# define __ApplTpTxFC(tpChannel)
# endif
#endif

#if !defined (TP_USE_PADDING)
#  if defined (TP_ENABLE_USER_CHECK)
#   define TP_USE_PADDING                       kTpOn
#  else
#   define TP_USE_PADDING                       kTpOff
#  endif
#endif

#ifndef TP_PADDING_PATTERN 
# define TP_PADDING_PATTERN                    0xff
#endif

#ifndef TP_SECURITY_LEVEL
# define TP_SECURITY_LEVEL 2 /* highest Security class */
#endif

#ifndef TP_MEMORY_MODEL_DATA
# define TP_MEMORY_MODEL_DATA                  /* V_MEMRAM1_NEAR or V_MEMRAM1_FAR: has to be done via user_cfg */
#endif
#ifndef TP_API_CALL_TYPE
# define TP_API_CALL_TYPE 
#endif
#ifndef TP_API_CALLBACK_TYPE
# define TP_API_CALLBACK_TYPE 
#endif
#ifndef TP_INTERNAL_CALL_TYPE
# define TP_INTERNAL_CALL_TYPE 
#endif

#ifndef TP_EXTERNAL_INLINE
# define TP_EXTERNAL_INLINE
#endif
#ifndef TP_INTERNAL_INLINE
# define TP_INTERNAL_INLINE
#endif

#if defined (TP_TEST_ROUTINE_CONF_TIMEOUT)
/* Make test code harmless */
# if !defined (TpTestTxCheckForcingConfTimeout)
#  define TpTestTxCheckForcingConfTimeout()  (1)
# endif
# if !defined (TpTestRxCheckForcingConfTimeout)
#  define TpTestRxCheckForcingConfTimeout()  (1)
# endif
#endif

/* Mixed AUTOSAR configuration */

#if defined (TP_ENABLE_SINGLE_MSG_OBJ)
/* Define PGN */
/* Define Prio */
#endif

#if (TP_USE_FAST_PRECOPY == kTpOn) && (TP_USE_APPL_PRECOPY == kTpOn) 
# error Check TA function is not supported in case of optimized range check
#endif

/* Some micros support configurations where the software is configured in near model             */
/* but the CAN registers are in far memory. To support this the following switch must be enabled */
#if (!defined (TP_ENABLE_INTERMEDIATE_CANCHIP_BUFFER) && !defined (TP_DISABLE_INTERMEDIATE_CANCHIP_BUFFER))
# define TP_DISABLE_INTERMEDIATE_CANCHIP_BUFFER
#endif

/*******************************************************************************
* Consistency of switches
*******************************************************************************/
#if (TP_USE_EXT_IDS_FOR_NORMAL == kTpOn)
#  if (TP_USE_MIXED_IDS_FOR_NORMAL == kTpOn)
#   error "The parallel usage of mixed identifiers (11/29 bit) for normal addressing is not supported"
# endif
#endif

#if defined (TP_TX_ERROR_IND_COMPATIBILITY)
# error TP_TX_ERROR_IND_COMPATIBILITY is not supported any more
#endif 

#define kTpMaxCanChannels 7
/* CanChannel has 3 bits width */
#if( kCanNumberOfChannels > kTpMaxCanChannels )
# error "The number of CAN channels supported is limited to a maximum of 7"
#endif

#if !defined (C_ENABLE_EXTENDED_ID)
#endif
/* Addressing */

# if defined (TP_ENABLE_NORMAL_ADDRESSING)
#  if defined (TP_ENABLE_NORMAL_MIXED11_ADDRESSING)
#  else
#  endif
# endif





#if  (TP_USE_MULTIPLE_BASEADDRESS == kTpOn)
#  error multiple baseaddresses works only with extended addressing
#endif

/* TX: Dynamic Id */

#if (TP_USE_APPL_PRECOPY == kTpOn)                || \
    (TP_USE_MULTIPLE_ECU_NR == kTpOn)
# error GATEWAY_PRECOPY or MULTIPLE_ECU_NR only with EXTENDED_ADDRESSING, NORMAL_FIXED_ADDRESSING and MIXED_ADDRESSING
#endif 


#if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
# if (TP_USE_TX_CHANNEL_WITHOUT_FC == kTpOn)
# error Channel without FlowControl are not supported by SingleConnectionTP
# endif
#endif

#if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
# if (TP_USE_RX_CHANNEL_WITHOUT_FC == kTpOn)
# error Channel without FlowControl are not supported by SingleConnectionTP
# endif
#endif

#if defined (TP_ENABLE_VARIABLE_DLC)
# if (TP_USE_PADDING == kTpOn )
# error Padding cannot be combined with variable DLC. Deactivate one option.
# endif
#endif

#if defined(TP_USE_TX_ID_APPL_CHECK)
# if (TP_USE_TX_ID_APPL_CHECK == kTpOn)
# error Feature no longer supported.
# endif
#endif

#if defined (TP_ENABLE_REQUEST_QUEUE) &&  defined(TP_DISABLE_MIN_TIMER)
# error Re-transmission feature only available if observation of minimum sending distance is activated
#endif


#if defined (TP_ENABLE_DYN_AWAIT_CORRECT_SN) &&  !defined(TP_ENABLE_MULTI_CHANNEL_TP)
# error Dynamic SN check only available for multi channel configurations
#endif

#if defined (TP_ENABLE_DYN_CHANNEL_TIMING) 
# error Dynamic timings only available for dynamic multi channel configurations
#endif

#if defined ( TP_ENABLE_SINGLE_CHAN_MULTICONN )
# if defined ( TP_ENABLE_SINGLE_CHANNEL_TP ) && \
    (defined( TP_ENABLE_NORMAL_ADDRESSING ) ) 
# else
#   error Single TP channel with multiple CAN channels not supported
# endif
#endif

#if !defined( kTpWFTmax )
# if defined( TP_ENABLE_FC_WAIT )
#  define kTpWFTmax     0xFF
# else
#  define kTpWFTmax     0x01
# endif
#else
# if ( kTpWFTmax > 0 )
#  if !defined( TP_ENABLE_FC_WAIT )
#   define TP_ENABLE_FC_WAIT
#  endif
# else
#  if defined( TP_ENABLE_FC_WAIT )
#   undef  TP_ENABLE_FC_WAIT
#  endif
# endif
#endif

#if !defined( kTpTxWFTmax )
# if defined( TP_ENABLE_MCAN )
#  define kTpTxWFTmax     0x00
# else
#  define kTpTxWFTmax     0xFF
# endif
#endif

#if (!defined (TP_DISABLE_ACKNOWLEDGED_CONNECTION))
# define TP_DISABLE_ACKNOWLEDGED_CONNECTION
#endif

# define TP_DISABLE_SF_ACKNOWLEDGE

#if (!defined (TP_ENABLE_STRICT_DL_CHECK) && !defined (TP_DISABLE_STRICT_DL_CHECK))
# define TP_DISABLE_STRICT_DL_CHECK
#endif


/*******************************************************************************
* Transport protocol global result codes
*******************************************************************************/
#define kTpSuccess            0  /*Everythings fine*/
#define kTpFailed             1  /*Error*/
#define kTpBusy               3  /*tpTransmit while tp is running*/
#define kTpTxBufferUnderrun   4  /*Not enough data to send*/

/* ApplTpTxErrorIndication */
#define kTpHoldChannel        0
#define kTpFreeChannel        1

#define kTpNoChannel          0xFF
#if defined (TP_ENABLE_SINGLE_MSG_OBJ)
#define kTpTxChannel          0x00
#define kTpRxChannel          0x80
#endif

#define kTpChannelInHold      0x03
#define kTpChannelInUse       0x01
#define kTpChannelNotInUse    0x00


#if (defined(TP_ENABLE_FC_WAIT) || defined(TP_ENABLE_FC_SUPPRESS) || defined(TP_ENABLE_FC_OVERFLOW))
# define kTpFCClearToSend           0x00
#endif
#if defined (TP_ENABLE_FC_WAIT)
# define kTpFCStatusWait            0x01
#endif
#if defined (TP_ENABLE_FC_SUPPRESS)
# define kTpFCSuppressFrame         0x02
#endif
#if defined (TP_ENABLE_FC_OVERFLOW)
# define kTpFCStatusOverflow        0x03
#endif
/***************************************************************************/
/* error codes                                                             */
/***************************************************************************/
/* error numbers for ErrorCallback functions                               */

/* RX: */
#define kTpRxErrWrongSNreceived                       0x01
#define kTpRxErrTransmissionOfFCFailed                0x02
#define kTpRxErrCFTimeout                             0x03
#define kTpRxErrConfIntTimeout                        0x04
#define kTpRxErrFcCanIdIsMissing                      0x05
#define kTpRxErrFF_SFreceivedAgain                    0x06
#define kTpRxErrRxResetChannelIsCalled                0x07
#define kTpRxErrTpInitIsCalled                        0x08
#define kTpRxErrWFTmaxOverrun                         0x09
                                                      
/* TX: */                                             
#define kTpTxErrBufferUnderrun                        0x21
#define kTpTxErrConfIntTimeout                        0x22
#define kTpTxErrFCTimeout                             0x23
#define kTpTxErrTransmissionFailed                    0x24
#define kTpTxErrFCNotExpected                         0x25
#define kTpTxErrFCWrongFlowStatus                     0x26
#define kTpTxErrTxResetChannelIsCalled                0x27
#define kTpTxErrTpInitIsCalled                        0x28
#define kTpTxErrFCOverrun                             0x32
#define kTpTxErrSFExeedsFrameLength                   0x33
#define kTpTxErrWFTmaxOverrun                         0x34

/* error numbers for User Assertions 0x00-0xff */
#define kTpErrNoDynObjAtTpInit                        0x00
#define kTpErrChannelNrTooHigh                        0x01
#define kTpErrConnectionNrTooHigh                     0x02
#define kTpErrChannelNotInUse                         0x03
#define kTpErrWrongAddressingFormat                   0x04
#define kTpErrNoCanChannelFound                       0x05
#define kTpErrCommunicationIsRunning                  0x06
#define kTpErrChannelNotInPreTransmitState            0x07
#define kTpErrCanChannelNotSupported                  0x08
#define kTpErrMemCpyInvalidParameter                  0x09
#define kTpErrPrecopyCheckInvalidReturn               0x0A

/* ASSERT - TX: */
#define kTpTxErrChannelAlreadyInQueue                 0x11 
#define kTpTxErrNoFrameAtPretransmitSpecified         0x12
#define kTpTxErrWrongFrameAtPretransmitSpecified      0x13
#define kTpTxErrNoConfStateAtConfirmationSpecified    0x14
#define kTpTxErrWrongConfStateAtConfirmationSpecified 0x15
#define kTpTxErrFrameAlreadyInQueue                   0x16
#define kTpTxErrDatalengthTooHigh                     0x17
#define kTpTxErrChannelIsNotInUse                     0x18
#define kTpTxErrTransmissionInProgress                0x19
#define kTpTxErrSetResponseWithoutFc                  0x1A
#define kTpTxErrNoStateSpecified                      0x1B
#define kTpTxErrNotInWaitFCState                      0x1C
#define kTpTxErrTxFlagsInconsitent                    0x1D
#define kTpTxErrMinTimerIsInconsistent                0x1E

/* ASSERT - RX: */
#define kTpRxErrNoConfStateAtConfirmationSpecified    0x21
#define kTpRxErrWrongConfStateAtConfirmationSpecified 0x22
#define kTpRxErrChannelAlreadyInQueue                 0x23
#define kTpRxErrFrameAlreadyInQueue                   0x24
#define kTpRxErrNoChannelFree                         0x25
#define kTpRxErrCFnotAssignableToAChannel             0x26
#define kTpRxErrFCnotAssignableToAChannel             0x27
#define kTpRxErrSetResponseWithoutFc                  0x28
#define kTpRxErrNoStateSpecified                      0x29
#define kTpRxErrNotInWaitCFState                      0x2A
#define kTpRxErrChannelNotInState                     0x2B
#define kTpRxErrInconsistentSemaphore                 0x2C
#define kTpRxErrFCWaitCombinedWithNoFC                0x2D
#define kTpRxErrInconsistentFlowStatus                0x2E

/***************************************************************************/
/* constants                                                               */
/***************************************************************************/

V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2 kTpMainVersion;
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2 kTpSubVersion;
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2 kTpBugFixVersion;


/*******************************************************************************
* Typedef declarations
*******************************************************************************/
typedef enum 
{
  kTpFalse = 0,
  kTpTrue  = 1
} tpBool;

typedef vuint8 tTpConnectionHandle;

/*******************************************************************************
* Optimization for different CPU types
*******************************************************************************/
#if defined (C_CPUTYPE_8BIT)
typedef canuint8        canuintCPUtype;
typedef cansint8        cansintCPUtype;
#else
typedef unsigned int    canuintCPUtype;
typedef signed int      cansintCPUtype;
#endif

#if defined (C_CPUTYPE_8BIT)
# if (TpTxTimeoutFC < 255) &&\
    (TpRxTimeoutCF < 255) &&\
    (TpTxTransmitCF < 255) &&\
    (kTpTxConfirmationTimeout < 255) &&\
    (kTpRxConfirmationTimeout < 255) 
typedef canuint8 tTpEngineTimer;
# else
typedef canuint16 tTpEngineTimer;
# endif
#else
typedef canuint16 tTpEngineTimer;
#endif
/******************************************************************************/

#if defined (C_MULTIPLE_RECEIVE_CHANNEL) 
#else
# if ( kCanNumberOfChannels > 1 ) /* more than one CanChannel ? */

  typedef canuint8 CanChannelHandle;

  typedef struct 
  {
#if (TP_USE_GATEWAY_API == kTpOn)
    CanChipMsgPtr     pChipMsgObj; 
#endif   
    CanChipDataPtr    pChipData;
#if defined (C_ENABLE_EXTENDED_ID)
    canuint32         CanRxActualId;
#else
    canuint16         CanRxActualId;
#endif
    canuint8          CanRxActualDLC;
    CanChannelHandle  Channel;
  } tCanRxInfoStruct;

  typedef tCanRxInfoStruct          *CanRxInfoStructPtr;

# endif
#endif

typedef union 
{
  CanChipDataPtr DataCanBufferPtr;
  TP_MEMORY_MODEL_DATA canuint8 * DataApplBufferPtr; 
} tTpBufferPtr;

typedef struct 
{
  tTpBufferPtr DataBufferPtr;
/* Description : List of pointer to receive buffers from the application      */
/******************************************************************************/
  canuint16 DataIndex;
/* Description : Index to transmit data - used instead of DataSegments        */
/* Val. range / Coding: 0..4095                                               */
/******************************************************************************/
  canuint16 DataLength;
/* Description : Length of data to be transmitted. The size of this variable  */
/*               limits the maximum size of messages.                         */
/* Val. range / Coding: 0..4095                                               */
/******************************************************************************/
#if (TP_USE_MULTIPLE_ECU_NR == kTpOn)
  canuint8 EcuNumber;
/* Description :                                                              */
/* Val. range / Coding: 0..0xff                                               */
/******************************************************************************/
#endif
#if defined(TP_ENABLE_MF_RECEPTION)
# if (TP_USE_EXTENDED_API_BS == kTpOn)
  canuint8 BlockSize;
/* Description :                                                              */
/* Val. range / Coding: 0..0xff                                               */
/******************************************************************************/
# endif
# if (TP_USE_EXTENDED_API_STMIN == kTpOn)
  canuint8 STMin;
/* Description :                                                              */
/* Val. range / Coding: 0..0xff                                               */
/******************************************************************************/
# endif
#endif
#if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
#else
  canuint8  Connection;
/* Description :   Indirection Table to store the ConnectionNr                */
/* Val. range / Coding: 0..255                                                */
/******************************************************************************/
#endif
#if defined(TP_ENABLE_MF_RECEPTION)
# if defined (TP_ENABLE_FC_WAIT)
#  if defined (TP_ENABLE_NORMAL_ADDRESSING)       
  canuint8 FFDataBuffer[6];
#  else
  canuint8 FFDataBuffer[5];
#  endif
/* Description : Non ISO feature: acknowledged connections                    */
/* with FC after last CF                                                      */
/* Val. range / Coding: 0..1                                                  */
/******************************************************************************/
# endif
#endif
  canbittype sequencenumber:4; /*lint !e46*/
/* Description : SequenceNumber SN                                            */
/* Val. range / Coding: 0..0xf                                                */
/******************************************************************************/

# if (kTpNumberOfCanChannels > 1)
  canbittype CanChannel:3;    /*lint !e46*/
/* Description : For a MultiCAN System, the physical CanChannel               */
/*               have to be stored in RAM for being changed by the            */
/*               application. To support this, the additional functions       */
/*               TpTxSetCanChannel and TpRxGetCanChannel are provided.        */
/* Val. range / Coding: 0..7                                                  */
/******************************************************************************/
  canbittype stuffbits0:1;
# else
  canbittype stuffbits1:4;
# endif

#if (defined(TP_ENABLE_FC_WAIT) || defined(TP_ENABLE_FC_SUPPRESS) || defined(TP_ENABLE_FC_OVERFLOW))
  canbittype ApplGetBufferStatus:2;        /*lint !e46*/
/* Description : Status flag to proceed SF/FF reception  */
/*                                                       */
/* Val. range / Coding: 0..3                                                  */
/******************************************************************************/
#endif
#if defined(TP_ENABLE_MF_RECEPTION)
# if (TP_USE_RX_CHANNEL_WITHOUT_FC == kTpOn)
  canbittype withoutFC:1;       /*lint !e46*/
/* Description : Channel without using FlowControls                           */
/*               (Static:GenTool provides no send message for this tpChannel) */
/* Val. range / Coding: 0..1                                                  */
/******************************************************************************/
# endif
#endif
#if defined (TP_ENABLE_DYN_AWAIT_CORRECT_SN)
  canbittype SNwait:1;       /*lint !e46*/
/* Description : Wait for correct SN instead of reinitialization              */
/* Val. range / Coding: 0 = strict SN check, 1 = wait for correct SN          */
/******************************************************************************/
#endif
#if (TP_USE_APPL_PRECOPY == kTpOn) && (TP_USE_FAST_PRECOPY == kTpOff) && defined (TP_DISABLE_CHECKTA_COMPATIBILITY)
  canbittype ta_type:1;
#endif

} tTpRxInfoStruct;

typedef struct 
{
  TP_MEMORY_MODEL_DATA canuint8 *DataBufferPtr;
/* Description : List of pointer to transmit buffers from the application     */
/******************************************************************************/
  canuint16 DataIndex;
/* Description : Index to transmit data - used instead of DataSegments        */
/* Val. range / Coding: 0..4095                                               */
/******************************************************************************/
  canuint16 DataLength;
/* Description : Length of data to be transmitted. The size of this variable  */
/*               limits the maximum size of messages.                         */
/* Val. range / Coding: 0..4095                                               */
/******************************************************************************/
#if (TP_USE_MULTIPLE_ECU_NR == kTpOn)
  canuint8 EcuNumber;
/* Description :                                                              */
/* Val. range / Coding: 0..                                                   */
/******************************************************************************/
#endif
#if (TP_USE_ONLY_FIRST_FC == kTpOn)
  canuint8 BlockSize;
/* Description :                                                              */
/* Val. range / Coding: 0..                                                   */
/******************************************************************************/
#endif
#if (TP_USE_STMIN_OF_FC == kTpOn)
  canuint8 STMin;
/* Description : Global value to store the requested min. separation time     */ 
/*               received in the flow control frames. The received STmin      */
/*               value will be adjusted to multiples of TpTxCallCycle.        */
/* Val. range / Coding: 0..255 / in ms                                        */
/******************************************************************************/
# if (TP_USE_FAST_TX_TRANSMISSION == kTpOn)
  canuint8 STminInFrame;
# endif
#endif
#if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
#else
  canuint8  Connection;
/* Description :   Indirection Table to store the ConnectionNr                */
/* Val. range / Coding: 0..255                                                */
/******************************************************************************/
#endif
  canbittype sequencenumber:4;    /*lint !e46*/
/* Description : SequenceNumber SN                                            */
/* Val. range / Coding: 0..0xf                                                */
/******************************************************************************/
#if defined (TP_ENABLE_VARIABLE_DLC)
  canbittype CurrentTpCanDLC:4;
/* Description : Stores the DLC while sending with variable DLC               */
/* Val. range / Coding: 1..4                                                  */
/******************************************************************************/
#else
  canbittype stuffbits1:4;
#endif

#if (kTpNumberOfCanChannels > 1)
#endif
#if (TP_USE_TX_CHANNEL_WITHOUT_FC == kTpOn)
  canbittype withoutFC:1;       /*lint !e46*/
/* Description : Channel without waiting for FlowControls                     */
/* Val. range / Coding: 0..1                                                  */
/******************************************************************************/
#endif
#if defined (TP_ENABLE_FC_MSG_FLOW_DYN_CHECK)
  canbittype FCwfs:1;       /*lint !e46*/
/* Description : Strict Flow Control checking for wrong flow status           */
/* Val. range / Coding: 0 = non strict check, 1 = strict check                */
/******************************************************************************/
#endif
#if defined (TP_ENABLE_RUNTIME_CHECK)
# if (TP_USE_MULTIPLE_ECU_NR == kTpOn)
  canbittype EcuNumberIsValid:1;        /*lint !e46*/
/* Description : Stores validity for member 'EcuNumber'                       */
/* Val. range / Coding: 0..1                                                  */
/******************************************************************************/
# endif
#endif
} tTpTxInfoStruct;

/***************************************************************************/
/* Callback Functions                                                      */
/***************************************************************************/
#if defined (TP_ENABLE_USER_CHECK)                || \
    defined (TP_ENABLE_INTERNAL_CHECK)            || \
    defined (TP_ENABLE_GEN_CHECK)
extern void     TP_API_CALLBACK_TYPE ApplTpFatalError(canuint8 errorNumber);
#endif

#if (TP_USE_PRE_COPY_CHECK == kTpOn)
# if defined (C_MULTIPLE_RECEIVE_CHANNEL)         || \
     defined (C_SINGLE_RECEIVE_CHANNEL)           
extern canuint8 TP_API_CALLBACK_TYPE TpPreCopyCheckFunction(CanRxInfoStructPtr rxStruct);
#else
#  if defined (C_MULTIPLE_RECEIVE_BUFFER)
extern canuint8 TP_API_CALLBACK_TYPE TpPreCopyCheckFunction(CanChipDataPtr rxDataPtr);
#  endif
#  if defined (C_SINGLE_RECEIVE_BUFFER)
extern canuint8 TP_API_CALLBACK_TYPE TpPreCopyCheckFunction(CanReceiveHandle rxObject);
#  endif
# endif
#endif


/*******************************************************************************
* Functions declarations 
*******************************************************************************/
#if defined (C_ENABLE_MEMCOPY_SUPPORT)
#else
# if defined (TP_ENABLE_INTERMEDIATE_CANCHIP_BUFFER)
# else
#  if defined (VGEN_ENABLE_VSTDLIB)
#  else
extern void     TP_API_CALL_TYPE TpIntMemCpy(TP_MEMORY_MODEL_DATA void *pDest, TP_MEMORY_MODEL_DATA void* pSrc, canuint16 nCnt);
#  endif
# endif
#endif

extern void     TP_API_CALL_TYPE TpInitPowerOn(void);
extern void     TP_API_CALL_TYPE TpInit(void);

#if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
#else
# if ( kTpNumberOfCanChannels > 1 ) /* more than one canChannel ? */
# endif
#endif

extern void     TP_API_CALL_TYPE TpTask(void);
extern void     TP_API_CALL_TYPE TpTxTask(void);
extern void     TP_API_CALL_TYPE TpRxTask(void);


/* Single Channel API */
#if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
  extern canuint8 TP_API_CALL_TYPE TpTransmit(TP_MEMORY_MODEL_DATA canuint8* pdata, canuint16 count);
  extern void     TP_API_CALL_TYPE TpTxResetChannel(void);

# if (kTpNumberOfCanChannels > 1)
# endif
# if (TP_USE_MULTIPLE_ECU_NR == kTpOn)
  extern void     TP_API_CALL_TYPE TpTxSetEcuNumber(canuint8 ecuNr);
# endif
# if defined (TP_DISABLE_NORMAL_ADDRESSING)
  extern void     TP_API_CALL_TYPE TpTxSetResponse(void);
# else
#  define TpTxSetResponse() 
# endif

  extern void     TP_API_CALL_TYPE TpRxResetChannel(void);  
  extern canuint8 TP_API_CALL_TYPE TpRxGetStatus(void);
# if (TP_USE_EXTENDED_API_BS == kTpOn)
  extern void     TP_API_CALL_TYPE TpRxSetBS(canuint8 newBS);
  extern canuint8 TP_API_CALL_TYPE TpRxGetBS(void);
# endif
#if (TP_USE_APPL_PRECOPY == kTpOn) && (TP_USE_FAST_PRECOPY == kTpOff) && defined (TP_DISABLE_CHECKTA_COMPATIBILITY)
  extern t_ta_type TP_API_CALL_TYPE TpRxGetTaType(void);
# endif
# if (TP_USE_EXTENDED_API_STMIN == kTpOn)
  extern void     TP_API_CALL_TYPE TpRxSetSTMIN(canuint8 newSTmin);
  extern canuint8 TP_API_CALL_TYPE TpRxGetSTMIN(void);
# endif
# if (TP_USE_MULTIPLE_ECU_NR == kTpOn)
  extern canuint8 TP_API_CALL_TYPE TpRxGetEcuNumber(void);
# endif
# if (TP_USE_FAST_TX_TRANSMISSION == kTpOn)
  extern TP_EXTERNAL_INLINE canuint8 TP_API_CALL_TYPE TpTxPrepareSendImmediate(void);
  extern TP_EXTERNAL_INLINE void TP_API_CALL_TYPE TpTxSendImmediate(void);
  extern canuint8 TP_API_CALL_TYPE TpTxGetSTminInFrame(void);
# endif

#  if (kTpNumberOfCanChannels > 1)
  extern canuint8 TP_API_CALL_TYPE TpRxGetCanChannel(void);
#  else
#  define TpRxGetCanChannel() (TP_CAN_CHANNEL_INDEX)
#  endif
  extern CanChipDataPtr TP_API_CALL_TYPE TpRxGetCanBuffer(void);

# if (TP_USE_OVERRUN_INDICATION == kTpOn)
  extern void TP_API_CALL_TYPE TpRxSetBufferOverrun(void);
# endif
# if defined (TP_TEST_ROUTINE_CONF_TIMEOUT)
  extern void TP_API_CALL_TYPE TpTxTestForceConfirmationTimeout(void);
  extern void TP_API_CALL_TYPE TpRxTestForceConfirmationTimeout(void);
# endif
#if defined (TP_ENABLE_FC_WAIT)
  extern void     TP_API_CALL_TYPE TpRxSetClearToSend(TP_MEMORY_MODEL_DATA canuint8 * pBuffer);
#endif
#if (defined (TP_ENABLE_FC_WAIT) || defined(TP_ENABLE_FC_SUPPRESS) || defined(TP_ENABLE_FC_OVERFLOW))
  extern void     TP_API_CALL_TYPE TpRxSetFCStatus(canuint8 FCStatus);
  extern canuint8 TP_API_CALL_TYPE TpRxGetFCStatus(void);
#endif



#else /* Multi Channel API */

  extern canuint8 TP_API_CALL_TYPE TpTransmit                   (canuint8 tpChannel, TP_MEMORY_MODEL_DATA canuint8 *pdata, canuint16 count);

  extern void     TP_API_CALL_TYPE TpTxResetChannel             (canuint8 tpChannel);
#  define TpTxGetConnectionStatus(connection)                   (connection)

# if (kTpNumberOfCanChannels > 1)
# endif
# if (TP_USE_MULTIPLE_ECU_NR == kTpOn)
  extern void     TP_API_CALL_TYPE TpTxSetEcuNumber             (canuint8 tpChannel, canuint8 ecuNr);
# endif

  extern void     TP_API_CALL_TYPE TpRxResetChannel             (canuint8 tpChannel);
  extern canuint8 TP_API_CALL_TYPE TpRxGetStatus                (canuint8 tpChannel);
                                                              
# if (TP_USE_EXTENDED_API_BS == kTpOn)                        
  extern void     TP_API_CALL_TYPE TpRxSetBS                    (canuint8 tpChannel, canuint8 newBS);
  extern canuint8 TP_API_CALL_TYPE TpRxGetBS                    (canuint8 tpChannel);
# endif                                                       
# if (TP_USE_EXTENDED_API_STMIN == kTpOn)                     
  extern void     TP_API_CALL_TYPE TpRxSetSTMIN                 (canuint8 tpChannel, canuint8 newSTmin);
  extern canuint8 TP_API_CALL_TYPE TpRxGetSTMIN                 (canuint8 tpChannel);
# endif                                                       
#if (TP_USE_APPL_PRECOPY == kTpOn) && (TP_USE_FAST_PRECOPY == kTpOff) && defined (TP_DISABLE_CHECKTA_COMPATIBILITY)
  extern t_ta_type TP_API_CALL_TYPE TpRxGetTaType               (canuint8 tpChannel);
# endif
# if (kTpNumberOfCanChannels > 1)
  extern canuint8 TP_API_CALL_TYPE TpRxGetCanChannel            (canuint8 tpChannel);
# else
#  define TpRxGetCanChannel(tpChannel)                          (TP_CAN_CHANNEL_INDEX)
# endif
# if (TP_USE_MULTIPLE_ECU_NR == kTpOn)
  extern canuint8 TP_API_CALL_TYPE TpRxGetEcuNumber             (canuint8 tpChannel);
# endif

#if defined( TP_ENABLE_MF_RECEPTION )
#endif

# if defined (TP_DISABLE_NORMAL_ADDRESSING)
  extern void     TP_API_CALL_TYPE TpTxSetResponse              (canuint8 rxChannel, canuint8 txChannel);
# else
#  define TpTxSetResponse(rxChannel, txChannel)
# endif
# if (TP_USE_FAST_TX_TRANSMISSION == kTpOn)
  extern TP_EXTERNAL_INLINE canuint8 TP_API_CALL_TYPE TpTxPrepareSendImmediate(canuint8 tpChannel);
  extern TP_EXTERNAL_INLINE void TP_API_CALL_TYPE TpTxSendImmediate(canuint8 tpChannel);
  extern canuint8 TP_API_CALL_TYPE TpTxGetSTminInFrame(canuint8 tpChannel);
# endif

  extern CanChipDataPtr TP_API_CALL_TYPE TpRxGetCanBuffer(canuint8 tpChannel);

# if (TP_USE_OVERRUN_INDICATION == kTpOn)
  extern void TP_API_CALL_TYPE TpRxSetBufferOverrun(canuint8 tpChannel);
# endif
# if defined (TP_TEST_ROUTINE_CONF_TIMEOUT)
  extern void TP_API_CALL_TYPE TpTxTestForceConfirmationTimeout(canuint8 tpChannel);
  extern void TP_API_CALL_TYPE TpRxTestForceConfirmationTimeout(canuint8 tpChannel);
# endif

#if defined (TP_ENABLE_FC_WAIT)
  extern void     TP_API_CALL_TYPE TpRxSetClearToSend(canuint8 tpChannel, TP_MEMORY_MODEL_DATA canuint8 * pBuffer);
#endif
#if (defined (TP_ENABLE_FC_WAIT) || defined(TP_ENABLE_FC_SUPPRESS) || defined(TP_ENABLE_FC_OVERFLOW))
  extern void     TP_API_CALL_TYPE TpRxSetFCStatus(canuint8 tpChannel, canuint8 FCStatus);
  extern canuint8 TP_API_CALL_TYPE TpRxGetFCStatus(canuint8 tpChannel);
#endif

#if defined (TP_ENABLE_DYN_AWAIT_CORRECT_SN)
extern void TP_API_CALL_TYPE TpRxSetWaitCorrectSN(canuint8 tpChannel, tpBool wait);
#endif

#if defined (TP_ENABLE_FC_MSG_FLOW_DYN_CHECK)
extern void TP_API_CALL_TYPE TpTxSetStrictFlowControlCheck(canuint8 tpChannel, tpBool strict);
#endif

#if defined (TP_ENABLE_DYN_CHANNEL_TIMING)
extern void TP_API_CALL_TYPE TpRxSetTimeoutCF(canuint8 tpChannel, tTpEngineTimer time);
extern void TP_API_CALL_TYPE TpRxSetTimeoutConfirmation(canuint8 tpChannel, tTpEngineTimer time);
extern void TP_API_CALL_TYPE TpTxSetTimeoutFC(canuint8 tpChannel, tTpEngineTimer time);
extern void TP_API_CALL_TYPE TpTxSetTimeoutConfirmation(canuint8 tpChannel, tTpEngineTimer time);
#endif

#endif /* End of Multi Channel API */



#if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
extern void TP_API_CALL_TYPE TpTxStateTask(void);
# define TpTxAllStateTask()     (TpTxStateTask())
#else
extern void TP_API_CALL_TYPE TpTxStateTask(canuint8 tpChannel);
extern void TP_API_CALL_TYPE TpTxAllStateTask(void);
#endif
#if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
extern void TP_API_CALL_TYPE TpRxStateTask(void);
# define TpRxAllStateTask()     (TpRxStateTask())
#else
extern void TP_API_CALL_TYPE TpRxStateTask(canuint8 tpChannel);
extern void TP_API_CALL_TYPE TpRxAllStateTask(void);
#endif

#if defined (TP_ENABLE_VARIABLE_DLC)
#  if defined (TP_ENABLE_SINGLE_MSG_OBJ) /* Geny */
#   if defined(C_MULTIPLE_RECEIVE_CHANNEL) || defined(C_SINGLE_RECEIVE_CHANNEL)
#    if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
extern canuint8 TP_INTERNAL_CALL_TYPE TpTxPreTransmit(CanTxInfoStruct txStruct);
#    else
extern canuint8 TP_INTERNAL_CALL_TYPE TpDrvTxPreTransmit(vuintx tpChannel, CanTxInfoStruct txStruct);
#    endif
#   else
#    if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
extern canuint8 TP_INTERNAL_CALL_TYPE TpTxPreTransmit(CanChipDataPtr txDataPtr);
#    else
extern canuint8 TP_INTERNAL_CALL_TYPE TpDrvTxPreTransmit(vuintx tpChannel, CanChipDataPtr txDataPtr);
#    endif
#   endif
#  endif
#endif


/*******************************************************************************
* Variable declarations
*******************************************************************************/
#if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
#else
#  if defined (TP_ENABLE_NORMAL_ADDRESSING)        
/* Added for multichannel transport layer extension */
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2 TpRxHandleToChannel[];
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2 TpTxHandleToChannel[];
#  endif
#endif

extern MEMORY_NEAR_TP_SAVE tTpRxInfoStruct tpRxInfoStruct[kTpRxChannelCount];
extern MEMORY_NEAR_TP_SAVE tTpTxInfoStruct tpTxInfoStruct[kTpTxChannelCount];

struct tTpCopyToCanInfoStruct_s
{
  canuint8                       Channel;     /* TP Channel                           */
  TxDataPtr                      pDestination;/* Pointer to destination buffer        */
  TP_MEMORY_MODEL_DATA canuint8* pSource;     /* Pointer to source buffer (if linear) */
  canuint16                      Length;      /* The maximum length to copy           */
};

#if (TP_USE_TX_HANDLE_CHANGEABLE == kTpOn)
# if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
extern MEMORY_NEAR_TP_SAVE CanTransmitHandle tpTxHandle;
#  if defined (TP_ENABLE_SINGLE_MSG_OBJ)
#  endif
# else
extern MEMORY_NEAR_TP_SAVE CanTransmitHandle tpTxHandle_Field[kTpTxChannelCount];
#  if defined (TP_ENABLE_SINGLE_MSG_OBJ)
#  endif
# endif
#endif

#if defined (TP_ENABLE_MIN_TIMER) 
# if !defined (kTpTxMinTimer)
V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2 TpTxMinTimer[kTpTxChannelCount];
# endif
#endif





/*******************************************************************************
* Variable declarations
*******************************************************************************/
# if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
# else
/*******************************************************************************
* NAME:              TpRxGetConnectionNumber
*
* CALLED BY:         Application
* PRECONDITIONS:     
*
* PARAMETER:         
* RETURN VALUE:      
*******************************************************************************/
#  define TpRxGetConnectionNumber(tpChannel) (tpRxInfoStruct[tpChannel].Connection)
# endif

# if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
# else
/*******************************************************************************
* NAME:              TpRxSetConnectionNumber
*
* CALLED BY:         Application
* PRECONDITIONS:     
*
* PARAMETER:         
* RETURN VALUE:      
*******************************************************************************/
#  define TpRxSetConnectionNumber(tpChannel, connection) (tpRxInfoStruct[(tpChannel)].Connection = (connection))
# endif

/*******************************************************************************
* NAME:              TpTxGetDataBuffer/TpRxGetDataBuffer
*
* CALLED BY:         Application
* PRECONDITIONS:     
*
* PARAMETER:         
* RETURN VALUE:      
*******************************************************************************/
#if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
# define TpTxGetDataBuffer()         (tpTxInfoStruct[0].DataBufferPtr)
# define TpRxGetDataBuffer()         (tpRxInfoStruct[0].DataBufferPtr.DataApplBufferPtr)
#else
# define TpTxGetDataBuffer(tpChannel)  (tpTxInfoStruct[tpChannel].DataBufferPtr)
# define TpRxGetDataBuffer(tpChannel)  (tpRxInfoStruct[tpChannel].DataBufferPtr.DataApplBufferPtr)
#endif
/*******************************************************************************
* NAME:              TpTxGetDataIndex/TpRxGetDataIndex
*
* CALLED BY:         Application
* PRECONDITIONS:     
*
* PARAMETER:         
* RETURN VALUE:      
*******************************************************************************/
#if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
# define TpTxGetDataIndex()          (tpTxInfoStruct[0].DataIndex)
# define TpRxGetDataIndex()          (tpRxInfoStruct[0].DataIndex)
#else
# define TpTxGetDataIndex(tpChannel)   (tpTxInfoStruct[tpChannel].DataIndex)
# define TpRxGetDataIndex(tpChannel)   (tpRxInfoStruct[tpChannel].DataIndex)
#endif

#if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
#else
/*******************************************************************************
* NAME:              TpTxGetConnectionNumber
*
* CALLED BY:         Application
* PRECONDITIONS:     
*
* PARAMETER:         
* RETURN VALUE:      
*******************************************************************************/
#  define TpTxGetConnectionNumber(tpChannel) (tpTxInfoStruct[tpChannel].Connection)
#endif


/*******************************************************************************
* NAME:              TpTxData
*
* CALLED BY:         Application
* PRECONDITIONS:     
*
* PARAMETER:         
* RETURN VALUE:      
*******************************************************************************/
#if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
#  define TpTxData()          (CanGetTxDataPtr(kTpTxHandle_Field[0]))
#else
#  define TpTxData(tpChannel)    (CanGetTxDataPtr(kTpTxHandle_Field[tpChannel]))
#endif

#if defined (TP_ENABLE_EXTENDED_FIXED_ADDRESSING)
/*******************************************************************************
* NAME:              TpTxSetPriority
*
* CALLED BY:         Application
* PRECONDITIONS:     
*
* PARAMETER:         
* RETURN VALUE:      
*******************************************************************************/
# define TpTxSetPriority(tpChannel, priority)  (( tpTxCanIdHigh[tpChannel] &= 0x00ff), \
                                                tpTxCanIdHigh[tpChannel] |= ((canuint8)priority)<<8)
#endif

#if defined (TP_ENABLE_EXTENDED_FIXED_ADDRESSING)
/*******************************************************************************
* NAME:              TpTxSetParameterGroupIdentification
*
* CALLED BY:         Application
* PRECONDITIONS:     
*
* PARAMETER:         
* RETURN VALUE:      
*******************************************************************************/
# define TpTxSetParameterGroupIdentification (tpChannel, identification) (( tpTxCanIdHigh[tpChannel] &= 0xff00), \
                                                                          tpTxCanIdHigh[tpChannel] |= (canuint8)identification)
#endif

#if defined (TP_ENABLE_EXTENDED_FIXED_ADDRESSING)
/*******************************************************************************
* NAME:              TpRxGetParameterGroupIdentification
*
* CALLED BY:         Application
* PRECONDITIONS:     
*
* PARAMETER:         
* RETURN VALUE:      
*******************************************************************************/
# define TpRxGetParameterGroupIdentification  ( (canuint8) tpTxCanIdHigh[tpChannel] )
#endif


#if (TP_USE_TX_HANDLE_CHANGEABLE == kTpOn)
# if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
/*******************************************************************************
* NAME:              TpSetTxHandle
*
* CALLED BY:         Application
* PRECONDITIONS:     
*
* PARAMETER:         
* RETURN VALUE:      
*******************************************************************************/
#  define TpSetTxHandle(x) (tpTxHandle = x)
#  if defined (TP_ENABLE_SINGLE_MSG_OBJ)
#  endif
# else
#   error "Not supported configuration!"
#  if defined (TP_ENABLE_SINGLE_MSG_OBJ)
#  endif
# endif
#endif


#if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
#else
/*******************************************************************************
* 'Static Normal Addressing' declarations
*******************************************************************************/
#  if defined (TP_ENABLE_NORMAL_ADDRESSING)       

  #define TpResetRxChannel(tpChannel) TpRxResetChannel(tpChannel)
  #define SetRxChannelInUse(tpChannel)  /* */

  /*******************************************************************************
  * Transport layer callback functions: MemCpy-functions for function tabels
  *******************************************************************************/

#if defined (C_ENABLE_MEMCOPY_SUPPORT)
# define __ApplTpTxCopyToCAN_mem(infoStruct)               CanCopyToCan((infoStruct->pDestination), (infoStruct->pSource), infoStruct->Length);
# define __ApplTpRxCopyFromCAN_mem(tpChannel, src, datLen) CanCopyFromCan((&(tpRxInfoStruct[tpChannel].DataBufferPtr.DataApplBufferPtr[tpRxInfoStruct[tpChannel].DataIndex])), (src), (datLen));
#else
# if defined (TP_ENABLE_INTERMEDIATE_CANCHIP_BUFFER)
#  error This configuration is not supported: C_ENABLE_MEMCOPY_SUPPORT is missing
# else
#  if defined (VGEN_ENABLE_VSTDLIB)
#   define __ApplTpTxCopyToCAN_mem(infoStruct) (void)VStdMemCpyRamToRam( (void *)(infoStruct->pDestination), (void *)(infoStruct->pSource), infoStruct->Length);
#   define __ApplTpRxCopyFromCAN_mem(tpChannel, src, datLen) (void)VStdMemCpyRamToRam((void *)(&(tpRxInfoStruct[tpChannel].DataBufferPtr.DataApplBufferPtr[tpRxInfoStruct[tpChannel].DataIndex])), (void *)(src), (datLen))
#  else
#   define __ApplTpTxCopyToCAN_mem(infoStruct) (void)TpIntMemCpy( (TP_MEMORY_MODEL_DATAvoid *)(infoStruct->pDestination), (TP_MEMORY_MODEL_DATA void *)(infoStruct->pSource), infoStruct->Length);
#   define __ApplTpRxCopyFromCAN_mem(tpChannel, src, datLen) (void)TpIntMemCpy((TP_MEMORY_MODEL_DATA void *)(&(tpRxInfoStruct[tpChannel].DataBufferPtr.DataApplBufferPtr[tpRxInfoStruct[tpChannel].DataIndex])), (TP_MEMORY_MODEL_DATA void *)(src), (datLen))
#  endif
# endif
#endif
  /*******************************************************************************
  * Transport layer callback functions: name definition
  *******************************************************************************/
#   if defined (TP_ENABLE_SINGLE_MSG_OBJ)
#   endif
  /*******************************************************************************
  * Transport layer callback functions: Pointer type definition
  *******************************************************************************/
#    if (TP_USE_GATEWAY_API == kTpOn)    
  typedef canuint8 *  TP_API_CALLBACK_TYPE (*ApplTpGetRxBufferFct)(canuint8, canuint16, CanRxInfoStructPtr);
#    else
  typedef canuint8 *  TP_API_CALLBACK_TYPE (*ApplTpGetRxBufferFct)(canuint8, canuint16);
#    endif
  typedef void        TP_API_CALLBACK_TYPE (*ApplTpIndicationFct)(canuint8, canuint16);
  typedef void        TP_API_CALLBACK_TYPE (*ApplTpConfirmationFct)(canuint8, canuint8);
  typedef void        TP_API_CALLBACK_TYPE (*ApplTpRxErrorIndicationFct)(canuint8, canuint8);
  typedef canuint8    TP_API_CALLBACK_TYPE (*ApplTpTxErrorIndicationFct)(canuint8, canuint8);
  typedef canuint8    TP_API_CALLBACK_TYPE (*ApplTpCopyToCanFct)(TpCopyToCanInfoStructPtr);
  typedef void        TP_API_CALLBACK_TYPE (*ApplTpCopyFromCanFct)(canuint8, canuint8 *, canuint16);
  typedef void        TP_API_CALLBACK_TYPE (*ApplTpNotificationFct)(canuint8, canuint8);
  typedef void        TP_API_CALLBACK_TYPE (*ApplTpCanMessageTransmittedFct)(canuint8);
  typedef void        TP_API_CALLBACK_TYPE (*ApplTpCanMessageReceivedFct)(canuint8);
  typedef void        TP_API_CALLBACK_TYPE (*ApplTpTxFCFct)(canuint8);
  typedef void        TP_API_CALLBACK_TYPE (*ApplTpSFFct)(canuint8);
  typedef void        TP_API_CALLBACK_TYPE (*ApplTpFFFct)(canuint8);
  typedef void        TP_API_CALLBACK_TYPE (*ApplTpCFFct)(canuint8);
  /*******************************************************************************
  * Transport layer callback functions: External pointer declaration
  *******************************************************************************/
  V_MEMROM0 extern V_MEMROM1 ApplTpIndicationFct            V_MEMROM2 TpRxIndication[];
  V_MEMROM0 extern V_MEMROM1 ApplTpConfirmationFct          V_MEMROM2 TpTxConfirmation[];
  V_MEMROM0 extern V_MEMROM1 ApplTpRxErrorIndicationFct     V_MEMROM2 TpRxErrorIndication[];
  V_MEMROM0 extern V_MEMROM1 ApplTpTxErrorIndicationFct     V_MEMROM2 TpTxErrorIndication[];
  V_MEMROM0 extern V_MEMROM1 ApplTpCopyToCanFct             V_MEMROM2 TpTxCopyToCan[];
#   if (TP_USE_CUSTOM_RX_MEMCPY == kTpOn)                    
  V_MEMROM0 extern V_MEMROM1 ApplTpCopyFromCanFct           V_MEMROM2 TpRxCopyFromCan[];
#   endif                                                    
  V_MEMROM0 extern V_MEMROM1 ApplTpNotificationFct          V_MEMROM2 TpTxNotification[];
  V_MEMROM0 extern V_MEMROM1 ApplTpCanMessageTransmittedFct V_MEMROM2 TpTxCanMessageTransmitted[];
  V_MEMROM0 extern V_MEMROM1 ApplTpCanMessageReceivedFct    V_MEMROM2 TpRxCanMessageReceived[];
  V_MEMROM0 extern V_MEMROM1 ApplTpTxFCFct                  V_MEMROM2 TpTxFC[];
  V_MEMROM0 extern V_MEMROM1 ApplTpSFFct                    V_MEMROM2 TpRxSF[];
  V_MEMROM0 extern V_MEMROM1 ApplTpFFFct                    V_MEMROM2 TpRxFF[];
  V_MEMROM0 extern V_MEMROM1 ApplTpCFFct                    V_MEMROM2 TpRxCF[];
  V_MEMROM0 extern V_MEMROM1 ApplTpGetRxBufferFct           V_MEMROM2 TpGetRxBuffer[];

  V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2 TpTxFlowControl[kTpTxChannelCount];
  V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2 TpRxFlowControl[kTpRxChannelCount];

#   if defined (TP_ENABLE_NORMAL_MIXED11_ADDRESSING)
  V_MEMROM0 extern V_MEMROM1 canuint8 V_MEMROM2 TpAddressingFormatOffset[kTpTxChannelCount];
#   endif
#  endif
#endif

/*******************************************************************************
* CODEDOUBLED CAN-DRIVERS
*******************************************************************************/
#if defined (C_MULTIPLE_RECEIVE_CHANNEL)
#else
# if ( kCanNumberOfChannels > 1 ) /* more than one CanChannel ? */
  typedef canuint8 (*TpCanTransmitFct)(CanTransmitHandle txObject);

  typedef void (*TpCanCancelTransmitFct)(CanTransmitHandle txObject);
 
# endif
#endif

/*******************************************************************************
* Functional Reception Tree 
*******************************************************************************/
#if defined TP_FUNC_ENABLE_RECEPTION
  
  void TP_API_CALL_TYPE TpFuncResetChannel(void);
  
#  if defined (TP_ENABLE_SINGLE_CHANNEL_TP)
#   define TpFuncSetResponse()
#  else
#   define TpFuncSetResponse(tpChannel)
#  endif
  
# if defined (TP_FUNC_ENABLE_EXTENDED_ADDRESSING)      || \
     defined (TP_FUNC_ENABLE_NORMAL_FIXED_ADDRESSING)  || \
     defined (TP_FUNC_ENABLE_MIXED_29_ADDRESSING)
  canuint8 TP_API_CALL_TYPE TpFuncGetSourceAddress(void);
# endif
  
# if defined (TP_FUNC_ENABLE_EXTENDED_ADDRESSING)      || \
     defined (TP_FUNC_ENABLE_NORMAL_FIXED_ADDRESSING)  || \
     defined (TP_FUNC_ENABLE_MIXED_29_ADDRESSING)
  canuint8 TP_API_CALL_TYPE TpFuncGetTargetAddress(void);
# endif
  
#  if (kTpNumberOfCanChannels > 1)
  extern canuint8 TP_API_CALL_TYPE TpFuncGetCanChannel(void);
#  else
#   define TpFuncGetCanChannel()                          (TP_CAN_CHANNEL_INDEX)
#  endif
  CanChipDataPtr TP_API_CALL_TYPE TpFuncGetCanBuffer(void);

# if defined (TP_FUNC_ENABLE_MIXED_29_ADDRESSING)      || \
     defined (TP_FUNC_ENABLE_MIXED_11_ADDRESSING)
  canuint8 TP_API_CALL_TYPE TpFuncGetAddressExtension(void);
# endif
  

#endif
/* endif TP_FUNC_ENABLE_RECEPTION */

#if defined (__cplusplus)
}
#endif

#endif /* OSEKTPMC_H */
/* STOPSINGLE_OF_MULTIPLE */
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
