/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    icm_timeout_p.h
 *
 * description: 
 * Private header file for icm command timeout behaviour
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
#if !defined(ICM_TIMEOUT_P_H)
#define      ICM_TIMEOUT_P_H
/**
 *
 * Private header file for icm command timeout behaviour
 *
 * @file
 * @ingroup icm_timeout
 */


#if defined( UNIT_TEST ) && !defined(_lint)
extern ts_time_t icm_last_cmd_time;
extern ts_time_t  icm_resp_timeout;
extern enum IcmRespState_e  icm_resp_state_val;
#endif

PRIVATE bool_t  icm_determine_timeout(void);

#endif      /* ICM_TIMEOUT_P_H */

