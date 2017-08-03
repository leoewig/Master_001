/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   prodder.h 
 *
 * description:  Prodder declarations
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial version
 *                       
 *
 *****************************************************************************/
/**
 * @file
 * @defgroup prodder Prodder
 *
 * Prodder declarations
 *
 */

#ifndef PRODDER_H
#define PRODDER_H
#include "pit.h"
#include "prodder_types.h"

/**
 * XGATE pit interrupt used to drive RTW code
 */
extern INTERRUPT void xgpit_rtw_isr(void);
/**
 * XGATE software trigger used to initialise RTW code
 */
extern INTERRUPT void xgswt_rtw_init(void);
/**
 * S12 interrupt called by _sif() from xgswt_rtw_init to signal that
 * initialisation is complete and RTW code can be started
 */
extern INTERRUPT void isr_rtw_init_complete(void);

/**
 * ISR to schedule rtc_emu loop
 */
extern INTERRUPT void isr_pit_rtc_emu_loop(void);


extern void SetDacChannel(uint8_t p_U8_channel, uint16_t p_U16_value);


extern uint32_t PROD_U32_auto_dac_enable_flags;

/****************************************************************************
 * Interfacing variables used to control message transmission 
 ***************************************************************************/
extern bool_t PROD_ECM_BMS_00_enable;
extern bool_t PROD_IC_HCAN_enable;
extern bool_t PROD_OBC_1_enable;
extern bool_t PROD_OBC_2_enable;
extern bool_t PROD_CCU_1_enable;
extern bool_t PROD_DCT_1_enable;
extern bool_t PROD_MCU_4_enable;
extern bool_t PROD_MCU_3_enable;
extern bool_t PROD_ECM_HMI_enable;

/****************************************************************************
 *
 ***************************************************************************/

extern bool_t pb0;
extern bool_t pb1;

#endif /* ifndef PRODDER_H */
