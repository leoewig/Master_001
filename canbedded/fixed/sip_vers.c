/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     sip_vers.c 
 *
 * description:   sip version
 *
 *
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           sip version for S12XEP100
 *
 *****************************************************************************/


#include "v_cfg.h"
#include "v_ver.h"
#include "v_def.h"
#if defined( VGEN_GENY )
 #include "v_inc.h"
#endif

/* ---------------------------------------------------------------------------
 * Can Driver
 * --------------------------------------------------------------------------*/
#if defined( VGEN_ENABLE_CAN_DRV )
#include "can_inc.h"
#endif

/* ---------------------------------------------------------------------------
 * CCL
 * --------------------------------------------------------------------------*/
#if defined( VGEN_ENABLE_CCL )
#include "ccl_inc.h"
#endif

/* ---------------------------------------------------------------------------
 * Diag_Candesc_uds
 * --------------------------------------------------------------------------*/
#ifdef VGEN_ENABLE_DIAG_CANDESC_UDS
#include "desc.h"
#endif

/* ---------------------------------------------------------------------------
 * IL Vector
 * --------------------------------------------------------------------------*/
#if defined( VGEN_ENABLE_IL_VECTOR )
#include "il_inc.h"
#include "il_def.h"
#endif

/* ---------------------------------------------------------------------------
 * Lin Driver
 * --------------------------------------------------------------------------*/
#ifdef VGEN_ENABLE_LIN_DRV
#include "lin_inc.h"
#endif

/* ---------------------------------------------------------------------------
 * Lin TP
 * --------------------------------------------------------------------------*/
#if defined ( DTL_ECUTYPE_MASTER ) || defined (DTL_ECUTYPE_SLAVE)
#include "dtl_lin.h"
#endif

/* ---------------------------------------------------------------------------
 * NM_osek
 * --------------------------------------------------------------------------*/
#if defined VGEN_ENABLE_NM_OSEK_D
#include "n_onmdef.h"
  #if defined( VGEN_GENY )
    #if defined (VGEN_ENABLE_NM_OSEK_D_UD)
#include "onmxdc.h"
    #endif
  #endif
#endif

/* ---------------------------------------------------------------------------
 * TPMC
 * --------------------------------------------------------------------------*/
#if defined( VGEN_ENABLE_TP_ISO_MC )
#include "tpmc.h"
#endif

/* ---------------------------------------------------------------------------
 * Description: VSTDLIB.H includes the Vector Standard Library header.
 *--------------------------------------------------------------------------*/
#if defined ( VGEN_ENABLE_VSTDLIB )
  #include "vstdlib.h"
#endif

/* ---------------------------------------------------------------------------
 * XCP
 * --------------------------------------------------------------------------*/
#if defined(VGEN_ENABLE_XCP)
#include "XcpProf.h"
#endif


#include "sip_vers.h"



/******************************************************************************
| version check
|*****************************************************************************/
/* Used to check source against header. This is NOT the SIP version! */
#if ( SIP_VERS_VERSION != _COMMON_SIPVERSIONCHECK_VERSION )
  #error "SipCheck: Different main/sub version in header and source!"
#endif
#if ( SIP_VERS_RELEASE_VERSION != _COMMON_SIPVERSIONCHECK_RELEASE_VERSION )
  #error "SipCheck: Different release versions header and source!"
#endif


/******************************************************************************
| check components (presence, versions, functionalities)
|*****************************************************************************/

/*----------------------------------------------------------------------------
  Can Driver
  ---------------------------------------------------------------------------*/
#if defined( VGEN_ENABLE_CAN_DRV )
#if defined( C_COMP_COSMIC_MCS12X_MSCAN12 )
 #define CAN_DRIVER_OK 
 #if( DRVCAN_MCS12XMSCANHLL_VERSION != _DRVCAN_HW_VERSION )
  #error "CanDrv: Wrong version detected for this SIP delivery!"
 #endif
 #if( DRVCAN_MCS12XMSCANHLL_RELEASE_VERSION != _DRVCAN_HW_RELEASE_VERSION )
  #error "CanDrv: Wrong version detected for this SIP delivery!"
 #endif
#endif
#endif

/*----------------------------------------------------------------------------
  Ccl
  ---------------------------------------------------------------------------*/
#if defined( VGEN_ENABLE_CCL )
 #if( CCL__CORE_VERSION != _CCL__CORE_VERSION )
  #error "Ccl: Wrong main/sub version detected for this SIP delivery!"
 #endif
 #if( CCL__CORE_RELEASE_VERSION != _CCL__CORE_RELEASE_VERSION )
  #error "Ccl: Wrong release version detected for this SIP delivery!"
 #endif

 #if (CCL_VECTOR_VERSION != _CCL_OEM_VERSION)
  #error "CCL_Oem: Source and Header file are inconsistent for this SIP!"
 #endif
 #if (CCL_VECTOR_RELEASE_VERSION != _CCL_OEM_RELEASE_VERSION)
  #error "CCL_Oem: Source and Header file are inconsistent for this SIP!"
 #endif
#endif

/*----------------------------------------------------------------------------
  DIAG_CANDESC_UDS
  ---------------------------------------------------------------------------*/
#ifdef VGEN_ENABLE_DIAG_CANDESC_UDS
 #if( DESC_VERSION != _DIAG_CANDESC__COREBASE_VERSION )
  #error "CanDesc: Wrong version detected for this SIP delivery!"
 #endif
 #if( DESC_RELEASE_VERSION != _DIAG_CANDESC__COREBASE_RELEASE_VERSION )
  #error "CanDesc: Wrong version detected for this SIP delivery!"
 #endif
#endif

/*----------------------------------------------------------------------------
  Il_Vector
  ---------------------------------------------------------------------------*/ 
#if defined( VGEN_ENABLE_IL_VECTOR)
 #if( IL_VECTOR_VERSION != _IL_VECTOR_VERSION )
  #error "IL: Wrong main/sub version detected for this SIP delivery!"
 #endif
 #if( IL_VECTOR_RELEASE_VERSION != _IL_VECTOR_RELEASE_VERSION )
  #error "IL: Wrong release version detected for this SIP delivery!"
 #endif
#endif

/* ---------------------------------------------------------------------------
 * Asr LinDrv
 * --------------------------------------------------------------------------*/
#if defined( VGEN_ENABLE_LIN_DRV )

#if defined( C_COMP_COSMIC_MCS12X_MSCAN12 ) 
 #define LIN_DRIVER_OK
#if defined( SIO_INTERFACE_SCISTD)
 #if( DRVLIN_MCS12XSCISTD_VERSION != _DRVLIN_HW_VERSION )
  #error "LinDrv: Wrong main or sub version detected for this SIP delivery!"
 #endif
 #if( DRVLIN_MCS12XSCISTD_RELEASE_VERSION != _DRVLIN_HW_RELEASE_VERSION )
  #error "LinDrv: Wrong release version detected for this SIP delivery!"
 #endif
#endif
#endif
#endif

/*----------------------------------------------------------------------------
  NM_OSEK_D
  ---------------------------------------------------------------------------*/ 
#if defined (VGEN_ENABLE_NM_OSEK_D)
 #if !defined NM_DIROSEK_VERSION
  #define NM_DIROSEK_VERSION 0 /* avoid compiler warning "'NM_DIROSEK_VERSION' is not defined,so it's treated as 0" */
 #endif
 #if !defined NM_OSEK_VERSION
  #define NM_OSEK_VERSION 0 /* avoid compiler warning "'NM_OSEK_VERSION' is not defined,so it's treated as 0" */
 #endif
 #if(( NM_DIROSEK_VERSION != _NM_DIROSEK_VERSION ) && \
	( NM_OSEK_VERSION != _NM_DIROSEK_VERSION ))
  #error "NM: Wrong version detected for this SIP delivery!"
 #endif
 #if !defined NM_DIROSEK_RELEASE_VERSION
  #define NM_DIROSEK_RELEASE_VERSION 0 /* avoid compiler warning "'NM_DIROSEK_RELEASE_VERSION' is not defined,so it's treated as 0" */
 #endif
 #if !defined NM_OSEK_BUGFIX_VERSION
  #define NM_OSEK_BUGFIX_VERSION 0 /* avoid compiler warning "'NM_OSEK_BUGFIX_VERSION' is not defined,so it's treated as 0" */
 #endif
 #if(( NM_DIROSEK_RELEASE_VERSION != _NM_DIROSEK_RELEASE_VERSION ) && \
	( NM_OSEK_BUGFIX_VERSION != _NM_DIROSEK_RELEASE_VERSION ))
  #error "NM: Wrong version detected for this SIP delivery!"
 #endif
#endif

/*----------------------------------------------------------------------------
  Tp_ISO_MC
  ---------------------------------------------------------------------------*/
#if defined( VGEN_ENABLE_TP_ISO_MC )
 #if( TP_ISO15765_VERSION != _TP_ISO15765_VERSION )
  #error "Tp_Iso15765: Wrong main/sub version detected for this SIP delivery!"
 #endif
 #if( TP_ISO15765_RELEASE_VERSION != _TP_ISO15765_RELEASE_VERSION )
  #error "Tp_Iso15765: Wrong release version detected for this SIP delivery!"
 #endif
#endif

/*----------------------------------------------------------------------------
  Tp_Iso15765 on LIN
  ---------------------------------------------------------------------------*/
#if defined ( DTL_ECUTYPE_MASTER )
 #if defined ( DTL_PROCESSING_RAW )
  #if( TPLIN_RAWMASTECU_VERSION != _TPLIN_RAWMASTECU_VERSION )
   #error "TP-LIN: Wrong version detected for this SIP delivery!"
  #endif
  #if( TPLIN_RAWMASTECU_RELEASE_VERSION != _TPLIN_RAWMASTECU_RELEASE_VERSION )
   #error "TP-LIN: Wrong release version detected for this SIP delivery!"
  #endif
 #endif
 #if defined ( DTL_PROCESSING_COOKED )
  #if( TPLIN_COOCKEDMASTECU_VERSION != _TPLIN_COOCKEDMASTECU_VERSION )
   #error "TP-LIN: Wrong version detected for this SIP delivery!"
  #endif
  #if( TPLIN_COOCKEDMASTECU_RELEASE_VERSION != _TPLIN_COOCKEDMASTECU_RELEASE_VERSION )
   #error "TP-LIN: Wrong release version detected for this SIP delivery!"
  #endif
 #endif
#endif
#if defined ( DTL_ECUTYPE_SLAVE )
 #if defined ( DTL_PROCESSING_COOKED )
  #if( TPLIN_COOCKEDSLAVECU_VERSION != _TPLIN_COOCKEDSLAVECU_VERSION )
   #error "TP-LIN: Wrong version detected for this SIP delivery!"
  #endif
  #if( TPLIN_COOCKEDSLAVECU_RELEASE_VERSION != _TPLIN_COOCKEDSLAVECU_RELEASE_VERSION )
   #error "TP-LIN: Wrong release version detected for this SIP delivery!"
  #endif
 #endif
#endif

/*----------------------------------------------------------------------------
  Common_VStdLib
  ---------------------------------------------------------------------------*/
#if defined( C_COMP_COSMIC_MCS12X_MSCAN12 )
 #if( VSTDLIB_MCS12X_VERSION != _VSTDLIB_HW_VERSION )
  #error "VStdLib: Wrong main/sub version detected for this SIP delivery!"
 #endif
 #if( VSTDLIB_MCS12X_RELEASE_VERSION != _VSTDLIB_HW_RELEASE_VERSION )
  #error "VStdLib: Wrong release version detected for this SIP delivery!"
 #endif
#endif

/*----------------------------------------------------------------------------
  Xcp
  ---------------------------------------------------------------------------*/
#if defined ( VGEN_ENABLE_XCP )
  #if ( CP_XCP_VERSION         != _CP_XCP_VERSION )
    #error "CP_XCP: Wrong version detected for this SIP delivery!"
  #endif
  #if ( CP_XCP_RELEASE_VERSION != _CP_XCP_RELEASE_VERSION   )
    #error "CP_XCP: Wrong version detected for this SIP delivery!"
  #endif
#endif

/*----------------------------------------------------------------------------
  V_DEF
  ---------------------------------------------------------------------------*/
#if ( COMMON_VDEF_VERSION != _COMMON_VDEF_VERSION   )
  #error "V_Def: Wrong version detected for this SIP delivery!"
#endif
#if ( COMMON_VDEF_RELEASE_VERSION != _COMMON_VDEF_RELEASE_VERSION )
  #error "V_Def: Wrong version detected for this SIP delivery!"
#endif


/******************************************************************************
| Store additional SIP information in ROM constants
|*****************************************************************************/
/* ROM CATEGORY 4 START*/
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kSipMainVersion   = (vuint8)(VGEN_DELIVERY_VERSION_BYTE_0);
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kSipSubVersion    = (vuint8)(VGEN_DELIVERY_VERSION_BYTE_1);
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kSipBugFixVersion = (vuint8)(VGEN_DELIVERY_VERSION_BYTE_2);
/* ROM CATEGORY 4 END*/
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
