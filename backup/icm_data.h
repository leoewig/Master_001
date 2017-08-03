/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    icm_data.h
 *
 * description:  
 * Internal comms data processing header.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(ICM_DATA_H)
#define      ICM_DATA_H
/**
 *
 * Internal comms data processing header.
 *
 *
 * @file
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */


/**
 * Data manipulation functionality for ICM, the mapping between data in
 * messages and application state is performed in this component.
 *
 * <h2> ICM data processing Overview </h2>
 *
 * <h2> Approach used </h2>
 * This module contains the functionality related to the manipulation of
 * data signals within ICM - as opposed to the ICM state machine or the
 * low-level packing and unpacking of frames. The functions in this module
 * are called from other parts of ICM, when they determine that a certain
 * action must be performed.
 *
 * The caller must poll for the completion of a given command using the
 * appropriate completion function.
 *
 *  | Command                    | Completion function      |
 *  |----------------------------|--------------------------|
 *  | icm_trigger_netctl()       | icm_status_completion()  |
 *  | icm_trigger_config()       | icm_status_completion()  |
 *  | icm_trigger_balcmd()       | None                     |
 *  | icm_trigger_ref()          | icm_ref_completion()     |
 *  | icm_trigger_meas_cfg_cells()  | icm_measure_completion() |
 *  | icm_trigger_measure_none() | icm_measure_completion() |
 *
 * This module does not have access to the non-decoded messages, it operates
 * only the unpacked data structures of data that is about to be sent, or
 * which has been received and decoded from its network representation
 *
 * <h2> Triggering commands </h2>
 * The functions icm_trigger_netctl() icm_trigger_balcmd() icm_trigger_config()
 * icm_trigger_meas_cfg_cells() and icm_trigger_ref() can be used to trigger their
 * respective network operations. Each one will update internal data structures
 * in anticipation of the response, map between strategy variables and output
 * demands to be sent and call the lower-level frame transmission function
 * to perform the packing and queueing of the command.
 *
 *
 * <h2>Command completion </h2>
 * The icm_status_completion() icm_ref_completion() and
 * icm_measure_completion() functions are used to determine whether a given
 * command has completed. Each of these functions must be called until it
 * returns a value indicating that the command is deemed to have finished
 * either through having received all the CAN data it was expecting, or by
 * having timed out waiting for the data.
 *
 * These functions call the appropriate timeout manipulation functions
 * and inspect received data to determine whether the required response has
 * been received. When a command is deemed to have 'completed' (whether
 * successfully or not) the appropriate data processing and fault detection
 * functions are called.
 *
 * Once a command has completed, the data for it is inspected and any
 * appropriate faults (including timeouts) are udpated.
 * Examples:
 * - icm_process_completed_measure() operates on cell data.
 * - icm_validate_status_data() inspects status information.
 * - icm_validate_refs() inspects references.
 *
 * The caller should call a completion function as long as it returns a value
 * of #IcmRespPending.
 *
 * Calibration
 ====================
 * Timeout calibration
 * --------------------
 * The timeout values (in units of #ts_time_t) can be determined by the
 * following cal items:
 * - #icm_ref_measure_timeout
 * - #icm_netcmd_timeout
 * - #icm_cell_measure_timeout
 * - #icm_config_timeout
 * Note that these timeouts include the time for the far end to perform the
 * command, not just a CAN round-trip.
 *
 * VTBM configuration
 * --------------------
 * The VTBM configuration block is built up from the relevant Cal items.
 * - icm_vtbm_temp_sens_type Defines the VTBM temperature sensor type
 * - pl_num_vtbms determines the number of VTBMs present in the pack
 * - pl_num_cells_on_vtbm defines the number of cells on each VTBM
 * The items icm_resp_delay_step and icm_resp_delay_add are used as part of
 * the generation.
 *
 * Querying for status
 * =====================
 * To obtain information at a high-level, a collection of boolean variables
 * is provided that can be used as the basis for state machine decisions. The
 * xyz_once variables indicate that the corresponding operation has been
 * performed
 *
 * | Flag                         | Written by   |
 * |------------------------------|--------------|
 * | #icm_unconfig_vtbm_detected  | Status       |
 * | #icm_all_vtbm_cfg_ok         | Status       |
 * | #icm_all_vtbm_cal_ok         | Status       |
 * | #icm_all_versions_valid      | Status       |
 * | #icm_all_status_checked_once | Status       |
 * | #icm_all_refs_meas_once      | Ref measure  |
 * | #icm_all_cells_meas_once     | Cell measure |
 *
 * icm_clear_connection_state() can be used to reset the connection state flags
 * after the far end has been power-cycled etc.
 *
 * <h2> Requirement Satisfaction </h2>
 * See table in icm.h
 *
 * @defgroup icm_data ICM client data processing
 * @ingroup icm
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/**
 * Determine whether a given VTBM number is valid for performing operations
 * this will compare against #pl_num_vtbms
 * @param[in] num the VTBM number to inspect
 * @retval TRUE if num represents a VTBM which is configured to be present
 * @retval FALSE otherwise */
#define IS_VALID_VTBM_NUM(num) (bool_t)( (num)<pl_num_vtbms )

/**
 * The cell number used when the balancing action is set to disabled*/
#define DISABLED_BAL_CELL_NUM   0u

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern FRAM enum BalAction_e    icm_bal_action_dmd[MAX_NUM_VTBM];

extern FRAM uint8_t             icm_bal_cell_dmd[MAX_NUM_VTBM];

/* Startup check flags */
extern bool_t       icm_config_sent;
extern bool_t       icm_all_refs_meas_once;
extern bool_t       icm_all_cells_meas_once;

extern ts_time_t    icm_last_refs_trigger;

/*****************************************************************************
 * FUNCTION PROTOTYPES
 ****************************************************************************/

void icm_data_init(void);
void icm_clear_measure_once(void);

/************ Functions for determining status ******************************/

bool_t icm_cell_meas_valid(void);

bool_t icm_is_all_bal_dmd_off(void);

uint8_t icm_n_measure_cycle(void);

/************ Incoming message handlers         *****************************/


/************ Functions for triggering messages *****************************/

enum IcmTransmit_e icm_trigger_netctl(bool_t response_required);

enum IcmTransmit_e icm_trigger_balcmd(void);

enum IcmTransmit_e icm_trigger_config(void);

enum IcmTransmit_e icm_trigger_meas_cfg_cells(void);

enum IcmTransmit_e  icm_trigger_measure_none(void);

enum IcmTransmit_e icm_trigger_ref(void);

/************ Functions for command completion *****************************/

enum IcmRespState_e icm_ref_completion(bool_t connecting);

enum IcmRespState_e icm_measure_completion(bool_t connecting);

#endif      /* ICM_DATA_H */

