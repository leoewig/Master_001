/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   i2c.h
 *
 * description:  Header file for low-level I2C control
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
 * Header file for low-level I2C control
 *
 */
 /**
 * I2C Driver Interface
 *
 * @par Requirements:
 * A requirement document is not available, but the module is a carry
 * over from the BATMAN project.
 *
 * @par Brief Introduction about I2C Bus:
 * The inter-IC bus (IIC) is a two-wire, bidirectional serial bus that provides
 * a simple, efficient method of data exchange between devices.
 * A device that sends data onto the bus is defined as a transmitter and a
 * device receiving data as a receiver. The device that controls the message is
 * called a master (CPU in this case). The devices that are controlled by the
 * master are referred to as slaves (RTC DS1339 in this case). The bus is
 * controlled by the CPU. It generates the serial clock (SCL), controls the bus
 * access, and generates the START and STOP conditions. This module provides the
 * interfaces required to communicate with RTC over the I2C bus.
 *
 * @par Approach used:
 * The BMS project has only one node( RTC ) as the user of the I2C services. The
 * current interfaces could therefore be simplified (Design review comment).
 * The design is made simpler and yet is flexible enough to cater to future changes.
 * The design caters only ONE I2C user/node( RTC ). This is reflected
 * by the enumerator element I2C_MAX_DEV in the enum I2CDeviceIndex_t. Any
 * addition to the no.of I2C nodes would involve inserting the new enum element
 * before the I2C_MAX_DEV in the enum I2CDeviceIndex_t.
 *
 * @par Precondition:
 * The interrupt handler I2C_Interrupt() must be assigned to the I2C interrupt
 * vector address before the I2C module can be used.
 *
 * @par Initialization:
 * To use the I2C driver, First and the foremost step is to initialize the driver
 * with the required configuration parameters. Refer the I2C_Initialise()
 * declaration for details.
 *
 * @par Handshake:
 * When the bus is free a master initiates communication by sending a START
 * signal. IBB flag in the IBSR register is polled to confirm the START signal
 * is detected. This signal denotes the begining of a new data transfer and brings
 * all the slaves out of their idle states. Following the START signal, the first
 * data byte transmitted by the master is the slave address. The slave address is
 * written into the IBDR register and the IBB flag is polled to confirm the
 * address is transmitted. This is a seven-bit calling address followed
 * by a R/!W bit. The R/!W bit tells the slave the desired direction of data
 * transfer. Only the slave with a calling address that matches the transmitted
 * address responds by sending back an acknowledge bit.
 *
 * @par Data Transfer:
 * A reception of an acknowledge after the START signal, indicates that the slave
 * is ready to respond to the master's request. Now the master sends out the data
 * to be transferred byte-by-byte after receiving an acknowledge bit for each byte
 * transmitted, thus indicating the successful reception of data by the slave.
 * If the slave doesn't sends an acknowledge to the master, the master can then
 * generate a stop signal to abort the data transfer or a start signal
 *(repeated start) to commence a new calling.
 *
 * When a data byte transfer is completed an I2C interrupt is triggered
 * (indicated by the TCF bit in the IBSR register). Based on the direction of
 * data transfer appropriate function gets called. Correspondingly a data byte
 * is either written into the IBDR register or read from the IBDR register.
 * The interfaces available for Transmitting and Receiving the data are
 * I2C_Transmit() and I2C_Receive() respectively.
 * The I2C status needs to be checked before initiating any further read or write
 * operation. The function I2C_OpComplete() provides this mechanism.
 * If no more data is to be transferred / received a STOP signal is generated to
 * indicate the end of the current data transfer.
 *
 * @par Limitation:
 * The i2c functions I2C_OpComplete(), I2C_Transmit() & I2C_Receive() should all
 * be called from a single task, and not from multiple tasks, as this module
 * is designed to service only a single node/user.
 *
 * @defgroup I2C I2C Driver
 * @ingroup platform
 * @{
 * @file
 * @}
 */
#if !defined( I2C_H )
#define       I2C_H

#include "hardware.h"
#include "basic_types.h"
#include "i2c_types.h"

/** This function registers the I2C nodes and internally maintains a master list
 * of all the nodes with their configuration parameters.
 * 
 *   @param reg_node contains the configuration parameters of the requesting I2C node
 *   @return flag indicating success or failure of initialization */
I2C_ProgressType I2C_Initialise(I2CDevice_t reg_node);

void I2C_ResetBusCycle(void);

/** Transmit specified data to specified device.
 *  Data is not buffered during the duration of the operation, hence the data
 *  pointed to by p_data must remain constant until I2COpComplete().
 *
 *  @param p_device_index indicates the user of the I2C services. Internally it is
 *  used to identify the device type and the device specific information.
 *  @param p_data points to the data which is to be transmitted.
 *  @param p_len contains the length of the data to be transmitted (in bytes)
 *
 *  @retval I2C_OK  When the transmission was successfully initiated.
 *  @retval I2C_OP_IN_PROGRESS denotes some operation going on in the I2C bus
 *           and is busy @ the moment.
 *  @retval I2C_INVALID_DEVICE denotes an invalid device number.*/
FCODE I2C_ProgressType I2C_Transmit( I2CDeviceIndex_t       p_device_index,
                             FPTR const uint8_t*    p_data,
                             const size_t           p_len );

/** Received some data from the specified device
 *  Data is received asynchronously.  The caller should poll I2COpComplete() to
 *  determine when the buffer contains the received data.
 *
 *  @param p_device_index indicates the user of the I2C services. Internally it is
 *  used to identify the device type and the device specific information.
 *  @param p_rcv_data points to the buffer where the data is to be stored.
 *  @param p_len contains the length of the data to receive (in bytes).
 *
 *  @retval I2C_OK  When the reception was successfully initiated.
 *  @retval I2C_OP_IN_PROGRESS denotes some operation going on in the I2C bus
 *           and is busy @ the moment.
 *  @retval I2C_INVALID_DEVICE denotes an invalid device number. */
FCODE I2C_ProgressType I2C_Receive( I2CDeviceIndex_t       p_device_index,
                             FPTR uint8_t*         p_rcv_data,
                            const size_t           p_len );

/** Check to see if a previously initiated read/write operation is still in progress or not
 * @retval I2c_not_complete When the initiated I2C read/write operation is not completed.
 * @retval I2c_success When the read/write I2C operation is completed successfully.
 * @retval I2c_failure When a communication failure occurs.*/
FCODE I2C_ProgressType I2C_OpComplete( void );

/** Interrupt service routine - Only master mode implemented */
extern INTERRUPT void I2C_Interrupt( void );

#endif
