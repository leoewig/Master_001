/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   icm_fault_p.h
 *
 * description:  
 * Internal comms fault detection private header
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(ICM_FAULT_P_H)
#define      ICM_FAULT_P_H
/**
 *
 * Internal comms fault detection private header
 *
 * @file
 * @ingroup icm_fault
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/*
 * enumerations and macro definitions made available must be documented here to
 * avoid duplication.
 *
 * PRIVATE functions are documented in the source file.
 * Non-PRIVATE variables and functions (i.e. those shared between files in the
 * feature) are documented here.
 */

/**
 * Data structure used to represent the state of a fault for a given
 * foreground invocation in ICM */
struct Icm_flt_s
{
    /** The subtype of the fault that was tested */
    Fm_fault_subtype_list_e    fault_subtype;
    bool_t              detected;
    bool_t              inspected;
};

#if defined(UNIT_TEST) && !defined(_lint)
extern FRAM struct Icm_flt_s icm_faults[Icm_flt_count] ;

extern FRAM uint16_t       icm_invalid_cell_rec;
extern uint16_t       icm_invalid_cellpair_rec;
extern uint16_t       icm_unexpect_cellsig_rec;
extern uint16_t       icm_invalid_vtbm_cellmsg_rec;
extern uint16_t       icm_invalid_ref_vtbm_rec;
extern uint16_t       icm_unexpect_ref_rec;
extern uint16_t       icm_unexpect_cellmsg_rec;

#endif

PRIVATE Fm_fault_subtype_list_e icm_vtbm_to_subtype(uint8_t vtbm_num);

PRIVATE void icm_chk_uncommanded_bal(uint8_t nvtbm, enum BalAction_e action);

PRIVATE void icm_chk_unactioned_bal(uint8_t nvtbm, enum BalAction_e action,
    uint8_t num_cell);

PRIVATE void icm_chk_plat_cell_t_min(const FPTR struct xg_s12_data_s * datap);
PRIVATE void icm_chk_plat_cell_t_max(const FPTR struct xg_s12_data_s * datap);
PRIVATE void icm_chk_plat_cell_t_open(const FPTR struct xg_s12_data_s * datap);
PRIVATE void icm_chk_plat_cell_t_short(const FPTR struct xg_s12_data_s * datap);

void icm_chk_unexpected(const volatile FPTR uint16_t  *new_ctr_p, 
    uint16_t FPTR  *old_ctr_p, enum IcmFault_e fault,
    Fm_fault_subtype_list_e subtype );

void icm_chk_plat_ref_unexpected(void);

#endif      /* ICM_FAULT_P_H */

