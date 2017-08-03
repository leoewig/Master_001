/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    icm_server_frames_p.h
 *
 * description:  
 * Private header for ICM Server Frames
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
#if !defined(ICM_SERVER_FRAMES_P_H_)
#define ICM_SERVER_FRAMES_P_H_

/**
 * Private header for ICM Server Frames
 *
 * @file
 * @ingroup icm_server_frames
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
#if (defined(UNIT_TEST)) && (!defined(_lint))
extern bool_t  had_full_config;
extern uint16_t vtbm_rx_cfg_flags;
extern uint8_t vtbm_sysconfig_buff[MAX_CONFIG_SIZE];
#endif

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
/******************************************************************************
 * PACK FUNCTIONS
 ******************************************************************************/

PRIVATE enum MsgStat_e icm_unpack_measure_bal(uint8_t flg_tgt,struct icm_measctl_s *pout);
PRIVATE uint8_t  vtbm_pack_reqval_bal(const struct vtbm_status_s  *statp);
PRIVATE void  vtbm_pack_module_flags_counter(const uint8_t counter,
    const struct vtbm_status_s  *statp, uint8_t *flags1, uint8_t *flags2_counter);
PRIVATE uint8_t  icm_pack_refdig(const struct icm_ref_data_s *refp);

/******************************************************************************
 * READ FUNCTIONS
 ******************************************************************************/

PRIVATE enum MsgStat_e icm_read_cfg_head(const uint8_t buffer[], struct config_header_s *headp);
PRIVATE void icm_read_sig_cfg(const uint8_t buffer[], uint8_t vcount[], uint8_t tcount[]);
PRIVATE void icm_read_vtbm_ip_cfg(uint8_t cfg_byte, uint8_t *vcountp, uint8_t *tcountp);

/******************************************************************************
 * STORE FUNCTIONS
 ******************************************************************************/

PRIVATE void vtbm_store_cfg_head(const struct ican_msg_s *msgp);
PRIVATE void vtbm_store_cfg_body(const struct ican_msg_s *msgp);
PRIVATE void vtbm_store_cfg_data( uint8_t frame_num, const uint8_t data[]);

/******************************************************************************
 * ENCODE FUNCTIONS
 ******************************************************************************/

PRIVATE void encode_u16(FPTR uint8_t buffer[], uint16_t value, uint8_t nbits, uint8_t rshift);
PRIVATE void vtbm_encode_voltage( cell_v_t voltage, enum CellVstat_e status, FPTR uint8_t * datap );
PRIVATE void vtbm_encode_temp(temperature_t temp, enum CellTstat_e status, uint8_t rshift, FPTR uint8_t * datap );

/******************************************************************************
 * TO RAW FUNCTIONS
 ******************************************************************************/

PRIVATE uint16_t vtbm_voltstatus_to_raw(enum CellVstat_e voltstat, uint16_t raw_val);
PRIVATE uint16_t vtbm_tempstatus_to_raw(enum CellTstat_e tempstat, uint16_t raw_val);

/******************************************************************************
 * OTHER FUNCTIONS
 ******************************************************************************/

PRIVATE void erase_buffer(uint8_t datap[] , uint16_t nbytes);

#endif /* ICM_SERVER_FRAMES_P_H_ */
