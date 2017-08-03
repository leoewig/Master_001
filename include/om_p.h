/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    om_p.h
 *
 * description:  Private Header file for Output Monitoring Feature
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined( OM_P_H )
#define       OM_P_H
/**
 * @file
 * @ingroup OM
 * Private Header file for Output Monitoring Feature
 *
 */

 /* Copyright (c) Linkedriving Tech 2011 - All rights reserved */

#include "mmc.h"        /* For MMC specific types */
#include "om_types.h"

/******************************************************************************
 * PRIVATE MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * PRIVATE FUNCTION DECLARATIONS
 *****************************************************************************/
PRIVATE void om_monitor_digital_outputs            (void);
PRIVATE void om_running_actions                    (void);
PRIVATE void om_shutdown_actions                   (void);
PRIVATE void om_read_diagnostics_inputs            (void);
PRIVATE void om_fault_filter_contactor_diagnostics (void);
PRIVATE void om_validate_input                     (enum Om_diagnostic_input_type_e diag_input);
PRIVATE void om_update_contactor_wear_data         (enum Om_diagnostic_input_type_e sensor);

PRIVATE current_t om_adc_to_current (adc_v_t adc_value);

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

#if defined(UNIT_TEST)
extern CONST bool_t *om_enabled_hs[Om_max_diag_inputs];
extern CONST bool_t *om_enabled_ls[Om_max_diag_inputs];
extern struct om_sensor_data_s * CONST om_diag_currents[Om_max_diag_inputs];
#endif

#endif /* OM_P_H */
