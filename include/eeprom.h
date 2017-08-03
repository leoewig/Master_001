/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  eeprom.h
 *
 * description:  
 * Header file for EEPROM Driver control
 * 
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
/**
 * @file
 *
 * Header file for EEPROM Driver control
 */
 /**
 * EEPROM Driver Interface
 *
 * @par Requirements:
 * There is no requirement document currently available. The design is carried out
 * based on the understanding of the architecture.
 *
 * @par A brief introduction to the EEPROM device AT25128A:
 * The AT25128A provides 131,072 bits of serial electrically-erasable programmable
 * read-only memory (EEPROM) organized as 16,384 words of 8 bits each. It is
 * enabled through the Chip Select pin (CS) and accessed via a three-wire
 * interface consisting of Serial Data Input (SI), Serial Data Output (SO), and
 * Serial Clock (SCK). All programming cycles are completely self-timed, and
 * no separate erase cycle is required before write.
 *
 * @note The AT25128A/256A is capable of a 64-byte page write operation.
 * After each byte of data is received, the six low-order address bits are
 * internally incremented by one; the high order bits of the address will remain
 * constant. If more than 64 bytes of data are transmitted, the address counter
 * will roll over and the previously written data will be overwritten.
 *
 * @note Before any attempt can be made to read or write to the EEPROM, the EEPROM
 * must have indicated, via the status register, that it has completed all previous
 * write operations. If the EEPROM device does not indicate that it is ready to
 * perform the next operation in an acceptable time (#ee_device_timeout_us), then the
 * read/write routines will exit and indicate that a device timeout error has occurred.
 *
 * @par Preconditions:
 * Before using the EEPROM interface:
 * - the SPI drivers must be initialised
 * - the TS feature must be initialised
 * - eeprom_init() must be called
 *
 * @par Approach Used:
 * The EEPROM driver is initialised by calling eeprom_init(), which will return either
 * @ref Ee_driver_ready or #Ee_device_error.  If eeprom_init() returns #Ee_device_error, then a
 * problem was detected while communicating with the EEPROM and all subsequent calls to
 * eeprom_read_data() or eeprom_write_data() will fail and return Ee_device_error.
 *
 * The EEPROM driver module provides interfaces to read/write data from/to the EEPROM.
 * The reading of the EEPROM data is achieved through the interface eeprom_read_data().
 * and the writing of data to the EEPROM is achieved through eeprom_write_data().
 * @note These functions can be called from different tasks.
 *
 * @par Read Operation:
 * To read from the EEPROM, the interface available is eeprom_read_data(). This
 * function waits for the EEPROM device to complete any previous write operations and then
 * performs some basic sanity checks before initiating the read sequence.
 * The SPI driver interfaces are used to perform the necessary read sequence,
 * by pulling the CS line low, sending the Read-Opcode followed by the address
 * from where the data has to be read.
 *
 * @par Write Operation:
 * To write to the EEPROM, the interface available is eeprom_write_data(). This
 * function waits for the EEPROM device to complete any previous write operations and then
 * performs some basic sanity check before initiating the write sequence.
 * The SPI driver interfaces are used to perform the necessary write sequence,
 * like write enabling the device via the WREN instruction, pulling the CS line
 * low, sending the Write-Opcode followed by the address and the data to be
 * programmed. The eeprom_write_data also tracks the number of bytes written, computes
 * the data transfer length based on the write address location and the remaining
 * numberof bytes to be written.
 *
 * @par EEPROM device status:
 * eeprom_status() will return the current state of the driver, which will be one of:
 * + Ee_driver_uninitialised if eeprom_init() has not been called
 * + Ee_driver_ready
 * + Ee_driver_busy indicating that a lower-priority task is using the EEPROM
 * + Ee_device_error if intialisation failed
 *
 * @par Task safety
 * The EEPROM driver can be used from different tasks, however, attempts to read or write
 * data while a lower priority task is accessing the EEPROM will result in the call
 * to eeprom_read_data() or eeprom_write_data() returning Ee_driver_busy.
 * 
 * @defgroup EEPROM EEPROM Driver
 * @ingroup platform
 * @{
 * @file
 * @}
*/

#if !defined( EEPROM_H )
#define       EEPROM_H

#include "basic_types.h"
#include "eeprom_types.h"

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/** The eeprom_read_data() function provides an interface to read the data from the
 * EEPROM. It waits for the EEPROM device to complete any previous write operations and then
 * checks for the validity of the buffer pointer, where the data is
 * to be stored, and expects the user to provide a sufficiently large buffer so as
 * to avoid corrupting of data by the pointer. Before initiating a read operation
 * it also checks if the requested read operation would cross the EEPROM address
 * boundary of 0x3fff(16k). If the buffer pointer is NULL or if the read would
 * cross the address boundary then no attempt to read the data from EEPROM is made.
 * However if these checks are passed, a read operation is then initiated.
 * Internally, it makes use of the SPI driver interfaces to read the data over the SPI.
 *
 * @param address is the EEPROM device memory address from where the
 * data is to be read
 * @param buffer_p is the pointer to the destination buffer to where the read
 * data is to be stored.
 * @param length is the no.of data bytes to be read from the eeprom.
 *
 * @note The User MUST ensure that the buffer where the data is to be stored is
 * sufficiently large to store the requested data bytes and the requested read
 * address should not cross the device address boundary.
 * This function must only be called from a single task (except during
 * system initialization).
 *
 * @retval Ee_driver_uninitialised      eeprom_init() has not been called
 * @retval Ee_device_error              eeprom_init() was unsuccessful
 * @retval Ee_arg_address_error The requested range of data does not all lie in the EEPROM
 *                              or the requested length was zero
 * @retval Ee_arg_buffer_error  buffer_p is NULL
 * @retval Ee_driver_busy       A lower priority task is using the EEPROM
 * @retval Ee_device_timeout    A timeout occured waiting for the EEPROM device to accept the
 *                              command
 * @retval Ee_driver_ready      The operation was a success
 */
Eeprom_Op_Status eeprom_read_data(const uint16_t  address,
                                         FPTR uint8_t * buffer_p,
                                        uint16_t  length);

/** The eeprom_write_data() function provides an interface to write the data into the
 * EEPROM device. Before writing the data to the requested location it waits for the
 * EEPROM device to complete any previous write operations and then checks
 * whether the requested location is write protected or not and if the write operation
 * would cross the eeprom address boundary of 0x3fff(16k). If it is write
 * protected or if the write would cross the address boundary then no attempt to
 * write the data to the eeprom is made. However if these checks are passed, a
 * write operation is then initiated. Internally it uses the SPI driver
 * interfaces to write the data over the SPI.
 * Since the eeprom writing task is a low priority task, it is preemptible by
 * other high priority tasks. Once all the high priority tasks are over and it gains
 * back the control, the writing is resumed for as long as it is not preempted or
 * until the writing is completed.
 *
 * @note The device is capable of a 64-byte page write operation and is
 * automatically returned to the write disable state at the completion of a write
 * cycle. However, the user can still request any length of data to be written as
 * long as the address location where the data is to be written doesn't crosses
 * the boundary address(0x3FFF). Internally this function takes care of performing
 * necessary actions to initiate a multi-page write and thereby servicing the
 * request for writing large amount (greater than 64Bytes) of data.
 * This function must only be called from a single task (except during
 * system initialization).
 *
 * @param address is the EEPROM device memory address location to where
 * the data is to be written. For multiple bytes writing this is the start address.
 * @param buffer_p is the pointer to the source buffer which contains the data
 * to be written into the eeprom.
 * @param length is the no.of data bytes to write to the eeprom.
 *
 * @retval Ee_driver_uninitialised      eeprom_init() has not been called
 * @retval Ee_device_error              eeprom_init() was unsuccessful
 * @retval Ee_arg_address_error The requested range of data does not all lie in the EEPROM
 *                              or the requested length was zero
 * @retval Ee_arg_buffer_error  buffer_p is NULL
 * @retval Ee_driver_busy       A lower priority task is using the EEPROM
 * @retval Ee_device_timeout    A timeout occured waiting for the EEPROM device to accept or
 *                              perform the command
 * @retval Ee_driver_ready      The operation was a success.
 *
 * @note if Ee_device_timeout is returned, none, some or all of the data may have been written
 * to the EEPROM
 */
Eeprom_Op_Status eeprom_write_data(const uint16_t  address,
                                    FPTR const uint8_t * buffer_p,
                                    uint16_t  length);

/** Probe the eeprom driver status.
 *
 * @retval Ee_driver_uninitialised      eprom_init() has not been called
 * @retval Ee_driver_ready              The EEPROM is available
 * @retval Ee_driver_busy               A lower priority task is accessing the EEPROM
 */
Eeprom_Op_Status eeprom_status(void);

/**
 * Initialise the EEPROM driver
 *
 * This function must be called after spi_init() is called and before any calls to
 * eeprom_read_data() or eeprom_write_data().  If the EEPROM is found to have protected
 * areas of memory, this protection is cleared.
 *
 * It checks communication with the EEPROM by manipulating its status register.
 *
 * @retval Ee_device_error      Communication could not be established
 * @retval Ee_driver_ready      The EEPROM is ready for use
 */
Eeprom_Op_Status eeprom_init(void);

#endif
