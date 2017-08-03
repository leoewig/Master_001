/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     vmi_p.h
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
#if !defined(VMI_P_H)
#define      VMI_P_H
/**
 * @file
 * @ingroup vmi
 * $Id: vmi_p.h 11570 2014-01-08 17:56:06Z pr4 $
 */

/* Copyright (c) Linkedriving Tech 2011 - All rights reserved */

#include "vmi_types.h"
#include "fm.h"
#include "sensor_cal_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

#define VMI_ADC_INTERP_SIZE 2u

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern CONST uint16_t vmi_hv_adc_counts            [VMI_ADC_INTERP_SIZE];
extern CONST pack_v_t vmi_lo_res_veh_pos_adc_volts [VMI_ADC_INTERP_SIZE];
extern CONST pack_v_t vmi_lo_res_veh_neg_adc_volts [VMI_ADC_INTERP_SIZE];
/******************************************************************************
 * Interface to SMG - the sister-feature of VMI
 *****************************************************************************/

extern bool_t vmi_mon_cond_isa_12v;      /**< Flag indicating the whether monitoring conditions for the 12v supply are present */
extern bool_t vmi_mon_cond_vtbm_can_5v;  /**< Flag indicating the whether monitoring conditions for the VTBM CAN 5V supply are present */
extern bool_t vmi_mon_cond_vtbm_can_gnd; /**< Flag indicating the whether monitoring conditions for the VTBM CAN ground supply are present */
extern bool_t vmi_mon_cond_vpos;         /**< Flag indicating the whether monitoring conditions for the VPOS supply are present */
extern bool_t vmi_mon_cond_vneg;         /**< Flag indicating the whether monitoring conditions for the VNEG supply are present */
extern bool_t vmi_mon_cond_12v_power;    /**< Flag indicating the whether monitoring conditions for the HVIL supply are present */
extern bool_t vmi_mon_cond_12v_logic;    /**< Flag indicating the whether monitoring conditions for the 12v supply are present */

#ifdef UNIT_TEST
extern ts_time_t                           vmi_12v_logic_mon_ts;
#endif
/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/* documentation in source file */
extern  void                    vmi_read_hi_res_voltages(void);
extern  void                    vmi_read_lo_res_voltages(void);
extern  void                    vmi_read_supply_voltages(void);

extern  pack_v_t                vmi_calc_hi_res_rail_voltage     (uint16_t adc_value,FPTR sensor_cal_hv_adc_t CONST * cal);
extern  pack_v_t                vmi_calc_hi_res_rail_voltage_isol(uint16_t adc_value,FPTR sensor_cal_hv_adc_t CONST * cal);
extern  pack_v_t                vmi_calc_hi_res_bus_voltage      (pack_v_t v_pos, pack_v_t v_neg);

extern  pack_v_t                vmi_calc_lo_res_pos_voltage     (uint16_t adc_value);
extern  pack_v_t                vmi_calc_lo_res_neg_voltage     (uint16_t adc_value);
PRIVATE pack_v_t                vmi_calc_lo_res_voltage         (pack_v_t v_pos, pack_v_t v_neg);

PRIVATE supply_v_t              vmi_calc_supply_voltage            (enum vmi_supply_e supply);

PRIVATE void                    vmi_determine_hi_res_0v_status     (enum vmi_bus_e bus);
PRIVATE void                    vmi_determine_lo_res_0v_status     (enum vmi_rail_e rail);

extern  Fm_fault_subtype_list_e vmi_validate_bus_voltage
                                    (enum vmi_bus_e bus,
                                     pack_v_t  voltage,
                                     pack_v_t  voltage_pos,
                                     pack_v_t  voltage_neg,
                                     bool_t *  fault_present);

extern  bool_t                  vmi_open_circuit(
                                    pack_v_t voltage_in,
                                    pack_v_t tolerance);

extern  bool_t                  vmi_short_circuit(
                                    pack_v_t voltage_in,
                                    pack_v_t tolerance);

PRIVATE void                    vmi_update_isa_12v_status(void);
PRIVATE void                    vmi_update_vtbm_can_status(void);
PRIVATE void                    vmi_update_vpos_status(void);
PRIVATE void                    vmi_update_vneg_status(void);
PRIVATE void                    vmi_update_12v_power_status(void);
PRIVATE void                    vmi_update_12v_logic_status(void);
PRIVATE void                    vmi_update_12v_logic_obd_status(void);
PRIVATE void                    vmi_12v_logic_transition_settling_prd(enum vmi_12v_logic_mon_state_e state);

PRIVATE void                    vmi_fault_filter_12v_power(void);
PRIVATE void                    vmi_fault_filter_12v_logic(void);

PRIVATE Fm_fault_subtype_list_e vmi_convert_supply_status_to_fault_subtype(
                                    enum vmi_supply_status_e supply_status);

#endif /* VMI_P_H */
