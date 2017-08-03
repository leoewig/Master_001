/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      exc_integrity.c
 *
 * description:    External CAN data integrity module
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li          External CAN data integrity module
 *
 *****************************************************************************/

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#include "exc.h"
#include "exc_integrity_p.h"
#include "exc_p.h"
#include "ts.h"
//#include "cgi.h"
#include "ms.h"
#if !defined PRODDER
#include "fm.h"
//#include "vmi.h"
#endif

/******************************************************************************
 * CALIBRATABLE PARAMETERS
 *****************************************************************************/
#define MEMORY_LOCATION CALIBRATION_MEMORY_S12
#include "memory_layout.h"
/*lint -esym(528, exc_int_check_disabled) */
PRIVATE CAL bool_t exc_int_check_disabled = FALSE;
/**<
 * @a2l{conversion,bool_t}
 * @a2l{longidentifier,Flag to enable or disable additional data integrity checking
 * on external CAN interface}
 */

PRIVATE CAL ts_time_t exc_int_veh_inhibit_time = (ts_time_t) 100;
/**<
 * @a2l{conversion,ts_time_t}
 * @a2l{longidentifier,Time after entering ignition state 5 for which time-out checking
 * of CAN messages on the vehicle CAN bus is inhibited}
 * @a2l{annotation,REQ_BMS_VIO_IM_013}
 */

PRIVATE CAL supply_v_t exc_int_veh_min_supply_voltage = SUPPLY_V(10.5);
/**<
 * @a2l{conversion,supply_v_t}
 * @a2l{LongIdentifier,The minimal logic supply voltage for vehicle bus fault monitoring allowed}
 * @a2l{annotation,REQ_BMS_VIO_IM_013}
 */

PRIVATE CAL supply_v_t exc_int_veh_max_supply_voltage = SUPPLY_V(15.5);
/**<
 * @a2l{conversion,supply_v_t}
 * @a2l{LongIdentifier,The maximal logic supply voltage for vehicle bus fault monitoring allowed}
 * @a2l{annotation,REQ_BMS_VIO_IM_013}
 */

#define MEMORY_LOCATION GENERAL_MEMORY
#include "memory_layout.h"

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

PRIVATE ts_time_t exc_msg_timeouts[Exc_max_timeouts] =
{
    TIME( 1000), /*  1s */
    TIME( 5000), /*  5s */
    TIME(10000)  /* 10s */
};

#define EXC_INTEGRITY_TX(msg)
#define EXC_INTEGRITY_RX_SIG(msg, sig)
/* initialize exc integrity data structures for each message from enum exc_int_rx_messages_e list */
#define EXC_INTEGRITY_RX(msg, timeout, flt_lvl, flt, chk_lvl, bus)    \
    {EXC_INVALID_COUNTER_VALUE, 0, timeout, Exc_int_invalid, flt_lvl, flt, chk_lvl, bus},

/**
 * Data integrity check structure containing helper data needed for data and timeout checking.
 * Dimension of the array is deduced from number of messages configured in exc_integrity
 * file and increased by one to deal with the situation, when there're no messages
 * actually configured to perform integrity checked.
 */
PRIVATE volatile struct exc_int_msg_s exc_int_msg_data
    [(uint16_t) Exc_int_rx_msg_max + 1u] =
{
#include "exc_integrity.def"
    /* dummy record*/
    {EXC_INVALID_COUNTER_VALUE, (ts_time_t) 0, Exc_timeout_long, Exc_int_invalid,
        Exc_int_no_faults, Fm_internal_error_unexpected, Exc_int_check_nothing, Exc_vehicle_bus}
};

/**
 * Flag array signaling current timeout fault monitoring state for each bus
 */
PRIVATE bool_t exc_int_timeout_fault_monitored[Exc_max_buses] =
{
    FALSE,
    FALSE,
    FALSE
};

/**
 * Flag array signaling presence of checksum fault of any message for each bus
 */
PRIVATE bool_t exc_int_checksum_fault_present[Exc_max_buses] =
{
    FALSE,
    FALSE,
    FALSE
};

/**
 * Flag array signaling presence of counter fault of any message for each bus
 */
PRIVATE bool_t exc_int_cnt_fault_present[Exc_max_buses] =
{
    FALSE,
    FALSE,
    FALSE
};

/* configuration dependent */

/**
 * Fault codes to be reported when invalid counter or checksum fault occurs
 * for the messages configured to report either all faults or checksum or counter
 * failures
 */
PRIVATE Fm_fault_list_e exc_int_failure_fm_code[Exc_max_buses] =
{
//    Fm_veh_comms_integrity_failure,     /* vehicle bus */
    Fm_internal_error_unexpected,       /* ISA bus */
    Fm_charger_communication            /* charger bus*/
};

/**
 * Flag signaling entrance to Ignition state
 */
PRIVATE bool_t exc_int_ignition_entered = FALSE;

/**
 * Timestamp of entering Ignition state
 */
PRIVATE ts_time_t exc_int_ignition_entered_ts;

PRIVATE uint8_t exc_last_nign_status;

/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/

/* PRODDER specific flags modifying functionality */
#if defined PRODDER
/** Flag to enable / disable faulty counter value insertion. */
bool_t PROD_exc_int_invalid_counter_en = FALSE;
/** Flag to enable / disable faulty checksum value insertion. */
bool_t PROD_exc_int_invalid_checksum_en = FALSE;
#endif

/* create list of constant pointers for each configured signal pointing
 * to the message status the signal belongs to */
#undef EXC_INTEGRITY_RX
#define EXC_INTEGRITY_RX(msg, timeout, flt_lvl, flt, chk_lvl, bus)
#undef EXC_INTEGRITY_RX_SIG
#define EXC_INTEGRITY_RX_SIG(msg, sig)      \
    (CONST enum exc_int_status_e *) &EXC_MSG_DATA_STRUCT(msg).status,

enum exc_int_status_e * CONST exc_int_signal_statuses
    [(uint16_t)Exc_int_rx_sig_max + 1u] =
{
#include "exc_integrity.def"
    /* dummy record */
    NULL
};

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/
/*
 * Define pre-transmit functions for all messages listed in exc_integrity.def
 * using EXC_INTEGRITY_TX macro. Declarations are located in can_par.h
 * (generated by CANbedded from GENy configuration).
 */
#undef EXC_INTEGRITY_RX_SIG
#define EXC_INTEGRITY_RX_SIG(msg, sig)
#undef  EXC_INTEGRITY_TX
#define EXC_INTEGRITY_TX(msg)                                                   \
/* Last counter value holder for each  pre-transmit function */                 \
PRIVATE uint8_t EXC_TX_COUNTER_VAR_NAME(msg) = EXC_INITIAL_COUNTER_VALUE;       \
vuint8 EXC_PRETRANSMIT_FN_NAME(msg)(CanTxInfoStruct txStruct)                   \
{                                                                               \
    /* write message's counter signal with local counter value */               \
    CAN_TX_SIG(EXC_COUNTER_SIGNAL_NAME(msg), EXC_TX_COUNTER_VAR_NAME(msg));     \
    /* increase local counter variable */                                       \
    EXC_TX_COUNTER_VAR_NAME(msg) =                                              \
        exc_increase_counter(EXC_TX_COUNTER_VAR_NAME(msg));                     \
    /* update checksum. CanGetTxDataPtr() is used for fetching the pointer to   \
     * the user data which are about to be sent. Although it is not documented  \
     * anywhere in CANbedded user documentation, it's only option found, how to \
     * perform such a task and also found safe after inspecting CANbedded's     \
     * CanCopyDataAndStartTransmission(), which is the caller of this           \
     * pre-transmit function and which is using it in the same context          \
     */                                                                         \
    CAN_TX_SIG(EXC_CHECKSUM_SIGNAL_NAME(msg),                                   \
        exc_calculate_checksum(CanGetTxDataPtr(txStruct.Handle),                \
            CanGetTxDlc(txStruct.Handle) - 1u));                                \
    /* Inform the caller, that it needs to copy out the data from internal      \
     * buffers (from signal value holders into the transmit buffer)             \
     * prior transmission */                                                    \
    return kCanCopyData;                                                        \
}

/*
 * Include def file now, so it'll expand to pre-transmit function definitions
 * using the above template macro
 */
/**
 * @lint -esym(960,8.5) Disable reporting of "no object/function definitions
 * in header files"
 */
/*lint -esym(960,8.5) */
#include "exc_integrity.def"
/**
 * @lint +esym(960,8.5) Enable reporting of "no object/function definitions
 * in header files"
 */
/*lint +esym(960,8.5) */

/*
 * Define pre-copy functions for all messages configured in
 * exc_integrity.def
 */
#undef  EXC_INTEGRITY_TX
#define EXC_INTEGRITY_TX(msg)
#undef  EXC_INTEGRITY_RX
#define EXC_INTEGRITY_RX(msg, timeout, flt_lvl, flt, chk_lvl, bus)                             \
/*lint -esym(818, rxStruct)*/                                                                  \
vuint8 EXC_PRECOPY_FN_NAME(msg)(CanRxInfoStructPtr rxStruct)                                   \
{                                                                                              \
    vuint8 res = kCanCopyData;                                                                 \
    if (exc_int_check_disabled == FALSE)                                                       \
    {                                                                                          \
        if(exc_check_rx_message(&EXC_MSG_DATA_STRUCT(msg),                                     \
            (uint8_t CONST *) rxStruct->pChipData,                                             \
            CanRxActualDLC(rxStruct)) != Exc_int_valid)                                        \
        {                                                                                      \
            res = kCanNoCopyData;                                                              \
        }                                                                                      \
    }                                                                                          \
    return res;                                                                                \
}

/*
 * Include def file now, so it'll expand to pre-copy function definition
 * using the above template macro
 */
/**
 * @lint -esym(960,8.5) Disable reporting of "no object/function definitions
 * in header files"
 */
/*lint -esym(960,8.5) */
#include "exc_integrity.def"
/**
 * @lint +esym(960,8.5) Enable reporting of "no object/function definitions
 * in header files"
 */
/*lint +esym(960,8.5) */

#undef  EXC_INTEGRITY_RX
#undef  EXC_INTEGRITY_RX_SIG
#undef  EXC_INTEGRITY_TX

/**
 * Update exc integrity module
 * - update timeouts - test whether a timeout time has expired since the last valid message
 *      has been received and eventually flag this message (and all its signals)
 *      as #Exc_int_timeouted

 * @note last_valid_timestamp and status members of #exc_int_msg_data of each message
 * are being manipulated in precopy functions, which are called from the interrupt context,
 * so we need to disable interrupts while manipulating it here to ensure consistency
 */
void exc_int_update(void)
{
    uint8_t ccr;
    uint8_t msg;
    bool_t msg_timeouted;
    bool_t timeout_fault_present;
    enum exc_bus_e bus;

#if !defined PRODDER
    /* update integrity checking criteria for each bus */
    exc_int_update_fault_handling();
#endif

    /* check timeout condition of all configured receiving messages */
    for (msg = 0u; msg < (uint8_t) Exc_int_rx_msg_max; msg++)
    {
        /* reset flags before checking each message status */
        msg_timeouted = FALSE;
        /* critical section start */
        SAVE_INTERRUPTS(ccr);
        if (ts_is_elapsed_ms(ts_get_ms(), exc_int_msg_data[msg].last_valid_timestamp,
            exc_msg_timeouts[exc_int_msg_data[msg].timeout]))
        {
            if((exc_int_msg_data[msg].check_level == Exc_int_check_all) ||
               (exc_int_msg_data[msg].check_level == Exc_int_check_timeout))
            {
                exc_int_msg_data[msg].status = Exc_int_timeouted;
                msg_timeouted = TRUE;
            }
        }
        /* critical section end */
        RESTORE_INTERRUPTS(ccr);

#if !defined PRODDER
        /* for messages enabled for timeout fault reporting, deduce whether
         * the fault has occurred */
        if((exc_int_msg_data[msg].report_level == Exc_int_all_faults)
            || (exc_int_msg_data[msg].report_level == Exc_int_timeout_faults))
        {
            if(msg_timeouted)
            {
                timeout_fault_present = TRUE;
            }
            else
            {
                timeout_fault_present = FALSE;
            }

            /*note that exc_int_msg_data[msg].fault must be set to valid value
             * when Exc_int_all_faults or Exc_int_timeout_faults configured! */
            (void) fm_update_fault_filter(
                exc_int_msg_data[msg].fault,
                Fm_general_failure_information,
                timeout_fault_present,
                exc_int_timeout_fault_monitored[exc_int_msg_data[msg].bus]);
        }
#endif
        /* reset statuses to invalid when we've lost the network */
        if(exc_is_bus_unavailable(exc_int_msg_data[msg].bus)
            && (exc_int_msg_data[msg].status == Exc_int_valid))
        {
            exc_int_msg_data[msg].status = Exc_int_invalid;
        }

        if((exc_int_check_disabled == TRUE)
            || (exc_int_msg_data[msg].check_level == Exc_int_check_nothing))
        {
            exc_int_msg_data[msg].status = Exc_int_valid;
        }
    }
    /* capture all checksum and counter failures since last call to this update
     * function. Note that these are set in receive ISR only for
     * messages configured for Exc_int_all_faults or Exc_int_sum_cnt_faults globally
     * per each bus, i.e. leading to just single fault (and counter or checksum subtype)
     * per each bus. Since manipulated in ISR context, ensure consistency
     * by reading them inside critical section. */
#if !defined PRODDER
    for(bus = (enum exc_bus_e) 0; bus < Exc_max_buses; bus++)
    {
        SAVE_INTERRUPTS(ccr);
        if(exc_int_checksum_fault_present[bus])
        {
    //        (void) fm_update_fault_filter(
    //            exc_int_failure_fm_code[bus],
    //            Fm_checksums,
    //            TRUE,
    //            (bool_t) (exc_int_timeout_fault_monitored[bus] == FALSE));
            exc_int_checksum_fault_present[bus] = FALSE;
        }
        if(exc_int_cnt_fault_present[bus])
        {
//            (void) fm_update_fault_filter(
//                exc_int_failure_fm_code[bus],
 //               Fm_counters,
  //              TRUE,
   //             (bool_t) (exc_int_timeout_fault_monitored[bus] == FALSE));
            exc_int_cnt_fault_present[bus] = FALSE;
        }
        RESTORE_INTERRUPTS(ccr);
    }
#endif
}

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/**
 * Increase given variable by one, apply mask defined
 * by #EXC_COUNTER_MASK and return it.
 *
 * @param   value counter variable to be increased and masked
 * @return  increased and masked counter variable
 */
PRIVATE uint8_t exc_increase_counter(uint8_t value)
{
#if defined PRODDER
    if(PROD_exc_int_invalid_counter_en == TRUE)
    {
        /* don't increase the counter */
        return (value & EXC_COUNTER_MASK);
    }
    else
    {
        return ((value + 1u) & EXC_COUNTER_MASK);
    }
#else
    return ((value + 1u) & EXC_COUNTER_MASK);
#endif
}

/**
 * Calculate checksum.
 * Algorithm used is inclusive or of all the data bytes in the frame,
 * but the last one.
 *
 * @param src       pointer to the data from which a checksum shall
 *                  be calculated
 * @param len       length of data to be calculated
 * @return          calculated checksum
 */
PRIVATE uint8_t exc_calculate_checksum(uint8_t CONST * src, uint8_t len)
{
    uint8_t i;
    uint8_t checksum = 0u;
    for (i = 0u; i < len; i++)
    {
        checksum ^= src[i];
    }
#if defined PRODDER
    if(PROD_exc_int_invalid_checksum_en == TRUE)
    {
        return 0xFFu;
    }
    else
    {
        return checksum;
    }
#else
    return checksum;
#endif
}

/**
 * Check message validity. The message is considered to be valid if
 * checksum found in the message matches the one calculated by exc_calculate_checksum()
 * and if counter value found in the message is different of the last received one.
 * It is suggested, that the last byte of the message holds checksum and high
 * nibble of the next to last byte holds counter value.
 *
 * @param int_data  pointer to data integrity measures data structure associated
 *                  to the message to be checked
 * @param msg_data  pointer to content of the message
 * @param dlc       length of the message to be checked
 * @return          Exc_int_invalid if message integrity found
 *                  invalid (only for messages configured to be checked for all or
 *                  just checksums a counters)
 * @return          Exc_int_valid if message integrity found valid, i.e.
 *                  if both integrity check criteria has been met (for messages
 *                  configured to be checked for all or just checksums and counters) or
 *                  if it was just received (for messages configured to be checked for
 *                  timeout only)
 */
PRIVATE enum exc_int_status_e exc_check_rx_message(
    volatile struct exc_int_msg_s * int_data, uint8_t CONST * msg_data, uint8_t CONST dlc)
{
    enum exc_int_status_e res = Exc_int_invalid;
    uint8_t counter_frame = (uint8_t)(
        (msg_data[dlc - (EXC_COUNTER_POS_FROM_END + 1u)] >> EXC_COUNTER_SHIFT) & EXC_COUNTER_MASK);
    uint8_t checksum_frame = msg_data[dlc - (EXC_CHECKSUM_POS_FROM_END + 1u)];

    /* Perform checksum and counter check for all messages configured as one of following:
     * -Exc_int_check_sum_cnt
     * -Exc_int_check_all
     */
    if((int_data->check_level == Exc_int_check_all) ||
       (int_data->check_level == Exc_int_check_sum_cnt))
    {
        /* compare calculated checksum with the one found in the frame */
        if (exc_calculate_checksum(msg_data, dlc - 1u) == checksum_frame)
        {
            /* Compare expected counter value with the one found in the frame.
             * If valid counter hasn't been received so far, message is deemed
             * to fail the check.
             */
            if(int_data->counter != EXC_INVALID_COUNTER_VALUE)
            {
                if (int_data->counter != counter_frame)
                {
                    /* message has passed the integrity check */
                    res = Exc_int_valid;
                    /* reload the timeout timer */
                    int_data->last_valid_timestamp = ts_get_ms();
                }
                else
                {
                    /* flag failure if configured to do so */
                    if((int_data->report_level == Exc_int_all_faults)
                        || (int_data->report_level == Exc_int_sum_cnt_faults))
                    {
                        exc_int_cnt_fault_present[int_data->bus] = TRUE;
                    }
                }
            }
            /* store last received counter value, if at least checksum test
             * has passed */
            int_data->counter = counter_frame;
        }
        else
        {
            /* flag failure if configured to do so */
            if((int_data->report_level == Exc_int_all_faults)
                || (int_data->report_level == Exc_int_sum_cnt_faults))
            {
                exc_int_checksum_fault_present[int_data->bus] = TRUE;
            }
        }
    }
    /* record reception timestamp for messages configured as to check
     * timeout only. For those, mere reception in time makes check passed */
    if(int_data->check_level == Exc_int_check_timeout)
    {
        int_data->last_valid_timestamp = ts_get_ms();
        res = Exc_int_valid;
    }
    /* update message status */
    int_data->status = res;
    return res;
}

/**
 * Reset the data structure of receiving messages belonging to given bus.
 * Intended to be used when certain bus has entered timeout monitoring state
 * in order to allow message come in before its designed timeout period has passed
 */
PRIVATE void exc_int_reset(enum exc_bus_e bus)
{
    uint8_t msg;
    uint8_t ccr;
    ts_time_t ts = ts_get_ms();
    for (msg = 0u; msg < (uint8_t) Exc_int_rx_msg_max; msg++)
    {
        /* critical section start */
        SAVE_INTERRUPTS(ccr);
        /* reset the data structure of each message belonging to bus */
        if(bus == exc_int_msg_data[msg].bus)
        {
            /* keep valid statuses */
            if(exc_int_msg_data[msg].status == Exc_int_timeouted)
            {
                exc_int_msg_data[msg].status = Exc_int_invalid;
            }
            exc_int_msg_data[msg].last_valid_timestamp = ts;
        }
        /* critical section end */
        RESTORE_INTERRUPTS(ccr);
    }
}

#if !defined PRODDER
/**
 * Detect integrity checking enabled / disable transition for each bus,
 * perform reset of integrity checking data structure and fault monitoring condition flag
 */
PRIVATE void exc_int_update_fault_handling(void)
{

 
}
#endif
