/**
 *
 * Current measurement interface
 *
 * @file
 * @ingroup imi
 * $Id: imi_cust.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2015 - All rights reserved */

#if !defined( IMI_CUST_H )
#define       IMI_CUST_H

#include "basic_types.h"
#include "mmc.h"    /* For pack_v_t type. */
#include "imi_types_cust.h"

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/* Documented in source */
extern struct Imi_latest_data_s imi_latest_data;

/* Documented in source */
extern enum imi_hv_bus_zero_curr_status_e imi_hv_bus_zero_curr_status;

/* Documented in source */
extern enum imi_hv_bus_zero_curr_status_e imi_hv_bus_zero_curr_charge_status;

/* Documented in source */
extern enum imi_hv_bus_zero_curr_status_e imi_hv_bus_zero_curr_discharge_status;

/* Documented in source */
extern bool_t imi_running;

/* Documented in source */
extern CAL current_t imi_zero_curr_tol_mA;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/* Documented in source */
enum imi_integral_validity_e imi_get_integ_cur_val(CONST bool_t reset,
                                         FPTR high_res_charge_t *CONST data_p);

#endif
