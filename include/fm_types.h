/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   fm_types.h
 *
 * description:  
 * Data types header for the Failure Management feature
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
/**
 * @file
 */

#if !defined(FM_TYPES_H)
#define FM_TYPES_H
#include "utils.h"
#include "nvs_cfg.h"

/** Macro defining the reset value of the fault filter count. */
#define FM_RESET_FLT_COUNT 0x00u
#define FM_MAX_FLTER       (uint16_t)0xFFu
/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/** @polyspace:begin<MISRA-C:19.4:Not a defect:Justify with annotations> .def files need this approach */

/*
 * Select out of the .def file the element we need here to define the fault enum
 */
#define FAULT_SUBTYPE(flt_subtype_name)             /* not needed here */
#define FAIL_FUNC(func_name)                        /* not needed here */
#define FAULT_GROUP(group_name)                     /* not needed here */
#define DEF_DTC(fault, subtype, dtc, snap, conf_th) /* not needed here */
#define FAULT_TABLE(Fault_type, failure_absent_inc_rate, failure_absent_dec_rate, \
                    flt_threshold, failure_present_inc_rate, failure_present_dec_rate, \
                    fail_func_link, fault_group_link, obd_relevant, latch_type, \
                    mon_cond_count_down, mon_flags, description_string, diagnosis_string ) \
    Fm_ ## Fault_type,


/**
 * Enumeration of all faults across the system.
 * @note The enum element "Fm_max_fault_types" is always be the last element
 * of the enum. Check the reference section of the design for details on mapping
 * the fault types with fault subtypes. The enum is populated by the
 * FAULT_TABLE entries in the mmc_fm.def file.
 */
typedef enum
{
    #include "fm.def"
    Fm_max_fault_types  /**< Is the last element and is used define the max number of enums. */
}Fm_fault_list_e;

/* make macro innocuous */
#undef  FAULT_TABLE
#define FAULT_TABLE(Fault_type, failure_absent_inc_rate, failure_absent_dec_rate, \
                    flt_threshold, failure_present_inc_rate, failure_present_dec_rate, \
                    fail_func_link, fault_group_link, obd_relevant, latch_type, \
                    mon_cond_count_down, mon_flags, description_string, diagnosis_string )    /* Not needed any more */

/*
 * Select out of the .def file the element we need here to define the fault
 * subtype enum
 */
#undef  FAULT_SUBTYPE
#define FAULT_SUBTYPE(flt_subtype_name) \
    Fm_ ## flt_subtype_name,

/**
 * Enumeration to specify the subtype of a fault.
 * Where there are no subtypes for a fault, the enum element
 * Fm_general_failure_information should be used.
 * @note No check is make on subtype. The module user must ensure that
 * the correct sub type is passed.
 * Check the reference section of the design for details on mapping
 * the fault subtypes with fault types. The enum is populated by the
 * FAULT_SUBTYPE() entries in the mmc_fm.def file.
 */
typedef enum
{
    #include "fm.def"
    Fm_any_subtype,                 /**< Used by OBD to link all fault sub-types
                                         to one DTC. Must not be used by FM as an
                                         actual fault sub-type. */
    Fm_general_failure_information  /**< Max fault sub-type enum, Use this enum
                                   to identify that there is no fault sub-type*/
}Fm_fault_subtype_list_e;

/* make macro innocuous */
#undef  FAULT_SUBTYPE
#define FAULT_SUBTYPE(flt_subtype_name) /* not needed any more */

/*
 * Select out of the .def file the element we need here to define the
 * failure function enum
 */
#undef  FAIL_FUNC
#define FAIL_FUNC(func_name) \
    Fm_ ## func_name,

/**
 * Enumeration to denote the list of failure functions that a fault may trigger.
 * Each element represents a failure function. The enum is populated by
 * the FAIL_FUNC() entries in the mmc_fm.def file and is required to query if a
 * particular failure function is active.
 */
typedef enum
{
    Fm_no_fail_fun = 0,     /**< Is the first element */
    #include "fm.def"
    Fm_max_failure_function /**< Is the last last element, and defines the total number of failure functions */
}Fm_fail_func_list_e;

/* make macro innocuous */
#undef  FAIL_FUNC
#define FAIL_FUNC(func_name) /* not needed beyond here */


/*
 * Select out of the .def file the element we need here to define the
 * fault group enum
 */
#undef  FAULT_GROUP
#define FAULT_GROUP(group_name) \
    Fm_ ## group_name,

/**
 * Enumeration to define the list of fault groups that a fault may trigger
 * Each element represents a fault group.
 * The enum is populated by the FAULT_GROUP() entries in the mmc_fm.def file
 * and is required to query if a particular fault group is active.
 */
typedef enum
{
    Fm_fault_cluster_none = 0u, /* Should always be the first element */
    #include "fm.def"
    Fm_max_fault_cluster     /* Should always be the last element */
} Fm_fault_group_list_e;


/* make macro innocuous */
#undef  FAULT_GROUP
#define FAULT_GROUP(fault_group) /* not needed beyond here */


/**
 * Enumeration to define the latch type for the fault filter output.
 * @note changes to this enum must be mirrored in the ASAP2 COMPU_METHOD
 * in fm.a2l
 */
typedef enum
{
    Fm_no_latch,             /**< fault counter that will clear during an ignition cycle */
    Fm_ignition_cycle_latch, /**< fault counter that will not count-down during the ignition cycle,
                                    but is reset at during an ignition cycle.*/
    Fm_ignition_off_latch,   /**< fault counter will not count-down but is cleared during ignition off */
    Fm_service_latch,         /**< fault counter will not count-down or reset
                                  (unless requested by a service tool).*/
    /** Fault is cleared at power-on/reset, intended for faults that are 
     * checked only once at startup. */
    Fm_power_on_latch
}Fm_latch_type_e;

typedef enum
{
    Fm_ignition_off = FALSE,
    Fm_ignition_on  = TRUE,
    Fm_dont_know
} Fm_ignition_status_e;
/**
 * Structure containing the fault filter attributes:<br>
 * 1) A flag indicating whether or not the fault filter has been updated since
 *   last reset, and <br>
 * 2) A fault counter.
 */
struct fm_flt_fltr_s
{
    bool_t update_last_reset;   /**< indicates if an update has happened
                                        since last reset occured.<br>
                                - TRUE: Update has occured since last reset,<br>
                                - FALSE: No update happened since last reset. */
    uint8_t flt_count;          /**< The current fault filter count. */
    uint8_t update_count;       /**< The number of updates since the fault was last reset. */
};


/** @polyspace:end<MISRA-C:19.4:Not a defect:Justify with annotations> .def files need this approach */

/**
 * Macro specifying the size of the Fault Manager data set.
 */
#define FM_DATA_SET_SIZE (sizeof(fm_data_set_s) * Fm_max_fault_types)

#define FM_NVS_USED (sizeof(nvs_header_s) + FM_DATA_SET_SIZE)
/**
 * FM NVS layout version to ensure incompatible data isn't loaded after
 * reflash
 */
#define FM_NVS_LAYOUT_VERSION           3u

/**
 * Structure containing the fault manager feature data set. It contains:
 *   - an indication of whether the fault is currently present
 *   - the fault_subtype associated with the fault
 * These bits are packed into a single byte in order to reduce FM's NVS
 * usage.
 * The private macros that are used to manipulate the fault status, are
 * defined in fm_c.
 *
 * Update FM_NVS_LAYOUT_VERSION if fm_data_set_s is changed.
 */
typedef struct
{
    uint8_t fault_status;
} fm_data_set_s;

/** Structure to represent Fm RAM image. It contains the fault manager feature
 * data set, Feature version number and some reserved bytes. This structure's
 * object is registered in the NVS feature table #nvs_feat_list.*/

typedef struct
{
    /**@lint --e{92}
     * Negative array dimension or bit field length
     * There are no lint options that will make it lay structures out in the
     * same way as Cosmic. In particular, lint assumes that enums are
     * implemented as int and that structure members are aligned according
     * to their type. This means that lint calculates a larger value for
     * FM_NVS_USED than Cosmic does, leading it to believe that
     * reserved_bytes[] has a negative array size.
     * Since Cosmic also diagnoses negative array dimensions, it is acceptable
     * to suppress the lint warning for this wrapper.
     */
    /*lint --e{92} */
    nvs_header_s  header;                   /**< Feature Information Structure.*/
    fm_data_set_s data[(int16_t)Fm_max_fault_types]; /**< Fm data-set. */

   // uint8_t reserved_bytes[(int16_t)FM_MAX_NVS_BYTES - (int16_t) FM_NVS_USED];
                                            /**< Feature Reserved_bytes.*/
    uint8_t reserved_bytes[1u];
} fm_nvs_wrapper_s;

#define FM_FLAG_NONE        ((fm_flag_t)0x0u)
#define FM_FLAG_LOW_V_NOMON ((fm_flag_t)0x1u)
#define FM_FLAG_CRANK_NOMON ((fm_flag_t)0x2u)

/**
 * Type for monitoring suppression flags */
typedef uint8_t fm_flag_t;

/**
 * This defines the contents of the calibration structure for each fault.
 * The default contents are define via the file mmc_fm.def.
 */
typedef struct
{
    uint8_t undetected_inc_rate;    /**<Increment rate when fault is undetected */
    uint8_t undetected_dec_rate;    /**<Decrement rate when fault is undetected */
    uint8_t detected_inc_rate;      /**<Increment rate when fault is detected   */
    uint8_t detected_dec_rate;      /**<Decrement rate when fault is detected   */
    uint8_t threshold;              /**<Detection Threshold Value               */
    bool_t  monitor_count_down;     /**<flt will count down when unmonitored    */
    bool_t  obd;                    /**<OBD relevent fault                      */
    Fm_latch_type_e latch;          /**<flt latch type                          */

    uint32_t failure_functions_map; /**<Bit map of failure functions associated */
    uint16_t fault_group_map;       /**<Bit map of fault groups associated      */
    fm_flag_t  flags;               /**< Flags for suppression                  */
} fm_calibration_s;



/** the following is the minimum size of an array of U8s that will hold one bit per error code */
#define ACTIVE_FAULTS_BITFIELD_ELEMENT_BITS  (8u)
#define ACTIVE_FAULTS_BITFIELD_ARRAY_SIZE    ((((uint32_t)Fm_max_fault_types)+(ACTIVE_FAULTS_BITFIELD_ELEMENT_BITS-1u)) / ACTIVE_FAULTS_BITFIELD_ELEMENT_BITS)

#endif

