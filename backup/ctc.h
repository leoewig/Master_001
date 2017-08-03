#if !defined(CTC_H)
#define      CTC_H
/**
 *
 * Public header for the contactor control feature
 *
 * @file
 * @ingroup ctc
 * $Id: ctc.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2012-2015 - All rights reserved */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "basic_types.h"
#include "mmc.h"
#include "ctc_types.h"

/**
 * Contactor Control
 *
 * ## Overview
 * The contactor control feature performs the appropriate combination of steps
 * and checks required in order to properly open and close contactors.  The inputs
 * are demands to close either the DCLink or charger contactors (or to have both
 * sets open). The situation where a request to close both is asserted is treated
 * as an internal error.
 *
 * The opening/closing of the contactors will be implemented as a relatively simple,
 * if large, state machine.
 *
 * @defgroup ctc CTC Contactor Control
 */

/** Position of precharge contactor */
extern enum Ctc_cont_state_e ctc_precharge_contactor_closed;

/** Position of DCLink contactors */
extern enum Ctc_cont_state_e ctc_dclink_contactors_closed;

/** Validity indicator of the DCLink contactors position state */
extern data_validity_e       ctc_dclink_contactors_closed_validity;

/** Position of DCLink Pos contactor */
extern enum Ctc_cont_state_e ctc_dclink_pos_contactor_closed;

/** Validity indicator of the DCLink positive contactor position state */
extern data_validity_e       ctc_dclink_pos_contactor_closed_validity;

/** Position of DCLink Neg contactor */
extern enum Ctc_cont_state_e ctc_dclink_neg_contactor_closed;

/** Validity indicator of the DCLink negative contactor position state */
extern data_validity_e       ctc_dclink_neg_contactor_closed_validity;

/** DCLink demand */
extern enum Ctc_cont_state_e ctc_dclink_contactor_command;

/** Position of charger contactors */
extern enum Ctc_cont_state_e ctc_charger_contactors_closed;

/** Validity indicator of the charger contactors position state */
extern data_validity_e       ctc_charger_contactors_closed_validity;

/** Position of Charger Pos contactor */
extern enum Ctc_cont_state_e ctc_charger_pos_contactor_closed;

/** Validity indicator of the charger positive contactor position state */
extern data_validity_e       ctc_charger_pos_contactor_closed_validity;

/** Position of Charger Neg contactor */
extern enum Ctc_cont_state_e ctc_charger_neg_contactor_closed;

/** Validity indicator of the charger negative contactor position state */
extern data_validity_e       ctc_charger_neg_contactor_closed_validity;

/** Charger demand */
extern enum Ctc_cont_state_e ctc_charger_contactor_command;

extern CAL ts_time_t         ctc_cal_dclink_pre_close_time_ms;
extern CAL ts_time_t         ctc_cal_dclink_pos_close_time_ms;
extern CAL ts_time_t         ctc_cal_dclink_pos_wait_time_ms;
extern CAL ts_time_t         ctc_cal_dclink_neg_close_time_ms;
extern CAL ts_time_t         ctc_cal_dclink_neg_wait_time_ms;
extern CAL current_t         ctc_cal_dclink_drv_pre_close_max_i_mA;
extern CAL current_t         ctc_cal_dclink_drv_pre_close_min_i_mA;
extern CAL current_t         ctc_cal_dclink_drv_pos_close_min_i_mA;
extern CAL current_t         ctc_cal_dclink_drv_pos_close_max_i_mA;
extern CAL current_t         ctc_cal_dclink_drv_neg_close_min_i_mA;
extern CAL current_t         ctc_cal_dclink_drv_neg_close_max_i_mA;
extern CAL current_t         ctc_cal_dclink_pos_close_max_dischg_i_mA;
extern CAL current_t         ctc_cal_dclink_neg_close_max_dischg_i_mA;
extern CAL percent_t         ctc_cal_dclink_pos_volts_tolerance_pc;
extern CAL percent_t         ctc_cal_dclink_neg_volts_tolerance_pc;

extern CAL ts_time_t         ctc_cal_charger_pre_close_time_ms;
extern CAL ts_time_t         ctc_cal_charger_pos_close_time_ms;
extern CAL ts_time_t         ctc_cal_charger_pos_wait_time_ms;
extern CAL ts_time_t         ctc_cal_charger_neg_wait_time_ms;
extern CAL current_t         ctc_cal_charger_drv_pre_close_i_nominal_mA;
extern CAL current_t         ctc_cal_charger_drv_pre_close_i_tolerance_mA;
extern CAL current_t         ctc_cal_charger_drv_pos_close_i_nominal_mA;
extern CAL current_t         ctc_cal_charger_drv_pos_close_i_tolerance_mA;
extern CAL current_t         ctc_cal_charger_drv_neg_close_i_nominal_mA;
extern CAL current_t         ctc_cal_charger_drv_neg_close_i_tolerance_mA;
extern CAL current_t         ctc_cal_charger_pos_close_i_nominal_mA;
extern CAL current_t         ctc_cal_charger_neg_close_i_nominal_mA;
extern CAL current_t         ctc_cal_charger_dclink_pos_close_i_nominal_mA;
extern CAL current_t         ctc_cal_charger_dclink_neg_close_i_nominal_mA;
extern CAL percent_t         ctc_cal_charger_pos_volts_tolerance_pc;
extern CAL percent_t         ctc_cal_charger_neg_volts_tolerance_pc;

extern CAL supply_v_t        ctc_cal_open_state_v_lower_limit;
extern CAL supply_v_t        ctc_cal_open_state_v_upper_limit;
extern CAL supply_v_t        ctc_cal_closed_state_v_lower_limit;
extern CAL supply_v_t        ctc_cal_closed_state_v_upper_limit;
extern CAL supply_v_t        ctc_cal_feedback_state_tolerance_v;
extern CAL ctc_fb_sig_calibration_t ctc_cal_fb_sig_scaling[Ctc_fb_sig_count];

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/**************************************************************************//**
 * Initialisation function for Contactor Control, to be called at startup
 *****************************************************************************/
FCODE void ctc_init (void);

/**************************************************************************//**
 * Inputs update function for CTC
 *
 * - Reads the current HS/LS contactor positions
 * - Determines if the contactor is closed from the HS/LS positions
 *****************************************************************************/
FCODE void ctc_read_inputs (void);

extern void ctc_ss_data_sequence_state(enum Ctc_sequence_e *seq, 
                                uint16_t *seq_idx, 
                                bool_t *in_fast_close);


extern pack_v_t ctc_ss_data_v_prior_precharge(void);



/**************************************************************************//**
 * Update state machine for CTC
 *****************************************************************************/
FCODE void ctc_update (void);

#endif      /* CTC_H */

