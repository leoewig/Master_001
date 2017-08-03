/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   fm_p.h 
 *
 * description:  
 * Private header for Fm
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
/*************************************************************************//**
 *
 * Private header for Fm
 *
 *
 *****************************************************************************
 *
 * This header makes internal functions and data of Fm available to a
 * unit test harness.
 */

/**
 * @addtogroup FM
 * @{
 * @file
 * @}
 */
#if !defined(FM_P_H)
#define      FM_P_H

#include "utils.h"
#include "fm_types.h"

/** Macro to assign the value to the elements of the enum Fm_fail_func_val_e*/
#define FM_SET_BIT_POSITION_VAL(x) (1uL << (x))

/** Macro to detect which failure function is linked to a particular fault */
#define FM_BIT_SHIFT_32_BIT_VAR 0x01uL

/**
 * Macros to read and write the elements of a fault's status.
 */

/*
 * Check that all subtypes will fit in 7 bits.
 * This relies on Fm_general_failure_information being the last member
 * of the subtype enumeration. 
 * If this needs to change, note that OBD snapshots assume the sub-type 
 * can be represented by a uint8_t.
 */
STATIC_ASSERT(Fm_general_failure_information <= 127,
              Too_many_subtypes_for_FM_to_pack);

/* Use bit 7 to indicate whether a fault is present or not */
#define FAULT_PRESENT_MASK      (uint8_t)0x80u

/* Use all the other bits to indicate the fault subtype */
#define FAULT_SUBTYPE_MASK      (0xffu & (uint8_t)~FAULT_PRESENT_MASK)

/* Is a a fault present */
#define IS_FAULT_PRESENT(fault) \
    ((fm_ram_image.data[fault].fault_status & FAULT_PRESENT_MASK) != 0u)

/* Mark a fault as being present */
#define SET_FAULT(fault) \
    (fm_ram_image.data[fault].fault_status |= FAULT_PRESENT_MASK)

/* Mark a fault as being absent */
#define CLEAR_FAULT(fault) \
    (fm_ram_image.data[fault].fault_status &= (uint8_t)~FAULT_PRESENT_MASK)

/* Retrieve the subtype stored for a fault */
#define GET_FAULT_SUBTYPE(fault) \
    ((Fm_fault_subtype_list_e)(fm_ram_image.data[fault].fault_status & \
                               FAULT_SUBTYPE_MASK))
#define SET_FAULT_SUBTYPE(fault, subtype) \
    (fm_ram_image.data[fault].fault_status = \
        (fm_ram_image.data[fault].fault_status \
            & (uint8_t)~FAULT_SUBTYPE_MASK) | \
        (uint8_t)(subtype))

/******************************************************************************
 * FUNCTION DECLARATION
 *****************************************************************************/
PRIVATE FCODE void fm_update_debug_faults(const Fm_fault_list_e flt_idx, const Fm_fault_subtype_list_e subt);
PRIVATE FCODE bool_t determine_filter_output(const Fm_fault_list_e flt_type,
                                             const Fm_fault_subtype_list_e flt_subtype,
                                             const uint8_t flt_count);
/* Reset fault filters bepending on latch type specificed. */
PRIVATE FCODE void reset_all_fault_filter_type(const Fm_latch_type_e reset_latch);
/* Activate the failure functions and fault groups during an init event for all
 * "Service_latch" fault filters. */
PRIVATE FCODE void activate_fault_functions(void);
/* Activate the failure functions associated with a given fault type. */
PRIVATE FCODE void activate_fault_fail_func(const Fm_fault_list_e flt_type);
/* Activate the failure functions associated with a given fault type. */
PRIVATE FCODE void activate_fault_group(const Fm_fault_list_e flt_type);
/* Deactivate the failure functions associated with a given fault type. */
PRIVATE FCODE void deactivate_fault_fail_func(const Fm_fault_list_e flt_type);
/* Deactivate the fault group associated with a given fault type. */
PRIVATE FCODE void deactivate_fault_group(const Fm_fault_list_e flt_type);
/*Decrement the fault filter count */
PRIVATE FCODE void fm_dec_flt_count(Fm_fault_list_e flt_indx, uint8_t dec_rate);
/*Increment the fault filter count */
PRIVATE FCODE void fm_inc_flt_count(Fm_fault_list_e flt_indx, uint8_t inc_rate);
/* Function to clear a single fault */
PRIVATE FCODE void fm_clear_fault(const Fm_fault_list_e fault_type, 
                                  bool_t notify_obd);

/******************************************************************************
 * VARIABLES
 *****************************************************************************/
#if defined(UNIT_TEST) && !defined(_lint)

extern FRAM Fm_ignition_status_e prev_ignition_input;
extern FRAM Fm_ignition_status_e ignition_input;

extern FRAM uint16_t fail_func_active_count[Fm_max_failure_function];
extern FRAM uint16_t fault_group_active_count[Fm_max_fault_cluster];
extern FRAM struct fm_flt_fltr_s fault_fltr[Fm_max_fault_types];

extern PRIVATE CAL fm_calibration_s Fm_fault_cal[Fm_max_fault_types];
#endif      /* UNIT_TEST */

#endif      /* FM_P_H */

