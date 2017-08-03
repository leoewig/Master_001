/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  exc_types.h
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
#if !defined(EXC_TYPES_H)
#define      EXC_TYPES_H
/**
 *
 *
 *
 * @file
 * @ingroup exc
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#if defined (POLYSPACE) ||  defined(HOSTED)
#include "v_cfg.h"
#include "v_def.h"
#include "drv_par.h"

#define CanChannelHandle uint8_t
#endif

#include "fm_types.h"
#include "ts_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/**
 * intermediate macro to make sure macros are expanded before the ##
 * takes two arguments which are concatenated together
 */
#define         EXC_CAT2(a,b)            a ## b

/**
 * intermediate macro to make sure macros are expanded before the ##
 * takes three arguments which are concatenated together
 */
#define         EXC_CAT3(a,b,c)            a ## b ## c

/**
 * intermediate macro to make sure macros are expanded before the ##
 * takes four arguments which are concatenated together
 */
#define         EXC_CAT4(a,b,c,d)          a ## b ## c ## d

/**
 * intermediate macro to make sure macros are expanded before the ##
 * concatenates two arguments and puts bracket around the second one
 */
#define EXC_CAT_FN(body, arg)       body(##arg##)

/*
 * transmission macros
 */

/**
 * Set the value of a signal to be transmitted.  If the signal is part of a
 * signal group, all signals in the group should be written and then the group
 * itself is written with #CAN_TX_GRP
 * @param[in] sig   The name of the signal (as defined in the CANdb file)

 * @param[in] val   The value to which a signal shall be set or a pointer to
 * a buffer, if a signal's length is longer than 4 bytes
 */
#if defined (UNIT_TEST)
#define         CAN_TX_SIG(sig,val) EXC_CAT3(ut_IlPutTx,sig,(val))
#elif defined (POLYSPACE)
#define         CAN_TX_SIG(sig,val) (PS_CAN_TX_SIG(val))
#else
#define         CAN_TX_SIG(sig,val) EXC_CAT3(IlPutTx,sig,(val))
#endif /* UNIT_TEST */

/**
 * Transmit a group of coherent signals whose values have been set
 * with #CAN_TX_SIG
 * @param[in] grp   The name of the signal-group (as defined in the CANdb file)
 */
#if defined (UNIT_TEST)
#define         CAN_TX_GRP(grp)     EXC_CAT3(ut_IlPutTx,grp,())
#elif defined (POLYSPACE)
#define         CAN_TX_GRP(grp)    (PS_CAN_TX_GRP())
#else
#define         CAN_TX_GRP(grp)     EXC_CAT3(IlPutTx,grp,())
#endif /* UNIT_TEST */

/**
 * Get confirmation that a signal has been transmitted.  Once set, flag will
 * remain set until cleared by calling #CAN_TX_SIG_CONFIRMATION_CLR
 * @note a confirmation flag will only be available for a given signal if it
 * has been explicitly configured in GENy.
 *
 * @param[in] sig   The name of the signal (as defined in the CANdb file)
 */
#if defined (UNIT_TEST)
#define         CAN_TX_SIG_CONFIRMATION(sig)    \
                                        EXC_CAT3(ut_IlGet,sig,Confirmation())
#elif defined (POLYSPACE)
#define         CAN_TX_SIG_CONFIRMATION(sig)  (PS_CAN_TX_SIG_CONFIRMATION())
#else
#define         CAN_TX_SIG_CONFIRMATION(sig)   \
                                        EXC_CAT3(IlGet,sig,Confirmation())
#endif /* UNIT_TEST */

/**
 * Clear signal transmission confirmation flag
 *
 * @param[in] sig   The name of the signal (as defined in the CANdb file)
 */
#if defined (UNIT_TEST)
#define         CAN_TX_SIG_CONFIRMATION_CLR(sig) \
                                    EXC_CAT3(ut_IlClr,sig,Confirmation() )
#elif defined (POLYSPACE)
#define         CAN_TX_SIG_CONFIRMATION_CLR(sig)  (PS_CAN_TX_SIG_CONFIRMATION_CLR())
#else
#define         CAN_TX_SIG_CONFIRMATION_CLR(sig)   \
                                    EXC_CAT3(IlClr,sig,Confirmation() )
#endif /* UNIT_TEST */

/*
 * reception macros
 */

/**
 * Read the value of a received signal.  If the signal is part of a signal
 * group, the group must first be read with #CAN_RX_GRP
 *
 * @param[in] sig   The name of the signal (as defined in the CANdb file)
 * @return A value of a signal. Return type depends on signal length.
 */
#if defined (UNIT_TEST)
#define         CAN_RX_SIG(sig)     EXC_CAT3(ut_IlGetRx,sig,())
#elif defined (POLYSPACE)
#define         CAN_RX_SIG(sig)    (PS_CAN_RX_SIG())
#else
#define         CAN_RX_SIG(sig)     EXC_CAT3(IlGetRx,sig,())
#endif /* UNIT_TEST */


/**
 * Read the value of a received signal for signals longer than 4 bytes.
 * If the signal is part of a signal group, the group must first be read
 * with #CAN_RX_GRP
 *
 * @param[in] sig   The name of the signal (as defined in the CANdb file)
 * @param[out] out  A pointer to a buffer to be filled with signal data.
 */
#if defined (UNIT_TEST)
#define         CAN_RX_SIG_ARRAY(sig, out)     EXC_CAT3(ut_IlGetRx,sig,(out))
#elif defined (POLYSPACE)
#define         CAN_RX_SIG_ARRAY(sig, out)    (PS_CAN_RX_SIG())
#else
#define         CAN_RX_SIG_ARRAY(sig, out)     EXC_CAT3(IlGetRx,sig,(out))
#endif /* UNIT_TEST */

/**
 * Read a signal-group from a received message.  This must be executed prior
 * to reading the individual signals with #CAN_RX_SIG
 *
 * @param[in] grp   The name of the signal-group (as defined in the CANdb file)
 */
#if defined (UNIT_TEST)
#define         CAN_RX_GRP(grp)     EXC_CAT3(ut_IlGetRx,grp,())
#elif defined (POLYSPACE)
#define         CAN_RX_GRP(grp)    (PS_CAN_RX_GRP())
#else
#define         CAN_RX_GRP(grp)     EXC_CAT3(IlGetRx,grp,())
#endif /* UNIT_TEST */

/**
 * Get an indication that a signal has been received.  Once set, the flag will
 * remain set until cleared by calling #CAN_RX_SIG_INDICATION_CLR.
 * @note an indication flag will only be available for a given signal if it
 * has been explicitly configured in GENy.
 *
 * @param[in] sig   The name of the signal (as defined in the CANdb file)
 */
#if defined (UNIT_TEST)
#define         CAN_RX_SIG_INDICATION(sig)      EXC_CAT3(ut_IlGet,sig,Indication())
#elif defined (POLYSPACE)
#define         CAN_RX_SIG_INDICATION(sig)     (PS_CAN_RX_SIG_INDICATION())
#else
#define         CAN_RX_SIG_INDICATION(sig)      EXC_CAT3(IlGet,sig,Indication())
#endif /* UNIT_TEST */

/**
 * Clear the indication that a signal has been received.
 *
 * @param[in] sig   The name of the signal (as defined in the CANdb file)
 */
#if defined (UNIT_TEST)
#define         CAN_RX_SIG_INDICATION_CLR(sig)  EXC_CAT3(ut_IlClr,sig,Indication())
#elif defined (POLYSPACE)
#define         CAN_RX_SIG_INDICATION_CLR(sig) (PS_CAN_RX_SIG_INDICATION_CLR())
#else
#define         CAN_RX_SIG_INDICATION_CLR(sig)  EXC_CAT3(IlClr,sig,Indication())
#endif /* UNIT_TEST */

/**
 * Get symbolic name's value
 *
 * @param[in] sig   The name of the signal (as defined in the CANdb file)
 * @param[in] vname The symbolic name (as defined in signal's value table in the CANdb file)
 */

#define         CAN_VALUE_NAME(sig,vname)  UCONST(EXC_CAT4(k,sig,_,vname))

/**
 * Read status of receiving signal. Signal must be configured to have such a status associated
 * using *_exc_integrity.def file first.
 *
 * @param[in] sig   The name of the signal (as defined in the CANdb file)
 * @return  Exc_int_invalid   if signal is not valid
 * @return  Exc_int_valid     if signal is valid, i.e. that it passed checksum,
 *                                  counter and timeout checking.
 */
#if defined (POLYSPACE)
#define         CAN_RX_SIG_STATUS(sig)    (PS_CAN_RX_SIG_STATUS())
#else
#define         CAN_RX_SIG_STATUS(sig)     *exc_int_signal_statuses[EXC_CAT2(Exc_int_rx_sig_,sig)]
#endif /* POLYSPACE */

#if defined HOSTED
/**
 * Set status of receiving signal. Used for unit testing only
 */
#define CAN_RX_SET_SIG_STATUS(sig, status)  *exc_int_signal_statuses[EXC_CAT2(Exc_int_rx_sig_,sig)] = status
#endif

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/**
 * Enumeration of CANbedded enabled buses
 */
#if defined POLYSPACE || defined HOSTED
#define CCL_vehicle_bus 0u
#define CCL_charger_bus 2u
#define CCL_isa_bus 1u
#endif

/**
 * Enumeration of CANbedded driven buses
 */
enum exc_bus_e
{
    Exc_vehicle_bus = CCL_vehicle_bus,  /**< vehicle bus */
    Exc_isa_bus = CCL_isa_bus,          /**< isa bus */
    Exc_charger_bus = CCL_charger_bus,  /**< charger bus */
    Exc_max_buses                       /**< number of buses */
};

/**
 * Enumeration of network management modes
 */
enum exc_nm_status_e
{
    Exc_nm_starting_up,         /**< Indicates that the network management is starting up.
                                     This is set by the exc feature during after start-up */
    Exc_nm_limp_home,           /**< Indicates that the NM is in the mode LimpHome. LimpHome
                                     is entered, if the NM can’t send or receive NM messages */
    Exc_nm_ring_established,    /**< Indicates that the current network is stable. Stable means,
                                     that the configuration has not changed during the last
                                     ring cycle. */
    Exc_nm_shutdown_or_sleep,   /**< Indicates whether the bus is asleep or not or transceiver
                                     has been disabled */
    Exc_nm_unknown              /**< Indicates that the NM status couldn't been retrieved
                                     or it returned unknown status */
};

/**
 * Enumeration of available cyclic message control configurations.
 */
enum exc_cyclic_control_e
{
    Exc_cyclic_control_not_set,     /**< cyclic message transmission hasn't been configured yet */
    Exc_cyclic_control_tx_disabled, /**< cyclic message transmission has been disabled */
    Exc_cyclic_control_tx_enabled    /**< cyclic message transmission has been enabled */
};

/**
 * Structure holding cyclic message transmission configuration.
 */
struct exc_cyclic_control_s
{
    uint16_t tx_handle;     /**< handle to transmit message, which transmission is to be
                                         enabled or disabled */

    bool_t enable_flag;     /**< pointer to a flag controlling message transmission */

    enum exc_cyclic_control_e last_cfg; /**< last message transmission configuration */
};

/**
 * Enumeration of possible CANbedded bus states
 */
enum exc_state_e
{
    Exc_error_active,   /**< Active operational state */
    Exc_warning,        /**< At least one error was detected during
                            transmission or reception */
    Exc_error_passive,  /**< More than 127 errors were detected */
    Exc_bus_off         /**< More than 255 errors were detected */
};

/* Extraction of public types related to additional data integrity measures */

/* create enumeration of all receiving signals enabled for additional data integrity
 * measures */
/* following macros are used in exc_integrity_file.def file. */
#define EXC_INTEGRITY_TX(msg)
#define EXC_INTEGRITY_RX(msg, timeout, flt_lvl, flt, chk_lvl, bus)
#define EXC_INTEGRITY_RX_SIG(msg, sig)      Exc_int_rx_sig_ ## sig,

/**
 * Enumeration of all signals enabled to be checked using additional data integrity measures
 * and to have a status variable associated.
 */
enum exc_int_rx_signals_e
{
#include "exc_integrity.def"
    Exc_int_rx_sig_max
};

#undef EXC_INTEGRITY_RX_SIG
#define EXC_INTEGRITY_RX_SIG(msg, sig)
#undef EXC_INTEGRITY_RX
/* Get the list of receiving messages from exc_integrity. def using EXC_INTEGRITY_RX macro */
#define EXC_INTEGRITY_RX(msg, timeout, flt_lvl, flt, chk_lvl, bus)    Exc_int_rx_msg_ ## msg,

/**
 * Enumeration of all receiving messages enabled for additional data integrity checking
 */
enum exc_int_rx_messages_e
{
#include "exc_integrity.def"
    Exc_int_rx_msg_max
};

#undef  EXC_INTEGRITY_RX_SIG
#undef  EXC_INTEGRITY_RX
#undef  EXC_INTEGRITY_TX


/**
 * Enumeration of available data integrity statuses associated to a message
 * and / or its signals
 */
enum exc_int_status_e
{
    Exc_int_invalid,      /**< Message and all belonging signals have failed counter
                                    and checksum checking */
    Exc_int_timeouted,    /**< Message and all belonging signals have timeouted */
    Exc_int_valid         /**< Message and all belonging signals are valid */
};

/**
 * Enumeration of available data integrity faults reporting options.
 */
enum exc_int_report_level_e
{
    Exc_int_sum_cnt_faults,     /**< report checksum and counter faults only */
    Exc_int_timeout_faults,     /**< report timeout faults only */
    Exc_int_all_faults,         /**< report timeout, checksum and counter faults */
    Exc_int_no_faults           /**< report no faults */
};

/**
 * Enumeration of available data integrity check options (levels)
 */
enum exc_int_check_level_e
{
    Exc_int_check_sum_cnt,      /**< Check counter and checksum only */
    Exc_int_check_timeout,      /**< Check timeout only */
    Exc_int_check_all,          /**< Check counter, checksum and timeout */
    Exc_int_check_nothing       /**< Check nothing, message and / or signals
                                           are always valid */
};

/**
 * Enumeration of possible messge timeouts
 */
enum exc_timeout_e
{
    Exc_timeout_short,      /**< For messages with period < 100ms */
    Exc_timeout_medium,     /**< For messages with period 100ms - 1000ms */
    Exc_timeout_long,       /**< For messages with period > 1000ms or multiplexed */
    Exc_max_timeouts
};

/**
 * Structure holding helper data used for data integrity checking
 * of each receiving message
 */
struct exc_int_msg_s
{
    uint8_t counter;                    /**< Last valid received counter value (passing checksum
                                             checking only) */
    ts_time_t last_valid_timestamp;     /**< Timestamp of last valid message reception
                                             (passing checksum and counter checking) */
    enum exc_timeout_e timeout;           /**< Timeout period of this message */
    enum exc_int_status_e status;       /**< Actual status of this message and all belonging
                                             signals */
    enum exc_int_report_level_e report_level; /**< Fault reporting level */
    Fm_fault_list_e fault;              /**< Fault name to be reported, when report_level is not
                                             #Exc_int_no_faults */
    enum exc_int_check_level_e check_level; /**< Integrity check level */
    enum exc_bus_e bus;                 /**< A bus to which message belongs */
};


/* Extraction of public types related to cyclic message transmission */

/**
 * Macro for message name extraction from exc_cyclic.def file
 */
#define EXC_CYCLIC_MSG_CONTROL(msg)   EXC_CAT2(Exc_msg_tx_handle_,msg),
/**
 * Enumeration of messages, which transmission may be enabled or disabled in run-time.
 * Message names are taken from exc_cyclic.def configuration file.
 */
/**
 * @lint -esym(769,exc_cyclic_control_names_e::Exc_msg_tx_handle_*)
 * All items up to exc_msg_tx_handle_max are not referenced. This serves
 * just for getting the number of records in exc_cyclic.def file.
 */
/*lint -esym(769,exc_cyclic_control_names_e::Exc_msg_tx_handle_*) */
enum exc_cyclic_control_names_e
{
	#include "exc_cyclic.def"
    Exc_msg_tx_handle_max   /* a last item providing number of items in total */
};
#undef EXC_CYCLIC_MSG_CONTROL

#endif      /* EXC_TYPES_H */
