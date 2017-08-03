/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   mmc_rtw.h
 *
 * description: Header for handcode to include RTW declarations
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
/**
 * Header for handcode to include RTW declarations
 *
 * This file insulates handcode from changes to the strucutre of the Simulink
 * model
 *
 * @file
 * @ingroup rtw
 */
//#include "cm_ref.h"     /* the top level include file */

/** Size used to reserve space for cells. Must be same as RTW_MAX_NUM_CELLS. */
#define MAX_NUM_CELLS RTW_MAX_NUM_CELLS
