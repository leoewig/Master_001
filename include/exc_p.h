/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      exc_p.h
 *
 * description:    PRIVATE macros
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li    PRIVATE macros
 *
 *****************************************************************************/
#if !defined(EXC_P_H)
#define      EXC_P_H


/**
 * Create transmitting message handle from message name
 */
#define EXC_GET_TX_HANDLE(msg)  EXC_CAT2(IlTxMsgHnd,msg)

/**
 * Enable transmission of single cyclic message.
 */
#if defined (UNIT_TEST)
#define CAN_CYCLIC_START(msg_handle)
#else
#define CAN_CYCLIC_START(msg_handle)    EXC_CAT_FN((void)IlStartCycle,msg_handle)
#endif

/**
 * Disable transmission of single cyclic message.
 */
#if defined (UNIT_TEST)
#define CAN_CYCLIC_STOP(msg_handle)
#else
#define CAN_CYCLIC_STOP(msg_handle)     EXC_CAT_FN((void)IlStopCycle,msg_handle)
#endif

/*
 * PRIVATE function prototypes (documented in source)
 */
#if defined MMC
PRIVATE void exc_update_nm_status(void);
PRIVATE void exc_vehicle_nm(void);
PRIVATE void exc_shutdown_inhibit_update(void);
#endif
PRIVATE void exc_set_interrupt_levels(void);
PRIVATE void exc_update_state(enum exc_bus_e bus);
PRIVATE void exc_cyclic_msg_control(void);
PRIVATE void exc_update_states(void);
PRIVATE void exc_idle_stress_gpage(void);
PRIVATE void exc_cyclic_msg_control_forced(void);
PRIVATE void exc_int_reset(enum exc_bus_e bus);
PRIVATE void exc_charger_nm(void);
PRIVATE void exc_charger_comms_enable(void);
PRIVATE void exc_charger_comms_disable(void);
#if !defined PRODDER
PRIVATE void exc_update_faults(void);
#endif /* PRODDER */

#ifdef UNIT_TEST
extern enum exc_state_e exc_state[];
extern ts_time_t exc_msg_timeouts[];
extern volatile struct exc_int_msg_s exc_int_msg_data[];
extern uint8_t exc_last_nign_status;
extern bool_t exc_int_timeout_fault_monitored[];
extern CAL ts_time_t exc_int_veh_inhibit_time;
extern ts_time_t exc_int_ignition_entered_ts;
extern bool_t exc_int_ignition_entered;
extern bool_t exc_last_nign_status;
extern bool_t exc_charger_bus_fault_reporting_en;
extern CAL bool_t exc_stress_gpage;
#endif /* UNIT_TEST */

#endif      /* EXC_P_H */

