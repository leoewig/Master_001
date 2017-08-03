/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   obd_types.h
 *
 * description:  On-Board Diagnostics
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
/**
 *
 * On-Board Diagnostics
 *
 * @file
 * @ingroup obd
 */


#if !defined(OBD_TYPES_H)
#define      OBD_TYPES_H

#include "utils.h"
#include "nvs_cfg.h"
#include "mmc.h"
#include "ts_types.h"

/*
 * enumerations and macro definitioms made available must be documented here to
 * avoid duplication.
 *
 * PRIVATE functions are documented in the source file.
 * Non-PRIVATE variables and functions (i.e. those shared between files in the
 * feature) are documented here.
 */

/**
 * Maximum number of DTCs that can be supported
 * This should not be confused with N_SUPPORTED_DTCS which is the number
 * of DTCs actually supported, a quantity that will vary according to the
 * configuration.
 * Note: Supporting more DTCs may require increasing the size of the diagnpstic
 *       buffer in GENy
 */
#define OBD_MAX_SUPPORTED_DTCS  300u

/**
 * Maximum number of DTCs that can be active simultanously.
 *
 * This number is limited by the availability of NVS available to the OBD
 * feature. If it needs to be made larger, either make more NVS available or
 * reduce the size of the active DTC descriptor, particularly the snapshot.
 */
#define OBD_MAX_ACTIVE_DTCS     10u

/** 
 * Macros representing potential DTC lifecycle states.
 */
#define OBD_DTC_LC_CLEAR                (0u)
#define OBD_DTC_LC_PENDING              (1u)
#define OBD_DTC_LC_CONFIRMED            (2u)
#define OBD_DTC_LC_PERMANENT            (4u)
#define OBD_DTC_LC_PEND_CONF            (OBD_DTC_LC_PENDING | OBD_DTC_LC_CONFIRMED)
#define OBD_DTC_LC_CONF_PERM            (OBD_DTC_LC_CONFIRMED | OBD_DTC_LC_PERMANENT)
#define OBD_DTC_LC_PEND_PERM            (OBD_DTC_LC_PENDING | OBD_DTC_LC_PERMANENT)
#define OBD_DTC_LC_PEND_CONF_PERM       (OBD_DTC_LC_PENDING | OBD_DTC_LC_CONF_PERM)

/** 
 * Macros for powertrain status states as indicated by NPowertrainStatus CAN
 * signal.
 */
#define OBD_CYC_TORQUE_NOT_AVAILABLE        (0u)
#define OBD_CYC_TORQUE_AVAILABLE            (1u)

/** 
 * Normal trip status flags corresponding to satisfied conditions.
 */
#define OBD_NORM_TRIP_CLEAR                 (0x00u)
#define OBD_NORM_TRIP_ENG_RUN_TIME          (0x01u)
#define OBD_NORM_TRIP_VEH_SPEED             (0x02u)
#define OBD_NORM_TRIP_VEH_IDLE              (0x04u)
#define OBD_NORM_TRIP_COND_SATISFIED        (OBD_NORM_TRIP_ENG_RUN_TIME | \
                                             OBD_NORM_TRIP_VEH_SPEED | \
                                             OBD_NORM_TRIP_VEH_IDLE)

/** 
 * Macro used to covert engine speed to PID0C_nEngine.
 */
#define OBD_ENGINE_RPM(rpm)     ((uint16_t)((uint16_t)(rpm) * 4u))

/** 
 * Macro used to covert MPH to KMH.
 */
#define OBD_MPH_TO_KMH(p)       ((float32_t)(p) * 1.60934f)

/** 
 * Macro used to covert a percentage to freeze frame CAN signals values.
 */
#define OBD_FF_PERCENT(p)       ((uint8_t)((float32_t)(p) * 2.55f))


enum obd_checksum_state_e
{
    obd_checksum_idle,
    obd_checksum_in_progress,
    obd_checksum_complete
};

/**
 * Enumeration of OBD services that make use of the PID supported PIDs.
 * The enumeration is used for indexing into the arrays containing the 
 * definitions of supported PIDs.
 */
enum obd_pid_support_services_e
{
    obd_pid_supp_01 = 0u,
    obd_pid_supp_02,
    obd_pid_supp_08,
    obd_pid_supp_09,
    obd_pid_supp_max
};

/**
 * Enumeration of events affecting the DTC life cycle.
 */
typedef enum
{
    obd_lc_evt_none = 0,            /**< no DTC life cycle event occurred */
    obd_lc_evt_fault_detected,      /**< fault detected */
    obd_lc_evt_drive_cyc_ended,     /**< drive cycle ended */
    obd_lc_evnt_warm_up_cyc_ended,  /**< warm-up cycle ended */
    obd_lc_evnt_normal_cyc_ended    /**< normal cycle ended */
} obd_dtc_lc_events_e;

/**
 * Enumeration of OBD cycle states.
 */
typedef enum
{
    obd_cyc_s_init = 0,             /**< cycle detection initialisation */
    obd_cyc_s_detecting,            /**< cycle detection under way */
    obd_cyc_s_completed             /**< OBD cycle has been completed */
} obd_cycle_state_e;

/**
 * These private types correspond to types used in the automatically generated
 * diagnostics template file. This is done in preference to including the
 * header file for the diagnostics code but it could equally be done by
 * including that file and either defining these interface types in terms
 * of the CANbedded types, or by using the CANdela types directly.
 * Note: obd_msg_len_t is defined as unsigned short because the
 * corresponding CANbedded type is defined this way. It would be nice to use
 * uint16_t but basic_types.h defines this as unsigned short which is not
 * compatible with unsigned int, meaning that uint16_t * and obd_msg_len_t *
 * can't be assigned to each other.
 */
typedef unsigned short  obd_msg_len_t;
typedef uint8_t         obd_msg_byte_t;

/**
 * This type holds an environment data extended data record
 *
 * It is also convenient to store the cycle counter and permanent status, 
 * used for managing the DTC, as part of this structure even though it is 
 * not part of the required extended data and not reported as such.
 */
typedef struct obd_dtc_extdata_tag {
    uint8_t             occur_cnt;      /**< Occurence counter: the number
                                             of times the fault for this
                                             DTC has failed since the DTC
                                             was initially triggered */
    uint8_t             heal_cnt;       /**< Healing counter: the number
                                             of times the test for a
                                             Confirmed DTC has passed
                                             since it last failed */
    uint8_t             freq_cnt;       /**< Frequency counter: the number of
                                             times the fault for a confirmed
                                             DTC has occurred */
    uint8_t             ign_cnt;        /**< Ignition cycle counter: the
                                             number of ignition on events
                                             since a DTC became confirmed */
    uint8_t             uctest_cnt;     /**< Uncompleted test counter: the
                                             number of driving cycles since
                                             test for a DTC last completed */
    uint8_t             cycle_cnt;      /**< Cycle counter used for managing
                                             DTC's lifecycle */
    bool_t              is_permanent;   /**< Is the DTC permanent status set? */
} obd_dtc_extdata_s;

/**
 * An enumeration used to represent the reportable fields in the extended
 * data record, i.e. those that can be accessed using ISO 14229 service
 * $19 subfunction $06.
 * Confusingly, ISO 14229 refers to these as "records" rather than fields.
 * Each enumerator is given the same name as the corresponding field in
 * obd_dtc_extdata_s with two exceptions:
 *   - The cycle counter is for internal use so isn't reportable
 *   - The value 0xff corresponds to a request for all fields
 */
typedef enum
{
    Extdata_occur_cnt = 0x01,
    Extdata_heal_cnt = 0x02,
    Extdata_freq_cnt = 0x03,
    Extdata_ign_cnt = 0x04,
    Extdata_uctest_cnt = 0x05,
    Extdata_all = 0xff
} Extdata_field_e;

/**
 * The number of bytes reserved in a snapshot record for the common part of
 * a snapshot (as defined by obd_ss_common).
 */
#define OBD_COMMON_SNAPSHOT_SIZE        17u

/**
 * The number of bytes reserved in a snapshot record for the DTC-specific
 * part of a snapshot.
 * 
 * It is essential that none of the DTC-specific snapshot recording functions
 * write more than this number of bytes to a snapshot record. Such functions
 * can check both the number of bytes written and the overall index into the
 * snapshot so have two means to avoid overrunning the buffer.
 */
#define OBD_PERDTC_SNAPSHOT_SIZE        20u

/**
 * The maximum number of bytes required for any DTC to store its snapshot
 * data.
 */
#define OBD_MAX_SNAPSHOT_SIZE           \
        (OBD_COMMON_SNAPSHOT_SIZE + OBD_PERDTC_SNAPSHOT_SIZE)

/**
 * Enumeration of all snapshot elements.
 */
#undef DEF_SS_DID
#undef DEF_SNAP
#define DEF_SNAP(name, dids, description)   /* not used */
#define DEF_SS_DID(id, size, description)   Snapshot_did_ ##id,

typedef enum
{
    #include "snap.def"
    Snapshot_did_count
} Snapshot_dids_e;
#undef DEF_SS_DID
#undef DEF_SNAP

/**
 * An enumeration used to represent the reportable snapshots, i.e. those
 * that can be accessed using ISO 14229 service $19 subfunction $04.
 * As well as individual snapshots, the value 0xff can be used to request
 * all snapshots.
 */
typedef enum
{
    Snapshot_confirmed = 0x01,
    Snapshot_latest = 0x02,
    Snapshot_all = 0xff
} Snapshot_e;

/**
 * Macros that map from Snapshot_e onto the elements in the snapshot
 * array. 
 */
#define DTC_SNAP_CONFIRMED              0u
#define DTC_SNAP_LATEST                 1u

/**
 * Structure that defines the supported state of a single group of PIDs.
 *
 * Various OBD services use PIDs 0x00, 0x20, 0x40, 0x60, 0x80, 0xA0,
 * 0xC0 and 0xE0 to retrieve the supported status of PIDs relevant to that
 * service.
 */
typedef struct
{
    uint8_t  pid;                   /**< support PID number */
    uint32_t support_status;        /**< support status of PIDs */
} obd_pid_support_t;

/**
 * Structure used to define the PID support status for a given OBD service. 
 *
 * Various OBD services use PIDs 0x00, 0x20, 0x40, 0x60, 0x80, 0xA0,
 * 0xC0 and 0xE0 to retrieve the supported status of PIDs relevant to that
 * service.
 */
typedef struct
{
    const obd_pid_support_t* pid_support; /**< pointer to an array 'support'
                                               PIDs and associated support
                                               status definitions */
    uint8_t            count;             /**< number of supported 'support' 
                                               PIDs */
} obd_pid_support_defn_t;

/**
 * A type representing a pointer to a function that records a snapshot
 * data element. The snapshot data is written into the buffer pointed 
 * to by the first parameter, starting at the index given by the second
 * parameter. The index of the byte after the last byte written is returned.
 * The function also expects the index of the DTC which the snapshot relates
 * to allow access to fault specific data e.g. fault sub-type.
 */
typedef uint16_t (*snapshot_fn_t) (FPTR uint8_t *buf, uint16_t start_idx, uint16_t dtc_idx);

/**
 * Structure that describes an element in a snapshot.
 *
 * Arrays of this structure type can be used both when recording snapshot
 * data and when reporting it.
 */
typedef struct
{
    uint16_t        did;        /**< The DID corresponding to the element */
    uint8_t         size;       /**< The number of bytes occupied by this element */
    snapshot_fn_t   rec_fn;     /**< function that collects the snapshot DID data */
} obd_ss_did_descriptor_s;

typedef const FPTR obd_ss_did_descriptor_s* obd_ss_did_desc_p;
typedef const FPTR Snapshot_dids_e*         obd_ss_snap_dids_p;

/**
 * Structure that describes the snapshot.
 */
typedef struct
{
    uint8_t             n_dids; /**< Total number of snapshot DIDs */
    obd_ss_snap_dids_p  dids;   /**< Pointer to an array of DID indices of interest */
} obd_snapshot_descriptor_s;

typedef const FPTR obd_snapshot_descriptor_s* obd_ss_snap_desc_p;

/**
 * This type describes an active DTC. It is used to record data as the
 * DTC progresses through its lifecycle.
 *
 * The DTC is stored as the index into the vector of supported DTCs.
 * The DTC number, fault and subtype can all be looked-up quickly in the
 * corresponding vectors using this index.
 */
typedef struct obd_dtc_tag {
    uint16_t            dtc_idx;        /**< The index of the DTC in the
                                             various vectors that record
                                             information about it */
    bool_t              occupied;       /**< Is this record being used
                                             for a DTC at the moment? */
    obd_dtc_extdata_s   extdata;        /**< Extended data for this DTC */
    uint8_t             snapshot[2][OBD_MAX_SNAPSHOT_SIZE];
                                        /**< Snapshot data for this DTC:
                                             one entry records the snapshot
                                             at the time the DTC became
                                             confirmed, the other records
                                             the snapshot at the time of the 
                                             most recent test failure */
} obd_dtc_s;


/**
 * Structure that describes the freeze frame data.
 *
 * pid02_dtc equal to 0x0000 indicates no freeze frame data exists.
 * Consequently, when the freeze frame is to be cleared, pid02_dtc must 
 * be set to 0x0000.
 */
typedef struct {
    uint16_t    pid02_dtc;              /**< DTC that caused freeze frame data storage */
    uint8_t     pid04_clv;              /**< calculated load value */
    uint8_t     pid05_eng_t;            /**< engine coolant temperature */
    uint16_t    pid0C_eng_rpm;          /**< engine RPM */
    uint8_t     pid0D_veh_speed;        /**< vehicle speed sensor */
    uint16_t    pid42_module_v;         /**< control module voltage */
    uint8_t     pid49_acc_ped_pos_d;    /**< accelerator pedal position D */
    uint8_t     pid4A_acc_ped_pos_e;    /**< accelerator pedal position E */
} obd_ff_s;

/**
 * Structure containing the states of the warm-up cycle and normal trip.
 * These need to be retained across power cycles as the drive cycle they
 * relate can span across a power cycle.
 */
typedef struct {
    obd_cycle_state_e obd_warmup_cycle_state;
    obd_cycle_state_e obd_normal_trip_state;
    uint8_t           obd_normal_trip_status;
    ts_time_t         obd_min_spd_drive_time_ms;    /**< accumulated time driving 
                                                         above speed threshold */
} obd_cyc_s;

/*
 * NVS for the OBD feature
 */

#define OBD_NVS_USED (sizeof(nvs_header_s) + sizeof(obd_nvs_data_s) )

/**
 * OBD NVS layout version to ensure incompatible data isn't loaded after
 * reflash
 */
#define OBD_NVS_LAYOUT_VERSION          9u


typedef struct
{
    uint8_t     obd_status[OBD_MAX_SUPPORTED_DTCS]; /**< Status for all DTCs */
    obd_dtc_s   obd_dtc[OBD_MAX_ACTIVE_DTCS];       /**< OBD active DTC data */
    obd_ff_s    obd_ff;                             /**< freeze frame data */
    obd_cyc_s   obd_cycle_data;                     /**< cycle status information */
} obd_nvs_data_s;

/**
 * Structure to represent the RAM image of OBD non-volatile storage. It
 * contains the fault manager feature. An object of this type is 
 * is registered in the NVS feature table #nvs_feat_list
 */
typedef struct
{
    /**@lint --e{92}
     * Negative array dimension or bit field length
     * There are no lint options that will make it lay structures out in the
     * same way as Cosmic. In particular, lint assumes that enums are
     * implemented as int and that structure members are aligned according
     * to their type. This means that lint calculates a larger value for
     * OBD_NVS_USED than Cosmic does, leading it to believe that
     * reserved_bytes[] has a negative array size.
     * Since Cosmic also diagnoses negative array dimensions, it is acceptable
     * to suppress the lint warning for this wrapper.
     */
    /*lint --e{92} */
    nvs_header_s        header;                 /**< Header Information  */
    obd_nvs_data_s      data;
    uint8_t reserved_bytes[(int16_t)OBD_MAX_NVS_BYTES - (int16_t)OBD_NVS_USED];
} obd_nvs_wrapper_s;

/**
 * Obtain the OBD status byte for a given DTC
 */
#define OBD_GET_DTC_STATUS(dtc_idx) (obd_ram_image.data.obd_status[dtc_idx])

/**
 * Modify the OBD status byte for a given DTC
 */
#define OBD_SET_DTC_STATUS(dtc_idx, status) \
        (obd_ram_image.data.obd_status[dtc_idx] = (status))

/**
 * Shorthand for obd_dtc[] entry whose index is idx.
 *
 * The dtc_idx member of the obd_dtc[] entry gives the index by which the
 * DTC's number, fault and subtype can be looked up in the corresponding
 * vectors.
 */
#define OBD_ACTIVE_DTC(idx) (obd_ram_image.data.obd_dtc[idx])

/**
 * Shorthand for the fault associated with the active DTC whose index into
 * obd_dtc[] is idx.
 *
 * The dtc_idx member of the obd_dtc[] entry gives the index by which the
 * DTC's number, fault and subtype can be looked up in the corresponding
 * vectors.
 */
#define OBD_ACTIVE_DTC_FAULT(idx) \
        (obd_dtc_fault[OBD_ACTIVE_DTC(idx).dtc_idx])

/**
 * Obtain the status of the active DTC whose index into obd_dtc[] is idx.
 *
 * The dtc_idx member of the obd_dtc[] entry gives the index by which the
 * DTC's status can be looked up.
 */
#define OBD_GET_ACTIVE_DTC_STATUS(idx) \
        OBD_GET_DTC_STATUS(OBD_ACTIVE_DTC(idx).dtc_idx)
 
/** 
 * Shorthand for retrieving 'Pending' status of ACTIVE DTCs
 */
#define OBD_DTC_IS_PENDING(idx) \
        ((bool_t)((OBD_GET_ACTIVE_DTC_STATUS(idx) & OBD_PENDING_DTC) != 0u))

/** 
 * Shorthand for retrieving 'Confirmed' status of ACTIVE DTCs
 */
#define OBD_DTC_IS_CONFIRMED(idx) \
        ((bool_t)((OBD_GET_ACTIVE_DTC_STATUS(idx) & OBD_CONFIRMED_DTC) != 0u))

/** 
 * Shorthand for retrieving 'Permanent' status of ACTIVE DTCs
 */
#define OBD_DTC_IS_PERMANENT(idx) \
        (OBD_ACTIVE_DTC(idx).extdata.is_permanent)

/**
 * Obtain the DTC number that caused the freeze frame data to be recorded
 */
#define OBD_FF_DTC          (obd_ram_image.data.obd_ff.pid02_dtc)

/**
 * Shorthand for determining whether a freeze frame has been stored
 */
#define OBD_IS_FF_SET       ((bool_t)(OBD_FF_DTC != 0x0000u))

/**
 * Shorthand for determining whether a DTC is related to the security
 * access faults.
 */
#define OBD_IS_DTC_SECA(idx) \
    ((bool_t)(((obd_dtc_fault[idx] == Fm_security_lvl2_prot_max_attempts) || \
               (obd_dtc_fault[idx] == Fm_security_lvl2_prot_unit_locked)) \
           && (obd_dtc_subtype[idx] == Fm_general_failure_information)))

/**
 * Shorthand for accessing warm-up cycle state
 */
#define OBD_WARMUP_CYCLE_STATE      (obd_ram_image.data.obd_cycle_data.obd_warmup_cycle_state)

/**
 * Shorthand for accessing normal trip state
 */
#define OBD_NORMAL_TRIP_STATE       (obd_ram_image.data.obd_cycle_data.obd_normal_trip_state)
 
/**
 * Shorthand for accessing normal trip status
 */
#define OBD_NORMAL_TRIP_STATUS      (obd_ram_image.data.obd_cycle_data.obd_normal_trip_status)
 
/**
 * Shorthand for accessing the accumulated time driving above the normal trip speed
 * threshold
 */
#define OBD_MIN_SPEED_DRIVE_TM_MS   (obd_ram_image.data.obd_cycle_data.obd_min_spd_drive_time_ms)
 
/**
 * Shorthand for determining whether a warm-up cycle has completed
 */
#define OBD_IS_WARMUP_CYCLE_COMPLETED   ((bool_t)(OBD_WARMUP_CYCLE_STATE == obd_cyc_s_completed))

/**
 * Shorthand for determining whether a normal trip has completed
 */
#define OBD_IS_NORMAL_TRIP_COMPLETED    ((bool_t)(OBD_NORMAL_TRIP_STATE == obd_cyc_s_completed))

#endif      /* OBD_TYPES_H */

