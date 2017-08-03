#if !defined(NNM_H)
#define      NNM_H
/**
 *
 * Native Network Manager header file
 *
 * @file
 * @ingroup nnm
 * $Id: nnm.h 12192 2014-06-30 14:50:16Z pr4 $
 */

/* Copyright (c) Ricardo plc 2012 - All rights reserved */

/**
 *
 * <h2> NNM Overview </h2>
 *
 * <h2> Approach used </h2>
 * The NNM is the network management layer for CAN buses making use of the ican
 * driver for their communications. Responsibilities include:
 *    * Control of the transceivers
 *    * Control of the shutdown keepalive
 *    * Kicking off any necessary handshaking
 *
 * External features can request a given bus be active or inactive and nnm
 * can do the behind-the-scenes things to action the request.
 *
 *  This feature sits above the CAN driver, but below the signal-oriented
 *  interface to the application.
 *
 * <h2> How to use </h2>
 * Call nnm_init() at startup
 * Call nnm_update() as periodic task
 *
 * Call nnm_set_comms_required() to enable or disable communication on a given
 * bus.
 * nnm_get_comms_enabled() will indicate whether a given transceiver is enabled
 * or not.
 *
 * @defgroup nnm Native Network Manager
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "ican_cfg.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/



#endif      /* NNM_H */

