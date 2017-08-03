/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  ctc_p.h
 *
 * description:  
 * Contactor control private header
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
#if !defined(CTC_P_H)
#define      CTC_P_H
/**
 *
 * Contactor control private header
 *
 * @file
 * @ingroup ctc
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/*
 * enumerations and macro definitioms made available must be documented here to
 * avoid duplication.
 *
 * PRIVATE functions are documented in the source file.
 * Non-PRIVATE variables and functions (i.e. those shared between files in the
 * feature) are documented here.
 */
#include "ctc_types.h"


/******************************************************************************
 * CALIBRATIONS
 *****************************************************************************/



/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

extern enum Ctc_cont_state_e ctc_cont_veh_psn_expect;
extern enum Ctc_cont_state_e ctc_cont_chg_psn_expect;
extern enum Ctc_sequence_e ctc_sequence;

extern enum Ctc_cont_state_e ctc_chg_delayed_neg_dmd; /**< Delayed version of ctc_chg_neg_dmd used in contactor closing allowed logic */
extern enum Ctc_cont_state_e ctc_chg_delayed_pos_dmd; /**< Delayed version of ctc_chg_pos_dmd used in contactor closing allowed logic */
extern enum Ctc_cont_state_e ctc_cont_chg_psn_dmd;
extern FRAM ctc_nvs_wrapper_s ctc_ram_image;

extern enum Ctc_cont_state_e ctc_veh_delayed_neg_dmd; /**< Delayed version of ctc_veh_neg_dmd used in contactor closing allowed logic */
extern enum Ctc_cont_state_e ctc_veh_delayed_pos_dmd; /**< Delayed version of ctc_veh_pos_dmd used in contactor closing allowed logic */
extern enum Ctc_cont_state_e ctc_veh_delayed_prechg_dmd; /**< Delayed version of ctc_prechg_dmd used in contactor closing allowed logic  */

extern bool_t ctc_allows_veh_closed; /**< Closing of vehicle contactors allowed */
extern bool_t ctc_allows_veh_closed_fast;

extern ts_time_t ctc_event_time;
extern current_t ctc_i_precharge;
/**<
 * @a2l{conversion,current_t}
*/
extern current_t ctc_i_veh_neg;
/**<
 * @a2l{conversion,current_t}
*/
extern current_t ctc_i_veh_pos;
/**<
 * @a2l{conversion,current_t}
*/
extern current_t ctc_i_chg_neg;
/**<
 * @a2l{conversion,current_t}
*/
extern current_t ctc_i_chg_pos;
/**<
 * @a2l{conversion,current_t}
*/

extern bool_t       ctc_in_fast_close;
/**
 * contains the last measurement that failed during closure */
extern ctc_measure_enum ctc_last_measurement_fault ;

extern CAL energy_t ctc_max_prechg_res_store_mj;
extern CAL energy_t ctc_prechg_hi_thold_mj;
extern CAL energy_t ctc_prechg_dissipate_hi_mj;
extern CAL energy_t ctc_prechg_dissipate_lo_mj;

/*---------- 'overall' contactor position for each bus ----------*/
extern enum Ctc_cont_state_e ctc_cont_veh_psn_dmd;

extern const struct ctc_sequence_step ctc_MainOpen_Seq[];


/*---------- Charger status as demanded by ctc ------------------*/

extern enum Ctc_cont_state_e ctc_chg_neg_ls_dmd;
/**<
 * @a2l{LongIdentifier,MMC low-side demand charger negative contactor}
 */
extern enum Ctc_cont_state_e ctc_chg_neg_hs_dmd;
/**<
 * @a2l{LongIdentifier,MMC high-side demand charger negative contactor}
 */

/* MMC contactor outputs for charger bus */
extern enum Ctc_cont_state_e ctc_chg_pos_ls_dmd;
/**<
 * @a2l{LongIdentifier,MMC low-side demand charger positive contactor}
 */
/* MMC contactor outputs for charger bus */
extern enum Ctc_cont_state_e ctc_chg_pos_hs_dmd;
/**<
 * @a2l{LongIdentifier,MMC high-side demand charger positive contactor}
 */

/*---------- MMC High-side contactor outputs for Pos DC-link vehicle bus -----*/
extern enum Ctc_cont_state_e ctc_veh_pos_HS_dmd;
/**<
 * @a2l{LongIdentifier,MMC High-Side demand for Vehicle HV+}
 */
/*---------- MMC Low-side contactor outputs for Pos DC-link vehicle bus ------*/
extern enum Ctc_cont_state_e ctc_veh_pos_LS_dmd;
/**<
 * @a2l{LongIdentifier,MMC Low-Side demand for Vehicle HV+}
 */
/*---------- MMC High-side contactor outputs for neg DC-link vehicle bus -----*/
extern enum Ctc_cont_state_e ctc_veh_neg_HS_dmd;
/**<
 * @a2l{LongIdentifier,MMC High-Side demand for Vehicle HV-}
 */
/*---------- MMC Low-side contactor outputs for neg DC-link vehicle bus ------*/
extern enum Ctc_cont_state_e ctc_veh_neg_LS_dmd;
/**<
 * @a2l{LongIdentifier,MMC Low-Side output for Vehicle HV-}
 */

/*---------- MMC high-side contactor outputs for precharger -----*/
extern enum Ctc_cont_state_e ctc_prechg_HS_dmd;
/**<
 * @a2l{LongIdentifier,MMC High-side demand Vehicle Precharge}
 */
/*---------- MMC low-side contactor outputs for precharger ------*/
extern enum Ctc_cont_state_e ctc_prechg_LS_dmd;
/**<
 * @a2l{LongIdentifier,MMC Low-side demand Vehicle Precharge}
 */



/* Contactor closing prevented by high precharge energy */

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

/* Contactor state variables */

extern ts_time_t ctc_last_read_update;
extern int32_t   ctc_delay_counter;
/**<
 * @a2l{conversion,ts_time_t}
 */
extern ts_time_t ctc_update_interval_ms; /**< time between intervals in ms */

extern bool_t ctc_sequence_fault;
extern ctc_measure_enum ctc_seq_last_measurement_fault[(int8_t)(Ctc_seq_complete)];

/* Currently selected contractor sequence */
extern bool_t ctc_chg_fault; /**< @mantis{4747} Implement or find something else that does this. Indicates a fault relating to the charger contactors. Required for REQ_BMS_CHRG_CON_010 */


/******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *****************************************************************************/

#endif      /* CTC_P_H */

