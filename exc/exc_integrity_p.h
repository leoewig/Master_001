/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      exc_integrity_p.h
 *
 * description:    Private header for External CAN data integrity module
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li    Private header for External CAN data integrity module
 *
 *****************************************************************************/
#if !defined(EXC_INTEGRITY_P_H_)
#define EXC_INTEGRITY_P_H_

/**
 *
 * @file
 * @ingroup exc
 *
 * Private header for External CAN data integrity module
 *
 * $Id: exc_integrity_p.h 9774 2013-08-01 08:31:44Z tg10 $
 */

/* Copyright (c) Linkedriving Tech 2015 - All rights resevered */


/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "ts.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/*
 * Following macros reflect message and signal configuration made in GENy
 * tool, hence it needs to be updated accordingly whenever
 * it changes.
 */
/** Byte position of counter byte counted from the end of the message */
#define EXC_COUNTER_POS_FROM_END        (1u)
/** Byte position of checksum byte counted from the end of the message */
#define EXC_CHECKSUM_POS_FROM_END       (0u)
/** Bit position of counter signal's MSB bit according to @ref exc_adi */
#define EXC_COUNTER_STARTBIT            (7u)
/** Bit size of counter signal according to @ref exc_adi */
#define EXC_COUNTER_BITSIZE             (4u)
/** Default counter value according to @ref exc_adi */
#define EXC_INITIAL_COUNTER_VALUE       (0x0fu)
/** Macro used for counter signal name extraction */
#define EXC_COUNTER_SIGNAL_NAME(msg)    EXC_CAT3(N,msg,Counter)
/** Macro used for checksum signal name extraction */
#define EXC_CHECKSUM_SIGNAL_NAME(msg)   EXC_CAT3(N,msg,Checksum)
/** Macro used for pre-transmit function name from message name */
#define EXC_PRETRANSMIT_FN_NAME(msg)    EXC_CAT2(ExcPreTx_,msg)
/** Macro used for pre-copy function name extraction */
#define EXC_PRECOPY_FN_NAME(msg)        EXC_CAT2(ExcPreRx_,msg)

/* Internal macros not reflecting database file or deduced from above
 * macros */
/** Macro used for counter variable extraction for each transmission message */
#define EXC_TX_COUNTER_VAR_NAME(msg)    EXC_CAT3(exc_tx_,msg,_counter)
/** Macro used for integrity helper data for each in bound message */
#define EXC_MSG_DATA_STRUCT(msg)   exc_int_msg_data[EXC_CAT2(Exc_int_rx_msg_,msg)]
/** Mask used for wrapping counter variable */
#define EXC_COUNTER_MASK                (0xffu >> (8u - EXC_COUNTER_BITSIZE))
/** Macro used for extracting counter from CAN frame byte it lives in */
#define EXC_COUNTER_SHIFT ((EXC_COUNTER_STARTBIT + 1u) - EXC_COUNTER_BITSIZE)
/** Invalid counter value used for first counter comparison in pre-copy
 * functions */
#define EXC_INVALID_COUNTER_VALUE         (0xffu)

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
#if defined HOSTED
extern bool_t exc_int_check_disabled;
extern volatile struct exc_int_msg_s exc_int_msg_data[];
#endif

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
PRIVATE uint8_t exc_increase_counter(uint8_t value);
PRIVATE uint8_t exc_calculate_checksum(uint8_t CONST * src, uint8_t len);
PRIVATE enum exc_int_status_e exc_check_rx_message(
    volatile struct exc_int_msg_s * int_data, uint8_t CONST * msg_data, uint8_t CONST dlc);
#if !defined PRODDER
PRIVATE void exc_int_update_fault_handling(void);
#endif

#endif /* EXC_INTEGRITY_P_H_ */
