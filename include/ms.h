/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   ms.h
 *
 * description: Main state module header file
 *    
 *      
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(MS_H)
#define      MS_H
/**
 *
 * Main state module header file
 *
 * @file
 * @ingroup ms
 * $Id: ms.h 12113 2014-06-02 11:01:20Z pr4 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 * <h2> Main State Machine Overview </h2>
 * The main state control feature is responsible for the core state machine
 * controlling the strategy. It is responsible for:
 * - Controlling the overall strategy mode and the modes of all features.
 * - Controlling the possible contactor positions in all states
 * - Power management - active/inactive/low power.
 *
 * <h2> Approach used </h2>
 * The various system inputs are processed into a vehicle mode request and a
 * charger mode request in their respective features. The main state control
 * arbitrates between these requests and transitions to the relevant mode
 * unless a condition prevents it (e.g. a fault).
 *
 * The initialisation complete input indicates that all startup tasks (e.g.
 * VTBM discovery) have completed. The system state disallows the closing of
 * any contactors until this has been performed.
 *
 * The vehicle contactor destination signal is an indication to the
 * CTC feature of the desired vehicle contactor position: open or closed.
 *
 * <h2> Configuration </h2>
 * Each feature that has a requirement to perform shutdown actions before the
 * MMC can actually enter shutdown state should declate it's interest in mmc_ms.def.
 *
 * This creates a boolean flag for each feature in mmc_ms.def that can be used by the
 * other features to prevent MS entering the shutdown state before they are ready.
 *
 * A pending entry to shutdown can be detected by other features using the
 * #ms_shutdown_pending flag. These features then maintain their own shutdown inhibit flags
 * and when they have completed all necessary actions will clear this flag. When
 * all of the shutdown inhibit flags have been cleared then the MS feature shall enter
 * the shutdown state.
 *
 * <h2> How to use </h2>
 * The MS feature is S12 code and is called from a single task.
 *
 * ms_init() should be called once to initialise the MS feature and then
 * ms_update() should be called in each scheduler tick to maintain the
 * system state machine.
 *
 * It is the responsibilty of other features to ensure that they are shutdown correctly
 * when MS indicates that it is about to enter shutdown mode (by setting the flag
 * #ms_shutdown_pending). Features that need to perform shutdown actions should set their
 * flag in #ms_feature_shutdown_state when they see the #ms_shutdown_pending flag set and once
 * they have performed all of the necessary actions then they should lower their flag in the
 * array #ms_feature_shutdown_state.
 *
 * When all of these flags are clear then MS will enter shutdown mode. External features
 * maintain these shutdown inhibit flags by calling ms_set_inhibit_flag() or ms_clear_inhibit_flag() as appropriate.
 *
 * <h2> External Access To Data </h2>
 * Features that require data provided by the main system state feature
 * can access the following directly
 * - #ms_sys_state
 * - #ms_veh_cont_dest
 * - #ms_balancing_enabled
 * - #ms_shutdown_pending
 *
 * The #ms_feature_shutdown_state can be accessed using the routines
 * - ms_set_inhibit_flag()
 * - ms_clear_inhibit_flag()
 *
 * <h2> Requirement Satisfaction </h2>
 *
 * The requirements for this feature can be found in the
 * <a href="http://viewvc/viewvc.cgi/trunk/docs/reqs/generic/RU_BMS_MainState.doc?root=Q52717_NightWing&view=co">
 * Main State Machine And Application Startup Requirements Section 3 MMC MAIN STATE MACHINE</a> document.
 *
 * | Requirement | Code | Tested By |
 * | ---------------------------|-------------------------|---------------------------------------------|
 * | REQ_BMS_FFF_CBI_001 | BAL feature requirement? | |
 * | REQ_BMS_FFF_CBI_002 | BAL feature requirement? | |
 * | REQ_BMS_MS_CHG_001 | ms_update() | pat_chg__from_standby() |
 * | REQ_BMS_MS_CHG_002 | ms_set_outputs() | pat_chg__from_standby() |
 * | REQ_BMS_MS_CHG_003 | ms_set_outputs() | pat_chg__from_standby() |
 * | REQ_BMS_MS_CLOSE_001 | ms_update() | pat_close__from_standby() |
 * | REQ_BMS_MS_CLOSE_002 | ms_set_outputs() | pat_close__from_standby() |
 * | REQ_BMS_MS_CLOSE_004 | ms_set_outputs() | pat_close__from_standby() |
 * | REQ_BMS_MS_CLOSE_005 | ms_set_outputs() | pat_close__from_standby() |
 * | REQ_BMS_MS_CLOSE_006 | ms_set_outputs() | pat_close__from_standby() |
 * | REQ_BMS_MS_CLOSE_007 | ms_set_outputs() | pat_close__from_standby() |
 * | REQ_BMS_MS_CLOSE_008 | ms_set_outputs() | pat_close__from_standby() |
 * | REQ_BMS_MS_CLOSE_009 | ms_set_outputs() | pat_close__from_standby() |
 * | REQ_BMS_MS_ERR_001 | ms_set_outputs() | pat_err__from_initialisation(), pat_err__from_standby(), pat_err__from_veh_closing(), pat_err__from_veh_active(), pat_err__from_veh_opening(), pat_err__from_charger() |
 * | REQ_BMS_MS_ERR_003 | ms_update() | pat_err__from_initialisation(), pat_err__from_standby(), pat_err__from_veh_closing(), pat_err__from_veh_active(), pat_err__from_veh_opening(), pat_err__from_charger() |
 * | REQ_BMS_MS_ERR_004 | ms_set_outputs() | pat_err__from_initialisation(), pat_err__from_standby(), pat_err__from_veh_closing(), pat_err__from_veh_active(), pat_err__from_veh_opening(), pat_err__from_charger() |
 * | REQ_BMS_MS_INIT_001 | ms_set_outputs() | |
 * | REQ_BMS_MS_INIT_002 | ms_set_outputs() | |
 * | REQ_BMS_MS_OPEN_001 | ms_update() | pat_open__from_veh_closing() |
 * | REQ_BMS_MS_OPEN_002 | ms_update() | pat_open__from_veh_active(), pat_open__from_veh_active() |
 * | REQ_BMS_MS_OPEN_003 | ms_set_outputs() | pat_open__from_veh_closing(), pat_open__from_veh_active() |
 * | REQ_BMS_MS_OPEN_005 | ms_set_outputs() | pat_open__from_veh_closing(), pat_open__from_veh_active() |
 * | REQ_BMS_MS_OUTPUT_001 | ms_set_outputs() | All MS PATs |
 * | REQ_BMS_MS_SHUTDOWN_001 | ms_update() | pat_shutdown__from_standby() |
 * | REQ_BMS_MS_SHUTDOWN_002 | ms_update() | pat_shutdown__from_error() |
 * | REQ_BMS_MS_SHUTDOWN_004 | ms_set_outputs() | pat_shutdown__from_standby(), pat_shutdown__from_error() |
 * | REQ_BMS_MS_SHUTDOWN_005 | ms_set_outputs() | pat_shutdown__from_standby(), pat_shutdown__from_error() |
 * | REQ_BMS_MS_SHUTDOWN_006 | ms_update() | |
 * | REQ_BMS_MS_SHUTDOWN_007 | #ms_feature_shutdown_state, ms_set_inhibit_flag(), ms_clear_inhibit_flag() | |
 * | REQ_BMS_MS_SHUTDOWN_008 | #ms_shutdown_pending | |
 * | REQ_BMS_MS_SHUTDOWN_009 | #ms_feature_shutdown_state, ms_set_inhibit_flag(), ms_clear_inhibit_flag() | |
 * | REQ_BMS_MS_STANDBY_001 | ms_update() | pat_standby__from_initialisation() |
 * | REQ_BMS_MS_STANDBY_002 | ms_update() | pat_standby__from_veh_opening() |
 * | REQ_BMS_MS_STANDBY_003 | ms_update() | pat_standby__from_charger() |
 * | REQ_BMS_MS_STANDBY_005 | ms_set_outputs() | pat_standby__from_initialisation(), pat_standby__from_veh_opening(), pat_standby__from_charger() |
 * | REQ_BMS_MS_STANDBY_008 | ms_update() | pat_standby__from_initialisation() |
 * | REQ_BMS_MS_STANDBY_009 | ms_update() | pat_standby__from_initialisation() |
 * | REQ_BMS_MS_STANDBY_010 | ms_update() | pat_standby__from_initialisation() |
 * | REQ_BMS_MS_STANDBY_011 | ms_update() | pat_standby__from_initialisation() |
 * | REQ_BMS_MS_STANDBY_012 | ms_update() | pat_standby__from_initialisation() |
 * | REQ_BMS_MS_STANDBY_013 | ms_update() | pat_standby__from_initialisation() |
 * | REQ_BMS_MS_STANDBY_014 | ms_set_outputs() | pat_standby__from_initialisation(), pat_standby__from_veh_opening(), pat_standby__from_charger() |
 * | REQ_BMS_MS_VEH_001 | ms_update() | pat_veh__from_veh_closing() |
 * | REQ_BMS_MS_VEH_002 | ms_set_outputs() | pat_veh__from_veh_closing() |
 * | REQ_BMS_MS_VEH_004 | ms_set_outputs() | pat_veh__from_veh_closing() |
 * | REQ_BMS_SUP_NV_012 | ms_update() | |
 *
 * @defgroup ms MS Main State
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "mmc.h"  /* mmc.h is a prerequisite */
//#include "cmd.h"
//#include "ctc.h"  /* for Ctc_cont_state_e  */
#include "flash_eee_types.h" /* flash_eee_reset_type */
#include "ms_types.h"

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern enum ms_state_e       ms_sys_state;
extern enum Ms_wakeup_mode   ms_wakeup_type;
//extern enum Ctc_cont_state_e ms_veh_cont_dest;
extern bool_t                ms_balancing_enabled;
extern bool_t                ms_shutdown_pending;
extern bool_t                ms_ecu_reset_pending;
extern bool_t                ms_fgloop_shutdown_complete;
extern ts_time_t             ms_left_initialisation;
extern enum Ms_wakeup_mode ms_wakeup_type;
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 * Initialisation function for module to be called at startup.
 */
void ms_init(void);

/**
 * This function determines if a state transition is required and sets the outputs
 * accordingly.
 *
 * Initially all possible state transitions are checked.
 *
 * <b> Standby State </b>
 *
 * - Entered from Initialisation if
 *  - EEPROM data restoration is complete
 *  - all operations related to the initialisation of current
 *    reading device have been completed
 *  - communications with all VTBMs have been established and
 *    network-level configuration and initialisation has completed
 *  - initial communication with the RTC has been established
 *  - no error conditions exist
 *
 * - Entered from Vehicle Opening if
 *  - the contactor opening sequence has completed
 *  - no error conditions exist
 *
 * - Entered from Charger if
 *  - charger mode request is not asserted
 *  - the charger state is charging not possible
 *  - no error conditions exist
 *
 * - Entered from Error if
 *  - at least one feature is not ready to shutdown
 *  - error conditions has been removed
 *
 * <b> Vehicle Closing State </b>
 *
 * - Entered from Standby if
 *  - vehicle mode request is asserted
 *  - charger mode request is not asserted
 *  - expected position of all contactors is open
 *  - commanded position of all contactors is open
 *  - no error conditions exist
 *
 * <b> Vehicle Active State </b>
 *
 * - Entered from Vehicle Closing if
 *  - vehicle contactor closing sequence has completed successfully
 *  - no error conditions exist
 *
 * <b> Vehicle Opening State </b>
 *
 * - Entered from Vehicle Closing if
 *  - vehicle mode request is not asserted
 *  - no error conditions exist
 *
 * - Entered from Vehicle Active if
 *  - vehicle mode request is not asserted
 *  - no error conditions exist
 *
 * <b> Charger State </b>
 *
 * - Entered from Standby if
 *  - vehicle mode request is not asserted
 *  - charger mode request is asserted
 *  - no error conditions exist
 *
 * <b> Shutdown State </b>
 *
 * - Entered from Standby if
 *  - all features indicate that shutdown is not inhibited
 *  - no error conditions exist
 *
 * - Entered from Error if
 *  - all features indicate that shutdown is not inhibited
 *  - no error conditions exist
 *
 * <b> Error State </b>
 *
 * - Entered from all other states (except shutdown) if
 *  - the error mode condition is asserted
 *
 * <b> Low Power State </b>
 *
 * - Entered from Shutdown if
 *  - no EEPROM write is either pending or in progress
 *  - the internal CAN shutdown has completed
 *
 * Finally ms_set_outputs() is called to set the outputs to the required state
 */
void ms_update( void );

/* Documentation in source */
void                     ms_update_shutdown_state     (enum ms_inhibits_e feature, enum ms_shutdown_state_e new_state);
enum ms_shutdown_state_e ms_get_feature_shutdown_state(enum ms_inhibits_e feature);
enum ResetReqResult_e ms_request_reset(flash_eee_reset_type_e reset_type);

#endif      /* MS_H */
