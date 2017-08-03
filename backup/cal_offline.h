/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    cal_offline.h
 *
 * description:  Pack-wide offline cal header
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Public header for ADC Conversion utility routines
 *                       
 *
 *****************************************************************************/
#if !defined(PACK_OFFLINE_CAL_H)
#define PACK_OFFLINE_CAL_H


/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "cal_offline_types.h"
#include "hardware.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern FROM cal_pack_offline_t  cal_offline_data;
//extern cal_off_data_status_t    cal_offline_data1_status;
//extern cal_off_data_status_t    cal_offline_data2_status;
extern uint32_t                 cal_off_cell_man_timestamp;
extern bool_t                   cal_off_cell_man_timestamp_valid;
extern bool_t                   cal_off_pack_sn_valid;


/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

extern void     cal_offline_init(void);

#endif /* PACK_OFFLINE_CAL_H */
