/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    obd.h 
 *
 * description: On-Board Diagnostics  
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
#if !defined(OBD_H)
#define      OBD_H
/**
 *
 * On-Board Diagnostics
 *
 * @file
 * @ingroup obd
 * $Id: obd.h 12614 2015-02-26 15:18:19Z dc17 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 *
 * <h2> OBD Overview </h2>
 *
 * This module implements operations on DTCs including:
 *   - setting and clearing them
 *   - controlling whether or not they are active
 *   - reporting all supported DTCs
 *   - reporting the number of DTCs according to status
 *   - reporting DTCs according to status
 *   - reporting any environment data recorded a DTC
 *
 * It operates by provding an interface between the auto-generated
 * CANbedded code (appdesc_implemented.c) and the fault management
 * feature.
 *
 * <h2> Requirements </h2>
 *
 * There requirements for this feature have been taken from
 * <a href="http://viewvc/viewvc.cgi/trunk/docs/reqs/raw/network_and_software_pack/P12_074.00 McLaren Diagnostic Specification.docx?root=Q52717_NightWing&view=co">Project 12 Diagnostics Specification</a>. The 11 Nov 2011 revision, on
 * which this design has been based, is incomplete. In order to allow work
 * to progress while waiting for clarification from the customer, some
 * requirements have been taken from
 * <a href="http://viewvc/viewvc.cgi/trunk/docs/reqs/raw/network_and_software_pack/P11_074.11 Generic Diagnostic Specification.docx?root=Q52717_NightWing&view=co">McLaren Unified Diagnostic Services</a>.
 *
 * A requirement identifier of the form REQ_DIAG_<i>req</i> corresponds to
 * the requirement <i>req</i> in the P12 diagnostics specification.
 * A requirement in the P11 diagnostics specification has the form
 * P11_DIAG_<i>req</i> so maps onto REQ_P11_DIAG_<i>req</i> in this design
 * document.
 *
 * DTC life cycle and OBD drive cycles are specified in
 * <a href="http://viewvc/viewvc.cgi/trunk/docs/reqs/generic/RU_BMS_Support.doc">BMS Generic Support Function Specification</a>.
 *
 * <h2> DTCs and faults </h2>
 *
 * <h3> DTC Definition </h3>
 *
 * A DTC is a (fault, subtype) pair that is capable of being reported
 * via UDS diagnostics. ISO 15031-6 DTCs use a 3-byte code of which the
 * most significant 2 bits identify the group (powertrain, chassis, body,
 * network).
 *
 * Once activated, DTCs follow a lifecycle which starts when the DTCs fault
 * occurs with the specified subtype, and continues until the DTC is
 * cleared, by means of a diagnostic tool or self-healing. Associated with
 * the lifecyle is the concept of "permanence". Some operations act on
 * all DTCs, others (such as clearing) may only act on non-permanent DTCs.
 *
 * It is important to remember that a fault may clear itself, e.g. when
 * the fault condition goes away, but that its corresponding DTC may
 * remain active.
 *
 * As an active DTC progress through its lifecycle, environment data
 * relating to the DTC is recorded. The environment data consists of:
 *   - extended data
 *   - snapshot data recorded when the DTC first became confirmed
 *   - snapshot data recorded when the corresponding fault most
 *      recently occurred
 *   - (for emission related faults) freeze frame data when the first
 *      emission related fault occurs
 *
 * Extended data has a fixed format.
 *
 * Snapshot data may vary according to DTC but there is an initial sequence
 * that is common to all DTCs.
 *
 * Freeze frame data has a fixed format. There is only one freeze frame for
 * all emission related DTCs.
 *
 * <h3> Relationship to faults </h3>
 *
 * Every DTC must map onto a (fault, subtype). It is possibile that every
 * (fault, subtype) will map onto a DTC but this design allows for
 * (fault, subtype) pairs that do not correspond to a DTC.
 *
 * Because several DTCs may map onto the same fault, albeit with different
 * subtypes, it follows that it is not possible for more than one underlying
 * fault condition to be active at the same time. However, the associated 
 * DTC(s) will remain active until healed or cleared via diagnostic requests.
 *
 * The subtype is in several cases used to provide additional information for
 * the same underlying fault (e.g. at which point in the contactor closing 
 * sequence did the closing operation fail). In these situations it is not
 * desirable to associate each (fault, subtype) pair with a unique DTC. Instead,
 * a multiple (fault, subtype) to single DTC mapping is required. To achieve 
 * this, the special subtype Fm_any_subtype has been introduced. When assigned 
 * to a DTC together with a fault, this subtype will result in all the fault
 * condition events contributing to the life cycle of the single DTC. This 
 * mechanism comes with the following assumptions:
 * - The Fm_any_subtype subtype is not used by the FM feature as an actual
 *   subtype.
 * - No other DTCs are mapped to a given fault when (fault, Fm_any_subtype)
 *   is already mapped to a DTC. The implementation will always give precedence 
 *   to a DTC mapped to an actual subtype. Hence that subtype would not 
 *   contribute to the life cycle of the DTC mapped to Fm_any_subtype.
 *
 * The subtype Fm_general_failure_information plays a special role. It is
 * used by FM to mean both (a) a fault that has no specific subtypes, and
 * (b) for a fault that has specific subtypes, the absence of a fault.
 * Therefore, a fault that has DTCs defined with both a specific subtype
 * AND the general subtype is invalid - there's no way to indicate that the
 * DTC's fault is absent.
 * [Edit DC17]: I am sceptical about the above statement regarding 
 * "Fm_general_failure_information". Having searched through the code I was
 * unable to find the special condition where Fm_general_failure_information
 * indicates the absence of a fault.
 *
 * <h3> Description of DTCs </h3>
 *
 * The descriptions of DTCs are stored in fm.def. The macro DEF_DTC has
 * the following form:
 *
 * DEF_DTC(fault, subtype, dtc, snap, conf_th)
 *
 * where the parameters are interpreted as follows:
 *   - fault: the fault that this DTC maps to
 *   - subtype: the subtype of the fault that this DTC maps to
 *   - dtc: the 24-bit code for the DTC
 *   - snap:      a pointer to an array of snapshot element descriptors
 *                that describe how to record and interpret the DTC-specific 
 *                snapshot data buffer. Each element contains the DID, size  
 *                and a pointer to a function that will record DTC-specific
 *                snapshot data. If there are no DTC-specific snapshot entries,
 *                then the pointer is NULL.
 *   - conf_th:   the occurrence count threshold at which a fault is 
 *                'confirmed'.
 *
 * It seems very unlikely that two DTCs that map into the same fault
 * would belong to different DTC groups. The design makes this important
 * assumption, particularly when dealing with operations on DTC groups,
 * but it is not easy (possible?) to check using the preprocessor. It is
 * therefore very important that the DEF_DTCS macros satisfy this expectation.
 *
 * <h3> Mapping DTCs to faults </h3>
 *
 * This project is likely to define a few hundred DTCs, most of which are in
 * the range 0x0A7000 to 0x17B000 but with an outlier at 0xC29A00. Even if
 * the outlier is treated as a special case, a lookup table to map DTCs to
 * (fault, subtype) pairs will be impractically large and very sparsely
 * populated.
 *
 * The mapping of any given DTC can be handled with reasonable efficiency,
 * and without writing any custom lookup routines, by means of a switch
 * statement.
 *
 * Since it is likely that iterating over all DTCs and acting on their
 * corresponding faults is likely to be a common operation, a vector
 * of DTCs and corresponding faults is also needed. This vector could be
 * used to perform the general lookup but (a) it isn't guaranteed to be
 * in ascending order of DTC (or fault), and (b) it would require
 * scanning half the vector on average in order to find a DTC. The switch
 * method is likely to be more efficient in this case.
 *
 * <h3> Mapping faults to DTCs </h3>
 *
 * Since a fault may correspond to more than one DTC, a simple lookup
 * table can't be generated using the C preprocessor. It would be possible
 * to generate a table dynamically, in which the entry for each fault was
 * a list of (subtype, DTC) pairs but this wastes RAM (likely to be at
 * least 6 bytes per DTC.
 *
 * Instead, as for the DTC to fault mapping, a switch statement can be used
 * with reasonable efficiency. A (fault, subtype) pair is encoded as
 * ((fault << n) | subtype), where n is 1 more than the number of bits
 * needed to encode the largest subtype, and this encoded value can be used
 * as the controlling expression of the switch.
 *
 * <h2> DTC Setting </h2>
 *
 * UDS diagnostic service $85 allows "DTC Setting" to be enabled or disabled.
 * When DTC setting is disabled for a given DTC, its status is effectively
 * frozen and its corresponding fault filter is disabled. When DTC setting
 * is enabled for a DTC, it operates normally.
 *
 * The ability to clear DTCs by external means, e.g. the diagnostic tool,
 * is unaffected by DTC setting. Clearing a DTC does not automatically
 * re-enable DTC setting for it.
 *
 * The control provided by the DTC setting service lasts only until one of the
 * following occurs:
 *   - It is re-enabled by the DTC setting service
 *   - an ECUReset event occurs
 *   - the ECU powers up
 *   - the diagnostic session times out or a session transition occurs
 *
 * These suggest that there is no need to record DTC setting status in NVRAM.
 *
 * For this project, DTC setting is controlled by DTC group. Although UDS
 * diagnostic services support control on a per-DTC basis, this is not
 * required for this project. However, because the DTC groups are not
 * disjoint, the easiest way to implement DTC setting is to record it for
 * each fault separately, relying on the assumption that all DTCs associated
 * with a given fault will be members of the same DTC groups.
 *
 * If a request is made to control the setting of the group of all DTCs,
 * then setting for ALL faults, regardless of whether they correspond to
 * DTCs or not, is controlled. This is done (a) for efficiency (it's likely
 * to be the most common use case), and (b) because it may be convenient
 * to have a means to control all faults.
 *
 * All other DTCs are controlled according to the group (emissions,
 * powertrain, chassis, body or network) and faults that are not related
 * to such DTCs are not controlled.
 *
 * @todo Note that similar considerations apply to clearing DTCs so perhaps
 * make the same comments there.
 *
 * <h2> Data structures </h2>
 *
 * <h3> Representation of DTCs </h3>
 * DTCs are often represented by an index which is used to obtain information
 * about the DTC from various vectors. The maximum number of DTCs that
 * can be supported by the implementation is defined as
 * OBD_MAX_SUPPORTED_DTCS.
 *
 * <h3> Obtaining information about DTCs </h3>
 * Given a DTC index, the following vectors provide information about the DTC:
 *   - obd_ram_image.obd_status[] holds the OBD status bits that are used to
 *     manage the DTC's lifecycle
 *   - #obd_dtc_num_hi holds the most significant 8 bits of the DTC number
 *   - #obd_dtc_num_lo holds the least significant 16 bits of the DTC number
 *   - #obd_dtc_fault holds the fault number corresponding to the DTC
 *   - #obd_dtc_subtype holds the subtype of the fault corresponding to the
 *     DTC
 *
 * Environment data for each DTC, along with the DTC number itself and a
 * cycle counter that is used for managing its lifecycle transitions,
 * is stored in obd_dtc_data[].
 * Entries in this vector are allocated to DTCs as they occur. When the
 * vector is full, no further DTCs can be triggered. However, it is possible
 * for entries to become available again as DTCs are cleared. Since the
 * entries can be freed in any order, it is necessary to maintain an
 * occupancy indicator for each DTC.
 *
 * <h3> DTC Snapshots </h3>
 *
 * Snapshots are recorded in obd_ram_image.obd_dtc[] for active DTCs.
 * The snapshot array is 2-dimensional: the outer dimension selects between
 * the two snapshots (Confirmed or Latest), while the inner dimension selects
 * the byte within the snapshot record.
 *
 * The initial part of each of the two snapshots is common to all DTCs.
 * The remainder, which may be zero-length, can be defined on a per-DTC
 * basis. The common part of a snapshot is recorded by the function
 * obd_record_snapshot_common(). The DTC-specific part, if any, is
 * recorded by the function specified in the DTC's DEF_DTC definition.
 *
 * When a snapshot is reported, the logical elements it contains are
 * reported individually, with each element being preceded by a DID that
 * indicates the number of bytes occupied by the element and how to
 * interpret it. It would be wasteful to store this information in
 * the snapshot record itself and NVS space is very limited. Therefore,
 * each type of snapshot record, including the common one, has a descriptor
 * of type obd_snapshot_descriptor_s that is used to unpack the snapshot
 * record and turn it into a snapshot report when requested.
 *
 * Each entry in the descriptor specifies the DID and the number of bytes
 * the corresponding data occupies. The final entry in the descriptor
 * is denoted by an entry that specifies a size of 0 bytes.
 * 
 * @todo It's easy to calculate the size of each descriptor statically, so
 * investigate the possibility of passing that size to the function that
 * reports snapshots, thus avoiding the need for the terminator.
 *
 * <h3> Freeze Frame </h3>
 *
 * Freeze frame data is recorded in obd_ram_image.obd_ff. The freeze frame
 * is a collection of data parameters that are identified and access via
 * diagnostics request using an associated PID.
 *
 * The freeze frame data is recorded when the first emission related DTC 
 * becomes pending. Subsequent emission related DTCs will not trigger the
 * freeze frame capture unless the freeze frame has been cleared. 
 *
 * The freeze frame is cleared via the diagnostic service $04, $14 or when
 * the DTC which caused the freeze frame capture clears (heals itself). The
 * freeze frame is considered 'empty' when the DTC parameter (pid02_dtc) is
 * set to 0.
 *
 * <h2> Interface </h2>
 *
 * The public functions provided by the OBD feature are divided into
 * three groups:
 *   - functions called when an internal event occurs, e.g. a fault is set
 *   - functions called in response to UDS diagnostic service request
 *   - miscellaneous functions, including the background task
 *
 * <h2> Requirement Satisfaction </h2>
 *
 * | Requirement              | Satisfied By | Unit Tested By | PATed by |
 * | -------------------------|--------------|----------------|----------|
 * | REQ_DIAG_FM_005          | obd_clear_diagnostic_information |  | pat_obd_svc14__clear_group |
 * | REQ_DIAG_FM_006          | obd_clear_diagnostic_information |  | pat_obd_svc14__clear_group |
 * | REQ_DIAG_FM_037          | vmi_update_12v_logic_obd_status() |  | |
 * | REQ_DIAG_FM_038          | vmi_update_12v_logic_obd_status() |  | |
 * | REQ_DIAG_FM_039          | vmi_update_12v_logic_obd_status() |  | |
 * | REQ_DIAG_FM_040          | vmi_update_12v_logic_obd_status() |  | |
 * | REQ_DIAG_FM_041          | FM_FLAG_LOW_V_NOMON,fm_update_fault_filter() |  | |
 * | REQ_DIAG_FM_053**        | #OBD_DTC_FORMAT |  | |
 * | REQ_DIAG_FM_054          | various |  | |
 * | REQ_DIAG_FM_055          | #obd_ram_image |  | |
 * | REQ_DIAG_FM_056          | #obd_ram_image |  | |
 * | REQ_DIAG_FM_057          | #obd_ram_image |  | |
 * | REQ_DIAG_FM_058          | #obd_ram_image but unused  until DTC lifecycles are implemented |  | |
 * | REQ_DIAG_FM_059          | #obd_ram_image |  | |
 * | REQ_DIAG_FM_060          | #obd_ram_image |  | |
 * | REQ_DIAG_FM_061          | #obd_ram_image |  | |
 * | REQ_DIAG_FM_062          | #obd_ram_image |  | |
 * | REQ_DIAG_FM_063          | #obd_ram_image but unused |  | |
 * | REQ_DIAG_FM_064          | #obd_clear_dtc_only |  | |
 * | REQ_DIAG_FM_065          | #obd_dtc_s |  | |
 * | REQ_DIAG_FM_066          | #obd_dtc_s |  | |
 * | REQ_DIAG_FM_067          | #obd_dtc_s, #obd_record_snapshot |  | pat_obd_svc19__snapshot_distinct |
 * | REQ_DIAG_FM_068          | #obd_clear_dtc_only |  | |
 * | REQ_DIAG_FM_069          | #obd_fault_present_event |  | pat_obd_svc19__extdata_occurrence |
 * | REQ_DIAG_FM_070          | #obd_fault_not_present_event |  | pat_obd_svc19__extdata_healing |
 * | REQ_DIAG_FM_071          | #obd_dtc_confirmed_event |  | pat_obd_svc19__extdata_frequency |
 * | REQ_DIAG_FM_072          | #obd_inc_all_ign_cnt |  | pat_obd_svc19__extdata_ignition_cycle |
 * | REQ_DIAG_FM_073          | #obd_update_faults_at_start_of_drive_cycle, #obd_fault_monitored_event |  | pat_obd_svc19__extdata_uncompleted_test |
 * | REQ_DIAG_FM_074          | #vio_read_IC_HCAN, #obd_record_snapshot_common |  | pat_obd_svc19__snapshot_active_DTC |
 * | REQ_DIAG_FM_075          | #vio_read_BCPowertrain, #obd_record_snapshot_common |  | pat_obd_svc19__snapshot_active_DTC |
 * | REQ_DIAG_FM_076          | #vio_read_ECM_14, #obd_record_snapshot_common |  | pat_obd_svc19__snapshot_active_DTC |
 * | REQ_DIAG_FM_077          | #vio_read_ECM_14, #obd_record_snapshot_common |  | pat_obd_svc19__snapshot_active_DTC |
 * | REQ_DIAG_FM_078          | #vio_read_HVAC_SDCP, #obd_record_snapshot_common |  | pat_obd_svc19__snapshot_active_DTC |
 * | REQ_DIAG_FM_079          | #vio_read_SBCPTStatus, #obd_record_snapshot_common |  | pat_obd_svc19__snapshot_active_DTC |
 * | REQ_DIAG_FM_080          | #obd_record_snapshot |  | pat_obd_svc19__snapshot_DTC_specific |
 * | REQ_DIAG_CDS_001         | #obd_control_dtc_setting |  | pat_obd_svc85__* |
 * | REQ_DIAG_CDS_002         | CANbedded |  | pat_obd_svc85__functional |
 * | REQ_DIAG_CDS_003         | CANbedded |  | Many of pat_obd_svc_85__* but not tested in Programming sessions |
 * | REQ_DIAG_CDS_004         | CANbedded |  | Many of pat_obd_svc_85__* |
 * | REQ_DIAG_CDS_005         | Not a software requirement  |  | |
 * | REQ_DIAG_CDS_006         | #obd_control_dtc_setting  |  | pat_obd_svc_85__groups |
 * | REQ_DIAG_CDS_007         | #obd_control_dtc_setting  |  | pat_obd_svc_85__groups |
 * | REQ_DIAG_CDS_008         | Note 3 |  | |
 * | REQ_DIAG_CDS_009         | #obd_control_dtc_setting, #fm_update_fault_filter  |  | pat_obd_svc_85__control |
 * | REQ_DIAG_CDS_010         | #obd_control_dtc_setting, #fm_update_fault_filter  |  | pat_obd_svc_85__control |
 * | REQ_DIAG_CDS_011         | #obd_control_dtc_setting  |  | Note 4 |
 * | REQ_DIAG_CDS_012         | #obd_control_dtc_setting  |  | pat_obd_svc85__setting_idempotent |
 * | REQ_DIAG_CDS_013         | Note 1 |  | |
 * | REQ_DIAG_CDS_014         | #obd_clear_dtc_only |  | pat_obd_svc85__DTC_clearing |
 * | REQ_DIAG_CDS_015         | #obd_dtc_setting_disabled |  | Note 2 |
 * | REQ_DIAG_CDS_016         | ApplDescOnTransitionSession |  | pat_obd_svc85__session_timeout, pat_obd_svc85__session_transition |
 * | REQ_DIAG_CDS_017         | #obd_dtc_setting_disabled |  | pat_obd_svc85__reset |
 * | REQ_DIAG_CDS_018         | update_faults_at_start_of_drive_cycle, obd_dtc_confirmed_event, obd_dtc_fault_present_event, Note 5 |  | pat_obd_svc85__status |
 * | REQ_DIAG_CDS_019         | CANbedded |  | pat_obd_svc85__invalid_subfunction |
 * | REQ_DIAG_CDS_020         | ApplDescSendTurn_DTC_Setting_On, ApplDescSendTurn_DTC_Setting_Off |  | pat_obd_svc85__invalid_format |
 * | REQ_DIAG_CDS_021         | Note 1 |  | |
 * | REQ_DIAG_CDS_022         | #obd_control_dtc_setting |  | pat_obd_svc85_invalid_dtc_group |
 * | REQ_DIAG_CDS_023         | CANbedded |  | pat_obd_svc85__default_session |
 * | REQ_P11_DIAG_FM_079      | #obd_clear_diagnostic_information |  | pat_obd_svc14__clear_group |
 * | REQ_BMS_SUP_OBD_001      |  |  |  |
 * | REQ_BMS_SUP_OBD_002      |  |  |  |
 * | REQ_BMS_SUP_OBD_003      |  |  |  |
 * | REQ_BMS_SUP_OBD_004      |  |  |  |
 * | REQ_BMS_SUP_OBD_006      |  |  |  |
 * | REQ_BMS_SUP_OBD_007      |  |  |  |
 * | REQ_BMS_SUP_OBD_010      |  |  |  |
 * | REQ_BMS_SUP_OBD_011      |  |  |  |
 * | REQ_BMS_SUP_OBD_012      |  |  |  |
 * | REQ_BMS_SUP_OBD_013      |  |  |  |
 * | REQ_BMS_SUP_OBD_014      |  |  |  |
 * | REQ_BMS_SUP_OBD_015      |  |  |  |
 * | REQ_BMS_SUP_OBD_016      |  |  |  |
 * | REQ_BMS_SUP_OBD_017      |  |  |  |
 * | REQ_BMS_SUP_OBD_018      |  |  |  |
 * | REQ_BMS_OBD_LC_001       | #obd_nvs_wrapper_s::obd_dtc |  |  |
 * | REQ_BMS_OBD_LC_002       | #obd_nvs_wrapper_s::obd_status, #obd_dtc_extdata_s::is_permanent, #obd_dtc_extdata_s::cycle_cnt |  | pat_obd_life_cyc__dtc_heal_pending, pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_003       | obd_fault_present_event |  | pat_obd_life_cyc__dtc_healing, pat_obd_life_cyc__dtc_heal_pending, pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_004       | obd_lc_state_clear |  | pat_obd_life_cyc__dtc_healing, pat_obd_life_cyc__dtc_heal_pending, pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_005       | obd_lc_state_clear |  | pat_obd_life_cyc__dtc_healing, pat_obd_life_cyc__dtc_heal_pending, pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_006       | obd_lc_entry_pending, obd_lc_entry_pending_confirmed_permanent |  |  |
 * | REQ_BMS_OBD_LC_007       | #obd_ram_image |  |  |
 * | REQ_BMS_OBD_LC_008       | obd_lc_state_pending |  | pat_obd_life_cyc__dtc_healing, pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_009       | obd_lc_state_pending |  | pat_obd_life_cyc__dtc_healing, pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_010       | obd_lc_state_pending |  | pat_obd_life_cyc__dtc_heal_pending |
 * | REQ_BMS_OBD_LC_011       | obd_clear_dtc_and_fault |  | pat_obd_life_cyc__dtc_heal_pending |
 * | REQ_BMS_OBD_LC_012       | obd_clear_dtc_and_fault |  | pat_obd_life_cyc__dtc_healing, pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_013       | obd_clear_dtc_and_fault |  | pat_obd_life_cyc__dtc_healing, pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_014       | obd_lc_state_pending_confirmed_permanent |  | pat_obd_life_cyc__dtc_healing, pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_015       | obd_lc_entry_confirmed_permanent |  | pat_obd_life_cyc__dtc_healing, pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_016       | obd_lc_state_confirmed_permanent |  | pat_obd_life_cyc__dtc_healing, pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_017       | obd_lc_state_confirmed_permanent |  | pat_obd_life_cyc__dtc_healing, pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_018       | obd_lc_entry_confirmed_permanent |  | pat_obd_life_cyc__dtc_healing |
 * | REQ_BMS_OBD_LC_019       | obd_clear_dtc_and_fault |  | pat_obd_life_cyc__dtc_healing |
 * | REQ_BMS_OBD_LC_020       | obd_lc_state_confirmed |  | pat_obd_life_cyc__dtc_healing |
 * | REQ_BMS_OBD_LC_021       | obd_lc_state_confirmed |  | pat_obd_life_cyc__dtc_healing |
 * | REQ_BMS_OBD_LC_022       | obd_lc_state_confirmed |  | pat_obd_life_cyc__dtc_healing |
 * | REQ_BMS_OBD_LC_023       | obd_lc_state_confirmed_permanent |  | pat_obd_life_cyc__dtc_healing, pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_024       | #obd_ram_image |  |  |
 * | REQ_BMS_OBD_LC_025       | obd_lc_state_permanent |  | pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_026       | obd_lc_state_permanent |  | pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_027       | obd_lc_state_permanent |  | pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_028       | obd_lc_state_pending_permanent |  | pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_029       | obd_lc_state_pending_permanent |  | pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_030       | obd_lc_state_pending_permanent |  | pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_031       | obd_lc_entry_pending_permanent |  |  |
 * | REQ_BMS_OBD_LC_032       | obd_clear_dtc_and_fault |  | pat_obd_life_cyc__permanent_dtcs |
 * | REQ_BMS_OBD_LC_033       | obd_is_mil_set |  |  |
 * | REQ_BMS_OBD_DC_001       | #obd_engine_start_rpm_th |  | pat_obd_cyc__warmup_cycle |
 * | REQ_BMS_OBD_DC_002       | obd_is_engine_running |  | pat_obd_cyc__warmup_cycle |
 * | REQ_BMS_OBD_WUC_001      | obd_detect_warmup_cycle |  | pat_obd_cyc__warmup_cycle |
 * | REQ_BMS_OBD_WUC_002      | obd_detect_warmup_cycle |  | pat_obd_cyc__warmup_cycle |
 * | REQ_BMS_OBD_WUC_003      | obd_detect_warmup_cycle |  | pat_obd_cyc__warmup_cycle |
 * | REQ_BMS_OBD_NT_001       | obd_detect_normal_trip |  | pat_obd_cyc__normal_trip_eng_run_time, pat_obd_cyc__normal_trip_veh_speed_time, pat_obd_cyc__normal_trip_idle_conditions, pat_obd_cyc__normal_trip_idle_time, pat_obd_cyc__normal_trip |
 * | REQ_BMS_OBD_NT_002       | obd_detect_normal_trip |  | pat_obd_cyc__normal_trip_eng_run_time, pat_obd_cyc__normal_trip_veh_speed_time, pat_obd_cyc__normal_trip_idle_conditions, pat_obd_cyc__normal_trip_idle_time, pat_obd_cyc__normal_trip |
 *
 * ** There are two requirements numbered FM_053. This refers to the one in
 * in section 2.10.3.
 *
 * Notes:
 * 1. Conditions are always correct for controlling DTC setting so this
 *    server is always able to start/stop logging and never needs to send
 *    a "Conditions not correct" response.
 * 2. It's not been possible to test this yet because the ECU Reset service
 *    isn't implemented. It should be straightforward to add a P/AT similar to
 *    pat_obd_svc85__reset that uses the reset service instead of the
 *    prodder.
 * 3. Individual DTC control isn't required and isn't supported though
 *    the design makes it easy to implement.
 * 4. It's not easy to test for this - it's just how it works.
 * 5. This requirement appears to conflict with REQ_DIAG_CDS_014 in that
 *    DTC clearing is specifically required to occur even if DTC setting
 *    is disabled for a DTC. Further, only the OBD_TEST_WARNING_INDICATOR
 *    can be unaffacted by DTC clearing and even then only in the case that
 *    the DTC requests the warning indicator be "latched for failsafe"
 *    (ISO 14229:2006 Table D.10).
 *
 * <h2> Version Numbers </h2>
 * The software shall contain the following:
 * | Version Number                      | Owning Feature | Notes                                  |
 * | ----------------------------------- | -------------- | -------------------------------------- |
 * | Application Software version number |                |                                        |
 * | OEM Software version number         |                | EOL data?                              |
 * | Pack serial number                  |                | EOL data?                              |
 * | Calibration identifier              |                |                                        |
 * | Calibration Verification number     |                | 32-bit CRC, to be calculated within 1s of startup, where is it stored NVM?  |
 * | Application Verification number     |                | 32-bit CRC, to be calculated within 60s of startup, where is it stored NVM? |
 *
 * <h3>Questions</h3>
 * -# Are any of the above already stored
 * -# Are any of the above already calculated
 * -# Do we have a 32 bit CRC routine
 * -# Are the verification numbers cleared when we re-program the unit
 *
 * <h2> OBD Events </h2>
 * The folllowing events shall be triggered by the FM module:
 * - fault monitored, the fault has been updated sufficiently for OBD to know that
 *   the fault is either present or not present, until this happens the fault state
 *   is indeterminate
 * - fault present, the fault state has changed from not present to present
 * - fault not present, the fault state has changed from present to not present
 *
 * <h2> Limitations </h2>
 * Describe any limitations of the feature, if there are any.
 *
 * <h2> How to use </h2>
 * A description of exactly what the application and other features must do to
 * use this feature.
 *
 * <h2> Configuration </h2>
 * If the feature has configuration items then point to where they are.
 *
 * <h2> Requirement Satisfaction </h2>
 * A link to the requirements document, and optionally a list of where in the
 * design the requirements are satisfied.
 *
 * @defgroup obd OBD On-Board Diagnostics
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "basic_types.h"
#include "obd_types.h"
#include "fm_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * CALIBRATIONS
 *****************************************************************************/


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern bool_t obd_calibration_checksum_available; /** The calibration checksum has been computed & is available (if TRUE) */
extern bool_t obd_application_checksum_available; /** The application checksum has been computed & is available (if TRUE) */

extern bool_t          obd_fault_present_detected;
extern bool_t          obd_fault_not_present_detected;
extern bool_t          obd_fault_monitored_detected;
extern Fm_fault_list_e obd_fault_type;

/**
 * The maximum number of DTCs that can be active at any moment.
 */
extern FROM uint8_t obd_max_active_dtcs;

/**
 * The number of supported DTCs in this configuration.
 */
extern FROM uint16_t obd_n_supported_dtcs;

/**
 * The number of DTCs that are currently active
 * This isn't stored in OBD's NVS data but is recalculated from the stored
 * DTC data on initialisation
 */
extern uint8_t obd_num_active_dtcs;

/**
 * RAM image of OBD non-volatile storage
 */
extern FRAM obd_nvs_wrapper_s obd_ram_image;

/**
 * Odometer reading (km) divided by 10 and clipped to a maximum of 65535
 * in the event that the actual odometer reading is higher.
 */
extern uint16_t         obd_Odometer;

/**
 * Raw ambient temperature obtained from vehicle I/O interface and suitable
 * for storage and onward transmission
 */
extern uint8_t          obd_TAmbientRaw;

/**
 * Time of day: second
 */
extern uint8_t          obd_tSecond;

/**
 * Time of day: minute
 */
extern uint8_t          obd_tMinute;

/**
 * Time of day: hour
 */
extern uint8_t          obd_tHour;

/**
 * Time of day: day
 */
extern uint8_t          obd_tDay;

/**
 * Time of day: month
 */
extern uint8_t          obd_tMonth;

/**
 * Time of day: year
 */
extern uint8_t          obd_tYear;

/**
 * Vehicle speed (kph) divided by 2
 */
extern uint8_t          obd_vVehicle;

/**
 * Engine speed (rpm) divided by 100
 */
extern uint8_t          obd_nEngine;

/**
 * Battery voltage multiplied by 10
 */
extern uint8_t          obd_VBattery;

/**
 * Calculated load value (used for freeze frame data)
 * Scaling/bit as per ISO 15031-5 
 *    100/255 %
 */
extern uint8_t          obd_ff_CalculatedLoad;

/**
 * Engine coolant temperature (used for freeze frame data)
 * Scaling/bit as per ISO 15031-5 
 *    1C with -40C offset
 */
extern uint8_t          obd_ff_TEngineCoolant;

/**
 * Engine RPM (used for freeze frame data)
 * Scaling/bit as per ISO 15031-5 
 *    1/4rpm 
 */
extern uint16_t         obd_ff_nEngine;

/**
 * Vehicle speed sensor (used for freeze frame data)
 * Scaling/bit as per ISO 15031-5 
 *    1kmh (Note: 255kmh max!)
 */
extern uint8_t          obd_ff_vVehicle;

/**
 * Absolute pedal position D (used for freeze frame data)
 * Scaling/bit as per ISO 15031-5 
 *    100/255 %
 */
extern uint8_t          obd_ff_AbsolutePedalPosnD;

/**
 * Absolute pedal position E (used for freeze frame data)
 * Scaling/bit as per ISO 15031-5 
 *    100/255 %
 */
extern uint8_t          obd_ff_AbsolutePedalPosnE;


/**
 * Engine coolant temperature.
 */
extern obd_temperature_t    obd_TEngineCoolant;

/**
 * Vehicle speed signal for instrument cluster display.
 */
extern uint16_t         obd_vVehicleIC;


/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/******************************************************************************
 *                                                                            *
 * Public functions in the event callback group                               *
 *                                                                            *
 *****************************************************************************/

/**
 * Update a DTC when a fault occurs and call any event-driven functions
 * that are triggered by the changes.
 *
 * @param fault_type the fault that has just occurred
 *
 * @param fault_subtype the subtype of the fault that has just occurred
 *
 * If DTC setting is enabled for this fault and the (fault, subtype) pair
 * corresponds to a DTC then perform the the following actions:
 *   - If the DTC isn't active then attempt to activate it using
 *     get_or_create_dtc_handle()
 *   - If the DTC is active then:
 *    - Update the OBD status for the DTC and call any event-driven
 *      functions using #obd_update_status
 *    - Increment the extended data fault occurrence counter
 *    - Record the "latest failed test" snapshot for this DTC
 */
extern void obd_fault_present_event(const Fm_fault_list_e fault_type,
        const Fm_fault_subtype_list_e fault_subtype);

/**
 * Update a DTC when a fault clears and call any event-driven functions
 * that are triggered by the changes.
 *
 * @param fault_type the fault that has just cleared
 *
 * @param fault_subtype the subtype of the fault that has just cleared
 *
 * If DTC setting is enabled for this fault and the (fault, subtype) pair
 * corresponds to a DTC then perform the the following actions:
 * Otherwise, increment the healing counter for the DTC provided that all
 *   - If the DTC is active and is confirmed then increment its extended
 *     data healing counter
 *   - Update the OBD status for the DTC and call any event-driven
 *     functions using #obd_update_status
 */
extern void obd_fault_not_present_event(const Fm_fault_list_e fault_type,
        const Fm_fault_subtype_list_e fault_subtype);

/**
 * Update the status of DTCs and call any event-driven functions that
 * are triggered by the changed when the test for a fault has completed,
 * i.e. the fault is considered to have been monitored.
 *
 * @param fault_type the fault that has become monitored.
 *
 * If DTC setting is enabled for this fault:
 *   - Update the OBD status for each DTC that is related to this fault
 *     (they have all become monitored at the same time) and call any
 *     event-driven functions using #obd_update_status_for_fault
 *   - If there is an active DTC for this fault then clear the extended
 *     data uncompleted test counter. There cannot be more than one
 *     such DTC because it's not possible for two subfaults to be active
 *     simulataneously.
 */
extern void obd_fault_monitored_event(const Fm_fault_list_e fault_type);

/**
 * Update all DTC statuses on an ignition-on transition
 */
extern void obd_ignition_on_event(void);

/**
 * Initialise the OBD feature.
 */
extern void obd_init(void);

/**
 * Clear all faults and remove all stored DTC data. This is used by FM
 * to restore the faults and DTCs to a clean state when NVS restoration fails.
 */
extern void obd_reinit(void);

/**
 * Periodic task used for OBD cycle evaluation (drive cycle, warm-up cycle 
 * and normal trip).
 */

/**
 *
 * Process ISO 14229 service $14: ClearDiagnosticInformation<br>
 * 
 * @param req the request data byte buffer
 *
 * For this service, there should be 3 request data bytes that specify the
 * group of DTCs that are to be cleared. Although the service can also be
 * used to clear a single DTC, there is no requirement to support this
 * function (FM_007 P12 Section 2.10).<br>
 * The 3-byte request specifies the group of DTCs to be cleared according
 * to the P11 spec (Section 7, definition of DTC groups):<br>
 * | Request  | Group                      |
 * |----------|----------------------------|
 * | 0x000000 | Emission-related systems   |
 * | 0x100000 | Powertrain DTCs            |
 * | 0x400000 | Chassis DTCs               |
 * | 0x800000 | Body DTCs                  |
 * | 0xC00000 | Network communication DTCs |
 * | 0xffffff | All DTCs                   |
 * When asked to clear "All DTCs", this function clears all faults, whether or
 * not they have a corresponding DTC. Otherwise, it only clears faults for
 * which there is a DTC.
 * The "Emission-related systems" group of DTCs is taken to be that group
 * for which the calibratable "obd" is set.
 * @retval TRUE indicates that the request was well-formed, i.e. the request
 *         corresponded to a supported group.
 * @retval FALSE otherwise.
 */
bool_t obd_clear_diagnostic_information(const obd_msg_byte_t *req);

/**
 *
 * Process ISO 14229 service $19 $01: ReportNumberOfDtcByStatusMask<br>
 * 
 * @param req the request data byte buffer
 * @param res_len a pointer to the location into which the function should
 *                number of data bytes in the response
 * @param res the response data byte buffer into which the function should
 *            write the response
 *
 * The single request data byte supplies the DTC status mask.<br>
 * Any DTCs for which the current status has _any_ (not all - see
 * ISO 14229:2006 Section 11.3.1.3) of the masked bits are reported. The
 * response bytes are placed into the res[] buffer by this function as
 * follows:<br>
 *  - res[0]: a byte indicating which mask bits are supported by the
 *            implementation<br>
 *  - res[1]: a byte indicating the DTC FormatIdentifier used by the
 *            implementation<br>
 *  - res[2]: number of DTCs matching the mask (most significant 8 bits)<br>
 *  - res[3]: number of DTCs matching the mask (least significant 8 bits)<br>
 */
void obd_report_number_of_dtc_by_status_mask(const obd_msg_byte_t *req,
                                             obd_msg_len_t *res_len,
                                             obd_msg_byte_t *res);

/**
 *
 * Process ISO 14229 service $19 $02: ReportDtcByStatusMask<br>
 * 
 * @param req the request data byte buffer
 * @param res_len a pointer to the location into which the function should
 *                number of data bytes in the response
 * @param res the response data byte buffer into which the function should
 *            write the response
 *
 * The single request data byte supplies the DTC status mask.<br>
 * Any DTCs for which the current status has _any_ (not all - see
 * ISO 14229:2006 Section 11.3.1.3) of the masked bits are reported. The
 * response bytes are placed into the res[] buffer by this function as
 * follows:<br>
 *  - res[0]: a byte indicating which mask bits are supported by the
 *            implementation<br>
 *  For each DTC i that matches the mask
 *  - res[4*i+1]: DTC most significant byte
 *  - res[4*i+2]: DTC middle byte
 *  - res[4*i+3]: DTC least significant byte
 *  - res[4*i+4]: DTC status byte
 */
void obd_report_dtc_by_status_mask(const obd_msg_byte_t *req,
                                   obd_msg_len_t *res_len,
                                   obd_msg_byte_t *res);

/**
 *
 * Process ISO 14229 service $19 $04: ReportDtcSnapshotRecordByDtcNumber<br>
 * 
 * @param req the request data byte buffer
 * @param res_len a pointer to the location into which the function should
 *                number of data bytes in the response
 * @param res the response data byte buffer into which the function should
 *            write the response
 * @retval TRUE indicates that the request was well-formed, i.e. the request
 *         corresponded to a supported DTC and that the requested snapshot
 *         record was valid.
 * @retval FALSE otherwise.

 * The request data consists of 4 bytes.
 * The first 3 bytes identify the DTC whose snapshot data is being requested.
 * The final byte identifies which of the snapshot records is requested as
 * follows:
 *   - 0x00: the snapshot when the DTC first became Confirmed
 *   - 0x01: the snapshot when the most recent test failure occured
 *   - 0xff: both snapshots
 *
 * The response bytes are placed into the res[] buffer by this function as
 * follows:<br>
 *  - res[0..2]: the DTC number being reported, i.e. echo back the request<br>
 *  - res[3]: the status of that DTC
 *
 * Subsequent bytes describe the snapshot records as follows:
 *  - Snapshot record number (0x00 or 0x01)
 *  - number of DIDs in this snapshot record
 *  - a list of (DID, data) pairs
 */
bool_t obd_report_dtc_snapshot_data(const obd_msg_byte_t *req,
                                    obd_msg_len_t *res_len,
                                    obd_msg_byte_t *res);

/**
 *
 * Process ISO 14229 service $19 $06: ReportDtcExtendedDataRecordByDtcNumber<br>
 * 
 * @param req the request data byte buffer
 * @param res_len a pointer to the location into which the function should
 *                number of data bytes in the response
 * @param res the response data byte buffer into which the function should
 *            write the response
 * @retval TRUE indicates that the request was well-formed, i.e. the request
 *         corresponded to a supported DTC and that the requested extended
 *         data record was valid.
 * @retval FALSE otherwise.

 * The request data consists of 4 bytes.
 * The first 3 bytes identify the DTC whose extended data is being requested.
 * The final byte identifies which of the extended data records (i.e. fields)
 * is requested as follows:
 *   - 0x01: occurrence counter
 *   - 0x02: healing counter
 *   - 0x03: frequency counter
 *   - 0x04: ignition counter
 *   - 0x05: uncompleted test counter
 *   - 0xff: all counters
 *
 * The response bytes are placed into the res[] buffer by this function as
 * follows:<br>
 *  - res[0..2]: the DTC number being reported, i.e. echo back the request<br>
 *  - res[3]: the status of that DTC
 *
 * Subsequent bytes consist of (field_number, field_data) pairs.
 */
bool_t obd_report_dtc_extended_data(const obd_msg_byte_t *req,
                                    obd_msg_len_t *res_len,
                                    obd_msg_byte_t *res);

/**
 *
 * Process ISO 14229 service $19 $0a: ReportSupportedDtc<br>
 * 
 * @param res the response data byte buffer into which the function should
 *            write the response
 *
 * This service is similar to $19 $02 except that it has no request data
 * bytes. Instead, an implicit DTC status mask of 0xff is applied.
 * the DTC status mask to be applied.<br>
 * Provided that there is no request data, this function writes an
 * entry into the res[] buffer for each DTC as follows:<br>
 *  - res[0]: a byte indicating which mask bits are supported by the
 *            implementation<br>
 *  For each DTC i
 *  - res[4*i+1]: DTC most significant byte
 *  - res[4*i+2]: DTC middle byte
 *  - res[4*i+3]: DTC least significant byte
 *  - res[4*i+4]: DTC status byte
 * @retval The number of bytes written to the response buffer.
 */
uint16_t obd_report_supported_dtc(obd_msg_byte_t *res);

/**
 *
 * Process ISO 14229 service $85: ControlDTCSetting<br>
 * 
 * @param req the request data byte buffer which describes the DTC group
 *            which is to have DTC setting controlled
 *
 * @param enable_setting TRUE if DTC setting is to be enabled and FALSE
 *            if it is to be disabled
 *
 * @param nrp pointer to location into which any negative response code
 *            should be written
 *
 * For this service, there should be 3 request data bytes that specify the
 * group of DTCs that is to be controlled. Although the service can also be
 * used to control a single DTC, there is no requirement to support this
 * function (CDS_008 P12 Section 2.11).<br>
 * The 3-byte request specifies the group of DTCs to be controlled according
 * to the P11 spec (Section 7, definition of DTC groups):<br>
 * | Request  | Group                      |
 * |----------|----------------------------|
 * | 0x000000 | Emission-related systems   |
 * | 0x100000 | Powertrain DTCs            |
 * | 0x400000 | Chassis DTCs               |
 * | 0x800000 | Body DTCs                  |
 * | 0xC00000 | Network communication DTCs |
 * | 0xffffff | All DTCs                   |
 * When asked to control "All DTCs", this function enables or disables
 * updating of all faults, whether or not they correspond to a DTC.
 * Otherwise, it only clears faults for which there is a corresponding DTC.
 * The "Emission-related systems" group of DTCs is taken to be that group
 * for which the calibratable "obd" is set.
 * @retval TRUE indicates that the request was well-formed, i.e. the request
 *         corresponded to a supported group.
 * @retval FALSE otherwise, in which case a negative response code is written
 *         to the location pointed to by nrp
 */
bool_t obd_control_dtc_setting(const obd_msg_byte_t *req,
        bool_t enable_setting, obd_msg_byte_t *nrp);

/*****************************************************************************
 *                                                                           *
 * Public functions in the miscellaneous group
 *                                                                           *
 ****************************************************************************/

/**
 * OBD background task
 *
 * Since most OBD functions are called-back, either by CANbedded or FM, there
 * isn't much work for this function to do.
 * It monitors the calibration item obd_reset_all_faults and, on transition
 * from FALSE to TRUE, it causes all faults, whether or not they correspond
 * to DTCs and whether or not those DTCs are Permanent, to be cleared.
 */
void obd_main(void);

/**
 * Return a value suitable for use by the ReadECUDiagnosticState service
 * that indicates the status of DTC setting.
 *
 * @return 0x00 DTC setting is undefined
 * @return 0x10 DTC setting is enabled
 * @return 0x20 DTC setting is disabled
 *
 * The requirements assume that DTC setting is a binary option so far as
 * reporting its status is concerned. However, the requirements also specify
 * that DTC setting must be controlled by DTC group. Therefore, it's possible
 * for DTC setting to be enabled for some DTCs but not for others.
 *
 * The approach taken for this function is to return 0x10 when DTC setting
 * is enabled for all supported DTCs and to return 0x20 when it is disabled
 * for all supported DTCs. In any other situation, 0x00 (unknown / undefined)
 * is returned.
 *
 * The safest way to implement this is to iterate over the supported DTCs.
 * Checking the bit vector directly might give an answer (i.e. if every
 * DTC maps to a fault) but if there are "gaps" in the bit vector created
 * by faults that have no DTC, then there's no option but to check the
 * the supported DTCs.
 */
uint8_t obd_get_dtc_setting_status(void);

/**
 *
 * Is DTC setting enabled for a DTC?
 *
 * @param fault_type corresponding to the DTC about which the enquiry
 *                   is being made
 * 
 * This function relies on the fact that all DTCs that are mapped onto
 * a given fault are members of the same groups, i.e. if one DTC is a
 * member of group G then all DTCs are members of group G. This doesn't
 * preclude a DTC from belonging to more than one group, e.g.
 * powertrain and emission-relevant DTCs.
 */
bool_t obd_is_dtc_setting_enabled(Fm_fault_list_e fault_type);

/**
 *
 * Process ISO 15031 service $01: RequestCurrentPowertrainDiagnosticData<br>
 * 
 * @param req the request data byte buffer
 * @param req_len number of request data in the request buffer
 * @param res_len a pointer to the location into which the function should
 *                write the number of data bytes in the response
 * @param res the response data byte buffer into which the function should
 *            write the response
 *
 * @return TRUE if the request is valid and the response data has been set
 * @return FALSE if the request was rejected. The request is rejected if it
 *               contains a mixture of support and data PIDs.
 *
 * The service may either request the PID support status or individual PIDs.
 * A mixture of PID support status and PID data requests in a single message
 * is not permitted.
 */
bool_t obd_report_emission_diag_data(const obd_msg_byte_t *req,
        obd_msg_len_t req_len, obd_msg_len_t *res_len, obd_msg_byte_t *res);

/**
 *
 * Process ISO 15031 service $02: RequestPowertrainFreezeFrameData<br>
 * 
 * @param req the request data byte buffer
 * @param req_len number of request data in the request buffer
 * @param res_len a pointer to the location into which the function should
 *                write the number of data bytes in the response
 * @param res the response data byte buffer into which the function should
 *            write the response
 *
 * @return TRUE if the request is valid and the response data has been set
 * @return FALSE if the request was rejected. The request is rejected if it
 *               contains a mixture of support and data PIDs or invalid frame
 *               numbers.
 *
 * The service may either request the PID support status or individual PIDs.
 * A mixture of PID support status and PID data requests in a single message
 * is not permitted.
 */
bool_t obd_report_emission_ff(const obd_msg_byte_t *req, obd_msg_len_t req_len, 
       obd_msg_len_t *res_len, obd_msg_byte_t *res);

/**
 *
 * Process ISO 15031 service $03: RequestEmissionRelatedDTCs<br>
 * 
 * @param res_len a pointer to the location into which the function should
 *                write the number of data bytes in the response
 * @param res the response data byte buffer into which the function should
 *            write the response
 *
 * The service reports only confirmed DTCs.
 */
void obd_report_emission_dtcs_confirmed(obd_msg_len_t *res_len, 
        obd_msg_byte_t *res);

/**
 *
 * Process ISO 15031 service $04: ClearEmissionRelatedDiagnosticInfo<br>
 *
 * The service clears emission related faults, DTC status and environmental 
 * data.
 */
void obd_clear_emission_dtcs(void);

/**
 *
 * Process ISO 15031 service $07: RequestEmissionRelatedDTCsCurrentDriveCycle<br>
 *  
 * @param res_len a pointer to the location into which the function should
 *                write the number of data bytes in the response
 * @param res the response data byte buffer into which the function should
 *            write the response
 *
 * The service reports only pending DTCs.
 */
void obd_report_emission_dtcs_pending(obd_msg_len_t *res_len, 
        obd_msg_byte_t *res);

/**
 *
 * Process ISO 15031 service $09: RequestVehicleInformation<br>
 * 
 * @param req the request data byte buffer
 * @param req_len number of request data in the request buffer
 * @param res_len a pointer to the location into which the function should
 *                write the number of data bytes in the response
 * @param res the response data byte buffer into which the function should
 *            write the response
 *
 * @return TRUE if the request is valid and the response data has been set
 * @return FALSE if the request was rejected. The request is rejected if it
 *               contains a mixture of support and info PIDs or more than 1
 *               info PID request in the same message.
 *
 * The service may either request the PID support status or individual PIDs.
 * A mixture of PID support status and PID data requests in a single message
 * is not permitted.
 */
bool_t obd_report_emission_veh_info(const obd_msg_byte_t *req, 
        obd_msg_len_t req_len, obd_msg_len_t *res_len, obd_msg_byte_t *res);

/**
 *
 * Process ISO 15031 service $0A: RequestPermanentEmissionRelatedDTCs<br>
 * 
 * @param res_len a pointer to the location into which the function should
 *                write the number of data bytes in the response
 * @param res the response data byte buffer into which the function should
 *            write the response
 *
 * The service reports only permanent DTCs.
 */
void obd_report_emission_dtcs_permanent(obd_msg_len_t *res_len, 
        obd_msg_byte_t *res);

/**
 *
 * Return whether MIL is required to be illuminated. MIL status 
 * should be set if at least one emission related fault is Confirmed
 * and Permanent.
 * 
 * @return TRUE if MIL should be illuminated, FALSE if not
 */
bool_t obd_is_mil_set(void);

/**
 *
 * Clears an active DTC given its fault type and sub-type and deletes
 * any environmental data associated with it. The DTC is cleared only
 * if it is confirmed and/or pending.
 *
 * @param fault_type the fault type associated with the DTC
 *
 * @param subtype the subfault type associated with the DTC
 *
 */
void obd_clear_active_dtc_and_env_data(Fm_fault_list_e fault_type,
                                       Fm_fault_subtype_list_e subtype);

#endif      /* OBD_H */

