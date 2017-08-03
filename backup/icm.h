/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   icm.h
 *
 * description:   
 * ICM public header file, features external to ICM should consume this file.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(ICM_H)
#define      ICM_H
/**
 *
 * ICM public header file, features external to ICM should consume this file.
 *
 * @file
 * @ingroup icm
 */

/* Copyright (c) Linkedriving Tech 2012,2013 - All rights reserved */

/**
 *
 * This feature is responsible for managing the CAN communications on the
 * internal bus between the MMC and VTBMS. It is considered the owner of the
 * internal 'can' driver feature and must service it appropriately.
 *
 * This feature is responsible for:
 *   <ul>
 *   <li> Controlling the power to the VTBMs
 *   <li> Sending the appropriate message sequences to the VTBMs given the system mode
 *   <li> Making the necessary calls to the CAN driver API to implement each sequence.
 *   <li> Performing processing on the received inputs e.g. checking
 *   returned status and making this available to the strategy and deriving
 *   information such as max, min etc.
 *   <li> Initiation of measurement cycles
 *   <li> Checking for internal CAN faults and recording them
 *   </ul>
 *
 * Usage
 * ------------------
 * - icm_init() should be called at MMC startup
 * - icm_update() should be called once every foreground task invocation
 * - The system should wait for ICM_IS_READY() before attempting any BMS
 * functionality that involves the battery e.g. before closing contactors.
 *
 * Only the icm.h header is intended to be used by features external to icm
 * The other headers define interfaces between components internal to icm.
 *
 * Overall Structure
 * ---------------------
 *  - icm.c Contains the implementation of the state machine controlling the
 *  sequence of commands sent.
 *  - @ref icm_data maps between internal data structures and commands
 *  - @ref icm_timeout is used by icm_data to determine whether the most
 *  recent command has timed out.
 *  - @ref icm_fault is used by icm_data for fault detection and tracking
 *  - @ref icm_client_frames is used to convert commands between their
 *  application and down-the-wire representation.
 *  - @ref icm_client_rx pops incoming CAN frames off the queue and passes them
 *  to the appropriate handler.
 *
 * State Machine
 * =================
 *
 * @par Startup
 * 'Initialisation' of this feature involves communication with VTBMs, as such
 * it is implemented in the  icm_update() function executed once the system is
 * already running.
 * Callers should use  ICM_IS_READY() to determine whether ICM has started up
 * successfully.
 * The top level of the state machine is indicated in the #icm_state variable.
 *
 * At startup the state machine moves into #IcmState_off represting the VTBMs
 * being switched off. If it is determined that communication is needed
 * @ref icm_want_comms.  Power to the VTBMs will be enabled and ICM will move
 * into #IcmState_waiting for a period of time allowing for VTBM start-up.
 *
 * After the VTBM startup interval has expired ICM moves into
 * @ref IcmState_connecting and attempts to exchange CAN with the VTBMs.
 *
 * Once connection is succeddful, ICM moves into IcmState_active during which
 * it will repeatedly measure cell voltages etc.
 *
 * If communication is no longer required, ICM moves into #IcmState_shutdown
 * and a sequence of operations to gracefully shut down the VTBMs is performed.
 *
 * Connecting - icm_state_connecting()
 * -------------------------------------
 *  - Query VTBMs for protocol version  icm_connecting_version()
 *  - Transmit the configuration
 *  - Confirm receipt of configuration  icm_connecting_config()
 *  - Check reference voltages          icm_connecting_refs()
 *  - Check cell voltage and temperature    icm_connecting_cellmeas()
 *
 *  Once these tasks have been performed, this feature considers itself ready
 *  and flags its status as such. BMS contactors cannot be closed until this
 *  sequence has been completed.
 * The substate is of type #IcmConnecting_e and stored in
 * @ref icm_connecting_state
 *
 * Active measurement - icm_state_active()
 * ----------------------------------------
 *  Once a connection has been establisted a periodic measurement task is
 *  performed. ICm will:
 *  - perform a measurement and gather data     icm_active_measure()
 *  - request and update status for each VTBM   icm_active_status()
 *  - optionally measure references if due      icm_active_refs()
 *  - transmit balancing demand                 icm_schedule_balancing()
 *
 * If a VTBM declares itself unconfigured when the MMC is in this state, it
 * move back to connecting.
 * The substate is defined by #IcmActive_e
 *
 * Switching off - icm_state_shutdown()
 * ---------------------------------------
 * The VTBM hardware requires that they are put through a graceful shutdown
 * sequence.
 * The substate is defined by #IcmShutdown_e and stored in
 * @ref icm_shutdown_state
 *
 * @par State machine transitions
 * Note that the state machine only moves by 1 transition per execution so
 * when a substate meets a condition for leaving the parent state, that
 * parent state will not transition away until the next time the state machine
 * is called.
 *
 * <HR>
 *
 * XGATE usage
 * ====================
 * ICM doesn't use the XGATE to decode incomming messages, though this was
 * originally planned. Artefacts of this may be found in the code, such as 
 * xg_ prefixed functions in the ICM code. These all actually run on the S12
 * core, not the XGATE.
 *
 *
 * Communication
 * ====================
 * @par Timeouts
 * Timeouts for commands are all monitored on the S12 micro.
 * The faults recorded on timeout are recorded against a specific VTBM, so
 * on detecting that insufficient data has arrived, ICM must inspect the
 * state of all data received from each VTBM to determine which one has caused
 * the timeout.
 *
 * @par RX and TX
 * CAN RX and TX is performed using a circular buffer to the CAN driver
 * @ref ican.
 * At each call of icm_update() the icm_read_rx_queue() checks for any new
 * incoming messages.
 * Messages are sent by using the appropriate trigger function in icm_data.c
 * e.g. icm_trigger_meas_cfg_cells()

 * Fault detection
 * =================
 *
 * Fault detection is performed by inspecting the data in @ref icm_data and
 * calling the fault setting function in @ref icm_fault. The
 * ICM fault detection functions can be called multiple times per execution
 *  cycle, but will only call the underlying @ref FM functions once.
 *
 * @par Group faults
 * Many of the faults detected by ICM can only be decided if a complete
 * collection of information from the VTBMs is available (or if the strategy
 * has given up waiting for the information. Note that not all fault detection
 * functionality operates at the same rate. Some faults are per-can-frame
 * others are per-command (which may require many can frames to be received
 *  before the fault condition is evaluated)
 * See icm_fault.h for more information on faults.
 *
 * Reprogramming mode
 * ===================
 * When the signal vprg_repgrog_request is asserted, ICM will go through a
 * normal shutdown and enter reprogramming mode, leaving the VTBMs disabled.
 * Control of the internal CAN bus is then handed on to an external feature,
 * which then becomes responsible for turning the VTBMs on/off and
 * communication over the internal CAN bus.
 *
 * ---
 *
 * Messages
 * =========
 * <h2> Transmitted Messages </h2>
 *
 * @par Control Messages
 * These messages are received by all VTBMs and control high-priority tasks
 * such as measurement.
 *
 * @par System configuration
 * These messages broadcast the shape of the pack to all VTBMs. A system
 * config consists of multiple frames.
 *
 * @par Network Management
 * This tells all VTBMs which mode the BMS is in and requests a status
 * response.
 *
 * @par Balancing control
 * These messages are directed to individual VTBMs and indicate how they should
 * configure their balancing output for the duration of the next measure cycle.
 *
 * See @ref icm_data for more information about the mapping between MMC state
 * and network commands.
 *
 * <h2> Received Messages </h2>
 * Incoming frames are popped off the @ref ican queue and passed to
 * icm_process_frame() for checking and decoding. See @ref icm_client_rx()
 *
 * @par VTBM status
 * This is received by the S12 and contains status and error
 * information about each VTBM.
 *
 * @par Cell data
 * These messages contain cell voltages and temperatures transmitted from the
 * VTBMs.
 *
 * @par Cell reference voltages
 * These messages contain measurements of voltages on the VTBM board
 *
 * See the @ref icm_data component for more information
 *
 * Calibration
 * ============
 * - #icm_vtbm_startup_delay Determines the length of time the MMC will wait for
 * the VTBMs before trying to communicate
 * - #icm_shutdown_interval The time to wait for VTBMs to shut down
 * - #icm_refs_interval_ms the interval at which references are checked when
 * the system is active.
 * - #icm_max_shutdown_attempts the number of attempts at stopping the VTBMs
 * before disabling their power.
 *
 * ---
 *
 * <h2> Requirement Satisfaction </h2>
 * <h3> Protocol </h3>
 * Requirements for the ICM feature are in:
 * http://viewvc/viewvc.cgi/trunk/docs/reqs/generic/RU_BMS_MeasDaq.doc?revision=988&root=Q52717_NightWing&view=co
 *
 * The protocol is defined:
 * in http://viewvc/viewvc.cgi/trunk/docs/des/sys/int_comms_spec.doc?revision=2759&root=Q52717_NightWing&view=co
 *
 * See also the faults table in @ref icm_fault
 *
 * | Requirement             | Satisfied By            | Tested By                  |
 * | ------------------------|-------------------------|----------------------------|
 * | REQ_BMS_ICOMM_BUS_001   | TBD - CAN driver?       | TBD CAN driver tests?      |
 * | REQ_BMS_ICOMM_BUS_002   | TBD - CAN driver?       | TBD CAN driver tests?      |
 * | REQ_BMS_ICOMM_VALID_001 | icm_frame_csum()        | pat_icm_invalid_csum()     |
 * | REQ_BMS_ICOMM_VALID_001 | icm_frame_csum()        | pat_icm_valid_csum()       |
 * | REQ_BMS_ICOMM_VALID_002 | icm_valid_checksum()    | pat_icm_invalid_csum()     |
 * | REQ_BMS_ICOMM_VALID_002 | icm_valid_checksum()    | pat_icm_valid_csum()       |
 * | REQ_BMS_ICOMM_VALID_002 | icm_pack_netctl()       | pat_mmc_tx_csum()          |
 * | REQ_BMS_ICOMM_VALID_002 | icm_tx_sysconfig()      | pat_mmc_tx_csum()          |
 * | REQ_BMS_ICOMM_VALID_002 | icm_trigger_measure_none() | pat_mmc_tx_csum()       |
 * | REQ_BMS_ICOMM_VALID_002 | icm_pack_measctl()      | pat_mmc_tx_csum()          |
 * | REQ_BMS_ICOMM_VALID_003 | icm_process_frame()     | pat_icm_bad_csum_data()    |
 * | REQ_BMS_ICOMM_VALID_004 | icm_process_frame()     | pat_icm_invalid_dlc()      |
 * | REQ_BMS_ICOMM_VALID_005 | VTBM only               | VTBM only                  |
 * | REQ_BMS_ICOMM_VALID_006 | icm_trigger_netctl()    | pat_icm_invalid_ver()      |
 * | REQ_BMS_ICOMM_VALID_007 | VTBM only               | VTBM only                  |
 * | REQ_BMS_ICOMM_VALID_008 | VTBM only               | VTBM only                  |
 * | REQ_BMS_ICOMM_VALID_009 | VTBM only               | VTBM only                  |
 * | REQ_BMS_ICOMM_VALID_010 | VTBM only               | VTBM only                  |
 * | REQ_BMS_ICOMM_VALID_011 | VTBM only               | VTBM only                  |
 * | REQ_BMS_ICOMM_VALID_012 | VTBM only               | VTBM Only                  |
 * | REQ_BMS_ICOMM_BOOT_001  | TBD bootloader          | TBD                        |
 * | REQ_BMS_ICOMM_BOOT_002  | TBD bootloader          | TBD                        |
 * | REQ_BMS_ICOMM_MEASCTL_001 | icm_schedule_active() | pat_icm_measctl()          |
 * | REQ_BMS_ICOMM_MEASCTL_002 | icm_trigger_meas_cfg_cells() | pat_icm_measctl()   |
 * | REQ_BMS_ICOMM_MEASCTL_003 | VTBM only             | VTBM only                  |
 * | REQ_BMS_ICOMM_MEASCTL_004 | VTBM only             | VTBM only                  |
 * | REQ_BMS_ICOMM_MEASCTL_005 | VTBM only             | VTBM only                  |
 * | REQ_BMS_ICOMM_MEASCTL_006 | icm_tx_meas_cfg_cells() | pat_icm_measure_action() |
 * | REQ_BMS_ICOMM_MEASCTL_006 | icm_setup_measure_none()| pat_icm_measure_action() |
 * | REQ_BMS_ICOMM_MEASCTL_006 | icm_tx_meas_refs()      | pat_icm_measure_action() |
 * | REQ_BMS_ICOMM_MEASCTL_007 | icm_tx_measctl()        | TBD not used by MMC        |
 * | REQ_BMS_ICOMM_MEASCTL_008 | icm_tx_measctl()        | pat_icm_meas_no_balance()  |
 * | REQ_BMS_ICOMM_MEASCTL_009 | icm_tx_measctl()        | TBD not used by MMC        |
 * | REQ_BMS_ICOMM_MEASCTL_010 | icm_tx_measctl()        | pat_icm_meas_no_balance()  |
 * | REQ_BMS_ICOMM_MEASCTL_011 | TBD not implemented     | TBD not implemented        |
 * | REQ_BMS_ICOMM_MEASCTL_012 | VTBM only               | VTBM only                  |
 * | REQ_BMS_ICOMM_MEASCTL_013 | icm_pack_measctl_target() | pat_icm_measctl_cfg()   |
 * | REQ_BMS_ICOMM_MEASCTL_014 | icm_pack_measctl_target() | TBD not implemented     |
 * | REQ_BMS_ICOMM_MEASCTL_015 | VTBM only                 | VTBM only                  |
 * | REQ_BMS_ICOMM_MEASCTL_016 | icm_build_config_header() | pat_icm_cell_timing()   |
 * | REQ_BMS_ICOMM_MEASCTL_017 | VTBM only                 | VTBM only                  |
 * | REQ_BMS_ICOMM_MEASCTL_018 | VTBM only                 | pat_icm_no_bad_meas()      |
 * | REQ_BMS_ICOMM_MEASREF_001 | icm_tx_meas_refs()        | pat_icm_measure_ref()      |
 * | REQ_BMS_ICOMM_MEASREF_002 | VTBM only                 | VTBM only                  |
 * | REQ_BMS_ICOMM_MEASNONE_001 | icm_trigger_measure_none() | pat_icm_measure_none()   |
 * | REQ_BMS_ICOMM_CONFIG_001   | icm_tx_sysconfig()       | pat_icm_config()         |
 * | REQ_BMS_ICOMM_CONFIG_002   | icm_packed_vtbm_cfg()    | pat_icm_config_counts()  |
 * | REQ_BMS_ICOMM_CONFIG_003   | icm_packed_vtbm_cfg()    | pat_icm_config_counts()  |
 * | REQ_BMS_ICOMM_CONFIG_004   | icm_build_config_header()| pat_icm_config_header()|
 * | REQ_BMS_ICOMM_CONFIG_005   | icm_build_config_body()  | pat_icm_config_body()  |
 * | REQ_BMS_ICOMM_CONFIG_006   |  icm_tx_sysconfig()       | pat_icm_config_crc()   |
 * | REQ_BMS_ICOMM_CONFIG_007   | icm_build_config_header()| pat_icm_t_sensor_prt() |
 * | REQ_BMS_ICOMM_CONFIG_007   | icm_build_config_header()| pat_icm_t_sensor_ntc() |
 * | REQ_BMS_ICOMM_CONFIG_008   | icm_build_config_crc()   | pat_icm_config_crc()       |
 * | REQ_BMS_ICOMM_BAL_001      | icm_tx_balctl()          | pat_icm_balancing()        |
 * | REQ_BMS_ICOMM_BAL_002      | VTBM only                | VTBM only                  |
 * | REQ_BMS_ICOMM_CTLFLAGS_001 | TBD - it is unclear where this demand comes from |    |
 * | REQ_BMS_ICOMM_CELLDATA_001 | xg_rx_celldata()         | pat_icm_rx_cell_volt()     |
 * | REQ_BMS_ICOMM_CELLDATA_002 | xg_unpack_celldata()  | pat_icm_rx_cell_volt()     |
 * | REQ_BMS_ICOMM_CELLDATA_003 | xg_unpack_celldata()  | pat_icm_rx_cell_volt()     |
 * | REQ_BMS_ICOMM_CELLDATA_004 | xg_unpack_celldata()  | pat_icm_rx_cell_temp()     |
 * | REQ_BMS_ICOMM_CELLDATA_005 | xg_unpack_celldata()  | pat_icm_rx_cell_temp()     |
 * | REQ_BMS_ICOMM_CELLDATA_006 | xg_unpack_celldata()  | pat_icm_cell_counter()     |
 * | REQ_BMS_ICOMM_CELLDATA_007 | xg_decode_volt()      | pat_icm_volt_vals()        |
 * | REQ_BMS_ICOMM_CELLDATA_008 | xg_decode_temp()      | pat_icm_temp_vals()        |
 * | REQ_BMS_ICOMM_CELLDATA_009 | xg_decode_volt()    | pat_icm_volt_error()       |
 * | REQ_BMS_ICOMM_CELLDATA_009 | xg_decode_volt()    | pat_icm_volt_missing()     |
 * | REQ_BMS_ICOMM_CELLDATA_009 | xg_decode_volt()    | pat_icm_volt_opencirc()    |
 * | REQ_BMS_ICOMM_CELLDATA_009 | xg_decode_volt()    | pat_icm_volt_shortcirc()   |
 * | REQ_BMS_ICOMM_CELLDATA_009 | xg_decode_temp()    | pat_icm_temp_error()       |
 * | REQ_BMS_ICOMM_CELLDATA_009 | xg_decode_temp()    | pat_icm_temp_missing()     |
 * | REQ_BMS_ICOMM_CELLDATA_009 | xg_decode_temp()    | pat_icm_temp_opencirc()    |
 * | REQ_BMS_ICOMM_CELLDATA_009 | xg_decode_temp()    | pat_icm_temp_shortcirc()   |
 * | REQ_BMS_ICOMM_STATUS_001  | icm_rx_vtbm_status()  | pat_icm_bad_protver()      |
 * | REQ_BMS_ICOMM_STATUS_003  | icm_rx_vtbm_status()  | pat_icm_int_err()          |
 * | REQ_BMS_ICOMM_STATUS_004  | icm_rx_vtbm_status()  | pat_icm_no_eol()           |
 * | REQ_BMS_ICOMM_STATUS_005  | icm_rx_vtbm_status()  | pat_icm_no_config()        |
 * | REQ_BMS_ICOMM_STATUS_006  | icm_rx_vtbm_status()  | pat_icm_balance_status()   |
 * | REQ_BMS_ICOMM_STATUS_007  | icm_rx_vtbm_status()  | pat_icm_balance_status()   |
 * | REQ_BMS_ICOMM_STATUS_008  | icm_rx_vtbm_status()  | pat_icm_errcode()          |
 * | REQ_BMS_ICOMM_STATUS_009  | icm_rx_vtbm_status()  | pat_icm_vtbm_rx_errcode()  |
 * | REQ_BMS_ICOMM_STATUS_010  | icm_rx_vtbm_status()  | pat_icm_vtbm_tx_errcode()  |
 * | REQ_BMS_ICOMM_STATUS_011  | VTBM only             | VTBM only                  |
 * | REQ_BMS_ICOMM_REFDATA_001 | icm_tx_meas_refs()    | pat_icm_rx_refs()          |
 * | REQ_BMS_ICOMM_REFDATA_002 | xg_unpack_refdata()   | pat_icm_rx_actref()        |
 * | REQ_BMS_ICOMM_REFDATA_003 | xg_unpack_refdata()   | pat_icm_rx_passref()       |
 * | REQ_BMS_ICOMM_REFDATA_004 | xg_decode_volt()      | pat_icm_bad_passref()      |
 * | REQ_BMS_ICOMM_REFDATA_004 | xg_decode_volt()      | pat_icm_bad_actref()       |
 * | REQ_BMS_ICOMM_XCP_001     | icm_process_frame()   | pat_icm_ignore_xcp()       |
 * | REQ_BMS_ICOMM_XCP_002     | icm_process_frame()   | pat_icm_ignore_xcp()       |
 * | REQ_BMS_ICOMM_XCP_003     | icm_process_frame()   | pat_icm_ignore_xcp()       |
 *
 * ### ICM 'application logic' feature
 *
 * | Requirement          | Satisfied By                      | Tested by                         |
 * | ---------------------|-----------------------------------|-----------------------------------|
 * | REQ_BMS_ICM_MSG_001  | see above                         | see ICOMM tests                   |
 * | REQ_BMS_ICM_MSG_003  | icm_process_frame_fault()         | pat_icm_unrecognised_message()    |
 * | REQ_BMS_ICM_MSG_005  | icm_check_vtbm_internal_error()   | pat_icm_internal_errcode()        |
 * | REQ_BMS_ICM_MSG_006  | icm_chk_plat_cell_unexpected()    | pat_icm_cell_unexpected()         |
 * | REQ_BMS_ICM_MSG_007  | icm_read_rx_queue()               | pat_icm_ignore_xcp()              |
 * | REQ_BMS_ICM_MSG_008  | icm_process_frame_fault()         | pat_icm_bad_dlc()                 |
 * | REQ_BMS_ICM_MSG_009  | icm_process_frame_fault()         | pat_icm_bad_data()                |
 * | REQ_BMS_ICM_SPO_001  | icm_set_pwr_outputs()             | pat_icm_vtbm_on_power()           |
 * | REQ_BMS_ICM_SPO_002  | icm_state_waiting()               | pat_icm_vtbm_on_comms()           |
 * | REQ_BMS_ICM_SPO_002  | icm_state_waiting()               | pat_timingcheck()                 |
 * | REQ_BMS_ICM_SPO_003  | icm_chk_plat_cell_unexpected()    | pat_icm_cell_unexpected()         |
 * | REQ_BMS_ICM_SPO_004  | icm_validate_status_data()        | pat_icm_invalid_protocol()        |
 * | REQ_BMS_ICM_SPO_005  | icm_chk_cell_timeout()            | pat_icm_init_timeout_cells()      |
 * | REQ_BMS_ICM_SPO_005  | icm_chk_vtbm_timeouts()           | pat_icm_init_timeout_refs()       |
 * | REQ_BMS_ICM_SPO_005  | icm_chk_vtbm_timeouts()           | pat_icm_init_timeout_status()     |
 * | REQ_BMS_ICM_SPO_006  | icm_state_shutdown()              | pat_icm_shutdown()                |
 * | REQ_BMS_ICM_SPO_007  | icm_shutdown_wait()               | pat_icm_shutdown()                |
 * | REQ_BMS_ICM_SPO_008  | icm_mmc_keepalive()               | pat_icm_state_shutdown()          |
 * | REQ_BMS_ICM_SPO_009  | icm_state_shutdown()              | pat_icm_bal_shutdown()            |
 * | REQ_BMS_ICM_SPO_010  | icm_update_state()                | pat_icm_power_off()               |
 * | REQ_BMS_ICM_SPO_011  | icm_connecting_config_enter()     | pat_icm_reconfig()                |
 * | REQ_BMS_ICM_SPO_012  | icm_connecting_config()           | plat_icm_reconfig()               |
 * | REQ_BMS_ICM_SPO_012  | icm_state_active()                | pat_icm_reconfig()                |
 * | REQ_BMS_ICM_SPO_013  | icm_check_cfg_responses()         | pat_icm_unconfig()                |
 * | REQ_BMS_ICM_SPO_014  | icm_chk_vtbm_eolcal_response()    | pat_icm_eol()                     |
 * | REQ_BMS_ICM_SPO_015  | icm_chk_vtbm_hw_mismatch()        | pat_icm_hw_sw_incompatible()      |
 * | REQ_BMS_ICM_MEAS_001 | icm_active_measure_enter()        | pat_icm_measure_cycle()           |
 * | REQ_BMS_ICM_MEAS_002 | ICM_IS_READY()                    | pat_mmc_idle()                    |
 * | REQ_BMS_ICM_MEAS_004 | icm_tx_meas_cfg_cells()           | pat_icm_measure_payload()         |
 * | REQ_BMS_ICM_MEAS_005 | icm_set_cmd_timeout()             | pat_icm_measure_timeout()         |
 * | REQ_BMS_ICM_MEAS_006 | icm_measure_completion()          | pat_icm_measure_completion()      |
 * | REQ_BMS_ICM_MEAS_007 | icm_status_completion()           | pat_icm_resp_timeout_status()     |
 * | REQ_BMS_ICM_MEAS_007 | icm_ref_completion()              | pat_icm_resp_timeout_refs()       |
 * | REQ_BMS_ICM_MEAS_011 | icm_connecting_refs_enter()       | pat_icm_connecting_refs()         |
 * | REQ_BMS_ICM_MEAS_012 | icm_update_refs_trigger()         | pat_icm_periodic_refs()           |
 * | REQ_BMS_ICM_MEAS_013 | icm_chk_vtbm_timeouts()           | pat_icm_resp_timeout_refs()       |
 * | REQ_BMS_ICM_MEAS_014 | icm_update_bal_trigger()          | pat_icm_bal_periodic()            |
 * | REQ_BMS_ICM_MEAS_015 | icm_chk_cell_timeout()            | pat_icm_resp_timeout_cells()      |
 * | REQ_BMS_ICM_MEAS_015 | icm_chk_cell_timeout()            | pat_icm_resp_timeout_refs()       |
 * | REQ_BMS_ICM_MEAS_015 | icm_chk_cell_timeout()            | pat_icm_resp_timeout_status()     |
 * | REQ_BMS_ICM_MEAS_016 | icm_chk_plat_n_cell_t_lost()      | pat_icm_measure_timeout()         |
 * | REQ_BMS_ICM_MEAS_017 | icm_chk_plat_n_cell_v_lost()      | pat_icm_measure_timeout()         |
 * | REQ_BMS_ICM_MEAS_018 | icm_chk_plat_vtbm_n_cell_t_lost() | pat_icm_measure_timeout()         |
 * | REQ_BMS_ICM_MEAS_019 | icm_chk_plat_vtbm_n_cell_v_lost() | pat_icm_measure_timeout()         |
 * | REQ_BMS_ICM_MEAS_020 | icm_chk_plat_cell_t_max()         | plat_icm_cell_t_max()             |
 * | REQ_BMS_ICM_MEAS_021 | icm_chk_plat_cell_v()             | plat_icm_cell_v_disconnect()      |
 * | REQ_BMS_ICM_MEAS_023 | icm_chk_uncommanded_bal()         | pat_icm_unexpected_bal()          |
 * | REQ_BMS_ICM_MEAS_024 | icm_chk_uncommanded_bal()         | pat_icm_mismatch_bal()            |
 * | REQ_BMS_ICM_MEAS_025 | icm_chk_unactioned_bal()          | pat_icm_unactioned_bal()          |
 * | REQ_BMS_ICM_MEAS_026 | icm_chk_plat_cell_t_min()         | plat_icm_cell_t_min()             |
 * | REQ_BMS_ICM_MEAS_027 | icm_chk_plat_cell_t_short()       | plat_icm_cell_t_shorted()         |
 * | REQ_BMS_ICM_MEAS_028 | icm_chk_plat_cell_t_open()        | plat_icm_cell_t_disconnect()      |
 * | REQ_BMS_ICM_MEAS_029 | xg_set_volt_minmax()              | hut_set_v_minmax()                |
 * | REQ_BMS_ICM_MEAS_029 | xg_set_volt_minmax()              | hut_set_v_minmax_valid()          |
 * | REQ_BMS_ICM_MEAS_029 | xg_set_temp_minmax()              | hut_set_t_minmax()                |
 * | REQ_BMS_ICM_MEAS_029 | xg_store_cellvolt()               | hut_store_cellvolt()              |
 * | REQ_BMS_ICM_MEAS_029 | hut_store_celltemp()              | hut_store_celltemp()              | 
 * | REQ_BMS_ICM_MEAS_029 | xg_store_celltemp()               | hut_store_celltemp_offset()       |
 * | REQ_BMS_ICM_MEAS_029 | xg_process_measurement()      | hut_process_cell_data_detect_faults() |
 * | REQ_BMS_ICM_MEAS_029 | xg_set_deltas()                   | hut_process_cell_data_delta()     |
 * | REQ_BMS_ICM_REF_001  | icm_chk_vactive_ref()             | pat_icm_vtbm_ref_v_active()       |
 * | REQ_BMS_ICM_REF_002  | icm_chk_vpassive_ref()            | pat_icm_vtbm_ref_v_passive()      |
 * | REQ_BMS_ICM_REF_003  | icm_chk_vpassive_ref()            | pat_icm_vtbm_ref_v_passive_high() |
 * | REQ_BMS_ICM_REF_004  | icm_chk_gnd_ref()                 | pat_icm_vtbm_ref_v_start()        |
 * | REQ_BMS_ICM_REF_005  | icm_chk_dig_ref()                 | pat_icm_vtbm_ref_dig_active()     |
 * | REQ_BMS_ICM_REF_006  | icm_chk_dig_ref()                 | pat_icm_vtbm_ref_dig_passive()    |
 * | REQ_BMS_ICM_REF_007  | icm_chk_dig_ref()                 | pat_icm_vtbm_ref_dig_start()      |
 *
 * @defgroup icm ICM Internal Communications
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "icm_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/* See definition */
extern enum     IcmState_e icm_state;
/* see definition */
extern uint8_t  icm_ref_update_count ;

/* See definition */
extern bool_t   icm_want_comms;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 *  Initialisation function for internal communications, to be called at startup
 */
void icm_init(void);

/**
 * Periodic update function to be called from MMC foreground task */
void icm_update(void);

/**
 * Interface function for use in reprogramming mode, to request VTBMS are
 * enabled or disabled from an external source */
void icm_reprog_enable_vtbms(bool_t enable);

/**
 * Query ICM about the status of a VTBM's balancing */
void icm_get_bal_status(uint8_t vtbm_num,struct icm_bal_status_s *result_p);

/**
 * Returns the current active state required by OBD for a snapshot record */
enum IcmActive_e icm_ss_data_active_state(void);
/**
 * Returns the current connecting state required by OBD for a snapshot record  */
enum IcmConnecting_e icm_ss_data_connecting_state(void);
/**
 * Returns the current shutdown state required by OBD for a snapshot record  */
enum IcmShutdown_e icm_ss_data_shutdown_state(void);

#endif      /* ICM_H */

