/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     log_cfg.h
 *
 * description: Public header for Logging feature
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
#if !defined(LOG_CFG_H)
#define      LOG_CFG_H
/*************************************************************************//**
 *
 * Public header for Logging feature
 *
 *
 * Copyright (c) Linkedriving Tech 2011 - All rights reserved
 *
 * @file
 * @ingroup LOG
 *****************************************************************************/

/******************************************************************************
 * INCLUDES
 *****************************************************************************/


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


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/** Enum containing the list of monitored and logged events. The name of the 
 *  generated enums begins with "Log_event_cntr_" followed by the counter name.
 */
#undef  EVENT_COUNTER
#define EVENT_COUNTER(counter, event_type, enable_cond) Log_event_cntr_ ##counter,
typedef enum
{
    #include "log.def"
    Log_event_cntr_max      /* Should always be the last element. */
}Log_event_counters_e;
#undef  EVENT_COUNTER
#define EVENT_COUNTER(counter, event_type, enable_cond)

/** Enum containing the list of log tables. The name of the generated enums begins 
 *  with "Log_table_" followed by the table name.
 */
#undef  LOG_TABLE
#define LOG_TABLE(table, event_type, enable_cond, src_data_adr, src_data_type, validity_fun, interval_count, custom_eval_fun) \
    Log_table_ ##table,
typedef enum
{
    #include "log.def"
    Log_table_max           /* Should always be the last element. */
} Log_table_e;
#undef  LOG_TABLE
#define LOG_TABLE(table, event_type, enable_cond, src_data_adr, src_data_type, validity_fun, interval_count, custom_eval_fun)

/** Enum containing the list of integrated variables. The name of the generated
 *  enums begins with "Log_integ_" followed by the integrated entity.
 */
#undef  DATA_INTEGRATION
#define DATA_INTEGRATION(integral, event_type, enable_cond, src_data_adr, validity_fun) \
    Log_integ_ ##integral,
typedef enum
{
    #include "log.def"
    Log_integ_max           /* Should always be the last element. */
} Log_integrals_e;
#undef  DATA_INTEGRATION
#define DATA_INTEGRATION(integral, event_type, enable_cond, src_data_adr, validity_fun)

#endif      /* LOG_CFG_H */
