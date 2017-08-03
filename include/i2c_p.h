/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   i2c_p.h
 *
 * description:  Header file for test access to PRIVATE functions in I2C
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
 * @ingroup platform
 *
 * Header file for test access to PRIVATE functions in I2C
 *
 */

#if !defined( I2C_P_H )
#define       I2C_P_H

/** Initialize setup for I2C module */
PRIVATE FCODE void I2C_Setup( const I2CDeviceIndex_t p_device_index );
/** Initialize I2C transfer (transmit or receive) */
PRIVATE FCODE I2C_ProgressType I2C_InitTransfer(void);
/** Transmit data on I2C bus */
PRIVATE FCODE void I2C_Tx(void);
/** Transmit data */
PRIVATE FCODE void I2C_TxData(void);
/** Receive data from I2C bus */
PRIVATE FCODE void I2C_Rx(void);

#if defined HOSTED_UNIT_TEST
extern I2C_ProgressType       m_op_progress;
#endif

#endif  /* I2C_P_H */
