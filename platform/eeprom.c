/**
 * @file
 * @ingroup EEPROM
 *
 * Routines for controlling the EEPROM Driver.
 *
 * $Id: eeprom.c 6251 2013-01-16 18:33:32Z vs7 $
 * EEPROM Driver Implementation
*/
/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "eeprom.h"
#include "spi.h"
#include "ts.h"
#include "watchdog.h"
#include "eeprom_p.h"

/******************************************************************************
 * PRIVATE MACRO DEFINITIONS
 *****************************************************************************/
#define CMD_WRITE_DATA                  0x02u /**< Write Data Command */
#define CMD_READ_DATA                   0x03u /**< Read Data Command */
#define CMD_READ_STATUS_REG             0x05u /**< Read the Status Register */
#define CMD_WRITE_STATUS_REG            0x01u /**< Write the Status Register */
#define CMD_WRITE_ENABLE                0x06u /**< Set the Write Enable Latch */
#define CMD_WRITE_DISABLE               0x04u /**< Clear the Write Enable Latch */

#define LEN_WRITE_ENABLE                0x01u /**< No.of bytes to transmit in issuing WRite ENable command */
#define LEN_WRITE_DISABLE               0x01u /**< No.of bytes to transmit in issuing WRite DIsable command */
#define LEN_READ_STATUS_REG             0x02u /**< No.of bytes to transmit in issuing the Read Status register command */
#define LEN_WRITE_STATUS_REG            0x02u /**< No.of bytes to transmit in issuing the Write Status register command */

#define DEVICE_NOT_READY_BIT            0x01u /**< Status Reg.Bit 0 (!Rdy) */
#define DEVICE_WRITE_ENABLE_BIT         0x02u /**< Status Reg.Bit 1 (WEN) */

#define EEPROM_LENGTH                 0x4000u /**< number of bytes of data
                                                stored in EEPROM */

#define EEPROM_PAGE_LENGTH                64u /**< Size of EEPROM unit that can
                                              be written in one go */
#define EEPROM_PAGE_ADDRESS_MASK        \
                        ((uint16_t) ~ (uint16_t) (EEPROM_PAGE_LENGTH-1u))
                                            /**< mask for EEPROM address
                                             * which gives first byte of page */

/** test whether #DEVICE_NOT_READY_BIT is clear */
#define EEPROM_IS_READY(status_register) \
                            ((status_register & DEVICE_NOT_READY_BIT) == 0u)

/******************************************************************************
 * CALIBRATABLE VARIABLE DEFINITIONS
 *****************************************************************************/

#define MEMORY_LOCATION CALIBRATION_MEMORY_S12
#include "memory_layout.h"
/**
 * Maximum time allowed for a write to the EEPROM.
 * @note although type is ts_ticks_t, units are microseconds
 * @a2l{min,0}
 * @a2l{max,1000000}
 * @a2l{units,microseconds}
 * @a2l{longidentifier,The maximum time allowed for the EEPROM device to become ready (microseconds)}
 */
PRIVATE CAL ts_ticks_t ee_device_timeout_us = 5500u;


/******************************************************************************
 * PRIVATE TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * VARIABLES DEFINITIONS
 *****************************************************************************/
PRIVATE Eeprom_Op_Status ee_driver_status = Ee_driver_uninitialised;
/**<
 * @a2l{conversion, Eeprom_Op_Status}
 * @a2l{LongIdentifier, Variable to Convey the eeprom status}
 */

/******************************************************************************
 * PUBLIC FUNCTION DEFINITIONS
 *****************************************************************************/
/**Read the data from the EEPROM */
Eeprom_Op_Status eeprom_read_data(const uint16_t  address,
                                         FPTR uint8_t * buffer_p,
                                        uint16_t  length)
{
    Eeprom_Op_Status read_status;

    /* Check for the validity of the address range */
    if(!is_addr_range_valid(address,length))
    {
        read_status = Ee_arg_address_error;
    }
    else if (buffer_p == FNULL)
    {
        read_status = Ee_arg_buffer_error;
    }
    else if (ee_driver_status == Ee_driver_ready)
    {
        /* 
         * driver is free - we haven't interrupted a lower priority task
         * using the EEPROM.
         * Now mark the driver as busy so that a higher priority task won't
         * interfere with the SPI comms
         */
	ee_driver_status = Ee_driver_busy;
        if (!wait_for_ready())
        {
            read_status = Ee_device_timeout;
        }
        else
        {
            eeprom_perform_read(address, buffer_p, length);

            /* It worked */
            read_status = Ee_driver_ready;
        }

        /* And free up the driver */
	ee_driver_status = Ee_driver_ready;
    }
    else
    {
        /* report back the reason for not reading */
        read_status = ee_driver_status;
    }
    return read_status;
}

/**Write data into the EEPROM */
Eeprom_Op_Status eeprom_write_data(const uint16_t  address,
                                    FPTR const uint8_t * buffer_p,
                                     uint16_t  length)
{
    Eeprom_Op_Status write_status;

    if (!is_addr_range_valid(address,length))
    {
        /* either length is zero or address range doesn't fit in EEPROM */
        write_status = Ee_arg_address_error;
    }
    else if (buffer_p == FNULL)
    {
        /* buffer_p is NULL */
        write_status = Ee_arg_buffer_error;
    }
    else if (ee_driver_status == Ee_driver_ready)
    {
        /* 
         * driver is free - we haven't interrupted a lower priority task
         * using the EEPROM.
         * Now mark the driver as busy so that a higher priority task won't
         * interfere with the SPI comms
         */
	ee_driver_status = Ee_driver_busy;

        /* and write the data */
        write_status = eeprom_write_pages(address,buffer_p,length);

        /* free up the driver */
        ee_driver_status = Ee_driver_ready;
    }
    else
    {
        /* explain why we didn't do anything */
        write_status = ee_driver_status;
    }

    return write_status;
}


/**Get the EEPROM driver status */
Eeprom_Op_Status eeprom_status(void)
{
    return ee_driver_status;
}

#ifdef UNIT_TEST
/**
 * helper function to set the driver status
 */
void eeprom_set_status(Eeprom_Op_Status new_status)
{
    ee_driver_status = new_status;
}
#endif


Eeprom_Op_Status eeprom_init(void)
{
    uint8_t status_register;
    bool_t ready;
    Eeprom_Op_Status status = Ee_driver_ready;

    status_register = read_status_reg();

    /* if device is busy, waity for it to become ready */
    ready = (EEPROM_IS_READY(status_register)) ? TRUE : wait_for_ready();
    
    if (!ready)
    {
        /*
         * there was timeout waiting for EEPROM to become ready,
         * but since we weren't actually writing data, call it
         * a device error
         */
	status = Ee_device_error;
    }

    if (status == Ee_driver_ready)
    {
        /* 
         * EEPROM ok so far.  Let's see the Write-enable bit change to
         * make sure
         */
        eeprom_write_enable();
        if (!eeprom_is_write_enabled())
        {
            /* write enable didn't work: EEPROM appears faulty */
	    status = Ee_device_error;
        }
    }
    if (status == Ee_driver_ready)
    {
        /* to get here, we are write enabled */
	eeprom_clear_all_protection();
	eeprom_write_disable_wait_for_ready();
        /* 
         * with protection cleared and no write protection, status register
         * should read as all zeros
         */
        status_register = read_status_reg();
        if (status_register != 0u)
        {
            /* either clear protection or disabling write failed:
             * EEPROM appears faulty
             */
	    status = Ee_device_error;
        }
    }
    ee_driver_status = status;
    return status;
}



/******************************************************************************
 * PRIVATE FUNCTION DEFINITIONS
 *****************************************************************************/
#if LEN_READ_STATUS_REG != 2u
#error Read status register command length has changed!
#endif
/**
 * Read the EEPROM Status Register.
 *
 * @retval Returns the status register value
 */
PRIVATE uint8_t read_status_reg(void)
{
    return eeprom_two_byte_command(CMD_READ_STATUS_REG,0u);
}

/**
 * Check that address range defined by a start address and length is valid
 *
 * @param addr Start address for the operation
 * @param len No.of bytes to be read/written.
 *
 * @retval TRUE range lies within the EEPROM
 * @retval FALSE either len is zero or range does not lie within the EEPROM
 */
PRIVATE bool_t is_addr_range_valid(uint16_t addr, uint16_t len)
{
    bool_t valid_address;
    /* make end_address 32-bits to catch overflow */
    uint32_t end_address = (uint32_t) addr + (uint32_t) len;

    valid_address = ((len > 0u) && (end_address <= EEPROM_LENGTH))
                    ? TRUE : FALSE;

    return valid_address;
}

/**
 *  Wait for EEPROM to be ready to accept another command
 *
 *  Blocks until either /RDY bit in status register clears or the maximum time
 *  @ref ee_device_timeout_us is been exceeded.
 *  @retval TRUE EEPROM is now ready to accept a command
 *  @retval FALSE the timeout occured
 *
 *  We are careful in the loop to account for the forground interrupting us and
 *  making the timeout appear to have occurred: timeouts in themselves don't
 *  matter because they could be down to higher priority tasks coming along,
 *  all that matters is a timeout after which the EEPROM isn't ready.
 */
PRIVATE bool_t wait_for_ready()
{
    bool_t     is_ready     = FALSE;
    bool_t     exitloop   = FALSE;
    ts_ticks_t start_time = ts_get_ticks();

    while (exitloop == FALSE)
    {
        if (TS_IS_ELAPSED_US(start_time, ee_device_timeout_us))
        {
            exitloop = TRUE;
        }
        if (EEPROM_IS_READY(read_status_reg()))
        {
            is_ready   = TRUE;
            exitloop = TRUE;
        }
    }

    return is_ready;
}

#if LEN_WRITE_STATUS_REG != 2u
#error Write status register command length has changed!
#endif
/**
 * clear WPEN, BP1, BP0 in the status register
 */
PRIVATE void eeprom_clear_all_protection(void)
{
    /* just send all zeros to clear the protection bits */
    (void) eeprom_two_byte_command(CMD_WRITE_STATUS_REG,0u);
}

/**
 * checks that WEN bit is set in status register
 * @retval TRUE EEPROM is write enabled
 * @retval FALSE EEPROM is write-protected
 */
PRIVATE bool_t eeprom_is_write_enabled(void)
{
    return (read_status_reg() & DEVICE_WRITE_ENABLE_BIT) ? TRUE : FALSE;
}

#if (LEN_WRITE_ENABLE != 1u) || (LEN_WRITE_DISABLE != 1u)
#error read/write enable command length has changed
#endif
/**
 * Sets the write enable (WEN) bit in the EEPROM status register
 */
PRIVATE void eeprom_write_enable(void)
{
 
    eeprom_single_byte_command(CMD_WRITE_ENABLE);
}

/**
 * Clears the write enable (WEN) bit in the EEPROM status register
 * and then waits for EEPROM to become ready (because we have seen
 * EEPROM set /RDY bit briefly after this command)
 */
PRIVATE void eeprom_write_disable_wait_for_ready(void)
{
    eeprom_single_byte_command(CMD_WRITE_DISABLE);
    (void) wait_for_ready();
}


/**
 * Splits a write up into EEPROM page writes and calls eeprom_write_page()
 * to write each page
 */
PRIVATE Eeprom_Op_Status eeprom_write_pages(const uint16_t  start_address,
                                    FPTR const uint8_t * buffer_p,
                                         uint16_t  length)
{
    Eeprom_Op_Status write_status;
    
    /* address of first byte of page containing start_address */
    uint16_t first_page_address = (start_address & EEPROM_PAGE_ADDRESS_MASK);

    /* number of bytes to be written to first page */
    uint16_t first_page_bytes; 

    /* address of first byte of second page to be written */
    uint16_t second_page_address;

    /* address of first byte of page containing last byte to be written */
    uint16_t last_page_address = 
                ((start_address + (length - 1u)) & EEPROM_PAGE_ADDRESS_MASK);

    uint16_t last_page_bytes;   /* number of bytes to be written to last page */
    uint16_t bytes_written;     /* bytes written so far */
    uint16_t page_address;      /* address of current page used in for-loop */

    if (last_page_address == first_page_address)
    {
        /* single page write, so just write the lot in one go */
        write_status = eeprom_perform_write(start_address,buffer_p,length);
    }
    else
    {
        second_page_address = first_page_address + EEPROM_PAGE_LENGTH;
        first_page_bytes = second_page_address - start_address;

        /* write the fraction of the first page that's required */
        write_status = eeprom_perform_write(start_address,buffer_p,
                                                first_page_bytes);
        bytes_written = first_page_bytes;

        /* write all complete pages up to but excluding last page */
        for (page_address = second_page_address ;
                (write_status == Ee_driver_ready) 
                && (page_address < last_page_address) ;
                page_address += EEPROM_PAGE_LENGTH)
        {
            write_status = eeprom_perform_write(page_address,
                    &buffer_p[bytes_written],EEPROM_PAGE_LENGTH);
            bytes_written += EEPROM_PAGE_LENGTH;
        }

        /* and write the last page */
        last_page_bytes = (start_address + length) - last_page_address;

        if (write_status == Ee_driver_ready)
        {
            write_status = eeprom_perform_write(last_page_address,
                                        &buffer_p[bytes_written],
                                        last_page_bytes);
        }
    }
    return write_status;
}

/** Read a stream of data from the specified address of the EEPROM
 *
 *  No attempt is made to check that the buffer is large enough for the data,
 *  this is the responsibility of the calling routine
 *  @param address      Start address for the read
 *  @param buffer_p     Pointer to buffer in which to store read data
 *  @param length       Number of bytes to read
 *
 *  @todo Break up very large reads to protect against COP timeout.
 *  Assuming 2us per byte, a 30ms COP period allows for a max length of
 *  ~15kbytes before the COP expires.
 */

PRIVATE void eeprom_perform_read(uint16_t  address,
                         FPTR uint8_t * buffer_p,
                              uint16_t  length)
{
    kick_watchdog();
    eeprom_send_cmd_and_address(CMD_READ_DATA,address);
    eeprom_receive_data(buffer_p,length);
    kick_watchdog();
}

/**
 * Write a single page of data to the EEPROM.
 *
 * Arguments are not checked for validity: that must have been done by the
 * caller.
 * Sequence is:
 * + Service the COP
 * + wait for EEPROM to become ready
 * + Send Write enable
 * + Send Write command with first address
 * + Send the data
 * + wait to become ready again
 *
 * @param address      Start address for the write
 * @param buffer_p     Pointer to buffer containing data to write
 * @param length       Number of bytes to write
 *
 * @retval Ee_driver_ready      EEPROM ready for use again
 * @retval Ee_device_timeout    EEPROM timed out either before or after write
 */
PRIVATE Eeprom_Op_Status eeprom_perform_write(uint16_t  address,
                    FPTR const uint8_t * buffer_p,
                               uint16_t  length)
{
    Eeprom_Op_Status write_status;

    kick_watchdog();
    if (!wait_for_ready())
    {
        write_status = Ee_device_timeout;
    }
    else
    {
        eeprom_write_enable();
        eeprom_send_cmd_and_address(CMD_WRITE_DATA,address);
        eeprom_send_data(buffer_p,length);
        write_status = (wait_for_ready()) ? Ee_driver_ready : Ee_device_timeout;
    }
    return write_status;
}

/*
 * SPI interface functions
 *
 * These are protected by ifndef HOSTED so that they can be replaced by a hosted
 * simulator in HUTs
 */
#ifndef HOSTED
/**
 * Sends a one-byte command followed by a two-byte address and leaves
 * chip selected so that data can follow.
 *
 * @note this function assumes that address is stored big-endian so that
 * the cast of its address to a pointer has the correct byte order
 */
PRIVATE void eeprom_send_cmd_and_address(uint8_t command_byte,uint16_t address)
{
    /* spi_transfer_partial() leaves the chip selected on exit */

    /* send one command byte */
    spi_transfer_partial(Spi_EEPROM,FNULL,(FPTR uint8_t *) &command_byte, 1u);
    /* and two address bytes */
    spi_transfer_partial(Spi_EEPROM,FNULL,(FPTR uint8_t *) &address, 2u);
}

/**
 * Sends a stream of data to the EEPROM and ignores what comes back
 */
PRIVATE void eeprom_send_data(FPTR const uint8_t *buffer_p, uint16_t length)
{
    spi_transfer(Spi_EEPROM, FNULL, buffer_p, length);
}

/**
 * Receives a stream of data from the EEPROM and ignores what comes back
 */
PRIVATE void eeprom_receive_data(FPTR uint8_t *buffer_p, uint16_t length)
{
    spi_transfer(Spi_EEPROM, buffer_p, FNULL, length);
}

/**
 * Transmits a single byte to the EEPROM and ignores the byte coming back
 */
PRIVATE void eeprom_single_byte_command(uint8_t command)
{
    spi_transfer(Spi_EEPROM, FNULL, (FPTR uint8_t*)&command, 1u);
}
/**
 * Transmits two bytes to the EEPROM and returns the second byte received.
 *
 * This command is for reading and writing the status register.  When writing,
 * the return value will be ignored; when reading, the EEPROM will ignore
 * the transmitted byte when returning the status register value.
 * 
 * @param command       First byte to be transmitted
 * @param tx_data       Second byte to be transmitted
 * @returns             Data returned in the byte following the command
 */
PRIVATE uint8_t eeprom_two_byte_command(uint8_t command,uint8_t tx_data)
{
    uint8_t tx_stream[2];
    uint8_t rx_stream[2];
    tx_stream[0] = command;
    tx_stream[1] = tx_data;
    spi_transfer(Spi_EEPROM, (FPTR uint8_t *)rx_stream,
            (FPTR uint8_t*)tx_stream, 2u);
    return rx_stream[1];
}

#endif


