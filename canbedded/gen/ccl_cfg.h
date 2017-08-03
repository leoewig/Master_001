/* -----------------------------------------------------------------------------
  Filename:    ccl_cfg.h
  Description: Toolversion: 01.01.00.00.80.01.45.02.00.00
                
                Serial Number: CBD0800145
                Customer Info: McLaren Automotive Ltd.
                               McLaren / MC9S12X / Cosmic 4.7.8 / SC + MC + LIN Master
                
                
                Generator Fwk   : GENy 
                Generator Module: Ccl__core
                
                ECU: 
                        TargetSystem: Hw_Mcs12xCpu
                        Compiler:     Cosmic
                        Derivates:    MCS12X
                
                Channel "vehicle":
                        Databasefile: vehicle.dbc
                        Bussystem:    CAN
                        Manufacturer: McLaren
                        Node:         BMS
                Channel "isa":
                        Databasefile: isa_control.dbc
                        Bussystem:    CAN
                        Manufacturer: McLaren
                        Node:         mmc
                Channel "charger":
                        Databasefile: charger.dbc
                        Bussystem:    CAN
                        Manufacturer: McLaren
                        Node:         BMS

  Generated by , 2015-02-25  17:50:19
 ----------------------------------------------------------------------------- */
/* -----------------------------------------------------------------------------
  C O P Y R I G H T
 -------------------------------------------------------------------------------
  Copyright (c) 2001-2008 by Vector Informatik GmbH. All rights reserved.
 
  This software is copyright protected and proprietary to Vector Informatik 
  GmbH.
  
  Vector Informatik GmbH grants to you only those rights as set out in the 
  license conditions.
  
  All other rights remain with Vector Informatik GmbH.
 -------------------------------------------------------------------------------
 ----------------------------------------------------------------------------- */

#if !defined(__CCL_CFG_H__)
#define __CCL_CFG_H__

#include "can_inc.h"
#include "il_inc.h"
#include "n_onmdef.h"
#include "tpmc.h"
#include "Desc.h"
#include "AppDesc.h"
#include "xcpprof.h"
/***************************************************************************/ 
/* Version                  (abcd: Main version ab Sub Version cd )        */ 
/***************************************************************************/ 

#define CCL_DLL_VERSION  0x0201
#define CCL_DLL_BUGFIX_VERSION  0x00
#define CCL__COREDLL_VERSION                 0x0311u
#define CCL__COREDLL_RELEASE_VERSION         0x04u
/***************************************************************************/ 
/* Defines                                                                 */ 
/***************************************************************************/ 

#define C_ENABLE_CCL 

#define CCL_ENABLE_ERROR_HOOK  /* CclFatalError function is enabled */ 

#define CCL_ENABLE_DEBUG /* enables the debug mode and switches the assertions to on */ 

#define CCL_DISABLE_EMC_WAKEUP

#define CCL_DISABLE_WAKEUP_REG
#define CCL_ENABLE_INTERNAL_REQUEST
#define CCL_ENABLE_EXTERNAL_REQUEST
#define CCL_ENABLE_SLEEP_REPETITION
#define CCL_NM_TYPE_NONE

#define CCL_ENABLE_CANBEDDED_HANDLING
#define CCL_ENABLE_SCHEDULE_TASK
#define CCL_DISABLE_CONTAINER_TASK
#define CCL_DISABLE_STOP_MODE_ECU
#define CCL_DISABLE_POWER_DOWN_MODE_ECU
#define CCL_ENABLE_CUSTOMER_MODE_ECU
#define CCL_DISABLE_NET_STATE_RESTRICTION

#define CCL_DISABLE_TRCV_PORT_INT

#define CCL_ENABLE_SYSTEM_SHUTDOWN

#define CCL_ENABLE_INDEXED

#define CCL_ENABLE_SW_COM_STATE

#define kCclNrOfSystemChannels 3

#define kCclNrOfChannels 3  /* number of used channels */

#define kCclNrOfNetworks 3  /* number of used networks */ 

#define CCL_DISABLE_MULTIPLE_NODES  /* no multiple nodes */ 

#define CCL_ENABLE_BUSOFF_START

#define CCL_ENABLE_BUSOFF_END

#define CCL_ENABLE_BUSSTART

#define kCclNetReqTableSize 3  /* size of network request tabless */
#define kCclNumberOfUser 6
#define kCclCycleTime 10


/* Communication Handles */
#define CCL_vehicle_bus    0
#define CCL_isa_bus    1
#define CCL_charger_bus    2
#define CCL_DiagReq_0    3
#define CCL_DiagReq_1    4
#define CCL_DiagReq_2    5
/***************************************************************************/ 
/* Macros                                                                  */ 
/***************************************************************************/ 


  /* *** Communication Request access macros *** */
#define CclSet_vehicle_bus()                            CclRequestCommunication(CCL_vehicle_bus)
#define CclSet_isa_bus()                        CclRequestCommunication(CCL_isa_bus)
#define CclSet_charger_bus()                            CclRequestCommunication(CCL_charger_bus)
#define CclSet_DiagReq_0()                          CclRequestCommunication(CCL_DiagReq_0)
#define CclSet_DiagReq_1()                          CclRequestCommunication(CCL_DiagReq_1)
#define CclSet_DiagReq_2()                          CclRequestCommunication(CCL_DiagReq_2)


  /* *** Release Communication access macros *** */
#define CclRel_vehicle_bus()                            CclReleaseCommunication(CCL_vehicle_bus)
#define CclRel_isa_bus()                        CclReleaseCommunication(CCL_isa_bus)
#define CclRel_charger_bus()                            CclReleaseCommunication(CCL_charger_bus)
#define CclRel_DiagReq_0()                          CclReleaseCommunication(CCL_DiagReq_0)
#define CclRel_DiagReq_1()                          CclReleaseCommunication(CCL_DiagReq_1)
#define CclRel_DiagReq_2()                          CclReleaseCommunication(CCL_DiagReq_2)

extern void CclComStart(CanChannelHandle channel);
extern void CclComStop(CanChannelHandle channel);
extern void CclComWait(CanChannelHandle channel);
extern void CclComResume(CanChannelHandle channel);
extern void CCL_API_CALLBACK_TYPE ApplCclComStart(CanChannelHandle channel);
extern void CCL_API_CALLBACK_TYPE ApplCclComStop(CanChannelHandle channel);
extern void CCL_API_CALLBACK_TYPE ApplCclComWait(CanChannelHandle channel);
extern void CCL_API_CALLBACK_TYPE ApplCclComResume(CanChannelHandle channel);
extern void CclBusOffStart(CanChannelHandle channel);
extern void CclBusOffEnd(CanChannelHandle channel);
extern void CCL_API_CALLBACK_TYPE ApplCclBusOffStart(CanChannelHandle channel);
extern void CCL_API_CALLBACK_TYPE ApplCclBusOffEnd(CanChannelHandle channel);
extern void CCL_API_CALLBACK_TYPE ApplCclInit(CanChannelHandle channel);
extern void CCL_API_CALLBACK_TYPE ApplCclInitTrcv( CanChannelHandle channel );
extern void CCL_API_CALLBACK_TYPE ApplCclWakeUpTrcv( CanChannelHandle channel );
extern void CCL_API_CALLBACK_TYPE ApplCclStandbyTrcv( CanChannelHandle channel );
extern void CCL_API_CALLBACK_TYPE ApplCclSleepTrcv( CanChannelHandle channel );
extern void CCL_API_CALL_TYPE CclScheduleTask(void);
/*
 * Configuration file which GENy will place at the bottom of ccl_cfg.h
 *
 * Changes to this file will not take effect until GENy regenerates the
 * sources.  For this reason, we just include a regular header file
 */
#include "exc_ccl_cfg.h"

/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 184458331
      #error "The magic number of the generated file <C:\work\pr4\bms\nightwing\sw\mmc\proto\canbedded\gen\ccl_cfg.h> is different. Please check time and date of generated files!"
  #endif
#else
  #define MAGIC_NUMBER 184458331
#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

#endif /* __CCL_CFG_H__ */
