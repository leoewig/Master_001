/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    nvs.h
 *
 * description:  NVS public header file
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
#if !defined NVS_H
#define NVS_H
/**
 * NVS public header file
 * @file
 * @ingroup nvs
 *
 */
/* Copyright (c) Linkedriving Tech 2012 */
/**
 * @defgroup nvs NVS Non-Volatile Storage
 *
 * Manages feature data which must be stored to EEPROM
 *
 * # Configure NV Memory for a feature
 * To use non-volatile memory, the following must be done:
 * + add the feature to the #Nvs_feature_list_e enumeration in nvs_cfg.h
 * + add the feature to the #nvs_feat_list array in nvs_cfg.c
 * + reserve a maximum size for the feature in nvs_cfg.h
 *
 * The nvs_feat_list entry must name the feature's _RAM image_ variable.
 *
 * ## Feature RAM image
 *
 * All NV data for a feature is stored in a RAM image structure.  This structure
 * forms a wrapper around the actual feature data and has three members:
 * a header, the feature data and an array of reserved bytes.
 * An example of the RAM image structure is #ctc_nvs_wrapper_s which is
 * used to define the CTC data #ctc_ram_image.
 *
 * ### Feature RAM image header
 * The header is an #nvs_header_s structure and is used by the NVS feature to
 * store information about a feature's NV data.  The header must be statically
 * initialised by the #NVS_HEADER_INIT macro.  This macro takes a single
 * argument, the layout version number.  The layout version number must be
 * changed whenever there is a risk that data stored in an EEPROM could be
 * incorrectly interpreted if the feature's NV data has changed.  For example,
 * if the meaning of a flag at a particulaar place in the data changes.
 * Note that the EERPOM data is covered by a checksum, so adding additional
 * bytes to the NV data will always stop old stored data from being loaded
 * from EEPROM regardless of whether the layout version number is changed.
 *
 * ### Feature Data
 * The actual feature data is the second member of the RAM image.  It must be
 * statically initialised with values that should be used if there is no
 * valid data found in the EEPROM.
 *
 * ### Reserved bytes
 * Reserved bytes are defined to allow features to occupy a fixed location in
 * EEPROM even if the data structures for individual features change.  This will
 * allow EEPROM data for one feature to be recovered irrespective of whether
 * the data structure for another has changed - provided, of course, that no
 * more than the total number of reserved bytes is added to the feature.
 *
 * The definition of the reserved bytes is done in such a way as to produce
 * a compiler error if the data structure becomes too large, so you must
 * use existng definitions, e.g. #ctc_nvs_wrapper_s, as a guide.
 *
 * The reserved bytes should be initialised by #NVS_RESERVED_BYTES_INIT which
 * includes lint suppression to avoid a too-few-initialisers warning.
 * 
 * # Using NV data
 * The feature can use the data in the Feature RAM image as regular FRAM
 * variables.  Before the feature is initialised, NVS will have attempted to 
 * load EEPROM data over the image.  If no valid EEPROM data can be found,
 * the ram image will continue to have its statgically initialised variable.
 * The success or failure of EEPROM restoration is found by invoking the macro
 * @ref NVS_RESTORE_STATUS on the ram image.
 *
 * When the system shuts down normally, the RAM image is automatically saved 
 * back to the EEPROM.  A feature can also request a save during normal
 * operation by calling nvs_request_store().
 *
 * ## Redundant storage
 * The two most recently saved images are kept in the EEPROM.  If the
 * most recently saved data is found to be corrupt then the older copy
 * is restored and #NVS_RESTORE_STATUS will return #Old_data_available.
 *
 * # Installation
 * EEPROM data is restored by calling nvs_init().  This must be called after
 * the eeprom driver is initialised but before any features which require
 * NV data to have been restored.
 *
 * Errors encountered during intialisation are reported by calling 
 * nvs_init_failure_management() after failure management has been initialised.
 *
 * nvs_background() must be called as part of the idle loop both to complete
 * initialisation tasks and to service requests by features to write EEPROM
 * data.
 *
 * # Shut down
 * Calling nvs_background(TRUE) will result in all features' data being written
 * to EEPROM.  This should therefore be done as part of the shutdown procedure.
 *
 * # Saving data during normal operation
 * Calling nvs_store_request() will flag that a feature's data should be stored
 * to NV RAM.  Once a request has been made, the NV data will be captured by
 * nvs_background() at its next execution and stored.  It is therefore possible
 * that the data saved will be different from the data at the moment of the
 * store request, although it will be a coherent copy.
 *
 * # Requirements trace
 * | Requirement | Satisfied by | Unit test | PAT |
 * |-------------|--------------|-----------|-----|
 * | REQ_BMS_SUP_NV_001 | #nvs_feat_list | test_nvs_restore_all_features__data() | |
 * | REQ_BMS_SUP_NV_003 | nvs_background() | test_nvs_background() | |
 * | REQ_BMS_SUP_NV_004 | nvs_calc_data_crc_of_buffer() | test_nvs_calc_data_crc_of_buffer() | |
 * | REQ_BMS_SUP_NV_013 | #NVS_EEPROM_BANK_SIZE, nvs_feature_eeprom_address() | test_nvs_feature_eeprom_address() | |
 * | REQ_BMS_SUP_NV_005 | nvs_update_write_status() | test_nvs_update_write_status() | |
 * | REQ_BMS_SUP_NV_002 | nvs_restore_all_features() | test_nvs_restore_all_features() | |
 * | REQ_BMS_SUP_NV_016 | nvs_determine_read_order() | test_nvs_determine_read_order() | |
 * | REQ_BMS_SUP_NV_006 | nvs_check_header() | test_nvs_check_header() | |
 * | REQ_BMS_SUP_NV_007 | nvs_read_second_choice_to_buffer() | test_nvs_read_second_choice_to_buffer() | |
 * | REQ_BMS_SUP_NV_014 | nvs_read_second_choice_to_buffer() | test_nvs_read_second_choice_to_buffer() | |
 * | REQ_BMS_SUP_NV_008 | nvs_set_restore_status() | test_nvs_set_restore_status() | |
 * | REQ_BMS_SUP_NV_009 | #Nvs_feature_list_e | by inspection | |
 * | REQ_BMS_SUP_NV_010 | #nvs_feat_list | test_nvs_restore_all_features__data() | |
 * | REQ_BMS_SUP_NV_011 | Not implemented | | |
 * | REQ_BMS_SUP_NV_012 | main() | | |
 * | REQ_BMS_SUP_NV_015 | crc16() | test_nvs_calc_data_crc_of_buffer() | |
 */
#include "nvs_types.h"
#include "nvs_cfg.h"
#include "flash_eee_types.h"

void nvs_init(void);
void nvs_request_store(Nvs_feature_list_e feat_index);
void nvs_request_all_and_reset(flash_eee_reset_type_e reset_type);
void nvs_request_checkpoint(Nvs_feature_list_e feat_index);
void nvs_init_failure_management(void);
bool_t nvs_background(bool_t store_all);
bool_t nvs_is_busy(void);

void nvs_bg_write_no_reset(bool_t store_all);
#endif /* #if !defined NVS_H */
