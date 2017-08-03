/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    cal.h
 *
 * description:  
 * XCP calibration header file
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
#ifndef CAL_H
#define CAL_H
/**
 * XCP calibration header file
 *
 * @file
 * @ingroup cal
 *
 */
/* Copyright (c) Linkedriving Tech 2015 - All rights resevered */
/**
 * XCP calibration support
 *
 * # Overview
 * This module supports calibration by CANape over XCP.  It is intended to
 * work with both the CANbedded and IXCP XCP implementations.
 *
 * # Initialisation
 * The calibration areas are defined as regular initialised RAM, allowing the
 * base calibration to be stored in the regular static data intialisation
 * table.  At start up, the base calibration is copied from flash to RAM.
 *
 * The calibration intialisation fuinction, cal_init(), inspects the EEPROM for
 * a stored calibration.  If one is present and is found to be compatible with
 * the strategy, it is copied out of the EEPROM and over the base calibration.
 *
 * # Calibration from CANape
 * The implementation uses the calibration page paradigm: the base calibration
 * in flash is bank 0 and the RAM calibration is bank 1.
 *
 * All writes from CANape are stored in EEPROM, making them non-volatile.  The
 * first time a write is performed on a module, the entire base calibration is
 * written to EEPROM.  Subsequent writes only require the specifc data to
 * be written.
 *
 * All writes are non-volatile because CANape does not currently support the
 * STORE_CAL_REQ option.
 *
 * # Resetting to the base calibration
 * A stored calibration is discarded by selecting "Initialize Calibration RAM"
 * in CANape.
 *
 * Reflashing a module (whether over CAN or via the debugger) with an
 * incompatible strategy or a changed calibration will also result in any
 * stored calibration being discarded.
 *
 * # Implementation
 * Initialisation is performed by cal_init().  This must be called after both 
 * the EEPROM and nvs features have been initialised.
 *
 * Four entry points are provided for the XCP feature(s):
 * + cal_get_cal_page()
 * + cal_set_cal_page()
 * + cal_copy_flash_cal()
 * + cal_write()
 *
 * Since the EEPROM is too slow for writes to occur in the foreground, none
 * of the above functions will actually write the EEPROM, instead they will
 * leave the requested write to a background function cal_idle_tasks().
 *
 * cal_idle_tasks() must be called from the idle loop.
 *
 * @defgroup cal CAL Calibration support
 */
#include "cal_types.h"

extern FRAM cal_nvs_wrapper_s cal_ram_image;

void cal_init(void);
void cal_idle_tasks(void);
void cal_init_failure_management(void);

cal_page_e cal_get_cal_page(void);
cal_status_e cal_set_cal_page(cal_xcp_slave_e slave,cal_page_e page);
cal_status_e cal_copy_flash_cal(cal_xcp_slave_e slave);
cal_status_e cal_write(cal_xcp_slave_e slave,FPTR uint8_t *dest_address,
                        uint8_t size, FPTR const uint8_t *src_p);

/*
 * Calibration ID reported over UDS
 */
extern CAL uint16_t calibration_version;
extern CAL char_t calibration_id[CALIBRATION_ID_LENGTH+1u];

#endif /* #ifndef CAL_H */
