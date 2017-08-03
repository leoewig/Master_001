/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   cal_p.h
 *
 * description:  
 *  XCP calibration private header file
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
#ifndef CAL_P_H
#define CAL_P_H
/**
 * XCP calibration private header file
 *
 * @file
 * @ingroup cal
 *
 */
/* Copyright (c) Linkedriving Tech 2015 - All rights resevered */

PRIVATE bool_t cal_is_eeprom_cal_current(void);
PRIVATE cal_status_e cal_load_cal_from_eeprom(void);
PRIVATE void cal_discard_eeprom(void);
PRIVATE cal_status_e cal_find_init_locations(
                                FPTR const uint8_t *init_locations_p[]);
PRIVATE uint16_t cal_checksum(FPTR const uint8_t *p, uint16_t length);
PRIVATE cal_status_e cal_reload_flash_cal(void);
PRIVATE cal_status_e cal_address_in_range(FPTR const uint8_t *cal_p,
                                                         uint8_t size);
PRIVATE void cal_write_calram(cal_xcp_slave_e slave, FPTR const uint8_t *src_p,
                FPTR uint8_t *dest_p, uint8_t size,uint16_t eeprom_address);
PRIVATE cal_status_e cal_load_cal_from_eeprom(void);
PRIVATE cal_status_e cal_write_whole_cal(void);
PRIVATE cal_status_e cal_write_eeprom_bytes(FPTR const uint8_t *src_p,
                                                uint16_t eeprom_address,
                                                uint8_t size);
PRIVATE void cal_send_dto(cal_xcp_slave_e slave,cal_status_e status);
PRIVATE cal_status_e cal_initialise_nvs_data(void);
PRIVATE cal_status_e cal_find_eeprom_address(uint16_t *address,
                                                FPTR const uint8_t *cal_p,
                                                uint8_t size);
PRIVATE void cal_send_canbedded_dto(cal_status_e status);
PRIVATE void cal_send_ixcp_dto(cal_status_e status);

/*
 * initialisation table parsing functions in cal_cosmic.c
 */
FPTR const uint8_t * cosmic_init_flash_address(
                                        FPTR const uint8_t *ram_address);

PRIVATE uint16_t cosmic_init_flash_start(void);
PRIVATE FPTR const uint8_t 
                        *cosmic_init_flash_first_data_p(uint16_t first_offset);
PRIVATE FPTR const cosmic_init_entry_s *cosmic_init_first_entry(void);
PRIVATE bool_t cosmic_init_not_at_end(FPTR const cosmic_init_entry_s *entry_p);
PRIVATE bool_t cosmic_init_entry_is_paged(
                                FPTR const cosmic_init_entry_s *entry_p);
PRIVATE FPTR uint8_t *cosmic_init_entry_ram_address(
                        FPTR const cosmic_init_entry_s *entry_p);
PRIVATE uint16_t cosmic_init_entry_flash_end(
                        FPTR const cosmic_init_entry_s *entry_p);



#endif /*#ifndef CAL_P_H */
