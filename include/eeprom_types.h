/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  eeprom_types.h
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
#if !defined( EEPROM_TYPES_H )
#define       EEPROM_TYPES_H
/**
 * @file
 * @ingroup EEPROM
 *
 * Header file for EEPROM Driver control
 *
 */

/**
 * A list of Eeprom device return values indicating the eeprom device status.
 * Used as return values for eeprom_status(), eeprom_init(),
 * eeprom_write_data() and eeprom_read_data().
 *
 * @note Any changes must be mirrorred in Eeprom_Op_Status_vtab
 */
typedef enum
{
    Ee_driver_uninitialised, /**< eeprom_init() hasn't been called yet */
    Ee_driver_ready,/**< When the eeprom has successfully completed its current task
                and is available for Next read / write operation.*/
    Ee_driver_busy,/**< When the eeprom is busy executing the
                 read / write operation.*/
    Ee_device_error, /**< The hardware is not behaving correctly */
    Ee_device_timeout, /**< A write took too long to complete */
    Ee_arg_buffer_error,/**< When the destination pointer to which
                         data is to be written is a NULL pointer.*/
    Ee_arg_address_error/**< When a write/read request crosses the 16k eeprom
                            boundary address of 0x3fff */

} Eeprom_Op_Status;

#endif /* EEPROM_TYPES_H */
