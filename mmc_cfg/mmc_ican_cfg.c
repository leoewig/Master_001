/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      mmc_ican_cfg.c
 *
 * description:  
 * MMC Internal CAN bus data structure definitions
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */
#include "basic_types.h"
#include "ican.h"
#include "ican_p.h"
#include "ican_cfg.h"


#define         INTERNAL_TXQ_SIZE               20u   /**< transmit buffer size
                              @note available buffer size is one less than this
                              @todo analyse required size */
#define         INTERNAL_S12_RXQ_SIZE           90u     /**< S12 receive buffer
                              @note available buffer size is one less than this
                              @todo analyse required size */
/** @todo
    * I have slowed down the prodder transmit so that 
    *it fits into this buffer. Once the implementation is finalised
    * the prodder should be adjusted and the mmc tested at full speed. */

#define         INTERNAL_XGATE_RXQ_SIZE         5u    
                              /**< XGATE receive buffer
                              @note available buffer size is one less than this
                              @todo analyse required size */

/*
 * PUBLIC DATA (documented in header)
 */

PRIVATE struct ican_msg_s internal_tx_msg[INTERNAL_TXQ_SIZE];
PRIVATE struct ican_msg_s internal_s12rx_msg[INTERNAL_S12_RXQ_SIZE];
PRIVATE struct ican_msg_s internal_xgaterx_msg[INTERNAL_XGATE_RXQ_SIZE];

CONST struct ican_hw_s ican_hw[Can_n_channels] =  {
    { CAN_INTERNAL_BUS,&ican_channel2_regs },
};

//enum icanspeeds_e ican_speed[Can_n_channels] = {
//    Canspeed_1000,
//};

enum icanspeeds_e ican_speed[Can_n_channels] = {
    Canspeed_500,
};

struct ican_rxtxq_s ican_txq[Can_n_channels] = {
    {INTERNAL_TXQ_SIZE,0u,0u,0u,internal_tx_msg},
};

struct ican_rxtxq_s ican_s12_rxq[Can_n_channels] = {
    {INTERNAL_S12_RXQ_SIZE,0u,0u,0u,internal_s12rx_msg},
};

struct ican_rxtxq_s ican_xgate_rxq[Can_n_channels] = {
    {INTERNAL_XGATE_RXQ_SIZE,0u,0u,0u,internal_xgaterx_msg},
};

struct ican_filter_cfg_s ican_filter_cfg[Can_n_channels] = {
    { Canfilter_4_16bit,{0u,0u,0u,0u},{WIDE_OPEN_16BIT_MASK,0u,0u,0u} },
};

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/


/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
