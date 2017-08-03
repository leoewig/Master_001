/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   icm_client_frames.h
 *
 * description:   
 *  Frame packing and transmission for internal CAN
 *  This component provides an API to queue up internal comms frames on demand.
 *  It makes no attempt to provide any higher layer of logic above this, and
 *  thus is driven by the logic of the main icm.c file.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(ICM_CLIENT_FRAMES_H)
#define      ICM_CLIENT_FRAMES_H
/**
 *
 * Frame packing and transmission for internal CAN
 *
 * This component provides an API to queue up internal comms frames on demand.
 * It makes no attempt to provide any higher layer of logic above this, and
 * thus is driven by the logic of the main icm.c file.

 *
 * @file
 * @ingroup icm_client_frames
 * $Id: icm_client_frames.h 12024 2014-04-24 13:35:30Z dc17 $
 */

 /**
 * This component provides an API to queue up internal comms frames on demand.
 * It provides conversions between the network-packed representation and
 * structs containing variables in application units. Having encoded the frame,
 * it is transmitted.
 *
 * Unlike the equivalent 'trigger' commands in icm_data.c these functions do not
 * keep track of state.
 *
 * Transmitting messages
 * =====================
 * - Use icm_tx_measctl() to transmit a measure control message
 * - Use icm_tx_netctl() to transmit a network control message
 * - Use icm_tx_sysconfig() to tranmit a system configuration block
 * - Use icm_tx_balctl() to transmit a balancing control message
 *
 * All the transmit functions return a IcmTransmit_e indicating their success
 * or failure.
 *
 * Decoding messages
 * ======================
 * - Use icm_unpack_vtbm_status() to decode a received #ican_msg_s CAN frame
 * into a struct #vtbm_status_s representing the payload. This function should
 * be called if a message is popped off the incoming queue and is identified as
 * having a CAN ID corresponding to a status message.
 *
 * Other incoming frames are decoded on the XGATE and thus are not included in
 * this file.
 *
 * Implementation
 * ==================
 * Each of the transmit functions accepts data in an application-specific
 * format contained within a struct. The members of this are are in physical
 * units or abstracted enumerations.
 *
 * The actual data packing is done in the corresponding packing functions:
 * icm_pack_measctl() icm_pack_balctl() icm_pack_netctl and
 * icm_pack_sysconfig_data(). Each of these functions packs a single outgoing
 * frame. icm_tx_msg() is then called for each of these CAN messages.
 *
 * A small number of enumerations with simple values are defined so as to
 * have the same values both in the application and down the wire.
 * Each struct (or data array in the case of icm_tx_sysconfig() ) is packed
 * into its network representation in one or more CAN messages as appropriate
 * including the frame checksum.
 *
 * @note It is the responsibility of the caller to update any message counter
 * contained within the data passed into these functions.
 *
 * @defgroup icm_client_frames ICM client CAN frame manipulation
 * @ingroup icm
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "icm_types.h"
#include "icm_frames_types.h"   /* For message structure types */
#include "ican.h"

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
#ifdef UNIT_TEST
extern struct ican_msg_s icm_last_rejected_msg;
#endif

/*****************************************************************************
 * Stored information
 ****************************************************************************/

void icm_capture_rejected_cmd(void);

/*****************************************************************************
 * Transmission from MMC
 ****************************************************************************/

enum IcmTransmit_e icm_tx_measctl(const struct icm_measctl_s *pcmd);

enum IcmTransmit_e icm_tx_netctl(const struct icm_netctl_s *cmdp);

enum IcmTransmit_e icm_tx_sysconfig(const FPTR uint8_t datap[] );

enum IcmTransmit_e icm_tx_balctl(const struct icm_balctl_s *pcmd);

/*****************************************************************************
 * Receipt
 ****************************************************************************/

/* All other received message unpacking notionally  lives on the XGATE now */

/* Unpack the VTBM status message */
enum IcmUnpackResult_e icm_unpack_vtbm_status(const struct ican_msg_s *msgp,
                              struct vtbm_status_s * statp);

/**
 * Returns a pointer to the structure containing information on the last rejected
 * message. This is required by OBD for snapshot records. */
struct ican_msg_s* icm_ss_data_rejected_msg(void);

#endif      /* ICM_CLIENT_FRAMES_H */

