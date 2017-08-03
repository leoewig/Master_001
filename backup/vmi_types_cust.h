/**
 *
 * Voltage Measurement Interface types
 *
 * @file
 * @ingroup vmi
 * $Id: vmi_types_cust.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2013-2015 - All rights reserved */

#if !defined(VMI_TYPES_CUST_H)
#define      VMI_TYPES_CUST_H

#include "basic_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

#define VMI_SUPPLY_5V            SUPPLY_V(  5.0)
#define VMI_SUPPLY_12V           SUPPLY_V( 12.0)
#define VMI_SUPPLY_MINUS_12V     SUPPLY_V(-12.0)

#define VMI_ACTIVE_BUS_V         (vmi_hv_hi_res_data [vmi_active_bus].voltage)           /**< Allows external feature access into VMI data structures */
#define VMI_ACTIVE_BUS_V_VALID   (vmi_hv_hi_res_data [vmi_active_bus].voltage_validity)  /**< Allows external feature access into VMI data structures */

#define VMI_CHG_BUS_V            (vmi_hv_hi_res_data [Vmi_charger_bus].voltage)          /**< Allows external feature access into VMI data structures */
#define VMI_CHG_BUS_V_VALID      (vmi_hv_hi_res_data [Vmi_charger_bus].voltage_validity) /**< Allows external feature access into VMI data structures */
#define VMI_CHG_NEG_V            (vmi_hv_hi_res_data_neg [Vmi_charger_bus].voltage)      /**< Allows external feature access into VMI data structures */
#define VMI_CHG_POS_V            (vmi_hv_hi_res_data_pos [Vmi_charger_bus].voltage)      /**< Allows external feature access into VMI data structures */

#define VMI_VEH_BUS_V            (vmi_hv_hi_res_data [Vmi_vehicle_bus].voltage)          /**< Allows external feature access into VMI data structures */
#define VMI_VEH_BUS_V_VALID      (vmi_hv_hi_res_data [Vmi_vehicle_bus].voltage_validity) /**< Allows external feature access into VMI data structures */
#define VMI_VEH_NEG_V            (vmi_hv_hi_res_data_neg [Vmi_vehicle_bus].voltage)      /**< Allows external feature access into VMI data structures */
#define VMI_VEH_POS_V            (vmi_hv_hi_res_data_pos [Vmi_vehicle_bus].voltage)      /**< Allows external feature access into VMI data structures */

#define VMI_LOW_RES_BUS_V        (vmi_hv_lo_res_data [Vmi_overall].voltage)              /**< Allows external feature access into VMI data structures */
#define VMI_LOW_RES_BUS_V_VALID  (vmi_hv_lo_res_data [Vmi_overall].voltage_validity)     /**< Allows external feature access into VMI data structures */
#define VMI_LOW_RES_BUS_0V       (vmi_hv_lo_res_data [Vmi_overall].hv_0v)                /**< Allows external feature access into VMI data structures */
#define VMI_LOW_RES_BUS_0V_VALID (vmi_hv_lo_res_data [Vmi_overall].hv_0v_validity)       /**< Allows external feature access into VMI data structures */

#define VMI_LOW_RES_NEG_V        (vmi_hv_lo_res_data [Vmi_negative].voltage)             /**< Allows external feature access into VMI data structures */
#define VMI_LOW_RES_NEG_V_VALID  (vmi_hv_lo_res_data [Vmi_negative].voltage_validity)    /**< Allows external feature access into VMI data structures */
#define VMI_LOW_RES_NEG_0V       (vmi_hv_lo_res_data [Vmi_negative].hv_0v)               /**< Allows external feature access into VMI data structures */
#define VMI_LOW_RES_NEG_0V_VALID (vmi_hv_lo_res_data [Vmi_negative].hv_0v_validity)      /**< Allows external feature access into VMI data structures */

#define VMI_LOW_RES_POS_V        (vmi_hv_lo_res_data [Vmi_positive].voltage)             /**< Allows external feature access into VMI data structures */
#define VMI_LOW_RES_POS_V_VALID  (vmi_hv_lo_res_data [Vmi_positive].voltage_validity)    /**< Allows external feature access into VMI data structures */
#define VMI_LOW_RES_POS_0V       (vmi_hv_lo_res_data [Vmi_positive].hv_0v)               /**< Allows external feature access into VMI data structures */
#define VMI_LOW_RES_POS_0V_VALID (vmi_hv_lo_res_data [Vmi_positive].hv_0v_validity)      /**< Allows external feature access into VMI data structures */

#define VMI_ISA_12V__V           (vmi_supply_data [Vmi_isa_12v].voltage)                 /**< Allows external feature access into VMI data structures */
#define VMI_VTBM_CAN_5V_V        (vmi_supply_data [Vmi_vtbm_can_5v].voltage)             /**< Allows external feature access into VMI data structures */
#define VMI_VTBM_CAN_GND_V       (vmi_supply_data [Vmi_vtbm_can_gnd].voltage)            /**< Allows external feature access into VMI data structures */
#define VMI_VPOS_12V_V           (vmi_supply_data [Vmi_vpos_12v].voltage)                /**< Allows external feature access into VMI data structures */
#define VMI_VNEG_12V_V           (vmi_supply_data [Vmi_vneg_12v].voltage)                /**< Allows external feature access into VMI data structures */
#define VMI_12V_POWER_V          (vmi_supply_data [Vmi_12v_power].voltage)               /**< Allows external feature access into VMI data structures */
#define VMI_12V_LOGIC_V          (vmi_supply_data [Vmi_12v_logic].voltage)               /**< Allows external feature access into VMI data structures */
#define VMI_12V_LOGIC_V_VALID    (vmi_supply_data [Vmi_12v_logic].voltage_validity)      /**< Allows external feature access into VMI data structures */

#define VMI_HI_RES_SUPPLY_V_BP   (13) /**< The number of binary points in vmi_hi_res_supply_v_t */
#define VMI_HI_RES_SUPPLY_V(v)   (BP_SCALE((v), vmi_hi_res_supply_v_t, VMI_HI_RES_SUPPLY_V_BP))

#define VMI_HI_RES_BUS_V_BP      (13) /**< The number of binary points in vmi_hi_res_bus_v_t */
#define VMI_HI_RES_BUS_V(v)      (BP_SCALE((v), vmi_hi_res_bus_v_t,    VMI_HI_RES_BUS_V_BP))

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/**
 * Enumeration to allow access for the individual rails into the
 * various data structures of this feature */
enum vmi_rail_e
{
    Vmi_overall,
    Vmi_positive,
    Vmi_negative,
    Vmi_last_rail
};

/**
 * Enumeration to allow access for individual supplies to the
 * various data structures of this feature */
enum vmi_supply_e
{
    Vmi_isa_12v = 0,
    Vmi_vpos_12v,
    Vmi_vneg_12v,
    Vmi_12v_power,
    Vmi_12v_logic,
    Vmi_5v_cur,
    Vmi_5v_ctc_feedback,
    Vmi_last_supply
};

/**
 * Enumeration to allow access for individual supplies to the
 * various data structures of this feature */
enum vmi_bus_e
{
    Vmi_vehicle_bus = 0,
    Vmi_charger_bus,
    Vmi_last_bus
};

/**
 * Enumeration to allow determination of the current state of the HV level */
enum vmi_hv_0v_e
{
    Vmi_zero = 0,
    Vmi_non_zero
};

/**
 * Enumeration for the possible states of a supply status */
enum vmi_supply_status_e
{
    Vmi_supply_ok = 0,
    Vmi_supply_high,
    Vmi_supply_low,
    Vmi_supply_sensor_fault
};

/** Structure containing all of the data describing the current state of a voltage rail */
struct vmi_hv_rail_s
{
    pack_v_t         voltage;
    data_validity_e  voltage_validity;
    enum vmi_hv_0v_e hv_0v;
    data_validity_e  hv_0v_validity;
};

/** Structure containing all of the data describing the current state of a supply voltage  */
struct vmi_hv_supply_s
{
    supply_v_t       voltage;
    data_validity_e  voltage_validity;
};

#endif      /* VMI_TYPES_H */
