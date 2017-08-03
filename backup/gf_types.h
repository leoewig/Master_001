/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   gf_types.h
 *
 * description:  Ground Fault Indication public header file
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
#if !defined(GF_TYPES_H)
#define      GF_TYPES_H
/**
 *
 * Ground Fault Indication public header file
 *
 * @file
 * @ingroup gf
 */
#include "mmc.h"
#include "ctc_types.h"
#include "fm_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

#define GF_LOOKUP_OR_SIZE                  34u  /**< Size of RES lookup tables */
#define GF_SYMMETRIC_PERIOD  (ts_time_t) 1000   /**< Number of millseconds between symmetric fault detection */

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

enum gf_symmetric_state_e
{
    Gf_symmetric_iso_det_off,
    Gf_symmetric_iso_det_on,
    Gf_symmetric_complete
};

enum gf_state_e
{
    Gf_state_contactors_open,
    Gf_state_asymmetric,
    Gf_state_symmetric_initial,
    Gf_state_symmetric_periodic,
    Gf_max_states
};

enum gf_bus_e
{
    Gf_bus_vehicle = 0,
    Gf_bus_charger,
    Gf_max_buses
};

struct gf_status_data_s
{
    bool_t                  updated_since_closure;
    data_validity_e         updated_since_closure_validity;
    bool_t                  fault_present;
    data_validity_e         fault_present_validity;
    bool_t                  fault_since_closure;
    data_validity_e         fault_since_closure_validity;
};

struct gf_data_s
{
    ohm_per_volt_t          pos_ohm_per_volt;
    pack_v_t                pos_voltage;
    ohm_per_volt_t          neg_ohm_per_volt;
    pack_v_t                neg_voltage;
};

struct gf_const_data_s
{
    pack_v_t *              inst_bal_lo_pos_rail_v;
    pack_v_t *              inst_bal_lo_neg_rail_v;
    pack_v_t *              inst_bal_pos_rail_v;
    pack_v_t *              inst_bal_neg_rail_v;
    pack_v_t *              inst_unbal_pos_rail_v;
    pack_v_t *              inst_unbal_neg_rail_v;

    pack_v_t *              stored_bal_lo_pos_rail_v;
    pack_v_t *              stored_bal_lo_neg_rail_v;
    pack_v_t *              stored_bal_pos_rail_v;
    pack_v_t *              stored_bal_neg_rail_v;
    pack_v_t *              stored_unbal_pos_rail_v;
    pack_v_t *              stored_unbal_neg_rail_v;

    bool_t *                contactor_position;
    bool_t *                iso_det;

    CONST Fm_fault_list_e   flt_code_asym_critical;
    CONST Fm_fault_list_e   flt_code_asym_non_critical;

    CONST Fm_fault_list_e   flt_code_sym_critical;
    CONST Fm_fault_list_e   flt_code_sym_non_critical;

    CONST Fm_fault_list_e   flt_code_asym_run_critical;
    CONST Fm_fault_list_e   flt_code_asym_run_non_critical;

    CONST Fm_fault_list_e   flt_code_sym_run_critical;
    CONST Fm_fault_list_e   flt_code_sym_run_non_critical;

    CAL ohm_t *             asym_ohm_pos;
    CAL ohm_t *             asym_ohm_neg;

    CAL ohm_t *             asym_ohm_pos_with_bleed;
    CAL ohm_t *             asym_ohm_neg_with_bleed;

    CAL ohm_t *             sym_ohm_pos;
    CAL ohm_t *             sym_ohm_neg;

    CONST ts_time_t         initial_wait;
};

#endif      /* GF_TYPES_H */
