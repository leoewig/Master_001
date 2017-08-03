#if !defined(CMD_H)
#define      CMD_H
/**
 *
 * cmd Charger mode determination public header file
 *
 *
 * @file
 * @ingroup cmd
 * $Id: cmd.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2012-2015 - All rights reserved */

/**
 *
 * This module turns the (processed) inputs - obtained from the charger
 * interface - into an operating mode demand, which is passed to the main state
 * control.
 *
 * The charger mode request is an indication to the main state control that the
 * charging strategy wishes to run.
 * Charging required is an indication that the battery cells need charging,
 * however this does not necessarily mean that charging is possible.  Charging
 * allowed is an indication that the conditions required for charging of cells
 * have been met.
 *
 * @defgroup cmd CMD Charger mode determination
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "basic_types.h"
#include "cmd_types.h"
#include "ctc_types.h"


/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * CALIBRATABLES
 *****************************************************************************/


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/**
 * CMD struct for NVS module. */
extern FRAM cmd_nvs_wrapper_s cmd_ram_image;
/**
 * Flag inidcating whether CMD has detected an MMC error condition */
extern bool_t   cmd_error_mode;
/**
 *  Charge mode requested flag to MS  */
extern bool_t   cmd_chg_mode_req;
/**
 *  CMD contactor state requests  */
extern enum Ctc_cont_state_e    cmd_dclink_ctc_command;
extern enum Ctc_cont_state_e    cmd_charger_ctc_command;

extern bool_t  cmd_chg_can_bus_required;
extern bool_t  cmd_veh_can_required;
extern bool_t  cmd_isolation_inhibit;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 * cmd feature startup initialisation function
 */
FCODE void cmd_init(void);

/**
 * cmd feature periodic task invocation function
 */
FCODE void cmd_update(void);

#endif      /* CMD_H */

