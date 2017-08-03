/* STARTSINGLE_OF_MULTIPLE */
/*****************************************************************************
| Project Name: CAN Driver
|    File Name: can_inc.h
|
|  Description: definition of can driver include files
|               Please adapt such that the compiler finds the following files:
|                            CAN_CFG.H
|                            CAN_DEF.H
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2006 by Vector Informatik GmbH.      All rights reserved.
|
| This software is copyright protected and proprietary 
| to Vector Informatik GmbH. Vector Informatik GmbH 
| grants to you only those rights as set out in the 
| license conditions. All other rights remain with 
| Vector Informatik GmbH.
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     ------------------------------------
| Ht           Heike Honert              Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
| Date         Ver  Author  Description
| ---------    ---  ------  --------------------------------------------------
| 08-Mai-01    1.0   Ht     creation
| 2003-01-28   1.1   Ht     protection from multiple include of this file
| 2006-03-23   1.2   Ht     include osek.h before can_def.h due to ISR-macro
|                           remove include of nm_cfg.h
| 2007-03-05   1.3   Ht     include v_par.h to support multiple ECU
|****************************************************************************/
#ifndef _CAN_INC_H
#define _CAN_INC_H

#include "can_cfg.h"               /* dependend configuration of the driver.*/
                                   /* configuration file.                   */
#if defined ( C_ENABLE_OSEK_OS )
 #include "osek.h"                  /*include of OSEK-Header */
/* #error "insert the include for OSEK OS and delete this line" */
#endif

#if defined(VGEN_GENY)
#include "v_par.h"                 /* some vector internal defines          */
#endif

#include "can_def.h"               /* CPU CAN driver header                     */
#include "xcan_def.h"              /* Co-processor CAN driver header            */


/* Include Register header of used controller derivative */
#endif /* _CAN_INC_H */
/* STOPSINGLE_OF_MULTIPLE */
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
