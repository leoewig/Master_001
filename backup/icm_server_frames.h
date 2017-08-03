/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    icm_server_frames.h
 *
 * description:  
 * MMC and VTBM internal CAN frame packing/unpacking module
 * 
 * This file contains declarations of functions implementing the packing and unpacking on
 * the S12 for the internal protocol.
 * This file is shared between VTBM & Prodder builds, and IS included in HUTs
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(ICM_SERVER_FRAMES_H)
#define      ICM_SERVER_FRAMES_H
/**
 *
 * MMC and VTBM internal CAN frame packing/unpacking module
 *
 * This file contains declarations of functions implementing the packing and unpacking on
 * the S12 for the internal protocol.
 * This file is shared between VTBM & Prodder builds, and IS included in HUTs
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

enum ConfigStatus_e
{
    /** One or more Configuration frames are absent */
    CfgStatIncomplete,
    /** A complete config has been seen */
    CfgStatComplete,
    /** There was an error in processing the config */
    CfgStatError
};

/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/
extern bool_t vtbm_protocol_checked;
#if 0
extern struct config_header_s vtbm_config_header;
extern uint8_t vtbm_tinp_ct[SYSCFG_NUM_VTBM];
extern uint8_t vtbm_vinp_ct[SYSCFG_NUM_VTBM];
#endif
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

void vtbm_process_frame(const struct ican_msg_s *msgp);
bool_t vtbm_get_had_full_config(void);
enum ConfigStatus_e vtbm_get_cfg_status(void);
void vtbm_clear_cfg_status(void);
enum ConfigStatus_e vtbm_store_cfg_frame(const struct ican_msg_s *msgp);
enum MsgStat_e vtbm_decode_sysconfig(struct icm_syscfg_s *icm_syscfg);
enum MsgStat_e vtbm_validate_sysconfig(void);

/******************************************************************************
 * Top level frame decoding
 *****************************************************************************/

enum MsgStat_e icm_unpack_measctl(const struct ican_msg_s *msgp, struct icm_measctl_s *pout);
enum MsgStat_e icm_unpack_netctl(const struct ican_msg_s *msgp, struct icm_netctl_s *pout);
enum MsgStat_e icm_unpack_vtbmctl(const struct ican_msg_s *msgp, struct icm_balctl_s *pout);

/******************************************************************************
 * Top field unpacking for specific frames
 *****************************************************************************/

enum MsgStat_e icm_unpack_meas_action(uint8_t meas_action, struct icm_measctl_s *pout);
enum MsgStat_e icm_unpack_measure_target(uint8_t flgtarget, struct icm_measctl_s *pout);
enum MsgStat_e icm_unpack_bal_cmd(uint8_t balcmd, struct icm_balctl_s *pout);
void icm_unpack_ctl_flags(const struct ican_msg_s *msgp, struct icm_balctl_s *pout);


/******************************************************************************
 * Message packing
 *****************************************************************************/

void vtbm_pack_status(const struct vtbm_status_s *statp, uint8_t *counter, struct ican_msg_s *msgp );
void vtbm_pack_celldata(const struct icm_cell_pair_data_s *cellp, struct ican_msg_s *msgp);
#ifdef VTBM_EOL_TEST
void vtbm_pack_raw_celldata(const struct icm_cell_pair_raw_data_s *cellp, struct ican_msg_s *msgp);
#endif
void vtbm_pack_refdata(const struct icm_ref_data_s *refp, struct ican_msg_s *msgp);

#endif      /* ICM_SERVER_FRAMES_H */

