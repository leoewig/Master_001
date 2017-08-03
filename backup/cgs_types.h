#if !defined(CGS_TYPES_H)
#define      CGS_TYPES_H
/*************************************************************************//**
 *
 * Public header for Charge Strategy Feature (cgs)
 *
 * Copyright (c) Ricardo plc 2011-2015 - All rights reserved
 *
 * @file
 * @ingroup cgs
 * $Id: cgs_types.h 873 2015-03-16 11:19:41Z j_bailey $
 *
 *****************************************************************************
 *
 * This header defines the public interface to the cgs module
 *
 *****************************************************************************/

 /************************************************************************//**
 * Options for describing the state of the charging CCO and CHG state
 * machines, or the reason for not charging (if in #Cgs_sm_cco_decision).
 *
 * It is used for #cgs_state and #cgs_last_result.
 * - #cgs_state is a live indication of the current state.
 * - #cgs_last_result is only updated just before the contactors open.
 *
 *****************************************************************************/
enum  cgs_state_e {
    /* The first word in the comments is the state of the cgs_sm_cco_state
       state machine */
    /* Values from cgs_commence_chg(), cgs_continue_chg() here. */
    Cgs_s_nc_ok,         /**< 0  Cgs_sm_cco_decision: Ok to charge */
    Cgs_s_nc_no_dmd,     /**< 1  Cgs_sm_cco_decision: Charging is not demanded */
    Cgs_s_nc_cell_t_na,  /**< 2  Cgs_sm_cco_decision: cell temperatures not available */
    Cgs_s_nc_cell_v_na,  /**< 3  Cgs_sm_cco_decision: cell voltages not available */
    Cgs_s_nc_v_invalid,  /**< 4  Cgs_sm_cco_decision: voltage on the charger bus invalid */
    Cgs_s_nc_v_not_zero, /**< 5  Cgs_sm_cco_decision: voltage on the charger bus not zero */
    Cgs_s_nc_in_prog,    /**< 6  Cgs_sm_cco_decision: Charging already in progress */
    Cgs_s_nc_v_avail,    /**< 7  Cgs_sm_cco_decision: Charger voltage available low */
    Cgs_s_nc_pp_low,     /**< 8  Cgs_sm_cco_decision: Power avail less then Power/ prediction limits */
    Cgs_s_nc_cooldown,   /**< 9  Cgs_sm_cco_decision: cool down timer not expired */
    Cgs_s_nc_cell_t,     /**< 10 Cgs_sm_cco_decision: cell temperature outside boundary */
    Cgs_s_nc_cell_v,     /**< 11 Cgs_sm_cco_decision: cell voltage outside boundary */
    Cgs_s_nc_hvil,       /**< 12 Cgs_sm_cco_decision: HVIL not intact */
    Cgs_s_nc_chg_inh,    /**< 13 Cgs_sm_cco_decision: Charge inhibition fault reaction active */
    Cgs_s_nc_no_tether,  /**< 14 Cgs_sm_cco_decision: No tether */
    Cgs_s_nc_not_present,/**< 15 Cgs_sm_cco_decision: Charger not present */
    Cgs_s_nc_not_ok_chg, /**< 16 Cgs_sm_cco_decision: Not ok to charge */
    Cgs_s_nc_ct_open,    /**< 17 Cgs_sm_cco_decision: Contactors open */
    Cgs_s_nc_ct_fault,   /**< 18 Cgs_sm_cco_decision: Contactor fault */
    Cgs_s_nc_current,    /**< 19 Cgs_sm_cco_decision: Current reading not valid */
    Cgs_s_not_poss,      /**< 20 Cgs_sm_cco_not_poss: Charging not possible */
    Cgs_s_chg_shutdown,  /**< 21 Cgs_sm_cco_chg_shutdown - Shutdown the charger*/
    Cgs_s_hv,            /**< 22 Cgs_sm_cco_hv: Charger HV connection */
    Cgs_s_hv_timeout,    /**< 23 Cgs_sm_cco_hv: timeout */
    Cgs_s_v_eq,          /**< 24 Cgs_sm_cco_v_eq: Performing Voltage Equalisation */
    Cgs_s_v_eq_ab_to,    /**< 25 Cgs_sm_cco_v_eq: Aborted due to time out */
    Cgs_s_cc,            /**< 26 Cgs_sm_cco_ct_closure: Contactor Closure */
    Cgs_s_cc_to,         /**< 27 Cgs_sm_cco_ct_closure: Aborted due to time out*/
    Cgs_s_chg_ok,        /**< Cgs_sm_cco_charging: Charging ok */
    Cgs_s_chg_to,        /**< Cgs_sm_cco_charging: Too long charging */
    Cgs_s_chg_isol,       /**< Cgs_sm_cco_charging: Charger isolated outputs */
    Cgs_s_v_avail,       /**< Cgs_sm_cco_charging: Charger voltage available low */
    Cgs_s_chg_unint,     /**< Cgs_sm_cco_charging: Un-initialised */
    Cgs_s_chg_v_ramp_up, /**< Cgs_sm_cco_charging: Voltage ramp up */
    Cgs_s_chg_i_ramp_up, /**< Cgs_sm_cco_charging: Current ramp up */
    Cgs_s_chg_i_const,   /**< Cgs_sm_cco_charging: Constant current */
    Cgs_s_chg_v_const,   /**< Cgs_sm_cco_charging: Constant voltage */
    Cgs_s_chg_completed, /**< Cgs_sm_cco_charging: Charging complete */
    Cgs_s_termination,   /**< Cgs_sm_cco_chg_term: Charging termination */
    Cgs_s_isolation,     /**< Cgs_sm_cco_isolation: Ensure isolation */
    Cgs_s_chg_stop,      /**< Cgs_sm_cco_stop: Stop Charging */
    Cgs_s_nc_supply_v_invalid, /**< Cgs_sm_cco_decision: 12V supply reading not valid */
    Cgs_s_nc_supply_v         /**< Cgs_sm_cco_decision: 12V supply reading not valid */
};

/****************************************************************************
 * Options for the state of the Charger Co-ordination state machine
 *****************************************************************************/
typedef enum {
  Cgs_sm_cco_start,      /**< Start the charging process */
  Cgs_sm_cco_decision,   /**< Evaluating Start of Charge Cycle */
  Cgs_sm_cco_not_poss,   /**< Charging not possible */
  Cgs_sm_cco_chg_shutdown,/**< Charger shutdown - shutdown the charger*/
  Cgs_sm_cco_hv,         /**< Charger HV connection */
  Cgs_sm_cco_v_eq,       /**< Voltage Equalisation */
  Cgs_sm_cco_ct_closure, /**< Pack Contactor Closure */
  Cgs_sm_cco_charging,   /**< Charging */
  Cgs_sm_cco_chg_term,   /**< Charging termination */
  Cgs_sm_cco_stop,       /**< Stop Charging- stop the charging process */
  Cgs_sm_cco_isolation   /**< Ensure isolation */
} cgs_sm_cco_e;

/**
 * Available charging termination reason
 */
enum cgs_term_reason_e
{
    Cgs_term_init,                    /**< Initialization value */
    Cgs_term_no_charger,              /**< Charging has been terminated due to missing
                                           charger */
    Cgs_term_completed_v_reached,     /**< Charging has been completed and cell voltage
                                           has been reached*/
    Cgs_term_completed_v_not_reached  /**< Charging has been completed but cell voltage
                                           has not been reached*/
};


#endif      /* CGS_TYPES_H */
