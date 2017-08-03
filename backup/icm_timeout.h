/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    icm_timeout.h
 *
 * description: 
 *Header file for ICM command timeout tracking
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
#if !defined(ICM_TIMEOUT_H)
#define      ICM_TIMEOUT_H
/**
 *
 * @file
 * @ingroup icm_timeout
 * Header file for ICM command timeout tracking
 *
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 *
 * <h2> Overview </h2>
 * This module provides a uniform interface for accessing the timeout
 * information for all outgoing messages, in order to ensure consistent
 * detection of timeout errors in communication.
 *
 * Usage
 * =========
 * - Use icm_set_cmd_timeout() to start a timeout for an outgoing command
 * - Use icm_resp_state() to obtain the current state of the timeout
 * - Use icm_clear_resp_pending() to indicate a command has been completed
 * successfully.
 *  - Use icm_clear_timeout() to set the state back to IcmRespIdle from timed
 * out.
 */

 /**
 * Timeout managment for issuing network commands.
 * @defgroup icm_timeout Internal Comms command timeout tracking
 * @ingroup icm
 */

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/** A value that can be used as an argument to icm_set_cmd_timeout()
 * for the time value if no response is expected */
#define ICM_NO_TIMEOUT_VAL (ts_time_t)0

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

void    icm_check_timeout(void);

void    icm_clear_resp_pending(void);

void    icm_set_cmd_timeout(ts_time_t to_val);

void    icm_clear_timeout(void);

enum IcmRespState_e  icm_resp_state(void);

#endif      /* ICM_TIMEOUT_H */

