/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    icm_data_p.h
 *
 * description:  
 * Internal comms data processing private header
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(ICM_DATA_P_H)
#define      ICM_DATA_P_H
/**
 *
 * Internal comms data processing private header
 *
 * @file
 * @ingroup icm_data
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/*****************************************************************************
 * MACROS
 ****************************************************************************/

/*****************************************************************************
 * PRIVATE DATA FOR UNIT TEST
 ****************************************************************************/

#if defined(UNIT_TEST) && !defined(_lint)

/*****************************************************************************
 * Cal items
 ****************************************************************************/
extern CAL ts_time_t icm_ref_measure_timeout;
extern CAL ts_time_t icm_netcmd_timeout;
extern CAL ts_time_t icm_cell_measure_timeout;
extern CAL ts_time_t icm_config_timeout;
extern CAL uint8_t icm_resp_delay_step;
extern CAL uint8_t icm_resp_delay_add;
extern CAL cell_v_t icm_min_vactive_ref_v;
extern CAL cell_v_t icm_min_vpassive_ref_v;
extern CAL cell_v_t icm_max_vpassive_ref_v;
extern CAL cell_v_t icm_max_gnd_ref_v;
extern CAL cell_v_t icm_ref_tol;
extern CAL enum IcmTempSens_e icm_vtbm_temp_sens_type;

/*****************************************************************************
 * Network control/Status response
 ****************************************************************************/

extern uint8_t              icm_netctl_counter;

/*****************************************************************************
 * Pack config
 ****************************************************************************/


extern FRAM uint8_t icm_pack_config[MAX_CONFIG_SIZE];

/*****************************************************************************
 * Reference measurement
 ****************************************************************************/

extern bool_t      icm_all_refs_meas_once;
extern struct icm_measctl_s icm_measctl_cmd;

#endif /*  defined(UNIT_TEST) && !defined(_lint) */

/*****************************************************************************
 * Config creation
 ****************************************************************************/

PRIVATE void    icm_build_config(void);
PRIVATE void    icm_build_config_header(void);
PRIVATE void    icm_build_config_body(void);
PRIVATE uint8_t icm_packed_sig_count(uint8_t count);
PRIVATE uint8_t icm_packed_vtbm_cfg(uint8_t vtbm_num);
PRIVATE void    icm_build_config_crc(void);

/*****************************************************************************
 * Reference measurement functions
 ****************************************************************************/

PRIVATE enum IcmTransmit_e  icm_tx_meas_refs(void);
PRIVATE void icm_prepare_refdata_measure(void);
PRIVATE void icm_clear_rx_ref_info(void);
PRIVATE bool_t icm_all_refs_received(void);
PRIVATE void icm_mark_missing_refs(void);
PRIVATE void icm_validate_refs(void);

/*****************************************************************************
 * cell measurement functions
 ****************************************************************************/

PRIVATE enum IcmTransmit_e  icm_tx_meas_cfg_cells(void);
PRIVATE void icm_setup_measure_none(struct icm_measctl_s *measctl_p);
PRIVATE void icm_set_meas_expect_count(enum MeasTarg_e targ);
PRIVATE cellcount_t icm_expected_cell_frames(uint8_t nvtbm, enum MeasTarg_e targ);
PRIVATE void icm_prepare_meas_info(void);
PRIVATE void icm_reset_seen_cellframes(void);
PRIVATE void icm_check_measure_errors(bool_t connecting);

/*****************************************************************************
 * Balancing functions
 ****************************************************************************/

PRIVATE void icm_determine_bal_output(void);
PRIVATE void icm_transfer_bal_output(void);
PRIVATE void icm_set_disable_bal(void);

/*****************************************************************************
 * Status functions
 ****************************************************************************/




PRIVATE bool_t  icm_validate_ref_values(uint8_t vtbm_num);
PRIVATE void    icm_process_completed_measure(bool_t connecting);

/*****************************************************************************
 * Misc functions
 ****************************************************************************/

PRIVATE void    icm_inc_cycle(uint8_t *cyclep);
PRIVATE void    icm_set_goffsets(void);

#endif      /* ICM_DATA_P_H */

