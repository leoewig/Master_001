/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      log.h
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
#if !defined(LOG_H)
#define      LOG_H
/*************************************************************************//**
 *
 * Public header for Logging feature
 *
 * @file
 * @ingroup LOG
 *
 *****************************************************************************/
/* Copyright (c) Linkedriving Tech 2012 - All rights reserved.*/

/**
 *
 * <h1> Logging Feature </h1>
 *
 * ##Design goals
 *
 * The aim of the Logging feature is to provide generic data logging for the
 * BCM.
 *
 * The requirements document for the feature can be found
 * <a href="http://viewvc/viewvc.cgi/trunk/docs/reqs/generic/RU_BMS_Logging.doc?root=Q52717_NightWing&view=co">here</a>.
 * This design intends to cover all of the requirements specified in this
 * document.
 *
 * ##Requirement Traceability:
 * <center>
 * | Requirement Tags    | Design Mapping                  | Test Cases               |
 * | --------------------------|---------------------------|--------------------------|
 * |REQ_BMS_LOG_DC_001   | log_detect_event_drive_cycle()     | NA |
 * |REQ_BMS_LOG_FLX_001  | mmc_log.def | NA |
 * |REQ_BMS_LOG_STR_001  | log_nvdata_s | NA |
 * |REQ_BMS_LOG_STR_002  | log_reprogram_data() | NA |
 * |REQ_BMS_LOG_STR_003  | log_retrieve_data() | see diagnostics (read DIDs) PATs |
 * |REQ_BMS_LOG_STR_004  | log_reset_data() | NA |
 * |REQ_BMS_LOG_STR_005  | log_init(), log_reset_data()| NA |
 * |REQ_BMS_LOG_STR_006  | log_reset_data(), log_reprogram_data(), log_update_timestamp() | NA |
 * |REQ_BMS_LOG_VAL_001  | #log_tbl_src_data_validation, #log_intg_src_data_validation, log_is_cell_temperature_valid(), log_is_cell_voltage_valid(), log_is_current_valid(), log_is_balancing_data_valid(), log_update_data_extremes() | pat_log__data_validity(), pat_log__cell_t_min_max(), pat_log__cell_v_min_max(), pat_log__soc_min_max(), pat_log__cell_delta_t_table(), pat_log__cell_delta_v_table(), pat_log__veh_bus_i_table(), pat_log__chg_bus_i_table() |
 * |REQ_BMS_LOG_HIST_001 | #log_tbl_intervals, log_table_eval(), log_table_get_bin()  | pat_log__cell_t_table() |
 * |REQ_BMS_LOG_HIST_002 | NA | pat_log__op_mode_logging(), pat_log__cell_t_table(), pat_log__cell_delta_t_table(), pat_log__cell_delta_v_table(), pat_log__cell_delta_v_table(),pat_log__veh_bus_i_table(), pat_log__chg_bus_i_table() |
 * |REQ_BMS_LOG_HIST_003 | NA | pat_log__op_mode_logging(), pat_log__cell_t_table(), pat_log__cell_delta_t_table(), pat_log__cell_delta_v_table(), pat_log__cell_delta_v_table(), pat_log__veh_bus_i_table(), pat_log__chg_bus_i_table() |
 * |REQ_BMS_LOG_TIM_001  | log_detect_event_periodic() | pat_log__op_mode_logging(), pat_log__cell_t_table(), pat_log__cell_delta_t_table(), pat_log__cell_delta_v_table(), pat_log__cell_delta_v_table(), pat_log__veh_bus_i_table(), pat_log__chg_bus_i_table() |
 * |REQ_BMS_LOG_TIM_002  | log_update_event_counters(), log_table_eval(), log_table_eval_cell_temp(), log_table_eval_vtbm_balancing(), log_update_integral_data(), INC_UINT() | pat_log__data_storage_limits() |
 * |REQ_BMS_LOG_TIM_003  | #log_nvdata_s::table_bins, #uint32_t | pat_log__data_storage_limits() |
 * |REQ_BMS_LOG_TIM_004  | #log_tbl_src_data, log_collect_data() | NA |
 * |REQ_BMS_LOG_USG_001  | #Log_event_counters_e, #log_nvdata_s::event_counters, #uint32_t | pat_log__ign_cycle_cntr() |
 * |REQ_BMS_LOG_USG_002  | #Log_event_counters_e, #log_nvdata_s::event_counters, #uint32_t | pat_log__ctc_close_counters() |
 * |REQ_BMS_LOG_USG_003  | #Log_event_counters_e, #log_nvdata_s::event_counters, #uint32_t | pat_log__ctc_close_counters() |
 * |REQ_BMS_LOG_USG_004  | #Log_event_counters_e, #log_nvdata_s::event_counters, #uint32_t | pat_log__op_mode_logging() |
 * |REQ_BMS_LOG_OM_001   | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables() | pat_log__op_mode_logging() |
 * |REQ_BMS_LOG_OM_002   | #Log_op_mode_e, #log_op_mode, log_set_op_mode() | pat_log__op_mode_logging() |
 * |REQ_BMS_LOG_TEMP_001 | log_tbl_en_cond_cell_temperature, #Log_en_cond_e  | pat_log__cell_t_table() |
 * |REQ_BMS_LOG_TEMP_002 | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables(), log_table_eval_cell_temp() | pat_log__cell_t_table() |
 * |REQ_BMS_LOG_TEMP_003 | #log_tbl_intervals, LOG_TABLE(interval_count) | pat_log__cell_t_table() |
 * |REQ_BMS_LOG_TEMP_004 | #Log_table_e, #log_tbl_intervals, #INTERVALS() | pat_log__cell_t_table() |
 * |REQ_BMS_LOG_TEMP_005 |  |
 * |REQ_BMS_LOG_TEMP_006 | log_table_eval_cell_temp(), #log_tbl_intervals | pat_log__cell_t_table() | 
 * |REQ_BMS_LOG_SOC_001  | log_tbl_en_cond_soc, #Log_en_cond_e | pat_log__cell_delta_v_table() |
 * |REQ_BMS_LOG_SOC_002  | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables(), log_table_eval() | pat_log__cell_delta_v_table() |
 * |REQ_BMS_LOG_SOC_003  | #log_tbl_intervals, LOG_TABLE(interval_count) | pat_log__cell_delta_v_table() |
 * |REQ_BMS_LOG_SOC_004  | #Log_table_e, #log_tbl_intervals, #INTERVALS() | pat_log__cell_delta_v_table() |
 * |REQ_BMS_LOG_SOC_005  | #Log_data_extremes_s::highest_soc, log_update_data_extremes() | pat_log__soc_min_max() |
 * |REQ_BMS_LOG_SOC_006  | #Log_data_extremes_s::lowest_soc, log_update_data_extremes() | pat_log__soc_min_max() |
 * |REQ_BMS_LOG_CHG_001  | #Log_event_counters_e, #log_nvdata_s::event_counters, #uint32_t, log_update_event_counters() | NA |
 * |REQ_BMS_LOG_CHG_002  | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables(), log_table_eval() | NA |
 * |REQ_BMS_LOG_CHG_003  | #Log_event_chg_cyc_end, LOG_TABLE(event_type) | NA |
 * |REQ_BMS_LOG_CHG_004  | #log_tbl_intervals, LOG_TABLE(interval_count) | NA |
 * |REQ_BMS_LOG_CHG_005  | #Log_table_e, #log_tbl_intervals, #INTERVALS() | NA |
 * |REQ_BMS_LOG_CHG_006  | #Log_integrals_e, #log_nvdata_s::data_integration, #uint32_t, log_update_integral_data() | NA |
 * |REQ_BMS_LOG_REG_001  | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables(), log_table_eval() | NA |
 * |REQ_BMS_LOG_REG_002  | #Log_event_drv_cyc_end, LOG_TABLE(event_type) | NA |
 * |REQ_BMS_LOG_REG_003  | #log_tbl_intervals, LOG_TABLE(interval_count) | NA |
 * |REQ_BMS_LOG_REG_004  | #Log_table_e, #log_tbl_intervals, #INTERVALS() | NA |
 * |REQ_BMS_LOG_REG_005  | #Log_integrals_e, #log_nvdata_s::data_integration, #uint32_t, log_update_integral_data() | NA |
 * |REQ_BMS_LOG_VEU_001  | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables(), log_table_eval() | NA |
 * |REQ_BMS_LOG_VEU_002  | #Log_event_drv_cyc_end, LOG_TABLE(event_type) | NA |
 * |REQ_BMS_LOG_VEU_003  | #log_tbl_intervals, LOG_TABLE(interval_count) | NA |
 * |REQ_BMS_LOG_VEU_004  | #Log_table_e, #log_tbl_intervals, #INTERVALS() | NA |
 * |REQ_BMS_LOG_VEU_005  | #Log_integrals_e, #log_nvdata_s::data_integration, #uint32_t, log_update_integral_data() | NA |
 * |REQ_BMS_LOG_CV_001   | #Log_data_extremes_s::highest_cell_v, #Log_data_extremes_s::cell_num_hi_v, log_update_data_extremes() | pat_log__cell_v_min_max() |
 * |REQ_BMS_LOG_CV_002   | #Log_data_extremes_s::lowest_cell_v, #Log_data_extremes_s::cell_num_lo_v, log_update_data_extremes() | pat_log__cell_v_min_max() |
 * |REQ_BMS_LOG_DCV_001  | log_tbl_en_cond_cell_delta_v, #Log_en_cond_e | pat_log__cell_delta_v_table() |
 * |REQ_BMS_LOG_DCV_002  | #mdm_cell_v_delta | pat_log__cell_delta_v_table() |
 * |REQ_BMS_LOG_DCV_003  | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables(), log_table_eval() | pat_log__cell_delta_v_table() |
 * |REQ_BMS_LOG_DCV_004  | #log_tbl_intervals, LOG_TABLE(interval_count) | pat_log__cell_delta_v_table() |
 * |REQ_BMS_LOG_DCV_005  | #Log_table_e, #log_tbl_intervals, #INTERVALS() | pat_log__cell_delta_v_table() |
 * |REQ_BMS_LOG_CT_001   | #Log_data_extremes_s::highest_cell_t, #Log_data_extremes_s::cell_num_hi_t, log_update_data_extremes() | pat_log__cell_t_min_max() |
 * |REQ_BMS_LOG_CT_002   | #Log_data_extremes_s::lowest_cell_t, #Log_data_extremes_s::cell_num_lo_t, log_update_data_extremes() | pat_log__cell_t_min_max() |
 * |REQ_BMS_LOG_DCT_001  | log_tbl_en_cond_cell_delta_t, #Log_en_cond_e | pat_log__cell_delta_t_table() |
 * |REQ_BMS_LOG_DCT_002  | #mdm_cell_t_delta | pat_log__cell_delta_t_table() |
 * |REQ_BMS_LOG_DCT_003  | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables(), log_table_eval() | pat_log__cell_delta_t_table() |
 * |REQ_BMS_LOG_DCT_004  | #log_tbl_intervals, LOG_TABLE(interval_count) | pat_log__cell_delta_t_table() |
 * |REQ_BMS_LOG_DCT_005  | #Log_table_e, #log_tbl_intervals, #INTERVALS() | pat_log__cell_delta_t_table() |
 * |REQ_BMS_LOG_BAL_001  | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables(), log_table_eval(), log_get_cell_bal_data() | NA |
 * |REQ_BMS_LOG_BAL_002  | #log_tbl_intervals, LOG_TABLE(interval_count) | NA |
 * |REQ_BMS_LOG_BAL_003  | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables(), log_table_eval(), log_get_cell_bal_data() | NA |
 * |REQ_BMS_LOG_BAL_004  | #log_tbl_intervals, LOG_TABLE(interval_count) | NA |
 * |REQ_BMS_LOG_BAL_005  | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables(), log_table_eval_vtbm_actv_bal(), log_get_cell_bal_data() | NA |
 * |REQ_BMS_LOG_BAL_006  | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables(), log_table_eval_vtbm_pasv_bal(), log_get_cell_bal_data() | NA |
 * |REQ_BMS_LOG_VCUR_001 | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables(), log_table_eval() | pat_log__veh_bus_i_table() |
 * |REQ_BMS_LOG_VCUR_002 | #Log_en_cond_veh_ctc_closed, LOG_TABLE(enable_cond) | pat_log__veh_bus_i_table() |
 * |REQ_BMS_LOG_VCUR_003 | #log_tbl_intervals, LOG_TABLE(interval_count) | pat_log__veh_bus_i_table() |
 * |REQ_BMS_LOG_VCUR_004 | #Log_table_e, #log_tbl_intervals, #INTERVALS() | pat_log__veh_bus_i_table() |
 * |REQ_BMS_LOG_CCUR_001 | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables(), log_table_eval() | pat_log__chg_bus_i_table() |
 * |REQ_BMS_LOG_CCUR_002 | #Log_en_cond_chg_ctc_closed, LOG_TABLE(enable_cond) | pat_log__chg_bus_i_table() |
 * |REQ_BMS_LOG_CCUR_003 | #log_tbl_intervals, LOG_TABLE(interval_count) | pat_log__chg_bus_i_table() |
 * |REQ_BMS_LOG_CCUR_004 | #Log_table_e, #log_tbl_intervals, #INTERVALS() | pat_log__chg_bus_i_table() |
 * |REQ_BMS_LOG_DPL_001  | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables(), log_table_eval() | NA |
 * |REQ_BMS_LOG_DPL_002  | #Log_en_cond_veh_ctc_closed, LOG_TABLE(enable_cond) | NA |
 * |REQ_BMS_LOG_DPL_003  | #log_tbl_intervals, LOG_TABLE(interval_count) | NA |
 * |REQ_BMS_LOG_DPL_004  | #Log_table_e, #log_tbl_intervals, #INTERVALS() | NA |
 * |REQ_BMS_LOG_RPL_001  | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables(), log_table_eval() | NA |
 * |REQ_BMS_LOG_RPL_002  | #Log_en_cond_veh_ctc_closed, LOG_TABLE(enable_cond) | NA |
 * |REQ_BMS_LOG_RPL_003  | #log_tbl_intervals, LOG_TABLE(interval_count) | NA |
 * |REQ_BMS_LOG_RPL_004  | #Log_table_e, #log_tbl_intervals, #INTERVALS() | NA |
 * |REQ_BMS_LOG_IR_001   | NA | NA |
 * |REQ_BMS_LOG_IR_002   | NA | NA |
 * |REQ_BMS_LOG_IR_003   | NA | NA |
 * |REQ_BMS_LOG_IR_004   | NA | NA |
 * |REQ_BMS_LOG_COO_001  | #Log_table_e, #log_nvdata_s::table_bins, log_update_tables(), log_table_eval() | NA |
 * |REQ_BMS_LOG_COO_002  | #log_tbl_intervals, LOG_TABLE(interval_count) | NA |
 * |REQ_BMS_LOG_COO_003  | #Log_table_e, #log_tbl_intervals, #INTERVALS() | NA |
 * |REQ_BMS_LOG_HVI_001  | #Log_event_counters_e, #log_nvdata_s::event_counters, #Log_event_hvil_opened, #Log_en_cond_any_ctc_closed, EVENT_COUNTER(event_type, enable_cond) | pat_log__hvil_open_cntr() |
 * |REQ_BMS_LOG_HVI_002  | #Log_event_counters_e, #log_nvdata_s::event_counters, #Log_event_hvil_opened, #Log_en_cond_all_ctc_open, EVENT_COUNTER(event_type, enable_cond) | pat_log__hvil_open_cntr() |
 * |REQ_BMS_VMD_CTCC_001 | #log_prechg_ctc_end_of_life_th |  |
 * |REQ_BMS_VMD_CTCC_002 | #log_veh_pos_ctc_end_of_life_th |  |
 * |REQ_BMS_VMD_CTCC_003 | #log_veh_neg_ctc_end_of_life_th |  |
 * |REQ_BMS_VMD_CTCC_004 | #log_chg_pos_ctc_end_of_life_th |  |
 * |REQ_BMS_VMD_CTCC_005 | #log_chg_neg_ctc_end_of_life_th |  |
 * |REQ_BMS_VMD_CTCC_006 | #log_prechg_ctc_wear_x_current, #log_prechg_ctc_wear_y_increment, #log_veh_pos_ctc_wear_x_current, #log_veh_pos_ctc_wear_y_increment, #log_veh_neg_ctc_wear_x_current, #log_veh_neg_ctc_wear_y_increment, #log_chg_pos_ctc_wear_x_current, #log_chg_pos_ctc_wear_y_increment, #log_chg_neg_ctc_wear_x_current, #log_chg_neg_ctc_wear_y_increment |  |
 * |REQ_BMS_VMD_CTCC_007 | #log_veh_pos_ctc_wear_x_current, #log_veh_pos_ctc_wear_y_increment, #log_veh_neg_ctc_wear_x_current, #log_veh_neg_ctc_wear_y_increment |  |
 * |REQ_BMS_VMD_CTCC_008 | #log_prechg_ctc_wear_x_current, #log_prechg_ctc_wear_y_increment, #log_chg_pos_ctc_wear_x_current, #log_chg_pos_ctc_wear_y_increment, #log_chg_neg_ctc_wear_x_current, #log_chg_neg_ctc_wear_y_increment |  |
 * |REQ_BMS_LOG_CTC_005  | #log_nvdata_s::data_integration |  |
 * |REQ_BMS_LOG_CTC_006  | log_detect_event_vehicle_ctc(), log_detect_event_charger_ctc(), log_lookup_ctc_wear_inc(), log_update_integral_data() |  |
 * |REQ_BMS_LOG_CTC_007  | log_post_process() |  |
 * |REQ_BMS_LOG_CTC_008  | log_post_process() |  |
 * |REQ_BMS_LOG_CTC_009  | log_post_process() |  |
 * |REQ_BMS_LOG_CTC_010  | log_post_process() |  |
 * |REQ_BMS_LOG_CTC_011  | log_post_process() |  |
 * |REQ_BMS_LOG_CTC_012  | log_post_process() |  |
 * |REQ_BMS_LOG_CTC_013  | log_post_process() |  |
 * |REQ_BMS_LOG_CTC_014  | log_post_process() |  |
 * |REQ_BMS_LOG_CTC_015  | log_post_process() |  |
 * |REQ_BMS_LOG_CTC_016  | log_post_process() |  |
 * |REQ_BMS_LOG_CTC_017  | log_post_process() |  |
 * |REQ_BMS_LOG_CTC_018  | log_post_process() |  |
 * |REQ_BMS_LOG_CTC_019  | log_post_process() |  |
 * |REQ_BMS_LOG_CTC_020  | log_post_process() |  |
 * |REQ_BMS_LOG_CTC_021  | log_post_process() |  |
 *
 * </center>
 * ##Approach used
 *
 * The log module deals with four types of data logs:
 *  + Event Counters
 *  + Measurement Extremes
 *  + Tables
 *  + Integrators
 *
 * For simplicity of design, and therefore improved maintenance and easier
 * testing, the log data items are configured in the mmc_log.def file. During the 
 * build process, mmc_log.def is used to generated enumerations in log_cfg.h for each 
 * log type containing all log items, as well as any necessary configuration parameters
 * in the respective source files.
 *
 * Generally speaking, data logging is triggered by some event and takes place provided
 * certain enable conditions are satisfied. Suitable enumerations are defined in 
 * log_types.h that list all required events, #Log_event_e, and all enable conditions,
 * #Log_en_cond_e. New events and enable conditions may be added to these if required 
 * in the future. The enumerations define the position of the corresponding 
 * event/enable condition flag in the bit fields #log_event_status and 
 * #log_en_cond_status respectively. Each log item can only be linked to a single event
 * and single enable condition. 
 *
 * The state of the event flags in #log_event_status is updated every time the 
 * log_update() function is called. These flags will then trigger the subsequent logging 
 * process if set. The events are detected even when logging itself is suspended (paused) 
 * leading to the log event being missed. The reason for this being that the event 
 * detection does not introduce much of a computational overhead and maintains the 
 * defined period of the 'periodic log event'. Subsequent processing however will only 
 * occur if logging is enabled (not paused). This includes collecting additional log 
 * data, checking all enable conditions and performing and evaluating the various log 
 * items. 
 
 * The collection of additional log data is performed by log_collect_data(). The purpose 
 * of the function is to collect or compute data that cannot be simply referenced in the 
 * mmc_log.def configuration file as a public variable. Since most data is not required
 * at all times, the data collection process is also driven by the events captured in
 * #log_event_status.
 * 
 * ###Event Counters
 * Event Counters are used to hold the number of times an event has occurred e.g.
 * number of successful charge cycles and are implemented in log_counter.c.
 *
 * Although the requirements for the capacity of the counters vary, they are
 * identically stored in an array of type uint32.
 * This case is more memory efficient than for tables as all counters except
 * the 2 for HVIL broken require 32 bit storage. They are defined in
 * log_nvdata_s::event_counters where the array is indexed using the enum 
 * #Log_event_counters_e. These counters are only ever incremented in single steps,
 * if the configured event  has occurred and the enable condition has been satisfied. 
 * This is done with a call to log_update_event_counters() to increment the appropriate 
 * event counter.
 *
 * The event counters are configured in mmc_log.def using the EVENT_COUNTER macro. The
 * macro consists of three arguments, the unique event counter name, the #Log_event_e
 * event is being counted and the enable condition #Log_en_cond_e.
 *
 * ###Measurement Extremes
 * Measurement Extremes capture data related to the maximum and minimum values of 
 * certain measurements. The implementation, in log_extremes.c, is manual in that the 
 * measurement checks and subsequent logging is hardcoded for each log item - there 
 * is no configuration in mmc_log.def. The manual approach was chosen as the measurement
 * checks are straight forward and an automated solution using a configuration in 
 * mmc_log.def would result in considerably more complex code.
 * Checking for measurement extreme values is not driven by any particular event and
 * hence is performed during every log cycle as long as logging is not paused. Presently
 * the process is not computationally expensive however should this change in the 
 * future, it would be acceptable to trigger it by the periodic event instead. 
 *
 * ###Tables
 * Tables collect the histogram data related to a specific log item. Tables are 
 * implemented in log_table.c in a fairly generic way such that standard tables can be
 * fully configured in the mmc_log.def file without having to modify the source code.
 * Exceptions exist for tables that require logging of multiple ranges etc. which 
 * require custom evaluation functions. 
 * 
 * Tables are configured in mmc_log.def via the macros LOG_TABLE and INTERVALS. 
 * LOG_TABLE consists of the following arguments:
 * -# A unique name for the table.
 * -# The event #Log_event_e that triggers the log action.
 * -# The log enable condition #Log_en_cond_e.
 * -# Address of the source data. May be set to NULL for custom evaluation tables.
 * -# Source data type, see #Log_src_data_type_e. May be set to #Log_src_data_type_none 
 *    for custom evaluation tables.
 * -# Pointer to the source data validity function. May be NULL in which case the  
 *    source data is assumed valid.
 * -# Number of defined intervals (bins) for the table.
 * -# Pointer to the custom evaluation function. Can be NULL in which case the 
 *    standard evaluation function is used.
 * 
 * The INTERVALS macro defines the table interval ranges. It is of fixed size, 13, 
 * to allow up to 12 ranges (bins). As most tables do not require 12 ranges, unused
 * entries should be set to '0'. The intervals are defined as #int32_t values in order
 * to cover all required ranges thus far.
 * 
 * The data to be logged in RAM and EEPROM is stored in #log_nvdata_s::table_bins.
 * The structure is made up of individual arrays defined for each table. This approach
 * allows variable length tables required for efficient EEPROM utilisation. Pointers
 * to the table data arrays are defined in #log_tbl_bins providing access to the tables
 * using the #Log_table_e enumeration.
 *
 * Most of the tables record time data and must be capable of storing up to 10 years 
 * of continuous usage data. To satisfy this requirement, the bin type has been set to 
 * #uint32_t. Furthermore, individual tables may be disabled altogether to provide 
 * flexibility when it comes to resource management. This is achieved by defining the 
 * corresponding table disable macro (see mmc_log.def). Note however, by disabling
 * individual tables the LOG NVM layout will change hence the version macro
 * #LOG_NVS_LAYOUT_VERSION will need to be incremented.
 *
 * The table log process is invoked by calling the function log_update_tables().
 * The log_update_tables function cycles through all available tables, checking 
 * whether the configured event has occurred and the enable condition has been 
 * satisfied. If both conditions are satisfied and the source data is deemed valid,
 * the log table is evaluated. A custom evaluation function is invoked if defined,
 * otherwise the default function log_table_eval() is used.
 * Log table evaluation consists establishing which range the source data lies in 
 * and incrementing the appropriate bin. In the default case, the source data is 
 * converted to #int32_t to match the type of the corresponding intervals. This 
 * conversion will not lead to any data loss for the current set of log table
 * requirements.
 * In order for a value to be classified as in-range of an arbitrary bin, it must
 * satisfy the following condition: BIN_lo_interval < VALUE <= BIN_hi_interval
 * Furthermore, the implementation assumes that the table intervals are defined in
 * a monotonically increasing manner.
 *
 * ###Integrators
 * Integrators are used to accumulate measurement data when triggered by an event.
 * The individual integrators are configured in mmc_log.def via the DATA_INTEGRATION
 * macro. The macro consists of the following arguments:
 * -# Unique integrator name.
 * -# The event #Log_event_e that triggers the log action.
 * -# The log enable condition #Log_en_cond_e.
 * -# Address of the source data. 
 * -# Pointer to the source data validity function. May be NULL in which case the  
 *    source data is assumed valid.
 *
 * The integrator log is implemented in log_integ.c with the accumulated data 
 * stored in log_nvdata_s::data_integration[]. Individual integrators are indexed by 
 * by the #Log_integrals_e enumeration. Current log requirements have a close 
 * relationship between the required integrator items and some log tables in that they
 * both rely on the same input data. The reasons for keeping the two types of logs
 * separate instead of including integrators as an additional element in the table 
 * configuration are listed below:
 * - By keeping the log functionality separate, the solution is more flexible to any 
 *   future changes. These changes may include different event/enable conditions for 
 *   the integrated measurement and its associated table, addition of new integrator 
 *   items that are not related to any table etc.
 * - The requirements call for a means of disabling tables but not the integrated 
 *   data values. If the integrators were included as part of the table, the disable
 *   macro would apply to them as well.
 * - Keeping the two types of functionality separate results in a cleaner and more 
 *   readable solution with very little additional overhead.
 *
 * ##Initialisation
 * log_init() will be called by the application on start up.
 * The RAM copy of the image will contain the default value if the
 * restoration was unsuccessful otherwise it contains the data restored from
 * the EEPROM. It also starts the 200ms tick timer & captures the timestamp in
 * the variable #log_time_stamp.
 *
 * ##Logging Updates
 * log_update() will be called by the application foreground task
 * when new cell data etc. is available, probably every 10ms.
 *
 * Event detection will be executed during every cycle. Assuming logging is not
 * paused, data collection and enable condition checks are also executed in every
 * cycle. Whilst event detection and enable conditions checks do not add much of a
 * computational burden, data collection may be more involved. To prevent certain
 * data collections/calculations are only performed on an event, i.e. only when 
 * needed. Individual log type updates are invoked which then determine whether  
 * logging of each log item should take place based on the state of the event and 
 * enable condition flags.
 *
 * ##Storage
 * The logged data will be stored in EEPROM and mirrored in RAM. The reading and
 * writing of this data to and from EEPROM happens outside of the logging
 * feature and is taken care by the NVS Manager feature.
 *
 * All logging data stored in EEPROM is declared in a single structure,
 * #log_nvs_wrapper_s, which is contiguous and easily identified.
 *
 * The Logging feature maintains a date and time stamp within the logged data
 * block (#log_nvdata_s::timestamp). This is updated whenever the data block is
 * initialised.
 *
 * Function log_reset_data() is provided so that the application can cause the
 * logged data block to be re-initialised.
 *
 * ##Data Backup / Restore
 * Functions log_pause() and log_resume() are provided so that the
 * application can stop the logging feature from writing to the logging data
 * (#log_nvdata_s).
 * The Diagnostics feature can then directly read or write
 * the data block to perform backup and restore.
 *
 * ##Configuration and Calibration
 * All configurable log items are defined in mmc_log.def. The configuration is achieved
 * with the aid of the following macros:
 * + EVENT_COUNTER(counter, event_type, enable_cond)
 *   Configures the event counter log items.
 * + LOG_TABLE(table, event_type, enable_cond, src_data_adr, src_data_type, 
 *             validity_fun, interval_count, custom_eval_fun)
 *   Configures the table log items.
 * + INTERVALS(low_limit, Up_1st_Int, Up_2nd_Int, Up_3rd_Int, Up_4th_Int, 
 *             Up_5th_Int, Up_6th_Int, Up_7th_Int, Up_8th_Int, Up_9th_Int, 
 *             Up_10th_Int, Up_11th_Int, Up_12th_Int)
 *   Configures the range boundaries that define each table log item bin. Values 
 *   specified by the INTERVALS macro must be monotonically increasing. Intervals
 *   must be specified in a way that accounts for the scaling of the input data, 
 *   i.e. using TEMP(<interval_value>) if the input data is of type temperature_t.
 *   Furthermore, any unused entries should be set to 0. A particular value will be 
 *   considered as in-range of a bin when: BIN_lo_interval < VALUE <= BIN_hi_interval
 *   For example, BIN 1 range would be given by [low_limit Up_1st_Int],
 *                BIN 2 range would be given by [Up_1st_Int Up_2nd_Int] etc.
 * + DATA_INTEGRATION(integral, event_type, enable_cond, src_data_adr, validity_fun)
 *   Configures the integrator log items.
 *
 * ##Fault Detection
 * The requirement document does not specify any fault conditions that should
 * be reported. The onus of reporting data validity lies on the individual module.
 * A mechanism is put in place by the log module to allow other modules to report
 * the data validity through a function whose prototype is:
 *
 * bool_t function_name(void).
 *
 * ##Limitations
 *
 * ##Integration
 * To use this feature, you must:
 * -# Include log.h and call log_init() once from initialisation code.
 * -# Call log_update() from the main application task after each time cell
 *      data is updated.
 *
 * @defgroup LOG Logging
 * @ingroup platform
 */


/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "log_types.h"
#include "nvs_cfg.h"    /* For NVS configuration. */
#include "hardware.h"   /* For INTERNAL_ERROR */


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

//extern FRAM log_nvs_wrapper_s  log_ram_image;   /**< Log NV data block */
extern bool_t log_data_reset;                   /**< Log data reset confirmation flag */


/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/**************************************************************************//**
 *  Initialisation of log module
 * This function is used to initialize the log data.
 *
 * @mantis{3671} Update the implementation when the NVS module is designed.
 *****************************************************************************/
extern void log_init(void);

/**************************************************************************//**
 *  Updates the tables and counters with the currently
 *  available data. Called by the main application task.
 *
 *  If not paused (indicated by #log_paused), updates the tables and
 *  counter with the currently available data. Otherwise does nothing.
 *
 *  Uses private functions log_start_process() and log_collect_data() to
 *  do this.
 *****************************************************************************/
extern void log_update(void);

/**************************************************************************//**
 *  Causes the data and tables to be reset.
 *
 *  This function causes the #log_nvdata_s data structure to be reset
 *  and the timestamp updated with the current value from the system.
 *****************************************************************************/
extern void log_reset_data(void);

/**************************************************************************//**
 * Interface for reprogramming the log data.
 *
 * This function is called when a new log data is to be programmed. It copies
 * the data present at the location pointed by the pointer new_data_p and
 * updates the RAM image #log_nvdata_s. This data would then be programmed into
 * the NVM during the shutdown event.
 *
 * @param new_data_p Pointer to the new data block.
 *
 * @retval Returns the status of copying the data into the RAM image.
 *  + TRUE When the data is copied successfully.
 *  + FALSE When the data is not copied successfully.
 *****************************************************************************/
extern bool_t log_reprogram_data(FPTR log_nvdata_s* new_data_p);

/**************************************************************************//**
 * Interface for retrieving the stored log data.
 *
 * This function copies the log data present in its RAM image log_nvdata_s to
 * the location pointed by the dest_p pointer.
 *
 * @param des_p Destination pointer where the data needs to be stored.
 *
 * @retval Returns the status of copying the data to the desired location.
 *  + TRUE When the data is copied successfully.
 *  + FALSE When the data is not copied successfully.
 *****************************************************************************/
extern void log_retrieve_data(FPTR log_nvdata_s* des_p);

/**************************************************************************//**
 * Interface for clearing stored log data.
 *
 * Currently this function clears the contactor wear counters. It is anticipated
 * that development vehicles will encounter contactor wear faults and this way
 * we provide a means of clearing the fault trigger.
 * @todo: this function is likely to change as we need to clear other data.
 *****************************************************************************/
extern void log_clear_data(void);

#endif      /* LOG_H */
