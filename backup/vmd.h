/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     vmd.h
 *
 * description:   Public header file for the vmd feature
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Public header file for the vmd feature
 *                       
 *
 *****************************************************************************/
#if !defined(VMD_H)
#define      VMD_H

/**
 * This module turns the (processed) inputs - obtained from the vehicle
 * interface - into an operating mode request, which is passed to the main
 * state control. This is a request for the BMS to move into a mode where
 * the vehicle contactors will be closed.
 *
 * Allow vehicle contactor closing indicates that this feature would request a
 * transition to the Vehicle Active mode if the Arbitrated HCU request indicated
 * that vehicle contactors should be closed.
 *
 * Vehicle contactor open pending is an indication that that this feature has
 * determined (either due to input arbitration or the presence of a fault) that
 * contactors will be opened, this allows an opportunity for current flow to
 * cease before contactors are opened.
 *
 * @defgroup vmd VMD Vehicle Mode Determination
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "vmd_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern CAL ts_time_t vmd_afterrun_bal_timeout_ms;
extern CAL ts_time_t vmd_force_open_delay;
extern CAL current_t vmd_safe_i_opening_threshold;
extern CAL ts_time_t vmd_i_high_grace_period_ms;
extern CAL current_t vmd_i_gross_overload_threshold;
extern CAL ts_time_t vmd_main_fuse_blow_period_ms;
extern CAL percent_t vmd_overvoltage_threshold;
extern CAL ts_time_t vmd_contactors_closed_ignition_off_timeout;
extern CAL ts_time_t vmd_immobilisation_wait_timeout;

extern bool_t vmd_afterrun_bal_in_progress;
extern bool_t vmd_error_mode;
extern bool_t vmd_veh_can_required;
extern bool_t vmd_veh_close_allow;
extern bool_t vmd_veh_indicate_close_allow;
extern bool_t vmd_veh_demand_present;
extern bool_t vmd_veh_mode_req;
extern bool_t vmd_veh_contactor_open_pending;
extern bool_t vmd_imm_indicate_tethered;
extern bool_t vmd_cell_management_run;
extern bool_t vmd_ign_last;
extern bool_t vmd_charge_cycle_inhibit;
extern enum Vmd_override_type_e vmd_override_type;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 *  Module initialisation function, to be called at startup
 */
void vmd_init(void);

/**
 *  Update function, to be called once per task in order to process its data
 *  Must be called before ms.
 */
void vmd_update(void);

#endif      /* VMD_H */

