#if !defined(CGI_ONBC_P_H)
#define      CGI_ONBC_P_H
/**
 *
 * This file defines all of the prototypes used internally within the
 * on-board charger interface feature
 *
 * @file
 * @ingroup cgi
 * $Id: cgi_onbc_p.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2014-2015 - All rights reserved */

#include "cgi_types.h"
#include "mmc.h"


 /******************************************************************************
 * PRIVATE MACROS
 *****************************************************************************/

/** Shorthand macro for determining an invalid connection confirmation state */
#define CGI_ONBC_INVALID_CC  (bool_t)((cgi_onbc_cc_state == Cgi_onbc_cc_err_short_to_gnd) \
                                  ||  (cgi_onbc_cc_state == Cgi_onbc_cc_err_unexpected) \
                                  ||  (cgi_onbc_cc_state == Cgi_onbc_cc_err_short_to_batt))

/** Shorthand macro for determining an invalid pilot state */
#define CGI_ONBC_INVALID_PLT (bool_t)((cgi_onbc_pilot_state == Cgi_onbc_plt_err_unexpected) \
                                  ||  (cgi_onbc_pilot_state == Cgi_onbc_plt_err_short_to_batt))

/** Shorthand macro for determining whether the given CC signal indicates charger 
 *  tethered */
#define CGI_ONBC_CC_TETHERED(cc) (bool_t)(((cc) == Cgi_onbc_cc_tethered_32A) \
									  ||  ((cc) == Cgi_onbc_cc_tethered_16A))

/** Shorthand macro for determining whether the pilot input indicates the presence 
 *  of a charger */
#define CGI_ONBC_PILOT_PRESENT (bool_t)((cgi_onbc_pilot_state == Cgi_onbc_plt_charger_waiting) \
                                    ||  (cgi_onbc_pilot_state == Cgi_onbc_plt_charger_ready))


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/**
 * Enumeration of on-board charger variants with respect to maximum current
 * capability.
 */
typedef enum
{
    Cgi_onbc_chg_low_i,     /**< low current connection */
    Cgi_onbc_chg_high_i,    /**< high current connection */
    Cgi_onbc_variant_count  /**< total number of charger variants */
} cgi_onbc_chg_types_e;

/**
 * Enumeration of analog charger related signals
 */
enum cgi_onbc_ip_signal_e
{
    Cgi_onbc_cc,            /**< Charger's connection confirmation input signal */
    Cgi_onbc_pilot,         /**< Charger's pilot input signal */
    Cgi_onbc_signal_count   /**< Number of onboard charger related input signals */
};


/**
 * Enumeration for the possible connection confirmation input levels */
enum cgi_onbc_cc_state_e
{
  Cgi_onbc_cc_err_short_to_gnd,     /**< signal shorted to ground */
  Cgi_onbc_cc_err_unexpected,       /**< unexpected voltage level */
  Cgi_onbc_cc_tethered_32A,         /**< tethered level (32A connection) */
  Cgi_onbc_cc_tethered_16A,         /**< tethered level (16A connection) */
  Cgi_onbc_cc_untethered,           /**< untethered level */
  Cgi_onbc_cc_err_short_to_batt     /**< signal shorted to battery */
};

/**
 * Enumeration for the possible pilot input levels */
enum cgi_onbc_pilot_state_e
{
  Cgi_onbc_plt_err_unexpected,      /**< unexpected voltage level */
  Cgi_onbc_plt_no_charger,          /**< no charger level */
  Cgi_onbc_plt_charger_waiting,     /**< charger waiting (for BMS) level */
  Cgi_onbc_plt_charger_ready,       /**< charger ready (to charge) level */
  Cgi_onbc_plt_err_short_to_batt    /**< signal shorted to battery */
};


/******************************************************************************
 * CALIBRATIONS
 *****************************************************************************/

extern CAL current_t    cgi_onbc_current_limit;
extern CAL current_t    cgi_onbc_supply_current_limit[Cgi_onbc_variant_count];
extern CAL supply_v_t   cgi_onbc_cc_tolerance_v;
extern CAL supply_v_t   cgi_onbc_plt_tolerance_v;
extern CAL supply_v_t   cgi_onbc_cc_l_tethered_32A_lower_v;
extern CAL supply_v_t   cgi_onbc_cc_l_tethered_32A_upper_v;
extern CAL supply_v_t   cgi_onbc_cc_l_tethered_16A_lower_v;
extern CAL supply_v_t   cgi_onbc_cc_l_tethered_16A_upper_v;
extern CAL supply_v_t   cgi_onbc_cc_l_untethered_lower_v;
extern CAL supply_v_t   cgi_onbc_cc_l_untethered_upper_v;
extern CAL supply_v_t   cgi_onbc_plt_l_no_charger_upper_v;
extern CAL supply_v_t   cgi_onbc_plt_l_chrg_ready_lower_v;
extern CAL supply_v_t   cgi_onbc_plt_l_chrg_ready_upper_v;
extern CAL supply_v_t   cgi_onbc_plt_l_chrg_waiting_lower_v;
extern CAL supply_v_t   cgi_onbc_plt_l_chrg_waiting_upper_v;
extern CAL ts_time_t    cgi_onbc_chg_offline_timeout;
extern CAL ts_time_t    cgi_onbc_pilot_settling_time;
extern CAL struct cgi_input_signal_calibration_s cgi_onbc_cal_cc;
extern CAL struct cgi_input_signal_calibration_s cgi_onbc_cal_pilot;


/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/


/******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *****************************************************************************/

PRIVATE void    cgi_onbc_update_enabled_state(void);
PRIVATE void    cgi_onbc_update_cc_state(void);
PRIVATE void    cgi_onbc_update_pilot_state(void);
PRIVATE bool_t  cgi_onbc_is_unexpected_pilot_state(void);
PRIVATE void    cgi_onbc_update_pilot_pwm_state(void);
PRIVATE void    cgi_onbc_update_tethered_status(void);
PRIVATE void    cgi_onbc_update_tethered_to_nothing(void);
PRIVATE void    cgi_onbc_update_tethered_to_something(void);
PRIVATE void    cgi_onbc_update_tethered_to_charger(void);
PRIVATE void    cgi_onbc_update_tethered_to_active(void);
PRIVATE void    cgi_onbc_update_connection_error_status(void);
PRIVATE void    cgi_onbc_update_comms_error_status(void);
PRIVATE void    cgi_onbc_update_charger_error_status(void);
PRIVATE void    cgi_onbc_update_current_limit(void);
PRIVATE void    cgi_onbc_update_is_capable_to_charge(void);
PRIVATE void    cgi_onbc_update_hv_isol_state(void);
PRIVATE void    cgi_onbc_update_events(void);
PRIVATE void    cgi_onbc_set_active_state(void);
PRIVATE void    cgi_onbc_set_comms_state(void);
PRIVATE void    cgi_onbc_set_pilot_control_op(void);
PRIVATE void    cgi_onbc_set_current_request(void);
PRIVATE void    cgi_onbc_set_voltage_request(void);
PRIVATE void    cgi_onbc_set_chg_state_request(void);


#endif      /* CGI_ONBC_P_H */

