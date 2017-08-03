/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   gf_p.h
 *
 * description:  
 * Ground Fault Detection private header
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
#if !defined(GF_P_H)
#define      GF_P_H
/**
 *
 * Ground Fault Detection private header
 *
 * @file
 * @ingroup gf
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#include <mmc.h>
#include "gf_types.h"

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

extern enum gf_state_e           gf_state           [Gf_max_buses];
extern ts_time_t                 gf_symmetric_timer [Gf_max_buses];
extern ts_time_t                 gf_contactor_event_time [Gf_max_buses];
extern enum gf_symmetric_state_e gf_symmetric_state [Gf_max_buses];
extern struct gf_status_data_s   gf_status          [Gf_max_buses];

#if defined UNIT_TEST
extern bool_t                    gf_cont_closed_prev[Gf_max_buses];
#endif

/* Snapshot Data */
extern bool_t                    gf_inst_veh_contactors;
extern bool_t                    gf_inst_chg_contactors;

extern pack_v_t                  gf_inst_bal_veh_pos_rail_v;
extern pack_v_t                  gf_inst_bal_veh_neg_rail_v;
extern pack_v_t                  gf_inst_bal_veh_lo_pos_rail_v;
extern pack_v_t                  gf_inst_bal_veh_lo_neg_rail_v;
extern pack_v_t                  gf_inst_bal_chg_pos_rail_v;
extern pack_v_t                  gf_inst_bal_chg_neg_rail_v;

extern pack_v_t                  gf_inst_unbal_veh_pos_rail_v;
extern pack_v_t                  gf_inst_unbal_veh_neg_rail_v;
extern pack_v_t                  gf_inst_unbal_chg_pos_rail_v;
extern pack_v_t                  gf_inst_unbal_chg_neg_rail_v;

extern pack_v_t                  gf_veh_hi_res_pos_bal;
extern pack_v_t                  gf_veh_hi_res_neg_bal;
extern pack_v_t                  gf_veh_lo_res_pos_bal;
extern pack_v_t                  gf_veh_lo_res_neg_bal;
extern pack_v_t                  gf_veh_hi_res_pos_unbal;
extern pack_v_t                  gf_veh_hi_res_neg_unbal;
extern pack_v_t                  gf_chg_pos_bal;
extern pack_v_t                  gf_chg_neg_bal;
extern pack_v_t                  gf_chg_pos_unbal;
extern pack_v_t                  gf_chg_neg_unbal;

extern CONST struct gf_const_data_s gf_const_data           [Gf_max_buses];
extern CONST struct gf_cal_data_s   gf_cal_data             [Gf_max_buses];
extern       struct gf_data_s       gf_symmetric_data       [Gf_max_buses];

extern CONST int32_t                gf_lookup_resistance    [GF_LOOKUP_OR_SIZE];
extern CONST float32_t              gf_lookup_or_veh_nonsym [GF_LOOKUP_OR_SIZE];
extern CONST float32_t              gf_lookup_or_veh_sym    [GF_LOOKUP_OR_SIZE];
extern CONST float32_t              gf_lookup_or_chg_nonsym [GF_LOOKUP_OR_SIZE];
extern CONST float32_t              gf_lookup_or_chg_sym    [GF_LOOKUP_OR_SIZE];

/******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *****************************************************************************/

extern ohm_per_volt_t gf_calc_asym_ohm_per_volt(
                          pack_v_t v1,
                          pack_v_t v2,
                          pack_v_t v_bus,
                          ohm_t    res);
extern ohm_per_volt_t gf_calc_sym_ohm_per_volt(
                          pack_v_t         v1,
                          pack_v_t         v2,
                          CAL ohm_t      * res1,
                          CAL ohm_t      * res2,
                          CONST pack_v_t * v_asym);

extern bool_t          gf_cont_closed_event        (enum gf_bus_e  bus);
extern void            gf_exit_symmetric_check     (enum gf_bus_e  bus);
extern void            gf_grab_data_snapshot       (void);
extern bool_t          gf_bleed_resistor_in_circuit(void);

extern void            gf_isolation_off            (enum gf_bus_e  bus);
extern void            gf_new_closing_cycle        (enum gf_bus_e bus);
extern void            gf_reset_fault_flags        (enum gf_bus_e bus);
extern void            gf_set_fault_flags          (enum gf_bus_e bus,
                                                    bool_t        fault_present);
extern void            gf_reset_update_flags       (enum gf_bus_e bus);
extern void            gf_set_update_flags         (enum gf_bus_e bus);
extern enum gf_state_e gf_update_state             (enum gf_bus_e bus);

extern bool_t gf_asym_determine_if_fault_present   (enum gf_bus_e             bus,
                                                    pack_v_t                  v_pos,
                                                    pack_v_t                  v_neg,
                                                    ohm_per_volt_t            fault_level,
                                                    Fm_fault_subtype_list_e * subtype);
extern bool_t gf_sym_determine_if_fault_present    (enum gf_bus_e             bus,
                                                    pack_v_t                  v_pos,
                                                    pack_v_t                  v_neg,
                                                    ohm_per_volt_t            fault_level,
                                                    Fm_fault_subtype_list_e * subtype);

extern void            gf_asymmetric               (enum gf_bus_e bus);
extern void            gf_symmetric                (enum gf_bus_e bus);

extern void            gf_measuring                (enum gf_bus_e bus);
extern void            gf_processing               (enum gf_bus_e bus);

#endif      /* GF_P_H */

