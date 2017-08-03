/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   flash_prog_types.h
 *
 * description:  
 *  Macros and types for flash programming
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
#ifndef FLASH_PROG_TYPES_H
#define FLASH_PROG_TYPES_H
/**
 * Macros and types for flash programming
 *
 * @file
 */

/**
 * Characterise P-flash and D-flash for programming
 */
typedef struct {
    uint16_t program_command /*lint -esym(768,program_command) TODO remove */;
    uint16_t clear_command;
    uint32_t start_address;
    uint32_t length;
    uint32_t sector_mask;
    uint32_t sector_size;
} flash_parameters_s;

#define IN_FLASH(address,params)        ((address >= (params)->first_address) \
                                        && (address <= (params)->last_address))

#define FLASH_SECTOR_START(address,mask)        (address & mask)


#define FLASH_PHRASE_WORD_LENGTH     4u      /**< Must program 4 2-byte words
                                               at a time */
#define FLASH_PHRASE_BYTE_LENGTH     (FLASH_PHRASE_WORD_LENGTH * 2u)

typedef struct {
    bool_t empty;                       /**< FALSE if bytes have been stored
                                          since buffer was programmed to flash
                                          */
    uint32_t phrase_address;            /**< address of phrase which will be
                                          programmed with data in thsi buffer */
    uint16_t program_command;
    uint8_t data[FLASH_PHRASE_BYTE_LENGTH];  /**< the data to be written to flash */
} flash_buffer_s;

typedef enum {
    Flash_unknown,
    Flash_pflash,
    Flash_dflash
} flash_location_e;

/**
 * value returned by flash_program() and flash_clear() if the address range
 * doesn't fit entirely in either P-flash or D-flash
 *
 * Value chosen to _not_ clash with any of the error bits in FSTAT so that
 * an actual programming error can be distinguished from an address error
 */
#define FLASH_ADDRESS_ERROR             0x80u

#define FLASH_PHRASE_ADDRESS(address)   \
                        (address & (uint32_t) ~(FLASH_PHRASE_BYTE_LENGTH - 1uL))

#define FLASH_ADDRESS_IN_BUFFER_PHRASE(address)  \
                (FLASH_PHRASE_ADDRESS(address) == flash_buffer.phrase_address)

#endif /* ifndef FLASH_PROG_TYPES_H */
