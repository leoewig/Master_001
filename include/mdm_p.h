/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    mdm_p.h
 *
 * description:  Internal Header for MDM feature
 *    
 *      
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Apr-28 Jun Wu          initial version
 *                       
 *****************************************************************************/
#if !defined(MDM_P_H)
#define      MDM_P_H
/**
 *
 * Internal Header for MDM feature
 *
 * @file
 * @ingroup mdm
 */


/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/*****************************************************************************
 * MACROS
 *****************************************************************************/


/*****************************************************************************
 * Variables
 *****************************************************************************/


//extern uint8_t      mdm_curr_buff_count;
//extern uint8_t      mdm_curr_valid_count;
//extern bool_t       mdm_curr_buff_overflow;
//extern uint8_t      mdm_buff_v_count;
//extern uint8_t      mdm_valid_buff_v_count;
//extern FRAM real32_T     mdm_volt_buff_V[RTW_VI_BUFFER_LEN];
//extern FRAM real32_T     mdm_curr_buff_A[RTW_VI_BUFFER_LEN];


extern data_validity_e mdm_minmax_v_valid;


extern data_validity_e mdm_minmax_i_valid;
extern current_t mdm_max_i;
extern current_t mdm_min_i;


/*****************************************************************************
 * * PRIVATE function prototypes (documented in source)
 *****************************************************************************/

/* Called from mdm_init() */

PRIVATE void mdm_invalidate_outputs(void);

/* Called from mdm_update() */
PRIVATE void mdm_buffer_fg_data(void);
PRIVATE void mdm_update_v_minmax(void);
PRIVATE void mdm_update_curr_minmax(void);
//PRIVATE void mdm_store_timestamp(void);
PRIVATE void mdm_update_v_buff(void);
PRIVATE void mdm_update_curr_buff(void);
PRIVATE void mdm_buffer_current(current_t current, data_validity_e curr_valid);
PRIVATE void mdm_store_temperature(void);

PRIVATE void mdm_read_bg_output(void);
PRIVATE void mdm_bg_fg_swap(void);

PRIVATE void mdm_meas_start_bg(void);
PRIVATE bool_t mdm_bg_run(void);
PRIVATE void mdm_trsf_fg_data(void);
//PRIVATE void mdm_trsf_timestamp(void);
PRIVATE void mdm_trsf_curr_buff(void);
PRIVATE void mdm_trsf_v_buff(void);
PRIVATE void mdm_trsf_v_buff_valid(void);
PRIVATE void mdm_trsf_curr_integ(void);
PRIVATE void mdm_trsf_bcm_temp(void);
//PRIVATE void mdm_trsf_timestamp(void);
PRIVATE void mdm_shutdown_control(void);
PRIVATE void mdm_task_shutdown_control(void);
PRIVATE void mdm_cycle_minmax(void);


/* Called from mdm_prep_bg_inputs */
PRIVATE void mdm_conv_minmax_temp(void);
PRIVATE void mdm_set_mean_cell_temp(void);

/* Called from mdm_post_bg_outputs */
PRIVATE void mdm_bg_op_transfer(bool_t tmm_ran);

/* Helpers */
//PRIVATE ENUM_DATA_VALID mdm_map_valid(data_validity_e validity);


PRIVATE data_validity_e mdm_join_validity(data_validity_e lhs,
    data_validity_e rhs);

PRIVATE data_validity_e mdm_update_validity(bool_t have_new, 
    data_validity_e prev_val);

#endif      /* MDM_P_H */

