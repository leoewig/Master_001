/**
 * @file
 * @ingroup ican
 *
 * @todo write stuff here
 */

#include "basic_types.h"
#include "hardware.h"
#include "ican.h"
#include "ican_p.h"

/**
 * see whether all h/w transmit buffers are full
 * (i.e. txe0, txe1 and txe2 are all zero)
 * @param       HWP     pointer to CAN hardware registers
 * @retval      TRUE    all buffers contain messages waiting to be sent
 * @retval      FALSE   at least one buffer is empty
 */
#define		CAN_TX_FULL(HWP)		((HWP)->CANTFLG == 0u)
/**
 * see whether all h/w transmit buffers are empty
 * (i.e. txe0, txe1 and txe2 are all 1)
 * @param       HWP     pointer to CAN hardware registers
 * @retval      TRUE    all buffers are empty
 * @retval      FALSE   at least one buffer has a message waiting to be sent
 */
#define		CAN_TX_EMPTY(HWP)		((HWP)->CANTFLG == 7u)

bool_t icantx(uint16_t id, enum can_handle_e chnl, uint8_t dlc,
			FPTR const uint8_t data[])
{
    struct ican_rxtxq_s *txqp = &ican_txq[chnl]; /* the transmit queue for the
						  CAN controller */
    struct ican_msg_s *msgp; /* pointer to a slot in the queue where we'll copy
			       our message */
    uint8_t i;
    bool_t message_queued = FALSE;

    /**
     * @note Queue handling won't cope with multiple contexts sending data on a
     * single controller.  To do so, would need to protect test and
     * manipulation of the queue pointer.
     */
    /** @polyspace<MISRA-C:12.6:Low:Improve> Could code could be changed to fix this? */
    /** @polyspace<MISRA-C:13.2:Low:Improve> Could code could be changed to fix this? */
    if (ICAN_ERROR_IS_PRESENT(chnl,ican_bus_off))
    {
        /* do nothing */
    }
    else if (!QUEUE_FULL(txqp))
    {
	msgp = NEW_MESSAGE(txqp); /* get a pointer to the next free space
       				     in the queue */
	msgp->id = id;
	msgp->dlc = dlc;
	for (i = 0u ; i < dlc ; i++)
	{
	    msgp->data[i] = data[i];
	}
	MESSAGE_ADDED(txqp); /* indicate to the Tx interrupt that there is
				a message in the queue */
	/*
	 * Make sure the transmit interrupt is enabled.
	 * Note that if interrupts were already enabled, it is (just) possible
	 * that the ISR will have transmitted this message and disabled
	 * interrupts by now.  We will therefore be enabling an interrupt when
	 * the queue is already empty - but that is harmless because the
	 * interrupt will see that the queue is empty and just disable them
	 * again.
	 */
	ENABLE_CAN_TX_INTERRUPTS(ican_hw[chnl].hwp);
	message_queued = TRUE;
        /* there was space so clear the error present flag */
        ICAN_CLEAR_ERROR_PRESENT(chnl,ican_tx_buffer_overflow);
    }
    else
    {
        /* queue is full so set the error flag */
        ICAN_SET_ERROR(chnl,ican_tx_buffer_overflow);
    }
    return message_queued;
}

bool_t ican_tx_impaired(enum can_handle_e chnl)
{
    bool_t is_impaired = ((ican_hw[chnl].hwp->CANRFLG & CAN_TSTATE_MASK) != 0u)
                            || QUEUE_FULL(&ican_txq[chnl]);
    return is_impaired;
}

bool_t ican_rx_impaired(enum can_handle_e chnl)
{
    bool_t is_impaired = ((ican_hw[chnl].hwp->CANRFLG & CAN_RSTATE_MASK) != 0u)
                            || QUEUE_FULL(&ican_s12_rxq[chnl])
                            || QUEUE_FULL(&ican_xgate_rxq[chnl]);
    return is_impaired;
}

/**
 * Called from CAN transmit interrupt.
 * Will fill the CAN Transmit buffers with any pending messages in the transmit
 * queue.
 * @param hwp   Pointer to hardware registers for the controller causing the
 *        interrupt
 * @param txqp  pointer to the buffer of messages waiting to be transmitted
 *
 * If there are no messages left in the queue, this function will disable
 * the transmission interrupt.
 *
 * Function is suitable for either or both S12 and XGATE
 */
void ican_tx_from_isr(volatile struct can_controller_s *hwp,
				struct ican_rxtxq_s *txqp)
{
    uint8_t i; 	/* index into the data array */
    struct ican_msg_s *msgp;

    if (CAN_TX_EMPTY(hwp))
    {
	/* no messages queued so we can reset the priority logic */
	txqp->priority = 0u;    /* 0 is the highest priority */
    }

    while (!QUEUE_EMPTY(txqp) && !CAN_TX_FULL(hwp)
            && ((txqp->priority < CAN_MINIMUM_PRIORITY) || CAN_TX_EMPTY(hwp)))
    {
	/*
	 * There is a message to send, an output buffer to put it in and
         * we can add a new message with lower priority than any pending ones.
         * If there is no room for a lower priority message, we need to wait
         * for the buffers to empty so we can reset the logic.
         * Note that the scheme as implemented won't actually queue a message
         * with the minimum priority (255) but will stop at 254.  This is such
         * a marginal issue that any extra code complexity couldn't be
         * justified.
	 */

	msgp = OLDEST_MESSAGE(txqp); /* point to the next message to send */

	hwp->CANTBSEL = hwp->CANTFLG; /* select lowest numbered free buffer
					 (see CAN controller reference).  */
	hwp->CANTXFG.id01 = (uint16_t) (msgp->id << STD_ID_SHIFT);
						/* write the message ID */

	/* copy the data */
	for (i = 0u ; i < msgp->dlc ; i++)
	{
	    hwp->CANTXFG.data[i] = msgp->data[i];
	}
	hwp->CANTXFG.dlr = msgp->dlc;	/* write the data length count to the
					   data length register */
	hwp->CANTXFG.priority = txqp->priority;

	hwp->CANTFLG = hwp->CANTBSEL;	/* enable the message for sending */
        TEST_HOOK(0);

	/*
	 * update priority for the next message:
	 */

	if (txqp->priority < CAN_MINIMUM_PRIORITY)
	{
	    txqp->priority += 1u;       /* priority runs from 0 (highest)
                                           to CAN_MINIMUM_PRIORITY (255) lowest
                                           */
	}
	REMOVE_MESSAGE(txqp);		/* and move on to the next message */
    }

    /*
     * If there are no more queued messages, we don't need the transmission
     * interrupt, so disable it. 
     * BUT, what if the queue fills from the other core between our testing
     * for QUEUE_EMPTY and actually disabling the interrupt? That could leave
     * interrupts disabled when we need them.  Therefore we double-check that
     * they really should be disabled and re-enable if necessary.
     */
    if (QUEUE_EMPTY(txqp))
    {
        TEST_HOOK(1);   /* hosted test hook to let us inject a message */
	DISABLE_CAN_TX_INTERRUPTS(hwp);
        if (!QUEUE_EMPTY(txqp))
        {
            /* 
             * Crikey, a new message was queued after we checked for the
             * empty queue, so we need to re-enable interrupts
             */
            ENABLE_CAN_TX_INTERRUPTS(hwp);
        }
    }
}

void ican_error_from_isr(volatile struct can_controller_s *hwp,
                    enum can_handle_e chnl)
{
    uint8_t canrflg = hwp->CANRFLG;

    if (canrflg & CAN_CSC)
    {
        /* 
         * a state-change interrupt has occured which will be caused by
         * entering/leaving bus off or the warning/error status changing
         */
        if ((canrflg & CAN_TSTATE_MASK) == CAN_TSTATE_IS_BUSOFF)
        {
            /* gone bus off, so latch the error */
            ICAN_SET_ERROR(chnl,ican_bus_off);
        }
        else
        {
            ICAN_CLEAR_ERROR_PRESENT(chnl,ican_bus_off);
        }

        if ((canrflg & CAN_TSTATE_MASK) == CAN_TSTATE_ERROR)
        {
            /* Transmit error counter indicates an error */
            ICAN_SET_ERROR(chnl,ican_tx_error);
        }
        else
        {
            ICAN_CLEAR_ERROR_PRESENT(chnl,ican_tx_error);
        }

        if ((canrflg & CAN_TSTATE_MASK) == CAN_TSTATE_WARNING)
        {
            /* Transmit error counter indicates a warning */
            ICAN_SET_ERROR(chnl,ican_tx_warning);
        }
        else
        {
            ICAN_CLEAR_ERROR_PRESENT(chnl,ican_tx_warning);
        }

        if ((canrflg & CAN_RSTATE_MASK) == CAN_RSTATE_ERROR)
        {
            /* Receive error counter indicates an error */
            ICAN_SET_ERROR(chnl,ican_rx_error);
        }
        else
        {
            ICAN_CLEAR_ERROR_PRESENT(chnl,ican_rx_error);
        }

        if ((canrflg & CAN_RSTATE_MASK) == CAN_RSTATE_WARNING)
        {
            /* Receive error counter indicates a warning */
            ICAN_SET_ERROR(chnl,ican_rx_warning);
        }
        else
        {
            ICAN_CLEAR_ERROR_PRESENT(chnl,ican_rx_warning);
        }


        TEST_HOOK(0);
        hwp->CANRFLG = CAN_CSC;     /* clear the interrupt by writing the bit */
        TEST_HOOK(1);
    }
    if (canrflg & CAN_OVR)
    {
        /*
         * A receiver overrun has been detected.
         * Overrun is a special case because it is always transient, it
         * doesn't make much sense to say the error is 'present'.
         * Therefore we just set the latched flag.
         */
        ican_errors_latched[chnl].ican_rx_hw_buffer_overflow = TRUE;
        TEST_HOOK(2);
        hwp->CANRFLG = CAN_OVR;     /* clear the interrupt by writing the bit */
        TEST_HOOK(3);
        ICAN_CLEAR_ERROR_PRESENT(chnl,ican_rx_hw_buffer_overflow);
    }
}

#ifdef HOSTED
/*
 * CAN register structures are located with Cosmic's '@' so
 * we need to define regular variables for hosted unit testing
 */
volatile struct can_controller_s ican_channel0_regs;
volatile struct can_controller_s ican_channel1_regs;
volatile struct can_controller_s ican_channel2_regs;
volatile struct can_controller_s ican_channel3_regs;
#endif
