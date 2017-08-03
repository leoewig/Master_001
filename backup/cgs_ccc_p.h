/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   cgs_ccc_p.h 
 *
 * description:  
 * Private header for cgs
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
/*************************************************************************//**
 *
 *
 *****************************************************************************
 *
 * This header makes internal functions and data of cgs available to a
 * unit test harness.
 */

/**
 * @addtogroup cgs
 * @{
 * @file
 * @}
 */
#if !defined(CGS_CCC_P_H)
#define      CGS_CCC_P_H

#if 0
#include "fm.h"
#include "cgs_types.h"
#endif


 /****************************************************************************
 * Macros
 *****************************************************************************/

/**************************************************************************//**
* Options for the charging state machine #cgs_sm_chg_state
*****************************************************************************/

/**************************************************************************//**
* Options for cgs_chg_v_ok() and cgs_chg_i_ok()
*****************************************************************************/

/******************************************************************************
 * FUNCTION DEFINITIONS
 *****************************************************************************/


PRIVATE void      cgs_cc_accrue_maxv(cell_v_t max_v);
pack_v_t  cgs_run_cv_vctl(pack_v_t prev_setpoint, pack_v_t voltage_target,
    pack_v_t volt_step);
PRIVATE current_t cgs_run_cv_ictl(void);
PRIVATE cell_v_t  cgs_cc_calc_cellv_avg(uint32_t sum_v, uint8_t ctr);
PRIVATE int16_t   cgs_cc_calc_errterm(cell_v_t target_v, cell_v_t maxcell_v);
PRIVATE void cgs_cc_calc_integral( int32_t *integral_p ,int16_t iv_errval,
                                   int32_t high_lim);

PRIVATE int32_t cgs_cc_calc_iv_limit(int16_t pv_erval_v);
PRIVATE void    cgs_cc_reset_maxv_sum(cell_v_t value);

#if defined UNIT_TEST && !defined _lint

#endif /* UNIT_TEST && !defined _lint */

/******************************************************************************
 * VARIABLES
 *****************************************************************************/


/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/
#if defined UNIT_TEST && !defined _lint
extern uint8_t cgs_cc_bg_previous_ctr;
extern uint8_t cgs_cc_maxcellv_avg_ctr;
extern uint32_t cgs_cc_maxcellv_sum;
extern int32_t cgs_cc_iv_integral;
extern int16_t cgs_cc_iv_errval;
extern int32_t cgs_cc_iv_limit;
extern int16_t cgs_cc_pv_errval;

extern CAL int16_t cgs_cc_pv_mul;
extern CAL int16_t cgs_cc_iv_mul;
extern CAL pack_v_t cgs_ccc_max_v_step;

#endif /* UNIT_TEST && !defined _lint */

/******************************************************************************
 * CALIBRATIONS
 *****************************************************************************/


#endif      /* CGS_CCC_P_H */
