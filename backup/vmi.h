/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     vmi.h
 *
 * description:   vmi
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           vmi
 *                       
 *
 *****************************************************************************/
#if !defined(VMI_H)
#define      VMI_H
/**
 *
 * This feature reads the high & low resoultion voltage measurements and the supply
 * inputs in raw ADC counts, converts them into values representing the actual
 * voltages and makes them available to the rest of the system.
 *
 * @file
 * @ingroup vmi
 */

/* Copyright (c) Linkedriving Tech 2011 - All rights reserved */

/**
 *
 * <h1>Voltage Measurement Input Feature</h1>
 *
 * <h2>Design Goals</h2>
 *
 * Voltage Measurement Input module for reading raw ADC voltage values and
 * converting to values representing the actual bus & supply voltages.
 *
 * The raw ADC values are read from the hardware by the A/D Driver and are then
 * read from the result variables declared by this module.
 *
 * The requirements document for the feature can be found in
 * <a href="http://viewvc/viewvc.cgi/trunk/docs/reqs/generic/RU_BMS_Support.doc?root=Q52717_NightWing&view=co">BMS Support Requirements</a>.
 * This design intends to cover all of the VMI requirements specified in this
 * document.
 *
 * <h2> Approach Used </h2>
 *
 * This module reads the high voltage (both high and low resolution) and supply
 * inputs from the hardware and providing that the values are within bounds,
 * flags them as #New_data_available, and converts the raw data to values representing
 * the actual bus voltages.
 *
 * If any value is not within bounds, then it is held at the last valid value, flagged
 * as #Old_data_available & the appropriate fault condition is flagged.
 *
 * <h2> Fault Handling </h2>
 *
 * The method of detecting faults has not got any defined requirements at present.
 *
 * However, if an error is present, then the associated fault filter will be updated
 * based upon the presence or absence of the fault condition.
 *
 * <h2> HW/SW Specification Equations & Constants </h2>
 *
 * All of the equations in the HW/SW Specification for the calucations are linear,
 * i.e. of the form y = Ax + B. Hence in orer to simplify the processing performed
 * the voltages are calculated using lookup tables & the utlitity interpolation routines.
 *
 * From the HW/SW Specification:
 * <h3> High Resolution Inputs - Vehicle</h3>
 *
 * @par
 * V<sub>pos</sub> = V<sub>adc</sub> * k1\n
 * V<sub>neg</sub> = V<sub>adc</sub> * k2 + k3
 *
 * <h3> High Resolution Inputs - Charger</h3>
 *
 * @par
 * V<sub>pos</sub> = V<sub>adc</sub> * kch1\n
 * V<sub>neg</sub> = V<sub>adc</sub> * kch2 + kch3
 *
 * <h3> Low Resolution Inputs </h3>
 *
 * @par
 * V<sub>pos</sub> = V<sub>adc</sub> * kl1 + kl2\n
 * V<sub>neg</sub> = V<sub>adc</sub> * kl3 + kl4
 *
 * <h2> Feature Integration </h2>
 *
 * It is assumed that the main voltage measurement input reading routine
 * shall only be called from a single place and that multiple entries are not
 * possible.
 *
 * vmi_init() shall be called to initialise this feature, vmi_read_inputs() shall
 * then be called at regular intervals thereafter.
 *
 * The high voltage bus voltages and validity can then be read from the array #vmi_hv_hi_res_data.
 *
 * The low resolution high voltage bus voltages and validity can be read from the array #vmi_hv_lo_res_data.
 *
 * The supply voltages and validity can then be read from the array #vmi_supply_data.
 *
 * <h2> Requirement Satisfaction </h2>
 * The requirements document for the feature can be found in
 * <a href="http://viewvc/viewvc.cgi/trunk/docs/reqs/generic/RU_BMS_Support.doc?root=Q52717_NightWing&view=co">BMS Support Requirements section 3.5</a>.
 *
 * All requirements are satisfied by this module.
 *
 * Requirements are satisfied as follows:
 *
 * | Requirement | Code | Tested By |
 * | ---------------------------|-------------------------|---------------------------------------------|
 * | REQ_BMS_PP_V_001 | vmi_hi_res.c, vmi_lo_res.c | |
 * | REQ_BMS_PP_V_002 | vmi_hi_res.c, vmi_lo_res.c | |
 * | REQ_BMS_SMON_006 | smg_mon_supply_off_events() | |
 * | REQ_BMS_SMON_007 | smg_mon_supply_on_events() | pat_smg_vtbm_can_5v_enabled_low(), pat_smg_vtbm_can_5v_enabled_high() |
 * | REQ_BMS_SMON_008 | smg_mon_supply_on_events() | pat_smg_vtbm_can_5v_disabled() |
 * | REQ_BMS_SMON_010 | mon_12v_power_status(), smg_fault_filter_12v_power() | |
 * | REQ_BMS_SMON_011 | smg_fault_filter_12v_power() | pat_smg_12v_power_hvil_status_veh_contactors_open_low(), pat_smg_12v_power_hvil_status_veh_contactors_open_high(), pat_smg_12v_power_hvil_status_veh_contactors_closed_low(), pat_smg_12v_power_hvil_status_veh_contactors_closed_high() |
 * | REQ_BMS_SMON_012 | smg_fault_filter_12v_power() | pat_smg_12v_power_hvil_status_veh_contactors_open_low(), pat_smg_12v_power_hvil_status_veh_contactors_open_high(), pat_smg_12v_power_hvil_status_veh_contactors_closed_low(), pat_smg_12v_power_hvil_status_veh_contactors_closed_high() |
 * | REQ_BMS_SMON_015 | smg_mon_supply_on_events(), smg_mon_supply_off_events() | pat_smg_vtbm_can_5v_enabled_low(), pat_smg_vtbm_can_5v_enabled_high(), pat_smg_vtbm_can_5v_disabled() |
 * | REQ_BMS_SMON_016 | smg_mon_supply_on_events() | pat_smg_vtbm_can_5v_enabled_low(), pat_smg_vtbm_can_5v_enabled_high(), pat_smg_vtbm_can_5v_disabled() |
 * | REQ_BMS_SMON_017 | smg_mon_supply_on_events(), smg_mon_supply_off_events() | pat_smg_isa_sensor_enabled_low(), pat_smg_isa_sensor_enabled_high(), pat_smg_isa_sensor_enabled_invalid(), pat_smg_isa_sensor_disabled_high(), pat_smg_isa_sensor_disabled_invalid() |
 * | REQ_BMS_SMON_018 | smg_mon_supply_on_events() | pat_smg_isa_sensor_enabled_low() |
 * | REQ_BMS_SMON_019 | #smg_isa_sensor_off_time_threshold | |
 * | REQ_BMS_SMON_020 | #smg_isa_sensor_off_time_threshold | |
 * | REQ_BMS_SMON_021 | #smg_vtbm_can_on_time_threshold | |
 * | REQ_BMS_SMON_022 | #smg_vtbm_can_off_time_threshold | |
 * | REQ_BMS_SMON_023 | #smg_isa_sensor, #smg_on_timers, smg_mon_supply_on_events() | pat_smg_isa_sensor_enabled_low(), pat_smg_isa_sensor_enabled_high(), pat_smg_isa_sensor_enabled_invalid() |
 * | REQ_BMS_SMON_024 | #smg_vtbm_can_5v, #smg_on_timers, smg_mon_supply_on_events() | |
 * | REQ_BMS_SMON_025 | #smg_isa_sensor, #smg_off_timers, smg_mon_supply_off_events() | pat_smg_isa_sensor_disabled_high(), pat_smg_isa_sensor_disabled_invalid() |
 * | REQ_BMS_SMON_026 | #smg_vtbm_can_5v, #smg_off_timers, smg_mon_supply_off_events() | |
 * | REQ_BMS_SMON_027 | mon_12v_power_status(), smg_fault_filter_12v_power() | |
 * | REQ_BMS_SMON_028 | smg_fault_filter_12v_power() | pat_smg_12v_power_hvil_status_veh_contactors_open_low(), pat_smg_12v_power_hvil_status_veh_contactors_open_high(), pat_smg_12v_power_hvil_status_veh_contactors_closed_low(), pat_smg_12v_power_hvil_status_veh_contactors_closed_high() |
 * | REQ_BMS_SMON_029 | smg_fault_filter_12v_power() | |
 * | REQ_BMS_SMON_031 | #smg_isa_sensor_sply_tol | |
 * | REQ_BMS_SMON_032 | #smg_vtbm_can_5v_tol | |
 * | REQ_BMS_SMON_033 | #smg_vtbm_can_gnd_tol | |
 * | REQ_BMS_SMON_034 | #smg_vpos_tol | |
 * | REQ_BMS_SMON_035 | #smg_vneg_tol | |
 * | REQ_BMS_SMON_036 | smg_fault_filter_vneg() | pat_smg_vneg_low(), pat_smg_vneg_high() |
 * | REQ_BMS_SMON_037 | smg_fault_filter_vpos()| pat_smg_vpos_low(), pat_smg_vpos_high() |
 * | REQ_BMS_SMON_038 | smg_mon_supply_off_events() | pat_smg_vtbm_can_gnd() |
 * | REQ_BMS_SMON_039 | vmi_read_hi_res_voltages() | |
 * | REQ_BMS_SMON_040 | vmi_read_hi_res_voltages() | |
 * | REQ_BMS_SMON_041 | smg_mon_supply_off_events() | pat_smg_vtbm_can_gnd() |
 * | REQ_BMS_SMON_042 | smg_fault_filter_vneg() | pat_smg_vneg_low(), pat_smg_vneg_high() |
 * | REQ_BMS_SMON_043 | smg_fault_filter_vneg() | |
 * | REQ_BMS_SMON_044 | smg_fault_filter_vneg() | pat_smg_vneg_low(), pat_smg_vneg_high() |
 * | REQ_BMS_SMON_045 | smg_fault_filter_vpos() | pat_smg_vpos_low(), pat_smg_vpos_high() |
 * | REQ_BMS_SMON_046 | smg_fault_filter_vpos() | pat_smg_vpos_low(), pat_smg_vpos_high() |
 * | REQ_BMS_SMON_047 | smg_mon_supply_on_events() | pat_smg_isa_sensor_enabled_low() |
 * | REQ_BMS_SMON_048 | smg_mon_supply_on_events() | pat_smg_isa_sensor_enabled_high() |
 * | REQ_BMS_SMON_049 | #vmi_isa_supply_lower_limit | |
 * | REQ_BMS_SMON_050 | #vmi_isa_supply_upper_limit | |
 * | REQ_BMS_SMON_051 | smg_mon_supply_on_events() | pat_smg_isa_sensor_enabled_high() |
 * | REQ_BMS_SMON_052 | smg_mon_supply_off_events() | |
 * | REQ_BMS_SMON_053 | smg_mon_supply_on_events() | pat_smg_isa_sensor_enabled_invalid() |
 * | REQ_BMS_SMON_054 | smg_mon_supply_on_events() | pat_smg_isa_sensor_enabled_invalid() |
 * | REQ_BMS_SMON_055 | #vmi_isa_12v_status | |
 * | REQ_BMS_SUP_HVM_001 | #vmi_bus_e, #vmi_hv_hi_res_data, #vmi_rail_e, #vmi_hv_lo_res_data | pat_vmi_veh_bus_hi_res_pos_low(), pat_vmi_veh_bus_hi_res_pos_mid(), pat_vmi_veh_bus_hi_res_pos_high(), pat_vmi_veh_bus_hi_res_neg_low(), pat_vmi_veh_bus_hi_res_neg_mid(), pat_vmi_veh_bus_hi_res_neg_high() |
 * | REQ_BMS_SUP_HVM_003 | #Vmi_vehicle_bus, #vmi_hv_hi_res_data, vmi_read_hi_res_voltages() | pat_vmi_veh_bus_hi_res_pos_low(), pat_vmi_veh_bus_hi_res_pos_mid(), pat_vmi_veh_bus_hi_res_pos_high(), pat_vmi_veh_bus_hi_res_neg_low(), pat_vmi_veh_bus_hi_res_neg_mid(), pat_vmi_veh_bus_hi_res_neg_high() |
 * | REQ_BMS_SUP_HVM_004 | #Vmi_charger_bus, #vmi_hv_hi_res_data, vmi_read_hi_res_voltages() | pat_vmi_chg_bus_pos_low(), pat_vmi_chg_bus_pos_mid(), pat_vmi_chg_bus_pos_high(), pat_vmi_chg_bus_neg_low(), pat_vmi_chg_bus_neg_mid(), pat_vmi_chg_bus_neg_high() |
 * | REQ_BMS_SUP_HVM_005 | #vmi_hv_hi_res_data, vmi_read_hi_res_voltages(), #vmi_hv_lo_res_data, vmi_read_lo_res_voltages() | |
 * | REQ_BMS_SUP_HVM_006 | vmi_calc_hi_res_rail_voltage(), vmi_calc_lo_res_voltage() | pat_vmi_veh_bus_hi_res_pos_low(), pat_vmi_veh_bus_hi_res_pos_mid(), pat_vmi_veh_bus_hi_res_pos_high(), pat_vmi_veh_bus_hi_res_neg_low(), pat_vmi_veh_bus_hi_res_neg_mid(), pat_vmi_veh_bus_hi_res_neg_high(), pat_vmi_chg_bus_pos_low(), pat_vmi_chg_bus_pos_mid(), pat_vmi_chg_bus_pos_high(), pat_vmi_chg_bus_neg_low(), pat_vmi_chg_bus_neg_mid(), pat_vmi_chg_bus_neg_high() |
 * | REQ_BMS_SUP_HVM_015 | #vmi_hv_hi_res_data, #vmi_hv_lo_res_data, #vmi_supply_data | |
 * | REQ_BMS_SUP_HVM_016 | Marked as TODO | |
 * | REQ_BMS_SUP_HVM_017 | Marked as TODO | |
 * | REQ_BMS_SUP_HVM_018 | vmi_read_hi_res_voltages() | |
 * | REQ_BMS_SUP_HVM_019 | vmi_read_hi_res_voltages() | |
 * | REQ_BMS_SUP_HVM_020 | #Vmi_isa_12v, #vmi_supply_data | |
 * | REQ_BMS_SUP_HVM_021 | #Vmi_vtbm_can_5v, #vmi_supply_data | |
 * | REQ_BMS_SUP_HVM_023 | #Vmi_12v_power, #vmi_supply_data | |
 * | REQ_BMS_SUP_HVM_024 | #Vmi_12v_power, #vmi_supply_data, vmi_read_supply_voltages(), vmi_calc_supply_voltage() | |
 * | REQ_BMS_SUP_HVM_025 | #Vmi_vtbm_can_5v, #vmi_supply_data, vmi_read_supply_voltages(), vmi_calc_supply_voltage() | |
 * | REQ_BMS_SUP_HVM_027 | #Vmi_12v_power, #vmi_supply_data, vmi_read_supply_voltages(), vmi_calc_supply_voltage() | |
 * | REQ_BMS_SUP_HVM_028 | #vmi_12v_power_optimal_voltage | Tested by code inspection |
 * | REQ_BMS_SUP_HVM_029 | #vmi_12v_power_tolerance | Tested by code inspection |
 * | REQ_BMS_SUP_HVM_030 | #vmi_12v_power_status | |
 * | REQ_BMS_SUP_HVM_031 | smg_fault_filter_12v_power() | pat_smg_12v_power_low(), pat_smg_12v_power_high() |
 * | REQ_BMS_SUP_HVM_032 | smg_fault_filter_12v_power() | pat_smg_12v_power_low(), pat_smg_12v_power_high() |
 * | REQ_BMS_SUP_HVM_033 | #Vmi_vehicle_bus, #vmi_hv_hi_res_0v_tolerance | Tested by code inspection |
 * | REQ_BMS_SUP_HVM_034 | #Vmi_vehicle_bus, #vmi_hv_0v_e, #vmi_hv_hi_res_data | |
 * | REQ_BMS_SUP_HVM_035 | #Vmi_vehicle_bus, #vmi_hv_hi_res_0v_tolerance, vmi_read_hi_res_voltages(), #vmi_hv_hi_res_data | |
 * | REQ_BMS_SUP_HVM_036 | #Vmi_vehicle_bus, #vmi_hv_hi_res_0v_tolerance, vmi_read_hi_res_voltages(), #vmi_hv_hi_res_data | |
 * | REQ_BMS_SUP_HVM_037 | #Vmi_vehicle_bus, #vmi_hv_hi_res_0v_tolerance, vmi_read_hi_res_voltages(), vmi_fault_filter_12v_logic() | |
 * | REQ_BMS_SUP_HVM_038 | #Vmi_charger_bus, #vmi_hv_0v_e, #vmi_hv_hi_res_data | |
 * | REQ_BMS_SUP_HVM_039 | #Vmi_charger_bus, #vmi_hv_hi_res_0v_tolerance, vmi_read_hi_res_voltages(), #vmi_hv_hi_res_data | |
 * | REQ_BMS_SUP_HVM_040 | #Vmi_charger_bus, #vmi_hv_hi_res_0v_tolerance, vmi_read_hi_res_voltages(), #vmi_hv_hi_res_data | |
 * | REQ_BMS_SUP_HVM_041 | #Vmi_charger_bus, #vmi_hv_hi_res_0v_tolerance, vmi_read_hi_res_voltages(), vmi_fault_filter_12v_logic() | |
 * | REQ_BMS_SUP_HVM_042 | #Vmi_charger_bus, #vmi_hv_stable_tolerance, #vmi_hv_unstable_tolerance | Tested by code inspection |
 * | REQ_BMS_SUP_HVM_043 | #vmi_hv_hi_res_0v_tolerance | Tested by code inspection |
 * | REQ_BMS_SUP_HVM_044 | #vmi_12v_power_monitoring_lower_limit | Tested by code inspection |
 * | REQ_BMS_SUP_HVM_045 | #Vmi_vehicle_bus, #vmi_hv_lo_res_data, vmi_read_lo_res_voltages() | pat_vmi_veh_bus_lo_res_pos_low(), pat_vmi_veh_bus_lo_res_pos_mid(), pat_vmi_veh_bus_lo_res_pos_high(), pat_vmi_veh_bus_lo_res_neg_low(), pat_vmi_veh_bus_lo_res_neg_mid(), pat_vmi_veh_bus_lo_res_neg_high()|
 * | REQ_BMS_SUP_HVM_046 | #vmi_hv_lo_res_0v_tolerance, vmi_read_lo_res_voltages(), #vmi_hv_lo_res_data | |
 * | REQ_BMS_SUP_HVM_047 | #vmi_hv_lo_res_0v_tolerance, vmi_read_lo_res_voltages(), #vmi_hv_lo_res_data | |
 * | REQ_BMS_SUP_HVM_048 | #vmi_hv_lo_res_0v_tolerance, vmi_read_lo_res_voltages(), #vmi_hv_lo_res_data | |
 * | REQ_BMS_SUP_HVM_049 | #vmi_hv_lo_res_0v_tolerance | Tested by code inspection |
 * | REQ_BMS_SUP_HVM_050 | #vmi_12v_logic_lower_limit | Tested by code inspection |
 * | REQ_BMS_SUP_HVM_050 | | |
 * | REQ_BMS_SUP_HVM_051 | #vmi_12v_logic_upper_limit | Tested by code inspection |
 * | REQ_BMS_SUP_HVM_051 | #vmi_hv_hi_res_data_pos_isol, #vmi_hv_hi_res_data_neg_isol, vmi_read_hi_res_voltages() | |
 * | REQ_BMS_SUP_HVM_052 | vmi_read_hi_res_voltages() | |
 * | REQ_BMS_SUP_HVM_054 | #Vmi_12v_logic, #vmi_supply_data, vmi_read_supply_voltages(), vmi_calc_supply_voltage() | |
 * | REQ_BMS_SUP_HVM_056 | vmi_fault_filter_12v_logic() | pat_smg_12v_logic_low(), pat_smg_12v_logic_high() |
 * | REQ_BMS_SUP_HVM_057 | vmi_fault_filter_12v_logic() | pat_smg_12v_logic_low(), pat_smg_12v_logic_high() |
 * | REQ_BMS_SUP_HVM_058 | #Vmi_vtbm_can_gnd, #vmi_supply_data | |
 * | REQ_BMS_SUP_HVM_059 | #Vmi_vpos_12v, #vmi_supply_data | |
 * | REQ_BMS_SUP_HVM_060 | #Vmi_vneg_12v, #vmi_supply_data | |
 * | REQ_BMS_SUP_HVM_061 | #Vmi_12v_logic, #vmi_supply_data | |
 * | REQ_BMS_SUP_HVM_062 | #Vmi_vtbm_can_gnd, #vmi_supply_data, vmi_read_supply_voltages(), vmi_calc_supply_voltage() | |
 * | REQ_BMS_SUP_HVM_063 | #Vmi_vpos_12v, #vmi_supply_data, vmi_read_supply_voltages(), vmi_calc_supply_voltage() | |
 * | REQ_BMS_SUP_HVM_064 | #Vmi_vneg_12v, #vmi_supply_data, vmi_read_supply_voltages(), vmi_calc_supply_voltage() | |
 * | REQ_BMS_SUP_HVM_066 | | |
 * | REQ_BMS_SUP_HVM_067 | #vmi_isa_12v_status | |
 * | REQ_BMS_SUP_HVM_069 | #vmi_vtbm_can_status | |
 * | REQ_BMS_SUP_HVM_070 | vmi_update_vtbm_can_5v_status_disabled(), vmi_update_vtbm_can_5v_status_enabled(), vmi_update_vtbm_can_gnd_status() | |
 * | REQ_BMS_SUP_HVM_071 | vmi_update_12v_power_status() | |
 * | REQ_BMS_SUP_HVM_072 | mon_12v_power_status(), smg_fault_filter_12v_power() | pat_smg_12v_power_low(), pat_smg_12v_power_high() |
 * | REQ_BMS_SUP_HVM_073 | vmi_fault_filter_12v_logic() | pat_smg_12v_logic_low(), pat_smg_12v_logic_high() |
 *
 * @defgroup vmi Voltage Measurement Input
 * @ingroup platform
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "mmc.h"
#include "vmi_types.h"

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern struct vmi_hv_rail_s        vmi_hv_hi_res_data         [Vmi_last_bus];            /**< The current overall hi-res data read for each HV bus. */
extern struct vmi_hv_rail_s        vmi_hv_hi_res_data_pos     [Vmi_last_bus];            /**< The current positive rail hi-res data read for each HV bus. */
extern struct vmi_hv_rail_s        vmi_hv_hi_res_data_neg     [Vmi_last_bus];            /**< The current negative rail hi-res data read for each HV bus. */
extern struct vmi_hv_rail_s        vmi_hv_hi_res_data_pos_isol[Vmi_last_bus];            /**< The current isolated positive rail hi-res data read for each HV bus. */
extern struct vmi_hv_rail_s        vmi_hv_hi_res_data_neg_isol[Vmi_last_bus];            /**< The current isolated negative rail hi-res data read for each HV bus. */
extern struct vmi_hv_rail_s        vmi_hv_lo_res_data         [Vmi_last_rail];           /**< The current lo-res data read for the vehicle HV bus. */
extern struct vmi_hv_supply_s      vmi_supply_data            [Vmi_last_supply];         /**< The last valid voltages read for each LV supply */

extern uint16_t                    vmi_hv_hi_res_pos_adc_values     [Vmi_last_bus];
extern uint16_t                    vmi_hv_hi_res_pos_adc_values_isol[Vmi_last_bus];
extern uint16_t                    vmi_hv_hi_res_neg_adc_values     [Vmi_last_bus];
extern uint16_t                    vmi_hv_hi_res_neg_adc_values_isol[Vmi_last_bus];
extern uint16_t                    vmi_hv_lo_res_adc_values         [Vmi_last_rail];
extern uint16_t                    vmi_supply_adc_values            [Vmi_last_supply];

extern enum vmi_bus_e              vmi_active_bus;                                  /**< The currently active HV bus */

extern enum vmi_supply_status_e    vmi_isa_12v_status;                              /**< Flag indicating the state of the monitoring conditions for the 12v supply */
extern enum vmi_supply_status_e    vmi_vtbm_can_status;                             /**< Flag indicating the state of the monitoring conditions for the VTBM CAN supply */
extern enum vmi_supply_status_e    vmi_vtbm_can_5v_status;                          /**< Flag indicating the state of the monitoring conditions for the VTBM CAN 5V supply */
extern enum vmi_supply_status_e    vmi_vtbm_can_gnd_status;                         /**< Flag indicating the state of the monitoring conditions for the VTBM CAN ground supply */
extern enum vmi_supply_status_e    vmi_vpos_status;                                 /**< Flag indicating the state of the monitoring conditions for the VPOS supply */
extern enum vmi_supply_status_e    vmi_vneg_status;                                 /**< Flag indicating the state of the monitoring conditions for the VNEG supply */
extern enum vmi_supply_status_e    vmi_12v_power_status;                            /**< Flag indicating the state of the monitoring conditions for the HVIL supply */
extern enum vmi_supply_status_e    vmi_12v_logic_status;                            /**< Flag indicating the instantaneous state of the monitoring conditions for the 12v supply */
extern enum vmi_supply_status_e    vmi_12v_logic_obd_status;                        /**< Flag indicating the state of the monitoring conditions (with hysteresis) for the 12v supply */
extern enum vmi_supply_status_e    vmi_5v_ctc_feedback_status;                      /**< Flag indicating the instantaneous state of the monitoring conditions for the 5v contactor position feedback supply */


extern CAL pack_v_t                vmi_hv_stable_tolerance;                         /**< Tolerance for detection of errors on the HV bus, when the voltage is stable} */
extern CAL pack_v_t                vmi_hv_unstable_tolerance;                       /**< Tolerance for detection of errors on the HV bus, when the voltage is unstable} */
extern CAL pack_v_t                vmi_hv_hi_res_0v_tolerance;                      /**< Tolerance for detection of high res HV bus discharge} */
extern CAL pack_v_t                vmi_hv_lo_res_0v_tolerance;                      /**< Tolerance for detection of low res HV bus discharge} */

extern CAL struct vmi_supplies_calibration_s vmi_cal_isa_12v_sensor; /**< Coefficient used in determing the 12V voltage (HW/SW Spec section 2.1.7)} */
extern CAL struct vmi_supplies_calibration_s vmi_cal_vtbm_can_5v;    /**< Coefficient used in determing the VTBM CAN voltage (HW/SW Spec section 2.1.12)} */
extern CAL struct vmi_supplies_calibration_s vmi_cal_vtbm_can_gnd;   /**< Coefficient used in determing the VTBM CAN voltage (HW/SW Spec section 2.1.12)} */
extern CAL struct vmi_supplies_calibration_s vmi_cal_12v_pos;        /**< Coefficient used in determing the VPOS 12V voltage (HW/SW Spec section 2.1.16) */
extern CAL struct vmi_supplies_calibration_s vmi_cal_12v_neg;        /**< Coefficient used in determing the VNEG 12V voltage (HW/SW Spec section 2.1.15) */
extern CAL struct vmi_supplies_calibration_s vmi_cal_12v_power;      /**< Coefficient used in determing the HVIL voltage (HW/SW Spec section 2.1.6)} */
extern CAL struct vmi_supplies_calibration_s vmi_cal_12v_logic;      /**< Coefficient used in determing the HVIL voltage (HW/SW Spec section 2.1.6)} */
extern CAL struct vmi_supplies_calibration_s vmi_cal_5v_cur;      /**< Coefficient used in determing the HVIL voltage (HW/SW Spec section 2.1.6)} */
extern CAL struct vmi_supplies_calibration_s vmi_cal_5v_ctc_feedback;      /**< Coefficient used in determing the HVIL voltage (HW/SW Spec section 2.1.6)} */

extern CAL supply_v_t vmi_12v_power_monitoring_lower_limit;          /**< Lower level for the HVIL} */
extern CAL supply_v_t vmi_12v_power_optimal_voltage;                 /**< Acceptable level for the HVIL} */
extern CAL supply_v_t vmi_12v_power_tolerance;                       /**< Tolerance for detection of errors on the HVIL} */
extern CAL supply_v_t vmi_12v_logic_lower_limit;                     /**< Lower level for acceptable 12V battery supply} */
extern CAL supply_v_t vmi_12v_logic_lower_hyst;                      /**< Lower level hysteresis for healing 12V battery supply under-voltage */
extern CAL ts_time_t  vmi_12v_logic_lo_test_period;                  /**< Test period for 12V battery supply under-voltage events */
extern CAL supply_v_t vmi_12v_logic_upper_limit;                     /**< Upper level for acceptable 12V battery supply} */
extern CAL supply_v_t vmi_12v_logic_upper_hyst;                      /**< Upper level hysteresis for healing 12V battery supply over-voltage */
extern CAL ts_time_t  vmi_12v_logic_hi_test_period;                  /**< Test period for 12V battery supply over-voltage events */
extern CAL supply_v_t vmi_isa_supply_lower_limit;                    /**< Lower level for acceptable ISA supply} */
extern CAL supply_v_t vmi_isa_supply_upper_limit;                    /**< Upper level for acceptable ISA supply} */


#ifdef PRODUCTION
extern CAL uint32_t   vmi_prod_cal_s12;
extern CAL uint32_t   vmi_prod_cal_shared;
#endif

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/* documentation in source file */
FCODE void vmi_init(void);

/* documentation in source file */
FCODE void vmi_read_inputs(void);

#endif      /* VMI_H */
