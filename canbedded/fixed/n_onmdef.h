/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     n_onmdef.h 
 *
 * description:   n_onmdef
 *
 *
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           n_onmdef for S12XEP100
 *
 *****************************************************************************/

#ifndef V_NM_DIROSEK_HEADER
#define V_NM_DIROSEK_HEADER
#define NM_OSEK_H

/**********************************************************************************************************************
  Includes
**********************************************************************************************************************/
#include "v_cfg.h"
#include "v_def.h"

#if defined ( VGEN_ENABLE_NM_OSEK_D )

#include "nm_cfg.h"

#include "can_inc.h"
  #if defined ( VGEN_GENY )
#include "can_par.h"
  #endif

/**********************************************************************************************************************
  Version data
**********************************************************************************************************************/
/* ##V_CFG_MANAGEMENT ##CQProject : Nm_DirOsek CQComponent : Implementation */
#define NM_DIROSEK_VERSION                0x0374
#define NM_DIROSEK_RELEASE_VERSION        0x03

/**********************************************************************************************************************
  Configuration check
**********************************************************************************************************************/
#if defined ( VGEN_GENY )
  #if defined ( __NM_CFG_H__ ) || \
      defined ( __NMDIROSEK_CFG_H__ )
  #else
    #error "Nm_DirOsek: Configuration file is not included. Please check your configuration."
  #endif
#endif

/**********************************************************************************************************************
  Global configuration
**********************************************************************************************************************/
/* implicit configuration options */



/* compatibility defines for older configuration tools */
#if defined ( VGEN_GENY )
#endif

#if defined ( C_ENABLE_MULTICHANNEL_API )
  #define NM_ENABLE_INDEXED_DRVCAN
#endif

#if ( kNmNumberOfSystemChannels > kNmNumberOfChannels )
  #define NM_ENABLE_APPLCHANNEL_INDIRECTION
#endif

#if defined ( kComNumberOfNodes )
#else
  #define kComNumberOfNodes  1
#endif


/**********************************************************************************************************************
  Global typedefs
**********************************************************************************************************************/
/*
 * Name:    NMModeName
 * Used by: GotoMode() (parameter) 
 */
typedef enum
{
  BusSleep = 0,
  Awake
} NMModeName;

/*
 * Name:    NmInitType
 * Used by: NmOsekInit() (parameter) 
 */
typedef enum
{
  NM_DISABLE = 0,             /* do not start NM                */
  NM_NORMAL = 1,              /* SleepInd=0, active mode        */
  NM_NMPASSIVE = 2,           /* SleepInd=1, passive mode       */
  NM_SLEEPIND = 3,            /* SleepInd=1, active mode        */
  NM_SLEEPIND_NMPASSIVE = 4,  /* SleepInd=1, passive mode       */
#if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
  NM_SENDSLEEPACK = 5,        /* send NM message with SleepAck  */
#endif
  NM_CANSLEEP = 6             /* NM is asleep                   */
} NmInitType;


#if defined ( NM_ENABLE_CONFIG )
/* 
 * Name:    NmReturnType
 * Used by: CmpConfig() (return type) 
 */
typedef vuint8 NmReturnType;
#endif

#if defined ( NM_ENABLE_ONM_EXT_API )
/* 
 * Name:    NmNodeType
 * Used by: NmReadRecvSourceAddress() (parameter) 
 */
typedef vuint8 NmNodeType;

/* 
 * Name:    NmMsgType
 * Used by: OnmExtConfirmation() (parameter)
 *          OnmExtPrecopy() (parameter) 
 *          OnmExtTask() (parameter) 
 */
typedef vuint16 NmMsgType;
#endif

#if defined ( STATUSTYPEDEFINED )
/* another OSEK component has already defined the status type */
#else
  #define STATUSTYPEDEFINED
  #define E_OK                  0

/* 
 * Name:    StatusType
 * Used by: ReadRingData() (return type)
 *          TransmitRingData() (return type)
 */
typedef unsigned char StatusType;
#endif

/* Note: This define is not defined with STATUSTYPEDEFINED */
#define E_NotOK               64


/* 
 * Name:    NmStatusType
 * Used by: GetStatus() (parameter)
 *          NmGetStatus() (return type) 
 */
typedef vuint8 NmStatusType;

#if defined ( NM_ENABLE_CONFIG ) 
/* 
 * Name:    NmConfigType
 * Used by: ConfigMaskTable (type of array)
 *          TargetConfigTable (type of array)
 *          GetConfig (parameter)
 *          NmGetCanErrorConfig (parameter)
 */
typedef struct
{
    #if defined ( NM_ENABLE_INDEXED_NM )
  vuint8 nmConfigChar[(NM_NUMBER_OF_CONFIG_NODES)/8 * kNmNumberOfChannels];
    #else
  vuint8 nmConfigChar[(NM_NUMBER_OF_CONFIG_NODES)/8];
    #endif
} NmConfigType;
#endif


/**********************************************************************************************************************
  Global data defines
**********************************************************************************************************************/
/* NM type */
#if defined ( NM_ENABLE_INDEXED_NM )
  #define NM_CHANNEL_NMTYPE_ONLY              CanChannelHandle channel
  #define NM_CHANNEL_NMTYPE_FIRST             CanChannelHandle channel,
  #define NM_CHANNEL_NMPARA_ONLY              channel
  #define NM_CHANNEL_NMPARA_FIRST             channel,
#else
  #define NM_CHANNEL_NMTYPE_ONLY              void
  #define NM_CHANNEL_NMTYPE_FIRST             
  #define NM_CHANNEL_NMPARA_ONLY              
  #define NM_CHANNEL_NMPARA_FIRST             
#endif

/* CAN type */
#if defined ( NM_ENABLE_INDEXED_NM )
  #if defined ( NM_ENABLE_CHANNEL_INDIRECTION )
    #define NM_CHANNEL_CANTYPE_ONLY           CanChannelHandle canChannel
    #define NM_CHANNEL_CANTYPE_FIRST          CanChannelHandle canChannel,
    #define NM_CHANNEL_CANPARA_ONLY           NmNmToCanIndirection[channel]
    #define NM_CHANNEL_CANPARA_FIRST          NmNmToCanIndirection[channel],
  #else
    #define NM_CHANNEL_CANTYPE_ONLY           CanChannelHandle channel
    #define NM_CHANNEL_CANTYPE_FIRST          CanChannelHandle channel,
    #define NM_CHANNEL_CANPARA_ONLY           channel
    #define NM_CHANNEL_CANPARA_FIRST          channel,
  #endif
#else
  #if ( kCanNumberOfChannels > 1 )
    #define NM_CHANNEL_CANTYPE_ONLY           CanChannelHandle canChannel
    #define NM_CHANNEL_CANTYPE_FIRST          CanChannelHandle canChannel,

    #if defined ( NM_CAN_CHANNEL )            
      #define NM_CHANNEL_CANPARA_ONLY         NM_CAN_CHANNEL
      #define NM_CHANNEL_CANPARA_FIRST        NM_CAN_CHANNEL,
    #else
      #define NM_CHANNEL_CANPARA_ONLY         0
      #define NM_CHANNEL_CANPARA_FIRST        0,
    #endif
  #else
    #define NM_CHANNEL_CANTYPE_ONLY           void
    #define NM_CHANNEL_CANTYPE_FIRST          
    #define NM_CHANNEL_CANPARA_ONLY           
    #define NM_CHANNEL_CANPARA_FIRST          
  #endif
#endif

/* APPL type */
#if defined ( NM_ENABLE_INDEXED_NM )
  #define NM_CHANNEL_APPLTYPE_ONLY            CanChannelHandle channel
  #define NM_CHANNEL_APPLTYPE_FIRST           CanChannelHandle channel,
  #if defined ( NM_ENABLE_APPLCHANNEL_INDIRECTION )
    #define NM_CHANNEL_APPLPARA_ONLY          NmNmToSystemIndirection[channel]
    #define NM_CHANNEL_APPLPARA_FIRST         NmNmToSystemIndirection[channel],
  #else
    #define NM_CHANNEL_APPLPARA_ONLY          channel
    #define NM_CHANNEL_APPLPARA_FIRST         channel,
  #endif
#else                                         
  #define NM_CHANNEL_APPLTYPE_ONLY            void
  #define NM_CHANNEL_APPLTYPE_FIRST           
  #define NM_CHANNEL_APPLPARA_ONLY            
  #define NM_CHANNEL_APPLPARA_FIRST           
#endif

#if defined ( NM_API_CALL_TYPE )
#else
  #define NM_API_CALL_TYPE
#endif

#if defined ( NM_API_CALLBACK_TYPE )
#else
  #define NM_API_CALLBACK_TYPE
#endif

#if defined ( NM_INTERNAL_CALL_TYPE )
#else
  #define NM_INTERNAL_CALL_TYPE
#endif



#if defined ( NM_ENABLE_ONM_EXT_API )
  /*
   * Name:        NM_MSG_...
   * Description: symbolic values for NM extension API
   * Used by      NmTask()
   *              NmPrecopy()
   *              NmConfirmation()
   */
  #define NM_MSG_INVALID        ((NmMsgType)0x0000)
  #define NM_MSG_RESET_ALIVE    ((NmMsgType)0x0001)
  #define NM_MSG_SKIPPED_ALIVE  ((NmMsgType)0x0002)
  #define NM_MSG_RING           ((NmMsgType)0x0004)
  #define NM_MSG_LIMPHOME       ((NmMsgType)0x0008)
  #define NM_MSG_SLEEP_IND      ((NmMsgType)0x0010)
  #define NM_MSG_SLEEP_ACK      ((NmMsgType)0x0020)
  /* flags for NmTask() */
  #define NM_MSG_TIMEOUT_RX     ((NmMsgType)0x0080)
  #define NM_MSG_TIMEOUT_TX     ((NmMsgType)0x0100)
  #define NM_MSG_WAIT_RX        ((NmMsgType)0x0200)
  #define NM_MSG_WAIT_TX        ((NmMsgType)0x0400)
  #define NM_MSG_START_TX       ((NmMsgType)0x0800)
  #define NM_MSG_RECEIVED       ((NmMsgType)0x1000)
  #define NM_MSG_TOKEN_RECEIVED ((NmMsgType)0x2000)
#endif

#if defined ( NM_ENABLE_ASSERTION )
  /*
   * Name:        kNmAssert...
   * Description: symbolic values for NM assertions
   * Used by      ApplNmDirOsekFatalError() (parameter)
   */
  #define kNmAssertConfirmationHasWrongCanHandle  0x10
  #define kNmAssertWrongFlagWaitBusSleep          0x11
  #define kNmAssertWrongFlagBusSleep              0x12
  #define kNmAssertInvalidChannel                 0x13
  #define kNmAssertCanIsOffline                   0x14
#endif

/**********************************************************************************************************************
  Global macro defines
**********************************************************************************************************************/
/*
 * Name:        NmState...()
 * Description: macros for status evaluation
 * Used by      application
 */
/* ESCAN00022737 */
#define NmStateRingStable(state)          (((state) & (NmStatusType)0x0001) != 0 ? 1 : 0)
#define NmStateBusError(state)            (((state) & (NmStatusType)0x0002) != 0 ? 1 : 0)
#define NmStateActive(state)              (((state) & (NmStatusType)0x0004) != 0 ? 1 : 0)
#define NmStatePassive(state)             (((state) & (NmStatusType)0x0004) != 0 ? 0 : 1)
#define NmStateLimphome(state)            (((state) & (NmStatusType)0x0008) != 0 ? 1 : 0)
#define NmStateBusSleep(state)            (((state) & (NmStatusType)0x0010) != 0 ? 1 : 0)
#define NmStateWaitBusSleep(state)        (((state) & (NmStatusType)0x0020) != 0 ? 1 : 0)
#define NmStateTxRingDataAllowed(state)   (((state) & (NmStatusType)0x0040) != 0 ? 1 : 0)
#define NmStateBusSleepInd(state)         (((state) & (NmStatusType)0x0080) != 0 ? 1 : 0)
#define NmStateBusWakeUp(state)           (((state) & (NmStatusType)0x0080) != 0 ? 0 : 1)

/**********************************************************************************************************************
  Global data prototypes (exported, RAM)
**********************************************************************************************************************/




/**********************************************************************************************************************
  Global data prototypes (exported, ROM)
**********************************************************************************************************************/

V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kNmMainVersion;
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kNmSubVersion;
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kNmReleaseVersion;

/* for compatibility reasons */
#define kNmBugfixVersion      kNmReleaseVersion


/**********************************************************************************************************************
  Global data prototypes (imported, RAM)
**********************************************************************************************************************/
#if defined ( NM_ENABLE_INDEXED_NM )
#else 
  #if defined ( NM_ENABLE_MULTIPLE_NODES )
    #if defined ( NM_CAN_CHANNEL )
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 comMultipleECUCurrent[kCanNumberOfChannels];
    #else
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 comMultipleECUCurrent;
    #endif
  #endif
#endif


/**********************************************************************************************************************
  Global data prototypes (imported, ROM)
**********************************************************************************************************************/

/*----------------------------------------------------------
 * from configuration tool
 *----------------------------------------------------------*/
#if defined ( NM_ENABLE_INDEXED_NM )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 NmCallCycle_Field[kNmNumberOfChannels];
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 NmCountRingTyp_Field[kNmNumberOfChannels];
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 NmCountRingMax_Field[kNmNumberOfChannels];
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 NmCountLimphome_Field[kNmNumberOfChannels];
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 NmCountWaitBussleep_Field[kNmNumberOfChannels];

  #if defined ( VGEN_GENY )
    #if ( kNmNumberOfRanges > 1 )
V_MEMROM0 extern V_MEMROM1 vuint8  V_MEMROM2 NmRangeMask_Field[kNmNumberOfChannels];
V_MEMROM0 extern V_MEMROM1 vuint16 V_MEMROM2 NmRangeCode_Field[kNmNumberOfChannels];
    #endif
  #endif

  #if defined ( NM_ENABLE_CHANNEL_INDIRECTION )
V_MEMROM0 extern V_MEMROM1 CanChannelHandle V_MEMROM2 NmCanToNmIndirection[kCanNumberOfChannels];
V_MEMROM0 extern V_MEMROM1 CanChannelHandle V_MEMROM2 NmNmToCanIndirection[kNmNumberOfChannels];
  #endif

  #if ( kNmNumberOfSystemChannels > kNmNumberOfCanChannels )
V_MEMROM0 extern V_MEMROM1 CanChannelHandle V_MEMROM2 NmSystemToNmIndirection[kNmNumberOfSystemChannels];
V_MEMROM0 extern V_MEMROM1 CanChannelHandle V_MEMROM2 NmNmToSystemIndirection[kNmNumberOfChannels];
  #else
    #define NmSystemToNmIndirection     NmCanToNmIndirection
    #define NmNmToSystemIndirection     NmNmToCanIndirection
  #endif

    #if defined ( NM_ENABLE_MULTIPLE_NODES )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 NmEcuNr_Field[kNmNumberOfChannels][kComNumberOfNodes];
V_MEMROM0 extern V_MEMROM1 CanTransmitHandle V_MEMROM2 NmTxObj_Field[kNmNumberOfChannels][kComNumberOfNodes];
    #else
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 NmEcuNr_Field[kNmNumberOfChannels];
V_MEMROM0 extern V_MEMROM1 CanTransmitHandle V_MEMROM2 NmTxObj_Field[kNmNumberOfChannels];
    #endif

V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 NmCanPara_Field[kNmNumberOfChannels];


  #if defined ( NM_ENABLE_FAST_BUSOFF_RECOVERY ) 
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 NmCountLimphomeShort_Field[kNmNumberOfChannels];
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 NmCountShortNumber_Field[kNmNumberOfChannels];
  #endif


#endif


#if defined ( NM_ENABLE_INDEXED_NM )
#else 
  #if defined ( NM_ENABLE_MULTIPLE_NODES )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 NmEcuNr_Field[kComNumberOfNodes];
V_MEMROM0 extern V_MEMROM1 CanTransmitHandle V_MEMROM2 NmTxObj_Field[kComNumberOfNodes];
  #endif
#endif

/*----------------------------------------------------------
 * from application
 *----------------------------------------------------------*/
#if defined ( NM_ENABLE_CONFIG )
V_MEMROM0 extern V_MEMROM1 NmConfigType V_MEMROM2 ConfigMaskTable[];
V_MEMROM0 extern V_MEMROM1 NmConfigType V_MEMROM2 TargetConfigTable[];
#endif


/**********************************************************************************************************************
  Global function prototypes
**********************************************************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------
  service functions
---------------------------------------------------------------------------------------------------------------------*/
extern void NM_API_CALL_TYPE NmOsekInit( NM_CHANNEL_NMTYPE_FIRST NmInitType initMode );
extern void NM_API_CALL_TYPE NmTask( NM_CHANNEL_NMTYPE_ONLY );
extern void NM_API_CALL_TYPE GotoMode( NM_CHANNEL_NMTYPE_FIRST NMModeName mode );

extern void NM_API_CALL_TYPE GetStatus( NM_CHANNEL_NMTYPE_FIRST NmStatusType *dest );
extern NmStatusType NM_API_CALL_TYPE NmGetStatus( NM_CHANNEL_NMTYPE_ONLY );

extern void NM_API_CALL_TYPE StartNM( NM_CHANNEL_NMTYPE_ONLY );
extern void NM_API_CALL_TYPE StopNM( NM_CHANNEL_NMTYPE_ONLY );
extern void NM_API_CALL_TYPE TalkNM( NM_CHANNEL_NMTYPE_ONLY );
extern void NM_API_CALL_TYPE SilentNM( NM_CHANNEL_NMTYPE_ONLY );

#if defined ( NM_ENABLE_RINGDATA_ACCESS )
extern StatusType NM_API_CALL_TYPE ReadRingData( NM_CHANNEL_NMTYPE_FIRST vuint8 *ringData );
extern StatusType NM_API_CALL_TYPE TransmitRingData( NM_CHANNEL_NMTYPE_FIRST vuint8 *ringData );
#endif

#if defined ( NM_ENABLE_CONFIG )
extern void         NM_API_CALL_TYPE GetConfig( NM_CHANNEL_NMTYPE_FIRST NmConfigType *dataPtr );
extern NmReturnType NM_API_CALL_TYPE CmpConfig( NM_CHANNEL_NMTYPE_FIRST vuint8 idx );
#endif

#if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
extern vuint8 NM_API_CALL_TYPE NmGetPrepareSendSleepAck( NM_CHANNEL_NMTYPE_ONLY );
extern vuint8 NM_API_CALL_TYPE NmGetSendSleepAck( NM_CHANNEL_NMTYPE_ONLY );
#endif

#if defined ( NM_ENABLE_REMOTESLEEP_INDICATION )
extern vuint8 NM_API_CALL_TYPE NmGetRemoteSleepInd( NM_CHANNEL_NMTYPE_ONLY );
#endif

#if defined ( NM_ENABLE_ONM_EXT_API )
extern vuint8*    NM_API_CALL_TYPE NmGetUdStartAddress( NM_CHANNEL_NMTYPE_ONLY );
extern StatusType NM_API_CALL_TYPE NmReadRecvSourceAddress( NM_CHANNEL_NMTYPE_FIRST NmNodeType *source );
#endif









/*---------------------------------------------------------------------------------------------------------------------
  service functions (DrvCan)
---------------------------------------------------------------------------------------------------------------------*/
/*
 * The prototypes of the service functions for the CAN driver are defined
 * by the CAN driver.
 * - NmCanError()
 * - NmConfirmation()
 * - NmPrecopy()
 * - NmPreTransmit()
 */

/*---------------------------------------------------------------------------------------------------------------------
  callback functions
---------------------------------------------------------------------------------------------------------------------*/
extern void NM_API_CALLBACK_TYPE ApplNmCanNormal( NM_CHANNEL_APPLTYPE_ONLY );
extern void NM_API_CALLBACK_TYPE ApplNmCanSleep( NM_CHANNEL_APPLTYPE_ONLY );
extern void NM_API_CALLBACK_TYPE ApplNmCanBusSleep( NM_CHANNEL_APPLTYPE_ONLY );
extern void NM_API_CALLBACK_TYPE ApplNmBusOff( NM_CHANNEL_APPLTYPE_ONLY );

#if defined ( NM_ENABLE_EXTENDED_CALLBACK )
extern void NM_API_CALLBACK_TYPE ApplNmBusOffEnd( NM_CHANNEL_APPLTYPE_ONLY );
extern void NM_API_CALLBACK_TYPE ApplNmBusStart( NM_CHANNEL_APPLTYPE_ONLY );
extern void NM_API_CALLBACK_TYPE ApplNmWaitBusSleep( NM_CHANNEL_APPLTYPE_ONLY );
extern void NM_API_CALLBACK_TYPE ApplNmWaitBusSleepCancel( NM_CHANNEL_APPLTYPE_ONLY );
#endif


#if defined ( NM_ENABLE_RINGDATA_ACCESS )
extern void NM_API_CALLBACK_TYPE ApplNmIndRingData( NM_CHANNEL_APPLTYPE_ONLY );
#endif

#if defined ( NM_ENABLE_SLEEPACK_NOTIFICATION )
extern vuint8 NM_API_CALLBACK_TYPE ApplNmSendSleepAck( NM_CHANNEL_APPLTYPE_ONLY );
#endif



#if defined ( NM_ENABLE_ASSERTION )
extern void NM_API_CALLBACK_TYPE ApplNmDirOsekFatalError( vuint8 error );
#endif


#if defined ( NM_ENABLE_ONM_EXT_API )
extern void NM_INTERNAL_CALL_TYPE OnmExtConfirmation(NM_CHANNEL_NMTYPE_FIRST NmMsgType);
extern void NM_INTERNAL_CALL_TYPE OnmExtPrecopy(NM_CHANNEL_NMTYPE_FIRST NmMsgType, CanChipDataPtr);
extern void NM_API_CALL_TYPE      OnmExtInit(NM_CHANNEL_NMTYPE_ONLY);
extern void NM_API_CALL_TYPE      OnmExtTask(NM_CHANNEL_NMTYPE_FIRST NmMsgType);
#endif

#endif /* VGEN_ENABLE_NM_OSEK_D */
#endif /* V_NM_DIROSEK_HEADER */

/**********************************************************************************************************************
  EOF
**********************************************************************************************************************/
/* ORGANI: STOPSINGLE_OF_MULTIPLE */

/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
