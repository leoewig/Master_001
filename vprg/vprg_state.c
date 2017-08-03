/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:       vprg_state.c
 *
 * description:  
 * VTBM reprogramming source
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/


#include "basic_types.h"
#include "hardware.h"
#include "flash_eee.h"
#include "ts.h"
//#include "icm.h"
//#include "pl.h"                 /* for pl_num_vtbms */
#include "ixcp.h"               /* for ixcp_prog_allowed() */
//#include "ican.h"               /* for CAN_MAX_DATA */
#include "exc.h"                /* for ApplXcpSend() */
#include "ms.h"
#include "vprg.h"
#include "vprg_p.h"
#include "nvs.h"
#include "fm.h"

#define MEMORY_LOCATION S12_MEMORY
#include "memory_layout.h"


/******************************************************************************
 * COMPILE-TIME CHECKS
 *****************************************************************************/

/*
 * PUBLIC DATA (documented in header)
 */
bool_t vprg_reprog_request = FALSE;

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/


/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/**
 * Updates the user request and calculates the new state (stored in #vprg_state)
 */
void vprg_update(void)
{}

/**
 * Runs VTBM reprogramming idle tasks according to the current main state
 */
void vprg_idle_tasks(void)
{}

/**
 * Receives an XCP User Command from the CANbedded XCP slave
 */
void vprg_xcp_user_command(FPTR const uint8_t *data)
{}

/**
 * Request from the UDS parser to start automatic VTBM reprogramming.
 *
 * @return TRUE     request has been successfully placed
 * @return FALSE    request has been rejected
 */
bool_t vprg_uds_vtbm_reprogramming_req(void)
{}

/**
 * Collect current vprg status data to be sent in UDS ResultsVTBM_Reprogramming command
 *
 * @param p_finished        state of vprg uds demand
 * @param p_mask            a mask currently or to be used
 * @param p_image           an image number currently or to be used
 * @param p_latest_error    latest error code occurring during reprogramming
 * @param p_image_status    latest error code occurring during image validation
 * @param p_comms_error     latest error code occurring during communication with vtbm
 */
void vprg_uds_get_status(bool_t *p_finished, uint16_t* p_mask, uint16_t *p_image,
    vprg_error_e *p_latest_error, vprg_ci_state_e *p_image_status, vprg_comms_error_e * p_comms_error)
{}

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/**
 * Fuses the three sources of VTBM re-programming (UDS, XCP, calibration) into
 * a single demand
 */
PRIVATE void vprg_update_demand(vprg_demand_e *demand_p,
                    image_index_t *image_index_p, vprg_vtbm_mask_t *vtbm_mask_p)
{}

/**
 * Interface between diagnostics and VTBM reprogramming.
 *
 * Unlike the xcp and cal vtbm reprogramming, UDS reprogramming is only triggered
 * by the UDS and processes automatically.
 *
 * When vprg_uds_active is found active (a request from the UDS has been placed),
 * perform following steps according to the #vprg_state:
 * - Vprg_state_idle: vprg is ready to start the reprogramming, start the sequence using
 *      parameters found in calibratables #vprg_uds_vtbm_image and #vprg_uds_vtbm_mask,
 *      by demanding #Vprg_dmd_prepare state
 * - Vprg_state_ready_to_program: an image has been successfully checked, start
 *      reprogramming by demanding #Vprg_dmd_program
 * - Vprg_state_error: an error has occurred during programming, recover
 *      vprg feature by demanding #Vprg_dmd_return_to_idle
 * - Vprg_state_programming_complete: programming has been successfully finished,
 *      release #vprg_uds_active, so the next request may be placed
 * -Vprg_state_checking_image, Vprg_state_communicating, Vprg_state_cycle_vtbms,
 * Vprg_state_shutdown_icm: transient states, do nothing
 */
PRIVATE void vprg_update_uds_demand(vprg_demand_e *demand_p,
        image_index_t *image_index_p, vprg_vtbm_mask_t *vtbm_mask_p)
{}

/**
 * Interface between diagnostics and XCP User Command
 */
PRIVATE void vprg_update_xcp_usercmd_demand(vprg_demand_e *demand_p,
                    image_index_t *image_index_p, vprg_vtbm_mask_t *vtbm_mask_p)
{}

/**
 * VTBM re-programming can be controlled via calibration values (with
 * the demand #vprg_demand_cal) but the non-volatility of cals means we must
 * take care to ignore requests left over from a previous power cycle.
 * Therefore, we only take account of the requested demand #vprg_demand_cal if
 * we have previously seen it set to #Vprg_dmd_none.
 */
PRIVATE void vprg_update_cal_demand(vprg_demand_e *demand_p,
                    image_index_t *image_index_p, vprg_vtbm_mask_t *vtbm_mask_p)
{}


/**
 * Fuse the three sources of re-programming request into a single demand and
 * image number.
 *
 * @param cal_demand Action requested by #vprg_demand_cal
 * @param cal_image_index image index from #vprg_image_cal
 * @param cal_vtbm_mask vtbm mask from #vprg_vtbm_mask_cal
 * @param xcp_demand Action requested by XCP User Command
 * @param xcp_image_index image index from XCP User Command
 * @param xcp_vtbm_mask vtbm mask from XCP User Command
 * @param uds_demand Action requested by diagnostics
 * @param uds_image_index image index from diagnostics
 * @param uds_vtbm_mask vtbm mask from diagnostics
 * @param[out] demand_p Pointer through which the resolved demand is written.
 *              This will be #Vprg_dmd_inconsistent if more than one request
 *              input is not #Vprg_dmd_none
 * @param[out] image_index_p Pointer through which requested image index is
 *              written
 * @param[out] vtbm_mask_p Pointer through which requested vtbm mask is written
 */
PRIVATE void vprg_resolve_demand(
                vprg_demand_e cal_demand, image_index_t cal_image_index,
                    vprg_vtbm_mask_t cal_vtbm_mask,
                vprg_demand_e xcp_demand, image_index_t xcp_image_index,
                    vprg_vtbm_mask_t xcp_vtbm_mask,
                vprg_demand_e uds_demand, image_index_t uds_image_index,
                    vprg_vtbm_mask_t uds_vtbm_mask,
                vprg_demand_e *demand_p, image_index_t *image_index_p,
                    vprg_vtbm_mask_t *vtbm_mask_p)
{}

/**
 * Make sure that the demand is a known member of #vprg_demand_e.  This is to
 * ensure #vprg_demand_cal isn't set to nonsense.
 * @returns a valid demand or #Vprg_dmd_unknown
 */
PRIVATE vprg_demand_e vprg_validate_demand(vprg_demand_e unvalidated_demand)
{}

/**
 * The main VTBM re-prgramming state machine
 */
PRIVATE vprg_state_e vprg_state_machine(vprg_state_e current_state,
                                vprg_demand_e demand, image_index_t image_index,
                                vprg_vtbm_mask_t vtbm_mask)
{}

/**
 * Called on entry to #Vprg_state_idle.
 *
 * Sends a DTO if one is pending and clears any errors
 */
PRIVATE vprg_state_e vprg_idle_enter(void)
{}
/**
 * Called while in #Vprg_state_idle
 *
 * Any request other than #Vprg_dmd_none or #Vprg_dmd_prepare will trigger an
 * error
 */
PRIVATE vprg_state_e vprg_idle_during(vprg_demand_e demand,
                                        image_index_t image_index)
{}

/**
 * Called on entry to #Vprg_state_checking_image to tell idle loop functions
 * to check the requested image.
 */
PRIVATE vprg_state_e vprg_checking_image_enter(image_index_t image_index)
{}

/**
 * monitor progress of image check
 */
PRIVATE vprg_state_e vprg_checking_image_during(void)
{}

/**
 * ready to program so send a DTO to report the success of the image check if
 * one is pending
 */
PRIVATE vprg_state_e vprg_ready_to_program_enter(void)
{}

/**
 * Called while in #Vprg_state_ready_to_program
 *
 * Valid requests are #Vprg_dmd_none, #Vprg_dmd_program
 * and #Vprg_dmd_return_to_idle.  Anything else will trigger an error
 */
PRIVATE vprg_state_e vprg_ready_to_program_during(vprg_demand_e demand,
                        image_index_t image_index,vprg_vtbm_mask_t vtbm_mask)
{}

/**
 * A request to program has been received.  Only allow if the following
 * are true:
 * + vehicle conditions allow reprogramming (from
 * + the image has been checked
 * + the vtbm_mask is non-zero and consistent with #pl_num_vtbms
 *
 * If conditions are good, latch the image number and vtbm_mask (by passing
 * them to the communications routines via vprg_comms_set_demand()) and
 * enter the shutdown ICM state to beign the process.
 */
PRIVATE vprg_state_e vprg_check_demand_and_start_programming(
                        image_index_t image_index,vprg_vtbm_mask_t vtbm_mask)
{}


/**
 * Start of a programming event so ask ICM to stop talking to the VTBMs.
 */
PRIVATE vprg_state_e vprg_shutdown_icm_enter(void)
{}

/**
 * Wait for ICM to acknowledge the shutdown request and move on to
 * power cycling the VTBMs when it does.
 */
PRIVATE vprg_state_e vprg_shutdown_icm_during(void)
{}

/**
 * Switch off VTBMs and record the time.
 *
 * For the first VTBM to be reprogrammed, they will already have been switched
 * off by ICM as it transitions to repgrogramming mode, but if there are
 * additional VTBMs to be reprogrammed in the same mask, then we will reach
 * here with VTBMs powered up.
 */
PRIVATE vprg_state_e vprg_cycle_vtbms_enter(void)
{}

/**
 * State to power-cycle VTBMs so that we can connect to their bootloader's
 * XCP slave.
 *
 * Normally, it won't matter whether we connect to the bootloader or the
 * application, but if we've broken the application then we definitely need
 * the bootloader.  The cal #vprg_cycle_on_delay_ms can be tuned to ensure
 * we talk to the bootloader and not the app.
 */
PRIVATE vprg_state_e vprg_cycle_vtbms_during(void)
{}

/**
 * Signals to idle loop to start communicating to the VTBMs.  The image index
 * and VTBM mask were saved previously by vprg_communicating_enter()
 */
PRIVATE vprg_state_e vprg_communicating_enter(void)
{}

/**
 * Keep an eye on the progress of communications by calling vprg_comms_status()
 * If the communication is complete, see if any VTBMs remain to be programmed
 * and either begin the process for the next one, by cycling power again, or
 * enter the Programming Complete state.
 */
PRIVATE vprg_state_e vprg_communicating_during(void)
{}

/**
 * Let go of the MMC power hold and send a DTO if one is pending
 */
PRIVATE vprg_state_e vprg_programming_complete_enter(void)
{}

/**
 * Act on any new demand.  The only ones that make sense are
 * to return to idle or to reset the MMC.
 */
PRIVATE vprg_state_e vprg_programming_complete_during(vprg_demand_e demand)
{}

/**
 * Check the demand.  The only one which makes sense is to return to idle
 */
PRIVATE vprg_state_e vprg_error_during(vprg_demand_e demand)
{}

/** Provide the subcode to report on reprogramming failure, determined by
 * looking at  #vprg_comms_error and translating into a best fit subcode*/
PRIVATE Fm_fault_subtype_list_e vprg_get_failure_subcode(void)
{}

/** If the given error is one to be reported as a fault code/DTC, as opposed to
 * faults which result in rejection of commands over the network for example,
 * update the appropriate ff.
 * @param[in] error the vprg eror to inspect and report if necessary */
PRIVATE void vprg_report_fault(vprg_error_e error)
{}

/**
 * Enter error state, let go of power hold, and send a DTO if one is pending
 */
PRIVATE vprg_state_e vprg_error_enter(vprg_error_e error)
{}

/**
 * Called when the request received from a User Command XCP message has
 * completed.  If we are now in the error state, something went
 * wrong so we need to send an eror message.  Otherwise, the request
 * succeeded so send a result message
 */
PRIVATE void vprg_send_dto(vprg_state_e new_state)
{}
