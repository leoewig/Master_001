/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   smi.h
 *
 * description:  
 * Public header file for the smi feature
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
#if !defined(SMI_H)
#define      SMI_H
/**
 *
 * Public header file for the smi feature
 *
 * @file
 * @ingroup smi
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 *
 * This feature manages the interaction between the MMC and SMC
 *
 * for example:  coordination of contactor demands. It is responsible for
 * translating communication between the SMC and MMC over SPI into application
 * variables

 * @defgroup smi Supervisor Micro Interaction
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "smi_types.h"

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern enum     smi_op_state_e smi_smc_state;
extern enum     smi_wake_src_e smi_wake_src;
extern bool_t   smi_chip_select;
extern bool_t   smi_init_complete;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 * Initialise the smc interface module */
void smi_init(void);

/* Read in data from driver context and make available to the foreground task*/
void smi_read_inputs(void);

/* Write out data from fg context to whatever underlying interface it is sent
 * to*/
void smi_write_outputs(void);

#endif      /* SMI_H */

