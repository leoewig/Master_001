#if !defined(CGI_TYPES_H)
#define      CGI_TYPES_H
/*************************************************************************//*!
 *
 * @brief
 * Public header for Charger Interface / Communications Feature [cgi]
 *
 * Copyright (c) Ricardo plc 2014-2015 - All rights reserved
 *
 * @file
 * $Id: cgi_types.h 873 2015-03-16 11:19:41Z j_bailey $
 *
 *****************************************************************************/

#include "mmc.h"


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/**
 * Enumeration of types of charger applicable to the BMS. Used for indexing
 * charger parameters/data and indication of a currently active charger.
 * By having the high current off-board charger first, the BMS will select
 * that charger over the low current type in a scenario when no charger is 
 * currently active and both are detected.
 */
typedef enum
{
    Cgi_chg_offboard = 0,   /**< high current off-board charger */
    Cgi_chg_onboard,        /**< low current on-board charger */ 

    Cgi_chg_count,          /**< number of available chargers */
    Cgi_chg_none            /**< used to indicate no charger active */
} Cgi_charger_type_e;

/** Scaling parameters to convert the charger input ADC signals to actual voltages */
struct cgi_input_signal_calibration_s
{
    supply_v_t gain;
    supply_v_t offset;
};

/**
 * Enumeration for the possible proximity input levels */
enum cgi_proximity_state_e
{
  Cgi_prx_err_short_to_gnd,     /**< signal shorted to ground */
  Cgi_prx_err_unexpected,       /**< unexpected voltage level */
  Cgi_prx_guarded,              /**< guarded level */
  Cgi_prx_tethered,             /**< tethered level */
  Cgi_prx_cable_fault,          /**< cable fault level */
  Cgi_prx_untethered,           /**< untethered level */
  Cgi_prx_err_short_to_batt,    /**< signal shorted to battery */
  Cgi_prx_state_default         /**< signal not classified */
};

/**
 * Enumeration for the possible pilot input levels */
enum cgi_pilot_state_e
{
  Cgi_plt_err_unexpected,       /**< unexpected voltage level */
  Cgi_plt_no_charger,           /**< no charger level */
  Cgi_plt_charger_waiting,      /**< charger waiting (for BMS) level */
  Cgi_plt_charger_ready,        /**< charger ready (to charge) level */
  Cgi_plt_err_short_to_batt     /**< signal shorted to battery */
};


/**
 *
 */
enum cgi_charger_status_e
{
    Cgi_chg_s_default,
    Cgi_chg_s_fault,
    Cgi_chg_s_sleep,
    Cgi_chg_s_charge,
    Cgi_chg_s_standby,
    Cgi_chg_s_waiting_for_BMS,
    Cgi_chg_s_12V_recovery,
    Cgi_chg_s_HV_safety_checks,
    Cgi_chg_s_not_defined
};

/**
 * Enumeration of charger high voltage contactor states
 */
typedef enum 
{
    Chg_hv_isolated,        /**< The HV outputs are isolated */
    Chg_hv_connected        /**< The HV outputs are connected */
} cgi_hv_isolation_state_e;

/** Structure containing the configuration of the charger related analog signals */
typedef struct
{
    CONST volatile uint16_t *                   sensor_address;
    data_validity_e                             * sensor_validity;
    CAL struct cgi_input_signal_calibration_s   * cal;
} cgi_signals_configuration_t;


#endif  /* CGI_TYPES_H */
