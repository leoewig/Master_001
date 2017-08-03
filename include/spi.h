/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   spi.h 
 *
 * description:  
 * Declarations required by both SPI masters and SPI slaves
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
/**
 * @file
 * @ingroup spi
 *
 * Declarations required by both SPI masters and SPI slaves
 *
 */
/**
 * Serial Peripheral Interface
 *
 * The SPI driver consists of a common initialisation function and separate
 * submodules for SPI masters and SPI slaves.
 *
 * A bit about SPI
 * ---------------
 *
 * SPI is a 4-wire full-duplex master/slave protocol. The bus master selects
 * a slave, by driving a slave-select pin low, and drives a clock line to
 * transfer data.  Data is always read and written - on separate wires -
 * simultaneously.  At the end of the transfer, the slave-select goes high to
 * tell the slave that the transmission is complete.
 *
 * The S12 has a no-frills SPI controller: each transfer is only 8 or 16-bits
 * (typically 8) and there is very little buffering.  The lack of buffering
 * drives the design because transfers are too fast (5MHz is typical) to take
 * interrupts for each one.
 *
 * The SPI can be configured to drive a slave select (SS) automatically but for
 * some peripherals, such as the EEPROM, this is not suitable because the
 * peripheral requires multi-byte transfers with SS used to indicate End of
 * Message.
 *
 * The SPI can be configured to use just one data line (bidirectional pin
 * mode) leaving the other one for general IO purpose. In such a case SPI
 * becomes half-duplex. If SPI is configured to the master mode,
 * only MOSI is used. If SPI is configured to the slave mode, only MISO is
 * used.
 *
 * Design goals
 * ------------
 *
 * Goals are:
 * - Provide a flexible means of configuring each SPI channel, including:
 * 	+ Master or slave
 * 	+ Bus speed
 * 	+ 8/16 bit transfers
 * 	+ Pins used (via MODRR register)
 * 	+ Manual or automatic control of slave select
 * - Support full-duplex transfers
 * - Support large data transfers, e.g. read entire EEPROM
 * - Be suitable for S12 or XGATE core
 * - Have a low interrupt overhead
 * - Support multiple targets with different SPI peripherals.  Principally
 * the MMC and the test prodder.
 *
 * Requirements
 * ------------
 *
 * The SPI driver does not have an explicit requriements list but was developed
 * to satisfy the above Design goals.
 *
 * Configuration
 * -------------
 *
 * Each target must define a SPI configuration header and source file.  The
 * build system copies the target-specific files to the build directory,
 * renaming them spi_cfg.h and spi_cfg.c.  For the main application, these
 * files are called mmc_spi_cfg.h and mmc_spi_cfg.c.
 *
 * The header cfg file (e.g. mmc_spi_cfg.h) contains the #spi_channel_e
 * enumeration which give the application a handle to each SPI controller.
 *
 * The source cfg file (e.g. mmc_spi_cfg.c) contains the #spi_cfg array
 * of #spi_cfg_s structures which define the configuration for each controller.
 *
 * Slave select configuration (bus masters only)
 * ---------------------------------------------
 *
 * If possible, slave select (SS) control should be left to the SPI hardware
 * by setting spi_cfg::ss to #Spi_ss_auto.  If this is not possible, because the
 * peripheral requires SS to be held low for an entire multi-byte transfer,
 * then set spi_cfg::ss to #Spi_ss_explicit.  In this case, the SPI driver
 * will drive SS explicitly as a digital output.
 *
 * Integration
 * -----------
 *
 * The application gets access to the driver by including spi.h
 *
 * Initialisation
 * --------------
 *
 * Before any SPI transfers can be made, the application must call spi_init()
 * which configures each controller according to the information in #spi_cfg.
 *
 * For each controller configured with spi_cfg[].ss = Spi_ss_explicit, the
 * SS port will be set up as a digital output and the line will be set high.
 * Port manipulation is performed with interrupts disabled to ensure port
 * read-modify-writes do not interfere with any pre-empting tasks.
 *
 * spi_init() must be called from the S12 only (because of the need to disable
 * interrupts around port manipulation)
 *
 * Data transfers
 * --------------
 *
 * Reads and writes are both performed by calling spi_transfer() and
 * spi_transfer_partial() from the S12 or xspi_transfer() and
 * xspi_transfer_partial() from the XGATE.  To allow multi-byte transfers to be
 * handled, the functions take data pointers and a byte count.
 *
 * If the controller is configured with spi_cfg[].ss = #Spi_ss_explicit,
 * spi_transfer() (xspi_transfer()) will drive the SS port low before the first
 * byte and high after the last one.  spi_transfer_partial()
 * (xspi_transfer_partial()) drives the SS port low at the start but leaves it
 * low at the end of the transfer to allow subsequent calls to be made to
 * continue the transfer.  Interrupts are disabled _on the current core_
 * for driving the SS to ensure read-modify-writes do not interfere with any
 * pre-empting tasks.
 *
 * Since interrupts are only disabled on one core, interference from the other
 * must not be possible by design (see below).
 *
 * Multi-node bus support
 * ----------------------
 *
 * Multi-node buses are supported by the spi_enable() and spi_disable()
 * functions which enable or disable a spi controller.  When disabled, a node's
 * slave-select port will be set as a digital input.
 *
 * Data integrity
 * --------------
 *
 * spi_transfer() and xspi_transfer() can run simultaneously and can be
 * re-entered provided all active calls are for different controllers.
 *
 * If using a controller configured with spi_cfg[].ss = Spi_ss_explicit, great
 * care must be taken to avoid corrupting the state of the port containing SS
 * by making sure that all writes to the port are from a single core.
 *
 * @defgroup spi SPI Serial Peripheral Interface Driver
 * @ingroup platform
 */

#ifndef SPI_H
#define SPI_H

#include "basic_types.h"
#include "spi_types.h"

/*
 * include bus configuration here so that calling code needs only this header
 */
#include "spi_cfg.h"

#ifndef _XGATE_
/**
 * Initialise all used SPI ports
 *
 * Common configuration
 * --------------------
 * All ports will be configured with the following:
 * - LSB-first disabled (so all transfers are MS bit first)
 * - MISO/MOSI both used (bidrectional mode is disabled)
 * - clock generation continues in wait mode (the default behaviour)
 * - SPI controller enabled
 *
 * Per-port configuration
 * ----------------------
 * The following are configured for each port according to #spi_cfg:
 * - master or slave
 * - processor pins used
 * - baud rate
 * - clock phase and polarity
 * - transfer width (8 or 16 bits)
 * - control of slave-select for bus masters (however, mode fault detection
 *	is not supported)
 */
void spi_init(void);

/**
 * Disables a SPI controller
 *
 * @param channel       Channel to be disabled
 *
 * This function will disable a controller that has been previously initialised
 * by spi_init() or spi_enable().
 *
 * Does the following:
 * 1. Disables the SPI controller (clears SPIxCR1.SPE)
 * 2. Atomically sets the slave-select as a digital input.
 * @note It should not be called while the SPI channel is transferring data.
 */
void spi_disable(enum spi_channel_e channel);

/**
 * Enables SPI controller
 *
 * @param channel       Channel to be enabled
 *
 * This function will enable a SPI controller that has previously been disabled
 * by spi_disable().
 *
 * Does the following:
 * 1. If the channel uses explicit slave-select control, the digital port will
 *    be set high (i.e. chip _not_ selected) and configured as an output.
 * 2. The SPI controller is enabled (SPIxCR1.SPE is set).
 *
 * @note It should not be called while the SPI channel is transferring data.
 */
void spi_enable(enum spi_channel_e channel);
#endif /* ifndef _XGATE_ */


/**
 * Read and Write data on the bus
 *
 * Transfers the requested number of bytes on the SPI bus.  Before calling this
 * function, the bus must have been configured as a master in the SPI
 * configuration file (probably mmc_spi_cfg.c) and spi_init() must have been
 * called.
 * For non-duplex transfers, either the read buffer or the write buffer can
 * be set to NULL.
 * If the bus has been configured with explicit slave select (SS) control, SS
 * will be brought low at the start of the transfer, remain low for the
 * duration, and go high at the end.
 *
 * @param channel	SPI bus to be used
 * @param[out] readp
 *                      Global pointer to start of buffer to store data read
 *                      from the SPI bus.  If NULL, data read will be
 *                      discarded.  If not NULL, readp must point to a buffer
 *                      of at least nbytes in length.  readp must be
 *                      word-aligned if reading from a 16-bit bus on the XGATE.
 * @param[in] writep    Global pointer to start of buffer containing data to be
 *                      written to the SPI bus.  If NULL, no new data will be
 *                      written (so last transmission will be repeated).  If
 *                      not NULL, writep must point to at least nbytes of data
 *                      to transmit.  writep must be word-aligned if writing
 *                      to a 16-bit bus on the XGATE.
 * @param nbytes	Number of bytes to transfer.  If zero, no bytes will
 * 			be transferred.
 * 			If nbytes is odd and the bus has been configured for 16
 * 			bit transfers, an extra byte will be transferred. '0'
 * 			will be transmitted as the final byte and the final
 * 			byte received will be discarded.
 *
 * The function blocks while the transfer is carried out so it is not suitable
 * for transferring a large amount of data within a CPU interrupt or XGATE
 * high-priority thread.
 */

#if defined _XGATE_
void xspi_transfer(enum spi_channel_e channel, uint8_t *readp,
		    const uint8_t *writep, uint16_t nbytes);
#else
void spi_transfer(enum spi_channel_e channel, FPTR uint8_t *readp,
		    FPTR const uint8_t *writep, uint16_t nbytes);
#endif


/**
 * Incomplete SPI transfer
 *
 * All paramters are identical to spi_transfer()
 *
 * Operation is identical to spi_transfer() except that the slave remains
 * selected when the function returns to allow subsequent calls to this
 * function (and, finally, spi_transfer) to be made as part of the same
 * data transfer.  This allows, for example, a read to be followed by a write.
 *
 * It is an error to call this function for a master configured with auto
 * slave select control (i.e. spi_cfg::ss set to #Spi_ss_auto).
 */

#if defined _XGATE_
void xspi_transfer_partial(enum spi_channel_e channel, uint8_t *readp,
		    const uint8_t *writep, uint16_t nbytes);
#else
void spi_transfer_partial(enum spi_channel_e channel, FPTR uint8_t *readp,
		    FPTR const uint8_t *writep, uint16_t nbytes);
#endif
#endif /* SPI_H */
