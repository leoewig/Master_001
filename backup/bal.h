/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    bal.h
 *
 * description:  
 * Balancing feature public header
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
#if !defined(BAL_H)
#define      BAL_H
/**
 *
 * Balancing feature public header
 *
 * @file
 * @ingroup bal
 */

/* Copyright (c) Linkedriving Tech 2015 - All rights resevered */

/**
 * This feature determines the balancing demand to be transmitted to VTBMs.
 *
 * The aim of the balancing algorithm is to reduce the spread of level of
 * charge between cells in the pack. Levelling down is more straight forward
 * than levelling up and, since active balancing affects more cells and
 * involves larger currents than passive balancing, active balancing is the
 * most effective means of lowering the charge of cells. The essence of the
 * algorithm is therefore to use active balancing to reduce the charge in cells
 * unless so doing would either make the receiving cell the highest charged
 * cell in the pack, or would make a supplying cell the lowest charged cell in
 * the pack.
 *
 * ## Approach used 
 * The need for balancing is determined by the estimate of the charge in each
 * cell. This estimate is made periodically by the cell management features
 * along with a mechanism for indicating that an estimate has been made.
 *
 * Two styles of balancing are supported:
 * + Charge correction to correct charge imbalances within the pack
 * + Charge maintenance to prevent imbalances caused by the different numbers of
 *      cells attached to VTBMs from getting worse.
 *
 * Charge correction is enabled for a calibratable time after each charge
 * estimation, after which charge maintenance is performed indefinitely.  The
 * logic to switch between charge correction and charge maintenance is done
 * by the UMC unmetered charge feature.
 *
 * ## Design Details 
 * The balancing variables are initialised by the bal_init() function.
 *
 * The balancing activities are performed by the bal_update() function. This is
 * a periodic function, called once per background tick. It calls the following:
 * + bal_check_chg_estmn() to check whether a new charge estmation has been
 *      done and perform the related activities,
 * + bal_determine_balancing_status() to determine the balancing status,
 * + Call umc_update() to calculate charge flows over the last cycle and
 *      determine whether a new balancing phase has started.
 *
 * If a new phase has started, 
 * + bal_calculate_mode_dmd() is called to calculate the required balancing
 *
 * + bal_update_mdm_data() makes the results of the balancing calculation
 *      available to the foreground.
 *
 * ## Limitations #
 * Since this feature runs in the background task, it cannot directly access any
 * data present in the foreground task. If it has to access any such data, it
 * must use mdm. Likewise, if any of the features that run in the foreground task,
 * wants to access the data present in this feature, they must use mdm.
 *
 * ## How to use #
 * This feature must be called in the background task after the cell management
 * features and the umc post-cell management function.
 *
 * ## Configuration #
 * NIL
 *
 * ## Requirement Satisfaction #
 * The requirements for this feature can be found in the
 * <a href="http://viewvc/viewvc.cgi/trunk/docs/reqs/generic/RU_BMS_Balancing.doc?root=Q52717_NightWing&view=co">
 * Balancing SRS Section 4 Balancing Requirements</a> document.
 *
 * | Requirement             | Satisfied By       | Unit Tested By | PAT Tested by |
 * | ------------------------|--------------------|----------------|---------------|
 * | REQ_BMS_BAL_ENTRY_001   | bal_init(), aps_bg_init() |         |               | 
 * | REQ_BMS_BAL_ENTRY_002   | umc_ram_image.data, bal_init_chg_estimate_timer() | | | 
 * | REQ_BMS_BAL_PERIODIC_001 | bal_update(), mmc_aps.c -> bg_task |    |          | 
 * | REQ_BMS_BAL_PERIODIC_002 | bal_determine_balancing_status() |      |          | 
 * | REQ_BMS_BAL_PERIODIC_003 | bal_calculate_mode_dmd() |      |               | 
 * | REQ_BMS_BAL_PERIODIC_004 | bal_calculate_mode_dmd() |      |               | 
 * | REQ_BMS_BAL_INP_001      | UMC_NV(cell_charge_C) |             |               | 
 * | REQ_BMS_BAL_INP_002      | #pcd_ctr_packChargeEst |             |               | 
 * | REQ_BMS_BAL_INP_003      | #dcd_flg_inhibitBalancing |            |               | 
 * | REQ_BMS_BAL_INP_004      | #mdmd_enum_cellVoltValid |               |               | 
 * | REQ_BMS_BAL_INP_005      | #mdmv_V_packCellTerm_V |          |               | 
 * | REQ_BMS_BAL_INP_006      | #mdmd_enum_cellVoltValid |               |               | 
 * | REQ_BMS_BAL_INP_007      | mdmv_t_cellSkinMeas_dC[MDM_MIN_IDX]  |   |               | 
 * | REQ_BMS_BAL_INP_008      | #VMI_12V_LOGIC_V |           |               | 
 * | REQ_BMS_BAL_INP_009      | #ms_balancing_enabled |      |               | 
 * | REQ_BMS_BAL_CAL_001      | #bal_enable_balancing |            |               | 
 * | REQ_BMS_BAL_CAL_002      | #bal_chg_tolerance_C |         |               | 
 * | REQ_BMS_BAL_CAL_004      | #bal_min_cell_v     |      |               | 
 * | REQ_BMS_BAL_CAL_005      | #bal_max_cell_v     |      |               | 
 * | REQ_BMS_BAL_CAL_007      | #bal_min_cell_t   |      |               | 
 * | REQ_BMS_BAL_CAL_008      | #bal_12v_batt_limit | | |
 * | REQ_BMS_BAL_CAL_009      | #bal_12v_batt_limit | | |
 * | REQ_BMS_BAL_NV_001       | umc_ram_image.data.is_chg_estimated | | |
 * | REQ_BMS_BAL_NV_002       | umc_ram_image.data.is_chg_estimated | | |
 * | REQ_BMS_BAL_NV_003       | bal_init_chg_estimate_timer() | | |
 * | REQ_BMS_BAL_NV_006       | umc_ram_image.data.chg_estmn_tmr_ms | | |
 * | REQ_BMS_BAL_NV_007       | umc_ram_image.data.chg_estmn_tmr_ms | | |
 * | REQ_BMS_BAL_NV_008       | umc_ram_image.data.chg_estmn_tmr_ms | | |
 * | REQ_BMS_BAL_NV_009       | bal_init_chg_estimate_timer()| | |
 * | REQ_BMS_BAL_NV_010       | bal_update_estimation_timer() | | |
 * | REQ_BMS_BAL_NV_011       | bal_update_estimation_timer() | | |
 * | REQ_BMS_BAL_NV_012       | bal_new_chg_estimation(), bal_update_estimation_timer() | | |
 * | REQ_BMS_BAL_STATUS_001   | #Bal_status_e, #bal_status | | |
 * | REQ_BMS_BAL_STATUS_002   | #bal_status | | |
 * | REQ_BMS_BAL_STATUS_003   | bal_determine_balancing_status(), bal_disabled_check() | | |
 * | REQ_BMS_BAL_STATUS_004   | bal_determine_balancing_status(), bal_inactive_check() | | |
 * | REQ_BMS_BAL_STATUS_005   | bal_determine_balancing_status(), bal_halted_check() | | |
 * | REQ_BMS_BAL_STATUS_006   | bal_determine_balancing_status(), bal_halted_check() | | |
 * | REQ_BMS_BAL_STATUS_007   | bal_determine_balancing_status(), bal_halted_check() | | |
 * | REQ_BMS_BAL_STATUS_009   | bal_determine_balancing_status(), bal_halted_check() | | |
 * | REQ_BMS_BAL_STATUS_010   | bal_determine_balancing_status(), bal_halted_check() | | |
 * | REQ_BMS_BAL_STATUS_011   | bal_determine_balancing_status(), bal_halted_check(), bal_12v_too_low() | | |
 * | REQ_BMS_BAL_STATUS_012   | bal_determine_balancing_status(), bal_halted_check(), bal_12v_too_low()| | |
 * | REQ_BMS_BAL_STATUS_013   | bal_determine_balancing_status(), bal_halted_check() | | |
 * | REQ_BMS_BAL_STATUS_008   | bal_determine_balancing_status() | | |
 * | REQ_BMS_BAL_PHASE_001    | bal_new_phase_check() | | |
 * | REQ_BMS_BAL_PHASE_004    | bal_new_phase_check() | | |
 * | REQ_BMS_BAL_PHASE_005    | bal_new_phase_check() | | |
 * | REQ_BMS_BAL_MODE_001     | #bal_mode_dmd[] | | |
 * | REQ_BMS_BAL_MODE_003     | #bal_dmd_cell[] | | |
 * | REQ_BMS_BAL_ACTIVE_001   | bal_set_mode_dmd_vtbm() | | |
 * | REQ_BMS_BAL_ACTIVE_002   | bal_can_receive_active() | | |
 * | REQ_BMS_BAL_ACTIVE_003   | bal_can_receive_active() | | |
 * | REQ_BMS_BAL_ACTIVE_004   | bal_can_supply_active() | | |
 * | REQ_BMS_BAL_ACTIVE_005   | bal_can_supply_active() | | |
 * | REQ_BMS_BAL_ACTIVE_006   | bal_set_mode_dmd_vtbm() | | |
 * | REQ_BMS_BAL_PASSIVE_001  | bal_set_mode_dmd_vtbm() | | |
 * | REQ_BMS_BAL_PASSIVE_002  | bal_set_mode_dmd_vtbm() | | |
 * | REQ_BMS_BAL_PASSIVE_003  | bal_can_receive_passive() | | |
 * | REQ_BMS_BAL_PASSIVE_004  | bal_can_receive_passive() | | |
 * | REQ_BMS_BAL_PASSIVE_005  | bal_set_mode_dmd_vtbm() | | |
 *
 * @defgroup bal BAL Balancing
 * @ingroup bgcalc
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "bal_types.h"
#include "mdm_types.h"  /* For MAX_NUM_VTBM */
#include "hardware.h"   /* For INTERNAL_ERROR */

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
/* Documented in source */
extern FRAM Bal_status_e bal_status;
extern FRAM enum BalAction_e bal_mode_dmd[MAX_NUM_VTBM];
extern FRAM cellcount_t bal_dmd_cell[MAX_NUM_VTBM];

extern CAL float32_t bal_chg_tolerance_C ;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/* Documented in source */
void bal_init(void);

/* Documented in source */
void bal_update(void);

void bal_store_halted_reason(void);

#endif      /* BAL_H */
