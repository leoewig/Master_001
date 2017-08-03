/**
 *
 * Voltage Measurement interface
 *
 * @file
 * @ingroup vmi
 * $Id: vmi_cust.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2012-2015 - All rights reserved */

#if !defined(VMI_CUST_H)
#define      VMI_CUST_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "mmc.h"
#include "vmi_types_cust.h"

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
extern enum vmi_supply_status_e    vmi_vpos_status;                                 /**< Flag indicating the state of the monitoring conditions for the VPOS supply */
extern enum vmi_supply_status_e    vmi_vneg_status;                                 /**< Flag indicating the state of the monitoring conditions for the VNEG supply */
extern enum vmi_supply_status_e    vmi_12v_power_status;                            /**< Flag indicating the state of the monitoring conditions for the HVIL supply */
extern enum vmi_supply_status_e    vmi_12v_logic_status;                            /**< Flag indicating the instantaneous state of the monitoring conditions for the 12v supply */
extern enum vmi_supply_status_e    vmi_12v_logic_obd_status;                        /**< Flag indicating the state of the monitoring conditions (with hysteresis) for the 12v supply */
extern enum vmi_supply_status_e    vmi_5v_cur_status;                               /**< Flag indicating the instantaneous state of the monitoring conditions for the 5v current sensor supply */
extern enum vmi_supply_status_e    vmi_5v_ctc_feedback_status;                      /**< Flag indicating the instantaneous state of the monitoring conditions for the 5v contactor position feedback supply */

extern CAL pack_v_t                vmi_hv_stable_tolerance;                         /**< Tolerance for detection of errors on the HV bus, when the voltage is stable} */
extern CAL pack_v_t                vmi_hv_unstable_tolerance;                       /**< Tolerance for detection of errors on the HV bus, when the voltage is unstable} */
extern CAL pack_v_t                vmi_hv_hi_res_0v_tolerance;                      /**< Tolerance for detection of high res HV bus discharge} */
extern CAL pack_v_t                vmi_hv_lo_res_0v_tolerance;                      /**< Tolerance for detection of low res HV bus discharge} */

extern CAL struct vmi_supplies_calibration_s vmi_cal_isa_12v_sensor; /**< Coefficient used in determing the 12V voltage (HW/SW Spec section 2.1.7)} */
extern CAL struct vmi_supplies_calibration_s vmi_cal_12v_pos;        /**< Coefficient used in determing the VPOS 12V voltage (HW/SW Spec section 2.1.16) */
extern CAL struct vmi_supplies_calibration_s vmi_cal_12v_neg;        /**< Coefficient used in determing the VNEG 12V voltage (HW/SW Spec section 2.1.15) */
extern CAL struct vmi_supplies_calibration_s vmi_cal_12v_power;      /**< Coefficient used in determing the HVIL voltage (HW/SW Spec section 2.1.6)} */
extern CAL struct vmi_supplies_calibration_s vmi_cal_12v_logic;      /**< Coefficient used in determing the HVIL voltage (HW/SW Spec section 2.1.6)} */
extern CAL struct vmi_supplies_calibration_s vmi_cal_5v_cur_sensor;  /**< Coefficient used in determing the 5V current sensor voltage (HW/SW Spec section 2.1.11)} */
extern CAL struct vmi_supplies_calibration_s vmi_cal_5v_cfb_mon;     /**< Coefficient used in determing the 5V contactor position feedback supply voltage (HW/SW Spec section 2.1.17)} */

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

#endif      /* VMI_H */
