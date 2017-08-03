#if !defined(CMD_TYPES_H)
#define      CMD_TYPES_H
/**
 *
 * cmd Charger mode determination public header file
 *
 * @file
 * @ingroup cmd
 * $Id: cmd_types.h 873 2015-03-16 11:19:41Z j_bailey $
 */
/* Copyright (c) Ricardo plc 2015 - All rights reserved */

#include "nvs_cfg.h"


/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

#define CMD_NVS_USED (sizeof(nvs_header_s) + sizeof(cmd_nvs_data_s))

/**
 * NVS layout version to ensure incompatible data isn't loaded after reflashing
 */
#define CMD_NVS_LAYOUT_VERSION          1u


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/** Stored charger cycle termination reason */
enum Cmd_term_reason_e
{
    /** No charge event stored */
    Cmd_term_none,
    /** battery charged to capacity */
    Cmd_term_batt_full,
    /** Charging terminated externally */
    Cmd_term_external,
    /** Aborted by BMS error */
    Cmd_term_bms_error,
    /** Aborted by charger error */
    Cmd_term_charger_error,
    /** 12V supply below minimum threshold */
    Cmd_term_12V_low,
    /** HVIL interrupted */
    Cmd_term_hvil,
    /* None of the above */
    Cmd_term_other=255u
};

/** CMD struct for NVS usage. Update CMD_NVS_LAYOUT_VERSION if changing 
 */
typedef struct
{
    uint8_t         spare;

} cmd_nvs_data_s;

typedef struct
{
    nvs_header_s header;  /**< Feature Information Structure.*/
    cmd_nvs_data_s data;   /**< Module NV data-set. */
    uint8_t reserved_bytes[(int16_t) CMD_MAX_NVS_BYTES - (int16_t) CMD_NVS_USED];
                                            /**< Feature Reserved_bytes.*/
} cmd_nvs_wrapper_s;

#endif      /* CMD_TYPES_H */

