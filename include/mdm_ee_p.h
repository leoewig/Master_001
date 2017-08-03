/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     mdm_ee_p.h 
 *
 * description:  Internal Header for MDM eeprom data transfer component
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
#if !defined(MDM_EE_P_H)
#define      MDM_EE_P_H
/**
 *
 * Internal Header for MDM eeprom data transfer component
 *
 * @file
 * @ingroup mdm_ee
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 * MDM eeprom data transfer.
 *
 * @defgroup mdm_ee MDM EEPROM transfer
 * @ingroup mdm
 *
 * Usage
 * ------
 * Call mdm_ee_init() at startup after eeprom data restore has completed. This
 * will set up the appropriate signals for the cell management code.
 *
 * Call mdm_ee_update() after cell management has completed in the backround
 * This will inspect the outputs and determine whether any eeprom data needs to
 * be written back, if so it will populare the RAM image and call NVS to
 * perform the write.
 *
 **/

#if defined(UNIT_TEST)
extern uint16_t mdm_ee_image_writes_ctr;
extern uint16_t mdm_ctr_reqEEWrite;
#endif

/*****************************************************************************
 * Functions
 *****************************************************************************/

PRIVATE void mdm_read_image(void);
PRIVATE void mdm_read_tmm_seq_image(void);
PRIVATE void mdm_read_lin_seq_image(void);

PRIVATE void mdm_write_image(void);

PRIVATE void mdm_write_tmm_seq_image(void);
PRIVATE void mdm_write_lin_seq_image(void);

#endif      /* MDM_EE_P_H */

