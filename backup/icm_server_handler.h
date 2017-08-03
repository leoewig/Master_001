/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    icm_server_handler.h 
 *
 * description:  
 *  Shared interface for VTBM & Prodder
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
#if !defined(ICM_SERVER_HANDLER_H)
#define ICM_SERVER_HANDLER_H
/**
 *
 * Shared interface for VTBM & Prodder
 *
 * @file
 * @ingroup icm
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */
/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "icm_frames_types.h"
/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
extern void vtbm_process_frame(const struct ican_msg_s *msgp);

extern bool_t vtbm_check_netctl_ctr(const struct ican_msg_s *msgp);
extern bool_t vtbm_check_measctl_ctr(const struct ican_msg_s *msgp);
extern bool_t vtbm_check_vtbmctl_ctr(const struct ican_msg_s *msgp);
extern bool_t vtbm_check_sysconfig_ctr(const struct ican_msg_s *msgp);
#ifdef VTBM_EOL_TEST
extern bool_t vtbm_check_debug_ctr(const struct ican_msg_s *msgp);
#endif

extern void vtbm_do_control (const struct icm_balctl_s *vtbm_ctlp);
extern void vtbm_do_measctl(const struct icm_measctl_s *meas_ctlp);
extern void vtbm_do_netctl(const struct icm_netctl_s *netctlp);
extern void vtbm_do_syscfg(const struct icm_syscfg_s *icm_syscfg);
#ifdef VTBM_EOL_TEST
extern void vtbm_do_debug(const struct icm_debug_s *icm_debug);
#endif

extern bool_t vtbm_id_matches(const struct ican_msg_s *msgp);
extern bool_t vtbm_comms_get_is_configured(void);

extern void vtbm_on_msg_error(enum MsgStat_e status, const struct ican_msg_s *ican_msgp, bool_t targetted_msg);
extern void vtbm_on_config_incomplete(enum MsgStat_e status, const struct ican_msg_s *ican_msgp);
extern void vtbm_on_config_invalid(enum MsgStat_e status, const struct ican_msg_s *ican_msgp);

#endif /* ICM_SERVER_HANDLER_H */
