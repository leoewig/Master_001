/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    mdm.h
 *
 * description:  MDM public header file
 *    
 *      
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Apr-28 Jun Wu          initial version
 *                       
 *
 *****************************************************************************/
#if !defined(MDM_H)
#define      MDM_H

/**
 *
 * MDM public header file
 * @file
 * @ingroup mdm
 *
 */

/* Copyright (c) Linkedriving Tech 2015,2016 - All rights reserved */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "mmc_rtw.h"             /* for MAX_NUM_CELLS */
#include "mmc.h"
#include "mdm_types.h"



/******************************************************************************
 * Checks
 *****************************************************************************/


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
/**
 * Outputs from TMM management */
struct bg_fg_data_s
{
	int16_t TMM_angle;
};
/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/**
 * Buffer that has been filled with values - used by fg task
 * These are the outputs from the background to the foreground */
extern FPTR CONST struct bg_fg_data_s    * mdm_bg_output_p;

/** Indication from bg to fg that the output structure has been updated */
extern uint8_t mdm_bg_op_read_ctr;



/******************************************************************************
 * Mapped temperature inputs
 *****************************************************************************/
/*
 * NVS data
 */
extern FRAM mdm_nvs_wrapper_s mdm_ram_image;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

void mdm_init(void);
//void mdm_update(void);
void mdm_prep_bg_inputs(void);
void mdm_TMM_task(void);
void mdm_post_bg_outputs(bool_t tmm_ran);

/*************************************
 * Floating point inspection helpers
 *************************************/
//bool_t is_finite(real32_T value);
//bool_t is_not_finite(real32_T value);
//bool_t is_infinite(real32_T value);
//bool_t is_nan(real32_T value);

/****************************************
 * Clipping output conversion functions *
 ***************************************/

#endif      /* MDM_H */

