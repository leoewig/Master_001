/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    pp_p.h
 *
 * description:  Pack Protection Private Prototypes
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial version
 *                       
 *
 *****************************************************************************/
#if !defined(PP_P_H)
#define      PP_P_H
/**
 *
 * Pack Protection Private Prototypes
 *
 *
 * @file
 * @ingroup PP
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#include "fm.h"         /* For applying Fault filter. */
//#include "tmi_types.h"  /* Temperature Measurement. */
#include "mmc.h"
/*
 * Enumerations and macro definitioms made available must be documented here to
 * avoid duplication.
 *
 * PRIVATE functions are documented in the source file.
 * Non-PRIVATE variables and functions (i.e. those shared between files in the
 * feature) are documented here.
 */

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/


/******************************************************************************
 * CALIBRATIONS
 *****************************************************************************/
#if defined UNIT_TEST && !defined _lint

extern PRIVATE pack_v_t  pp_prev_veh_bus_v;      /* ut */
extern PRIVATE pack_v_t  pp_prev_chg_bus_v;      /* ut */
extern PRIVATE bool_t    pp_prev_veh_bus_v_valid;/* ut */
extern PRIVATE bool_t    pp_prev_chg_bus_v_valid;/* ut */
extern PRIVATE current_t pp_prev_i;
extern PRIVATE current_t pp_delta_i;

/**
 * Calibrations
 */
extern CAL float KP; 		//Proportional Const 
extern CAL float KI; 		//Integral Const 
extern CAL float KD; 			//Derivative Const 

//extern PRIVATE CAL current_t pp_max_discharge_veh_i_ma;
//extern PRIVATE CAL current_t pp_max_charge_veh_i_ma;
//extern PRIVATE CAL current_t pp_max_discharge_chg_i_ma;
//extern PRIVATE CAL current_t pp_max_charge_chg_i_ma;

extern PRIVATE CAL pack_v_t pp_max_crit_hv_v;
extern PRIVATE CAL pack_v_t pp_max_oper_hv_v;
extern PRIVATE CAL pack_v_t pp_min_crit_hv_v;
extern PRIVATE CAL pack_v_t pp_min_oper_hv_v;
extern PRIVATE CAL ts_time_t pp_veh_hv_dischg_time_limit_ms;
extern PRIVATE CAL ts_time_t pp_chg_dischg_time_limit_ms;
extern PRIVATE CAL pack_v_t pp_max_oper_cellsum_v;
extern PRIVATE CAL pack_v_t pp_max_crit_cellsum_v;
extern PRIVATE CAL pack_v_t pp_min_oper_cellsum_v;
extern PRIVATE CAL pack_v_t pp_min_crit_cellsum_v;
extern PRIVATE CAL pack_v_t pp_cellsum_v_tolerance;
extern PRIVATE CAL cell_v_t pp_max_crit_cell_v;
extern CAL cell_v_t pp_max_crit_cell_v;
extern PRIVATE CAL cell_v_t pp_max_oper_cell_v;
extern PRIVATE CAL cell_v_t pp_min_crit_cell_v;
extern CAL cell_v_t pp_min_crit_cell_v;
extern PRIVATE CAL cell_v_t pp_min_oper_cell_v;
extern PRIVATE CAL cell_v_t pp_min_for_vtbm_cell_v;
extern PRIVATE CAL cell_v_t pp_cell_v_max_delta;
extern CAL cell_v_t pp_cell_v_tolerance;
extern PRIVATE CAL temperature_t pp_cell_skin_min_t;
extern PRIVATE CAL temperature_t pp_cell_skin_max_t;
extern PRIVATE CAL temperature_t pp_cell_skin_max_delta_t;
extern CAL temperature_t pp_cell_skin_tolerance_t;
extern PRIVATE CAL temperature_t pp_cell_core_min_t;
extern PRIVATE CAL temperature_t pp_cell_core_max_t;
extern PRIVATE CAL temperature_t pp_cell_core_max_delta_t;
extern PRIVATE CAL temperature_t pp_cell_core_tolerance_t;
//extern PRIVATE CAL temperature_t pp_pack_max_t[Tmi_last_sensor];
//extern PRIVATE CAL temperature_t pp_pack_min_t[Tmi_last_sensor];
extern PRIVATE CAL percent_t     pp_min_soh;

/*---------------------------------------------------------------------------------------------*/
/*
 * PRIVATE function prototypes (documented in source)
 */
/* foreground functions
 */
extern PRIVATE void pp_monitor_operating_current(void);
extern PRIVATE void pp_mon_curnt_veh_ctc_closed(void);
extern PRIVATE void pp_mon_curnt_chg_ctc_closed(void);
extern PRIVATE void pp_mon_curnt_at_zero_indication(void);

extern PRIVATE void pp_monitor_hv_vehicle_voltage(void);
extern PRIVATE void pp_mon_veh_hv_volt_upper_limits(void);
extern PRIVATE void pp_mon_veh_hv_volt_above_prev_value(void);
extern PRIVATE void pp_mon_veh_hv_volt_lower_limits(void);

extern PRIVATE void pp_monitor_hv_charger_voltage(void);
extern PRIVATE void pp_mon_chg_hv_volt_upper_limits(void);
extern PRIVATE void pp_mon_chg_hv_volt_above_prev_value(void);
extern PRIVATE void pp_mon_chg_hv_volt_lower_limits(void);

extern PRIVATE void pp_monitor_current_limit(void);
extern PRIVATE void pp_mon_pack_i_discharge_lim(void);
extern PRIVATE void pp_mon_pack_i_charge_lim_veh_ctc_closed(void);
extern PRIVATE void pp_mon_pack_i_charge_lim_chg_ctc_closed(void);


/* background functions
 */
extern PRIVATE void pp_background_monitor_sum_cell_voltage_limits(void);
extern PRIVATE void pp_background_monitor_cell_voltage_limits(void);
extern PRIVATE void pp_background_monitor_modelled_cell_temps(void);
extern PRIVATE void pp_background_monitor_measured_cell_temps(void);
extern PRIVATE void pp_background_monitor_state_of_health(void);
extern PRIVATE void pp_background_monitor_pack_temperatures(void);
#if 0
extern PRIVATE void pp_background_monitor_veh_hv_volt_v_vs_sum(void);
#endif
extern PRIVATE void pp_background_monitor_chg_hv_volt_v_vs_sum(void);

#endif      /* UNIT TEST */

PRIVATE void pp_bg_monitor_veh_hv_v_vs_sum(void);
PRIVATE void pp_v_vs_sum_conditions(
    bool_t *vi_readings_valid_p,
    bool_t *above_temp_thold_p);
PRIVATE void pp_v_vs_sum_params(
    bool_t vi_conditions_met,
    bool_t above_temp_thold,
    bool_t *mon_cond_met_p,
    pack_v_t *tolerance_p,
    Fm_fault_subtype_list_e *subtype_p);

PRIVATE void pp_bg_check_veh_hv_v_vs_sum(bool_t mon_cond_met,
    pack_v_t tolerance, Fm_fault_subtype_list_e subtype);
#endif      /* PP_P_H */

