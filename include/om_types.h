/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    om_types.h
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
#if !defined( OM_TYPES_H )
#define       OM_TYPES_H
/**
 * @file
 * @ingroup OM
 * Header file for Output Monitoring Feature
 *
 */

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/
/** Type used for interim ADC -> mA calculations */
typedef int32_t om_diag_v_t;

#define OM_DIAG_V_BP 6
#define OM_DIAG_V(v) BP_SCALE((v), om_diag_v_t, OM_DIAG_V_BP)

/** Constants used in ADC -> mA equation, see HW/SW Specification for full equation NB includes mA scaling factor */
#define OM_A (   5880L * 1000L)
#define OM_B (  12000L)
#define OM_C (    160L)

#define OM_ON_OPEN_LOAD  V_TO_C(0.08)
#define OM_OFF_OPEN_LOAD V_TO_C(0.08)

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
/**
 * Enumeration to specify the digital fault diagnostic input.
 */
enum Om_diagnostic_input_type_e
{
    Om_cont_pre_charge,   /**< Enum const for Vehicle HV Bus pre-charge CTC status. */
    Om_cont_veh_pos,      /**< Enum const for Vehicle HV Bus +ve CTC status. */
    Om_cont_veh_neg,      /**< Enum const for Vehicle HV Bus -ve CTC status. */
    Om_cont_chg_pos,      /**< Enum const for Charger HV Bus +ve CTC status. */
    Om_cont_chg_neg,      /**< Enum const for Charger HV Bus -ve CTC status. */
    Om_max_diag_inputs    /* Should always be the last element.*/
};

/*
 * Structure for storing the high-side driver feedback current.
 */
struct om_sensor_data_s
{
    current_t       feedback_i_mA;
    data_validity_e feedback_i_valid;
    current_t       fault_i_mA;
    data_validity_e fault_i_valid;
    current_t       previous_i;
    data_validity_e previous_i_valid;
};

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

#endif /* OM_TYPES_H */
