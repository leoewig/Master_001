/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    icm_client_rx.h 
 *
 * description:   
 * Header file for ICM client queue reading.
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
#if !defined(ICM_CLIENT_RX_H)
#define      ICM_CLIENT_RX_H
/**
 * @file
 * @ingroup icm_client_rx
 * Header file for ICM client queue reading.
 * $Id: icm_client_rx.h 4131 2012-10-04 10:19:50Z pr4 $
 */

 /**
 * This module pops CAN frames off the queue for the internal CAN bus, invokes
 * the appropriate validation functions for message counter and checksum and
 * then calls the appropriate handler function for each frame.
 *
 * It has knowledge of the ican data structure and of the functions that turn
 * a status ican messages into structures on the S12.
 *
 * Each message popped off the queue is passed to icm_process_frame(). This
 * function expands the message map defined in icm_frame_data.defs
 *
 * 
 * This functionality is separate from the other ICM client functions, because 
 * it is not needed in all builds.
 *
 * icm_read_rx_queue() Should be called from the main ICM task
 *
 * @defgroup icm_client_rx ICM client received frame dispatcher
 * @ingroup icm
 */

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

void icm_read_rx_queue(void);

#endif      /* ICM_CLIENT_RX_H */

