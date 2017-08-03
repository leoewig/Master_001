/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   obd_ids_types.h
 *
 * description:  
 *      Macros and data types for reading Software, Calibration and Hardware
 *      identification over UDS
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined OBD_IDS_TYPES_H
#define OBD_IDS_TYPES_H


/** length of the s/w part number (excluding a trailing NULL) */
#define OBD_MCLAREN_SW_PART_NUMBER_LENGTH       13u

/** Maximum allowed length of the ECU name (excluding a 
 * trailing NULL) as defined in ISO-15031 */
#define OBD_ECU_NAME_MAX_LEN                    20u

/** Actual length of the ECU name (excluding a trailing NULL) */
#define OBD_ECU_NAME_LEN                        17u

/**
 * ECU Name as reported by OBD service $09 (PID $0A)
 */
#define OBD_ECU_NAME                    "BECM-B+EnergyCtrl"

/** Length of the McLaren hardware ID (excluding a trailing NULL) */
#define OBD_HW_ID_LEN                           13u

/**
 * McLaren part number for the battery pack (DID 0xF191)
 */
#define OBD_HARDWARE_ID                 "000000000.00."

/**
 * Application s/w part number
 */

#ifdef GTR_BUILD
/* Release 15.2 */
#define OBD_MCLAREN_SW_PART_NUMBER      "12M0971RP.01."
#else
/* Release 15.1 */
#define OBD_MCLAREN_SW_PART_NUMBER      "12M0845CP.11."
#endif

/**
 * H/W design date in BCD YYWW format
 * @todo decide whether to replace this with a genuine date
 */
#define OBD_HW_DESIGN_DATE                      0x1301u

/**
 * Supplier ID for Axeon/Linkedriving 0 -> Not assigned
 */
#define OBD_SUPPLIER_IDENTIFICATION             0u

/**
 * Default value to be used when a BCD YYWW-format version is unavailable
 */
#define OBD_DEFAULT_VERSION                     0x1111u

/**
 * Structure for the IDBlock reported over UDS
 */
typedef struct {
    uint16_t hw_version;
    uint16_t blsw_version;
    uint16_t appsw_version;
    uint16_t cal_version;
    char_t mclaren_sw_partnumber[OBD_MCLAREN_SW_PART_NUMBER_LENGTH];
    uint8_t supplier_identification;
    uint16_t diagnostic_sw_version;
} obd_idblock_s;

#endif /* if !defined OBD_IDS_TYPES_H */
