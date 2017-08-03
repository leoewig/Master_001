/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   smg_p.h
 *
 * description:  
 * Private header file for SMG - This file defines all the parameters used
 * internally within the SMG feature.
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
#if !defined(SMG_P_H)
#define      SMG_P_H
/**
 * Private header file for SMG - This file defines all the parameters used
 * internally within the SMG feature.
 *
 * @file
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#include "smg_types.h"

/*
 * PRIVATE function prototypes (documented in source)
 */
PRIVATE FCODE void smg_mon_supply_on_events     (smg_devices_e idx);
PRIVATE FCODE void smg_mon_supply_off_events    (smg_devices_e idx);
PRIVATE FCODE void mon_12v_power_status         (void);
//PRIVATE FCODE void smg_fault_filter_12v_power   (bool_t                mon_present,
 //                                                enum Ctc_cont_state_e exp_cont_psn,
 //                                                Fm_fault_list_e       fault_type);
PRIVATE FCODE bool_t is_any_veh_bus_cont_cmded_close_by_bcm(void);
PRIVATE FCODE bool_t charger_contactors_closed  (void);
PRIVATE FCODE void smg_fault_filter_vpos        (void);
PRIVATE FCODE void smg_fault_filter_vneg        (void);
PRIVATE FCODE void smg_fault_filter_vtbm_can_gnd(void);

#endif /* SMG_P_H */
