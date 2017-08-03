/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    ixcp_slave.c
 *
 * description: XCP private definitions
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
#ifndef IXCP_P_H
#define IXCP_P_H
/**
 * @file
 * @ingroup ixcp
 *
 * XCP private definitions
 */
/**
 * XCP master and slave on the internal CAN bus
 */
#include "ixcp_types.h"

/*
 * functions in ixcp_slave.c called from ixcp_prog.c
 */
ixcp_dto_type_e ixcp_sequence_error(uint8_t *dto_payloadp);
ixcp_dto_type_e ixcp_unknown_command(uint8_t *dto_payloadp);

/*
 * Programming functions in ixcp_prog.c
 */
ixcp_dto_type_e ixcp_program_start_reprog(uint8_t *dto_payload);
ixcp_dto_type_e ixcp_program_clear(const uint8_t *cro_data,
                                        uint8_t *dto_payload);
ixcp_dto_type_e ixcp_program(const uint8_t *cro_data,uint8_t *dto_payload);
ixcp_dto_type_e ixcp_program_max(const uint8_t *cro_data,uint8_t *dto_payload);
ixcp_dto_type_e ixcp_program_reset(uint8_t *dto_payload);

/*
 * static functions in ixcp_slave.c
 */
PRIVATE void ixcp_read_cro(const uint8_t *datap);

PRIVATE ixcp_dto_type_e ixcp_connect(uint8_t *dto_payloadp);
PRIVATE ixcp_dto_type_e ixcp_disconnect(void);
PRIVATE ixcp_dto_type_e ixcp_get_status(void);
PRIVATE ixcp_dto_type_e ixcp_get_comm_mode_info(uint8_t *dto_payloadp);
PRIVATE ixcp_dto_type_e ixcp_get_id(const uint8_t *cro_datap,
                                        uint8_t *dto_payloadp);
PRIVATE ixcp_dto_type_e ixcp_set_mta(const uint8_t *cro_datap,
                                        uint8_t *dto_payloadp);
PRIVATE ixcp_dto_type_e ixcp_upload(const uint8_t *cro_datap,
                                        uint8_t *dto_payloadp);
PRIVATE ixcp_dto_type_e ixcp_short_upload(const uint8_t *cro_datap,
                                        uint8_t *dto_payloadp);
PRIVATE ixcp_dto_type_e ixcp_get_daq_processor_info(void);
PRIVATE ixcp_dto_type_e ixcp_get_daq_resolution_info(uint8_t *dto_payloadp);
PRIVATE ixcp_dto_type_e ixcp_get_cal_page(uint8_t *dto_payloadp);
PRIVATE ixcp_dto_type_e ixcp_set_cal_page(const uint8_t *cro_datap,
                                        uint8_t *dto_payloadp);
PRIVATE ixcp_dto_type_e ixcp_copy_cal_page(const uint8_t *cro_datap,
                                        uint8_t *dto_payloadp);
PRIVATE ixcp_dto_type_e ixcp_build_checksum(const uint8_t *cro_datap);
PRIVATE ixcp_dto_type_e ixcp_download(const uint8_t *cro_datap,
                                        uint8_t *dto_payloadp);
PRIVATE ixcp_dto_type_e ixcp_download_max(const uint8_t *cro_datap,
                                        uint8_t *dto_payloadp);


PRIVATE ixcp_dto_type_e ixcp_set_mta_from_cro(const uint8_t *five_byte_address,
                                        uint8_t *dto_payloadp);
PRIVATE ixcp_dto_type_e ixcp_do_upload(uint8_t length,uint8_t *dto_payloadp);
PRIVATE ixcp_dto_type_e ixcp_do_download(uint8_t length,const uint8_t *srcp,
                                        uint8_t *dto_payloadp);

PRIVATE void ixcp_calculate_checksum(void);

PRIVATE ixcp_dto_type_e ixcp_program_start_app(uint8_t *dto_payloadp);

/*
 * string functions defined so the bootblock doesn't need to put the library
 * functions in RAM
 */
PRIVATE uint16_t ixcp_strlen(const char_t *string);
PRIVATE void ixcp_strcpy(char_t *dest, const char_t *src);
PRIVATE void ixcp_strcat(char_t *dest, const char_t *src);

/*
 * static functions in ixcp_prog.c
 */
PRIVATE ixcp_dto_type_e ixcp_do_program(const uint8_t *cro_data,
                            uint8_t *dto_payload, uint8_t len);
PRIVATE ixcp_dto_type_e ixcp_flash_error(uint8_t flash_errors,
                                                uint8_t *dto_payload);
#endif /* ifndef IXCP_P_H */
