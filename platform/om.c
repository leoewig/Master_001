/**
 *
 * Routines for controlling the Output Monitoring Module.
 *
 * @file
 * @ingroup OM
 * $Id: om.c 12608 2015-02-26 12:44:21Z pr4 $
 *
*/

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "om.h"     /* Public Header File for module */
#include "om_p.h"   /* Private Header File for module */
#include "fm.h"     /* Fault Logging */
//#include "vmi.h"    /* For HVIL status */
#include "dio.h"    /* For DIO outputs */
#include "dio_cfg.h"/* For reading data from ports */
#include "ai.h"     /* For access to analogue inputs */
#include "ms.h"     /* For current state */
//#include "imi.h"    /* For DC Link current */
//#include "vmi.h"    /* for hvil status */

struct om_sensor_data_s om_precharge_current = {AMPS(0.0), New_data_available, AMPS(0.0), No_data_available, AMPS(0.0), No_data_available};
struct om_sensor_data_s om_veh_pos_current   = {AMPS(0.0), New_data_available, AMPS(0.0), No_data_available, AMPS(0.0), No_data_available};
struct om_sensor_data_s om_veh_neg_current   = {AMPS(0.0), New_data_available, AMPS(0.0), No_data_available, AMPS(0.0), No_data_available};
struct om_sensor_data_s om_chg_pos_current   = {AMPS(0.0), New_data_available, AMPS(0.0), No_data_available, AMPS(0.0), No_data_available};
struct om_sensor_data_s om_chg_neg_current   = {AMPS(0.0), New_data_available, AMPS(0.0), No_data_available, AMPS(0.0), No_data_available};

bool_t om_cont_precharge_valid = FALSE;
bool_t om_cont_veh_pos_valid   = FALSE;
bool_t om_cont_veh_neg_valid   = FALSE;
bool_t om_cont_chg_pos_valid   = FALSE;
bool_t om_cont_chg_neg_valid   = FALSE;

bool_t om_isa_supply_fault = FALSE;

/******************************************************************************
 * PRIVATE VARIABLES DEFINITIONS
 *****************************************************************************/

/** Array of fault types for the diagnostic input signals. The order of the
 * variables MUST be as per the sequence in the enum #Om_diagnostic_input_type_e.
 */
PRIVATE CONST Fm_fault_list_e om_diag_fault_types[Om_max_diag_inputs] =
{
    Fm_plat_veh_hv_pre_driver,
    Fm_plat_veh_hv_pos_driver,
    Fm_plat_veh_hv_neg_driver,
    Fm_plat_charger_hv_pos_driver,
    Fm_plat_charger_hv_neg_driver
};

/** Array of pointers to the HS readback signals which indicate that the monitoring of
 * diagnostic relating to all contactors is enabled. The order of the variables MUST be
 * as per the sequence in the enum #Om_diagnostic_input_type_e.
 * These are used to check whether the diagnostic inputs have been
 * successfully enabled.
 */
PRIVATE CONST bool_t *om_enabled_hs[Om_max_diag_inputs] =
{
    &do_rb_pre_chg_hs_enbl,         /**< Pin status of pre-charge CTC o/p. */
    &do_rb_veh_dc_pos_hs_mmc,       /**< Pin status of Vehicle HV Bus +ve CTC o/p. */
    &do_rb_veh_dc_neg_hs_mmc,       /**< Pin status of Vehicle HV Bus -ve CTC o/p. */
    &do_rb_chg_dc_pos_hs_mmc,       /**< Pin status of Charger HV Bus +ve CTC o/p. */
    &do_rb_chg_dc_neg_hs_mmc        /**< Pin status of Charger HV Bus -ve CTC o/p. */
};

/** Array of pointers to the LS readback signals which indicate that the monitoring of
 * diagnostic relating to all contactors is enabled. The order of the variables MUST be
 * as per the sequence in the enum #Om_diagnostic_input_type_e.
 * These are used to check whether the diagnostic inputs have been
 * successfully enabled.
 */
PRIVATE CONST bool_t *om_enabled_ls[Om_max_diag_inputs] =
{
    &do_rb_pre_chg_ls_enbl,         /**< Pin status of pre-charge CTC o/p. */
    &do_rb_veh_dc_pos_ls_mmc,       /**< Pin status of Vehicle HV Bus +ve CTC o/p. */
    &do_rb_veh_dc_neg_ls_mmc,       /**< Pin status of Vehicle HV Bus -ve CTC o/p. */
    &do_rb_chg_dc_pos_ls_mmc,       /**< Pin status of Charger HV Bus +ve CTC o/p. */
    &do_rb_chg_dc_neg_ls_mmc        /**< Pin status of Charger HV Bus -ve CTC o/p. */
};
  
/** Array of pointers to the platform variables containing the current value of the ADC inputs */
//PRIVATE CONST sensor_address_t om_adc_inputs[Om_max_diag_inputs] =
//{
//    &hld_adc_pre_charge_diag,    /**< Diagnostic feedback for Vehicle HV Bus pre-charge CTC HS o/p. */
//    &hld_adc_veh_diag_positive,  /**< Diagnostic feedback for Vehicle HV Bus +ve CTC HS o/p. */
//    &hld_adc_veh_diag_negative,  /**< Diagnostic feedback for Vehicle HV Bus -ve CTC HS o/p. */
//    &hld_adc_chg_diag_positive,  /**< Diagnostic feedback for Charger HV Bus +ve CTC HS o/p. */
//    &hld_adc_chg_diag_negative   /**< Diagnostic feedback for Charger HV Bus -ve CTC HS o/p. */
//};

/** Array of pointers to the variables containing the current value of the calculated currents
 * for the diagnostic inputs */
PRIVATE struct om_sensor_data_s * CONST om_diag_currents[Om_max_diag_inputs] =
{
    &om_precharge_current,
    &om_veh_pos_current,
    &om_veh_neg_current,
    &om_chg_pos_current,
    &om_chg_neg_current
};

/** Array of pointers to the flags containing the current validity of the calculated currents
 * for the diagnostic inputs */
PRIVATE bool_t * CONST om_validity_flags[Om_max_diag_inputs] =
{
    &om_cont_precharge_valid,
    &om_cont_veh_pos_valid,
    &om_cont_veh_neg_valid,
    &om_cont_chg_pos_valid,
    &om_cont_chg_neg_valid
};

/** Array containing the last read value of the raw ADC (for test purposes) */
uint16_t om_adc_values[Om_max_diag_inputs] =
{
    0u,
    0u,
    0u,
    0u,
    0u
};
/******************************************************************************
 * PRIVATE FUNCTION DECLARATIONS
 *****************************************************************************/

/******************************************************************************
 * PRIVATE FUNCTION DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * This function monitors the state of discrete digital outputs with the
 * commanded values. It monitors only those outputs which have been configured as
 * Dio_interface_function. If the commanded state doesn't matches the read-back state
 * it updates the fault filter with fault type as Fm_plat_uncommanded_output_state.
 ******************************************************************************/
PRIVATE void om_monitor_digital_outputs(void)
{
 //   uint8_t i           = 0u;
 //   bool_t  flt_present = FALSE;
 //
 //   /* Monitor the discrete digital output commanded pin state vs the actual pin state. */
 //   for (i = 0u; i < dio_n_outputs; i++)
 //   {
 //       if(dio_output_cfg[i].interface == Dio_interface_function)
 //       {
 //           /** NB This test must take into account
 //            * -we cannot use the commanded level, as thuis may have been changed within this tick
 //            * -therefore we must use the pin level, accounting for active high/low
 //            */
 //           if (dio_output_cfg[i].active_high_low == Dio_active_high)
 //           {
 //               if (*(dio_output_cfg[i].read_back) != *(dio_output_cfg[i].dmd))
 //               {
 //                   flt_present = TRUE;
 //               }
 //           }
 //           else
 //           {
 //               if (*(dio_output_cfg[i].read_back) == *(dio_output_cfg[i].dmd))
 //               {
 //                   flt_present = TRUE;
 //               }
 //           }
 //       }
 //   }
 //
 //   (void) fm_update_fault_filter(
 //              Fm_plat_uncommanded_output_state,
 //              Fm_general_failure_information,
 //              flt_present,
 //              TRUE);
}

/******************************************************************************
 * This function performs the fault detection of the Contactor Diagnostic inputs
 * and sets the #om_validity_flags accordingly. NB Faults are only detected if both
 * the high-side and low-side drivers are at the same state.
 ******************************************************************************/
PRIVATE void om_validate_input(enum Om_diagnostic_input_type_e diag_input)
{
 //   bool_t validity = TRUE;
 //
 //   if ((*om_enabled_hs [diag_input] == FALSE)
 //   &&  (*om_enabled_ls [diag_input] == FALSE))
 //   {
 //       if (om_adc_values[diag_input] > OM_OFF_OPEN_LOAD)
 //       {
 //           validity = FALSE;
 //       }
 //   }
 //   else
 //   {
 //       if ((*om_enabled_hs [diag_input] == TRUE)
 //       &&  (*om_enabled_ls [diag_input] == TRUE))
 //       {
 //           if (om_adc_values[diag_input] < OM_ON_OPEN_LOAD)
 //           {
 //               validity = FALSE;
 //           }
 //       }
 //   }
 //
 //   *om_validity_flags[diag_input] = validity;
}


/******************************************************************************
 * The om_update_diagnostics_validity() function provides an interface to
 * monitor diagnostic inputs associated with all used contactors.
 *
 * If any fault status representing the diagnostic inputs are set and the HVIL
 * status indicates that HVIL is intact then a corresponding fault filter is
 * applied by calling the function fm_update_fault_filter().
 ******************************************************************************/
PRIVATE void om_fault_filter_contactor_diagnostics()
{
 //   bool_t                  mon_cond_met = FALSE;
 //   bool_t                  flt_present  = FALSE;
 //   Fm_fault_list_e         flt_type;
 //   enum Om_diagnostic_input_type_e i;
 //
 //   for (i = (enum Om_diagnostic_input_type_e) 0u; i < Om_max_diag_inputs; i++)
 //   {
 //       /* Get the current fault type */
 //       flt_type = om_diag_fault_types[i];
 //
 //       /* Monitor Condition */
 //      // mon_cond_met = (Vmi_supply_ok == vmi_12v_power_status);
 //
 //       /* Fault is present if the variable value is FALSE. */
 //       flt_present = (*om_validity_flags[i] != TRUE);
 //
 //       (void) fm_update_fault_filter(
 //                  flt_type,
 //                  Fm_general_failure_information,
 //                  flt_present,
 //                  mon_cond_met);
 //   }
}

/******************************************************************************
 * Function to convert from ADC counts to diagnostic input current. The type
 * #om_diag_v_t is used in the interim steps to try to maintain as much accuracy
 * in the calculations as possible.
 *
 * See the HW/SW Specification for complete details of the equation.
 ******************************************************************************/
PRIVATE current_t om_adc_to_current(adc_v_t adc_value)
{
//    current_t i = AMPS(0.0);
//    om_diag_v_t v_temp1;
//    om_diag_v_t v_temp2;
//    om_diag_v_t v_adc;
//
//    v_adc = OM_DIAG_V(C_TO_V(adc_value));
//
//    v_temp1 = muls32(v_adc,   OM_A, OM_DIAG_V_BP, 0, OM_DIAG_V_BP);
//    v_temp2 = muls32(v_adc,   OM_C, OM_DIAG_V_BP, 0, OM_DIAG_V_BP);
//    v_temp2 = adds32(v_temp2, OM_B, OM_DIAG_V_BP, 0, OM_DIAG_V_BP);
//
//    i = divs32(v_temp1, v_temp2, OM_DIAG_V_BP, OM_DIAG_V_BP, CURRENT_BP);
//
//    return i;
}

/**
 * Store all data required to determine contactor wear
 * @param[in] sensor - The sensor whose data is to be captured
 */
PRIVATE void om_update_contactor_wear_data(enum Om_diagnostic_input_type_e sensor)
{
//    uint32_t i_now;
//    uint32_t i_then;
//
//    if (*om_validity_flags[sensor] == FALSE)
//    {
//        /* Fault condition is present */
//        if (imi_latest_data.latest_mA_validity == New_data_available)
//        {
//            /* Use absolute value for comparison */
//            i_now  = abs32(imi_latest_data.latest_mA);
//        }
//        else
//        {
//            i_now = abs32(AMPS(0.0));
//        }
//
//        if (om_diag_currents[sensor]->previous_i_valid == New_data_available)
//        {
//            /* Use absolute value for comparison */
//            i_then  = abs32(om_diag_currents[sensor]->previous_i);
//        }
//        else
//        {
//            i_then = abs32(AMPS(0.0));
//        }
//
//        /* Only update on first detection of fault */
//        if (om_diag_currents[sensor]->fault_i_valid == No_data_available)
//        {
//            /* Update the fault current, for use in detecting contactor wear */
//            if ((om_diag_currents[sensor]->previous_i_valid == New_data_available)
//            ||  (imi_latest_data.latest_mA_validity         == New_data_available))
//            {
//                if (i_now > i_then)
//                {
//                    om_diag_currents[sensor]->fault_i_mA = imi_latest_data.latest_mA;
//
//                    SET_VALIDITY(om_diag_currents[sensor]->fault_i_valid);
//                }
//                else
//                {
//                    om_diag_currents[sensor]->fault_i_mA = om_diag_currents[sensor]->previous_i;
//
//                    SET_VALIDITY(om_diag_currents[sensor]->fault_i_valid);
//                }
//            }
//        }
//        else
//        {
//            /* Fault has already been detected */
//            CLEAR_VALIDITY(om_diag_currents[sensor]->fault_i_valid);
//        }
//    }
//    else
//    {
//        /* Fault condition is not present */
//        //if (vmi_12v_power_status == Vmi_supply_ok)
//        //{
//        //    om_diag_currents[sensor]->fault_i_valid = No_data_available;
//        //}
//    }
}

/******************************************************************************
 * The om_read_diagnostics_inputs() function provides an interface to
 * read the ADC values of the diagnostic inputs (if available from AI) and to convert
 * them from ADC counts into currents (mA).
 ******************************************************************************/
PRIVATE void om_read_diagnostics_inputs()
{
 //   enum Om_diagnostic_input_type_e i;
 //
 //   for (i = (enum Om_diagnostic_input_type_e) 0u; i < Om_max_diag_inputs; i++)
 //   {
 //       om_adc_values[i] = *om_adc_inputs[i];
 //
 //       /* Check for faults on the input */
 //       om_validate_input(i);
 //
 //       if (*om_validity_flags[i] == TRUE)
 //       {
 //           /* Only update the current if the driver is fault free */
 //           om_diag_currents[i]->feedback_i_mA = om_adc_to_current(om_adc_values[i]);
 //
 //           SET_VALIDITY(om_diag_currents[i]->feedback_i_valid);
 //       }
 //       else
 //       {
 //           CLEAR_VALIDITY(om_diag_currents[i]->feedback_i_valid);
 //       }
 //
 //       om_update_contactor_wear_data(i);
 //
 //       /* Store the current flowing, for the next pass */
 //       om_diag_currents[i]->previous_i       = imi_latest_data.latest_mA;
 //       om_diag_currents[i]->previous_i_valid = imi_latest_data.latest_mA_validity;
 //   }
}

/******************************************************************************
 * The om_shutdown_actions() function provides an interface to control the
 * OM feature when the MMC is shutting down. At present this consists of
 * -informing MS that OM shutdown is complete.
 ******************************************************************************/
PRIVATE void om_shutdown_actions()
{
    /* Indicate to MS that OM shutdown actions are complete */
//    ms_update_shutdown_state(ms_inhibit_om, Ms_shutdown_allowed);
}

/***************************************************************************
 * The om_running_actions() function provides an interface to control the
 * OM feature when the MMC is not shutting down. At present this consists of
 * -detecting transitons of the HVIL status
 * -monitoring the contactor diagnostics
 * -monitoring the state of the digital outputs
 * -reading the current ISA supply status and making this available to the MMC
 ******************************************************************************/
PRIVATE void om_running_actions()
{
//    om_read_diagnostics_inputs();
//
//    om_fault_filter_contactor_diagnostics();
//
//    /* Monitor the discrete digital output pins */
//    om_monitor_digital_outputs();
//
//    /* Update the instantaneous ISA supply status */
//    om_isa_supply_fault = di_isa_diag;
}

 /****************************************************************************
 * PUBLIC FUNCTION DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * The om_init() function provides an interface to initialise the OM feature.
 * At present, no specific initialisation is required.
 ******************************************************************************/
void om_init(void)
{
}

/******************************************************************************
 * The om_read_inputs() function provides overall control of the OM feature
 * when invoked by the Scheduler.
 ******************************************************************************/
void om_read_inputs(void)
{
   // if (ms_shutdown_pending == FALSE)
   // {
   //     om_running_actions();
   // }
   // else
   // {
   //     /* MS is requesting a shutdown */
   //     om_shutdown_actions();
   // }
}

/******************************************************************************
 * Collect OM high side diagnostic input enable state required by OBD snapshots.
 * 
 * @param diag_ip Diagnostic input of interest
 ******************************************************************************/
bool_t om_ss_data_hs_diag_en_state(enum Om_diagnostic_input_type_e diag_ip)
{
 //   bool_t retval = FALSE;
 //   if (diag_ip < Om_max_diag_inputs)
 //   {
 //       retval = *om_enabled_hs[diag_ip];
 //   }
 //   return retval;
}

/******************************************************************************
 * Collect OM low side diagnostic input enable state required by OBD snapshots.
 * 
 * @param diag_ip Diagnostic input of interest
 ******************************************************************************/
bool_t om_ss_data_ls_diag_en_state(enum Om_diagnostic_input_type_e diag_ip)
{
//    bool_t retval = FALSE;
//    if (diag_ip < Om_max_diag_inputs)
//    {
//        retval = *om_enabled_ls[diag_ip];
//    }
//    return retval;
}

/******************************************************************************
 * Collect OM diagnostic input current data required by OBD snapshots.
 * 
 * @param diag_ip Diagnostic input of interest
 * @param current Pointer to a location in which the current measurement is
 *                to be returned
 * @param validity Pointer to a location in which the current measurement 
 *                 validity is to be returned
 ******************************************************************************/
void om_ss_data_diag_current(enum Om_diagnostic_input_type_e diag_ip, 
                             current_t *current, data_validity_e *validity)
{
 //   if (diag_ip < Om_max_diag_inputs)
 //   {
 //      *current = om_diag_currents[diag_ip]->feedback_i_mA;
 //      *validity = om_diag_currents[diag_ip]->feedback_i_valid;
 //   }
}
