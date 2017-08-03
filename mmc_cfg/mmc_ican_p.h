/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      mmc_ican_p.h
 *
 * description:  
 * Private header for MMC Internal CAN driver configuration
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/
#if !defined(MMC_ICAN_P_H)
#define      MMC_ICAN_P_H


/*
 * PRIVATE function prototypes (documented in source)
 */
PRIVATE void mmc_ican_init_interrupt_handlers(void);
PRIVATE void mmc_ican_internal_fault_update(void);
PRIVATE void mmc_ican_filter_bus_unavailable(void);
PRIVATE void mmc_ican_filter_tx_buffer_full(void);
PRIVATE void mmc_ican_filter_rx_buffer_full(void);


#endif      /* MMC_ICAN_P_H */

