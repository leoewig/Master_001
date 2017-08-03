/**
 * @file
 * @ingroup danomem
 *
 * Dan-o-mem module implementation
 *
 * $Id: danomem.c 10338 2013-08-30 16:18:35Z pr4 $
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#ifdef MMC
#include "cal.h"
#endif
#include "danomem.h"
#include "danomem_p.h"


/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/**
 * Memory reader
 *
 * @param[in]   type    a danomem_type_t type denoting desired data type to be read
 * @param[in]   addr_p  a global address to be read from
 * @param[out]  out     a uint32_t type pointer to be written with result. Data are
 *                      packed according to Dan-O-mem protocol
 * @return DM_res_ok    if operation was successful
 *         DM_res_err   if error has occurred (unknown type)
 */
 danomem_res_t danomem_read_data(danomem_type_t type, volatile const FPTR void *addr_p,
    uint32_t *out)
{
    danomem_res_t res = DM_res_ok;
    uint8_t ccr;

    /* Enter critical section to avoid data corruption */
    SAVE_INTERRUPTS(ccr);
    switch (type)
    {
        case DM_type_8:
            /* read one byte and place it to msb location of uint32_t output */
            *out = (uint32_t) (((uint32_t) *(FPTR uint8_t *) addr_p) << 24);
            break;
        case DM_type_16:
            /* read two bytes and place them to msb location of uint32_t output */
            *out = (uint32_t) (((uint32_t) *(FPTR uint16_t *) addr_p) << 16);
            break;
        case DM_type_24:
            /* read two bytes and place them to msb location of uint32_t output */
            *out = (uint32_t) (((uint32_t) *(FPTR uint16_t *) addr_p) << 16);
            /* read one more byte and place it to second lsb byte location of uint32_t output */
            *out |= (uint32_t) (((uint32_t) *((FPTR uint8_t *) addr_p + 2u)) << 8);
            break;
        case DM_type_32:
            /* read four bytes */
            *out = *(FPTR uint32_t *) addr_p;
            break;
        default:
            /* got a Dan-o-mem request with an unknown type */
            res = DM_res_err;
            break;
    }
    RESTORE_INTERRUPTS(ccr);
    return res;
}

/**
 * Memory writer
 *
 * @param[in] type      a danomem_type_t type denoting desired data type to be written
 * @param[in] addr_p    a global address to be written to
 * @param[in] data      a uint32_t type data value to be written. Data are masked
 *                      out according to type in order to perform desired write operation
 *                      type.
 * @return DM_res_ok    if operation was successful
 *         DM_res_err   if error has occurred (unknown type)
 */

danomem_res_t danomem_write_data(danomem_type_t type, volatile FPTR const void *addr_p,
    uint32_t data)
{
    danomem_res_t res;

#ifndef MMC
    /* VTBM and prodders always writes directly to memory */
    res = danomem_native_write(type,(FPTR const void *)addr_p,data);
#else
    /* 
     * MMC has calibration RAM so route writes through the calibration
     * feature in the first instance, and only attempt to write directly if
     * that fails
     */
    res = danomem_cal_write(type,(FPTR void *)addr_p,data);
    if (res == DM_res_err)
    {
        res = danomem_native_write(type,(FPTR const void *)addr_p,data);
    }
#endif
    return res;
}

PRIVATE danomem_res_t danomem_native_write(danomem_type_t type,
                                            const FPTR void *addr_p,
                                            uint32_t data)
{
    danomem_res_t res = DM_res_ok;
    uint8_t ccr;

    /* Enter critical section to avoid data corruption */
    SAVE_INTERRUPTS(ccr);

    /**
     * @lint Investigate : unusual pointer cast\n
     * -e{740}
     */
    /*lint -e{740} */
    switch (type)
    {
        case DM_type_8:
            /* Write one byte with msb byte of input data only */
            *(FPTR uint8_t *) addr_p = *(uint8_t *) &data;
            break;
        case DM_type_16:
            /* Write two bytes with 2 msb bytes of input data only */
            *(FPTR uint16_t *) addr_p = *(uint16_t *) &data;
            break;
        case DM_type_24:
            /* Write three bytes with 3 msb bytes of input data */
            *(FPTR uint16_t *) addr_p = *(uint16_t *) &data;
            *((FPTR uint8_t *) addr_p + 2u) = *((uint8_t *) &data + 2u);
            break;
        case DM_type_32:
            /* Write four bytes with all 4 bytes of input data */
            *(FPTR uint32_t *) addr_p = *(uint32_t *) &data;
            break;
        default:
            /* got a Dan-o-mem request with an unknown type */
            res = DM_res_err;
            break;
    }
    RESTORE_INTERRUPTS(ccr);
    return res;
}



/**
 * Convert two digit number in ascii format into binary format
 *
 * @param[in] hi    ascii coded hi nibble
 * @param[in] lo    ascii coded lo nibble
 * @return      converted number
 */
uint8_t ascii2bin(CONST char_t hi, CONST char_t lo)
{
    uint8_t out;
    out = (uint8_t) (((uint8_t) hi & 0x0Fu) << 4u) | ((uint8_t) lo & 0x0Fu);
    return out;
}

#ifdef MMC
/**
 * Calls the cal_write() to put a Danomem write into calibration RAM and eeprom.
 * @retval DM_res_ok    Request was successful
 * @retval DM_res_err   Request failed - probably because addr_p doesn't
 *                      point to the calibration area
 */
PRIVATE danomem_res_t danomem_cal_write(danomem_type_t type,
                                            FPTR void *addr_p,
                                            uint32_t data)
{
    uint8_t ccr;
    uint8_t length;
    FPTR uint8_t *data_p;       /* cal_write() takes the data as a pointer */
    danomem_res_t dano_res = DM_res_ok;
    cal_status_e cal_res;

    data_p = (FPTR uint8_t *)&data;

    switch (type)
    {
        case DM_type_8:
            length = 1u;
            break;
        case DM_type_16:
            length = 2u;
            break;
        case DM_type_24:
            length = 3u;
            break;
        case DM_type_32:
            length = 4u;
            break;
        default:
            /* unknown type so unknown length */
            dano_res = DM_res_err;
            length = 0u;        /* dummy write to calm lint */
            break;
    }

    if (dano_res == DM_res_ok)
    {
        SAVE_INTERRUPTS(ccr);
        cal_res = cal_write(Cal_slave_danomem,addr_p,length,data_p);
        RESTORE_INTERRUPTS(ccr);

        switch (cal_res)
        {
            case Cal_success:
                dano_res = DM_res_ok;
                break;

            case Cal_pending:
                dano_res = DM_res_pend;
                break;

            case Cal_failure:
            default:
                dano_res = DM_res_err;
                break;
        }
    }
    return dano_res;
}
#endif
