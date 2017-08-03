/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   icm_types.h
 *
 * description: 
 * Type definitions for internal comms. The dataypes in this file are those
 * that tend to belong to the outside-world interface of the ICM feature
 * rather than the detail of data mangling for transfer down the wire
 * 
 * 
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
#if !defined(ICM_TYPES_H)
#define      ICM_TYPES_H
/**
 *
 * Type definitions for internal comms. The dataypes in this file are those
 * that tend to belong to the outside-world interface of the ICM feature
 * rather than the detail of data mangling for transfer down the wire
 *
 * @file
 * @ingroup icm
 *
 */

/* Copyright (c) Linkedriving Tech 2012,2013 - All rights reserved */


/*****************************************************************************
 * MACROS
 ****************************************************************************/

/** The maximum number of cells that a VTBM can respond with */
#define MAX_VTBM_CELLS 16u

/**
 * The seed value to place in icm_meas_trig_counter, used to subdivide the
 * background cycle time into a number of foreground task invocations */
#define ICM_BG_TRIG_SEED (MMC_BG_MS/MMC_FG_MS)

/** Default cal value for VTBM start time
 * @mantis{5090} Default CAL change - validation or reqs update required?
 */
#define ICM_DEFAULT_VTBM_STARTUP_DELAY 350

/**
 * The bitmask in xg_refs_diag indicating the passive balancing diagnostic
 * input was active at the start of the refs sequence
 * Bits are set within xg_refs_diag  */
#define ICM_REF_DIG_START_SET       1u
/**
 * The bitmask in xg_refs_diag indicating the passive balancing diagnostic
 * input was active at whilst checking the passive balancing circuit
 * Bits are set within xg_refs_diag  */
#define ICM_REF_DIG_PASSIVE_SET     2u
/**
 * The bitmask in xg_refs_diag indicating the passive balancing diagnostic
 * input was active whilst checking the active balancing circuit.
 * Bits are set within xg_refs_diag  */
#define ICM_REF_DIG_ACTIVE_SET      4u

/**
 * The desired value of xg_refs_diag for each VTBM */
#define ICM_REF_DIG_REQUIRED (ICM_REF_DIG_START_SET | ICM_REF_DIG_PASSIVE_SET)

/**
 * Return TRUE if ICM is ready for normal BMS operation and has completed
 * all necessary start-up operations */
#define ICM_IS_READY() (bool_t)(IcmState_active == icm_state)
/**
 * Return TRUE if ICM has halted regular VTBM communications so that VTBM
 * reprogramming can occur */
#define ICM_IN_REPROG() (bool_t)(IcmState_reprog == icm_state)


/*****************************************************************************
 * ENUMS
 ****************************************************************************/

/**
 * Data type for diagnostic message, physical vs functional */
enum DiagMsgType_e
{
    DiagPhys,
    DiagFunc
};

/**
 * An operation signal sent from the ICM feature to the XG */
enum IcmXgMeasState_e
{
    IcmMeasIdle,
    IcmMeasAcquire,
    IcmMeasError
};

/**
 * The internal state of the ICM module */
enum IcmState_e
{
    /** Module is uninitialised */
    IcmState_uninit,
    /** Comms is disabled, VTBMs are off */
    IcmState_off,
    /** Waiting after enabling for VTBM startup */
    IcmState_waiting,
    /** Connecting message handshake */
    IcmState_connecting,
    /** Up and running, measurements are possible etc. */
    IcmState_active,
    /** Graceful shutdown */
    IcmState_shutdown,
    /** Reprogramming mode */
    IcmState_reprog
};

/**
 * Connecting sub-state for ICM */
enum IcmConnecting_e
{
    /** Starting point */
    IcmConnectIdle,
    /** Initial version check */
    IcmConnectVersion,
    /** Waiting for all VTBMs to be configured */
    IcmConnectConfig,
    /** Waiting for VTBM references */
    IcmConnectRefs,
    /** Waiting for a first measurement of cells */
    IcmConnectCellMeas,
    /** The state entered once connection is completed */
    IcmConnectComplete
};

/**
 * The substate of the communications between the MMC and the VTBMs
 * The MMC cycles around these various states during normal operation
 * once the VTBM is up and running. */
enum IcmActive_e
{
    /** Not expecting a reply but otherwise happy. VTBMs might be balancing */
    IcmActiveIdle,
    /** Expect a measurement, either VTBMs are measuring or sending data.
     * The XGATE owns the data buffer in this state.
     * Setting this state invalidates the xgate's measured cell data buffer.*/
    IcmActiveMeasure,
    /** Expect refs measurement, either VTBMs are measuring or sending data.
     * Setting this states invalidates the XGATE's reference measure buffer.*/
    IcmActiveRefs,
    /** Expect status */
    IcmActiveStatus
};

enum IcmShutdown_e
{
    /** Tell all VTBMs not to perform Balancing */
    IcmShutdownStopBal,
    /** Perform a 'none' measurement to cause it to take effect without
     * selecting any cells */
    IcmShutdownNoMeas,
    /** Confirm no VTBM is balancing */
    IcmShutdownConfirm,
    /** Wait for time to elapse until shutdown */
    IcmShutdownWait
};

/**
 * The status of a decoded temperature signal
 * @note changes to this enum must be mirrored in the respective ASAP2 COMPU_VTAB
 * in icm.a2l
 */
enum CellTstat_e
{
    /** Temperature reading was successful */
    CellTok,
    /** Temperature reading failed for unspecified reason */
    CellTError,
    /** Temperature reading was not attempted */
    CellTNoAttempt,
    /** Open circuit detected */
    CellTDisconnect,
    /** Short circuit detected */
    CellTShortCirc
};

/**
 * The status of a decoded voltage signal
 * Note that the values of this enum do not match the network representation
 * @note changes to this enum must be mirrored in the respective ASAP2 COMPU_VTAB
 * in icm.a2l
 */
enum CellVstat_e
{
    /**
     *  Voltage reading was successful */
    CellVok=0,
    /**
     * Voltage reading failed for unspecified reason. */
    CellVError,
    /** Voltage reading was not attempted */
    CellVNoAttempt,
    /** Open-circuit detected */
    CellVDisconnect,
    /** Short circuit detected */
    CellVShortCirc
};

/**
 * Return value of functions that might transmit/queue one or more outgoing
 * frames
 */
enum IcmTransmit_e
{
    /** Did not transmit anything */
    IcmTransmitNone,
    /** Transmit/Queued OK */
    IcmTransmitOK,
    /** Failed to transmit/Queue the message */
    IcmTransmitErr
};

/**
 * The validity of a signal received from the VTBM, Valid and Invalid are
 * definite assertions of the state of the signal on the most recent reading
 * Unknown means that the reading failed. */
enum IcmRxValidity_e
{
    /** Signal received as valid at last update */
    IcmRxValid,
    /** Signal received invalid at last update */
    IcmRxInvalid,
    /** Signal not received */
    IcmRxUnknown
};

/**
 * The status of the communication channel between the MMC and VTBMs
 */
enum IcmRespState_e
{
    /** CAN bus is free for a command */
    IcmRespIdle,
    /** MMC is waiting for a response */
    IcmRespPending,
    /** Insufficient response seen for most recent command, but timeout limit
     * has as expired*/
    IcmRespTimeout
};


/**
 * Information about bad decoded messages on the S12 */
enum IcmUnpackResult_e
{
    IcmUnpackOk=0,
    IcmUnpackBadDLC,
    IcmUnpackBadChecksum,
    IcmUnpackBadResp,
    IcmUnpackBadBalStatus,
    IcmUnpackBadVTBM
};

/**
 * Enumeration of available module configured statuses
 */
enum IcmCfgState_e
{
    IcmCfgConfigured,      /**< vtbm module is configured */
    IcmCfgUnconfigured,    /**< vtbm module is unconfigured */
    IcmCfgUnknown          /**< vtbm configuration status is yet unknown */
};

/**
 * Enumeration of available flash single-bit fault presence statuses
 */
enum IcmSingleBitFaultState_e
{
    IcmSingleBitFaultAbsent,   /**< Single-bit fault has occured */
    IcmSingleBitFaultPresent,  /**< Single-bit fault hasn't occured */
    IcmSingleBitFaultUnknown   /**< Status of single-bit fault is yet unknown */
};

/**
 * Enumeration of available emulated eeprom fault presence statuses
 */
enum IcmEeeFaultState_e
{
    IcmEeeFaultAbsent,     /**< Emulated eeprom fault hasn't occurred */
    IcmEeeFaultPresent,    /**< Emulater eeprom fault has occurred */
    IcmEeeFaultUnknown     /**< Status of emulated eeprom fault is yet unknown */
};


/******************************************************************************
 * Structures
 *****************************************************************************/

/**
 * A data structure used to query the balancing status of a VTBM */
struct icm_bal_status_s
{
    data_validity_e     validity;
    enum BalAction_e    balance_action;
    uint8_t             balance_cell;
};

#endif      /* ICM_TYPES_H */

