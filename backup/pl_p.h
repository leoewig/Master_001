/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    pl_p.h 
 *
 * description:  PRIVATE function prototypes (documented in source)
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(PL_P_H)
#define      PL_P_H
/**
 *
 * Private header file for the Pack Layout feature. This file defines all the
 * parameters used internally within the Pack Layout feature.
 *
 * @file
 * @ingroup pl
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/*
 * PRIVATE function prototypes (documented in source)
 */
PRIVATE void pl_verify_num_vtbms(void);
PRIVATE void pl_verify_num_cells(void);
PRIVATE void pl_calculate_cell_start_indices(void);
PRIVATE void pl_verify_vtbm_pwr_sply_cfg(void);

#endif      /* PL_P_H */
