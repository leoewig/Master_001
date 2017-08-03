/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    icm_csum.h
 *
 * description:  
 * Internal comms checksum public header file.
 * This file implements the algorithm used by both MMC and VTBM to calculate
 * additional data integrity checksums used on the internal CAN bus.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(ICM_CSUM_H)
#define      ICM_CSUM_H
/**
 *
 * Internal comms checksum public header file.
 * This file implements the algorithm used by both MMC and VTBM to calculate
 * additional data integrity checksums used on the internal CAN bus.
 *
 * @file
 * @ingroup icm_csum
 */

/**
 * Implements the frame checksum and frame counter functionality for use on
 * the internal CAN bus.
 *
 * Overview
 * ------------
 * This module provides functions used by the MMC and VTBM for CAN frame
 * validation. It does not contain any data.
 *
 * Usage
 * ----------
 * - Call icm_frame_csum() to calculate the internal comms checksum over a block
 * of  data.
 * - Call icm_valid_checksum() to validate the checkum on a #ican_msg_s
 * - Call icm_valid_frame_ctr() to validate a standard frame counter.
 *
 * @defgroup icm_csum Internal Comms frame checking
 * @ingroup icm
 */

/******************************************************************************
 * MACROS
 *****************************************************************************/

/**
 * Length of the smallest allowable ican frame
 * 1 byte of counter (or a nybble at any rate)
 * 1 byte of checksum
 */
#define     MIN_ICAN_BYTES 2u

/**
 * The byte offset from the frame _length_ at which thestatus=
 * message counter is placed ( by default)
 */
#define  ICAN_COUNTER_OFFSET 2u

/**
 * Return the byte index that contains the message counter of a protocol frame
 */
#define ICM_CTR_IDX(msp)    ((msgp)->dlc-ICAN_COUNTER_OFFSET)

/**
 * Extract a standard format message counter from a frame
 * By default this is in the penultimate bye of the message
 * And occupies 4 bits
 */
#define ICM_EXTRACT_COUNTER(msgp,mask) \
    ((mask) & ((msgp)->data[ICM_CTR_IDX(msgp) ]) )

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/* See body for doxygen */
uint8_t icm_frame_csum(const uint8_t frame[],uint8_t size, uint8_t seed);

/* See body for doxygen */
bool_t  icm_valid_checksum(const struct ican_msg_s *msgp);

/* See body for doxygen */
bool_t icm_valid_frame_ctr(const struct ican_msg_s * msgp,
    uint8_t mask, uint8_t *prev_counter );

#endif      /* ICM_CSUM_H */

