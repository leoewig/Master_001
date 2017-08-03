/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   om.h
 *
 * description:  Header file for Output Monitoring Feature
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
/**
 * @file
 *
 * Header file for Output Monitoring Feature
 *
 */
 /**
 * Output Monitoring Feature
 *
 * ## Requirements:
 * The requirements for the feature can be found in the
 * MMC Output Monitoring Section 4.1 of the RU_BMS_Fault_Diagnosis.doc document.
 *
 * ## Requirement Traceability:
 * | Requirement      | Satisfied By | Test Functions |
 * |------------------|--------------|-----------|
 * | REQ_BMS_OMON_002 | om_read_diagnostics_inputs(), om_fault_filter_contactor_diagnostics() | om_read_diagnostics_inputs__data_available() om_read_diagnostics_inputs__no_data_available(), om_fault_filter_contactor_diagnostics__monitoring_condition_present(), om_fault_filter_contactor_diagnostics__monitoring_condition_not_present() |
 * | REQ_BMS_OMON_008 | om_monitor_digital_outputs() | test_om_monitor_digital_outputs() |
 * | REQ_BMS_OMON_023 | #om_validity_flags | om_read_diagnostics_inputs__data_available() om_read_diagnostics_inputs__no_data_available(), om_fault_filter_contactor_diagnostics__monitoring_condition_present(), om_fault_filter_contactor_diagnostics__monitoring_condition_not_present() |
 * | REQ_BMS_OMON_024 | om_read_diagnostics_inputs() | om_read_diagnostics_inputs__data_available() om_read_diagnostics_inputs__no_data_available(), om_fault_filter_contactor_diagnostics__monitoring_condition_present(), om_fault_filter_contactor_diagnostics__monitoring_condition_not_present() |
 * | REQ_BMS_OMON_025 | #om_sensor_data_s.feedback_i_valid | om_read_diagnostics_inputs__data_available() om_read_diagnostics_inputs__no_data_available() |
 * | REQ_BMS_OMON_026 | #om_sensor_data_s.feedback_i_valid | om_read_diagnostics_inputs__data_available() om_read_diagnostics_inputs__no_data_available() |
 * | REQ_BMS_OMON_027 | #om_isa_supply_fault | Requirements not defined |
 * | REQ_BMS_OMON_003 | om_fault_filter_contactor_diagnostics() | om_pre_charge__disabled_no_faults(), om_pre_charge__disabled_fault_high(), om_pre_charge__disabled_fault_high_hvil_not_intact(), om_pre_charge__enabled_no_faults(), om_pre_charge__enabled_fault_low(), om_pre_charge__enabled_fault_low_hvil_not_intact() |
 * | REQ_BMS_OMON_004 | om_fault_filter_contactor_diagnostics() | om_veh_pos__disabled_no_faults(), om_veh_pos__disabled_fault_high(), om_veh_pos__disabled_fault_high_hvil_not_intact(), om_veh_pos__enabled_no_faults(), om_veh_pos__enabled_fault_low(), om_veh_pos__enabled_fault_low_hvil_not_intact() |
 * | REQ_BMS_OMON_005 | om_fault_filter_contactor_diagnostics() | om_veh_neg__disabled_no_faults(), om_veh_neg__disabled_fault_high(), om_veh_neg__disabled_fault_high_hvil_not_intact(), om_veh_neg__enabled_no_faults(), om_veh_neg__enabled_fault_low(), om_veh_neg__enabled_fault_low_hvil_not_intact() |
 * | REQ_BMS_OMON_006 | om_fault_filter_contactor_diagnostics() | om_chg_pos__disabled_no_faults(), om_chg_pos__disabled_fault_high(), om_chg_pos__disabled_fault_high_hvil_not_intact(), om_chg_pos__enabled_no_faults(), om_chg_pos__enabled_fault_low(), om_chg_pos__enabled_fault_low_hvil_not_intact() |
 * | REQ_BMS_OMON_007 | om_fault_filter_contactor_diagnostics() | om_chg_neg__disabled_no_faults(), om_chg_neg__disabled_fault_high(), om_chg_neg__disabled_fault_high_hvil_not_intact(), om_chg_neg__enabled_no_faults(), om_chg_neg__enabled_fault_low(), om_chg_neg__enabled_fault_low_hvil_not_intact() |
 * | REQ_BMS_OMON_028 | #om_isa_supply_fault | Requirements not defined |
 * | REQ_BMS_OMON_009 | om_monitor_digital_outputs() | test_om_monitor_digital_outputs() |
 * | REQ_BMS_OMON_022 | om_monitor_digital_outputs() | test_om_monitor_digital_outputs() |
 * | REQ_BMS_OMON_010 | om_fault_filter_contactor_diagnostics() | om_pre_charge__disabled_no_faults(), om_pre_charge__disabled_fault_high(), om_pre_charge__disabled_fault_high_hvil_not_intact(), om_pre_charge__enabled_no_faults(), om_pre_charge__enabled_fault_low(), om_pre_charge__enabled_fault_low_hvil_not_intact() |
 * | REQ_BMS_OMON_012 | om_fault_filter_contactor_diagnostics() | om_veh_pos__disabled_no_faults(), om_veh_pos__disabled_fault_high(), om_veh_pos__disabled_fault_high_hvil_not_intact(), om_veh_pos__enabled_no_faults(), om_veh_pos__enabled_fault_low(), om_veh_pos__enabled_fault_low_hvil_not_intact() |
 * | REQ_BMS_OMON_013 | om_fault_filter_contactor_diagnostics() | om_veh_pos__disabled_no_faults(), om_veh_pos__disabled_fault_high(), om_veh_pos__disabled_fault_high_hvil_not_intact(), om_veh_pos__enabled_no_faults(), om_veh_pos__enabled_fault_low(), om_veh_pos__enabled_fault_low_hvil_not_intact() |
 * | REQ_BMS_OMON_014 | om_fault_filter_contactor_diagnostics() | om_veh_neg__disabled_no_faults(), om_veh_neg__disabled_fault_high(), om_veh_neg__disabled_fault_high_hvil_not_intact(), om_veh_neg__enabled_no_faults(), om_veh_neg__enabled_fault_low(), om_veh_neg__enabled_fault_low_hvil_not_intact() |
 * | REQ_BMS_OMON_015 | om_fault_filter_contactor_diagnostics() | om_veh_neg__disabled_no_faults(), om_veh_neg__disabled_fault_high(), om_veh_neg__disabled_fault_high_hvil_not_intact(), om_veh_neg__enabled_no_faults(), om_veh_neg__enabled_fault_low(), om_veh_neg__enabled_fault_low_hvil_not_intact() |
 * | REQ_BMS_OMON_016 | om_fault_filter_contactor_diagnostics() | om_chg_pos__disabled_no_faults(), om_chg_pos__disabled_fault_high(), om_chg_pos__disabled_fault_high_hvil_not_intact(), om_chg_pos__enabled_no_faults(), om_chg_pos__enabled_fault_low(), om_chg_pos__enabled_fault_low_hvil_not_intact() |
 * | REQ_BMS_OMON_017 | om_fault_filter_contactor_diagnostics() | om_chg_pos__disabled_no_faults(), om_chg_pos__disabled_fault_high(), om_chg_pos__disabled_fault_high_hvil_not_intact(), om_chg_pos__enabled_no_faults(), om_chg_pos__enabled_fault_low(), om_chg_pos__enabled_fault_low_hvil_not_intact() |
 * | REQ_BMS_OMON_018 | om_fault_filter_contactor_diagnostics() | om_chg_neg__disabled_no_faults(), om_chg_neg__disabled_fault_high(), om_chg_neg__disabled_fault_high_hvil_not_intact(), om_chg_neg__enabled_no_faults(), om_chg_neg__enabled_fault_low(), om_chg_neg__enabled_fault_low_hvil_not_intact() |
 * | REQ_BMS_OMON_019 | om_fault_filter_contactor_diagnostics() | om_chg_neg__disabled_no_faults(), om_chg_neg__disabled_fault_high(), om_chg_neg__disabled_fault_high_hvil_not_intact(), om_chg_neg__enabled_no_faults(), om_chg_neg__enabled_fault_low(), om_chg_neg__enabled_fault_low_hvil_not_intact() |
 * | REQ_BMS_OMON_029 | om_fault_filter_contactor_diagnostics() | om_chg_pos__disabled_no_faults(), om_chg_pos__disabled_fault_high(), om_chg_pos__disabled_fault_high_hvil_not_intact(), om_chg_pos__enabled_no_faults(), om_chg_pos__enabled_fault_low(), om_chg_pos__enabled_fault_low_hvil_not_intact() |
 * | REQ_BMS_OMON_020 | om_monitor_digital_outputs(), fm_update_fault_filter() | test_om_monitor_digital_outputs() |
 * | REQ_BMS_OMON_021 | om_monitor_digital_outputs(), fm_update_fault_filter() | test_om_monitor_digital_outputs() |
 *
 *
 * ## Design Details:
 * The OM module is used by the S12. This module provides a facility for
 * monitoring:
 * - Inputs from the contactors which allow fault detection of the contactor drivers
 * - Outputs from MMC itself to determine whether or not the actual electrical
 * state of the output matches the commanded state.
 *
 * The om_init() function has, at present, no functionality. It is, however, present
 * to allow the OM software to be easily maintainable.
 *
 * The foreground task calls the function om_read_inputs() to monitor
 * the diagnostic inputs associated with all used contactors. These inputs are:
 * - Vehicle HV bus pre-charge contactor current & fault status.
 * - Vehicle HV bus positive contactor current & fault status.
 * - Vehicle HV bus negative contactor current & fault status.
 * - Charger HV bus positive contactor current & fault status.
 * - Charger HV bus negative contactor current & fault status.
 *
 * The current associated with the diagnostic inputs is determined by om_read_diagnostics_inputs()
 * om_validate_input() & om_adc_to_current().
 * The fault filtering of the inputs is performed by om_fault_filter_contactor_diagnostics().
 *
 * If any of these fault status is found to be TRUE, then it means there is a fault
 * associated with the driver. If the HVIL status indicates that the HVIL is intact
 * and the digital input indicates presence of a driver fault then the corresponding
 * fault filter is updated.
 * The respective faults for the inputs are as follows:
 * - #Fm_plat_veh_hv_pre_driver
 * - #Fm_plat_veh_hv_pos_driver
 * - #Fm_plat_veh_hv_neg_driver
 * - #Fm_plat_charger_hv_pos_driver
 * - #Fm_plat_charger_hv_neg_driver
 *
 * The monitoring of the pin states of all configured discrete digital outputs
 * is performed by the function om_monitor_digital_outputs(). This function gets
 * called by the om_read_inputs() function and if the actual state of any
 * monitored digital output pin does not match the commanded state then a fault
 * filter with fault type #Fm_plat_uncommanded_output_state is applied.
 *
 * A mismatch due to any of the outputs lacking in the system, doesn't mean the
 * presense of a fault but could mean that the given output is (intentionally)
 * not present and such cases would be dealt with via calibration.

 * @defgroup OM Output Monitor
 * @ingroup platform
 * @{
 * @file
 * @}
*/

#if !defined( OM_H )
#define       OM_H

#include "basic_types.h"
#include "mmc.h"
#include "om_types.h"


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern bool_t om_isa_supply_fault; /**< indicates if an instantaneous isa supply fault is present */

extern struct om_sensor_data_s om_precharge_current;
extern struct om_sensor_data_s om_veh_pos_current;
extern struct om_sensor_data_s om_veh_neg_current;
extern struct om_sensor_data_s om_chg_pos_current;
extern struct om_sensor_data_s om_chg_neg_current;

extern bool_t om_cont_precharge_valid;
extern bool_t om_cont_veh_pos_valid;
extern bool_t om_cont_veh_neg_valid;
extern bool_t om_cont_chg_pos_valid;
extern bool_t om_cont_chg_neg_valid;

extern uint16_t om_adc_values[Om_max_diag_inputs];

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/*
 * The om_init() function provides an interface to enable the monitoring of
 * diagnostic relating to all used contactors. It sets a logic high on the
 * variables mapped to the following diagnostic enable signals:
 * + Charger HV contactor driver diagnostic control (@mantis{4928}) &
 */
void om_init(void);

/*
 * @mantis{4928} This needs updating for analog diagnostics.
 * The om_read_inputs() function provides an interface to
 * -enable the monitoring of the diagnostic inputs if the HVIL is intact.
 * -monitor diagnostic inputs associated with all used contactors. Based
 * on the fault status represented by the variables:
 * - TODO
 * - TODO
 * - TODO
 * - TODO
 * - TODO
 * the OM module performs monitoring of the feedback inputs. If any fault status
 * representing the diagnostic inputs are set and the HVIL status indicates that
 * HVIL is intact then a corresponding fault filter is applied by calling
 * the function fm_update_fault_filter().
 */
void om_read_inputs(void);

/*
 * Collect OM high side diagnostic input enable state required by OBD snapshots.
 * 
 * @param diag_ip Diagnostic input of interest
 *
 */
bool_t om_ss_data_hs_diag_en_state(enum Om_diagnostic_input_type_e diag_ip);

/*
 * Collect OM low side diagnostic input enable state required by OBD snapshots.
 * 
 * @param diag_ip Diagnostic input of interest
 *
 */
bool_t om_ss_data_ls_diag_en_state(enum Om_diagnostic_input_type_e diag_ip);

/*
 * Collect OM diagnostic input current data required by OBD snapshots.
 * 
 * @param diag_ip Diagnostic input of interest
 *
 * @param current Pointer to a location in which the current measurement is
 *                to be returned
 *
 * @param validity Pointer to a location in which the current measurement 
 *                 validity is to be returned
 *
 */
void om_ss_data_diag_current(enum Om_diagnostic_input_type_e diag_ip, 
                             current_t *current, data_validity_e *validity);

#endif
