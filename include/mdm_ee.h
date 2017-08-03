/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     mdm_ee.h
 *
 * description:  MDM eeprom transfer public header file
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
#if !defined(MDM_EE_H)
#define      MDM_EE_H
/**
 *
 * MDM eeprom transfer public header file
 * @file
 * @ingroup mdm
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/
//extern FRAM mdm_nvs_wrapper_s mdm_ram_image;

/******************************************************************************
 * FUCNTIONS
 *****************************************************************************/

void mdm_ee_init(void);
void mdm_ee_update(void);

bool_t mdm_populate_valid_image(void);
void   mdm_populate_image(void);
void    mdm_write_valid_image(void);
#endif      /* MDM_EE_H */

