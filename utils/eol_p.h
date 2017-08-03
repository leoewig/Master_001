/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  eol_p.h 
 *
 * description:  
 *
 * End-of-line data module private header
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/
#if !defined(EOL_P_H_)
#define EOL_P_H_



/******************************************************************************
 * INCLUDES
 *****************************************************************************/

/******************************************************************************
 * PRIVATE MACRO DEFINITIONS
 *****************************************************************************/

/**
 * EOL validity marker value
 */
#define EOL_VALIDITY_MARKER    (0x9BA5EDFEu)

/**
 * Supported EOL data layout version
 */
#define EOL_SUPPORTED_DATA_LAYOUT_VERSION        (2u)

/**
 * EOL Padding value
 */
#define EOL_PADDING        (0xffu)

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
#if (defined UNIT_TEST) && (!defined _lint)
extern eol_t vtbm_eol;
#endif

/******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *****************************************************************************/

PRIVATE uint8_t is_ascii_string(FPTR const char_t * in, uint8_t max_len);
PRIVATE bool_t check_hw_version(FPTR const char_t * hw,
                                const char_t * const supported_hw_versions[]);

#endif /* EOL_P_H_ */
