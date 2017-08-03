/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   icm_server_handler_p.h 
 *
 * description:  
 *  Private header for ICM server handler
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
#if !defined(ICM_SERVER_HANDLER_P_H_)
#define ICM_SERVER_HANDLER_P_H_

/**
 *
 *  Private header for ICM server handler
 *
 * @file
 * @ingroup icm_server_handler
 * $Id: icm_server_handler_p.h 5640 2012-12-06 13:53:18Z jh24 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

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
#if defined( UNIT_TEST ) && !_lint
extern uint32_t icm_invalid_id_count;
extern uint32_t icm_invalid_dlc_count;
extern uint32_t icm_invalid_csum_count;
extern uint32_t icm_invalid_ctr_count;
#endif
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
PRIVATE void vtbm_read_measctl(const struct ican_msg_s *msgp);
PRIVATE void vtbm_read_netctl(const struct ican_msg_s *msgp);
PRIVATE void vtbm_read_vtbmctl(const struct ican_msg_s *msgp);
PRIVATE void vtbm_read_sysconfig(const struct ican_msg_s *msgp);

#endif /* ICM_SERVER_HANDLER_P_H_ */
