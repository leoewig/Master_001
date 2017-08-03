/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   ms_p.h
 *
 * description: Private header file for the ms feature
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
#if !defined(MS_P_H)
#define      MS_P_H
/**
 *
 * Private header file for the ms feature
 *
 * @file
 * @ingroup ms
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#include "ms_types.h"

#if defined (UNIT_TEST) && !defined(_lint)
extern enum ms_shutdown_state_e    ms_feature_shutdown_state [ms_last_inhibit];
extern enum ms_inhibit_type_e     ms_feature_shutdown_types [ms_last_inhibit];
extern uint8_t ms_wakeup_src_tries;
extern CAL uint8_t ms_max_wakeup_src_tries;
#endif

/*
 * PRIVATE function prototypes (documented in source)
 */
PRIVATE void                  ms_set_outputs(void) ;
PRIVATE bool_t                ms_check_for_shutdown_initiation(void);
PRIVATE bool_t                ms_have_all_features_shutdown(void);
PRIVATE void    ms_wakeup_update(void);
PRIVATE void                  ms_determine_wakeup(void);
PRIVATE enum Ms_wakeup_mode ms_wakeup_from_wus(void);

PRIVATE void ms_reset_if_required(void);
PRIVATE bool_t ms_reset_action_inhibited(void);

#endif      /* MS_P_H */

