/*************************************************************************//**
 *
 * Public header for Charge Strategy Feature (cgs)
 *
 * Copyright (c) Ricardo plc 2011-2015 - All rights reserved
 *
 * @file
 * @ingroup cgs
 * $Id: cgs.h 873 2015-03-16 11:19:41Z j_bailey $
 *
 *****************************************************************************
 *
 * This header defines the public interface to the cgs module
 *
 *****************************************************************************/

/**
 * Charging Overview
 *
 * Several modules are used to implement the charging functionality:
 * -# Charger Interface (<a href="group__cgi.html">CGI</a>) handles all
 *    communications with the charger.
 * -# Charger Mode Determination (<a href="group__cmd.html">CMD</a>) decides if
 *    we should charge.
 * -# Charger Strategy (<a href="group__cgs.html">CGS</a>) does the charging and
 *    is split into two sub-modules:
 *    Co-Ordination (CGS_CCO) and Charging (CGS_CHG) which really
 *    does the charging.
 *
 * Charger Co-Ordination is a state machine which manages the charging process
 * at a high level. It's states are:
 *  -# Evaluating Start of Charge Cycle
 *  -# Charging not possible
 *  -# Charger shutdown - shutdown the charger
 *  -# Charger HV connection
 *  -# Voltage Equalisation
 *  -# Pack Contactor Closure
 *  -# Charging
 *  -# Charging termination
 *  -# Stop Charging- stop the charging process
 *  -# Ensure isolation
 *
 * Charging Strategy is a state machine that does the actual charging. It's
 * states are voltage ramp up, current ramp up, constant current, constant
 * voltage, and charging complete.
 *
 * @defgroup chargingoverview CGS Charging Overview
 */
/**
 * This module covers Charger Co-ordination and Charging Strategy.
 *
 * @defgroup cgs CGS Charger Strategy
 * @ingroup chargingoverview
 *
 *
 * # Charger Strategy Feature #
 *
 * The aim of this feature is to provide charger control in order to charge the
 * pack, including determination of whether charging is possible.
 *
 * ## Approach used #
 *
 * The feature is implemented in two state machines:
 * Charger Co-ordination (cgs_do_sm_cco()) and Charging Strategy (cgs_do_sm_chg()).
 * They are separate so that the charging strategy can be easily changed, and if
 * required, it will be easy to switch between different charging strategies at
 * run-time.
 *
 * ### Charger Co-ordination state machine #
 *
 * This decides when to open and close contactors, prepare for charging and
 * handle charging termination.

 * The function cgs_do_sm_cco() contains the functionality for this state machine.
 * It's state is stored in #cgs_sm_cco_state.
 *
 * ### Charging state machine #
 *
 * This handles the constant current, constant voltage phases of charging.
 *
 * The Charging state machine is a sub-state of the Charger Co-ordination state
 * machine. It is only executed when #cgs_sm_cco_state is #Cgs_sm_cco_charging.
 * Before the charging state machine is used it is initialised with a
 * call to cgs_sm_chg_init(). The charging state machine is run by calling
 * cgs_do_sm_chg(), which returns #Cgs_sm_chg_completed when charging has
 * completed.
 *
 * ## Assumptions #
 * This design assumes that the constant current / constant voltage charging
 * method will be used and provides no method of switching to another. However,
 * the code is structured so that it would be easy to do this if required.
 *
 * ## Limitations #
 *
 * -# LTS not implemented
 *
 * ## Integration #
 *
 * This module consists of S12 code called from a single task.
 *
 * To use this feature:
 * -#  Include cgs.h and call cgs_init() once from initialisation code.
 * -#  Call cgs_update() periodically from the main application task.
 * -#  The state of the charging function, or reason for not charging is
 *     available in #cgs_state.
 * -#  The result of the previous charge cycle is available in #cgs_last_result
 *     (this is stored in NVRAM).
 *
 *  ## Validity Flags #
 *
 * In the requirements document the first paragraph of section 7.1.9 says
 * that where the charger functionality relies on signals being valid these
 * should be checked for validity. The following tables are an aid to
 * ensuring that the design meets this requirement.
 *
 * External variables used by cgs_chg:
 *
 * | External variable                           | Validity flag                   | Where checked  |
 * | --------------------------------------------|---------------------------------|----------------|
 * | mdm_cell_v_max_value     | mdm_cell_v_valid | cgs_capacity() |
 * | mdm_number_of_cells_received | no validity flag                | n/a            |
 * | imi_integral_data.curr_latest_mA                | imi_integral_data.curr_latest_valid | cgs_common_checks() |
 *
 * External variables used by cgs_cco:
 *
 * | External variable                           | Validity flag                   | Where checked  |
 * | --------------------------------------------|---------------------------------|----------------|
 * | di_ign_dbn | no validity flag | - |
 * | cgi_is_charger_present | no validity flag | - |
 * | cgi_tether_type | no validity flag | - |
 * | cgi_is_charger_active | no validity flag | - |
 * | cgi_hv_isolation | cgi_hv_isolation_validity | cgs_do_sm_cco_hv() |
 * | imi_integral_data.curr_latest_mA | imi_integral_data.curr_latest_valid | cgs_terminate_chg(), cgs_commence_chg(), cgs_chg_i_dir_ok |
 * | VMI_CHG_BUS_V | VMI_CHG_BUS_V_VALID | cgs_do_ff_v_undesirable() |
 * | ctc_chg_pos_dmd | no validity flag | - |
 * | mdm_cell_v_sum | mdm_cell_v_valid | cgs_common_checks() |
 * | cmd_chg_mode_req | no validity flag | - |
 * | mdm_cell_t_max_value | mdm_cell_t_valid | cgs_common_checks() |
 * | mdm_cell_t_min_value | mdm_cell_t_valid | cgs_common_checks() |
 * | pd_pchrg_lim_short_w | no validity flag | - |
 * | vmi_hvil_status | no validity flag  | - |
 * | ctc_cont_chg_psn_expect | no validity flag  | - |
 * | cgi_charger_voltage_limit | cgi_charger_voltage_limit_valid | cgs_get_charger_voltage_limit() |
 *
 *
 */

#if !defined(CGS_H)
#define      CGS_H

/******************************************************************************
 * MACROS
 *****************************************************************************/
#define CGS_IS_CHARGING ((cgs_state == Cgs_s_chg_ok) || \
                         (cgs_state == Cgs_s_chg_v_ramp_up) || \
                         (cgs_state == Cgs_s_chg_i_ramp_up) || \
                         (cgs_state == Cgs_s_chg_i_const) || \
                         (cgs_state == Cgs_s_chg_v_const))


/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "nvs_cfg.h"
#include "mmc.h"

#include "cgs_types.h"
#include "ctc_types.h"

/******************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
extern enum cgs_state_e cgs_state; /**< State of the charging function or reason
                                     for not charging */
extern enum cgs_state_e cgs_last_result; /**< Result of the previous charge cycle */
extern enum Ctc_cont_state_e cgs_chg_cont_dest; /**< Charger contactor demand
                                                               (input to CTC) */
extern bool_t           cgs_chg_start_timedout; /**< Flag to indicate the start
                                                 of charge cycle evaluation has
                                                 timed out */

extern dummy_e         cgs_charge_cycle_state_latest; /**< @mantis{4245} As specified in Data Dictionary */
extern dummy_e         cgs_charge_cycle_state_previous; /**< @mantis{4245} As specified in Data Dictionary */
extern bool_t          cgs_charger_allowed; /**< @mantis{4245} As specified in Data Dictionary */
extern dummy_t         cgs_charger_current_demand; /**< @mantis{4245} As specified in Data Dictionary */
extern pack_v_t        cgs_charger_voltage_demand; /**< @mantis{4245} As specified in Data Dictionary */
extern bool_t          cgs_zero_current_flowing_on_charger_bus; /**< @mantis{4245} As specified in Data Dictionary */
extern cgs_sm_cco_e    cgs_sm_cco_state; /**< The charger co-ordination state machine */
extern enum cgs_term_reason_e cgs_term_reason; /**< Charging termination reason */
extern int16_t         cgs_chg_time_remaining_min; /**< Remaining charging time in minutes */

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
/**************************************************************************//**
 *
 *  Initialisation of cgs module
 *
 *  This function is called by the main application task. It does the following:

 *  -# Sets the Charger Co-ordination state machine (#cgs_sm_cco_state)
 *     to #Cgs_sm_cco_decision 
 *  -# Sets #cgs_cool_down_active to false
 *  -# Sets #cgs_charger_fault to false.
 *  -# Sets #cgs_state to #Cgs_s_nc_no_dmd.
 *  -# Sets #cgs_chg_i_dmd and #cgs_chg_v_dmd to zero.
 *
 *  @mantis{4290} #cgs_last_result needs initialising somehow. It's in nvram so not sure
 *  how this works yet.
 *
 *****************************************************************************/
extern void cgs_init( void );

/**************************************************************************//**
 *
 *  Resets the Charger Co-ordination state machine.
 *
 *  This function is called during initialisation (by cgs) and after the charger
 *  state completes (by cmd) to reset the state machine state and prepare it for
 *  another cycle.

 *  -# Sets the Charger Co-ordination state machine (#cgs_sm_cco_state)
 *     to #Cgs_sm_cco_start.
 *
 *****************************************************************************/
extern void cgs_sm_reset(void);

/**************************************************************************//**
 *
 *  Runs the Charger Co-ordination state machine.
 *
 *  This function is called by the main application task. It does the following:
 *  -# Detects an ignition on event (using cgs_ev_ign_on() or a charger just connected
 *     event (using cgs_ev_chg_connect()), and then sets #cgs_cool_down_active
 *     to false
 *  -# Detects an ignition on event (using cgs_ev_ign_on()), and then sets
 *     #cgs_chg_start_timedout to false
 *  -# Sets #cgs_time_now to the current time.
 *  -# Call cgs_do_ff() to update the fault filters.
 *  -# Runs the Charger Co-ordination state machine (cgs_do_sm_cco()).
 *  -# Sets voltage and current requests from charger using cgs_set_i() and
 *        cgs_set_v().
 *
 *****************************************************************************/
extern void cgs_update( void );

/**************************************************************************//**
 *
 *  Returns whether there is capacity for charging.
 *
 *   @retval        TRUE  there is capacity
 *   @retval        FALSE there is no capacity
 *
 *  If cell voltage measurements are available, and all cell voltages are not
 *  hihgher than the cell voltage limit for which constant current charging is
 *  allowed (#cgs_cc_up_cell_v_lim) then the function will return TRUE, else
 *  FALSE
 *
 *  Note that this function is public as it seems that the Charging Demand
 *  features requires this
 *
 *****************************************************************************/
extern bool_t cgs_capacity ( void );

extern bool_t cgs_cell_temp_not_ok_for_charging ( void );

#endif      /* CGS_H */

