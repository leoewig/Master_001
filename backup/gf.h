/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    gf.h
 *
 * description:  Ground Fault Indication public header file
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(GF_H)
#define      GF_H
/**
 *
 * Ground Fault Indication public header file
 *
 * @file
 * @ingroup gf
 * $Id: gf.h 11312 2013-11-15 10:19:18Z tg10 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 *
 * This feature makes use of the HV bus voltage components (voltage
 * measured on both positive and negative high voltage rails) in order
 * to infer whether a ground fault is present.
 *
 * This measurement can only be made when contactors are closed. The
 * Ground Fault indication is critically dependant on the exact
 * hardware of the HV bus measurement and as such is not fully
 * abstracted from the hardware implementation of the voltage
 * measurement circuit by the VMI module in the way that other features
 * are.
 *
 * <h3>Asymmetric Calculations</h3>
 * In order to determine an asymmetric ground fault error exists the following actions
 * must be taken
 * - calculate V<sub>cells_unbal</sub>  = V<sub>hvpos_unbal</sub> - V<sub>hvneg_unbal</sub>
 * - calculate V<sub>offset_unbal</sub> = V<sub>hvpos_unbal</sub> + V<sub>hvneg_unbal</sub>
 * - calculate OR<sub>unbal</sub>       = V<sub>offset_unbal</sub> / V<sub>cells_unbal</sub>
 * - calculate the safety limit RES<sub>lookup</sub> = LIMIT * V<sub>cells_unbal</sub> (where LIMIT is calibratable)
 * - use RES<sub>lookup</sub> to access the asymmetric lookup table to find the equivalent offset ratio OR<sub>nonsym</sub>
 *
 * An error exists if OR<sub>unbal</sub> > ABS (OR<sub>nonsym</sub>)
 *
 * The above calculation must be done with different values for LIMIT in order to detect both critical
 * and non-critical faults.
 *
 * <h3>Symmetric Calculations</h3>
 * In order to determine a symmetric ground fault error exists the following actions
 * must be taken
 * - calculate V<sub>cells_bal</sub>  = V<sub>hvpos_bal</sub> - V<sub>hvneg_bal</sub>
 * - calculate V<sub>offset_bal</sub> = V<sub>hvpos_bal</sub> + V<sub>hvneg_bal</sub>
 * - calculate OR<sub>bal</sub>       = V<sub>offset_bal</sub> / V<sub>cells_bal</sub>
 * - calculate the safety limit RES<sub>lookup</sub> = LIMIT * V<sub>cells_bal</sub> (where LIMIT is calibratable)
 * - use RES<sub>lookup</sub> to access the symmetric lookup table to find the equivalent offset ratio OR<sub>sym</sub>
 *
 * The above calculation must be done twice to calculate OR<sub>bal</sub> and OR<sub>unbal</sub>.
 *
 * An error exists if OR<sub>bal</sub> > ABS (OR<sub>sym</sub>) or ABS(OR<sub>bal</sub> - OR<sub>unbal</sub>) > "magic number"
 *
 * @todo Need to expand this section quite a lot
 *
 * <h2> Requirements </h2>
 *
 * The requirements for this feature can be found in <a href="http://viewvc/viewvc.cgi/trunk/docs/reqs/generic/RU_BMS_Support.doc?root=Q52717_NightWing&view=co">BMS Support Requirements</a>.
 * It is thoroughly recommended that the reader is also familiar with the document <a href="/Catc-filesrv-01/F/Q52717-McLaren P12 Battery Project Space/_UNCONTROLLED/Electrical Design/Design Notes/GFI/Ground Fault Indication 14-Mar-13.docx ">Operating Principles Of The Linkedriving Ground Fault Indicator</a>.
 *
 * | Requirement                | Code                    | Tested By                                   |
 * | ---------------------------|-------------------------|---------------------------------------------|
 * | REQ_BMS_CTC_VC_118 | fm_is_fault_monitored(), #Fm_pack_gnd_fault_asymmetric_crit_chg, #Fm_pack_gnd_fault_asymmetric_crit_chg | |
 * | REQ_BMS_GFI_001 | | |
 * | REQ_BMS_GFI_002 | | |
 * | REQ_BMS_GFI_003 | | |
 * | REQ_BMS_GFI_004 | | |
 * | REQ_BMS_GFI_005 | | |
 * | REQ_BMS_GFI_006 | | |
 * | REQ_BMS_GFI_011 | | |
 * | REQ_BMS_GFI_012 | | |
 * | REQ_BMS_GFI_013 | | |
 * | REQ_BMS_GFI_014 | | |
 * | REQ_BMS_SUP_GF_001 | #gf_data_s updated_since_closure | |
 * | REQ_BMS_SUP_GF_002 | #gf_data_s updated_since_closure | |
 * | REQ_BMS_SUP_GF_003 | gf_symmetric() | |
 * | REQ_BMS_SUP_GF_005 | #gf_data_s fault_present          | |
 * | REQ_BMS_SUP_GF_006 | #gf_data_s fault_present          | |
 * | REQ_BMS_SUP_GF_007 | #gf_data_s fault_since_closure    | |
 * | REQ_BMS_SUP_GF_008 | #gf_data_s fault_since_closure    | |
 * | REQ_BMS_SUP_GF_009 | gf_asym_determine_if_fault_present() | |
 * | REQ_BMS_SUP_GF_010 | gf_asym_determine_if_fault_present() | |
 * | REQ_BMS_SUP_GF_011 | gf_asym_determine_if_fault_present(), #gf_critical_level  | pat_gf_asym_critical_veh_neg_initial(), pat_gf_asym_critical_veh_pos_initial() |
 * | REQ_BMS_SUP_GF_012 | gf_asym_determine_if_fault_present(), #gf_critical_level  | pat_gf_asym_critical_veh_neg_initial() |
 * | REQ_BMS_SUP_GF_013 | gf_asym_determine_if_fault_present(), #gf_non_critical_level  | pat_gf_asym_non_critical_veh_neg_running(), pat_gf_asym_non_critical_veh_pos_running() |
 * | REQ_BMS_SUP_GF_014 | gf_asym_determine_if_fault_present(), #gf_critical_level  | pat_gf_asym_critical_chg_neg_initial(), pat_gf_asym_critical_chg_pos_initial() |
 * | REQ_BMS_SUP_GF_015 | gf_asym_determine_if_fault_present(), #gf_non_critical_level  | pat_gf_asym_non_critical_chg_neg_running(), pat_gf_asym_non_critical_chg_pos_running(), pat_gf_asym_non_critical_chg_pos_running_bleed() |
 * | REQ_BMS_SUP_GF_017 | gf_symmetric() | |
 * | REQ_BMS_SUP_GF_018 | #gf_critical_level | |
 * | REQ_BMS_SUP_GF_019 | #gf_non_critical_level | |
 * | REQ_BMS_SUP_GF_020 | gf_asym_determine_if_fault_present(), #gf_critical_level | |
 * | REQ_BMS_SUP_GF_022 | gf_asym_determine_if_fault_present(), #gf_critical_level  | pat_gf_asym_critical_veh_pos_initial() |
 * | REQ_BMS_SUP_GF_024 | gf_asym_determine_if_fault_present(), #gf_critical_level | |
 * | REQ_BMS_SUP_GF_026 | gf_asym_determine_if_fault_present(), #gf_critical_level  | pat_gf_asym_critical_chg_neg_initial() |
 * | REQ_BMS_SUP_GF_027 | gf_asym_determine_if_fault_present(), #gf_critical_level  | pat_gf_asym_critical_chg_pos_initial() |
 * | REQ_BMS_SUP_GF_029 | gf_asym_determine_if_fault_present(), #gf_non_critical_level | |
 * | REQ_BMS_SUP_GF_031 | gf_asym_determine_if_fault_present(), #gf_non_critical_level | |
 * | REQ_BMS_SUP_GF_033 | gf_asym_determine_if_fault_present(), #gf_non_critical_level  | |
 * | REQ_BMS_SUP_GF_034 | gf_asym_determine_if_fault_present(), #gf_non_critical_level  | |
 * | REQ_BMS_SUP_GF_035 | gf_asym_determine_if_fault_present(), #gf_non_critical_level  | pat_gf_asym_non_critical_veh_neg_running() |
 * | REQ_BMS_SUP_GF_036 | gf_asym_determine_if_fault_present(), #gf_non_critical_level  | pat_gf_asym_non_critical_veh_pos_running() |
 * | REQ_BMS_SUP_GF_038 | gf_asym_determine_if_fault_present(), #gf_non_critical_level  | pat_gf_asym_non_critical_chg_neg_running(), pat_gf_asym_non_critical_chg_neg_running_bleed_dormant(), pat_gf_asym_non_critical_chg_neg_running_bleed_init(), pat_gf_asym_non_critical_chg_neg_running_bleed_discharge(), pat_gf_asym_non_critical_chg_neg_running_bleed_shutdown(), pat_gf_asym_non_critical_chg_neg_running_bleed_m1_fault(), pat_gf_asym_non_critical_chg_neg_running_bleed_m2_fault(), pat_gf_asym_non_critical_chg_neg_running_bleed_m3_fault(), pat_gf_asym_non_critical_chg_pos_running(), pat_gf_asym_non_critical_chg_pos_running_bleed() |
 * | REQ_BMS_SUP_GF_039 | gf_asym_determine_if_fault_present(), #gf_non_critical_level  | |
 * | REQ_BMS_SUP_GF_041 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_042 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_043 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_044 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_045 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_046 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_047 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_048 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_049 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_050 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_051 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_052 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_053 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_054 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_055 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_056 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_057 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_058 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_059 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_060 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_061 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_062 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_063 | fm_is_fault_monitored(), #Fm_pack_gnd_fault_asymmetric_crit_veh | |
 * | REQ_BMS_SUP_GF_063 | gf_asym_determine_if_fault_present(), #gf_critical_level  | |
 * | REQ_BMS_SUP_GF_064 | fm_is_fault_monitored(), #Fm_pack_gnd_fault_asymmetric_crit_chg | |
 * | REQ_BMS_SUP_GF_064 | gf_asym_determine_if_fault_present(), #gf_critical_level  | |
 * | REQ_BMS_SUP_GF_065 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_066 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_067 | gf_asym_determine_if_fault_present(), #gf_critical_level  | pat_gf_asym_critical_veh_neg_running(), pat_gf_asym_critical_veh_pos_running() |
 * | REQ_BMS_SUP_GF_068 | gf_asym_determine_if_fault_present(), #gf_critical_level  | pat_gf_asym_critical_veh_neg_running() |
 * | REQ_BMS_SUP_GF_069 | gf_asym_determine_if_fault_present(), #gf_critical_level  | pat_gf_asym_critical_veh_pos_running() |
 * | REQ_BMS_SUP_GF_070 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_071 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_072 | gf_asym_determine_if_fault_present(), #gf_critical_level  | pat_gf_asym_critical_chg_neg_running(), pat_gf_asym_critical_chg_neg_running_bleed(), pat_gf_asym_critical_chg_pos_running(), pat_gf_asym_critical_chg_pos_running_bleed() |
 * | REQ_BMS_SUP_GF_073 | gf_asym_determine_if_fault_present(), #gf_critical_level  | pat_gf_asym_critical_chg_neg_running(), pat_gf_asym_critical_chg_neg_running_bleed() |
 * | REQ_BMS_SUP_GF_074 | gf_asym_determine_if_fault_present(), #gf_critical_level  | pat_gf_asym_critical_chg_pos_running(), pat_gf_asym_critical_chg_pos_running_bleed() |
 * | REQ_BMS_SUP_GF_075 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_076 | Symmetric ground fault detection is not implemented | |
 * | REQ_BMS_SUP_GF_077 | Symmetric ground fault detection is not implemented | |
 *
 * <h2> Limitations </h2>
 * Due to the complex nature of the calculations required, this feature will use floating point arithmetic
 * in order to perfrom the calculations.
 *
 * @defgroup gf GF Ground Fault Indication
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "gf_types.h"
#include "nvs_cfg.h"    /* For NVS configuration */

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern CAL ohm_per_volt_t gf_critical_level;
extern CAL ohm_per_volt_t gf_non_critical_level;
extern CAL pack_v_t gf_dc_link_voltage;
extern       struct gf_data_s       gf_asymmetric_data      [Gf_max_buses];

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

bool_t       gf_test_complete(enum gf_bus_e bus);
void FCODE   gf_init         (void);
void FCODE   gf_update       (void);

#endif      /* GF_H */

