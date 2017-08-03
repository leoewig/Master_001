/**
 *
 * Measurement data manager interface types
 *
 * @file
 * @ingroup mdm
 * $Id: mdm_types_cust.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2015 - All rights reserved */

#if !defined(MDM_TYPES_CUST_H)
#define      MDM_TYPES_CUST_H

/** 
 * Maximum number of VTBMs which can be calibrated without needing to recompile
 */
#define MAX_NUM_VTBM            12u

typedef enum MdmBgShutdown_e
{
    MdmBgNotShutdown,
    MdmBgShutdownPend,
    MdmBgShutdownDone,
    MdmBgShutdownFailed
} MdmBgShutdown_t;

#endif      /* MDM_TYPES_CUST_H */
