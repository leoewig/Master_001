/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_aps.h
 *
 * description:  Application services public header file.
 *    
 *      
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
/** This file is intended to be included more than once, hence no protection */
/**
 * File used to locate data in far memory.
 *
 * This file is suitable for multiple inclusion.
 *
 * @file
 * @ingroup location
 * $Id: memory_layout.h 8692 2013-05-24 15:05:47Z gvs $
 */

/**
 * Approach used is to define #MEMORY_LOCATION and then include memory_layout.h
 * before defining far data items.
 *
 * This file can be included multiple times with different values of
 * \ref MEMORY_LOCATION
 *
 * ## What goes where
 * Location                     | Macro | Section       | S12 addressing mode
 * -----------------------------|-------|---------------|--------------------
 * \ref GENERAL_MEMORY          | FRAM  | .gdata        | \@gpage
 *      GENERAL_MEMORY          | FROM  | .fgdata       | \@gpage \@far
 *      GENERAL_MEMORY          | CAL   | .calram       | \@gpage const
 * \ref S12_MEMORY              | FRAM  | .gdata_s12    |
 *      S12_MEMORY              | FROM  | .fgdata_s12   |
 *      S12_MEMORY              | CAL   | .calram_s12   |
 * \ref XGATE_RAM               | FRAM  | .xdata        |
 *      XGATE_RAM               | FROM  | .calram       |
 *      XGATE_RAM               | CAL   | .calram       |
 * \ref XGATE_FLASH             | FRAM  | .xdata        |
 *      XGATE_FLASH             | FROM  | .xrom         |
 *      XGATE_FLASH             | CAL   | .xrom         |
 *
 * @note #CALIBRATION_MEMORY and #CALIBRATION_MEMORY_S12 are no longer required
 * so are defined as synonyms for #GENERAL_MEMORY and # S12_MEMORY respectively
 *
 * @defgroup location Memory location
 * @ingroup utils
 */

/**
 * @lint
 * Lint can't tell what this file does so switch off header file not used
 * warning\n
 * -esym(766,memory_layout.h)
 */
/*lint -esym(766,memory_layout.h) */

/* Copyright (c) Linkedriving Tech 2011 - All rights reserved */

/* Included for getting rid of #pragma section for hosted testing .*/
#ifndef HOSTED

#ifndef MEMORY_LOCATION
#error  MEMORY_LOCATION must be defined before including memory_layout.h
#endif

/*
 * define the various options for MEMORY_LOCATION the first time file is
 * included
 */
#ifndef GENERAL_MEMORY
#define         GENERAL_MEMORY                  1u
#define         S12_MEMORY                      2u
#define         CALIBRATION_MEMORY              GENERAL_MEMORY
#define         CALIBRATION_MEMORY_S12          S12_MEMORY
#define         XGATE_RAM                       5u
#define         XGATE_FLASH                     6u
#define         EOL_MEMORY                      7u
#define         VTBM_IMAGE                      8u
#endif

#if MEMORY_LOCATION == GENERAL_MEMORY
/*
 * Default settings for far data:
 * @gpage {} -> .gdata
 * @gpage const {} -> .calram
 * @gpage const @far {} -> .fgdata    (Cosmic doesn't separate const/non-const)
 */
#pragma section @gpage {}
#pragma section @gpage const {calram}
#pragma section @gpage @far {}

#elif MEMORY_LOCATION == S12_MEMORY
#ifdef _XGATE_
#error S12_MEMORY sections cannot be addressed from the XGATE
#endif
/*
 * Put FRAM variables in .gdata_s12 and FROM constants in .fgdata.
 * Neither section can be addressed by the XGATE
 */
#pragma section @gpage {gdata_s12}
#pragma section @gpage const {calram_s12}
#pragma section @gpage @far {fgdata_s12}

#elif MEMORY_LOCATION == XGATE_RAM
/* only allowed in XGATE-targeted code */
#ifndef _XGATE_
#error MEMORY_LOCATION XGATE_RAM only allowed in XGATE code
#endif
/*
 * default for XGATE is to put everything in RAM
 */
#pragma section {}                     /* initialised variables in .xdata */
#pragma section const {calram}         /* consts in .calram */

#elif MEMORY_LOCATION == XGATE_FLASH
/* only allowed in XGATE-targeted code */
#ifndef _XGATE_
#error MEMORY_LOCATION XGATE_FLASH only allowed in XGATE code
#endif
/*
 * put XGATE constants into flash
 */
#pragma section {}               /* initialised variables in .xdata */
#pragma section const {xrom}     /* consts in .xrom */

#elif MEMORY_LOCATION == EOL_MEMORY
/*
 * Put EOL memory block in .eol
 */
#pragma section @gpage @far const {eol}

#else
/* MEMORY_LOCATION set to an unknown value */
#error unknown value for MEMORY_LOCATION
#endif
#endif

/* undefine for next time */

#undef MEMORY_LOCATION


