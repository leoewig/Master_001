/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    icm_status_p.h
 *
 * description:  ICM status processing internal header
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
#if !defined(ICM_STATUS_P_H)
#define      ICM_STATUS_P_H
/**
 *
 * ICM status processing internal header
 *
 * @file
 * @ingroup icm_status
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/*
 * enumerations and macro definitioms made available must be documented here to
 * avoid duplication.
 *
 * PRIVATE functions are documented in the source file.
 * Non-PRIVATE variables and functions (i.e. those shared between files in the
 * feature) are documented here.
 */

 #if defined(UNIT_TEST) && !defined(_lint)
extern bool_t               icm_all_vtbm_cal_ok;
extern FRAM uint8_t         icm_rx_status_count;
extern FRAM bool_t          icm_vtbm_status_rxd[MAX_NUM_VTBM];
extern FRAM data_validity_e icm_vtbm_status_condition[MAX_NUM_VTBM];
#endif

PRIVATE void    icm_mark_missing_status(void);
PRIVATE void    icm_process_statuses(void);
PRIVATE bool_t  icm_validate_status_data(const struct vtbm_status_s  *statp);
PRIVATE void    icm_check_cfg_responses(void);
PRIVATE void    icm_check_eee_responses(void);
PRIVATE void    icm_check_single_bit_fault_responses(void);
PRIVATE void    icm_check_cal_responses(void);
PRIVATE void    icm_chk_vtbm_can_errors(const struct vtbm_status_s  *statp);
PRIVATE void    icm_store_vtbm_status(const struct vtbm_status_s  *statp);
PRIVATE void    icm_process_status_data(const struct vtbm_status_s  *statp);
PRIVATE void    icm_check_all_vtbm_versions(void);
PRIVATE bool_t  icm_validate_version(const struct vtbm_status_s  *statp);
PRIVATE void    icm_check_rejection(const struct vtbm_status_s  *statp);
PRIVATE enum    IcmCfgState_e icm_check_vtbm_cfg(uint8_t vtbm_num);
PRIVATE void    icm_check_vtbm_single_bit_fault(uint8_t vtbm_num);
PRIVATE void    icm_check_vtbm_eee_fault(uint8_t vtbm_num);
#endif      /* ICM_STATUS_P_H */

