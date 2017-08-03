/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   imi_p.h
 *
 * description: 
 * Private header file for IMI - This file defines all the parameters used
 * internally within the IMI feature.
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
#if !defined(IMI_P_H)
#define      IMI_P_H
/**
 *
 * Private header file for IMI - This file defines all the parameters used
 * internally within the IMI feature.
 *
 * @file
 * @ingroup imi
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 * PRIVATE TYPE DEFINITIONS
 */
/**
 * Structure to provide the integral value of the measured current along with
 * the data quality indicator for integral current values.
 */
struct Imi_integral_data_s
{
    high_res_charge_t            integral_uC;          /**< Most recent integral value of measured current over a cell measurement cycle. */
    enum imi_integral_validity_e integral_uC_validity; /**< Data quality indicator for the current integral. */
};

/*
 * PRIVATE function prototypes (documented in source)
 */
PRIVATE void            imi_check_zero_curr_status(void);
PRIVATE high_res_charge_t imi_exe_rectangle_rule(CONST current_t current_now,
                                              high_res_charge_t integral_now);
PRIVATE bool_t          imi_is_sensor_faulty(void);
PRIVATE data_validity_e imi_read_sensor_current(current_t *CONST sens_curr_p);
PRIVATE void            imi_update_current_data(current_t latest_value);

/*
 * Calibrations
 */
extern CAL current_t imi_zero_curr_tol_mA;

/*
 * Parameters needed in the Hosted Unit Test
 */
#if defined UNIT_TEST  && !defined _lint

extern struct Imi_integral_data_s imi_integral_data;
extern        ts_time_t           prev_ts;
extern        current_t           prev_current;
extern PRIVATE current_t *curr_sensors_tol[Imi_last_sensor];

#endif /* UNIT_TEST */

#endif /* IMI_P_H */
