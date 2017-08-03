/**
 * @file
 * @ingroup mmc_cfg
 * @ingroup dio
 *
 * MMC digital I/O configuration
 *
 * @note this file will be copied by the build system to dio_cfg.h
 *
 * $Id: dio_cfg.h 873 2015-03-16 11:19:41Z j_bailey $
 */
/* Copyright (c) Ricardo plc 2012 */

#ifndef MMC_DIO_DEF_H
#define MMC_DIO_DEF_H

#include "dio_types_cust.h"


/** Input Variables */
extern bool_t di_psu_event_alert;
extern bool_t di_cc_wkup_mmc;
extern bool_t di_vtbm_5v;
extern bool_t di_vtbm_gnd;
extern bool_t di_isa_diag;
extern bool_t di_chg_control_pilot;
extern bool_t di_ign_dbn;
extern bool_t di_a_pos;
extern bool_t di_ens;
extern bool_t di_can_veh_wake_up;

#if defined (PLATFORM_TEST)
extern bool_t di_spare_input_1;
extern bool_t di_spare_input_2;
extern bool_t di_spare_input_3;
extern bool_t di_spare_input_4;
#endif /* defined (PLATFORM_TEST) */

/**
 * charger pilot PWM input data
 */
extern Dio_pin_signal_data_t pi_chg_pilot_data;

/** Output Variables */
extern bool_t do_veh_dc_pos_hs_mmc;
extern bool_t do_veh_dc_neg_hs_mmc;
extern bool_t do_veh_dc_pos_ls_mmc;
extern bool_t do_veh_dc_neg_ls_mmc;
extern bool_t do_chg_dc_pos_hs_mmc;
extern bool_t do_chg_dc_neg_hs_mmc;
extern bool_t do_chg_dc_pos_ls_mmc;
extern bool_t do_chg_dc_neg_ls_mmc;
extern bool_t do_pre_chg_hs_enbl;
extern bool_t do_pre_chg_ls_enbl;
extern bool_t do_ptc_control;
extern bool_t do_wdi;
extern bool_t do_psu_ltch_clk;
extern bool_t do_isa_enable;
extern bool_t do_5v_ref_buf_enb;
extern bool_t do_pilot_control;
extern bool_t do_iso_det_vehicle;
extern bool_t do_iso_det_charger;
extern bool_t do_lem_spl_enbl;
extern bool_t do_vtbm_can_enbl_hs;
extern bool_t do_vtbm_can_enbl_ls;
extern bool_t do_cp3;
extern bool_t do_led_pe_output_1;
extern bool_t do_spare_output_hs;
extern bool_t do_spare_output_ls;
extern bool_t do_keep_alive;
extern bool_t do_veh_can_dis;
extern bool_t do_chg_can_dis;
extern bool_t do_led_pj_output_2;
extern bool_t do_driver_enable_edge;
extern bool_t do_ana_mux_a0;
extern bool_t do_ana_mux_a1;
extern bool_t do_ana_mux_a2;
extern bool_t do_ana_mux_1_en;
extern bool_t do_psu_ltch_str;
extern bool_t do_led_evb_only_4;

extern bool_t       po_enabled_fan1;
extern percent_t    po_duty_fan1;
extern frequency_t  po_freq_fan1;


/**
 *  Application variables in which to store the most recently demanded level
 *  of an output (taking into account active high/low state.
 */
extern bool_t do_dmd_veh_dc_pos_hs_mmc;
extern bool_t do_dmd_veh_dc_pos_ls_mmc;
extern bool_t do_dmd_keep_alive;
extern bool_t do_dmd_veh_dc_neg_hs_mmc;
extern bool_t do_dmd_veh_dc_neg_ls_mmc;
extern bool_t do_dmd_chg_dc_pos_hs_mmc;
extern bool_t do_dmd_chg_dc_neg_hs_mmc;
extern bool_t do_dmd_chg_dc_pos_ls_mmc;
extern bool_t do_dmd_chg_dc_neg_ls_mmc;
extern bool_t do_dmd_pre_chg_hs_enbl;
extern bool_t do_dmd_pre_chg_ls_enbl;
extern bool_t do_dmd_ptc_control;
extern bool_t do_dmd_wdi;
extern bool_t do_dmd_psu_ltch_clk;
extern bool_t do_dmd_isa_enable;
extern bool_t do_dmd_5v_ref_buf_enb;
extern bool_t do_dmd_pilot_control;
extern bool_t do_dmd_iso_det_vehicle;
extern bool_t do_dmd_iso_det_charger;
extern bool_t do_dmd_lem_spl_enbl;
extern bool_t do_dmd_vtbm_can_enbl_hs;
extern bool_t do_dmd_vtbm_can_enbl_ls;
extern bool_t do_dmd_cp3;
extern bool_t do_dmd_led_pe_output_1;
extern bool_t do_dmd_spare_output_hs;
extern bool_t do_dmd_spare_output_ls;
extern bool_t do_dmd_veh_can_dis;
extern bool_t do_dmd_driver_enable_edge;
extern bool_t do_dmd_chg_can_dis;
extern bool_t do_dmd_led_pj_output_2;
extern bool_t do_dmd_ana_mux_a0;
extern bool_t do_dmd_ana_mux_a1;
extern bool_t do_dmd_ana_mux_a2;
extern bool_t do_dmd_ana_mux_1_en;
extern bool_t do_dmd_psu_ltch_str;
extern bool_t do_dmd_led_evb_only_4;

/**
 *  Application variables in which to store the value
 *  read back from an output pin.
 */
extern bool_t do_rb_veh_dc_pos_hs_mmc;
extern bool_t do_rb_veh_dc_pos_ls_mmc;
extern bool_t do_rb_veh_dc_neg_hs_mmc;
extern bool_t do_rb_veh_dc_neg_ls_mmc;
extern bool_t do_rb_chg_dc_pos_hs_mmc;
extern bool_t do_rb_chg_dc_neg_hs_mmc;
extern bool_t do_rb_chg_dc_pos_ls_mmc;
extern bool_t do_rb_chg_dc_neg_ls_mmc;
extern bool_t do_rb_pre_chg_hs_enbl;
extern bool_t do_rb_pre_chg_ls_enbl;
extern bool_t do_rb_ptc_control;
extern bool_t do_rb_wdi;
extern bool_t do_rb_psu_ltch_clk;
extern bool_t do_rb_keep_alive;
extern bool_t do_rb_isa_enable;
extern bool_t do_rb_5v_ref_buf_enb;
extern bool_t do_rb_pilot_control;
extern bool_t do_rb_iso_det_vehicle;
extern bool_t do_rb_iso_det_charger;
extern bool_t do_rb_lem_spl_enbl;
extern bool_t do_rb_vtbm_can_enbl_hs;
extern bool_t do_rb_vtbm_can_enbl_ls;
extern bool_t do_rb_cp3;
extern bool_t do_rb_led_pe_output_1;
extern bool_t do_rb_spare_output_hs;
extern bool_t do_rb_spare_output_ls;
extern bool_t do_rb_veh_can_dis;
extern bool_t do_rb_chg_can_dis;
extern bool_t do_rb_driver_enable_edge;
extern bool_t do_rb_led_pj_output_2;
extern bool_t do_rb_ana_mux_a0;
extern bool_t do_rb_ana_mux_a1;
extern bool_t do_rb_ana_mux_a2;
extern bool_t do_rb_ana_mux_1_en;
extern bool_t do_rb_psu_ltch_str;
extern bool_t do_rb_led_evb_only_4;

/* TMM2.0 */
extern bool_t do_hbrige_1;
extern bool_t do_hbrige_2;
extern bool_t do_hbrige_3;
extern bool_t do_hbrige_4;
extern bool_t do_lin_en_2;
extern bool_t do_lin_en_1;
extern bool_t do_dac_en_1;
extern bool_t do_dac_en_2;
extern bool_t do_tmm_trigger_0;
extern bool_t do_tmm_trigger_1;
extern bool_t do_tmm_trigger_2;
extern bool_t do_tmm_trigger_3;
extern bool_t do_tmm_trigger_4;
extern bool_t do_tmm_trigger_5;
extern bool_t do_tmm_trigger_6;
extern bool_t do_tmm_trigger_7;

#endif /* MMC_DIO_DEF_H */
