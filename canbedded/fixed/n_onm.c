/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     n_onm.c 
 *
 * description:   n_onm
 *
 *
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           n_onm for S12XEP100
 *
 *****************************************************************************/

#define V_NM_DIROSEK_SOURCE

/**********************************************************************************************************************
  Includes
**********************************************************************************************************************/
#include "n_onmdef.h"

#if defined ( VGEN_ENABLE_NM_OSEK_D )


/**********************************************************************************************************************
  Version check
**********************************************************************************************************************/
/*--- check version of static files ---------------------------------------------------------------------------------*/
#if ( NM_DIROSEK_VERSION != 0x0374 )
  #error "Nm_DirOsek: Header and Source file are inconsistent (version)!"
#endif
#if ( NM_DIROSEK_RELEASE_VERSION != 0x03 )
  #error "Nm_DirOsek: Header and Source file are inconsistent (release version)!"
#endif

/*--- check version of configuration tool / generated files ---------------------------------------------------------*/
#if defined ( VGEN_GENY )
  #if defined ( NM_DIROSEKDLL_VERSION )
    #if ( NM_DIROSEKDLL_VERSION < 0x0200 )
      #error "Nm_DirOsek: This version of Nm_DirOsek requires a newer version of the Nm_DirOsek GENy DLL"
    #endif
  #else
    #error "Nm_DirOsek: 'NM_DIROSEKDLL_VERSION' is not defined. Please check your configuration file."
  #endif
#endif

/**********************************************************************************************************************
  ORGANI check
**********************************************************************************************************************/

/**********************************************************************************************************************
  Local configuration
**********************************************************************************************************************/
#if defined ( kCanNumberOfChannels )
#else
    #error "Nm_DirOsek: kCanNumberOfChannels is not defined"
#endif


#if defined ( NM_ENABLE_INDEXED_NM ) || \
    defined ( NM_ENABLE_MULTIPLE_NODES )
  #define NM_INT_DISABLE_OPTIMIZED_SKIPPED_DETECTION
#else
  #if ( kNmEcuNr == 0x00 )
    #define NM_INT_ENABLE_OPTIMIZED_SKIPPED_DETECTION
  #else
    #define NM_INT_DISABLE_OPTIMIZED_SKIPPED_DETECTION
  #endif
#endif

/**********************************************************************************************************************
  Configuration check
**********************************************************************************************************************/
/*--- check system setup ----------------------------------------------------*/
#if defined ( VGEN_ENABLE_NM_OSEK_D )
#else
  #error "Nm_DirOsek: Source is part of the project, but not part of the system configuration."
#endif

#if defined ( C_CPUTYPE_BITORDER_MSB2LSB ) || \
    defined ( C_CPUTYPE_BITORDER_LSB2MSB )
#else
  #error "Nm_DirOsek: Unknown/invalid setting for CPUTYPE"
#endif

/*--- check channel assignment ----------------------------------------------*/
#if defined ( kNmNumberOfChannels )
  #if ( kNmNumberOfChannels < 1 )
    #error "Nm_DirOsek: Invalid value of 'kNmNumberOfChannels'."
  #endif
#else
  #error "Nm_DirOsek: 'kNmNumberOfChannels' is not defined. Please check your configuration file."
#endif


/*--- check timing values ---------------------------------------------------*/


/*--- check features --------------------------------------------------------*/
/*
 * Name:        NM_ENABLE_CHANNEL_INDIRECTION
 * Description: This define enables/disables 
 * Origin:      GenTool system configuration
 */
#if defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  #if defined ( NM_ENABLE_INDEXED_NM )
  #else
    #error "Nm_DirOsek: Systems with channel indirection need indexed NM."
  #endif
#endif

#if defined ( NM_ENABLE_COPY_RINGDATA )
  #if defined ( NM_ENABLE_COPY_RINGDATA_TO_TEMPBUFFER )
    #error "Nm_DirOsek: Copy of Ringdata only possible if temporary buffer for ringdata is disabled"
  #endif
#endif

#if defined ( NM_ENABLE_CONFIG )
  #if defined ( NM_NUMBER_OF_CONFIG_NODES )
    #if ( (  (NM_NUMBER_OF_CONFIG_NODES) > 0)       && \
          ( ((NM_NUMBER_OF_CONFIG_NODES) % 8) == 0) && \
          (  (NM_NUMBER_OF_CONFIG_NODES) <= 256) )
    #else
      #error "Nm_DirOsek: Invalid value of NM_NUMBER_OF_CONFIG_NODES."
    #endif
  #else
    #error "Nm_DirOsek: NM_NUMBER_OF_CONFIG_NODES is not defined."
  #endif
#endif

#if defined ( NM_ENABLE_CONFIG )
  #if defined ( NM_NUMBER_OF_CONFIG_NODES )
  #else
    #error "Nm_DirOsek: NM_NUMBER_OF_CONFIG_NODES is not defined."
  #endif
#endif

#if defined ( NM_ENABLE_EARLY_BUSOFF_REINIT )
  #if defined ( C_ENABLE_DRIVER_STATUS )
  #else
    #error "Nm_DirOsek: Feature 'NM_ENABLE_EARLY_BUSOFF_REINIT' requires a DrvCan that supports 'C_ENABLE_DRIVER_STATUS' "
  #endif
#endif

/**********************************************************************************************************************
  Local data defines
**********************************************************************************************************************/
/* 
 * Name:        kNmTxMsgSize
 *              tNmTxMsgType
 * Description: These defines determine the internal representation of the NM message.
 *              Depending on the used CPU type, the most efficient access is used.
 */
#if defined ( C_CPUTYPE_8BIT )
  #define kNmTxMsgSize            8
  #define tNmTxMsgType            vuint8
#else
  #if defined ( C_CPUTYPE_16BIT )
    #define kNmTxMsgSize          4
    #define tNmTxMsgType          vuint16
  #else
    #if defined ( C_CPUTYPE_32BIT )
      #define kNmTxMsgSize        2
      #define tNmTxMsgType        vuint32
    #else
      #error "Nm_DirOsek: Missing/Unknown setting for C_CPUTYPE_xBIT. Please check v_cfg.h."
    #endif
  #endif
#endif

/* 
 * Name:        RX_CNT_...
 *              TX_CNT_...
 * Description: These are the masks, thresholds and increments for the
 *              NM RX and TX counters.
 */
#define RX_CNT_MASK               ((vuint8)0x0F)
#define RX_CNT_STEP               ((vuint8)0x01)
#define TX_CNT_MASK               ((vuint8)0xF0)
#define TX_CNT_STEP               ((vuint8)0x10)

#if defined ( RX_CNT_MAX )
/* Note: RX_CNT_MAX can be configured in the user configuration file */
#else
  #define RX_CNT_MAX              ((vuint8)4)
#endif

  #define TX_CNT_MAX              ((vuint8)8)

#if defined ( NM_ENABLE_CONFIG ) 
/* 
 * Name:        BYTE_OFFSET_MASK
 * Description: This mask is used for calculation of the desired entry 
 *              within the node and/or error configuration table
 */
  #define BYTE_OFFSET_MASK        ((vuint8)0x07)
#endif

/*--- symbolic values for states ---------------------------------------------*/
/* 
 * Name:        NM_ACTION_...
 * Description: These are the symbolic names of the states of the NM state machine.
 */
#define NO_ACTION                     ((NmActionType)0x00)
#define NM_ACTION_START               ((NmActionType)0x01)
#define NM_ACTION_WAIT_SENDTOKEN      ((NmActionType)0x02)
#define NM_ACTION_WAIT_RINGMSG        ((NmActionType)0x03)
#define NM_ACTION_LH_WAIT_LIMPHOME_TX ((NmActionType)0x04)
#define NM_ACTION_LH_PREPARE_SLEEP    ((NmActionType)0x05)
#define NM_ACTION_GO_BUSSLEEP         ((NmActionType)0x06)
#define NM_ACTION_BUS_SLEEP           ((NmActionType)0x07)
#define NM_ACTION_REPEAT              ((NmActionType)0x80)



/* Defines for NM message bits */
  #define ALIVE                     ((vuint8)0x01)
  #define RING                      ((vuint8)0x02)
  #define LIMPHOME                  ((vuint8)0x04)

  #define SLEEP_IND                 ((vuint8)0x10)
  #define SLEEP_ACK                 ((vuint8)0x20)

  #define CTRL_INIT                 ((vuint8)0x00)
  #define DATA_INIT                 ((vuint8)0x00)


/**********************************************************************************************************************
  Local macro defines
**********************************************************************************************************************/
/*-----------------------------------------------------------------------------
 * function-like macros
 *---------------------------------------------------------------------------*/
#if defined ( NM_ENABLE_ASSERTION )
  /*
   * Name:        NmAssert() 
   *              NmAssertAlways()
   * Description: These macro are used for assertion handling during system integration and test.
   */
  #define NmAssert(p, e)         if((p)==0) ApplNmDirOsekFatalError(e)
  #define NmAssertAlways(p, e)   ( ApplNmDirOsekFatalError(e) )
#else
  #define NmAssert(p, e)
  #define NmAssertAlways(p, e)
#endif

  #define NmDebug(text)         


/*
 * Name:        NmCalculateConfigByte()
 * Description: This macro is used to calculate the byte for access the node and/or error configuration array.
 */
#if defined ( NM_ENABLE_INDEXED_NM )
  #define NmCalculateConfigByte(i)  (vuint8)((vuint8)((i)>>3) + (channel*(vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8)))
#else
  #if defined ( NM_ENABLE_MULTIPLE_NODES )
    #define NmCalculateConfigByte(i)  (vuint8)((i)>>3)
  #else
    /* ESCAN00020990 */
    #if ( kNmEcuNr <= 0x07 )
      /* ESCAN00020580 */
      #define NmCalculateConfigByte(i)  (vuint8)(0)
    #else
      #define NmCalculateConfigByte(i)  (vuint8)((i)>>3)  /* PRQA S 3453 */ /* 2007-12-17, vismas: due to runtime resources */
    #endif
  #endif
#endif

/*
 * Name:        NmResetNmStatusRxFlags()
 * Description: This macro is used to reset the low nibble of nmStateByteLocal.
 */
#define NmResetNmStatusRxFlags()    (nmStateByteLocal &= (vuint8)0xF0)

/*
 * Name:        NmGetRxMsgSourceAdr()
 * Description: This macro is used to determine the CAN ID of a received NM message.
 * Note:        This macro is only valid in NmPrecopy().
 */
#if defined ( NM_ENABLE_INDEXED_DRVCAN ) || \
    defined ( C_SINGLE_RECEIVE_CHANNEL )
  #define NmGetRxMsgSourceAdr()     ((vuint8)((vuint8)(CanRxActualId(rxStruct)) & kNmSourceMask))
#else
  #define NmGetRxMsgSourceAdr()     ((vuint8)((vuint8)(CanRxActualId      ) & kNmSourceMask))
#endif

/*-----------------------------------------------------------------------------
 * wrapper-like macros
 *---------------------------------------------------------------------------*/
/* multi-channel CAN driver */
#if defined ( C_SINGLE_RECEIVE_BUFFER ) || \
    defined ( C_MULTIPLE_RECEIVE_BUFFER )
  #if defined ( NM_CAN_CHANNEL )
    #if ( NM_CAN_CHANNEL == 0 )
      #define CanRxActualId          CanRxActualId_0
      #define CanTransmit            CanTransmit_0
      #define CanResetBusOffStart(a) CanResetBusOffStart_0(a)
      #define CanResetBusOffEnd(a)   CanResetBusOffEnd_0(a)
      #define CanResetBusSleep(a)    CanResetBusSleep_0(a)
      #define CanOffline             CanOffline_0
      #define CanOnline              CanOnline_0
      #define CanInterruptDisable    CanInterruptDisable_0
      #define CanInterruptRestore    CanInterruptRestore_0
      #define CanRxActualDLC         CanRxActualDLC_0
      #define NmConfirmation         NmConfirmation_0
      #define NmPrecopy              NmPrecopy_0
      #define NmCanError             NmCanError_0
      #define nmSendMess             nmSendMess_0
    #endif
    #if ( NM_CAN_CHANNEL == 1 )
      #define CanRxActualId          CanRxActualId_1
      #define CanTransmit            CanTransmit_1
      #define CanResetBusOffStart(a) CanResetBusOffStart_1(a)
      #define CanResetBusOffEnd(a)   CanResetBusOffEnd_1(a)
      #define CanResetBusSleep(a)    CanResetBusSleep_1(a)
      #define CanOffline             CanOffline_1
      #define CanOnline              CanOnline_1
      #define CanInterruptDisable    CanInterruptDisable_1
      #define CanInterruptRestore    CanInterruptRestore_1
      #define CanRxActualDLC         CanRxActualDLC_1
      #define NmConfirmation         NmConfirmation_1
      #define NmPrecopy              NmPrecopy_1
      #define NmCanError             NmCanError_1
      #define nmSendMess             nmSendMess_1
    #endif
    #if ( NM_CAN_CHANNEL == 2 )
      #define CanRxActualId          CanRxActualId_2
      #define CanTransmit            CanTransmit_2
      #define CanResetBusOffStart(a) CanResetBusOffStart_2(a)
      #define CanResetBusOffEnd(a)   CanResetBusOffEnd_2(a)
      #define CanResetBusSleep(a)    CanResetBusSleep_2(a)
      #define CanOffline             CanOffline_2
      #define CanOnline              CanOnline_2
      #define CanInterruptDisable    CanInterruptDisable_2
      #define CanInterruptRestore    CanInterruptRestore_2
      #define CanRxActualDLC         CanRxActualDLC_2
      #define NmConfirmation         NmConfirmation_2
      #define NmPrecopy              NmPrecopy_2
      #define NmCanError             NmCanError_2
      #define nmSendMess             nmSendMess_2
    #endif
    #if ( NM_CAN_CHANNEL == 3 )
      #define CanRxActualId          CanRxActualId_3
      #define CanTransmit            CanTransmit_3
      #define CanResetBusOffStart(a) CanResetBusOffStart_3(a)
      #define CanResetBusOffEnd(a)   CanResetBusOffEnd_3(a)
      #define CanResetBusSleep(a)    CanResetBusSleep_3(a)
      #define CanOffline             CanOffline_3
      #define CanOnline              CanOnline_3
      #define CanInterruptDisable    CanInterruptDisable_3
      #define CanInterruptRestore    CanInterruptRestore_3
      #define CanRxActualDLC         CanRxActualDLC_3
      #define NmConfirmation         NmConfirmation_3
      #define NmPrecopy              NmPrecopy_3
      #define NmCanError             NmCanError_3
      #define nmSendMess             nmSendMess_3
    #endif
    #if ( NM_CAN_CHANNEL == 4 )
      #define CanRxActualId          CanRxActualId_4
      #define CanTransmit            CanTransmit_4
      #define CanResetBusOffStart(a) CanResetBusOffStart_4(a)
      #define CanResetBusOffEnd(a)   CanResetBusOffEnd_4(a)
      #define CanResetBusSleep(a)    CanResetBusSleep_4(a)
      #define CanOffline             CanOffline_4
      #define CanOnline              CanOnline_4
      #define CanInterruptDisable    CanInterruptDisable_4
      #define CanInterruptRestore    CanInterruptRestore_4
      #define CanRxActualDLC         CanRxActualDLC_4
      #define NmConfirmation         NmConfirmation_4
      #define NmPrecopy              NmPrecopy_4
      #define NmCanError             NmCanError_4
      #define nmSendMess             nmSendMess_4
    #endif
  #endif
#endif



/*-----------------------------------------------------------------------------
 * macros for RX buffer (on-chip receive buffer)
 *---------------------------------------------------------------------------*/
/*
 * Name:        nmRxMsgBuffer
 * Description: 
 */
  #if defined ( C_SINGLE_RECEIVE_BUFFER )
    #if defined ( NM_CAN_CHANNEL )
      #if ( NM_CAN_CHANNEL == 0 )
        #define nmRxMsgBuffer               ((CanChipDataPtr)(CanRDSPtr_0))
      #endif
      #if ( NM_CAN_CHANNEL == 1 )
        #define nmRxMsgBuffer               ((CanChipDataPtr)(CanRDSPtr_1))
      #endif
      #if ( NM_CAN_CHANNEL == 2 )
        #define nmRxMsgBuffer               ((CanChipDataPtr)(CanRDSPtr_2))
      #endif
      #if ( NM_CAN_CHANNEL == 3 )
        #define nmRxMsgBuffer               ((CanChipDataPtr)(CanRDSPtr_3))
      #endif
    #else
      #define nmRxMsgBuffer                 ((CanChipDataPtr)(CanRDSPtr))
    #endif
  #endif
  #if defined ( C_MULTIPLE_RECEIVE_BUFFER )
    #define nmRxMsgBuffer                   ((CanChipDataPtr)(rxDataPtr))
  #endif
  #if defined ( NM_ENABLE_INDEXED_DRVCAN ) || \
      defined ( C_SINGLE_RECEIVE_CHANNEL )
    #define nmRxMsgBuffer                   ((CanChipDataPtr)(rxStruct->pChipData))
  #endif


/*-----------------------------------------------------------------------------
 * macros for variable access
 *---------------------------------------------------------------------------*/
#if defined ( NM_ENABLE_INDEXED_NM )
  #define NMVAR_CH_DEF(var)               var[kNmNumberOfChannels] /* PRQA S 3410 */ /* 2007-12-17, vismas: not possible for this kind of macro usage */
  #define NMVAR(var)                      var[channel]             /* PRQA S 3410 */ /* 2007-12-17, vismas: not possible for this kind of macro usage */
#else
  #define NMVAR_CH_DEF(var)               var /* PRQA S 3410 */ /* 2007-12-17, vismas: not possible for this kind of macro usage */
  #define NMVAR(var)                      var /* PRQA S 3410 */ /* 2007-12-17, vismas: not possible for this kind of macro usage */
#endif

/* nmState */
#define nmStateByteGlobal            NMVAR(nmState).c[0]
#define nmStateByteLocal             NMVAR(nmState).c[1]
#define nmStateRingStable            NMVAR(nmState).b.b_RingStable
#define mmStateBusError              NMVAR(nmState).b.b_BusError
#define nmStateActive                NMVAR(nmState).b.b_Active
#define nmStateLimpHome              NMVAR(nmState).b.b_LimpHome
#define nmStateBusSleep              NMVAR(nmState).b.b_BusSleep
#define nmStateWaitBusSleep          NMVAR(nmState).b.b_WaitBusSleep
  #define nmStateTxRingDataAllow     NMVAR(nmState).b.b_TxRingDataAllow
#define nmStateBusSleepInd           NMVAR(nmState).b.b_BusSleepInd
#define nmStateNmMsgRecv             NMVAR(nmState).b.b_NmMsgRecv
#define nmStateRingMsgRecv           NMVAR(nmState).b.b_RingMsgRecv
#define nmStateImAddressed           NMVAR(nmState).b.b_ImAddressed
#define nmStateImSkippedOver         NMVAR(nmState).b.b_ImSkippedOver
#define nmStateNmMsgRecvRemind       NMVAR(nmState).b.b_NmMsgRecvRemind
#define nmStateRxTxLockUd            NMVAR(nmState).b.b_RxTxLockUd  /* ESCAN00008104 */
#define nmStateRingPreStable         NMVAR(nmState).b.b_RingPreStable
#define nmStatePrepSleep             NMVAR(nmState).b.b_PrepSleep


#if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION ) || \
    defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
/* nmStateGwMaster */
  #define nmStateGwMasterFlags       NMVAR(nmStateGwMaster).c[0]
  #define nmStateGwMasterDummy       NMVAR(nmStateGwMaster).c[1]
  #if defined( NM_ENABLE_SLEEPACK_NOTIFICATION )
    #define nmStatePrepSendSleepAck  NMVAR(nmStateGwMaster).b.b_PrepareSendSleepAck
  #endif
  #if defined( NM_ENABLE_REMOTESLEEP_INDICATION )
    #define nmStateRemotePrepSleep   NMVAR(nmStateGwMaster).b.b_RemotePrepareSleep
    #define nmStateRemoteSleepInd    NMVAR(nmStateGwMaster).b.b_RemoteSleepInd
  #endif
#endif

/* nmSendMess */
  #define NmTxMsgFlagSleepAck         (*(tNmTxMsgTypeUnion*)NMVAR(nmSendMess)).b.bSleepAck
  #define NmTxMsgFlagSleepInd         (*(tNmTxMsgTypeUnion*)NMVAR(nmSendMess)).b.bSleepInd
  #define NmTxMsgFlagConfig           (*(tNmTxMsgTypeUnion*)NMVAR(nmSendMess)).b.bConfig
  #define NmTxMsgByte0                (*(tNmTxMsgTypeUnion*)NMVAR(nmSendMess)).c[0]
  #define NmTxMsgByte1                (*(tNmTxMsgTypeUnion*)NMVAR(nmSendMess)).c[1]
  #define NmTxMsgByte2                (*(tNmTxMsgTypeUnion*)NMVAR(nmSendMess)).c[2]
    #define NmTxMsgByteCmd            (*(tNmTxMsgTypeUnion*)NMVAR(nmSendMess)).b.cCmd
    #define NmTxMsgFlagCmdSet         (*(tNmTxMsgTypeUnion*)NMVAR(nmSendMess)).b.cCmd
    #define NmTxMsgFlagCmdIf          (*(tNmTxMsgTypeUnion*)NMVAR(nmSendMess)).b.cCmd
    #define NmTxMsgByteDest           (*(tNmTxMsgTypeUnion*)NMVAR(nmSendMess)).c[0]
    #define NmTxMsgByteUser0          (*(tNmTxMsgTypeUnion*)NMVAR(nmSendMess)).c[2]
    #define NmTxMsgByteUser1          (*(tNmTxMsgTypeUnion*)NMVAR(nmSendMess)).c[3]
    #define NmTxMsgByteUser2          (*(tNmTxMsgTypeUnion*)NMVAR(nmSendMess)).c[4]
    #define NmTxMsgByteUser3          (*(tNmTxMsgTypeUnion*)NMVAR(nmSendMess)).c[5]
    #define NmTxMsgByteUser4          (*(tNmTxMsgTypeUnion*)NMVAR(nmSendMess)).c[6]
    #define NmTxMsgByteUser5          (*(tNmTxMsgTypeUnion*)NMVAR(nmSendMess)).c[7]

/* nmRxMsgBuffer */
  #define nmRxMsgFlagDestAdr        ((vuint8)(nmRxMsgBuffer[0]))
  #define nmRxMsgFlagCmd            ((vuint8)(nmRxMsgBuffer[1] & (vuint8)0x0F))
  #define nmRxMsgFlagSleepInd       ((vuint8)(nmRxMsgBuffer[1] & SLEEP_IND))
  #define nmRxMsgFlagSleepAck       ((vuint8)(nmRxMsgBuffer[1] & SLEEP_ACK))

  #define nmRxMsgUserStartAdr               (&nmRxMsgBuffer[2])
  #define nmRxMsgByteUser0          ((vuint8)(nmRxMsgBuffer[2]))
  #define nmRxMsgByteUser1          ((vuint8)(nmRxMsgBuffer[3]))
  #define nmRxMsgByteUser2          ((vuint8)(nmRxMsgBuffer[4]))
  #define nmRxMsgByteUser3          ((vuint8)(nmRxMsgBuffer[5]))
  #define nmRxMsgByteUser4          ((vuint8)(nmRxMsgBuffer[6]))
  #define nmRxMsgByteUser5          ((vuint8)(nmRxMsgBuffer[7]))


/*-----------------------------------------------------------------------------
 * macros for configuration access
 *---------------------------------------------------------------------------*/
#if defined ( NM_ENABLE_INDEXED_NM )
  #define NMCFG_COUNT_RINGTYP             NmCountRingTyp_Field[channel]
  #define NMCFG_COUNT_RINGMAX             NmCountRingMax_Field[channel]
  #define NMCFG_COUNT_LIMPHOME            NmCountLimphome_Field[channel]
  #define NMCFG_COUNT_WAITBUSSLEEP        NmCountWaitBussleep_Field[channel]
  #define kNmCfg_CanPara                  NmCanPara_Field[channel]
#else
  #define NMCFG_COUNT_RINGTYP             NM_COUNT_RING_TYP
  #define NMCFG_COUNT_RINGMAX             NM_COUNT_RING_MAX
  #define NMCFG_COUNT_LIMPHOME            NM_COUNT_LIMPHOME
  #define NMCFG_COUNT_WAITBUSSLEEP        NM_COUNT_WAIT_BUSSLEEP
  #define kNmCfg_CanPara                  kNmCanPara
#endif


#if defined ( NM_ENABLE_FAST_BUSOFF_RECOVERY )
  #if defined ( NM_ENABLE_INDEXED_NM )
    #define NMCFG_COUNT_LIMPHOME_SHORT    NmCountLimphomeShort_Field[channel]
    #define NMCFG_COUNT_SHORT_NUMBER      NmCountShortNumber_Field[channel]
  #else
    #define NMCFG_COUNT_LIMPHOME_SHORT    NM_COUNT_LIMPHOME_SHORT
    #define NMCFG_COUNT_SHORT_NUMBER      NM_COUNT_SHORT_NUMBER
  #endif
#endif

  #if defined ( NM_ENABLE_INDEXED_NM )
    #if defined ( NM_ENABLE_MULTIPLE_NODES )
      #define kNmCfg_EcuNr                  nmEcuNr_FieldMultipleNodes[channel]
      #define kNmCfg_TxObj                  nmTxObj_FieldMultipleNodes[channel]
    #else
      #define kNmCfg_EcuNr                  NmEcuNr_Field[channel]
      #define kNmCfg_TxObj                  NmTxObj_Field[channel]
    #endif
  #else
    #if defined ( NM_ENABLE_MULTIPLE_NODES )
      #if defined ( NM_CAN_CHANNEL )
        #define kNmCfg_EcuNr                NmEcuNr_Field[comMultipleECUCurrent[NM_CAN_CHANNEL]]
        #define kNmCfg_TxObj                NmTxObj_Field[comMultipleECUCurrent[NM_CAN_CHANNEL]]
      #else
        #define kNmCfg_EcuNr                NmEcuNr_Field[comMultipleECUCurrent]
        #define kNmCfg_TxObj                NmTxObj_Field[comMultipleECUCurrent]
      #endif
    #else
        #define kNmCfg_EcuNr                kNmEcuNr
        #define kNmCfg_TxObj                kNmTxObj
    #endif
  #endif

/* 
 * Name:        kNmSourceMask 
 * Description: This mask is used to determine the assigned station address 
 *              of a received NM message.
 * Note:        Only valid within NmPrecopy()
 */
#if defined ( VGEN_GENY )
/* GENy generates the mask information depending on the DBC attribute NmMessageCount */
  #if defined ( NM_ENABLE_INDEXED_NM )
    #if ( kNmNumberOfRanges > 1 )
      #define kNmSourceMask               (NmRangeMask_Field[channel])
    #else
      #define kNmSourceMask               ((vuint8)NM_RANGE_MASK)
    #endif
  #else
    #define   kNmSourceMask               ((vuint8)NM_RANGE_MASK)
  #endif
#endif



/**********************************************************************************************************************
  Local data types and structures
**********************************************************************************************************************/
/****************************************
 * Name:        NmActionType
 * Description: This type is used for the state of the NM state machine.
 */
typedef vuint8 NmActionType;

/****************************************
 * Name:        tNmTimeCnt
 * Description: This type is used for the NM task counter.
 * Note       : The value range of the counter may depend on the used features.
 */
typedef vuint8 tNmTimeCnt;

/****************************************
 * Name:        tNmStateType
 * Description: This type represents the various status flags.
 */
typedef union
{
  vuint8 c[2];

  #if defined ( C_CPUTYPE_BITORDER_MSB2LSB )
  struct 
  {
    vbittype    b_BusSleepInd     :1;     /* byte0 bit7 */
    vbittype    b_TxRingDataAllow :1;     /* byte0 bit6 */
    vbittype    b_WaitBusSleep    :1;     /* byte0 bit5 */
    vbittype    b_BusSleep        :1;     /* byte0 bit4 */
    vbittype    b_LimpHome        :1;     /* byte0 bit3 */
    vbittype    b_Active          :1;     /* byte0 bit2 */
    vbittype    b_BusError        :1;     /* byte0 bit1 */
    vbittype    b_RingStable      :1;     /* byte0 bit0 */

    vbittype    b_PrepSleep       :1;     /* byte1 bit7 */
    vbittype    b_RingPreStable   :1;     /* byte1 bit6 */
    vbittype    b_RxTxLockUd      :1;     /* byte1 bit5 */  /* ESCAN00008104 */
    vbittype    b_NmMsgRecvRemind :1;     /* byte1 bit4 */
    vbittype    b_ImSkippedOver   :1;     /* byte1 bit3 */
    vbittype    b_ImAddressed     :1;     /* byte1 bit2 */
    vbittype    b_RingMsgRecv     :1;     /* byte1 bit1 */
    vbittype    b_NmMsgRecv       :1;     /* byte1 bit0 */
  }b;
  #else
  struct
  {
    vbittype    b_RingStable      :1;     /* byte0 bit0 */
    vbittype    b_BusError        :1;     /* byte0 bit1 */
    vbittype    b_Active          :1;     /* byte0 bit2 */
    vbittype    b_LimpHome        :1;     /* byte0 bit3 */
    vbittype    b_BusSleep        :1;     /* byte0 bit4 */
    vbittype    b_WaitBusSleep    :1;     /* byte0 bit5 */
    vbittype    b_TxRingDataAllow :1;     /* byte0 bit6 */
    vbittype    b_BusSleepInd     :1;     /* byte0 bit7 */

    vbittype    b_NmMsgRecv       :1;     /* byte1 bit0 */
    vbittype    b_RingMsgRecv     :1;     /* byte1 bit1 */
    vbittype    b_ImAddressed     :1;     /* byte1 bit3 */
    vbittype    b_ImSkippedOver   :1;     /* byte1 bit4 */
    vbittype    b_NmMsgRecvRemind :1;     /* byte1 bit6 */
    vbittype    b_RxTxLockUd      :1;     /* byte1 bit5 */  /* ESCAN00008104 */
    vbittype    b_RingPreStable   :1;     /* byte1 bit6 */
    vbittype    b_PrepSleep       :1;     /* byte1 bit7 */
  }b;
  #endif
} tNmStateType;

  #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION ) || \
      defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
/****************************************
 * Name:        tNmStateTypeGwMaster
 * Description: This type represents the various status flags for gateway usage.
 */
typedef union 
{
  vuint8 c[2];

    #if defined ( C_CPUTYPE_BITORDER_MSB2LSB )
  struct 
  {
    vbittype b_Dummy0_7            :1;   /* byte0 bit7 */
    vbittype b_Dummy0_6            :1;   /* byte0 bit6 */
    vbittype b_Dummy0_5            :1;   /* byte0 bit5 */
    vbittype b_Dummy0_4            :1;   /* byte0 bit4 */
    vbittype b_Dummy0_3            :1;   /* byte0 bit3 */
    vbittype b_PrepareSendSleepAck :1;   /* byte0 bit2 */
    vbittype b_RemoteSleepInd      :1;   /* byte0 bit1 */
    vbittype b_RemotePrepareSleep  :1;   /* byte0 bit0 */

    vbittype b_Dummy1_7            :1;   /* byte1 bit7 */
    vbittype b_Dummy1_6            :1;   /* byte1 bit6 */
    vbittype b_Dummy1_5            :1;   /* byte1 bit5 */
    vbittype b_Dummy1_4            :1;   /* byte1 bit4 */
    vbittype b_Dummy1_3            :1;   /* byte1 bit3 */
    vbittype b_Dummy1_2            :1;   /* byte1 bit2 */
    vbittype b_Dummy1_1            :1;   /* byte1 bit1 */
    vbittype b_Dummy1_0            :1;   /* byte1 bit0 */
  }b;
    #else
  struct
  {
    vbittype b_RemotePrepareSleep  :1;   /* byte0 bit0 */
    vbittype b_RemoteSleepInd      :1;   /* byte0 bit1 */
    vbittype b_PrepareSendSleepAck :1;   /* byte0 bit2 */
    vbittype b_Dummy0_3            :1;   /* byte0 bit3 */
    vbittype b_Dummy0_4            :1;   /* byte0 bit4 */
    vbittype b_Dummy0_5            :1;   /* byte0 bit5 */
    vbittype b_Dummy0_6            :1;   /* byte0 bit6 */
    vbittype b_Dummy0_7            :1;   /* byte0 bit7 */

    vbittype b_Dummy1_0            :1;   /* byte1 bit0 */
    vbittype b_Dummy1_1            :1;   /* byte1 bit1 */
    vbittype b_Dummy1_2            :1;   /* byte1 bit2 */
    vbittype b_Dummy1_3            :1;   /* byte1 bit3 */
    vbittype b_Dummy1_4            :1;   /* byte1 bit4 */
    vbittype b_Dummy1_5            :1;   /* byte1 bit5 */
    vbittype b_Dummy1_6            :1;   /* byte1 bit6 */
    vbittype b_Dummy1_7            :1;   /* byte1 bit7 */
  }b;
    #endif
} tNmStateTypeGwMaster;
  #endif

/****************************************
 * Name:        tNmTxMsgTypeUnion
 * Description: This type represents the message layout of the NM TX message.
 */
typedef union
{
  vuint8 c[8];

  #if defined ( C_CPUTYPE_BITORDER_MSB2LSB )
  struct
  {
    vbittype cDest          :8;   /* byte0 bit07..00 */
    vbittype bReserved0     :1;   /* byte1 bit15 */
    vbittype bConfig        :1;   /* byte1 bit14 */
    vbittype bSleepAck      :1;   /* byte1 bit13 */
    vbittype bSleepInd      :1;   /* byte1 bit12 */
    vbittype cCmd           :4;   /* byte1 bit11..08 */
                                  /* byte2..7 are accessed by byte element of union */
  }b;
  #else
  struct
  {
    vbittype cDest          :8;   /* byte0 bit07..00 */
    vbittype cCmd           :4;   /* byte1 bit11..08 */
    vbittype bSleepInd      :1;   /* byte1 bit12 */
    vbittype bSleepAck      :1;   /* byte1 bit13 */
    vbittype bConfig        :1;   /* byte1 bit14 */
    vbittype bReserved0     :1;   /* byte1 bit15 */
                                  /* byte2..7 are accessed by byte element of union */
  }b;
  #endif
} tNmTxMsgTypeUnion;


/**********************************************************************************************************************
  Global data definitions (ROM)
**********************************************************************************************************************/

V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kNmMainVersion    = (vuint8)(NM_DIROSEK_VERSION >> 8);
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kNmSubVersion     = (vuint8)(NM_DIROSEK_VERSION & 0xFF);
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kNmReleaseVersion = (vuint8)(NM_DIROSEK_RELEASE_VERSION);


/**********************************************************************************************************************
  Global data definitions (RAM)
**********************************************************************************************************************/


/****************************************
* Name       : nmSendMess
* Description: 
* Values     : 
* Note       : Name is fix, as it is used by the CAN driver
*/
V_MEMRAM0 V_MEMRAM1_NEAR tNmTxMsgType V_MEMRAM2_NEAR NMVAR_CH_DEF(nmSendMess)[kNmTxMsgSize];


/**********************************************************************************************************************
  Local data definitions (ROM)
**********************************************************************************************************************/

/**********************************************************************************************************************
  Local data definitions (RAM)
**********************************************************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------
  Page0
*--------------------------------------------------------------------------------------------------------------------*/

/****************************************
* Name:        nmState
* Description: 
* Values:      
*/
static tNmStateType MEMORY_NEAR NMVAR_CH_DEF(nmState);

/****************************************
* Name:        nmTxRequest
* Description: ESCAN00004259
* Values:      
*/
static volatile vuint8 MEMORY_NEAR NMVAR_CH_DEF(nmTxRequest);


#if defined ( NM_ENABLE_MULTIPLE_NODES )
/****************************************
* Name:        nmEcuNr_FieldMultipleNodes
* Description: 
* Values:      
*/
  #if defined ( NM_ENABLE_INDEXED_NM )
static vuint8 MEMORY_NEAR nmEcuNr_FieldMultipleNodes[kNmNumberOfChannels];
  #else
/* not possible */
  #endif
#endif

#if defined ( NM_ENABLE_MULTIPLE_NODES )
/****************************************
* Name:        nmTxObj_FieldMultipleNodes
* Description: 
* Values:      
*/
  #if defined ( NM_ENABLE_INDEXED_NM )
static vuint8 MEMORY_NEAR nmTxObj_FieldMultipleNodes[kNmNumberOfChannels];
  #else
/* not possible */
  #endif
#endif


#if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION ) || \
    defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
/****************************************
* Name:        nmStateGwMaster
* Description: 
* Values:      
*/
static tNmStateTypeGwMaster MEMORY_NEAR NMVAR_CH_DEF(nmStateGwMaster);
#endif

/*---------------------------------------------------------------------------------------------------------------------
  Extended page
*--------------------------------------------------------------------------------------------------------------------*/

#if defined ( NM_ENABLE_COPY_RINGDATA_TO_TEMPBUFFER )
/****************************************
 * Name:        nmUserDataBuffer
 * Description: 
 * Values:      
 */
static vuint8 NMVAR_CH_DEF(nmUserDataBuffer)[6];
#endif









/****************************************
* Name:        nmTimeCnt
* Description: 
* Values:      
*/
static tNmTimeCnt NMVAR_CH_DEF(nmTimeCnt);

/****************************************
* Name:        nmAction
* Description: 
* Values:      
*/
static NmActionType NMVAR_CH_DEF(nmAction);

/****************************************
* Name:        nmRxNmMsgSourceAdr
* Description: 
* Values:      
*/
static vuint8 NMVAR_CH_DEF(nmRxNmMsgSourceAdr);

/****************************************
* Name:        nmRxTxCnt
* Description: 
* Values:      
*/
static vuint8 NMVAR_CH_DEF(nmRxTxCnt);

/****************************************
* Name:        nmLogicalSuccessor
* Description: 
* Values:      
*/
static vuint8 NMVAR_CH_DEF(nmLogicalSuccessor);

#if defined ( NM_ENABLE_CONFIG )
/****************************************
* Name:        nmConfig
* Description: 
* Values:      
* Notes      : This variable is identical for single/multi channel systems.
*              The channel-dependency is handled by the type of the variable.
*/
static NmConfigType nmConfig;
#endif





#if defined ( NM_ENABLE_FAST_BUSOFF_RECOVERY )
/****************************************
* Name:        nmBusOffCounter
* Description: 
* Values:      
*/
static vuint8 NMVAR_CH_DEF(nmBusOffCounter);
#endif







#if defined ( NM_ENABLE_EARLY_BUSOFF_REINIT )
/* ESCAN00022927 */
/****************************************
* Name:        nmDrvCanIsBusOff
* Description: This flag indicates if the DrvCan is in BusOf state.
*              This flag is set upon BusOff event and cleared as soon the DrvCan is re-init within CanResetBusOffEnd().
* Values:      
*/
static vuint8 NMVAR_CH_DEF(nmDrvCanIsBusOff);
#endif


/**********************************************************************************************************************
  Local function prototypes
**********************************************************************************************************************/
static void NmTransmitSkippedAlive( NM_CHANNEL_NMTYPE_ONLY );
static void NmResetNodeConfig( NM_CHANNEL_NMTYPE_ONLY );



/**********************************************************************************************************************
  Global function definitions
**********************************************************************************************************************/
/******************************************************************************
 * Name:            NmCanError
 * Called by:       CAN driver
 * Parameter:       (CAN channel)
 * Return value:    ---
 * Description:     This API informs the NM about a BusOff event.
 * Precondition:    ---
 *****************************************************************************/
void NM_INTERNAL_CALL_TYPE NmCanError(NM_CHANNEL_CANTYPE_ONLY)
{
  #if defined ( NM_ENABLE_INDEXED_NM )
    #if defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  CanChannelHandle channel = NmCanToNmIndirection[canChannel];
    #endif
  #else
    #if defined ( NM_ENABLE_INDEXED_DRVCAN )
      #if defined ( NM_CAN_CHANNEL )
        #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  canChannel = canChannel; /* avoid compiler warnings */
        #endif
      #endif
    #endif
  #endif

  CanOffline(NM_CHANNEL_CANPARA_ONLY);

  mmStateBusError = 1;


    #if defined ( NM_ENABLE_INDEXED_DRVCAN )
  /* code: macros must be encapsulated in this way */
  CanResetBusOffStart((vuint8)NM_CHANNEL_CANPARA_ONLY, kNmCfg_CanPara);
    #else
  CanResetBusOffStart(kNmCfg_CanPara);
    #endif

    #if defined ( NM_ENABLE_EARLY_BUSOFF_REINIT )
  /* ESCAN00022927 */
  NMVAR(nmDrvCanIsBusOff) = 1; /* set channel-specific flag */
    #endif

  if( (nmStatePrepSleep == 1) && \
      ((nmStateBusSleep == 1) || (nmStateWaitBusSleep == 1)) )
  {
    /* ESCAN00009420:
     * There was a BusOff after the SleepAck, i.e. while the NM is about to be in sleep mode
     * Hold the Sleep state regardless of BusOff - restart always with Reset Alive.
     * The call of ApplNmBusOff() and ApplNmBusOffEnd() is not symmetric.
     * The Application has to decide, depending on the WaitBusSleep-State within
     *  ApplNmBusOff(), if it is neccessary to stop the communication (IL)
     *  or not if already in this state.
     */
    #if defined ( NM_ENABLE_INDEXED_DRVCAN )
    /* code: macros must be encapsulated in this way */
    CanResetBusOffEnd(NM_CHANNEL_CANPARA_ONLY, kNmCfg_CanPara);
    #else
    CanResetBusOffEnd(kNmCfg_CanPara);
    #endif
  }
  else
  {
    NmResetNmStatusRxFlags();
    nmStateNmMsgRecvRemind = 0;
    nmStateRxTxLockUd      = 0; /* ESCAN00008104 */
    NMVAR(nmTxRequest)     = 0; /* cancel any NM TX request */

    /* clear RX counter, set TX counter to 1 */
    NMVAR(nmRxTxCnt) = TX_CNT_STEP;

    if((NMVAR(nmAction) & ((NmActionType)(~(vuintx)NM_ACTION_REPEAT))) != NM_ACTION_LH_PREPARE_SLEEP)
    {
      /* start LimpHome timer */
    #if defined ( NM_ENABLE_FAST_BUSOFF_RECOVERY )
      if( NMVAR(nmBusOffCounter) > 0 )
      {
        NMVAR(nmTimeCnt) = NMCFG_COUNT_LIMPHOME_SHORT;
      } 
      else
      {
        NMVAR(nmTimeCnt) = NMCFG_COUNT_LIMPHOME;
      }
    #else
      NMVAR(nmTimeCnt)   = NMCFG_COUNT_LIMPHOME;
    #endif
      /* goto LimpHome */
      NMVAR(nmAction)  = NM_ACTION_LH_WAIT_LIMPHOME_TX;
    }
  }

  #if defined ( NM_ENABLE_FAST_BUSOFF_RECOVERY )
  /* update BusOff counter */
  if( NMVAR(nmBusOffCounter) > 0 )
  {
    NMVAR(nmBusOffCounter)--;
  }
  #endif

  /* inform application about start of BusOff */
  ApplNmBusOff(NM_CHANNEL_APPLPARA_ONLY);
}

/******************************************************************************
 * Name:            NmConfirmation
 * Called by:       CAN driver (TX interrupt)
 * Parameter:       TX object
 * Return value:    ---
 * Description:     This API informs the NM about the successful transmission of
 *                  the NM message.
 * Precondition:    ---
 *****************************************************************************/
void NM_INTERNAL_CALL_TYPE NmConfirmation(CanTransmitHandle txObject)
{

  #if defined ( NM_ENABLE_ONM_EXT_API )
  NmMsgType onmStateParam = NM_MSG_INVALID; 
  #endif


  #if defined ( NM_ENABLE_INDEXED_NM )
  CanChannelHandle channel;
  #endif

  #if defined ( NM_ENABLE_INDEXED_NM )
  /* determine NM channel from CAN transmit handle */
    #if defined( NM_ENABLE_MULTIPLE_NODES )
  for( channel=0; txObject!=NmTxObj_Field[channel][comMultipleECUCurrent[channel]]; channel++ ) 
  {
    /* ESCAN00014458 */
  }
    #else
      #if ( kNmNumberOfChannels == 1 )
  channel = 0;
      #else
  for( channel=0; txObject!=kNmCfg_TxObj; channel++ ) 
  {
  }
      #endif
    #endif
  #else
    NmAssert( txObject == kNmTxObj, kNmAssertConfirmationHasWrongCanHandle );

    #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  txObject = txObject; /* PRQA S 3199 */ /* 2007-11-27, vismas: dummy assignment for unused parameter */
    #endif
  #endif


  /* release waiting for NmConfirmation (ESCAN00008104) */
  nmStateRxTxLockUd = 0;

  /* message transmitted => reset TX counter */
  NMVAR(nmRxTxCnt) &= ((vuint8)(~(vuintx)TX_CNT_MASK));

  #if defined ( NM_ENABLE_FAST_BUSOFF_RECOVERY ) 
  /* message received => reset BusOff counter */
  NMVAR(nmBusOffCounter) = NMCFG_COUNT_SHORT_NUMBER;
  #endif


  /*---------------------------------------------------------------------------*/
  if( NmTxMsgFlagCmdIf == RING )
  /*---------------------------------------------------------------------------*/
  {
    #if defined ( NM_ENABLE_ONM_EXT_API )
    onmStateParam |= NM_MSG_RING; 
    #endif

    /* ESCAN00007856 */
    if( (nmStatePrepSleep == 1) && (NmTxMsgFlagSleepAck == 1) )
    {
      CanOffline(NM_CHANNEL_CANPARA_ONLY);

      #if defined ( NM_ENABLE_ONM_EXT_API )
      onmStateParam |= NM_MSG_SLEEP_ACK; 
      #endif

      nmStateWaitBusSleep  = 1;
      NMVAR(nmTxRequest) = 0; /* cancel NM TX request, nmStateRxTxLockUd is already 0 */


      NMVAR(nmTimeCnt)  = NMCFG_COUNT_WAITBUSSLEEP;
      NMVAR(nmAction) = NM_ACTION_GO_BUSSLEEP;
    }
    else
    {
      if( (nmStateBusSleepInd == 1) && (NmTxMsgFlagSleepInd == 1) )
      {
        #if defined ( NM_ENABLE_ONM_EXT_API )
        onmStateParam |= NM_MSG_SLEEP_IND; 
        #endif

        nmStatePrepSleep = 1;

        #if defined( NM_ENABLE_REMOTESLEEP_INDICATION )
        /* Remote sleep indication depends only on received sleep indication flags */
        #endif
      }

      /*
        Do not wait for any Ring message if we are currently go to sleep.
        This could happen, if we receive a Sleep Acknowledge (NmPrecopy()) while waiting for
        our own Confirmation for the Sleep Indication message.
        Then, this last retrigger and state transition could be discarded.
        Leave the nmStateImAddressed and nmStatePrepSleep information too.
        These flags got it's latest update also in NmPrecopy().
      */
      if( nmStateWaitBusSleep == 0 ) /* ESCAN00007856 */
      {
        nmStateImAddressed = 0;
        nmStateLimpHome = 0; /* ESCAN00017962 */

        /* re-trigger timer to avoid duplicate rings */
        NMVAR(nmTimeCnt) = NMCFG_COUNT_RINGMAX;
        NMVAR(nmAction) = NM_ACTION_WAIT_RINGMSG;
      }
    }
  }
  else
  {
    /*---------------------------------------------------------------------------*/
    if( NmTxMsgFlagCmdIf == LIMPHOME )
    /*---------------------------------------------------------------------------*/
    {
      #if defined ( NM_ENABLE_ONM_EXT_API )
      onmStateParam |= NM_MSG_LIMPHOME; 
      #endif

      if( (nmStateActive == 1) && (nmStateNmMsgRecvRemind == 1) )
      {
        NMVAR(nmAction) = NM_ACTION_START;
      }
    }
    else
    {
      /*---------------------------------------------------------------------------*/
      if( NmTxMsgFlagCmdIf == ALIVE )
      /*---------------------------------------------------------------------------*/
      {
        #if defined ( NM_ENABLE_ONM_EXT_API )
        /* check if own node is addressed */
        if( NmTxMsgByteDest  == kNmCfg_EcuNr )
        {
          onmStateParam |= NM_MSG_RESET_ALIVE; 
        }
        else
        {
          onmStateParam |= NM_MSG_SKIPPED_ALIVE; 
        }
        #endif

        if( (NMVAR(nmAction) == NM_ACTION_LH_WAIT_LIMPHOME_TX) ||
            (NMVAR(nmAction) == NM_ACTION_LH_PREPARE_SLEEP   ) )
        {
          /* set TX counter, use as Limphome TX mark, reset only on limphome msg */
          NMVAR(nmRxTxCnt) = TX_CNT_STEP; 
        }
      }
    }
  }




  #if defined ( NM_ENABLE_ONM_EXT_API )
  OnmExtConfirmation( NM_CHANNEL_NMPARA_FIRST onmStateParam ); 
  #endif

}

/******************************************************************************
 * Name:            NmPrecopy
 * Called by:       CAN driver (RX interrupt)
 * Parameter:       RX object
 * Return value:    kCanNoCopyData
 * Description:     This API informs the NM about the reception of
 *                  a NM message.
 * Precondition:    ---
 *****************************************************************************/
  #if defined ( C_SINGLE_RECEIVE_BUFFER )
vuint8 NM_INTERNAL_CALL_TYPE NmPrecopy(CanReceiveHandle rcvObject)
{
  #endif
  #if defined ( C_MULTIPLE_RECEIVE_BUFFER )
vuint8 NM_INTERNAL_CALL_TYPE NmPrecopy(CanChipDataPtr rxDataPtr)
{
  #endif
  #if defined ( NM_ENABLE_INDEXED_DRVCAN ) || \
      defined ( C_SINGLE_RECEIVE_CHANNEL )
vuint8 NM_INTERNAL_CALL_TYPE NmPrecopy(CanRxInfoStructPtr rxStruct)
{
  #endif


  #if defined ( NM_ENABLE_ONM_EXT_API )
  NmMsgType onmStateParam = NM_MSG_INVALID; 
  #endif

    #if defined ( NM_ENABLE_INDEXED_NM )
      #if defined ( NM_ENABLE_CHANNEL_INDIRECTION )
  CanChannelHandle channel = NmCanToNmIndirection[rxStruct->Channel];
      #else
  CanChannelHandle channel = rxStruct->Channel;
      #endif
    #endif

  #if defined ( NM_ENABLE_CONFIG ) 
  vuint8 configByte;
  #endif


  #if defined ( C_SINGLE_RECEIVE_BUFFER )
    #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  rcvObject = rcvObject;
    #endif
  #endif


  /* get the source address from the received NM message valid until the next message will be received */
  NMVAR(nmRxNmMsgSourceAdr) = NmGetRxMsgSourceAdr();



  /*---------------------------------------------------------------------------*/
  if( (nmRxMsgFlagCmd == RING ) || (nmRxMsgFlagCmd == ALIVE) )
  /*---------------------------------------------------------------------------*/
  {
    /* ESCAN00007856 */

    nmStateNmMsgRecv = 1;

  #if defined ( NM_ENABLE_CONFIG )
    /*--- update NM node table ---*/
    if( NMVAR(nmRxNmMsgSourceAdr) <= ((vuint8)((NM_NUMBER_OF_CONFIG_NODES) - 1)) )
    {
      /* calculate the index for the config table */
      configByte = NmCalculateConfigByte(NMVAR(nmRxNmMsgSourceAdr));

      /* set bit of received NM node within NM node table */
      nmConfig.nmConfigChar[configByte] |= (vuint8)(1<<(NMVAR(nmRxNmMsgSourceAdr) & BYTE_OFFSET_MASK));
    }
  #endif

    /*--- determine logical successor ---*/
    if( NMVAR(nmLogicalSuccessor) > kNmCfg_EcuNr )
    {
      if( (NMVAR(nmRxNmMsgSourceAdr) < NMVAR(nmLogicalSuccessor)) &&
          (NMVAR(nmRxNmMsgSourceAdr) > kNmCfg_EcuNr ) )
      {
        NMVAR(nmLogicalSuccessor) = NMVAR(nmRxNmMsgSourceAdr);
      }
    }
    else
    {
      if( (NMVAR(nmRxNmMsgSourceAdr) < NMVAR(nmLogicalSuccessor)) ||
          (NMVAR(nmRxNmMsgSourceAdr) > kNmCfg_EcuNr ) )
      {
        NMVAR(nmLogicalSuccessor) = NMVAR(nmRxNmMsgSourceAdr);
      }
    }

    /*---------------------------------------------------------------------------*/
    if( nmRxMsgFlagCmd == RING ) /* RX RING */
    /*---------------------------------------------------------------------------*/
    {
  #if defined ( NM_ENABLE_ONM_EXT_API )
      onmStateParam |= NM_MSG_RING; 
  #endif

      nmStateRingMsgRecv = 1;

      /* check if own node got the token */
      if( (nmRxMsgFlagDestAdr == kNmCfg_EcuNr ) ||            /* own node addressed */
          (nmRxMsgFlagDestAdr == NMVAR(nmRxNmMsgSourceAdr)) ) /* sender addressed */
      {
  #if defined ( NM_ENABLE_ONM_EXT_API )
        onmStateParam |= NM_MSG_WAIT_TX; 
  #endif

        nmStateImAddressed = 1;

        /*--- copy used data ---*/

        if( nmRxMsgFlagDestAdr == kNmCfg_EcuNr ) /* own node addressed */
        {
    #if defined ( NM_ENABLE_RINGDATA_ACCESS )
          /* own node has the token => allow ring data access */
          nmStateTxRingDataAllow = 1;
    #endif


          if( nmStateRxTxLockUd == 0 ) /*  */
          {
            /* ESCAN00008104: protect send buffer: user data should not be copied until TX request was sent */

      #if defined ( NM_ENABLE_COPY_RINGDATA )
            /* OSEK mechanism: TX buffer is filled with RX buffer */

            NmTxMsgByteUser0 = nmRxMsgByteUser0;
            NmTxMsgByteUser1 = nmRxMsgByteUser1;
            NmTxMsgByteUser2 = nmRxMsgByteUser2;
            NmTxMsgByteUser3 = nmRxMsgByteUser3;
            NmTxMsgByteUser4 = nmRxMsgByteUser4;
            NmTxMsgByteUser5 = nmRxMsgByteUser5;
      #else

        #if defined ( NM_ENABLE_COPY_RINGDATA_TO_TEMPBUFFER )
            /* modified mechanism: RX buffer is copied to temporary buffer */
            NMVAR(nmUserDataBuffer)[0] = nmRxMsgByteUser0;
            NMVAR(nmUserDataBuffer)[1] = nmRxMsgByteUser1;
            NMVAR(nmUserDataBuffer)[2] = nmRxMsgByteUser2;
            NMVAR(nmUserDataBuffer)[3] = nmRxMsgByteUser3;
            NMVAR(nmUserDataBuffer)[4] = nmRxMsgByteUser4;
            NMVAR(nmUserDataBuffer)[5] = nmRxMsgByteUser5;
        #endif

            /* modified mechanism: TX buffer is filled with default values */

            NmTxMsgByteUser0 = DATA_INIT;
            NmTxMsgByteUser1 = DATA_INIT;
            NmTxMsgByteUser2 = DATA_INIT;
            NmTxMsgByteUser3 = DATA_INIT;
            NmTxMsgByteUser4 = DATA_INIT;
            NmTxMsgByteUser5 = DATA_INIT;
      #endif
          }
        }
      }
      else
      {
        /* own node not addressed */
        nmStateImAddressed = 0;

        /*--- check for skipped ---*/
      #if defined ( NM_INT_ENABLE_OPTIMIZED_SKIPPED_DETECTION )
        /* Code: Special use case: In this configuration, the kNmCfg_EcuNr is a preprocessor define with the value 0.
         *       This leads to some compiler warnings for the standard algorithm. 
         *       The warnings can be removed and the code can be optimized for this use case.*/
        /* ESCAN00020580 */
        if( NMVAR(nmRxNmMsgSourceAdr) >= nmRxMsgFlagDestAdr )
        {
          nmStateImSkippedOver = 1;
        }
      #else
        if( (vsint16)nmRxMsgFlagDestAdr < (vsint16)kNmCfg_EcuNr ) /* ESCAN00007275 */
        {
          if( (         NMVAR(nmRxNmMsgSourceAdr) >= nmRxMsgFlagDestAdr     ) &&
              ((vsint16)NMVAR(nmRxNmMsgSourceAdr) <  (vsint16)kNmCfg_EcuNr))
          {
            nmStateImSkippedOver = 1;
          }
        }
        else
        {
          if( NMVAR(nmRxNmMsgSourceAdr) < nmRxMsgFlagDestAdr )
          {
            if( (vsint16)NMVAR(nmRxNmMsgSourceAdr) < (vsint16)kNmCfg_EcuNr )
            {
              nmStateImSkippedOver = 1;
            }
          }
          else
          {
            nmStateImSkippedOver = 1;
          }
        }
      #endif
      }

    }
    /*---------------------------------------------------------------------------*/
    else /* RX ALIVE */
    /*---------------------------------------------------------------------------*/
    {
  #if defined ( NM_ENABLE_ONM_EXT_API )
      /* determine type of ALIVE message */
      if( nmRxMsgFlagDestAdr == NMVAR(nmRxNmMsgSourceAdr) )
      {
        onmStateParam |= NM_MSG_RESET_ALIVE; 
      }
      else
      {
        onmStateParam |= NM_MSG_SKIPPED_ALIVE; 
      }
  #endif

      /* ring is unstable */
      nmStateRingStable    = 0;
      nmStateRingPreStable = 0;

  #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
      /* ESCAN00007430 */
      /* nmStateRemoteSleepInd depends only on received sleep indication flags */
      /* Any NM message without set Sleep Indication resets the Remote Sleep Indication.
       * This will be done a few lines later.
       * A SkippedAlive with a set SleepInd does not influence nmStateRemoteSleepInd.
       * A StartupAlive always has no set SleepInd. */
  #endif
    }
  }
  else
  {
    /*---------------------------------------------------------------------------*/
    if( nmRxMsgFlagCmd == LIMPHOME ) /* RX LIMPHOME */
    /*---------------------------------------------------------------------------*/
    {
  #if defined ( NM_ENABLE_ONM_EXT_API )
      onmStateParam |= NM_MSG_LIMPHOME; 
  #endif

      nmStateNmMsgRecv = 1;
    }
  }

  /*---------------------------------------------------------------------------*/
  if( nmStateNmMsgRecv == 1 )  /* RX valid NM message */
  /*---------------------------------------------------------------------------*/
  {
  #if defined ( NM_ENABLE_ONM_EXT_API )
    if( nmRxMsgFlagSleepInd == SLEEP_IND )
    {
      onmStateParam |= NM_MSG_SLEEP_IND;
    }
  #endif

    /* NM message received => reset RX counter */
    NMVAR(nmRxTxCnt) &= (vuint8)(~(vuintx)RX_CNT_MASK);

    /* check if network sleep status is not given any more */
    if( (nmRxMsgFlagSleepInd != SLEEP_IND) || (nmStateBusSleep == 1) ) /* ESCAN00007436 */
    {
      nmStatePrepSleep = 0;

  #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
      nmStatePrepSendSleepAck = 0;
      NmTxMsgFlagSleepAck     = 0;
  #endif

  #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
      nmStateRemotePrepSleep = 0;
      nmStateRemoteSleepInd  = 0;
  #endif

    }

    /* RX SleepAck */
    if( nmRxMsgFlagSleepAck == SLEEP_ACK )
    {
  #if defined ( NM_ENABLE_ONM_EXT_API )
      onmStateParam |= NM_MSG_SLEEP_ACK;
  #endif

      if( (nmStateBusSleep == 0) && (nmStateWaitBusSleep == 0) ) /* ESCAN00007535 */
      {
        /* Start transition to BUSSLEEP */

        if( nmStateBusSleepInd == 1 )
        {
          /* RX SleepAck and sleep is allowed */

          /* stop TX path of CAN controller */
          CanOffline(NM_CHANNEL_CANPARA_ONLY);

          nmStatePrepSleep     = 1;
          nmStateWaitBusSleep  = 1;
          nmStateRxTxLockUd    = 0; /* ESCAN00008104 */
          NMVAR(nmTxRequest) = 0; /* cancel NM TX request */

  #if defined ( NM_ENABLE_RINGDATA_ACCESS )
          nmStateTxRingDataAllow = 0; /* disable user(ring)data access */
  #endif

  #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
          nmStateRemotePrepSleep = 1;
          nmStateRemoteSleepInd     = 1;
  #endif


          NMVAR(nmTimeCnt)  = NMCFG_COUNT_WAITBUSSLEEP;
          NMVAR(nmAction) = NM_ACTION_GO_BUSSLEEP;
        }
        else
        {
          /* RX SleepAck but sleep is not allowed */

          /* ESCAN00021051 */
          if( (NMVAR(nmAction) == NM_ACTION_LH_WAIT_LIMPHOME_TX) || 
              (NMVAR(nmAction) == NM_ACTION_LH_PREPARE_SLEEP) )
          {
            if( mmStateBusError == 1 )
            {
              mmStateBusError = 0;

              #if defined( C_MULTIPLE_RECEIVE_CHANNEL )
              /* macros must be encapsulated in this way */
              CanResetBusOffEnd(NM_CHANNEL_CANPARA_ONLY, kNmCfg_CanPara);
              #else
              CanResetBusOffEnd(kNmCfg_CanPara);
              #endif

              #if defined( NM_ENABLE_EXTENDED_CALLBACK )
              ApplNmBusOffEnd(NM_CHANNEL_APPLPARA_ONLY);
              #endif
            }
          }

  #if defined ( NM_ENABLE_IMMEDIATE_ALIVE )
          nmStatePrepSleep = 0;
          /* restart NM immediately */
          NMVAR(nmAction) = NM_ACTION_START; 
  #endif

  #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
          nmStatePrepSendSleepAck = 0;
          NmTxMsgFlagSleepAck     = 0;
  #endif

  #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
          nmStateRemotePrepSleep = 0;
          nmStateRemoteSleepInd     = 0;
  #endif

        }
      }
      else
      {
        /* NM is already in sleep mode => ignore SleepAck */
      }
    }
  }

  #if defined ( NM_ENABLE_ONM_EXT_API )
  OnmExtPrecopy( NM_CHANNEL_NMPARA_FIRST onmStateParam, nmRxMsgUserStartAdr );
  #endif

  /* no need for further processing of RX message:
   * - there is no buffer for the NM message => save ressources
   * - the message has been completely processed by the NM
   */
  return( kCanNoCopyData );
}


/******************************************************************************
 * Name:            NmOsekInit
 * Called by:       application
 * Parameter:       initMode: initialization parameter
 * Return value:    ---
 * Description:     This API initializes the NM
 * Precondition:    transceiver and CAN driver have to be initialized
 *****************************************************************************/
void NM_API_CALL_TYPE NmOsekInit(NM_CHANNEL_NMTYPE_FIRST NmInitType initMode )
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && \
      defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
    #if defined ( NM_ENABLE_MULTIPLE_NODES )
  CanChannelHandle sysChannel = channel;
    #endif
  channel = NmSystemToNmIndirection[channel];
  #endif

  CanInterruptDisable();

  nmStateByteGlobal = 0;
  nmStateByteLocal  = 0;

  NMVAR(nmTxRequest) = 0; /* ESCAN00004259 */

  #if defined ( NM_ENABLE_INDEXED_NM ) && defined ( NM_ENABLE_MULTIPLE_NODES )
  /* store configuration data in local variable for direct access and runtime optimization */
    #if defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
  kNmCfg_EcuNr = NmEcuNr_Field[channel][comMultipleECUCurrent[sysChannel]];
  kNmCfg_TxObj = NmTxObj_Field[channel][comMultipleECUCurrent[sysChannel]];
    #else
  kNmCfg_EcuNr = NmEcuNr_Field[channel][comMultipleECUCurrent[channel]];
  kNmCfg_TxObj = NmTxObj_Field[channel][comMultipleECUCurrent[channel]];
    #endif
  #endif


  #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION ) || \
      defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
  nmStateGwMasterFlags = 0;
  nmStateGwMasterDummy = 0;
  #endif

  NMVAR(nmTimeCnt) = 0;
  NMVAR(nmRxNmMsgSourceAdr) = kNmCfg_EcuNr;  /* initial value - self addressed */
  NMVAR(nmRxTxCnt) = 0x00;


  #if defined ( NM_ENABLE_COPY_RINGDATA_TO_TEMPBUFFER )
  /*
   * nmUserDataBuffer[0] = 0x00;
   * nmUserDataBuffer[1] = 0x00;
   * nmUserDataBuffer[2] = 0x00;
   * nmUserDataBuffer[3] = 0x00;
   * nmUserDataBuffer[4] = 0x00;
   * nmUserDataBuffer[5] = 0x00;
   * => no initialization needed, because array can only be accessed when ring is stable.
   *    Before the ring gets stable, a message must be received.
   *    This array is updated when a ring message is received.
   */
  #endif




  #if defined ( NM_ENABLE_FAST_BUSOFF_RECOVERY )
  NMVAR(nmBusOffCounter) = NMCFG_COUNT_SHORT_NUMBER;
  #endif






  #if defined ( NM_ENABLE_EARLY_BUSOFF_REINIT )
  /* ESCAN00022927 */
  NMVAR(nmDrvCanIsBusOff) = 0;
  #endif

  #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
  if( initMode != NM_SENDSLEEPACK )
  /* 
   * Perform default initialization only if not NM_SENDSLEEPACK.
   * Otherwise leave the NM message data to allow an asnchronous
   * SleepAck
   */
  #endif
  {
    /* initialize NM control bytes */
    NmTxMsgByte0 = CTRL_INIT;
    NmTxMsgByte1 = CTRL_INIT;


    /* initialize node configuration array (nmConfig) */
    NmResetNodeConfig(NM_CHANNEL_NMPARA_ONLY);
  }


  /* set default state => can be overwritten in the init-specific handling */
  NMVAR(nmAction) = NM_ACTION_START;

  /*------------------------
   * specific handling
   *------------------------*/
  switch( initMode )
  {
    /*---------------------------------*/
    case NM_NORMAL:
    /*---------------------------------*/
      nmStateActive       = 1;
   /* nmStateBusSleep     = 0; */
   /* nmStateBusSleepInd  = 0; */
   /* nmStatePrepSleep    = 0; */
      break;
    /*---------------------------------*/
    case NM_NMPASSIVE:
    /*---------------------------------*/
   /* nmStateActive       = 0; */
   /* nmStateBusSleep     = 0; */
   /* nmStateBusSleepInd  = 0; */
   /* nmStatePrepSleep    = 0; */
      break;
    /*---------------------------------*/
    case NM_SLEEPIND:
    /*---------------------------------*/
      nmStateActive       = 1;
   /* nmStateBusSleep     = 0; */
      nmStateBusSleepInd  = 1;
   /* nmStatePrepSleep    = 0; */

      break;
    /*---------------------------------*/
    case NM_SLEEPIND_NMPASSIVE:
    /*---------------------------------*/
   /* nmStateActive       = 0; */
   /* nmStateBusSleep     = 0; */
      nmStateBusSleepInd  = 1;
   /* nmStatePrepSleep    = 0; */
      break;
  #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
    /*---------------------------------*/
    case NM_SENDSLEEPACK:
    /*---------------------------------*/
      nmStateActive           = 1;
   /* nmStateBusSleep         = 0; */
      nmStateBusSleepInd      = 1;
      nmStatePrepSleep        = 1;

      nmStatePrepSendSleepAck = 1;

    #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
      nmStateRemotePrepSleep  = 1;
      nmStateRemoteSleepInd   = 1;
    #endif
      
      NMVAR(nmPrepareSleepCnt)  = 0;
      NmTxMsgFlagSleepAck       = 0;

      /*
       * Note:
       * nmStateImSkippedOver and/or nmStateRingMsgRecv 
       * are cleared during initialization.
       */

      if( NMVAR(nmLogicalSuccessor) == kNmCfg_EcuNr )
      {
        NMVAR(nmLogicalSuccessor) = (vuint8)(~kNmCfg_EcuNr);
      }
      /* else: leave the successor */

      NMVAR(nmAction) = NM_ACTION_WAIT_SENDTOKEN;
      break;
  #endif
    /*---------------------------------*/
    case NM_CANSLEEP:
    /*---------------------------------*/
      nmStateActive       = 1;
      nmStateBusSleep     = 1;
      nmStateBusSleepInd  = 1;
      nmStatePrepSleep    = 1;

      NMVAR(nmAction) = NM_ACTION_BUS_SLEEP;


  #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
      nmStatePrepSendSleepAck = 1;
      NmTxMsgFlagSleepAck     = 1;
  #endif

  #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
      nmStateRemotePrepSleep  = 1;
      nmStateRemoteSleepInd   = 1;
  #endif

      break;
    /*---------------------------------*/
    case NM_DISABLE:
      /* code: the break-statement is intentionally left out here */
    /*---------------------------------*/
    default :
    /*---------------------------------*/
      NMVAR(nmAction) = NO_ACTION;
      break;
  }

  #if defined ( NM_ENABLE_ONM_EXT_API )
    #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
  if( initMode != NM_SENDSLEEPACK )
  {
    OnmExtInit( NM_CHANNEL_NMPARA_ONLY );
  }
    #else
  OnmExtInit( NM_CHANNEL_NMPARA_ONLY );
    #endif
  #endif

  switch( NMVAR(nmAction) )
  {
    case NM_ACTION_START:
      /* notify application about start of network operation */
      ApplNmCanNormal( NM_CHANNEL_APPLPARA_ONLY );
      CanOnline( NM_CHANNEL_CANPARA_ONLY );
  #if defined ( NM_ENABLE_EXTENDED_CALLBACK )
      ApplNmBusStart( NM_CHANNEL_APPLPARA_ONLY );
  #endif
      break;
    case NM_ACTION_BUS_SLEEP:
      /* notify application about end of network operation */
      CanOffline(NM_CHANNEL_CANPARA_ONLY);
      ApplNmCanSleep( NM_CHANNEL_APPLPARA_ONLY );
      ApplNmCanBusSleep( NM_CHANNEL_APPLPARA_ONLY );
      break;
    default:
      /* do nothing */
      break;
  }

  CanInterruptRestore();
}


/******************************************************************************
 * Name:            NmTask
 * Called by:       application
 * Parameter:       (NM channel)
 * Return value:    ---
 * Description:     This API is the cyclic task function of the NM.
 *                  It contains the NM main state machine.
 * Precondition:    NM has to be initializes
 *                  Has to be called cyclically with a constant, 
 *                  predefined task cycle.
 *****************************************************************************/
void NM_API_CALL_TYPE NmTask(NM_CHANNEL_NMTYPE_ONLY)
{
  #if defined ( NM_ENABLE_ONM_EXT_API )
  NmMsgType onmStateParam; 
  #endif


  #if defined ( NM_ENABLE_INDEXED_NM ) && \
      defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
  channel = NmSystemToNmIndirection[channel];
  #endif

  CanInterruptDisable();

  #if defined ( NM_ENABLE_EARLY_BUSOFF_REINIT )
  /* ESCAN00022927 */
  if( NMVAR(nmDrvCanIsBusOff) != 0 )
  {
    /* check if DrvCan indicates that it has recovered from BusOff, i.e. it is ready for re-init */
    if( CanGetDriverStatus(NM_CHANNEL_CANPARA_ONLY) != kCanDriverBusoff )
    {
      NMVAR(nmDrvCanIsBusOff) = 0;

      #if defined ( NM_ENABLE_INDEXED_DRVCAN )
      /* code: macros must be encapsulated in this way */
      CanResetBusOffEnd(NM_CHANNEL_CANPARA_ONLY, kNmCfg_CanPara);
      #else
      CanResetBusOffEnd(kNmCfg_CanPara);
      #endif
    }
  }
  #endif


  /* update task counter */
  if( NMVAR(nmTimeCnt) > 0 )
  {
    NMVAR(nmTimeCnt)--;
  }

  CanInterruptRestore();

  /* code: loop for immediate switch to next state in time-critical cases */
  do
  {
    #if defined ( NM_ENABLE_ONM_EXT_API )
    onmStateParam = NM_MSG_INVALID; 
    #endif

    CanInterruptDisable();

    NMVAR(nmAction) &= (NmActionType)(~(vuintx)NM_ACTION_REPEAT);

    /* check NM TX counter */
    if( (vuint8)((NMVAR(nmRxTxCnt) & TX_CNT_MASK)>>4) > TX_CNT_MAX )
    {
      /* TX counter run off -> start limphome */
      NMVAR(nmRxTxCnt) = TX_CNT_STEP; /* use TX counter as limphome mark */
      NMVAR(nmTimeCnt) = NMCFG_COUNT_LIMPHOME;
      NMVAR(nmAction) = NM_ACTION_LH_WAIT_LIMPHOME_TX;
    }

    /*--- NM state machine ---*/
    switch( NMVAR(nmAction) )
    {
     /*----------------------------------------------------------*/
      case NM_ACTION_START:
     /*----------------------------------------------------------*/
        #if defined ( NM_ENABLE_ONM_EXT_API )
        onmStateParam |= (NmMsgType)(NM_MSG_RESET_ALIVE | NM_MSG_WAIT_TX); 
        #endif

        NmResetNodeConfig( NM_CHANNEL_NMPARA_ONLY );

        /* reset internal status bytes */ 
        nmStateByteLocal        = 0;

        /* reset global status bytes that do not depend on application */

        nmStateRingStable       = 0;
        mmStateBusError         = 0; /* ESCAN00009711 */
     /* nmStateActive           = 0; => do not reset */
        nmStateLimpHome         = 0;
        nmStateBusSleep         = 0;
        nmStateWaitBusSleep     = 0; /* ESCAN00009420 */
        nmStateTxRingDataAllow  = 0;
     /* nmStateBusSleepInd      = 0; => do not reset */

        NMVAR(nmTxRequest) = 0;  /* ESCAN00004259 */
        NMVAR(nmRxNmMsgSourceAdr) = kNmCfg_EcuNr; /* initial value: self-addressed */
        NMVAR(nmRxTxCnt) += RX_CNT_STEP; /* increment RX counter */

        /* unlock TX path */
        CanOnline(NM_CHANNEL_CANPARA_ONLY); /* ESCAN00004328 */

        #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
        nmStatePrepSendSleepAck = 0;
        NmTxMsgFlagSleepAck     = 0;
        #endif

        #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
        nmStateRemotePrepSleep = 0;
        nmStateRemoteSleepInd  = 0;
        #endif


        if( nmStateActive == 1 )
        { 
          #if defined ( NM_ENABLE_ONM_EXT_API )
          onmStateParam |= NM_MSG_START_TX; 
          #endif

          nmStateRxTxLockUd  = 1; /* ESCAN00008104 - wait for NmConfirmation() */
          NMVAR(nmTxRequest) = 1; /* request TX of ALIVE message */

          NMVAR(nmRxTxCnt) += TX_CNT_STEP; /* increment TX counter */

          /*--- build ALIVE message ---*/

          NMVAR(nmLogicalSuccessor) = kNmCfg_EcuNr;
          NmTxMsgByteDest           = kNmCfg_EcuNr;
          NmTxMsgFlagCmdSet         = ALIVE;
          NmTxMsgFlagSleepAck       = 0;
          NmTxMsgFlagSleepInd       = 0;

          /* initialize user data */
          NmTxMsgByteUser0 = DATA_INIT;
          NmTxMsgByteUser1 = DATA_INIT;
          NmTxMsgByteUser2 = DATA_INIT;
          NmTxMsgByteUser3 = DATA_INIT;
          NmTxMsgByteUser4 = DATA_INIT;
          NmTxMsgByteUser5 = DATA_INIT;
        }

        /* check NM RX counter */
        if( (NMVAR(nmRxTxCnt) & RX_CNT_MASK) <= RX_CNT_MAX )
        {
          NMVAR(nmTimeCnt) = (vuint8)(NMCFG_COUNT_RINGTYP + 1);
          NMVAR(nmAction) = NM_ACTION_WAIT_SENDTOKEN;
        }
        else
        {
          /* TX counter run off -> start limphome */
          NMVAR(nmRxTxCnt) = TX_CNT_STEP; /* use TX counter as limphome mark */

          NMVAR(nmTimeCnt) = NMCFG_COUNT_LIMPHOME;
          NMVAR(nmAction)  = NM_ACTION_LH_WAIT_LIMPHOME_TX;
        }

        break;

     /*----------------------------------------------------------*/
      case NM_ACTION_WAIT_SENDTOKEN:
     /*----------------------------------------------------------*/
        #if defined ( NM_ENABLE_ONM_EXT_API )
        onmStateParam |= (NmMsgType)(NM_MSG_RING | NM_MSG_WAIT_TX); 
        #endif

        if( nmStateActive == 0 )
        {
          /* NmPassive */
          nmStatePrepSleep = nmStateBusSleepInd;

          #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
          /* remote sleep indication is not defined in passive mode */
          #endif


          /* there is no TX is passive mode => change to waiting state */
          NMVAR(nmTimeCnt) = NMCFG_COUNT_RINGMAX;
          NMVAR(nmAction)  = NM_ACTION_WAIT_RINGMSG;
        }
        else
        {
          /* NmActive */

          /* check for skipped condition */
          if( nmStateImSkippedOver == 1 )
          {
            /*
             * code: don't change the counter. 
             *       The own NM message might be transmitted now or by the next task call,
             */

            #if defined ( NM_ENABLE_ONM_EXT_API )
            onmStateParam |= (NmMsgType)(NM_MSG_SKIPPED_ALIVE | NM_MSG_START_TX); 
            #endif

            /* transmit SKIPPED ALIVE message */
            NmTransmitSkippedAlive(NM_CHANNEL_NMPARA_ONLY);
          }

          /* check for NM message reception */
          if( nmStateRingMsgRecv == 1 )
          {
            if( nmStateImAddressed == 1 )
            {
              /* refresh timer */
              NMVAR(nmTimeCnt) = NMCFG_COUNT_RINGTYP;
            }
            else
            {
              /* another node received the token => cancel own transmit token */
              NMVAR(nmTimeCnt) = NMCFG_COUNT_RINGMAX;
              NMVAR(nmAction) = NM_ACTION_WAIT_RINGMSG;
            }
          }
          else
          {
            if( NMVAR(nmTimeCnt) == 0 )
            {
              /*--- transmit RING message ---*/

              #if defined ( NM_ENABLE_ONM_EXT_API )
              onmStateParam |= NM_MSG_START_TX; 
              #endif

              #if defined ( NM_ENABLE_RINGDATA_ACCESS )
              /* token is passed on => disable access on userdata */
              nmStateTxRingDataAllow = 0;
              #endif

              nmStateRxTxLockUd  = 1; /* ESCAN00008104 - wait for NmConfirmation() */
              NMVAR(nmTxRequest) = 1; /* request TX of RING message */

              NMVAR(nmRxTxCnt) += TX_CNT_STEP; /* increment NM TX counter */

              /*--- determine status of flags SleepInd and SleepAck ---*/
              if( NMVAR(nmLogicalSuccessor) != kNmCfg_EcuNr )
              {
                /* check if SleepAck should be transmitted */
                if( (nmStatePrepSleep == 1) && (nmStateBusSleepInd == 1) )
                {
                  #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
                  /* let application decide if SleepAck should be set */
                  if( ApplNmSendSleepAck(NM_CHANNEL_APPLPARA_ONLY) )
                  {
                    NmTxMsgFlagSleepAck = 1;
                  }
                  else
                  {
                    NmTxMsgFlagSleepAck = 0;
                  }

                  nmStatePrepSendSleepAck = 1;
                  #else
                  /* set SleepAck */
                  NmTxMsgFlagSleepAck = 1;
                  #endif
                }
                else
                {
                  NmTxMsgFlagSleepAck = 0;

                }

                /* bit SleepInd is directly derived from the application request */
                NmTxMsgFlagSleepInd = nmStateBusSleepInd;
              }
              else
              {
                /* there is no SleepInd and no SleepAck if the node addresses itself */
                NmTxMsgFlagSleepAck = 0;
                NmTxMsgFlagSleepInd = 0;
              }

              /*--- check stability of ring ---*/
              if( nmStateRingPreStable == 1 )
              {
                nmStateRingStable = 1;

              }
              else
              {
                nmStateRingPreStable = 1;
              }

              #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
              /* process remote sleep indication */
              if( nmStateRemotePrepSleep == 1 )
              {
                nmStateRemoteSleepInd = 1;
              }
              else
              {
                nmStateRemotePrepSleep = 1;
              }
              #endif

              /* build RING message */
              NmTxMsgFlagCmdSet = RING;
              NmTxMsgByteDest   = NMVAR(nmLogicalSuccessor);


              /* state transition */
              NMVAR(nmTimeCnt) = NMCFG_COUNT_RINGMAX;
              NMVAR(nmAction)  = NM_ACTION_WAIT_RINGMSG;
            }
          }
        }
        break;
     /*----------------------------------------------------------*/
      case NM_ACTION_WAIT_RINGMSG:
     /*----------------------------------------------------------*/
        #if defined ( NM_ENABLE_ONM_EXT_API )
        onmStateParam |= (NmMsgType)(NM_MSG_RING | NM_MSG_WAIT_RX); 
        #endif

        /* check for received NM message */
        if( nmStateRingMsgRecv == 1 )
        {
          #if defined ( NM_ENABLE_ONM_EXT_API )
          onmStateParam |= NM_MSG_RECEIVED; 
          #endif

          if( (nmStateActive == 1) && (nmStateImAddressed == 1) )
          {
            /* token received */
            #if defined ( NM_ENABLE_ONM_EXT_API )
            onmStateParam |= NM_MSG_TOKEN_RECEIVED; 
            #endif

            #if defined ( NM_ENABLE_RINGDATA_ACCESS )
            if( nmStateRingStable == 1 )
            {
              /* inform application about possibility to access ringdata */
              ApplNmIndRingData(NM_CHANNEL_APPLPARA_ONLY);
            }
            #endif

            /* state transition */
            NMVAR(nmTimeCnt)  = NMCFG_COUNT_RINGTYP;
            NMVAR(nmAction) = NM_ACTION_WAIT_SENDTOKEN;
          }
          else
          {
            /* refresh timer */
            NMVAR(nmTimeCnt) = NMCFG_COUNT_RINGMAX;
          }
        }

        if( nmStateActive == 1 )
        {
          /* check for skipped condition */
          if( nmStateImSkippedOver == 1 )
          {
            /*
             * code: don't change the counter. 
             *       The own NM message might be transmitted now or by the next task call,
             */

            #if defined ( NM_ENABLE_ONM_EXT_API )
            onmStateParam |= (NmMsgType)(NM_MSG_SKIPPED_ALIVE | NM_MSG_WAIT_TX | NM_MSG_START_TX); 
            #endif

            /* transmit SKIPPED ALIVE message */
            NmTransmitSkippedAlive(NM_CHANNEL_NMPARA_ONLY);
          }
        }
        else
        {
          /* NmPassive */

          /*--- check stability of ring ---*/
          if( nmStateRingPreStable == 1 )
          {
            nmStateRingStable = 1;
          }
          else
          {
            nmStateRingPreStable = 1;
          }

          nmStatePrepSleep = nmStateBusSleepInd;

          #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
          /* remote sleep indication is not defined in passive mode */
          #endif

        }

        if( NMVAR(nmTimeCnt) == 0 )
        { 
          /* RX timeout */
          #if defined ( NM_ENABLE_ONM_EXT_API )
          onmStateParam |= NM_MSG_TIMEOUT_RX; 
          #endif

          /* state transition */
          NMVAR(nmAction) = NM_ACTION_START;
        }
        break;
     /*----------------------------------------------------------*/
      case NM_ACTION_LH_WAIT_LIMPHOME_TX:
      case NM_ACTION_LH_PREPARE_SLEEP:
     /*----------------------------------------------------------*/
        NmResetNodeConfig(NM_CHANNEL_NMPARA_ONLY);
        nmStateLimpHome = 1;

        NMVAR(nmRxTxCnt) &= (vuint8)(~(vuintx)RX_CNT_MASK);  /* reset NM RX counter */

        #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
        /* 
         * If a gateway is in limphome, other networks should not be hindered 
         * to enter sleep mode.
         */
        nmStateRemotePrepSleep = 1;
        nmStateRemoteSleepInd  = 1;
        #endif

        /* remember if a NM message has been received while in limphome */
        if( nmStateNmMsgRecv == 1 )
        {
          nmStateNmMsgRecvRemind = 1;
        }

        if( (nmStateNmMsgRecvRemind == 1) && 
            ((nmStateActive == 0) || (NMVAR(nmRxTxCnt) == 0)) )
        {
          NMVAR(nmRxTxCnt) = 0; /* reset NM RX and TX counter */

          /* state transition (time-critical) */
          NMVAR(nmAction) = (NmActionType)(NM_ACTION_START | NM_ACTION_REPEAT);
        }
        else
        {
         /*----------------------------------------------------------
           NM_ACTION_LH_WAIT_LIMPHOME_TX
          *----------------------------------------------------------*/
          if( NMVAR(nmAction) == NM_ACTION_LH_WAIT_LIMPHOME_TX )
          {
            #if defined ( NM_ENABLE_ONM_EXT_API )
            onmStateParam |= (NmMsgType)(NM_MSG_LIMPHOME | NM_MSG_WAIT_TX); 
            #endif

            if( NMVAR(nmTimeCnt) == 0 )
            {
              /* check if there is was a BusOff */
              if( mmStateBusError == 1 )
              {
                mmStateBusError = 0;

                /* recover BusOff */ /* ESCAN00022927 */
                #if defined ( NM_ENABLE_EARLY_BUSOFF_REINIT )
                if ( NMVAR(nmDrvCanIsBusOff) != 0 )
                {
                  NMVAR(nmDrvCanIsBusOff) = 0;
                #else
                {
                #endif
                  #if defined ( NM_ENABLE_INDEXED_DRVCAN )
                  /* code: macros must be encapsulated in this way */
                  CanResetBusOffEnd( NM_CHANNEL_CANPARA_ONLY, kNmCfg_CanPara );
                  #else
                  CanResetBusOffEnd( kNmCfg_CanPara );
                  #endif
                }

                #if defined ( NM_ENABLE_EXTENDED_CALLBACK )
                ApplNmBusOffEnd(NM_CHANNEL_APPLPARA_ONLY);
                #endif
              }

              CanOnline(NM_CHANNEL_CANPARA_ONLY);

              if( nmStateActive == 1 )
              {
                /* transmit LIMPHOME message */

                #if defined ( NM_ENABLE_ONM_EXT_API )
                onmStateParam |= NM_MSG_START_TX; 
                #endif

                nmStatePrepSleep = nmStateBusSleepInd;
                nmStateRxTxLockUd = 1; /* ESCAN00008104 - wait for NmConfirmation() */
                NMVAR(nmTxRequest) = 1; /* request transmission of LimpHome message */

                NMVAR(nmRxTxCnt) = TX_CNT_STEP;  /* set TX counter as limphome mark */

                #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
                /* remote sleep indication depends only on received sleep indication flags */
                #endif


                /* build LIMPHOME message */
                NmTxMsgByteDest      = NMVAR(nmLogicalSuccessor);
                NmTxMsgFlagCmdSet    = LIMPHOME;
                NmTxMsgFlagSleepAck  = 0; /* ESCAN00007382 */
                NmTxMsgFlagSleepInd = nmStateBusSleepInd;

              }

              NMVAR(nmTimeCnt) = NMCFG_COUNT_LIMPHOME;
            }

            if( nmStateActive == 0 )
            {
              nmStatePrepSleep = nmStateBusSleepInd;

              #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
              /* remote sleep indication is not defined in passive mode */
              #endif

            }

            if( nmStatePrepSleep == 1 )
            {

              /* state transition (time-critical) */
              NMVAR(nmTimeCnt) = NMCFG_COUNT_RINGMAX;
              NMVAR(nmAction) = (NmActionType)(NM_ACTION_LH_PREPARE_SLEEP | NM_ACTION_REPEAT);
            }
          }
          /*----------------------------------------------------------
            NM_ACTION_LH_PREPARE_SLEEP
           *----------------------------------------------------------*/
          else
          {
            if( nmStatePrepSleep == 1 )
            {

              if( nmStateRingMsgRecv == 1 )
              {
                NMVAR(nmTimeCnt) = NMCFG_COUNT_RINGMAX; /* refresh timer */
              }
              else
              {
                if( NMVAR(nmTimeCnt) == 0 )
                {
                  /* start sleep sequence */

                  CanOffline(NM_CHANNEL_CANPARA_ONLY);

                  nmStateWaitBusSleep = 1;
                  nmStateRxTxLockUd   = 0; /* ESCAN00008104 */
                  NMVAR(nmTxRequest)  = 0; /* cancel NM TX request */


                  if( mmStateBusError == 1 ) /* PRQA S 0715 */ /* 2008-02-05, vismas: changes in algo are critical */
                  {
                    mmStateBusError = 0;

                    /* recover BusOff */

                    #if defined ( NM_ENABLE_INDEXED_DRVCAN )
                    /* code: macros must be encapsulated in this way */
                    CanResetBusOffEnd(NM_CHANNEL_CANPARA_ONLY, kNmCfg_CanPara);
                    #else
                    CanResetBusOffEnd(kNmCfg_CanPara);
                    #endif

                    #if defined ( NM_ENABLE_EXTENDED_CALLBACK )
                    ApplNmBusOffEnd(NM_CHANNEL_APPLPARA_ONLY);
                    #endif
                  }

                  /* state transition */
                  NMVAR(nmTimeCnt) = NMCFG_COUNT_WAITBUSSLEEP;
                  NMVAR(nmAction) = NM_ACTION_GO_BUSSLEEP;
                }
              }
            }
            else
            {
              /* application wakeup */
              nmStateNmMsgRecvRemind = 0;

              /* ESCAN00022144 - unlock CAN-Transmit path immediately */
              CanOnline(NM_CHANNEL_CANPARA_ONLY);

              NMVAR(nmTimeCnt) = NMCFG_COUNT_LIMPHOME;  /* stop PREPARE_SLEEP */
              NMVAR(nmAction) = NM_ACTION_LH_WAIT_LIMPHOME_TX;
            }
          }
        }
        break;
      /*----------------------------------------------------------*/
      case NM_ACTION_GO_BUSSLEEP:
      /*----------------------------------------------------------*/
        NmAssert( nmStateWaitBusSleep != 0, kNmAssertWrongFlagWaitBusSleep );
        NmAssert( nmStateBusSleep == 0, kNmAssertWrongFlagBusSleep );

        NmResetNodeConfig( NM_CHANNEL_NMPARA_ONLY );
        nmStateNmMsgRecvRemind = 0;


        NMVAR(nmRxTxCnt) = 0;

        #if defined ( NM_ENABLE_EXTENDED_CALLBACK )
        if( NMVAR(nmTimeCnt) == ((vuint8)(NMCFG_COUNT_WAITBUSSLEEP - 1)) )
        {
          /* inform application */
          ApplNmWaitBusSleep(NM_CHANNEL_APPLPARA_ONLY);
        }
        #endif

        /* check sleep condition */
        if( nmStatePrepSleep == 0 )
        {
          /* 
           * sleep condition not true anymore
           * => - GotoMode(Awake) called or
           *    - RX of NM message with SleepInd=0 *
           * => abort sleep sequence 
           */

          nmStateWaitBusSleep = 0;  /* leave the context, because of possible WakeUp detection in GotoMode() */

          /* ESCAN00008463 - unlock CAN-Transmit path immediately */
          CanOnline(NM_CHANNEL_CANPARA_ONLY);


          /* check if LimpHome was active before the sleep sequence */
          if( nmStateLimpHome == 1 )
          {
            NMVAR(nmRxTxCnt) = TX_CNT_STEP;  /* use TX counter as limphome mark */

            /* update time counter */
            #if defined ( NM_ENABLE_FAST_BUSOFF_RECOVERY )
            if( (mmStateBusError == 1) && (NMVAR(nmBusOffCounter) > 0) ) /* ESCAN00010498 */
            {
              NMVAR(nmTimeCnt) = NMCFG_COUNT_LIMPHOME_SHORT;
            } 
            else
            {
              NMVAR(nmTimeCnt) = NMCFG_COUNT_LIMPHOME;
            }
            #else
            NMVAR(nmTimeCnt)   = NMCFG_COUNT_LIMPHOME;
            #endif

            /* state transition */
            NMVAR(nmAction)  = NM_ACTION_LH_WAIT_LIMPHOME_TX;

          }
          else
          {
            /* state transition (time-critical) */
            NMVAR(nmAction) = (NmActionType)(NM_ACTION_START | NM_ACTION_REPEAT);
          }

          #if defined ( NM_ENABLE_EXTENDED_CALLBACK )
          /* inform application that NmWaitBusSleep is left */
          ApplNmWaitBusSleepCancel(NM_CHANNEL_APPLPARA_ONLY);
          #endif

          /* note: If NmWaitBusSleep is left towards NmLimpHome, the CAN TX path is offline until the next task cycle */
        }
        else
        {
          /* sleep condition is still true */

          if( NMVAR(nmTimeCnt) == 1 )
          {
            CanOffline(NM_CHANNEL_CANPARA_ONLY);

            nmStateRxTxLockUd  = 0; /* ESCAN00008104 */
            NMVAR(nmTxRequest) = 0; /* cancel any NM TX request */

            /* initialize CAN-Chip to clear TX requests */
            #if defined ( NM_ENABLE_INDEXED_DRVCAN )
            /* code: macros must be encapsulated in this way */
            CanResetBusSleep((vuint8)NM_CHANNEL_CANPARA_ONLY, kNmCfg_CanPara);
            #else
            CanResetBusSleep(kNmCfg_CanPara);
            #endif
          }
          else
          {
            if( NMVAR(nmTimeCnt) == 0 )
            {
              nmStateWaitBusSleep = 0;
              nmStateBusSleep     = 1;

              /* inform application */
              ApplNmCanSleep(NM_CHANNEL_APPLPARA_ONLY);
              ApplNmCanBusSleep(NM_CHANNEL_APPLPARA_ONLY);

              /* state transition */
              NMVAR(nmAction) = NM_ACTION_BUS_SLEEP;
            }
          }
        }
        break;
      /*----------------------------------------------------------*/
      case NM_ACTION_BUS_SLEEP:
      /*----------------------------------------------------------*/
        NmAssert( nmStateWaitBusSleep == 0, kNmAssertWrongFlagWaitBusSleep );
        NmAssert( nmStateBusSleep != 0, kNmAssertWrongFlagBusSleep );

        nmStateLimpHome = 0;

        /* check sleep condition */
        if( nmStatePrepSleep == 0 ) /* ESCAN00007436 */
        {
          /* 
           * sleep condition not true anymore
           * => - GotoMode(Awake) called or
           *    - RX of any NM message *
           * => abort sleep
           */
          nmStateBusSleep = 0;


          /* inform application to enable transceiver and to wake CAN controller */
          ApplNmCanNormal(NM_CHANNEL_APPLPARA_ONLY);
          /* unlock CAN TX path */
          CanOnline(NM_CHANNEL_CANPARA_ONLY);
          #if defined ( NM_ENABLE_EXTENDED_CALLBACK )
          ApplNmBusStart(NM_CHANNEL_APPLPARA_ONLY);
          #endif

          /* state transition (time-critical) */
          NMVAR(nmAction) = (NmActionType)(NM_ACTION_START | NM_ACTION_REPEAT);
        }
        break;

      /*----------------------------------------------------------*/
      case NO_ACTION:
      case NM_ACTION_REPEAT:
      default :
      /*----------------------------------------------------------*/
        NmResetNodeConfig( NM_CHANNEL_NMPARA_ONLY );

        /* reset all status flags */
        nmStateByteGlobal   = 0;
        nmStateByteLocal    = 0;

        NMVAR(nmTxRequest)  = 0; /* ESCAN00004259 */

        NMVAR(nmTimeCnt)    = 0;
        NMVAR(nmRxTxCnt)    = 0;
        NMVAR(nmRxNmMsgSourceAdr) = kNmCfg_EcuNr;  /* initial value - self addressed */

        #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
        nmStatePrepSendSleepAck = 0;
        NmTxMsgFlagSleepAck     = 0;
        #endif

        #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
        nmStateRemotePrepSleep = 0;
        nmStateRemoteSleepInd  = 0;
        #endif
        break;
    } /* switch(NMVAR(nmAction)) */


    #if defined ( NM_ENABLE_ONM_EXT_API )
    OnmExtTask(NM_CHANNEL_NMPARA_FIRST onmStateParam); 
    #endif

    NmResetNmStatusRxFlags();

    CanInterruptRestore();

  /*----------------------------------------------------------*/
  } while( (NMVAR(nmAction) & NM_ACTION_REPEAT) != 0 ); 
  /*----------------------------------------------------------*/

  /*-------------------------------------
   * transmission handling 
   *-------------------------------------*/
  if( (NMVAR(nmTxRequest) == 1) && (nmStateActive == 1) )
  {
    NmAssert( CanGetStatus(NM_CHANNEL_CANPARA_ONLY) == kCanTxOn, kNmAssertCanIsOffline );




    //if( CanTransmit(kNmCfg_TxObj) == kCanTxOk )
    //{
    //  NMVAR(nmTxRequest) = 0;
//
    //}
    //else
    //{
    //}
  }
}

/******************************************************************************
 * Name:            GotoMode
 * Called by:       application
 * Parameter:       (NM channel)
 *                  mode  -Awake    application needs communication
 *                        -BusSleep application is ready for sleep
 * Return value:    ---
 * Description:     This API informs the NM about the communication needs of 
 *                  the application.
 * Precondition:    ---
 *****************************************************************************/
void NM_API_CALL_TYPE GotoMode( NM_CHANNEL_NMTYPE_FIRST NMModeName mode )
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && \
      defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
  channel = NmSystemToNmIndirection[channel];
  NmAssert( channel != 0xFF, kNmAssertInvalidChannel );
  #endif

  CanInterruptDisable();

  if( mode == BusSleep )
  {
    nmStateBusSleepInd = 1;
  }
  else /* code: enum-value. => mode==Awake */
  {

    #if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
    nmStatePrepSendSleepAck = 0;
    NmTxMsgFlagSleepAck = 0;
    #endif

    #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
    /* nmStateRemotePrepSleep is not affected */
    /* nmStateRemoteSleepInd is not affected */
    #endif

    nmStateBusSleepInd = 0;
    nmStatePrepSleep   = 0;
  }

  CanInterruptRestore();
}


/******************************************************************************
 * Name:            GetStatus
 * Called by:       application
 * Parameter:       (NM channel)
 *                  *dest   pointer to application-specific variable
 * Return value:    ---
 * Description:     This API copies the current NM state into an 
 *                  application-specific variable.
 * Precondition:    ---
 *****************************************************************************/
void NM_API_CALL_TYPE GetStatus( NM_CHANNEL_NMTYPE_FIRST NmStatusType *dest )
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && \
      defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
  channel = NmSystemToNmIndirection[channel];
  NmAssert( channel != 0xFF, kNmAssertInvalidChannel );
  #endif

  *dest = (NmStatusType)(nmStateByteGlobal);
}


/******************************************************************************
 * Name:            NmGetStatus
 * Called by:       application
 * Parameter:       (NM channel)
 * Return value:    state of NM
 * Description:     This API returns the current NM state.
 * Precondition:    ---
 *****************************************************************************/
NmStatusType NM_API_CALL_TYPE NmGetStatus( NM_CHANNEL_NMTYPE_ONLY )
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && \
      defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
  channel = NmSystemToNmIndirection[channel];
  NmAssert( channel != 0xFF, kNmAssertInvalidChannel );
  #endif

  return( (NmStatusType)(nmStateByteGlobal) );
}


/******************************************************************************
 * Name:            StartNM
 * Called by:       application
 * Parameter:       (NM channel)
 * Return value:    ---
 * Description:     This API starts the NM.
 * Precondition:    ---
 *****************************************************************************/
void NM_API_CALL_TYPE StartNM ( NM_CHANNEL_NMTYPE_ONLY )
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && \
      defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
  /* Code: No channel indirection needed, as only external API is called.
           Channel indirection is handled within that funtion. */
  #endif

  NmOsekInit( NM_CHANNEL_NMPARA_FIRST NM_NORMAL );
}


/******************************************************************************
 * Name:            StopNM
 * Called by:       application
 * Parameter:       (NM channel)
 * Return value:    ---
 * Description:     This API stops the NM.
 * Precondition:    ---
 *****************************************************************************/
void NM_API_CALL_TYPE StopNM( NM_CHANNEL_NMTYPE_ONLY )
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && \
      defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
  /* Code: No channel indirection needed, as only external API is called.
           Channel indirection is handled within that funtion. */
  #endif

  NmOsekInit( NM_CHANNEL_NMPARA_FIRST NM_DISABLE );
}


/******************************************************************************
 * Name:            TalkNM
 * Called by:       application
 * Parameter:       (NM channel)
 * Return value:    ---
 * Description:     This API sets the NM into active mode, i.e. the NM
 *                  can transmit NM messages and take part in the logical ring.
 * Precondition:    ---
 *****************************************************************************/
void NM_API_CALL_TYPE TalkNM( NM_CHANNEL_NMTYPE_ONLY )
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && \
      defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
  channel = NmSystemToNmIndirection[channel];
  NmAssert( channel != 0xFF, kNmAssertInvalidChannel );
  #endif

  CanInterruptDisable();
  nmStateActive = 1;
  CanInterruptRestore();
}


/******************************************************************************
 * Name:            SilentNM
 * Called by:       application
 * Parameter:       (NM channel)
 * Return value:    ---
 * Description:     This API sets the NM into passive mode, i.e. the NM
 *                  cannot transmit NM messages. It only monitors the logical ring.
 * Precondition:    ---
 *****************************************************************************/
void NM_API_CALL_TYPE SilentNM( NM_CHANNEL_NMTYPE_ONLY )
{
  #if defined ( NM_ENABLE_INDEXED_NM ) && \
      defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
  channel = NmSystemToNmIndirection[channel];
  NmAssert( channel != 0xFF, kNmAssertInvalidChannel );
  #endif

  CanInterruptDisable();
  nmStateActive = 0;
  CanInterruptRestore();
}


#if defined ( NM_ENABLE_RINGDATA_ACCESS )
/******************************************************************************
 * Name:            ReadRingData
 * Called by:       application
 * Parameter:       (NM channel)
 *                  *ringData   pointer to a user-specific array where the
 *                              user data have to be copied in
 * Return value:    - E_NotOK   access on ring data not possible because
 *                              - received ring data has not been stored
 *                              - ring is not stable
 *                  - E_OK      array 'ringData' has been updated with data 
 *                              from the NM message
 * Description:     This API copies the last received user data into an
 *                  application-specific buffer <ringData>.
 * Precondition:    ---
 *****************************************************************************/
StatusType NM_API_CALL_TYPE ReadRingData( NM_CHANNEL_NMTYPE_FIRST vuint8* ringData )
{
  StatusType rval;

  #if defined ( NM_ENABLE_INDEXED_NM ) && \
      defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
  channel = NmSystemToNmIndirection[channel];
  NmAssert( channel != 0xFF, kNmAssertInvalidChannel );
  #endif

  if( nmStateRingStable == 0 )
  {
    /* ring is not stable => ringData is not valid */
    rval = E_NotOK;
  }
  else
  {
  #if defined ( NM_ENABLE_COPY_RINGDATA )
    /* 
     * normal OSEK mechanism
     * received ring data was copied to NM TX buffer
     * => get it from there
     */
    CanInterruptDisable();
    *(ringData    ) = NmTxMsgByteUser0;
    *(ringData + 1) = NmTxMsgByteUser1;
    *(ringData + 2) = NmTxMsgByteUser2;
    *(ringData + 3) = NmTxMsgByteUser3;
    *(ringData + 4) = NmTxMsgByteUser4;
    *(ringData + 5) = NmTxMsgByteUser5;
    CanInterruptRestore();

    rval = E_OK;
  #elif defined ( NM_ENABLE_COPY_RINGDATA_TO_TEMPBUFFER )
    /* 
     * modified mechanism:
     * received ring data was copied to specific NM buffer
     * => get it from there
     */
    CanInterruptDisable();
    *(ringData    ) = NMVAR(nmUserDataBuffer)[0];
    *(ringData + 1) = NMVAR(nmUserDataBuffer)[1];
    *(ringData + 2) = NMVAR(nmUserDataBuffer)[2];
    *(ringData + 3) = NMVAR(nmUserDataBuffer)[3];
    *(ringData + 4) = NMVAR(nmUserDataBuffer)[4];
    *(ringData + 5) = NMVAR(nmUserDataBuffer)[5];
    CanInterruptRestore();

    rval = E_OK;
  #else
    /* 
     * received ring data was not copied at all
     * => impossible to access it
     */
    #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
    *ringData = *ringData;
    #endif

    rval = E_NotOK;
  #endif
  }
  return( rval );
}
#endif


#if defined ( NM_ENABLE_RINGDATA_ACCESS )
/******************************************************************************
 * Name:            TransmitRingData
 * Called by:       application
 * Parameter:       (NM channel)
 *                  *ringData   pointer to a user-specific array where the
 *                              user data have to be copied from
 * Return value:    - E_NotOK   access on ring data not possible
 *                  - E_OK      ring data have been updated
 * Description:     This API copies the user data from an application-specific buffer
 *                  into the user data of the NM message buffer.
 * Precondition:    ---
 *****************************************************************************/
StatusType NM_API_CALL_TYPE TransmitRingData( NM_CHANNEL_NMTYPE_FIRST vuint8* ringData )
{
  StatusType rval;

  #if defined ( NM_ENABLE_INDEXED_NM ) && \
      defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
  channel = NmSystemToNmIndirection[channel];
  NmAssert( channel != 0xFF, kNmAssertInvalidChannel );
  #endif

  /* check if access on ring data is not allowed */
  if( nmStateTxRingDataAllow == 0 )
  {
    rval = E_NotOK;
  }
  else
  {
    CanInterruptDisable();

    NmTxMsgByteUser0 = *(ringData    );
    NmTxMsgByteUser1 = *(ringData + 1);
    NmTxMsgByteUser2 = *(ringData + 2);
    NmTxMsgByteUser3 = *(ringData + 3);
    NmTxMsgByteUser4 = *(ringData + 4);
    NmTxMsgByteUser5 = *(ringData + 5);

    CanInterruptRestore();

    rval = E_OK;
  }
  return( rval );
}
#endif


#if defined ( NM_ENABLE_CONFIG )
/******************************************************************************
 * Name:            GetConfig
 * Called by:       application
 * Parameter:       (NM channel)
 *                  *dataPtr    pointer to the application-specific array
 *                              where the configuration has to be copied to
 * Return value:    ---
 * Description:     This API copies the current node configuration into an
 *                  application-specific buffer.
 * Precondition:    ---
 *****************************************************************************/
void NM_API_CALL_TYPE GetConfig( NM_CHANNEL_NMTYPE_FIRST NmConfigType *dataPtr )
{
  vuint8 configByte;

  #if defined ( NM_ENABLE_INDEXED_NM ) && \
      defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
  channel = NmSystemToNmIndirection[channel];
  NmAssert( channel != 0xFF, kNmAssertInvalidChannel );
  #endif

  CanInterruptDisable();

  #if defined ( NM_ENABLE_INDEXED_NM )
  for( configByte =   (vuint8)channel      * (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8);
       configByte < (((vuint8)channel + 1) * (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8));
       configByte++)
  {
  #else
  for( configByte = 0; 
       configByte < (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8); 
       configByte++)
  {
  #endif
    /* copy data */
    dataPtr->nmConfigChar[configByte] = nmConfig.nmConfigChar[configByte];
  }

  CanInterruptRestore();
}
#endif


#if defined ( NM_ENABLE_CONFIG )
/******************************************************************************
 * Name:            CmpConfig
 * Called by:       application
 * Parameter:       (NM channel)
 *                  idx:  index of ConfigMaskTable and TargetConfigTable
 * Return value:    - 1: configuration does match reference
 *                  - 0: configuration does not match reference
 * Description:     This API compares the current NM node configuration with a 
 *                  reference configuration stored in memory.
 *                  The reference is identified by <idx>.
 * Precondition:    ---
 *****************************************************************************/
NmReturnType CmpConfig( NM_CHANNEL_NMTYPE_FIRST vuint8 idx )
{
  vuint8 configByte;
  NmReturnType rval = 1;

  #if defined ( NM_ENABLE_INDEXED_NM ) && \
      defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
  channel = NmSystemToNmIndirection[channel];
  NmAssert( channel != 0xFF, kNmAssertInvalidChannel );
  #endif

  CanInterruptDisable();

  #if defined ( NM_ENABLE_INDEXED_NM )
  for( configByte =   (vuint8)channel      * (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8);
       configByte < (((vuint8)channel + 1) * (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8));
       configByte++ )
  #else
  for( configByte = 0;
       configByte < (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8); 
       configByte++ )
  #endif
  {
    /* compare data */
    if( ((TargetConfigTable[idx].nmConfigChar[configByte] ^ nmConfig.nmConfigChar[configByte]) &
        ConfigMaskTable[idx].nmConfigChar[configByte]) != 0x00 )
    {
      rval = 0;
      break;
    }
  }

  CanInterruptRestore();
  return( rval );
}
#endif


#if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
/******************************************************************************
 * Name:            NmGetPrepareSendSleepAck
 * Called by:       application
 * Parameter:       (NM channel)
 * Return value:    state of PrepareSleepAck
 * Description:     This API returns the current status of the flag 
 *                  NmGetPrepareSendSleepAck
 * Precondition:    ---
 *****************************************************************************/
vuint8 NM_API_CALL_TYPE NmGetPrepareSendSleepAck( NM_CHANNEL_NMTYPE_ONLY )
{
  vuint8 rval;

  #if defined ( NM_ENABLE_INDEXED_NM ) && \
      defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
  channel = NmSystemToNmIndirection[channel];
  NmAssert( channel != 0xFF, kNmAssertInvalidChannel );
  #endif

  if( nmStatePrepSendSleepAck == 1 )
  {
    rval = 1;
  }
  else
  {
    rval = 0;
  }

  return( rval );
}
#endif


#if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
/******************************************************************************
 * Name:            NmGetSendSleepAck
 * Called by:       application
 * Parameter:       (NM channel)
 * Return value:    state of NmTxMsgFlagSleepAck
 * Description:     This API returns the current state of the flag
 *                  NmTxMsgFlagSleepAck
 * Precondition:    ---
 *******************************************************************************/
vuint8 NM_API_CALL_TYPE NmGetSendSleepAck( NM_CHANNEL_NMTYPE_ONLY )
{
  vuint8 rval;

  #if defined ( NM_ENABLE_INDEXED_NM ) && \
      defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
  channel = NmSystemToNmIndirection[channel];
  NmAssert( channel != 0xFF, kNmAssertInvalidChannel );
  #endif

  if( NmTxMsgFlagSleepAck == 1 )
  {
    rval = 1;
  }
  else
  {
    rval = 0;
  }

  return( rval );
}
#endif


#if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
/******************************************************************************
 * Name:            NmGetRemoteSleepInd
 * Called by:       NM extensions (OnmExt)
 * Parameter:       (NM channel)
 * Return value:    state of nmStateRemoteSleepInd
 * Description:     This API returns the current state of the flag
 *                  nmStateRemoteSleepInd
 * Precondition:    ---
 *******************************************************************************/
vuint8 NM_API_CALL_TYPE NmGetRemoteSleepInd( NM_CHANNEL_NMTYPE_ONLY )
{
  vuint8 rval;

  #if defined ( NM_ENABLE_INDEXED_NM ) && \
      defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
  channel = NmSystemToNmIndirection[channel];
  NmAssert( channel != 0xFF, kNmAssertInvalidChannel );
  #endif

  if( nmStateRemoteSleepInd == 1 )
  {
    rval = 1;
  }
  else
  {
    rval = 0;
  }

  return( rval );
}
#endif


#if defined ( NM_ENABLE_ONM_EXT_API )
/******************************************************************************
 * Name:            NmGetUdStartAddress
 * Called by:       ONMX
 * Parameter:       (NM channel) (already indirected by ONMX)
 * Return value:    pointer to first UserData byte
 * Description:     This API returns a pointer to the start address of the NM 
 *                  user data.
 *                  This pointer does not change during runtime, i.e. the address
 *                  is does not change after the initialization.
 * Precondition:    API may only be called by NM extensions ONMX
 * Note:            ONMX API works with already calculated internal NM channel
 *******************************************************************************/
vuint8* NM_API_CALL_TYPE NmGetUdStartAddress( NM_CHANNEL_NMTYPE_ONLY )
{
  return( &(NmTxMsgByteUser0) );
}
#endif


#if defined ( NM_ENABLE_ONM_EXT_API )
/******************************************************************************
 * Name:            NmReadRecvSourceAddress
 * Called by:       ONMX
 * Parameter:       (NM channel) (already indirected by ONMX)
 *                  source    reference of source address
 * Return value:    - E_NotOK
 *                  - E_OK
 * Description:     
 * Precondition:    CAN interrupts have to be locked
 *                  API may only be called by NM extensions ONMX
 * Note:            ONMX API works with already calculated internal NM channel
 *******************************************************************************/
StatusType NM_API_CALL_TYPE NmReadRecvSourceAddress( NM_CHANNEL_NMTYPE_FIRST NmNodeType* source )
{
  StatusType rval;

  if( (mmStateBusError     == 1) ||
      (nmStateWaitBusSleep == 1) ||
      (nmStateBusSleep     == 1) )
  {
    /* source address is undefined */
    rval = E_NotOK;
  }
  else
  {
    /* lock interrupts => done by ONMX */
    *source = (NmNodeType)NMVAR(nmRxNmMsgSourceAdr);
    /* release interrupts => done by ONMX */

    rval = E_OK;
  }
  return ( rval );
}
#endif

































/**********************************************************************************************************************
  Local function definitions
**********************************************************************************************************************/
/******************************************************************************
 * Name:            NmTransmitSkippedAlive
 * Called by:       Nm_DirOsek
 * Parameter:       (NM channel)
 * Return value:    ---
 * Description:     This API prepares a SkippedAlive message for asynchronous
 *                  transmission.
 * Precondition:    ---
 *******************************************************************************/
static void NmTransmitSkippedAlive( NM_CHANNEL_NMTYPE_ONLY )
{
  nmStateRxTxLockUd = 1; /* ESCAN00008104 */

  NMVAR(nmTxRequest) = 1;           /* request transmission */
  NMVAR(nmRxTxCnt)  += TX_CNT_STEP; /* increment TX counter */

  NmTxMsgFlagCmdSet    = ALIVE;
  NmTxMsgByteDest      = NMVAR(nmLogicalSuccessor);
  NmTxMsgFlagSleepInd  = nmStateBusSleepInd;
  NmTxMsgFlagSleepAck  = 0;


  nmStateRingStable    = 0;
  nmStateRingPreStable = 0;

  #if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
  nmStateRemotePrepSleep  = 0;
  nmStateRemoteSleepInd   = 0;
  #endif
}


/******************************************************************************
 * Name:            NmResetNodeConfig
 * Called by:       Nm_DirOsek
 * Parameter:       (NM channel)
 * Return value:    ---
 * Description:     This API resets the node configuration.
 * Precondition:    ---
 * Note:            This API is also available, even if the node configuration 
 *                  monitoring is not enabled.
 *******************************************************************************/
static void NmResetNodeConfig( NM_CHANNEL_NMTYPE_ONLY )
{
  #if defined ( NM_ENABLE_CONFIG )
  vuint8 configByte;
  #endif

  nmStateRingStable    = 0;
  nmStateRingPreStable = 0;

  #if defined ( NM_ENABLE_CONFIG )
  /* clear the whole configuration */
    #if defined ( NM_ENABLE_INDEXED_NM )
  for( configByte =   (vuint8)channel      * (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8);
       configByte < (((vuint8)channel + 1) * (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8));
       configByte++ )
    #else
  for( configByte = 0; 
       configByte < (vuint8)((NM_NUMBER_OF_CONFIG_NODES)/8);
       configByte++ )
    #endif
  {
    nmConfig.nmConfigChar[configByte] = 0x00;
  }

  /* set own ECU number as present */
  configByte = NmCalculateConfigByte( kNmCfg_EcuNr );
  nmConfig.nmConfigChar[configByte] |= (vuint8)(1<<(kNmCfg_EcuNr & BYTE_OFFSET_MASK));
  #endif
}





#endif /* VGEN_ENABLE_NM_OSEK_D */

/**********************************************************************************************************************
  EOF
**********************************************************************************************************************/
/* ORGANI: STOPSINGLE_OF_MULTIPLE */

/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
