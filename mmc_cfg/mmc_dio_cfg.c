/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_dio_cfg.c
 *
 * description:  
 * MMC digital I/O configuration
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 * 2015-Sep-04 Rui Li           Change DIO configure for new BMS
 *
 *****************************************************************************/
/**
 */

/**
 *  variables for 'unused' inputs are conditionally defined
 *  they are NOT in MMC build to avoid clutter
 *  they ARE in PLAT_TEST to enable checking of test-environment
 *  (switched by if defined (PLATFORM_TEST))
 */

#include "basic_types.h"
#include "dio.h"
#include "dio_cfg.h"
#include "dio_veh_can_disable.h"
#include "dio_chg_can_disable.h"

bool_t di_chg_control_pilot;

/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store val read from pin }
 */
bool_t di_ens;

/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store val read from pin }
 */
bool_t di_ign_dbn;

/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store val read from pin @mantis{3883} }
 */
bool_t di_psu_event_alert;



/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store val read from pin @mantis{3883} }
 */

bool_t di_isa_diag;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store val read from pin}
 */

bool_t di_cc_wkup_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store val read from pin}
 */

bool_t di_vtbm_5v;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store val read from pin}
 */

bool_t di_vtbm_gnd;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store val read from pin}
 */

bool_t di_psu_ltch_so;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store val read from pin}
 */

bool_t di_bms_enbl_psu_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store val read from pin}
 */

bool_t di_bms_a_pos;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store val read from pin}
 */

bool_t di_input_1;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store val read from pin}
 */

bool_t di_input_2;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store val read from pin}
 */

#if defined (PLATFORM_TEST)
bool_t di_spare_input_1;      /**< App variable to store val read from pin */
bool_t di_spare_input_2;      /**< App variable to store val read from pin */
bool_t di_spare_input_3;      /**< App variable to store val read from pin */
bool_t di_spare_input_4;      /**< App variable to store val read from pin */
#endif /* defined (PLATFORM_TEST) */

#if defined (HOSTED)
bool_t di_test_dummy;   /**< For testing purposes only */
#endif

bool_t do_veh_dc_pos_hs_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_veh_dc_neg_hs_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_veh_dc_pos_ls_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_veh_dc_neg_ls_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_chg_dc_pos_hs_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_chg_dc_neg_hs_mmc;

bool_t do_en_LIN;
bool_t do_dmd_en_LIN;
bool_t do_rb_en_LIN;

/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_chg_dc_pos_ls_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_chg_dc_neg_ls_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_pre_chg_hs_enbl;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_pre_chg_ls_enbl;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_isa_enable;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_5v_ref_buf_enb;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_pilot_control;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_iso_det_vehicle;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_iso_det_charger;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_lem_spl_enbl;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_vtbm_can_enbl_hs;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_vtbm_can_enbl_ls;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_vtbm_acquisition;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_led_pe_output_1;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_spare_output_hs;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_spare_output_ls;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_keep_alive;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_veh_can_dis;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_chg_can_dis;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_led_pj_output_2;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_driver_enable_edge;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_ana_mux_a0;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_ana_mux_a1;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_ana_mux_a2;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_ana_mux_1_en;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_ana_mux_2_en;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_psu_ltch_str;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_led_evb_only_3;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_led_evb_only_4;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store value to write to pin}
 */

bool_t do_dmd_veh_dc_pos_hs_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_veh_dc_pos_ls_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_keep_alive;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_veh_dc_neg_hs_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_veh_dc_neg_ls_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_chg_dc_pos_hs_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_chg_dc_neg_hs_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_chg_dc_pos_ls_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_chg_dc_neg_ls_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_pre_chg_hs_enbl;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_pre_chg_ls_enbl;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_isa_enable;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_5v_ref_buf_enb;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_pilot_control;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_iso_det_vehicle;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_iso_det_charger;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_lem_spl_enbl;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_vtbm_can_enbl_hs;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_vtbm_can_enbl_ls;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_vtbm_acquisition;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_led_pe_output_1;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_spare_output_hs;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_spare_output_ls;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_veh_can_dis;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_driver_enable_edge;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_chg_can_dis;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_led_pj_output_2;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_ana_mux_a0;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_ana_mux_a1;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_ana_mux_a2;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_ana_mux_1_en;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_ana_mux_2_en;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_psu_ltch_str;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_led_evb_only_3;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_dmd_led_evb_only_4;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App variable to store most recent demand}
 */

bool_t do_rb_veh_dc_pos_hs_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_veh_dc_pos_ls_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_veh_dc_neg_hs_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_veh_dc_neg_ls_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_chg_dc_pos_hs_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_chg_dc_neg_hs_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_chg_dc_pos_ls_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_chg_dc_neg_ls_mmc;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_pre_chg_hs_enbl;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

 bool_t do_rb_pre_chg_ls_enbl;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_keep_alive;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_isa_enable;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_5v_ref_buf_enb;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_pilot_control;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_iso_det_vehicle;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_iso_det_charger;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_lem_spl_enbl;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_vtbm_can_enbl_hs;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_vtbm_can_enbl_ls;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_vtbm_acquisition;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_led_pe_output_1;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_spare_output_hs;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_spare_output_ls;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_veh_can_dis;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_chg_can_dis;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_driver_enable_edge;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_led_pj_output_2;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_ana_mux_a0;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_ana_mux_a1;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_ana_mux_a2;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_ana_mux_1_en;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_ana_mux_2_en;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_psu_ltch_str;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_led_evb_only_3;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */

bool_t do_rb_led_evb_only_4;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, App var to store value read back from pin}
 */
 
 /*LDT*/
bool_t do_pre_charg_hs_output;
bool_t do_vehicle_dc_neg_ls_output;
bool_t do_d_dac_ctrl_1;
bool_t do_d_dac_ctrl_2;
bool_t do_vtbm_gnd_enbl_ls;
bool_t do_charger_dc_neg_ls;
bool_t do_wdi_output;
bool_t do_hbrige_1;
bool_t do_hbrige_2;
bool_t do_hbrige_3;
bool_t do_hbrige_4;


bool_t do_dmd_pre_charg_hs_output;
bool_t do_dmd_vehicle_dc_neg_ls_output;
bool_t do_dmd_d_dac_ctrl_1;
bool_t do_dmd_d_dac_ctrl_2;
bool_t do_dmd_vtbm_gnd_enbl_ls;
bool_t do_dmd_charger_dc_neg_ls;
bool_t do_dmd_wdi_output;
bool_t do_dmd_hbrige_1;
bool_t do_dmd_hbrige_2;
bool_t do_dmd_hbrige_3;
bool_t do_dmd_hbrige_4;


bool_t do_rb_pre_charg_hs_output;
bool_t do_rb_vehicle_dc_neg_ls_output;
bool_t do_rb_d_dac_ctrl_1;
bool_t do_rb_d_dac_ctrl_2;
bool_t do_rb_vtbm_gnd_enbl_ls;
bool_t do_rb_charger_dc_neg_ls;
bool_t do_rb_wdi_output;
bool_t do_rb_hbrige_1;
bool_t do_rb_hbrige_2;
bool_t do_rb_hbrige_3;
bool_t do_rb_hbrige_4;

/* TMM2.0 */
bool_t do_lin_en_2;
bool_t do_lin_en_1;
bool_t do_dac_en_1;
bool_t do_dac_en_2;
bool_t do_tmm_trigger_0;
bool_t do_tmm_trigger_1;
bool_t do_tmm_trigger_2;
bool_t do_tmm_trigger_3;
bool_t do_tmm_trigger_4;
bool_t do_tmm_trigger_5;
bool_t do_tmm_trigger_6;
bool_t do_tmm_trigger_7;

bool_t do_dmd_lin_en_2;
bool_t do_dmd_lin_en_1;
bool_t do_dmd_dac_en_1;
bool_t do_dmd_dac_en_2;
bool_t do_dmd_tmm_trigger_0;
bool_t do_dmd_tmm_trigger_1;
bool_t do_dmd_tmm_trigger_2;
bool_t do_dmd_tmm_trigger_3;
bool_t do_dmd_tmm_trigger_4;
bool_t do_dmd_tmm_trigger_5;
bool_t do_dmd_tmm_trigger_6;
bool_t do_dmd_tmm_trigger_7;

bool_t do_rb_lin_en_2;
bool_t do_rb_lin_en_1;
bool_t do_rb_dac_en_1;
bool_t do_rb_dac_en_2;
bool_t do_rb_tmm_trigger_0;
bool_t do_rb_tmm_trigger_1;
bool_t do_rb_tmm_trigger_2;
bool_t do_rb_tmm_trigger_3;
bool_t do_rb_tmm_trigger_4;
bool_t do_rb_tmm_trigger_5;
bool_t do_rb_tmm_trigger_6;
bool_t do_rb_tmm_trigger_7;

Dio_pin_signal_data_t pi_chg_pilot_data;

/******************************************************************************
 * CALIBRATIONS
 *****************************************************************************/

#undef MEMORY_LOCATION
#define MEMORY_LOCATION CALIBRATION_MEMORY
#include "memory_layout.h"

CAL bool_t do_or_en_driver_latch_en = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_veh_dc_pos_hs_mmc = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_veh_dc_pos_ls_mmc = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_veh_dc_neg_hs_mmc = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_veh_dc_neg_ls_mmc = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_chg_dc_pos_hs_mmc = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_chg_dc_neg_hs_mmc = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_chg_dc_pos_ls_mmc = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_chg_dc_neg_ls_mmc = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_pre_chg_hs_enbl = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_pre_chg_ls_enbl = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_isa_enable = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_5v_ref_buf_enb = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_pilot_control = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_iso_det_vehicle = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_iso_det_charger = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_lem_spl_enbl = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_vtbm_can_enbl_hs = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_vtbm_can_enbl_ls = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_vtbm_acquisition = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_led_pe_output_1 = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_spare_output_hs = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_spare_output_ls = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_keep_alive = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_veh_can_dis = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_chg_can_dis = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_driver_enable_edge = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_led_pj_output_2 = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_ana_mux_a0 = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_ana_mux_a1 = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_ana_mux_a2 = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_ana_mux_1_en = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_ana_mux_2_en = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_psu_ltch_str = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_led_evb_only_3 = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_en_led_evb_only_4 = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Override enable flag}
 */

CAL bool_t do_or_val_veh_dc_pos_hs_mmc = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_veh_dc_pos_ls_mmc = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_veh_dc_neg_hs_mmc = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_veh_dc_neg_ls_mmc = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_chg_dc_pos_hs_mmc = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_chg_dc_neg_hs_mmc = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_chg_dc_pos_ls_mmc = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_chg_dc_neg_ls_mmc = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_pre_chg_hs_enbl = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_pre_chg_ls_enbl = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_isa_enable = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_5v_ref_buf_enb = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_pilot_control = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_iso_det_vehicle = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_iso_det_charger = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_lem_spl_enbl = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_vtbm_can_enbl_hs = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_vtbm_can_enbl_ls = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_vtbm_acquisition = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_led_pe_output_1 = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_spare_output_hs = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_spare_output_ls = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_keep_alive = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_veh_can_dis = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_chg_can_dis = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_driver_enable_edge = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_led_pj_output_2 = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_ana_mux_a0 = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_ana_mux_a1 = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_ana_mux_a2 = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_ana_mux_1_en = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_ana_mux_2_en = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_psu_ltch_str = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_led_evb_only_3 = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */

CAL bool_t do_or_val_led_evb_only_4 = FALSE;
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Value used for output when overriding}
 */
 
 
/*LDT*/
CAL bool_t do_or_en_pre_charg_hs_output = FALSE;
CAL bool_t do_or_en_vehicle_dc_neg_ls_output = FALSE;
CAL bool_t do_or_en_d_dac_ctrl_1 = FALSE;
CAL bool_t do_or_en_d_dac_ctrl_2 = FALSE;
CAL bool_t do_or_en_vtbm_gnd_enbl_ls = FALSE;
CAL bool_t do_or_en_charger_dc_neg_ls = FALSE;
CAL bool_t do_or_en_wdi_output = FALSE;
CAL bool_t do_or_en_hbrige_1 = FALSE;
CAL bool_t do_or_en_hbrige_2 = FALSE;
CAL bool_t do_or_en_hbrige_3 = FALSE;
CAL bool_t do_or_en_hbrige_4 = FALSE;

CAL bool_t do_or_val_pre_charg_hs_output = FALSE;
CAL bool_t do_or_val_vehicle_dc_neg_ls_output = FALSE;
CAL bool_t do_or_val_d_dac_ctrl_1 = FALSE;
CAL bool_t do_or_val_d_dac_ctrl_2 = FALSE;
CAL bool_t do_or_val_vtbm_gnd_enbl_ls = FALSE;
CAL bool_t do_or_val_charger_dc_neg_ls = FALSE;
CAL bool_t do_or_val_wdi_output = FALSE;
CAL bool_t do_or_val_hbrige_1 = FALSE;
CAL bool_t do_or_val_hbrige_2 = FALSE;
CAL bool_t do_or_val_hbrige_3 = FALSE;
CAL bool_t do_or_val_hbrige_4 = FALSE;

CAL bool_t do_or_en_en_LIN = FALSE;
CAL bool_t do_or_val_en_LIN = FALSE;

CAL bool_t do_or_en_lin_en_2;
CAL bool_t do_or_en_lin_en_1;
CAL bool_t do_or_en_dac_en_1;
CAL bool_t do_or_en_dac_en_2;

CAL bool_t do_or_en_tmm_trigger_0;
CAL bool_t do_or_en_tmm_trigger_1;
CAL bool_t do_or_en_tmm_trigger_2;
CAL bool_t do_or_en_tmm_trigger_3;
CAL bool_t do_or_en_tmm_trigger_4;
CAL bool_t do_or_en_tmm_trigger_5;
CAL bool_t do_or_en_tmm_trigger_6;
CAL bool_t do_or_en_tmm_trigger_7;

CAL bool_t do_or_val_lin_en_2;
CAL bool_t do_or_val_lin_en_1;
CAL bool_t do_or_val_dac_en_1;
CAL bool_t do_or_val_dac_en_2;

CAL bool_t do_or_val_tmm_trigger_0;
CAL bool_t do_or_val_tmm_trigger_1;
CAL bool_t do_or_val_tmm_trigger_2;
CAL bool_t do_or_val_tmm_trigger_3;
CAL bool_t do_or_val_tmm_trigger_4;
CAL bool_t do_or_val_tmm_trigger_5;
CAL bool_t do_or_val_tmm_trigger_6;
CAL bool_t do_or_val_tmm_trigger_7;


#undef MEMORY_LOCATION
#define MEMORY_LOCATION S12_MEMORY
#include "memory_layout.h"


CONST struct dio_in_cfg_s dio_input_cfg[] = {

    /** --- Port A - Inputs --- */
/*    ptr-to-var             port        bit polarity         interface  */
 //   { &di_can_veh_wake_up,   Dio_port_a, 0u, Dio_active_low,  Dio_interface_function },
 //   { &di_psu_event_alert,   Dio_port_a, 4u, Dio_active_high, Dio_interface_function },

    /* --- Ports B -inputs --- */
    //{ &di_cc_wkup_mmc,   Dio_port_b, 4u, Dio_active_high, Dio_interface_function },
	

    /* --- Ports C & D don't exist --- */

    /* --- Port E - inputs --- */
   // { &di_vtbm_5v,   Dio_port_e, 0u, Dio_active_high, Dio_interface_function },
    { &di_vtbm_gnd,   Dio_port_e, 1u, Dio_active_high, Dio_interface_function },

    /* --- Ports F & G don't exist --- */

    /* --- Port H are all Outputs or alternative function pins --- */
    /* bits 0..3 are EEPROM SPI */

    /* --- Port I doesn't exist --- */

    /* --- Port J are all Outputs or alternative function pins --- */
    /* bits 6 and 7 are RTC I2C */

    /** --- Port K - Inputs --- */
/*    ptr-to-var             port        bit polarity         interface  */
    /* bit 6 does not exist */
    { &di_isa_diag,          Dio_port_k, 7u, Dio_active_low,  Dio_interface_function },

    /* --- Port L doesn't exist --- */

    /* --- Port M has no inputs (it is 4 CAN ports) --- */

    /* --- Ports N & O don't exist --- */

    /* --- Port P are all Outputs --- */

    /* --- Ports Q & R don't exist --- */

    /* --- Port S - inputs or alternative function pins --- */
    /* bits 4 and 6 are PSU SPI */
    // { &di_psu_ltch_so,  Dio_port_s, 4u, Dio_active_low,  Dio_interface_function }, 

    /** --- Port T - Inputs --- */
/*    ptr-to-var             port        bit polarity         interface  */
    { &di_chg_control_pilot, Dio_port_t, 1u, Dio_active_high, Dio_interface_function },
    //{ &di_ign_dbn,              Dio_port_t, 2u, Dio_active_low,  Dio_interface_function },  /** Schematic calls this BMS_ENBL */
    //{ &di_bms_enbl_psu_mmc,  Dio_port_t, 2u, Dio_active_low,  Dio_interface_function }, 
    //{ &di_bms_a_pos,           Dio_port_t, 4u, Dio_active_low,  Dio_interface_function },
    //{ &di_ens,                        Dio_port_t, 5u, Dio_active_low,  Dio_interface_function },
   // { &di_input_1,                  Dio_port_t, 6u, Dio_active_low,  Dio_interface_function },
   // { &di_input_2,                  Dio_port_t, 7u, Dio_active_low,  Dio_interface_function },
    
#if defined (PLATFORM_TEST)
    { &di_spare_input_1,     Dio_port_t, 4u, Dio_active_low, Dio_interface_function },
    { &di_spare_input_2,     Dio_port_t, 5u, Dio_active_low, Dio_interface_function },
    { &di_spare_input_3,     Dio_port_t, 6u, Dio_active_low, Dio_interface_function },
    { &di_spare_input_4,     Dio_port_t, 7u, Dio_active_low, Dio_interface_function },
#endif /* defined (PLATFORM_TEST) */
#if defined HOSTED
/* These are here to ensure that the array is big enough for test_input_cfg1
 * the values will be overwritten so aren't important
 */
    { &di_test_dummy,Dio_port_t, 3u, Dio_active_high, Dio_interface_function },
    { &di_test_dummy,Dio_port_t, 3u, Dio_active_high, Dio_interface_function },
    { &di_test_dummy,Dio_port_t, 3u, Dio_active_high, Dio_interface_function },
    { &di_test_dummy,Dio_port_t, 3u, Dio_active_high, Dio_interface_function },
    { &di_test_dummy,Dio_port_t, 3u, Dio_active_high, Dio_interface_function },
    { &di_test_dummy,Dio_port_t, 3u, Dio_active_high, Dio_interface_function },
#endif  /* HOSTED */
};
/**<
 * @a2l{[].port, LongIdentifier, I/O port}
 * @a2l{[].bitnum, conversion, uint8_t}
 * @a2l{[].bitnum, LongIdentifier, pin number of I/O port (range 0 to 7)}
 * @a2l{[].interface, LongIdentifier, direct or via dio_read_inputs() dio_write_outputs()}
 */

/**
 * @todo locate in shared CPU/XGATE flash
 */
CONST struct dio_out_cfg_s dio_output_cfg[] =
{
    /** Port A - Outputs */
/*             var root           port        bit polarity         interface  */
    /*
     * NB The MMC bootblock needs to drive veh_can_dis, too, so if it changes here then
     * VEH_CAN_DIS_PORT, VEH_CAN_DIS_DDR and VEH_CAN_DIS_MASK must be changed in the
     * bootblock, too.
     */

    /*Linkedriving configured for a new BMS*/	
    DEF_DIG_OP(tmm_trigger_0,       Dio_port_a, 0u, Dio_active_high, Dio_interface_function),
    DEF_DIG_OP(tmm_trigger_1,       Dio_port_a, 1u, Dio_active_high, Dio_interface_function),
    DEF_DIG_OP(tmm_trigger_2,       Dio_port_a, 2u, Dio_active_high, Dio_interface_function),
    DEF_DIG_OP(tmm_trigger_3, 		Dio_port_a, 3u, Dio_active_high, Dio_interface_function),
    /*PA5 is not an output, is used as PSU EVENT Alert PWM Input*/
    DEF_DIG_OP(tmm_trigger_4, 		Dio_port_a, 4u, Dio_active_high, Dio_interface_function),
    DEF_DIG_OP(tmm_trigger_5, 		Dio_port_a, 5u, Dio_active_high, Dio_interface_function),
    DEF_DIG_OP(tmm_trigger_6,       Dio_port_a, 6u, Dio_active_high, Dio_interface_function),
    DEF_DIG_OP(tmm_trigger_7,       Dio_port_a, 7u, Dio_active_high, Dio_interface_function),
     /*
     * NB The MMC bootblock drives do_keep_alive, too, so if it changes here,
     * the bootblock will need updating
     */
    //DEF_DIG_OP(keep_alive,        Dio_port_a, 7u, Dio_active_high, Dio_interface_function),
    /** Port B - Outputs */
/*             var root           port        bit polarity         interface  */
    DEF_DIG_OP(pre_chg_ls_enbl,   Dio_port_b, 0u, Dio_active_high, Dio_interface_function),
    DEF_DIG_OP(5v_ref_buf_enb,    Dio_port_b, 1u, Dio_active_low,  Dio_interface_function),
    DEF_DIG_OP(iso_det_vehicle,   Dio_port_b, 2u, Dio_active_high, Dio_interface_function),
    DEF_DIG_OP(lin_en_2,     Dio_port_b, 3u, Dio_active_high, Dio_interface_function),
    DEF_DIG_OP(lin_en_1,    Dio_port_b, 4u, Dio_active_high, Dio_interface_function),
    DEF_DIG_OP(dac_en_1,   Dio_port_b, 5u, Dio_active_high, Dio_interface_function),
    DEF_DIG_OP(dac_en_2,      Dio_port_b, 6u, Dio_active_high,  Dio_interface_function),
    //DEF_DIG_OP(vtbm_can_enbl_hs,  Dio_port_b, 7u, Dio_active_low,  Dio_interface_function),
	DEF_DIG_OP(vtbm_can_enbl_hs,  Dio_port_b, 7u, Dio_active_low,  Dio_interface_direct),

    /* Ports C & D don't exist */

    /** Port E - Outputs */
/*             var root           port        bit polarity         interface  */
    /* defintion of vtbm_acquisition must match TIMING_MONITOR_PORT and
     * TIMING_MONITOR_MASK */	
    //DEF_DIG_OP(vtbm_acquisition,  Dio_port_e, 2u, Dio_active_high, Dio_interface_direct),


/*Linkedriving configured for new BMS*/
    DEF_DIG_OP(d_dac_ctrl_2,  Dio_port_e, 2u, Dio_active_low, Dio_interface_direct),
    DEF_DIG_OP(led_pe_output_1,   Dio_port_e, 3u, Dio_active_high, Dio_interface_function),
    //DEF_DIG_OP(vtbm_gnd_enbl_ls,  Dio_port_e, 4u, Dio_active_high, Dio_interface_function),
	DEF_DIG_OP(vtbm_gnd_enbl_ls,  Dio_port_e, 4u, Dio_active_high, Dio_interface_direct),
    DEF_DIG_OP(chg_dc_neg_ls_mmc,  Dio_port_e, 7u, Dio_active_high, Dio_interface_function),
   

    /* Ports F & G don't exist */

    /** Port H - Outputs */
    /* bits 0..3 are EEPROM SPI */
/*             var root           port        bit polarity         interface  */
    //DEF_DIG_OP(pre_chg_hs_enbl,   Dio_port_h, 4u, Dio_active_high, Dio_interface_function),
    //DEF_DIG_OP(veh_dc_neg_ls_mmc, Dio_port_h, 5u, Dio_active_high, Dio_interface_function),
    //DEF_DIG_OP(chg_dc_neg_ls_mmc, Dio_port_h, 6u, Dio_active_high, Dio_interface_function),
    //DEF_DIG_OP(vtbm_can_enbl_ls,  Dio_port_h, 7u, Dio_active_high, Dio_interface_function),

    /* Port I doesn't exist */

    /** Port J - Outputs */
/*             var root           port        bit polarity         interface  */
    DEF_DIG_OP(led_pj_output_2,   Dio_port_j, 1u, Dio_active_high, Dio_interface_function),
    /* bits 6..7 are RTC I2C */

    /** Port K - Outputs */
    DEF_DIG_OP(ana_mux_a0,        Dio_port_k, 0u, Dio_active_high, Dio_interface_direct),
    DEF_DIG_OP(ana_mux_a1,        Dio_port_k, 1u, Dio_active_high, Dio_interface_direct),
    DEF_DIG_OP(ana_mux_a2,        Dio_port_k, 2u, Dio_active_high, Dio_interface_direct),
    DEF_DIG_OP(ana_mux_1_en,      Dio_port_k, 3u, Dio_active_high, Dio_interface_direct),
    DEF_DIG_OP(psu_ltch_str,      Dio_port_k, 5u, Dio_active_high, Dio_interface_function),

    /* Port L doesn't exist */

    /* Port M has no outputs (it is 4 CAN ports) */

    /* Ports N & O don't exist */

/** @todo Some PortP lines may be PWM - this may affect how they need to appear here (at the moment they are vanilla digital outputs) */
    /** Port P - Outputs */
/*             var root           port        bit polarity         interface  */
    /* bit 0 is unused */
    //DEF_DIG_OP(led_evb_only_3,    Dio_port_p, 2u, Dio_active_high, Dio_interface_function),
    //DEF_DIG_OP(led_evb_only_3,    Dio_port_p, 0u, Dio_active_high, Dio_interface_function),
    //DEF_DIG_OP(veh_dc_pos_ls_mmc, Dio_port_p, 4u, Dio_active_high, Dio_interface_function),
    //DEF_DIG_OP(spare_output_hs,   Dio_port_p, 5u, Dio_active_high, Dio_interface_function),
    //DEF_DIG_OP(chg_dc_pos_ls_mmc, Dio_port_p, 6u, Dio_active_high, Dio_interface_function),
    //DEF_DIG_OP(spare_output_ls,   Dio_port_p, 7u, Dio_active_high, Dio_interface_function),
    
    /** Port S outputs */
    //DEF_DIG_OP(veh_dc_pos_hs_mmc, Dio_port_s, 0u, Dio_active_high, Dio_interface_function),
    //DEF_DIG_OP(veh_dc_neg_hs_mmc, Dio_port_s, 1u, Dio_active_high, Dio_interface_function),
    //DEF_DIG_OP(chg_dc_pos_hs_mmc, Dio_port_s, 2u, Dio_active_high, Dio_interface_function),
    //DEF_DIG_OP(en_LIN, Dio_port_s, 4u, Dio_active_high, Dio_interface_function),
    /* bits 4 and 6 are PSU SPI, bits 5 and 7 are not used */
    /* bit 7 is unused */

    /** Port T - Outputs */
/*             var root           port        bit polarity         interface  */

/*Linkedriving configured for a new BMS*/
    DEF_DIG_OP(wdi_output,  Dio_port_t, 0u, Dio_active_high, Dio_interface_direct),
    //DEF_DIG_OP(spare_output_hs,  Dio_port_t, 2u, Dio_active_high, Dio_interface_direct),
    /* bits 1,2,4..7 are inputs */
	
    DEF_DIG_OP(ana_mux_2_en, Dio_port_t, 3u, Dio_active_high, Dio_interface_direct),
    DEF_DIG_OP(hbrige_3, Dio_port_t, 4u, Dio_active_high, Dio_interface_function),
    DEF_DIG_OP(hbrige_4, Dio_port_t, 5u, Dio_active_high, Dio_interface_function),
    DEF_DIG_OP(hbrige_1, Dio_port_t, 6u, Dio_active_high, Dio_interface_function),
    DEF_DIG_OP(hbrige_2, Dio_port_t, 7u, Dio_active_high, Dio_interface_function),
    //{ &di_input_1,                  Dio_port_t, 6u, Dio_active_low,  Dio_interface_function },
   // { &di_input_2,                  Dio_port_t, 7u, Dio_active_low,  Dio_interface_function },
    /* bits 1,2,4..7 are inputs */
};
/**<
 * @a2l{[].port, LongIdentifier, I/O port}
 * @a2l{[].bitnum, conversion, uint8_t}
 * @a2l{[].bitnum, LongIdentifier, pin number of I/O port (range 0 to 7)}
 * @a2l{[].interface, LongIdentifier, direct or via dio_read_inputs() dio_write_outputs()}
 */

/* **************** PULL RESISTOR  C O N F I G U R A T I O N **************** */

CONST struct dio_pull_cfg_s dio_pull_cfg[] =
{
    /*  port name   bitmask     pull resistor type*/
    {   Dio_port_a, 0xffu,      Dio_pull_default},
    {   Dio_port_b, 0xffu,      Dio_pull_default},
    {   Dio_port_e, 0xffu,      Dio_pull_default},
    {   Dio_port_h, 0xffu,      Dio_pull_default},
    {   Dio_port_j, 0xffu,      Dio_pull_default},
    {   Dio_port_k, 0xffu,      Dio_pull_default},
    {   Dio_port_m, 0xffu,      Dio_pull_default},
    {   Dio_port_p, 0xffu,      Dio_pull_default},
    {   Dio_port_s, 0xffu,      Dio_pull_default},
    {   Dio_port_s, 0x10u,      Dio_pull_none},
    {   Dio_port_t, 0xffu,      Dio_pull_default},
};
/**<
 * @a2l{[].port, LongIdentifier, I/O port}
 * @a2l{[].bitmask, conversion, uint8_t}
 * @a2l{[].bitmask, LongIdentifier, pin bit mask of I/O port to be configured}
 * @a2l{[].pull_cfg, LongIdentifier, pull resistor type configuration}
 */

CONST uint8_t dio_n_inputs =
        (uint8_t) (sizeof(dio_input_cfg)/sizeof(struct dio_in_cfg_s));
/**<
 * @a2l{conversion, uint8_t}
 */

CONST uint8_t dio_n_outputs =
        (uint8_t) (sizeof(dio_output_cfg)/sizeof(struct dio_out_cfg_s));
/**<
 * @a2l{conversion, uint8_t}
 */

CONST uint8_t dio_n_pull_cfgs =
        (uint8_t) (sizeof(dio_pull_cfg)/sizeof(struct dio_pull_cfg_s));
/**<
 * @a2l{conversion, uint8_t}
 */
