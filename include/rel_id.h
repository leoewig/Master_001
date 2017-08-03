/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    rel_id.h
 *
 * description:  This file holds declaration information about the embedded version strings
 *
 *
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
#if ! defined REL_ID_H
#define REL_ID_H

/**
 *
 * This file holds declaration information about the embedded version strings
 *
 * @file
 * @ingroup release_id
 */


#include "utils.h"

/**
 * size of release ID array (must be no bigger than size of .relid section
 * in .lkf file)
 */
#define RELEASE_ID_SIZE         48u
#define SVN_URL_SIZE            128u

/**
 * Length of software_id array excluding trailing NULL
 */
#define SOFTWARE_ID_LENGTH      13u

#define N_SUPPORTED_DTCS ((sizeof obd_dtc_fault) / (sizeof obd_dtc_fault[0]))
STATIC_ASSERT(SOFTWARE_ID_LENGTH <= RELEASE_ID_SIZE,
              Software_ID_too_long);

#if defined MMC || defined FBL
#if (defined VP && defined PRODUCTION) || (!defined VP && !defined PRODUCTION)
#error precisely one of VP or PRODUCTUIION must be defined
#endif
#endif

#if defined MMC
/* The MMC actually uses the release_id when reporting the SW ID.
 * Since the release_id may be longer than SOFTWARE_ID_LENGTH, it 
 * is cropped when reported */
#   if defined VP
#       define SOFTWARE_ID             "P12_BMS_APPV1"
#   else   /* PRODUCTION */
#       define SOFTWARE_ID             "P12_BMS_APPP1"
#   endif
#elif defined VTBM
#define SOFTWARE_ID             "P12_BMS_VTBM1"
#elif defined BBLK
#define SOFTWARE_ID             "P12_BMS_BBLK1"
#elif defined FBL
#   if defined VP
#       define SOFTWARE_ID             "P12_BMS_FBLV1"
#   else /* PRODUCTION */
#       define SOFTWARE_ID             "P12_BMS_FBLP1"
#   endif
#else
#define SOFTWARE_ID             "P12_BMS_OTHER"
#endif


/**
 * global address of the svn_date_bcd variable on a bootloader
 * so that application diagnostics can read it
 */
#define BL_SVN_DATE_BCD_ADDRESS 0x7ffe60u
/**
 * global address of the software_id variable obn a bootloader
 * so that application diagnostics can read it
 */
#define BL_SOFTWARE_ID_ADDRESS 0x7ffe62u

extern FROM char_t release_id[RELEASE_ID_SIZE];
extern FROM uint16_t svn_date_bcd;
extern FROM char_t svn_url[SVN_URL_SIZE];
extern FROM char_t software_id[SOFTWARE_ID_LENGTH+1];


#endif /* ! defined REL_ID_H */
