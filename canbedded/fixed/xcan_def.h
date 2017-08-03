
/**********************************************************************************************************************
| Project Name: C A N - D R I V E R
|    File Name: XCAN_DEF.H
|
|  Description: Implementation of the CAN driver
|               Target systems: Freescale MCS12X - XGate part
|               Compiler:       Cosmic
|                               Metrowerks
|----------------------------------------------------------------------------------------------------------------------
|               C O P Y R I G H T
|----------------------------------------------------------------------------------------------------------------------
|   Copyright (c) by Vector Informatik GmbH.     All rights reserved.
|
|   This software is copyright protected and proprietary to 
|   Vector Informatik GmbH. Vector Informatik GmbH grants to you 
|   only those rights as set out in the license conditions. All 
|   other rights remain with Vector Informatik GmbH.
|----------------------------------------------------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|----------------------------------------------------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     ------------------------------------------------------------------------------
| Ml           Patrick Markl             Vector Informatik GmbH 
| Msr          Martin Schlodder          Vector Informatik GmbH 
| Ou           Mihai Olariu              Vector Informatik GmbH
|----------------------------------------------------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|----------------------------------------------------------------------------------------------------------------------
| Date       Version   Author  Description
| ---------  --------  ------  ----------------------------------------------------------------------------------------
| 2004-07-05 00.01.00    Ml     Creation 
| 2004-10-14 01.00.00    Ml     Added support for Metrowerks
| 2004-11-21 01.01.00    Ml     ESCAN00010293: Store value of CIDAC
| 2005-10-09 01.02.00    Ml     ESCAN00013822: rx handle is stored channel specific
|                        Ml     ESCAN00013823: Range is called correctly
| 2005-10-31 02.00.00    Ml     Added transmission path
|                        Msr/Ml ESCAN00013352: Added synchronization
|                        Ml     Split into high and low level driver
| 2005-12-20 02.01.00    Ml     ESCAN00014745: Added encapsulation for GENy
|                        Ml     ESCAN00014792: no changes
| 2006-04-10 02.02.00    Ml     ESCAN00015415: CanXGateArgs also declared in case of non-GENy tools
|                        Ml     ESCAN00016034: no changes
| 2006-06-06 02.03.00    Ml     ESCAN00016549: no changes
|                        Ml     ESCAN00016704: no changes
| 2007-01-21 02.04.00    Ml     ESCAN00018781: no changes
|                        Ou     ESCAN00019728: Update for the multi ECU configuration support in XGate
| 2007-03-09 02.05.00    Ml     ESCAN00019893: Removed parts for transmission
| 2007-04-09 02.06.00    Ou     ESCAN00020104: Upgrade to RI 1.5
|                        Ou     ESCAN00020163: Apply the erratum workaround regarding "SSEM instruction"
| 2007-09-11 02.07.00    Ou     ESCAN00019366: no changes
|                        Ou     ESCAN00021695: Messages received in range(s) are not processed
|                        Ou     ESCAN00022295: Wrong extern declaration of array CanXGateArgs
| 2008-08-20 02.08.00    Ou     ESCAN00027373: ID search is not further performed if at least one range was hit
| 2008-12-16 02.09.00    Ou     ESCAN00032051: Remove the inverted comma in the XGate code
|*********************************************************************************************************************/
#if !defined(_XCAN_DEF_H_)
# define _XCAN_DEF_H_

# if defined(C_ENABLE_XGATE_USED)
/*********************************************************************************************************************/
/* Defines                                                                                                           */
/*********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : DrvCan_Mcs12xXgateExt CQComponent : Implementation */
#  define DRVCAN_MCS12XXGATEEXT_VERSION         0x0209
#  define DRVCAN_MCS12XXGATEEXT_RELEASE_VERSION 0x00

#  if defined( C_COMP_COSMIC_MCS12X_MSCAN12 )
#   define C_XG_IRQ_1 @interrupt
#   define C_XG_IRQ_2
#   define C_XG_IRQ_3
#   define C_XG_API_1
#   define C_XG_API_2
#   define C_XG_API_3
#  endif


#  if defined( C_COMP_COSMIC_MCS12X_MSCAN12 )
#   define C_XG_IRQ_ARG_TYPE  tCanXGateArgType
#  endif


#  if defined(C_SINGLE_RECEIVE_CHANNEL)
#   define CAN_XG_CHANNEL_CANTYPE_ONLY      void
#   define CAN_XG_CHANNEL_CANTYPE_FIRST
#   define CAN_XG_CHANNEL_CANTYPE_LOCAL    
#   define CAN_XG_CHANNEL_CANPARA_ONLY
#   define CAN_XG_CHANNEL_CANPARA_FIRST
#  else
#   define CAN_XG_CHANNEL_CANTYPE_ONLY     CanChannelHandle channel
#   define CAN_XG_CHANNEL_CANTYPE_FIRST    CanChannelHandle channel,
#   define CAN_XG_CHANNEL_CANTYPE_LOCAL    CanChannelHandle channel;
#   define CAN_XG_CHANNEL_CANPARA_ONLY     channel
#   define CAN_XG_CHANNEL_CANPARA_FIRST    channel,
#  endif


/*********************************************************************************************************************/
/* Data types / structs / unions                                                                                     */
/*********************************************************************************************************************/
typedef struct
{
  volatile vuint8* pCanAddress;         /* physical address of CAN controller     */
  vsint16          nRxSearchStartIndex; /* Channel specific start index of Rx msg */
  vsint16          nRxSearchStopIndex;  /* Channel specific stop index of Rx msg  */
  vuint16          nLogChannel;         /* logical CAN channel                    */
}
tCanXGateArgType;

#  if defined(C_ENABLE_RANGE_0) || defined(C_ENABLE_RANGE_1) || defined(C_ENABLE_RANGE_2) || defined(C_ENABLE_RANGE_3)
/* stores information about the configured ranges */
typedef struct
{
  tCanRxId0 nMask[4];
  tCanRxId0 nCode[4];
#    if !defined(C_SINGLE_RECEIVE_CHANNEL)
  vuint16   nActiveMask; /* stores which of the ranges are valid */
#    endif
}
tCanXGateChannelRangeCfg;
#  endif

typedef struct
{
  vuint8 nByte0;
  vuint8 nByte1;
  vuint8 nByte2;
  vuint8 nByte3;
  vuint8 nByte4;
  vuint8 nByte5;
  vuint8 nByte6;
  vuint8 nByte7;
}
tCanXGateDataBufferStruct;

typedef union
{
  tCanXGateDataBufferStruct  s;
  vuint8                     a[8];
}
tCanXGateDataBufferUnion;

typedef struct
{
#  if defined(C_ENABLE_EXTENDED_ID)
  vuint32 Id;                           /* complete ID        */
#  else
  vuint16 Id;                           /* complete ID        */
  vuint16 IdEx;                         /* Extended ID bytes  */
#  endif
  tCanXGateDataBufferUnion  DataFld;    /* Data 0 .. 7        */
  vuint8  Dlc;
}
tCanXGateMsgObj;

/* Parametertype for precopy functions (within the XGate!) */
typedef struct 
{
  CanChannelHandle  nChannel;
  CanChipMsgPtr     pChipMsgObj;
  CanReceiveHandle  nHandle;
} 
tCanXGateRxInfoStruct;

typedef vuint8 (*CanXGateApplPrecopyFct)(tCanXGateRxInfoStruct* rxStruct);

/*********************************************************************************************************************/
/* Function prototypes                                                                                               */
/*********************************************************************************************************************/

C_XG_IRQ_1 void   C_XG_IRQ_2 CanXGateRxInterrupt(C_XG_IRQ_ARG_TYPE context) C_XG_IRQ_3;


/*********************************************************************************************************************/
/* external Variables                                                                                                */
/*********************************************************************************************************************/

V_MEMROM0 extern V_MEMROM1 tCanRxId0         V_MEMROM2 CanXGateRxId0[kCanNumberOfRxObjects];

#  if defined(VGEN_GENY)
#   if defined(C_ENABLE_XGATE_PRECOPY_FCT)
V_MEMROM0 extern V_MEMROM1 CanXGateApplPrecopyFct V_MEMROM2 CanXGateApplPrecopyFctPtr[kCanNumberOfRxObjects];
#   endif
#  endif

V_MEMRAM0 extern V_MEMRAM1 V_MEMROM0 V_MEMROM1 tCanXGateArgType V_MEMRAM2 V_MEMROM2 CanXGateArgs[kCanNumberOfPhysChannels];



#  if defined(C_ENABLE_RANGE_0) || defined(C_ENABLE_RANGE_1) || defined(C_ENABLE_RANGE_2) || defined(C_ENABLE_RANGE_3)
/* Rangeconfiguration */
V_MEMRAM0 extern V_MEMRAM1 tCanXGateChannelRangeCfg V_MEMRAM2 canRangeConfig[kCanNumberOfChannels];
/* shared variable to exchange the hit range */
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 canXgHitRange[kCanNumberOfChannels];
#  endif

/* shared variable to exchange the Rx message handle */
V_MEMRAM0 extern V_MEMRAM1 CanReceiveHandle V_MEMRAM2 canXgRxHandle[kCanNumberOfChannels];

/* temporary buffer for the currently received message */
V_MEMRAM0 extern V_MEMRAM1 tMsgObject V_MEMRAM2 canRxTmpBuf[kCanNumberOfChannels];

/* flag to synchronize the XGate Rx Irq and the CPU12 Rx interrupt */
V_MEMRAM0 extern V_MEMRAM1 volatile vuintx V_MEMRAM2 canRxSyncObj[kCanNumberOfChannels];



# endif
#endif
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
