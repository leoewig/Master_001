/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    nvs_p.h
 *
 * description:  NVS private header file
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
#if !defined NVS_P_H
/**
 * NVS private header file
 * @file
 * @ingroup nvs
 *
 */
#define NVS_P_H

#include "basic_types.h"
#include "eeprom_types.h"
#include "nvs.h"
#include "nvs_cfg.h"

extern FROM nvs_feat_list_s nvs_feat_list[Nvs_Max_feature];

PRIVATE void nvs_calculate_feature_offsets(void);
PRIVATE void nvs_initialise_write_status(void);
PRIVATE data_validity_e nvs_restore_all_features(void);
PRIVATE data_validity_e nvs_restore_feature(Nvs_feature_list_e index);
PRIVATE void nvs_read_both_headers(Nvs_feature_list_e feat_index,
                                    nvs_header_s *header_array,
                                    nvs_header_status_s *status_array);
PRIVATE nvs_header_status_s nvs_read_header(Nvs_feature_list_e feat_index,
                                    nvs_bank_e bank_number,
                                    nvs_header_s *header_p);
PRIVATE nvs_header_status_s nvs_check_header(Nvs_feature_list_e feat_index,
                                    const nvs_header_s *header_p);
PRIVATE nvs_bank_read_order_e nvs_determine_read_order(
                                    const nvs_header_status_s *status_array);
PRIVATE nvs_bank_read_order_e nvs_read_order_when_both_valid(
                                    const nvs_header_status_s *status_array);
PRIVATE nvs_bank_e nvs_new_bank_from_read_order(
                                    nvs_bank_read_order_e order_to_try);
PRIVATE bool_t nvs_read_to_buffer(Nvs_feature_list_e feat_index,
                                    nvs_bank_read_order_e order_to_try,
                                    nvs_header_s *headers_p,
                                    nvs_bank_e *bank_restored_p);
PRIVATE bool_t nvs_read_first_choice_to_buffer(Nvs_feature_list_e feat_index,
                                    nvs_bank_read_order_e order_to_try,
                                    nvs_header_s *headers_p,
                                    nvs_bank_e *bank_restored_p);
PRIVATE bool_t nvs_read_second_choice_to_buffer(Nvs_feature_list_e feat_index,
                                    nvs_bank_read_order_e order_to_try,
                                    nvs_header_s *headers_p,
                                    nvs_bank_e *bank_restored_p);
PRIVATE bool_t nvs_read_bank_to_buffer(Nvs_feature_list_e feat_index,
                                    nvs_header_s *header_p,
                                    nvs_bank_e bank);
PRIVATE void nvs_copy_header_to_buffer(nvs_header_s *header_p);
PRIVATE bool_t nvs_check_data_crc_in_buffer(uint16_t size);
PRIVATE uint16_t nvs_calc_data_crc_of_buffer(uint16_t size);
PRIVATE uint16_t nvs_calc_header_crc_of_buffer(void);
PRIVATE bool_t nvs_buffer_version_matches(Nvs_feature_list_e feat_index);
PRIVATE void nvs_copy_from_buffer(Nvs_feature_list_e feat_index);
PRIVATE void nvs_mark_as_been_read(Nvs_feature_list_e feat_index);
PRIVATE void nvs_set_restore_status(Nvs_feature_list_e feat_index,
                                    data_validity_e restore_status);
PRIVATE void nvs_set_write_status(Nvs_feature_list_e feat_index,
                                        nvs_bank_e bank_restored,
                                        const nvs_header_s *headers);
PRIVATE uint16_t nvs_feature_eeprom_address(Nvs_feature_list_e feat_index, 
                                    nvs_bank_e bank_number);
PRIVATE void nvs_request_save_header_only(Nvs_feature_list_e feat_index);
PRIVATE void nvs_request_save_data(Nvs_feature_list_e feat_index);

/*
 * NVS background functions
 */
PRIVATE void nvs_bg_shutdown_detector(void);
PRIVATE void nvs_bg_write(bool_t shutdown);
PRIVATE void nvs_bg_check_shutdown_complete(void);
PRIVATE Eeprom_Op_Status nvs_bg_write_data(Nvs_feature_list_e feat_index, 
    bool_t checkpoint);
PRIVATE Eeprom_Op_Status nvs_bg_write_header(Nvs_feature_list_e feat_index);
PRIVATE Eeprom_Op_Status nvs_bg_write_header_to_bank(
                        Nvs_feature_list_e feat_index,nvs_bank_e bank_to_write);
PRIVATE Eeprom_Op_Status nvs_bg_write_all_to_bank(
                        Nvs_feature_list_e feat_index,nvs_bank_e bank_to_write,
                        bool_t checkpoint
                        );

PRIVATE void nvs_bg_copy_header_from_image_to_buffer(
                                                Nvs_feature_list_e feat_index);
PRIVATE void nvs_bg_copy_all_to_buffer(Nvs_feature_list_e feat_index);
PRIVATE void nvs_update_header_in_buffer(void);
PRIVATE void nvs_update_write_status(Nvs_feature_list_e feat_index);
PRIVATE Eeprom_Op_Status nvs_write_eeprom(Nvs_feature_list_e feat_index,
                                    nvs_bank_e bank, uint16_t bytes_to_copy);

#endif /* #if !defined NVS_P_H */
