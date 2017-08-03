/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   exc.h
 *
 * description:  
 *  External CAN: an interface to CANbedded
 * 
 * 
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
#if !defined(EXC_H)
#define      EXC_H

/**
 *
 * External CAN: an interface to CANbedded
 *
 * @file
 * @ingroup exc
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 * EXC module documentation
 *
 * EXC Overview
 * ------------
 * EXC provides functions and macros, which enables the user to read
 * and write all signals defined with CANdb++ and GENy tools.
 *
 * Approach used
 * -------------
 * EXC provides minimal interface to CANbedded in order to simplify
 * its usage. All macros and functions necessary to access CANbedded signals
 * are defined in EXC header file, so no other file is needed to include.
 *
 * Limitations
 * -----------
 *
 * How to use
 * ----------
 * To initialize and update:
 * + include exc.h
 * + call #exc_init() once to initialize CANbedded and EXC
 * + call #exc_start() once to start CANbedded layer
 * + call #exc_update() periodically at 1ms rate to refresh CANbedded layer
 *
 * To read a signal:
 * + check if a signal's value has been changed with #CAN_RX_SIG_INDICATION
 * macro, if an indication flag is associated to a signal.
 * + read the value with #CAN_RX_SIG macro. If a signal length is longer
 * than 4 bytes, it shall be read with #CAN_RX_SIG_ARRAY macro instead.
 * If signal is a part of a group, it must be read with #CAN_RX_GRP macro
 * first.
 * + clear the indication flag with #CAN_RX_SIG_INDICATION_CLR macro,
 * if an indication flag is associated to a signal.
 *
 * To get a status of a signal enabled to be processed using additional data integrity
 * measures:
 * + get status with CAN_RX_SIG_STATUS() macro using signal name as a parameter. This
 * macro returns one of #exc_int_status_e values
 *
 * To write a signal:
 * + set a new value with #CAN_TX_SIG macro. If the signal
 * is part of a signal group, all signals in the group should be set
 * and then the group itself is written with #CAN_TX_GRP macro
 * + get confirmation that a signal or group has been transmitted with
 * the #CAN_TX_SIG_CONFIRMATION macro
 * + clear confirmation flag with #CAN_TX_SIG_CONFIRMATION_CLR macro
 *
 * To get a binary value of a symbolic value name:
 * + use #CAN_VALUE_NAME with a signal name and value name as parameters.
 * This may be used only if there's a value table associated with a signal.
 *
 * To get a status of a bus:
 * + call exc_get_state() with a #exc_bus_e parameter to get
 * a status of a bus from #exc_state_e
 *
 * All signals, groups, their indication / confirmation flag names
 * and associated value tables are listed
 * in @ref cbd_msgs " Messages, Signals and Value tables"
 *
 * @anchor exc_adi
 * Additional data integrity
 * -------------------------
 * Some messages are required to be processed using additional data integrity
 * measures according to <a href="http://viewvc/viewvc.cgi/trunk/docs/reqs/project/p12/raw/network_and_software_pack/P12_068%2002%20Additional%20Signal%20Data%20Integrity%20Measures.pdf?revision=224&root=Q52717_NightWing&view=co">Additional Signal Data Integrity Measures</a>
 * Transmitting messages may be provided with incrementing counter and checksum according to the
 * algorithm described in above document and having suggested signal names.
 * Receiving messages may be checked for correct counter, checksum and timeout and
 * eventually not passed to the user. Receiving messages may be also configured
 * to report  different levels of failure using fault management service.
 * This is performed during pre-transmit and pre-copy events.
 *
 * All outgoing messages listed in Protected Messages list in above document must be enabled
 * in GENy configuration tool to have a Pre-Transmit function named by the message name prefixed
 * with ExcPreTx_ and listed in the exc_integrity.def file (see bellow) using EXC_INTEGRITY_TX()
 * macro.
 *
 * All incoming messages listed in Protected Messages list in above document
 * must be enabled to have a Pre-Copy function named by the message name prefixed with ExcPreRx_
 * and listed in configuration file using EXC_INTEGRITY_RX() macro. Second parameter shall be
 * timeout time of message in milliseconds, third parameter shall be one
 * of #exc_int_report_level_e enumeration and configures whether a fault related to this
 * message shall be reported to the fault management feature or not. Next parameter shall
 * be a fault code name to be reported, when timeout fault occurs (please note, that previous
 * parameter must be either #Exc_int_timeout_faults or #Exc_int_all_faults in this case).
 * Next parameter configures whether just checksum and counter, just timeout, both or no check
 * shall be performed.
 *
 * All signals listed in the exc_integrity.def file using EXC_INTEGRITY_RX_SIG() macro
 * will be provided with a status signaling its validity. User may read this status
 * using CAN_RX_SIG_STATUS() public macro.
 *
 * Cyclic messages transmission control
 * ------------------------------------
 * Transmission of cyclic messages may be enabled or disabled in run-time. All messages listed
 * in provided exc_cyclic.def file using #EXC_CYCLIC_MSG_CONTROL() macro together with a CAL bool_t
 * type flag will be enabled or disabled for transmission according to the state of this flag
 * in the run-time.
 *
 * Configuration
 * -------------
 * All channels, nodes, messages and signals are configured in CANdb++
 * and GENy (documentation is in separated file). User has to
 * add, rename or remove channel initialization and start in #exc_init
 * and #exc_start functions, if changed in GENy. See note in #exc_init
 * function comment for details. User also has to add all used interrupt
 * handlers to target's handler interrupt file and
 * edit #exc_set_interrupt_levels. CAN Channels 0, 1 and 3 are currently
 * used for vehicle, charger and isa buses.
 *
 * Each target must define a exc_integrity configuration file and configure
 * the build system to copy it to the build directory, renaming it
 * to exc_integrity.def. For the main application, this file is
 * called mmc_exc_integrity.def.
 *
 * Provide exc_cyclic configuration file and configure the build system to
 * copy it to the build directory, renaming it to exc_cyclic.def, if cyclic message
 * transmission control is requested. For the main application, this file is
 * called mmc_exc_cyclic.def.
 *
 * Calibration
 * -----------
 * - #exc_int_check_disabled bypasses data integrity check, if set
 * - #exc_int_veh_inhibit_time inhibits fault checking on the vehicle bus for
 * the time specified
 * - #exc_int_veh_min_supply_voltage and #exc_int_veh_max_supply_voltage configures
 * a voltage range of logic supply at which fault checking on the vehicle bus
 * is enabled
 *
 * Requirement Satisfaction
 * ------------------------
 *
 * | Requirement        | Satisfied by                          | Unit tested | PAT tested |
 * |--------------------|---------------------------------------|-------------|------------|
 * | REQ_BMS_NM_001     | CANbedded NM component                | _not tested_| _not tested_|
 * | REQ_BMS_NM_002     | exc_shutdown_inhibit_update()         |             |             |
 * | REQ_BMS_NM_003     | exc_vehicle_nm()                      |             |            |
 * | REQ_BMS_NM_004     | exc_shutdown_inhibit_update()         |             |            |
 * | REQ_BMS_NM_005     | exc_get_nm_status()                   | test_exc_update_nm_status()|            |
 * | REQ_BMS_VIO_NM_001 | CANbedded NM component                | _not tested_| _not tested_|
 * | REQ_BMS_VIO_NM_002 | CANbedded NM component                | _not tested_| _not tested_|
 * | REQ_BMS_VMD_SM_007 | exc_init()                            |             |            |
 * | REQ_BMS_VMD_SM_008 | exc_init()                            |             |            |
 * | REQ_BMS_VMD_SM_003 | CANbedded NM component, ApplCclWakeUpTrcv()|             |            |
 * | REQ_BMS_VMD_SM_004 | CANbedded NM component                |             |            |
 * | REQ_BMS_VMD_SM_005 | ApplCclStandbyTrcv()                  |             |            |
 * | REQ_BMS_VMD_SM_009 | exc_vehicle_nm()                      |             |            |
 * | REQ_BMS_VIO_IM_010  | ExcPreRx_ECM_BMS_00(), ExcPreRx_CCU_1(), ExcPreRx_DCT_1(), ExcPreRx_MCU_4 | exc_check_rx_message | pat_exc_integrity_veh_ECM_BMS_00_flt(), pat_exc_integrity_veh_CCU_1_flt(), pat_exc_integrity_veh_DCT_1_flt(), pat_exc_integrity_veh_MCU_4_flt() |
 * | REQ_BMS_VIO_IM_011  | ExcPreTx_BMS_*()                     | exc_check_tx_message | pat_exc_integrity_veh_BMS_*() |
 * | REQ_BMS_VIO_IM_002 | exc_calculate_checksum(), exc_check_rx_message() | test_exc_data_integrity_rx() | pat_exc_integrity_veh_ECM_BMS_00_flt()|
 * | REQ_BMS_VIO_IM_001 | exc_increase_counter(), exc_check_rx_message() | test_exc_data_integrity_rx()| pat_exc_integrity_veh_ECM_BMS_00_flt()|
 * | REQ_BMS_VIO_IM_007 | exc_check_rx_message()                | test_exc_data_integrity_rx()| pat_exc_integrity_veh_*_flt()|
 * | REQ_BMS_VIO_IM_003 | exc_check_rx_message()                | test_exc_data_integrity_rx()| pat_exc_integrity_veh_*_flt()|
 * | REQ_BMS_VIO_IM_006 | CAN_RX_SIG_STATUS()                   | test_exc_data_integrity_rx()| pat_exc_integrity_veh_*_flt()|
 * | REQ_BMS_VIO_IM_005 | exc_check_rx_message()                | test_exc_data_integrity_rx()| pat_exc_integrity_veh_*_flt()|
 * | REQ_BMS_VIO_IM_004 | exc_int_update()                      | test_exc_data_integrity_rx()| pat_exc_integrity_veh_*_flt()|
 * | REQ_BMS_VIO_IM_015 | #exc_msg_timeouts[]                   | exc_hlp_get_msg_timeout_period() | pat_exc_integrity_veh_*_flt()|
 * | REQ_BMS_VIO_IM_008 | #exc_int_msg_data[], exc_int_update() | test_exc_data_integrity_rx()| |
 * | REQ_BMS_VIO_IM_009 | exc_int_update()                      | test_exc_data_integrity_rx()|  pat_exc_integrity_veh_*_flt()|
 * | REQ_BMS_VIO_IM_014 | exc_int_update_fault_handling(), #exc_int_veh_inhibit_time | test_exc_fault_monitor_veh() | pat_exc_integrity_veh_*_flt()|
 * | REQ_BMS_VIO_IM_013 | exc_int_update_fault_handling()       | test_exc_fault_monitor_veh() | pat_exc_integrity_veh_*_flt()|
 * | REQ_BMS_VIO_VFH_001 | exc_int_update_fault_handling(), exc_int_update() |             | pat_exc_integrity_veh_ECM_BMS_00_flt() |
 * | REQ_BMS_VIO_VFH_003 | exc_int_update_fault_handling(), exc_int_update() |             | pat_exc_integrity_veh_CCU_1_flt() |
 * | REQ_BMS_VIO_VFH_005 | exc_int_update_fault_handling(), exc_int_update() |             | pat_exc_integrity_veh_DCT_1_flt() |
 * | REQ_BMS_VIO_VFH_007 | exc_int_update_fault_handling(), exc_int_update() |             | pat_exc_integrity_veh_MCU_4_flt() |
 * | REQ_BMS_VIO_VFH_009 | exc_int_update_fault_handling(), exc_int_update() |             | pat_exc_integrity_veh_chk_flt()   |
 * | REQ_BMS_VIO_VFH_011 | exc_int_update_fault_handling(), exc_int_update() |             | pat_exc_integrity_veh_cnt_flt()   |
 * | REQ_BMS_CBD_CS00_001 | ExcPreTx_BMS_00(), exc_increase_counter() | test_exc_data_integrity_tx()| pat_exc_integrity_veh_BMS_00()|
 * | REQ_BMS_CBD_CS00_002 | ExcPreTx_BMS_00()                   | test_exc_data_integrity_tx()| |
 * | REQ_BMS_CBD_CS00_003 | ExcPreTx_BMS_00(), exc_calculate_checksum() | test_exc_data_integrity_tx()| pat_exc_integrity_veh_BMS_00()|
 * | REQ_BMS_SUP_EXC_001 | exc_update_states()                  | test_exc_fault_report()| |
 * | REQ_BMS_SUP_EXC_003 | exc_update_states()                  | test_exc_fault_report()| |
 * | REQ_BMS_SUP_EXC_005 | exc_update_states()                  | test_exc_fault_report()| |
 * | REQ_BMS_SUP_EXC_002 | exc_update_state(), exc_is_bus_unavailable() | test_exc_fault_report()| |
 * | REQ_BMS_SUP_EXC_004 | exc_update_state(), exc_is_bus_unavailable() | test_exc_fault_report()| |
 * | REQ_BMS_SUP_EXC_006 | exc_update_state(), exc_is_bus_unavailable() | test_exc_fault_report()| |
 * | REQ_BMS_SUP_EXC_007 | exc_update_faults()                  | test_exc_fault_report()| |
 * | REQ_BMS_SUP_EXC_008 | exc_update_faults()                  | test_exc_fault_report()| |
 * | REQ_BMS_SUP_EXC_009 | exc_update_faults()                  | test_exc_fault_report()| |
 * | REQ_BMS_SUP_EXC_010 | exc_update_faults()                  | test_exc_fault_report()| |
 * | REQ_BMS_SUP_EXC_011 | exc_update_faults()                  | test_exc_fault_report()| |
 * | REQ_BMS_SUP_EXC_012 | exc_update_faults()                  | test_exc_fault_report()| |
 * | REQ_BMS_SUP_EXC_013 | CANbedded bus recovery               | _not tested_ | |
 * | REQ_BMS_SUP_EXC_014 | CANbedded bus recovery               | _not tested_| |
 * | REQ_BMS_SUP_EXC_015 | CANbedded bus recovery               | _not tested_| |
 * | REQ_BMS_SUP_EXC_016 | unsatisfiable due to the way CANbedded works | _not tested_| _not tested_|
 * | REQ_BMS_SUP_EXC_019 | unsatisfiable due to the way CANbedded works | _not tested_| _not tested_|
 * | REQ_BMS_SUP_EXC_022 | unsatisfiable due to the way CANbedded works | _not tested_| _not tested_|
 * | REQ_BMS_SUP_EXC_017 | unsatisfiable due to the way CANbedded works | _not tested_| _not tested_|
 * | REQ_BMS_SUP_EXC_018 | unsatisfiable due to the way CANbedded works | _not tested_| _not tested_|
 * | REQ_BMS_SUP_EXC_020 | unsatisfiable due to the way CANbedded works | _not tested_| _not tested_|
 * | REQ_BMS_SUP_EXC_021 | unsatisfiable due to the way CANbedded works | _not tested_| _not tested_|
 * | REQ_BMS_SUP_EXC_023 | unsatisfiable due to the way CANbedded works | _not tested_| _not tested_|
 * | REQ_BMS_SUP_EXC_024 | unsatisfiable due to the way CANbedded works | _not tested_| _not tested_|
 * | REQ_BMS_SUP_EXC_025 | exc_update_faults()                  | test_exc_fault_report()| |
 * | REQ_BMS_SUP_EXC_026 | exc_update_faults()                  | test_exc_fault_report()| |
 * | REQ_BMS_P12_EXC_001 | ExcPreRx_OBC_1(), ExcPreRx_OBC_2()   | test_exc_data_integrity_rx() | pat_exc_integrity_chg_OBC_1_flt(), pat_exc_integrity_chg_OBC_2_flt() |
 * | REQ_BMS_P12_EXC_002 | ExcPreTx_BMS_CH1(), ExcPreTx_BMS_CH2() | test_exc_data_integrity_tx() | pat_exc_integrity_chg_BMS_CH1(), pat_exc_integrity_chg_BMS_CH2() |
 * | REQ_BMS_VIO_CFH_001 | exc_int_update_fault_handling() | test_exc_fault_monitor_chg() | pat_exc_integrity_chg_obc_1_flt() |
 * | REQ_BMS_VIO_CFH_002 | exc_int_update()                | test_exc_data_integrity_timeout() | pat_exc_integrity_*_flt() |
 * | REQ_BMS_VIO_CFH_003 | exc_int_update()                | test_exc_data_integrity_timeout() | pat_exc_integrity_*_flt() |
 * | REQ_BMS_VIO_CFH_004 | exc_int_update()                | test_exc_data_integrity_timeout() | pat_exc_integrity_*_flt() |
 * | REQ_BMS_VIO_CFH_005 | exc_int_update()                | test_exc_data_integrity_timeout() | pat_exc_integrity_*_flt() |
 *
 *
 * @defgroup exc EXC External CAN interface
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#if defined (HOSTED)
#include "exc_stubs.h"
#endif

#if !defined(POLYSPACE) && !defined(HOSTED)
/* normal embedded target, these are excluded from PC and Polyspace builds
 * exc_types.h will pull in any necessary definitions for those targets */
#include "ccl_inc.h"
#include "v_inc.h"
#if defined (UNIT_TEST)
#include "ut_canbedded_stubs.h"
#endif /* UNIT_TEST */
#endif /* POLYSPACE, HOSTED */

#include "exc_types.h"

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern enum exc_int_status_e * CONST exc_int_signal_statuses
    [Exc_int_rx_sig_max + 1];

extern enum exc_nm_status_e exc_veh_nm_status;

extern struct exc_cyclic_control_s exc_cyclic_control[(uint16_t)Exc_msg_tx_handle_max + 1u];

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
void exc_init(void);
void exc_start(void);
void exc_update(void);
void exc_idle_tasks(void);
void exc_int_update(void);
enum exc_nm_status_e exc_get_vehicle_nm_status(void);
bool_t exc_is_bus_unavailable(enum exc_bus_e bus);
bool_t exc_chg_comms_enabled(void);

void CclScheduleTask_Channel(enum exc_bus_e can_channel);

/* declarations of stub functions for unit testing */

/* declaration of stub functions for polyspace */
#ifdef POLYSPACE
/**
 * Overrides CAN_RX_SIG_INDICATION_CLR macro
 */
void PS_CAN_RX_SIG_INDICATION_CLR(void);

/**
 * Overrides CAN_RX_SIG_INDICATION macro
 */
bool_t PS_CAN_RX_SIG_INDICATION(void);

/**
 * Overrides CAN_RX_GRP macro
 */
void PS_CAN_RX_GRP(void);

/**
 * Overrides CAN_RX_SIG macro
 */
uint32_t PS_CAN_RX_SIG(void);

/**
 * Overrides CAN_RX_SIG_STATUS macro
 */
enum exc_int_status_e PS_CAN_RX_SIG_STATUS(void);


/**
 * Overrides CAN_TX_SIG macro
 */
void PS_CAN_TX_SIG(uint32_t val);

/**
 * Overrides CAN_TX_GRP macro
 */
void PS_CAN_TX_GRP(void);

/**
 * Overrides CAN_TX_SIG_CONFIRMATION macro
 */
bool_t PS_CAN_TX_SIG_CONFIRMATION(void);

/**
 * Overrides CAN_TX_SIG_CONFIRMATION_CLR macro
 */
void PS_CAN_TX_SIG_CONFIRMATION_CLR(void);
#endif

/* PRODDER specific flags modifying functionality */
#if defined PRODDER
/** Flag to enable / disable faulty counter value insertion. */
extern bool_t PROD_exc_int_invalid_counter_en;
/** Flag to enable / disable faulty checksum value insertion. */
extern bool_t PROD_exc_int_invalid_checksum_en;
#endif

#endif      /* EXC_H */

