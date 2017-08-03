/**
 * @file
 *
 * Routines for initialising and controlling the I2C Bus.
 *
 * $Id: i2c.c 11203 2013-11-06 17:40:53Z vs7 $
 * I2C Driver Implementation
*/
/**
 * @ingroup I2C
 * @{
 * @file
 * @}
 */
/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "hardware.h"
#include "i2c.h"
#include "main_isr.h"
#include "isr.h"
#include "ts.h"

/******************************************************************************
 * PRIVATE MACRO DEFINITIONS
 *****************************************************************************/

/** I2C Status Register masks */
/**Mask for Received Acknowledge bit(0) of the IIC Status Register.
 * This bit denotes the value of SDA during the acknowledge bit of a bus cycle.
 * 0 -> Acknowledge received
 * 1 -> No acknowledge received */
#define I2C_IBSR_RXAK           0x01u

/** Mask for I-Bus Interrupt bit(1) of the IIC Status Register.
 * This bit is set when one of the following conditions occurs:
 * a)Arbitration lost (IBAL bit set)
 * b)Data transfer complete (TCF bit set)
 * c)Addressed as slave (IAAS bit set)
 * It will cause a processor interrupt request if the IBIE bit is set.
 * @note: This bit must be cleared by software, writing a one to it. */
#define I2C_IBSR_IBIF           0x02u

/** Mask for Arbitration Lost bit(4) of the IIC Status Register.
 * The arbitration lost bit (IBAL) is set by hardware when the arbitration
 * procedure is lost.
 * @note: This bit must be cleared by software, by writing a one to it. A write
 * of 0 has no effect on this bit */
#define I2C_IBSR_IBAL           0x10u

/** Mask for Bus Busy Bit bit(5) of the IIC Status Register.
 * This bit indicates the status of the bus.
 * 0 -> IDLE State. When a STOP signal is detected.
 * 1 -> BUSY State. When a START signal is detected. */
#define I2C_IBSR_IBB            0x20u

/** Mask for Addressed as a Slave Bit?bit(6) of the IIC Status Register.
 * When its own specific address (I-bus address register) is matched with the
 * calling address or it receives the general call address with GCEN== 1,this
 * bit is set.
 * 0 -> Not addressed.
 * 1 -> Addressed as a slave. */
#define I2C_IBSR_IAAS           0x40u

/** Mask for Data Transferring Bit(7) of the IIC Status Register.
 * When its own specific address (I-bus address register) is matched with the
 * calling address or it receives the general call address with GCEN== 1,this
 * bit is set.
 * 0 -> Transfer in progress.
 * 1 -> Transfer complete. Set by the falling edge of the 9th clock of a
 * byte transfer.
 * @note: This bit is only valid during or immediately following a transfer
 * to the IIC module or from the IIC module.*/
#define I2C_IBSR_TCF            0x80u

/** Mask for Transmit Acknowledge Enable?Bit(3) of the IIC Control Register.
 * This bit specifies the value driven onto SDA during data acknowledge cycles
 * for both master and slave receivers.
 * 0 -> An acknowledge signal will be sent out to the bus at the 9th clock bit
 *      after receiving one byte data.
 * 1 -> No acknowledge signal response is sent (i.e., acknowledge bit = 1)*/
#define I2C_IBCR_TXAK           0x08u

/** Mask for Transmit/Receive Mode Select Bit(4) of the IIC Control Register.
 * This bit selects the direction of master and slave transfers.
 * 0 -> Receive.
 * 1 -> Transmit.*/
#define I2C_IBCR_TXRX           0x10u

/** Mask for Master/Slave Mode Select Bit(5) of the IIC Control Register.
 * When this bit is changed from 0 to 1, a START signal is generated on the bus,
 * and the master mode is selected. When this bit is changed from 1 to 0, a STOP
 * signal is generated and the operation mode changes from master to slave.
 * @note: A STOP signal should only be generated if the IBIF flag is set.
 * 0 -> Slave Mode.
 * 1 -> Master Mode.*/
#define I2C_IBCR_MSSL           0x20u

/** Mask for I-Bus Interrupt Enable Bit(6) of the IIC Control Register.
 * 0 -> Interrupts from the IIC bus module are disabled. Note that this does not
 *      clear any currently pending interrupt condition.
 * 1 -> Interrupts from the IIC bus module are enabled. An IIC bus interrupt
 *      occurs provided the IBIF bit in the status register is also set.*/
#define I2C_INTERRUPT_ENABLE    0x40u

/** Mask for I-Bus Enable Bit(7) of the IIC Control Register.
 * This bit controls the software reset of the entire IIC bus module.
 * 0 -> The module is reset and disabled. This is the power-on reset situation.
 * 1 -> The IIC bus module is enabled.This bit must be set before any other IBCR
 *      bits have any effect.*/
#define I2C_ENABLE_BUS          0x80u

/**
 * maximum pause time in ticks for busy I2C waits (50usec). The delay utilized
 * the free running PIT timer.
 */
#define I2C_FIFTY_USEC		USEC_TO_TICKS(50u)

/******************************************************************************
 * PRIVATE FUNCTION DECLARATIONS
 *****************************************************************************/

/* defined in a header so they can be accessed for unit-test */
#include "i2c_p.h"

/******************************************************************************
 * PRIVATE TYPE DEFINITIONS
 *****************************************************************************/
/**
 * Enumeration to denote the state of the I2C bus transfer cycle*/
typedef enum
{
    I2C_BUS_FREE_CYCLE,
    I2C_ADDRESS_CYCLE,
    I2C_RX_DATA_CYCLE
} I2C_BusCycleType;

/**
 * Enumeration to denote the Communication direction. */
typedef enum
{
    I2C_TRANSMIT_DATA = 0u,
    I2C_RECEIVE_DATA  = 1u
} I2C_TransferType;

/******************************************************************************
 * PRIVATE CONSTANTS DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * VARIABLES DEFINITIONS
 *****************************************************************************/
/** Master List of all nodes connected on I2C bus */
PRIVATE I2CDevice_t reg_device_list[I2C_MAX_DEV];

/** ID of node to which communication is required */
PRIVATE I2CDeviceIndex_t       m_current_device;
/** Flag used for detection of specific cycle in I2C communication */
PRIVATE I2C_ProgressType       m_op_progress;
/** Pointer to buffer used for receiving data */
PRIVATE FPTR uint8_t*               m_data_buffer = NULL;
/** Specific cycle of communication */
PRIVATE I2C_BusCycleType  I2C_cycle;
/** Required transfer type for actual cycle START-STOP */
PRIVATE I2C_TransferType  transfer_req;
/** Pointer on data that shall be transmit in this cycle */
PRIVATE FPTR const uint8_t*    tx_data;
/** Number of bytes that was transmitted */
PRIVATE size_t            tx_data_no;
/** Number of bytes that are supposed to be transmit */
PRIVATE size_t            tx_data_len;
/** Number of bytes that was received */
PRIVATE size_t            rx_data_no;
/** Number of bytes that are supposed to be received */
PRIVATE size_t            rx_data_len;


/******************************************************************************
 * PRIVATE FUNCTION DEFINITIONS
 *****************************************************************************/
/**Initialize setup for I2C module.
 * @param p_device_index denotes the device number for which the I2C bus is
 * to be setup. */
PRIVATE FCODE void I2C_Setup( const I2CDeviceIndex_t p_device_index )
{
    /* Set new device as active */
    m_current_device = p_device_index;

    /* Disable module */
    IB0CR = 0x00u;
    /* Update the frequency divider register and select the required division
     * ratio to obtain SCL frequency from system clock */
    switch(reg_device_list[m_current_device].bus_speed)
    {
        case I2C_100:
            IB0FD = 0x63u;
        break;

        case I2C_400:
            IB0FD = 0x52u;
        break;

        default:
        break;
    }

    /* Enable module */
    IB0CR |= I2C_ENABLE_BUS;
    /* Set interrupt enable */
    IB0CR |= I2C_INTERRUPT_ENABLE;
    /* Initial cycle */
    I2C_cycle = I2C_BUS_FREE_CYCLE;
    /* TXAK bit set to zero so ack is enabled */
}

/**
 * Initialize I2C transfer (transmit or receive)
 * The order of operations performed around the timouts, are
 * important, no not reorder them.
 */
PRIVATE FCODE I2C_ProgressType I2C_InitTransfer(void)
{
    ts_ticks_t starting_ticks, current_ticks;
    bool_t timeout;
    uint8_t ccr;

    starting_ticks = ts_get_ticks();
    timeout=FALSE;

    /* Create START - Master and TX set  */
    IB0CR |= (uint8_t)(I2C_IBCR_MSSL | I2C_IBCR_TXRX);

    SAVE_INTERRUPTS(ccr);
    /* Wait for IBB flag to set to confirm the START signal is detected.*/
    while ((0u == (IB0SR & I2C_IBSR_IBB)) && (!timeout))
    {
        current_ticks = ts_get_ticks();
        if ((current_ticks - starting_ticks) >= I2C_FIFTY_USEC)
        {
            timeout = TRUE;
        }
    }
    RESTORE_INTERRUPTS(ccr);
    /* did the transaction timed out? */
    if (timeout)
    {
    	return I2c_failure;
    }

    starting_ticks = ts_get_ticks();
    timeout = FALSE;

    /* Send Slave address and transfer type */
    /* Change the state of the I2C bus to denote the Address Cycle. */
    I2C_cycle = I2C_ADDRESS_CYCLE;
    /* D7:D1 contains the Slave address and the DO bit contains the R/!W bit.*/
    IB0DR = (uint8_t)((uint16_t)((reg_device_list[m_current_device].address & 0x007Fu) << 1) |
                                     (uint16_t)transfer_req);

    /* Wait for IBB flag to set to confirm that the data(Slave address) is being
    transmitted*/
    SAVE_INTERRUPTS(ccr);
    while ((0u == (IB0SR & I2C_IBSR_IBB)) && (!timeout))
    {
        current_ticks = ts_get_ticks();
        if ((current_ticks - starting_ticks) >= I2C_FIFTY_USEC)
        {
            timeout = TRUE;
        }
    }
    RESTORE_INTERRUPTS(ccr);
    /* did the transaction timed out? */
    if (timeout)
    {
    	return I2c_failure;
    }
    return I2c_not_complete;
}

/** An intermediate function to transmit data.
 * The transfer request for the I2C_RECEIVE_DATA is present here. It Occurs when
 * a receive data is expected to be performed. Now to do this the uC needs to
 * send a START signal followed by the slave address. Hence in this case it
 * sets the IBCR Tx/!Rx bit to 1 (to Transmit the slave add.). After
 * receiving an ACK from the slave (an interrupt gets generated) this code
 * snippet changes the mode to reception(as should be, to receive data) by
 * negating the IBCR Tx/!Rx bit and changing the I2C_cycle to I2C_RX_DATA_CYCLE.
*/
PRIVATE FCODE void I2C_Tx(void)
{
    /* Is this end of address cycle of reception transfer, or.... */
    if(I2C_RECEIVE_DATA == transfer_req)
    {
        if(IB0SR & I2C_IBSR_RXAK)
        {
            /* slave not there or not responding */
            I2C_cycle = I2C_BUS_FREE_CYCLE;
            m_op_progress = I2c_failure;

            /* Generate STOP signal */
            IB0CR &= (uint8_t)(~I2C_IBCR_MSSL);
        }
        else
        {
            /* it's there, start the reception of slave's first byte */
            /* If we want just one...*/
            if(1u == rx_data_len)
            {
                /* ...do not transmit acknowledge after next transfer */
                IB0CR |= I2C_IBCR_TXAK;
            }
            else /* otherwise acknowledge after this next received byte */
            {
                IB0CR &= (uint8_t)(~I2C_IBCR_TXAK);
            }
            /* Set I2C to receive mode */
            IB0CR &= (uint8_t)(~I2C_IBCR_TXRX);
            /* Dummy read of IBDR which will start the transfer */
            (void) IB0DR;
            /* Set receive data cycle */
            I2C_cycle = I2C_RX_DATA_CYCLE;
        }
    }
    /* ...are we transmitting to a slave? */
    else
    {
        if(IB0SR & I2C_IBSR_RXAK)
        {
            /* slave doesn't want more data */
            I2C_cycle = I2C_BUS_FREE_CYCLE;
            m_op_progress = I2c_failure;

            /* Generate STOP signal */
            IB0CR &= (uint8_t)(~I2C_IBCR_MSSL);
        }
        else
        {
            I2C_TxData();
        }
    }
}

/** The data to be transmitted is written into the I2C Data Register, initiating
 *  the data transfer.*/
PRIVATE FCODE void I2C_TxData(void)
{
    /* Check if last byte was already send */
    if(tx_data_len == tx_data_no)
    {
        /* Generate STOP signal */
        IB0CR &= (uint8_t)(~I2C_IBCR_MSSL);

        I2C_cycle = I2C_BUS_FREE_CYCLE;
        m_op_progress = I2c_success;
    }
    else
    {
        /* Transfer data and update counter of already transmited data bytes */
        IB0DR = tx_data[tx_data_no];
        tx_data_no++;
    }
}

/** The received data byte is copied from the I2C Data Register into the data
 * buffer. Once all the expected data bytes are received a STOP signal is
 * generated just before reading the last data byte.*/
PRIVATE FCODE void I2C_Rx(void)
{
    if(I2C_RX_DATA_CYCLE == I2C_cycle)
    {
        /* Increment counter of received bytes */
        rx_data_no++;

        /* Check if this wasn't last requested byte */
        if(rx_data_len == rx_data_no)
        {
            /* Generate STOP signal before last byte is read */
            IB0CR &= (uint8_t)(~I2C_IBCR_MSSL);

            I2C_cycle = I2C_BUS_FREE_CYCLE;
            m_op_progress = I2c_success;
        }
        else if((rx_data_len - 1u) == rx_data_no)
        {
            /* Do not transmit acknowledge after last byte that shall be received */
            IB0CR |= I2C_IBCR_TXAK;
        }
        else
        {
            /* Acknowledge on I2C bus is generated automatically */
        }

        /* Read and save data - needs to be after non-ack and Stop generation */
        m_data_buffer[ rx_data_no - 1U ] = IB0DR;
    }
    else
    {
        /* Something went wrong */
    }
}

/******************************************************************************
 * PUBLIC FUNCTION DEFINITIONS
 *****************************************************************************/

/**
 * Resets bus cycle to bus free state, so next transaction may occur even
 * without reinitializing the peripheral. Intended to be used for retrying
 * a part of complex operations
 */
void I2C_ResetBusCycle(void)
{
    I2C_cycle = I2C_BUS_FREE_CYCLE;
    m_op_progress = I2c_success;
}

/* Set correct slave on I2C bus */
I2C_ProgressType I2C_Initialise(I2CDevice_t reg_node)
{
    m_op_progress = I2c_failure;

    if(reg_node.dev_indx < I2C_MAX_DEV)
    {
        /* register the device into the Device Master list */
        reg_device_list[reg_node.dev_indx].dev_indx  = reg_node.dev_indx;
        reg_device_list[reg_node.dev_indx].address   = reg_node.address;
        reg_device_list[reg_node.dev_indx].bus_speed = reg_node.bus_speed;

        m_current_device = I2C_MAX_DEV;
        m_op_progress    = I2c_success;
        I2C_cycle = I2C_BUS_FREE_CYCLE;
        /* Allow I2C bus interrupt */
        (void)set_interrupt_level(VECT_OFFSET_IIC0_Bus, INT_LVL_DEFAULT, INT_CPU_DEST);
    }

    return (m_op_progress);
}

/* Transmit requires number of bytes on I2C bus to specific slave/device */
FCODE I2C_ProgressType I2C_Transmit(I2CDeviceIndex_t  p_device_index,
                            FPTR const uint8_t* p_data,
                            const size_t        p_len )
{
    I2C_ProgressType ret_val = I2c_success;
    /* Check if the device is registered */
    if(p_device_index < I2C_MAX_DEV)
    {
        /* Check for operation already in progress */
        if(I2C_BUS_FREE_CYCLE == I2C_cycle)
        {
            /* Set the configuration parameters for the requested registered device */
            if( m_current_device != p_device_index )
            {
                I2C_Setup( p_device_index );
            }
            /* Setup for transmit operation */
            tx_data             = p_data;
            tx_data_no          = 0u;
            tx_data_len         = p_len;
            m_op_progress       = I2c_not_complete;
            transfer_req        = I2C_TRANSMIT_DATA;

            /* Prepare specific slave for transmitting
             * - START signal, slave address and R/W = 0 */
            m_op_progress = I2C_InitTransfer();
        }
        else
        {
            ret_val = I2c_not_complete;
        }
    }
    else
    {
        ret_val = I2c_failure;
    }

    return ret_val;
}

/* Receive requires number of bytes on I2C bus from specific slave/device */
FCODE I2C_ProgressType I2C_Receive(I2CDeviceIndex_t p_device_index,
                            FPTR uint8_t*         p_rcv_data,
                           const size_t           p_len )
{
    I2C_ProgressType ret_val = I2c_success;

    if(p_device_index < I2C_MAX_DEV)
    {
        /* Check for operation already in progress */
        if(I2C_BUS_FREE_CYCLE == I2C_cycle)
        {
            if( m_current_device != p_device_index )
            {
                I2C_Setup( p_device_index );
            }
            /* Setup for receive operation */
            m_data_buffer       = p_rcv_data;
            rx_data_no          = 0u;
            rx_data_len         = p_len;
            m_op_progress       = I2c_not_complete;
            transfer_req        = I2C_RECEIVE_DATA;

            /* Prepare specific slave for receiving
             * - START signal, slave address and R/W = 1 */
            m_op_progress = I2C_InitTransfer();
        }
        else
        {
            ret_val = I2c_not_complete;
        }
    }
    else
    {
        ret_val = I2c_failure;
    }

    return ret_val;
}

/* Check completition of operation */
FCODE I2C_ProgressType I2C_OpComplete(void)
{
    return(m_op_progress);
}

/* Interrupt service routine - Only master mode implemented */
INTERRUPT void I2C_Interrupt(void)
{
    /* check that for error status, we have lost arbitration, or we have have
     * addresses as a slave (there should be no other masters on the bus) */

    /* clear the interrupt bit in all cases */
    IB0SR |= I2C_IBSR_IBIF;

    if (0u != (IB0SR & (I2C_IBSR_IAAS | I2C_IBSR_IBAL)))
    {
        /* Clear status regiaster */
        IB0SR |= ( I2C_IBSR_IBIF | I2C_IBSR_IBAL | I2C_IBSR_IAAS);

        /* mark the i2c transaction as a failure */
        I2C_cycle     = I2C_BUS_FREE_CYCLE;
        m_op_progress = I2c_failure;
    }

    /* normal case - a transfer has completed, note that if an error occured TCF
     * if already be cleared out */
    if(0u != (IB0SR & I2C_IBSR_TCF))
    {
        /* Check if direction is TX */
        if(0u != (IB0CR & I2C_IBCR_TXRX))
        {
            I2C_Tx();
        }
        else /* Direction is RX */
        {
            I2C_Rx();
        }
    }
}
