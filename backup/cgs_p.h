/*************************************************************************//**
 *
 * Private header for cgs
 *
 * Copyright (c) Ricardo plc 2011-2015 - All rights reserved
 *
 * @file
 * $Id: cgs_p.h 873 2015-03-16 11:19:41Z j_bailey $
 *
 *****************************************************************************
 *
 * This header makes internal functions and data of cgs available to a
 * unit test harness.
 */

/**
 * @addtogroup cgs
 * @{
 * @file
 * @}
 */
#if !defined(CGS_P_H)
#define      CGS_P_H

#include "fm_cust.h"
#include "cgs_types.h"

/*
 * enumerations and macro definitioms made available must be documented here to
 * avoid duplication.  Variables and functions are documented in the source
 * file.
 */

 /****************************************************************************
 * Macros
 *****************************************************************************/

/**************************************************************************//**
* Options for the charging state machine #cgs_sm_chg_state
*****************************************************************************/
typedef enum {
  Cgs_sm_chg_uninit,    /**< Un-initialised */
  Cgs_sm_chg_v_ramp_up, /**< Voltage ramp up */
  Cgs_sm_chg_i_ramp_up, /**< Current ramp up */
  Cgs_sm_chg_i_const,   /**< Constant current */
  Cgs_sm_chg_v_const,   /**< Constant voltage */
  Cgs_sm_chg_completed  /**< Charging complete */
} cgs_sm_chg_state_e;

/**************************************************************************//**
* Options for cgs_chg_v_ok() and cgs_chg_i_ok()
*****************************************************************************/
typedef enum {
  Cgs_ok,   /**< ok */
  Cgs_high, /**< too high */
  Cgs_low   /**< too low */
} cgs_ok_e;

/******************************************************************************
 * FUNCTION DEFINITIONS
 *****************************************************************************/
extern cgs_sm_chg_state_e cgs_do_sm_chg ( void );
extern void cgs_sm_chg_init ( void );

/******************************************************************************
 * REALLY PRIVATE FUNCTION DEFINITIONS (needed for test)
 *****************************************************************************/
#if defined UNIT_TEST && !defined _lint
extern bool_t cgs_ev_chg_connect(void);
extern bool_t cgs_ev_ign_on(void);
extern cgs_ok_e cgs_chg_i_ok(void);
extern enum cgs_state_e cgs_common_checks(void);
extern enum cgs_state_e cgs_commence_chg(void);
extern void cgs_do_sm_cco_decision(void);
extern void cgs_do_sm_cco_v_eq_entry(void);
extern void cgs_do_sm_cco_v_eq(void);
extern enum cgs_state_e cgs_continue_chg(void);
extern void cgs_do_sm_cco_ct_closure(void);
extern void cgs_do_sm_cco_charging_entry(void);
extern void cgs_do_sm_cco_charging(void);
extern void cgs_do_sm_cco_chg_term_entry(enum cgs_state_e abort_reason);
extern void cgs_do_sm_cco_chg_term(void);
extern void cgs_do_v_eq(void);
extern nw_sf_e cgs_terminate_chg(void);
extern enum cgs_state_e cgs_common_checks(void);
extern enum cgs_state_e cgs_commence_chg(void);
extern enum cgs_state_e cgs_continue_chg(void);
extern bool_t cgs_chg_i_dir_ok(void);
extern void cgs_set_i(void);
extern void cgs_set_v(void);
extern enum cgs_state_e cgs_translate_state(cgs_sm_chg_state_e chg_state);
extern void cgs_do_sm_cco_decision_entry(void);
extern void cgs_do_sm_cco_not_poss(void);
extern void cgs_do_sm_cco_chg_shutdown_entry(void);
extern void cgs_do_sm_cco_chg_shutdown(void);
extern void cgs_do_sm_cco_hv_entry(void);
extern void cgs_do_sm_cco_hv(void);
extern void cgs_do_sm_cco_ct_closure_entry(void);
extern void cgs_do_sm_cco_stop_entry(void);
extern void cgs_do_sm_cco_stop(void);
extern void cgs_do_sm_cco_isolation_entry(void);
extern void cgs_do_sm_cco_isolation(void);
extern void cgs_do_ff_chg_shutdown(void);
extern void cgs_do_ff_unexpected(void);
extern void cgs_do_ff_v_undesirable(void);
extern void cgs_do_ff_i_high(void);
extern void cgs_do_ff_i_low(void);
extern void cgs_do_ff_i_direction(void);
extern bool_t cgs_cool_down_timer_expired (void);
extern bool_t cgs_lower_faults(void);
extern pack_v_t cgs_get_charger_voltage_limit(void);
extern void cgs_do_sm_cco(void);
extern void cgs_do_ff(void);

extern void cgs_sm_chg_init (void);
extern bool_t cgs_capacity (void);
extern void cgs_do_sm_chg_v_ramp_up_entry(void);
extern void cgs_do_sm_chg_v_ramp_up(void);
extern void cgs_do_sm_chg_i_ramp_up_entry(void);
extern void cgs_do_sm_chg_i_ramp_up(void);
extern void cgs_do_sm_chg_i_const(void);
extern void cgs_do_sm_chg_v_const_entry(void);
extern void cgs_do_sm_chg_v_const(void);
extern void cgs_do_sm_chg_completed(void);


#endif /* UNIT_TEST && !defined _lint */

/******************************************************************************
 * VARIABLES
 *****************************************************************************/
extern current_t cgs_chg_i_dmd; /**< Charging current demand */
extern pack_v_t cgs_chg_v_dmd; /**< Charging voltage demand */
extern ts_time_t cgs_time_now;  /**< Local copy of current time */

/******************************************************************************
 * PRIVATE CGS_CCO DATA
 *****************************************************************************/
#if defined UNIT_TEST && !defined _lint
extern ts_time_t cgs_cool_down_ts;       /**< Time stamp for cool down timer */
extern bool_t cgs_cool_down_active;  /**< Flag to indicate that cool down
                                            timer is active */
extern ts_time_t cgs_cco_time_entered_state_ts; /**< Time stamp for measuring
                                              time in current state */
extern bool_t cgs_charger_fault;     /**< Charger fault reported by CGI */
extern bool_t cgs_prev_ign;          /**< Previous state of #di_ign_dbn */
extern bool_t cgs_prev_chg_present;  /**< Previous value
                                                   of #cgi_is_charger_present */
#endif /* UNIT_TEST && !defined _lint */

/******************************************************************************
 * PRIVATE CGS_CGS DATA
 *****************************************************************************/
#if defined UNIT_TEST && !defined _lint
extern cgs_sm_chg_state_e cgs_sm_chg_state;
extern pack_v_t cgs_ramp_start_v;
extern current_t cgs_ramp_start_i;
extern ts_time_t cgs_ramp_start_ms;
extern ts_time_t cgs_cv_update_start;
extern current_t cgs_ir_target_i;
#endif /* UNIT_TEST && !defined _lint */

/******************************************************************************
 * CALIBRATIONS
 *****************************************************************************/
 /*
 lint -esym(759,cgs_com_v_zero_tol)
 lint -esym(759,cgs_term_i_zero_tol)
 lint -esym(759,cgs_com_power_min)
 lint -esym(759,cgs_cool_down_time)
 lint -esym(759,cgs_com_cell_t_low)
 lint -esym(759,cgs_com_cell_t_high)
 lint -esym(759,cgs_max_charge_time)
 lint -esym(759,cgs_term_to)
 lint -esym(759,cgs_isolation_to)
 lint -esym(759,cgs_ve_dyn_v_offset)
 lint -esym(759,cgs_decision_to)
 lint -esym(759,cgs_ve_to)
 lint -esym(759,cgs_chg_shutdown_to)
 lint -esym(759,cgs_chg_hv_to)
 lint -esym(759,cgs_ct_closure_to)
 lint -esym(759,cgs_ve_v_tol)
 lint -esym(759,cgs_v_req_tol)
 lint -esym(759,cgs_ve_dynamic)
 lint -esym(759,cgs_ve_v_static)
 lint -esym(759,cgs_i_dir_req_tol)
 lint -esym(759,cgs_perform_v_eq)

 lint -esym(759,cgs_cc_up_cell_v_lim)
 lint -esym(759,cgs_vr_i_init)
 lint -esym(759,cgs_vr_ramp_duration_ms)
 lint -esym(759,cgs_cc_i_ramp_duration_ms)
 lint -esym(759,cgs_cv_v_set)
 lint -esym(759,cgs_cv_i_low_lim)
 lint -esym(759,cgs_cv_update_period_ms)
 lint -esym(759,cgs_cv_up_cell_v_lim)
 lint -esym(759,cgs_cv_v_reduce)
 lint -esym(759,cgs_cv_red_factor)
 lint -esym(759,cgs_i_req_tol)
*/

/*lint -esym(759,cgs_com_v_zero_tol) */
/*lint -esym(759,cgs_term_i_zero_tol) */
/*lint -esym(759,cgs_com_power_min) */
/*lint -esym(759,cgs_cool_down_time) */
/*lint -esym(759,cgs_com_cell_t_low) */
/*lint -esym(759,cgs_com_cell_t_high) */
/*lint -esym(759,cgs_max_charge_time) */
/*lint -esym(759,cgs_term_to) */
/*lint -esym(759,cgs_isolation_to) */
/*lint -esym(759,cgs_ve_dyn_v_offset) */
/*lint -esym(759,cgs_decision_to) */
/*lint -esym(759,cgs_ve_to) */
/*lint -esym(759,cgs_chg_shutdown_to) */
/*lint -esym(759,cgs_chg_hv_to) */
/*lint -esym(759,cgs_ct_closure_to) */
/*lint -esym(759,cgs_ve_v_tol) */
/*lint -esym(759,cgs_v_req_tol) */
/*lint -esym(759,cgs_ve_dynamic) */
/*lint -esym(759,cgs_ve_v_static) */
/*lint -esym(759,cgs_i_dir_req_tol) */
/*lint -esym(759,cgs_perform_v_eq) */

/*lint -esym(759,cgs_cc_up_cell_v_lim) */
/*lint -esym(759,cgs_vr_i_init) */
/*lint -esym(759,cgs_vr_ramp_duration_ms) */
/*lint -esym(759,cgs_cc_i_ramp_duration_ms) */
/*lint -esym(759,cgs_cv_v_set) */
/*lint -esym(759,cgs_cv_i_low_lim) */
/*lint -esym(759,cgs_cv_update_period_ms) */
/*lint -esym(759,cgs_cv_up_cell_v_lim) */
/*lint -esym(759,cgs_cv_v_reduce) */
/*lint -esym(759,cgs_cv_red_factor) */
/*lint -esym(759,cgs_i_req_tol) */

extern CAL pack_v_t cgs_com_v_zero_tol;
extern CAL current_t cgs_term_i_zero_tol;
extern CAL power_t cgs_com_power_min;
extern CAL ts_time_t cgs_cool_down_time;
extern CAL temperature_t cgs_com_cell_t_low;
extern CAL temperature_t cgs_com_cell_t_high;
extern CAL ts_time_t cgs_max_charge_time;
extern CAL ts_time_t cgs_term_to;
extern CAL ts_time_t cgs_isolation_to;
extern CAL pack_v_t cgs_ve_dyn_v_offset;
extern CAL ts_time_t cgs_decision_to;
extern CAL supply_v_t cgs_min_supply_voltage;
extern CAL ts_time_t cgs_ve_to;
extern CAL ts_time_t cgs_chg_shutdown_to;
extern CAL ts_time_t cgs_chg_hv_to;
extern CAL ts_time_t cgs_ct_closure_to;
extern CAL pack_v_t cgs_ve_v_tol;
extern CAL pack_v_t cgs_v_req_tol;
extern CAL bool_t cgs_ve_dynamic;
extern CAL pack_v_t cgs_ve_v_static;
extern CAL current_t cgs_i_dir_req_tol;
extern CAL bool_t cgs_perform_v_eq;
extern CAL cell_v_t cgs_cc_up_cell_v_lim;
extern CAL current_t cgs_vr_i_init;
extern CAL ts_time_t cgs_vr_ramp_duration_ms;
extern CAL ts_time_t cgs_cc_i_ramp_duration_ms;
extern CAL pack_v_t cgs_cv_v_set;
extern CAL current_t cgs_cv_i_low_lim;
extern CAL ts_time_t cgs_cv_update_period_ms;
extern CAL cell_v_t cgs_cv_up_cell_v_lim;
extern CAL cell_v_t cgs_cv_v_reduce;
extern CAL percent_t cgs_cv_red_factor;
extern CAL current_t cgs_i_req_tol;
extern CAL cell_v_t cgs_cv_up_cell_v_lim_hyst;
extern CAL temperature_t cgs_time_map_x_cell_t[];
extern CAL percent_t cgs_time_map_y_soc[];
extern CAL int16_t cgs_time_map_z_minutes[];


#endif      /* CGS_P_H */
