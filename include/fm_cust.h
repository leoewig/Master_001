/**
 *
 * Fault Manager interface
 *
 * @file
 * @ingroup fm
 * $Id: fm_cust.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2015 - All rights reserved */

#if !defined( FM_CUST_H )
#define       FM_CUST_H

#include "basic_types.h"
#include "fm_types.h"
#include "nvs_cfg.h"



/******************************************************************************
 * MACROS
 *****************************************************************************/


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/* Structure to represent FM RAM image. It contains the fault manager feature
 * data set, Feature version number and some reserved bytes. This structure's
 * object is registered in the NVS feature table #nvs_feat_cfg[].*/
extern FRAM fm_nvs_wrapper_s fm_ram_image;

/* Debug Data - Start */
extern Fm_fault_list_e fm_fault_first;
extern Fm_fault_list_e fm_fault_second;
extern Fm_fault_list_e fm_fault_latest;
extern Fm_fault_subtype_list_e fm_subtype_first;
extern Fm_fault_subtype_list_e fm_subtype_second;
extern Fm_fault_subtype_list_e fm_subtype_latest;

extern uint8_t fm_debug_ctr;
/* Debug Data - End */

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/* Initialize the FM module */
FCODE void fm_init(void);

/* Detects the toggling of Ignition input signal. */
FCODE void fm_update(void);

/* Updates fault filter based on the parameters and fault conditions. */
FCODE bool_t fm_update_fault_filter(const Fm_fault_list_e flt_type,
                                    const Fm_fault_subtype_list_e flt_subtype,
                                    const bool_t flt_present,
                                    const bool_t mon_condition);

/* Resets the requested fault filter and the corresponding filter output.*/
FCODE void fm_reset_fault_filter(const Fm_fault_list_e fault_type);

/* Resets the all fault filters and the corresponding filter outputs.*/
FCODE void fm_reset_fault_filters_for_new_cycle(void);

/* Determine if a fault filter has been updated since last requested reset.*/
FCODE bool_t fm_fault_filter_update_status(const Fm_fault_list_e fault_type);

/* Function to query whether or not a particular failure is set.*/
FCODE bool_t fm_is_fault_present(const Fm_fault_list_e fault_type);

/* Function to query whether or not a particular fault is considered as "monitored". */
FCODE bool_t fm_is_fault_monitored(const Fm_fault_list_e fault_idx);

/* Function to query whether or not a particular failure function is active.*/
FCODE bool_t fm_is_fail_function_active(const Fm_fail_func_list_e fail_func);
/* Function to query whether or not a particular fault group is set.*/
FCODE bool_t fm_is_fault_group_active(const Fm_fault_group_list_e group_type);

/* Function to clear all non-latching faults */
FCODE void fm_clear_faults_non_latching(void);

FCODE void fm_clear_faults_ign_latching(void);

FCODE void fm_clear_faults_service_latch(void);

/* Function to clear all faults */
FCODE void fm_clear_faults_all(void);

/* Function to query the current subtype associated with a particular
 * fault type.  */
FCODE Fm_fault_subtype_list_e fm_get_fault_subtype(const Fm_fault_list_e fault_type);

/**
 * Return the OBD status byte for a fault
 *
 * @param fault_idx the fault whose status is to be returned
 *
 * @return the status for that fault or 0u if the fault is out of range
 */
uint8_t fm_get_obd_status_byte(Fm_fault_list_e fault_idx);

/**
 * Set the OBD status byte for a fault
 *
 * @param fault_idx the fault whose status is to be set
 * @param status the status byte to be stored for this fault
 *
 * If the fault is out of range, no status is updated
 */
void fm_set_obd_status_byte(Fm_fault_list_e fault_idx, uint8_t status);

/**
 * Does fault correspond to an emission-relevant DTC?
 *
 * @param fault_idx the fault whose emission-relevance is to be checked.
 *
 * The caller is expected to have checked whether the fault corresponds
 * to a DTC. If it doesn't the value returned may not have any meaning.
 */
bool_t fm_is_dtc_emission_relevant(Fm_fault_list_e fault_idx);

/**
 * For testing purpose only. It is used to check the fault counter value of
 * a particular fault type.
 */
FCODE uint8_t fm_get_fault_counter(const Fm_fault_list_e fault_type);
#endif
