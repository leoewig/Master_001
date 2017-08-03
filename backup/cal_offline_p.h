/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    cal_offline_p.h
 *
 * description:  
 * Private header for offline calibration component
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
#if !defined(CAL_OFFLINE_P_H)
#define      CAL_OFFLINE_P_H
/**
 *
 * Private header for offline calibration component
 *
 * @file
 * @ingroup cal_offline
 * $Id: cal_offline_p.h 11014 2013-10-23 07:53:19Z dc17 $
 */

/* Copyright (c) Linkedriving Tech 2015 - All rights resevered */


PRIVATE void cal_offline_check_data1(void);
PRIVATE void cal_offline_check_data2(void);
PRIVATE cal_off_data_status_t cal_offline_vtbm_t_range_check(void);
PRIVATE void cal_off_set_cell_manufacture_date(void);

#endif      /* CAL_OFFLINE_P_H */

