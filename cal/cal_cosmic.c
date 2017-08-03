/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     cal_cosmic.c
 *
 * description:   XCP calibration support source
 *
 * Decoding of the Cosmic-specific pointers and static data initalisation table
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           decoding of pointer
 *
 *****************************************************************************/

#include "basic_types.h"
#include "hardware.h"           /* for ASM */
#include "cosmic_pointers.h"
#include "cal.h"
#include "cal_p.h"

/**
 * Symbol defined by Cosmic which points to the static initialisation table.
 * It has two leading underscores in assembler, so need to miss one off to
 * access it from C.
 */
LINKER_SYMBOL(CONST cosmic_idesc_s _idesc__,{0u});

/**
 * Macro to force acces to _idesc__ to be via a global GPAGE address
 *
 * Alternative would be to set a global symbol in the .lkf file equivalent to
 * __idesc__.  Would that be any less confusing?
 */
#define         IDESC_P ((FPTR const cosmic_idesc_s *)&_idesc__)


/**
 * Returns a pointer to the static initialisation data for the given section.
 * See the "Automatic Data Initialization" section in the Cosmic compiler manual
 * for information about the layout.  The address of the initialisation table
 * is given by the linker symbol __idesc__.  We match its layout with the C
 * structures:
 * + #cosmic_idesc_s for the whole table
 * + #cosmic_init_entry_s for each entry within the table
 *
 * @note We do not attempt to handle unpaged entries in __idesc__.
 * @note The ROM addresses in the intialisation table are unreliable because
 * they start in the paged ROM window (~0x8000) but overflow into the
 * upper fixed window (0xc000-0xffff) and then overflow through zero.  We 
 * infer the global address of the first ROM address only, subsequent
 * addresses are used only to work out the length of sections using 16-bit 
 * arithmetic.  This is fine, provided the entire initialisation table fits in 
 * 64k.  Given that we only have 64k of RAM, this is a reasonable approach.
 *
 * @param ram_address   starting address for RAM data section
 * @returns             start of static initialisation data in flash memory
 * @retval #FNULL       ram_address couldn't be found in the initialisation
 *                      table
 */
FPTR const uint8_t * cosmic_init_flash_address(FPTR const uint8_t *ram_address)
{
    uint16_t flash_first_local;
    uint16_t flash_local;
    uint16_t offset_from_start;
    FPTR const uint8_t *flash_data_start_p;
    FPTR const uint8_t *flash_data_p;
    bool_t found_address;
    bool_t is_paged;
    bool_t table_ok;
    FPTR const cosmic_init_entry_s *entry_p;

    flash_first_local = cosmic_init_flash_start();
    flash_data_start_p
                    = cosmic_init_flash_first_data_p(flash_first_local);
    flash_local = flash_first_local;

    table_ok = TRUE;
    found_address = FALSE;
    for (entry_p = cosmic_init_first_entry();
         cosmic_init_not_at_end(entry_p) && table_ok && !found_address;
            entry_p++)
    {
        is_paged = cosmic_init_entry_is_paged(entry_p);

        if (!is_paged)
        {
            /* 
             * Non-paged entries have a different layout so we cannot
             * treat it as an array of structures.
             * We don't expect to ever encounter a non-paged entry so
             * we haven't coded for the extra complexity
             */
            BREAK_IF_DEBUGGING;
            table_ok = FALSE;
        }
        else if (cosmic_init_entry_ram_address(entry_p) == ram_address)
        {
            /* 
             * ROM local address will be the end address from the previous
             * entry - so no need to decode this entry
             */
            found_address = TRUE;
        }
        else
        {
            flash_local = cosmic_init_entry_flash_end(entry_p);
        }
    }

    if (found_address)
    {
        offset_from_start = flash_local - flash_first_local;
        flash_data_p = &flash_data_start_p[offset_from_start];
    }
    else
    {
        flash_data_p = FNULL;
    }
    return flash_data_p;
}

/**
 * Returns the initial data offset found in the initialisation table.
 *
 */
PRIVATE uint16_t cosmic_init_flash_start(void)
{
    return IDESC_P->rom_start;
}

/**
 * Converts the first offset in the initialisation table into a pointer.
 *
 * We can only do this reliably for the first offset because it's the only one
 * for which we can safely assume that it lies on the same PPAGE as the
 * initialisation table itself.
 */
PRIVATE FPTR const uint8_t 
                        *cosmic_init_flash_first_data_p(uint16_t first_local)
{
    FPTR const uint8_t *first_data_p;
    cosmic_gpage_address_u idesc_address_u;
    FPTR uint8_t *idesc_global_p;
    uint16_t idesc_offset;
    uint16_t first_offset;


    /* 
     * Although __idesc__ is in ROM, leave 'const' out of the cast so that
     * it has the same type as idesc_global_p and idesc_address_u.address_p
     */
    idesc_global_p = (FPTR uint8_t *) IDESC_P;

    idesc_address_u.address_p = idesc_global_p;

    idesc_offset = CAL_ROM_OFFSET_FROM_ADDRESS_WORD(
                        idesc_address_u.address_struct.address_word);
    first_offset = CAL_ROM_OFFSET_FROM_ADDRESS_WORD(first_local);
    first_data_p = &idesc_global_p[first_offset - idesc_offset];

    return first_data_p;
}

/**
 * Returns a pointer to the first initialisation table entry, which comes after
 * the initial ROM address.
 */
PRIVATE FPTR const cosmic_init_entry_s *cosmic_init_first_entry(void)
{
    return &IDESC_P->table[0];
}

/**
 * Check an entry's flag to see if it is the end-of-table marker
 */
PRIVATE bool_t cosmic_init_not_at_end(FPTR const cosmic_init_entry_s *entry_p)
{
    bool_t end_marker = (entry_p->flag == 0u);
    return (bool_t) !end_marker;
}

/**
 * Check an entry's flag to see if it is a paged or unpaged entry.
 * Paged entries have their top bit set.
 */
PRIVATE bool_t cosmic_init_entry_is_paged(
                                FPTR const cosmic_init_entry_s *entry_p)
{
    bool_t is_paged = (entry_p->flag & 0x80u);
    return is_paged;
}

/**
 * Convert the paged RAM address in an entry to a pointer.
 *
 * An intialisation table has the page in a different position from that
 * used by a Cosmic far pointer.  The details of the two layouts are
 * contianed in the two data types #cosmic_gpage_address_u
 * and #cosmic_init_entry_s
 */
PRIVATE FPTR uint8_t *cosmic_init_entry_ram_address(
                        FPTR const cosmic_init_entry_s *entry_p)
{
    cosmic_gpage_address_u address;

    address.address_struct.gpage = (uint8_t) entry_p->ram_gpage;
    address.address_struct.zero = 0u;
    address.address_struct.address_word = entry_p->ram_start;

    return address.address_p;
}

/**
 * Return the local ROM end address in an entry.  This is only used to
 * calculate the 16-bit offset from the start of the table, so Cosmic's
 * inability to generate the correct paged address is not a problem.
 */
PRIVATE uint16_t cosmic_init_entry_flash_end(
                        FPTR const cosmic_init_entry_s *entry_p)
{
    return  entry_p->rom_end;
}
