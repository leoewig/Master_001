/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      flash_eee_bblk_layout.c 
 *
 * description:   
 * Layout of data in emulated EEPROM shared between the bootloader and
 * application.
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li    Layout of data in emulated EEPROM shared between 
 *                       the bootloader and application.
 *
 *****************************************************************************/

/**
 * Layout of data in emulated EEPROM shared between the bootloader and
 * application.
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * @note This file relies on Cosmic linking uninitialised data in the opposite
 * order to which it is defined.  If you change anything in this file you
 * must confirm that bootblock_eee lies at the start of the bl_eee section!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 * @file
 * @ingroup flash_eee
 *
 * $Id: flash_eee_bblk_layout.c 9304 2013-07-05 00:59:16Z gvs $
 */
#include "basic_types.h"
#include "flash_eee_types.h"

/*
 * Locate near data into the flash_eee_ram partition.
 * {Curlies} used for everything except FBL because they are compiled without 
 * a .bss so all data is located into initialised sections.
 */
#if defined HOSTED
/* keep pragma out of hosted build */
#elif defined FBL
/* Compiled with .bss so relocate with [brackets] */
#pragma section [bl_eee]
#else
/* Compiled without .bss so relocate with {curlies} */
#pragma section {bl_eee}
#endif


#if defined FBL || defined MMC || defined BBLK_MMC
#ifdef FBL
/**
 * @lint -esym(765,fbl_meta_info) -esym(714,fbl_meta_info) lint thinks symbol
 * could be made static and is not referenced because the FBL source files
 * are excluded from lint analysis.
 */
/*lint -esym(765,fbl_meta_info) -esym(714,fbl_meta_info) */
#endif
/**
 * meta info populated by the FBL only
 */
fbl_eee_meta_s fbl_meta_info[FBL_N_LOGICAL_BLOCKS];
#endif

/**
 * Structure shared between the bootloader and the application.
 * See #flash_eee_boot_s for a description of each member
 *
 * @a2l{.last_internal_error_location,type,ASCII}
 * @a2l{.last_reset,conversion,flash_eee_reset_type_e}
 * @a2l{.next_reset,conversion,flash_eee_reset_type_e}
 */
flash_eee_boot_s bootblock_eee;
