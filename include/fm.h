/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   fm.h
 *
 * description:  
 * Header file for Fault Management Feature
 * 
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
/**
 * @file
 *
 * Header file for Fault Management Feature
 *
 */
 /**
 * Fault Management Feature
 *
 * ## Requirements:
 * for understanding the mapping of fault types with their corresponding fault subtypes.
 * The Column "DTC Mnemonic" & "Failure Type Mnemonic" in the sheet is represented
 * as the fault type (via enum "Fm_fault_list_e") and fault subtype
 * (via enum "Fm_fault_subtype_list_e")
 * in the design.
 *
 * ## Overview
 * The Fm module maintains information regarding faults.<br>
 * The module consists of the following interface functions;
 *   - fm_init()              : feature initialization,
 *   - fm_update()            : detection of ignition toggling,
 *   - fm_update_fault_filter(): maintenance of faults, main user interface,
 *   - fm_reset_fault_filter(): fault reset,
 *   - fm_fault_filter_update_status() : report if fault has been reset,
 *   - fm_is_fault_present()  : query if a fault is detected (has reached its threshold value),
 *   - fm_is_fail_function_active() : returns TRUE if specified failure function is active,
 *   - fm_is_fault_group_active()   : returns TRUE if specified fault group is active,
 *   - fm_clear_faults_non_latching() : clears all faults that are calibrated as non latching,
 *   - fm_clear_faults_all()  : clears all faults and decrements associated failure function and
 *                           fault group counters,
 *   - fm_get_fault_subtype() : returns the stored fault sub-type for a given fault
 *                           (whether detected or not),
 *   - fm_get_fault_counter() : returns the current fault count for a given fault.
 *                            @note fm_is_fault_present should be used in favour of
 *                            this function. I suspect this function should be dropped
 *                            from the public interface.<br>
 *
 * Lets examine a single fault.<br>
 * Each fault has its own enumerated name. A leaky bucket counter is maintained for each fault.
 * Fundamentally each module needs to pass two parameters to Fm so that it can successfully maintain
 * the fault status, these are whether the fault was present and whether the caller was able to test
 * for the presence of the fault (whether the fault was monitored).
 * If the fault is present then the counter is increased, if the fault is not present then the counter
 * is decreased. During periods where the condition is not monitored by the caller then it is a matter
 * of calibration whether the bucket still leaks.<br>
 * There is an associated threshold that will cause a fault to become detected (and fm_is_fault_present()
 * will return TRUE for a given fault). Once detected different fault counter increment and decrement
 * values are selected.<br>
 * Faults can become undetected when their leaky bucket counter is zero: <br>
 * However also note that the undetection behavour of faults is also controlled by its fault latch type.
 * See below for further details.
 * Each fault can also be associated with an inhibit function and an entirely separate fault group.
 * Interface functions (fm_is_fail_function_active() and fm_is_fault_group_active()) are provided to
 * query their status.
 *
 * ## Configuration
 * The Fm feature owns a calibration structure that defines configuration parameters
 * associated with each fault. This is stored in #Fm_fault_cal. The initial calibration
 * for this structure is defined in the file mmc_fm.def.<br>
 * Within that file, new faults, fault sub-types, failure functions and fault groups
 * can be defined. Additionally other information can be specified for each fault.<br>
 * The calibraetable parameters are;
 *    - increment rate when fault is present but not detected (#fm_calibration_s->undetected_inc_rate),
 *    - decrement rate when fault is not present and not detected (#fm_calibration_s->undetected_dec_rate),
 *    - increment rate when a fault is present and already detected (#fm_calibration_s->detected_inc_rate),
 *    - increment rate when a fault is not present but currently detected (#fm_calibration_s->detected_dec_rate),
 *    - threshold of detection (#fm_calibration_s->threshold),
 *    - flag to indicate fault counts down if un-monitored; note that Fm will not unilaterally leak filters
 *     (#fm_calibration_s->monitor_count_down), the fault detection function MUST be called by the user,
 *    - flag fault as OBD relevant (#fm_calibration_s->obd),
 *    - Fault Latch type  (#fm_calibration_s->latch),
 *    - associated DTC value (note this is independent of the fault enumeration) (#fm_calibration_s->dtc),
 *    - 32-bit map of associated failure functions (note, macros are provided to set these up)
 *      (#fm_calibration_s->failure_functions_map),
 *    - 32-bit map of associated fault groups (note, macros are provided to set these up)
 *      (#fm_calibration_s->fault_group_map).
 *
 * ## Integration
 * The function Fm_init() must be called during initialization, after the nvs has been recovered.<br>
 * The function will check for successfull nvs recovery of Fm data are re-populate its internal status appropriately.
 *
 * The function Fm_update() must be call periodically, and detects the toggling of the ignition line. If this is detected
 * faults that are calibrated to be #Fm_ignition_cycle_latch are reset. Note that #Fm_no_latch faults are NOT reset when
 * when the ignition toggles so that device drivers can log their failures during power on (so that the faults will persist
 * into the ignition cycle).
 *
 * All other functions are user interface functions.
 *
 * ## FM usage Guidelines:<br>
 * To maintain a faults status with Fm call the function fm_update_fault_filter().
 * with the following parameters;<br>
 *  - the fault to be detected (of type #Fm_fault_list_e),
 *  - the sub-type to be stored if the fault is detected (of type #Fm_fault_subtype_list_e),
 *  - a boolean flag to idicate if the fault is currently active (type #bool_t)
 *  - a boolean flag to indicate if the caller was able to evaluate the fault (type #bool_t).
 *
 * An example below shows how this function can be used to apply a fault filter:
 * @par
 * Fault filter example:
 * @code
 * bool_t fault_present = false;
 * bool_t mon_cond_met  = false;
 *
 * mon_cond_met  = (is_my_data_current)       ? TRUE : FALSE;
 * fault_present = (fault_detected_condition) ? TRUE : FALSE;

 * (void) fm_update_fault_filter(Fm_my_fault, Fm_relevant_sub_type, fault_present, mon_cond_met);
 *
 * @endcode
 *
 * ## Requirement Traceability:
 * | Requirement Tags | Satisfied By | Tested By |
 * |------------------|--------------|-----------|
 * | REQ_BMS_SUP_FFP_001 | #fm_calibration_s->undetected_inc_rate | N/A |
 * | REQ_BMS_SUP_FFP_002 | #fm_calibration_s->undetected_dec_rate | N/A |
 * | REQ_BMS_SUP_FFP_003 | #fm_calibration_s->threshold | N/A |
 * | REQ_BMS_SUP_FFP_004 | #fm_calibration_s->detected_inc_rate| N/A |
 * | REQ_BMS_SUP_FFP_005 | #fm_calibration_s->detected_dec_rate | N/A |
 * | REQ_BMS_SUP_FFP_008 | #fm_calibration_s->failure_functions_map | N/A |
 * | REQ_BMS_SUP_FFP_009 | #fm_calibration_s->obd| N/A |
 * | REQ_BMS_SUP_FFP_010 | #fm_calibration_s->dtc | N/A |
 * | REQ_BMS_SUP_FFP_012 | #fm_calibration_s->latch | N/A |
 * | REQ_BMS_SUP_FFP_023 | #fm_calibration_s->monitor_count_down | N/A |
 * | REQ_BMS_SUP_FFF_001 | fm_update_fault_filter() | N/A |
 * | REQ_BMS_SUP_FFF_016 | fm_update_fault_filter() | N/A |
 * | REQ_BMS_SUP_FFF_002 | fm_update_fault_filter() | test_fm_update_fault_filter1() |
 * | REQ_BMS_SUP_FFF_024 | fm_update_fault_filter() | test_fm_update_fault_filter2() |
 * | REQ_BMS_SUP_FFF_025 | fm_update_fault_filter() | test_fm_update_fault_filter3() |
 * | REQ_BMS_SUP_FFF_003 | fm_update_fault_filter() | test_fm_update_fault_filter4() |
 * | REQ_BMS_SUP_FFF_004 | fm_update_fault_filter() | test_fm_update_fault_filter5() |
 * | REQ_BMS_SUP_FFF_005 | fm_update_fault_filter() | test_fm_update_fault_filter6() |
 * | REQ_BMS_SUP_FFF_006 | fm_update_fault_filter() | test_fm_update_fault_filter7() |
 * | REQ_BMS_SUP_FFF_007 | fm_update_fault_filter(), determine_filter_output() | test_fm_update_fault_filter4() |
 * | REQ_BMS_SUP_FFF_017 | fm_update_fault_filter(), determine_filter_output() | test_fm_update_fault_filter4() |
 * | REQ_BMS_SUP_FFF_028 | fm_init(), fm_data_set_s | Fm_Pat_test_fm_init__fault_type_restoration() |
 * | REQ_BMS_SUP_FFF_029 | fm_update_fault_filter(), determine_filter_output() | test_fm_update_fault_filter2() |
 * | REQ_BMS_SUP_FFF_008 | fm_update_fault_filter(), determine_filter_output(), activate_fault_fail_func() | test_fm_update_fault_filter4() |
 * | REQ_BMS_SUP_FFF_009 | fm_update_fault_filter() | test_fm_update_fault_filter4() |
 * | REQ_BMS_SUP_FFF_018 | fm_update_fault_filter() | test_fm_update_fault_filter3() |
 * | REQ_BMS_SUP_FFF_010 | fm_update_fault_filter(), determine_filter_output() |  test_fm_update_fault_filter3() |
 * | REQ_BMS_SUP_FFF_011 | determine_filter_output(), deactivate_fault_fail_func() | test_fm_update_fault_filter4() |
 * | REQ_BMS_SUP_FFF_012 | fm_is_fail_function_active() | test_fm_is_fail_function_active() |
 * | REQ_BMS_SUP_FFF_015 | #Fm_fail_func_list_e , #fm_calibration_s->failure_functions_map | N/A |
 * | REQ_BMS_SUP_FFF_013 | fm_reset_fault_filter() | test_fm_reset_fault_filter() |
 * | REQ_BMS_SUP_FFF_014 | #fm_flt_fltr_s::update_last_reset, fm_fault_filter_update_status() | test_fm_fault_filter_update_status() |
 * | REQ_BMS_SUP_FFF_019 | fm_update(), #prev_ignition_input, #ignition_input | test_fm_update() |
 * | REQ_BMS_SUP_FFF_021 | fm_update(), #prev_ignition_input, #ignition_input | test_fm_update() |
 * | REQ_BMS_SUP_FFF_026 | fm_init(), reset_all_fault_filter_type() | test_fm_init2() |
 * | REQ_BMS_SUP_FFF_027 | fm_init(), reset_all_fault_filter_type() | test_fm_init2() |
 * | REQ_BMS_SUP_FFF_023 | fm_update(), reset_all_fault_filter_type(), fm_init() | test_fm_init2(), test_fm_update() |
 * | REQ_BMS_SUP_FFF_020 | #fm_data_set_s::fault_status | N/A |
 * | REQ_BMS_SUP_FFF_022 | fm_init(), reset_all_fault_filter_type(), activate_fault_functions() | Fm_Pat_test_fm_init__failure_activation_on_restoration() |
 * | REQ_BMS_SUP_FFF_030 | fm_is_fault_present() | test_fm_update()
 * | REQ_BMS_SUP_FFF_032 | determine_filter_output() | test_fm_update_fault_filter8() through to test_fm_update_fault_filter12()
 * | REQ_BMS_SUP_FFF_033 | determine_filter_output() | test_fm_update_fault_filter8() through to test_fm_update_fault_filter12()
 * | REQ_BMS_SUP_FFF_034 | determine_filter_output() | test_fm_update_fault_filter13()
 * | REQ_DIAG_FM_005 | fm_update_fault_filter() | pat_fm__crank |
 * | REQ_DIAG_FM_041 | fm_update_fault_filter() | pat_fm__12v_logic_low |
 *
 *
 * ## Design Details:
 * The Fm module provides a facility to confirm whether or not a failure is
 * present. It is based on a "leaky bucket" principle where the presence of a
 * fault generally causes the filter to count up and the absence of a fault
 * causes the filter to count down. When the filter count reaches a threshold,
 * the status of the output is changed to confirm the presence of a failure.
 * When the status of a fault changes, FM shall inform the OBD feature of
 * the change using obd_fault_present_event() or obd_fault_not_present_event().
 * FM shall also inform the OBD feature when sufficient updates have been made
 * to the filter for the fault to be considered as monitored using
 * obd_fault_monitored_event().

 * The structure struct #fm_flt_fltr_s represents the main Fm module structure.
 * This structure consists of a update_last_reset flag, a fault counter and an update counter.
 *
 * @par The FM Fault Filter Structure looks like:<br>
 * <center>
 * | Structure Data Member     |   Description             |
 * | --------------------------|---------------------------|
 * | update_last_reset flag    | Indicates that the fault filter has been updated since last reset|
 * | flt_count                 | Indicates the fault filter counter |
 * | update_count              | Indicates the number of times the fault has been updated |
 * </center>
 *
 * The FM module has data which need to be stored into the Non-Volatile
 * memory when a certain criteria is met. These form the Fm data set
 * and are classified under the structure struct #fm_data_set_s, as:<br>
 * @par The FM Data Set looks like:
 * <center>
 * | Structure Data Member  |   Description          |
 * | -----------------------| -----------------------|
 * | obd_status             | Status bits which include presence or absence of the fault |
 * | flt_subtype            | Indicates Fault Subtype |
 * </center>
 *
 * The obd_status member is updated for all faults, whether they correspond to
 * a DTC or not.
 *
 * These parameters are restored by the NVS manager upon power-on, and both the
 * fail functions and fault groups are set up appropratiatly for the restored fault
 * filter output status.
 *
 * @defgroup FM Fault Management
 * @ingroup platform
 * @{
 * @file
 * @}
*/

#if !defined( FM_H )
#define       FM_H

/**
 * Define macros that correspond to DTC groups.
 * The DTC_xMASK macros can be used to check whether a DTC corresponds
 * to a certain group.
 * The x(code) macros provide a convenient way to write DTC codes in fm.def
 * Since these may be needed when fm.def is included they need to be
 * defined here.
 */
#define DTC_PMASK       0x00u     /* Powertrain */
#define DTC_CMASK       0x40u     /* Chassis    */
#define DTC_BMASK       0x80u     /* Body       */
#define DTC_UMASK       0xc0u     /* Network    */
#define P(code)         (((uint32_t)DTC_PMASK << 16U) | 0x ## code ## UL)
#define C(code)         (((uint32_t)DTC_CMASK << 16U) | 0x ## code ## UL)
#define B(code)         (((uint32_t)DTC_BMASK << 16U) | 0x ## code ## UL)
#define U(code)         (((uint32_t)DTC_UMASK << 16U) | 0x ## code ## UL)

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

/* Resets fault filters in preparation for a new drive cycle */
FCODE void fm_reset_fault_filters_for_new_drive_cycle(void);

/* Resets the requested fault filter and the corresponding filter output.*/
FCODE void fm_reset_fault_filter(const Fm_fault_list_e fault_type);

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
