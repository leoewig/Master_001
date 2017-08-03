/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     vmd_p.h
 *
 * description:   Private header for vmd
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Private header for vmd
 *                       
 *
 *****************************************************************************/

/*************************************************************************//**
 *
 * Private header for vmd
 *
 *
 * This header makes internal functions and data of vmd available to a
 * unit test harness.
 */

/**
 * @addtogroup vmd
 * @{
 * @file
 * @}
 */
#if !defined(VMD_P_H)
#define      VMD_P_H

#include "basic_types.h"
#include "mmc.h"
#include "vmd_types.h"

#include "cgi.h"     /* Charger Interface         */
#include "vio.h"     /* Vehicle Interface         */

/******************************************************************************
 * FUNCTION DECLARATION
 *****************************************************************************/
extern  void vmd_init(void);

/******************************************************************************
 * VARIABLES
 *****************************************************************************/
#if defined(UNIT_TEST) && !defined(_lint)

extern bool_t                     vmd_open_with_grace_pending;
extern ts_time_t                  vmd_grace_start;
extern bool_t                     vmd_main_fuse_blow_pending;
extern ts_time_t                  vmd_main_fuse_blow_start;
extern ts_time_t                  vmd_afterrun_bal_start;
extern bool_t                     vmd_prev_veh_demand_present;
extern bool_t                     vmd_contactors_opened;
extern bool_t                     vmd_contactors_closed;
extern enum vio_ign_status_e      vmd_ign_status_prev;
extern ts_time_t                  vmd_ign_off_time;
extern bool_t                     vmd_imm_tethered;
extern enum cms_messages_e        vmd_imm_last_cms;
extern enum Vmd_override_status_e vmd_override_status;
extern bool_t                     vmd_imm_last_vehicle_moving;
extern bool_t                     vmd_imm_last_vehicle_active;
extern enum cgi_proximity_state_e vmd_imm_last_proximity_state;
extern enum cgi_pilot_state_e     vmd_imm_last_pilot_state;
extern ts_time_t                  vmd_override_time;
extern bool_t                     vmd_imm_vehicle_moving;
extern enum cms_messages_e        vmd_last_cluster_msg;

extern CAL pack_v_t vmd_veh_close_allow_limit;

extern bool_t vmd_check_error_conditions        (void);
extern void   vmd_active_during                 (void);
extern void   vmd_standby_during                (void);
extern void   vmd_determine_closing_allowed     (void);
extern void   vmd_afterrun_balancing            (void);
extern void   vmd_detect_vehicle_demand_rescind (void);
extern void   vmd_startup_dclink_monitoring     (void);
extern void   vmd_maintain_ignition_status      (void);
extern void   vmd_overcurrent_detection         (void);
extern void   vmd_determine_indicate_closing_allowed(void);
#endif      /* UNIT_TEST */

extern CAL speed_t  vmd_immob_veh_spd_thresh;

#endif /* VMD_P_H */

