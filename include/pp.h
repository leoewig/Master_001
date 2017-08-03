/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    pp.h
 *
 * description:  Pack Protection header
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial version
 *                       
 *
 *****************************************************************************/
#if !defined(PP_H)
#define      PP_H
/**
 *
 * Pack Protection header
 *
 * Public header for the PP feature
 *
 * @file
 * @ingroup PackProtection
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 *
 * <h2> Pack Protection Overview </h2>
 * Pack protection monitors a variety of system parameters. These parameters are
 * compared against either calibrations or calculated variables to detect fault
 * conditions.
 * Each fault is only raised if its monitoring conditions are met. These conditions
 * are mainly signal valididity but can also include other signals such as contactor
 * state.
 * The functions are split into two groups one group executing in the foreground
 * and one group executing in the background.
 *
 *
 * <h2> Approach used </h2>
 * The functions are split into two groups one group executing in the foreground
 * and one group executing in the background.

 * <h2> Limitations </h2>
 * The background tasks in this feature cannot directly access any data present in
 * foreground tasks. MDM provides this module with a coherent data set.
 *
 * <h2> How to use </h2>
 * The foreground routines are called via pp_update_fg()
 * The background functions are called via pp_update_bg()
 *
 * <h2> How to use </h2>
 * pp_init() Initialization function, called once at start up before other PP
 * functions can be called.
 *
 * pp_update_fg() must be installed in the foreground after vmi_update(),
 * imi_update() and tmi_update() to perform monitoring of high-frequency data.
 *
 * pp_update_bg() must be installed in the background after cell data
 * calculations to monitor both cell data measurements and the results of the
 * cell modeling.
 *
 *
 * <h2> Configuration </h2>
 * Each fault must be calibrated for relevant detection values and signal
 * tolerances. (In addition the faults must be calibrated to request the
 * relevant failure functions when detection occurs, however this is outside
 * the scope of pack protection which only raises faults).
 *
 * <h2> Requirements </h2>
 * The requirements document for the feature can be found in the
 * <a href="http://viewvc/viewvc.cgi/trunk/docs/reqs/generic/RU_BMS_Fault_Diagnosis.doc?root=Q52717_NightWing&view=co">
 * Pack Protection Section 4.3</a> of the RU_BMS_Fault_Diagnosis.doc document.
 *
 * Requirements are satisfied as follows:
 *
 * | Requirement | Code | Tested By |
 * | --------------------------- | ------------------------- | --------------------------------------------- |
 * | REQ_BMS_GFI_007 | | |
 * | REQ_BMS_GFI_008 | | |
 * | REQ_BMS_GFI_009 | | |
 * | REQ_BMS_GFI_010 | | |
 * | REQ_BMS_PP_CT_003 | #pp_cell_core_min_t | |
 * | REQ_BMS_PP_CT_004 | #pp_cell_core_max_t | |
 * | REQ_BMS_PP_CT_005 | pp_background_monitor_modelled_cell_temps() | pat_pp_cell_core_min_t() |
 * | REQ_BMS_PP_CT_006 | pp_background_monitor_modelled_cell_temps() | pat_pp_cell_core_max_t() |
 * | REQ_BMS_PP_CT_007 | pp_background_monitor_modelled_cell_temps() | pat_pp_cell_core_min_t() |
 * | REQ_BMS_PP_CT_008 | pp_background_monitor_modelled_cell_temps() | pat_pp_cell_core_min_t() |
 * | REQ_BMS_PP_CT_009 | pp_background_monitor_modelled_cell_temps() | pat_pp_cell_core_max_t() |
 * | REQ_BMS_PP_CT_010 | pp_background_monitor_modelled_cell_temps() | pat_pp_cell_core_max_t() |
 * | REQ_BMS_PP_CT_011 | #pp_cell_core_max_delta_t | |
 * | REQ_BMS_PP_CT_012 | pp_background_monitor_modelled_cell_temps() | pat_pp_cell_max_del_mod_t() |
 * | REQ_BMS_PP_CT_013 | pp_background_monitor_measured_cell_temps() | pat_pp_cell_max_del_mod_t() |
 * | REQ_BMS_PP_CT_014 | pp_background_monitor_measured_cell_temps() | pat_pp_cell_max_del_mod_t() |
 * | REQ_BMS_PP_CT_015 | #pp_cell_skin_min_t | |
 * | REQ_BMS_PP_CT_016 | #pp_cell_skin_max_t | |
 * | REQ_BMS_PP_CT_017 | pp_background_monitor_measured_cell_temps() | pat_pp_cell_skin_min_t() |
 * | REQ_BMS_PP_CT_018 | pp_background_monitor_measured_cell_temps() | pat_pp_cell_skin_max_t() |
 * | REQ_BMS_PP_CT_019 | pp_background_monitor_measured_cell_temps() | pat_pp_cell_skin_min_t() |
 * | REQ_BMS_PP_CT_020 | pp_background_monitor_measured_cell_temps() | pat_pp_cell_skin_min_t() |
 * | REQ_BMS_PP_CT_021 | pp_background_monitor_measured_cell_temps() | pat_pp_cell_skin_max_t() |
 * | REQ_BMS_PP_CT_022 | pp_background_monitor_measured_cell_temps() | pat_pp_cell_skin_max_t() |
 * | REQ_BMS_PP_CT_023 | #pp_cell_skin_max_delta_t | |
 * | REQ_BMS_PP_CT_024 | implemented in mdm | |
 * | REQ_BMS_PP_CT_025 | pp_background_monitor_measured_cell_temps() | pat_pp_cell_delta_t() |
 * | REQ_BMS_PP_CT_026 | pp_background_monitor_measured_cell_temps() | pat_pp_cell_delta_t() |
 * | REQ_BMS_PP_CT_027 | pp_background_monitor_measured_cell_temps() | pat_pp_cell_delta_t() |
 * | REQ_BMS_PP_CT_028 | #pp_cell_skin_tolerance_t | |
 * | REQ_BMS_PP_CT_029 | #pp_cell_core_tolerance_t | |
 * | REQ_BMS_PP_CV_004 | #pp_max_oper_cellsum_v | |
 * | REQ_BMS_PP_CV_005 | #pp_min_oper_cellsum_v | |
 * | REQ_BMS_PP_CV_006 | pp_background_monitor_cell_voltage_limits() | pat_pp_pack_max_sum_v() |
 * | REQ_BMS_PP_CV_007 | pp_background_monitor_cell_voltage_limits() | pat_pp_pack_min_sum_v() |
 * | REQ_BMS_PP_CV_008 | #pp_max_crit_cellsum_v | |
 * | REQ_BMS_PP_CV_009 | #pp_min_crit_cellsum_v | |
 * | REQ_BMS_PP_CV_010 | pp_background_monitor_cell_voltage_limits() | pat_pp_pack_max_crit_sum_v() |
 * | REQ_BMS_PP_CV_011 | pp_background_monitor_cell_voltage_limits() | pat_pp_pack_min_crit_sum_v() |
 * | REQ_BMS_PP_CV_012 | pp_background_monitor_cell_voltage_limits() | pat_pp_pack_max_sum_v() |
 * | REQ_BMS_PP_CV_013 | pp_background_monitor_cell_voltage_limits() | pat_pp_pack_max_sum_v() |
 * | REQ_BMS_PP_CV_014 | pp_background_monitor_cell_voltage_limits() | pat_pp_pack_max_crit_sum_v() |
 * | REQ_BMS_PP_CV_015 | pp_background_monitor_cell_voltage_limits() | pat_pp_pack_max_crit_sum_v() |
 * | REQ_BMS_PP_CV_016 | pp_background_monitor_cell_voltage_limits() | pat_pp_pack_min_sum_v() |
 * | REQ_BMS_PP_CV_017 | pp_background_monitor_cell_voltage_limits() | pat_pp_pack_min_sum_v() |
 * | REQ_BMS_PP_CV_018 | pp_background_monitor_cell_voltage_limits() | pat_pp_pack_min_crit_sum_v() |
 * | REQ_BMS_PP_CV_019 | pp_background_monitor_cell_voltage_limits() | pat_pp_pack_min_crit_sum_v() |
 * | REQ_BMS_PP_CV_020 | #pp_min_oper_cell_v | |
 * | REQ_BMS_PP_CV_021 | #pp_min_crit_cell_v | |
 * | REQ_BMS_PP_CV_022 | #pp_max_oper_cell_v | |
 * | REQ_BMS_PP_CV_023 | #pp_max_crit_cell_v | |
 * | REQ_BMS_PP_CV_024 | #pp_cell_v_max_delta | |
 * | REQ_BMS_PP_CV_025 | Implemented by mdm | |
 * | REQ_BMS_PP_CV_026 | pp_background_monitor_cell_voltage_limits() | pat_pp_cell_delta_v() |
 * | REQ_BMS_PP_CV_027 | pp_background_monitor_cell_voltage_limits() | pat_pp_cell_delta_v() |
 * | REQ_BMS_PP_CV_028 | pp_background_monitor_cell_voltage_limits() | pat_pp_cell_delta_v() |
 * | REQ_BMS_PP_CV_029 | pp_background_monitor_cell_voltage_limits() | pat_pp_cell_min_v() |
 * | REQ_BMS_PP_CV_030 | pp_background_monitor_cell_voltage_limits() | pat_pp_cell_min_crit_v() |
 * | REQ_BMS_PP_CV_031 | pp_background_monitor_cell_voltage_limits() | pat_pp_cell_max_v() |
 * | REQ_BMS_PP_CV_032 | pp_background_monitor_cell_voltage_limits() | pat_pp_cell_max_crit_v() |
 * | REQ_BMS_PP_CV_033 | pp_background_monitor_cell_voltage_limits() | pat_pp_cell_min_v() |
 * | REQ_BMS_PP_CV_034 | pp_background_monitor_cell_voltage_limits() | pat_pp_cell_min_v() |
 * | REQ_BMS_PP_CV_035 | pp_background_monitor_cell_voltage_limits() | pat_pp_cell_min_crit_v() |
 * | REQ_BMS_PP_CV_036 | pp_background_monitor_cell_voltage_limits() | pat_pp_cell_min_crit_v() |
 * | REQ_BMS_PP_CV_037 | pp_background_monitor_cell_voltage_limits() | pat_pp_cell_max_v() |
 * | REQ_BMS_PP_CV_038 | pp_background_monitor_cell_voltage_limits() | pat_pp_cell_max_v() |
 * | REQ_BMS_PP_CV_039 | pp_background_monitor_cell_voltage_limits() | pat_pp_cell_max_crit_v() |
 * | REQ_BMS_PP_CV_040 | pp_background_monitor_cell_voltage_limits() | pat_pp_cell_max_crit_v() |
 * | REQ_BMS_PP_CV_041 | #pp_cell_v_tolerance | |
 * | REQ_BMS_PP_CV_042 | #pp_min_for_vtbm_cell_v | |
 * | REQ_BMS_PP_CV_043 | #pp_vtbm_op_below_v_threshold_ms | |
 * | REQ_BMS_PP_CV_044 | pp_background_monitor_cell_voltage_limits() | |
 * | REQ_BMS_PP_CV_045 | pp_background_monitor_cell_voltage_limits() | |
 * | REQ_BMS_PP_CV_046 | pp_background_monitor_cell_voltage_limits() | |
 * | REQ_BMS_PP_I_002 | #pp_max_discharge_i_ma | |
 * | REQ_BMS_PP_I_003 | #pp_max_charge_i_ma | |
 * | REQ_BMS_PP_I_004 | pp_mon_curnt_veh_ctc_closed() | pat_pp_max_discharge_i() |
 * | REQ_BMS_PP_I_005 | pp_mon_curnt_veh_ctc_closed() | pat_pp_max_charge_i() |
 * | REQ_BMS_PP_I_006 | pp_mon_curnt_chg_ctc_closed() | |
 * | REQ_BMS_PP_I_007 | pp_mon_curnt_chg_ctc_closed() | |
 * | REQ_BMS_PP_I_008 | pp_mon_curnt_veh_ctc_closed() | pat_pp_max_discharge_i() |
 * | REQ_BMS_PP_I_009 | pp_mon_curnt_veh_ctc_closed() | pat_pp_max_discharge_i() |
 * | REQ_BMS_PP_I_010 | pp_mon_curnt_veh_ctc_closed() | pat_pp_max_charge_i() |
 * | REQ_BMS_PP_I_011 | pp_mon_curnt_veh_ctc_closed() | pat_pp_max_charge_i() |
 * | REQ_BMS_PP_I_012 | pp_mon_curnt_chg_ctc_closed() | |
 * | REQ_BMS_PP_I_013 | pp_mon_curnt_chg_ctc_closed() | |
 * | REQ_BMS_PP_I_014 | pp_mon_curnt_chg_ctc_closed() | |
 * | REQ_BMS_PP_I_015 | pp_mon_curnt_chg_ctc_closed() | |
 * | REQ_BMS_PP_I_016 | pp_mon_curnt_at_zero_indication() | Fm_pack_unexpected_i() |
 * | REQ_BMS_PP_I_017 | pp_mon_curnt_at_zero_indication() | Fm_pack_unexpected_i() |
 * | REQ_BMS_PP_I_018 | pp_mon_curnt_at_zero_indication() | Fm_pack_unexpected_i() |
 * | REQ_BMS_PP_PL_002 | pp_mon_pack_i_discharge_lim() | pat_pp_min_crit_charge_i() |
 * | REQ_BMS_PP_PL_003 | pp_mon_pack_i_discharge_lim() | pat_pp_max_avail_discharge_i() |
 * | REQ_BMS_PP_PL_004 | #mdm_bg_output_p->cm.curr_chg_avail_short | |
 * | REQ_BMS_PP_PL_005 | pp_mon_pack_i_charge_lim_veh_ctc_closed() | pat_pp_max_veh_power() |
 * | REQ_BMS_PP_PL_006 | pp_mon_pack_i_charge_lim_veh_ctc_closed() | pat_pp_max_crit_charge_i() |
 * | REQ_BMS_PP_PL_008 | pp_mon_pack_i_charge_lim_chg_ctc_closed() | |
 * | REQ_BMS_PP_PL_010 | pp_mon_pack_i_charge_lim_chg_ctc_closed() | |
 * | REQ_BMS_PP_PL_011 | pp_mon_pack_i_charge_lim_veh_ctc_closed() | pat_pp_max_crit_charge_i() |
 * | REQ_BMS_PP_PL_012 | pp_mon_pack_i_charge_lim_veh_ctc_closed() | pat_pp_max_crit_charge_i() |
 * | REQ_BMS_PP_PL_013 | pp_mon_pack_i_charge_lim_veh_ctc_closed() | pat_pp_max_veh_power() |
 * | REQ_BMS_PP_PL_014 | pp_mon_pack_i_charge_lim_veh_ctc_closed() | pat_pp_max_veh_power() |
 * | REQ_BMS_PP_PL_015 | pp_mon_pack_i_discharge_lim() | pat_pp_min_crit_charge_i() |
 * | REQ_BMS_PP_PL_016 | pp_mon_pack_i_discharge_lim() | pat_pp_min_crit_charge_i() |
 * | REQ_BMS_PP_PL_017 | pp_mon_pack_i_discharge_lim() | pat_pp_max_avail_discharge_i() |
 * | REQ_BMS_PP_PL_018 | pp_mon_pack_i_discharge_lim() | pat_pp_max_avail_discharge_i() |
 * | REQ_BMS_PP_PL_019 | pp_mon_pack_i_charge_lim_chg_ctc_closed() | |
 * | REQ_BMS_PP_PL_020 | pp_mon_pack_i_charge_lim_chg_ctc_closed() | |
 * | REQ_BMS_PP_PL_021 | pp_mon_pack_i_charge_lim_chg_ctc_closed() | |
 * | REQ_BMS_PP_PL_022 | pp_mon_pack_i_charge_lim_chg_ctc_closed() | |
 * | REQ_BMS_PP_PL_025 | #mdm_bg_output_p->cm.cm_usage_limits_valid | |
 * | REQ_BMS_PP_PL_026 | #mdm_bg_output_p->cm.cm_usage_limits_valid | |
 * | REQ_BMS_PP_PT_003 | pp_background_monitor_pack_temperatures() | pat_pp_pack_max_t_inlet(), pat_pp_pack_max_t_outlet() |
 * | REQ_BMS_PP_PT_004 | pp_background_monitor_pack_temperatures() | pat_pp_pack_min_t_inlet(), pat_pp_pack_min_t_outlet() |
 * | REQ_BMS_PP_PT_005 | pp_background_monitor_pack_temperatures() | pat_pp_pack_max_t_inlet(), pat_pp_pack_max_t_outlet() |
 * | REQ_BMS_PP_PT_006 | pp_background_monitor_pack_temperatures() | pat_pp_pack_max_t_inlet(), pat_pp_pack_max_t_outlet() |
 * | REQ_BMS_PP_PT_007 | pp_background_monitor_pack_temperatures() | pat_pp_pack_min_t_inlet(), pat_pp_pack_min_t_outlet() |
 * | REQ_BMS_PP_PT_008 | pp_background_monitor_pack_temperatures() | pat_pp_pack_min_t_inlet(), pat_pp_pack_min_t_outlet() |
 * | REQ_BMS_PP_SOH_002 | #pp_min_soh | |
 * | REQ_BMS_PP_SOH_003 | pp_background_monitor_state_of_health() | |
 * | REQ_BMS_PP_SOH_004 | pp_background_monitor_state_of_health() | |
 * | REQ_BMS_PP_SOH_005 | pp_background_monitor_state_of_health() | |
 * | REQ_BMS_PP_V_003 | #pp_max_crit_hv_v | |
 * | REQ_BMS_PP_V_004 | #pp_max_oper_hv_v | |
 * | REQ_BMS_PP_V_005 | pp_mon_veh_hv_volt_upper_limits() | pat_pp_pack_max_crit_veh_v() |
 * | REQ_BMS_PP_V_006 | pp_mon_veh_hv_volt_upper_limits() | pat_pp_pack_max_veh_v() |
 * | REQ_BMS_PP_V_007 | #pp_min_crit_hv_v | |
 * | REQ_BMS_PP_V_008 | #pp_min_oper_hv_v | |
 * | REQ_BMS_PP_V_009 | pp_mon_veh_hv_volt_lower_limits() | pat_pp_pack_min_crit_veh_v() |
 * | REQ_BMS_PP_V_010 | pp_mon_veh_hv_volt_lower_limits() | pat_pp_pack_min_veh_v() |
 * | REQ_BMS_PP_V_012 | pp_mon_veh_hv_volt_lower_limits() | pat_pp_pack_veh_v_vs_sum() |
 * | REQ_BMS_PP_V_013 | pp_mon_veh_hv_volt_above_prev_value() | pat_pack_veh_open_high_v() |
 * | REQ_BMS_PP_V_014 | #pp_veh_hv_dischg_time_limit_ms | |
 * | REQ_BMS_PP_V_016 | Looks like duplicate with ctc | |
 * | REQ_BMS_PP_V_017 | pp_mon_veh_hv_volt_upper_limits() | pat_pp_pack_max_crit_veh_v() |
 * | REQ_BMS_PP_V_018 | pp_mon_veh_hv_volt_upper_limits() | pat_pp_pack_max_crit_veh_v() |
 * | REQ_BMS_PP_V_019 | pp_mon_veh_hv_volt_upper_limits() | pat_pp_pack_max_veh_v() |
 * | REQ_BMS_PP_V_020 | pp_mon_veh_hv_volt_upper_limits() | pat_pp_pack_max_veh_v() |
 * | REQ_BMS_PP_V_021 | pp_mon_veh_hv_volt_lower_limits() | pat_pp_pack_min_crit_veh_v() |
 * | REQ_BMS_PP_V_022 | pp_mon_veh_hv_volt_lower_limits() | pat_pp_pack_min_crit_veh_v() |
 * | REQ_BMS_PP_V_023 | pp_mon_veh_hv_volt_lower_limits() | pat_pp_pack_min_veh_v() |
 * | REQ_BMS_PP_V_024 | pp_mon_veh_hv_volt_lower_limits() | pat_pp_pack_min_veh_v() |
 * | REQ_BMS_PP_V_025 | pp_mon_veh_hv_volt_above_prev_value() | pat_pack_veh_open_high_v() |
 * | REQ_BMS_PP_V_026 | pp_mon_veh_hv_volt_above_prev_value() | pat_pack_veh_open_high_v() |
 * | REQ_BMS_PP_V_027 | pp_mon_veh_hv_volt_lower_limits() | pat_pp_pack_veh_v_vs_sum() |
 * | REQ_BMS_PP_V_028 | pp_mon_veh_hv_volt_lower_limits() | pat_pp_pack_veh_v_vs_sum() |
 * | REQ_BMS_PP_V_031 | pp_mon_chg_hv_volt_upper_limits() | |
 * | REQ_BMS_PP_V_032 | pp_mon_chg_hv_volt_upper_limits() | |
 * | REQ_BMS_PP_V_033 | pp_mon_chg_hv_volt_above_prev_value() | |
 * | REQ_BMS_PP_V_034 | pp_mon_chg_hv_volt_lower_limits() | |
 * | REQ_BMS_PP_V_035 | pp_mon_chg_hv_volt_lower_limits() | |
 * | REQ_BMS_PP_V_036 | pp_mon_chg_hv_volt_lower_limits() | |
 * | REQ_BMS_PP_V_037 | pp_mon_chg_hv_volt_lower_limits() | |
 * | REQ_BMS_PP_V_038 | #pp_chg_dischg_time_limit_ms | |
 * | REQ_BMS_PP_V_039 | pp_mon_chg_hv_volt_upper_limits() | |
 * | REQ_BMS_PP_V_040 | pp_mon_chg_hv_volt_upper_limits() | |
 * | REQ_BMS_PP_V_041 | pp_mon_chg_hv_volt_upper_limits() | |
 * | REQ_BMS_PP_V_042 | pp_mon_chg_hv_volt_upper_limits() | |
 * | REQ_BMS_PP_V_043 | pp_mon_chg_hv_volt_lower_limits() | |
 * | REQ_BMS_PP_V_044 | pp_mon_chg_hv_volt_lower_limits() | |
 * | REQ_BMS_PP_V_045 | pp_mon_chg_hv_volt_lower_limits() | |
 * | REQ_BMS_PP_V_046 | pp_mon_chg_hv_volt_lower_limits() | |
 * | REQ_BMS_PP_V_047 | pp_mon_chg_hv_volt_above_prev_value() | |
 * | REQ_BMS_PP_V_048 | pp_mon_chg_hv_volt_above_prev_value() | |
 * | REQ_BMS_PP_V_049 | pp_mon_veh_hv_volt_lower_limits() | pat_pp_pack_veh_v_vs_sum() |
 * | REQ_BMS_PP_V_050 | pp_mon_veh_hv_volt_lower_limits(), pp_mon_chg_hv_volt_lower_limits() | |
 * | REQ_BMS_PP_V_051 | duplicate functionality with ctc | |
 * | REQ_BMS_PP_V_052 | duplicate functionality with ctc | |
 * | REQ_BMS_PP_V_053 | pp_mon_chg_hv_volt_lower_limits() | |
 * | REQ_BMS_PP_V_054 | pp_mon_chg_hv_volt_lower_limits() | |
 * | REQ_BMS_PP_V_055 | Now part of CTC | |
 * | REQ_BMS_PP_V_056 | Now part of CTC | |
 *
 * @defgroup PackProtection PP Pack protection
 *
 */
/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "hardware.h"   /* For INTERNAL_ERROR */
#include "mmc.h"
#include "pp_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern CAL current_t pp_delta_i_threshold; /**< Threshold current level for stable bus voltages */
extern CAL pack_v_t  pp_pack_v_stable_drop;
extern CAL pack_v_t  pp_pack_v_unstable_drop;
extern CAL float32_t pp_pack_res_ohm;
extern CAL current_t pp_max_delta_i_threshold;

extern CAL temperature_t pp_v_vs_temp_lim;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 * Initialisation function for module to be called at startup.
 */
void pp_init(void);

/**
 * This function updates pack protection fault flags derived from high-frequency
 * data.
 *
 * Foreground monitoring tasks include:
 *  - Current monitoring
 *  - Vehicle high voltage bus monitoring
 *  - Charger bus monitoring
 *  - Power limit monitoring
 */
void pp_update_fg(void);

/**
 * This function updates pack protection fault flags either derived from
 * low-frequency or for slowly varying data (e.g. pack temperature).
 *
 * Background monitoring tasks include:
 *  - Vehicle bus and cell voltage sum comparison
 *  - Charger bus and cell voltage sum comparison
 *  - Bus discharge monitoring
 *  - Cell voltage monitoring
 *  - Cell temperature monitoring
 *  - State-of-health monitoring
 *  - Pack temperature monitoring
 */
void pp_update_bg(void);

#endif      /* PP_H */

