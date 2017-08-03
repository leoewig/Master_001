/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    nvs_p.h
 *
 * description:  Defines each feature's NVS memory usage.
 *   
 *   
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined NVS_CFG_H
#define NVS_CFG_H
/**
 * @file
 * @ingroup nvs
 *
 * Defines each feature's NVS memory usage.
 *
 */
/* Copyright (c) Linkedriving Tech 2012 */
#include "nvs_types.h"          /* needed by everyone using this file */


#define NVS_EEPROM_BANK0_START        0u      /**< EEPROM address of 1st byte
                                                  of NVS information */
#define NVS_EEPROM_BANK_SIZE          4096u   /**< EEPROM space devoted to
                                                  each NVS bank */
/**
 * EEPROM address immediately following NVS data, used to check it doesn't
 * overlap with the calibration area
 */
#define NVS_EEPROM_END_ADDRESS          (NVS_EEPROM_BANK0_START + \
                                                 (2u*NVS_EEPROM_BANK_SIZE))
/**
 * A list of Feature which have Non-Volatile data to be restored from the eeprom
 * and written to the eeprom. These enum constant are to be used in sequence,
 * as defined here, while registering respective modules in the nvs config array
 * @ref nvs_feat_list[].
 */
typedef enum
{
    //Nvs_Log_feature,    /**< Logging */
    //Nvs_Fm_feature,     /**< Fault Management */
    //Nvs_Obd_feature,    /**< On-Board Diagnostics */
    //Nvs_Umc_feature,    /**< Unmetered charge and balancing */
    Nvs_Mdm_feature,    /**< Cell management interface */
    //Nvs_Ctc_feature,    /**< Contactor control. */
    //Nvs_Cmd_feature,    /**< Charger mode determination */
    Nvs_Cal_feature,     /**< Calibration */
    Nvs_Rtc_feature,    /**< Real-Time Clock */
    Nvs_Max_feature     /**< Count of features with NVS.  Must come last! */
} Nvs_feature_list_e;


/*
 * ----------------------------------------------------------------------
 * You MUST make sure that:
 *      1. the total of the below is less than 4096 bytes
 *      2. The largest value is saved as NVS_LARGEST_FEATURE_BYTES
 * The EEPROM can write 64-byte pages efficiently, so keep the definitions
 * aligned to 64-byte boundaries if possible.
 * ----------------------------------------------------------------------
 */
//#define LOG_MAX_NVS_BYTES     992u /**< Log storage including NVS header */
//#define FM_MAX_NVS_BYTES      256u /**< FM storage including NVS header */
#define LOG_MAX_NVS_BYTES     32u /**< Log storage including NVS header */
#define FM_MAX_NVS_BYTES      32u /**< FM storage including NVS header */
//#define OBD_MAX_NVS_BYTES    1246u /**< OBD storage including NVS header */
#define OBD_MAX_NVS_BYTES    64u /**< OBD storage including NVS header */
//#define UMC_MAX_NVS_BYTES    1346u /**< UMC (and BAL) storage including NVS header */
#define UMC_MAX_NVS_BYTES    128u /**< UMC (and BAL) storage including NVS header */
//#define MDM_MAX_NVS_BYTES     128u /**< Cell management storage including NVS
#define MDM_MAX_NVS_BYTES     2048u /**< Cell management storage including NVS
                                        header */
#define CTC_MAX_NVS_BYTES      32u /**< Contactor storage including NVS header */
#define CMD_MAX_NVS_BYTES      32u /**< Charger mode determination storage including NVS header */
#define RTC_MAX_NVS_BYTES      32u /**< Real-Time Clock storage including NVS header*/
#define CAL_MAX_NVS_BYTES      24u /**< Calibration-support storage including
                                        NVS header (N.B. Actual calibration data
                                        is stored elsewhere on the EEPROM and not
                                        via NVS) */
#define RTC_MAX_NVS_BYTES    32u   /**< Real-Time Clock storage */

#define NVS_RESERVED_TOTAL (LOG_MAX_NVS_BYTES + FM_MAX_NVS_BYTES + \
    OBD_MAX_NVS_BYTES + UMC_MAX_NVS_BYTES + BAL_MAX_NVS_BYTES + \
    MDM_MAX_NVS_BYTES + CTC_MAX_NVS_BYTES + CMD_MAX_NVS_BYTES + \
    RTC_MAX_NVS_BYTES + CAL_MAX_NVS_BYTES)

#if NVS_RESERVED_TOTAL >= 4096u
#error Total size must be less than 4096 bytes
#endif


/**
 * NVS buffer used to store a single feature's data before computing the
 * CRC.  This must be at least as big as the largest feature's usage.
 */
#define NVS_LARGEST_FEATURE_BYTES   4096u

/**
 * <!-- comment attached to file because it applies to mulitple variables -->
 * @file
 * @lint -esym(768,reserved_bytes): inhibit Lint warning that reserved_bytes[]
 *      arrays aren't accessed
 */
/*lint -esym(768,reserved_bytes) */
#endif /* #if !defined NVS_CFG_H */
