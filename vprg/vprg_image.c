/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      vprg_image.c 
 *
 * description:  
 * VTBM reprogramming image access
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/


#include <string.h>
#include "basic_types.h"
#include "hardware.h"
#include "crc.h"
#include "vprg.h"
#include "vprg_p.h"


/******************************************************************************
 * COMPILE-TIME CHECKS
 *****************************************************************************/

/*
 * PUBLIC DATA
 */

/**
 * Last image that was checked and found to be ok.
 */
image_index_t vprg_last_checked_image = VPRG_NO_IMAGE;

/**
 * Error detected by the last completed image check.
 * @a2l{conversion,vprg_image_error_e}
 */
vprg_image_error_e vprg_image_error = Vprg_image_error_none;

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

/**
 * The image folder as located by the .lkf file.
 * @note The initialiser will not end up in the binary because it is only
 * there for lint and hosted testing's benefit.
 * The actual image folder will be appended by post-processing.
 */
LINKER_SYMBOL(FROM image_folder_header_s image_folder, 
                                        IMAGE_FOLDER_LINT_INITIALISER);

/**
 * Image checker state
 * @a2l{conversion,vprg_ci_state_e}
 */
PRIVATE vprg_ci_state_e vprg_ci_state = Vprg_ci_state_none;

/**
 * Index within the image folder of the image to be checked
 * (0 -> the first image)
 */
PRIVATE image_index_t vprg_image_to_check = 0u;


/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/**
 * Called as part of the MMC's idle loop to check an image
 */
void vprg_check_image_idle_task(void)
{
    switch (vprg_ci_state)
    {
        case Vprg_ci_state_checking:
            vprg_ci_state = vprg_check_folder(vprg_image_to_check);
            break;
        case Vprg_ci_state_none:
        case Vprg_ci_state_ok:
        case Vprg_ci_state_error:
            /* nothing in progress so nothing to do */
            break;
        default:
            VPRG_INTERNAL_ERROR();
            break;
    }
}

/**
 * Called from the main (foreground) vprg state machine to start an image
 * check
 */
void vprg_check_image_request(image_index_t image_index)
{
    if (vprg_ci_state == Vprg_ci_state_checking)
    {
        /* a check is in progress - this shouldn't be possible */
        VPRG_INTERNAL_ERROR();
    }
    vprg_image_to_check = image_index;
    vprg_image_error = Vprg_image_error_none;

    vprg_ci_state = Vprg_ci_state_checking;
}

vprg_ci_state_e vprg_check_image_status(void)
{
    return vprg_ci_state;
}


/**
 * Get the image header corresponding to the given image_index.  If
 * image_index is too big for the folder, returns #FNULL
 */
FPTR image_header_s *vprg_get_image_header(image_index_t image_index)
{
    FPTR image_header_s *header_p;
    FPTR uint8_t *folder_address;
    FPTR uint8_t *image_address;

    if (vprg_check_image_index(image_index))
    {
        /* get a byte pointer to ther folder */
        folder_address = (FPTR uint8_t *) &image_folder;
        /* add the image offset to get the image address */
        image_address = folder_address
                                + image_folder.image_offset[image_index];
        /** @lint --e{826} suspicious pointer-to-pointer conversion:
         * cast the byte pointer to and image header pointer */
        /*lint -e{826} */
        header_p = (FPTR image_header_s *) image_address;
    }
    else
    {
        header_p = FNULL;
        /* vprg_image_error will have been set by 
         * vprg_check_image_index() */
    }
    return header_p;
}


/**
 * Get a pointer to the first sector, which immediately follows the
 * image header
 */
FPTR image_sector_header_s *vprg_first_sector( 
                                    FPTR const image_header_s *image_header_p)
{
    FPTR uint8_t *first_sector_address;
    FPTR image_sector_header_s *first_sector_p;

    /*
     * the first sector will start immediately after the image header,
     * so get a byte pointer from image_header_p and add its size to it
     */
    first_sector_address = (FPTR uint8_t *)image_header_p 
                                + sizeof(image_header_s);
    /** @lint -e{826} suspicious pointer assignment: cast the byte pointer
     * to an image_sector_header pointer */
    /*lint -e{826} */
    first_sector_p = (FPTR image_sector_header_s *) first_sector_address;
    return first_sector_p;
}

/**
 * Get a pointer to the sector following the current one.
 *
 * @param image_sector_p        Pointer to the current sector
 * @returns pointer to the next sector
 * @note if image_sector_p points to the last sector in the image, the
 * value returned will _not_ be a valid sector pointer.  Therefore the caller
 * must keep track of the number of sectors in the image (as given by the
 * number_of_sectors field in the image header).
 */
FPTR image_sector_header_s *vprg_next_sector( 
                            FPTR const image_sector_header_s *image_sector_p)
{
    uint16_t sector_length;
    FPTR uint8_t *this_sector_address;
    FPTR uint8_t *next_sector_address;
    FPTR image_sector_header_s *next_sector_p;

    sector_length = vprg_sector_length(image_sector_p);

    /*
     * get a byte pointer to the start of the current sector
     */
    this_sector_address = (FPTR uint8_t *)image_sector_p;
    /*
     * add the length of the sector to it to get the start of the next one
     */
    next_sector_address = this_sector_address + sector_length;
    /*
     * @lint -e{826} suspicious pointer assingment: cast the byte pointer
     * back to an image sector pointer
     */
    /*lint -e{826} */
    next_sector_p = (FPTR image_sector_header_s *) next_sector_address ;
    return next_sector_p;
}


/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/**
 * Check in turn that the image folder, the desired image and the sectors
 * within the image are valid
 */
PRIVATE vprg_ci_state_e vprg_check_folder(image_index_t image_index)
{
    bool_t ok;
    vprg_ci_state_e return_state;

    ok = vprg_check_folder_header();
    if (ok)
    {
        ok = vprg_check_image(image_index);
    }

    if (ok)
    {
        return_state = Vprg_ci_state_ok;
        vprg_last_checked_image = image_index;
    }
    else
    {
        return_state = Vprg_ci_state_error;
        /* vprg_image_error will have been set by one of the
         * sub-functions */
    }
    return return_state;
}

/**
 * Check the format of the folder header by checking that it starts with
 * the correct header string, has a sensible length and ends with the correct
 * CRC
 */
PRIVATE bool_t vprg_check_folder_header(void)
{
    bool_t ok;

    ok = vprg_check_folder_header_string();
    if (ok)
    {
        ok = vprg_check_folder_header_contents();
    }
    if (ok)
    {
        ok = vprg_check_folder_header_crc();
    }

    return ok;
}

/**
 * Check that the image folder has the correct header string
 */
PRIVATE bool_t vprg_check_folder_header_string(void)
{
    bool_t ok;

    if (STRNCMP((FPTR char_t *)image_folder.header_string,
            (FPTR char_t *)FOLDER_HEADER_STRING,FOLDER_HEADER_STRING_LEN) != 0)
    {
        /* header string doesn't match */
        vprg_image_error = Vprg_image_error_no_folder;
        ok = FALSE;
    }
    else
    {
        ok = TRUE;
    }
    return ok;
}

/**
 * Check the number of images stored and the length of the header (given
 * by the offset of the first image) against limits.
 */
PRIVATE bool_t vprg_check_folder_header_contents(void)
{
    bool_t ok;
    uint32_t header_length;

    header_length = image_folder.image_offset[0];
    if ((header_length > IMAGE_FOLDER_HEADER_MAX_LENGTH)
            || (image_folder.number_of_images > IMAGE_FOLDER_MAX_IMAGES))
    {
        vprg_image_error = Vprg_image_error_folder_corrupt;
        ok = FALSE;
    }
    else
    {
        ok = TRUE;
    }
    return ok;
}

/**
 * The image folder header has a trailing CRC so the CRC of the whole
 * record must be zero.  The length of the header is the same as the offset
 * of the first image.
 */
PRIVATE bool_t vprg_check_folder_header_crc(void)
{
    bool_t ok;
    uint16_t header_crc;

    header_crc = crc16(CRC16_SEED,(const FPTR uint8_t *)&image_folder,
                            (uint16_t) image_folder.image_offset[0]);
    if (header_crc != 0u)
    {
        vprg_image_error = Vprg_image_error_folder_corrupt;
        ok = FALSE;
    }
    else
    {
        ok = TRUE;
    }

    return ok;
}

/**
 * Check the image at the given index by checking:
 * + the index is within range
 * + the image header has the correct format
 * + each of the sectors has the correct format
 */
PRIVATE bool_t vprg_check_image(image_index_t image_index)
{
    bool_t ok;
    FPTR image_header_s *image_header_p;

    ok = TRUE;

    image_header_p = vprg_get_image_header(image_index);
    if (image_header_p != FNULL)
    {
        ok = vprg_check_image_header(image_header_p);
        if (ok)
        {
            ok = vprg_check_image_sectors(image_header_p);
        }
    }
    else
    {
        /* image_header_p is FNULL, so image_index was too big.
         * vprg_image_error will have been set in the subfunction
         */
        ok = FALSE;
    }
    return ok;
}
/**
 * Check that an image index is small enough to index an image within the
 * folder
 */
PRIVATE bool_t vprg_check_image_index(image_index_t image_index)
{
    bool_t ok;

    if (image_index >= image_folder.number_of_images)
    {
        vprg_image_error = Vprg_image_error_bad_image_index;
        ok = FALSE;
    }
    else
    {
        ok = TRUE;
    }
    return ok;
}

/**
 * Check that an image header starts with the correct header string and ends
 * with the correct CRC
 */
PRIVATE bool_t vprg_check_image_header(
                                FPTR const image_header_s *image_header_p)
{
    bool_t ok;
    ok = vprg_check_image_header_string(image_header_p);
    if (ok)
    {
        ok = vprg_check_image_crc(image_header_p);
    }
    return ok;
}

/**
 * Check the image header has te correct header string
 */
PRIVATE bool_t vprg_check_image_header_string(
                                    FPTR const image_header_s *image_header_p)
{
    bool_t ok;

    if (STRNCMP((FPTR char_t *)image_header_p->header_string,
            (FPTR char_t *)IMAGE_HEADER_STRING,IMAGE_HEADER_STRING_LEN) != 0)
    {
        ok = FALSE;
        vprg_image_error = Vprg_image_error_bad_image_header;
    }
    else
    {
        ok = TRUE;
    }
    return ok;
}

/**
 * Check that the CRC of the whole image header (which includes a trailing
 * CRC word) is zero
 */
PRIVATE bool_t vprg_check_image_crc(FPTR const image_header_s *image_header_p)

{
    bool_t ok;
    uint16_t crc;

    crc = crc16(CRC16_SEED,(FPTR uint8_t *) image_header_p,
                        sizeof(image_header_s));
    if (crc == 0u)
    {
        ok = TRUE;
    }
    else
    {
        ok = FALSE;
        vprg_image_error = Vprg_image_error_bad_image_header;
    }
    return ok;
}

/**
 * Check that every image sector is valid
 */
PRIVATE bool_t vprg_check_image_sectors(
                                FPTR const image_header_s *image_header_p)
{
    bool_t ok;
    FPTR image_sector_header_s *sector_header_p;
    uint16_t i;

    ok = TRUE;
    sector_header_p = vprg_first_sector(image_header_p);
    for (i = 0u ; ok && (i < image_header_p->number_of_sectors); i++)
    {
            ok = vprg_check_sector(sector_header_p);
            sector_header_p = vprg_next_sector(sector_header_p);
    }
    return ok;
}
/**
 * Calculate the length of a sector from its number_of_bytes field and the
 * fixed addtional number of bytes around it
 */
PRIVATE uint16_t vprg_sector_length(
                            FPTR const image_sector_header_s *image_sector_p)
{
    uint16_t sector_length;

    sector_length = SECTOR_HEADER_STRING_LEN
                        + sizeof(image_sector_p->number_of_bytes) 
                        + sizeof(image_sector_p->address) 
                        + image_sector_p->number_of_bytes /* data bytes */
                        + sizeof(uint16_t);     /* and check sum at end */
    return sector_length;
}

/**
 * Check that format of a sector by confirming it has the correct header
 * string, a sensible length and a correct CRC at the end.
 */
PRIVATE bool_t vprg_check_sector(
                            FPTR const image_sector_header_s *image_sector_p)
{
    bool_t ok;

    ok = vprg_check_sector_header_string(image_sector_p);
    if (ok)
    {
        ok = vprg_check_sector_length(image_sector_p);
    }
    if (ok)
    {
        ok = vprg_check_sector_crc(image_sector_p);
    }
    return ok;
}

/**
 * Check that a sector starts with the correct header string
 */
PRIVATE bool_t vprg_check_sector_header_string(
                            FPTR const image_sector_header_s *image_sector_p)
{
    bool_t ok;

    if (STRNCMP((FPTR char_t *)image_sector_p->header_string,
                (FPTR char_t *)SECTOR_HEADER_STRING,SECTOR_HEADER_STRING_LEN)
            == 0)
    {
        ok = TRUE;
    }
    else
    {
        ok = FALSE;
        vprg_image_error = Vprg_image_error_bad_sector;
    }
    return ok;
}

/**
 * Check that the sector number_of_bytes is not too large
 */
PRIVATE bool_t vprg_check_sector_length(
                            FPTR const image_sector_header_s *image_sector_p)
{
    bool_t ok;
    if (image_sector_p->number_of_bytes > IMAGE_SECTOR_MAX_DATA_BYTES)
    {
        ok = FALSE;
        vprg_image_error = Vprg_image_error_bad_sector;
    }
    else
    {
        ok = TRUE;
    }

    return ok;
}

/**
 * Confirm that the CRC of the entire sector structure - including the
 * trailing crc word - is zero
 */
PRIVATE bool_t vprg_check_sector_crc(
                            FPTR const image_sector_header_s *image_sector_p)
{
    bool_t ok;
    uint16_t sector_length;
    uint16_t crc;

    sector_length = vprg_sector_length(image_sector_p);

    crc = crc16(CRC16_SEED,(FPTR uint8_t *)image_sector_p, sector_length);

    if (crc == 0u)
    {
        ok = TRUE;      /* the sector has a correct CRC word at the end */
    }
    else
    {
        ok = FALSE;     /* CRC word doesn't match the sector data */
        vprg_image_error = Vprg_image_error_bad_sector;
    }
    return ok;
}
