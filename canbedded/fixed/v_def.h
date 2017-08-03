/* STARTSINGLE_OF_MULTIPLE */
/*****************************************************************************
| Project Name: V E C T O R   -  Common module type definition header
|    File Name: v_def.h
|
|  Description: Declares types and definitions common to all 
|               VECTOR CANbedded modules.
|               If some of the definitions collides with Company-internal
|               type definitions, modifications could be made here.
|               It must be ensured, that own types are compliant 
|               with Vector internal definitions 
|
|               Note:
|               =====
|               that some hardware dependent settings included in this file.
|               Never mix up files with same namings but intended for 
|               other hardware platforms.
|
|
|     This file is intended for the following hardware platform:
|   ---------------------------------------------------------- 
|
|            Compiler:   see module file
|            Controller:
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2007 by Vector Informatik GmbH.       All rights reserved.
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
|   Hp         Armin Happel              Vector Informatik GmbH
|   Ht         Heike Honert              Vector Informatik GmbH
|   dH         Gunnar de Haan            Vector Informatik GmbH
|   Et         Thomas Ebert              Vector Informatik GmbH
|   Wr         Hans Waaser               Vector Informatik GmbH
|   Vg         Frank Voorburg            Vector CANtech, Inc.
|   Ds         Hussain Darwish           Vector CANtech, Inc.
|   RAP        Rebecca Pipkorn           Vector CANtech, Inc.
|   BWR        Brandon Root              Vector CANtech, Inc.
|   Ml         Patrick Markl             Vector Informatik GmbH
|   Pl         Georg Pfluegel            Vector Informatik GmbH
|   LS         Konrad Lazarus            Vector Informatik GmbH
|   Zw         Werner Ziegler            Vector Informatik GmbH
|   Bs         Thomas Balster            Vector Informatik GmbH
|   Stu        Bernd Stumpf              Vector Informatik GmbH
|   Pet        Thomas Petrus             Vector Informatik GmbH
|   To         Torsten Schmidt           Vector Informatik GmbH
|   Ap         Andreas Pick              Vector Informatik GmbH
|   Tri        Frank Triem               Vector Informatik GmbH
|   WM         Marco Wierer              Vector Informatik GmbH
|   Bir        Holger Birke              Vector Informatik GmbH
|   Ces        Senol Cendere             Vector Informatik GmbH
|   Ms         Gunnar Meiss              Vector Informatik GmbH
|   Ths        Thomas Sommer             Vector Informatik GmbH
|   Krt        Kerstin Thim              Vector Informatik GmbH
|   Fn         Matthias Fleischmann      Vector Informatik GmbH
|   Svh        Sven Hesselmann           Vector Informatik GmbH
|   Ard        Thomas Arnold             Vector Informatik GmbH
|   Her        Peter Herrmann            Vector Informatik GmbH
|   swk        Slawomir Wilk             Vector Informatik GmbH
|   Ou         Mihai Olariu              Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date       Ver    Author  Description
| ---------  ----   ------  --------------------------------------------------
| 13-Mar-01  1.00    Hp     Creation
| 04-Apr-01  1.01    Ht     general rework 
| 05-Apr-01  1.02    dH     added Fujitsu FFMC16LX
| 05-Apr-01  1.03    HH     added COMP_GHS_V85X
| 11-Apr-01  1.04    Et     added LIN components
| 23-Apr-01  1.05    Si     corrections for C_COMP_IAR_78K0
|                           lint comments added
| 29-Mai-01  1.06    Pl     added ARM7 STMICRO
| 22-Jun-01  1.07    Ht     added ANSI CANoe
| 03-Jul-01          Wr     added C_COMP_OKI_CC665S
| 12-Jul-01          Et     support combination of "old" CAN driver and "new" LIN driver
| 13-Jul-01  1.08    Ht     error check for supported systems and switches added
| 16-Jul-01          dH     added C_COMP_HEW_SH7055 / C_COMP_HEW_H8S
| 17-Aug-01  1.09    Et     deleted keywords __near and __far for C_COMP_FUJITSU_8L
| 21-Aug-01  1.10    Ht/Vg  added C_COMP_DIABDATA_PPC
| 25-Aug-01  1.11    Ml     added #pragma MESSAGE DISABLE C1106 for Hiware HC08 compiler
| 06-Sep-01  1.12    Ht/Vg  added C_COMP_GNU_ST9
| 11-Sep-01  1.13    Fr     added C_COMP_ANSI_TMS_470
| 21-Sep-01  1.14    Pl     added C_COMP_MCP18_PIC_INT
| 25-Sep-01  1.15    Vg     added C_COMP_MICROTEC_MC376 and C_COMP_TASKING_MC376
| 28-Sep-01  1.16    Hp     added C_COMP_GHS_TX39 and C_COMP_TOSHIBA_TLC900
| 23-Oct-01  1.17    Js     added C_COMP_DIABDATA_MCORE
| 23-Oct-01  1.18    dH     changed ..._SH705X to ..._SH705X
| 05-Nov-01  1.19    dH     added M32R
| 19-Nov-01  1.20    Ht     added C_COMP_xxx_XC16X
| 22-Nov-01  1.21    dH     added C_COMP_IAR_H8S
| 11-Nov-01  1.22    Fz     added C_COMP_MITSUBISHI_M32C/C_COMP_NEC_V85X
| 19-Dec-01  1.23    dH     changed Fujitsu FFMC16LX
| 28-Jan-02  1.24    Ml     ESCAN00002160: added paging keywords for HC12
| 31-Jan-02  1.25    Pl     added C_COMP_KEIL_T89C51C
| 07-Mar-02  1.26    Vg/Rn  added C_COMP_METROWERKS_PPC
| 27-Mar-02  1.27    Fz     added C_COMP_IAR_V85X
| 09-Apr-02  1.28    Ht     addaption to LI 1.3
| 02-Apr-02  1.29    Ml     added C_COMP_METROWERKS_MGT5100
|                           added C_COMP_METROWERKS_DSP56F80X
| 13-May-02  1.30    LS     MEMORY_HUGE added for C_COMP_TASKING_C16X
| 21-May-02  1.31    Ml     canbittype for HC12 is now unsigned short
| 10-Jul-02  1.32    Ml     canbittype for HC12 changed to unsigned char
| 16-Jul-02  1.33a   Fz     canbittype for V850 changed to unsigend int
|                           _c_bits32 added  
| 31-Jul-02          Ts     added C_COMP_HEW_SH70XX_HCAN2
| 31-Jul-02  1.33    Ht     _c_bits16 added for C_CPUTYPE_BITORDER_MSB2LSB
| 31-Jul-02  1.34    Si     pc-lint comments for 78K0 changed
| 02-Aug-02  1.33    Sf     ESCAN00003192  Memory qualifier for M16C changed
| 08-Aug-02  1.34    Zw     ESCAN00003456  added STmicro ST7 (beCAN) / Hiware
| 18-Aug-02          Pet    ESCAN00003530  delete keyword near for HC08
| 06-Sep-02  1.35    Zw     ESCAN00003688: MEMORY_NEAR defined to nothing for ST7_beCAN
| 09-Sep-02  1.36    Bs     controller OKI MSM9225B (C_COMP_GHS_ARM7TM) added (needed for FBL)
| 10-Sep-02  1.37    Ds     added C_COMP_COSMIC_ST7_BECAN
|                           and   C_COMP_HIWARE_ST7_BECAN
| 13-Sep-02  1.38    Ht     rework the module
| 17-Sep-02  1.39    Stu    add the C_COMP_NEC_78K0
| 17-Sep-02  1.40    Vg     ESCAN00003614 added __declspec() to place const 
|                           variables into ROM for Metrowerks
| 18-Sep-02  1.41    Stu    added C_COMP_NEC_78K0
| 07-Oct-02  1.42    dH     added C_COMP_GAIO_SH705X
| 11-Oct-02  1.43    dH     added C_COMP_FUJITSU_16LX_HL
| 14-Oct-02  1.44    Ml     changed vbittype for MGT5100 into unsigned short
| 14-Oct-02  1.45    Stu    changed vbittype for 78k0 into unsigned char
| 18-Oct-02  1.46    Pl     added C_COMP_TASKING_ST10_CCAN 
|                           added C_COMP_ARM_ST30_CCAN 
| 30-Oct-02  1.47    Ml     added C_COMP_ARM_470 and C_COMP_TI_470
| 06-Nov-02  1.48    Wr     added C_COMP_NATIONAL_CR16 
| 07-Nov-02  1.49    RAP    added C_COMP_DIABDATA_MPC823
|                    RAP    added C_COMP_GHS_STARFISH
| 14-Nov-02  1.50    An     corrected the banking defines for Cosmic and Hiware
| 27-Nov-02  1.50    Pl     added C_COMP_FUJITSU_FR50
| 02-Dec-02  1.51    Si     MEMORY_NEAR changed for C_COMP_NEC_78K0
| 11-Dec-02          Pet    added C_COMP_IAR_CEVF
| 18-Dec-02  1.52    Si     added C_COMP_NEC_78K0
| 04-Jan-03          Et     added memory qualifier for C_COMP_IAR_CEVF
| 22-Jan-03          Ap     added C_COMP_ARM_EASYCAN
| 28-Jan-03  1.53    Pl     added C_COMP_KEIL_ST10_CCAN
|                           delete C_COMP_ARM_ARM7STM
| 03-Apr-04  1.54    To     added C_COMP_METROWERKS_DSP5683X
|                    ZW     canbittype for CEVG is now unsigned char
|                    Hp     Add MEMORY_HUGE for XC16x
|                    Fz     added  C_COMP_IAR_CR16 and C_COMP_NATIONAL_CR16 changed
|                    Ml     added  C_COMP_TI_TMS320
| 2003-05-13 1.60    Ht     support V_MEMROM0
| 2003-05-14 1.61    Tri    added V_MEMROM0 definition for MGT5100
| 2003-05-15 1.62    Pl     added C_COMP_FUJITSU_FR60_CCAN
| 2003-05-20 1.63    WM     added near/far memory defines for C_COMP_HIWARE_12
| 2003-06-11 1.64    CB     MEMORY_HUGE added for C_COMP_TASKING_ST10_CCAN    
| 2003-06-18 1.65    Bir    added C_COMP_MICROCHIP_DSPIC30
| 2003-07-08 1.66    Bir    no local lint command
| 2003-08-15 1.67    Rr     changed MEMORY_FAR for M32C
| 2003-08-18 1.68    BWR    added support for MAC710X
| 2003-09-02 1.69    BWR    added support for MC332 with external Intel 82527 
| 2003-09-09 1.70    Bir    added C_COMP_TASKING_TRICORE_MULTICAN & C_COMP_GNU_TRICORE_MULTICAN
| 2003-10-30 1.71    RAP    added C_COMP_MICROCHIP_PIC18
| 2003-11-13 2.00    Ht     memory qualifier changed to new definition
| 2004-01-08 2.01    BWR    added support for MPC55XX 
| 2004-01-16 2.02    Ml     added cast macros
| 2004-01-22 2.03    dH     added C_COMP_HEW_SH2_HCAN1 and C_COMP_HEW_H8S_HCAN1
| 2004-02-13 2.04    WM     ESCAN00007465: Added banking support for C_COMP_IAR_12
| 2004-02-18 2.05    RAP    Update for C_COMP_MICROCHIP_PIC18
| 2004-03-12 2.06    Ml     changed canbittype for TMS470
| 2004-03-13 2.07    dH     added C_COMP_HEW_H8TINY
| 2004-04-20 2.08    dH     added C_COMP_HEW_H8SX_HCAN1
| 2004-04-08 2.09    Rna    Memoryclassifier V_MEMROM0 and MEMORY_ROM for Metrowerks PPC changed
| 2004-05-14 2.10    Ms     added Release Version define
|                           added include for vstdlib.h
| 2004-05-18 2.11    Ml     changed plattform check to work in any case (NO_ELSE)
|                    Ml     added V_NULL
| 2004-06-17 2.12    Ms     changed C_COMP_SUPPORTED_PLATTFORM -> V_COMP_SUPPORTED_PLATTFORM
| 2004-06-26 2.13    RAP    added C_COMP_GHS_SJA2020
| 2004-06-30 2.14    Ml     added C_COMP_COSMIC_MCS12X_MSCAN12
| 2004-08-17 2.15    Pl     Memoryclassifier V_MEMROM0/1/2 for the C_COMP_TOSHIBA_TLC900 changed
| 2004-08-31 2.16    Ml     changed bitfieldtype for MCS12X
| 2004-09-08 2.17    RAP    changed vbittype for C_COMP_GHS_SJA2020
| 2004-09-12 2.18    BWR    added C_COMP_GHS_PPC
| 2004-09-20 2.19    Ces    added C_COMP_GHS_ST30_CCAN
| 2004-09-27 2.20    Ms     -ESCAN00009636: Naming Conventions
| 2004-09-28 2.21    Bir    changed bitfieldtype for TriCore MultiCAN
| 2004-09-30 2.22    Ht     Review
| 2004-10-06 2.23    Ml     Added C_COMP_MTRWRKS_MCS12X_MSCAN12  
| 2004-10-27 2.24    Ml     changed V_ENABLE_VSTDLIB -> VGEN_ENABLE_VSTDLIB
| 2004-10-29 2.25    Pl     Added C_COMP_GHS_TX19
| 2004-11-05 2.26    Ml     Added C_COMP_COSMIC_MCS12X_MSCAN12 and C_COMP_COSMIC_MCS12_MSCAN12
| 2004-11-08 2.27    Et     support compiler option -fNP (C_COMP_MITSUBISHI_M32C)
| 2004-11-10 2.28    Ml     Added compatibility for VStdLib
| 2004-11-15 2.29    dH     new memoryclassifier for C_COMP_FUJITSU_16LX/C_COMP_FUJITSU_16LX_HL
| 2004-11-15 2.30    Ms     ESCAN00010228: Support C_COMP specific V_NULL definition
| 2004-11-19 2.31    Ml     bitfields for TMS470 are now int
|                    Ths    ESCAN00010435: Due to compatibility V_DEF_VERSION added
| 2005-01-31 2.32    Ml     Added C_COMP_IAR_TMS430_SCCHECC
| 2005-02-09 2.33    Ht     support V_NULL for C_COMP_KEIL_C5X5C
| 2005-03-10 2.34    Ces    Memory qualifier MEMORY_SADDR and MEMORY_NEAR for IAR 78K0 compiler
| 2005-03-22 2.35    Pl     support V_NULL for C_COMP_TASKING_ST10_CCAN
| 2005-03-22 2.36    Ces    support C_COMP_IAR_78K0_AFCAN
|                           support V_NULL for Tasking C16x/ST10, Mitsubishi M16C, IAR 78K0,
|                           Metrowerks DSP5683x and GreenHills ST30
| 2005-03-24 2.37    Krt    Set cast macros for M32C controller, to avoid compiler warnings
|                    Bir    support V_NULL for C_COMP_TASKING_C16X, C_COMP_TASKING_XC16X, C_COMP_GNU_TRICORE_MULTICAN
|                    Ces    MEMORY_NEAR corrections for IAR 78K0 AFCAN
| 2005-06-17 2.38    Ht     remove duplicated compiler switch
|                    dH     added C_COMP_HEW_SH2_RCAN
|                    Pl     added C_COMP_GNU_ST10_CCAN
|                    Ds     Added support for ADI Blackfin (BF5xx)
|                    Ml     Added QNX platforms
|                    Fz     MEMORY_HUGE changed for V85X IAR compiler in case tiny memory model
|                    Ht     default definition of NULL
| 2005-06-20 2.39    BWR    Added support for MAC7100 IAR compiler
|                    dH     changed bittype for C_COMP_HEW_SH2_RCAN to unsigned char
| 2005-08-24 2.40    Rna    position for NULL definitions moved to prevent redefinitions for HC12 cosmic (string.h)
| 2005-10-05 2.41    Ces    adaptions for IAR compiler M16C
| 2005-10-14 2.42    Fn     added C_COMP_IAR_AVR_CANARY
|                    Ces    corrections for C_COMP_IAR_M16C
|                    Ces    added C_COMP_IAR_M32C
|                    Ces    added V_MEMROM3 for C_COMP_MITSUBISHI_M16C
| 2005-10-25 2.43    Ces    IAR M32C: MEMORY_ROM is always far const
| 2005-12-02 2.44    dH     added C_COMP_HEW_H8S_RCAN
|                    Fn     added C_COMP_MICROCHIP_PIC18 for V_NULL
|                    Ht     V_COMP_SUPPORTED_PLATTFORM changed to V_DEF_SUPPORTED_PLATFORM 
|                    Pl     added C_COMP_MATSUSHITA_MN103S_CCAN
|                    Svh    added C_COMP_TOSHIBA_TLCS870
|                    Ces    changed compiler version check of IAR M16C and IAR M32C
| 2005-12-20 2.45    Ml     Added C_COMP_METROWERKS_MGT5X00
|                    Ces    changed MEMORY_NORMAL for IAR M16C and IAR M32C
| 2006-01-27 2.46    Ml     ESCAN00014816: Added cast defines for MCS12x with comsic
|                    Ml     ESCAN00015045: Added support for constants in global pages
|                    Ml     Added C_COMP_MTRWRKS_MPC5X00
|                    Svh    Added C_COMP_ARM_ADUC703X
| 2006-02-08 2.47    Pl     Added C_COMP_IAR_ML67Q25XX_CCAN
| 2006-02-09         Ml     Changed check for XGate compiler
| 2006-02-10 2.48    Ces    Added #define V_NULL for MITSUBISHI M32C and MITSUBISHI M32R
| 2006-02-20 2.49    Ml     Changes for dependency builder to accept XGate compiler specific preprocessor
| 2006-03-28 2.50    Ard    Added C_COMP_ARM_OMAP
|                    Pl     Added C_COMP_TOSHIBA_TLCS900
|                    Svh    Added C_COMP_HPINFOTECH_AVR_CANARY
| 2006-04-27 2.51    dH     added C_COMP_MATSUSHITA_MN101E_CCAN
|                    Ap     added C_COMP_HITECH_PIC18
|                    Fn     modifications for C_COMP_MICROCHIP_PIC18
|                    Ces    Added C_COMP_GHS_TX49 and C_COMP_NEC_78K0_AFCAN
| 2006-05-02 2.52    Ces    Change MEMORY_NEAR and MEMORY_SADDR for C_COMP_NEC_78K0_AFCAN
|                    Svh    Added C_COMP_IAR_ADUC703X
| 2006-05-19 2.53    Ces    Change MEMORY_NEAR for C_COMP_NEC_78K0_AFCAN
|                    Ces    Change define V_NULL for C_COMP_MITSUBISHI_M32R
| 2006-08-03 2.54    Her    Added type cast for C_COMP_: COSMIC_08/NEC_78K0_AFCAN/COSMIC_MCS12X_MSCAN12
|                    Fn     Added C_COMP_MICROCHIP_DSPIC33
|                    Ap     Added C_COMP_MTRWRKS_MCS08_MSCAN and C_COMP_COSMIC_MCS08_MSCAN
|                    BWR    Added C_COMP_GAIO_MPC55XX 
|                    Pl     Added C_COMP_FUJITSU_16FX_CCAN
| 2006-08-04 2.55    Ces    Added C_COMP_GHS_LPC2XXX
| 2006-08-09 2.56    Svh    Added V_MEMRAM1_NEAR for C_COMP_NEC_78K0
| 2006-08-13 2.57    Ml     Changed bitfield type for C_COMP_COSMIC_MCS08_MSCAN
|                    Pl     Added C_COMP_ARM_AVCCORE1_CCAN  
| 2006-08-30 2.58    Wr     Added C_COMP_GNU_VR55XX_AFCAN
|            2.58    Ml     Added C_COMP_DIABDATA_MPC5X00_MSCAN
| 2006-09-22 2.59    swk    Added C_COMP_KEIL_ST10_CCAN
|                    Ml     Changed bitfield type for C_COMP_COSMIC_MCS08_MSCAN back to vuint8
| 2006-10-23 2.60    BWR    Added C_COMP_GNU_MICROBLAZE_LOGICORECAN
|                    Ard    Added C_COMP_GHS_MAC710X
| 2006-10-23 2.61    Bir    Added C_COMP_TASKING_XC2000_MULTICAN
| 2006-11-14         Ml     Added function near/far qualifier for platform MCS12X
| 2006-11-20         BWR    Added C_COMP_METROWERKS_MPC55XX_FLEXCAN
|                           Added C_COMP_DIABDATA_MCORE_FLEXCAN
| 2006-11-20         Bir    changes for C_COMP_TASKING_XC2000_MULTICAN
| 2006-12-14         Pl     Added C_COMP_TI_TMS470_DCAN
| 2007-01-10         Bir    changes for C_COMP_TASKING_XC2000_MULTICAN
| 2007-01-11         Ces    Added C_COMP_IAR_MCS12_MSCAN12
| 2007-02-01 2.62    Svh    Added C_COMP_FUJITSU_8FX
| 2007-02-19 2.63    Ces    Added C_COMP_GHS_SAF7780_PELICAN
|                    Ces    Changed MEMORY_NORMAL for C_COMP_MITSUBISHI_M32C
| 2007-06-21 2.64    Ces    Changed MEMORY_NORMAL for C_COMP_MITSUBISHI_M16C
|                    Fn     Added C_COMP_IAR_R32C
|                    BWR    Added C_COMP_DIABDATA_MCF_FLEXCAN
|                    Awh    Added C_COMP_KEIL_XC16X 
|                    Ou     Changed vbittype for C_COMP_MTRWRKS_MCS12X_MSCAN12 to unsigned char, additionally disabled warning C1106 for this compiler (see #pragma MESSAGE DISABLE C1106)
| 2007-09-07 2.65    Ou     Changed back vbittype for C_COMP_MTRWRKS_MCS12X_MSCAN12 to unsigned int
|                    Ard    Added C_COMP_GHS_TMS470
| 2007-09-28 2.66    Fn     Added C_COMP_COSMIC_ST79_BECAN3
|                    Ces    Added V_MEMROMx_NEAR and V_MEMROMx_FAR for C_COMP_IAR_M16C
|                           Added V_MEMROMx_NEAR and V_MEMROMx_FAR for C_COMP_IAR_M32C
|                           Use predefined compiler macros __VX__ and _C166 for C_COMP_TASKING_XC2000_MULTICAN
| 2007-10-24 2.67    Fn     Changed C_COMP_COSMIC_ST79_BECAN3 to C_COMP_COSMIC_STM8A_BECAN3
|                    Fn     Added C_COMP_RENESAS_R32C
| 2007-11-09 2.68    Wr     Added C_COMP_KEIL_SLC8051
| 2007-11-14 2.69    Svh    Added C_COMP_KEIL_ADUC703X
| 2007-11-27 2.70    Fn     Adapted rom qualifier for: C_COMP_RENESAS_R32C, C_COMP_IAR_R32C and C_COMP_MICROCHIP_DSPIC33
|                    Ml     Added memory mapping defines for C_COMP_MTRWRKS_MCS12X_MSCAN12
| 2007-12-03 2.71    Ard    Added C_COMP_MICROSOFT_TMS320ARM
| 2008-01-14 2.72    Ou     Added memory mapping defines for C_COMP_MTRWRKS_MCS08_MSCAN and C_COMP_COSMIC_MCS08_MSCAN
|                    Ces    Added C_COMP_GHS_MPC55XX and C_COMP_GHS_MPC55XX_FLEXCAN2
|                           Added C_COMP_GAIO_MPC55XX_FLEXCAN2
|                           Added C_COMP_DIABDATA_MPC55XX_FLEXCAN2
|                           Changed memory qualifier for IAR M16C and IAR M32C
| 2008-02-19 2.73    Ces    Corrected V_NULL for DIABDATA_MPC55XX
| 2008-03-05 2.74    Pl     Added C_COMP_GNU_NIOS_DCAN
| 2008-03-12         Fn     Added V_MEMROM2_FAR and V_MEMROM2_NEAR for C_COMP_IAR_AVR_CANARY
| 2008-04-18         Ap     Added C_COMP_GNU_AVR_CANARY
| 2008-05-05 2.75    Bir    Possibility to change V_MEMROM2 for XC2000 (user config)
| 2008-05-05 2.76    Ou     Change the behavior for V_MEMROM3 on MCS12X (relevant just for Cosmic compiler, global constants feature)
|****************************************************************************/

#ifndef  V_DEF_H
#define  V_DEF_H

#include "basic_types.h"
#include <vio.h>
/***************************************************************************/
/* Version                  (abcd: Main version=ab, Sub Version=cd )       */
/***************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : Common_Vdef CQComponent : Implementation */
#define COMMON_VDEF_VERSION            0x0276u
#define COMMON_VDEF_RELEASE_VERSION    0x00u

/* compatibility for IL versions < 3.52 */
#define V_DEF_VERSION                  COMMON_VDEF_VERSION

/***************************************************************************/
/* Supported Systems                                                       */
/***************************************************************************/
#if defined( C_COMP_COSMIC_MCS12X_MSCAN12 )     

# define V_DEF_SUPPORTED_PLATFORM
#endif


#if !defined V_DEF_SUPPORTED_PLATFORM
# error "driver not supported or C_COMP_xxx_yyy not defined"
#endif




/***************************************************************************/
/***************************************************************************/
/****  Hardware independent settings  **************************************/
/***************************************************************************/
/***************************************************************************/
/*--- standard memory qualifier definition --------------------------------*/

/* 8-Bit qualifier */
typedef unsigned char  vuint8;
typedef signed char    vsint8;
# define canuint8 vuint8
# define cansint8 vsint8

/* 16-Bit qualifier */
typedef unsigned short vuint16;
typedef signed short   vsint16;
#define canuint16 vuint16
#define cansint16 vsint16

/* 32-Bit qualifier */
typedef unsigned long  vuint32;
typedef signed long    vsint32;
#define canuint32 vuint32
#define cansint32 vsint32


typedef unsigned char *TxDataPtr;              /* ptr to transmission data buffer */
typedef unsigned char *RxDataPtr;              /* ptr to receiving data buffer    */

/***************************************************************************/
/*  Defines                                                                */
/***************************************************************************/



/***************************************************************************/
/***************************************************************************/
/****  Hardware/Compiler dependent settings   ******************************/
/***************************************************************************/
/***************************************************************************/

/*  data type according to the CPU type */
#if defined( C_CPUTYPE_8BIT )
/* 8-Bit qualifier */
# ifndef vuintx
#  define vuintx vuint8
# endif
# ifndef vsintx
#  define vsintx vsint8
# endif
#else
# if defined( C_CPUTYPE_16BIT )
/* 16-Bit qualifier */
#  ifndef vuintx
#   define vuintx vuint16
#  endif
#  ifndef vsintx
#   define vsintx vsint16
#  endif
# else
#  if defined( C_CPUTYPE_32BIT )
/* 32-Bit qualifier */
#   ifndef vuintx
#    define vuintx vuint32
#   endif
#   ifndef vsintx
#    define vsintx vsint32
#   endif
#  endif
# endif
#endif


/* qualifier for bitfield definition */

#if defined( C_COMP_COSMIC_MCS12X_MSCAN12 )     
typedef unsigned char    vbittype;
#else
typedef unsigned int     vbittype;
#endif
#define canbittype       vbittype



#if defined( C_SUPPORT_MIXED_CAN_LIN )
  /* support combination of "old" CAN driver and "new" LIN driver */
#else
/* Structure for bit accessed memory.           */
/* The bit-order is hardware/compiler dependent */
#  if defined( C_CPUTYPE_BITORDER_LSB2MSB )
struct _c_bits8
{
  canbittype  b0:1;  /* LSB-Bits (lower adr!) */
  canbittype  b1:1;
  canbittype  b2:1;
  canbittype  b3:1;
  canbittype  b4:1;
  canbittype  b5:1;
  canbittype  b6:1;
  canbittype  b7:1;  /* MSB-Bits (higher address) unused */
 };

struct _c_bits16 
{
  canbittype  b0:1;  /* lower adr */
  canbittype  b1:1;
  canbittype  b2:1;
  canbittype  b3:1;
  canbittype  b4:1;
  canbittype  b5:1;
  canbittype  b6:1;
  canbittype  b7:1;

  canbittype  b10:1; /* higher adr */
  canbittype  b11:1;
  canbittype  b12:1;
  canbittype  b13:1;
  canbittype  b14:1;
  canbittype  b15:1;
  canbittype  b16:1;
  canbittype  b17:1;
};

struct _c_bits32 
{
  canbittype  b0:1;  /* lower adr */
  canbittype  b1:1;
  canbittype  b2:1;
  canbittype  b3:1;
  canbittype  b4:1;
  canbittype  b5:1;
  canbittype  b6:1;
  canbittype  b7:1;

  canbittype  b10:1; /* mid low adr */
  canbittype  b11:1;
  canbittype  b12:1;
  canbittype  b13:1;
  canbittype  b14:1;
  canbittype  b15:1;
  canbittype  b16:1;
  canbittype  b17:1;

  canbittype  b20:1; /* mid high adr */
  canbittype  b21:1;
  canbittype  b22:1;
  canbittype  b23:1;
  canbittype  b24:1;
  canbittype  b25:1;
  canbittype  b26:1;
  canbittype  b27:1;

  canbittype  b30:1; /* higher adr */
  canbittype  b31:1;
  canbittype  b32:1;
  canbittype  b33:1;
  canbittype  b34:1;
  canbittype  b35:1;
  canbittype  b36:1;
  canbittype  b37:1;
};

# else /* MSB2LSB */
#  if defined( C_CPUTYPE_BITORDER_MSB2LSB )
struct _c_bits8         
{
  canbittype  b7:1;  /* MSB-Bits (Higher addr!) */
  canbittype  b6:1;
  canbittype  b5:1;
  canbittype  b4:1;
  canbittype  b3:1;
  canbittype  b2:1;
  canbittype  b1:1;
  canbittype  b0:1;  /* LSB-Bits (higher address) unused */
};

struct _c_bits16 
{
  canbittype  b7:1;   /* lower adr */
  canbittype  b6:1;
  canbittype  b5:1;
  canbittype  b4:1;
  canbittype  b3:1;
  canbittype  b2:1;
  canbittype  b1:1;
  canbittype  b0:1;

  canbittype  b17:1;  /* higher adr */
  canbittype  b16:1;
  canbittype  b15:1;
  canbittype  b14:1;
  canbittype  b13:1;
  canbittype  b12:1;
  canbittype  b11:1;
  canbittype  b10:1;
};


struct _c_bits32 
{
  canbittype  b7:1;   /* lower adr */
  canbittype  b6:1;
  canbittype  b5:1;
  canbittype  b4:1;
  canbittype  b3:1;
  canbittype  b2:1;
  canbittype  b1:1;
  canbittype  b0:1;

  canbittype  b17:1;  /* mid low adr */
  canbittype  b16:1;
  canbittype  b15:1;
  canbittype  b14:1;
  canbittype  b13:1;
  canbittype  b12:1;
  canbittype  b11:1;
  canbittype  b10:1;

  canbittype  b27:1;  /* mid high adr */
  canbittype  b26:1;
  canbittype  b25:1;
  canbittype  b24:1;
  canbittype  b23:1;
  canbittype  b22:1;
  canbittype  b21:1;
  canbittype  b20:1;

  canbittype  b37:1;  /* higher adr */
  canbittype  b36:1;
  canbittype  b35:1;
  canbittype  b34:1;
  canbittype  b33:1;
  canbittype  b32:1;
  canbittype  b31:1;
  canbittype  b30:1;
};
#  else
#  error "Bitorder unknown: C_CPUTYPE_BITORDER_MSB2LSB or C_CPUTYPE_BITORDER_LSB2MSB has to be defined"
#  endif /* C_CPUTYPE_BITORDER_MSB2LSB */
# endif  /* C_CPUTYPE_BITORDER_LSB2MSB */
#endif




/***************************************************************************/
/* Memory qualifier                                                        */
/***************************************************************************/























#if defined(C_COMP_COSMIC_MCS12X_MSCAN12)
# define V_API_FAR           @far
# define V_CALLBACK_FAR      @far
# define V_INTERNAL_FAR      @far
# define V_API_NEAR          @near
# define V_CALLBACK_NEAR     @near
# define V_INTERNAL_NEAR     @near

# define VUINT8_CAST         (vuint8)
# define VSINT8_CAST         (vsint8)
# define VUINT16_CAST        (vuint16)
# define VSINT16_CAST        (vsint16)
# define CANBITTYPE_CAST     (canbittype)
# define CANSINTCPUTYPE_CAST (int)
# define CANUINTCPUTYPE_CAST (unsigned int)

# if defined(V_ENABLE_USED_GLOBAL_CONST)
#  if defined( MAKE_DEPEND )
/* The GNU compiler does not understand the following if-directive => make the dependency generator work... */
#  else
#   define __XGATE_CHECK_VAR__ __TRGT__[0]
#   if(__XGATE_CHECK_VAR__ == 'X')
#    define V_MEMROM1         /* XGate part */
#    define V_MEMROM3         /* XGate part */
#   else
#    define V_MEMROM1 @gpage  /* MCS12x part */
#    if (V_COMPVERSION >= 407)
#     define V_MEMROM3 @far   /* MCS12x part */
#    else
#     define V_MEMROM3        /* MCS12x part */
#    endif
#   endif
#  endif
# endif
#endif






#ifndef MEMORY_HUGE
#  define MEMORY_HUGE               /* no entry                         */
#endif









/* default defines */
          /* used to store permanent data  */
#ifndef V_MEMROM0
# define V_MEMROM0                  /* addition qualifier data access in ROM  */
#endif

#ifndef V_MEMROM1_NEAR
# define V_MEMROM1_NEAR             /* fast data access in ROM */
#endif

#ifndef V_MEMROM1
# define V_MEMROM1                 /* fast data access in ROM */
#endif

#ifndef V_MEMROM1_FAR
# define V_MEMROM1_FAR             /* slow addressing mode in ROM */
#endif


#ifndef MEMORY_ROM_NEAR
# ifndef V_MEMROM2_NEAR
#  define V_MEMROM2_NEAR   const    /* fast data access in ROM */
# endif
  /* compatibility for modules which use old definition of memory qualifer */
# define MEMORY_ROM_NEAR   V_MEMROM2_NEAR
#else
# define V_MEMROM2_NEAR    MEMORY_ROM_NEAR
#endif

#ifndef MEMORY_ROM
# ifndef V_MEMROM2
#  define V_MEMROM2        const    /* fast data access in ROM */
# endif
  /* compatibility for modules which use old definition of memory qualifer */
# define MEMORY_ROM        V_MEMROM2       
#else
# define V_MEMROM2         MEMORY_ROM
#endif

#ifndef MEMORY_ROM_FAR
# ifndef V_MEMROM2_FAR
#  define V_MEMROM2_FAR    const    /* slow addressing mode in ROM */
# endif
  /* compatibility for modules which use old definition of memory qualifer */
# define MEMORY_ROM_FAR    V_MEMROM2_FAR
#else
# define V_MEMROM2_FAR     MEMORY_ROM_FAR
#endif

#ifndef V_MEMROM3
# define V_MEMROM3        
#endif


/* default defines */
           /* used to store volatile data  */
#ifndef V_MEMRAM0
# define V_MEMRAM0                  /* addition qualifier data access in RAM  */
#endif

#ifndef V_MEMRAM1_NEAR
# define V_MEMRAM1_NEAR             /* fast data access in RAM */
#endif

#ifndef V_MEMRAM1
# define V_MEMRAM1                 /* fast data access in RAM */
#endif

#ifndef V_MEMRAM1_FAR
# define V_MEMRAM1_FAR             /* slow addressing mode in RAM */
#endif


#ifndef MEMORY_NEAR
# ifndef V_MEMRAM2_NEAR
#  define V_MEMRAM2_NEAR           /* fast data access in RAM */
# endif
  /* compatibility for modules which use old definition of memory qualifer */
# define MEMORY_NEAR   V_MEMRAM2_NEAR
#else
# define V_MEMRAM2_NEAR    MEMORY_NEAR
#endif

#ifndef MEMORY_NORMAL
# ifndef V_MEMRAM2
#  define V_MEMRAM2                 /* fast data access in RAM */
# endif
  /* compatibility for modules which use old definition of memory qualifer */
# define MEMORY_NORMAL   V_MEMRAM2       
#else
# define V_MEMRAM2           MEMORY_NORMAL
#endif

#ifndef MEMORY_FAR
# ifndef V_MEMRAM2_FAR
#  define V_MEMRAM2_FAR            /* slow addressing mode in RAM */
# endif
  /* compatibility for modules which use old definition of memory qualifer */
# define MEMORY_FAR    V_MEMRAM2_FAR
#else
# define V_MEMRAM2_FAR     MEMORY_FAR
#endif

#ifndef V_MEMRAM3
# define V_MEMRAM3        
#endif

#if !defined(VUINT8_CAST)
# define VUINT8_CAST
#endif 
#if !defined(VSINT8_CAST)
# define VSINT8_CAST
#endif
#if !defined(VUINT16_CAST)
# define VUINT16_CAST
#endif
# if !defined(VSINT16_CAST)
# define VSINT16_CAST
#endif
#if !defined(CANBITTYPE_CAST)
# define CANBITTYPE_CAST
#endif
#if !defined(CANSINTCPUTYPE_CAST)
# define CANSINTCPUTYPE_CAST
#endif
#if !defined(CANUINTCPUTYPE_CAST)
# define CANUINTCPUTYPE_CAST
#endif

/***************************************************************************/
/*  Common_VStdLib                                                         */
/***************************************************************************/
# if defined (VGEN_ENABLE_VSTDLIB) || defined (V_ENABLE_VSTDLIB)
#  include "vstdlib.h"
# endif

/***************************************************************************/
/*  V_NULL Definition                                                      */
/***************************************************************************/



#if !defined(V_NULL)
# define V_NULL ((void*)0)
#endif


#ifndef NULL
# define NULL V_NULL
#endif


#endif /* V_DEF_H */


/* STOPSINGLE_OF_MULTIPLE */
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
