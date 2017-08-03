/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   flash_fault.h
 *
 * description:  
 *  Header for flash fault detection
 * 
 * 
 *  
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#ifndef FLASH_FAULT_H
#define FLASH_FAULT_H
/**
 * Header for flash fault detection
 *
 * @file
 * @ingroup flash_fault
 */

/**
 * Flash fault detection consists of scanning flash in the bootloader to
 * ensure flash is fault-free before launching an app and then monitoriting
 * for an error within the application
 *
 * @defgroup flash_fault Flash ECC error detection
 * @ingroup flash
 */

void isr_flash_fault_init(void);
INTERRUPT void isr_flash_fault(void);
void flash_nonfatal_fault_check(void);

/**
 * Mask of DFDIF in FERSTAT and DFDIE in FERCNFG
 */
#define FERSTAT_DFDIF           2u
/**
 * Mask of DFDIE in FERCNFG
 */
#define FERCNFG_DFDIE           FERSTAT_DFDIF

/**
 * Mask of SFDIF in FERSTAT and SFDIE in FERCNFG
 */
#define FERSTAT_SFDIF           1u

#if defined VTBM
void vtbm_single_bit_fault_update(void);
#endif

#if defined VTBM
extern bool_t vtbm_single_bit_fault_present;
#endif

#endif /* ifndef FLASH_FAULT_H */
