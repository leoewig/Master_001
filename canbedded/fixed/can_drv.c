/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     can_drv.c 
 *
 * description:   can driver
 *
 *
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           can driver for S12XEP100
 *
 *****************************************************************************/

#define C_DRV_INTERNAL

/*lint -function(exit,ApplCanFatalError)*/

/***************************************************************************/
/* Include files                                                           */
/***************************************************************************/

#include "can_inc.h"


/*lint --e{659}*/

/***************************************************************************/
/* Version check                                                           */
/***************************************************************************/
#if(DRVCAN_VERSION                     != 0x0507u)
# error "Source and Header file are inconsistent!"
#endif
#if(DRVCAN_RELEASE_VERSION             != 0x01u)
# error "Source and Header file are inconsistent!"
#endif

#if( C_VERSION_REF_IMPLEMENTATION != 0x150u)
# error "Generated Data and CAN driver source file are inconsistent!"
#endif

#if( DRVCAN__COREHLL_VERSION != 0x0206)
# error "Source and Header file are inconsistent!"
#endif
#if( DRVCAN__COREHLL_RELEASE_VERSION != 0x00)
# error "Source and Header file are inconsistent!"
#endif

#if ( ( DRVCAN__HLLTXQUEUEBIT_VERSION != 0x0104) )
# error "TxQueue Source and Header Version inconsistent!"
#endif
#if ( ( DRVCAN__HLLTXQUEUEBIT_RELEASE_VERSION != 0x00) )
# error "TxQueue Source and Header Version inconsistent!"
#endif


#if defined( DRVCAN__HLLTXQUEUEBIT_VERSION )
# if ( ( DRVCAN__HLLTXQUEUEBIT_VERSION != 0x0104) || \
       ( DRVCAN__HLLTXQUEUEBIT_RELEASE_VERSION != 0x00)  )
#  error "TxQueue Version inconsistent!"
# endif

/* defines to satisfy MISRA checker tool */
# define DRVCAN__HLLTXQUEUEBYTE_VERSION 0x0000
# define DRVCAN__HLLTXQUEUEBYTE_RELEASE_VERSION 0x00

#else
# if defined( DRVCAN__HLLTXQUEUEBYTE_VERSION )
#  if ( ( DRVCAN__HLLTXQUEUEBYTE_VERSION != 0x0103) || \
       ( DRVCAN__HLLTXQUEUEBYTE_RELEASE_VERSION != 0x00)  )
#   error "TxQueue Version inconsistent!"
#  endif
# else
#  error "No TxQueue available"
# endif

/* defines to satisfy MISRA checker tool */
# define DRVCAN__HLLTXQUEUEBIT_VERSION 0x0000
# define DRVCAN__HLLTXQUEUEBIT_RELEASE_VERSION 0x00

#endif


/***************************************************************************/
/* Defines                                                                 */
/***************************************************************************/


/* ##RI-1.1: Object parameters for Tx-Observe functions */
/* status of transmit objects */
#define kCanBufferFree                                     ((CanTransmitHandle)0xFFFFFFFFU)   /* mark a transmit object is free */
#define kCanBufferCancel                                   ((CanTransmitHandle)0xFFFFFFFEU)   /* mark a transmit object as canceled */
#define kCanBufferMsgDestroyed                             ((CanTransmitHandle)0xFFFFFFFDU)   /* mark a transmit object as destroyed */
/* reserved value because of definition in header:    0xFFFFFFFCU */
/* valid transmit message handle:   0x0 to kCanNumberOfTxObjects   */

/* return values */ 
#define kCanHlFinishRx                                     ((vuint8)0x00)
#define kCanHlContinueRx                                   ((vuint8)0x01)

#define  CANHL_TX_QUEUE_BIT
/* Define chiphardware                     */
/* Constants concerning can chip registers */
/* control registers for msg objects       */

#if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  

/* Bitmask of CAN-Register CTL0:                                                         */
# define  INITRQ       ((vuint8)0x01)    /* initialized mode request                     */ 
# define  SLPRQ        ((vuint8)0x02)    /* Sleep request, go to internal sleep mode     */
# define  WUPE         ((vuint8)0x04)    /* wakeup enable                                */
# define  TIMER        ((vuint8)0x08)    /* timer enable                                 */
# define  SYNCH        ((vuint8)0x10)    /* synchronized status                          */
# define  CSWAI        ((vuint8)0x20)    /* CAn stops in waitmode                        */
# define  RXACT        ((vuint8)0x40)    /* receiver active                              */
# define  RXFRM        ((vuint8)0x80)    /* received frame flag                          */

/* Bitmask of CAN-Register CTL1:                                                         */
# define  INITAK       ((vuint8)0x01)    /* initialized mode ack                         */ 
# define  SLPAK        ((vuint8)0x02)    /* Sleep mode acknowledge                       */
# define  WUPM         ((vuint8)0x04)    /* wakeup mode                                  */
# define  LISTEN       ((vuint8)0x10)    /* listen only                                  */
# define  LOOPB        ((vuint8)0x20)    /* loopback mode                                */
# define  CLKSRC       ((vuint8)0x40)    /* clocksource                                  */
# define  CANE         ((vuint8)0x80)    /* CAN enabled                                  */

/* Bitmasks of CAN receiver flag register CRFLG:                                         */
# define  WUPIF        ((vuint8)0x80)    /* Wake-up interrupt flag                       */
# define  CSCIF        ((vuint8)0x40)    /* CAN status change interrupt flag             */
# define  RSTAT1       ((vuint8)0x20)    /* Receiver status bit 1 flag                   */
# define  RSTAT0       ((vuint8)0x10)    /* Receiver status bit 0 flag                   */
# define  TSTAT1       ((vuint8)0x08)    /* Transmitter status bit 1 flag                */
# define  TSTAT0       ((vuint8)0x04)    /* Transmitter status bit 0 flag                */
# define  OVRIF        ((vuint8)0x02)    /* Overrun interrupt flag                       */
# define  RXF          ((vuint8)0x01)    /* Receive buffer full flag                     */
# define  BOFFIF       ((vuint8)(TSTAT0 | TSTAT1))  /* Status : BusOff */

/* Bitmasks of CAN receiver interrupt enable register CRIER:                             */
# define  WUPIE        ((vuint8)0x80)    /* Wake-up interrupt enable                     */
# define  CSCIE        ((vuint8)0x40)    /* CAN status change interrupt enable           */
# define  RSTAT1E      ((vuint8)0x20)    /* Receiver status bit 1 enable                 */
# define  RSTAT0E      ((vuint8)0x10)    /* Receiver status bit 0 enable                 */
# define  TSTAT1E      ((vuint8)0x08)    /* Transmitter status bit 1 enable              */
# define  TSTAT0E      ((vuint8)0x04)    /* Transmitter status bit 0 enable              */
# define  OVRIE        ((vuint8)0x02)    /* Overrun interrupt enable                     */
# define  RXE          ((vuint8)0x01)    /* Receive buffer full enable                   */

/* Bitmasks of CAN transmitter abort request register CTARQ:                             */
# define  ABTRQ2       ((vuint8)0x04)
# define  ABTRQ1       ((vuint8)0x02)
# define  ABTRQ0       ((vuint8)0x01)

/* Bitmasks of CAN transmitter abort request register CTAAK:                             */
# define  ABTAK2       ((vuint8)0x04)
# define  ABTAK1       ((vuint8)0x02)
# define  ABTAK0       ((vuint8)0x01)

/* Bitmasks of CAN transmit buffer selection register CTBSEL:                            */
# define  TX2          ((vuint8)0x04)
# define  TX1          ((vuint8)0x02)
# define  TX0          ((vuint8)0x01)

/* Bitmasks of CAN miscellaneous Register CMISC:                                         */
# define BOHOLD        ((vuint8)0x01)

# define  CRIER_RX_MASK      ((vuint8)0x01)
# define  CRIER_ERROR_MASK   ((vuint8)0x7E)
# define  CRIER_WAKEUP_MASK  ((vuint8)0x80)

# if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
#  define CANXGSEM    (*(volatile vuint16*)(V_REG_BLOCK_ADR + 0x380 + 0x1A)) /* XGATE semaphore register */
#  define CANXGMCTL   (*(volatile vuint16*)(V_REG_BLOCK_ADR + 0x380 + 0x00)) /* XGATE module control register */


#  define kCanCanSemaphoreMask       ((vuint16)0x0001)
#  define kCanCheckCanSemaphore      ((vuint16)0x0000)
#  define kCanSetCanSemaphore        ((vuint16)0x0101)
#  define kCanClearCanSemaphore      ((vuint16)0x0100)
#  define kCanDisableXGateInterrupts ((vuint16)0x0100)
#  define kCanRestoreXGateInterrupts ((vuint16)0x0101)

#  if defined(C_ENABLE_XGATE_USED)
#   define kCanCRIERInterruptsDisabledVal     (vuint8)1 /* let the interrupt be processed in XGate when the message is received */
#   define CanBeginCriticalXGateSection()     do{ CANXGSEM = kCanSetCanSemaphore; }while((CANXGSEM & kCanCanSemaphoreMask) == kCanCheckCanSemaphore)
#   define CanEndCriticalXGateSection()       CANXGSEM = kCanClearCanSemaphore
#   define CanDisableXGateInterrupts()        if(canCanXGateIrqtCount == 0){ CANXGMCTL = kCanDisableXGateInterrupts; }canCanXGateIrqtCount++
#   define CanRestoreXGateInterrupts()        canCanXGateIrqtCount--;if(canCanXGateIrqtCount == 0){ CANXGMCTL = kCanRestoreXGateInterrupts; }
#  else
#   define kCanCRIERInterruptsDisabledVal     (vuint8)0
#   define CanBeginCriticalXGateSection()
#   define CanEndCriticalXGateSection()
#   define CanDisableXGateInterrupts()
#   define CanRestoreXGateInterrupts()
#  endif
# else
#  define kCanCRIERInterruptsDisabledVal      (vuint8)0
#  define CanBeginCriticalXGateSection()
#  define CanEndCriticalXGateSection()
#  define CanDisableXGateInterrupts()
#  define CanRestoreXGateInterrupts()
# endif


#endif

/***************************************************************************/
/* macros                                                                  */
/***************************************************************************/

#if !(defined( C_HL_DISABLE_RX_INFO_STRUCT_PTR ) || defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR ))
# define C_HL_ENABLE_RX_INFO_STRUCT_PTR
#endif

#if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
# define CAN_HL_P_RX_INFO_STRUCT(channel)                  (pCanRxInfoStruct)  
# define CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)           (pCanRxInfoStruct->Handle)  
#else
# define CAN_HL_P_RX_INFO_STRUCT(channel)                  (&canRxInfoStruct[channel])
# define CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)           (canRxInfoStruct[channel].Handle)  
#endif 


/*disabled - lint -emacro( (572,778), C_RANGE_MATCH) */


#if defined( C_SINGLE_RECEIVE_CHANNEL )
# if (kCanNumberOfUsedCanRxIdTables == 1)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, mask, code) \
                  (  ((idRaw0) & (tCanRxId0)~MK_RX_RANGE_MASK_IDSTD0(mask)) == MK_RX_RANGE_CODE_IDSTD0(code) )
#  define C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, mask, code) \
                  (  ((idRaw0) & (tCanRxId0)~MK_RX_RANGE_MASK_IDEXT0(mask)) == MK_RX_RANGE_CODE_IDEXT0(code) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 2)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, mask, code) \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK_IDSTD0(mask)) == MK_RX_RANGE_CODE_IDSTD0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK_IDSTD1(mask)) == MK_RX_RANGE_CODE_IDSTD1(code) ) )
#  define C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, mask, code) \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK_IDEXT0(mask)) == MK_RX_RANGE_CODE_IDEXT0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK_IDEXT1(mask)) == MK_RX_RANGE_CODE_IDEXT1(code) ) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 3)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, mask, code) \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK_IDSTD0(mask)) == MK_RX_RANGE_CODE_IDSTD0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK_IDSTD1(mask)) == MK_RX_RANGE_CODE_IDSTD1(code) ) && \
                    ( ((idRaw2) & (tCanRxId2)~ MK_RX_RANGE_MASK_IDSTD2(mask)) == MK_RX_RANGE_CODE_IDSTD2(code) ) )
#  define C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, mask, code) \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK_IDEXT0(mask)) == MK_RX_RANGE_CODE_IDEXT0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK_IDEXT1(mask)) == MK_RX_RANGE_CODE_IDEXT1(code) ) && \
                    ( ((idRaw2) & (tCanRxId2)~ MK_RX_RANGE_MASK_IDEXT2(mask)) == MK_RX_RANGE_CODE_IDEXT2(code) ) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 4)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, mask, code) \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK_IDSTD0(mask)) == MK_RX_RANGE_CODE_IDSTD0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK_IDSTD1(mask)) == MK_RX_RANGE_CODE_IDSTD1(code) ) && \
                    ( ((idRaw2) & (tCanRxId2)~ MK_RX_RANGE_MASK_IDSTD2(mask)) == MK_RX_RANGE_CODE_IDSTD2(code) ) && \
                    ( ((idRaw3) & (tCanRxId3)~ MK_RX_RANGE_MASK_IDSTD3(mask)) == MK_RX_RANGE_CODE_IDSTD3(code) ) )
#  define C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, mask, code) \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK_IDEXT0(mask)) == MK_RX_RANGE_CODE_IDEXT0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK_IDEXT1(mask)) == MK_RX_RANGE_CODE_IDEXT1(code) ) && \
                    ( ((idRaw2) & (tCanRxId2)~ MK_RX_RANGE_MASK_IDEXT2(mask)) == MK_RX_RANGE_CODE_IDEXT2(code) ) && \
                    ( ((idRaw3) & (tCanRxId3)~ MK_RX_RANGE_MASK_IDEXT3(mask)) == MK_RX_RANGE_CODE_IDEXT3(code) ) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 5)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, mask, code) \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK_IDSTD0(mask)) == MK_RX_RANGE_CODE_IDSTD0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK_IDSTD1(mask)) == MK_RX_RANGE_CODE_IDSTD1(code) ) && \
                    ( ((idRaw2) & (tCanRxId2)~ MK_RX_RANGE_MASK_IDSTD2(mask)) == MK_RX_RANGE_CODE_IDSTD2(code) ) && \
                    ( ((idRaw3) & (tCanRxId3)~ MK_RX_RANGE_MASK_IDSTD3(mask)) == MK_RX_RANGE_CODE_IDSTD3(code) ) && \
                    ( ((idRaw4) & (tCanRxId4)~ MK_RX_RANGE_MASK_IDSTD4(mask)) == MK_RX_RANGE_CODE_IDSTD4(code) ) )
#  define C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, mask, code) \
                  ( ( ((idRaw0) & (tCanRxId0)~ MK_RX_RANGE_MASK_IDEXT0(mask)) == MK_RX_RANGE_CODE_IDEXT0(code) ) && \
                    ( ((idRaw1) & (tCanRxId1)~ MK_RX_RANGE_MASK_IDEXT1(mask)) == MK_RX_RANGE_CODE_IDEXT1(code) ) && \
                    ( ((idRaw2) & (tCanRxId2)~ MK_RX_RANGE_MASK_IDEXT2(mask)) == MK_RX_RANGE_CODE_IDEXT2(code) ) && \
                    ( ((idRaw3) & (tCanRxId3)~ MK_RX_RANGE_MASK_IDEXT3(mask)) == MK_RX_RANGE_CODE_IDEXT3(code) ) && \
                    ( ((idRaw4) & (tCanRxId4)~ MK_RX_RANGE_MASK_IDEXT4(mask)) == MK_RX_RANGE_CODE_IDEXT4(code) ) )
# endif
#else     /* C_MULTIPLE_RECEIVE_CHANNEL */

# if (kCanNumberOfUsedCanRxIdTables == 1)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)    \
                                (  ((idRaw0) & (tCanRxId0)~(mask.Id0)) == (code.Id0) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 2)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)    \
                                ( ( ((idRaw0) & (tCanRxId0)~(mask.Id0)) == (code.Id0) ) &&\
                                  ( ((idRaw1) & (tCanRxId1)~(mask.Id1)) == (code.Id1) ) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 3)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)    \
                                ( ( ((idRaw0) & (tCanRxId0)~(mask.Id0)) == (code.Id0) ) &&\
                                  ( ((idRaw1) & (tCanRxId1)~(mask.Id1)) == (code.Id1) ) &&\
                                  ( ((idRaw2) & (tCanRxId2)~(mask.Id2)) == (code.Id2) ) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 4)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)    \
                                ( ( ((idRaw0) & (tCanRxId0)~(mask.Id0)) == (code.Id0) ) &&\
                                  ( ((idRaw1) & (tCanRxId1)~(mask.Id1)) == (code.Id1) ) &&\
                                  ( ((idRaw2) & (tCanRxId2)~(mask.Id2)) == (code.Id2) ) &&\
                                  ( ((idRaw3) & (tCanRxId3)~(mask.Id3)) == (code.Id3) ) )
# endif
# if (kCanNumberOfUsedCanRxIdTables == 5)
/* Msg(4:3410) Macro parameter not enclosed in (). MISRA Rule 96 - no change */
#  define C_RANGE_MATCH( CAN_RX_IDRAW_PARA, mask, code)    \
                                ( ( ((idRaw0) & (tCanRxId0)~(mask.Id0)) == (code.Id0) ) &&\
                                  ( ((idRaw1) & (tCanRxId1)~(mask.Id1)) == (code.Id1) ) &&\
                                  ( ((idRaw2) & (tCanRxId2)~(mask.Id2)) == (code.Id2) ) &&\
                                  ( ((idRaw3) & (tCanRxId3)~(mask.Id3)) == (code.Id3) ) &&\
                                  ( ((idRaw4) & (tCanRxId4)~(mask.Id4)) == (code.Id4) ) )
# endif
#endif


#if (kCanNumberOfUsedCanRxIdTables == 1)
# define CAN_RX_IDRAW_PARA                                 idRaw0
#endif
#if (kCanNumberOfUsedCanRxIdTables == 2)
# define CAN_RX_IDRAW_PARA                                 idRaw0,idRaw1
#endif
#if (kCanNumberOfUsedCanRxIdTables == 3)
# define CAN_RX_IDRAW_PARA                                 idRaw0,idRaw1,idRaw2
#endif
#if (kCanNumberOfUsedCanRxIdTables == 4)
# define CAN_RX_IDRAW_PARA                                 idRaw0,idRaw1,idRaw2,idRaw3
#endif
#if (kCanNumberOfUsedCanRxIdTables == 5)
# define CAN_RX_IDRAW_PARA                                 idRaw0,idRaw1,idRaw2,idRaw3,idRaw4
#endif


#if defined( C_SINGLE_RECEIVE_CHANNEL )
# define channel                                           ((CanChannelHandle)0)
# define canHwChannel                                      ((CanChannelHandle)0)
# define CAN_HL_HW_CHANNEL_STARTINDEX(channel)             ((CanChannelHandle)0)
# define CAN_HL_HW_CHANNEL_STOPINDEX(channel)              ((CanChannelHandle)0)
# define CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel)        (kCanMsgTransmitObj)
# define CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel)           (kCanHwTxNormalIndex)

/* Offset which has to be added to change the hardware Tx handle into a logical handle, which is unique over all channels */
/*        Tx-Hardware-Handle - CAN_HL_HW_TX_STARTINDEX(canHwChannel) + CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel) */
# define CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)          ((vsintx)0-kCanHwTxStartIndex)

# define CAN_HL_TX_STARTINDEX(channel)                     ((CanTransmitHandle)0)
# define CAN_HL_TX_STAT_STARTINDEX(channel)                ((CanTransmitHandle)0)
# define CAN_HL_TX_DYN_ROM_STARTINDEX(channel)             (kCanNumberOfTxStatObjects)
# define CAN_HL_TX_DYN_RAM_STARTINDEX(channel)             ((CanTransmitHandle)0)
/* # define CAN_HL_RX_STARTINDEX(channel)                     ((CanReceiveHandle)0) */
/* index to access the ID tables - Basic index only for linear search 
   for hash search this is the start index of the ??? */
# define CAN_HL_RX_BASIC_STARTINDEX(channel)               ((CanReceiveHandle)0)
# if defined( C_SEARCH_HASH ) 
#  define CAN_HL_RX_FULL_STARTINDEX(canHwChannel)          ((CanReceiveHandle)0)
# else
#  define CAN_HL_RX_FULL_STARTINDEX(canHwChannel)          (kCanNumberOfRxBasicCANObjects)
# endif
# define CAN_HL_INIT_OBJ_STARTINDEX(channel)               ((vuint8)0)
# define CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel)         ((CanObjectHandle)0)
# define CAN_HL_HW_TX_STARTINDEX(canHwChannel)             (kCanHwTxStartIndex)
# define CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel)        (kCanHwRxFullStartIndex)
# define CAN_HL_HW_RX_BASIC_STARTINDEX(canHwChannel)       (kCanHwRxBasicStartIndex)
# define CAN_HL_HW_UNUSED_STARTINDEX(canHwChannel)         (kCanHwUnusedStartIndex)

# define CAN_HL_TX_STOPINDEX(channel)                      (kCanNumberOfTxObjects)
# define CAN_HL_TX_STAT_STOPINDEX(channel)                 (kCanNumberOfTxStatObjects)
# define CAN_HL_TX_DYN_ROM_STOPINDEX(channel)              (kCanNumberOfTxObjects)
# define CAN_HL_TX_DYN_RAM_STOPINDEX(channel)              (kCanNumberOfTxDynObjects)
/* # define CAN_HL_RX_STOPINDEX(channel)                      (kCanNumberOfRxObjects) */
# define CAN_HL_RX_BASIC_STOPINDEX(channel)                (kCanNumberOfRxBasicCANObjects)
# if defined( C_SEARCH_HASH ) 
#  define CAN_HL_RX_FULL_STOPINDEX(canHwChannel)           (kCanNumberOfRxFullCANObjects)
# else
#  define CAN_HL_RX_FULL_STOPINDEX(canHwChannel)           (kCanNumberOfRxBasicCANObjects+kCanNumberOfRxFullCANObjects)
# endif
# define CAN_HL_INIT_OBJ_STOPINDEX(channel)                (kCanNumberOfInitObjects)
# define CAN_HL_LOG_HW_TX_STOPINDEX(canHwChannel)          (kCanNumberOfUsedTxCANObjects)
# define CAN_HL_HW_TX_STOPINDEX(canHwChannel)              (kCanHwTxStartIndex     +kCanNumberOfUsedTxCANObjects)
# define CAN_HL_HW_RX_FULL_STOPINDEX(canHwChannel)         (kCanHwRxFullStartIndex +kCanNumberOfRxFullCANObjects)
# define CAN_HL_HW_RX_BASIC_STOPINDEX(canHwChannel)        (kCanHwRxBasicStartIndex+kCanNumberOfUsedRxBasicCANObjects)
# define CAN_HL_HW_UNUSED_STOPINDEX(canHwChannel)          (kCanHwUnusedStartIndex +kCanNumberOfUnusedObjects)

#else
#  define canHwChannel                                     channel   /*brackets are not allowed here due to compiler error with Renesas HEW compiler for SH2*/
#  define CAN_HL_HW_CHANNEL_STARTINDEX(channel)            (channel)
#  define CAN_HL_HW_CHANNEL_STOPINDEX(channel)             (channel)

#  define CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel)       (CanHwMsgTransmitIndex[(canHwChannel)])
#  define CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel)          (CanHwTxNormalIndex[(canHwChannel)])
/* Offset which has to be added to change the hardware Tx handle into a logical handle, which is unique over all channels */
/*        Tx-Hardware-Handle - CAN_HL_HW_TX_STARTINDEX(canHwChannel) + CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel) */
#  define CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)         (CanTxOffsetHwToLog[(canHwChannel)])

# define CAN_HL_TX_STARTINDEX(channel)                     (CanTxStartIndex[(channel)])
# define CAN_HL_TX_STAT_STARTINDEX(channel)                (CanTxStartIndex[(channel)])
# define CAN_HL_TX_DYN_ROM_STARTINDEX(channel)             (CanTxDynRomStartIndex[(channel)])
# define CAN_HL_TX_DYN_RAM_STARTINDEX(channel)             (CanTxDynRamStartIndex[(channel)])
/* # define CAN_HL_RX_STARTINDEX(channel)                     (CanRxStartIndex[(channel)]) */
/* index to access the ID tables - Basic index only for linear search */
# define CAN_HL_RX_BASIC_STARTINDEX(channel)               (CanRxBasicStartIndex[(channel)])      
# define CAN_HL_RX_FULL_STARTINDEX(canHwChannel)           (CanRxFullStartIndex[(canHwChannel)])
# define CAN_HL_INIT_OBJ_STARTINDEX(channel)               (CanInitObjectStartIndex[(channel)])      
# define CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel)         (CanLogHwTxStartIndex[(canHwChannel)])
#  define CAN_HL_HW_TX_STARTINDEX(canHwChannel)             (CanHwTxStartIndex[(canHwChannel)])   
#  define CAN_HL_HW_RX_FULL_STARTINDEX(canHwChannel)        (CanHwRxFullStartIndex[(canHwChannel)])
#  define CAN_HL_HW_RX_BASIC_STARTINDEX(canHwChannel)       (CanHwRxBasicStartIndex[(canHwChannel)]) 
#  define CAN_HL_HW_UNUSED_STARTINDEX(canHwChannel)         (CanHwUnusedStartIndex[(canHwChannel)])
                                                           
# define CAN_HL_TX_STOPINDEX(channel)                      (CanTxStartIndex[(channel) + 1]) 
# define CAN_HL_TX_STAT_STOPINDEX(channel)                 (CanTxDynRomStartIndex[(channel)])
# define CAN_HL_TX_DYN_ROM_STOPINDEX(channel)              (CanTxStartIndex[(channel) + 1]) 
# define CAN_HL_TX_DYN_RAM_STOPINDEX(channel)              (CanTxDynRamStartIndex[(channel) + 1])
/* # define CAN_HL_RX_STOPINDEX(channel)                      (CanRxStartIndex[(channel) + 1]) */
/* index to access the ID tables - Basic index only for linear search */
# define CAN_HL_RX_BASIC_STOPINDEX(channel)                (CanRxFullStartIndex[CAN_HL_HW_CHANNEL_STARTINDEX(channel)])
# define CAN_HL_INIT_OBJ_STOPINDEX(channel)                (CanInitObjectStartIndex[(channel) + 1])
# define CAN_HL_LOG_HW_TX_STOPINDEX(canHwChannel)          (CanLogHwTxStartIndex[(canHwChannel) +1])      
#  define CAN_HL_HW_TX_STOPINDEX(canHwChannel)              (CanHwTxStopIndex[(canHwChannel)])
#  define CAN_HL_HW_RX_FULL_STOPINDEX(canHwChannel)         (CanHwRxFullStopIndex[(canHwChannel)])
#  define CAN_HL_HW_RX_BASIC_STOPINDEX(canHwChannel)         (CanHwRxBasicStopIndex[(canHwChannel)])
#  define CAN_HL_HW_UNUSED_STOPINDEX(canHwChannel)         (CanHwUnusedStopIndex[(canHwChannel)])

#endif


#if defined( C_SINGLE_RECEIVE_CHANNEL )

# define CANRANGE0ACCMASK(i)                               C_RANGE0_ACC_MASK
# define CANRANGE0ACCCODE(i)                               C_RANGE0_ACC_CODE
# define CANRANGE1ACCMASK(i)                               C_RANGE1_ACC_MASK
# define CANRANGE1ACCCODE(i)                               C_RANGE1_ACC_CODE
# define CANRANGE2ACCMASK(i)                               C_RANGE2_ACC_MASK
# define CANRANGE2ACCCODE(i)                               C_RANGE2_ACC_CODE
# define CANRANGE3ACCMASK(i)                               C_RANGE3_ACC_MASK
# define CANRANGE3ACCCODE(i)                               C_RANGE3_ACC_CODE

/* generated id type of the range */
# define CANRANGE0IDTYPE(i)                                C_RANGE0_IDTYPE
# define CANRANGE1IDTYPE(i)                                C_RANGE1_IDTYPE
# define CANRANGE2IDTYPE(i)                                C_RANGE2_IDTYPE
# define CANRANGE3IDTYPE(i)                                C_RANGE3_IDTYPE

# define APPL_CAN_MSG_RECEIVED( i )                        (APPL_CAN_MSGRECEIVED( i ))

# define APPLCANRANGE0PRECOPY( i )                         (ApplCanRange0Precopy( i ))   
# define APPLCANRANGE1PRECOPY( i )                         (ApplCanRange1Precopy( i ))   
# define APPLCANRANGE2PRECOPY( i )                         (ApplCanRange2Precopy( i ))   
# define APPLCANRANGE3PRECOPY( i )                         (ApplCanRange3Precopy( i ))   

# define APPL_CAN_BUSOFF( i )                              (ApplCanBusOff())
# define APPL_CAN_WAKEUP( i )                              (ApplCanWakeUp())

# if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
#  define APPLCANCANCELNOTIFICATION( i, j )                (APPL_CAN_CANCELNOTIFICATION( j ))
# else
#  define APPLCANCANCELNOTIFICATION( i, j )
# endif
# if defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
#  define APPLCANMSGCANCELNOTIFICATION( i )                (APPL_CAN_MSGCANCELNOTIFICATION())
# else
#  define APPLCANMSGCANCELNOTIFICATION( i )
# endif

# define CAN_RX_INDEX_TBL(channel,id)                      (CanRxIndexTbl[id])

#else

# define CANRANGE0ACCMASK(i)                               (CanChannelObject[i].RangeMask[0])
# define CANRANGE0ACCCODE(i)                               (CanChannelObject[i].RangeCode[0])
# define CANRANGE1ACCMASK(i)                               (CanChannelObject[i].RangeMask[1])
# define CANRANGE1ACCCODE(i)                               (CanChannelObject[i].RangeCode[1])
# define CANRANGE2ACCMASK(i)                               (CanChannelObject[i].RangeMask[2])
# define CANRANGE2ACCCODE(i)                               (CanChannelObject[i].RangeCode[2])
# define CANRANGE3ACCMASK(i)                               (CanChannelObject[i].RangeMask[3])
# define CANRANGE3ACCCODE(i)                               (CanChannelObject[i].RangeCode[3])

/* generated id type of the range */
# define CANRANGE0IDTYPE(i)                                (CanChannelObject[i].RangeIdType[0])
# define CANRANGE1IDTYPE(i)                                (CanChannelObject[i].RangeIdType[1])
# define CANRANGE2IDTYPE(i)                                (CanChannelObject[i].RangeIdType[2])
# define CANRANGE3IDTYPE(i)                                (CanChannelObject[i].RangeIdType[3])

# define APPL_CAN_MSG_RECEIVED( i )                        (CanChannelObject[(i)->Channel].ApplCanMsgReceivedFct(i))

# define APPLCANRANGE0PRECOPY( i )                         (CanChannelObject[(i)->Channel].ApplCanRangeFct[0](i))
# define APPLCANRANGE1PRECOPY( i )                         (CanChannelObject[(i)->Channel].ApplCanRangeFct[1](i))   
# define APPLCANRANGE2PRECOPY( i )                         (CanChannelObject[(i)->Channel].ApplCanRangeFct[2](i))   
# define APPLCANRANGE3PRECOPY( i )                         (CanChannelObject[(i)->Channel].ApplCanRangeFct[3](i))   

# define APPL_CAN_BUSOFF( i )                              (CanChannelObject[i].ApplCanBusOffFct(i))
# define APPL_CAN_WAKEUP( i )                              (CanChannelObject[i].ApplCanWakeUpFct(i))

# if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
#  define APPLCANCANCELNOTIFICATION( i, j )                (CanChannelObject[i].ApplCanCancelNotificationFct( j ))
# else
#  define APPLCANCANCELNOTIFICATION( i, j )
# endif

# if defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
#  define APPLCANMSGCANCELNOTIFICATION( i )                (CanChannelObject[i].ApplCanMsgTransmitCancelNotifyFct( i ))
# else
#  define APPLCANMSGCANCELNOTIFICATION( i )
# endif

# define CAN_RX_INDEX_TBL(channel,id)                      (CanRxIndexTbl[channel][id])

#endif

#ifndef CAN_POLLING_IRQ_DISABLE
# define CAN_POLLING_IRQ_DISABLE(i)                        (CanCanInterruptDisable(i))
#endif
#ifndef CAN_POLLING_IRQ_RESTORE
# define CAN_POLLING_IRQ_RESTORE(i)                        (CanCanInterruptRestore(i))
#endif


#if defined( C_ENABLE_INDIVIDUAL_POLLING )
/* define first index to array CanHwObjIndivPolling[][] */
#  define CAN_HWOBJINDIVPOLLING_INDEX0                     (canHwChannel)
#endif


/* mask for range enable status */
#define kCanRange0                                         ((vuint16)1)
#define kCanRange1                                         ((vuint16)2)
#define kCanRange2                                         ((vuint16)4)
#define kCanRange3                                         ((vuint16)8)


/* Assertions ----------------------------------------------------------------*/
/*lint -function(exit,ApplCanFatalError)*/

/*lint -emacro( (506), assertUser) */
#if defined( C_ENABLE_USER_CHECK )
# if defined( C_SINGLE_RECEIVE_CHANNEL )
#  define assertUser(p,c,e)                                if (!(p))   {ApplCanFatalError(e);}
# else
#  define assertUser(p,c,e)                                if (!(p))   {ApplCanFatalError((c),(e));}
# endif
#else
# define assertUser(p,c,e)
#endif

/*lint -emacro( (506), assertGen) */
#if defined( C_ENABLE_GEN_CHECK )
# if defined( C_SINGLE_RECEIVE_CHANNEL )
#  define assertGen(p,c,e)                                 if (!(p))   {ApplCanFatalError(e);}
# else
#  define assertGen(p,c,e)                                 if (!(p))   {ApplCanFatalError((c),(e));}
# endif
#else
# define assertGen(p,c,e)
#endif

/*lint -emacro( (506), assertHardware) */
#if defined( C_ENABLE_HARDWARE_CHECK )
# if defined( C_SINGLE_RECEIVE_CHANNEL )
#  define assertHardware(p,c,e)                            if (!(p))   {ApplCanFatalError(e);}
# else
#  define assertHardware(p,c,e)                            if (!(p))   {ApplCanFatalError((c),(e));}
# endif
#else
# define assertHardware(p,c,e)
#endif

/*lint -emacro( (506), assertInternal) */
#if defined( C_ENABLE_INTERNAL_CHECK )
# if defined( C_SINGLE_RECEIVE_CHANNEL )
#  define assertInternal(p,c,e)                            if (!(p))   {ApplCanFatalError(e);}
# else
#  define assertInternal(p,c,e)                            if (!(p))   {ApplCanFatalError((c),(e));}
# endif
#else
# define assertInternal(p,c,e)
#endif

#if defined( C_ENABLE_TRANSMIT_QUEUE )
#if defined( C_CPUTYPE_32BIT )
# define kCanTxQueueSize      ((kCanTxQueueBytes + 3) >> 2)
# define kCanTxQueueShift     5
#endif

#if defined( C_CPUTYPE_16BIT )
# define kCanTxQueueSize      ((kCanTxQueueBytes + 1) >> 1)
# define kCanTxQueueShift     4
#endif

#if defined( C_CPUTYPE_8BIT )
# define kCanTxQueueSize      kCanTxQueueBytes
# define kCanTxQueueShift     3
#endif

/* mask used to get the flag index from the handle */
# define kCanTxQueueMask      (((vuint8)1 << kCanTxQueueShift) - (vuint8)1)


#if defined( C_SINGLE_RECEIVE_CHANNEL )
# define CAN_HL_TXQUEUE_PADBITS(channel)             ((CanTransmitHandle)0)
# define CAN_HL_TXQUEUE_STARTINDEX(channel)          ((CanSignedTxHandle)0)
# define CAN_HL_TXQUEUE_STOPINDEX(channel)           ((CanSignedTxHandle)kCanTxQueueSize)
#else
# define CAN_HL_TXQUEUE_PADBITS(channel)             ((CanTransmitHandle)CanTxQueuePadBits[(channel)])
# define CAN_HL_TXQUEUE_STARTINDEX(channel)          (CanTxQueueStartIndex[(channel)])
# define CAN_HL_TXQUEUE_STOPINDEX(channel)           (CanTxQueueStartIndex[(channel) + (CanChannelHandle)1])
#endif


#endif

#if defined(C_ENABLE_HW_LOOP_TIMER) 
#  define APPLCANTIMERSTART(a) ApplCanTimerStart(CAN_CHANNEL_CANPARA_FIRST a)
#  define APPLCANTIMERLOOP(a)  ApplCanTimerLoop(CAN_CHANNEL_CANPARA_FIRST a)
#  define APPLCANTIMEREND(a)   ApplCanTimerEnd(CAN_CHANNEL_CANPARA_FIRST a)
#else
# define APPLCANTIMERSTART(a)
# define APPLCANTIMERLOOP(a)  ((vuint8)kCanOk)
# define APPLCANTIMEREND(a)
#endif

#if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
/* mask to check for valid CPU Tx interrupts */
# define kCanPendingTxObjMask   ((vuint8)0x06)
#endif


#if defined (C_SINGLE_RECEIVE_CHANNEL)
# define CAN_CNTRL_BASIS_ADR(channel)       ((tCanCntrlRegBlock MEMORY_CAN *)(V_REG_BLOCK_ADR + kCanBasisAdr))
#else
# define CAN_CNTRL_BASIS_ADR(channel)       ((tCanCntrlRegBlock MEMORY_CAN *)(V_REG_BLOCK_ADR + CanBasisAdr[channel]))
#endif

#define CAN_RX_MAILBOX_BASIS_ADR(channel)   ((tMsgObject MEMORY_CAN *)&(CAN_CNTRL_BASIS_ADR(channel) -> CanRxBuf))
#define CAN_TX_MAILBOX_BASIS_ADR(channel)   ((tMsgObject MEMORY_CAN *)&(CAN_CNTRL_BASIS_ADR(channel) -> CanTxBuf))

#define CTL0                                (CAN_CNTRL_BASIS_ADR(channel) -> CanCTL0)
#define CTL1                                (CAN_CNTRL_BASIS_ADR(channel) -> CanCTL1)
#define CBTR0                               (CAN_CNTRL_BASIS_ADR(channel) -> CanCBTR0)
#define CBTR1                               (CAN_CNTRL_BASIS_ADR(channel) -> CanCBTR1)
#define CRFLG                               (CAN_CNTRL_BASIS_ADR(channel) -> CanCRFLG)
#define CRIER                               (CAN_CNTRL_BASIS_ADR(channel) -> CanCRIER)
#define CANTFLG                             (CAN_CNTRL_BASIS_ADR(channel) -> CanCTFLG)
#define CTIER                               (CAN_CNTRL_BASIS_ADR(channel) -> CanCTIER)
#define CTARQ                               (CAN_CNTRL_BASIS_ADR(channel) -> CanCTARQ)
#define CTAAK                               (CAN_CNTRL_BASIS_ADR(channel) -> CanCTAAK)
#define CTBSEL                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCTBSEL)
#define CIDAC                               (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDAC)
#define CMISC                               (CAN_CNTRL_BASIS_ADR(channel) -> CanCMISC)
#define CRXERR                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCRXERR)
#define CTXERR                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCTXERR)
#define CIDAR0                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDAR0)
#define CIDAR1                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDAR1)
#define CIDAR2                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDAR2)
#define CIDAR3                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDAR3)
#define CIDMR0                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDMR0)
#define CIDMR1                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDMR1)
#define CIDMR2                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDMR2)
#define CIDMR3                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDMR3)
#define CIDAR4                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDAR4)
#define CIDAR5                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDAR5)
#define CIDAR6                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDAR6)
#define CIDAR7                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDAR7)
#define CIDMR4                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDMR4)
#define CIDMR5                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDMR5)
#define CIDMR6                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDMR6)
#define CIDMR7                              (CAN_CNTRL_BASIS_ADR(channel) -> CanCIDMR7)

# if defined(C_SINGLE_RECEIVE_CHANNEL)
# define CanLL_CanInterruptDisable(canHwChannel, localInterruptOldFlagPtr)  CanCanInterruptDisableInternal((localInterruptOldFlagPtr))
# else
# define CanLL_CanInterruptDisable(canHwChannel, localInterruptOldFlagPtr)  CanCanInterruptDisableInternal((canHwChannel), (localInterruptOldFlagPtr))
# endif

# if defined(C_SINGLE_RECEIVE_CHANNEL)
# define CanLL_CanInterruptRestore(canHwChannel, localInterruptOldFlag)  CanCanInterruptRestoreInternal((localInterruptOldFlag))
# else
# define CanLL_CanInterruptRestore(canHwChannel, localInterruptOldFlag)  CanCanInterruptRestoreInternal((canHwChannel), (localInterruptOldFlag))
# endif

#if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
#define CanLL_TxIsHWObjFree(canHwChannel, txObjHandle)   (((CANTFLG & CanMailboxSelect[txObjHandle]) != 0) ? kCanTrue : kCanFalse)
#endif


#if defined( C_ENABLE_SLEEP_WAKEUP )
# define CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY)  (((CTL1 & SLPAK) == SLPAK) ? kCanTrue : kCanFalse)
#else
# define CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY)  (kCanFalse)
#endif

#define CanLL_HwIsStop(CAN_HW_CHANNEL_CANPARA_ONLY)   (((CTL1 & INITAK) == INITAK) ? kCanTrue : kCanFalse)

#define CanLL_HwIsBusOff(CAN_HW_CHANNEL_CANPARA_ONLY) (((CRFLG & BOFFIF) == BOFFIF) ?  kCanTrue : kCanFalse)

#if defined( C_ENABLE_EXTENDED_STATUS )
# define CanLL_HwIsPassive(CAN_HW_CHANNEL_CANPARA_ONLY) ((((CRFLG & (TSTAT1 | TSTAT0)) == TSTAT1) || ((CRFLG & (RSTAT1 | RSTAT0)) == RSTAT1)) ? kCanTrue : kCanFalse)

# define CanLL_HwIsWarning(CAN_HW_CHANNEL_CANPARA_ONLY) ((((CRFLG & (TSTAT1 | TSTAT0)) == TSTAT0) || ((CRFLG & (RSTAT1 | RSTAT0)) == RSTAT0)) ? kCanTrue : kCanFalse)
#endif /* C_ENABLE_EXTENDED_STATUS */

/***************************************************************************/
/* Defines / data types / structs / unions                                 */
/***************************************************************************/

#if defined( C_ENABLE_TRANSMIT_QUEUE )
/* declare data type for the queue */
#if defined( C_CPUTYPE_32BIT )
typedef vuint32 tCanQueueElementType;
#endif
#if defined( C_CPUTYPE_16BIT )
typedef vuint16 tCanQueueElementType;
#endif
#if defined( C_CPUTYPE_8BIT )
typedef vuint8  tCanQueueElementType;
#endif
#endif
#define CAN_INLINE_FCT

/* Define CAN Chip hardware; segment must be located in locator file    */
/* register layout of the can chip                                      */
/* Structure describing CAN receive buffer. */

/* The description of the hardware registers is located in the header because
   the XGate part needs also these typedefs
*/
#if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
# if defined(C_ENABLE_XGATE_USED)
typedef struct
{
  volatile vuint16* pXGif;
  vuint16           nTxMask;
  vuint16           nRxMask;
  vuint16           nErrorMask;
  vuint16           nWakeUpMask;
}
tCanIrqChannelInfo;
# endif
#endif

/* this structure is used to save the current CAN Irq status */
typedef volatile struct
{
  vuint8 oldCanCRIER;
  vuint8 oldCanCTIER;
}
tCanIrqRegister;

typedef tCanIrqRegister tCanLLCanIntOld;


/****************************************************************************/
/* Constants                                                                */
/****************************************************************************/

#if defined( C_ENABLE_TRANSMIT_QUEUE )
/* ROM CATEGORY 1 START*/
/* lookup table for setting the flags in the queue */
V_MEMROM0 static V_MEMROM1 tCanQueueElementType V_MEMROM2 CanShiftLookUp[1 << kCanTxQueueShift] = 
{
#if defined( C_CPUTYPE_32BIT ) || defined( C_CPUTYPE_16BIT ) || defined( C_CPUTYPE_8BIT )
  (tCanQueueElementType)0x00000001, (tCanQueueElementType)0x00000002, (tCanQueueElementType)0x00000004, (tCanQueueElementType)0x00000008, 
  (tCanQueueElementType)0x00000010, (tCanQueueElementType)0x00000020, (tCanQueueElementType)0x00000040, (tCanQueueElementType)0x00000080
#endif

#if defined( C_CPUTYPE_32BIT ) || defined( C_CPUTYPE_16BIT )
 ,(tCanQueueElementType)0x00000100, (tCanQueueElementType)0x00000200, (tCanQueueElementType)0x00000400, (tCanQueueElementType)0x00000800, 
  (tCanQueueElementType)0x00001000, (tCanQueueElementType)0x00002000, (tCanQueueElementType)0x00004000, (tCanQueueElementType)0x00008000
#endif

#if defined( C_CPUTYPE_32BIT )
 ,(tCanQueueElementType)0x00010000, (tCanQueueElementType)0x00020000, (tCanQueueElementType)0x00040000, (tCanQueueElementType)0x00080000, 
  (tCanQueueElementType)0x00100000, (tCanQueueElementType)0x00200000, (tCanQueueElementType)0x00400000, (tCanQueueElementType)0x00800000,
  (tCanQueueElementType)0x01000000, (tCanQueueElementType)0x02000000, (tCanQueueElementType)0x04000000, (tCanQueueElementType)0x08000000, 
  (tCanQueueElementType)0x10000000, (tCanQueueElementType)0x20000000, (tCanQueueElementType)0x40000000, (tCanQueueElementType)0x80000000
#endif
};

/* returns the highest pending flag from the lower nibble */
V_MEMROM0 static V_MEMROM1 vsint8 V_MEMROM2 CanGetHighestFlagFromNibble[16] = 
{
  -1,                /* (vsint8)0xFF - changed due to misra */
  0x00,
  0x01, 0x01,
  0x02, 0x02, 0x02, 0x02,
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03
};
/* ROM CATEGORY 1 END*/
#endif
/* Global constants with CAN driver main and subversion */
/* ROM CATEGORY 4 START*/
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kCanMainVersion   = (vuint8)(( DRVCAN_VERSION ) >> 8);  /*lint !e572 !e778*/
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kCanSubVersion    = (vuint8)(  DRVCAN_VERSION & 0x00FF );
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kCanBugFixVersion = (vuint8)(  DRVCAN_RELEASE_VERSION );
/* ROM CATEGORY 4 END*/

#if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
static V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 CanMailboxSelect[3] = 
{ 
  (vuint8)0x01, (vuint8)0x02, (vuint8)0x04
};

static V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 CanFirstMailbox[8] =
{
  (vuint8)0xFF, (vuint8)0x00, (vuint8)0x01, (vuint8)0x00,
  (vuint8)0x02, (vuint8)0x00, (vuint8)0x01, (vuint8)0x00
};

#endif

#if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
# if defined(C_ENABLE_XGATE_USED)
/* index = (MSCAN HW Offset >> 6) & 0x7 */
V_MEMROM0 V_MEMROM1 tCanIrqChannelInfo V_MEMROM2 CanXGateIrqChannelInfo[5] =
{
  {(volatile vuint16*)0x38C, 0x0100, 0x0200, 0x0400, 0x0800 }, /* CAN 0 */
  {(volatile vuint16*)0x38C, 0x0010, 0x0020, 0x0040, 0x0080 }, /* CAN 1 */
  {(volatile vuint16*)0x38C, 0x0001, 0x0002, 0x0004, 0x0008 }, /* CAN 2 */
  {(volatile vuint16*)0x38E, 0x1000, 0x2000, 0x4000, 0x8000 }, /* CAN 3 */
  {(volatile vuint16*)0x38E, 0x0100, 0x0200, 0x0400, 0x0800 }  /* CAN 4 */
};

V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 CanXGateChannelToInfo[8] = 
{
  (vuint8)0x03, /* Baseaddress: 0x200 -> Irq Channel 0x4d */
  (vuint8)0xFF,
  (vuint8)0x04, /* Baseaddress: 0x280 -> Irq Channel 0x49 */
  (vuint8)0xFF,
  (vuint8)0xFF,
  (vuint8)0x00, /* Baseaddress: 0x140 -> Irq Channel 0x59 */
  (vuint8)0x01, /* Baseaddress: 0x180 -> Irq Channel 0x55 */
  (vuint8)0x02, /* Baseaddress: 0x1C0 -> Irq Channel 0x51 */
};
# endif
#endif

#if defined(C_ENABLE_CAN_RAM_CHECK)
# if defined(C_ENABLE_EXTENDED_ID)    
V_MEMROM0 V_MEMROM1 static vuint32 V_MEMROM2 CanMemCheckId[3] = {0x55555555,0xAAAAAAAA,0x00000000};
# else
V_MEMROM0 V_MEMROM1 static vuint16 V_MEMROM2 CanMemCheckId[3]   = {0x5555,0xAAAA,0x0000};
# endif
V_MEMROM0 V_MEMROM1 static vuint8 V_MEMROM2 CanMemCheckData[3] = {0x55,0xAA,0x00};
V_MEMROM0 V_MEMROM1 static vuint8 V_MEMROM2 CanMemCheckDlc[3]  = {0x08,0x06,0x01};
V_MEMROM0 V_MEMROM1 static vuint8 V_MEMROM2 CanMemCheckPrio[3] = {0x01,0x02,0x04};
#endif

/***************************************************************************/
/* CAN-Hardware Data Definitions                                            */
/***************************************************************************/


/***************************************************************************/
/* external declarations                                                    */
/***************************************************************************/

#if !defined( CANDRV_SET_CODE_TEST_POINT )
# define CANDRV_SET_CODE_TEST_POINT(x)
#else
extern vuint8 tscCTKTestPointState[CTK_MAX_TEST_POINT];
#endif

/***************************************************************************/
/* global data definitions                                                 */
/***************************************************************************/

/* RAM CATEGORY 1 START*/
volatile CanReceiveHandle canRxHandle[kCanNumberOfChannels];
/* RAM CATEGORY 1 END*/

/* RAM CATEGORY 3 START*/
#if defined( C_ENABLE_CONFIRMATION_FCT ) && \
    defined( C_ENABLE_DYN_TX_OBJECTS )   && \
    defined( C_ENABLE_TRANSMIT_QUEUE )
CanTransmitHandle          confirmHandle[kCanNumberOfChannels];
#endif
/* RAM CATEGORY 3 END*/

/* RAM CATEGORY 1 START*/
#if defined( C_ENABLE_CONFIRMATION_FLAG )
/* Msg(4:0750) A union type has been used. MISRA Rules 110 - no change */
V_MEMRAM0 volatile V_MEMRAM1_NEAR union CanConfirmationBits V_MEMRAM2_NEAR CanConfirmationFlags;
#endif

#if defined( C_ENABLE_INDICATION_FLAG )
/* Msg(4:0750) A union type has been used. MISRA Rules 110 - no change */
V_MEMRAM0 volatile V_MEMRAM1_NEAR union CanIndicationBits   V_MEMRAM2_NEAR CanIndicationFlags;
#endif
/* RAM CATEGORY 1 END*/

/* RAM CATEGORY 1 START*/
#if defined( C_ENABLE_VARIABLE_RX_DATALEN )
/* ##RI1.4 - 3.31: Dynamic Receive DLC */
volatile vuint8 canVariableRxDataLen[kCanNumberOfRxObjects];
#endif
/* RAM CATEGORY 1 END*/

/* RAM CATEGORY 1 START*/
CanChipDataPtr canRDSRxPtr[kCanNumberOfChannels];
/* RAM CATEGORY 1 END*/

/* RAM CATEGORY 1 START*/
CanChipDataPtr canRDSTxPtr[kCanNumberOfUsedTxCANObjects];
/* RAM CATEGORY 1 END*/


/***************************************************************************/
/* local data definitions                                                  */
/***************************************************************************/

/* support for CAN driver features : */
/* RAM CATEGORY 1 START*/
static volatile CanTransmitHandle canHandleCurTxObj[kCanNumberOfUsedTxCANObjects];
/* RAM CATEGORY 1 END*/

/* RAM CATEGORY 2 START*/
#if defined( C_ENABLE_ECU_SWITCH_PASS )
static vuint8 canPassive[kCanNumberOfChannels];
#endif
/* RAM CATEGORY 2 END*/

/* RAM CATEGORY 2 START*/
#if defined( C_ENABLE_CAN_RAM_CHECK )
static vuint8 canComStatus[kCanNumberOfChannels]; /* stores the decision of the App after the last CAN RAM check */
#endif

#if defined( C_ENABLE_DYN_TX_OBJECTS )
static volatile vuint8 canTxDynObjReservedFlag[kCanNumberOfTxDynObjects];

# if defined( C_ENABLE_DYN_TX_ID )
static tCanTxId0 canDynTxId0[kCanNumberOfTxDynObjects];
#  if (kCanNumberOfUsedCanTxIdTables > 1)
static tCanTxId1 canDynTxId1[kCanNumberOfTxDynObjects];
#  endif
#  if (kCanNumberOfUsedCanTxIdTables > 2)
static tCanTxId2 canDynTxId2[kCanNumberOfTxDynObjects];
#  endif
#  if (kCanNumberOfUsedCanTxIdTables > 3)
static tCanTxId3 canDynTxId3[kCanNumberOfTxDynObjects];
#  endif
#  if (kCanNumberOfUsedCanTxIdTables > 4)
static tCanTxId4 canDynTxId4[kCanNumberOfTxDynObjects];
#  endif
#  if defined( C_ENABLE_MIXED_ID )
#   if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#   else
static tCanIdType                 canDynTxIdType[kCanNumberOfTxDynObjects];
#   endif
#  endif
# endif

# if defined( C_ENABLE_DYN_TX_DLC )
static vuint8                   canDynTxDLC[kCanNumberOfTxDynObjects];
# endif
# if defined( C_ENABLE_DYN_TX_DATAPTR )
static vuint8*                  canDynTxDataPtr[kCanNumberOfTxDynObjects];
# endif
# if defined( C_ENABLE_CONFIRMATION_FCT )
# endif 
#endif /* C_ENABLED_DYN_TX_OBJECTS */


#if defined( C_ENABLE_TX_MASK_EXT_ID )
static tCanTxId0 canTxMask0[kCanNumberOfChannels];
# if (kCanNumberOfUsedCanTxIdTables > 1)
static tCanTxId1 canTxMask1[kCanNumberOfChannels];
# endif
# if (kCanNumberOfUsedCanTxIdTables > 2)
static tCanTxId2 canTxMask2[kCanNumberOfChannels];
# endif
# if (kCanNumberOfUsedCanTxIdTables > 3)
static tCanTxId3 canTxMask3[kCanNumberOfChannels];
# endif
# if (kCanNumberOfUsedCanTxIdTables > 4)
static tCanTxId4 canTxMask4[kCanNumberOfChannels];
# endif
#endif

#if defined( C_ENABLE_VARIABLE_DLC )
static vuint8 canTxDLC_RAM[kCanNumberOfTxObjects];
#endif

#if defined( C_HL_ENABLE_COPROCESSOR_SUPPORT )
#else
static volatile vuint8 canStatus[kCanNumberOfChannels];

# if defined( C_ENABLE_PART_OFFLINE )
static vuint8 canTxPartOffline[kCanNumberOfChannels];
# endif
#endif
/* RAM CATEGORY 2 END*/

/* RAM CATEGORY 1 START*/
static vsintx          canCanInterruptCounter[kCanNumberOfChannels];
#if defined( C_HL_ENABLE_CAN_IRQ_DISABLE )
static tCanLLCanIntOld canCanInterruptOldStatus[kCanNumberOfHwChannels];
#endif
/* RAM CATEGORY 1 END*/

#if defined( C_HL_ENABLE_LAST_INIT_OBJ )
/* RAM CATEGORY 4 START*/
static CanInitHandle lastInitObject[kCanNumberOfChannels];
/* RAM CATEGORY 4 END*/
#endif
#if defined( C_ENABLE_TX_POLLING )          || \
    defined( C_ENABLE_RX_BASICCAN_POLLING ) || \
    defined( C_ENABLE_ERROR_POLLING )       || \
    defined( C_ENABLE_WAKEUP_POLLING )      
/* RAM CATEGORY 2 START*/
static vuint8 canPollingTaskActive[kCanNumberOfChannels];
/* RAM CATEGORY 2 END*/
#endif

/* RAM CATEGORY 1 START*/
static tCanRxInfoStruct        canRxInfoStruct[kCanNumberOfChannels];

#if defined( C_ENABLE_CAN_TX_CONF_FCT )
/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
static tCanTxConfInfoStruct    txInfoStructConf[kCanNumberOfChannels];
#endif

#if defined( C_ENABLE_COND_RECEIVE_FCT )
static volatile vuint8 canMsgCondRecState[kCanNumberOfChannels];
#endif

#if defined( C_ENABLE_RX_QUEUE )
# if defined( C_ENABLE_RX_QUEUE_EXTERNAL )
static tCanRxQueue* canRxQueuePtr;     /* pointer to the rx queue */
#  define canRxQueue                                       (*canRxQueuePtr)
# else
static tCanRxQueue canRxQueue;         /* the rx queue (buffer and queue variables) */
# endif
#endif
/* RAM CATEGORY 1 END*/

#if defined( C_ENABLE_TRANSMIT_QUEUE )
/* RAM CATEGORY 1 START*/
static volatile tCanQueueElementType canTxQueueFlags[kCanTxQueueSize];
/* RAM CATEGORY 1 END*/
#endif
#if defined( C_ENABLE_CAN_RAM_CHECK )
/* ROM CATEGORY 4 START*/
V_MEMROM0 static V_MEMROM1 vuint8  V_MEMROM2 CanMemCheckValues8bit[3] = 
{
  0xAA, 0x55, 0x00
};
V_MEMROM0 static V_MEMROM1 vuint16 V_MEMROM2 CanMemCheckValues16bit[3] = 
{
  0xAAAA, 0x5555, 0x0000
};
V_MEMROM0 static V_MEMROM1 vuint32 V_MEMROM2 CanMemCheckValues32bit[3] = 
{
  0xAAAAAAAAUL, 0x55555555UL, 0x00000000UL
};
/* ROM CATEGORY 4 END*/
#endif

#if defined(C_HL_ENABLE_COPROCESSOR_SUPPORT)
# if defined( C_ENABLE_PART_OFFLINE )
static canuint8 canTxPartOffline[kCanNumberOfChannels];
# endif
static volatile canuint8 canStatus[kCanNumberOfChannels];
#endif

/* CanTransmit needs to know if the Tx Irq is disabled */
static vuintx canCanIrqDisabled[kCanNumberOfChannels];

#if defined(C_ENABLE_XGATE_USED)
/* counter for the global XGate interrupt enable/ restore */
static vuintx canCanXGateIrqtCount;
#endif

#if defined(C_ENABLE_TX_POLLING)
static volatile vuint8 canllTxStatusFlag[kCanNumberOfChannels];
#endif


/***************************************************************************/
/*  local prototypes                                                       */
/***************************************************************************/
#if defined( C_ENABLE_RX_BASICCAN_OBJECTS )
# if defined( C_ENABLE_RX_QUEUE )
/* CODE CATEGORY 1 START*/
static vuint8 CanHL_ReceivedRxHandleQueue(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 1 END*/
# endif
/* CODE CATEGORY 1 START*/
# if defined( C_ENABLE_RX_QUEUE )
static vuint8 CanHL_ReceivedRxHandle( CAN_CHANNEL_CANTYPE_FIRST tCanRxInfoStruct *pCanRxInfoStruct );
# else
static vuint8 CanHL_ReceivedRxHandle( CAN_CHANNEL_CANTYPE_ONLY );
# endif
/* CODE CATEGORY 1 END*/
# if defined( C_ENABLE_INDICATION_FLAG ) || \
     defined( C_ENABLE_INDICATION_FCT )
/* CODE CATEGORY 1 START*/
static void CanHL_IndRxHandle( CanReceiveHandle rxHandle );
/* CODE CATEGORY 1 END*/
# endif
#endif
#if defined( C_ENABLE_RX_BASICCAN_OBJECTS )
/* CODE CATEGORY 1 START*/
static void CanBasicCanMsgReceived(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle);  
/* CODE CATEGORY 1 END*/
#endif

/* CODE CATEGORY 1 START*/
static void CanHL_TxConfirmation(CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle);
/* CODE CATEGORY 1 END*/
# if defined( C_ENABLE_CAN_TRANSMIT )
/* CODE CATEGORY 1 START*/
static vuint8 CanCopyDataAndStartTransmission(CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle, CanTransmitHandle txHandle)  C_API_3;   /*lint !e14 !e31*/
/* CODE CATEGORY 1 END*/
# endif   /* C_ENABLE_CAN_TRANSMIT */

#if defined( C_ENABLE_TRANSMIT_QUEUE )
/* CODE CATEGORY 4 START*/
static void CanDelQueuedObj( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 4 END*/
# if defined( C_ENABLE_TX_POLLING ) 
/* CODE CATEGORY 2 START*/
static void CanHl_RestartTxQueue( CAN_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 2 END*/
# endif
#endif 

/* CODE CATEGORY 2 START*/
static void CanHL_ErrorHandling( CAN_HW_CHANNEL_CANTYPE_ONLY );
/* CODE CATEGORY 2 END*/

#if defined( C_ENABLE_VARIABLE_RX_DATALEN )
/* CODE CATEGORY 1 START*/
static void CanSetVariableRxDatalen ( CanReceiveHandle rxHandle, vuint8 dataLen );
/* CODE CATEGORY 1 END*/
#endif


#if defined(C_ENABLE_SLEEP_WAKEUP)
/* CODE CATEGORY 4 START*/
static void CanLL_WakeUpHandling(CAN_CHANNEL_CANTYPE_ONLY);
/* CODE CATEGORY 4 END*/
#endif

static void CanCanInterruptDisableInternal(CAN_CHANNEL_CANTYPE_FIRST tCanLLCanIntOld* pOldFlagPtr);
static void CanCanInterruptRestoreInternal(CAN_CHANNEL_CANTYPE_FIRST tCanLLCanIntOld nOldFlag);

#if defined(C_ENABLE_CAN_RAM_CHECK)
/* CODE CATEGORY 4 START*/
static vuint8 CanLL_IsMailboxCorrupt(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle hwObjHandle);
/* CODE CATEGORY 4 END*/
#endif
/* CODE CATEGORY 1 START*/
/* CODE CATEGORY 1 END*/

/***************************************************************************/
/*  Error Check                                                            */
/***************************************************************************/

/***************** error check for Organi process **************************/







/***************** error check for not supported feature  **************************/




#if defined( C_ENABLE_COMMON_CAN )
# error "Common CAN is not supported with this CAN driver implementation"
#endif

#if defined( C_ENABLE_MULTI_ECU_CONFIG )
# error "Multiple Configuration is not supported with this CAN driver implementation"
#endif


#if (VSTDLIB__COREHLL_VERSION  <  0x0213 )
# error "Incompatible version of VStdLib. Core Version 2.13.00 or higher is necessary."
#endif

#if defined( C_CPUTYPE_8BIT )
# if (kCanNumberOfTxObjects > 250)
#  error "Too many transmit messages. This driver can only handle up to 250 transmit messages"
# endif
# if (kCanNumberOfRxObjects > 250)
#  error "Too many receive messages. This driver can only handle up to 250 receive messages"
# endif
#endif

#if defined( C_SEARCH_HASH )
# if defined( C_ENABLE_EXTENDED_ID ) 
#  if ( (kHashSearchMaxStepsEx < 1U) ||(kHashSearchMaxStepsEx > 32768U) )
#   error "kHashSearchMaxStepsEx has ilegal value"
#  endif
#  if (kHashSearchListCountEx > 32768U)
#   error "Hash table for extended ID is too large"
#  endif
# endif
# if defined( C_ENABLE_MIXED_ID ) || !defined( C_ENABLE_EXTENDED_ID ) 
#  if ( (kHashSearchMaxSteps < 1U) ||(kHashSearchMaxSteps > 32768U) )
#   error "kHashSearchMaxStepsEx has ilegal value"
#  endif
#  if (kHashSearchListCount > 32768U)
#   error "Hash table for standard ID is too large"
#  endif
# endif
#endif





#if defined( C_ENABLE_RX_QUEUE )
# if defined( C_CPUTYPE_8BIT ) && ( kCanRxQueueSize > 256 )
#  error "With 8 Bit CPU the Rx queue size has to be smaller or equal to 256"
# endif
# if !defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
#  error "RX-Queue requires C_HL_ENABLE_RX_INFO_STRUCT_PTR"
# endif
#endif



/***************************************************************************/
/*  Functions                                                              */
/***************************************************************************/


/****************************************************************************
| NAME:             CanCanInterruptDisableInternal
| CALLED BY:        CanCanInterruptDisable
| PRECONDITIONS:    none
|
| INPUT PARAMETERS: nChannel: CAN channel to be disabled
|                   pOldFlagPtr: pointer to variable which stores the current state
|
| RETURN VALUES:    -
|
| DESCRIPTION:      disables CAN IRQs
****************************************************************************/
static void CanCanInterruptDisableInternal(CAN_CHANNEL_CANTYPE_FIRST tCanLLCanIntOld* pOldFlagPtr)
{
  CanDeclareGlobalInterruptOldStatus
  CanNestedGlobalInterruptDisable();
  CanBeginCriticalXGateSection();
  pOldFlagPtr -> oldCanCRIER = CRIER;
  pOldFlagPtr -> oldCanCTIER = CTIER;
  CRIER = kCanCRIERInterruptsDisabledVal;
  CTIER = (vuint8)0;
  CanDisableXGateInterrupts();
  canCanIrqDisabled[channel] = kCanTrue;
  CanEndCriticalXGateSection();
  CanNestedGlobalInterruptRestore();
}

/****************************************************************************
| NAME:             CanCanInterruptRestoreInternal
| CALLED BY:        CanCanInterruptRestore
| PRECONDITIONS:    none
|
| INPUT PARAMETERS: channel: CAN channel to be restored
|                   nOldFlag: the old IRQ register value
|
| RETURN VALUES:    -
|
| DESCRIPTION:      retores the previous CAN IRQ state
****************************************************************************/
static void CanCanInterruptRestoreInternal(CAN_CHANNEL_CANTYPE_FIRST tCanLLCanIntOld nOldFlag)
{
  CanDeclareGlobalInterruptOldStatus
  CanNestedGlobalInterruptDisable();
  CanBeginCriticalXGateSection();
  CRIER = nOldFlag.oldCanCRIER;
  CTIER = nOldFlag.oldCanCTIER;
  CanRestoreXGateInterrupts();
  canCanIrqDisabled[channel] = kCanFalse;
  CanEndCriticalXGateSection();
  CanNestedGlobalInterruptRestore();
}

# if defined (C_ENABLE_MEMCOPY_SUPPORT)
/****************************************************************************
| NAME:             CanCopyFromCan
| CALLED BY:        Application
| PRECONDITIONS:    none
|
| INPUT PARAMETERS: void *             dst        | pointer to destionation buffer
|                   CanChipDataPtr     src        | pointer to CAN buffer
|                   vuint8             len        | number of bytes to copy
|
| RETURN VALUES:    -
|
| DESCRIPTION:      copy data from CAN receive buffer to RAM.
|
| ATTENTION:        
****************************************************************************/
/* Msg(4:3673) The object addressed by the pointer "txMsgStruct" is not modified in this function.
   The use of "const" should be considered to indicate that it never changes. MISRA Rule 81 - no change */
/* CODE CATEGORY 1 START*/
void CanCopyFromCan(void *dst, CanChipDataPtr src, vuint8 len)
{
  (void)VStdMemCpyRamToRam((void*)dst, (void*)src, len);
}
/* CODE CATEGORY 1 END*/

/****************************************************************************
| NAME:             CanCopyToCan
| CALLED BY:        Application
| PRECONDITIONS:    none
|
| INPUT PARAMETERS: void *             src        | pointer to source buffer
|                   CanChipDataPtr     dst        | pointer to CAN buffer
|                   vuint8             len        | number of bytes to copy
|
| RETURN VALUES:    -
|
| DESCRIPTION:      copy data from CAN receive buffer to RAM.
|
| ATTENTION:        
****************************************************************************/
/* Msg(4:3673) The object addressed by the pointer "txMsgStruct" is not modified in this function.
   The use of "const" should be considered to indicate that it never changes. MISRA Rule 81 - no change */
/* CODE CATEGORY 1 START*/
void CanCopyToCan(CanChipDataPtr dst, void * src, vuint8 len)
{
  (void)VStdMemCpyRamToRam((void*)dst, (void*)src, len);
}
/* CODE CATEGORY 1 END*/
# endif

#if defined( C_ENABLE_SLEEP_WAKEUP ) 
/****************************************************************************
| NAME:             CanLL_WakeUpHandling
| CALLED BY:        CanWakeUpTask() and WakeUp-ISR
| PRECONDITIONS:    none
|
| INPUT PARAMETERS: CanChannelHandle  channel        | current CAN channel
|                   and parameters which are defined in CanLL_WakeUpTaskLocalParameter
|
| RETURN VALUES:    -
|
| DESCRIPTION:      perform wakeup handling.
|
| ATTENTION:        CanLL_WakeUpHandling has to contain:
|                     ApplCanPreWakeUp(CAN_CHANNEL_CANPARA_ONLY);
|                     CanWakeUp(CAN_CHANNEL_CANPARA_ONLY);        - if hardware does not wake up automatically
|                     APPL_CAN_WAKEUP( channel );
|                   In case of CommonCAN, it might be necessary to call CanWakeUp even if the hardware
|                   wakes up automatically to make sure all associated HW channels are awaken.
****************************************************************************/
/* CODE CATEGORY 4 START*/
static void CanLL_WakeUpHandling(CAN_CHANNEL_CANTYPE_ONLY)
{
# if defined(C_ENABLE_APPLCANPREWAKEUP_FCT)
  ApplCanPreWakeUp(CAN_CHANNEL_CANPARA_ONLY); /* if hardware doesn't wake up automatically */
# endif
  (void)CanWakeUp(CAN_CHANNEL_CANPARA_ONLY);
  CRFLG = WUPIF; /* acknowledge wakeup flag */
  APPL_CAN_WAKEUP(channel);
}
/* CODE CATEGORY 4 END*/
#endif

# if defined(C_ENABLE_CAN_RAM_CHECK)
/* CODE CATEGORY 4 START*/
static vuint8 CanLL_IsMailboxCorrupt(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle hwObjHandle)
{  
  vuint8 i;
  vuint8 j;
  vuint8 returnVal;

  returnVal = kCanFalse;

  if(hwObjHandle != 0)
  {
    CTBSEL = (vuint8)hwObjHandle;
    for(i=0; i<3; i++)
    {
# if defined(C_ENABLE_EXTENDED_ID)
      CAN_TX_MAILBOX_BASIS_ADR(channel)->Id = CanMemCheckId[i];
# else
      CAN_TX_MAILBOX_BASIS_ADR(channel)->Id   = CanMemCheckId[i];
      CAN_TX_MAILBOX_BASIS_ADR(channel)->IdEx = CanMemCheckId[i];
# endif
      for(j=0; j<8; j++)
      {
        CAN_TX_MAILBOX_BASIS_ADR(channel)->DataFld[j] = CanMemCheckData[i];
      }
      CAN_TX_MAILBOX_BASIS_ADR(channel)->Dlc  = CanMemCheckDlc[i];
      CAN_TX_MAILBOX_BASIS_ADR(channel)->Prio = CanMemCheckPrio[i];

# if defined(C_ENABLE_EXTENDED_ID)
      if(CAN_TX_MAILBOX_BASIS_ADR(channel)->Id != CanMemCheckId[i])
# else
      if((CAN_TX_MAILBOX_BASIS_ADR(channel)->Id != CanMemCheckId[i]) ||
         (CAN_TX_MAILBOX_BASIS_ADR(channel)->IdEx != CanMemCheckId[i]))
# endif
      {
        returnVal = kCanTrue;
      }
      for(j=0; j<8; j++)
      {
        if(CAN_TX_MAILBOX_BASIS_ADR(channel)->DataFld[j] != CanMemCheckData[i])
        {
          returnVal = kCanTrue;
        }
      }
      if(CAN_TX_MAILBOX_BASIS_ADR(channel)->Dlc != CanMemCheckDlc[i])
      {
        returnVal = kCanTrue;
      }
      if(CAN_TX_MAILBOX_BASIS_ADR(channel)->Prio != CanMemCheckPrio[i])
      {
        returnVal = kCanTrue;
      }
    }
  }
  
  return returnVal;
}
/* CODE CATEGORY 4 END*/
# endif

#if ((defined( C_ENABLE_RX_BASICCAN_OBJECTS ) && !defined( C_ENABLE_RX_BASICCAN_POLLING )) || \
     !defined( C_ENABLE_TX_POLLING )        || \
      defined( C_ENABLE_INDIVIDUAL_POLLING ) || \
      !defined( C_ENABLE_ERROR_POLLING )     ||\
      !defined( C_ENABLE_WAKEUP_POLLING ))        /* ISR necessary; no pure polling configuration*/
/****************************************************************************
| NAME:             Interrupt Functions
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
|  Attention:  <Name> has to be repleased with the name of the ISR. 
|              Naming conventions: with Name = "","Rx","Tx","RxTx","Wakeup","Status" 
|  The name of the ISR will always have a channel index at the end. Even in single channel
|  systems.
****************************************************************************/

/* CODE CATEGORY 1 START*/
# if defined(C_ENABLE_ISR_SPLIT)

#  if !defined(C_ENABLE_RX_BASICCAN_POLLING)
/****************************************************************************
| NAME:             CanRxIrqHandler
| CALLED BY:        Can Rx interrupt 
| PRECONDITIONS:    Rx interrupt enabled
| INPUT PARAMETERS: CAN channel on which the interrupt was fired
| RETURN VALUES:    none
| DESCRIPTION:      codedoubled to indexed IRQ handling
****************************************************************************/
C_API_1 void C_API_2 CanRxIrqHandler(CAN_CHANNEL_CANTYPE_ONLY)
{
  CanBasicCanMsgReceived(CAN_CHANNEL_CANPARA_FIRST 0);
}
#  endif

#  if !defined(C_ENABLE_TX_POLLING)
/****************************************************************************
| NAME:             CanTxIrqHandler
| CALLED BY:        Can Tx interrupt 
| PRECONDITIONS:    Tx interrupt enabled
| INPUT PARAMETERS: CAN channel on which the interrupt was fired
| RETURN VALUES:    none
| DESCRIPTION:      codedoubled to indexed IRQ handling
****************************************************************************/
C_API_1 void C_API_2 CanTxIrqHandler(CAN_CHANNEL_CANTYPE_ONLY)
{
  CanHL_TxConfirmation(CAN_CHANNEL_CANPARA_FIRST CanFirstMailbox[CTIER & CANTFLG]);
}
#  endif

#  if !defined(C_ENABLE_ERROR_POLLING)
/****************************************************************************
| NAME:             CanErrorIrqHandler
| CALLED BY:        Can Error interrupt 
| PRECONDITIONS:    Error interrupt enabled
| INPUT PARAMETERS: CAN channel on which the interrupt was fired
| RETURN VALUES:    none
| DESCRIPTION:      codedoubled to indexed IRQ handling
****************************************************************************/
C_API_1 void C_API_2 CanErrorIrqHandler(CAN_CHANNEL_CANTYPE_ONLY)
{
  CanHL_ErrorHandling(CAN_CHANNEL_CANPARA_ONLY);
}
#  endif

#  if defined(C_ENABLE_SLEEP_WAKEUP)
#   if !defined(C_ENABLE_WAKEUP_POLLING)
/****************************************************************************
| NAME:             CanWakeUpIrqHandler
| CALLED BY:        Can Wakeup interrupt 
| PRECONDITIONS:    Wakeup interrupt enabled
| INPUT PARAMETERS: CAN channel on which the interrupt was fired
| RETURN VALUES:    none
| DESCRIPTION:      codedoubled to indexed IRQ handling
****************************************************************************/
C_API_1 void C_API_2 CanWakeUpIrqHandler(CAN_CHANNEL_CANTYPE_ONLY)
{
  CanLL_WakeUpHandling(CAN_CHANNEL_CANPARA_ONLY);
}
#   endif
#  endif

# endif /* C_ENABLE_ISR_SPLIT */

#endif /* C_ENABLE_RX_BASICCAN_OBJECTS... */
#if ((defined( C_ENABLE_RX_BASICCAN_OBJECTS ) && !defined( C_ENABLE_RX_BASICCAN_POLLING )) || \
     !defined( C_ENABLE_TX_POLLING )        || \
      defined( C_ENABLE_INDIVIDUAL_POLLING ) || \
      !defined( C_ENABLE_ERROR_POLLING )     ||\
      !defined( C_ENABLE_WAKEUP_POLLING ))        /* ISR necessary; no pure polling configuration*/


# if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  


#   if defined(C_ENABLE_ISR_SPLIT)
#   else /*C_ENABLE_ISR_SPLIT */


#    if defined(kCanPhysToLogChannelIndex_0)

/****************************************************************************
| NAME:             CanRxInterrupt_0
| CALLED BY:
| PRECONDITIONS:    Rx interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN Rx interrupt function
****************************************************************************/

#     if (!defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS)) || \
         (defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS))
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanRxInterrupt_0Cat)
#        if (osdIsrCanRxInterrupt_0Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanRxInterrupt_0)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanRxInterrupt_0Cat)
#        if (osdIsrCanRxInterrupt_0Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_RX_IRQ_FUNC(CanRxInterrupt_0)
#      endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanBasicCanMsgReceived(0);
#      else
  CanBasicCanMsgReceived((CanChannelHandle)kCanPhysToLogChannelIndex_0, 0);
#      endif
}
#     endif


/****************************************************************************
| NAME:             CanTxInterrupt_0
| CALLED BY:
| PRECONDITIONS:    Tx interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN Tx interrupt function
****************************************************************************/

#     if (!defined(C_ENABLE_TX_POLLING)) || (defined(C_ENABLE_TX_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING))
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanTxInterrupt_0Cat)
#        if (osdIsrCanTxInterrupt_0Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanTxInterrupt_0)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanTxInterrupt_0Cat)
#        if (osdIsrCanTxInterrupt_0Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_TX_IRQ_FUNC(CanTxInterrupt_0)
#       endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanHL_TxConfirmation(CanFirstMailbox[CTIER & CANTFLG & kCanPendingTxObjMask]);
#      else
  CanChannelHandle channel;
  channel = (CanChannelHandle)kCanPhysToLogChannelIndex_0;
  CanHL_TxConfirmation(channel, CanFirstMailbox[CTIER & CANTFLG & kCanPendingTxObjMask]);
#      endif
}
#     endif


/****************************************************************************
| NAME:             CanWakeUpInterrupt_0
| CALLED BY:
| PRECONDITIONS:    Wakeup interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN wakeup interrupt function
****************************************************************************/

#     if defined(C_ENABLE_SLEEP_WAKEUP)
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanWakeUpInterrupt_0Cat)
#        if (osdIsrCanWakeUpInterrupt_0Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanWakeUpInterrupt_0)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanWakeUpInterrupt_0Cat)
#        if (osdIsrCanWakeUpInterrupt_0Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_WAKEUP_IRQ_FUNC(CanWakeUpInterrupt_0)
#      endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanLL_WakeUpHandling();
#      else
  CanLL_WakeUpHandling((CanChannelHandle)kCanPhysToLogChannelIndex_0);
#      endif
}
#     endif


/****************************************************************************
| NAME:             CanErrorInterrupt_0
| CALLED BY:
| PRECONDITIONS:    Error interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN error interrupt function
****************************************************************************/

#     if !defined(C_ENABLE_ERROR_POLLING)
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanErrorInterrupt_0Cat)
#        if (osdIsrCanErrorInterrupt_0Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanErrorInterrupt_0)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanErrorInterrupt_0Cat)
#        if (osdIsrCanErrorInterrupt_0Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_ERROR_IRQ_FUNC(CanErrorInterrupt_0)
#      endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanHL_ErrorHandling();
#      else
  CanHL_ErrorHandling((CanChannelHandle)kCanPhysToLogChannelIndex_0);
#      endif
}
#     endif


#    endif /* kCanPhysToLogChannelIndex_0 */


#    if defined(kCanPhysToLogChannelIndex_1)

/****************************************************************************
| NAME:             CanRxInterrupt_1
| CALLED BY:
| PRECONDITIONS:    Rx interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN Rx interrupt function
****************************************************************************/

#     if (!defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS)) || \
         (defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS))
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanRxInterrupt_1Cat)
#        if (osdIsrCanRxInterrupt_1Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanRxInterrupt_1)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanRxInterrupt_1Cat)
#        if (osdIsrCanRxInterrupt_1Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_RX_IRQ_FUNC(CanRxInterrupt_1)
#      endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanBasicCanMsgReceived(0);
#      else
  CanBasicCanMsgReceived((CanChannelHandle)kCanPhysToLogChannelIndex_1, 0);
#      endif
}
#     endif


/****************************************************************************
| NAME:             CanTxInterrupt_1
| CALLED BY:
| PRECONDITIONS:    Tx interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN Tx interrupt function
****************************************************************************/

#     if (!defined(C_ENABLE_TX_POLLING)) || (defined(C_ENABLE_TX_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING))
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanTxInterrupt_1Cat)
#        if (osdIsrCanTxInterrupt_1Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanTxInterrupt_1)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanTxInterrupt_1Cat)
#        if (osdIsrCanTxInterrupt_1Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_TX_IRQ_FUNC(CanTxInterrupt_1)
#       endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanHL_TxConfirmation(CanFirstMailbox[CTIER & CANTFLG & kCanPendingTxObjMask]);
#      else
  CanChannelHandle channel;
  channel = (CanChannelHandle)kCanPhysToLogChannelIndex_1;
  CanHL_TxConfirmation(channel, CanFirstMailbox[CTIER & CANTFLG & kCanPendingTxObjMask]);
#      endif
}
#     endif


/****************************************************************************
| NAME:             CanWakeUpInterrupt_1
| CALLED BY:
| PRECONDITIONS:    Wakeup interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN wakeup interrupt function
****************************************************************************/

#     if defined(C_ENABLE_SLEEP_WAKEUP)
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanWakeUpInterrupt_1Cat)
#        if (osdIsrCanWakeUpInterrupt_1Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanWakeUpInterrupt_1)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanWakeUpInterrupt_1Cat)
#        if (osdIsrCanWakeUpInterrupt_1Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_WAKEUP_IRQ_FUNC(CanWakeUpInterrupt_1)
#      endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanLL_WakeUpHandling();
#      else
  CanLL_WakeUpHandling((CanChannelHandle)kCanPhysToLogChannelIndex_1);
#      endif
}
#     endif


/****************************************************************************
| NAME:             CanErrorInterrupt_1
| CALLED BY:
| PRECONDITIONS:    Error interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN error interrupt function
****************************************************************************/

#     if !defined(C_ENABLE_ERROR_POLLING)
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanErrorInterrupt_1Cat)
#        if (osdIsrCanErrorInterrupt_1Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanErrorInterrupt_1)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanErrorInterrupt_1Cat)
#        if (osdIsrCanErrorInterrupt_1Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_ERROR_IRQ_FUNC(CanErrorInterrupt_1)
#      endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanHL_ErrorHandling();
#      else
  CanHL_ErrorHandling((CanChannelHandle)kCanPhysToLogChannelIndex_1);
#      endif
}
#     endif


#    endif /* kCanPhysToLogChannelIndex_1 */


#    if defined(kCanPhysToLogChannelIndex_2)

/****************************************************************************
| NAME:             CanRxInterrupt_2
| CALLED BY:
| PRECONDITIONS:    Rx interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN Rx interrupt function
****************************************************************************/

#     if (!defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS)) || \
         (defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS))
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanRxInterrupt_2Cat)
#        if (osdIsrCanRxInterrupt_2Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanRxInterrupt_2)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanRxInterrupt_2Cat)
#        if (osdIsrCanRxInterrupt_2Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_RX_IRQ_FUNC(CanRxInterrupt_2)
#      endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanBasicCanMsgReceived(0);
#      else
  CanBasicCanMsgReceived((CanChannelHandle)kCanPhysToLogChannelIndex_2, 0);
#      endif
}
#     endif


/****************************************************************************
| NAME:             CanTxInterrupt_2
| CALLED BY:
| PRECONDITIONS:    Tx interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN Tx interrupt function
****************************************************************************/

#     if (!defined(C_ENABLE_TX_POLLING)) || (defined(C_ENABLE_TX_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING))
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanTxInterrupt_2Cat)
#        if (osdIsrCanTxInterrupt_2Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanTxInterrupt_2)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanTxInterrupt_2Cat)
#        if (osdIsrCanTxInterrupt_2Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_TX_IRQ_FUNC(CanTxInterrupt_2)
#       endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanHL_TxConfirmation(CanFirstMailbox[CTIER & CANTFLG & kCanPendingTxObjMask]);
#      else
  CanChannelHandle channel;
  channel = (CanChannelHandle)kCanPhysToLogChannelIndex_2;
  CanHL_TxConfirmation(channel, CanFirstMailbox[CTIER & CANTFLG & kCanPendingTxObjMask]);
#      endif
}
#     endif


/****************************************************************************
| NAME:             CanWakeUpInterrupt_2
| CALLED BY:
| PRECONDITIONS:    Wakeup interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN wakeup interrupt function
****************************************************************************/

#     if defined(C_ENABLE_SLEEP_WAKEUP)
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanWakeUpInterrupt_2Cat)
#        if (osdIsrCanWakeUpInterrupt_2Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanWakeUpInterrupt_2)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanWakeUpInterrupt_2Cat)
#        if (osdIsrCanWakeUpInterrupt_2Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_WAKEUP_IRQ_FUNC(CanWakeUpInterrupt_2)
#      endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanLL_WakeUpHandling();
#      else
  CanLL_WakeUpHandling((CanChannelHandle)kCanPhysToLogChannelIndex_2);
#      endif
}
#     endif


/****************************************************************************
| NAME:             CanErrorInterrupt_2
| CALLED BY:
| PRECONDITIONS:    Error interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN error interrupt function
****************************************************************************/

#     if !defined(C_ENABLE_ERROR_POLLING)
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanErrorInterrupt_2Cat)
#        if (osdIsrCanErrorInterrupt_2Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanErrorInterrupt_2)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanErrorInterrupt_2Cat)
#        if (osdIsrCanErrorInterrupt_2Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_ERROR_IRQ_FUNC(CanErrorInterrupt_2)
#      endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanHL_ErrorHandling();
#      else
  CanHL_ErrorHandling((CanChannelHandle)kCanPhysToLogChannelIndex_2);
#      endif
}
#     endif


#    endif /* kCanPhysToLogChannelIndex_2 */


#    if defined(kCanPhysToLogChannelIndex_3)

/****************************************************************************
| NAME:             CanRxInterrupt_3
| CALLED BY:
| PRECONDITIONS:    Rx interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN Rx interrupt function
****************************************************************************/

#     if (!defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS)) || \
         (defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS))
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanRxInterrupt_3Cat)
#        if (osdIsrCanRxInterrupt_3Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanRxInterrupt_3)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanRxInterrupt_3Cat)
#        if (osdIsrCanRxInterrupt_3Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_RX_IRQ_FUNC(CanRxInterrupt_3)
#      endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanBasicCanMsgReceived(0);
#      else
  CanBasicCanMsgReceived((CanChannelHandle)kCanPhysToLogChannelIndex_3, 0);
#      endif
}
#     endif


/****************************************************************************
| NAME:             CanTxInterrupt_3
| CALLED BY:
| PRECONDITIONS:    Tx interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN Tx interrupt function
****************************************************************************/

#     if (!defined(C_ENABLE_TX_POLLING)) || (defined(C_ENABLE_TX_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING))
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanTxInterrupt_3Cat)
#        if (osdIsrCanTxInterrupt_3Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanTxInterrupt_3)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanTxInterrupt_3Cat)
#        if (osdIsrCanTxInterrupt_3Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_TX_IRQ_FUNC(CanTxInterrupt_3)
#       endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanHL_TxConfirmation(CanFirstMailbox[CTIER & CANTFLG & kCanPendingTxObjMask]);
#      else
  CanChannelHandle channel;
  channel = (CanChannelHandle)kCanPhysToLogChannelIndex_3;
  CanHL_TxConfirmation(channel, CanFirstMailbox[CTIER & CANTFLG & kCanPendingTxObjMask]);
#      endif
}
#     endif


/****************************************************************************
| NAME:             CanWakeUpInterrupt_3
| CALLED BY:
| PRECONDITIONS:    Wakeup interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN wakeup interrupt function
****************************************************************************/

#     if defined(C_ENABLE_SLEEP_WAKEUP)
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanWakeUpInterrupt_3Cat)
#        if (osdIsrCanWakeUpInterrupt_3Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanWakeUpInterrupt_3)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanWakeUpInterrupt_3Cat)
#        if (osdIsrCanWakeUpInterrupt_3Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_WAKEUP_IRQ_FUNC(CanWakeUpInterrupt_3)
#      endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanLL_WakeUpHandling();
#      else
  CanLL_WakeUpHandling((CanChannelHandle)kCanPhysToLogChannelIndex_3);
#      endif
}
#     endif


/****************************************************************************
| NAME:             CanErrorInterrupt_3
| CALLED BY:
| PRECONDITIONS:    Error interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN error interrupt function
****************************************************************************/

#     if !defined(C_ENABLE_ERROR_POLLING)
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanErrorInterrupt_3Cat)
#        if (osdIsrCanErrorInterrupt_3Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanErrorInterrupt_3)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanErrorInterrupt_3Cat)
#        if (osdIsrCanErrorInterrupt_3Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_ERROR_IRQ_FUNC(CanErrorInterrupt_3)
#      endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanHL_ErrorHandling();
#      else
  CanHL_ErrorHandling((CanChannelHandle)kCanPhysToLogChannelIndex_3);
#      endif
}
#     endif


#    endif /* kCanPhysToLogChannelIndex_3 */


#    if defined(kCanPhysToLogChannelIndex_4)

/****************************************************************************
| NAME:             CanRxInterrupt_4
| CALLED BY:
| PRECONDITIONS:    Rx interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN Rx interrupt function
****************************************************************************/

#     if (!defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS)) || \
         (defined(C_ENABLE_RX_BASICCAN_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING) && defined(C_ENABLE_RX_BASICCAN_OBJECTS))
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanRxInterrupt_4Cat)
#        if (osdIsrCanRxInterrupt_4Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanRxInterrupt_4)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanRxInterrupt_4Cat)
#        if (osdIsrCanRxInterrupt_4Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_RX_IRQ_FUNC(CanRxInterrupt_4)
#      endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanBasicCanMsgReceived(0);
#      else
  CanBasicCanMsgReceived((CanChannelHandle)kCanPhysToLogChannelIndex_4, 0);
#      endif
}
#     endif


/****************************************************************************
| NAME:             CanTxInterrupt_4
| CALLED BY:
| PRECONDITIONS:    Tx interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN Tx interrupt function
****************************************************************************/

#     if (!defined(C_ENABLE_TX_POLLING)) || (defined(C_ENABLE_TX_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING))
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanTxInterrupt_4Cat)
#        if (osdIsrCanTxInterrupt_4Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanTxInterrupt_4)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanTxInterrupt_4Cat)
#        if (osdIsrCanTxInterrupt_4Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_TX_IRQ_FUNC(CanTxInterrupt_4)
#       endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanHL_TxConfirmation(CanFirstMailbox[CTIER & CANTFLG & kCanPendingTxObjMask]);
#      else
  CanChannelHandle channel;
  channel = (CanChannelHandle)kCanPhysToLogChannelIndex_4;
  CanHL_TxConfirmation(channel, CanFirstMailbox[CTIER & CANTFLG & kCanPendingTxObjMask]);
#      endif
}
#     endif


/****************************************************************************
| NAME:             CanWakeUpInterrupt_4
| CALLED BY:
| PRECONDITIONS:    Wakeup interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN wakeup interrupt function
****************************************************************************/

#     if defined(C_ENABLE_SLEEP_WAKEUP)
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanWakeUpInterrupt_4Cat)
#        if (osdIsrCanWakeUpInterrupt_4Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanWakeUpInterrupt_4)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanWakeUpInterrupt_4Cat)
#        if (osdIsrCanWakeUpInterrupt_4Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_WAKEUP_IRQ_FUNC(CanWakeUpInterrupt_4)
#      endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanLL_WakeUpHandling();
#      else
  CanLL_WakeUpHandling((CanChannelHandle)kCanPhysToLogChannelIndex_4);
#      endif
}
#     endif


/****************************************************************************
| NAME:             CanErrorInterrupt_4
| CALLED BY:
| PRECONDITIONS:    Error interrupt has to be enabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      MS-CAN error interrupt function
****************************************************************************/

#     if !defined(C_ENABLE_ERROR_POLLING)
#      if defined(C_ENABLE_OSEK_OS) && defined(C_ENABLE_OSEK_OS_INTCAT2)
#       if defined(osdIsrCanErrorInterrupt_4Cat)
#        if (osdIsrCanErrorInterrupt_4Cat != 2)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
ISR(CanErrorInterrupt_4)
#      else
#       if defined(C_ENABLE_OSEK_OS) && defined (osdIsrCanErrorInterrupt_4Cat)
#        if (osdIsrCanErrorInterrupt_4Cat != 1)
#         error "inconstant configuration of Osek-OS interrupt categorie between CANgen and OIL-configurator (CanIsr)"
#        endif
#       endif
CAN_ERROR_IRQ_FUNC(CanErrorInterrupt_4)
#      endif /* C_ENABLE_OSEK_OS */
{
#      if defined(C_SINGLE_RECEIVE_CHANNEL)
  CanHL_ErrorHandling();
#      else
  CanHL_ErrorHandling((CanChannelHandle)kCanPhysToLogChannelIndex_4);
#      endif
}
#     endif


#    endif /* kCanPhysToLogChannelIndex_4 */


#   endif /* C_ENABLE_ISR_SPLIT */



# endif /* C_COMP_COSMIC_MCS12X_MSCAN12... */


/* CODE CATEGORY 1 END*/
#endif

/****************************************************************************
| NAME:             CanInit
| CALLED BY:        CanInitPowerOn(), Network management
| PRECONDITIONS:    none
| INPUT PARAMETERS: Handle to initstructure
| RETURN VALUES:    none
| DESCRIPTION:      initialization of chip-hardware
|                   initialization of receive and transmit message objects
****************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanInit( CAN_CHANNEL_CANTYPE_FIRST CanInitHandle initObject )
{
#if defined( C_ENABLE_CAN_RAM_CHECK )
  vuint8                 canRamCheckStatus;
#endif
  CanObjectHandle        hwObjHandle;
#if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION ) || \
    defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
  CanTransmitHandle      txHandle;
#endif
  CanObjectHandle        logTxObjHandle;

  #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  # if defined(C_SEARCH_XGATE)
  vsintx i;
  # endif
  #endif
  vuint8 tmpCRIER;
  /*for TMM change CAN ID*/
//CanRxId0[28] = MK_STDID0(TMM_CAN_MSG[Rx_TMM_Sent]&0x7FF);                /* 0x302,			 Rx_TMM_Sent,           */
//CanRxId0[27] = MK_STDID0(TMM_CAN_MSG[Rx_TMM_Angle]&0x7FF);               /* 0x303,			 Rx_TMM_Angle,          */
//CanRxId0[26] = MK_STDID0(TMM_CAN_MSG[Rx_TMM_Limit_Config]&0x7FF);        /* 0x511			 Rx_TMM_Limit_Config,    */
//CanRxId0[25] = MK_STDID0(TMM_CAN_MSG[Rx_TMM_CAN_To_ANA]&0x7FF);          /* 0x3C2,			 Rx_TMM_CAN_To_ANA,      */
//CanRxId0[24] = MK_STDID0(TMM_CAN_MSG[Rx_TMM_PID]&0x7FF);                 /* 0x522,			 Rx_TMM_PID,             */
//CanRxId0[10] = MK_STDID0(TMM_CAN_MSG[Rx_TMM_LIN]&0x7FF);                 /* 0x310,			 Rx_TMM_LIN,             */
//CanRxId0[20] = MK_STDID0(TMM_CAN_MSG[Rx_TMM_New_CANID]&0x7FF);                  /* 0x091,	can not change the ID which change others' ID   */   

CanTxId0[66] = MK_STDID0(TMM_CAN_ID_Default[Tx_ECU_EndStopCounter]&0x7FF);                /* 0x400u,  		 Tx_TMM_Sent_Raw0 ,       */
CanTxId0[65] = MK_STDID0(TMM_CAN_ID_Default[Tx_ECU_SW_Version]&0x7FF);                /* 0x401u,  		 Tx_TMM_Sent_Raw1 ,       */
CanTxId0[64] = MK_STDID0(TMM_CAN_ID_Default[Tx_ECU_Modi]&0x7FF);                /* 0x402u,  		 Tx_TMM_Sent_Raw2 ,       */
CanTxId0[63] = MK_STDID0(TMM_CAN_ID_Default[Tx_ECU_AngleOutput]&0x7FF);                /* 0x403u,  		 Tx_TMM_Sent_Raw3 ,       */
CanTxId0[62] = MK_STDID0(TMM_CAN_ID_Default[Tx_ECU_SENT_ErrorCount]&0x7FF);                /* 0x404u,  		 Tx_TMM_Sent_Raw4 ,       */
CanTxId0[61] = MK_STDID0(TMM_CAN_ID_Default[Tx_ECU_LimitOutput]&0x7FF);                /* 0x405u,  		 Tx_TMM_Sent_Raw5 ,       */
CanTxId0[60] = MK_STDID0(TMM_CAN_ID_Default[Tx_ECU_RegelParaOutput]&0x7FF);                /* 0x406u,  		 Tx_TMM_Sent_Raw6 ,       */
CanTxId0[59] = MK_STDID0(TMM_CAN_ID_Default[Tx_ECU_RefPosOutput]&0x7FF);                /* 0x407u,  		 Tx_TMM_Sent_Raw7 ,       */
CanTxId0[58] = MK_STDID0(TMM_CAN_ID_Default[Tx_ECU_SENTModeOutput]&0x7FF);                /* 0x408u,  		 Tx_TMM_Sent_Raw8 ,       */
CanTxId0[57] = MK_STDID0(TMM_CAN_ID_Default[Tx_Module2_low]&0x7FF);                /* 0x409u,  		 Tx_TMM_Sent_Raw9 ,       */
CanTxId0[56] = MK_STDID0(TMM_CAN_ID_Default[Tx_Module2_high]&0x7FF);                /* 0x40Au,  	     Tx_TMM_Sent_Raw10,      */
CanTxId0[55] = MK_STDID0(TMM_CAN_ID_Default[Tx_Module1_low]&0x7FF);                /* 0x40Bu,  	     Tx_TMM_Sent_Raw11,      */
CanTxId0[54] = MK_STDID0(TMM_CAN_ID_Default[Tx_Module1_high]&0x7FF);                /* 0x40Cu,  	     Tx_TMM_Sent_Raw12,      */
CanTxId0[53] = MK_STDID0(TMM_CAN_ID_Default[Tx_LIN_RSCV_Data_B]&0x7FF);                /* 0x40Du,  	     Tx_TMM_Sent_Raw13,      */
CanTxId0[52] = MK_STDID0(TMM_CAN_ID_Default[Tx_LIN_RSCV_Output_B]&0x7FF);                /* 0x40Eu,  	     Tx_TMM_Sent_Raw14,      */
CanTxId0[51] = MK_STDID0(TMM_CAN_ID_Default[Tx_LIN_RSCV_Data_A]&0x7FF);                /* 0x40Fu,  	     Tx_TMM_Sent_Raw15,      */
CanTxId0[50] = MK_STDID0(TMM_CAN_ID_Default[Tx_LIN_RSCV_Output_A]&0x7FF);                /* 0x410u,  	     Tx_TMM_Sent_Raw16,      */
//CanTxId0[49] = MK_STDID0(TMM_CAN_ID_Default[Tx_ANA_A1A3]&0x7FF);                /* 0x411u,        Tx_TMM_Sent_Raw17,      */
//CanTxId0[49] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Sent_Raw17]&0x7FF);                /* 0x411u,  	     Tx_TMM_Sent_Raw17,      */
//CanTxId0[48] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Sent_Raw18]&0x7FF);                /* 0x412u,  	     Tx_TMM_Sent_Raw18,      */
//CanTxId0[47] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Sent_Raw19]&0x7FF);                /* 0x413u,  	     Tx_TMM_Sent_Raw19,      */
//CanTxId0[46] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Sent_Raw20]&0x7FF);                /* 0x414u,  	     Tx_TMM_Sent_Raw20,      */
//CanTxId0[45] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Sent_Raw21]&0x7FF);                /* 0x415u,  	     Tx_TMM_Sent_Raw21,      */
//CanTxId0[44] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Sent_Raw22]&0x7FF);                /* 0x416u,  	     Tx_TMM_Sent_Raw22,      */
//CanTxId0[43] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Sent_Raw23]&0x7FF);                /* 0x417u,  	     Tx_TMM_Sent_Raw23,      */
//CanTxId0[42] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Sent_Raw24]&0x7FF);                /* 0x418u,  	     Tx_TMM_Sent_Raw24,      */
CanTxId0[86] = MK_STDID0(TMM_CAN_ID_Default[Tx_ECU_SENT_B]&0x7FF);               /* 0x5B0u,    		 Tx_TMM_Current_Raw0 ,    */
//CanTxId0[85] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Current_Raw1]&0x7FF);               /* 0x5B1u,         	 Tx_TMM_Current_Raw1 ,    */
//CanTxId0[84] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Current_Raw2]&0x7FF);               /* 0x5B2u,         	 Tx_TMM_Current_Raw2 ,    */
//CanTxId0[83] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Current_Raw3]&0x7FF);               /* 0x5B3u,         	 Tx_TMM_Current_Raw3 ,    */
//CanTxId0[82] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Current_Raw4]&0x7FF);               /* 0x5B4u,         	 Tx_TMM_Current_Raw4 ,    */
//CanTxId0[81] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Current_Raw5]&0x7FF);               /* 0x5B5u,         	 Tx_TMM_Current_Raw5 ,    */
//CanTxId0[80] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Current_Raw6]&0x7FF);               /* 0x5B6u,         	 Tx_TMM_Current_Raw6 ,    */
//CanTxId0[79] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Current_Raw7]&0x7FF);               /* 0x5B7u,         	 Tx_TMM_Current_Raw7 ,    */
//CanTxId0[78] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Current_Raw8]&0x7FF);               /* 0x5B8u,         	 Tx_TMM_Current_Raw8 ,    */
//CanTxId0[77] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Current_Raw9]&0x7FF);               /* 0x5B9u,         	 Tx_TMM_Current_Raw9 ,    */
//CanTxId0[76] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Current_Raw10]&0x7FF);               /* 0x5BAu,         	 Tx_TMM_Current_Raw10,   */
//CanTxId0[75] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Current_Raw11]&0x7FF);               /* 0x5BBu,         	 Tx_TMM_Current_Raw11,   */
//CanTxId0[74] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Current_Raw12]&0x7FF);               /* 0x5BCu,         	 Tx_TMM_Current_Raw12,   */
//CanTxId0[88] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Display1]&0x7FF);                    /* 0x301u,         	 Tx_TMM_Display1,  BMS_CH1      */
//CanTxId0[87] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Display2]&0x7FF);                    /* 0x304u,         	 Tx_TMM_Display2,  BMS_CH2      */
//CanTxId0[73] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Display3]&0x7FF);                    /* 0x5BDu,	         Tx_TMM_Display3,  BMS_Diag_14     */
//CanTxId0[72] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Display4]&0x7FF);                    /* 0x5BEu,			 Tx_TMM_Display4,  BMS_Diag_15  */
//CanTxId0[71] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Display5]&0x7FF);                    /* 0x5BFu				 Tx_TMM_Display5,  BMS_Diag_16  */
CanTxId0[70] = MK_STDID0(TMM_CAN_ID_Default[Tx_ECU_SENT_A]&0x7FF);               /* 0x5B0u,         Tx_TMM_Current_Raw0 ,    */

/* TMM2.0 RX */
CanRxId0[5] = MK_STDID0(TMM_CAN_ID_Default[Rx_ECU_LimitInput]&0x7FF);       /* 0x7A2u MCU_4           */
CanRxId0[6] = MK_STDID0(TMM_CAN_ID_Default[Rx_ECU_RegelParaInput]&0x7FF);       /* 0x7A3u MCU_3           */
CanRxId0[9] = MK_STDID0(TMM_CAN_ID_Default[Rx_ECU_AngleInput]&0x7FF);       /* 0x7A6u ECM_BMS_00      */
CanRxId0[11] = MK_STDID0(TMM_CAN_ID_Default[Rx_ECU_SENTModeInput]&0x7FF);       /* 0x7A7u  VIN             */
CanRxId0[12] = MK_STDID0(TMM_CAN_ID_Default[Rx_CAN2ANA_RX2]&0x7FF);       /* 0x7A8u  CarConfig       */
CanRxId0[18] = MK_STDID0(TMM_CAN_ID_Default[Rx_CAN2ANA_RX1]&0x7FF);      /* 0x7AEu  CCU_1           */
CanRxId0[19] = MK_STDID0(TMM_CAN_ID_Default[Rx_LIN_RSCV_Input_B]&0x7FF);      /* 0x7AEu  DCT_1           */
CanRxId0[20] = MK_STDID0(TMM_CAN_ID_Default[Rx_LIN_RSCV_Input_A]&0x7FF);      /* 0x7AEu  ECM_6           */
CanRxId0[25] = MK_STDID0(TMM_CAN_ID_Default[Rx_LIN_SEQ_Confg1]&0x7FF);      /* 0x7AEu  ISA_Result_msg1          */
CanRxId0[24] = MK_STDID0(TMM_CAN_ID_Default[Rx_LIN_SEQ_Confg2]&0x7FF);      /* 0x7AEu  ISA_Result_msg2           */
CanRxId0[21] = MK_STDID0(TMM_CAN_ID_Default[Rx_LIN_SEQ_SEL]&0x7FF);      /* 0x7AEu  DMsut_rdreq           */
CanRxId0[23] = MK_STDID0(TMM_CAN_ID_Default[Rx_TMM_SEQ_Confg]&0x7FF);      /* 0x7AEu  ISA_Result_msg3           */


/* TMM Spare CAN ID */
//  CanRxId0[3] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Extern0]&0x7FF);       /* 0x7A0u	ECM_14          */
//  CanRxId0[4] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Extern1]&0x7FF);       /* 0x7A1u	DCDC_1          */
//  CanRxId0[7] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Extern4]&0x7FF);       /* 0x7A4u	MCU_2           */
//  CanRxId0[8] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Extern5]&0x7FF);       /* 0x7A5u	ECM_HMI         */
//  CanRxId0[13] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Extern9]&0x7FF);       /* 0x7A9u	CentralTime     */
//  CanRxId0[14] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Extern10]&0x7FF);      /* 0x7AAu	HVAC_SDCP       */
//  CanRxId0[15] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Extern11]&0x7FF);      /* 0x7ABu	SBCPTStatus     */
//  CanRxId0[16] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Extern12]&0x7FF);      /* 0x7ACu	IC_HCAN         */
//  CanRxId0[17] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Extern13]&0x7FF);      /* 0x7ADu	BCPowertrain    */
//  
//  CanRxId0[19] = MK_STDID0(TMM_CAN_MSG[Tx_TMM_Extern15]&0x7FF);      /* 0x7AFu	DCT_1           */

/* change can3 baud rate */
//CanInitObject[1].CanInitCBTR0 = TMM_CAN_BTR0;

#if defined( C_HL_ENABLE_LAST_INIT_OBJ )  
  lastInitObject[channel] = initObject;
#endif

#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
/* Msg(4:3759) Implicit conversion: int to unsigned short. MISRA Rule 43 - no change in RI 1.4 */
  initObject  += CAN_HL_INIT_OBJ_STARTINDEX(channel);
#endif


#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
#endif
  assertUser(initObject < kCanNumberOfInitObjects, channel, kErrorInitObjectHdlTooLarge);
  assertUser(initObject < CAN_HL_INIT_OBJ_STOPINDEX(channel), channel, kErrorInitObjectHdlTooLarge);

#if defined( C_ENABLE_CAN_RAM_CHECK )
  canRamCheckStatus = kCanRamCheckOk;
#endif

  {
    /* request init mode */
    CTL0 = INITRQ;
    APPLCANTIMERSTART(kCanLoopInitReq);
    while( ((CTL1 & INITAK) == 0) && APPLCANTIMERLOOP(kCanLoopInitReq) )
    {
      ; /* wait while not in initmode */
    }
    APPLCANTIMEREND(kCanLoopInitReq);
    
    CTL1  = (vuint8)(CanInitObject[initObject].CanInitCTL1 | CANE);
    
    #if defined(C_ENABLE_ALTERNATE_CLKSRC)
    CTL1  |= (vuint8)CLKSRC;
    #endif
    
    CBTR0 = CanInitObject[initObject].CanInitCBTR0;     /* set baudrate */
    CBTR1 = CanInitObject[initObject].CanInitCBTR1;
    CIDAC = CanInitObject[initObject].CanInitCIDAC;     /* set filter mode */
    
    /* set acceptance filter registers */
    CIDAR0 = CanInitObject[initObject].CanInitCIDAR0;
    CIDAR1 = CanInitObject[initObject].CanInitCIDAR1;
    CIDAR2 = CanInitObject[initObject].CanInitCIDAR2;
    CIDAR3 = CanInitObject[initObject].CanInitCIDAR3;
    CIDMR0 = CanInitObject[initObject].CanInitCIDMR0;
    CIDMR1 = CanInitObject[initObject].CanInitCIDMR1;
    CIDMR2 = CanInitObject[initObject].CanInitCIDMR2;
    CIDMR3 = CanInitObject[initObject].CanInitCIDMR3;
    CIDAR4 = CanInitObject[initObject].CanInitCIDAR4;
    CIDAR5 = CanInitObject[initObject].CanInitCIDAR5;
    CIDAR6 = CanInitObject[initObject].CanInitCIDAR6;
    CIDAR7 = CanInitObject[initObject].CanInitCIDAR7;
    CIDMR4 = CanInitObject[initObject].CanInitCIDMR4;
    CIDMR5 = CanInitObject[initObject].CanInitCIDMR5;
    CIDMR6 = CanInitObject[initObject].CanInitCIDMR6;
    CIDMR7 = CanInitObject[initObject].CanInitCIDMR7;
    
    if(canCanIrqDisabled[channel] == kCanTrue) /* disable CAN interrupts? */
    {
      canCanInterruptOldStatus[channel].oldCanCTIER = (vuint8)0x00; /* disable all Tx interrupts */
    }
    else
    {
      CanBeginCriticalXGateSection();
      CTIER = (vuint8)0x00; /* disable all Tx interrupts */
      CanEndCriticalXGateSection();
    }
    
    /* leave init mode */
    CTL0 &= (vuint8)~INITRQ;
    CTL0 = (vuint8)(CanInitObject[initObject].CanInitCTL0 & ((vuint8)~INITRQ));
    APPLCANTIMERSTART(kCanLoopExitInit);
    while(((CTL1 & INITAK) == INITAK) && APPLCANTIMERLOOP(kCanLoopExitInit))
    {
      ; /* wait until init mode is left */
    }
    APPLCANTIMEREND(kCanLoopExitInit);
    
    
    # if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
    #  if defined(C_ENABLE_XGATE_USED)
    /* initialize the handle */
    canXgRxHandle[channel] = (CanReceiveHandle)kCanBufferFree;
    canRxSyncObj[channel] = 0; /* unlock the XGate Rx path*/
    #   if defined(C_ENABLE_RANGE_0) || defined(C_ENABLE_RANGE_1) || defined(C_ENABLE_RANGE_2) || defined(C_ENABLE_RANGE_3)
    canXgHitRange[channel] = (vuint8)0x00;
    
    #    if defined(C_ENABLE_RANGE_0)
    #     if defined(C_SINGLE_RECEIVE_CHANNEL)
    #      if(CANRANGE0IDTYPE(channel) == kCanIdTypeStd)
    canRangeConfig[channel].nMask[0] = (tCanRxId0)~MK_RX_RANGE_MASK_IDSTD0(CANRANGE0ACCMASK(channel));
    canRangeConfig[channel].nCode[0] = (tCanRxId0) MK_RX_RANGE_CODE_IDSTD0(CANRANGE0ACCCODE(channel));
    #      else
    canRangeConfig[channel].nMask[0] = (tCanRxId0)~MK_RX_RANGE_MASK_IDEXT0(CANRANGE0ACCMASK(channel));
    canRangeConfig[channel].nCode[0] = (tCanRxId0) MK_RX_RANGE_CODE_IDEXT0(CANRANGE0ACCCODE(channel));
    #      endif
    #     else
    canRangeConfig[channel].nMask[0] = (tCanRxId0)~(CANRANGE0ACCMASK(channel).Id0);
    canRangeConfig[channel].nCode[0] = (tCanRxId0) (CANRANGE0ACCCODE(channel).Id0);
    #     endif
    #    else
    canRangeConfig[channel].nMask[0] = (tCanRxId0)0x00000000;
    canRangeConfig[channel].nCode[0] = (tCanRxId0)0xFFFFFFFF;
    #    endif
    
    #    if defined(C_ENABLE_RANGE_1)
    #     if defined(C_SINGLE_RECEIVE_CHANNEL)
    #      if(CANRANGE1IDTYPE(channel) == kCanIdTypeStd)
    canRangeConfig[channel].nMask[1] = (tCanRxId0)~MK_RX_RANGE_MASK_IDSTD0(CANRANGE1ACCMASK(channel));
    canRangeConfig[channel].nCode[1] = (tCanRxId0) MK_RX_RANGE_CODE_IDSTD0(CANRANGE1ACCCODE(channel));
    #      else
    canRangeConfig[channel].nMask[1] = (tCanRxId0)~MK_RX_RANGE_MASK_IDEXT0(CANRANGE1ACCMASK(channel));
    canRangeConfig[channel].nCode[1] = (tCanRxId0) MK_RX_RANGE_CODE_IDEXT0(CANRANGE1ACCCODE(channel));
    #      endif
    #     else
    canRangeConfig[channel].nMask[1] = (tCanRxId0)~(CANRANGE1ACCMASK(channel).Id0);
    canRangeConfig[channel].nCode[1] = (tCanRxId0) (CANRANGE1ACCCODE(channel).Id0);
    #     endif
    #    else
    canRangeConfig[channel].nMask[1] = (tCanRxId0)0x00000000;
    canRangeConfig[channel].nCode[1] = (tCanRxId0)0xFFFFFFFF;
    #    endif
    
    #    if defined(C_ENABLE_RANGE_2)
    #     if defined(C_SINGLE_RECEIVE_CHANNEL)
    #      if(CANRANGE2IDTYPE(channel) == kCanIdTypeStd)
    canRangeConfig[channel].nMask[2] = (tCanRxId0)~MK_RX_RANGE_MASK_IDSTD0(CANRANGE2ACCMASK(channel));
    canRangeConfig[channel].nCode[2] = (tCanRxId0) MK_RX_RANGE_CODE_IDSTD0(CANRANGE2ACCCODE(channel));
    #      else
    canRangeConfig[channel].nMask[2] = (tCanRxId0)~MK_RX_RANGE_MASK_IDEXT0(CANRANGE2ACCMASK(channel));
    canRangeConfig[channel].nCode[2] = (tCanRxId0) MK_RX_RANGE_CODE_IDEXT0(CANRANGE2ACCCODE(channel));
    #      endif
    #     else
    canRangeConfig[channel].nMask[2] = (tCanRxId0)~(CANRANGE2ACCMASK(channel).Id0);
    canRangeConfig[channel].nCode[2] = (tCanRxId0) (CANRANGE2ACCCODE(channel).Id0);
    #     endif
    #    else
    canRangeConfig[channel].nMask[2] = (tCanRxId0)0x00000000;
    canRangeConfig[channel].nCode[2] = (tCanRxId0)0xFFFFFFFF;
    #    endif
    
    #    if defined(C_ENABLE_RANGE_3)
    #     if defined(C_SINGLE_RECEIVE_CHANNEL)
    #      if(CANRANGE3IDTYPE(channel) == kCanIdTypeStd)
    canRangeConfig[channel].nMask[3] = (tCanRxId0)~MK_RX_RANGE_MASK_IDSTD0(CANRANGE3ACCMASK(channel));
    canRangeConfig[channel].nCode[3] = (tCanRxId0) MK_RX_RANGE_CODE_IDSTD0(CANRANGE3ACCCODE(channel));
    #      else
    canRangeConfig[channel].nMask[3] = (tCanRxId0)~MK_RX_RANGE_MASK_IDEXT0(CANRANGE3ACCMASK(channel));
    canRangeConfig[channel].nCode[3] = (tCanRxId0) MK_RX_RANGE_CODE_IDEXT0(CANRANGE3ACCCODE(channel));
    #      endif
    #     else
    canRangeConfig[channel].nMask[3] = (tCanRxId0)~(CANRANGE3ACCMASK(channel).Id0);
    canRangeConfig[channel].nCode[3] = (tCanRxId0) (CANRANGE3ACCCODE(channel).Id0);
    #     endif
    #    else
    canRangeConfig[channel].nMask[3] = (tCanRxId0)0x00000000;
    canRangeConfig[channel].nCode[3] = (tCanRxId0)0xFFFFFFFF;
    #    endif
    
    #    if !defined(C_SINGLE_RECEIVE_CHANNEL)
    canRangeConfig[channel].nActiveMask = CanChannelObject[channel].RangeActiveFlag;
    #    endif
    
    #   endif
    #  endif
    # endif
    
    /* Init Tx-Objects -------------------------------------------------------- */
    /* init saved Tx handles: */                   
    /* in case of CommonCAN, transmission is always on the frist HW channel of a CommonCAN channel */
  
    for (hwObjHandle=CAN_HL_HW_TX_STARTINDEX(canHwChannel); hwObjHandle<CAN_HL_HW_TX_STOPINDEX(canHwChannel); hwObjHandle++ )     /*lint !e661*/
    {
      logTxObjHandle = (CanObjectHandle)((vsintx)hwObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));     /*lint !e661*/

#if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION ) || \
    defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
      if((canStatus[channel] & kCanHwIsInit) == kCanHwIsInit)                    /*lint !e661*/
      {
        /* inform application, if a pending transmission is canceled */
        txHandle = canHandleCurTxObj[logTxObjHandle];

# if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
        if( txHandle < kCanNumberOfTxObjects )
        {
          APPLCANCANCELNOTIFICATION(channel, txHandle);
        }
# endif
# if defined( C_ENABLE_CAN_MSG_TRANSMIT_CANCEL_NOTIFICATION )
        if( txHandle == kCanBufferMsgTransmit)
        { 
          APPLCANMSGCANCELNOTIFICATION(channel);
        } 
# endif
      }
#endif

      canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;                   /* MsgObj is free */

#if defined( C_ENABLE_CAN_RAM_CHECK )
      if(kCanTrue == CanLL_IsMailboxCorrupt(CAN_HW_CHANNEL_CANPARA_FIRST hwObjHandle))
      {
# if defined( C_ENABLE_NOTIFY_CORRUPT_MAILBOX )
        ApplCanCorruptMailbox(CAN_CHANNEL_CANPARA_FIRST hwObjHandle);
# endif
        canRamCheckStatus = kCanRamCheckFailed;
      }
#endif

    }


    /* init unused msg objects ------------------------------------------------ */
    for (hwObjHandle=CAN_HL_HW_UNUSED_STARTINDEX(canHwChannel); hwObjHandle<CAN_HL_HW_UNUSED_STOPINDEX(canHwChannel); hwObjHandle++ )  /*lint !e661 !e681*/
    {
    }



#if defined( C_ENABLE_RX_BASICCAN_OBJECTS )
    /* init basic can receive msg object: ------------------------------------- */
    for (hwObjHandle=CAN_HL_HW_RX_BASIC_STARTINDEX(canHwChannel); hwObjHandle<CAN_HL_HW_RX_BASIC_STOPINDEX(canHwChannel); hwObjHandle++ )
    {
#if defined( C_ENABLE_CAN_RAM_CHECK )
      if(kCanTrue == CanLL_IsMailboxCorrupt(CAN_HW_CHANNEL_CANPARA_FIRST hwObjHandle))
      {
# if defined( C_ENABLE_NOTIFY_CORRUPT_MAILBOX )
        ApplCanCorruptMailbox(CAN_CHANNEL_CANPARA_FIRST hwObjHandle);
# endif
        canRamCheckStatus = kCanRamCheckFailed;
      }
#endif

      /* the LL driver has to know which ID tpyes have to be received by which object */
      #  if defined(C_ENABLE_TX_POLLING)
      canllTxStatusFlag[channel] = 0;
      #  endif
      
      tmpCRIER = (vuint8)0x00; /* disable all possible interrupt sources - then enable the used one */
      
      # if defined(C_ENABLE_ERROR_POLLING)
      tmpCRIER |= (vuint8)TSTAT0E;           /* in case of polling the status change interrupt is disabled */
      # else
      tmpCRIER |= (vuint8)(CSCIE | TSTAT0E); /* at least we want BusOff notification */
      #  if defined(C_ENABLE_OVERRUN)
      tmpCRIER |= OVRIE; /* enable overrun interrupt */
      #  endif
      #endif
      
      # if defined (C_ENABLE_INDIVIDUAL_POLLING) || (!defined (C_ENABLE_INDIVIDUAL_POLLING) && !defined(C_ENABLE_RX_BASICCAN_POLLING))
      #  if defined ( C_ENABLE_INDIVIDUAL_POLLING )
      if (CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][hwObjHandle] == (vuint8)0x00)
      #  endif
      {
        tmpCRIER |= RXE; /* enable Rx interrupt */
      }
      # endif
      
      # if defined(C_ENABLE_SLEEP_WAKEUP)
      #  if defined(C_ENABLE_WAKEUP_POLLING)
      #  else
      tmpCRIER |= WUPIE; /* enable wakeup interrupt */
      #  endif
      # endif
      
      if(canCanIrqDisabled[channel] == kCanTrue) /* disable CAN interrupts? */
      {
        canCanInterruptOldStatus[channel].oldCanCRIER = tmpCRIER;
      }
      else
      {
        CRIER = tmpCRIER;
      }
    }
#endif
                     
  } /* end of loop over all hw channels */

#if defined( C_ENABLE_TX_OBSERVE )
  ApplCanInit( CAN_CHANNEL_CANPARA_FIRST CAN_HL_LOG_HW_TX_STARTINDEX(canHwChannel), CAN_HL_LOG_HW_TX_STOPINDEX(canHwChannel) );
#endif
#if defined( C_ENABLE_MSG_TRANSMIT_CONF_FCT )
  APPL_CAN_MSGTRANSMITINIT( CAN_CHANNEL_CANPARA_ONLY );
#endif

#if defined( C_ENABLE_CAN_RAM_CHECK )
  if( canRamCheckStatus == kCanRamCheckFailed)
  {
    /* let the application decide if communication is disabled */
    if (ApplCanMemCheckFailed(CAN_CHANNEL_CANPARA_ONLY) == kCanDisableCommunication)
    {
      canComStatus[channel] = kCanDisableCommunication;
    }  
  }
#endif


} /* END OF CanInit */
/* CODE CATEGORY 4 END*/


/****************************************************************************
| NAME:             CanInitPowerOn
| CALLED BY:        Application
| PRECONDITIONS:    This function must be called by the application before
|                   any other CAN driver function 
|                   Interrupts must be disabled
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Initialization of the CAN chip
****************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanInitPowerOn( void )
{ 

#if defined( C_ENABLE_VARIABLE_DLC )        || \
      defined( C_ENABLE_DYN_TX_OBJECTS )      || \
      defined( C_ENABLE_INDICATION_FLAG )     || \
      defined( C_ENABLE_CONFIRMATION_FLAG )
  CanTransmitHandle txHandle;
#endif
#if defined( C_ENABLE_VARIABLE_RX_DATALEN )
  CanReceiveHandle rxHandle;
#endif
  CAN_CHANNEL_CANTYPE_LOCAL

#if defined( MISRA_CHECK )
  canCanInterruptOldStatus[0] = canCanInterruptOldStatus[0] ;
# if defined( C_HL_ENABLE_LAST_INIT_OBJ )
  lastInitObject[0]= lastInitObject[0];
# endif
#endif

  VStdInitPowerOn();

#if defined( C_ENABLE_VARIABLE_DLC )
  for (txHandle = 0; txHandle < kCanNumberOfTxObjects; txHandle++)
  {
    assertGen(XT_TX_DLC(CanGetTxDlc(txHandle))<(vuint8)9, kCanAllChannels, kErrorTxROMDLCTooLarge);
    canTxDLC_RAM[txHandle] = CanGetTxDlc(txHandle);
  }
#endif

#if defined( C_ENABLE_DYN_TX_OBJECTS )
  /*  Reset dynamic transmission object management -------------------------- */
  for (txHandle = 0; txHandle < kCanNumberOfTxDynObjects; txHandle++)
  {
    /*  Reset management information  */
    canTxDynObjReservedFlag[txHandle] = 0;
  }
#endif /* C_ENABLE_DYN_TX_OBJECTS */

#if defined( C_ENABLE_VARIABLE_RX_DATALEN )
  /*  Initialize the array with received dlc -------------------------- */
  for (rxHandle = 0; rxHandle < kCanNumberOfRxObjects; rxHandle++) {
    canVariableRxDataLen[rxHandle] = CanGetRxDataLen(rxHandle);
  }
#endif

#if defined( C_ENABLE_INDICATION_FLAG )
/* txHandle as loop variable is ok here, because the number of indication bytes is not as high as the number of Rx Messages */
  for (txHandle = 0; txHandle < kCanNumberOfIndBytes; txHandle++) {
    CanIndicationFlags._c[txHandle] = 0;
  }
#endif

#if defined( C_ENABLE_CONFIRMATION_FLAG )
  for (txHandle = 0; txHandle < kCanNumberOfConfBytes; txHandle++) {
    CanConfirmationFlags._c[txHandle] = 0;
  }
#endif

#if defined( C_ENABLE_RX_QUEUE )
# if defined( C_ENABLE_RX_QUEUE_EXTERNAL )
/* in this case, the rx queue pointer no initialized yet. This will be done ofter CanInitPowerOn() via ... by the application */
# else
  CanDeleteRxQueue();
# endif
#endif

  #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  {
    CanChannelHandle ch;
    for(ch = 0; ch < kCanNumberOfChannels; ch++)
    {
      canCanIrqDisabled[ch] = kCanFalse;
  #  if defined(C_ENABLE_RX_QUEUE)
      canRDSRxPtr[ch] = (vuint8*)(canRxInfoStruct[ch].pChipData);
  #  else
  #   if defined(C_SEARCH_XGATE)
      canRDSRxPtr[ch] = (vuint8*)&(canRxTmpBuf[ch].DataFld[0]);
      /* initialize the XGate interrupt counter */
      canCanXGateIrqtCount = 0;
  #   else
      canRDSRxPtr[ch] = (vuint8*)&(CAN_CNTRL_BASIS_ADR(ch) -> CanRxBuf.DataFld[0]);
  #   endif
  #  endif
  # if defined ( C_ENABLE_CAN_TX_CONF_FCT )
      txInfoStructConf[ch].pChipMsgObj = (CanChipDataPtr)(CAN_TX_MAILBOX_BASIS_ADR(ch));
      txInfoStructConf[ch].pChipData   = (CanChipMsgPtr)(CAN_TX_MAILBOX_BASIS_ADR(ch) -> DataFld);
  # endif
    }
  }
  #endif
  
  

#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  for (channel=0; channel<kCanNumberOfChannels; channel++)
#endif
  {

#if defined( C_ENABLE_CAN_RAM_CHECK )
    canComStatus[channel] = kCanEnableCommunication;
#endif
   
    canStatus[channel]              = kCanStatusInit;


#if defined( C_ENABLE_CAN_TX_CONF_FCT )
    txInfoStructConf[channel].Channel = channel;
#endif
    canRxInfoStruct[channel].Channel  = channel;
    canCanInterruptCounter[channel] = 0; 

#if defined( C_ENABLE_TX_POLLING )          || \
      defined( C_ENABLE_RX_BASICCAN_POLLING ) || \
      defined( C_ENABLE_ERROR_POLLING )       || \
      defined( C_ENABLE_WAKEUP_POLLING )      
    canPollingTaskActive[channel] = 0;
#endif

#if defined( C_ENABLE_DYN_TX_OBJECTS )
  /*  Reset dynamic transmission object management -------------------------- */
# if defined( C_ENABLE_CONFIRMATION_FCT ) && \
        defined( C_ENABLE_TRANSMIT_QUEUE )
    confirmHandle[channel] = kCanBufferFree;
# endif
#endif

#if defined( C_ENABLE_TX_MASK_EXT_ID )
    canTxMask0[channel] = 0;
# if (kCanNumberOfUsedCanTxIdTables > 1)
    canTxMask1[channel] = 0;
# endif
# if (kCanNumberOfUsedCanTxIdTables > 2)
    canTxMask2[channel] = 0;
# endif
# if (kCanNumberOfUsedCanTxIdTables > 3)
    canTxMask3[channel] = 0;
# endif
# if (kCanNumberOfUsedCanTxIdTables > 4)
    canTxMask4[channel] = 0;
# endif
#endif

#if defined( C_ENABLE_ECU_SWITCH_PASS )
    canPassive[channel]             = 0;
#endif

#if defined( C_ENABLE_PART_OFFLINE )
    canTxPartOffline[channel]       = kCanTxPartInit;
#endif
#if defined( C_ENABLE_COND_RECEIVE_FCT )
    canMsgCondRecState[channel]     = kCanTrue;
#endif

    {
#if defined( C_ENABLE_TRANSMIT_QUEUE )
  /* clear all Tx queue flags */
      CanDelQueuedObj( CAN_CHANNEL_CANPARA_ONLY );
#endif

      CanInit( CAN_CHANNEL_CANPARA_FIRST 0 );

      /* canStatus is only set to init and online, if CanInit() is called for this channel. */
      canStatus[channel]              |= (kCanHwIsInit | kCanTxOn);
    }

  }

} /* END OF CanInitPowerOn */
/* CODE CATEGORY 4 END*/

#if defined( C_ENABLE_TRANSMIT_QUEUE )
/************************************************************************
* NAME:               CanDelQueuedObj
* CALLED BY:          
* PRECONDITIONS:      
* PARAMETER:          notify: if set to 1 for every deleted obj the appl is notified
* RETURN VALUE:       -
* DESCRIPTION:        Resets the bits with are set to 0 in mask
*                     Clearing the Transmit-queue
*************************************************************************/
/* CODE CATEGORY 4 START*/
static void CanDelQueuedObj( CAN_CHANNEL_CANTYPE_ONLY )
{ 
  CanSignedTxHandle     queueElementIdx;
  #if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
  CanSignedTxHandle     elementBitIdx;
  CanTransmitHandle     txHandle;
  tCanQueueElementType  elem;
  #endif

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertInternal(channel < kCanNumberOfChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);
# endif

  #  if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
  if((canStatus[channel] & kCanHwIsInit) == kCanHwIsInit)
  {
    CanCanInterruptDisable(CAN_CHANNEL_CANPARA_ONLY);        /* avoid interruption by CanHL_TxConfirmation */
    for(queueElementIdx = CAN_HL_TXQUEUE_STARTINDEX(channel); queueElementIdx < CAN_HL_TXQUEUE_STOPINDEX(channel); queueElementIdx++)
    { 
      elem = canTxQueueFlags[queueElementIdx];
      if(elem != (tCanQueueElementType)0) /* is there any flag set in the queue element? */
      {
        /* iterate through all flags and notify application for every scheduled transmission */
        for(elementBitIdx = ((CanSignedTxHandle)1 << kCanTxQueueShift) - (CanSignedTxHandle)1; elementBitIdx >= (CanSignedTxHandle)0; elementBitIdx--)
        { 
          if( ( elem & CanShiftLookUp[elementBitIdx] ) != (tCanQueueElementType)0)
          { 
            txHandle = ((CanTransmitHandle)queueElementIdx << kCanTxQueueShift) - CAN_HL_TXQUEUE_PADBITS(channel) + (CanTransmitHandle)elementBitIdx;
            APPLCANCANCELNOTIFICATION(channel, txHandle);
          } 
        } 
        canTxQueueFlags[queueElementIdx] = (tCanQueueElementType)0;
      }
    } 
    CanCanInterruptRestore(CAN_CHANNEL_CANPARA_ONLY);
  } 
  else
  #  endif
  {
    for(queueElementIdx = CAN_HL_TXQUEUE_STARTINDEX(channel); queueElementIdx < CAN_HL_TXQUEUE_STOPINDEX(channel); queueElementIdx++)
    { 
      canTxQueueFlags[queueElementIdx] = (tCanQueueElementType)0;
    } 
  }

# if defined( C_HL_ENABLE_COPROCESSOR_SUPPORT )
  #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)
  # if defined(VGEN_GENY)
  #  if defined(C_ENABLE_XGATE_USED)
  canXgTxQueueCnt[channel] = 0;
  for (i = CAN_HL_TX_STARTINDEX(channel); i < CAN_HL_TX_STOPINDEX(channel); i++)
  {
    canXgTxQueueFlags[i] = 0;
  }
  #  endif
  # endif
  #endif
# endif
}
/* CODE CATEGORY 4 END*/
#endif

#if defined( C_ENABLE_CAN_TRANSMIT )
/* CODE CATEGORY 3 START*/
/****************************************************************************
| NAME:             CanCancelTransmit
| CALLED BY:        Application
| PRECONDITIONS:    none
| INPUT PARAMETERS: Tx-Msg-Handle
| RETURN VALUES:    none
| DESCRIPTION:      delete on Msg-Object
****************************************************************************/
C_API_1 void C_API_2 CanCancelTransmit( CanTransmitHandle txHandle )
{
  CanDeclareGlobalInterruptOldStatus
  CAN_CHANNEL_CANTYPE_LOCAL 
  CanObjectHandle        logTxObjHandle;
  /* ##RI1.4 - 1.6: CanCancelTransmit and CanCancelMsgTransmit */
#  if defined( C_ENABLE_CANCEL_IN_HW )
  CanObjectHandle        txObjHandle;
#  endif

# if defined( C_ENABLE_TRANSMIT_QUEUE )
  CanSignedTxHandle queueElementIdx; /* index for accessing the tx queue */
  CanSignedTxHandle elementBitIdx;  /* bit index within the tx queue element */
  CanTransmitHandle queueBitPos;  /* physical bitposition of the handle */
# endif

  if (txHandle < kCanNumberOfTxObjects)         /* legal txHandle ? */
  {
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
    channel = CanGetChannelOfTxObj(txHandle);
# endif

# if defined( C_ENABLE_MULTI_ECU_PHYS )
    assertUser(((CanTxIdentityAssignment[txHandle] & V_ACTIVE_IDENTITY_MSK) != (tVIdentityMsk)0 ), channel , kErrorDisabledTxMessage);
# endif

    CanNestedGlobalInterruptDisable();
# if defined( C_ENABLE_TRANSMIT_QUEUE )
    #if defined( C_ENABLE_INTERNAL_CHECK ) &&\
        defined( C_MULTIPLE_RECEIVE_CHANNEL )
    if (sizeof(queueBitPos == 1))
    {
      assertInternal( ((kCanNumberOfTxObjects + CanTxQueuePadBits[kCanNumberOfChannels-1]) <= 256u), kCanAllChannels, kErrorTxQueueTooManyHandle) /*lint !e572 !e506*/
    }
    else
    {
      assertInternal( ((kCanNumberOfTxObjects + CanTxQueuePadBits[kCanNumberOfChannels-1]) <= 65536u), kCanAllChannels, kErrorTxQueueTooManyHandle) /*lint !e572 !e506*/
    }
    #endif
    queueBitPos  = txHandle + CAN_HL_TXQUEUE_PADBITS(channel);
    queueElementIdx = (CanSignedTxHandle)(queueBitPos >> kCanTxQueueShift); /* get the queue element where to set the flag */
    elementBitIdx  = (CanSignedTxHandle)(queueBitPos & kCanTxQueueMask);   /* get the flag index wihtin the queue element */
    if( (canTxQueueFlags[queueElementIdx] & CanShiftLookUp[elementBitIdx]) != (tCanQueueElementType)0 )
    {
      canTxQueueFlags[queueElementIdx] &= ~CanShiftLookUp[elementBitIdx]; /* clear flag from the queue */
      APPLCANCANCELNOTIFICATION(channel, txHandle);
    }
# endif

    logTxObjHandle = (CanObjectHandle)((vsintx)CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));
    if (canHandleCurTxObj[logTxObjHandle] == txHandle)
    {
      canHandleCurTxObj[logTxObjHandle] = kCanBufferCancel;

      /* ##RI1.4 - 1.6: CanCancelTransmit and CanCancelMsgTransmit */
# if defined( C_ENABLE_CANCEL_IN_HW )
      txObjHandle = CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel);
      #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
      CTARQ |= CanMailboxSelect[txObjHandle]; /* request cancellation of the Tx message */
      #endif
      
      /* on successful cancellation the msCAN behaves as if a successful Tx occured */
# endif /* C_ENABLE_CANCEL_IN_HW */
      APPLCANCANCELNOTIFICATION(channel, txHandle);
    }

    CanNestedGlobalInterruptRestore();
  } /* if (txHandle < kCanNumberOfTxObjects) */
}
/* CODE CATEGORY 3 END*/

#endif /* if defined( C_ENABLE_CAN_TRANSMIT ) */


#if defined( C_ENABLE_MSG_TRANSMIT_CONF_FCT )
/****************************************************************************
| NAME:             CanCancelMsgTransmit
| CALLED BY:        Application
| PRECONDITIONS:    none
| INPUT PARAMETERS: none or channel
| RETURN VALUES:    none
| DESCRIPTION:      delete on Msg-Object
****************************************************************************/
/* CODE CATEGORY 3 START*/
C_API_1 void C_API_2 CanCancelMsgTransmit( CAN_CHANNEL_CANTYPE_ONLY )
{
  CanDeclareGlobalInterruptOldStatus
  CanObjectHandle  logTxObjHandle;
# if defined( C_ENABLE_CANCEL_IN_HW )
  CanObjectHandle  txObjHandle;    /* ##RI1.4 - 1.6: CanCancelTransmit and CanCancelMsgTransmit */
# endif

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
# endif


  logTxObjHandle = (CanObjectHandle)((vsintx)CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));

  CanNestedGlobalInterruptDisable();
  if (canHandleCurTxObj[logTxObjHandle] == kCanBufferMsgTransmit)
  {
    canHandleCurTxObj[logTxObjHandle] = kCanBufferCancel;

    /* ##RI1.4 - 1.6: CanCancelTransmit and CanCancelMsgTransmit */
# if defined( C_ENABLE_CANCEL_IN_HW )
    txObjHandle = CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel);
    #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
    CTARQ |= CanMailboxSelect[txObjHandle]; /* request cancellation of the Tx message */
    #endif
    
    /* on successful cancellation the msCAN behaves as if a successful Tx occured */
# endif
    APPLCANMSGCANCELNOTIFICATION(channel);
  }
  CanNestedGlobalInterruptRestore();
}
/* CODE CATEGORY 3 END*/
#endif


#if defined( C_ENABLE_CAN_TRANSMIT )
# if defined( C_ENABLE_VARIABLE_DLC )
/* CODE CATEGORY 2 START*/
/****************************************************************************
| NAME:             CanTransmitVarDLC
| CALLED BY:        Netmanagement, application
| PRECONDITIONS:    Can driver must be initialized
| INPUT PARAMETERS: Handle to Tx message, DLC of Tx message
| RETURN VALUES:    kCanTxFailed: transmit failed
|                   kCanTxOk    : transmit was succesful
| DESCRIPTION:      If the CAN driver is not ready for send, the application
|                   decide, whether the transmit request is repeated or not.
****************************************************************************/
C_API_1 vuint8 C_API_2 CanTransmitVarDLC(CanTransmitHandle txHandle, vuint8 dlc)
{
  assertUser(dlc<(vuint8)9, kCanAllChannels, kErrorTxDlcTooLarge);
  assertUser(txHandle<kCanNumberOfTxObjects, kCanAllChannels , kErrorTxHdlTooLarge);

  canTxDLC_RAM[ txHandle ] = (canTxDLC_RAM[ txHandle ] & CanLL_DlcMask) | MK_TX_DLC(dlc);

  return CanTransmit( txHandle );
} /* END OF CanTransmitVarDLC */
/* CODE CATEGORY 2 END*/
# endif   /* C_ENABLE_VARIABLE_DLC */

/****************************************************************************
| NAME:             CanTransmit
| CALLED BY:        application
| PRECONDITIONS:    Can driver must be initialized
| INPUT PARAMETERS: Handle of the transmit object to be send
| RETURN VALUES:    kCanTxFailed: transmit failed
|                   kCanTxOk    : transmit was succesful
| DESCRIPTION:      If the CAN driver is not ready for send, the application
|                   decide, whether the transmit request is repeated or not.
****************************************************************************/
# if defined( MISRA_CHECK )
  /* suppress misra message about multiple return */
#  pragma PRQA_MESSAGES_OFF 2006
# endif
/* CODE CATEGORY 2 START*/

C_API_1 vuint8 C_API_2 CanTransmit(CanTransmitHandle txHandle)  C_API_3   /*lint !e14 !e31*/
{
  CanDeclareGlobalInterruptOldStatus

# if !defined( C_ENABLE_TX_POLLING )          ||\
     !defined( C_ENABLE_TRANSMIT_QUEUE )      ||\
     defined( C_ENABLE_INDIVIDUAL_POLLING )
  CanObjectHandle      txObjHandle;
  CanObjectHandle      logTxObjHandle;
  vuint8             rc;
# endif   /* ! C_ENABLE_TX_POLLING  || ! C_ENABLE_TRANSMIT_QUEUE || C_ENABLE_TX_FULLCAN_OBJECTS || C_ENABLE_INDIVIDUAL_POLLING */
  CAN_CHANNEL_CANTYPE_LOCAL

# if defined( C_ENABLE_TRANSMIT_QUEUE )
  CanSignedTxHandle queueElementIdx; /* index for accessing the tx queue */
  CanSignedTxHandle elementBitIdx;  /* bit index within the tx queue element */
  CanTransmitHandle queueBitPos;  /* physical bitposition of the handle */
# endif
  #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  # if defined(C_ENABLE_ECU_SWITCH_PASS)
  #  if !defined(C_ENABLE_TX_POLLING) ||       \
        !defined( C_ENABLE_TRANSMIT_QUEUE )      
  #  else  
    CanObjectHandle      txObjHandle;
  #  endif   /* ! C_ENABLE_TX_POLLING  || ! C_ENABLE_TRANSMIT_QUEUE */
  #  if defined ( C_MULTIPLE_RECEIVE_CHANNEL)
    /* unfortunately needed because HL part initializes the channel too late */
    channel = CanGetChannelOfTxObj(txHandle);          /*lint !e661*/
  #  endif
  if(canPassive[channel] != 0)
  {
    /* initialize the tx object handle with 1 */
    txObjHandle = 1;
  }
  # endif
  #endif



  assertUser(txHandle<kCanNumberOfTxObjects, kCanAllChannels , kErrorTxHdlTooLarge);

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  channel = CanGetChannelOfTxObj(txHandle);          /*lint !e661*/
# endif

# if defined( C_ENABLE_MULTI_ECU_PHYS )
  assertUser(((CanTxIdentityAssignment[txHandle] & V_ACTIVE_IDENTITY_MSK) != (tVIdentityMsk)0 ), channel , kErrorDisabledTxMessage);
# endif

  /* test offline ---------------------------------------------------------- */
  if ( (canStatus[channel] & kCanTxOn) != kCanTxOn )           /* transmit path switched off */
  {
    return kCanTxFailed;
  }

# if defined( C_ENABLE_PART_OFFLINE )
  if ( (canTxPartOffline[channel] & CanTxSendMask[txHandle]) != (vuint8)0)   /*lint !e661*/ /* CAN off ? */
  {
    return (kCanTxPartOffline);
  }
# endif

# if defined( C_ENABLE_CAN_RAM_CHECK )
  if(canComStatus[channel] == kCanDisableCommunication)
  {
    return(kCanCommunicationDisabled);
  }
# endif

# if defined( C_ENABLE_SLEEP_WAKEUP )
  assertUser(!CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanSleep);
# endif
  assertUser(!CanLL_HwIsStop(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanStop);

  /* passive mode ---------------------------------------------------------- */
# if defined( C_ENABLE_ECU_SWITCH_PASS )
  if ( canPassive[channel] != (vuint8)0)                             /*  set passive ? */
  {
#  if defined( C_ENABLE_CAN_TX_CONF_FCT ) || \
    defined( C_ENABLE_CONFIRMATION_FCT )
    CanCanInterruptDisable(CAN_CHANNEL_CANPARA_ONLY);      /* avoid CAN Rx interruption */
#  endif

#  if defined( C_ENABLE_CAN_TX_CONF_FCT )
/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
    txInfoStructConf[channel].Handle  = txHandle;
    /* in case of passive mode we do not need to modifiy the mailbox */
    APPL_CAN_TX_CONFIRMATION(&txInfoStructConf[channel]);
#  endif

#  if defined( C_ENABLE_CONFIRMATION_FLAG )       /* set transmit ready flag  */
#   if C_SECURITY_LEVEL > 20
    CanNestedGlobalInterruptDisable();
#   endif
    CanConfirmationFlags._c[CanGetConfirmationOffset(txHandle)] |= CanGetConfirmationMask(txHandle);      /*lint !e661*/
#   if C_SECURITY_LEVEL > 20
    CanNestedGlobalInterruptRestore();
#   endif
#  endif

#  if defined( C_ENABLE_CONFIRMATION_FCT )
    {
      if ( CanGetApplConfirmationPtr(txHandle) != V_NULL )
      {
         (CanGetApplConfirmationPtr(txHandle))(txHandle);   /* call completion routine  */
      }
    }
#  endif /* C_ENABLE_CONFIRMATION_FCT */

#  if defined( C_ENABLE_CAN_TX_CONF_FCT ) || \
    defined( C_ENABLE_CONFIRMATION_FCT )
    CanCanInterruptRestore(CAN_CHANNEL_CANPARA_ONLY);
#  endif

    return kCanTxOk;
  }
# endif


   /* can transmit enabled ================================================== */

   /* ----------------------------------------------------------------------- */
   /* ---  transmit queue with one objects ---------------------------------- */
   /* ---  transmit using fullcan objects ----------------------------------- */
   /* ----------------------------------------------------------------------- */

#  if !defined( C_ENABLE_TX_POLLING )          ||\
      !defined( C_ENABLE_TRANSMIT_QUEUE )      ||\
      defined( C_ENABLE_INDIVIDUAL_POLLING )

  txObjHandle = CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel);                                          /* msg in object 0 */

  logTxObjHandle = (CanObjectHandle)((vsintx)txObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));
#  endif   /* ! C_ENABLE_TX_POLLING  || ! C_ENABLE_TRANSMIT_QUEUE || C_ENABLE_TX_FULLCAN_OBJECTS || C_ENABLE_INDIVIDUAL_POLLING */

  CanNestedGlobalInterruptDisable();
 
  /* test offline after interrupt disable ---------------------------------- */
  if ( (canStatus[channel] & kCanTxOn) != kCanTxOn )                /* transmit path switched off */
  {
    CanNestedGlobalInterruptRestore();
    return kCanTxFailed;
  }

# if defined( C_ENABLE_TRANSMIT_QUEUE )
#  if !defined( C_ENABLE_TX_POLLING )         ||\
      defined( C_ENABLE_INDIVIDUAL_POLLING )
  if (

#   if defined( C_ENABLE_TX_POLLING )
#    if defined( C_ENABLE_INDIVIDUAL_POLLING )
       ( ( CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][txObjHandle] != (vuint8)0 )       /* Object is used in polling mode! */
                         || (canHandleCurTxObj[logTxObjHandle] != kCanBufferFree) )    /* MsgObj used?  */
#    else
        /* write always to queue; transmission is started out of TxTask */
#    endif
#   else
       ( canHandleCurTxObj[logTxObjHandle] != kCanBufferFree)    /* MsgObj used?  */
#   endif
     )
#  endif   /*  ( C_ENABLE_TX_FULLCAN_OBJECTS )  || !( C_ENABLE_TX_POLLING ) || ( C_ENABLE_INDIVIDUAL_POLLING ) */

    {
      /* tx object 0 used -> set msg in queue: -----------------------------*/
      queueBitPos  = txHandle + CAN_HL_TXQUEUE_PADBITS(channel);
      queueElementIdx = (CanSignedTxHandle)(queueBitPos >> kCanTxQueueShift); /* get the queue element where to set the flag */
      elementBitIdx  = (CanSignedTxHandle)(queueBitPos & kCanTxQueueMask);   /* get the flag index wihtin the queue element */
      canTxQueueFlags[queueElementIdx] |= CanShiftLookUp[elementBitIdx];
      CanNestedGlobalInterruptRestore();
      return kCanTxOk;                          
  }
# endif  /* C_ENABLE_TRANSMIT_QUEUE */

# if !defined( C_ENABLE_TX_POLLING )          ||\
     !defined( C_ENABLE_TRANSMIT_QUEUE )      ||\
     defined( C_ENABLE_INDIVIDUAL_POLLING )

#  if defined( C_ENABLE_TRANSMIT_QUEUE )    && \
      ( !defined( C_ENABLE_TX_POLLING )         ||\
        defined( C_ENABLE_INDIVIDUAL_POLLING )  )
  else
#  endif
  {
  /* check for transmit message object free ---------------------------------*/
    if (( canHandleCurTxObj[logTxObjHandle] != kCanBufferFree)    /* MsgObj used?  */
       || ( !CanLL_TxIsHWObjFree( canHwChannel, txObjHandle ) )

      /* hareware-txObject is not free --------------------------------------*/
       )  /* end of if question */

    {  /* object used */
      /* tx object n used, quit with error */
      CanNestedGlobalInterruptRestore();
      return kCanTxFailed;
    }
  }

  /* Obj, pMsgObject points to is free, transmit msg object: ----------------*/
  canHandleCurTxObj[logTxObjHandle] = txHandle;/* Save hdl of msgObj to be transmitted*/
  CanNestedGlobalInterruptRestore();

  rc = CanCopyDataAndStartTransmission( CAN_CHANNEL_CANPARA_FIRST txObjHandle, txHandle);

#  if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
  if ( rc == kCanTxNotify)
  {
    rc = kCanTxFailed;      /* ignore notification if calls of CanCopy.. is performed within CanTransmit */
  }
#  endif


  return(rc);

# else   /* ! C_ENABLE_TX_POLLING  || ! C_ENABLE_TRANSMIT_QUEUE || C_ENABLE_TX_FULLCAN_OBJECTS || C_ENABLE_INDIVIDUAL_POLLING */
# endif   /* ! C_ENABLE_TX_POLLING  || ! C_ENABLE_TRANSMIT_QUEUE || C_ENABLE_TX_FULLCAN_OBJECTS || C_ENABLE_INDIVIDUAL_POLLING */

} /* END OF CanTransmit */
/* CODE CATEGORY 2 END*/
# if defined( MISRA_CHECK )
#  pragma PRQA_MESSAGES_ON 2006
# endif



/****************************************************************************
| NAME:             CanCopyDataAndStartTransmission
| CALLED BY:        CanTransmit, CanHl_RestartTxQueue and CanHL_TxConfirmation
| PRECONDITIONS:    - Can driver must be initialized
|                   - canTxCurHandle[logTxObjHandle] must be set
|                   - the hardwareObject (txObjHandle) must be free
| INPUT PARAMETERS: txHandle: Handle of the transmit object to be send
|                   txObjHandle:  Nr of the HardwareObjects to use
| RETURN VALUES:    kCanTxFailed: transmit failed
|                   kCanTxOk    : transmit was succesful
| DESCRIPTION:      If the CAN driver is not ready for send, the application
|                   decide, whether the transmit request is repeated or not.
****************************************************************************/
# if defined( MISRA_CHECK )
  /* suppress misra message about goto */
#  pragma PRQA_MESSAGES_OFF 2001,2015
# endif
/* CODE CATEGORY 1 START*/
static vuint8 CanCopyDataAndStartTransmission( CAN_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle, CanTransmitHandle txHandle) C_API_3   /*lint !e14 !e31*/
{
   CanDeclareGlobalInterruptOldStatus
   vuint8             rc;
   CanObjectHandle      logTxObjHandle;
#  if defined( C_ENABLE_COPY_TX_DATA )  
   TxDataPtr   CanMemCopySrcPtr;
#  endif
# if defined( C_ENABLE_DYN_TX_OBJECTS )
   CanTransmitHandle    dynTxObj;
# endif /* C_ENABLE_DYN_TX_OBJECTS */
# if defined( C_ENABLE_PRETRANSMIT_FCT )
   CanTxInfoStruct      txStruct;
# endif

   #if defined(C_ENABLE_COPY_TX_DATA)
   vsintx i;
   vuint8* pDestData;
   vuint8* pSrcData;
   #endif

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
   assertInternal(channel < kCanNumberOfChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);
# endif
   assertInternal(txHandle < kCanNumberOfTxObjects, kCanAllChannels , kErrorInternalTxHdlTooLarge);

# if defined( C_ENABLE_DYN_TX_OBJECTS )
   if (txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel))
   {
     dynTxObj = txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);
   }
   else
   {
     dynTxObj = (CanTransmitHandle)0;
   }
# endif /* C_ENABLE_DYN_TX_OBJECTS */

   assertInternal(txObjHandle < CAN_HL_HW_TX_STOPINDEX(canHwChannel), channel, kErrorTxObjHandleWrong);
   logTxObjHandle = (CanObjectHandle)((vsintx)txObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));

   assertHardware( CanLL_TxIsHWObjFree( canHwChannel, txObjHandle ), channel, kErrorTxBufferBusy);

   CanBeginCriticalXGateSection();
   #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
   CTBSEL = CanMailboxSelect[txObjHandle]; /* select the appropriate Tx buffer */
   #endif
   
   CanEndCriticalXGateSection();

   /* set id and dlc  -------------------------------------------------------- */
   {
# if defined( C_ENABLE_DYN_TX_DLC ) || \
      defined( C_ENABLE_DYN_TX_ID )
     if (txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel))
     {           /* set dynamic part of dynamic objects ----------------------*/
#  if defined( C_ENABLE_DYN_TX_ID )
        #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
        CAN_TX_MAILBOX_BASIS_ADR(channel) -> Id   = canDynTxId0[dynTxObj];
        CAN_TX_MAILBOX_BASIS_ADR(channel) -> Prio = TX1; /* Prio Std Tx > Prio LowLevel Tx */
        #endif
        
        
#  endif
 
#  if defined( C_ENABLE_DYN_TX_DLC )
        #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
        CAN_TX_MAILBOX_BASIS_ADR(channel) -> Dlc = (canDynTxDLC[dynTxObj]);
        #endif
        
#  endif
     }
     else
     {          /* set part of static objects assocciated the dynamic --------*/
#  if defined( C_ENABLE_DYN_TX_ID )
#   if defined( C_ENABLE_TX_MASK_EXT_ID )
#    if defined( C_ENABLE_MIXED_ID )
        if (CanGetTxIdType(txHandle)==kCanIdTypeStd)
        {
          #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
          CAN_TX_MAILBOX_BASIS_ADR(channel) -> Id   = CanGetTxId0(txHandle);
          CAN_TX_MAILBOX_BASIS_ADR(channel) -> Prio = TX1; /* Prio Std Tx > Prio LowLevel Tx */
          #endif
          
          
        }
        else
#    endif
        {
          /* mask extened ID */
          #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
          CAN_TX_MAILBOX_BASIS_ADR(channel) -> Id   = (CanGetTxId0(txHandle)|canTxMask0[channel]);
          CAN_TX_MAILBOX_BASIS_ADR(channel) -> Prio = TX1; /* Prio Std Tx > Prio LowLevel Tx */
          #endif
          
          
        }
#   else
        #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
        CAN_TX_MAILBOX_BASIS_ADR(channel) -> Id   = CanGetTxId0(txHandle);
        CAN_TX_MAILBOX_BASIS_ADR(channel) -> Prio = TX1; /* Prio Std Tx > Prio LowLevel Tx */
        #endif
        
        
#   endif
#  endif

#  if defined( C_ENABLE_DYN_TX_DLC )
#   if defined( C_ENABLE_VARIABLE_DLC )
        /* init DLC, RAM */
        #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
        CAN_TX_MAILBOX_BASIS_ADR(channel) -> Dlc = (canTxDLC_RAM[txHandle]);
        #endif
        
#   else
        /* init DLC, ROM */
        #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
        CAN_TX_MAILBOX_BASIS_ADR(channel) -> Dlc = CanGetTxDlc(txHandle);
        #endif
        
#   endif
#  endif
     }
# endif
     /* set static part commen for static and dynamic objects ----------------*/
# if defined( C_ENABLE_DYN_TX_ID )
# else
#  if defined( C_ENABLE_TX_MASK_EXT_ID )
#   if defined( C_ENABLE_MIXED_ID )
     if (CanGetTxIdType(txHandle)==kCanIdTypeStd)
     {
       #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
       CAN_TX_MAILBOX_BASIS_ADR(channel) -> Id   = CanGetTxId0(txHandle);
       CAN_TX_MAILBOX_BASIS_ADR(channel) -> Prio = TX1; /* Prio Std Tx > Prio LowLevel Tx */
       #endif
       
       
     }
     else
#   endif
     {
       /* mask extened ID */
       #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
       CAN_TX_MAILBOX_BASIS_ADR(channel) -> Id   = (CanGetTxId0(txHandle)|canTxMask0[channel]);
       CAN_TX_MAILBOX_BASIS_ADR(channel) -> Prio = TX1; /* Prio Std Tx > Prio LowLevel Tx */
       #endif
       
       
     }
#  else
     #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)
		/*if (CanGetTxId0[txHandle] == MK_STDID0(0x6F0u)) 
        {                   
			CanGetTxId0[txHandle] = MK_STDID0(fbl_meta_info[1].crc32&0x7FF);        
        }                   
		else if (CanGetTxId0[txHandle] == MK_STDID0(0x203u)) 
		{
			CanGetTxId0[txHandle] = MK_STDID0(fbl_meta_info[1].crc_start&0x7FF);       
		}
		else if (CanGetTxId0[txHandle] == MK_STDID0(0x213u)) 
		{
			CanGetTxId0[txHandle] = MK_STDID0(fbl_meta_info[1].crc_length&0x7FF);      
		}*/
     CAN_TX_MAILBOX_BASIS_ADR(channel) -> Id   = CanGetTxId0(txHandle);
     CAN_TX_MAILBOX_BASIS_ADR(channel) -> Prio = TX1; /* Prio Std Tx > Prio LowLevel Tx */
     #endif
     
     
#  endif
# endif
# if defined( C_ENABLE_DYN_TX_DLC )
# else
#  if defined( C_ENABLE_VARIABLE_DLC )
     /* init DLC, RAM */
     #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
     CAN_TX_MAILBOX_BASIS_ADR(channel) -> Dlc = (canTxDLC_RAM[txHandle]);
     #endif
     
#  else
     /* init DLC, ROM */
     #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
     CAN_TX_MAILBOX_BASIS_ADR(channel) -> Dlc = CanGetTxDlc(txHandle);
     #endif
     
#  endif
# endif

# if defined( C_ENABLE_MIXED_ID )
#  if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#  else
#   if defined( C_ENABLE_DYN_TX_DLC ) || \
       defined( C_ENABLE_DYN_TX_ID )
     if (txHandle >=  CAN_HL_TX_DYN_ROM_STARTINDEX(channel))
     {                      /* set dynamic part of dynamic objects */
#    if defined( C_ENABLE_DYN_TX_ID )
#    else
#    endif
     }
#   else
#   endif
#  endif
# endif

   }


 /* call pretransmit function ----------------------------------------------- */
# if defined( C_ENABLE_PRETRANSMIT_FCT )

   /* pointer needed for other modules */
   #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
   CanBeginCriticalXGateSection();
   CTBSEL                      = CanMailboxSelect[txObjHandle]; /* select the appropriate Tx buffer */
   (txStruct.pChipData)                = (CanChipDataPtr)&(CAN_TX_MAILBOX_BASIS_ADR(channel) -> DataFld[0]);
   canRDSTxPtr[logTxObjHandle] = (txStruct.pChipData);
   CanEndCriticalXGateSection();
   #endif
   txStruct.Handle      = txHandle;

   {  
    /* Is there a PreTransmit function ? ------------------------------------- */ 
    if ( CanGetApplPreTransmitPtr(txHandle) != V_NULL )    /* if PreTransmit exists */ 
    { 
      if ( (CanGetApplPreTransmitPtr(txHandle)) (txStruct) == kCanNoCopyData)  
      { 
      
        
        /* Do not copy the data - already done by the PreTransmit-function */  
        /* --- start transmission --- */  
        goto startTransmission; 
      } 
    } 
   }  
# endif /* C_ENABLE_PRETRANSMIT_FCT */

 /* copy data --------------------------------------------------------------- */
# if defined( C_ENABLE_COPY_TX_DATA )  
#  if defined( C_ENABLE_DYN_TX_DATAPTR )  
   if (txHandle >=  CAN_HL_TX_DYN_ROM_STARTINDEX(channel))  
   {  
      CanMemCopySrcPtr = canDynTxDataPtr[dynTxObj];  
   }  
   else  
#  endif  
   {  
     CanMemCopySrcPtr = CanGetTxDataPtr(txHandle);  
   }  
 /* copy via index in MsgObj data field, copy always 8 bytes -----------*/  
   if ( CanMemCopySrcPtr != V_NULL )   /* copy if buffer exists */
   {
#  if C_SECURITY_LEVEL > 10
     CanNestedGlobalInterruptDisable();  
#  endif

     #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
     pDestData = (vuint8*)&(CAN_TX_MAILBOX_BASIS_ADR(channel) -> DataFld[0]);
     pSrcData  = (vuint8*)CanMemCopySrcPtr;
     for(i = 7; i >= 0; i--)
     {
       pDestData[i] = pSrcData[i];
     }
     #endif

#  if C_SECURITY_LEVEL > 10
     CanNestedGlobalInterruptRestore();  
#  endif
   }
# endif /* ( C_ENABLE_COPY_TX_DATA ) */  

   CANDRV_SET_CODE_TEST_POINT(0x10A);

# if defined( C_ENABLE_PRETRANSMIT_FCT )
/* Msg(4:2015) This label is not a case or default label for a switch statement. MISRA Rule 55 */
startTransmission:
# endif

   /* test offline and handle and start transmission ------------------------ */
   CanNestedGlobalInterruptDisable();  
   /* If CanTransmit was interrupted by a re-initialization or CanOffline */  
   /* no transmitrequest of this action should be started      */  
   if ((canHandleCurTxObj[logTxObjHandle] == txHandle) && ((canStatus[channel] & kCanTxOn) == kCanTxOn))
   {  
     #if defined(C_ENABLE_TX_POLLING)
     # if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
     canllTxStatusFlag[channel] |= TX1;
     # endif
     #endif
     #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
     CanBeginCriticalXGateSection();
     CANTFLG = CanMailboxSelect[txObjHandle]; /* transmit message    */
     CTBSEL = TX2; /* ESCAN00022867, the assignment is relevant if normal transmission interrupts low-level transmission */
     #if defined (C_ENABLE_INDIVIDUAL_POLLING) || (!defined (C_ENABLE_INDIVIDUAL_POLLING) && !defined(C_ENABLE_TX_POLLING))
     # if defined ( C_ENABLE_INDIVIDUAL_POLLING )
     if (CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][txObjHandle] == (vuint8)0x00)
     # endif
     {
       /* in case of disabled Tx Irq the request needs to be stored in the old status variable */
       if(canCanIrqDisabled[channel] == kCanTrue)
       {
         canCanInterruptOldStatus[channel].oldCanCTIER |= CanMailboxSelect[txObjHandle]; /* enable Tx interrupt */
       }
       else
       {
         CTIER |= CanMailboxSelect[txObjHandle]; /* enable Tx interrupt */
       }
     }
     # endif
     CanEndCriticalXGateSection();
     #endif
     

# if defined( C_ENABLE_TX_OBSERVE )
     ApplCanTxObjStart( CAN_CHANNEL_CANPARA_FIRST logTxObjHandle );
# endif
     rc = kCanTxOk;                                    
   }  
   else  
   {  
# if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
     if (canHandleCurTxObj[logTxObjHandle] == txHandle)
     {
       /* only CanOffline was called on higher level */
       rc = kCanTxNotify;
     }
     else
# endif
     {
       rc = kCanTxFailed;   
     }
     assertInternal((canHandleCurTxObj[logTxObjHandle] == txHandle) || (canHandleCurTxObj[logTxObjHandle] == kCanBufferFree), channel, kErrorTxHandleWrong);
     canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;  /* release TxHandle (CanOffline) */
   }  

   CanNestedGlobalInterruptRestore();

   
   return (rc);   

} /* END OF CanCopyDataAndStartTransmission */
/* CODE CATEGORY 1 END*/
# if defined( MISRA_CHECK )
#  pragma PRQA_MESSAGES_ON 2001,2015
# endif
#endif /* if defined( C_ENABLE_CAN_TRANSMIT ) */


#if defined( C_ENABLE_TX_POLLING ) || \
    defined( C_ENABLE_RX_BASICCAN_POLLING ) || \
    defined( C_ENABLE_ERROR_POLLING ) || \
    defined( C_ENABLE_WAKEUP_POLLING ) 
/****************************************************************************
| NAME:             CanTask
| CALLED BY:        application
| PRECONDITIONS:
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      - cyclic Task, 
|                   - polling error bus off
|                   - polling Tx objects
|                   - polling Rx objects
****************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanTask(void)
{
  CAN_CHANNEL_CANTYPE_LOCAL

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  for (channel = 0; channel < kCanNumberOfChannels; channel++)
# endif
  {
    {
# if defined( C_ENABLE_ERROR_POLLING )
      CanErrorTask(CAN_CHANNEL_CANPARA_ONLY);
# endif

# if defined( C_ENABLE_SLEEP_WAKEUP )
#  if defined( C_ENABLE_WAKEUP_POLLING )
      CanWakeUpTask(CAN_CHANNEL_CANPARA_ONLY);
#  endif
# endif

# if defined( C_ENABLE_TX_POLLING ) 
      CanTxTask(CAN_CHANNEL_CANPARA_ONLY);
# endif


# if defined( C_ENABLE_RX_BASICCAN_OBJECTS ) && \
    defined( C_ENABLE_RX_BASICCAN_POLLING )
      CanRxBasicCANTask(CAN_CHANNEL_CANPARA_ONLY);
# endif
    }
  }
}
/* CODE CATEGORY 2 END*/
#endif

#if defined( C_ENABLE_ERROR_POLLING )
/****************************************************************************
| NAME:             CanErrorTask
| CALLED BY:        application
| PRECONDITIONS:
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      - cyclic Task, 
|                   - polling error status
****************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanErrorTask( CAN_CHANNEL_CANTYPE_ONLY )
{

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
# endif
  assertUser((canPollingTaskActive[channel] == (vuint8)0), channel, kErrorPollingTaskRecursion);

  if (canPollingTaskActive[channel] == (vuint8)0)  /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;

    {
# if defined( C_ENABLE_SLEEP_WAKEUP )
      if ( !CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY))
# endif
      {
        CAN_POLLING_IRQ_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
        CanHL_ErrorHandling(CAN_HW_CHANNEL_CANPARA_ONLY);
        CAN_POLLING_IRQ_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
      }
    }

    canPollingTaskActive[channel] = 0;
  }
}
/* CODE CATEGORY 2 END*/
#endif

#if defined( C_ENABLE_SLEEP_WAKEUP )
# if defined( C_ENABLE_WAKEUP_POLLING )
/****************************************************************************
| NAME:             CanWakeUpTask
| CALLED BY:        application
| PRECONDITIONS:
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      - cyclic Task, 
|                   - polling CAN wakeup event
****************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanWakeUpTask(CAN_CHANNEL_CANTYPE_ONLY)
{
  CanDeclareGlobalInterruptOldStatus

#  if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
#  endif
  assertUser((canPollingTaskActive[channel] == (vuint8)0), channel, kErrorPollingTaskRecursion);

  if (canPollingTaskActive[channel] == (vuint8)0)  /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;

    if((CRFLG & WUPIF) == WUPIF) /* check if a wakeup occured */
    {
      CANDRV_SET_CODE_TEST_POINT(0x111);
      CanNestedGlobalInterruptDisable();          /* ESCAN00021223 */
      CanLL_WakeUpHandling(CAN_CHANNEL_CANPARA_ONLY);
      CanNestedGlobalInterruptRestore();          /* ESCAN00021223 */
    }
    canPollingTaskActive[channel] = 0;
  }
}
/* CODE CATEGORY 2 END*/
# endif /* C_ENABLE_WAKEUP_POLLING */
#endif /* C_ENABLE_SLEEP_WAKEUP */

#if defined( C_ENABLE_TX_POLLING ) 
/****************************************************************************
| NAME:             CanTxTask
| CALLED BY:        application
| PRECONDITIONS:
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      - cyclic Task, 
|                   - polling Tx objects
****************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanTxTask( CAN_CHANNEL_CANTYPE_ONLY )
{
  CanObjectHandle      txObjHandle;

  vuint8 tmpCondition;
  CanDeclareGlobalInterruptOldStatus


# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
# endif
  assertUser((canPollingTaskActive[channel] == (vuint8)0), channel, kErrorPollingTaskRecursion);

  if (canPollingTaskActive[channel] == (vuint8)0)  /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;


# if defined( C_ENABLE_SLEEP_WAKEUP )
    if ( !CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY))
# endif
    {
      /*--  polling Tx objects ----------------------------------------*/

# if defined( C_ENABLE_TX_POLLING )
      /* check for global confirmation Pending and may be reset global pending confirmation */
      {
        for ( txObjHandle = CAN_HL_HW_TX_STARTINDEX(canHwChannel); txObjHandle < CAN_HL_HW_TX_STOPINDEX(canHwChannel) ; txObjHandle++ )
        {
#  if defined( C_ENABLE_INDIVIDUAL_POLLING )
          if ( CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][txObjHandle] != (vuint8)0 )
#  endif
          {
            /* check for dedicated confirmation pending */
            # if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
            CanNestedGlobalInterruptDisable();
            if(((CANTFLG & CanMailboxSelect[txObjHandle]) != 0) && ((canllTxStatusFlag[channel] & CanMailboxSelect[txObjHandle]) != 0))
            {
              tmpCondition = 1;
            }
            else
            {
              tmpCondition = 0;
            }
            CanNestedGlobalInterruptRestore();
            if(tmpCondition != 0)
            # endif
            {
              CANDRV_SET_CODE_TEST_POINT(0x110);
              CAN_POLLING_IRQ_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
              /* do tx confirmation */
              CanHL_TxConfirmation(CAN_HW_CHANNEL_CANPARA_FIRST txObjHandle );
              CAN_POLLING_IRQ_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
            }
          } /* if individual polling & object has to be polled */
        }
      }
# endif /*( C_ENABLE_TX_POLLING ) */


# if defined( C_ENABLE_TRANSMIT_QUEUE )
      CanHl_RestartTxQueue( CAN_CHANNEL_CANPARA_ONLY );
# endif /*  C_ENABLE_TRANSMIT_QUEUE */

    } /* if ( CanLL_HwIsSleep... ) */

    canPollingTaskActive[channel] = 0;
  }


} /* END OF CanTxTask */
/* CODE CATEGORY 2 END*/
#endif /* C_ENABLE_TX_POLLING */

#if defined( C_ENABLE_TRANSMIT_QUEUE )
# if defined( C_ENABLE_TX_POLLING ) 
/****************************************************************************
| NAME:             CanHl_RestartTxQueue
| CALLED BY:        CanTxTask, via CanLL (TxMsgDestroyed)
| PRECONDITIONS:
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      start transmission if queue entry exists and HW is free
****************************************************************************/
/* CODE CATEGORY 2 START*/
static void CanHl_RestartTxQueue( CAN_CHANNEL_CANTYPE_ONLY )
{
  CanTransmitHandle    txHandle;
#  if  defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
  vuint8             rc;
#  endif
  CanDeclareGlobalInterruptOldStatus

  CanSignedTxHandle         queueElementIdx;    /* use as signed due to loop */
  CanSignedTxHandle         elementBitIdx;
  tCanQueueElementType             elem;

#  if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertInternal(channel < kCanNumberOfChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);
#  endif


  if (canHandleCurTxObj[(vsintx)CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)] == kCanBufferFree)
  {
    for(queueElementIdx = CAN_HL_TXQUEUE_STOPINDEX(channel) - (CanSignedTxHandle)1; 
                             queueElementIdx >= CAN_HL_TXQUEUE_STARTINDEX(channel); queueElementIdx--)
    {
      elem = canTxQueueFlags[queueElementIdx];
      if(elem != (tCanQueueElementType)0) /* is there any flag set in the queue element? */
      {

        /* Transmit Queued Objects */
        /* start the bitsearch */
        #if defined( C_CPUTYPE_32BIT )
        if((elem & (tCanQueueElementType)0xFFFF0000) != (tCanQueueElementType)0)
        {
          if((elem & (tCanQueueElementType)0xFF000000) != (tCanQueueElementType)0)
          {
            if((elem & (tCanQueueElementType)0xF0000000) != (tCanQueueElementType)0)
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 28] + 28;
            }
            else /*0x0F000000*/
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 24] + 24;
            }
          }
          else
          {
            if((elem & (tCanQueueElementType)0x00F00000) != (tCanQueueElementType)0)
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 20] + 20;
            }
            else /* 0x000F0000*/
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 16] + 16;
            }
          }
        }
        else
        #endif
        {
        #if defined( C_CPUTYPE_32BIT ) || defined( C_CPUTYPE_16BIT )
          if((elem & (tCanQueueElementType)0x0000FF00) != (tCanQueueElementType)0)
          {
            if((elem & (tCanQueueElementType)0x0000F000) != (tCanQueueElementType)0)
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 12] + 12;
            }
            else /*0x00000F00*/
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 8] + 8;
            }
          }
          else
        #endif
          {
            if((elem & (tCanQueueElementType)0x000000F0) != (tCanQueueElementType)0)
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 4] + 4;
            }
            else /* 0x0000000F*/
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem];
            }
          }
        }
        txHandle = ((CanTransmitHandle)queueElementIdx << kCanTxQueueShift) - CAN_HL_TXQUEUE_PADBITS(channel) + (CanTransmitHandle)elementBitIdx;
        {

            CanNestedGlobalInterruptDisable();
            if (canHandleCurTxObj[(vsintx)CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)] == kCanBufferFree)
            {
              if ( (canTxQueueFlags[queueElementIdx] & CanShiftLookUp[elementBitIdx]) != (tCanQueueElementType)0 )
              {
                canTxQueueFlags[queueElementIdx] &= ~CanShiftLookUp[elementBitIdx]; /* clear flag from the queue */
                /* Save hdl of msgObj to be transmitted*/
                canHandleCurTxObj[(vsintx)CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)] = txHandle;
                CanNestedGlobalInterruptRestore();  
#  if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
                rc = CanCopyDataAndStartTransmission(CAN_CHANNEL_CANPARA_FIRST CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel), txHandle);
                if ( rc == kCanTxNotify)
                {
                  APPLCANCANCELNOTIFICATION(channel, txHandle);
                }
#  else
                (void)CanCopyDataAndStartTransmission(CAN_CHANNEL_CANPARA_FIRST CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel), txHandle);
#  endif
                return;
              }

            }
            CanNestedGlobalInterruptRestore();
            return;
        }
      }
    }
  }
} /* End of CanHl_RestartTxQueue */
/* CODE CATEGORY 2 END*/
# endif
#endif /*  C_ENABLE_TRANSMIT_QUEUE */


#if defined( C_ENABLE_RX_BASICCAN_POLLING ) && \
    defined( C_ENABLE_RX_BASICCAN_OBJECTS )
/****************************************************************************
| NAME:             CanRxBasicCANTask
| CALLED BY:        application
| PRECONDITIONS:
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      - cyclic Task, 
|                   - polling Rx BasicCAN objects
****************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanRxBasicCANTask(CAN_CHANNEL_CANTYPE_ONLY)
{

  CanObjectHandle     rxObjHandle;          /* keyword register removed 2005-06-29 Ht */


# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
# endif
  assertUser((canPollingTaskActive[channel] == (vuint8)0), channel, kErrorPollingTaskRecursion);

  if (canPollingTaskActive[channel] == (vuint8)0)  /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;


# if defined( C_ENABLE_SLEEP_WAKEUP )
    if ( !CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY))
# endif
    {

      {
        for (rxObjHandle=CAN_HL_HW_RX_BASIC_STARTINDEX(canHwChannel); rxObjHandle<CAN_HL_HW_RX_BASIC_STOPINDEX(canHwChannel); rxObjHandle++ )
        {
# if defined( C_ENABLE_INDIVIDUAL_POLLING )
          if ( CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][rxObjHandle] != (vuint8)0 )
# endif
          {
            /* check for dedicated indication pending */
            if(RXF == (CRFLG & RXF)) /* something received? */
            {
              CANDRV_SET_CODE_TEST_POINT(0x108);

              CAN_POLLING_IRQ_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
              CanBasicCanMsgReceived( CAN_HW_CHANNEL_CANPARA_FIRST rxObjHandle);
              CAN_POLLING_IRQ_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
            }
          } /* if individual polling & object has to be polled */
        }
      }
    } /* if ( CanLL_HwIsSleep... )  */

    canPollingTaskActive[channel] = 0;
  }

} /* END OF CanRxTask */
/* CODE CATEGORY 2 END*/
#endif /* C_ENABLE_RX_BASICCAN_POLLING && C_ENABLE_RX_BASICCAN_OBJECTS */

/****************************************************************************
| NAME:             CanHL_ErrorHandling
| CALLED BY:        CanISR(), CanErrorTask()
| PRECONDITIONS:
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      - error interrupt (busoff, error warning,...)
****************************************************************************/
/* CODE CATEGORY 2 START*/
static void CanHL_ErrorHandling( CAN_HW_CHANNEL_CANTYPE_ONLY )
{

#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertInternal(canHwChannel < kCanNumberOfHwChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);
#endif




  /* check for status register (bus error)--*/
  if( ((CRFLG & BOFFIF) == BOFFIF) && ((CRFLG & CSCIF) == CSCIF) )
  {
    /*==BUS OFF ERROR=========================*/
    APPL_CAN_BUSOFF( CAN_CHANNEL_CANPARA_ONLY );            /* call application specific function */
  }

#if defined( C_HL_ENABLE_OVERRUN_IN_STATUS )
  /* check for status register (overrun occured)--*/
# if defined( C_ENABLE_OVERRUN )
  if(OVRIF == (CRFLG & OVRIF))
  {
    ApplCanOverrun( CAN_CHANNEL_CANPARA_ONLY );
  }
# endif
#endif
  CRFLG = (OVRIF | CSCIF); /* clear overrunflag and status changed */

} /* END OF CanHL_ErrorHandling */
/* CODE CATEGORY 2 END*/


#if defined( C_ENABLE_INDIVIDUAL_POLLING )
# if defined( C_ENABLE_TX_POLLING )
/****************************************************************************
| NAME:             CanTxObjTask()
| CALLED BY:        
| PRECONDITIONS:    
| INPUT PARAMETERS: CAN_CHANNEL_CANTYPE_FIRST 
|                   CanObjectHandle txObjHandle
| RETURN VALUES:    none
| DESCRIPTION:      Polling special Object
****************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanTxObjTask(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle)
{

  vuint8 tmpCondition;
  CanDeclareGlobalInterruptOldStatus


#  if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(canHwChannel < kCanNumberOfHwChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
#  endif
  assertUser(txObjHandle < CAN_HL_HW_TX_STOPINDEX(canHwChannel), channel, kErrorHwHdlTooLarge);
  assertUser((CanSignedTxHandle)txObjHandle >= (CanSignedTxHandle)CAN_HL_HW_TX_STARTINDEX(canHwChannel), channel, kErrorHwHdlTooSmall);    /*lint !e568 */
  assertUser(CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][txObjHandle] != (vuint8)0, channel, kErrorHwObjNotInPolling);
  assertUser((canPollingTaskActive[channel] == (vuint8)0), channel, kErrorPollingTaskRecursion);

  if (canPollingTaskActive[channel] == (vuint8)0)  /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;

#  if defined( C_ENABLE_SLEEP_WAKEUP )
    if ( !CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY))
#  endif
    {

      /* check for dedicated confirmation pending */
      # if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
      CanNestedGlobalInterruptDisable();
      if(((CANTFLG & CanMailboxSelect[txObjHandle]) != 0) && ((canllTxStatusFlag[channel] & CanMailboxSelect[txObjHandle]) != 0))
      {
        tmpCondition = 1;
      }
      else
      {
        tmpCondition = 0;
      }
      CanNestedGlobalInterruptRestore();
      if(tmpCondition != 0)
      # endif
      {
        CAN_POLLING_IRQ_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
        /* do tx confirmation */
        CanHL_TxConfirmation(CAN_HW_CHANNEL_CANPARA_FIRST txObjHandle );
        CAN_POLLING_IRQ_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
      }

#  if defined( C_ENABLE_TRANSMIT_QUEUE )
      if ( txObjHandle == CAN_HL_HW_TX_NORMAL_INDEX(channel) )
      {
        CanHl_RestartTxQueue( CAN_CHANNEL_CANPARA_ONLY );
      }
#  endif /*  C_ENABLE_TRANSMIT_QUEUE */
    }

    canPollingTaskActive[channel] = 0;
  }
} /*CanTxObjTask*/
/* CODE CATEGORY 2 END*/
# endif    /* C_ENABLE_TX_POLLING */


# if defined( C_ENABLE_RX_BASICCAN_POLLING ) && \
    defined( C_ENABLE_RX_BASICCAN_OBJECTS )
/****************************************************************************
| NAME:             CanRxBasicCANObjTask()
| CALLED BY:        
| PRECONDITIONS:    
| INPUT PARAMETERS: CAN_CHANNEL_CANTYPE_FIRST 
|                   CanObjectHandle rxObjHandle
| RETURN VALUES:    none
| DESCRIPTION:      Polling special Object
****************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanRxBasicCANObjTask(CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle)
{
#  if kCanNumberOfHwObjPerBasicCan > 1
  CanObjectHandle i;
#  endif



#  if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(canHwChannel < kCanNumberOfHwChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
#  endif
  assertUser(rxObjHandle < CAN_HL_HW_RX_BASIC_STOPINDEX(canHwChannel), channel, kErrorHwHdlTooLarge);
  assertUser((CanSignedRxHandle)rxObjHandle >= (CanSignedRxHandle)CAN_HL_HW_RX_BASIC_STARTINDEX(canHwChannel), channel, kErrorHwHdlTooSmall); /*lint !e568 */
  assertUser(CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][rxObjHandle] != (vuint8)0, channel, kErrorHwObjNotInPolling);
  assertUser((canPollingTaskActive[channel] == (vuint8)0), channel, kErrorPollingTaskRecursion);

  if (canPollingTaskActive[channel] == (vuint8)0)  /* avoid reentrance */
  {
    canPollingTaskActive[channel] = 1;

#  if defined( C_ENABLE_SLEEP_WAKEUP )
    if ( !CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY) )
#  endif
    {

#  if kCanNumberOfHwObjPerBasicCan > 1
      /* loop over all HW objects assigned to one BasicCAN  */
      for (i = 0; i < kCanNumberOfHwObjPerBasicCan; i++)
      {
#  endif
        /* check for dedicated indication pending */
        if(RXF == (CRFLG & RXF)) /* something received? */
        {
          CANDRV_SET_CODE_TEST_POINT(0x109);
          CAN_POLLING_IRQ_DISABLE(CAN_CHANNEL_CANPARA_ONLY);
          CanBasicCanMsgReceived( CAN_HW_CHANNEL_CANPARA_FIRST rxObjHandle);
          CAN_POLLING_IRQ_RESTORE(CAN_CHANNEL_CANPARA_ONLY);
        }
#  if kCanNumberOfHwObjPerBasicCan > 1
        rxObjHandle++;
      }
#  endif
    }
    canPollingTaskActive[channel] = 0;
  }
} /*CanRxBasicCANObjTask*/
/* CODE CATEGORY 2 END*/
# endif
#endif /*C_ENABLE_INDIVIDUAL_POLLING*/

#if defined( C_ENABLE_RX_BASICCAN_OBJECTS )
/****************************************************************************
| NAME:             CanBasicCanMsgReceived
| CALLED BY:        CanISR()
| PRECONDITIONS:
| INPUT PARAMETERS: internal can chip number
| RETURN VALUES:    none
| DESCRIPTION:      - basic can receive
****************************************************************************/
# if defined( MISRA_CHECK )
  /* suppress misra message about multiple return and usage of goto */
#  pragma PRQA_MESSAGES_OFF 2006,2001,2015
# endif
/* CODE CATEGORY 1 START*/
static void CanBasicCanMsgReceived( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle rxObjHandle)
{
# if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
  tCanRxInfoStruct    *pCanRxInfoStruct;
# endif  
  
# if ( !defined( C_SEARCH_HASH ) )  ||\
     defined( C_HL_ENABLE_HW_RANGES_FILTER ) || \
     defined( C_ENABLE_RANGE_0 ) || \
     defined( C_ENABLE_RANGE_1 ) || \
     defined( C_ENABLE_RANGE_2 ) || \
     defined( C_ENABLE_RANGE_3 )
  tCanRxId0 idRaw0;
#  if (kCanNumberOfUsedCanRxIdTables > 1)
  tCanRxId1 idRaw1;
#  endif
#  if (kCanNumberOfUsedCanRxIdTables > 2)
  tCanRxId2 idRaw2;
#  endif
#  if (kCanNumberOfUsedCanRxIdTables > 3)
  tCanRxId3 idRaw3;
#  endif
#  if (kCanNumberOfUsedCanRxIdTables > 4)
  tCanRxId4 idRaw4;
#  endif
# endif

# if defined( C_SEARCH_HASH )
# if (kCanNumberOfRxBasicCANObjects > 0)
#  if defined( C_ENABLE_EXTENDED_ID )
#    if (kHashSearchListCountEx > 0)
  vuint32          idExt;
  vuint32          winternExt;        /* prehashvalue         */
#    endif
#   if defined( C_ENABLE_MIXED_ID )
#    if (kHashSearchListCount > 0) 
  vuint16          idStd;
  vuint16          winternStd;        /* prehashvalue         */
#    endif
#   endif
#  else
  vuint16          idStd;
  vuint16          winternStd;        /* prehashvalue         */
#  endif
  vuint16          i_increment;    /* delta for next step  */
  vsint16          count;
# endif  /* kCanNumberOfRxBasicCANObjects > 0 */
# endif

  #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  # if defined(C_ENABLE_XGATE_USED)
  #  if defined(C_HL_ENABLE_HW_RANGES_FILTER)
  #   if defined(C_ENABLE_RANGE_0) || defined(C_ENABLE_RANGE_1) || \
         defined(C_ENABLE_RANGE_2) || defined(C_ENABLE_RANGE_3)
  vuint8 tmpCanXgHitRange;
  #   endif
  #  endif
  vuint16 index; /* used to acknowledge pending XGate interrupt */
  vuint16 nInfoIndex;
  # endif
  #endif
  
  #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  vuint8* pId; /* points to the current ID in hardware (temp buffer for XGate) for remote frame check */
  #endif

# if defined( C_ENABLE_GENERIC_PRECOPY ) || \
    defined( C_ENABLE_PRECOPY_FCT )     || \
    defined( C_ENABLE_COPY_RX_DATA )    || \
    defined( C_ENABLE_INDICATION_FLAG ) || \
    defined( C_ENABLE_INDICATION_FCT )  || \
    defined( C_ENABLE_DLC_CHECK )       || \
    defined( C_ENABLE_NOT_MATCHED_FCT )

  CanReceiveHandle         rxHandle;

  rxHandle = kCanRxHandleNotUsed;

# endif

  CANDRV_SET_CODE_TEST_POINT(0x100);

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertInternal(canHwChannel < kCanNumberOfHwChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);
# endif

  #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  # if defined(C_SEARCH_XGATE)
  pId = (vuint8*)(&(canRxTmpBuf[channel]));
  # else
  pId = (vuint8*)CAN_RX_MAILBOX_BASIS_ADR(channel);
  # endif
  # if defined(C_ENABLE_EXTENDED_ID)
  /* extended ID? */
  if((pId[1] & (vuint8)0x08) == (vuint8)0x08)
  {
    /* remote frame in extended format? */
    if((pId[3] & (vuint8)0x01) == (vuint8)0x01)
    {
      goto finishBasicCan;
    }
  }
  #  if defined(C_ENABLE_MIXED_ID)
  else
  {
    /* remote frame in standard format? */
    if((pId[1] & (vuint8)0x10) == (vuint8)0x10)
    {
      goto finishBasicCan;
    }
  }
  #  endif
  # else
  /* standard ID? */
  if((pId[1] & (vuint8)0x10) == (vuint8)0x10)
  { 
    /* remote frame in standard format? */
    goto finishBasicCan;
  }
  # endif
  #endif
  

# if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
  pCanRxInfoStruct =  &canRxInfoStruct[channel];
  #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  # if defined(C_SEARCH_XGATE)
  (pCanRxInfoStruct->pChipMsgObj) = (CanChipMsgPtr)&(canRxTmpBuf[channel]);
  # else
  (pCanRxInfoStruct->pChipMsgObj) = (CanChipMsgPtr)CAN_RX_MAILBOX_BASIS_ADR(channel);
  # endif
  #endif
  
  
  #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  # if defined(C_SEARCH_XGATE)
  (pCanRxInfoStruct->pChipData) = (CanChipDataPtr)&(canRxTmpBuf[channel].DataFld[0]);
  # else
  (pCanRxInfoStruct->pChipData) = (CanChipDataPtr)&(CAN_RX_MAILBOX_BASIS_ADR(channel) -> DataFld[0]);
  # endif
  #endif
  
  
  canRDSRxPtr[channel] = pCanRxInfoStruct->pChipData;
# else
  #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  # if defined(C_SEARCH_XGATE)
  (canRxInfoStruct[channel].pChipMsgObj) = (CanChipMsgPtr)&(canRxTmpBuf[channel]);
  # else
  (canRxInfoStruct[channel].pChipMsgObj) = (CanChipMsgPtr)CAN_RX_MAILBOX_BASIS_ADR(channel);
  # endif
  #endif
  
  
  #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  # if defined(C_SEARCH_XGATE)
  (canRxInfoStruct[channel].pChipData) = (CanChipDataPtr)&(canRxTmpBuf[channel].DataFld[0]);
  # else
  (canRxInfoStruct[channel].pChipData) = (CanChipDataPtr)&(CAN_RX_MAILBOX_BASIS_ADR(channel) -> DataFld[0]);
  # endif
  #endif
  
  
  canRDSRxPtr[channel] = canRxInfoStruct[channel].pChipData;
# endif
  CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)      = kCanRxHandleNotUsed;  

# if defined( C_ENABLE_CAN_RAM_CHECK )
  if(canComStatus[channel] == kCanDisableCommunication)
  {
    goto finishBasicCan; /* ignore reception */
  }
# endif

# if defined( C_HL_ENABLE_OVERRUN_IN_STATUS )
# else
#  if defined( C_ENABLE_OVERRUN )
  #  error "not used by this driver"
    ApplCanOverrun( CAN_CHANNEL_CANPARA_ONLY );
  }
#  endif
# endif

  /**************************** reject remote frames  ****************************************/


  /**************************** reject messages with unallowed ID type ****************************************/
# if defined( C_HL_ENABLE_REJECT_UNWANTED_IDTYPE )
#  if defined( C_ENABLE_EXTENDED_ID )
#   if defined( C_ENABLE_MIXED_ID )
#   else
  if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) != kCanIdTypeExt)
  {
    goto finishBasicCan;
  }
#   endif
#  else
  if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) != kCanIdTypeStd)
  {
    goto finishBasicCan;
  }
#  endif
# endif /* C_HL_ENABLE_REJECT_UNWANTED_IDTYPE */


# if defined( C_ENABLE_COND_RECEIVE_FCT )
  /**************************** conditional message receive function  ****************************************/
  if(canMsgCondRecState[channel]==kCanTrue)
  {
    ApplCanMsgCondReceived( CAN_HL_P_RX_INFO_STRUCT(channel) );
  }
# endif

# if defined( C_ENABLE_RECEIVE_FCT )
  /**************************** call ApplCanMsgReceived() ****************************************************/
  if (APPL_CAN_MSG_RECEIVED( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanNoCopyData)
  {
    goto finishBasicCan;
  }
# endif
  

# if ( !defined( C_SEARCH_HASH ) ) || \
     defined( C_HL_ENABLE_HW_RANGES_FILTER ) || \
     defined( C_ENABLE_RANGE_0 ) || \
     defined( C_ENABLE_RANGE_1 ) || \
     defined( C_ENABLE_RANGE_2 ) || \
     defined( C_ENABLE_RANGE_3 )
  /**************************** calculate idRaw for filtering ************************************************/
#  if defined( C_ENABLE_EXTENDED_ID )
#   if defined( C_ENABLE_MIXED_ID )
  if (CanRxActualIdType(CAN_HL_P_RX_INFO_STRUCT(channel)) == kCanIdTypeExt)
#   endif
  {
#   if defined( C_ENABLE_RX_MASK_EXT_ID )
    idRaw0 = CanRxActualIdRaw0( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID0(C_MASK_EXT_ID);
#    if (kCanNumberOfUsedCanRxIdTables > 1)
    idRaw1 = CanRxActualIdRaw1( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID1(C_MASK_EXT_ID);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 2)
    idRaw2 = CanRxActualIdRaw2( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID2(C_MASK_EXT_ID);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 3)
    idRaw3 = CanRxActualIdRaw3( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID3(C_MASK_EXT_ID);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 4)
    idRaw4 = CanRxActualIdRaw4( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID4(C_MASK_EXT_ID);
#    endif
#   else
    idRaw0 = CanRxActualIdRaw0( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID0(0x1FFFFFFF);
#    if (kCanNumberOfUsedCanRxIdTables > 1)
    idRaw1 = CanRxActualIdRaw1( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID1(0x1FFFFFFF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 2)
    idRaw2 = CanRxActualIdRaw2( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID2(0x1FFFFFFF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 3)
    idRaw3 = CanRxActualIdRaw3( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID3(0x1FFFFFFF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 4)
    idRaw4 = CanRxActualIdRaw4( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_EXTID4(0x1FFFFFFF);
#    endif
#   endif /*  C_ENABLE_RX_MASK_EXT_ID */
  }
#   if defined( C_ENABLE_MIXED_ID )
  else
  {
    idRaw0 = CanRxActualIdRaw0( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID0(0x7FF);
#    if (kCanNumberOfUsedCanRxIdTables > 1)
    idRaw1 = CanRxActualIdRaw1( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID1(0x7FF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 2)
    idRaw2 = CanRxActualIdRaw2( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID2(0x7FF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 3)
    idRaw3 = CanRxActualIdRaw3( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID3(0x7FF);
#    endif
#    if (kCanNumberOfUsedCanRxIdTables > 4)
    idRaw4 = CanRxActualIdRaw4( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID4(0x7FF);
#    endif
  }
#   endif
#  else /* C_ENABLE_EXTENDED_ID */
  {
    idRaw0 = CanRxActualIdRaw0( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID0(0x7FF);
#   if (kCanNumberOfUsedCanRxIdTables > 1)
    idRaw1 = CanRxActualIdRaw1( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID1(0x7FF);
#   endif
#   if (kCanNumberOfUsedCanRxIdTables > 2)
    idRaw2 = CanRxActualIdRaw2( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID2(0x7FF);
#   endif
#   if (kCanNumberOfUsedCanRxIdTables > 3)
    idRaw3 = CanRxActualIdRaw3( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID3(0x7FF);
#   endif
#   if (kCanNumberOfUsedCanRxIdTables > 4)
    idRaw4 = CanRxActualIdRaw4( CAN_HL_P_RX_INFO_STRUCT(channel) ) & MK_STDID4(0x7FF);
#   endif
  }
#  endif /* C_ENABLE_EXTENDED_ID */
# endif /* ( !defined( C_SEARCH_HASH ) && ...  defined( C_ENABLE_RANGE_3 )*/

  /******************* Range filtering ********************************************************************/
# if defined( C_HL_ENABLE_HW_RANGES_FILTER )

  #  if defined(C_ENABLE_RANGE_0) || defined(C_ENABLE_RANGE_1) || \
        defined(C_ENABLE_RANGE_2) || defined(C_ENABLE_RANGE_3)
    tmpCanXgHitRange = canXgHitRange[channel];
  
  #   if defined( C_ENABLE_RANGE_0 )
    if((tmpCanXgHitRange & 0x01) != 0)
    {
  #    if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
      if ( (CanChannelObject[channel].RangeActiveFlag & kCanRange0) != (vuint16)0 )
      {
  #    else    /* C_SINGLE_RECEIVE_CHANNEL) */
      {
  #    endif
  
  #    if defined(C_ENABLE_RX_QUEUE_RANGE)
        if (CanRxQueueRange0[channel] == kCanTrue)
        {
          pCanRxInfoStruct->Handle      = kCanRxHandleRange0;
          (void)CanHL_ReceivedRxHandleQueue( CAN_CHANNEL_CANPARA_ONLY );
          goto finishBasicCan;
        }
      else
  #    endif  /* C_ENABLE_RX_QUEUE */
        {
          if (APPLCANRANGE0PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanNoCopyData)
          {
            goto finishBasicCan;
          }
        }
      }
    }
  #   endif  /* C_ENABLE_RANGE_0 */
  
  #   if defined( C_ENABLE_RANGE_1 )
    if((tmpCanXgHitRange & 0x02) != 0)
    {
  #    if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
      if ( (CanChannelObject[channel].RangeActiveFlag & kCanRange1) != (vuint16)0 )
      {
  #    else    /* C_SINGLE_RECEIVE_CHANNEL) */
      {
  #    endif
  
  #    if defined(C_ENABLE_RX_QUEUE_RANGE)
        if (CanRxQueueRange1[channel] == kCanTrue)
        {
          pCanRxInfoStruct->Handle      = kCanRxHandleRange1;
          (void)CanHL_ReceivedRxHandleQueue( CAN_CHANNEL_CANPARA_ONLY );
          goto finishBasicCan;
        }
      else
  #    endif  /* C_ENABLE_RX_QUEUE */
        {
          if (APPLCANRANGE1PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanNoCopyData)
          {
            goto finishBasicCan;
          }
        }
      }
    }
  #   endif  /* C_ENABLE_RANGE_1 */
  
  #   if defined( C_ENABLE_RANGE_2 )
    if((tmpCanXgHitRange & 0x04) != 0)
    {
  #    if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
      if ( (CanChannelObject[channel].RangeActiveFlag & kCanRange2) != (vuint16)0 )
      {
  #    else    /* C_SINGLE_RECEIVE_CHANNEL) */
      {
  #    endif
  
  #    if defined(C_ENABLE_RX_QUEUE_RANGE)
        if (CanRxQueueRange2[channel] == kCanTrue)
        {
          pCanRxInfoStruct->Handle      = kCanRxHandleRange2;
          (void)CanHL_ReceivedRxHandleQueue( CAN_CHANNEL_CANPARA_ONLY );
          goto finishBasicCan;
        }
      else
  #    endif  /* C_ENABLE_RX_QUEUE */
        {
          if (APPLCANRANGE2PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanNoCopyData)
          {
            goto finishBasicCan;
          }
        }
      }
    }
  #   endif  /* C_ENABLE_RANGE_2 */
  
  #   if defined( C_ENABLE_RANGE_3 )
    if((tmpCanXgHitRange & 0x08) != 0)
    {
  #    if defined ( C_MULTIPLE_RECEIVE_CHANNEL )
      if ( (CanChannelObject[channel].RangeActiveFlag & kCanRange3) != (vuint16)0 )
      {
  #    else    /* C_SINGLE_RECEIVE_CHANNEL) */
      {
  #    endif
  
  #    if defined(C_ENABLE_RX_QUEUE_RANGE)
        if (CanRxQueueRange3[channel] == kCanTrue)
        {
          pCanRxInfoStruct->Handle      = kCanRxHandleRange3;
          (void)CanHL_ReceivedRxHandleQueue( CAN_CHANNEL_CANPARA_ONLY );
          goto finishBasicCan;
        }
      else
  #    endif  /* C_ENABLE_RX_QUEUE */
        {
          if (APPLCANRANGE3PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanNoCopyData)
          {
            goto finishBasicCan;
          }
        }
      }
    }
  #   endif  /* C_ENABLE_RANGE_3 */
  
  #  endif

# else

  {
#  if defined( C_ENABLE_RANGE_0 )
#   if defined( C_MULTIPLE_RECEIVE_CHANNEL )
    if ( (CanChannelObject[channel].RangeActiveFlag & kCanRange0) != (vuint16)0 )
    {
#    if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == CANRANGE0IDTYPE(channel))
#    endif
      {
        if ( C_RANGE_MATCH( CAN_RX_IDRAW_PARA, CANRANGE0ACCMASK(channel), CANRANGE0ACCCODE(channel)) )
#   else    /* C_SINGLE_RECEIVE_CHANNEL) */
    {
#    if (CANRANGE0IDTYPE(channel) == kCanIdTypeStd )
#     if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeStd)
#     endif
      {
        if ( C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, CANRANGE0ACCMASK(channel), CANRANGE0ACCCODE(channel)) )
#    else  /* C_RANGE_0_IDTYPE == kCanIdTypeExt */
#     if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeExt)
#     endif
      {
        if ( C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, CANRANGE0ACCMASK(channel), CANRANGE0ACCCODE(channel)) )
#    endif
#   endif
        {
#   if defined( C_ENABLE_RX_QUEUE_RANGE )
          if (CanRxQueueRange0[channel] == kCanTrue)
          {
            pCanRxInfoStruct->Handle      = kCanRxHandleRange0;
            (void)CanHL_ReceivedRxHandleQueue( CAN_CHANNEL_CANPARA_ONLY );
            goto finishBasicCan;
          }
          else
#   endif  /* C_ENABLE_RX_QUEUE */
          {
            if (APPLCANRANGE0PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanNoCopyData)
            {
              goto finishBasicCan;
            }
          }
        }
      }
    }
#  endif  /* C_ENABLE_RANGE_0 */

#  if defined( C_ENABLE_RANGE_1 )
#   if defined( C_MULTIPLE_RECEIVE_CHANNEL )
    if ( (CanChannelObject[channel].RangeActiveFlag & kCanRange1) != (vuint16)0 )
    {
#    if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == CANRANGE1IDTYPE(channel))
#    endif
      {
        if ( C_RANGE_MATCH( CAN_RX_IDRAW_PARA, CANRANGE1ACCMASK(channel), CANRANGE1ACCCODE(channel)) )
#   else    /* C_SINGLE_RECEIVE_CHANNEL) */
    {
#    if (CANRANGE1IDTYPE(channel) == kCanIdTypeStd )
#     if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeStd)
#     endif
      {
        if ( C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, CANRANGE1ACCMASK(channel), CANRANGE1ACCCODE(channel)) )
#    else  /* C_RANGE_1_IDTYPE == kCanIdTypeExt */
#     if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeExt)
#     endif
      {
        if ( C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, CANRANGE1ACCMASK(channel), CANRANGE1ACCCODE(channel)) )
#    endif
#   endif
        {
#   if defined( C_ENABLE_RX_QUEUE_RANGE )
          if (CanRxQueueRange1[channel] == kCanTrue)
          {
            pCanRxInfoStruct->Handle      = kCanRxHandleRange1;
            (void)CanHL_ReceivedRxHandleQueue( CAN_CHANNEL_CANPARA_ONLY );
            goto finishBasicCan;
          }
          else
#   endif  /* C_ENABLE_RX_QUEUE */
          {
            if (APPLCANRANGE1PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanNoCopyData)
            {
              goto finishBasicCan;
            }
          }
        }
      }
    }
#  endif  /* C_ENABLE_RANGE_1 */

#  if defined( C_ENABLE_RANGE_2 )
#   if defined( C_MULTIPLE_RECEIVE_CHANNEL )
    if ( (CanChannelObject[channel].RangeActiveFlag & kCanRange2) != (vuint16)0 )
    {
#    if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == CANRANGE2IDTYPE(channel))
#    endif
      {
        if ( C_RANGE_MATCH( CAN_RX_IDRAW_PARA, CANRANGE2ACCMASK(channel), CANRANGE2ACCCODE(channel)) )
#   else    /* C_SINGLE_RECEIVE_CHANNEL) */
    {
#    if (CANRANGE2IDTYPE(channel) == kCanIdTypeStd )
#     if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeStd)
#     endif
      {
        if ( C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, CANRANGE2ACCMASK(channel), CANRANGE2ACCCODE(channel)) )
#    else  /* C_RANGE_2_IDTYPE == kCanIdTypeExt */
#     if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeExt)
#     endif
      {
        if ( C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, CANRANGE2ACCMASK(channel), CANRANGE2ACCCODE(channel)) )
#    endif
#   endif
        {
#   if defined( C_ENABLE_RX_QUEUE_RANGE )
          if (CanRxQueueRange2[channel] == kCanTrue)
          {
            pCanRxInfoStruct->Handle      = kCanRxHandleRange2;
            (void)CanHL_ReceivedRxHandleQueue( CAN_CHANNEL_CANPARA_ONLY );
            goto finishBasicCan;
          }
          else
#   endif  /* C_ENABLE_RX_QUEUE */
          {
            if (APPLCANRANGE2PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanNoCopyData)
            {
              goto finishBasicCan;
            }
          }
        }
      }
    }
#  endif  /* C_ENABLE_RANGE_2 */

#  if defined( C_ENABLE_RANGE_3 )
#   if defined( C_MULTIPLE_RECEIVE_CHANNEL )
    if ( (CanChannelObject[channel].RangeActiveFlag & kCanRange3) != (vuint16)0 )
    {
#    if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == CANRANGE3IDTYPE(channel))
#    endif
      {
        if ( C_RANGE_MATCH( CAN_RX_IDRAW_PARA, CANRANGE3ACCMASK(channel), CANRANGE3ACCCODE(channel)) )
#   else    /* C_SINGLE_RECEIVE_CHANNEL) */
    {
#    if (CANRANGE3IDTYPE(channel) == kCanIdTypeStd )
#     if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeStd)
#     endif
      {
        if ( C_RANGE_MATCH_STD( CAN_RX_IDRAW_PARA, CANRANGE3ACCMASK(channel), CANRANGE3ACCCODE(channel)) )
#    else  /* C_RANGE_3_IDTYPE == kCanIdTypeExt */
#     if defined( C_ENABLE_MIXED_ID ) && !defined( C_HL_ENABLE_IDTYPE_IN_ID )
      if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanIdTypeExt)
#     endif
      {
        if ( C_RANGE_MATCH_EXT( CAN_RX_IDRAW_PARA, CANRANGE3ACCMASK(channel), CANRANGE3ACCCODE(channel)) )
#    endif
#   endif
        {
#   if defined( C_ENABLE_RX_QUEUE_RANGE )
          if (CanRxQueueRange3[channel] == kCanTrue)
          {
            pCanRxInfoStruct->Handle      = kCanRxHandleRange3;
            (void)CanHL_ReceivedRxHandleQueue( CAN_CHANNEL_CANPARA_ONLY );
            goto finishBasicCan;
          }
          else
#   endif  /* C_ENABLE_RX_QUEUE */
          {
            if (APPLCANRANGE3PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) ) == kCanNoCopyData)
            {
              goto finishBasicCan;
            }
          }
        }
      }
    }
#  endif  /* C_ENABLE_RANGE_3 */

  }

# endif /* defined( C_HL_ENABLE_HW_RANGES_FILTER ) */

# if defined( C_ENABLE_GENERIC_PRECOPY ) || \
    defined( C_ENABLE_PRECOPY_FCT )     || \
    defined( C_ENABLE_COPY_RX_DATA )    || \
    defined( C_ENABLE_INDICATION_FLAG ) || \
    defined( C_ENABLE_INDICATION_FCT )  || \
    defined( C_ENABLE_DLC_CHECK )       || \
    defined( C_ENABLE_NOT_MATCHED_FCT )
# if (kCanNumberOfRxBasicCANObjects > 0)

   /* search the received id in ROM table: */

  #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  # if defined(C_SEARCH_XGATE)
  rxHandle = canXgRxHandle[channel]; /* get the handle which was found by the XGate */
  # endif
  #endif

#  if defined( C_SEARCH_LINEAR )
  /* ************* Linear search ******************************************** */
    

  for (rxHandle = CAN_HL_RX_BASIC_STARTINDEX(channel); rxHandle < CAN_HL_RX_BASIC_STOPINDEX(channel) ;rxHandle++)
  {
	/*if (CanRxId0[rxHandle] == MK_STDID0(0x408u))                                          //Add for TMM, find the corresponding index(buffer) with actual RX ID
	{                   
		CanRxId0[rxHandle] = MK_STDID0(fbl_meta_info[0].crc32&0x7FF);        
	}                   
	else if (CanRxId0[rxHandle] == MK_STDID0(0x231u)) 
	{
		CanRxId0[rxHandle] = MK_STDID0(fbl_meta_info[0].crc_start&0x7FF);       
	}
	else if (CanRxId0[rxHandle] == MK_STDID0(0x0A8u)) 
	{
		CanRxId0[rxHandle] = MK_STDID0(fbl_meta_info[0].crc_length&0x7FF);      
	}*/
	
    if( idRaw0 == CanGetRxId0(rxHandle) )
    {
#   if (kCanNumberOfUsedCanRxIdTables > 1)
      if( idRaw1 == CanGetRxId1(rxHandle) )
#   endif
      {
#   if (kCanNumberOfUsedCanRxIdTables > 2)
        if( idRaw2 == CanGetRxId2(rxHandle) )
#   endif
        {
#   if (kCanNumberOfUsedCanRxIdTables > 3)
          if( idRaw3 == CanGetRxId3(rxHandle) )
#   endif
          {
#   if (kCanNumberOfUsedCanRxIdTables > 4)
            if( idRaw4 == CanGetRxId4(rxHandle) )
#   endif
            {
#   if defined( C_ENABLE_MIXED_ID ) 
#    if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#    else
              /* verify ID type, if not already done with the ID raw */
              if (CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) ) == CanGetRxIdType(rxHandle))
#    endif
#   endif
              {
                break;    /*exit loop with index rxHandle */
              }
            }
          }
        }
      }
    }
  }
#  endif

#  if defined( C_SEARCH_HASH )
  /* ************* Hash search ********************************************* */
#   if defined( C_CPUTYPE_8BIT )
#    error "Hash search with 8-Bit CPU not tested yet"
#   endif   
    
#   if defined( C_ENABLE_EXTENDED_ID ) 
  /* one or more Extended ID listed */
#    if defined( C_ENABLE_MIXED_ID )
  if((CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) )) == kCanIdTypeExt)
#    endif
#    if (kHashSearchListCountEx > 0)
  {
  /* calculate the logical ID */
#     if defined( C_ENABLE_RX_MASK_EXT_ID )
    idExt          = (CanRxActualId( CAN_HL_P_RX_INFO_STRUCT(channel) ) &  C_MASK_EXT_ID ) | \
                                                                               ((vuint32)channel << 29);                
#     else
    idExt          = CanRxActualId( CAN_HL_P_RX_INFO_STRUCT(channel) )| ((vuint32)channel << 29);
#     endif

    winternExt     = idExt + kHashSearchRandomNumberEx;                    
    rxHandle           = (CanReceiveHandle)(winternExt % kHashSearchListCountEx);
#     if (kHashSearchListCountEx == 1) 
    i_increment = 0;
#     else
    i_increment = (vuint16)((winternExt % (kHashSearchListCountEx - 1)) + (vuint8)1);          /* ST10-CCAN Keil compiler complains without cast */
#     endif
    count       = (vsint16)kHashSearchMaxStepsEx - 1;               
  
    while(idExt != CanRxHashIdEx[rxHandle])   
    {
      if(count == 0)  
      {
#     if defined( C_ENABLE_NOT_MATCHED_FCT )
        ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
#     endif
        goto finishBasicCan;
      }
      count--;
      rxHandle += i_increment;
      if( rxHandle >= (vuint16)kHashSearchListCountEx ) 
      {
        rxHandle -= kHashSearchListCountEx;
      }
    }
    rxHandle = CanRxMsgIndirection[rxHandle+kHashSearchListCount];
  }
#    else /* (kHashSearchListCountEx > 0) */
  {
#     if defined( C_ENABLE_NOT_MATCHED_FCT )
    ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
#     endif
    goto finishBasicCan;
  }
#    endif /* (kHashSearchListCountEx > 0) */

#    if defined( C_ENABLE_MIXED_ID )
  else   /* if((CanRxActualIdType( CAN_HL_P_RX_INFO_STRUCT(channel) )) == kCanIdTypeStd)  */
#    endif
#   endif /* IF defined( C_ENABLE_EXTENDED_ID ) */

#   if defined( C_ENABLE_MIXED_ID ) || !defined( C_ENABLE_EXTENDED_ID ) 
#    if (kHashSearchListCount > 0) 
  {
    idStd          = ((vuint16)CanRxActualId( CAN_HL_P_RX_INFO_STRUCT(channel) )
                               | ((vuint16)channel << 11));                /* calculate the logical ID */
    winternStd     = idStd + kHashSearchRandomNumber;    
    rxHandle       = (CanReceiveHandle)(winternStd % kHashSearchListCount);
#     if (kHashSearchListCount == 1)
    i_increment = 0;
#     else
    i_increment = (vuint16)((winternStd % (kHashSearchListCount - 1)) + (vuint8)1);
#     endif
    count       = (vsint16)kHashSearchMaxSteps-1;

    /* type of CanRxHashId table depends on the used type of Id */
    while ( idStd != CanRxHashId[rxHandle])
    {
      if (count == (vsint16)0)
      {
#     if defined( C_ENABLE_NOT_MATCHED_FCT )
        ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
#     endif
        goto finishBasicCan;
      }
      count--; 
      rxHandle += i_increment;
      if ( rxHandle >= kHashSearchListCount )
      {
        rxHandle -= kHashSearchListCount;
      }
    }
    rxHandle = CanRxMsgIndirection[rxHandle];
  }
#    else /* (kHashSearchListCount > 0) */
  {
#     if defined( C_ENABLE_NOT_MATCHED_FCT )
    ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
#     endif
    goto finishBasicCan;
  }
#    endif /* (kHashSearchListCount > 0) */
#   endif /* defined( C_ENABLE_MIXED_ID ) || !defined( C_ENABLE_EXTENDED_ID ) */ 
#  endif /* defined( C_SEARCH_HASH ) */



  /**************************** handle filtered message ****************************************************/
#  if defined( C_SEARCH_HASH )
  assertInternal((rxHandle < kCanNumberOfRxObjects), kCanAllChannels , kErrorRxHandleWrong);  /* legal rxHandle ? */
#  else
  if ( rxHandle < CAN_HL_RX_BASIC_STOPINDEX(channel)) 
#  endif
  {
    /* ID found in table */
#  if defined( C_SEARCH_HASH ) 
#  else
#   if defined( C_ENABLE_RX_MSG_INDIRECTION )
    rxHandle = CanRxMsgIndirection[rxHandle];       /* indirection for special sort-algoritms */
#   endif
#  endif

    CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel) = rxHandle;  

#  if defined( C_ENABLE_RX_QUEUE )
    if (CanHL_ReceivedRxHandleQueue( CAN_CHANNEL_CANPARA_ONLY ) == kCanHlContinueRx)
#  else
    if (CanHL_ReceivedRxHandle( CAN_CHANNEL_CANPARA_ONLY ) == kCanHlContinueRx)
#  endif
    { 
#  if defined( C_ENABLE_INDICATION_FLAG ) || \
          defined( C_ENABLE_INDICATION_FCT )
      #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
      # if defined(C_ENABLE_XGATE_USED)
      #   if defined(C_SINGLE_RECEIVE_CHANNEL)
      index = (kCanBasisAdr >> 6) & 0x07;
      #   else
      index = (CanBasisAdr[channel] >> 6) & 0x07;
      #   endif
      nInfoIndex = CanXGateChannelToInfo[index];
      *CanXGateIrqChannelInfo[nInfoIndex].pXGif = CanXGateIrqChannelInfo[nInfoIndex].nRxMask;
      CanBeginCriticalXGateSection();
      canRxSyncObj[channel] = 0; 
      CanEndCriticalXGateSection();
      # else
      CRFLG = RXF; /* clear receive pending flag */
      # endif
      #endif
      

      CanHL_IndRxHandle( rxHandle );
      

      return;
#  endif
    }
  }
# endif /* kCanNumberOfRxBasicCANObjects > 0 */
#  if defined( C_ENABLE_NOT_MATCHED_FCT )
#   if defined( C_SEARCH_HASH )
#   else
#    if (kCanNumberOfRxBasicCANObjects > 0)
  else
#    endif /* kCanNumberOfRxBasicCANObjects > 0 */
  {
    ApplCanMsgNotMatched( CAN_HL_P_RX_INFO_STRUCT(channel) );
  }
#   endif
#  endif

# endif

  goto finishBasicCan;     /* to avoid compiler warning */

/* Msg(4:2015) This label is not a case or default label for a switch statement. MISRA Rule 55 */
finishBasicCan:

  /* make receive buffer free*/
  #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  # if defined(C_ENABLE_XGATE_USED)
  #   if defined(C_SINGLE_RECEIVE_CHANNEL)
  index = (kCanBasisAdr >> 6) & 0x07;
  #   else
  index = (CanBasisAdr[channel] >> 6) & 0x07;
  #   endif
  nInfoIndex = CanXGateChannelToInfo[index];
  *CanXGateIrqChannelInfo[nInfoIndex].pXGif = CanXGateIrqChannelInfo[nInfoIndex].nRxMask;
  CanBeginCriticalXGateSection();
  canRxSyncObj[channel] = 0; 
  CanEndCriticalXGateSection();
  # else
  CRFLG = RXF; /* clear receive pending flag */
  # endif
  #endif
  


  return;    /* to avoid compiler warnings about label without code */

} /* end of BasicCan */
/* CODE CATEGORY 1 END*/
# if defined( MISRA_CHECK )
#  pragma PRQA_MESSAGES_ON 2006,2001,2015
# endif
#endif


#if defined( C_ENABLE_RX_BASICCAN_OBJECTS )
# if ( kCanNumberOfRxObjects > 0 )
/****************************************************************************
| NAME:             CanHL_ReceivedRxHandle
| CALLED BY:        CanBasicCanMsgReceived, CanFullCanMsgReceived
| PRECONDITIONS:
| INPUT PARAMETERS: Handle of received Message to access generated data
| RETURN VALUES:    none
| DESCRIPTION:      DLC-check, Precopy and copy of Data for received message
****************************************************************************/
#  if defined( MISRA_CHECK )
  /* suppress misra message about multiple return  */
#   pragma PRQA_MESSAGES_OFF 2006
#  endif
/* CODE CATEGORY 1 START*/
#  if defined( C_ENABLE_RX_QUEUE )
static vuint8 CanHL_ReceivedRxHandle( CAN_CHANNEL_CANTYPE_FIRST tCanRxInfoStruct *pCanRxInfoStruct )
{
#  else
static vuint8 CanHL_ReceivedRxHandle( CAN_CHANNEL_CANTYPE_ONLY )
{
#  endif
#  if !defined( C_ENABLE_RX_QUEUE ) &&\
    defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
  tCanRxInfoStruct    *pCanRxInfoStruct;
#  endif

#  if defined( C_ENABLE_COPY_RX_DATA )
#   if C_SECURITY_LEVEL > 20
  CanDeclareGlobalInterruptOldStatus
#   endif
#  endif

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertInternal(channel < kCanNumberOfChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);
# endif

#  if !defined( C_ENABLE_RX_QUEUE ) &&\
    defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )
  pCanRxInfoStruct =  &canRxInfoStruct[channel];
#  endif

#  if defined( C_ENABLE_MULTI_ECU_PHYS )
  if ( (CanRxIdentityAssignment[CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)] & V_ACTIVE_IDENTITY_MSK) == (tVIdentityMsk)0 )
  {
    /* message is not a receive message in the active indentity */
    CANDRV_SET_CODE_TEST_POINT(0x10B);
    return  kCanHlFinishRx;
  }
#  endif


#  if defined( C_ENABLE_DLC_CHECK )  
#   if defined( C_ENABLE_DLC_CHECK_MIN_DATALEN )
  if ( (CanGetRxMinDataLen(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel))) > CanRxActualDLC( CAN_HL_P_RX_INFO_STRUCT(channel) ) )
#   else
  if ( (CanGetRxDataLen(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel))) > CanRxActualDLC( CAN_HL_P_RX_INFO_STRUCT(channel) ) )
#   endif
  {
    /* ##RI1.4 - 2.7: Callbackfunction-DLC-Check */
#   if defined( C_ENABLE_DLC_FAILED_FCT )
    ApplCanMsgDlcFailed( CAN_HL_P_RX_INFO_STRUCT(channel) ); 
#   endif  /*C_ENABLE_DLC_FAILED_FCT */
    return  kCanHlFinishRx;
  }
#  endif

#  if defined( C_ENABLE_VARIABLE_RX_DATALEN )
  CanSetVariableRxDatalen (CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel), CanRxActualDLC( CAN_HL_P_RX_INFO_STRUCT(channel)));
#  endif

#  if defined( C_ENABLE_GENERIC_PRECOPY )
  if ( APPL_CAN_GENERIC_PRECOPY( CAN_HL_P_RX_INFO_STRUCT(channel) ) != kCanCopyData)
  {
    return kCanHlFinishRx;  
  }
#  endif

#  if defined( C_ENABLE_PRECOPY_FCT )
  if ( CanGetApplPrecopyPtr(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)) != V_NULL )    /*precopy routine */
  {
    /* canRxHandle in indexed drivers only for consistancy check in higher layer modules */
    canRxHandle[channel] = CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel);
    
    if ( CanGetApplPrecopyPtr(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel))( CAN_HL_P_RX_INFO_STRUCT(channel) )==kCanNoCopyData )
    {  /* precopy routine returns kCanNoCopyData:   */
      return  kCanHlFinishRx;
    }                      /* do not copy data check next irpt */
  }
#  endif

#  if defined( C_ENABLE_COPY_RX_DATA )
  /* no precopy or precopy returns kCanCopyData : copy data -- */
  /* copy via index -------------------------------------------*/
  if ( CanGetRxDataPtr(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)) != V_NULL )      /* copy if buffer exists */
  {
    /* copy data ---------------------------------------------*/
#   if C_SECURITY_LEVEL > 20
    CanNestedGlobalInterruptDisable();
#   endif
    CANDRV_SET_CODE_TEST_POINT(0x107);
#   if defined( C_ENABLE_COPY_RX_DATA_WITH_DLC )
    if ( CanRxActualDLC( CAN_HL_P_RX_INFO_STRUCT(channel)) < CanGetRxDataLen(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)) )
    {
#    if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )    
      (void)VStdMemCpyRamToRam((void*)CanGetRxDataPtr(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)), (void*)pCanRxInfoStruct->pChipData, CanRxActualDLC(CAN_HL_P_RX_INFO_STRUCT(channel)));
#    else
      (void)VStdMemCpyRamToRam((void*)CanGetRxDataPtr(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)), (void*)canRxInfoStruct[channel].pChipData, CanRxActualDLC(CAN_HL_P_RX_INFO_STRUCT(channel)));
#    endif 
    }
    else
#   endif   /* C_ENABLE_COPY_RX_DATA_WITH_DLC */
    {
#   if defined( C_HL_ENABLE_RX_INFO_STRUCT_PTR )    
      (void)VStdMemCpyRamToRam((void*)CanGetRxDataPtr(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)), (void*)pCanRxInfoStruct->pChipData, CanGetRxDataLen(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)));
#   else
      (void)VStdMemCpyRamToRam((void*)CanGetRxDataPtr(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)), (void*)canRxInfoStruct[channel].pChipData, CanGetRxDataLen(CAN_HL_P_RX_INFO_STRUCT_HANDLE(channel)));
#   endif 
    }
#   if C_SECURITY_LEVEL > 20
    CanNestedGlobalInterruptRestore();
#   endif
  }
#  endif /* ( C_ENABLE_COPY_RX_DATA ) */

  CANDRV_SET_CODE_TEST_POINT(0x105);
  return kCanHlContinueRx;
} /* end of CanReceivceRxHandle() */
/* CODE CATEGORY 1 END*/
#  if defined( MISRA_CHECK )
#   pragma PRQA_MESSAGES_ON 2006
#  endif

#  if defined( C_ENABLE_INDICATION_FLAG ) || \
     defined( C_ENABLE_INDICATION_FCT )
/****************************************************************************
| NAME:             CanHL_IndRxHandle
| CALLED BY:        CanBasicCanMsgReceived, CanFullCanMsgReceived
| PRECONDITIONS:
| INPUT PARAMETERS: Handle of received Message to access generated data
| RETURN VALUES:    none
| DESCRIPTION:      DLC-check, Precopy and copy of Data for received message
****************************************************************************/
/* CODE CATEGORY 1 START*/
static void CanHL_IndRxHandle( CanReceiveHandle rxHandle )
{
#   if defined( C_ENABLE_INDICATION_FLAG )
#    if C_SECURITY_LEVEL > 20
  CanDeclareGlobalInterruptOldStatus
#    endif
#   endif

#   if defined( C_ENABLE_INDICATION_FLAG )
#    if C_SECURITY_LEVEL > 20
  CanNestedGlobalInterruptDisable();
#    endif
  CanIndicationFlags._c[CanGetIndicationOffset(rxHandle)] |= CanGetIndicationMask(rxHandle);
#    if C_SECURITY_LEVEL > 20
  CanNestedGlobalInterruptRestore();
#    endif
#   endif

#   if defined( C_ENABLE_INDICATION_FCT )
  if ( CanGetApplIndicationPtr(rxHandle) != V_NULL )
  {
    CanGetApplIndicationPtr(rxHandle)(rxHandle);  /* call IndicationRoutine */
  }
#   endif
}
/* CODE CATEGORY 1 END*/
#  endif /* C_ENABLE_INDICATION_FLAG || C_ENABLE_INDICATION_FCT  */
# endif /* ( kCanNumberOfRxObjects > 0 ) */
#endif


/****************************************************************************
| NAME:             CanHL_TxConfirmation
| CALLED BY:        CanISR()
| PRECONDITIONS:
| INPUT PARAMETERS: - internal can chip number
|                   - interrupt ID
| RETURN VALUES:    none
| DESCRIPTION:      - full can transmit
****************************************************************************/
#if defined( MISRA_CHECK )
 /* suppress misra message about multiple return  */
# pragma PRQA_MESSAGES_OFF 2006,2001,2015
#endif
/* CODE CATEGORY 1 START*/
static void CanHL_TxConfirmation( CAN_HW_CHANNEL_CANTYPE_FIRST CanObjectHandle txObjHandle)
{
  CanObjectHandle       logTxObjHandle;
  CanTransmitHandle     txHandle;

 
#if defined( C_ENABLE_TRANSMIT_QUEUE )
  CanSignedTxHandle         queueElementIdx;    /* use as signed due to loop */
  CanSignedTxHandle         elementBitIdx;
  tCanQueueElementType             elem;
  CanDeclareGlobalInterruptOldStatus
#else
# if defined( C_ENABLE_CONFIRMATION_FLAG )
#  if C_SECURITY_LEVEL > 20
  CanDeclareGlobalInterruptOldStatus
#  endif
# endif
#endif


#if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION ) && \
    defined( C_ENABLE_TRANSMIT_QUEUE )
  vuint8 rc;
#endif



# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertInternal(canHwChannel < kCanNumberOfHwChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);
# endif



  logTxObjHandle = (CanObjectHandle)((vsintx)txObjHandle + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));

  #if !defined(C_ENABLE_TX_POLLING)
  if((vuint8)0xFF == txObjHandle)
  {
    goto finishCanHL_TxConfirmation;
  }
  #endif
  
  #if !defined(C_ENABLE_TX_POLLING) || (defined(C_ENABLE_TX_POLLING) && defined(C_ENABLE_INDIVIDUAL_POLLING))
  # if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  {
    CTIER &= (vuint8)~CanMailboxSelect[txObjHandle]; /* deactivate Tx interrupt, might redundant in case of individual polling but is faster without a check */
  }
  
  
  # endif
  
  
  #else
  # if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  canllTxStatusFlag[channel] &= (vuint8)~CanMailboxSelect[txObjHandle];
  # endif
  #endif
  
  #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  CTBSEL = CanMailboxSelect[txObjHandle]; /* ESCAN00023651, make the mailbox visible that transmitted */
  #endif

  txHandle = canHandleCurTxObj[logTxObjHandle];           /* get saved handle */

  /* check associated transmit handle */
  if (txHandle == kCanBufferFree)
  {
    assertInternal (0, channel, kErrorTxHandleWrong);          /*lint !e506 !e774*/
    goto finishCanHL_TxConfirmation;
  }  

#if defined( C_ENABLE_TX_OBSERVE ) || \
    defined( C_ENABLE_CAN_TX_CONF_FCT )
# if defined( C_ENABLE_CANCEL_IN_HW )
  # if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  /* only if there is not HW cancellation acknowledgement */
  if((CTAAK & CanMailboxSelect[txObjHandle]) == 0)
  # endif
  
# endif
  {
# if defined( C_ENABLE_TX_OBSERVE )
    {
      ApplCanTxObjConfirmed( CAN_CHANNEL_CANPARA_FIRST logTxObjHandle );
    }
# endif

# if defined( C_ENABLE_CAN_TX_CONF_FCT )
/* ##RI-1.10 Common Callbackfunction in TxInterrupt */
    txInfoStructConf[channel].Handle  = txHandle;
    /* in case of passive mode we do not need to modifiy the mailbox */
    APPL_CAN_TX_CONFIRMATION(&txInfoStructConf[channel]);
# endif
  }
#endif /* defined( C_ENABLE_TX_OBSERVE ) || defined( C_ENABLE_CAN_TX_CONF_FCT ) */

#if defined( C_ENABLE_TRANSMIT_QUEUE )
# if defined( C_ENABLE_MSG_TRANSMIT )
  if (txObjHandle != CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel))
  {
    canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;                 /* free msg object of FullCAN or LowLevel Tx obj. */
  }
# endif
#else
  canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;                   /* free msg object if queue is not used */
#endif

  if (txHandle != kCanBufferCancel)
  {
#if defined( C_ENABLE_MSG_TRANSMIT )
    if (txObjHandle == CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel))
    {
# if defined( C_ENABLE_MSG_TRANSMIT_CONF_FCT )
      APPL_CAN_MSGTRANSMITCONF( CAN_CHANNEL_CANPARA_ONLY );
# endif

      goto finishCanHL_TxConfirmation;
    }
#endif

#if defined( C_ENABLE_MULTI_ECU_PHYS )
    assertInternal(((CanTxIdentityAssignment[txHandle] & V_ACTIVE_IDENTITY_MSK) != (tVIdentityMsk)0 ), channel , kErrorInternalDisabledTxMessage);
#endif

#if defined( C_ENABLE_CONFIRMATION_FLAG )       /* set transmit ready flag  */
# if C_SECURITY_LEVEL > 20
    CanNestedGlobalInterruptDisable();
# endif
    CanConfirmationFlags._c[CanGetConfirmationOffset(txHandle)] |= CanGetConfirmationMask(txHandle);
# if C_SECURITY_LEVEL > 20
    CanNestedGlobalInterruptRestore();
# endif
#endif

#if defined( C_ENABLE_CONFIRMATION_FCT )
    {
      if ( CanGetApplConfirmationPtr(txHandle) != V_NULL )
      {
        (CanGetApplConfirmationPtr(txHandle))(txHandle);   /* call completion routine  */
      }
    }
#endif /* C_ENABLE_CONFIRMATION_FCT */

  } /* end if kCanBufferCancel */
  
#if defined( C_ENABLE_TRANSMIT_QUEUE )
# if defined( C_ENABLE_MSG_TRANSMIT )
  if (txObjHandle == CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel)) 
# endif
  {
    CanNestedGlobalInterruptDisable();                /* ESCAN00008914 */

    for(queueElementIdx = CAN_HL_TXQUEUE_STOPINDEX(channel) - (CanSignedTxHandle)1; 
                             queueElementIdx >= CAN_HL_TXQUEUE_STARTINDEX(channel); queueElementIdx--)
    {
      elem = canTxQueueFlags[queueElementIdx];
      if(elem != (tCanQueueElementType)0) /* is there any flag set in the queue element? */
      {

        CanNestedGlobalInterruptRestore();

        /* start the bitsearch */
        #if defined( C_CPUTYPE_32BIT )
        if((elem & (tCanQueueElementType)0xFFFF0000) != (tCanQueueElementType)0)
        {
          if((elem & (tCanQueueElementType)0xFF000000) != (tCanQueueElementType)0)
          {
            if((elem & (tCanQueueElementType)0xF0000000) != (tCanQueueElementType)0)
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 28] + 28;
            }
            else /*0x0F000000*/
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 24] + 24;
            }
          }
          else
          {
            if((elem & (tCanQueueElementType)0x00F00000) != (tCanQueueElementType)0)
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 20] + 20;
            }
            else /* 0x000F0000*/
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 16] + 16;
            }
          }
        }
        else
        #endif
        {
        #if defined( C_CPUTYPE_32BIT ) || defined( C_CPUTYPE_16BIT )
          if((elem & (tCanQueueElementType)0x0000FF00) != (tCanQueueElementType)0)
          {
            if((elem & (tCanQueueElementType)0x0000F000) != (tCanQueueElementType)0)
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 12] + 12;
            }
            else /*0x00000F00*/
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 8] + 8;
            }
          }
          else
        #endif
          {
            if((elem & (tCanQueueElementType)0x000000F0) != (tCanQueueElementType)0)
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem >> 4] + 4;
            }
            else /* 0x0000000F*/
            {
              elementBitIdx = (CanSignedTxHandle)CanGetHighestFlagFromNibble[elem];
            }
          }
        }
        txHandle = ((CanTransmitHandle)queueElementIdx << kCanTxQueueShift) - CAN_HL_TXQUEUE_PADBITS(channel) + (CanTransmitHandle)elementBitIdx;
        {

            /* compute the logical message handle */
            CanNestedGlobalInterruptDisable();
 
            if ( (canTxQueueFlags[queueElementIdx] & CanShiftLookUp[elementBitIdx]) != (tCanQueueElementType)0 )
            {
              canTxQueueFlags[queueElementIdx] &= ~CanShiftLookUp[elementBitIdx]; /* clear flag from the queue */

              CanNestedGlobalInterruptRestore();  
              canHandleCurTxObj[logTxObjHandle] = txHandle;/* Save hdl of msgObj to be transmitted*/
# if defined( C_ENABLE_CAN_CANCEL_NOTIFICATION )
              rc = CanCopyDataAndStartTransmission( CAN_CHANNEL_CANPARA_FIRST txObjHandle,txHandle); 
              if ( rc == kCanTxNotify)
              {
                APPLCANCANCELNOTIFICATION(channel, txHandle);
              }
# else       /* C_ENABLE_CAN_CANCEL_NOTIFICATION */
              (void)CanCopyDataAndStartTransmission( CAN_CHANNEL_CANPARA_FIRST txObjHandle,txHandle); 
# endif /* C_ENABLE_CAN_CANCEL_NOTIFICATION */

              goto finishCanHL_TxConfirmation;
            }
            /* meanwhile, the queue is empty. E.g. due to CanOffline on higher level */
            canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;  /* free msg object if queue is empty */
            CanNestedGlobalInterruptRestore();  
            goto finishCanHL_TxConfirmation;
            
        }
        /* no entry found in Queue */
# if defined( CANHL_TX_QUEUE_BIT )
# else
        CanNestedGlobalInterruptDisable();                /*lint !e527 ESCAN00008914 */
                                                 /* unreachable in case of Bit-Queue */
# endif                                                 
      }
    }

    canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;  /* free msg object if queue is empty */
    CanNestedGlobalInterruptRestore();                 /* ESCAN00008914 */
  }
#endif
  /* check for next msg object in queue and transmit it */

/* Msg(4:2015) This label is not a case or default label for a switch statement. MISRA Rule 55 */
finishCanHL_TxConfirmation:

  #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  CTBSEL = (vuint8)((vuint8)~CTBSEL & kCanPendingTxObjMask); /* ESCAN00023651, make visible the mailbox that was probably changed (if it was not changed, this is not a risk) */
  #endif

  return;

} /* END OF CanTxInterrupt */
/* CODE CATEGORY 1 END*/
#if defined( MISRA_CHECK )
# pragma PRQA_MESSAGES_ON 2006,2001,2015
#endif


#if defined( C_ENABLE_ECU_SWITCH_PASS )
/************************************************************************
* NAME:               CanSetActive
* CALLED BY:          application
* PRECONDITIONS:      none
* PARAMETER:          none or channel
* RETURN VALUE:       none
* DESCRIPTION:        Set the CAN driver into active mode
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetActive( CAN_CHANNEL_CANTYPE_ONLY )
{
#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
#endif

  canPassive[channel] = 0;
} /* END OF CanSetActive */
/* CODE CATEGORY 4 END*/

/************************************************************************
* NAME:               CanSetPassive
* CALLED BY:          application
* PRECONDITIONS:      none
* PARAMETER:          none or channel
* RETURN VALUE:       none
* DESCRIPTION:        Set the can driver into passive mode
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetPassive( CAN_CHANNEL_CANTYPE_ONLY )
{
#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
#endif

  canPassive[channel] = 1;
 
# if defined( C_ENABLE_TRANSMIT_QUEUE )
  /* clear all Tx queue flags: */
  CanDelQueuedObj( CAN_CHANNEL_CANPARA_ONLY ); 
# endif
 

} /* END OF CanSetPassive */
/* CODE CATEGORY 4 END*/
#endif /* IF defined( C_ENABLE_ECU_SWITCH_PASS ) */


#if defined( C_ENABLE_OFFLINE )
/************************************************************************
* NAME:               CanOnline( CanChannelHandle channel )
* CALLED BY:          netmanagement
* PRECONDITIONS:      none
* PARAMETER:          none or channel
* RETURN VALUE:       none
* DESCRIPTION:        Switch on transmit path
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanOnline(CAN_CHANNEL_CANTYPE_ONLY)
{
  CanDeclareGlobalInterruptOldStatus

#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
#endif

  CanNestedGlobalInterruptDisable();

  canStatus[channel] |= kCanTxOn;

# if defined( C_ENABLE_ONLINE_OFFLINE_CALLBACK_FCT )
  APPL_CAN_ONLINE(CAN_CHANNEL_CANPARA_ONLY);
# endif
  CanNestedGlobalInterruptRestore();

}
/* CODE CATEGORY 4 END*/


/************************************************************************
* NAME:               CanOffline( CanChannelHandle channel )
* CALLED BY:          netmanagement
* PRECONDITIONS:      none
* PARAMETER:          none or channel
* RETURN VALUE:       none
* DESCRIPTION:        Switch off transmit path
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanOffline(CAN_CHANNEL_CANTYPE_ONLY)
{
  CanDeclareGlobalInterruptOldStatus

#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
#endif

  CanNestedGlobalInterruptDisable();

  canStatus[channel] &= kCanTxNotOn;

# if defined( C_ENABLE_ONLINE_OFFLINE_CALLBACK_FCT )
  APPL_CAN_OFFLINE(CAN_CHANNEL_CANPARA_ONLY);
# endif
  CanNestedGlobalInterruptRestore();

# if defined( C_ENABLE_TRANSMIT_QUEUE )
  CanDelQueuedObj( CAN_CHANNEL_CANPARA_ONLY );
# endif

}
/* CODE CATEGORY 4 END*/
#endif /* if defined( C_ENABLE_OFFLINE ) */


#if defined( C_ENABLE_PART_OFFLINE )
/************************************************************************
* NAME:               CanSetPartOffline
* CALLED BY:          application
* PRECONDITIONS:      none
* PARAMETER:          (channel), sendGroup
* RETURN VALUE:       none
* DESCRIPTION:        Switch partial off transmit path
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetPartOffline(CAN_CHANNEL_CANTYPE_FIRST vuint8 sendGroup)
{
  CanDeclareGlobalInterruptOldStatus

#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
#endif

  CanNestedGlobalInterruptDisable();
  canTxPartOffline[channel] |= sendGroup;   /* set offlinestate and Save for use of CanOnline/CanOffline */
  CanNestedGlobalInterruptRestore();
}
/* CODE CATEGORY 4 END*/


/************************************************************************
* NAME:               CanSetPartOnline
* CALLED BY:          application
* PRECONDITIONS:      none
* PARAMETER:          (channel), invSendGroup
* RETURN VALUE:       none
* DESCRIPTION:        Switch partial on transmit path
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetPartOnline(CAN_CHANNEL_CANTYPE_FIRST vuint8 invSendGroup)
{
  CanDeclareGlobalInterruptOldStatus

#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
#endif

  CanNestedGlobalInterruptDisable();
  canTxPartOffline[channel] &= invSendGroup;
  CanNestedGlobalInterruptRestore();
}
/* CODE CATEGORY 4 END*/


/************************************************************************
* NAME:               CanGetPartMode
* CALLED BY:          application
* PRECONDITIONS:      none
* PARAMETER:          none or channel
* RETURN VALUE:       canTxPartOffline
* DESCRIPTION:        return status of partoffline-Mode
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanGetPartMode(CAN_CHANNEL_CANTYPE_ONLY)
{
#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
#endif

  return canTxPartOffline[channel];
}
/* CODE CATEGORY 4 END*/
#endif


/****************************************************************************
| NAME:             CanGetStatus
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none or channel
| RETURN VALUES:    Bit coded status (more than one status can be set):
|                   kCanTxOn
|                   kCanHwIsStop        
|                   kCanHwIsInit        
|                   kCanHwIsInconsistent
|                   kCanHwIsWarning     
|                   kCanHwIsPassive     
|                   kCanHwIsBusOff      
|                   kCanHwIsSleep       
| DESCRIPTION:      returns the status of the transmit path and the CAN hardware.
|                   Only one of the statusbits Sleep,Busoff,Passiv,Warning is set.
|                   Sleep has the highest priority, error warning the lowerst. 
****************************************************************************/
#if defined( MISRA_CHECK )
 /* suppress misra message about multiple return  */
# pragma PRQA_MESSAGES_OFF 2006
#endif
/* CODE CATEGORY 3 START*/
C_API_1 vuint8 C_API_2 CanGetStatus( CAN_CHANNEL_CANTYPE_ONLY )
{
#if defined( C_ENABLE_EXTENDED_STATUS )
#endif


#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
#endif

#if defined( C_ENABLE_EXTENDED_STATUS )

# if defined( C_ENABLE_SLEEP_WAKEUP )
  /***************************** verify Sleep mode *************************************/
  if ( CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY)   )  { return ( VUINT8_CAST ( canStatus[channel] | kCanHwIsSleep ) ); }

# endif

  /***************************** verify Stop mode *************************************/
  if ( CanLL_HwIsStop(CAN_HW_CHANNEL_CANPARA_ONLY)    )  { return ( VUINT8_CAST ( canStatus[channel] | kCanHwIsStop ) ); }

  /***************************** verify Busoff *************************************/
  if ( CanLL_HwIsBusOff(CAN_HW_CHANNEL_CANPARA_ONLY)  )  { return (( VUINT8_CAST  canStatus[channel] | kCanHwIsBusOff ) ); }

  /***************************** verify Error Passiv *******************************/
  {
    if ( CanLL_HwIsPassive(CAN_HW_CHANNEL_CANPARA_ONLY) )  { return ( VUINT8_CAST ( canStatus[channel] | kCanHwIsPassive ) ); }
  }

  /***************************** verify Error Warning ******************************/
  {
    if ( CanLL_HwIsWarning(CAN_HW_CHANNEL_CANPARA_ONLY) )  { return ( VUINT8_CAST ( canStatus[channel] | kCanHwIsWarning ) ); }
  }
#endif
  return ( VUINT8_CAST (canStatus[channel] & kCanTxOn) );

} /* END OF CanGetStatus */
/* CODE CATEGORY 3 END*/
#if defined( MISRA_CHECK )
# pragma PRQA_MESSAGES_ON 2006
#endif


/****************************************************************************
| NAME:             CanSleep
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none or channel
| RETURN VALUES:    kCanOk, if CanSleep was successfull
|                   kCanFailed, if function failed
|                   kCanNotSupported, if this function is not supported
| DESCRIPTION:      disable CAN 
****************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanSleep(CAN_CHANNEL_CANTYPE_ONLY)
{
#if defined( C_ENABLE_SLEEP_WAKEUP )
  vuint8         canReturnCode;
#endif

#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
#endif
  assertUser((canCanInterruptCounter[channel] == (vsintx)0), channel, kErrorDisabledCanInt);

#if defined( C_ENABLE_COND_RECEIVE_FCT )
  /* this has to be done, even if SLEEP_WAKEUP is not enabled */
  canMsgCondRecState[channel] = kCanTrue;
#endif

#if defined( C_ENABLE_SLEEP_WAKEUP )

  assertUser((canStatus[channel] & kCanTxOn) != kCanTxOn, channel, kErrorCanOnline);

  {
    assertUser(!CanLL_HwIsStop(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanStop);
    {
    
      CTL0 |= SLPRQ;    /* set sleep request bit */
      APPLCANTIMERSTART(kCanLoopCanSleep);
      while(((CTL0 & SLPRQ) == SLPRQ) && ((CTL1 & SLPAK) != SLPAK) && APPLCANTIMERLOOP(kCanLoopCanSleep))
      {
        ; /* wait until CAN controller sets the sleep acknowledge flag */
      } 
      APPLCANTIMEREND(kCanLoopCanSleep);
    
      if(SLPAK == (CTL1 & SLPAK)) /* check if sleep request was successful */
      {
        canReturnCode = kCanOk;
      }
      else
      {
        canReturnCode = kCanFailed;
      }
    }
  }
  return canReturnCode;
#else
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && \
     defined( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;
# endif
  return kCanNotSupported;
#endif
} /* END OF CanSleep */
/* CODE CATEGORY 4 END*/

/****************************************************************************
| NAME:             CanWakeUp
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none or channel
| RETURN VALUES:    kCanOk, if CanWakeUp was successfull
|                   kCanFailed, if function failed
|                   kCanNotSupported, if this function is not supported
| DESCRIPTION:      enable CAN 
****************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanWakeUp( CAN_CHANNEL_CANTYPE_ONLY )
{
#if defined( C_ENABLE_SLEEP_WAKEUP )
  vuint8         canReturnCode;


#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
#endif
  assertUser((canCanInterruptCounter[channel] == (vsintx)0), channel, kErrorDisabledCanInt);

  {
    {
      CTL0 &= (vuint8)~SLPRQ; /* clear sleep request */
    
      APPLCANTIMERSTART(kCanLoopCanWakeup);
      while(((CTL1 & SLPAK) == SLPAK) && APPLCANTIMERLOOP(kCanLoopCanWakeup))
      { 
        ; /* wait until CAN controller is awake */
      } 
      APPLCANTIMEREND(kCanLoopCanWakeup);
    
      if(SLPAK == (CTL1 & SLPAK))
      {
        canReturnCode = kCanFailed; /* CAN controller still sleeping */
      }
      else
      {
        canReturnCode = kCanOk;
      }
    }
  }
  return canReturnCode;
#else
# if defined( C_MULTIPLE_RECEIVE_CHANNEL ) && \
     defined( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;
# endif
  return kCanNotSupported;
#endif /* C_ENABLE_SLEEP_WAKEUP */
} /* END OF CanWakeUp */
/* CODE CATEGORY 4 END*/


#if defined( C_ENABLE_STOP )
/****************************************************************************
| NAME:             CanStop
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    kCanOk, if success
|                   kCanFailed, if function failed
|                   kCanNotSupported, if this function is not supported
| DESCRIPTION:      stop CAN-controller
****************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanStop( CAN_CHANNEL_CANTYPE_ONLY )
{
  vuint8         canReturnCode;

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
# endif
  assertUser((canStatus[channel] & kCanTxOn) != kCanTxOn, channel, kErrorCanOnline);

  {
# if defined( C_ENABLE_SLEEP_WAKEUP )
    assertUser(!CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanSleep);
# endif
    {
      /* request init mode */
      CTL0 = INITRQ;
      APPLCANTIMERSTART(kCanLoopInitReq);
      while( ((CTL1 & INITAK) != INITAK) && APPLCANTIMERLOOP(kCanLoopInitReq) )
      { 
        ; /* wait while not in initmode */
      } 
      APPLCANTIMEREND(kCanLoopInitReq);
    
    
      if(INITAK == (CTL1 & INITAK))
      {
        canReturnCode = kCanOk;
        canStatus[channel] |= kCanHwIsStop;
      }
      else
      {
        canReturnCode = kCanFailed;
      }
    }
  }
  return canReturnCode;
}
/* CODE CATEGORY 4 END*/

/****************************************************************************
| NAME:             CanStart
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    kCanOk, if success
|                   kCanFailed, if function failed
|                   kCanNotSupported, if this function is not supported
| DESCRIPTION:      restart CAN-controller
****************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 vuint8 C_API_2 CanStart( CAN_CHANNEL_CANTYPE_ONLY )
{
  vuint8         canReturnCode;

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
# endif

  {
    CanInit(CAN_CHANNEL_CANPARA_FIRST lastInitObject[channel]);
    canStatus[channel] &= (vuint8)~kCanHwIsStop;
    canReturnCode = kCanOk;
  }
  return canReturnCode;
}
/* CODE CATEGORY 4 END*/
#endif /* if defined( C_ENABLE_STOP ) */


/****************************************************************************
| NAME:             CanCanInterruptDisable
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      disables CAN interrupts and stores old interrupt status
****************************************************************************/
/* CODE CATEGORY 1 START*/
C_API_1 void C_API_2 CanCanInterruptDisable( CAN_CHANNEL_CANTYPE_ONLY )
{
  CanDeclareGlobalInterruptOldStatus
#if defined( C_HL_ENABLE_CAN_IRQ_DISABLE )
#endif

  /* local variable must reside on stack or registerbank, switched */
  /* in interrupt level                                            */
  /* disable global interrupt                                      */
#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
#endif
  assertUser(canCanInterruptCounter[channel]<(vsint8)0x7f, kCanAllChannels, kErrorIntDisableTooOften);
#if defined( C_ENABLE_SLEEP_WAKEUP )
  assertUser(!CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanSleep);
#endif

  CanNestedGlobalInterruptDisable();
  if (canCanInterruptCounter[channel] == (vsintx)0)  /* if 0 then save old interrupt status */
  {
#if defined( C_HL_ENABLE_CAN_IRQ_DISABLE )
    {
      CanLL_CanInterruptDisable(canHwChannel, &canCanInterruptOldStatus[canHwChannel]);
    }
#endif
#if defined( C_ENABLE_INTCTRL_ADD_CAN_FCT )
    ApplCanAddCanInterruptDisable(channel);
#endif
  }
  canCanInterruptCounter[channel]++;               /* common for all platforms */

  CanNestedGlobalInterruptRestore();

} /* END OF CanCanInterruptDisable */
/* CODE CATEGORY 1 END*/

/****************************************************************************
| NAME:             CanCanInterruptRestore
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      restores the old interrupt status of the CAN interrupt if 
|                   canCanInterruptCounter[channel] is zero
****************************************************************************/
/* CODE CATEGORY 1 START*/
C_API_1 void C_API_2 CanCanInterruptRestore( CAN_CHANNEL_CANTYPE_ONLY )
{
  CanDeclareGlobalInterruptOldStatus
#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
#endif
  assertUser(canCanInterruptCounter[channel]>(vsintx)0, kCanAllChannels, kErrorIntRestoreTooOften);
#if defined( C_ENABLE_SLEEP_WAKEUP )
  assertUser(!CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanSleep);
#endif

  CanNestedGlobalInterruptDisable();
  /* restore CAN interrupt */
  canCanInterruptCounter[channel]--;

  if (canCanInterruptCounter[channel] == (vsintx)0)         /* restore interrupt if canCanInterruptCounter=0 */
  {
#if defined( C_HL_ENABLE_CAN_IRQ_DISABLE )    
    {
      CanLL_CanInterruptRestore(canHwChannel, canCanInterruptOldStatus[canHwChannel]);
    }
#endif

#if defined( C_ENABLE_INTCTRL_ADD_CAN_FCT )
    ApplCanAddCanInterruptRestore(channel);
#endif
  }
  CanNestedGlobalInterruptRestore();
} /* END OF CanCanInterruptRestore */
/* CODE CATEGORY 1 END*/

#if defined( C_ENABLE_MSG_TRANSMIT )
/************************************************************************
* NAME:               CanMsgTransmit
* CALLED BY:          CanReceivedFunction
* PRECONDITIONS:      Called in Receive Interrupt
* PARAMETER:          Pointer to message buffer data block; This can either be
*                     a RAM structure data block or the receive buffer in the
*                     CAN chip
* RETURN VALUE:       The return value says that a transmit request was successful
*                     or not
* DESCRIPTION:        Transmit functions for gateway issues (with dynamic
*                     messages). If the transmit buffer is not free, the message
*                     is inserted in the FIFO ring buffer.
*************************************************************************/
# if defined( MISRA_CHECK )
  /* suppress misra message about multiple return  */
#  pragma PRQA_MESSAGES_OFF 2006,2015
# endif
/* Msg(4:3673) The object addressed by the pointer "txMsgStruct" is not modified in this function.
   The use of "const" should be considered to indicate that it never changes. MISRA Rule 81 - no change */
/* CODE CATEGORY 2 START*/
C_API_1 vuint8 C_API_2 CanMsgTransmit( CAN_CHANNEL_CANTYPE_FIRST tCanMsgTransmitStruct *txMsgStruct )
{
  CanDeclareGlobalInterruptOldStatus
  vuint8                 rc;
  CanObjectHandle          txObjHandle;
  CanObjectHandle          logTxObjHandle;

  #if defined(C_ENABLE_COPY_TX_DATA)
  vsintx i;
  vuint8* pDestData;
  vuint8* pSrcData;
  #endif

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
# endif


  CanNestedGlobalInterruptDisable();

  /* --- test on CAN transmit switch --- */
  if ( (canStatus[channel] & kCanTxOn) != kCanTxOn )                /* transmit path switched off */
  {
    CanNestedGlobalInterruptRestore();
    return kCanTxFailed;
  }

# if defined( C_ENABLE_CAN_RAM_CHECK )
  if(canComStatus[channel] == kCanDisableCommunication)
  {
    CanNestedGlobalInterruptRestore();
    return(kCanCommunicationDisabled);
  }
# endif

# if defined( C_ENABLE_SLEEP_WAKEUP )
  assertUser(!CanLL_HwIsSleep(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanSleep);
# endif
  assertUser(!CanLL_HwIsStop(CAN_HW_CHANNEL_CANPARA_ONLY), channel, kErrorCanStop);
  
  /* --- check on passive state --- */
# if defined( C_ENABLE_ECU_SWITCH_PASS )
  if ( canPassive[channel] != (vuint8)0)                             /*  set passive ? */
  {
    CanNestedGlobalInterruptRestore();
#  if defined( C_ENABLE_MSG_TRANSMIT_CONF_FCT )
    APPL_CAN_MSGTRANSMITCONF( CAN_CHANNEL_CANPARA_ONLY );
#  endif
    return (kCanTxOk);
  }
# endif /* C_ENABLE_ECU_SWITCH_PASS */

  /* calculate index for canhandleCurTxObj (logical object handle) */
  logTxObjHandle = (CanObjectHandle)((vsintx)CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel));

  /* check for transmit message object free ---------------------------------*/
  /* MsgObj used?  */
  if (( canHandleCurTxObj[logTxObjHandle] != kCanBufferFree ))    
  {
    CanNestedGlobalInterruptRestore();
    return kCanTxFailed;
  }

  /* Obj, pMsgObject points to is free, transmit msg object: ----------------*/
  /* Save hdl of msgObj to be transmitted*/
  canHandleCurTxObj[logTxObjHandle] = kCanBufferMsgTransmit;
  CanNestedGlobalInterruptRestore();



  assertHardware( CanLL_TxIsHWObjFree(canHwChannel, CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel) ), channel, kErrorTxBufferBusy);

  txObjHandle = CAN_HL_HW_MSG_TRANSMIT_INDEX(canHwChannel);
  CanBeginCriticalXGateSection();
  #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
  CTBSEL = CanMailboxSelect[txObjHandle]; /* select the appropriate Tx buffer */
  #endif
  
  CanEndCriticalXGateSection();

  CanNestedGlobalInterruptDisable();
  /* Copy all data into transmit object */


  /* If CanTransmit was interrupted by a re-initialization or CanOffline */  
  /* no transmitrequest of this action should be started      */  
  if ((canHandleCurTxObj[logTxObjHandle] == kCanBufferMsgTransmit) && 
                                   ( (canStatus[channel] & kCanTxOn) == kCanTxOn ) )
  {  
     # if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
     (void)VStdMemCpyRamToRam((void*)CAN_TX_MAILBOX_BASIS_ADR(channel), (const void*)txMsgStruct, 13);
     CAN_TX_MAILBOX_BASIS_ADR(channel) -> Prio = TX2; /* Prio Std Tx > Prio LowLevel Tx */
     #endif

     #if defined(C_ENABLE_TX_POLLING)
     canllTxStatusFlag[channel] |= TX2;
     #endif
     CanBeginCriticalXGateSection();
     CANTFLG  = CanMailboxSelect[txObjHandle]; /* transmit message */
     CTBSEL = TX1; /* ESCAN00022867, the assignment is relevant if low-level transmission interrupts normal transmission */
     #if defined (C_ENABLE_INDIVIDUAL_POLLING) || (!defined (C_ENABLE_INDIVIDUAL_POLLING) && !defined(C_ENABLE_TX_POLLING))
     # if defined ( C_ENABLE_INDIVIDUAL_POLLING )
     if (CanHwObjIndivPolling[CAN_HWOBJINDIVPOLLING_INDEX0][txObjHandle] == (vuint8)0x00)
     # endif
     /* in case of disabled Tx Irq the request needs to be stored in the old status variable */
     {
       if(canCanIrqDisabled[channel] == kCanTrue)
       {
         canCanInterruptOldStatus[channel].oldCanCTIER |= CanMailboxSelect[txObjHandle]; /* enable Tx interrupt */
       }
       else
       {
         CTIER |= CanMailboxSelect[txObjHandle]; /* enable Tx interrupt */
       }
     }
     #endif
     CanEndCriticalXGateSection();

# if defined( C_ENABLE_TX_OBSERVE )
     ApplCanTxObjStart( CAN_CHANNEL_CANPARA_FIRST logTxObjHandle );
# endif
   
     rc = kCanTxOk;                                    
  }  
  else  
  {  
    /* release TxHandle (CanOffline) */
    canHandleCurTxObj[logTxObjHandle] = kCanBufferFree;  
    rc = kCanTxFailed;   
  }  

  CanNestedGlobalInterruptRestore();


  return rc;
} /*end of CanMsgTransmit() */
/* CODE CATEGORY 2 END*/
# if defined( MISRA_CHECK )
#  pragma PRQA_MESSAGES_ON 2006,2015
# endif
#endif


#if defined( C_ENABLE_DYN_TX_OBJECTS )
/************************************************************************
* NAME:           CanGetDynTxObj
* PARAMETER:      txHandle - Handle of the dynamic object to reserve
* RETURN VALUE:   kCanNoTxDynObjAvailable (0xFF) - 
*                   object not available
*                 0..F0 - 
*                   Handle to dynamic transmission object
* DESCRIPTION:    Function reserves and return a handle to a dynamic 
*                   transmission object
*
*                 To use dynamic transmission, an application must get
*                 a dynamic object from CAN-driver. 
*                 Before transmission, application must set all attributes 
*                 (id, dlc, data, confirmation function/flag, pretransmission
*                 etc. - as configurated). 
*                 Application can use a dynamic object for one or many
*                 transmissions (as it likes) - but finally, it must
*                 release the dynamic object by calling CanReleaseDynTxObj.
*************************************************************************/
# if defined( MISRA_CHECK )
  /* suppress misra message about multiple return  */
#  pragma PRQA_MESSAGES_OFF 2006
# endif
/* CODE CATEGORY 3 START*/
C_API_1 CanTransmitHandle C_API_2 CanGetDynTxObj(CanTransmitHandle txHandle )
{
  CanTransmitHandle nTxDynObj;
  CanDeclareGlobalInterruptOldStatus
  CAN_CHANNEL_CANTYPE_LOCAL
  
  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  channel = CanGetChannelOfTxObj(txHandle);
# endif

  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);  /*lint !e568 */

  nTxDynObj = txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);

  CanNestedGlobalInterruptDisable();
  if ( canTxDynObjReservedFlag[nTxDynObj] != (vuint8)0)
  {
    CanNestedGlobalInterruptRestore();
    return kCanNoTxDynObjAvailable;
  }
  /*  Mark dynamic object as used  */
  canTxDynObjReservedFlag[nTxDynObj] = 1;

# if defined( C_ENABLE_CONFIRMATION_FLAG )
  CanConfirmationFlags._c[CanGetConfirmationOffset(txHandle)] &= 
                            (vuint8)(~CanGetConfirmationMask(txHandle));
# endif
  CanNestedGlobalInterruptRestore();

  /* Initialize dynamic object */
# if defined( C_ENABLE_DYN_TX_DATAPTR )  
  canDynTxDataPtr[nTxDynObj] = V_NULL;  
# endif  
  

  return (txHandle);
}
/* CODE CATEGORY 3 END*/
# if defined( MISRA_CHECK )
#  pragma PRQA_MESSAGES_ON 2006
# endif

/************************************************************************
* NAME:           CanReleaseDynTxObj
* PARAMETER:      hTxObj -
*                   Handle of dynamic transmission object
* RETURN VALUE:   --
* DESCRIPTION:    Function releases dynamic transmission object
*                   which was reserved before (calling CanGetDynTxObj)
*                 
*                 After a transmission of one or more messages is finished,
*                 application must free the reserved resource, formally the
*                 dynamic transmission object calling this function.
*
*                 As the number of dynamic transmission object is limited,
*                 application should not keep unused dynamic transmission
*                 objects for a longer time.
*************************************************************************/
# if defined( MISRA_CHECK )
  /* suppress misra message about multiple return  */
#  pragma PRQA_MESSAGES_OFF 2006
# endif
/* CODE CATEGORY 3 START*/
C_API_1 vuint8 C_API_2 CanReleaseDynTxObj(CanTransmitHandle txHandle)
{
  CanTransmitHandle dynTxObj;
  CAN_CHANNEL_CANTYPE_LOCAL
# if defined( C_ENABLE_TRANSMIT_QUEUE )
  CanSignedTxHandle queueElementIdx; /* index for accessing the tx queue */
  CanSignedTxHandle elementBitIdx;  /* bit index within the tx queue element */
  CanTransmitHandle queueBitPos;  /* physical bitposition of the handle */
# endif
  
  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  channel = CanGetChannelOfTxObj(txHandle);
# endif

  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);  /*lint !e568 */

  dynTxObj = txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);

  assertInternal((canTxDynObjReservedFlag[dynTxObj] != (vuint8)0), channel, kErrorReleasedUnusedDynObj);

# if defined( C_ENABLE_TRANSMIT_QUEUE )
  #if defined( C_ENABLE_INTERNAL_CHECK ) &&\
      defined( C_MULTIPLE_RECEIVE_CHANNEL )
  if (sizeof(queueBitPos == 1))
  {
    assertInternal( ((kCanNumberOfTxObjects + CanTxQueuePadBits[kCanNumberOfChannels-1]) <= 256u), kCanAllChannels, kErrorTxQueueTooManyHandle) /*lint !e572 !e506*/
  }
  else
  {
    assertInternal( ((kCanNumberOfTxObjects + CanTxQueuePadBits[kCanNumberOfChannels-1]) <= 65536u), kCanAllChannels, kErrorTxQueueTooManyHandle) /*lint !e572 !e506*/
  }
  #endif
  queueBitPos  = txHandle + CAN_HL_TXQUEUE_PADBITS(channel);
  queueElementIdx = (CanSignedTxHandle)(queueBitPos >> kCanTxQueueShift); /* get the queue element where to set the flag */
  elementBitIdx  = (CanSignedTxHandle)(queueBitPos & kCanTxQueueMask);   /* get the flag index wihtin the queue element */
  if( (canTxQueueFlags[queueElementIdx] & CanShiftLookUp[elementBitIdx]) != (tCanQueueElementType)0 )
  {
  }
  else
# endif
  {
    if (
# if defined( C_ENABLE_CONFIRMATION_FCT ) && \
    defined( C_ENABLE_TRANSMIT_QUEUE )
         (confirmHandle[channel] == txHandle) ||       /* confirmation active ? */
# endif
         (canHandleCurTxObj[(vsintx)CAN_HL_HW_TX_NORMAL_INDEX(canHwChannel) + CAN_HL_TX_OFFSET_HW_TO_LOG(canHwChannel)] != txHandle) )
    {
      /*  Mark dynamic object as not used  */
      canTxDynObjReservedFlag[dynTxObj] = 0;
      return(kCanDynReleased);
    }
  }
  return(kCanDynNotReleased);
}
/* CODE CATEGORY 3 END*/
# if defined( MISRA_CHECK )
#  pragma PRQA_MESSAGES_ON 2006
# endif
#endif /* C_ENABLE_DYN_TX_OBJECTS */


#if defined( C_ENABLE_DYN_TX_ID ) 
# if !defined( C_ENABLE_EXTENDED_ID ) ||\
     defined( C_ENABLE_MIXED_ID )
/************************************************************************
* NAME:           CanDynTxObjSetId
* PARAMETER:      hTxObj -
*                   Handle of dynamic transmission object
*                 id -
*                   Id (standard-format) to register with dynamic object
* RETURN VALUE:   --
* DESCRIPTION:    Function registers submitted id (standard format)
*                 with dynamic object referenced by handle.
*************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanDynTxObjSetId(CanTransmitHandle txHandle, vuint16 id)
{
  CanTransmitHandle dynTxObj;

  CAN_CHANNEL_CANTYPE_LOCAL
  
  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);

#  if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  channel = CanGetChannelOfTxObj(txHandle);
#  endif

  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);  /*lint !e568 */
  assertUser(id <= (vuint16)0x7FF, channel, kErrorWrongId);

  dynTxObj = txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);

#  if defined( C_ENABLE_MIXED_ID )
#   if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#   else
  canDynTxIdType[dynTxObj]  = kCanIdTypeStd; 
#   endif
#  endif

  canDynTxId0[dynTxObj] = MK_STDID0(id);
#  if (kCanNumberOfUsedCanTxIdTables > 1)
  canDynTxId1[dynTxObj] = MK_STDID1(id);
#  endif
#  if (kCanNumberOfUsedCanTxIdTables > 2)
  canDynTxId2[dynTxObj] = MK_STDID2(id);
#  endif
#  if (kCanNumberOfUsedCanTxIdTables > 3)
  canDynTxId3[dynTxObj] = MK_STDID3(id);
#  endif
#  if (kCanNumberOfUsedCanTxIdTables > 4)
  canDynTxId4[dynTxObj] = MK_STDID4(id);
#  endif
}
/* CODE CATEGORY 2 END*/
# endif /* !defined( C_ENABLE_EXTENDED_ID ) || defined( C_ENABLE_MIXED_ID ) */
#endif /* C_ENABLE_DYN_TX_ID */

#if defined( C_ENABLE_DYN_TX_ID ) && \
    defined( C_ENABLE_EXTENDED_ID )
/************************************************************************
* NAME:           CanDynTxObjSetExtId
* PARAMETER:      hTxObj -  Handle of dynamic transmission object
*                 idExtHi - Id low word (extended-format) to register with
*                                                         dynamic object
*                 idExtLo - Id high word (extended-format) 
* RETURN VALUE:   --
* DESCRIPTION:    Function registers submitted id (standard format)
*                 with dynamic object referenced by handle.
*************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanDynTxObjSetExtId(CanTransmitHandle txHandle, vuint16 idExtHi, vuint16 idExtLo)
{
  CanTransmitHandle dynTxObj;
  CAN_CHANNEL_CANTYPE_LOCAL
  
  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  channel = CanGetChannelOfTxObj(txHandle);
# endif

  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);  /*lint !e568 */
  assertUser(idExtHi <= (vuint16)0x1FFF, channel, kErrorWrongId);

  dynTxObj = txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);

# if defined( C_ENABLE_MIXED_ID )
#  if defined( C_HL_ENABLE_IDTYPE_IN_ID )
#  else
  canDynTxIdType[dynTxObj] = kCanIdTypeExt; 
#  endif
# endif

  canDynTxId0[dynTxObj]      = MK_EXTID0( ((vuint32)idExtHi<<16) | (vuint32)idExtLo );
# if (kCanNumberOfUsedCanTxIdTables > 1)
  canDynTxId1[dynTxObj]      = MK_EXTID1( ((vuint32)idExtHi<<16) | (vuint32)idExtLo );
# endif
# if (kCanNumberOfUsedCanTxIdTables > 2)
  canDynTxId2[dynTxObj]      = MK_EXTID2( ((vuint32)idExtHi<<16) | (vuint32)idExtLo );
# endif
# if (kCanNumberOfUsedCanTxIdTables > 3)
  canDynTxId3[dynTxObj]      = MK_EXTID3( ((vuint32)idExtHi<<16) | (vuint32)idExtLo );
# endif
# if (kCanNumberOfUsedCanTxIdTables > 4)
  canDynTxId4[dynTxObj]      = MK_EXTID4( ((vuint32)idExtHi<<16) | (vuint32)idExtLo );
# endif
}
/* CODE CATEGORY 2 END*/
#endif


#if defined( C_ENABLE_DYN_TX_DLC )
/************************************************************************
* NAME:           CanDynTxObjSetDlc
* PARAMETER:      hTxObj -
*                   Handle of dynamic transmission object
*                 dlc -
*                   data length code to register with dynamic object
* RETURN VALUE:   --
* DESCRIPTION:    Function registers data length code with 
*                 dynamic object referenced by submitted handle.
*************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanDynTxObjSetDlc(CanTransmitHandle txHandle, vuint8 dlc)
{
  CanTransmitHandle dynTxObj;
  CAN_CHANNEL_CANTYPE_LOCAL
  
  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  channel = CanGetChannelOfTxObj(txHandle);
# endif
  
  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);  /*lint !e568 */
  assertUser(dlc<(vuint8)9, channel, kErrorTxDlcTooLarge);
  
  dynTxObj = txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);
  
# if defined( C_ENABLE_EXTENDED_ID )
  canDynTxDLC[dynTxObj] = MK_TX_DLC_EXT(dlc);
# else
  canDynTxDLC[dynTxObj] = MK_TX_DLC(dlc);
# endif
}
/* CODE CATEGORY 2 END*/
#endif /* C_ENABLE_DYN_TX_DLC */


#if defined( C_ENABLE_DYN_TX_DATAPTR )
/************************************************************************
* NAME:           CanDynTxObjSetData
* PARAMETER:      hTxObj -
*                   Handle of dynamic transmission object
*                 pData -
*                   data reference to be stored in data buffer of dynamic object
* RETURN VALUE:   --
* DESCRIPTION:    Functions stores reference to data registered with
*                 dynamic object.
*                 
*                 The number of byte copied is (always) 8. The number of 
*                 relevant (and consequently evaluated) byte is to be 
*                 taken from function CanDynObjGetDLC.
*************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanDynTxObjSetDataPtr(CanTransmitHandle txHandle, void* pData)
{
  CanTransmitHandle dynTxObj;
  CAN_CHANNEL_CANTYPE_LOCAL
  
  assertUser((txHandle < kCanNumberOfTxObjects), kCanAllChannels, kErrorTxHdlTooLarge);

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  channel = CanGetChannelOfTxObj(txHandle);
# endif

  assertUser((txHandle <  CAN_HL_TX_DYN_ROM_STOPINDEX(channel)), channel, kErrorAccessedInvalidDynObj);
  assertUser((txHandle >= CAN_HL_TX_DYN_ROM_STARTINDEX(channel)), channel, kErrorAccessedStatObjAsDyn);  /*lint !e568 */

  dynTxObj = txHandle - CAN_HL_TX_DYN_ROM_STARTINDEX(channel) + CAN_HL_TX_DYN_RAM_STARTINDEX(channel);

  canDynTxDataPtr[dynTxObj] = pData;
}
/* CODE CATEGORY 2 END*/
#endif /* C_ENABLE_DYN_TX_DATAPTR */




#if defined( C_ENABLE_TX_MASK_EXT_ID )
/************************************************************************
* NAME:               CanSetTxIdExtHi
* CALLED BY:          
* PRECONDITIONS:      CanInitPower should already been called.
* PARAMETER:          new source address for the 29-bit CAN-ID
* RETURN VALUE:       -
* DESCRIPTION:        Sets the source address in the lower 8 bit of the
*                     29-bit CAN identifier.
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetTxIdExtHi( CAN_CHANNEL_CANTYPE_FIRST  vuint8 mask )
{  
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
# endif
  assertUser(mask <= (vuint8)0x1F, channel, kErrorWrongMask);

  canTxMask0[channel] = (canTxMask0[channel] & MK_EXTID0(0x00FFFFFFUL)) | MK_EXTID0((vuint32)mask<<24);
# if (kCanNumberOfUsedCanTxIdTables > 1)
  canTxMask1[channel] = (canTxMask1[channel] & MK_EXTID1(0x00FFFFFFUL)) | MK_EXTID1((vuint32)mask<<24);
# endif
# if (kCanNumberOfUsedCanTxIdTables > 2)
  canTxMask2[channel] = (canTxMask2[channel] & MK_EXTID2(0x00FFFFFFUL)) | MK_EXTID2((vuint32)mask<<24);
# endif
# if (kCanNumberOfUsedCanTxIdTables > 3)
  canTxMask3[channel] = (canTxMask3[channel] & MK_EXTID3(0x00FFFFFFUL)) | MK_EXTID3((vuint32)mask<<24);
# endif
# if (kCanNumberOfUsedCanTxIdTables > 4)
  canTxMask4[channel] = (canTxMask4[channel] & MK_EXTID4(0x00FFFFFFUL)) | MK_EXTID4((vuint32)mask<<24);
# endif
}
/* CODE CATEGORY 4 END*/

/************************************************************************
* NAME:               CanSetTxIdExtMidHi
* CALLED BY:          
* PRECONDITIONS:      CanInitPower should already been called.
* PARAMETER:          new source address for the 29-bit CAN-ID
* RETURN VALUE:       -
* DESCRIPTION:        Sets the source address in the lower 8 bit of the
*                     29-bit CAN identifier.
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetTxIdExtMidHi( CAN_CHANNEL_CANTYPE_FIRST  vuint8 mask )
{
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
# endif

  canTxMask0[channel] = (canTxMask0[channel] & MK_EXTID0(0xFF00FFFFUL)) | MK_EXTID0((vuint32)mask<<16);   /*lint !e572*/
# if (kCanNumberOfUsedCanTxIdTables > 1)
  canTxMask1[channel] = (canTxMask1[channel] & MK_EXTID1(0xFF00FFFFUL)) | MK_EXTID1((vuint32)mask<<16);   /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 2)
  canTxMask2[channel] = (canTxMask2[channel] & MK_EXTID2(0xFF00FFFFUL)) | MK_EXTID2((vuint32)mask<<16);   /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 3)
  canTxMask3[channel] = (canTxMask3[channel] & MK_EXTID3(0xFF00FFFFUL)) | MK_EXTID3((vuint32)mask<<16);   /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 4)
  canTxMask4[channel] = (canTxMask4[channel] & MK_EXTID4(0xFF00FFFFUL)) | MK_EXTID4((vuint32)mask<<16);   /*lint !e572*/
# endif
}
/* CODE CATEGORY 4 END*/

/************************************************************************
* NAME:               CanSetTxIdExtMidLo
* CALLED BY:          
* PRECONDITIONS:      CanInitPower should already been called.
* PARAMETER:          new source address for the 29-bit CAN-ID
* RETURN VALUE:       -
* DESCRIPTION:        Sets the source address in the lower 8 bit of the
*                     29-bit CAN identifier.
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetTxIdExtMidLo( CAN_CHANNEL_CANTYPE_FIRST  vuint8 mask )
{
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
# endif

  canTxMask0[channel] = (canTxMask0[channel] & MK_EXTID0(0xFFFF00FFUL)) | MK_EXTID0((vuint32)mask<<8);    /*lint !e572*/
# if (kCanNumberOfUsedCanTxIdTables > 1)
  canTxMask1[channel] = (canTxMask1[channel] & MK_EXTID1(0xFFFF00FFUL)) | MK_EXTID1((vuint32)mask<<8);    /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 2)
  canTxMask2[channel] = (canTxMask2[channel] & MK_EXTID2(0xFFFF00FFUL)) | MK_EXTID2((vuint32)mask<<8);    /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 3)
  canTxMask3[channel] = (canTxMask3[channel] & MK_EXTID3(0xFFFF00FFUL)) | MK_EXTID3((vuint32)mask<<8);    /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 4)
  canTxMask4[channel] = (canTxMask4[channel] & MK_EXTID4(0xFFFF00FFUL)) | MK_EXTID4((vuint32)mask<<8);    /*lint !e572*/
# endif
}
/* CODE CATEGORY 4 END*/

/************************************************************************
* NAME:               CanSetTxIdExtLo
* CALLED BY:          
* PRECONDITIONS:      CanInitPower should already been called.
* PARAMETER:          new source address for the 29-bit CAN-ID
* RETURN VALUE:       -
* DESCRIPTION:        Sets the source address in the lower 8 bit of the
*                     29-bit CAN identifier.
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanSetTxIdExtLo( CAN_CHANNEL_CANTYPE_FIRST  vuint8 mask )
{
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser(channel < kCanNumberOfChannels, kCanAllChannels, kErrorChannelHdlTooLarge);
# endif

  canTxMask0[channel] = (canTxMask0[channel] & MK_EXTID0(0xFFFFFF00UL)) | MK_EXTID0((vuint32)mask);     /*lint !e572*/
# if (kCanNumberOfUsedCanTxIdTables > 1)
  canTxMask1[channel] = (canTxMask1[channel] & MK_EXTID1(0xFFFFFF00UL)) | MK_EXTID1((vuint32)mask);     /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 2)
  canTxMask2[channel] = (canTxMask2[channel] & MK_EXTID2(0xFFFFFF00UL)) | MK_EXTID2((vuint32)mask);     /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 3)
  canTxMask3[channel] = (canTxMask3[channel] & MK_EXTID3(0xFFFFFF00UL)) | MK_EXTID3((vuint32)mask);     /*lint !e572*/
# endif
# if (kCanNumberOfUsedCanTxIdTables > 4)
  canTxMask4[channel] = (canTxMask4[channel] & MK_EXTID4(0xFFFFFF00UL)) | MK_EXTID4((vuint32)mask);    /*lint !e572*/
# endif
}
/* CODE CATEGORY 4 END*/
#endif

#if defined( C_ENABLE_TX_OBSERVE )
/************************************************************************
* NAME:               CanTxGetActHandle
* CALLED BY:          
* PRECONDITIONS:      
* PARAMETER:          logical hardware object handle
* RETURN VALUE:       handle of the message in the assigned mailbox
* DESCRIPTION:        get transmit handle of the message, which is currently
*                     in the mailbox txHwObject.
*************************************************************************/
/* CODE CATEGORY 3 START*/
C_API_1 CanTransmitHandle C_API_2 CanTxGetActHandle( CanObjectHandle logicalTxHdl )
{
  assertUser(logicalTxHdl < kCanNumberOfUsedTxCANObjects, kCanAllChannels, kErrorTxHwHdlTooLarge);

  return (canHandleCurTxObj[logicalTxHdl]);       /*lint !e661*/
}
/* CODE CATEGORY 3 END*/
#endif

#if defined( C_ENABLE_VARIABLE_RX_DATALEN )
/************************************************************************
* NAME:               CanSetVariableRxDatalen
* CALLED BY:          
* PRECONDITIONS:      
* PARAMETER:          rxHandle: Handle of receive Message for which the datalen has
*                               to be changed
*                     dataLen:  new number of bytes, which have to be copied to the 
*                               message buffer.
* RETURN VALUE:       -
* DESCRIPTION:        change the dataLen of a receive message to copy a 
*                     smaller number of bytes than defined in the database.
*                     the dataLen can only be decreased. If the parameter
*                     dataLen is bigger than the statically defined value
*                     the statically defined value will be set.
*************************************************************************/
/* CODE CATEGORY 1 START*/
static void CanSetVariableRxDatalen (CanReceiveHandle rxHandle, vuint8 dataLen)
{
  assertInternal(rxHandle < kCanNumberOfRxObjects, kCanAllChannels , kErrorRxHandleWrong);  /* legal rxHandle ? */
  /* assertion for dataLen not necessary due to runtime check */

  if (dataLen < CanGetRxDataLen(rxHandle))
  {
    canVariableRxDataLen[rxHandle]=dataLen;
  }
  else
  {
    canVariableRxDataLen[rxHandle] = CanGetRxDataLen(rxHandle);
  }
}
/* CODE CATEGORY 1 END*/
#endif

#if defined( C_ENABLE_COND_RECEIVE_FCT )
/************************************************************************
* NAME:               CanSetMsgReceivedCondition
* CALLED BY:          Application
* PRECONDITIONS:      
* PARAMETER:          -.
* RETURN VALUE:       -
* DESCRIPTION:        The service function CanSetMsgReceivedCondition()
*                     enables the calling of ApplCanMsgCondReceived()
*************************************************************************/
/* CODE CATEGORY 3 START*/
C_API_1 void C_API_2 CanSetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY )
{
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser((channel < kCanNumberOfChannels), kCanAllChannels, kErrorChannelHdlTooLarge);
# endif

  canMsgCondRecState[channel] = kCanTrue;
}
/* CODE CATEGORY 3 END*/

/************************************************************************
* NAME:               CanResetMsgReceivedCondition
* CALLED BY:          Application
* PRECONDITIONS:      
* PARAMETER:          -
* RETURN VALUE:       -
* DESCRIPTION:        The service function CanResetMsgReceivedCondition()
*                     disables the calling of ApplCanMsgCondReceived()
*************************************************************************/
/* CODE CATEGORY 3 START*/
C_API_1 void C_API_2 CanResetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY )
{
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser((channel < kCanNumberOfChannels), kCanAllChannels, kErrorChannelHdlTooLarge);
# endif

  canMsgCondRecState[channel] = kCanFalse;
}
/* CODE CATEGORY 3 END*/

/************************************************************************
* NAME:               CanGetMsgReceivedCondition
* CALLED BY:          Application
* PRECONDITIONS:      
* PARAMETER:          -
* RETURN VALUE:       status of Conditional receive function:
*                     kCanTrue : Condition is set -> ApplCanMsgCondReceived 
*                                will be called
*                     kCanFalse: Condition is not set -> ApplCanMsgCondReceived
*                                will not be called
* DESCRIPTION:        The service function CanGetMsgReceivedCondition() 
*                     returns the status of the condition for calling
*                     ApplCanMsgCondReceived()
*************************************************************************/
/* CODE CATEGORY 3 START*/
C_API_1 vuint8 C_API_2 CanGetMsgReceivedCondition( CAN_CHANNEL_CANTYPE_ONLY )
{
# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertUser((channel < kCanNumberOfChannels), kCanAllChannels, kErrorChannelHdlTooLarge);
# endif

  return( canMsgCondRecState[channel] );
}
/* CODE CATEGORY 3 END*/
#endif


#if defined( C_MULTIPLE_RECEIVE_CHANNEL )
/************************************************************************
* NAME:           ApplCanChannelDummy
* PARAMETER:      channel
*                 current receive channel
* RETURN VALUE:   ---
* DESCRIPTION:    dummy-function for unused Callback-functions
*************************************************************************/
/* CODE CATEGORY 3 START*/
C_API_1 void C_API_2 ApplCanChannelDummy( CanChannelHandle channel )
{
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
  channel = channel;     /* to avoid lint warnings */
# endif
}
/* CODE CATEGORY 3 END*/
#endif   /* C_MULTIPLE_RECEIVE_CHANNEL */


#if defined( C_MULTIPLE_RECEIVE_CHANNEL ) 
/************************************************************************
* NAME:           ApplCanRxStructPtrDummy
* PARAMETER:      rxStruct
*                 pointer of CanRxInfoStruct
* RETURN VALUE:   kCanCopyData 
* DESCRIPTION:    dummy-function for unused Callback-functions
*************************************************************************/
/* CODE CATEGORY 1 START*/
C_API_1 vuint8 C_API_2 ApplCanRxStructPtrDummy( CanRxInfoStructPtr rxStruct )
{
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
  rxStruct = rxStruct;     /* to avoid lint warnings */
# endif
  return kCanCopyData;
}
/* CODE CATEGORY 1 END*/

/************************************************************************
* NAME:           ApplCanTxHandleDummy
* PARAMETER:      txHandle
*                 transmit handle
* RETURN VALUE:   ---
* DESCRIPTION:    dummy-function for unused Callback-functions
*************************************************************************/
/* CODE CATEGORY 1 START*/
C_API_1 void C_API_2 ApplCanTxHandleDummy( CanTransmitHandle txHandle )
{
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
  txHandle = txHandle;     /* to avoid lint warnings */
# endif
}
/* CODE CATEGORY 1 END*/
#endif   /* C_MULTIPLE_RECEIVE_CHANNEL || C_HL_ENABLE_DUMMY_FCT_CALL */


#if defined( C_ENABLE_RX_QUEUE )
/************************************************************************
* NAME:               CanHL_ReceivedRxHandleQueue
* CALLED BY:          CanBasicCanMsgReceived, CanFullCanMsgReceived
* Preconditions:      none
* PARAMETER:          none
* RETURN VALUE:       none
* DESCRIPTION:        Writes receive data into queue or starts further
*                     processing for this message
*************************************************************************/
/* CODE CATEGORY 1 START*/
static vuint8 CanHL_ReceivedRxHandleQueue(CAN_CHANNEL_CANTYPE_ONLY)
{
  CanDeclareGlobalInterruptOldStatus
  tCanRxInfoStruct    *pCanRxInfoStruct;


# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
  assertInternal(channel < kCanNumberOfChannels, kCanAllChannels, kErrorInternalChannelHdlTooLarge);
# endif

  /* Rx Queue is supported with C_HL_ENABLE_RX_INFO_STRUCT_PTR only! */
  pCanRxInfoStruct =  &canRxInfoStruct[channel];

  /* if C_ENABLE_APPLCANPRERXQUEUE is not set, a macro ApplCanPreRxQueue has to be provided by the tool */
  /* in case of ranges, ApplCanPreRxQueue has to return kCanCopyData! */
# if defined( C_ENABLE_APPLCANPRERXQUEUE )
  if(ApplCanPreRxQueue(CAN_HL_P_RX_INFO_STRUCT(channel)) == kCanCopyData)
# endif
  {
    /* Disable the interrupts because nested interrupts can take place -
      CAN interrupts of all channels have to be disabled here*/
    CanNestedGlobalInterruptDisable();
    if(canRxQueue.canRxQueueCount < kCanRxQueueSize)   /* Queue full ? */
    {
      if (canRxQueue.canRxQueueWriteIndex == (kCanRxQueueSize - 1) )
      {
        canRxQueue.canRxQueueWriteIndex = 0;
      }
      else
      {
        canRxQueue.canRxQueueWriteIndex++;
      }
      canRxQueue.canRxQueueBuf[canRxQueue.canRxQueueWriteIndex].Channel = channel;
      canRxQueue.canRxQueueBuf[canRxQueue.canRxQueueWriteIndex].Handle  = pCanRxInfoStruct->Handle;

      #if defined(C_COMP_COSMIC_MCS12X_MSCAN12)  
      # if defined(C_SEARCH_XGATE)
      (void)VStdMemCpyRamToRam((void*)&canRxQueue.canRxQueueBuf[canRxQueue.canRxQueueWriteIndex].CanChipMsgObj, (void*)&(canRxTmpBuf[channel]), sizeof(tCanMsgTransmitStruct));
      # else
      (void)VStdMemCpyRamToRam((void*)&canRxQueue.canRxQueueBuf[canRxQueue.canRxQueueWriteIndex].CanChipMsgObj, (void*)CAN_RX_MAILBOX_BASIS_ADR(channel), sizeof(tCanMsgTransmitStruct));
      # endif
      #endif

      canRxQueue.canRxQueueCount++;
    }
# if defined( C_ENABLE_RXQUEUE_OVERRUN_NOTIFY )
    else
    {
      ApplCanRxQueueOverrun();
    }
# endif
    CanNestedGlobalInterruptRestore();
  } 
# if defined( C_ENABLE_APPLCANPRERXQUEUE )
  else
  {
    /* Call the application call-back functions and set flags */
#  if defined( C_ENABLE_RX_QUEUE_RANGE )
    if (pCanRxInfoStruct->Handle < kCanNumberOfRxObjects )
#  endif
    { 
      return CanHL_ReceivedRxHandle(CAN_CHANNEL_CANPARA_FIRST pCanRxInfoStruct);
    }
  }
# endif
  return kCanHlFinishRx;
}
/* CODE CATEGORY 1 END*/

/************************************************************************
* NAME:               CanHandleRxMsg
* CALLED BY:          Application
* Preconditions:      none
* PARAMETER:          none
* RETURN VALUE:       none
* DESCRIPTION:        Calls PreCopy and/or Indication, if existent and
*                     set the indication flag
*************************************************************************/
/* CODE CATEGORY 2 START*/
C_API_1 void C_API_2 CanHandleRxMsg(void)
{
  CanDeclareGlobalInterruptOldStatus
  CAN_CHANNEL_CANTYPE_LOCAL
  tCanRxInfoStruct        localCanRxInfoStruct;

  while ( canRxQueue.canRxQueueCount != (vuintx)0 )
  {

# if defined( C_MULTIPLE_RECEIVE_CHANNEL )
    channel = canRxQueue.canRxQueueBuf[canRxQueue.canRxQueueReadIndex].Channel;
# endif


    CAN_POLLING_IRQ_DISABLE( CAN_CHANNEL_CANPARA_ONLY );

    /* Call the application call-back functions and set flags */
    localCanRxInfoStruct.Handle      = canRxQueue.canRxQueueBuf[canRxQueue.canRxQueueReadIndex].Handle;
    localCanRxInfoStruct.pChipData   = (CanChipDataPtr)&(canRxQueue.canRxQueueBuf[canRxQueue.canRxQueueReadIndex].CanChipMsgObj.DataFld[0]);
    canRDSRxPtr[channel] = localCanRxInfoStruct.pChipData;
    localCanRxInfoStruct.pChipMsgObj = (CanChipMsgPtr) &(canRxQueue.canRxQueueBuf[canRxQueue.canRxQueueReadIndex].CanChipMsgObj);
    localCanRxInfoStruct.Channel     = channel;

# if defined( C_ENABLE_RX_QUEUE_RANGE )
    switch ( localCanRxInfoStruct.Handle)
    {
#  if defined( C_ENABLE_RANGE_0 )
      case kCanRxHandleRange0: (void)APPLCANRANGE0PRECOPY( &localCanRxInfoStruct ); break;
#  endif
#  if defined( C_ENABLE_RANGE_1 )
      case kCanRxHandleRange1: (void)APPLCANRANGE1PRECOPY( &localCanRxInfoStruct ); break;
#  endif
#  if defined( C_ENABLE_RANGE_2 )
      case kCanRxHandleRange2: (void)APPLCANRANGE2PRECOPY( &localCanRxInfoStruct ); break;
#  endif
#  if defined( C_ENABLE_RANGE_3 )
      case kCanRxHandleRange3: (void)APPLCANRANGE3PRECOPY( &localCanRxInfoStruct ); break;
#  endif
      default:
#  if defined( C_ENABLE_INDICATION_FLAG ) || \
      defined( C_ENABLE_INDICATION_FCT )
             if( CanHL_ReceivedRxHandle( CAN_CHANNEL_CANPARA_FIRST &localCanRxInfoStruct ) == kCanHlContinueRx )
             {
               CanHL_IndRxHandle(localCanRxInfoStruct.Handle);
             }
#  else
             (void) CanHL_ReceivedRxHandle( CAN_CHANNEL_CANPARA_FIRST &localCanRxInfoStruct );
#  endif
             break;
   }
# else
#  if defined( C_ENABLE_INDICATION_FLAG ) || \
      defined( C_ENABLE_INDICATION_FCT )
    if( CanHL_ReceivedRxHandle( CAN_CHANNEL_CANPARA_FIRST &localCanRxInfoStruct ) == kCanHlContinueRx )
    {
      CanHL_IndRxHandle(localCanRxInfoStruct.Handle);
    }
#  else
    (void) CanHL_ReceivedRxHandle( CAN_CHANNEL_CANPARA_FIRST &localCanRxInfoStruct );
#  endif
# endif
    
    CAN_POLLING_IRQ_RESTORE( CAN_CHANNEL_CANPARA_ONLY );

    CanNestedGlobalInterruptDisable();
    if (canRxQueue.canRxQueueReadIndex == (kCanRxQueueSize - 1) )
    {
      canRxQueue.canRxQueueReadIndex = 0;
    }
    else
    {
      canRxQueue.canRxQueueReadIndex++;
    }
    canRxQueue.canRxQueueCount--;
    CanNestedGlobalInterruptRestore();
  }
  return;
} /* end of CanHandleRxMsg() */
/* CODE CATEGORY 2 END*/

/************************************************************************
* NAME:               CanDeleteRxQueue
* CALLED BY:          Application, CAN driver
* Preconditions:      none
* PARAMETER:          none
* RETURN VALUE:       none
* DESCRIPTION:        delete receive queue
*************************************************************************/
/* CODE CATEGORY 4 START*/
C_API_1 void C_API_2 CanDeleteRxQueue(void)
{
  CanDeclareGlobalInterruptOldStatus
  
  CanNestedGlobalInterruptDisable();
  canRxQueue.canRxQueueWriteIndex  = (vuintx)0xFFFFFFFF;
  canRxQueue.canRxQueueReadIndex   = 0;
  canRxQueue.canRxQueueCount       = 0;  
  CanNestedGlobalInterruptRestore();
} /* end of CanDeleteRxQueue() */
/* CODE CATEGORY 4 END*/

# if defined( C_ENABLE_RX_QUEUE_EXTERNAL )
/************************************************************************
* NAME:               CanSetRxQueuePtr
* CALLED BY:          Application
* Preconditions:      none
* PARAMETER:          pQueue: pointer to the external queue which is used by the driver
* RETURN VALUE:       none
* DESCRIPTION:        If the driver is configured to use a rx queue it is
*                     possible to use an application defined queue.
*                     In case of a QNX system the queue is located in the
*                     system page
*************************************************************************/
/* CODE CATEGORY 4 START*/
void CanSetRxQueuePtr(tCanRxQueue* pQueue)
{
  canRxQueuePtr = pQueue;
  CanDeleteRxQueue();
}
/* CODE CATEGORY 4 END*/
# endif  /* C_ENABLE_EXTERNAL_RX_QUEUE */
#endif /* C_ENABLE_RX_QUEUE */

/* End of channel */
/* STOPSINGLE_OF_MULTIPLE */

/* Kernbauer Version: 1.16 Konfiguration: can_driver Erzeugungsgangnummer: 1 */


/* Kernbauer Version: 1.16 Konfiguration: can_driver Erzeugungsgangnummer: 1 */


/* Kernbauer Version: 1.16 Konfiguration: can_driver Erzeugungsgangnummer: 1 */


/* Kernbauer Version: 1.16 Konfiguration: can_driver Erzeugungsgangnummer: 1 */


/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
