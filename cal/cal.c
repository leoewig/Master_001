/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      cal.c 
 *
 * description:   XCP calibration support source
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           XCP calibration support source
 *
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"           /* for SAVE/RESTORE_INTERRUPTS */
#include "nvs.h"
#include "crc.h"
#include "eeprom.h"
//#include "ican.h"               /* for CAN_MAX_DATA */
#include "exc.h"                /* for ApplXcpSend() */
#include "ixcp.h"               /* for the XCP definitions to put in messages
                                    when calling ApplXcpSend() */
//#include "obd_ids_types.h"      /* for OBD_DEFAULT_VERSION */
#include "mmc_danomem.h"
#include "cal.h"
#include "cal_p.h"
#include "watchdog.h"
#include "ts.h"
#include "fm.h"

/*
 * sanity check that NVS and CAL both fit on the EEPROM
 */
#if NVS_EEPROM_END_ADDRESS > CAL_EEPROM_START_ADDRESS
#error  NVS and CAL EEPROM areas overlap
#endif

/*
 * Symbols defined in the linker file.  We get their values in C by taking
 * their address
 */
LINKER_SYMBOL(FRAM uint8_t calram_s12_start,0u);
LINKER_SYMBOL(uint8_t calram_s12_size,0u);
LINKER_SYMBOL(FRAM uint8_t calram_start,0u);
LINKER_SYMBOL(uint8_t calram_size,0u);

/**
 * Calibration support information stored in NV RAM
 * @note this is stored separately from the actual calibration data which
 * is stored elsewhere on the EEPROM
 *
 * The interesting bit is the cal_nvs_data_s structure at its heart.
 */
FRAM cal_nvs_wrapper_s cal_ram_image = {
                                        NVS_HEADER_INIT(CAL_NVS_LAYOUT_VERSION),
                                        {
                                            FALSE,     /* eeprom_contains_cal */
                                            FALSE,      /* using_eeprom_cal */
                                            0u,         /* cal_checksum */
                                            {0u,0u}     /* flash CRCs */
                                        },
                                        NVS_RESERVED_BYTES_INIT
                                    };

/**
 * Array describing the calibration areas in the strategy.
 */
PRIVATE CONST cal_areas_s cal_areas[CAL_N_SECTIONS] = {
    /**
     * @lint --esym{923}
     * cast from pointer to unsigned int:
     * calram_size and calram_s12_size aren't really addresses -
     * they are assembly language symbols set with the \#size() macro.  We
     * get them into C by taking the address of the C symbol.  Lint, quite
     * understandably, finds this reprehensible.
     *
     * @lint -esym(551,calram_size,calram_s12_size) Symbol not accessed:
     * More curiously, lint thinks the symbols aren't accessed even though we
     * 'take their addresses'
     */
    /*lint --e{923} */
    /*lint -esym(551,calram_size,calram_s12_size) */
    { 
        (FPTR uint8_t *) &calram_start,
#ifndef HOSTED
        (uint16_t) &calram_size
#else
        0u
#endif

    },
    { 
        (FPTR uint8_t *) &calram_s12_start,
#ifndef HOSTED
        (uint16_t) &calram_s12_size,
#else
        0u
#endif
    }
};

/**
 * Keeps track of the notional calibration page we are using.
 * Defaults to the RAM page and only gets set to the flash page after the
 * calibration tool explictly sets the page to page 0
 */
PRIVATE cal_page_e cal_page = Cal_page_ram;

/**
 * Structure used to pass requests from the foreground to the background
 */
PRIVATE cal_eeprom_request_s cal_eeprom_requests = { 
                                    0u, /* initialise with no requests set */
                                    Cal_slave_canbedded,
                                    FNULL,
                                    0u
                                } ;

/**
 * Records the time taken to reload the base calibration stored in the RAM 
 * initialisation table.
 */
PRIVATE ts_time_t   cal_base_cal_reload_time_ms = 0;

#define MEMORY_LOCATION S12_MEMORY
#include "memory_layout.h"

/**
 * Calibration version in BCD YYWW date format reported over diagnostics
 * @a2l{LongIdentifier,Calibration version number in YYWW BCD format. e.g. 0x1301 (4865 decimal) is 2013 week 1}
 */
//CAL uint16_t calibration_version = OBD_DEFAULT_VERSION;
CAL uint16_t calibration_version = 0;
/**
 * Calibration ID
 * @a2l{type,ASCII}
 * @a2l{LongIdentifier,Calibration Identification string}
 */
CAL char_t calibration_id[CALIBRATION_ID_LENGTH+1u] = "P12_BMS_CAL01";

/**
 * Test calibration to let us monitor that the calibration system is working.
 * Assigned to the test variable #cal_test_var.
 *
 * Defaults to an arbitrary non-zero value
 */
PRIVATE CAL uint8_t cal_test_cal = CAL_TEST_CAL_DEFAULT;

/**
 * Test variable to let us monitor that the calibration system is working.
 * Assigned from the test cal #cal_test_cal in cal_idle_tasks()
 *
 * @lint -esym(551,cal_test_var) Test variable is not referenced
 */
/*lint -esym(551,cal_test_var) */
PRIVATE uint8_t cal_test_var = 0u;

/** Remember whether a cal error was detected on initialisation */
PRIVATE bool_t cal_error_present=FALSE;

/**
 * Initialise calibration
 *
 * If a calibration is stored in EEPROM, looks to see if it is usable and,
 * if it is, loads it.
 *
 * If a stored calibration is not found to be usable - presumably because the
 * module has been reprogrammed with a different binary - it is discarded
 */
void cal_init(void)
{
    bool_t cal_loaded;

    /* to keep Lint happy - we use this parameter for debugging PATs */
    (void)cal_base_cal_reload_time_ms;

    if ((NVS_RESTORE_STATUS(cal_ram_image) == No_data_available)
        || !cal_ram_image.data.eeprom_contains_cal)
    {
        /* no cal stored - do nothing */
    }
    else if (!cal_is_eeprom_cal_current())
    {
        /*
         * eeprom_contains_cal is TRUE but it does not appear to be current -
         * perhaps the module has been reflashed.  Discard it to make
         * future start-up time quicker
         */
        cal_discard_eeprom();
    }
    else if (cal_ram_image.data.using_eeprom_cal)
    {
        /*
         * Cal appears to be valid, and we were using it before the last
         * shutdown, so try reloading it.
         */
        cal_loaded = (cal_load_cal_from_eeprom() == Cal_success);
        if (!cal_loaded)
        {
            /* Remember that there was an error, so that it can be reported
             * once fm has been initialised */
            cal_error_present=TRUE;
            /*
             * something went wrong reading the cal from EEPROM so reload
             * the flash calibration
             */
            (void) cal_reload_flash_cal();
            cal_discard_eeprom();
        }
    }
    else
    {
        /*
         * We have a valid image in the EEPROM but we are not currently
         * using it - do nothing
         */
    }
}

/** Record a failed startup cal read with fm */
void cal_init_failure_management(void)
{
    (void) fm_update_fault_filter(Fm_plat_calibration_invalid,
            Fm_mmc,cal_error_present,TRUE);
}

/**
 * A background task to perform writing to the EEPROM.
 *
 * Requests are passed from the foreground in #cal_eeprom_requests.
 * If an action is performed, a DTO is sent by calling cal_send_dto().
 */ 
void cal_idle_tasks(void)
{
    uint8_t ccr;
    cal_eeprom_request_s local_eeprom_requests;
    cal_status_e status;

    SAVE_INTERRUPTS(ccr);
    /*
     * take a local copy atomically so that the foreground can continue doing
     * its thing
     */
    local_eeprom_requests = cal_eeprom_requests;
    /* 
     * and clear the requests so that the foreground can safely start setting
     * them again
     */
    CAL_CLEAR_REQUESTS(cal_eeprom_requests);
    RESTORE_INTERRUPTS(ccr);

    /*
     * There should only be one request at a time because master should wait for
     * response from one before sending another.  Therefore we don't worry
     * about performing multiple actions: we just pick a priority order
     * to test the flags and only perfrom a single action
     */
    if (local_eeprom_requests.request_byte & CAL_REQUEST_DISCARD_EEPROM_FLAG)
    {
        cal_discard_eeprom();
        status = Cal_success;
    }
    else if (local_eeprom_requests.request_byte & CAL_REQUEST_LOAD_EEPROM_FLAG)
    {
        status = cal_load_cal_from_eeprom();
        if (status == Cal_failure)
        {
            cal_discard_eeprom();
        }
    }
    else if (local_eeprom_requests.request_byte
                                        & CAL_REQUEST_WRITE_WHOLE_CAL_FLAG)
    {
        status = cal_write_whole_cal();
    }
    else if (local_eeprom_requests.request_byte
                                        & CAL_REQUEST_WRITE_CAL_BYTES_FLAG)
    {
        status = cal_write_eeprom_bytes(local_eeprom_requests.write_src_p,
                                        local_eeprom_requests.eeprom_address,
                                        local_eeprom_requests.write_size);
    }
    else
    {
        /* there is nothing to do so just set status to stop lint worrying */
        status = Cal_success;
    }

    if (local_eeprom_requests.request_byte != 0u)
    {
        /* we did attempt something so need to send a reply */
        cal_send_dto(local_eeprom_requests.xcp_slave,status);
    }

    /* 
     * finally, assign the test cal to the test output so that we can
     * see that calibration is working
     */
    cal_test_var = cal_test_cal;
}

/**
 * Return the notional calibration page.
 *
 * It is a _notional_ page because we always read calibration data from RAM
 * regardless of whether the data was sourced from Flash or EEPROM.
 * @retval #Cal_page_ram        The RAM page is notionally active
 * @retval #Cal_page_flash      The flash page is notionally active
 */
cal_page_e cal_get_cal_page(void)
{
    return (cal_page);
}

/**
 * Set the calibration page as requested.
 *
 * If a switch from flash page to RAM page is requested, the task is
 * deferred to the background because reading the EEPROM is a bit slow.
 *
 * @retval #Cal_success  request carried out (or no action required)
 * @retval #Cal_pending  the background has been requested to read the cal from
 *                      the EEPROM
 * @retval #Cal_failure  An attempt to read the cal from flash has failed,
 *                      see cal_reload_flash_cal()
 */
cal_status_e cal_set_cal_page(cal_xcp_slave_e slave,cal_page_e page)
{
    cal_status_e status;

    if (page == cal_page)
    {
        /* already set - nothing to do */
        status = Cal_success;
    }
    else if (page == Cal_page_ram)
    {
        if (cal_ram_image.data.eeprom_contains_cal)
        {
            /* 
             * we are switching to the RAM page and there is a calibration
             * stored in the EEPROM, so we need to copy it out into RAM
             */
            CAL_REQUEST_LOAD_EEPROM(cal_eeprom_requests,slave);
            status = Cal_pending;
        }
        else 
        {
            /* we are switching to the RAM page but have nothing stored, so just
             * note that we are now using the RAM page
             */
            cal_page = Cal_page_ram;
            status = Cal_success;
        }
    }
    else
    {
        /* new page is the flash page */
        if (cal_ram_image.data.using_eeprom_cal)
        {
            /* and we are running with a cal stored in the EEPROM, so we do
             * need to undo the changes
             */
            status = cal_reload_flash_cal();
            if (status == Cal_success)
            {
                cal_page = Cal_page_flash;
            }
        }
        else
        {
            /* no changes to make, so just note the page change */
            cal_page = Cal_page_flash;
            status = Cal_success;
        }
    }
    return status;
}

/**
 * Pretends to copy Flash calibration into the EEPROM by just making sure the
 * RAM image matches the flash image and invalidating anything stored in the
 * EEPROM
 *
 * @retval #Cal_success  No action was required
 * @retval #Cal_pending  The calibration has been restored from flash and
 *                      the background has been requested to discard the cal
 *                      from the EEPROM
 * @retval #Cal_failure  An attempt to read the cal from flash has failed.
 *                      See cal_reload_flash_cal()
 */
cal_status_e cal_copy_flash_cal(cal_xcp_slave_e slave)
{
    cal_status_e status = Cal_success;

    if (cal_ram_image.data.using_eeprom_cal)
    {
        /* we are currently using data stored in EEPROM rather than flash */
        status = cal_reload_flash_cal();
    }
    if ((status == Cal_success) && cal_ram_image.data.eeprom_contains_cal)
    {
        /*
         * There is data in the EEPROM which should never be used again -
         * this has the same effect as actually copying the flash data into
         * the EEPROM
         */
        CAL_REQUEST_DISCARD_EEPROM(cal_eeprom_requests,slave);
        status = Cal_pending;
    }
    return status;
}

/**
 * Interface function between XCP slave and EEPROM mirroring functions to
 * write calibration data.
 * @param slave         The xcp slave making the request (so that we know how
 *                      to send the DTO)
 * @param dest_p        Pointer into calibration RAM to be written
 * @param size          Number of bytes to write
 * @param src_p         Pointer to data to be written
 *
 * @retval #Cal_failure dest_address was not a valid calibration address
 * @retval #Cal_pending request has been passed to the background to be
 *                      carried out.
 */
cal_status_e cal_write(cal_xcp_slave_e slave,FPTR uint8_t *dest_p,
                        uint8_t size, FPTR const uint8_t *src_p)
{
    uint16_t eeprom_address;

    cal_status_e status;

    status = cal_find_eeprom_address(&eeprom_address,dest_p,size);

    if (status == Cal_success)
    {
        cal_write_calram(slave,src_p,dest_p,size,eeprom_address);
        status = Cal_pending;
    }
    return status;
}

/**
 * Calculates the address in EEPROM where non-volatile data for a given
 * item will be stored.
 * @param[out] eeprom_address_p Pointer through which the calculated address
 *                              is written
 * @param[in]  cal_p            Pointer to the calibratable to be saved
 * @param      size             number of bytes to be saved
 *
 * @retval #Cal_failure         Address and size doesn't lie wholly within a
 *                              calibration area.
 * @retval #Cal_success         eeprom_address has been written through
 *                              eeprom_address_p
 */
PRIVATE cal_status_e cal_find_eeprom_address(uint16_t *eeprom_address_p,
                                          FPTR const uint8_t *cal_p,
                                                uint8_t size)
{
    uint8_t i;
    cal_status_e status = Cal_failure;
    uint16_t segment_base_address;
    uint16_t offset_into_area;

    segment_base_address = CAL_EEPROM_START_ADDRESS;
    for (i = 0u ; i < CAL_N_SECTIONS ; i++)
    {
        /**
         * @lint Arithmetic with pointers
         * We need to check we are within a memory segment so have no choice
         * but to do comparisons.  All RAM memory is within a single GPAGE so
         * direct comparison of Cosmic FPTR pointers is valid
         * --e{946} Relational or subtrac toperator
         * --e{960} prohibited casts of complex expression
         */
        /*lint --e{946} --e{960}*/
        if ((cal_p >= cal_areas[i].ram_p)
             && ((cal_p + size) <= (cal_areas[i].ram_p + cal_areas[i].length))) 
        {
            /* data lies wholely within one of the calibration areas */
            offset_into_area = (uint16_t) (cal_p - cal_areas[i].ram_p);
            *eeprom_address_p = segment_base_address + offset_into_area;
                                
            status = Cal_success;
        }
        segment_base_address += cal_areas[i].length;
    }
    return status;
}

/**
 * Writes data into calibration RAM, updates the calibration checksum and
 * schedules writing to the EEPROM of both the checksum and the calibration
 * data.
 * @param slave         The xcp slave making the request (so that we know how
 *                      to send the DTO)
 * @param src_p         Pointer to data to be written
 * @param dest_p        Pointer into calibration area at which to write the
 *                      data
 * @param eeprom_address  Address in EEPROM to write data, too.
 * @param size          Number of bytes to write
 *
 */
PRIVATE void cal_write_calram(cal_xcp_slave_e slave, FPTR const uint8_t *src_p,
                    FPTR uint8_t *dest_p, uint8_t size,uint16_t eeprom_address)
{
    uint8_t i;
    uint16_t old_data_checksum;
    uint16_t new_data_checksum;

    old_data_checksum = cal_checksum(dest_p,(uint16_t) size);
    new_data_checksum = cal_checksum(src_p,(uint16_t) size);

    /* copy the data into calibration RAM */
    for (i = 0u ; i < size ; i++)
    {
        dest_p[i] = src_p[i];
    }


    /* request writing of data to EEPROM */
    if (!cal_ram_image.data.eeprom_contains_cal)
    {
        CAL_REQUEST_WRITE_WHOLE_CAL(cal_eeprom_requests,slave);
    }
    else
    {
        CAL_REQUEST_WRITE_CAL_BYTES(cal_eeprom_requests,slave,dest_p,size,
                                        eeprom_address);
        /* adjust the checksum and schedule it for writing to the EEPROM */
        cal_ram_image.data.cal_checksum += new_data_checksum-old_data_checksum;
        nvs_request_store(Nvs_Cal_feature);
    }
}

/**
 * Reloads the base calibration stored in the RAM initialisation table
 *
 * @retval #Cal_success  Base calibration copied over RAM calibration areas
 * @retval #Cal_failure  cal_find_init_locations() couldn't find the
 *                       base calibration data.
 */
PRIVATE cal_status_e cal_reload_flash_cal(void)
{
    cal_status_e status;
    FPTR const uint8_t * cal_init_table_p[CAL_N_SECTIONS];
    FPTR uint8_t *ram_p;
    FPTR const uint8_t * init_p;
    uint8_t section;
    uint16_t i;

    if (cal_ram_image.data.using_eeprom_cal == FALSE)
    {
        /* nothing to do */
        status = Cal_success;
    }
    else if (cal_find_init_locations(cal_init_table_p) == Cal_failure)
    {
        /* couldn't read the init table */
        status = Cal_failure;
    }
    else
    {
        ts_time_t start = ts_get_ms();
        kick_watchdog();
        for (section = 0u ; section < CAL_N_SECTIONS ; section++)
        {
            init_p = cal_init_table_p[section];
            ram_p = cal_areas[section].ram_p;

            for (i = 0u; i < cal_areas[section].length ; i++)
            {
                ram_p[i] = init_p[i];
            }
            /* if we are reloading lots of cals we will get a watchdog
             * reset (as found in OBD PATs) - hence need to kick it here */
            kick_watchdog();
        }
        cal_base_cal_reload_time_ms = ts_get_ms() - start;
        status = Cal_success;
    }
    return status;
}

/**
 * Called by cal_init() to check that the stored calibration is consistent
 * with the current strategy.
 *
 * This is done by comparing the CRCs of the flash calibration (which will
 * currently be in Cal RAM) with the CRCs stored in cal_ram_image
 */
PRIVATE bool_t cal_is_eeprom_cal_current(void)
{
    uint8_t i;
    uint16_t crc;
    bool_t crcs_match = TRUE;

    for (i = 0u ; i < CAL_N_SECTIONS ; i++)
    {
        crc = crc16(CRC16_SEED,cal_areas[i].ram_p,cal_areas[i].length);
        if (crc != cal_ram_image.data.flash_crc[i])
        {
            crcs_match = FALSE;
        }
    }
    return crcs_match;
}

/**
 * Loads a complete calibration from EEPROM into calibration ram.
 * Can be called either at start-up or by the calibration idle task
 * while the system is running.
 *
 * @retval #Cal_success EEPROM data written over RAM callibration areas
 * @retval #Cal_failure EEPROM couldn't be read or calibration checksum does
 *                      not match the value stored in #cal_ram_image
 */
PRIVATE cal_status_e cal_load_cal_from_eeprom(void)
{
    uint8_t i;
    cal_status_e load_ok = Cal_success;
    Eeprom_Op_Status eeprom_retcode;
    uint16_t eeprom_address;
    uint16_t section_checksum;
    uint16_t checksum;

    eeprom_address = CAL_EEPROM_START_ADDRESS;

    checksum = 0u;

    for (i = 0u ; (load_ok == Cal_success) && (i < CAL_N_SECTIONS) ; i++)
    {
        eeprom_retcode = eeprom_read_data(eeprom_address,
                                            cal_areas[i].ram_p,
                                            cal_areas[i].length);
        section_checksum = cal_checksum(cal_areas[i].ram_p,cal_areas[i].length);
        checksum += section_checksum;
        load_ok = (eeprom_retcode == Ee_driver_ready) 
                        ? Cal_success : Cal_failure;
        eeprom_address += cal_areas[i].length;
    }
    if ((load_ok == Cal_success)
            && (checksum != cal_ram_image.data.cal_checksum))
    {
        /* 
         * calibration checksum is not what we were expecting - perhaps there
         * was a power failure or reset during a write?
         */
        load_ok = Cal_failure;
    }
    return load_ok;
}

/**
 * Invalidates a stored cal by updating the cal_ram_image to show no
 * calibration is stored.  Can be called at start-up and in the background
 * while the systme is running.
 *
 * This function is called by cal_init() when the stored CRCs are found to
 * no longer match the flash calibration CRCs.  Doing this avoids wasting time
 * computing CRCs in the future.
 */
PRIVATE void cal_discard_eeprom(void)
{
    cal_ram_image.data.eeprom_contains_cal = FALSE;
    cal_ram_image.data.using_eeprom_cal = FALSE;
    nvs_request_store(Nvs_Cal_feature);
}

/**
 * finds the locations of calibration data in the RAM initialisation table
 * @param[out] init_locations_p Pointer to array of pointers containing at
 *                              least #CAL_N_SECTIONS into which the address
 *                              of calibration initialisation data is stored
 *
 * @retval      #Cal_success    Addresses found for all calibration sections
 * @retval      #Cal_failure    At least one address couldn't be found
 */
PRIVATE cal_status_e cal_find_init_locations(
                                    FPTR const uint8_t *init_locations_p[])
{
    uint8_t i;
    cal_status_e return_code = Cal_success;

    for (i = 0u ; i < CAL_N_SECTIONS ; i++)
    {
        init_locations_p[i] = 
                cosmic_init_flash_address(cal_areas[i].ram_p);
        if (init_locations_p[i] == FNULL)
        {
            /* couldn't find the address in the initialisation table */
            return_code = Cal_failure;
        }
    }
    return return_code;
}

/**
 * computes a simple 16-bit sum of bytes
 */
PRIVATE uint16_t cal_checksum(FPTR const uint8_t *p, uint16_t length)
{
    uint16_t checksum = 0u;
    uint16_t i;

    for (i = 0u ; i < length ; i++)
    {
        checksum += (uint16_t) p[i];
    }
    return checksum;
}

/**
 * Called by the background to write the whole calibration to EEPROM and
 * requests that the CAL NVS data (#cal_ram_image) is initialised.
 */
PRIVATE cal_status_e cal_write_whole_cal(void)
{
    uint8_t i;
    uint16_t eeprom_address = CAL_EEPROM_START_ADDRESS;
    Eeprom_Op_Status eeprom_stat;
    bool_t ok = TRUE;
    cal_status_e nvs_status;
    cal_status_e overall_status;

    for (i = 0u ; ok && (i < CAL_N_SECTIONS) ; i++)
    {
        eeprom_stat = eeprom_write_data(eeprom_address,cal_areas[i].ram_p,
                                cal_areas[i].length);
        eeprom_address += cal_areas[i].length;
        ok = (eeprom_stat == Ee_driver_ready);
    }

    if (ok)
    {
        nvs_status = cal_initialise_nvs_data();
        ok = (nvs_status == Cal_success);
    }

    if (ok)
    {
        overall_status = Cal_success;
    }
    else
    {
        overall_status = Cal_failure;
    }
    return overall_status;
}

/**
 * Performs blocking EEPROM write of calibration data
 */
PRIVATE cal_status_e cal_write_eeprom_bytes(FPTR const uint8_t *src_p,
                                                uint16_t eeprom_address,
                                                        uint8_t size)
{
    cal_status_e status;
    Eeprom_Op_Status eeprom_stat;

    eeprom_stat = eeprom_write_data(eeprom_address,src_p,(uint16_t)size);
    status = (eeprom_stat == Ee_driver_ready) ? Cal_success : Cal_failure;
    return status;
}

/**
 * Calls the appropriate XCP slave to send a DTO
 */
PRIVATE void cal_send_dto(cal_xcp_slave_e slave,cal_status_e status)
{
    switch (slave)
    {
        case Cal_slave_canbedded:
            cal_send_canbedded_dto(status);
            break;
        case Cal_slave_danomem:
#if!defined HOSTED
            danomem_send_pending();
#endif
            break;
        case Cal_slave_ixcp:
            cal_send_ixcp_dto(status);
            break;
        default:
            INTERNAL_ERROR();
            break;
    }
}

/**
 * function called to set #cal_ram_image when an entire new calibration is
 * written to the EEPROM, and to request that the NVS feature writes it to
 * EEPROM as soon as possible (rather than waiting for shutdown - because we
 * often pull the plug when calibrating).
 */
PRIVATE cal_status_e cal_initialise_nvs_data(void)
{
    cal_status_e status;
    uint8_t i;
    FPTR const uint8_t * cal_init_table_p[CAL_N_SECTIONS];
    cal_nvs_data_s nvs_data;
    uint8_t ccr;

    nvs_data.eeprom_contains_cal = TRUE;
    nvs_data.using_eeprom_cal = TRUE;

    if (cal_find_init_locations(cal_init_table_p) == Cal_failure)
    {
        /*
         * couldn't read the init table so can't do this calibration
         * malarkey
         */
        status = Cal_failure;
    }
    else
    {
        /* calulate CRCs */
        for (i = 0u ; i < CAL_N_SECTIONS ; i++)
        {
            nvs_data.flash_crc[i] = crc16(CRC16_SEED,cal_init_table_p[i],
                                            cal_areas[i].length);
        }

        /* calculate checksum by adding checksum of each cal section */
        nvs_data.cal_checksum = 0u;
        for (i = 0u ; i < CAL_N_SECTIONS ; i++)
        {
            nvs_data.cal_checksum += 
                        cal_checksum(cal_areas[i].ram_p,cal_areas[i].length);
        }

        SAVE_INTERRUPTS(ccr);
        cal_ram_image.data = nvs_data;
        RESTORE_INTERRUPTS(ccr);
        nvs_request_store(Nvs_Cal_feature);

        status = Cal_success;
    }
    return status;
}

/**
 * Get the CANbedded XCP slave to send a DTO.
 *
 * All the available descriptive errors - like address out of range - will have
 * been handled when the CRO was recieved.  If status is #Cal_failure, it is
 * probably down to an EEPROM failure so we can't do better than send a generic
 * error messsage
 *
 */
PRIVATE void cal_send_canbedded_dto(cal_status_e status)
{
    uint8_t msg[CAN_MAX_DATA] = { 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u } ;

    if (status == Cal_success)
    {
        msg[0] = (uint8_t) Ixcp_dto_res;
    }
    else
    {
        msg[0] = (uint8_t) Ixcp_dto_err;
        msg[1] = (uint8_t) Ixcp_err_generic;
    }

#ifndef HOSTED
    ApplXcpSend(CAN_MAX_DATA,msg);
#else
    /* add a static data interface if unit testing this function */
#endif
}


/**
 * Get the IXCP slave to send a DTO.
 *
 * All the available descriptive errors - like address out of range - will have
 * been handled when the CRO was recieved.  If status is #Cal_failure, it is
 * probably down to an EEPROM failure so we can't do better than send a generic
 * error messsage
 *
 */
PRIVATE void cal_send_ixcp_dto(cal_status_e status)
{
    uint8_t dto_data[CAN_MAX_DATA-1u] = { 0u, 0u, 0u, 0u, 0u, 0u, 0u } ;
    ixcp_dto_type_e dto_type;

    if (status == Cal_success)
    {
        dto_type = Ixcp_dto_res;
    }
    else
    {
        dto_type = ixcp_set_error(Ixcp_err_generic,dto_data);
    }
    ixcp_send_dto(dto_type,dto_data);
}

