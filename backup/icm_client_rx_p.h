/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   icm_client_rx_p.h
 *
 * description:  
 * Internal Header for Internal comms feature, client frame receiving
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(ICM_CLIENT_RX_P_H)
#define      ICM_CLIENT_RX_P_H
/**
 *
 * Internal Header for Internal comms feature, client frame receiving
 *
 * @file
 * @ingroup icm_client_rx
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/*****************************************************************************
 * Variables
 *****************************************************************************/

#if defined( UNIT_TEST ) && !_lint

extern uint32_t icm_invalid_dlc_count;

extern uint32_t icm_invalid_checksum_count;

extern uint32_t icm_invalid_counter_count;

extern uint32_t icm_processed_frames_count;

extern uint8_t icm_status_counters [MAX_NUM_VTBM];
extern uint8_t icm_refs_counters [MAX_NUM_VTBM];
extern uint16_t icm_last_unrecog_id;
#endif

/*****************************************************************************
 * * PRIVATE function prototypes (documented in source)
 *****************************************************************************/

PRIVATE void icm_process_frame(const struct ican_msg_s *msgp);

/* Receiving a status message */
PRIVATE void    icm_rx_vtbm_status(const struct ican_msg_s *msgp);

PRIVATE bool_t  icm_check_status_ctr(const struct ican_msg_s *msgp);
PRIVATE bool_t icm_check_refs_ctr(const struct ican_msg_s *msgp);
PRIVATE bool_t icm_check_cells_ctr(const struct ican_msg_s *msgp);
PRIVATE bool_t icm_check_std_ctr(const struct ican_msg_s *msgp, 
    uint8_t countersp[] );

PRIVATE void icm_process_frame_fault(
    const struct ican_msg_s *msgp,
    bool_t valid_dlc, bool_t valid_csum,
    bool_t valid_ctr);

PRIVATE void icm_process_unknown_frame(const struct ican_msg_s *msgp);

PRIVATE void icm_capture_bad_rx(enum IcmUnpackResult_e reason,
    const struct ican_msg_s *msgp);

#endif      /* ICM_CLIENT_RX_P_H */

