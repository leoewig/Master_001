/* -----------------------------------------------------------------------------
  Filename:    xcp_par.c
  Description: Toolversion: 01.01.00.00.80.01.45.02.00.00
                
                Serial Number: CBD0800145
                Customer Info: McLaren Automotive Ltd.
                               McLaren / MC9S12X / Cosmic 4.7.8 / SC + MC + LIN Master
                
                
                Generator Fwk   : GENy 
                Generator Module: Cp_Xcp
                
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

#if !defined(__XCP_PAR_C__)
#define __XCP_PAR_C__

#include "v_inc.h"
#include "XcpProf.h"
V_MEMROM0 V_MEMROM1 XcpCharType V_MEMROM2 kXcpStationId[] = "mmc_vehicle";
/* Events */
/* Online calibration */
/* Flash programming */
/* Transport Layer */
/*  */

/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 184458331
      #error "The magic number of the generated file <C:\work\pr4\bms\nightwing\sw\mmc\proto\canbedded\gen\xcp_par.c> is different. Please check time and date of generated files!"
  #endif
#else
  #define MAGIC_NUMBER 184458331
#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

#endif /* __XCP_PAR_C__ */
