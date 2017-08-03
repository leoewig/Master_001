#include "basic_types.h"
#include "hardware.h"

#include "nvs.h"
#include "nvs_p.h"
#include "nvs_cfg.h"
#include "eeprom.h"
#include "crc.h"
#include "fm.h"
#include "ms.h"         /* For shutdown flags */
#include "flash_eee.h"

/**
 * Static array storing the starting address for each feature's
 * EEPROM data.  Calculated by nvs_calculate_feature_offsets(),
 * used by nvs_feature_eeprom_address()
 */
PRIVATE FRAM uint16_t feature_bank0_address[Nvs_Max_feature];

/**
 * Local buffer used for all EEPROM reading and writing.
 *
 * Buffering reads allows the CRC to be checked before
 * feature initialisation data is overwritten.
 *
 * Buffering writes allows the write to be perfromed asynchronously from
 * the feature
 */
PRIVATE FRAM uint8_t nvs_buffer[NVS_LARGEST_FEATURE_BYTES];
/**
 * Array used for communication between foreground and background tasks.
 */
PRIVATE FRAM nvs_write_status_s nvs_write_status[Nvs_Max_feature];

/**
 * Single variable set to the 'worst' result of all the feature
 * restorations
 */
PRIVATE data_validity_e nvs_initialisation_status ;

/**
 * Flag to show that the NVS background is busy saving something.
 * Flag is needed by nvs_is_busy() because there is a gap between a
 * write_request flag being cleared and the data actually being saved to
 * EEPROM.
 */
PRIVATE FRAM bool_t nvs_bg_is_busy = FALSE;

/**
 * Flag requesting the reset the very next time the NVS data are stored.
 * Intended to be set only by the nvs_request_all_and_reset() function.
 */
//PRIVATE bool_t nvs_write_all_and_reset_req = FALSE;
uint8_t nvs_write_all_reset_req = 0u;

/**
 * Reset type to be performed by calling nvs_request_all_and_reset() function
 */
PRIVATE flash_eee_reset_type_e nvs_requested_reset_type = Reset_uninitialised;

/**
 * Initialise Non-volatile storage
 *
 * This function does not set any errors because it will be called
 * before failure management is running.  Instead, errors are latched in
 * #nvs_initialisation_status which can be used to update fault filters
 * by calling nvs_init_failure_management() after FM is up.
 */
void nvs_init(void)
{
    nvs_calculate_feature_offsets();
    nvs_initialise_write_status();
    /* commented out pending paging fix */
    nvs_initialisation_status = nvs_restore_all_features();
}
/**
 * Lower priority function to schedule blocking writes of NV data to the EEPROM.
 * @param store_all     Flag requesting that all features be stored to
 *                      EEPROM.
 * @retval      TRUE    A store_all request has been completed.
 * @retval      FALSE   Either a store_all has not been requested or the
 *                      request is not complete.
 *
 * Since nvs_bg_write() will complete a store_all in a single call, the return 
 * value will always be the same as store_all parameter.
 */
bool_t nvs_background(bool_t store_all)
{
    nvs_bg_write(store_all);
    return store_all;
}

/**
 * Request that a given feature's data be written to EEPROM.
 * Actual writing will be initiated by the NVS low-priority function
 * nvs_background().
 *
 * Interrupts are disabled around the setting of #nvs_write_status to avoid
 * the (unlikely) possibility of a higher priority task writing to it at the
 * same time.
 */
void nvs_request_store(Nvs_feature_list_e feat_index)
{
    uint8_t ccr;

    SAVE_INTERRUPTS(ccr);
    nvs_write_status[feat_index].write_request |= NVS_WRITE_DATA_REQUEST;
    /* This is a full write request, so checkpoint flag is cleared */
    nvs_write_status[feat_index].write_request &= (uint8_t)~(NVS_WRITE_CHECKPOINT_REQUEST);
    RESTORE_INTERRUPTS(ccr);
}

/** Request all features to store its data and request the NVS to reset
 * mmc immediately after it actually finishes the write, which should happen
 * very next time nvs_background() is called from the background or idle task.
 * This is intended to be used in situations, where system reset is needed as soon
 * as possible, so standard shutdown procedure can't be used, but still NVM data
 * are desired to keep in a good shape.
 */
void nvs_request_all_and_reset(flash_eee_reset_type_e reset_type)
{
    nvs_write_all_reset_req = 1;
    nvs_requested_reset_type = reset_type;
}

/**
 * Request that a given feature's data be written to EEPROM.
 * Actual writing will be initiated by the NVS low-priority function
 * nvs_background().
 *
 * Interrupts are disabled around the setting of #nvs_write_status to avoid
 * the (unlikely) possibility of a higher priority task writing to it at the
 * same time.
 *
 * A checkpoint will look like old data when read back
 */
void nvs_request_checkpoint(Nvs_feature_list_e feat_index)
{
    uint8_t ccr;

    SAVE_INTERRUPTS(ccr);
    nvs_write_status[feat_index].write_request |= 
        (NVS_WRITE_DATA_REQUEST | NVS_WRITE_CHECKPOINT_REQUEST);
    RESTORE_INTERRUPTS(ccr);
}

/**
 * Let foreground know if NVS is either actually busy doing something or
 * has outstanding writes.
 *
 * This is needed by reprogramming routines to refuse a reprogramming
 * request when there is EEPROM data to save.
 *
 * N.B. #nvs_bg_is_busy is set and cleared by idle-loop tasks so it will not
 * change state while this function is running.
 */
bool_t nvs_is_busy(void)
{
    Nvs_feature_list_e i;
    bool_t is_busy = FALSE;

    for (i = (Nvs_feature_list_e) 0 ; i < Nvs_Max_feature ; i++)
    {
        if (nvs_write_status[i].write_request != NVS_WRITE_IDLE)
        {
            is_busy = TRUE;
        }
    }
    if (nvs_bg_is_busy)
    {
        is_busy = TRUE;
    }
    return is_busy;
}



#ifndef PRODDER
/**
 * Uses the result from nvs_init() to update fault filters.
 *
 * This function is required because Failure Management will not be
 * operational until after fault management
 */
void nvs_init_failure_management(void)
{
    bool_t hard_error_present;
    bool_t soft_error_present;
    bool_t monitor;

    hard_error_present = (nvs_initialisation_status == No_data_available);
    soft_error_present = (nvs_initialisation_status == Old_data_available);
    monitor = TRUE;

    (void) fm_update_fault_filter(Fm_plat_eeprom_read_failed_hard,
            Fm_general_failure_information,
            hard_error_present,
            monitor);
    (void) fm_update_fault_filter(Fm_plat_eeprom_read_failed_soft,
            Fm_general_failure_information,
            soft_error_present,
            monitor);
}
#endif

/**
 * Calculates the EEPROM address for each feature's Bank 0 data
 * The results are stored in #feature_bank0_address.  If the EEPROM data
 * does not all fit in a single bank (i.e. the end address is greater than
 * #NVS_EEPROM_BANK0_START + #NVS_EEPROM_BANK_SIZE) then this function
 * will throw an internal error.
 */
PRIVATE void nvs_calculate_feature_offsets(void)
{
    int16_t i;
    int16_t last_index;
    uint16_t end_address;

    feature_bank0_address[0] = NVS_EEPROM_BANK0_START;

    for (i =  1 ; i < (int16_t) Nvs_Max_feature ; i++)
    {
        feature_bank0_address[i] = feature_bank0_address[i-1]
                                        + nvs_feat_list[i-1].wrapper_size;
    }

    last_index = (int16_t) Nvs_Max_feature - 1;
    end_address = feature_bank0_address[last_index]
                        + nvs_feat_list[last_index].wrapper_size;

    if (end_address > (NVS_EEPROM_BANK0_START + NVS_EEPROM_BANK_SIZE))
    {
        /*
         * oops - we've got the EEPROM layout wrong because we've written
         * off the end of our allocated space
         */
        INTERNAL_ERROR();
    }
}

/**
 * Initialises #nvs_write_status to #NVS_WRITE_IDLE for every feature
 */
PRIVATE void nvs_initialise_write_status(void)
{
    Nvs_feature_list_e i;
    nvs_write_status_s initial_write_status = { 
                                NVS_WRITE_IDLE, /* write_request */
                                0u,             /* next_write_count */
                                NVS_bank_none,  /* last_bank_restored */
                                NVS_bank_both   /* next_bank_to_write_data */
                            };

    for (i = (Nvs_feature_list_e) 0 ; i < Nvs_Max_feature ; i++)
    {
        nvs_write_status[i] = initial_write_status;
    }
}

/**
 * Calls nvs_restore_feature() for each feature, keeping track of
 * the least successful restoration.
 *
 * @retval New_data_available   The latest data for every feature was
 *                              restoreed from EEPROM
 * @retval Old_data_available   Data was restored from EEPROM for every feature
 *                              but the latest data was unavailable for at
 *                              least one feature.
 * @retval No_data_available    No EEPROM data was available for at least one
 *                              feature
 */
PRIVATE data_validity_e nvs_restore_all_features(void)
{
    Nvs_feature_list_e i;
    data_validity_e feature_validity;
    data_validity_e worst_validity = New_data_available;

    for (i = (Nvs_feature_list_e) 0 ; i < Nvs_Max_feature ; i++)
    {
        feature_validity = nvs_restore_feature(i);
        if (feature_validity == No_data_available)
        {
            /* at least one feature has no data available */
            worst_validity = No_data_available;
        }
        else if ((feature_validity == Old_data_available)
                && (worst_validity == New_data_available))
        {
            /* at least one feature received old data */
            worst_validity = Old_data_available;
        }
        else
        {
            /* do nothing */
        }
    }
    return worst_validity;
}

/**
 * Attempts to restore feature data for a single feature.
 *
 * The hard work is to decide which, if either, bank contains the latest data.
 * Headers from both banks are read by nvs_read_both_headers() before
 * deciding their relative age with nvs_determine_read_order().
 *
 * nvs_read_to_buffer() attempts to restore data from EEPROM, with the newer
 * buffer attempted first.
 * If coinsistent data is found in either buffer, it is copied into the
 * feature's NVS data structure and the restore_Status is set.
 *
 * @retval      No_data_available       No valid data was restored from the 
 *                                      EEPROM
 * @retval      Old_data_available      Data was restored from EEPROM but it
 *                                      either was not the most recently saved
 *                                      or its age could not be confirmed.
 * @retval      New_data_available      The most recently saved EEPROM data
 *                                      was restored.
 */ 
PRIVATE data_validity_e nvs_restore_feature(Nvs_feature_list_e feat_index)
{
    nvs_bank_read_order_e order_to_try;
    nvs_bank_e new_bank;
    nvs_bank_e bank_restored;
    bool_t data_restored;

    data_validity_e restore_status = New_data_available;

    nvs_header_s headers[2];
    nvs_header_status_s header_status[2];

    nvs_read_both_headers(feat_index,headers,header_status);

    order_to_try = nvs_determine_read_order(header_status);

    /* 
     * note which bank is the new one so that we can set the
     * status correctly 
     */
    new_bank = nvs_new_bank_from_read_order(order_to_try);

    /* try to restore data in the preferred order */
    data_restored = nvs_read_to_buffer(feat_index,order_to_try,
                                            headers,&bank_restored);

    if (data_restored)
    {
        if ((bank_restored == new_bank)
            && header_status[new_bank].unread_data)
        {
            /*
             * We've reads the newest data and it looks like it hasn't
             * been restored before
             */
            restore_status = New_data_available;
        }
        else
        {
            /* we have some data, just not the newest */
            restore_status = Old_data_available;
        }
        /* as we have something, copy it out of the buffer over the feature
         * structure */
        nvs_copy_from_buffer(feat_index);
    }
    else
    {
        restore_status = No_data_available;
    }

    nvs_set_restore_status(feat_index,restore_status);
    nvs_set_write_status(feat_index,bank_restored,headers);
    return restore_status;
}

/**
 * Reads NVS headers for a feature from both banks.
 *
 * @param feat_index    Index of feature being restored
 * @param[in,out] headers       Pointer to array of two header structures.
 *                      Must have been allocated to near (e.g. stack) memory
 *                      by the caller.
 * @param[in,out] status_array  Pointer to array of two header status
 *                      structures.  Must have been allocated to near
 *                      (e.g. stack) memory the caller.
 */
PRIVATE void nvs_read_both_headers(Nvs_feature_list_e feat_index,
                                        nvs_header_s *headers,
                                        nvs_header_status_s *status_array)
{
    status_array[0] = nvs_read_header(feat_index,NVS_bank_0,&headers[0]);
    status_array[1] = nvs_read_header(feat_index,NVS_bank_1,&headers[1]);
}

/**
 * Reads a feature header from EEPROM
 *
 * @param feat_index    Index of feature being restored
 * @param bank_number   EEPROM bank to be read
 * @param[in,out] header_p Pointer to header structure.  Must have been
 *                      allocated to near (e.g. stack) memory by the caller.
 * @returns A header status structure.  N.B. The header status structure is
 *                      returned on the stack and not written through a pointer.
 */
PRIVATE nvs_header_status_s nvs_read_header(Nvs_feature_list_e feat_index,
                                                nvs_bank_e bank_number,
                                                nvs_header_s *header_p)
{
    nvs_header_status_s status;
    Eeprom_Op_Status eeprom_stat;

    uint16_t header_address;
   
    header_address = nvs_feature_eeprom_address(feat_index,bank_number);

    eeprom_stat = eeprom_read_data(header_address,
                                    (FPTR uint8_t *)header_p,
                                    NVS_HEADER_SIZE);
    if (eeprom_stat == Ee_driver_ready)
    {
        /* we got something from the EEPROM so check it is self-consistent */
        status = nvs_check_header(feat_index,header_p);
    }
    else
    {
        /* Nothing from the EEPROM so mark header as invalid.  None of the
         * other fields matter */
        status.valid = FALSE;
    }

    return status;
}

/**
 * Check that an NVS header is valid and populate the statrus structure.
 *
 * Header will be valid if:
 * + #NVS_HEADER_PATTERN is stored in the .restore_status field
 * + The layout_version_number matches the version initialised from ROM.
 * + The header CRC is correct, showing that it hasn't been corrupted.
 *
 * Additional fields populated are:
 * + .unread_data, which is TRUE if write_count != last_read (because after
 *  a successful restore, last_read will be updated on the eeprom to equal 
 *  write_count)
 * + write_count, which is copied from the same field in the header structure
 *
 * @param feat_index    Index of feature being restored
 * @param header_p      Pointer to a header structure.  Must be allocated to
 *                      near (e.g. stack) memory by the caller.
 * @returns A header status structure.  N.B. The header status structure is
 *                      returned on the stack and not written through a pointer.
 *
 */
PRIVATE nvs_header_status_s nvs_check_header(Nvs_feature_list_e feat_index,
                                                const nvs_header_s *header_p)
{
    nvs_header_status_s status;
    uint8_t header_pattern;
    uint16_t calculated_header_crc ; /* calculated CRC for header, _excluding_ 
                                        header_crc word itself */
    uint8_t version_from_rom;
    uint8_t version_from_eeprom;

    header_pattern = (uint8_t) header_p->restore_status;
    version_from_eeprom = header_p->layout_version_number;

    /* look at ROM-initialised version number to make sure the data stored
     * in EEPROM is for the current version of the feature */

    version_from_rom =
        nvs_feat_list[feat_index].header_p->layout_version_number;

    calculated_header_crc = crc16(CRC16_SEED,(FPTR const uint8_t *)header_p,
                                NVS_HEADER_SIZE - sizeof(uint16_t));

    status.valid = ((header_pattern == NVS_HEADER_PATTERN)
                    && (version_from_rom == version_from_eeprom)
                    && (header_p->header_crc == calculated_header_crc));

    status.unread_data = (header_p->write_count != header_p->last_read);

    status.write_count = header_p->write_count;

    return status;
}

/**
 * Determine which banks and which order to try restoring EEPROM data from for a
 * feature.
 */
PRIVATE nvs_bank_read_order_e nvs_determine_read_order(
                                const nvs_header_status_s *status_array)
{
    nvs_bank_read_order_e read_order;

    if (status_array[0].valid && status_array[1].valid)
    {
        read_order = nvs_read_order_when_both_valid(status_array);
    }
    else if (status_array[0].valid)
    {
        read_order = NBRO_bank0_only;
    }
    else if (status_array[1].valid)
    {
        read_order = NBRO_bank1_only;
    }
    else
    {
        read_order = NBRO_none;
    }
    return read_order;
}

/**
 * Determine which banks and which order to try restoring EEPROM data when
 * both banks contain valid data.
 *
 * Uses the fact that banks are written in alternate order with the write_count
 * incremented between each count.  Therefore the newer bank
 */
PRIVATE nvs_bank_read_order_e nvs_read_order_when_both_valid(
                                        const nvs_header_status_s *status_array)
{
    nvs_bank_read_order_e read_order;

    uint8_t write_diffu;
    int8_t write_diffs;
   
    write_diffu = status_array[0].write_count - status_array[1].write_count;
    write_diffs = (int8_t) write_diffu;

    switch (write_diffs)
    {
        case 1:
            /* Bank 0 data is new */
            read_order = NBRO_bank0_newer_than_bank1;
            break;
        case -1:
            /* Bank 1 data is new */
            read_order = NBRO_bank1_newer_than_bank0;
            break;
        default:
            /*
             * counters are out of step:
             * either bank will do but neither can be considered new
             */
            read_order = NBRO_both_old;
            break;
    }
    return read_order;
}

/**
 * Helper function to extract which bank is new from the value of
 * the #nvs_bank_read_order_e enumeration.
 * @retval NVS_bank_0           input was NBRO_bank0_newer_than_bank1
 * @retval NVS_bank_1           input was NBRO_bank1_newer_than_bank0
 * @retval NVS_bank_none        input was anything else (because need two
 *                              consistent banks to deduce that one of them
 *                              is the newer)
 */
PRIVATE nvs_bank_e nvs_new_bank_from_read_order(
                                nvs_bank_read_order_e order_to_try)
{
    nvs_bank_e new_bank;
    switch (order_to_try)
    {
        case NBRO_bank0_newer_than_bank1:
            new_bank = NVS_bank_0;
            break;
        case NBRO_bank1_newer_than_bank0:
            new_bank = NVS_bank_1;
            break;
        case NBRO_bank0_only:
        case NBRO_bank1_only:
        case NBRO_both_old:
        case NBRO_none:
        default:
            new_bank = NVS_bank_none;
            break;
    }
    return new_bank;
}

/**
 * Reads data from the EEPROM to #nvs_buffer
 *
 * Both headers will have previously been read from EEPROM and
 * are passed to this function.  Data reading is attempted from the banks in
 * the order given by order_to_try.
 *
 * @param feat_index    feature index to read
 * @param order_to_try  bank order in which to attempt to recover data
 * @param[in] headers_p Two-element array containing the header data for each 
 *                      bank
 * @param[out] bank_restored_p    The bank which was restored, or #NVS_bank_none
 *                      if neither.
 * @retval              TRUE data from a bank was restored
 * @retval              No data could be restored.
 */
PRIVATE bool_t nvs_read_to_buffer(Nvs_feature_list_e feat_index,
                                nvs_bank_read_order_e order_to_try,
                                nvs_header_s *headers_p,
                                nvs_bank_e *bank_restored_p)
{
    bool_t first_choice_succeeded;
    bool_t second_choice_succeeded;
    bool_t something_read;
    
    first_choice_succeeded = nvs_read_first_choice_to_buffer(feat_index,
                                                            order_to_try,
                                                            headers_p,
                                                            bank_restored_p);
    if (!first_choice_succeeded)
    {
        second_choice_succeeded = nvs_read_second_choice_to_buffer(feat_index,
                                                            order_to_try,
                                                            headers_p,
                                                            bank_restored_p);
    }
    else
    {
        second_choice_succeeded = FALSE;
    }
    something_read = (first_choice_succeeded || second_choice_succeeded);
    return something_read;
}


/**
 * Attempts to read data from the first choice bank to #nvs_buffer
 *
 * Both headers will have previously been read from EEPROM and
 * are passed to this function.  Data reading is attempted from the first
 * (or only) bank specified by order_to_try.  If there is no first choice, no
 * dat ais read.
 *
 * @param feat_index    feature index to read
 * @param order_to_try  bank order in which to attempt to recover data. This
 *                      function will attempt to recover the first choice only.
 * @param[in] headers_p Two-element array containing the header data for each 
 *                      bank
 * @param[out] bank_restored_p    The bank which was restored, or #NVS_bank_none
 *                      if neither.
 * @retval              TRUE data from first choice bank was restored
 * @retval              No data could be restored.
 */

PRIVATE bool_t nvs_read_first_choice_to_buffer(Nvs_feature_list_e feat_index,
                                        nvs_bank_read_order_e order_to_try,
                                        nvs_header_s *headers_p,
                                        nvs_bank_e *bank_restored_p)
{
    nvs_bank_e first_choice = NVS_bank_none;
    bool_t first_choice_succeeded;

    switch (order_to_try)
    {
        case NBRO_bank0_only:
        case NBRO_bank0_newer_than_bank1:
        case NBRO_both_old:
            first_choice = NVS_bank_0;
            first_choice_succeeded = nvs_read_bank_to_buffer(feat_index,
                                                            &headers_p[0],
                                                            NVS_bank_0);
            break;
        case NBRO_bank1_only:
        case NBRO_bank1_newer_than_bank0:
            first_choice = NVS_bank_1;
            first_choice_succeeded = nvs_read_bank_to_buffer(feat_index,
                                                            &headers_p[1],
                                                            NVS_bank_1);
            break;
        case NBRO_none:
        default:
            first_choice_succeeded = FALSE;
    }

    *bank_restored_p =(first_choice_succeeded) ? first_choice : NVS_bank_none;
    return first_choice_succeeded;
}

/**
 * Attempts to read data from the second choice bank to #nvs_buffer.  Must
 * be called only if nvs_read_first_choice_to_buffer() was unable to restore
 * data.
 *
 * Both headers will have previously been read from EEPROM and
 * are passed to this function.  Data reading is attempted from the second
 * choice bank specified by order_to_try.  If there is no second choice,
 * no data is read.
 *
 * @param feat_index    feature index to read
 * @param order_to_try  bank order in which to attempt to recover data.  This
 *                      function will attempt to recover the second choice only.
 * @param[in] headers_p Two-element array containing the header data for each 
 *                      bank
 * @param[out] bank_restored_p    The bank which was restored, or #NVS_bank_none
 *                      if neither.
 * @retval              TRUE data from first choice bank was restored
 * @retval              No data could be restored.
 */
PRIVATE bool_t nvs_read_second_choice_to_buffer(Nvs_feature_list_e feat_index,
                                        nvs_bank_read_order_e order_to_try,
                                        nvs_header_s *headers_p,
                                        nvs_bank_e *bank_restored_p)
{
    nvs_bank_e second_choice = NVS_bank_none;
    bool_t second_choice_succeeded;
    
    switch (order_to_try)
    {
        case NBRO_bank0_newer_than_bank1:
        case NBRO_both_old:
            second_choice = NVS_bank_1;
            second_choice_succeeded = nvs_read_bank_to_buffer(feat_index,
                                                            &headers_p[1],
                                                            NVS_bank_1);
            break;
        case NBRO_bank1_newer_than_bank0:
            second_choice = NVS_bank_0;
            second_choice_succeeded = nvs_read_bank_to_buffer(feat_index,
                                                            &headers_p[0],
                                                            NVS_bank_0);
            break;
        case NBRO_bank0_only:
        case NBRO_bank1_only:
        case NBRO_none:
        default:
            /* there is no second choice to try */
            second_choice_succeeded = FALSE;
            break;
    }

    *bank_restored_p =(second_choice_succeeded) ? second_choice : NVS_bank_none;
   
    return second_choice_succeeded;
}
/**
 * Copies a header and data for a given bank to #nvs_buffer.
 *
 * The header to be copied is passed to the function. Data is read directly
 * from the EEPROM. After reading the data, its CRC is checked against the
 * value in the header.
 *
 * @param       feat_index      Feature to read
 * @param[in]   header_p        Pointer to header data for the bank being read.
 * @param       bank            The bank to restore
 * @retval      TRUE            Data was read from EEPROM with a correct
 *                              CRC
 * @retval      FALSE           Either the EEPROM couldn't be read or the
 *                              data's CRC did not agree with the data_crc
 *                              in the header.
 */
PRIVATE bool_t nvs_read_bank_to_buffer(Nvs_feature_list_e feat_index,
                                                nvs_header_s *header_p,
                                                nvs_bank_e bank)
{
    Eeprom_Op_Status eeprom_stat;
    bool_t crc_ok ;

    uint16_t data_address;
   
    nvs_copy_header_to_buffer(header_p);

    data_address = nvs_feature_eeprom_address(feat_index,bank)
                                + NVS_HEADER_SIZE;

    eeprom_stat = eeprom_read_data(data_address,nvs_buffer+NVS_HEADER_SIZE,
                                        nvs_feat_list[feat_index].data_size);
    if (eeprom_stat == Ee_driver_ready)
    {
        crc_ok = nvs_check_data_crc_in_buffer(
                                nvs_feat_list[feat_index].data_size);
    }
    else
    {
        crc_ok = FALSE;
    }
    return crc_ok;
}

/**
 * Copies header information to #nvs_buffer.
 */
PRIVATE void nvs_copy_header_to_buffer(nvs_header_s *header_p)
{
    uint8_t i;
    uint8_t *header_as_bytes = (uint8_t *)header_p;

    for (i = 0u ; i < NVS_HEADER_SIZE ; i++)
    {
        nvs_buffer[i] = header_as_bytes[i];
    }
}

/**
 * Checks the data in #nvs_buffer against the data CRC in the header stored
 * at the start of nvs_buffer.
 * @param       data_size       Number of bytes of data
 * @retval      TRUE            CRC of data matches value stored in header
 * @retval      FALSE           CRC does not match value stored in header
 */
PRIVATE bool_t nvs_check_data_crc_in_buffer(uint16_t data_size)
{
    bool_t crc_ok;
    uint16_t data_crc_in_header;
    uint16_t crc_of_data;

    data_crc_in_header = ((FRAM nvs_header_s *)nvs_buffer)->data_crc;
    crc_of_data = nvs_calc_data_crc_of_buffer(data_size);
    crc_ok = (data_crc_in_header == crc_of_data);

    return crc_ok;
}

/**
 * Calculates a CRC for the header stored in #nvs_buffer
 * @returns     CRC of the header _excluding_ the header_crc field itself
 */
PRIVATE uint16_t nvs_calc_header_crc_of_buffer(void)
{
    uint16_t crc;

    uint16_t crc_size ;
    
    crc_size = NVS_HEADER_SIZE - sizeof(uint16_t) ; /* CRC calculation
                                                       excludes CRC itself */
    crc = crc16(CRC16_SEED,nvs_buffer,crc_size);
    return crc;
}


/**
 * Calculates a CRC for the data stored in #nvs_buffer
 * @param       data_size       Number of bytes of data
 * @returns     CRC of the data
 */
PRIVATE uint16_t nvs_calc_data_crc_of_buffer(uint16_t data_size)
{
    uint16_t crc;

    uint16_t data_offset ;
    
    data_offset = NVS_HEADER_SIZE; /* data starts after the header */

    crc = crc16(CRC16_SEED,&nvs_buffer[data_offset],data_size);
    return crc;
}

/**
 * Compares the layout_version_number in #nvs_buffer with the value already
 * present in the feature RAM image.
 *
 * The feasture RAM image will contain a statically initialised value.  This
 * value will be incrementede whenever the ram image format changes in a way
 * which could lead to old EEPROM data being misinterpreted.
 * @retval      TRUE    the layout_version_numbers are the same
 * @retval      FALSE   the layout_version_numbers differ
 */
PRIVATE bool_t nvs_buffer_version_matches(Nvs_feature_list_e feat_index)
{
    uint8_t buffer_version;     /* version number read from EEPROM */
    uint8_t rom_version;        /* version number statically initialised
                                   into feature RAM image */
    bool_t versions_match;

    buffer_version = ((FRAM nvs_header_s *)nvs_buffer)->layout_version_number;
    rom_version = nvs_feat_list[feat_index].header_p->layout_version_number;

    versions_match = (buffer_version == rom_version);

    return versions_match;
}

/**
 * Copies the entire EEPROM image for a feature from #nvs_buffer to the
 * feature RAM image.
 */
PRIVATE void nvs_copy_from_buffer(Nvs_feature_list_e feat_index)
{
    uint16_t bytes_to_copy;
    uint16_t i;
    FRAM uint8_t *feature_nv_image;

    bytes_to_copy = NVS_HEADER_SIZE + nvs_feat_list[feat_index].data_size;

    if (bytes_to_copy > NVS_LARGEST_FEATURE_BYTES)
    {
        /* 
         * NV layout has been updated but someone forgot to change
         * NVS_LARGEST_FEATURE_BYTES to match the length of the largest
         * feature.  An internal error is better than either a scribbler off
         * the end of nvs_buffer[] or an arbitrary truncation
         */
        INTERNAL_ERROR();
    }
    feature_nv_image = (FRAM uint8_t *) nvs_feat_list[feat_index].header_p;

    for (i = 0u ; i < bytes_to_copy ; i++)
    {
        feature_nv_image[i] = nvs_buffer[i];
    }
    nvs_mark_as_been_read(feat_index);

    return;
}

/**
 * Updates a feature's header information to show that the current data stored
 * in EEPROM has been read and requests that the header be stored back to
 * EEPROM.
 *
 * Updating is done by setting last_read equal to write_count.
 */
PRIVATE void nvs_mark_as_been_read(Nvs_feature_list_e feat_index)
{
    FRAM nvs_header_s *header_p;
    uint8_t write_count;

    header_p = nvs_feat_list[feat_index].header_p;

    write_count = header_p->write_count;
    header_p->last_read = write_count;
    nvs_request_save_header_only(feat_index);
    return;
}

/**
 * Sets the restore_status of a feature's EEPROM header information to the
 * supplied value.
 *
 * @note the value stored on the EEPROM is the marker NVS_HEADER_PATTERN.  This
 * is replaced by this function with a valid value of data_validity_e
 */
PRIVATE void nvs_set_restore_status(Nvs_feature_list_e feat_index,
        data_validity_e restore_status)
{
    nvs_feat_list[feat_index].header_p->restore_status = restore_status;
    return;
}

/**
 * Sets up write status for a feature after restoration has been
 * attempted.
 */
PRIVATE void nvs_set_write_status(Nvs_feature_list_e feat_index,
                                        nvs_bank_e bank_restored,
                                        const nvs_header_s *headers)
{
    FRAM nvs_write_status_s *write_status_p = &nvs_write_status[feat_index];

    write_status_p->last_bank_restored = bank_restored;
    switch (bank_restored)
    {
        case NVS_bank_0:
            write_status_p->next_bank_to_write_data = NVS_bank_1;
            write_status_p->next_write_count 
                                = headers[NVS_bank_0].write_count + 1u;
            break;
        case NVS_bank_1:
            write_status_p->next_bank_to_write_data = NVS_bank_0;
            write_status_p->next_write_count 
                                = headers[NVS_bank_1].write_count + 1u;
            break;
        case NVS_bank_none:     /* neither header was good so write both */
            write_status_p->next_bank_to_write_data = NVS_bank_both;
            write_status_p->next_write_count = 0u;
            break;
        case NVS_bank_both: 
        default: 
            /* should not happen */
            INTERNAL_ERROR();
            break;
    }
}

/**
 * Returns the EEPROM address for a feature's NVS information.
 */
PRIVATE uint16_t nvs_feature_eeprom_address(Nvs_feature_list_e feat_index, 
                                        nvs_bank_e bank_number)
{
    uint16_t address;
    
    address = (bank_number == NVS_bank_0) ? feature_bank0_address[feat_index]
                                            : (feature_bank0_address[feat_index]
                                              + NVS_EEPROM_BANK_SIZE);
    return address;
}

/**
 * Flag to the NVS background that a feature's header should be re-saved to
 * the EEPROM.  
 *
 * Interrupts are disabled to prevent the (unlikely) problem
 * of two tasks attempting to manipulate the data at the same time.
 */
PRIVATE void nvs_request_save_header_only(Nvs_feature_list_e feat_index)
{
    uint8_t ccr;

    SAVE_INTERRUPTS(ccr);
    nvs_write_status[feat_index].write_request |= NVS_WRITE_HEADER_REQUEST;
    RESTORE_INTERRUPTS(ccr);
}

/*
 * Functions called from the background
 */

/**
 * Initiates blocking EEPROM writes for:
 * + any features for which a write has been requested
 * + all features if a shutdown is signaled
 * @param store_all     If TRUE, all features need writing to EEPROM.
 *                      If FALSE, only features requested via #nvs_write_status
 *                      are written.
 *
 * This function also contains the logic for setting and 
 * clearing #nvs_bg_is_busy.  Tis flag is needed to flag the interval
 * between clearing a write request and the data being saved to EEPROM.
 * We take a conservative approach to keep the logic simpler.
 */
PRIVATE void nvs_bg_write(bool_t store_all)
{
    Nvs_feature_list_e i;
    Eeprom_Op_Status status ;
    Eeprom_Op_Status captured_failure = Ee_driver_ready; /* changes if any
                                        write doesn't return Ee_driver_ready */
    bool_t write_performed = FALSE;     /* TRUE if at least one feature
                                           was written */
    bool_t fault_present;               /* TRUE if at least one write failed */
    bool_t checkpoint;
    bool_t nvs_write_all_and_reset_req_processed = FALSE;
    if(nvs_write_all_reset_req==1u)
    {
        nvs_write_all_and_reset_req_processed = TRUE;
        store_all = TRUE;
    }
    for (i = (Nvs_feature_list_e) 0 ; i < Nvs_Max_feature ; i++)
    {
        /* 
         * note that header writes only occur immediately after start-up and
         * will be to different bank than the first data write
         */
        if (nvs_write_status[i].write_request & NVS_WRITE_HEADER_REQUEST)
        {
            nvs_bg_is_busy = TRUE;
            status = nvs_bg_write_header(i);
            nvs_bg_is_busy = FALSE;
            write_performed = TRUE;
            if (status != Ee_driver_ready)
            {
                captured_failure = status;
            }
        }
        if (store_all || (nvs_write_status[i].write_request & NVS_WRITE_DATA_REQUEST))
        {
            /* if a store-all is requested, disregard any pending checkpoint 
             * flag as store_all in a full write */
            checkpoint = !store_all && (nvs_write_status[i].write_request
                & NVS_WRITE_CHECKPOINT_REQUEST);

            nvs_bg_is_busy = TRUE;
            status = nvs_bg_write_data(i,checkpoint);
            nvs_bg_is_busy = FALSE;
            write_performed = TRUE;
            if (status != Ee_driver_ready)
            {
                captured_failure = status;
            }
        }
    }
    fault_present = (captured_failure != Ee_driver_ready);
#ifndef PRODDER
    (void) fm_update_fault_filter(Fm_plat_eeprom_write,
                                    Fm_general_failure_information,
                                    fault_present,
                                    write_performed);
    if((nvs_write_all_reset_req==1u) && nvs_write_all_and_reset_req_processed)
    {
        /* Reset after nvs write has been requested. Do so only if write to the EEPROM was
         * successful */
        if(nvs_requested_reset_type != Reset_uncontrolled)
        {
            flash_eee_record_shutdown_and_wait(nvs_requested_reset_type);
        }
        RESET_S12();
    }
#endif
}


void nvs_bg_write_no_reset(bool_t store_all)
{
    Nvs_feature_list_e i;
    Eeprom_Op_Status status ;
    Eeprom_Op_Status captured_failure = Ee_driver_ready; /* changes if any
                                        write doesn't return Ee_driver_ready */
    bool_t write_performed = FALSE;     /* TRUE if at least one feature
                                           was written */
    bool_t checkpoint;


    for (i = (Nvs_feature_list_e) 0 ; i < Nvs_Max_feature ; i++)
    {
        /* 
         * note that header writes only occur immediately after start-up and
         * will be to different bank than the first data write
         */
        if (nvs_write_status[i].write_request & NVS_WRITE_HEADER_REQUEST)
        {
            nvs_bg_is_busy = TRUE;
            status = nvs_bg_write_header(i);
            nvs_bg_is_busy = FALSE;
            write_performed = TRUE;
            if (status != Ee_driver_ready)
            {
                captured_failure = status;
            }
        }
        if (store_all || (nvs_write_status[i].write_request & NVS_WRITE_DATA_REQUEST))
        {
            /* if a store-all is requested, disregard any pending checkpoint 
             * flag as store_all in a full write */
            checkpoint = !store_all && (nvs_write_status[i].write_request
                & NVS_WRITE_CHECKPOINT_REQUEST);

            nvs_bg_is_busy = TRUE;
            status = nvs_bg_write_data(i,checkpoint);
            nvs_bg_is_busy = FALSE;
            write_performed = TRUE;
            if (status != Ee_driver_ready)
            {
                captured_failure = status;
            }
        }
    }
}

/**
 * Updates the header information after a feature has been read
 */
PRIVATE Eeprom_Op_Status nvs_bg_write_header(Nvs_feature_list_e feat_index)
{
    Eeprom_Op_Status status;
    nvs_bank_e bank_to_write;

    bank_to_write = nvs_write_status[feat_index].last_bank_restored;

    if ((bank_to_write == NVS_bank_0) || (bank_to_write == NVS_bank_1))
    {
        status = nvs_bg_write_header_to_bank(feat_index,bank_to_write);
    }
    else
    {
        /* 
         * shouldn't have got here if a bank wasn't restored so return an
         * error code
         * Which one is a bit arbitrary but address error seems apt
         */
        status = Ee_arg_address_error;
    }
    return status;
}

/**
 * Writes a feature's header and data to EEPROM
 */
PRIVATE Eeprom_Op_Status nvs_bg_write_data(Nvs_feature_list_e feat_index, bool_t checkpoint)
{
    Eeprom_Op_Status status;
    nvs_bank_e bank_to_write;

    bank_to_write = nvs_write_status[feat_index].next_bank_to_write_data;
    switch (bank_to_write)
    {
        case NVS_bank_0:
        case NVS_bank_1:
            status = nvs_bg_write_all_to_bank(feat_index,bank_to_write,checkpoint);
            break;
        case NVS_bank_both:
            /* Neither bank contains valid data so need to write both of them.
             * Note that the order to write each bank must agree with the logic
             * in nvs_update_write_status() */
            status = nvs_bg_write_all_to_bank(feat_index,NVS_bank_0,checkpoint);
            if (status == Ee_driver_ready)
            {
                status = nvs_bg_write_all_to_bank(feat_index,NVS_bank_1,checkpoint);
            }
            break;
        case NVS_bank_none:
        default:
            /* should not happen so return an error (doesn't really matter
             * which one) */
            status = Ee_arg_address_error;
            break;
    }
    return status;
}

/**
 * Copies a feature NVS header to #nvs_buffer, prepares it for writing to
 * the EEPROM and then writes to to EEPROM
 * @returns status returned by nvs_write_eeprom()
 */
PRIVATE Eeprom_Op_Status nvs_bg_write_header_to_bank(
                        Nvs_feature_list_e feat_index,nvs_bank_e bank_to_write)
{
    nvs_bg_copy_header_from_image_to_buffer(feat_index);
    nvs_update_header_in_buffer();
    return nvs_write_eeprom(feat_index,bank_to_write,NVS_HEADER_SIZE);
}


/**
 * Copies a feature NVS header to #nvs_buffer atomically and updates
 * #nvs_write_status
 */
PRIVATE void nvs_bg_copy_header_from_image_to_buffer(
                        Nvs_feature_list_e feat_index)
{
    uint8_t ccr;
    uint16_t i;
    FRAM uint8_t *header_as_bytes = 
        (FRAM uint8_t *)nvs_feat_list[feat_index].header_p;

    SAVE_INTERRUPTS(ccr);
    for (i = 0u ; i < NVS_HEADER_SIZE ; i++)
    {
        nvs_buffer[i] = header_as_bytes[i];
    }
    /* now that header has been copied to buffer, clear the request */
    nvs_write_status[feat_index].write_request 
                                &= (uint8_t)~NVS_WRITE_HEADER_REQUEST;
    RESTORE_INTERRUPTS(ccr);
}

/**
 * Copies a feature RAM image, excluding reserved_bytes, to #nvs_buffer,
 * prepares it for writing to the EEPROM and then writes it.
 * @returns status returned by nvs_write_eeprom()
 *
 * Special care is taken to ensure the new write_count is not equal
 * to last_read, which could happen if there have been 256 writes since the
 * last read.  If write_count has wrapped round to last_read, then last_read is
 * reduced by one to make sure that the bank does still show unread data.
 *
 * If the save is marked as a checkpoint, last_read is set equal to
 * write_count.  When the header is read back, it is regarded as having no 
 * unread_data.
 * This allows partial checkpoint writes of a feature to be made
 * whilst still detecting the absence of a final eeprom write at strategy 
 * shutdown
 *
 */
PRIVATE Eeprom_Op_Status nvs_bg_write_all_to_bank(
                    Nvs_feature_list_e feat_index, nvs_bank_e bank_to_write,
                    bool_t checkpoint)
{
    uint16_t bytes_to_copy;
    uint16_t data_size;
    FRAM nvs_header_s *buffer_header_p = (FRAM nvs_header_s *)nvs_buffer;

    data_size = nvs_feat_list[feat_index].data_size;
    bytes_to_copy = data_size + NVS_HEADER_SIZE;

    nvs_bg_copy_all_to_buffer(feat_index);

    buffer_header_p->data_crc = nvs_calc_data_crc_of_buffer(data_size);
    buffer_header_p->write_count = 
                            nvs_write_status[feat_index].next_write_count;
    if (checkpoint)
    {
        /* When this header is read back, it should appear as though the final
         * write was lost (because it may yet be)  */
        buffer_header_p->last_read = buffer_header_p->write_count;
    }
    else if (buffer_header_p->last_read == buffer_header_p->write_count)
    {
        /* write counter has gone all the way round to last_read so bank
         * would look like there was no unread data.  Solve this by
         * decrementing last_read
         */
        buffer_header_p->last_read--;
    }
    else
    {
        /* not a checkpoint and no wrap */
    }

    nvs_update_header_in_buffer();
    nvs_update_write_status(feat_index);
    return nvs_write_eeprom(feat_index,bank_to_write,bytes_to_copy);
}


/**
 * Copies a feature NVS header and data to #nvs_buffer atomically and
 * clears NVS_WRITE_DATA_REQUEST bit in nvs_write_status[].write_request
 */
PRIVATE void nvs_bg_copy_all_to_buffer(Nvs_feature_list_e feat_index)
{
    uint8_t ccr;
    uint16_t bytes_to_copy;
    uint16_t i;
    FRAM uint8_t *image_as_bytes = 
                    (FRAM uint8_t *)nvs_feat_list[feat_index].header_p;

    bytes_to_copy = NVS_HEADER_SIZE + nvs_feat_list[feat_index].data_size;

    SAVE_INTERRUPTS(ccr);
    for (i = 0u ; i < bytes_to_copy ; i++)
    {
        nvs_buffer[i] = image_as_bytes[i];
    }
    /* now that header has been copied to buffer, clear the request */
    nvs_write_status[feat_index].write_request 
        &= (uint8_t)~(NVS_WRITE_DATA_REQUEST|NVS_WRITE_CHECKPOINT_REQUEST);
    RESTORE_INTERRUPTS(ccr);
}

/**
 * Prepares a header structure in #nvs_buffer for writing to the EEPROM by
 * replacing the restore_status with #NVS_HEADER_PATTERN and updating the
 * header CRC.
 */
PRIVATE void nvs_update_header_in_buffer(void)
{
    FRAM nvs_header_s *buffer_header_p = (FRAM nvs_header_s *)nvs_buffer;

    buffer_header_p->restore_status = (data_validity_e) NVS_HEADER_PATTERN;
    buffer_header_p->header_crc = nvs_calc_header_crc_of_buffer();
}

/**
 * Update feature write status to be ready for the next time the
 * feature data is written to EEPROM
 */
PRIVATE void nvs_update_write_status(Nvs_feature_list_e feat_index)
{
    FRAM nvs_write_status_s *write_status_p = &nvs_write_status[feat_index];

    write_status_p->next_write_count++;

    switch (write_status_p->next_bank_to_write_data)
    {
        case NVS_bank_0:
            write_status_p->next_bank_to_write_data = NVS_bank_1;
            break;
        case NVS_bank_1:
            write_status_p->next_bank_to_write_data = NVS_bank_0;
            break;
        case NVS_bank_both:
            /* if both banks were being updated, then this function will have
             * been called during the first update to bank 0, therefore the
             * next write should be to bank 1 */
            write_status_p->next_bank_to_write_data = NVS_bank_1;
            break;
        case NVS_bank_none:
        default:
            /* shouldn't happen - logic has gone awry so mark both banks as
             * needing writing */
            write_status_p->next_bank_to_write_data = NVS_bank_both;
    }
}

/**
 * Writes a feature to a given bank on the EEPROM.
 *
 * The image to write to the EEPROM must already be in #nvs_buffer
 * @param feat_index    feature being written
 * @param bank          Bank to write.  must be NVS_bank_0 or NVS_bank_1
 * @param bytes_to_copy Number of bytes to be written
 * @returns             EEPROM status returned by eeprom_write_data()
 */
PRIVATE Eeprom_Op_Status nvs_write_eeprom(Nvs_feature_list_e feat_index,
                                        nvs_bank_e bank,uint16_t bytes_to_copy)
{
    uint16_t eeprom_address;

    eeprom_address = nvs_feature_eeprom_address(feat_index,bank);
    return eeprom_write_data(eeprom_address,nvs_buffer,bytes_to_copy);
}

#ifdef PRODDER
#include "prodder_nvs.h"

/**
 * Provide access to feature NVS data from the prodder.  Placed here to
 * avoid the complexity of having save/restore functions being PRIVATE in
 * the app but public in the prodder.
 */
PRIVATE void prodder_nvs_write_feature(void);

PRIVATE PROD_nvs_command_code_e PROD_nvs_command_code = Prod_nvs_idle;
PRIVATE data_validity_e PROD_nvs_restore_status = No_data_available;
PRIVATE data_validity_e PROD_nvs_write_status = No_data_available;
PRIVATE Nvs_feature_list_e PROD_nvs_feature = (Nvs_feature_list_e) 0;

PRIVATE bool_t offsets_calculated = FALSE;

void prodder_nvs_machine(void)
{
    if (!offsets_calculated)
    {
        nvs_calculate_feature_offsets();
        offsets_calculated = TRUE;
    }
    switch (PROD_nvs_command_code)
    {
        case Prod_nvs_read_feature:
            PROD_nvs_restore_status = nvs_restore_feature(PROD_nvs_feature);
            PROD_nvs_command_code = Prod_nvs_idle;
            break;
        case Prod_nvs_write_feature:
            prodder_nvs_write_feature();
            PROD_nvs_command_code = Prod_nvs_idle;
            break;
        case Prod_nvs_idle:
        default:
            /* idle: do nothing */
            break;
    }
}

PRIVATE void prodder_nvs_write_feature(void)
{
    switch (PROD_nvs_write_status)
    {
        case New_data_available:
            /* save the NV data to both banks */
            nvs_write_status[PROD_nvs_feature].next_bank_to_write_data
                                                = NVS_bank_both;
            nvs_bg_write_data(PROD_nvs_feature,FALSE);
            break;
        case Old_data_available:
            /* 
             * save the NV data to both banks but adulterate the counters
             * so that on restoration it won't appear as new data
             */
            nvs_write_status[PROD_nvs_feature].next_write_count = 0u;
            nvs_bg_write_all_to_bank(PROD_nvs_feature,NVS_bank_0,FALSE);
            nvs_write_status[PROD_nvs_feature].next_write_count = 1u;
            nvs_bg_write_all_to_bank(PROD_nvs_feature,NVS_bank_0,FALSE);
            break;
        case No_data_available:
        default:
            /*
             * just trash the headers by making sure the first byte isn't
             * NVS_HEADER_PATTERN
             */
            nvs_buffer[0] = (uint8_t) ~NVS_HEADER_PATTERN;
            nvs_write_eeprom(PROD_nvs_feature,NVS_bank_0,1u);
            nvs_write_eeprom(PROD_nvs_feature,NVS_bank_1,1u);
            break;
    }
}
#endif /* ifdef PRODDER */
