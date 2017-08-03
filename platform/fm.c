/**
 * @file
 *
 * Routines for controlling the Fault Management Interface Module.
 *
 * $Id: fm.c 11799 2014-02-14 11:49:50Z dc17 $
 * Fault Management Interface Implementation
*/
/**
 * @ingroup FM
 * @{
 * @file
 * @}
 */
/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "fm.h"         /* Module Header file */
#include "fm_p.h"       /* Private Header File */
#include "dio_cfg.h"    /* For Ignition Toggling signal */
#include "nvs.h"        /* For access to nvs macro */
#include "fixedpoint.h" /* for access to max function */
#include "hardware.h"   /* for access to interrupt enable/disable*/
//#include "obd.h"        /* for access to OBD events */
//#include "vmi.h"        /* for access to supply voltage */
#include "vio.h"        /* for access to vehicle ignition status (crank) */
#include "watchdog.h"   /* for access to kick watchdog function */

/******************************************************************************
 * MACROS
 *****************************************************************************/
/**
 * Used to define the bit position for a fail function defined by its enum value */
#define SET_FAIL_FUNC(func_name)    FM_SET_BIT_POSITION_VAL(Fm_ ## func_name)
/**
 * Used to define the bit position for a fault group defined by its enum value */
#define SET_FAULT_GROUP(group_name) FM_SET_BIT_POSITION_VAL(Fm_ ## group_name)

/******************************************************************************
 * CALIBRATABLE PARAMETERS
 *****************************************************************************/
#define MEMORY_LOCATION CALIBRATION_MEMORY_S12
#include "memory_layout.h"

/*
 * currently there is a switch here between the order defined in the mmc_fm.def table and
 * the order defined in the structure, its possible to be removed in the fillness of time.
 * The new order defined here is a function of sensible structure layout rather that what
 * reads nicely to a human-bean in the .def table.
 */
#undef FAULT_TABLE
/**
 * This macro defines the conversion from the table to the calibration structure  */
#define FAULT_TABLE(Fault_type, failure_absent_inc_rate, failure_absent_dec_rate, \
                    flt_threshold, failure_present_inc_rate, failure_present_dec_rate, \
                    fail_func_link, fault_group_link, obd_relevant, latch_type, \
                    mon_cond_count_down, mon_flags, description_string, diagnosis_string ) \
{failure_absent_inc_rate, failure_absent_dec_rate, failure_present_inc_rate, failure_present_dec_rate, flt_threshold, mon_cond_count_down, obd_relevant, latch_type, fail_func_link, fault_group_link,mon_flags},

/**
 * This defines the fault calibration data structure. The default population is
 * defined via the file mmc_fm.def.
 */
PRIVATE CAL fm_calibration_s Fm_fault_cal[Fm_max_fault_types] = {
    #include "fm.def"
};

/* make macro innocuous */
#undef  FAULT_TABLE

/** The number of fault clusters/groups is determined by the number of bits in
 * the fault group map field. If this was needed in a header it could be 
 * redefined to use a fake pointer to the type (given the poiner is never
 *  derefenced), but this feels safer. */
#define MAX_AVAILABLE_FAULT_GROUP (8u*sizeof(Fm_fault_cal[0].fault_group_map))

/**
 * max possible number of fail functions given the width of the field they are
 * stored in */
#define MAX_AVAILABLE_FAIL_FUNC (8u*sizeof(Fm_fault_cal[0].failure_functions_map))

STATIC_ASSERT( (size_t)Fm_max_fault_cluster <= MAX_AVAILABLE_FAULT_GROUP,
              Fm_too_many_fault_groups_defined);

STATIC_ASSERT((size_t)Fm_max_failure_function <= MAX_AVAILABLE_FAIL_FUNC,
              Fm_too_many_failure_functions_defined);

/******************************************************************************
 * VARIABLES DEFINITIONS
 *****************************************************************************/
#define MEMORY_LOCATION S12_MEMORY
#include "memory_layout.h"

/* Debug Data - Start */
Fm_fault_list_e fm_fault_first;
/**<
 * @a2l{LongIdentifier, Debug variable to store the first fault number}
 */

Fm_fault_subtype_list_e fm_subtype_first;
/**<
 * @a2l{LongIdentifier, Debug variable to store the first fault sub-type}
 */

Fm_fault_list_e fm_fault_second;
/**<
 * @a2l{LongIdentifier, Debug variable to store the second fault number}
 */

Fm_fault_subtype_list_e fm_subtype_second;
/**<
 * @a2l{LongIdentifier, Debug variable to store the second fault sub-type}
 */

Fm_fault_list_e fm_fault_latest;
/**<
 * @a2l{LongIdentifier, Debug variable to store the latest fault number}
 */
Fm_fault_subtype_list_e fm_subtype_latest;
/**<
 * @a2l{LongIdentifier, Debug variable to store the latest fault sub-type}
 */

uint8_t fm_debug_ctr = 0u;
/* Debug Data - End */

/**
 * FM NV Data
 */
FRAM fm_nvs_wrapper_s fm_ram_image =
{
    NVS_HEADER_INIT(FM_NVS_LAYOUT_VERSION),
    { /* data on each fault*/
        {0u}
    },
    NVS_RESERVED_BYTES_INIT
};

PRIVATE Fm_ignition_status_e prev_ignition_input = Fm_dont_know;
/**<
 * @a2l{conversion, Fm_ignition_status_e}
 * @a2l{LongIdentifier, Variable to store the previous value of BMS_Enable
signal which represents the ignition state signal}
 */

PRIVATE Fm_ignition_status_e ignition_input = Fm_dont_know;
/**<
 * @a2l{conversion, Fm_ignition_status_e}
 * @a2l{LongIdentifier, Variable to capture the ignition state (BMS_Enable signal)
on Port T pin 2 using the dio module}
 */

/**
 * This array maintains a count of the number of fault that are detected and
 * have an associated fail function link. The relevent fail function counter
 * is incremented by 1 for each fault that is detected and accociated with
 * the fail function. If a fault becomes undetected and is accociated with the
 * fail function then 1 is subtracted from the fail function counter.<br>
 * When the fail function count is greater than zero the fail function is 'active'
 * and the function fm_is_fail_function_active() will return TRUE for that
 * fail function.<br>
 * When the fail function counter returns to zero then the fail function is
 * 'inactive' and the function fm_is_fail_function_active() will return FALSE.
 */
PRIVATE FRAM uint16_t fail_func_active_count[Fm_max_failure_function] = {0u};
/**<
 * @a2l{conversion, uint16_t}
 * @a2l{LongIdentifier, It indicates the number of fault filters contributing
 to a particular failure function}
 */

/**
 * This array maintains a count of the number of fault that are detected and
 * have an associated fault group link. The relevent fault group counter
 * is incremented by 1 for each fault that is detected and accociated with
 * the failt group. If a fault becomes undetected and is accociated with the
 * fault group then 1 is subtracted from the fault group.<br>
 * When the fault group count is greater than zero the fault group is 'active'
 * and the function fm_is_fault_group_active() will return TRUE for that
 * fault group.<br>
 * When the fault group counter returns to zero then the fault group is
 * 'inactive' and the function fm_is_fault_group_active() will return FALSE.
 */
PRIVATE FRAM uint16_t fault_group_active_count[Fm_max_fault_cluster] = {0u};
/**<
 * @a2l{conversion, uint16_t}
 * @a2l{LongIdentifier, It indicates the number of fault filters contributing
 to a particular failure function}
 */


/**
 * The fault filter structure (#fm_flt_fltr_s) contains the current fault filter
 * status for each fault. This consists of;<br>
 * - A flag indicating whether or not the fault filter has been updated since
 *   last reset<br>
 * - the fault filter counter.<br>
 * Other status information regarding fault filter is located in the nvs data
 * structure #fm_ram_image which is of type #fm_nvs_wrapper_s.
 */
PRIVATE FRAM struct fm_flt_fltr_s fault_fltr[Fm_max_fault_types];
/**<
 * @a2l{[].update_last_reset, conversion, bool_t}
 * @a2l{[].update_last_reset, LongIdentifier, Flag to Indicate if an update has
happened since last reset occured}
 * @a2l{[].flt_count, conversion, uint8_t}
 * @a2l{[].flt_count, LongIdentifier, Indicates the fault filter count}
 */


/******************************************************************************
 * PRIVATE FUNCTION DEFINITIONS
 *****************************************************************************/

/***************************************************************************//**
 *
 * The function fm_update_debug_faults updates #fm_fault_first, #fm_fault_second
 * and #fm_fault_latest following an ignition/power cycle. This will repopulate
 * teh faults debug information to inform the calibrator that active faults have
 * been recovered from nvm (which may have set failure functions or fault groups).
 * @note The information returned to the fault debuging variables will not
 * necessarily be in the right order. But will at least provide the calibrator
 * with information that faults store in NVM have been recovered.
 *
 ******************************************************************************/
PRIVATE FCODE void fm_update_debug_faults(const Fm_fault_list_e flt_idx, const Fm_fault_subtype_list_e subt)
{
    switch (fm_debug_ctr)
    {
        case 0:
            fm_fault_first   = flt_idx;
            fm_subtype_first = subt;
            fm_debug_ctr++;
        break;
        case 1:
            fm_fault_second   = flt_idx;
            fm_subtype_second = subt;
            fm_debug_ctr++;
        break;
        default:
            /* dont allow the counter to ever wrap */
            fm_debug_ctr = 2u;
        break;
    }

    fm_fault_latest   = flt_idx;
    fm_subtype_latest = subt;
 }


/***************************************************************************//**
 *
 * The function activate_fault_functions() is called by the fm_init() function
 * during the FM initialization to activate the failure functions (by increment
 * the associated failure function counters) for all fault filters that have not
 * been cleared (due to their latch type) following recovery from NVS.
 *
 ******************************************************************************/
PRIVATE FCODE void activate_fault_functions(void)
{
    Fm_fault_list_e flt_idx;

    for(flt_idx = (Fm_fault_list_e)0u; flt_idx < Fm_max_fault_types; flt_idx++)
    {
        /* Increment the Failure function count for the Service Latch.*/
        if (IS_FAULT_PRESENT(flt_idx))
        {
            /* Activate all the associated failure functions. */
            activate_fault_fail_func(flt_idx);
            /* Activate all the assocated fault groups */
            activate_fault_group(flt_idx);
            /* capture the fact that recovered faults exist
             * to give the calibrator a fighting chance */
            fm_update_debug_faults(flt_idx, GET_FAULT_SUBTYPE(flt_idx));
        }
        else
        {
            /* Do Nothing */
        }
    }
}


/***************************************************************************//**
 *
 * The function activate_fault_fail_func() is called to activate the failure
 * functions associated with the specified fault, when the fault is confirmed to
 * be a failure. i.e, when the fault counter reaches its upper threshold limit.
 *
 * @param flt_idx specifies the fault type
 ******************************************************************************/
PRIVATE FCODE void activate_fault_fail_func(const Fm_fault_list_e flt_idx)
{
    uint8_t fail_func = 0u;
    uint8_t ccr;

    /* Increment the Failure function count for the specified fault filter.*/
    for(fail_func = 0u;
        fail_func < (uint8_t)Fm_max_failure_function;
        fail_func++)
    {
        /* Find all the associated failure functions with the fault. */
        if((Fm_fault_cal[flt_idx].failure_functions_map &
             (FM_BIT_SHIFT_32_BIT_VAR << fail_func)) != 0u)
        {
            /* ensure thred safe access */
            SAVE_INTERRUPTS(ccr);
            /* Increment the failure function active count */
            if (fail_func_active_count[fail_func] < UINT16_MAX)
            {
                fail_func_active_count[fail_func] += 1u;
            }
            RESTORE_INTERRUPTS(ccr);
        }
        else
        {
            /* Do Nothing */
        }
    }
}


/***************************************************************************//**
 *
 * The function activate_fault_group() is called to activate the failure
 * functions associated with the specified fault, when the fault is confirmed to
 * be a failure. i.e, when the fault counter reaches its upper threshold limit.
 *
 * @param flt_idx specifies the fault type
 ******************************************************************************/
PRIVATE FCODE void activate_fault_group(const Fm_fault_list_e flt_idx)
{
    uint8_t group_idx = 0u;
    uint8_t ccr;

    /* Increment the Failure function count for the specified fault filter.*/
    for(group_idx = 0u;
        group_idx < (uint8_t)Fm_max_fault_cluster;
        group_idx++)
    {
        /* Find all the associated failure functions with the fault. */
        if((Fm_fault_cal[flt_idx].fault_group_map &
             (FM_BIT_SHIFT_32_BIT_VAR << group_idx)) != 0u)
        {
           /* ensure thred safe access */
            SAVE_INTERRUPTS(ccr);
            /* Increment the fault group active count  */
            if (fault_group_active_count[group_idx] < UINT16_MAX)
            {
                fault_group_active_count[group_idx] += 1u;
            }
            RESTORE_INTERRUPTS(ccr);
        }
        else
        {
            /* Do Nothing */
        }
    }
}

/***************************************************************************//**
 *
 * The function deactivate_fault_fail_func() is called by two functions:
 * + determine_filter_output(): to deactivate the failure
 * functions associated with the specified No latch fault filter type. If
 * the failure is confirmed, and the fault counter has been updated and has
 * reached 0, this function deactivates the failure functions assocaited
 * with the specified fault by decrementing the corresponding failure function
 * counters.
 *
 * + fm_reset_fault_filter(): to deactivate the failure
 * functions associated with the specified fault filter type. It deactivates the
 * failure functions assocaited with the specified fault by decrementing the
 * corresponding failure function counters.
 *
 * @param flt_idx specifies the fault type
 ******************************************************************************/
PRIVATE FCODE void deactivate_fault_fail_func(const Fm_fault_list_e flt_idx)
{
    uint8_t fail_func = 0u;
    uint8_t ccr;

    /*  Decrement the Failure function count for the specified fault filter.*/
    for(fail_func = 0u;
        fail_func < (uint8_t)Fm_max_failure_function;
        fail_func++)
    {
        /* Find all the associated failure functions with the fault. */
        if((Fm_fault_cal[flt_idx].failure_functions_map &
              (FM_BIT_SHIFT_32_BIT_VAR << fail_func)) != 0u)
        {
            /* ensure thred safe access */
            SAVE_INTERRUPTS(ccr);
            if(fail_func_active_count[fail_func] > 0u)
            {
                /* Decrement the failure function active count*/
                fail_func_active_count[fail_func] -= 1u;
            }
            RESTORE_INTERRUPTS(ccr);
        }
    }
}

/**
 * As above only for fault groups
 */
PRIVATE FCODE void deactivate_fault_group(const Fm_fault_list_e flt_idx)
{
    uint8_t group_idx = 0u;
    uint8_t ccr;

    /*  Decrement the Failure function count for the specified fault filter.*/
    for(group_idx = 0u;
        group_idx < (uint8_t)Fm_max_fault_cluster;
        group_idx++)
    {
        /* Find all the associated failure functions with the fault. */
        if((Fm_fault_cal[flt_idx].fault_group_map &
             (FM_BIT_SHIFT_32_BIT_VAR << group_idx)) != 0u)
        {
            /* ensure thred safe access */
            SAVE_INTERRUPTS(ccr);
            if(fault_group_active_count[group_idx] > 0u)
            {
                /* Decrement the failure function active count for the
                associated failure functions. */
                fault_group_active_count[group_idx] -= 1u;
            }
            RESTORE_INTERRUPTS(ccr);
        }
    }
}



/***************************************************************************//**
 * The main objective of this function is to determine the fault filter output.
 * This function is invoked by fm_update_fault_filter() to
 * determine the filter output after incrementing/decrementing the filter fault
 * counter appropriately.  The determine_filter_output() function then uses
 * this fault counter to determine the output of the filter.
 * If the counter reaches its threshold, the filter output is set to TRUE to
 * confirm the failure. It then calls activate_fault_fail_func()
 * to activate the associated failure functions.
 * If the counter reaches its lower saturation limit, the filter output is set
 * to FALSE to indicate the absence of failure. It then calls
 * deactivate_fault_fail_func()
 * to deactivate the associated failure functions.
 *
 * @param flt_idx Indicates the type of fault for which the filter is to be
 *                  updated.
 * @param flt_subtype Indicates the fault sub-type of the fault type.
 * @param flt_count Indicates the filter's fault counter value.
 *
 * @retval TRUE Output of filter, indicating presence of failure
 * @retval FALSE Output of filter, indicating absence of failure.
 *
 ******************************************************************************/
PRIVATE FCODE bool_t determine_filter_output(
                                    const Fm_fault_list_e flt_idx,
                                    const Fm_fault_subtype_list_e flt_subtype,
                                    const uint8_t flt_count
                                            )
{
    bool_t filter_output = FALSE;

    /* if the fault count == to the threshold value && the failure is not present */
    if((Fm_fault_cal[flt_idx].threshold == flt_count) &&
        (!IS_FAULT_PRESENT(flt_idx)))
    {
        /* Set the filter output to TRUE to indicate the presence of failure.*/
        SET_FAULT(flt_idx);
        /* Store the fault Subtype in the FM data set. */
        SET_FAULT_SUBTYPE(flt_idx, flt_subtype);
        /* Activate the associated failure functions for
            the specified fault filter.*/
        activate_fault_fail_func(flt_idx);
        /* Activate all the assocated fault groups */
        activate_fault_group(flt_idx);
        /* update debug variables */
        fm_update_debug_faults(flt_idx, flt_subtype);
        /* Let OBD know  */
        //obd_fault_present_event(flt_idx, flt_subtype);
    }

    /* is the fault count 0  && the failure is present */
    else if((0u == flt_count) && (IS_FAULT_PRESENT(flt_idx)))
    {
        Fm_fault_subtype_list_e subtype = GET_FAULT_SUBTYPE(flt_idx);

        /* && its not a latching fault */
        if (Fm_no_latch == Fm_fault_cal[flt_idx].latch)
        {
            /* Set the filter output to FALSE to indicate absence of failure */
            CLEAR_FAULT(flt_idx);
            SET_FAULT_SUBTYPE(flt_idx, Fm_general_failure_information);

            /* Deactivate the associated failure functions for
                the specified fault filter.*/
            deactivate_fault_fail_func(flt_idx);
            deactivate_fault_group(flt_idx);
        }
        /* Let OBD know, regardless of latching */
        //obd_fault_not_present_event(flt_idx, subtype);
    }
	
	else if((0u == flt_count) && (!IS_FAULT_PRESENT(flt_idx)))
    {
		fm_ram_image.data[flt_idx].fault_status = 0u;
		
		if(fm_fault_latest == (Fm_fault_list_e)0&&fm_fault_second== (Fm_fault_list_e)0)
			fm_debug_ctr;
		
		if(fm_fault_first == flt_idx)
		{
			fm_fault_first = fm_fault_second;
			fm_subtype_first = fm_subtype_second;
			fm_fault_second = fm_fault_latest;
			fm_subtype_second = fm_subtype_latest;
			fm_fault_latest = (Fm_fault_list_e)0;
			fm_subtype_latest = (Fm_fault_subtype_list_e)0;	
			
			if(fm_fault_first== (Fm_fault_list_e)0&&fm_fault_second== (Fm_fault_list_e)0)
			{
				fm_debug_ctr=0u;
			}
			else if (fm_fault_first!= (Fm_fault_list_e)0&&fm_fault_second== (Fm_fault_list_e)0)
			{
				fm_debug_ctr=1u;
			}
			
		}
		else if (fm_fault_second == flt_idx)
		{
			fm_fault_second = fm_fault_latest;
			fm_subtype_second = fm_subtype_latest;
			fm_fault_latest = (Fm_fault_list_e)0;
			fm_subtype_latest = (Fm_fault_subtype_list_e)0;	
			
			if(fm_fault_first== (Fm_fault_list_e)0&&fm_fault_second== (Fm_fault_list_e)0)
			{
				fm_debug_ctr=0u;
			}
			else if (fm_fault_first!= (Fm_fault_list_e)0&&fm_fault_second== (Fm_fault_list_e)0)
			{
				fm_debug_ctr=1u;
			}
		}
		/*else if (fm_fault_latest == flt_idx)
		{
			fm_fault_latest = (Fm_fault_list_e)0;
			fm_subtype_latest = (Fm_fault_subtype_list_e)0;	
			
			if(fm_fault_first== (Fm_fault_list_e)0&&fm_fault_second== (Fm_fault_list_e)0)
			{
				fm_debug_ctr=0u;
			}
			else if (fm_fault_first!= (Fm_fault_list_e)0&&fm_fault_second== (Fm_fault_list_e)0)
			{
				fm_debug_ctr=1u;
			}
		}*/
		else 
		{
			
		}
		
			
	}

    else
    {
        /* the fault was neither detected or undetetced */
    }

    /* Set the return value.*/
    filter_output = IS_FAULT_PRESENT(flt_idx);

    return filter_output;
}


/***************************************************************************//**
 * The function reset_all_fault_filter_type() is used to reset the
 * the fault faults that have a latch type that matches the parameter reset_latch.
 * #Fm_no_latch faults are cleared down during initialization.
 * #Fm_ignition_cycle_latch are cleared down when ignition on is detected.
 * For each fault the following is cleared down.
 *
 * -# Fault filter output and subtype, fm_data_set_s::fault_status
 * -# Update last reset flag, struct fm_flt_fltr_s::update_last_reset
 * -# Fault counter, fm_flt_fltr_s::flt_count
 *
 * It calls fm_reset_fault_filter() to reset the filter and to deactivate
 * the associated failure functions.
 *
 ******************************************************************************/
PRIVATE FCODE void reset_all_fault_filter_type(const Fm_latch_type_e reset_latch)
{
    Fm_fault_list_e flt;

    /* Determine the Filter Output during the init, upon restoration and upon
     detecting the ignition state input signal toggling from LOW->HIGH.*/

    /* Reset all the targeted latch type filters */
    for(flt = (Fm_fault_list_e) 0u; flt < Fm_max_fault_types; flt++)
    {
        if (reset_latch == Fm_fault_cal[flt].latch)
        {
            fm_reset_fault_filter(flt);
        }
        else
        {
            /* Do Nothing */
        }
    }
}


/***************************************************************************//**
 * Decrements fault count of the specified fault type by dec_rate only if
 * the dec rate is not more than flt count, otherwise it resets the fault
 * count to 0.<br>Also mark fault filter as updated.
 *
 * @param flt_indx Fault type for which the fault count is to be decremented.
 * @param fault_detected flag indicating whether the fault has reached it's
 * threshold.
 ******************************************************************************/
PRIVATE FCODE void fm_dec_flt_count(Fm_fault_list_e flt_indx, bool_t fault_detected)
{
    uint8_t dec_rate = (fault_detected) ?
                       Fm_fault_cal[flt_indx].detected_dec_rate :
                       Fm_fault_cal[flt_indx].undetected_dec_rate;

    /* down count undetected faults, or detected and non-latching faults */
    if ((FALSE == fault_detected) ||
        ((TRUE == fault_detected) && (Fm_no_latch == Fm_fault_cal[flt_indx].latch)))
    {
        /* if the remanding fault count greater than dec_rate */
        if(fault_fltr[flt_indx].flt_count >= dec_rate)
        {   /* subtract dec rate */
            fault_fltr[flt_indx].flt_count -= dec_rate;
        }
        else
        {   /* reset of zero */
            fault_fltr[flt_indx].flt_count = FM_RESET_FLT_COUNT;
        }
    }
    /* mark fault filter as updated since reset */
    fault_fltr[flt_indx].update_last_reset = TRUE;
}

/***************************************************************************//**
 * Increament the fault count for the specified filter and saturate at the
 * fault filter threshold.<br> Care is taken not to overflow the byte sized
 * fault filter as we dont know the value of the calibrated increments.<br>
 * Also mark fault filter as updated.
 *
 * @param flt_indx Fault type for which the fault count is to be incremented.
 * @param fault_detected flag indicating whether the fault has reached it's
 * threshold.
 ******************************************************************************/
PRIVATE FCODE void fm_inc_flt_count(Fm_fault_list_e flt_indx, bool_t fault_detected)
{
    uint16_t temp_fltr;
    uint16_t inc_rate = (fault_detected) ?
                        (uint16_t)Fm_fault_cal[flt_indx].detected_inc_rate :
                        (uint16_t)Fm_fault_cal[flt_indx].undetected_inc_rate;

    /* is the fault filter under the threshold? */
    if(((uint16_t)fault_fltr[flt_indx].flt_count + inc_rate) < (uint16_t)Fm_fault_cal[flt_indx].threshold)
    {   /* increment the filter, ensure we dont overflow */
        temp_fltr = (uint16_t)fault_fltr[flt_indx].flt_count + inc_rate;

        /* saturate filter to threshold (which is byte sized) */
        if (temp_fltr > (uint16_t)Fm_fault_cal[flt_indx].threshold)
        {
            fault_fltr[flt_indx].flt_count = Fm_fault_cal[flt_indx].threshold;
        }
        else
        {
            fault_fltr[flt_indx].flt_count = (uint8_t)temp_fltr;
        }
    }
    else
    {   /* reinforce saturation */
        fault_fltr[flt_indx].flt_count = Fm_fault_cal[flt_indx].threshold;
    }
    /* mark fault filter as updated since reset */
    fault_fltr[flt_indx].update_last_reset = TRUE;

}


/***************************************************************************//**
 * Increament the update count for the specified filter and saturate at the
 * fault filter threshold.
 *
 * @param flt_indx Fault type for which the fault count is to be incremented.

 ******************************************************************************/
PRIVATE FCODE void fm_inc_update_count(Fm_fault_list_e flt_indx)
{
    uint16_t temp_fltr;
    uint16_t inc_rate = Fm_fault_cal[flt_indx].undetected_inc_rate;

    /* is the update counter under the threshold? */
    if(fault_fltr[flt_indx].update_count < Fm_fault_cal[flt_indx].threshold)
    {   /* increment the update counter , ensure we dont overflow */
        temp_fltr = fault_fltr[flt_indx].update_count + inc_rate;

        /* saturate update counter to threshold (which is byte sized) */
        if (temp_fltr >= (uint16_t)Fm_fault_cal[flt_indx].threshold)
        {
            fault_fltr[flt_indx].update_count = Fm_fault_cal[flt_indx].threshold;

            /* Threshold has been reached, let OBD know */
            //obd_fault_monitored_event(flt_indx);
        }
        else
        {
            fault_fltr[flt_indx].update_count = (uint8_t)temp_fltr;
        }
    }
    else
    {   /* reinforce saturation */
        fault_fltr[flt_indx].update_count = Fm_fault_cal[flt_indx].threshold;
    }
}

/***************************************************************************//**
 * Function to clear a single fault
 *
 * @param fault_idx  Specifies the fault type
 * @param notify_obd If TRUE, a 'fault not present' event will be raised for the 
 *                   OBD feature. It should be set to FALSE when FM is being 
 *                   cleared due to a new drive cycle and TRUE otherwise.
 ******************************************************************************/
PRIVATE FCODE void fm_clear_fault(const Fm_fault_list_e fault_idx, bool_t notify_obd)
{
    Fm_fault_subtype_list_e subtype;

    subtype = GET_FAULT_SUBTYPE(fault_idx);

    /* Reset the requested filter o/p and fault subtype, and let OBD know */
    CLEAR_FAULT(fault_idx);
    SET_FAULT_SUBTYPE(fault_idx, Fm_general_failure_information);
    if (notify_obd)
    {
       // obd_fault_not_present_event(fault_idx, subtype);
    }

    /* Reset the fault count and the update since last reset flag. */
    fault_fltr[fault_idx].update_count = FM_RESET_FLT_COUNT;
    fault_fltr[fault_idx].flt_count    = FM_RESET_FLT_COUNT;
    fault_fltr[fault_idx].update_last_reset = FALSE;
}


 /******************************************************************************
 * PUBLIC FUNCTION DEFINITIONS
 ******************************************************************************/


/***************************************************************************//**
 *
 * The fm_init() function provides an interface to initialize the fault
 * management data structure. This function uses the fault filter calibratable
 * parameters & FM Data-set structure, fm_data_set_s, both retrived by the NVS
 * manager before the fm_init() function gets invoked.
 * On exiting low-power state the fm_init() get called which then calls the
 * reset_all_fault_filter_type() function to reset all the
 * No_latch & Ignition_Latch type filters, before the FM module can
 * entertain any filtering requests.
 * For the filters with latch type as "Service Latch", the filter output and the
 * fault subtype is restored. If the filter output was TRUE (indicating presence
 * of failure) then all the associated failure functions are activated by
 * incrementing the count of the respective failure function by calling the
 * function activate_fault_functions().
 *
 ******************************************************************************/
FCODE void fm_init(void)
{
    /* If the NVS restoration was unsuccessful, re-initialise the OBD
     * feature as if all faults had been cleared by a diagnostic tool.
     * It's necessary to do this because, even if OBD NVS restoration
     * was successful, its content will be out of step with that of FM.
     */
    if (fm_ram_image.header.restore_status == No_data_available)
    {
	//obd_reinit();
    }
    else
    {
        /* Unconditionally clear out non-latching faults */
        reset_all_fault_filter_type(Fm_no_latch);
        /* Unconditionally clear out ignition faults     */
        reset_all_fault_filter_type(Fm_ignition_cycle_latch);
        reset_all_fault_filter_type(Fm_power_on_latch);

        /* Activate failure functions for remaining faults */
        activate_fault_functions();
    }
}


/***************************************************************************//**
 *
 * The fm_update() function provides an interface to detect the toggling of
 * ignition input signal. It is called periodically by the scheduler.
 * On detectecting the toggling of the ignition input signal from low to high,
 * it calls the function reset_all_fault_filter_type() to reset all the
 * filter with the latch type as "Ignition latch" and "No Latch". With reset of
 * fault filter the associated failure functions are also deactivated.
 *
 ******************************************************************************/
FCODE void fm_update(void)
{
    /* update the ignition input. */
    ignition_input = (Fm_ignition_status_e)di_ign_dbn;

    /* Check if the ignition signal has toggled from non-high to high. */
    if((Fm_ignition_on != prev_ignition_input) && (Fm_ignition_on == ignition_input))
    {
        /* clear out "Ignition Latch" faults */
        reset_all_fault_filter_type(Fm_ignition_cycle_latch);

	    /* Tell OBD that there has been an ignition on event */
        //obd_ignition_on_event();
    }
    else if((Fm_ignition_on == prev_ignition_input) && (Fm_ignition_off == ignition_input))
    {
        /* clear out "Ignition off" faults */
        reset_all_fault_filter_type(Fm_ignition_off_latch);
    }
    else
    {
        /* neither ignition on or off so do nothing */
    }

    /* Update the previous ignition input value */
    prev_ignition_input = ignition_input;
}


/***************************************************************************//**
 * The function provides the main interface into fm for other modules. It is
 * used to maintain fault filter monitors, which control the validation of
 * fault conditions.<br>
 * The data that control the update of the fault filter are; whether the fault
 * is currently present, whether the monitoring conditions for detection are
 * valid, the fault filters internal state and its calibration.<br>
 * A fault is considered as "not monitored" if and only if any of the
 * following is TRUE:
 *    - The mon_condition parameter is FALSE
 *    - DTC setting is disabled for this fault
 *    - The engine isn't cranking
 *    - The fault is NOT Fm_plat_12v_logic and the supply voltage is low
 * The last two conditions are related, in that cranking may lead to
 * undervoltage conditions, but are required to be independent.
 * This final condition ensures that supply voltage faults are monitored
 * regardless of undervoltage conditions other than those caused by cranking.
 *
 * The update is performed as follows:<br>
 *    - Fault filter increment and decrement rates can be different based in
 *    wether the fault is detected/validated. These are set via calibration,
 *    - If the fault is monitored and present then the filter is incremented,
 *    - If the fault is absent and not detected then the filter is decremented,
 *    - If the fault is absent but previously detected then the filter may,
 *    be decremented (if the fault is not calibrated to latch),
 *    - if the fault is not monitored it may decrement the filter if calibrated.
 *
 * Detection.<br>
 * The fault is detected when the filter count reaches the calibrated threshold
 * value and is saturated at that value. If the fault is non-latching the
 * fault is undetected/cleared when the filter count becomes zero.
 *
 * @param flt the fault for which the filter is to be updated.
 * @param flt_subtype the appropriate fault sub-type to be stored.
 * @param flt_present fault presence.
 *                   + TRUE: fault present.
 *                   + FALSE: fault absent.
 * @param mon_condition Indicates whether or not the conditions for monitoring
 *                      the fault were fulfilled.
 *                      + TRUE: conditions for monitoring were fulfilled.
 *                      + FALSE: conditions for monitoring were not fulfilled.
 *
 * @retval TRUE indicates the presence of fault.
 * @retval FALSE indicates the absence of fault.
 *
 ******************************************************************************/
FCODE bool_t fm_update_fault_filter(const Fm_fault_list_e flt,
                                    const Fm_fault_subtype_list_e flt_subtype,
                                    const bool_t flt_present,
                                    const bool_t mon_condition)
{
    bool_t ret_val = FALSE;
    bool_t fault_detected = IS_FAULT_PRESENT(flt);
    bool_t crank;
    bool_t not_monitored;
    bool_t low_v_present,low_v_relevant;

    /*
     * Determine whether the engine is cranking based on the vehicle
     * ignition status. If vehicle comms have been lost, this will get
     * set to a non-crank value - the undervoltage condition should still
     * provide some degree of protection.
     */
 //   crank = (vio_ign_status == Vio_ign_status_crank);

    /* Determine whether 12V battery supply is low */     
    //low_v_present =  (Vmi_supply_low == vmi_12v_logic_obd_status);
    
    /* Determine whether a low voltage condition is relevant for this fault */
    low_v_relevant = TEST_MASK(Fm_fault_cal[flt].flags,FM_FLAG_LOW_V_NOMON);   
    
    if (crank && TEST_MASK(Fm_fault_cal[flt].flags,FM_FLAG_CRANK_NOMON))
    {
        /* If Vehicle is cranking and this is a crank suppressed fault,
         * it is not monitored */
        not_monitored = TRUE;
    }
    else if ((flt != Fm_plat_12v_logic) && (low_v_present && low_v_relevant))
    {
        /* If 12V supply is low and this fault is suppressed by low 12V
         * don't monitor it */
        not_monitored = TRUE;
    }
    else
    {
        /*
         * Don't monitor a fault if it's monitoring conditions aren't
         * satisfied */
        not_monitored = !mon_condition;
    }

    /* flt currently not monitored */
    if (not_monitored)
    {   /* flt is setup to count down when not monitored? */
        if (TRUE == Fm_fault_cal[flt].monitor_count_down)
        {   /* Decrement flt filter */
            fm_dec_flt_count(flt, fault_detected);
        }
    }
    else/* fault currently is monitored  */
    {   /* and its present */
        if (TRUE == flt_present)
        {   /* inc fault filter */
            fm_inc_flt_count(flt, fault_detected);
        }
        else
        {   /* dec fault filter */
            fm_dec_flt_count(flt, fault_detected);
        }

        /* Increment the update counter (for OBD purposes) */
        fm_inc_update_count(flt);
    }

    /* update fault output/detection status */
    ret_val = determine_filter_output(flt, flt_subtype, fault_fltr[flt].flt_count);

    return ret_val;
}


/***************************************************************************//**
 *
 * This function provides an interface to reset the fault filters in preparation
 * of a new drive cycle. 
 * Only non-latching faults and latching faults that are not currently set (and
 * thus latched) will be reset.
 *
 ******************************************************************************/
FCODE void fm_reset_fault_filters_for_new_drive_cycle(void)
{
    Fm_fault_list_e flt;

    for (flt = (Fm_fault_list_e) 0u; flt < Fm_max_fault_types; flt++)
    {
        /* we do not want to reset the fault if it is set and latching */
        if ((Fm_no_latch == Fm_fault_cal[flt].latch) 
        ||  (!IS_FAULT_PRESENT(flt)))
        {
            fm_reset_fault_filter(flt);
        }
        else
        {
            /* Do Nothing */
        }
    }
}

/***************************************************************************//**
 *
 * This function provides an interface to reset the fault filter value to Zero
 * and the output state to indicate that the fault is absent, on demand.
 * It also deactivates any associated failure functions.
 *
 * @param fault_idx Indicates the fault filter to be reset.
 *
 ******************************************************************************/
FCODE void fm_reset_fault_filter(const Fm_fault_list_e fault_idx)
{
    if (fault_idx < Fm_max_fault_types)
    {
        /*
         * If the fault is present, deactivate the associated failure
         * functions for the specified fault filter.
         */
        if (IS_FAULT_PRESENT(fault_idx))
        {
            deactivate_fault_fail_func(fault_idx);
            deactivate_fault_group(fault_idx);
        }

        /*
         * Clear fault count, filter o/p & reset the update since last
         * reset flag. Do not notify the OBD feature since the fault has not
         * healed naturally so we do not want to trigger the fault not present 
         * event.
         */
        fm_clear_fault(fault_idx, FALSE);
    }
}

/***************************************************************************//**
 * This function provides an interface to determine whether or not a fault filter
 * update has been performed since the last filter reset.
 *
 * @param fault_idx fault we are interested in.
 *
 * @retval TRUE indicates update has occurred since the last reset.
 * @retval FALSE indicates NO update has occurred since the last reset.
 *
 ******************************************************************************/
FCODE bool_t fm_fault_filter_update_status(const Fm_fault_list_e fault_idx)
{
    if (fault_idx < Fm_max_fault_types)
    {
        /* Update the flag since last requested reset */
        return (fault_fltr[fault_idx].update_last_reset);
    }
    return (FALSE);
}

/***************************************************************************//**
 * Function to query if a failure function is active.<br>
 * This functions checks the value of the requested failure function
 * counter, #fail_func_active_count[].
 *
 * @param #fail_idx is the failure function we are interested in.
 *
 * @retval TRUE indicates that the failure function is active.
 * @retval FALSE indicates that the failure funcrtion is not active.
 *
 * @note: If the fail function index passed is #Fm_no_fail_fun the function will
 *        still return a value. This essentially indicate if there are ANY
 *        faults active that are not member of other failure functions.
 *
 ******************************************************************************/
FCODE bool_t fm_is_fail_function_active(const Fm_fail_func_list_e fail_idx)
{
    if (fail_idx < Fm_max_failure_function)
    {
        return ((bool_t)(0u != fail_func_active_count[fail_idx]));
    }
    return (FALSE);
}

/***************************************************************************//**
 *
 * Function to query whether or not a particular fault is present.
 *
 * @param fault_idx Indicates the type of fault for which the query is made.
 *
 * @retval TRUE Indicates that the queried fault type is present.
 * @retval FALSE Indicates that the queried fault type is not present.
 *
 ******************************************************************************/
FCODE bool_t fm_is_fault_present(const Fm_fault_list_e fault_idx)
{
    if (fault_idx < Fm_max_fault_types)
    {
        return ((bool_t)IS_FAULT_PRESENT(fault_idx));
    }
    return (FALSE);
}

/***************************************************************************//**
 *
 * Function to query whether or not a particular fault is considered as "monitored".
 *
 * @param fault_idx Indicates the type of fault for which the query is made.
 *
 * @retval TRUE Indicates that the queried fault type is monitored.
 * @retval FALSE Indicates that the queried fault type is not monitored.
 *
 ******************************************************************************/
FCODE bool_t fm_is_fault_monitored(const Fm_fault_list_e fault_idx)
{
    bool_t answer = FALSE;

    if (fault_idx < Fm_max_fault_types)
    {
        if(fault_fltr[fault_idx].update_count >= Fm_fault_cal[fault_idx].threshold)
        {
            answer = TRUE;
        }
    }

    return answer;
}

/***************************************************************************//**
 * Function to query if a fault group is active.<br>
 *
 * @param group_idx defines the group type for which the query is made.
 *
 * @retval TRUE indicates that the queried group is active.
 * @retval FALSE indicates that the queried group is not active.
 *
 * @note: If the group index passed is #Fm_fault_cluster_none the function will
 *          still return a value. This is essentially indicate if there are ANY
 *          faults active that are not member of other fault groups.
 ******************************************************************************/
FCODE bool_t fm_is_fault_group_active(const Fm_fault_group_list_e group_idx)
{
    if (group_idx < Fm_max_fault_cluster)
    {
        return ((bool_t)(0u != fault_group_active_count[group_idx]));
    }
    return (FALSE);
}

/***************************************************************************//**
 *
 * Function to clear all non-latching faults.
 *
 ******************************************************************************/
FCODE void fm_clear_faults_non_latching(void)
{
    reset_all_fault_filter_type(Fm_no_latch);
}

FCODE void fm_clear_faults_ign_latching(void)
{
    reset_all_fault_filter_type(Fm_ignition_cycle_latch);
#if 1
    fm_debug_ctr = 0u;
    fm_fault_first = (Fm_fault_list_e)0;
    fm_subtype_first =  (Fm_fault_subtype_list_e)0;
    fm_fault_second = (Fm_fault_list_e)0;
    fm_subtype_second = (Fm_fault_subtype_list_e)0;
    fm_fault_latest = (Fm_fault_list_e)0;
    fm_subtype_latest = (Fm_fault_subtype_list_e)0;
#endif
}

FCODE void fm_clear_faults_service_latch(void)
{
    reset_all_fault_filter_type(Fm_service_latch );
}

/***************************************************************************//**
 *
 * Function to clear all faults.
 *
 ******************************************************************************/
FCODE void fm_clear_faults_all(void)
{
    Fm_fault_list_e i;

    /* Reset all filters when called by DispatchMmcUtils().*/
    for( i = (Fm_fault_list_e) 0u; i < Fm_max_fault_types; i++)
    {
        kick_watchdog();
        fm_reset_fault_filter(i);
    }
}

/***************************************************************************//**
 * Function to query the current subtype associated with a particular fault type.
 *
 * @param fault_idx Indicates the type of fault for which the query is made.
 *
 * @retval Indicates the current subtype associated with the queried fault type.
 *
 ******************************************************************************/
FCODE Fm_fault_subtype_list_e fm_get_fault_subtype(const Fm_fault_list_e fault_idx)
{
    if (fault_idx < Fm_max_fault_types)
    {
        return (GET_FAULT_SUBTYPE(fault_idx));
    }
    return (Fm_general_failure_information);
}

FCODE uint8_t fm_get_fault_counter(const Fm_fault_list_e fault_idx)
{
    if (fault_idx < Fm_max_fault_types)
    {
        return (fault_fltr[fault_idx].flt_count);
    }
    return (0u);
}

bool_t fm_is_dtc_emission_relevant(Fm_fault_list_e fault_idx)
{
    return Fm_fault_cal[fault_idx].obd;
}
