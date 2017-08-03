/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     mmc_isa_cfg.h
 *
 * description:  
 * MMC ISA Driver configuration
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/
#ifndef ISA_CFG_H
#define ISA_CFG_H
/**
 * @file
 * @ingroup mmc_cfg
 * @ingroup isa
 *
 * MMC ISA Driver configuration
 *
 *
 */

/******************************************************************************
 * CONFIGURATION
 *****************************************************************************/

 /** ISA message IDs */
#define ISA_CMD_SET_MODE       0x12u     /**< SET_MODE command ID */
#define ISA_CMD_SET_CONF       0x13u     /**< SET_CONFIG command ID */
#define ISA_CMD_GET_CONF       0x43u     /**< GET_CONFIG command ID */
#define ISA_CMD_GET_VER        0x4Eu     /**< GET_SW_VERSION command ID */
#define ISA_CMD_GET_STATUS     0x47u     /**< GET_STATUS command ID */
#define ISA_CMD_GET_SERIAL     0x4Fu     /**< GET_SERIAL_NUMBER command ID */
#define ISA_CMD_STORE          0x21u     /**< STORE function command ID */
#define ISA_RSP_MODE           0x82u     /**< SET_MODE response message ID */
#define ISA_RSP_CONFIG         0x83u     /**< SET_CONFIG response message ID */
#define ISA_RSP_STATUS         0x87u     /**< GET_STATUS Resoncse message ID */
#define ISA_RSP_SW_VER         0x8Eu     /**< SW_VERSION response message ID */
#define ISA_RSP_SERIAL         0x8Fu     /**< ISA_CMD_GET_SERIAL response message ID */

/**< ISA device configuration (SET_CONFIG message):
 * -# Free running measurement mode
 * -# Results frame freq not less than 100Hz
 * -# Secondary measurement channel current filtered input active
 * -# Start measurements immediately
 */
#define ISA_CONF_DB1     0x11u /**< Free running, Run */
#define ISA_CONF_DB1_ALT 0x01u /**< Free running only */
#define ISA_CONF_DB2     0x02u /**< Bit 1=1: current filtered input active */
#define ISA_CONF_DB3     0x0Eu /**< Averaging- 0x0E a reading every 9.8ms */
#define ISA_CONF_DB4     0x00u /**< Voltage divider switched off */
#define ISA_CONF_DB5     0xF4u /**< Low byte of baud rate required */
#define ISA_CONF_DB6     0x01u /** High byte of baud rate required */

/**< ISA Store message:
 * -# Configuration Set
 */
#define ISA_STORE_DB1    0x13u /**< Configuration Set */


#define ISA_MAX_RAW_RESULT     (1500000)   /**< The maximum allowable value for the
                                             raw current result */
#define ISA_MIN_RAW_RESULT    (-1500000)  /**< The minimum allowable value for the
                                             raw current result */

#define ISA_VER_LENGTH        3u      /**< Number of characters that make up
                                          the ISA version in #isa_sw_versions */
#define ISA_NUM_VERS          2u      /**< Number of different ISA versions
                                          supported in #isa_sw_versions */
#define ISA_INPUT_TYPE        2u      /**< Current filtered */

/**
 * Enumeration commands for SET_MODE
 * Note: the numeric values form part of the request command
 */
enum isa_set_mode_e  {
    Isa_set_mode_stop = 0x00u,
    Isa_set_mode_run = 0x01u
};

extern CONST uint8_t isa_sw_versions [ISA_NUM_VERS][ISA_VER_LENGTH];


#endif /* ISA_CFG_H */

