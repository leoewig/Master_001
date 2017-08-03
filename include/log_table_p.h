/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     log_table_p.h
 *
 * description: 
 *  
 * Private header file for LOG TABLE    
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(LOG_TABLE_P_H)
#define      LOG_TABLE_P_H
/**
 *
 * Private header file for LOG TABLE
 *
 * Only needed by log module
 *
 * @file
 * @ingroup LOG
 */

/* Copyright (c) Linkedriving Tech 2011 - All rights reserved */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "basic_types.h"


/******************************************************************************
 * PRIVATE MACRO DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * PRIVATE TYPE DEFINITION
 *****************************************************************************/

/** Enumeration of data types of source data used for log tables.
 */
typedef enum
{
    Log_src_data_type_none, /* type not specified */
    Log_src_data_type_u8,   /* uint8 */
    Log_src_data_type_u16,  /* uint16 */
    Log_src_data_type_u32,  /* uint32 */
    Log_src_data_type_s8,   /* int8 */
    Log_src_data_type_s16,  /* int16 */
    Log_src_data_type_s32   /* int32 */
} Log_src_data_type_e;



/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/


/******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES (documented in source)
 *****************************************************************************/

PRIVATE void    log_table_eval(Log_table_e table);
PRIVATE bool_t  log_table_src_data_to_int32(Log_table_e table, int32_t* result);
PRIVATE bool_t  log_table_get_bin(Log_table_e table, int32_t data, uint8_t* bin);

#ifndef LOG_DIS_TABLE_TEMPERATURE
PRIVATE void    log_table_eval_cell_strg_temp(Log_table_e table);
#endif
#ifndef LOG_DIS_TABLE_VTBM_ACTV_BAL
PRIVATE void    log_table_eval_vtbm_actv_bal(Log_table_e table);
#endif
#ifndef LOG_DIS_TABLE_VTBM_PASV_BAL
PRIVATE void    log_table_eval_vtbm_pasv_bal(Log_table_e table);
#endif
#if !defined(LOG_DIS_TABLE_VTBM_ACTV_BAL) || !defined(LOG_DIS_TABLE_VTBM_PASV_BAL)
PRIVATE void    log_table_eval_vtbm_balancing(Log_table_e table, enum BalAction_e type);
#endif

#if defined (UNIT_TEST) && !defined(_lint)
#else
#endif

#endif      /* LOG_TABLE_P_H */
