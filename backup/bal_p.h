/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    bal_p.h
 *
 * description:  
 * Balancing feature private header
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
#if !defined(BAL_P_H)
#define      BAL_P_H
/**
 *
 * Balancing feature private header
 *
 * @file
 * @ingroup bal
 */

/* Copyright (c) Linkedriving Tech 2015 - All rights resevered */

/*
 * PRIVATE function prototypes (documented in source)
 */
PRIVATE void bal_init_chg_estimate_timer(void);
PRIVATE void bal_init_vtbm_data(void);
PRIVATE bool_t bal_check_chg_estmn(void);
PRIVATE void bal_update_estimation_timer(void);
PRIVATE bool_t bal_new_chg_estimation(void);
PRIVATE Bal_status_e bal_determine_balancing_status(
                                                Bal_status_e current_status);
PRIVATE bool_t bal_disabled_check(void);
PRIVATE bool_t bal_chg_maintenance_check(void);
PRIVATE bool_t bal_halted_check(Bal_status_e current_status,
                                                bal_halted_reason_e *reason_p);
PRIVATE bool_t bal_12v_too_low(Bal_status_e current_status);


PRIVATE void bal_calculate_mode_dmd(void);
PRIVATE void bal_update_mdm_data(void);
PRIVATE void bal_override_span(void);
PRIVATE Bal_status_e bal_determine_ar_status(Bal_status_e current_status);

PRIVATE void bal_find_minmax_pack(void);
PRIVATE void bal_find_minmax_all_vtbms(void);
PRIVATE void bal_set_mode_dmd_all_vtbms(void);
PRIVATE bool_t bal_set_mode_dmd_vtbm(uint8_t vtbm_num);
PRIVATE void bal_find_cell_minmax_chg(FPTR cellcount_t *min_idx,FPTR cellcount_t *max_idx,
                        cellcount_t firstcell, cellcount_t length);
PRIVATE void bal_find_minmax_vtbm(uint8_t vtbm_num);
PRIVATE float32_t bal_find_min_v(cellcount_t firstcell, cellcount_t length);

PRIVATE bool_t bal_can_receive_active(uint8_t vtbm_num);
PRIVATE bool_t bal_supplier_can_supply_active(uint8_t vtbm_num);
PRIVATE bool_t bal_can_receive_passive(uint8_t vtbm_num);

PRIVATE void bal_update_spans(void);
PRIVATE void bal_capture_span (enum BalEstPos_e pos);
PRIVATE void bal_erase_span (enum BalEstPos_e pos);

PRIVATE void bal_span_latest_exceeded(enum BalEstPos_e alt_pos);
PRIVATE void bal_evaluate_span(void);

PRIVATE void bal_calc_dyn_tolerance(void);
PRIVATE void bal_calc_applied_tol(void);

#ifdef UNIT_TEST
/*
 * provide convenient defnitions when testing
 */
extern CAL bool_t bal_enable_balancing ;
extern CAL ts_time_t bal_correction_duration_ms ;
extern CAL supply_v_t bal_12v_batt_limit ;
extern CAL supply_v_t bal_12v_batt_hyst ;
extern CAL float32_t bal_min_cell_t ;
extern CAL float32_t bal_max_cell_v;
extern CAL float32_t bal_min_cell_v;
extern CAL bool_t bal_ignore_cm_inhibit;
extern CAL float32_t bal_chg_ar_tol_C;

extern CAL span_t bal_new_span_lim[BAL_EST_COUNT];
extern CAL span_t bal_alt_span_lim[BAL_EST_COUNT];
extern CAL span_t bal_new_span_accept[BAL_EST_COUNT];
extern CAL uint32_t bal_span_age_max;
extern CAL float32_t bal_chg_tolerance_frac;

extern FRAM float32_t bal_chg_applied_tol_C;
extern FRAM float32_t bal_chg_dyn_tolerance_C;
extern FRAM ts_time_t bal_prev_chg_estmn_time_ms;
extern FRAM uint16_t bal_prev_pcd_ctr;
extern FRAM ts_time_t bal_chg_estmn_start_time_ms;
extern FRAM bal_halted_reason_e bal_halted_reason;
extern FRAM cellcount_t bal_pack_min_chg_idx;
extern FRAM cellcount_t bal_pack_max_chg_idx;
extern FRAM float32_t bal_pack_min_chg_C;
extern FRAM float32_t bal_pack_max_chg_C;
extern FRAM cellcount_t bal_vtbm_min_chg_idx[MAX_NUM_VTBM];
extern FRAM cellcount_t bal_vtbm_max_chg_idx[MAX_NUM_VTBM];
extern FRAM float32_t bal_vtbm_min_chg_C[MAX_NUM_VTBM];
extern FRAM float32_t bal_vtbm_max_chg_C[MAX_NUM_VTBM];
extern FRAM float32_t bal_vtbm_min_chg_V[MAX_NUM_VTBM];
extern FRAM float32_t bal_vtbm_max_chg_V[MAX_NUM_VTBM];
extern FRAM float32_t bal_vtbm_min_v[MAX_NUM_VTBM];

#endif /* ifdef UNIT_TEST */

#endif      /* BAL_P_H */
