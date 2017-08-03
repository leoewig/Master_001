/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name: cosmic_pointers.c
 *
 * description:   
 * Decoding of the Cosmic-specific pointers
 *
 * These functions are used by XCP to go between integers and pointers.
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "basic_types.h"
#include "cosmic_pointers.h"

/*
 * Bootblock code needs to live in RAM for reprogramming purposes
 */
#if defined BBLK
#pragma section (ramcode)
#endif

/**
 * Convert a 32-bit global address into a pointer.
 *
 * The hard work is done by laying out the #cosmic_gpage_address_u union
 * to match the format used by the Cosmic tools.
 */
FPTR uint8_t *cosmic_fptr_from_global_address(uint32_t address32)
{
    cosmic_gpage_address_u address;

    address.address_struct.address_word = (uint16_t) address32;
    address.address_struct.gpage = (uint8_t) (address32 >> 16u);
    address.address_struct.zero = 0u;
    return address.address_p;
}

/**
 * Convert an #FPTR (\@gpage \@far) pointer to a 32-bit global address
 *
 * The hard work is done by laying out the #cosmic_gpage_address_u union
 * to match the format used by the Cosmic tools.
 */
uint32_t cosmic_global_address_from_fptr(FPTR const uint8_t *ptr_p)
{
    cosmic_gpage_address_u address;
    uint32_t address32;

    /*
     * cast away the const-ness because we only have one union for
     * const/pointer conversion and we're not going to write through it anyway
     */
    address.address_p = (FPTR uint8_t *)ptr_p;
    address32 = ((uint32_t) address.address_struct.gpage << 16u)
                + (uint32_t) address.address_struct.address_word;
    return address32;
}

