/**
 *
 * Timer Support interface
 *
 * @file
 * @ingroup ts
 * $Id: ts_cust.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2015 - All rights reserved */

#if !defined(TS_CUST_H)
#define      TS_CUST_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "ts_types.h"

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 *  Obtain the most recent millisecond timestamp
 *  @returns ts_time_t giving  the timestamp in milliseconds */
FCODE ts_time_t ts_get_ms(void);

/**
 * Check for an elapsed timer
 * @retval TRUE if newval - oldval >= interval_ms
 * @retval FALSE otherwise */
FCODE bool_t ts_is_elapsed_ms(ts_time_t newval, ts_time_t oldval,
	ts_time_t interval_ms);

/**
 * Obtain ticks counter value. The scaling is as per pit.h.
 * @note  If it is not called for a whole wrap-around period, the wrap is
 * missed. The wrap around period is 327.68 milliseconds.
 */
FCODE ts_ticks_t ts_get_ticks(void);

#endif      /* TS_CUST_H */

