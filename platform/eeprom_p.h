#if !defined(EEPROM_P_H)
#define      EEPROM_P_H
/**
 *
 * EEPROM driver services private header
 *
 *
 * @file
 * @ingroup EEPROM
 * $Id: eeprom_p.h 5452 2012-11-30 14:56:10Z gvs $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */
PRIVATE Eeprom_Op_Status eeprom_write_pages(const uint16_t  start_address,
                                    FPTR const uint8_t * buffer_p,
                                    uint16_t length);
PRIVATE  uint8_t read_status_reg(void);
PRIVATE  bool_t is_addr_range_valid(uint16_t addr, uint16_t len);
PRIVATE bool_t wait_for_ready(void);
PRIVATE void eeprom_clear_all_protection(void);
PRIVATE bool_t eeprom_is_write_enabled(void);
PRIVATE void eeprom_write_enable(void);
PRIVATE void eeprom_write_disable_wait_for_ready(void);
PRIVATE void eeprom_perform_read(uint16_t  address, FPTR uint8_t * buffer_p,
                                    uint16_t  length);
PRIVATE Eeprom_Op_Status eeprom_perform_write(uint16_t  address,
                                    FPTR const uint8_t * buffer_p,
                                    uint16_t  length);
PRIVATE void eeprom_send_cmd_and_address(uint8_t command_byte,uint16_t address);
PRIVATE void eeprom_send_data(FPTR const uint8_t *buffer_p, uint16_t length);
PRIVATE void eeprom_receive_data(FPTR uint8_t *buffer_p, uint16_t length);
PRIVATE void eeprom_single_byte_command(uint8_t command);
PRIVATE uint8_t eeprom_two_byte_command(uint8_t command,uint8_t tx_data);

#ifdef UNIT_TEST
void eeprom_set_status(Eeprom_Op_Status new_status);
#endif

#endif      /* EEPROM_P_H */
