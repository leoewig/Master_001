/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_ican.h
 *
 * description: Header for MMC ICAN customisation
 *    
 *      
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(MMC_ICAN_H)
#define      MMC_ICAN_H
/**
 *
 * Header for MMC ICAN customisation
 *
 * @file
 * @ingroup mmc_ican
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 * MMC ICAN Customisation
 * ======================
 *
 * This is a subfeature of the [Internal CAN](group__ican.html) driver feature.
 * It contains those apsects of the CAN driver which are not generalisable
 * between applications.
 *
 * The following aspects of the driver must be customised:
 * - Can channels under the control of the driver, and hence interrupt handlers
 *   for each channel
 * - Core on which interrupts are executed
 * - Size of s/w buffers for sending and receiving
 * - Interface between errors reported by the driver and the application's
 *   fault management (if any).
 * - Provision of decoder functions to read received messages
 * - Customisation of receive interrupt to:
 *      - allocate receive messages to S12 or XGATE for decoding
 *      - trigger decoding function if required (e.g. triggering a low
 *        priority XGATE task in response to a message being received)
 *
 * CAN channel allocation
 * ----------------------
 * Only the internal bus is configured, with the handle #Can_internal, on
 * the bus given by #CAN_INTERNAL_BUS.  These are defined in mmc_ican_cfg.h,
 * which is copied to ican_cfg.h by the build process.
 *
 * ### Limitations
 * To change the bus allocated to the internal bus, both #CAN_INTERNAL_BUS
 * _and_ the locations of the interrupt vectors must be edited.
 *
 * Cores used
 * ----------
 * The S12 is used for transmit and error interrupts.  The XGATE is used for
 * the receive interrupt.
 *
 * Configuration
 * -------------
 * Configuration of buffers is done in mmc_ican_cfg.c
 *
 * Message reception
 * -----------------
 * The receive interrupt is handled by the XGATE.  It examines the message
 * ID and places the message in either the XGATE or S12 message receive buffer.
 * For XGATE messages, 
 * Installation
 * ============
 *
 * The driver is initialised by calling mmc_ican_init() which will in turn call
 * ican_init().
 *
 * Fault management (i.e. the interface between [ICAN](group__ican.html) and
 * [FM](group__f_m.html) is performed by calling mmc_ican_update() periodically.
 *
 * Requirement Satisfaction
 * ========================
 *
 * Requirement | Description | Implementation | Test
 * ------------|-------------|----------------|-----
 * REQ_BMS_SUP_ICN_001 | Send and receive messages | icantx(), __TBD__ |  __TBD__
 * REQ_BMS_SUP_ICN_002 | Baud-rate configurable | ican_chnl_set_speed() | _not tested_
 * REQ_BMS_SUP_ICN_003 | Monitor bus state | ican_error_from_isr(), ican_chnl_enable_interrupts() | __TBD__
 * REQ_BMS_SUP_ICN_004 | Report bus off or tx/rx fault... | ican_error_from_isr() | __TBD__
 * REQ_BMS_SUP_ICN_005 | ...as PLAT_INT_CAN_BUS_UNAVAILABLE | mmc_ican_filter_bus_unavailable() | __TBD__
 * REQ_BMS_SUP_ICN_005 | ...type general failure | mmc_ican_filter_bus_unavailable() | __TBD__
 * REQ_BMS_SUP_ICN_018 | Automatic bus off recovery | ican_chnl_enable_and_set_clock_source() | __TBD__
 * REQ_BMS_SUP_ICN_020 | Refuse message transmission when bus off | icantx() | __TBD__
 * REQ_BMS_SUP_ICN_007 | Buffer rx in FIFO | _TBD_ | __TBD__
 * REQ_BMS_SUP_ICN_008 | Ensure sufficient buffer space | #INTERNAL_XGATE_RXQ_SIZE, #INTERNAL_S12_RXQ_SIZE | _not tested_
 * REQ_BMS_SUP_ICN_015 | Report message lost fault when s/w buffer full... | _TBD_ | __TBD__
 * REQ_BMS_SUP_ICN_016 | ... as PLAT_INT_CAN_RX_BUFFER_FULL | mmc_ican_filter_rx_buffer_full() | __TBD__
 * REQ_BMS_SUP_ICN_017 | ... type INTERNAL_BUFFER | mmc_ican_filter_rx_buffer_full() | __TBD__
 * REQ_BMS_SUP_ICN_021 | Report message lost fault when h/w buffer full... | mmc_ican_filter_rx_buffer_full() | __TBD__
 * REQ_BMS_SUP_ICN_016 | ... as PLAT_INT_CAN_RX_BUFFER_FULL | mmc_ican_filter_rx_buffer_full() | __TBD__
 * REQ_BMS_SUP_ICN_022 | ... type HARDWARE_BUFFER | mmc_ican_filter_rx_buffer_full() | __TBD__
 * REQ_BMS_SUP_ICN_009 | Buffer tx messages if h/w busy | icantx() | __TBD__
 * REQ_BMS_SUP_ICN_010 | tx buffer big enough | #INTERNAL_TXQ_SIZE | _not tested_
 * REQ_BMS_SUP_ICN_011 | send buffered tx message as soon as h/w is free | ican_tx_from_isr() | __TBD__
 * REQ_BMS_SUP_ICN_012 | Report tx buffer full fault... | icantx() | __TBD__
 * REQ_BMS_SUP_ICN_013 | ... as PLAT_INT_CAN_TX_BUFFER_FULL | mmc_ican_filter_tx_buffer_full() | __TBD__
 * REQ_BMS_SUP_ICN_014 | ... type General failure | mmc_ican_filter_tx_buffer_full() | __TBD__
 * REQ_BMS_SUP_ICN_019 | All message buffers can be cleared | __Not implemented__ | _not tested_
 * REQ_BMS_SUP_ICN_023 | Make available indication if transmitting without error or warning (or later) state reached | #ican_errors_present | __TBD__
 * REQ_BMS_SUP_ICN_028 | Make available indication if receiving without error or warning (or later) state reached | #ican_errors_present | __TBD__
 * REQ_BMS_SUP_ICN_024 | Make available indication if receive buffer is full | #QUEUE_FULL | __TBD__
 * REQ_BMS_SUP_ICN_025 | Make available indication if transmit buffer is full | #QUEUE_FULL | __TBD__
 * REQ_BMS_SUP_ICN_026 | Make available indication if transmit buffer is full or transmit warnings being reported | #ican_errors_present | __TBD__
 * REQ_BMS_SUP_ICN_027 | Make available indication if receive buffer is full or receive warnings being reported | #ican_errors_present | __TBD__
 * 
 * @untestable REQ_BMS_SUP_ICN_002, examine #icanspeeds_e and
 *              ican_chnl_set_speed()
 * @untestable REQ_BMS_SUP_ICN_008, check by analysis of bus load
 * @untestable REQ_BMS_SUP_ICN_010 , check by analysis of bus load
 * @defgroup mmc_ican MMC Internal CAN customisation
 * @ingroup ican
 * @ingroup mmc_cfg
 */

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


/******************************************************************************
 * FUNCTION PROTOTYPES Documented at source
 *****************************************************************************/

/**
 * S12 interrupt for Internal CAN receive
 */
INTERRUPT void isr_internalcan_ReceiveInterrupt(void);
/**
 * S12 interrupt for Internal CAN transmission
 */
INTERRUPT void isr_internalcan_TransmitInterrupt(void);

/**
 * S12 interrupt for Internal CAN errors
 */
INTERRUPT void isr_internalcan_ErrorInterrupt(void);

void mmc_ican_init(void);
void mmc_ican_update(void);

#endif      /* MMC_ICAN_H */

