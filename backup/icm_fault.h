/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    icm_fault.h 
 *
 * description:  
 * ICM fault detection header
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(ICM_FAULT_H)
#define      ICM_FAULT_H
/**
 *
 * ICM fault detection header
 *
 * @file
 * @ingroup icm_fault
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#include "icm_types.h"

/**
 * Fault detection for the ICM feature, this allows aggregation of information
 * for multiple VTBMs into a single call to the fault management. It also
 * facilitates the calling of FM's update once per task even if other code
 * related to a specific fault has not been invoked (and hence the fault is
 * not 'monitored' for that cycle).
 *
 *
 * <h2>Internal Comms fault management </h2>
 *
 * <h2> Approach used </h2>
 * ICM faults detection conditions and Fault codes set are a many-many mapping
 * An intermediate table is used to remember whether a given fault has been
 * detected since last time. This means that the ICM's fault update function
 * can be safely called multiple times for each fault in each iteration
 * e.g. once per VTBM or once per distinct fault subtype.
 *
 * <h2> Limitations </h2>
 * Can only be used for ICM faults, faults on multiple VTBMs will only be
 * reported for one VTBM due to the way the fault management works.
 *
 * <h2> How to use </h2>
 * Call icm_update_fault() each time a given fault condition is tested
 * If the fault is specific to a VTBM number (i.e. the VTBM number is the
 * subtype) then use icm_update_vtbm_fault().
 *
 * icm_update_fault_filters() must be called at the end of each ICM invocation
 * to push the faults to the FM feature.
 *
 * The other exposed functions (typically icm_chk_xxx ) in this header are used
 * to test a particular fault condition. They should be called at the point
 * where it is appropriate
 * to check that fault.
 *
 * fm_update_fault_filter() is called once for every ICM fault each time
 * icm_update_fault_filters() is called. The faults that this is done for are
 * listed in mmc_icm_faults.def
 *
 * <h2> Configuration </h2>
 * The fault filters themselves are configured by fm. The following CAL items
 * are available:
 * - #icm_cell_t_min
 * - #icm_cell_t_max
 * - #icm_cell_t_tol
 * - #icm_vtbm_cell_t_lost_thold
 * - #icm_vtbm_cell_v_lost_thold
 * - #icm_cell_v_lost_thold
 * - #icm_cell_t_lost_thold
 * - #icm_min_vactive_ref_v
 * - #icm_min_vpassive_ref_v
 * - #icm_max_vpassive_ref_v
 * - #icm_max_gnd_ref_v
 * - #icm_ref_tol
 *
 * <h2> Requirement Satisfaction </h2>
 * Faults detected by ICM are specified in:
 * svn:Q52717_NightWing/trunk/docs/reqs/generic/RU_BMS_Fault_Diagnosis.doc
 *
 * See also Fault table in http://viewvc/viewvc.cgi?view=revision&root=Q52717_NightWing&revision=2663
 *
 * The faults checked as listed in the table below:
 *
 * | Fault                              | Checked when         | By Function                       |
 * |------------------------------------|----------------------|-----------------------------------|
 * | plat_cell_t,                       | Measure completion   | icm_chk_plat_cell_t()             |
 * | plat_cell_v,                       | Measure completion   | icm_chk_plat_cell_v()             |
 * | plat_vtbm_n_cell_t_lost,           | Measure completion   | icm_chk_plat_vtbm_n_cell_t_lost() |
 * | plat_vtbm_n_cell_v_lost,           | Measure completion   | icm_chk_plat_vtbm_n_cell_v_lost() |
 * | plat_n_cell_t_lost,                | Measure completion   | icm_chk_plat_n_cell_t_lost()      |
 * | plat_n_cell_v_lost,                | Measure completion   | icm_chk_plat_n_cell_v_lost()      |
 * | plat_cell_unexpected,              | Measure completion   | icm_chk_plat_cell_unexpected()    |
 * | plat_vtbm_ref                      | Ref meas. completion | icm_validate_ref_values()         |
 * | plat_ic_unknown_msg                | On any Message       | icm_process_frame_fault()         |
 * | plat_vtbm_comms_invalid_data(1)    | On any Message       | icm_process_frame_fault()         |
 * | plat_vtbm_comms_invalid_data(2)    | On status message    | icm_chk_invalid_status_rxd()      |
 * | plat_vtbm_comms_protocol_mismatch, | On status message    | icm_validate_status_data()        |
 * | plat_vtbm_unconfigured,            | Status completion    | icm_check_cfg_responses()         |
 * | plat_vtbm_missing_eol_data,        | Status completion    | icm_check_cal_responses()         |
 * | plat_vtbm_internal_error_unexpected| Status received      | icm_check_vtbm_internal_error()   |
 * | plat_uncommanded_balancing,        | On valid status message | icm_chk_uncommanded_bal()      |
 * | plat_balancing_request_not_actioned| On valid status message | icm_chk_unactioned_bal()       |
 * | plat_sw_hw_incompatibility,        | On valid status message | icm_chk_vtbm_hw_mismatch()     |
 * | plat_vtbm_comms_initial_timeout(1),| Status Completion       | icm_chk_vtbm_timeouts()        |
 * | plat_vtbm_comms_initial_timeout(2),| Refs completion         | icm_chk_vtbm_timeouts()        |
 * | plat_vtbm_comms_response_timeout(1)| Status Completion       | icm_chk_vtbm_timeouts()        |
 * | plat_vtbm_comms_response_timeout(2)| Refs completion         | icm_chk_vtbm_timeouts()        |
 * | PLAT_VTBM_OPTOMOS_FAILURE fault    | Not implemented         | Not implemented                |
 *
 * Related faults
 * ----------------
 * The mmc_ican_filter_rx_buffer_full() function also checks  Fm_plat_int_can_rx_buffer_full
 * on the conditions:
 * - ican_rx_hw_buffer_overflow
 * - ican_rx_s12_sw_buffer_overflow
 * - ican_rx_xg_sw_buffer_overflow
 *  And Fm_plat_int_can_tx_buffer_full on ican_tx_buffer_overflow.
 *
 * So ICM does not set any additional faults if these conditions are detected.
 *
 * @defgroup icm_fault ICM fault detection
 * @ingroup icm
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "fm_types.h"       /* Needed by icm_fault.h */

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

#define ICM_FAULT(fltname,precond) Icm_flt_ ## fltname ,
enum IcmFault_e
{
#include "icm_faults.def"
    Icm_flt_count
};
#undef ICM_FAULT

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/


/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

void icm_update_fault_filters(void);

void icm_update_fault(enum IcmFault_e icm_fault,
                Fm_fault_subtype_list_e subtype, bool_t fault_detected);

void icm_update_vtbm_fault( enum IcmFault_e icm_fault, uint8_t num_vtbm,
                            bool_t fault_detected);

/*****************************************************************************
 * Measurement related faults
 ****************************************************************************/
void icm_chk_plat_cell_t(void);
void icm_chk_plat_cell_v(void);

void icm_chk_cell_timeout(bool_t initial,const FPTR uint8_t expected_frames[],
    const volatile FPTR uint8_t received_frames[] );

void icm_chk_plat_vtbm_n_cell_v_lost(void);
void icm_chk_plat_vtbm_n_cell_t_lost(void);
void icm_chk_plat_n_cell_t_lost(void);
void icm_chk_plat_n_cell_v_lost(void);
void icm_chk_plat_cell_unexpected(void);
void icm_chk_plat_ref_unexpected(void);
void icm_chk_vtbm_timeouts(bool_t initial,
    const FPTR bool_t sig_recv[] );

void    icm_check_balancing(uint8_t nvtbm, enum BalAction_e action,
    uint8_t ncell);

void icm_chk_vtbm_hw_mismatch(const struct vtbm_status_s  *statp);

void icm_check_vtbm_internal_error(const struct vtbm_status_s * statp);

bool_t icm_chk_status_source(const struct vtbm_status_s  *statp);
bool_t icm_chk_vtbm_eolcal_response(uint8_t num);
bool_t icm_chk_vactive_ref(uint8_t vtbm_num, cell_v_t value, enum CellVstat_e status);
bool_t icm_chk_vpassive_ref(uint8_t vtbm_num, cell_v_t value,
    enum CellVstat_e status);
bool_t icm_chk_gnd_ref(uint8_t vtbm_num,cell_v_t value,enum CellVstat_e status);
bool_t icm_chk_dig_ref(uint8_t vtbm_num,uint8_t obs_dig_mask);
#endif      /* ICM_FAULT_H */

