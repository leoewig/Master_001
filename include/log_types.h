/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     main_isr.h 
 *
 * description: 
 *  
 * Public header for Logging feature      
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(LOG_TYPES_H)
#define      LOG_TYPES_H
/*************************************************************************//**
 *
 * Public header for Logging feature
 *
 *
 * Copyright (c) Linkedriving Tech 2011 - All rights reserved
 *
 * @file
 *****************************************************************************/

/**
 * @ingroup LOG
 *
 * @{
 * @file
 * @}
 */


/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "mmc.h"
#include "nvs_cfg.h"
#include "mdm_types.h"      /* for MAX_NUM_VTBM */
#include "vio_types.h"      /* for VIO_VIN_LEN */
#include "log_cfg.h"


/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/** Blank macro definitions for .def file
 */
#undef  EVENT_COUNTER
#undef  LOG_TABLE
#undef  INTERVALS
#undef  DATA_INTEGRATION
#define EVENT_COUNTER(counter, event_type, enable_cond)
#define LOG_TABLE(table, event_type, enable_cond, src_data_adr, src_data_type, validity_fun, interval_count, custom_eval_fun)
#define INTERVALS(low_limit, Up_1st_Int, Up_2nd_Int, Up_3rd_Int, Up_4th_Int, Up_5th_Int, Up_6th_Int, Up_7th_Int, Up_8th_Int, Up_9th_Int, Up_10th_Int, Up_11th_Int, Up_12th_Int)
#define DATA_INTEGRATION(integral, event_type, enable_cond, src_data_adr, validity_fun)

/** Calculates the size of NVS used by the log feature.
 */
#define LOG_NVS_USED            (sizeof(nvs_header_s) + sizeof(log_nvdata_s))

/** Specifies the maximum number of intervals in a table. This is currently
 * based on the no.of VTBMs, which in P12 is 12.
 * However the number of VTBMs could be upto 32, in which case a lot of memory would
 * be wasted and therefore need to find a better method for allocating memory.
 */
#define LOG_MAX_NUM_OF_VTBM     (MAX_NUM_VTBM)

/** Macro to define the periodic log rate.
 */
#define PERIOD_RATE_MS          (200u)

/** Shorthand for accessing the vehicle energy throughput
 */
//#define LOG_ENERGY_VEHICLE      (log_ram_image.data.data_integration[Log_integ_enrg_out_veh_bus])

/** Shorthand for accessing the regen energy throughput
 */
//#define LOG_ENERGY_REGEN        (log_ram_image.data.data_integration[Log_integ_enrg_in_veh_bus])

/** Shorthand for accessing the charger energy throughput
 */
//#define LOG_ENERGY_CHARGER      (log_ram_image.data.data_integration[Log_integ_enrg_in_chg_bus])


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

typedef bool_t  (* Log_validity_func)(void);
typedef void    (* Log_custom_eval_func)(Log_table_e);

/** This enum lists all the events that trigger the log actions.
 */
typedef enum
{
    Log_event_continuous = 0u,      /**< logging is performed continuously */
    Log_event_periodic,             /**< logging is performed periodically (every 200ms) */
    Log_event_storage_wakeup,       /**< BMS has just woken up to record storage measurements */
    Log_event_storage_logging_ready,/**< all storage measurements are ready for logging */
    Log_event_ign_on,               /**< ignition turned ON event */
    Log_event_veh_ctc_closed,       /**< vehicle contactors closed */
    Log_event_veh_ctc_opened,       /**< vehicle contactors opened */
    Log_event_veh_ctc_state_change, /**< vehicle contactor state changed (opened or closed) */
    Log_event_chg_ctc_closed,       /**< charger contactors closed */
    Log_event_chg_ctc_opened,       /**< charger contactors opened */
    Log_event_chg_ctc_state_change, /**< charger contactor state changed (opened or closed) */
    Log_event_any_ctc_opened,       /**< any contactor opened */
    Log_event_any_ctc_state_change, /**< any contactor state changed (opened or closed) */
    Log_event_hvil_opened,          /**< HVIL opened */
    Log_event_drv_cyc_start,        /**< drive cycle started */
    Log_event_drv_cyc_end,          /**< drive cycle ended */
    Log_event_chg_cyc_start,        /**< charge cycle started */
    Log_event_chg_cyc_end           /**< charge cycle ended */
} Log_event_e;

/** This enum lists all the logging enable conditions.
 */
typedef enum
{
    Log_en_cond_sys_active = 0,     /**< system is operational */
    Log_en_cond_storage_mode,       /**< BMS has woken up to record storage measurements */
    Log_en_cond_veh_ctc_open,       /**< vehicle contactors are currently open */
    Log_en_cond_veh_ctc_closed,     /**< vehicle contactors are currently closed */
    Log_en_cond_chg_ctc_open,       /**< charger contactors are currently open */
    Log_en_cond_chg_ctc_closed,     /**< charger contactors are currently closed */
    Log_en_cond_all_ctc_open,       /**< all contactors are open since the last log update */
    Log_en_cond_any_ctc_closed,     /**< any contactors are or have been closed since the last log update */
    Log_en_cond_chg_cycle_succ,     /**< charger cycle completed successfully */
    Log_en_cond_op_mode_active      /**< current operating mode is not 'passive' */
} Log_en_cond_e;

/** Enumeration defining the different types of operating mode for which logging
 *  is to be done.
 */
typedef enum
{
    Log_op_mode_passive,            /**< system initialising or not active */
    Log_op_mode_standby,            /**< contactors open, ready to close */
    Log_op_mode_operational,        /**< vehicle contactors closed */
    Log_op_mode_charging,           /**< charger contactors closed */
    Log_op_mode_post_run            /**< balancing: ignition off, charger not active, unit alive */
} Log_op_mode_e;

/** Type used for storing measurement extremes.
 */
typedef struct
{
    percent_t       highest_soc;            /**< highest SOC seen */
    percent_t       lowest_soc;             /**< lowest SOC seen */
    cell_v_t        highest_cell_v;         /**< highest cell voltage measured */
    cellcount_t     cell_num_hi_v;          /**< cell number with highest V */
    cell_v_t        lowest_cell_v;          /**< lowest cell voltage measured */
    cellcount_t     cell_num_lo_v;          /**< cell number with lowest V */
    temperature_t   highest_cell_t;         /**< highest cell skin temperature */
    cellcount_t     cell_num_hi_t;          /**< cell number with highest cell skin temperature */
    temperature_t   lowest_cell_t;          /**< lowest cell skin temperature */
    cellcount_t     cell_num_lo_t;          /**< cell number with lowest cell skin temperature */
    temperature_t   highest_inlet_t;        /**< highest inlet coolant temperature */
    temperature_t   lowest_inlet_t;         /**< lowest inlet coolant temperature */
    temperature_t   highest_outlet_t;       /**< highest outlet coolant temperature */
    temperature_t   lowest_outlet_t;        /**< lowest outlet coolant temperature */
    temperature_t   highest_pack_delta_t;   /**< highest inlet/outlet coolant temperature delta */
    current_t       highest_chg_i_ma;       /**< highest charge current */
    current_t       highest_dis_i_ma;       /**< highest discharge current */    
    temperature_t   highest_strg_cell_t;    /**< highest storage cell skin temperature */
    cellcount_t     cell_num_hi_strg_t;     /**< cell number with highest storage cell skin temperature */
    temperature_t   lowest_strg_cell_t;     /**< lowest storage cell skin temperature */
    cellcount_t     cell_num_lo_strg_t;     /**< cell number with lowest storage cell skin temperature */
    cell_v_t        highest_op_cell_v;      /**< highest operational state cell voltage measured */
    cellcount_t     cell_num_hi_op_v;       /**< cell number with highest operational state V */
    cell_v_t        lowest_op_cell_v;       /**< lowest operational state cell voltage measured */
    cellcount_t     cell_num_lo_op_v;       /**< cell number with lowest operational state V */
} Log_data_extremes_s;

/** Type used for storing log table bins.
 */
#undef  LOG_TABLE
#define LOG_TABLE(table, event_type, enable_cond, src_data_adr, src_data_type, validity_fun, interval_count, custom_eval_fun) \
    uint32_t table[interval_count];
typedef struct
{
    #include "log.def"
    uint8_t  table_count;
} Log_data_tables_s;
#undef  LOG_TABLE
#define LOG_TABLE(table, event_type, enable_cond, src_data_adr, src_data_type, validity_fun, interval_count, custom_eval_fun)

/** Data type used for date and time storage
 *  @todo This must be replaced by the correct type when date and time have been implemented.
 */
typedef uint32_t    DATETIME;


/** LOG NVS layout version to ensure incompatible data isn't reloaded after reflash
 */
#define LOG_NVS_LAYOUT_VERSION          (9u)

/******************************************************************************
 * Type used to define the entire logging NV data block.
 * Update #LOG_NVS_LAYOUT_VERSION if changing
 *****************************************************************************/
typedef struct
{
    /* VIN */
   // char_t              vin[VIO_VIN_LEN];

    /* storage for event counters */
    uint32_t            event_counters[Log_event_cntr_max];
    /* storage for measurement extremes */
    Log_data_extremes_s extremes;
    /* storage for data parameters integrated over time */
    uint32_t            data_integration[Log_integ_max];
    /* storage for table bins - type is uint32_t to allow 10 years of continuous storage */
    Log_data_tables_s   table_bins;

    /* number of successive storage logging wake-up events */
    uint8_t             successive_storage_wakeups;

    /* date and time the data block was reset */
    DATETIME            timestamp;
} log_nvdata_s;

typedef struct
{
    nvs_header_s header;        /**< Feature Information Structure.*/
    log_nvdata_s data;          /**< Module NV data-set. */
    //uint8_t reserved_bytes[(int16_t) LOG_MAX_NVS_BYTES
    //                            - (int16_t) LOG_NVS_USED];
                                /**< Feature Reserved_bytes.*/
    uint8_t reserved_bytes[1u];
} log_nvs_wrapper_s;

#endif      /* LOG_TYPES_H */
