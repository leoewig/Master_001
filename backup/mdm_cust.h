/**
 *
 * Measurement data manager interface
 *
 * @file
 * @ingroup mdm
 * $Id: mdm_cust.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2015 - All rights reserved */

#if !defined(MDM_CUST_H)
#define      MDM_CUST_H

#include "mdm_types_cust.h"
#include "mmc_rtw.h"             /* for MAX_NUM_CELLS */
#include "bal_types.h" /* for Bal_status_e */
#include "basic_types.h"
#include "mmc.h"

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

struct bg_fg_bal_outputs_s
{
    uint8_t                     balanced_cell_demand[MAX_NUM_VTBM];
    enum BalAction_e            balancing_mode_demand[MAX_NUM_VTBM];
    Bal_status_e                balancing_status;
    /** Status equivalent for the purposes of after-run determination, it 
     * is set to @ref Bal_status_chg_maintenance based on range as well as 
     * time */
    Bal_status_e                balancing_ar_status;
    bool_t                      is_balancing_requested;
    bool_t                      plugin_charge_request;
};

/**
 * Outputs from cell management */
struct cm_op_s
{
    /* State of Charge */
    data_validity_e veh_soc_valid;          /**< battery pack SOC validity */
    percent_t       veh_soc;                /**< battery pack SOC */
    percent_t       veh_abs_soc;            /**< absolute battery pack SOC (unscaled) */
    uint16_t        pack_chg_est_ctr;       /**< counter indicating pack charge estimation update is available */
    percent_t       pack_chg_est_span;      /**< span of last pacj charge estimate for imbalance determination */
    PCE_E           pack_chg_est_type;      /**< relative SoC and type of the last pack charge estimate */
    charge_t        cell_capacity;          /**< estimated cell capacity */
    charge_t        cell_charge[MAX_NUM_CELLS]; /**< charge estimate of each cell */

    /* State of Health */
    percent_t       pack_resistance_growth; /**< battery pack resistance growth */
    percent_t       pack_capacity_fade;     /**< battery pack charge capacity fade */
    ohm_t           cell_res_est_ohm;       /**< average cell resistance estimate (at current temp.) */

    /*  Battery pack available energy */
    data_validity_e pack_avail_energy_valid;
    energy_wh_t     pack_avail_energy;

    /** Current limits & voltage estimates */
    data_validity_e avail_curr_lim_valid;   /**< validity of available current limits */
    data_validity_e pack_v_est_valid;       /**< validity of voltage estimates */
    LIM_REASON_E    chg_i_inst_lim_reason;  /**< reason for charge current limit */
    LIM_REASON_E    dis_i_inst_lim_reason;  /**< reason for charge current limit */
    LIM_REASON_E    chg_i_short_lim_reason; /**< reason for charge current limit */
    LIM_REASON_E    dis_i_short_lim_reason; /**< reason for charge current limit */
    current_t       curr_chg_avail_inst;    /**< instantaneously available charging current */
    current_t       curr_dis_avail_inst;    /**< instantaneously available discharging current */
    pack_v_t        pack_v_chg_inst_v;      /**< est. pack voltage at max. (instantaneous) charge current */
    pack_v_t        pack_v_dis_inst_v;      /**< est. pack voltage at max. (instantaneous) discharge current */
    current_t       curr_chg_avail_short;   /**< short-term available charging current */
    current_t       curr_dis_avail_short;   /**< short-term available discharging current */
    pack_v_t        pack_v_chg_short_v;     /**< est. pack voltage at max. (short pulse) charge current */
    pack_v_t        pack_v_dis_short_v;     /**< est. pack voltage at max. (short pulse) discharge current */
    
    /** Thermal management */
    bool_t          etms_ac_comp_enable;    /**< external thermal management system AC compressor enable request flag */
    bool_t          etms_heater_enable;     /**< external thermal management system heater enable request flag */
    bool_t          etms_fan_enable;        /**< external thermal management system fan enable request flag */
    frequency_t     etms_fan_freq;          /**< external thermal management system fan PWM frequency */
    percent_t       etms_fan_duty;          /**< external thermal management system fan PWM duty cycle */
    rpm_t           etms_ac_comp_speed;     /**< external thermal management system AC compressor speed */

    /** Action requests */
    bool_t          inhibit_balancing;      /**< inhibit balancing to allow charge estimation at start up */
    bool_t          inhibit_charge;       
    bool_t          keepalive;              /**< inhibit sleep mode */
    uint16_t        ee_write_ctr;           /**< counter to idicate a request to write to EEPROM */
};

/* Data structure output from the background task to the foreground task */
struct bg_fg_data_s
{
    /** Cell management outputs */
    struct cm_op_s cm;
    /** outputs from balancing */
    struct bg_fg_bal_outputs_s  bal;
};

/* TRUE if the background has been triggered due to a pending shutdown */
extern MdmBgShutdown_t  mdm_bg_shutdown_run;

extern temperature_t        mdm_t_inlet;
extern temperature_t        mdm_t_outlet;
extern data_validity_e      mdm_outlet_valid;
extern data_validity_e      mdm_inlet_valid;

extern temperature_t               mdm_cell_t_min_value;
extern pack_v_t                    mdm_cell_v_sum;
extern data_validity_e             mdm_cell_t_valid;
extern data_validity_e             mdm_cell_v_valid;
extern cell_v_t                    mdm_cell_v_max_value;
extern temperature_t               mdm_cell_t_avg_value;
extern temperature_t               mdm_cell_t_max_value;
extern cellcount_t                 mdm_cell_v_meas_count;
extern cellcount_t                 mdm_cell_v_min_num;
extern cell_v_t                    mdm_cell_v_min_value;
extern cellcount_t                 mdm_cell_v_max_num;
extern temperature_t               mdm_cell_t_delta;
extern cell_v_t                    mdm_cell_v_delta;
extern cellcount_t                 mdm_cell_t_max_num;
extern cellcount_t                 mdm_cell_t_min_num;

/**
 * Buffer that has been filled with values - used by fg task
 * These are the outputs from the background to the foreground */
extern FPTR CONST struct bg_fg_data_s    * mdm_bg_output_p;

/** Indication from bg to fg that the output structure has been updated */
extern uint8_t mdm_bg_op_read_ctr;

#endif      /* MDM_CUST_H */

