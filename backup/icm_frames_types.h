/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    icm_frames_types.h
 *
 * description:  
 * Type definitions for low-level internal comms.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(ICM_FRAMES_TYPES_H)
#define      ICM_FRAMES_TYPES_H
/**
 *
 * Type definitions for low-level internal comms.
 *
 * @file
 * @ingroup icm_client_frames
 * @ingroup icm_server_frames
 *
 */

#include "basic_types.h"
#include "icm_types.h"
#include "utils.h"
/* This header uses temperature and voltage datatypes from MMC
 header */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/******************************************************************************
 * Macros for frame identification
 *****************************************************************************/

/**
 * 11-bit Internal CAN all-bits-set mask */
#define ICAN_ID_MASK    0x7FFu

/** The default mask into the penultimate byte which is filled by the cycle
 * counter */
#define ICM_CYCLE_CTR_MASK      0x0Fu

/**
 * A value that can be used to seed (8 bit) counter variablesstatus=
 * that will be different from all valid counters
 */
#define ICM_INVALID_COUNTER 0xFFu

#if ICM_INVALID_COUNTER <= CYCLE_CTR_MASK
#error The invalid counter value must not be less than or equal to the mask
#endif

/**
 * Check whether a CAN ID matches a given ID with interest mask */
#define ID_MATCHES(lhs_,rhs_,mask_) ( ((lhs_)&(mask_))==((rhs_)&(mask_)) )

/******************************************************************************
 * CAN IDs
 *****************************************************************************/

/**
 * The CAN ID for Broadcast Measure control */
#define MEASCTL_ID          0x032u

/** The base CAN ID for VTBM status response */
#define VTBM_STATUS_BASE    0x540u

/** The mask within a CAN ID that identifies the VTBM number */
#define VTBM_NUM_MASK       0x1Fu

/**
 * The mask of interest bits in a CAN ID that would match any VTBM
 */
#define     ANY_VTBM_MASK           0x7E0u

#if (ANY_VTBM_MASK | VTBM_NUM_MASK) != ICAN_ID_MASK
#error VTBM mask inconsistency
#endif

/**
 * Extract the VTBM Index from a VTBM status CAN ID
 * @param[in] canid an 11-bit CAN ID 
 * @returns a VTBM number in the range 0 to 31*/
#define VTBM_CAN_IDX(canid) (uint8_t)( (canid) & VTBM_NUM_MASK)

/** The base CAN ID used for VTBM balancing control messages */
#define ICM_VTBMCTL_BASE_ID 0x140u

#define MAKE_VTBMCTL_ID(vtbm_num) ((uint16_t)ICM_VTBMCTL_BASE_ID + (vtbm_num))

/** The CAN ID for a system config. message */
#define     ICM_SYSCONFIG_ID        0x0B8u

/** CAN ID for network control */
#define     ICM_NETCTL_ID           0x0BFu

/*****************************************************************************
 * Message representation constants
 *****************************************************************************/

/**
 * The overall protocol version. If the CAN IDs change, so must this number */
#define ICM_PROTOCOL_VER 2u

/**
 * The Layout ID for the configuration block. This is separate from the 
 * protocol ID to allow new configuration information to be added without
 * affecting the rest of the protocol.
 **/
#define ICM_CFG_FMT_ID 1u

/*****************************************************************************
 * Measure control message
 *****************************************************************************/

/** The byte index of the action field */
#define MEASCTL_ACT_IDX     0u
/** The byte index of the target field */
#define MEASCTL_TGT_IDX     1u
/** The byte index of the counter field */
#define MEASCTL_CTR_IDX     2u
/** The byte index of the checksum field */
#define MEASCTL_CSUM_IDX    3u
/** The size of the measure control frame */
#define MEASCTL_SIZE        4u
/** The number of bytes in the Measure control message excluding the checksum */
#define MEASCTL_DATA_BYTES  (MEASCTL_SIZE-1u)

/**
 * Command code to a VTBM to measure without first selecting a cell
 */
#define MEASCTL_MEAS_NONE_MASK      0x10u
/**
 * Command code to a VTBM to measure configured cell voltages
 */
#define MEASCTL_MEAS_CFG_MASK       0x1Eu
/**
 * Command code to a VTBM to measure ALL cell voltages
 */
#define MEASCTL_MEAS_ALL_MASK       0x1Fu

/**
 * The overall mask to use when extracting the measure target fields
 */
#define MEASCTL_MEAS_TARGET_MASK    0x1Fu
/** The mask to apply in order to get a message counter of a control frame */
#define ICM_MEASCTL_COUNTER_MASK    ICM_CYCLE_CTR_MASK

/**
 * The shift to apply in order to store a balancing action in a byte.
 */
#define MEASCTL_BAL_ACT_SHIFT       4
/**
 * The shift to apply in order to store a balancing action in a byte.
 */
#define MEASCTL_BAL_CELL_MASK       0xFu
/** The control mask to apply in order to command a VTBM to measure with 
 * active balancing enabled */
#define MEASCTL_MEAS_ACTIVE_MASK    0x80u
/** The control mask to apply in order to command a VTBM to measure with 
 * passive balancing enabled */
#define MEASCTL_MEAS_PASSIVE_MASK   0x40u

/*****************************************************************************
 * VTBM individual control message
 *****************************************************************************/

/** The length in bytes of a VTBM balancing control frame */
#define ICM_VTBMCTL_BAL_LENGTH          4u
/** The position of the balancing command in a VTBM control frame */
#define ICM_VTBMCTL_BAL_IDX             0u
/** The position of the Clear error code flag in a VTBM control frame */
#define ICM_VTBMCTL_CLR_ERR_FLG_IDX     0u
/** The position of the Clear CAN flags flag in a VTBM control frame */
#define ICM_VTBMCTL_CLR_CAN_FLG_IDX     1u
/** The position of the frame counter in a VTBM control frame */
#define ICM_VTBMCTL_COUNTER_IDX         2u
/** The position of the balancing control checksum in a VTBM control frame */
#define ICM_VTBMCTL_CSUM_IDX            3u

/** The control mask to apply in order to command a VTBM to clear its internal error code */
#define ICM_VTBMCTL_CLR_ERR_FLG_MASK    0x40u
/** The mask to apply in order to get a balance action from a control frame */
#define ICM_VTBMCTL_BAL_ACT_MASK        0x30u
/** The mask to apply in order to get a balance cell from a control frame */
#define ICM_VTBMCTL_BAL_CELL_MASK       0x0Fu
/** The mask to apply in order to get a message counter of a control frame */
#define ICM_VTBMCTL_COUNTER_MASK        ICM_CYCLE_CTR_MASK

/** The shift to apply in order to store a Clear error code flag in a byte */
#define ICM_VTBMCTL_CLR_ERR_FLG_SHIFT   6u
/** The shift to apply in order to store a balance action in a byte */
#define ICM_VTBMCTL_BAL_ACT_SHIFT       4u
/****************************************************************************
 * Network control message
 ****************************************************************************/

/**
 * The number of bytes in a network control frame including checksum
 */
#define ICM_NETCTL_LENGTH       8u
/** Position of the protocol version byte */
#define ICM_NETCTL_VER_IDX      0
/** not used - zero padding */
#define ICM_NETCTL_UNUSED1_IDX  1
/** Position of the reply requested byte */
#define ICM_NETCTL_RREQ_IDX     2
/** not used - zero padding */
#define ICM_NETCTL_UNUSED2_IDX  3
/** not used - zero padding */
#define ICM_NETCTL_UNUSED3_IDX  4
/** not used - zero padding */
#define ICM_NETCTL_UNUSED4_IDX  5
/* Position of frame counter */
#define ICM_NETCTL_COUNTER_IDX  6
/* Position of checksum byte */
#define ICM_NETCTL_CSUM_IDX (ICM_NETCTL_LENGTH - 1u)
/** The VTBM response required mask of the byte denoted by NETCTL_RREQ_IDX */
#define ICM_NETCTL_VTBM_RR_MASK 0x80u
/** The message counter mask */
#define ICM_NETCTL_COUNTER_MASK ICM_CYCLE_CTR_MASK

/****************************************************************************
 * VTBM Status message 
 ****************************************************************************/
/** Index of Protocol Version in a message */
#define ICM_STATUS_PROT_VER_IDX                 0
/** Index of Internal error code in a message */
#define ICM_STATUS_ERRCODE_IDX                  1
/** Index of CAN error flags in a message */
#define ICM_STATUS_CAN_ERRFLAG_IDX              2
/** Index of CAN request validity and Balance status in a message */
#define ICM_STATUS_REQVAL_BAL_IDX               3
/** Unused Zero padding */
#define ICM_STATUS_UNUSED1                      4
/** Unused Zero padding */
#define ICM_STATUS_MODULE_FLAGS1_IDX            5
/** Index of Module status and Counter in a message */
#define ICM_STATUS_MODULE_FLAGS2_COUNTER_IDX    6
/** Index of Frame checksume in a message */
#define ICM_STATUS_CSUM_IDX         (VTBM_STATUS_SIZE - 1u)

/****************** Status Error Codes *******************************/

typedef enum IcmErrcode_e
{
/** VTBM no internal error detected */
    IcmErrcodeOk =              0u,
/** VTBM Unexpected internal error error code */
    IcmErrcodeUnexpected =      1u,
/** VTBM hardware mismatch Internal error code */
    IcmErrcodeHwMismatch =      2u,
/** EOL data invalid Internal error code */
    IcmErrcodeEolInvalid =      3u,
/** Calibration invalid Internal error code */
    IcmErrcodeCalInvalid =      4u,
/** Incomplete configuration message error code */
    IcmErrcodeConfIncomplete =  5u,
/** Invalid configuration message error code */
    IcmErrcodeConfInvalid =     6u,
/** Supply error detected error code */
    IcmErrcodeTempSupplyError = 7u,
/** A successfully decoded, but unsupported command was received*/
    IcmErrcodeCmdUnsupported =  8u,
/** An invalid cell number was specified */
    IcmErrcodeCellnumInvalid =  9u,
/** Invalid signal count specified */
    IcmErrcodeSigCountInvalid = 10u,
/** Signal out of range */
    IcmErrcodeValueOutOfRange = 11u,
/** Measurement in progress */
    IcmErrcodeMeasInProgress =  12u,
/** Cell select hardware error */
    IcmErrcodeCellSelHwError =  13u
} IcmErrcode_t;

/****************** Status Message CAN Error flags *******************************/

/** Rx Software buffer overflow */
#define ICM_STATUS_CAN_RXSWBUFF_OVERFLOW    0x01u
/** Rx Hardware buffer overflow */
#define ICM_STATUS_CAN_RXHWBUFF_OVERFLOW    0x02u
/** Tx Software buffer overflow */
#define ICM_STATUS_CAN_TXSWBUFF_OVERFLOW    0x04u
/** Rx Error */
#define ICM_STATUS_CAN_RX_ERR               0x08u
/** Rx Warning */
#define ICM_STATUS_CAN_RW_WARN              0x10u
/** Tx Error */
#define ICM_STATUS_CAN_TX_ERR               0x20u
/** Tx Warning */
#define ICM_STATUS_CAN_TX_WARN              0x40u
/** Bus Off detected */
#define ICM_STATUS_CAN_BUS_OFF              0x80u

/******************* CAN request validity  **************************************/

/** The position of the Request status field */
#define ICM_STATUS_REQ_RESP_SHIFT       6u
/** The mask to extract the value of the request response*/
#define ICM_STATUS_REQ_RESP_VMASK       0xC0u

/** Request valid */
#define ICM_STATUS_REQ_VALID            0u
/** Request invalid for current state */
#define ICM_STATUS_REQ_INVALID          1u
/** Request malformed */
#define ICM_STATUS_REQ_MALFORMED        2u

/***********************Balancing Status flags *********************************/

/** The mask field to extract the balancing cell number */
#define ICM_STATUS_BAL_CELL_MASK        0x0Fu
/** The shift to apply the position of the balancing status */
#define ICM_STATUS_BAL_STATUS_SHIFT     4u
/** The mask to exctract the balancing status */
#define ICM_STATUS_BAL_STATUS_MASK      0x30u

/***********************Module Status flags *********************************/

/** The mask of the bit to indicate module calibrated */
#define ICM_STATUS_MODULE_CALIBR_MASK   0x20u
/** The mask of the bit to indicate module configured*/
#define ICM_STATUS_MODULE_CONFIG_MASK   0x10u
/** The mask of the bit to indicate EOL error*/
#define ICM_STATUS_EOL_ERROR_MASK       0x40u
/** The mask of the bit to indicate emulated eeprom error */
#define ICM_STATUS_EEE_ERROR_MASK       0x80u
/** The mask of the message counter in a status message */
#define ICM_STATUS_COUNTER_MASK         0x0Fu

/** The mask of the bit to indicate flash single fault */
#define ICM_STATUS_SINGLE_FAULT_MASK   0x01u

/**
 * The number of bytes in a VTMB status message including checksum */
#define VTBM_STATUS_SIZE    8u
/**
 * Generate the VTBM status CAN ID for a given VTBM number */
#define MAKE_STATUS_ID(vtbm_num) (VTBM_STATUS_BASE + (uint16_t)(vtbm_num))

/****************************************************************************
 * Cell data message
 ****************************************************************************/

/**
 * The number of cells reported per returned data frame
 */
#define ICM_CELLDATA_PER_FRAME 2u
/** The CAN ID from which VTBM cell data frames are offset */
#define CELL_DATA_BASE_ID 0x200u

#ifdef VTBM_EOL_TEST
/** The CAN ID from which VTBM raw cell data frames are offset */
#define CELL_RAW_DATA_BASE_ID 0x300u
#endif

/** The number of bytes in a cell data message */
#define ICM_CELL_DATA_MSG_SIZE  8u
/** The shift to apply to extract the cell pair number (0-7) from a CAN ID */
#define ICM_CELL_PAIR_SHIFT 5

/**
 * Construct a CAN ID for the Cell data frame for a given VTBM and
 * cell pair
 * @param[in] vtbm_num the VTBM number 0-15
 * @param[in] pair_num the cell pair number, 0-7
 */
#define MAKE_CELLDATA_ID(vtbm_num,pair_num) \
    (CELL_DATA_BASE_ID \
    + (uint16_t)((uint16_t)(pair_num)<<ICM_CELL_PAIR_SHIFT) \
        + (vtbm_num) )

#ifdef VTBM_EOL_TEST
/**
 * Construct a CAN ID for the Raw Cell data frame for a given VTBM and
 * cell pair
 * @param[in] vtbm_num the VTBM number 0-15
 * @param[in] pair_num the cell pair number, 0-7
 */
#define MAKE_RAW_CELLDATA_ID(vtbm_num,pair_num) \
    (CELL_RAW_DATA_BASE_ID \
    + (uint16_t)((uint16_t)(pair_num)<<ICM_CELL_PAIR_SHIFT) \
        + (vtbm_num) )
#endif

/**
 * Extract a cell frame (pair) number from a CAN ID */
#define ICM_CANID2PAIR(canid) ( (uint8_t) ( (canid)>>ICM_CELL_PAIR_SHIFT) \
    & CELL_PAIR_MASK)

/**
 * The maximum pair index, this must be of the form (2^n)-1 for the masking
 * macros to work. If this 
 */
#define LAST_CELL_PAIR   7u
/**
 * Mask to use to extract the cell pair number from a CAN ID
 */
#define CELL_PAIR_MASK   LAST_CELL_PAIR
/**
 * The maximum number of possible cell pairs on a VTBM */
#define MAX_CELL_PAIR_COUNT (LAST_CELL_PAIR+1u)

#if CELL_PAIR_MASK !=7u
#error Check the setting of CELL_PAIR_MASK and LAST_CELL_PAIR
#endif

/**
 * The binary point of cell voltage as transmitted over the internal network*/
#define ICM_RAW_VCELL_BP    13
/* The extraction mask of a voltage value */
#define CELL_VOLT_MASK      0xFFFFu

/**
 * The binary point of cell temperature as transmitted over the internal network*/
#define ICM_RAW_CELLT_BP    3
 /** The number of bits in a temperature over the internal CAN */
#define CELL_TEMP_BITS 11u
/** The mask to apply to a shifted raw temperature value  to extract the 
 * relevant bits) */
#define CELL_TEMP_VMASK (((uint16_t)((uint16_t)1u<<CELL_TEMP_BITS))-1u)

/** The byte index of voltage0 in a cell data frame  */
#define CELL_DATA_V0_IDX    0u
/** The byte index of voltage1 in a cell data frame  */
#define CELL_DATA_V1_IDX    2u

/** The byte index of temperaure0 in a cell data frame  */
#define CELL_DATA_T0_IDX    4u
/** The byte index of temperaure1 in a cell data frame  */
#define CELL_DATA_T1_IDX    5u

/** The byte index of Cycle Counter in the cell data frame */
#define CELL_DATA_CYCLE_IDX 6u

/** The byte index of Gray-Checksum in the cell data frame */
#define CELL_DATA_CSUM_IDX  (ICM_CELL_DATA_MSG_SIZE - 1u)

/** The shift required for T0 in a cell data frame */
#define CELL_DATA_T0_RS 5u
/** The shift required for T1 in a cell data frame */
#define CELL_DATA_T1_RS 2u

/** The cycle counter mask used in the cell data message */
#define ICM_CELL_CYCLE_MASK 3u

/** The default cell voltage to use when resetting values */
#define ICM_DEFAULT_CELLV   (cell_v_t)0u

/** The default cell temperature to use when resetting values */
#define ICM_DEFAULT_CELLT   (temperature_t)0

/****************************************************************************
 * Reference data message
 ****************************************************************************/

/** The base CAN ID to offset ref data messages from */
#define REF_DATA_BASE_ID 0x560u

/**
 * Obtain the Reference data ID for a given VTBN number */
#define MAKE_REFDATA_ID(vtbm_num) \
    (REF_DATA_BASE_ID + (uint16_t)(vtbm_num) )

/**
 * The number of bytes in a reference response message, including checksum */
#define ICM_REF_FRAME_SIZE 8u

/** The index of Active balancing charger voltage in a ref data frame */
#define REF_DATA_AV_IDX     0u
/** The index of Passive balancing charger voltage in a ref data frame */
#define REF_DATA_PV_IDX     2u
/** The index of Start voltage reading in a ref data frame */
#define REF_DATA_SV_IDX     4u
/** The index of Cycle counter in a ref data frame */
#define REF_DATA_CYCLE_IDX  6u
/** The position of the byte containing the digital inputs */
#define REF_DATA_DIG_IDX    6u
/** The index of Gray-Checksum in a ref data frame */
#define REF_DATA_CSUM_IDX   (ICM_REF_FRAME_SIZE - 1u)

#define ICM_STATUS_STARTDIG_BIT 4
#define ICM_STATUS_PASSDIG_BIT 5
#define ICM_STATUS_ACTDIG_BIT 6

#define ICM_STATUS_STARTDIG_MASK (uint8_t)(1u<<ICM_STATUS_STARTDIG_BIT)
#define ICM_STATUS_PASSDIG_MASK  (uint8_t)(1u<<ICM_STATUS_PASSDIG_BIT)
#define ICM_STATUS_ACTDIG_MASK   (uint8_t)(1u<<ICM_STATUS_ACTDIG_BIT)

/** Mask for the Cycle counter in a ref data frame */
#define REF_DATA_CYCLE_MASK  ICM_CYCLE_CTR_MASK

/****************************************************************************
 * Pack configuration message
 ****************************************************************************/

/** The mask to extract the config body frame count */
#define CFG_COUNT_MASK      0x7Fu
/** The mask indicating that this frame is a header frame */
#define CFG_HEAD_MASK       0x80u

/** Return TRUE if the message passed in is a header message */
#define IS_CONFIG_HEAD(msgp) \
    (bool_t)(((msgp)->data[CFG_COUNT_IDX] & CFG_HEAD_MASK) != 0u)

/**
 * Extract the counter field from a config message
 * @param[in] datap the message data to inspect
 */
#define CFG_MSG_COUNT(datap) ( (datap)[CFG_COUNT_IDX] & CFG_COUNT_MASK)

#define ICM_IS_LAST_CFG_BODY(msgp)  (CFG_MSG_COUNT(msgp->data)==(NUM_CONFIG_FRAMES-1u))

/**
 * The number of data bytes in an ICM config message excluding the final
 * checksum. There are 7 bytes of data and 1 byte of checksum in each
 *  frame as standard
 **/
#define CONFIG_DATA_PER_MSG   7u

/**
 * The number of body frames in a configuration block */
#define NUM_BODY_FRAMES 6u

/**
 * The maximum number of frames in the config. data block
 * Having a larger size allows for the protocol to have additonal configuration
 * added without invalidating the memory areas the data is stored in */
#define NUM_CONFIG_FRAMES  7u

/**
 * The number of VTBMs the sysconfig is capable of representing.
 * This may be larger than the number of VTBMs the MMC can support
 * */
#define SYSCFG_NUM_VTBM 32u

/**
 * The mask applied to extract body frame number from a sysconfig frame
 */
#define SYSFG_COUNTER_MASK  0x7Fu
/**
 * The size of the area reserves for pack config
 */
#define MAX_CONFIG_SIZE (CONFIG_DATA_PER_MSG * NUM_CONFIG_FRAMES)

/** The position of the config frame counter */
#define CFG_COUNT_IDX       0
/** The byte position for the format byte in the config header */
#define CFG_FORMAT_IDX      1
/** The position of the response step byte */
#define CFG_RESP_STEP_IDX   2
/** The position of the response added delay byte */
#define CFG_RESP_ADD_IDX    3
/** The position of the temperature sensor type */
#define CFG_TEMP_SENS_IDX   4
/** Unused byte in config header */
#define CFG_UNUSED1_IDX     5
/** unused byte in config header */
#define CFG_UNUSED2_IDX     6

/** The number of data bytes (minus checksum) in a system config frame */
#define CFG_DATA_BYTES      7u

/** Number of bytes in a sysconfig frame, including checksum */
#define SYSCFG_FRAME_SIZE (CFG_DATA_BYTES+1u)

/**
 * The shift to apply to extract the temperature signal config from a VTBM
 * config byte */
#define CFG_TSIG_SHIFT    4

/**
 * Mask for V and T signal shifted values */
#define CFG_SIG_MASK     0xFu

/**
 * The position of the start of the 'body' frames describing each VTBM
 * in the sys config in the data stream extracted from the frames
 */
#define SYSCFG_BODY_POS 7u

/**
 * The number of VTBMs defined in each body frame */
#define SYSCFG_VTBMS_PER_CHUNK 6u

/**
 * The 'number' of the first body frame, i.e. it is 1 not 0
 */
#define ICM_CFG_FIRST_BODY  1u

/**
 * The seed for the CRC calculation of the config block */
#define ICM_CFG_CRC_SEED 0xFFFFu

/**
 * The total number of frames in a pack config, including one header frame
 * and all the body frames */
#define ICM_CFG_NUM_FRAMES (1u+NUM_BODY_FRAMES)

/**
 * The number of bytes required for the config CRC */
#define ICM_CFG_CRC_BYTES 2u

/**
 * The position of the most significant byte of the CRC in the config */
#define ICM_CFG_CRC_POS ((ICM_CFG_NUM_FRAMES * CFG_DATA_BYTES) \
    - ICM_CFG_CRC_BYTES)

#ifdef VTBM_EOL_TEST
/*****************************************************************************
 * Debug message used for EOL testing
 *****************************************************************************/
/** Debug message CAN ID */
#define ICM_DEBUG_MSG_ID    0x5FCu
/** Debug message length */
#define ICM_DEBUG_MSG_LEN   8u
/** Cell select hold time index in the message */
#define ICM_DEBUG_MSG_HOLD_TIME_IDX 0u
/** Position of flags in the message */
#define ICM_DEBUG_MSG_FLAGS_IDX     1u
/** Position of temperature supply settling time in the message */
#define ICM_DEBUG_MSG_TEMP_SUP_TIME_IDX 2u

/** Mask to get Switch temperature sensors suply flag from flags byte */
#define ICM_DEBUG_MSG_FLAGS_TEMP_SENS_SUPPLY_MSK    0x03u
/** Mask to get Disable WDG update flag from flags byte */
#define ICM_DEBUG_MSG_FLAGS_WDG_UPD_DISABLE         0x04u

#endif
/**
 * Temperature sensor supply control enum. For use in debug message.
 */
typedef enum IcmTempSensSupplyCtrl_e
{
    IcmTempSensSupplyNormal = 0, /**< Normal supply control */
    IcmTempSensSupplyNoEnable,   /**< Supply is never enabled */
    IcmTempSensSupplyNoDisable   /**< Supply is never disabled */
} IcmTempSensSupplyCtrl_t;

/*****************************************************************************
 * General-purpose macros

 ****************************************************************************/


#define GET_SEED_FROM_ID(msg_id) (uint8_t)(msg_id & 0xFFu)

/*****************************************************************************
 * Enums
 ****************************************************************************/
/**
 * This enum represents message error codes for received requests.
 */
enum MsgStat_e
{
    MsgValid,                 /**< Message valid */
    MsgDlcInvalid,            /**< Message DLC invalid */
    MsgCsumInvalid,           /**< Message Frame Checksum invalid */
    MsgCtrInvalid,            /**< Message Counter invalid */
    MsgInvalidState,          /**< Message invalid for current state */
    MsgMeasCtlActionInvalid,  /**< Message Measurement Control Action invalid */
    MsgMeasCtlTargetInvalid,  /**< Message Measurement Control Target invalid */
    MsgMeasCtlBalanceInvalid, /**< Message Measurement Control Balance flags invalid */
    MsgVtbmCtlActionInvalid,  /**< Message VTBM Control Action invalid */
    MsgVtbmCtlCellNumInvalid, /**< Message VTBM Control Cell number invalid */
    MsgSysCfgFramesNumInvalid,/**< Message System Configuration frames number invalid */
    MsgSysCfgFormatInvalid,   /**< Message System Configuration format invalid */
    MsgSysCfgCRC16Invalid,    /**< Message System Configuration CRC16 invalid */
    MsgSysCfgCellCntInvalid,  /**< Message System Configuration cell number invalid */
    MsgSysCfgTempCntInvalid,  /**< Message System Configuration temperature number invalid */
    MsgSysCfgTempSensInvalid, /**< Message System Configuration temperature sensor type invalid */
    MsgNetCtlMalformed        /**< Message Network Control malformed */
};

/**
 * This enum represents the decoded value of a VTBM's response to a request.
 * This enum is defined to match the protocol values. */
enum ReqStat_e
{
    /** The response is an acknowledgment of a valid request */
    ReqStatValid,
    /** The request was rejected due to the state of the receiver */
    ReqStatInvalidState,
    /** The request rejected because it was malformed */
    ReqStatMalformed
};

/**
 * The measure action values in a Measure Control command
 * This enum is defined to match the values in the protocol */
enum MeasActions_e
{
    /** Measure one or more cell voltages */
    MeasActionCellVolt  = 0,
    /** Measure Cell reference rails */
    MeasActionRefs      = 1,
    /** Apply balancing */
    MeasActionApplyBal  = 0xFFu
};

/**
 * This represents the target of a measure command
 * @warning This enum is NOT defined to match the values in the protocol,
 * because down the wire 'single' is represented as a cell number. */
enum MeasTarg_e
{
    MeasureTargSingle,  /* Measure 1 cell, specified by the cell number */
    MeasureTargNoCell,  /* Select no cells */
    MeasureTargCfgCell,
    MeasureTargAllCell
};

/**
 * The RX or TX status of the CAN transceiver.
 * This enum is defined to match the protocol values */
enum IcmCanStatus_e
{
    IcmCanStatusOK     =   0,
    IcmCanStatusWarn   =   1,
    IcmCanStatusErr    =   2,
    IcmCanStatusBusOff =   3
};

/**
 * Temperature sensor type
 * This enum is written so as to match the values specified in the protocol
 */
enum IcmTempSens_e
{
    /** Negative temperature coefficient */
    IcmTempSensNTC = 1,
    /** Platinum Resistance Thermometer */
    IcmTempSensPRT = 2
};

/*****************************************************************************
 * Data for the representation of comms frames
 ****************************************************************************/
#ifdef VTBM_EOL_TEST
/** Representation of VTBM debug message */
struct icm_debug_s
{
    uint8_t hold_time;
    uint8_t temp_sup_set_time;
    IcmTempSensSupplyCtrl_t  temp_sens_supply_ctrl;
    bool_t  wdg_update_disable;
};
#endif


/**
 * Representation of an unpacked config block header frame */
struct config_header_s
{
    uint8_t n_config_frames;
    uint8_t cfg_format;
    uint8_t resp_delay_step;
    uint8_t resp_delay_add;
    enum IcmTempSens_e temp_sens_type;
};

struct icm_syscfg_s
{
    struct config_header_s header;
    uint8_t vinp_ct[SYSCFG_NUM_VTBM];
    uint8_t tinp_ct[SYSCFG_NUM_VTBM];
};

/**
 * Representation of the data in a network control message */
struct icm_netctl_s
{
    /** The protocol version */
    uint8_t                 version;
    /**was a VTBM reply requested ? */
    bool_t                  vtbm_rreq;
    /** Frame counter */
    uint8_t counter;
};

/** Representation of the data in a measure control message */
struct icm_measctl_s
{
    /** The Action to perform
     * - Measure cells
     * - measure ref. voltage rails
     * - measure nothing - i.e. store no ADC reading*/
    enum MeasActions_e  measure_action;

    /**
     * If measuring cells, measure one of:
     *-  one of them
     * - all of the inputs
     * - only configured cells
     * - with no cells selected*/
    enum MeasTarg_e     measure_targ;

    /** If a measurement is performed, do it with the active balancing
     * activated */
    bool_t      meas_with_active;

    /**
     * If a measurement is performed, do it with the passive balancing
     * activated */
    bool_t      meas_with_passive;

    /**
     * The number of the cell to measure */
    uint8_t     meas_cell_num;

    /**
     * The measurement cycle counter */
    uint8_t     cycle_count;
};

/** Representation of the data in a measure control message */
struct icm_balctl_s
{
    /** VTBM number */
    uint8_t vtbm_num;
    /** Clear internal error flag after next transmit */
    bool_t clear_internal_error;
    /** Balancing to perform after the measurement */
    enum BalAction_e    bal_action;
    /** The number of the cell to balance as defined by bal_action after measurement */
    uint8_t     bal_cell_num;
    /** Clear reported CAN fault flags after next transmit */
    uint8_t     clear_can_flags;
    /**     counter for this frame */
    uint8_t     frame_count;
};

/**
 * The unpacked payload of a VTBM status response frame.
 */
struct vtbm_status_s
{
   /** The position of this VTBM, derived from the CANID */
   uint8_t          vtbm_num;
   /** The reported supported protocol version */
   uint8_t          version;
   /** reported internal error code */
   enum IcmErrcode_e internal_errcode;
   /** reported CAN error flags */
   uint8_t          can_error_flags;
   /** Status of request/response */
   enum ReqStat_e   request_status;
   /** Balancing status of VTBM */
   enum BalAction_e bal_status;
   /** Cell being balanced by VTBM */
   uint8_t          bal_cell;
   /** Module flags */
   struct
   {
        bool_t     module_configured;   /**< Module is configured */
        bool_t     module_calibrated;   /**< Module contain valid calibration record */
        bool_t     module_eol_ok;       /**< Module contain valid EOL record */
        bool_t     module_eee_ok;       /**< Emulated eeprom reports no error */
        bool_t     module_single_bit_fault_ok;  /**< Single bit flash fault didn't occur */
   } module_flags;
};

/**
 * Enumeration of raw cell voltage error values
 */
enum CellVstatVal_e
{
    CellVErrorVal       =0xFFFFu,
    CellVNoAttemptVal   =0xFFFEu,
    CellVDisconnectVal  =0xFFFDu,
    CellVShortCircVal   =0xFFFCu
};

/**
 * Special raw temperature values used to represent errors. These are specific
 * to the encoding of temperature and must be changed in the format of
 * temperature signals changes
 */
enum CellTstatVal_e
{
#if CELL_TEMP_BITS!=11u
#error Update these values to the appropriate raw error codes
#endif
    CellTErrorVal      =0x400,
    CellTNoAttemptVal  =0x401,
    CellTDisconnectVal =0x402,
    CellTShortCircVal  =0x403
};


/**
 * The unpacked representation of a cell data frame
 */
struct icm_cell_pair_data_s
{
    /** The number of the VTBM that this frame is from */
    uint8_t             vtbm_num;
    /** The cell pair number this data structure represents */
    uint8_t             pair_num;
    cell_v_t            voltage[ICM_CELLDATA_PER_FRAME];
    temperature_t       temperature[ICM_CELLDATA_PER_FRAME];
    enum CellVstat_e    volt_status[ICM_CELLDATA_PER_FRAME];
    enum CellTstat_e    temp_status[ICM_CELLDATA_PER_FRAME];
    uint8_t             cycle_count;
};

#ifdef VTBM_EOL_TEST
/**
 * The unpacked representation of a cell data frame
 */
struct icm_cell_pair_raw_data_s
{
    /** The number of the VTBM that this frame is from */
    uint8_t             vtbm_num;
    /** The cell pair number this data structure represents */
    uint8_t             pair_num;
    uint16_t            voltage[ICM_CELLDATA_PER_FRAME];
    uint16_t            temperature[ICM_CELLDATA_PER_FRAME];
    uint8_t             cycle_count;
};
#endif
/**
 * The state of a digital input */
typedef enum IcmDigState_e
{
    IcmDigInactive=0,
    IcmDigActive=1
} IcmDigState_t;

/**
 * The unpacked form of a reference measurement frame
 */
struct icm_ref_data_s
{
    /** The voltage measured with active balancing selected */
    cell_v_t            active_v;
    /** The voltage measured with passive balanding selected */
    cell_v_t            passive_v;
    /** The voltage measured at the start of the reference sequence */
    cell_v_t            start_v;
    /** The status of the active rail measurement */
    enum CellVstat_e    active_status;
    /** The status of the passive rail measurement */
    enum CellVstat_e    passive_status;
    /** The status of the starting measurement */
    enum CellVstat_e    startv_status;
    /** The VTBM number that this measurement came from */
    uint8_t             vtbm_num;
    /** The balancing diagnostic dig. input with charger selected */
    IcmDigState_t        active_diag;
    /** The balancing diagnostic dig. input with charger selected */
    IcmDigState_t        passive_diag;
    /**  The balancing diagnostic input at the start of the ref sequence */
    IcmDigState_t        start_diag;

    /** Measurement cycle counter */
    uint8_t             cycle_count;
};

#endif      /* ICM_FRAMES_TYPES_H */

