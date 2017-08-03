/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     mmc_ican.c
 *
 * description:  
 * MMC interface to internal CAN driver
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/

#include "basic_types.h"
#include "fm.h"
#include "hardware.h"
#include "isr.h"
#include "main_isr.h"
#include "ican.h"
#include "ican_p.h"
#include "ican_cfg.h"
#include "mmc_ican.h"
#include "mmc_ican_p.h"

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#if CAN_INTERNAL_BUS != 2u
#error  CAN Interrupts installed assuming internal bus on CAN 2
#endif


/*
 * PUBLIC DATA (documented in header)
 */


/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/


/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/**
 * Initialise Internal CAN driver
 */
void mmc_ican_init(void)
{
    mmc_ican_init_interrupt_handlers();
    ican_init();
}

/**
 * Periodic update function for Internal CAN driver
 *
 * Processes diagnostic messages and provides the interface to
 * [Fault management](group__fm.html)
 *
 * @todo add diagnostic message handling
 */
void mmc_ican_update(void)
{
    mmc_ican_internal_fault_update();
}

/**
 * @todo this is a temporary isr
 * The real one needs to:
 * + be on the XGATE
 * + have proper logic to route diagnostics to S12 and cell data to xgate
 * + trigger a decoder with an XGATE s/w interrupt on receipt of cell data 
 */
INTERRUPT void isr_internalcan_ReceiveInterrupt(void)
{
    volatile struct can_controller_s *hwp = ican_hw[Can_internal].hwp;
    struct ican_rxtxq_s *rxqp;

    struct ican_msg_s *msgp;
    uint16_t id;
    bool_t s12_message;
    uint8_t i;

    if (hwp->CANRFLG & CAN_RXF)
    {
	/* there is a message to be read */
        id = hwp->CANRXFG.id01 >> STD_ID_SHIFT;
        /** @todo put in core selection logic based on id */
        s12_message = (id > 1u);
        rxqp = (s12_message)
                ? &ican_s12_rxq[Can_internal] : &ican_xgate_rxq[Can_internal];
	if (!QUEUE_FULL(rxqp))
	{
	    /* there is room in the buffer to store the message */
	    msgp = NEW_MESSAGE(rxqp);
	    msgp->id = hwp->CANRXFG.id01 >> STD_ID_SHIFT;
	    msgp->dlc = hwp->CANRXFG.dlr;
	    for (i = 0u ; i < msgp->dlc ; i++)
	    {
		msgp->data[i] = hwp->CANRXFG.data[i];
	    }
	    MESSAGE_ADDED(rxqp);
	}
	else if (s12_message)
	{
	    ICAN_SET_ERROR(Can_internal,ican_rx_s12_sw_buffer_overflow);
	}
        else
        {
	    ICAN_SET_ERROR(Can_internal,ican_rx_xg_sw_buffer_overflow);
	}
	hwp->CANRFLG = CAN_RXF; /* clear the RXF flag (by writing 1 to it)*/
    }
}

/**
 * Can transmit interrupt which does nothing except call ican_tx_from_isr()
 * with arguments for Can_internal
 */
INTERRUPT void isr_internalcan_TransmitInterrupt(void)
{
    ican_tx_from_isr(ican_hw[Can_internal].hwp,&ican_txq[Can_internal]);
}

/**
 * Can error interrupt which does nothing except call ican_error_from_isr()
 * with arguments for Can_internal
 */ 
INTERRUPT void isr_internalcan_ErrorInterrupt(void)
{
    ican_error_from_isr(ican_hw[Can_internal].hwp,Can_internal);
}
 

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/**
 * Set up the priority and destination of the CAN interrupts
 *
 * @todo change receive interrupt to the XGATE
 */
PRIVATE void mmc_ican_init_interrupt_handlers(void)
{
    /*
     * Internal bus is on CAN 2
     */
    (void) set_interrupt_level(VECT_OFFSET_CAN2_TxInterrupt,
            INT_LVL_DEFAULT,INT_CPU_DEST);
    (void) set_interrupt_level(VECT_OFFSET_CAN2_RxInterrupt,
            INT_LVL_DEFAULT,INT_CPU_DEST);
    (void) set_interrupt_level(VECT_OFFSET_CAN2_ErrorInterrupt,
            INT_LVL_DEFAULT,INT_CPU_DEST);
}

/**
 * Interface between [ICAN](group__ican.html) module errors and
 * [FM](group_f_m.html) failures.
 */
PRIVATE void mmc_ican_internal_fault_update(void)
{
    mmc_ican_filter_bus_unavailable();
    mmc_ican_filter_tx_buffer_full();
    mmc_ican_filter_rx_buffer_full();
}


/**
 * Filter #Fm_plat_int_can_bus_unavailable Bus Unavailable failure
 *
 * This is caused by:
 * + Bus Off (ican_errors_latched.ican_bus_off)
 * + Tranmit error (ican_errors_latched.ican_tx_error)
 * + Receive error (ican_errors_latched.ican_rx_error)
 * Regardless of cause, subtype is always Fm_general_failure_information
 */
PRIVATE void mmc_ican_filter_bus_unavailable(void)
{
    bool_t fault_present = (ICAN_ERROR_IS_LATCHED(Can_internal,ican_bus_off)
                        | ICAN_ERROR_IS_LATCHED(Can_internal,ican_tx_error)
                        | ICAN_ERROR_IS_LATCHED(Can_internal,ican_rx_error));

//    (void) fm_update_fault_filter(Fm_plat_int_can_bus_unavailable,
//                            Fm_general_failure_information,
//                            fault_present, TRUE);
    /*
     * now clear the latches if there is no fault present
     */
    ICAN_CLEAR_LATCH_IF_ERROR_NOT_PRESENT(Can_internal,ican_bus_off);
    ICAN_CLEAR_LATCH_IF_ERROR_NOT_PRESENT(Can_internal,ican_tx_error);
    ICAN_CLEAR_LATCH_IF_ERROR_NOT_PRESENT(Can_internal,ican_rx_error);
}

/**
 * Filter Fm_plat_int_can_tx_buffer_full Tx buffer overflow failure
 *
 * This is caused by s/w transmit buffer being full when a message transmit is
 * attempted (ican_errors_latched.ican_tx_buffer_overflow)
 */
PRIVATE void mmc_ican_filter_tx_buffer_full(void)
{
    /* update transmit buffer full fault filter */
//    (void) fm_update_fault_filter(Fm_plat_int_can_tx_buffer_full,
 //                   Fm_general_failure_information,
  //                  ICAN_ERROR_IS_LATCHED(Can_internal,ican_tx_buffer_overflow),
   //                 TRUE);
    ICAN_CLEAR_LATCH_IF_ERROR_NOT_PRESENT(Can_internal,ican_tx_buffer_overflow);
}

/**
 * Filter Fm_plat_int_can_rx_buffer_full Rx buffer full failure
 *
 * Receive buffer full error has two subtypes: Fm_hardware_buffer (i.e.
 * overrun) and Fm_internal_buffer.
 * Matters are further complicated because we have two s/w buffers, one for
 * the S12 and one for the XGATE.Error flags are #ican_errors_latched
 * .ican_rx_hw_buffer_overflow, ican_rx_s12_sw_buffer_overflow, and 
 * .ican_rx_xg_sw_buffer_overflow
 *
 * We arbitrarily give the hardware priority in setting the subtype.
 */
PRIVATE void mmc_ican_filter_rx_buffer_full(void)
{
    if (ICAN_ERROR_IS_LATCHED(Can_internal,ican_rx_hw_buffer_overflow))
    {
        /* 
         * h/w buffer fault is latched so update filter with the
         * Fm_hardware_buffer subtype
         */
  //      (void) fm_update_fault_filter(Fm_plat_int_can_rx_buffer_full,
   //             Fm_hardware_buffer, TRUE, TRUE);
    }
    else if (ICAN_ERROR_IS_LATCHED(Can_internal,ican_rx_s12_sw_buffer_overflow)
        || ICAN_ERROR_IS_LATCHED(Can_internal,ican_rx_xg_sw_buffer_overflow))
    {
        /*
         * one of the s/w buffers has filled up so update filter with the
         * Fm_internal_buffer subtype
         */
  //      (void) fm_update_fault_filter(Fm_plat_int_can_rx_buffer_full,
  //              Fm_internal_buffer, TRUE, TRUE);
    }
    else
    {
        /* 
         * none of the contributing errors is present so update filter
         * with the fault absent.  NB the subtype here doesn't matter
         */
    //    (void) fm_update_fault_filter(Fm_plat_int_can_rx_buffer_full,
    //            Fm_hardware_buffer, FALSE, TRUE);
    }

    /*
     * Clear the latches if the faults aren't present
     */
    ICAN_CLEAR_LATCH_IF_ERROR_NOT_PRESENT(Can_internal,
                                                ican_rx_hw_buffer_overflow);
    ICAN_CLEAR_LATCH_IF_ERROR_NOT_PRESENT(Can_internal,
                                                ican_rx_s12_sw_buffer_overflow);
    ICAN_CLEAR_LATCH_IF_ERROR_NOT_PRESENT(Can_internal,
                                                ican_rx_xg_sw_buffer_overflow);
}
