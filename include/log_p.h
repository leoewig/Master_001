/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     log_p.h
 *
 * description: 
 *  
 * Private header file for LOG. Only needed for unit testing
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(LOG_P_H)
#define      LOG_P_H
/**
 *
 * Private header file for LOG.
 *
 * Only needed for unit testing
 *
 * @file
 * @ingroup LOG
 */

/* Copyright (c) Linkedriving Tech 2011 - All rights reserved */


/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "basic_types.h"
#include "log_types.h"
#include "rtc_types.h"
//#include "vmi_types.h"


/******************************************************************************
 * PRIVATE MACRO DEFINITIONS
 *****************************************************************************/

#if 0
/** Set of macro definitions that provide ability to enable/disable individual 
 *  tables 
 */
#define LOG_DIS_TABLE_OP_MODE                       /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_TEMPERATURE                   /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_SOC_HIGHEST                   /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_SOC_LOWEST                    /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_CHG_ENERGY                    /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_REGEN                         /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_VEH_ENERGY                    /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_DELTA_V                       /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_DELTA_T                       /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_N_ACTV_CELLS_BAL              /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_N_PASV_CELLS_BAL              /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_VTBM_ACTV_BAL                 /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_VTBM_PASV_BAL                 /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_VEH_I                         /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_CHG_I                         /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_DISCHG_POWER                  /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_REGEN_POWER                   /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_PRECHG_CTC_OPEN_I             /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_VEH_POS_CTC_OPEN_I            /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_VEH_NEG_CTC_OPEN_I            /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_CHG_POS_CTC_OPEN_I            /* [REQ_BMS_LOG_FLX_001] */
#define LOG_DIS_TABLE_CHG_NEG_CTC_OPEN_I            /* [REQ_BMS_LOG_FLX_001] */
#endif

/** Blank Macro Definitions For .def file
 */
#undef  EVENT_COUNTER
#undef  LOG_TABLE
#undef  INTERVALS
#undef  DATA_INTEGRATION
#define EVENT_COUNTER(counter, event_type, enable_cond)
#define LOG_TABLE(table, event_type, enable_cond, src_data_adr, src_data_type, validity_fun, interval_count, custom_eval_fun)
#define INTERVALS(low_limit, Up_1st_Int, Up_2nd_Int, Up_3rd_Int, Up_4th_Int, Up_5th_Int, Up_6th_Int, Up_7th_Int, Up_8th_Int, Up_9th_Int, Up_10th_Int, Up_11th_Int, Up_12th_Int)
#define DATA_INTEGRATION(integral, event_type, enable_cond, src_data_adr, validity_fun)

/** Macro to define increment count by 1.
 */
#define INC_COUNT                   (1u)

/** Size of the contactor wear map x & y axes.
 */
#define LOG_CTC_WEAR_MAP_SZ         (6u)

/** Maximum number of times LOG attempts to set the storage logging wake-up alarm
 */
#define LOG_MAX_ALARM_RETRIES       (10u)

/** Macro used for easy access to integrated data
 */
//#define LOG_INTEGRATOR(integ)   (log_ram_image.data.data_integration[(uint8_t)(integ)])

/** Shorthand for accessing the successive storage wakeups counter
 */
//#define LOG_N_STORAGE_WAKEUPS   (log_ram_image.data.successive_storage_wakeups)

/** Macro used for incrementing a uint32_t variable. The macro saturates the 
 *  the variable if the uint32_t maximum limit is reached.
 */
#define INC_UINT(value, inc_by) (value = (((value) < (UINT32_MAX - (inc_by))) ? \
                                          ((value) + (inc_by)) :  (UINT32_MAX)))

/** Macro for acquiring the absolute current value. Keeps LINT happy by checking
 *  the corner case where the input current is INT32_MIN (which is unlikely)
 */
#define LOG_ABS_I(current)  ((abs32(current) < (uint32_t)INT32_MAX) ? \
                             (current_t)abs32(current) : (current_t)INT32_MAX)

/** Macro to set the corresponding bit position represented by the value of the
 *  passed parameter. The values are set in power of 2. e.g if the value passed
 *  as parameter is 2 then bit 2 (starting from bit 0) would be set.
 */
#define LOG_SET_BIT(type, x)        ((type) ((type) 1u << ((type) (x))))

/** Macro used to check whether a specific event #Log_event_e has occurred. 
 */
#define LOG_EVENT_OCCURRED(e)       \
        ((bool_t) ((log_event_status & ((uint32_t)1u << ((uint8_t)(e)))) > 0u))

/** Macro used to check whether a specific enable condition #Log_en_cond_e has 
 *  has been satisfied.
 */
#define LOG_EN_COND_SATISFIED(ec)   \
        ((bool_t) ((log_en_cond_status & ((uint16_t)1u << ((uint8_t)(ec)))) > 0u))

#define LOG_NO_VALIDITY_FUN     ((Log_validity_func)0)
#define LOG_NO_EVALUATION_FUN   ((Log_custom_eval_func)0)

/******************************************************************************
 * DATA TYPES
 *****************************************************************************/

/**
 * Information about the status of the setting of an alarm */
enum Alarm_retry_e
{
    Alarm_retry_fail,	  /**< Don't call again, alarm setting failed, abandon */
    Alarm_retry_ack,      /**< Don't call again, the alarm was acknowledged */
    Alarm_retry_again,	  /**< Call again, the alarm was not set but could retry */
    Alarm_retry_disabled, /**< Don't call again, alarm is disabled */
    Alarm_retry_invalid   /**< Don't call again, alarm setting was invalid */
};


/******************************************************************************
 * CALIBRATIONS
 *****************************************************************************/
extern CAL uint16_t    log_wakeup_interval_1_min;
extern CAL uint16_t    log_wakeup_interval_2_min;
extern CAL uint16_t    log_wakeup_interval_change_th;
extern CAL ts_time_t   log_storage_mode_timeout;
extern CAL uint32_t    log_prechg_ctc_end_of_life_th;
extern CAL uint32_t    log_veh_pos_ctc_end_of_life_th;
extern CAL uint32_t    log_veh_neg_ctc_end_of_life_th;
extern CAL uint32_t    log_chg_pos_ctc_end_of_life_th;
extern CAL uint32_t    log_chg_neg_ctc_end_of_life_th;
extern CAL current_t   log_prechg_ctc_wear_x_current[LOG_CTC_WEAR_MAP_SZ];
extern CAL uint32_t    log_prechg_ctc_wear_y_increment[LOG_CTC_WEAR_MAP_SZ];
extern CAL current_t   log_veh_pos_ctc_wear_x_current[LOG_CTC_WEAR_MAP_SZ];
extern CAL uint32_t    log_veh_pos_ctc_wear_y_increment[LOG_CTC_WEAR_MAP_SZ];
extern CAL current_t   log_veh_neg_ctc_wear_x_current[LOG_CTC_WEAR_MAP_SZ];
extern CAL uint32_t    log_veh_neg_ctc_wear_y_increment[LOG_CTC_WEAR_MAP_SZ];
extern CAL current_t   log_chg_pos_ctc_wear_x_current[LOG_CTC_WEAR_MAP_SZ];
extern CAL uint32_t    log_chg_pos_ctc_wear_y_increment[LOG_CTC_WEAR_MAP_SZ];
extern CAL current_t   log_chg_neg_ctc_wear_x_current[LOG_CTC_WEAR_MAP_SZ];
extern CAL uint32_t    log_chg_neg_ctc_wear_y_increment[LOG_CTC_WEAR_MAP_SZ];
extern CAL bool_t      log_soc_validity_override_en;
extern CAL percent_t   log_soc_validity_override_th;

#undef  LOG_TABLE
#define LOG_TABLE(table, event_type, enable_cond, src_data_adr, src_data_type, validity_fun, interval_count, custom_eval_fun) \
extern  CAL Log_en_cond_e log_tbl_en_cond_ ##table;
    #include "log.def"

#undef  LOG_TABLE
#define LOG_TABLE(table, event_type, enable_cond, src_data_adr, src_data_type, validity_fun, interval_count, custom_eval_fun)


/******************************************************************************
 * PRIVATE DATA (defined in log.c)
 *****************************************************************************/

/** Bit field indicating the status of events corresponing to the list defined 
 *  by the #Log_event_e enumeration.
 *  A set bit indicates the corresponding event has occurred, a cleard bit
 *  indicates the corresponding event has not occurred.
 */
extern FRAM uint32_t    log_event_status;

/** Bit field indicating the status of enable conditions corresponing to the 
 *  list defined by the #Log_en_cond_e enumeraion.
 *  A set bit indicates the corresponding enable condition is satisified, a 
 *  cleard bit indicates the corresponding enable condition is not satisified.
 */
extern FRAM uint16_t    log_en_cond_status;

/** Indicates the current operating mode.
 */
extern Log_op_mode_e    log_op_mode;

/** High resolution measurement of energy delivered to the pack via 
 *  the charger bus. Used for accumulation of energy during a charge cycle.
 */
extern cycle_energy_wh_t   log_hi_res_chg_energy_in_Wh;

/** High resolution measurement of energy delivered to the pack via 
 *  the vehicle bus. Used for accumulation of energy during a drive cycle.
 */
extern cycle_energy_wh_t   log_hi_res_veh_energy_in_Wh;

/** High resolution measurement of energy supplied by the pack via the 
 *  vehicle bus. Used for accumulation of energy during a drive cycle.
 */
extern cycle_energy_wh_t   log_hi_res_veh_energy_out_Wh;

/** Low resolution measurement of energy delivered to the pack via 
 *  the charger bus. Used for logging charge cycle energy throughput.
 */
extern energy_wh_t     log_lo_res_chg_energy_in_Wh;

/** Low resolution measurement of energy delivered to the pack via 
 *  the vehicle bus. Used for logging drive cycle energy throughput.
 */
extern energy_wh_t     log_lo_res_veh_energy_in_Wh;

/** Low resolution measurement of energy supplied by the pack via the 
 *  vehicle bus. Used for logging drive cycle energy throughput.
 */
extern energy_wh_t     log_lo_res_veh_energy_out_Wh;

/** Current SOC.
 */
extern percent_t        log_current_soc;

/** Average of current cell temperature min & max.
 */
extern int32_t          log_avg_cell_t;

/** Pre-charger contactor wear increment.
 */
extern uint32_t         log_prechg_ctc_wear_inc;

/** Positive vehicle contactor wear increment.
 */
extern uint32_t         log_veh_pos_ctc_wear_inc;

/** Negative vehicle contactor wear increment.
 */
extern uint32_t         log_veh_neg_ctc_wear_inc;

/** Positive charger contactor wear increment.
 */
extern uint32_t         log_chg_pos_ctc_wear_inc;

/** Negative charger contactor wear increment.
 */
extern uint32_t         log_chg_neg_ctc_wear_inc;

/** Current storage period interval 
 */
extern uint16_t         log_current_storage_interval;

#ifndef LOG_DIS_TABLE_N_ACTV_CELLS_BAL
/** Indicates the number of cells currently being actively balanced.
 */
extern uint8_t          log_n_cell_active_bal;
#endif

#ifndef LOG_DIS_TABLE_N_PASV_CELLS_BAL
/** Indicates the number of cells currently being passively balanced.
 */
extern uint8_t          log_n_cell_passive_bal;
#endif

#if !defined(LOG_DIS_TABLE_VTBM_ACTV_BAL) || !defined(LOG_DIS_TABLE_VTBM_PASV_BAL)
/** Indicates the balancing state of each VTBM.
 */
//extern FRAM enum BalAction_e log_vtbm_bal_state[LOG_MAX_NUM_OF_VTBM];
#endif


/******************************************************************************
 * UNIT TEST DATA (defined in log.c)
 *****************************************************************************/

#ifdef UNIT_TEST
//#include "ctc.h"
#include "ms_types.h"
extern bool_t       log_paused;
extern ts_time_t    log_time_stamp;
extern bool_t       log_last_ign_on_status;
extern bool_t       log_last_chg_cycle_active_status;
extern bool_t       log_last_drv_cycle_active_status;
extern bool_t       log_last_hvil_open_status;
extern ts_time_t    log_last_ctc_change_time;
extern bool_t       log_veh_bus_energy_available;
extern bool_t       log_chg_bus_energy_available;
extern bool_t       log_storage_logging_done;
extern bool_t       log_latched_ign_on_status;
extern enum Ms_wakeup_mode      log_last_wakeup_type;
//extern enum Ctc_cont_state_e    log_last_veh_ctc_status;
//extern enum Ctc_cont_state_e    log_last_chg_ctc_status;
extern const uint8_t log_tbl_interval_counts[Log_table_max];
extern const int32_t log_tbl_intervals[Log_table_max][LOG_MAX_NUM_OF_VTBM + 1u];
#endif


/******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES  (documented in source)
 *****************************************************************************/

PRIVATE void     log_set_shutdown_state(void);
PRIVATE uint16_t log_set_storage_wakeup_interval(void);
PRIVATE void     log_set_alarm(uint16_t interval);
PRIVATE void     log_update_timestamp(void);
PRIVATE void     log_pause(void);
PRIVATE void     log_resume(void);
PRIVATE void     log_detect_events(void);
PRIVATE void     log_detect_event_periodic(void);
PRIVATE void     log_detect_event_storage_logging(void);
PRIVATE void     log_detect_event_ign_on(void);
PRIVATE void     log_detect_event_vehicle_ctc(void);
PRIVATE void     log_detect_event_charger_ctc(void);
PRIVATE void     log_detect_event_hvil_open(void);
PRIVATE void     log_detect_event_drive_cycle(void);
PRIVATE void     log_detect_event_charge_cycle(void);
PRIVATE void     log_check_enable_conditions(void);
PRIVATE void     log_set_op_mode(void);
PRIVATE void     log_setup_drive_cycle_data(void);
PRIVATE void     log_setup_charge_cycle_data(void);
PRIVATE void     log_collect_data(void);
PRIVATE void     log_collect_data_periodic(void);
PRIVATE void     log_collect_data_storage(void);
PRIVATE void     log_collect_data_drive_cycle(void);
PRIVATE void     log_collect_data_charge_cycle(void);
PRIVATE void     log_collect_data_contactor_currents(void);
//PRIVATE bool_t   log_calculate_energy_increment(enum vmi_bus_e bus, cycle_energy_wh_t *e_inc);
PRIVATE void     log_check_for_soc_validity_override(void);
PRIVATE uint32_t log_lookup_ctc_wear_inc(CAL current_t *xaxis, 
                                         CAL uint32_t *yaxis, current_t current);
PRIVATE void     log_post_process(void);
#if !defined(LOG_DIS_TABLE_N_ACTV_CELLS_BAL) || !defined(LOG_DIS_TABLE_N_PASV_CELLS_BAL) || !defined(LOG_DIS_TABLE_VTBM_ACTV_BAL) || !defined(LOG_DIS_TABLE_VTBM_PASV_BAL)
PRIVATE void     log_get_cell_bal_data(void);
#endif
PRIVATE enum Alarm_retry_e log_retry_set_alarm(rtc_alarm_req_e req, uint16_t alarm_minutes);
PRIVATE void     log_update_data_extremes_soc(void);
PRIVATE void     log_update_data_extremes_active_cell_vt(void);
PRIVATE void     log_update_data_extremes_storage_cell_vt(void);
PRIVATE void     log_update_data_extremes_coolant_t(void);
PRIVATE void     log_update_data_extremes_bus_i(void);



/******************************************************************************
 * SHARED MODULE FUNCTION PROTOTYPES (documented in source)
 *****************************************************************************/

extern bool_t   log_is_cell_temperature_valid(void);
extern bool_t   log_is_cell_voltage_valid(void);
extern bool_t   log_is_current_valid(void);
extern bool_t   log_is_balancing_data_valid(void);
extern bool_t   log_is_ctc_open_i_data_valid(void);
extern bool_t   log_is_chg_energy_data_valid(void);
extern bool_t   log_is_veh_energy_data_valid(void);
extern bool_t   log_is_soc_valid(void);
extern void     log_update_event_counters(void);
extern void     log_update_data_extremes(void);
extern void     log_update_tables(void);
extern void     log_update_integral_data(void);
extern uint32_t log_get_table_bin(Log_table_e table, uint8_t bin);

#endif      /* LOG_P_H */
