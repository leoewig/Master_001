
/*****************************************************************************
| Project Name:   XCP on CAN Transport Layer adaption
|    File Name:   xcp_can.h
|
|  Description: 
|    Header of adaption component for the interface XCP Protocol Layer
|    and XCP on CAN Transport Layer ( Vector CANbedded CAN Driver )
|
|  Limitations:
|    - Interleaved communication mode is not allowed on CAN
|    - Detection of all XCP slaves within a CAN network is not supported
|        ( GET_SLAVE_ID )
|    - Assignment of CAN identifiers to DAQ lists or STIM is not supported
|        ( GET_DAQ_ID, SET_DAQ_ID )
|    - Variable data length is not supported.
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2008 by Vector Informatik GmbH.           All rights reserved.
|
|       This software is copyright protected and 
|       proporietary to Vector Informatik GmbH.
|       Vector Informatik GmbH grants to you only
|       those rights as set out in the license conditions.
|       All other rights remain with Vector Informatik GmbH.
| 
|       Diese Software ist urheberrechtlich geschuetzt. 
|       Vector Informatik GmbH raeumt Ihnen an dieser Software nur 
|       die in den Lizenzbedingungen ausdruecklich genannten Rechte ein.
|       Alle anderen Rechte verbleiben bei Vector Informatik GmbH.
|
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     -------------------------------------
| Tri          Frank Triem               Vector Informatik GmbH
| Svh          Sven Hesselmann           Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
|  Date       Version  Author  Description
| ----------  -------  ------  -----------------------------------------------
| 2003-09-04  1.00.00   Tri    Creation based on CCP Driver adaption.
|                              MISRA rules applied.
| 2005-01-03  1.01.00   Tri    ESCAN00008009: Rename module versions according to PD_PSC_Development
|                              ESCAN00009121: Add copyright note
|                              ESCAN00009127: Remove XCP_ENABLE_SEND_BUFFER
|                              ESCAN00009141: Compiler warnings while compilation with Tasking Compiler
|                              ESCAN00008085: Apply new naming for the parameters of callbacks
|                              ESCAN00008010: Remove extern declaration for xcp struct
|                              ESCAN00009154: Update Seed & Key
|                              ESCAN00008008: Apply PSC naming convention for types and structurs
|                              ESCAN00008007: Rename the post-organified filenames to xcpProf.h and xcpProf.c
|                              ESCAN00009172: Atmega only: Compiler error due to pointer conversion from RAM to Flash
|                              ESCAN00010708: Create template _xcp_appl.c
|                              ESCAN00007209: Apply naming convention to callback functions
|                              ESCAN00009144: Minor changes
| 2005-02-28  1.02.00   Tri    ESCAN00011303: Add CQ version and comment
| 2005-05-05  1.03.00   Tri    ESCAN00007515: Warning when compiling XCP_CAN.C
|                              ESCAN00012311: Support CAN-Driver without transmit queue
|                              ESCAN00011772: Support multiple CAN channels
| 2006-05-30  1.04.00   Tri    ESCAN00016506: Remove ISR locks in XcpPreCopy
| 2006-10-24  1.05.00   Tri    ESCAN00018168: Minor changes and rework of consistency checks
| 2008-03-19  1.06.00   Svh    ESCAN00023519: allow global access to variable xcpChannelNumber
|                              ESCAN00023375: Grant access to the current active channel
|                              ESCAN00025417: Removed not needed variable xcpQueuedObjectDLC
|***************************************************************************/

#if !defined ( __XCP_CAN_H_ )
#define __XCP_CAN_H_


/***************************************************************************/
/* Include                                                                 */
/***************************************************************************/
#include "xcp_cfg.h"
#include "xcp_par.h"


/***************************************************************************/
/* Version                                                                 */
/***************************************************************************/

/* BCD coded version number */
/* ##V_CFG_MANAGEMENT ##CQProject : Cp_XcpOnCan CQComponent : Implementation */
#define CP_XCPONCAN_VERSION         0x0106u
#define CP_XCPONCAN_RELEASE_VERSION 0x00u


/***************************************************************************/
/* Default defintions                                                      */
/***************************************************************************/

/* Version of the XCP Transport Layer for CAN specification. */
#if defined ( XCP_TRANSPORT_LAYER_VERSION )
#else
  #define XCP_TRANSPORT_LAYER_VERSION 0x0100u
#endif

/* CTO and DTO are always 8 on CAN. */
#if defined ( kXcpMaxCTO )
#else
  #define kXcpMaxCTO 8
#endif
#if defined ( kXcpMaxDTO )
#else
  #define kXcpMaxDTO 8
#endif

/* The function XcpGetState is required for the protection against multiple connections. */
#if defined ( XCP_ENABLE_MULTI_CONNECTION_PROTECTION )
  #define XCP_ENABLE_GET_CONNECTION_STATE
#else
  #define XCP_DISABLE_GET_CONNECTION_STATE
#endif


/***************************************************************************/
/* Default macros                                                          */
/***************************************************************************/

/* Interrupt enable and disable functions */
#if defined ( ApplXcpInterruptDisable )
#else
  #define ApplXcpInterruptDisable() CanInterruptDisable()
#endif
#if defined ( ApplXcpInterruptEnable )
#else
  #define ApplXcpInterruptEnable() CanInterruptRestore()
#endif

/* XCP Packet transmit. */
#if defined ( XcpTransmit )
#else
  #define XcpTransmit() CanTransmit(XcpGetTransmitHandle())
#endif


#endif /* ! defined ( __XCP_CAN_H_ ) */


/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
