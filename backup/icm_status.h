/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   icm_status.h
 *
 * description:  ICM status processing header
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
#if !defined(ICM_STATUS_H)
#define      ICM_STATUS_H
/**
 *
 * ICM status processing header
 *
 * Additional file documentation if necessary
 *
 * @file
 * @ingroup icm_status
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 *
 * <h2> ICM status processing overview </h2>
 *
 * <h2> Approach used </h2>
 * This module provides facilities to the rest of ICM for manipulating status 
 * information.
 * 
 * icm_process_vtbm_status() is called after a status frame has been decoded.
 * its contents are inspected and stored.
 * Once the command resulting in status transmission is deemed to have finished
 * (either by receiving all responses or timing out) icm_status_completion()
 * is called to update information relating to collective status.
 * 
 *
 * <h2> How to use </h2>
 * icm_status_init() should be called at start-up
 * icm_age_status() should be called at the start of icm foreground processing
 * to mark the data from the previous invocation as 'old'.
 * icm_clear_status_ok() is used to clear the information about whether the 
 * connection is is a good state e.g. when we know a VTBM has been power
 * cycled.
 * icm_clear_status_rx() is used to clear the information that a complete set 
 * of status messages has been received in response to a command. This should 
 * be called before sending a command which expects a full set of status 
 * responses (config block or a network control messages).
 *
 * @defgroup icm_status ICM status processing
 * @ingroup icm
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
extern bool_t       icm_all_vtbm_cfg_ok;
extern bool_t       icm_unconfig_vtbm_detected;

extern bool_t       icm_all_versions_valid;
extern bool_t       icm_all_status_checked_once;

extern uint8_t      icm_rx_vtbm_can_err[MAX_NUM_VTBM];
extern FRAM struct  vtbm_status_s icm_vtbm_status[MAX_NUM_VTBM];
extern FRAM enum    IcmRxValidity_e icm_vtbm_status_valid[MAX_NUM_VTBM];

/* Incremented when a full update of VTBM status values has occurred */
extern uint8_t icm_status_update_ctr;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

void    icm_status_init(void);
void    icm_process_vtbm_status(const struct vtbm_status_s  *statp);
enum    IcmRespState_e icm_status_completion(bool_t connecting);

void    icm_age_status(void);
void    icm_clear_status_ok(void);
void    icm_clear_status_rx(void);
void    icm_clear_rx_can_errors(uint8_t num_vtbm);
bool_t  icm_is_all_balancing_status_off(void);
bool_t  icm_is_right_protocol_ver(uint8_t rxver);
void    icm_chk_invalid_status_rxd(uint8_t vtbm_num, enum IcmUnpackResult_e unpacked_ok);

#endif      /* ICM_STATUS_H */

