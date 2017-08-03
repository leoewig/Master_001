/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      flash_prog_p.h
 *
 * description:    Private headers for Flash Programming
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li     Private headers for Flash Programming
 *
 *****************************************************************************/
#ifndef FLASH_PROG_P_H
#define FLASH_PROG_P_H


PRIVATE flash_location_e flash_location(uint32_t start,uint32_t len);
PRIVATE uint8_t flash_clear_multiple_sectors(uint32_t start,uint32_t len,
                                  const flash_parameters_s *flash_paramp);
PRIVATE uint8_t flash_clear_sector(uint32_t address,uint16_t clear_command);
PRIVATE uint8_t flash_program_multiple(uint32_t address,uint8_t len,
                                const uint8_t *data,uint16_t program_command);
PRIVATE uint8_t flash_program_byte(uint32_t address,uint8_t byte,
                                    uint16_t program_command);
PRIVATE uint8_t flash_program_buffer(void);
PRIVATE void flash_program_buffer_initialise(uint32_t address,
                                                uint16_t program_command);
#endif /* #ifndef FLASH_PROG_P_H */
