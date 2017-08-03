/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   obd_p.h
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
#if !defined(OBD_P_H)
#define      OBD_P_H
/**
 *
 * On-Board Diagnostics
 *
 * @file
 * @ingroup obd
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#include "gf_types.h"
#include "om_types.h"
#include "tmi_types.h"
#include "vmi.h"
#include "vmi_types.h"

/*
 * enumerations and macro definitioms made available must be documented here to
 * avoid duplication.
 *
 * PRIVATE functions are documented in the source file.
 * Non-PRIVATE variables and functions (i.e. those shared between files in the
 * feature) are documented here.
 */
extern enum obd_checksum_state_e obd_calibration_checksum_state;
extern enum obd_checksum_state_e obd_application_checksum_state;

/**
 * Vector of faults corresponding to supported DTCs. 
 */
extern FROM Fm_fault_list_e obd_dtc_fault[];
/**
 * Vector of fault sub-types corresponding to supported DTCs. 
 */
extern FROM Fm_fault_subtype_list_e obd_dtc_subtype[];

/*
 * Clear any previous use of the macros that are used in fm.def
 */
#undef FAULT_TABLE
#undef FAULT_SUBTYPE
#undef FAIL_FUNC
#undef FAULT_GROUP

/**
 * The bits in the OBD status byte.
 *
 * In normal operation these bits are updated according to the table below.
 * On first initialisation, or loss of non-volatile storage, all bits are
 * cleared with the exception of OBD_TEST_NC_SINCE_CLEAR and
 * OBD_TEST_NC_THIS_CYCLE which are set.
 *
 * | Bit                         | Set                | Clear              |
 * | ----------------------------|--------------------|--------------------|
 * | OBD_TEST_FAILED             | By FM              | By FM              |
 * | OBD_TEST_FAILED_THIS_CYCLE  | failure            | start of cycle OR clear diagnostics |
 * | OBD_PENDING_DTC             | By DTC life cycle  | By DTC life cycle  |
 * | OBD_CONFIRMED_DTC           | By DTC life cycle  | By DTC life cycle  |
 * | OBD_TEST_NC_SINCE_CLEAR     | clear diagnostics  | test monitored     |
 * | OBD_TEST_FAILED_SINCE_CLEAR | failure            | clear diagnostics  |
 * | OBD_TEST_NC_THIS_CYCLE      | start of cycle OR clear diagnostics     | test monitored |
 * | OBD_TEST_WARNING_INDICATOR  | unused             | unused             |
 */
#define OBD_TEST_FAILED                 0x01u
#define OBD_TEST_FAILED_THIS_CYCLE      0x02u
#define OBD_PENDING_DTC                 0x04u
#define OBD_CONFIRMED_DTC               0x08u
#define OBD_TEST_NC_SINCE_CLEAR         0x10u
#define OBD_TEST_FAILED_SINCE_CLEAR     0x20u
#define OBD_TEST_NC_THIS_CYCLE          0x40u
#define OBD_TEST_WARNING_INDICATOR      0x80u

/**
 * The OBD DTCStatusMask that is supported by this implementation.<br>
 * The only bit that isn't supported is:
 *  - bit 7: warningIndicator
 *
 * The other bits are implemented although bit 3 (confirmedDTC) is always
 * reported true for any DTC that is set.
 * @todo implement warningIndicator for those DTCs that require it
 */
#define OBD_SUPPORTED_MASK      ( \
    OBD_TEST_FAILED               | \
    OBD_TEST_FAILED_THIS_CYCLE    | \
    OBD_PENDING_DTC               | \
    OBD_CONFIRMED_DTC             | \
    OBD_TEST_NC_SINCE_CLEAR       | \
    OBD_TEST_FAILED_SINCE_CLEAR   | \
    OBD_TEST_NC_THIS_CYCLE)

/**
 * The DTC diagnostic format that is supported by this implementation
 */
#define OBD_DTC_FORMAT          0x00u   /* ISO15031-6DTCFormat */

/**
 * DTC warm-up cycle count threshold for transitioning from 'confirmed'
 * to 'clear' status
 */
#define OBD_DTC_LC_CONF_TO_CLEAR_TH             (40u)

/**
 * DTC drive cycle count threshold for transitioning from 
 * 'pnding,confirmed,permanent' to 'confirmed,permanent' status
 */
#define OBD_DTC_LC_PEN_CON_PERM_TO_CON_PERM_TH  (1u)

/**
 * DTC drive cycle count threshold for transitioning from 
 * 'confirmed,permanent' to 'confirmed' status
 */
#define OBD_DTC_LC_CONF_PERM_TO_CONF_TH         (2u)

/**
 * DTC drive cycle count threshold for transitioning from 
 * 'pending, confirmed' to 'confirmed' status.
 * This is for non-OBD DTCs that cannot attain the state 
 * pending,confirmed,permanent. This count thus covers the required
 * drive cycles between 'pending,confirmed,permanent' and 'confirmed'.
 */
#define OBD_DTC_LC_PEND_CONF_TO_CONF_TH \
    (OBD_DTC_LC_CONF_PERM_TO_CONF_TH + OBD_DTC_LC_PEN_CON_PERM_TO_CON_PERM_TH)



/*
 * Feature shared function prototypes
 */

/**
 * Update the OBD status bits for a DTC and call any event-driven
 * functions that are triggered by the change
 * 
 * @param dtc_idx the index of the DTC whose status is to be changed
 *
 * @param bits_to_clear bits that are to be cleared in the status byte
 *
 * @param bits_to_set bits that are to be cleared in the status byte
 */
void obd_update_status(uint16_t dtc_idx, uint8_t bits_to_clear,
        uint8_t bits_to_set);

/**
 * Update the OBD status bits for a fault and call any event-driven
 * functions that are triggered by the change
 * 
 * @param fault_type the fault whose status is to be changed
 *
 * @param bits_to_clear bits that are to be cleared in the status byte
 *
 * @param bits_to_set bits that are to be cleared in the status byte
 *
 * This operates by identifying all DTCs that are associated with the
 * fault and updating the status of each DTC. It is intended that this
 * be called when, for example, a fault becomes monitored thereby marking
 * all associated DTCs as having been tested this cycle. If this function
 * is called when a (fault, subtype) pair is detected as having failed, it
 * will set all DTCs associated with the fault which is not the desired
 * behaviour. For dealing with single (fault, subtype) pairs, first look
 * up the corresponding DTC index and then call obd_update_status().
 */
void obd_update_status_for_fault(Fm_fault_list_e fault_type,
        uint8_t bits_to_clear, uint8_t bits_to_set);

/**
 * Perform actions when a DTC transitions to Confirmed
 *
 * @param dtc_idx index of the DTC that has just become Confirmed
 *
 * If DTC setting is enabled for this fault, record the Confirmed snapshot
 * in the DTC environment data
 */
void obd_dtc_confirmed_event(uint16_t dtc_idx);

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
 * Lookup the index of the DTC corresponding to a (fault, subtype) pair
 *
 * @param fault the fault type whose DTC index is wanted
 *
 * @param subtype the subfault type whose DTC index is wanted
 *
 * @param dtc_idx_p a pointer to the location into which the DTC index
 *                  should be written
 *
 * @return TRUE if this (fault, subtype) pair corresponds to a DTC and
 *         FALSE otherwise
 *
 * If the DTC does exist, its index is written into the location pointed
 * to by dtc_idx_p. Otherwise, this location is not modified.
 *
 * A linear search of the DTCs is made which is less than ideal. If timing
 * is an issue, it would possible, though not very nice, to use DEF_DTC
 * on fm.def to define a structure type whose members have type uint8_t and
 * whose names are a concatenation of the fault and subtype. The function
 * can be implemented as a switch statement whose case labels are an
 * encoding of the fault and subtype and whose actions are to return the
 * offsetof the member in the structure. Note: no structure need be
 * created in memory - offsetof works on types too.
 *
 * See earlier revisions of obc.c on branch b_m5915_dtcs for an example of
 * an encoding if required.
 */
bool_t lookup_dtc_idx_for_fault(Fm_fault_list_e fault,
        Fm_fault_subtype_list_e subtype, uint16_t *dtc_idx_p);

/**
 *
 * Return a handle for an active DTC given its fault type and sub-type
 *
 * @param fault_type the fault type whose DTC handle is wanted
 *
 * @param subtype the subfault type whose DTC index is wanted
 *
 * @return A handle in the range 0 .. OBD_MAX_ACTIVE_DTCS-1 if the DTC is
 *         active, or OBD_MAX_ACTIVE_DTCS if the DTC is inactive
 */
uint16_t get_dtc_handle(Fm_fault_list_e fault_type,
                        Fm_fault_subtype_list_e subtype);

/**
 * Return a handle for an active DTC given its fault type and DTC index,
 * creating a new one if necessary
 *
 * @param dtc_idx the DTC associated with the fault
 *
 * @return A handle in the range 0 .. OBD_MAX_ACTIVE_DTCS-1 if the DTC is
 *         now active, or OBD_MAX_ACTIVE_DTCS if the DTC couldn't be
 *         activated
 * 
 * The DTC index is used to find a match instead of the fault type as the
 * index represents a unique combination of fault type & sub-type. There
 * may be more than 1 active DTCs with the same fault type.
 *
 * This function could be implemented by calling get_dtc_handle() and then
 * allocating a new slot only if it fails to find the DTC. However, since
 * finding a free slot will require making another pass over the array, it's
 * as easy just to combine the two searches. This duplicates some of the
 * work of get_dtc_handle() but will, when the active DTC vector is full,
 * mean only making one pass over it rather than two.
 */
uint16_t get_or_create_dtc_handle(uint16_t dtc_idx);

/**
 * Is the DTC confirmed?
 *
 * @param dtc_idx the index of the DTC whose status is to be checked
 *
 * @return TRUE if the OBD_CONFIRMED_DTC bit is set in this DTC's status
 *         byte and FALSE otherwise.
 */
bool_t is_obd_dtc_confirmed(uint16_t dtc_idx);

/**
 * Is the DTC pending?
 *
 * @param dtc_idx the index of the DTC whose status is to be checked
 *
 * @return TRUE if the OBD_PENDING_DTC bit is set in this DTC's status
 *         byte and FALSE otherwise.
 */
bool_t is_obd_dtc_pending(uint16_t dtc_idx);

/**
 * Increment the extended data ignition cycle counter for all active DTCs
 * for which DTC setting is enabled. Each counter is saturated at its
 * maximum value
 */
void obd_inc_all_ign_cnt(void);

/**
 * Update DTCs and faults at the start of a drive cycle.
 *
 * For each active DTC for which DTC setting is enabled:
 *   - If the test for the DTC's fault hasn't completed during the 
 *     drivecycle, increment the extended data uncompleted test counter
 *
 * For each supported DTC for which DTC setting is enabled:
 *   - Clear the "test completed this cycle" bit in its OBD status
 */
void obd_update_faults_at_start_of_drive_cycle(void);

/**
 * Clear an extended data occurence counter
 *
 * @param handle A handle for the DTC whose counter is to be updated.
 *
 * The occurence counter is cleared.
 */
void obd_clear_occur_cnt(uint16_t handle);

/**
 * Increment an extended data occurrence counter
 *
 * @param handle A handle for the DTC whose counter is to be updated.
 *
 * The occurrence counter is incremented, saturating at the maximum value.
 */
void obd_inc_occur_cnt(uint16_t handle);

/**
 * Clear an extended data healing counter
 *
 * @param handle A handle for the DTC whose counter is to be updated.
 *
 * The healing counter is cleared.
 */
void obd_clear_heal_cnt(uint16_t handle);

/**
 * Increment an extended data healing counter
 *
 * @param handle A handle for the DTC whose counter is to be updated.
 *
 * The healing counter is incremented.
 */
void obd_inc_heal_cnt(uint16_t handle);

/**
 * Clear an extended data frequency counter
 *
 * @param handle A handle for the DTC whose counter is to be updated.
 *
 * The frequency counter is cleared.
 */
void obd_clear_freq_cnt(uint16_t handle);

/**
 * Increment an extended data frequency counter
 *
 * @param handle A handle for the DTC whose counter is to be updated.
 *
 * The frequency counter is incremented, saturating at the maximum value.
 */
void obd_inc_freq_cnt(uint16_t handle);

/**
 * Clear an extended data ignition cycle counter
 *
 * @param handle A handle for the DTC whose counter is to be updated.
 *
 * The ignition cycle counter is cleared.
 */
void obd_clear_ign_cnt(uint16_t handle);

/**
 * Increment an extended data ignition cycle counter
 *
 * @param handle A handle for the DTC whose counter is to be updated.
 *
 * The ignition cycle counter is incremented, saturating at the maximum value.
 */
void obd_inc_ign_cnt(uint16_t handle);

/**
 * Clear an extended data uncomplete test cycle counter
 *
 * @param handle A handle for the DTC whose counter is to be updated.
 *
 * The uncomplete test cycle counter is cleared.
 */
void obd_clear_uctest_cnt(uint16_t handle);

/**
 * Increment an extended data uncomplete test cycle counter
 *
 * @param handle A handle for the DTC whose counter is to be updated.
 *
 * The uncomplete test cycle counter is incremented, saturating at the
 * maximum value.
 */
void obd_inc_uctest_cnt(uint16_t handle);

/**
 * Clear an extended data cycle counter
 *
 * @param handle A handle for the DTC whose counter is to be updated.
 *
 * The cycle counter is cleared.
 */
void obd_clear_cycle_cnt(uint16_t handle);

/**
 * Clear an extended data is_permanent flag
 *
 * @param handle A handle for the DTC whose permanent status is to be 
 *               cleared.
 *
 * The is_permanent flag is cleared.
 */
void obd_clear_perm_status(uint16_t handle);

/**
 * Set an extended data is_permanent flag
 *
 * @param handle A handle for the DTC whose permanent status is to be 
 *               set.
 *
 * @return TRUE if the is_permanent flag is set.
 */
bool_t obd_set_perm_status(uint16_t handle);

/**
 * Increment an extended data cycle counter
 *
 * @param handle A handle for the DTC whose counter is to be updated.
 *
 * The cycle counter is incremented, saturating at the maximum value.
 */
void obd_inc_cycle_cnt(uint16_t handle);

/**
 * Grab a snapshot record for a DTC
 *
 * @param handle A handle for the DTC whose snapshot is to be grabbed
 * 
 * @param snapshot the kind of snapshot that is to be grabbed
 */
void obd_record_snapshot(uint16_t handle, Snapshot_e snapshot);

/**
 * Clear all non-Permanent emission-relevant DTCs and any environment data
 * stored for them. For Permanent DTCs retain the storage space and Permanent 
 * status flag only.
 */
void obd_clear_emission_relevant_dtcs(void);

/**
 * Is a DTC considered Permanent
 *
 * @param dtc_idx The index of the DTC which is being queried
 *
 * @return TRUE if the DTC is permanent and FALSE otherwise
 */
bool_t obd_is_dtc_permanent(uint16_t dtc_idx);

/**
 * Catches a DTC status change update to check whether an emission DTC
 * that has caused the freeze frame has been cleared.
 * 
 * @param dtc_idx the DTC associated with the cleared fault
 *
 * @param new_status the new DTC status
 */
void obd_emission_check_dtc_cleared_event(uint16_t dtc_idx, uint8_t new_status);

/**
 * Clear freeze frame data
 */
void obd_clear_freeze_frame(void);

/**
 * Update the DTC life cycle state based on the life cycle event.
 * 
 * @param dtc_idx the DTC associated with the life cycle event
 * @param lc_event the life cycle event type
 *
 * @return TRUE if the DTC state can be updated (i.e. selected DTC has had an
 *         'active DTC' slot allocated to it)
 * @return FALSE if the DTC state cannot be updated ('active DTC' slots are full)
 */
bool_t obd_update_dtc_state(uint16_t dtc_idx, obd_dtc_lc_events_e lc_event);

/**
 * Grab the freeze frame record 
 * 
 * @param dtc_idx the DTC associated with the fault
 */
void obd_record_freeze_frame(uint16_t dtc_idx);

/**
 * Return the handle for an active DTC (the index in the active 
 * DTC data array).
 * 
 * @param dtc_idx The DTC index of the DTC whose handle is wanted
 *
 * @return A handle in the range 0 .. OBD_MAX_ACTIVE_DTCS-1 if the DTC is
 *         active, or OBD_MAX_ACTIVE_DTCS if the DTC is inactive
 */
uint16_t get_dtc_handle_for_dtc_idx(uint16_t dtc_idx);

/**
 * Update all DTC statuses at the start of a drive cycle
 */

/**
 * Clear the OBD status for a DTC but do not clear the associated fault
 *
 * @param dtc_idx The index of the whose status is to be cleared
 *
 * This function does NOT remove any environment data that might have been
 * recorded for the DTC that is associated with the fault.
 *
 * This function is useful when all DTCs AND all faults have to be cleared.
 * Since there may be faults that do not correspond to a DTC, clearing all
 * DTCs is not sufficient to clear all faults. Conversely, clearing all
 * faults would require finding all DTCs corresponding to a fault and this
 * is not a cheap operation.
 *
 * It's therefore best to clear DTCs independently of faults by means of
 * this routine and then clear the faults separately.
 */
void obd_clear_dtc_only(uint16_t dtc_idx);

/**
 * Find the given DTC number in the vector of DTCs.
 *
 * @param dtc_num  The number of the DTC to be searched for
 * @param dtc_idx_p Pointer to a location in which the index of the DTC
 *                  is to be returned, assuming the DTC exists
 *
 * @return TRUE the DTC was found and FALSE otherwise
 *
 * If the DTC was found, return the index via dtc_idx_p.
 */
bool_t lookup_dtc_idx_for_dtc_num(uint32_t dtc_num, uint16_t *dtc_idx_p);

/**
 * Extract a DTC number from a diagnostic message buffer
 *
 * @param buf Pointer to the message buffer
 *
 * @return the DTC number
 *
 * The 3 bytes pointed to by buf are interpreted as a big-endian number
 * and returned
 */
uint32_t dtc_num_from_buf(const uint8_t *buf);

/**
 * Write a DTC number to a diagnostic message buffer
 *
 * @param dtc_idx The DTC index of the DTC whose number is to be written
 *
 * @param buf Pointer to the message buffer
 *
 * The DTC number is written as 3 bytes big-endian to the buffer.
 */
void dtc_num_to_buf(uint16_t dtc_idx, uint8_t *buf);


/*
 * PRIVATE function prototypes (documented in source)
 */
PRIVATE uint16_t obd_count_masked_dtcs(uint8_t mask);
PRIVATE uint16_t obd_report_masked_dtcs(bool_t ignore_mask,
        uint8_t mask, uint8_t *buf);
PRIVATE uint16_t obd_make_extdata_response(uint16_t dtc_idx,
        Extdata_field_e field, uint8_t *buf);
PRIVATE uint16_t obd_extdata_field_to_buf(const FPTR obd_dtc_extdata_s *extdata,
        Extdata_field_e field, uint8_t *buf);
PRIVATE void obd_clear_dtc_and_fault(uint16_t dtc_idx);
PRIVATE void obd_clear_all_faults(bool_t clear_perm);
PRIVATE void obd_clear_all_dtcs(void);
PRIVATE void obd_clear_dtc_group(uint8_t group_mask);
PRIVATE Fm_fault_list_e obd_map_dtc_to_fm(uint32_t dtc);
PRIVATE bool_t obd_is_dtc_group_all(const obd_msg_byte_t *req);
PRIVATE bool_t obd_is_dtc_group_emission(const obd_msg_byte_t *req);
PRIVATE bool_t obd_is_dtc_group_P(const obd_msg_byte_t *req);
PRIVATE bool_t obd_is_dtc_group_C(const obd_msg_byte_t *req);
PRIVATE bool_t obd_is_dtc_group_B(const obd_msg_byte_t *req);
PRIVATE bool_t obd_is_dtc_group_U(const obd_msg_byte_t *req);
PRIVATE void obd_control_dtc_setting_all(bool_t enable_setting);
PRIVATE void obd_control_dtc_setting_emission(bool_t enable_setting);
PRIVATE void obd_control_dtc_setting_group(bool_t enable_setting, uint8_t mask);
PRIVATE void obd_control_dtc_setting_for_fault(Fm_fault_list_e fault,
        bool_t enable_setting);

/* Private functions to grab DTC-specific snapshots */
PRIVATE void     obd_record_snapshot_dids(FPTR uint8_t *buf, uint16_t dtc_idx);
PRIVATE uint16_t obd_make_snapshot_response(uint16_t dtc_idx,
                                            Snapshot_e snapshot, uint8_t *buf);
PRIVATE uint16_t obd_report_snapshot_header(uint8_t *buf, Snapshot_e snapshot);
PRIVATE uint16_t obd_report_snapshot_dids(uint16_t dtc_idx, 
                                          const FPTR uint8_t *snapshot_buf, 
                                          uint8_t *buf);
PRIVATE uint16_t obd_report_snapshot_common_did(Snapshot_dids_e did_idx, 
                                                const FPTR uint8_t *snapshot_buf, 
                                                uint8_t *buf);
PRIVATE uint16_t obd_report_snapshot_specific_did(uint16_t dtc_idx, 
                                                  Snapshot_dids_e did_idx, 
                                                  const FPTR uint8_t *snapshot_buf, 
                                                  uint8_t *buf);
PRIVATE uint16_t obd_report_snapshot_unused_did(Snapshot_dids_e did_idx, uint8_t *buf);
PRIVATE uint16_t obd_ss_did_rec_hi_res_voltage(FPTR uint8_t *buf, uint16_t start_idx, 
                                               enum vmi_bus_e bus);
PRIVATE uint16_t obd_ss_did_rec_lo_res_voltage(FPTR uint8_t *buf, uint16_t start_idx, 
                                               enum vmi_rail_e rail);
PRIVATE uint16_t obd_ss_did_rec_supply_voltage(FPTR uint8_t *buf, uint16_t start_idx, 
                                               enum vmi_supply_e supply);
PRIVATE uint16_t obd_ss_did_rec_ctc_driver(FPTR uint8_t *buf, uint16_t start_idx, 
                                            enum Om_diagnostic_input_type_e driver);
PRIVATE uint16_t obd_ss_did_rec_gf_asymm_data(FPTR uint8_t *buf, uint16_t start_idx, 
                                              enum gf_bus_e bus);

#undef DEF_SS_DID
#undef DEF_SNAP
#define DEF_SNAP(name, dids, description)   /* not used */
#define DEF_SS_DID(id, size, description) \
    PRIVATE uint16_t obd_ss_did_rec_ ##id (FPTR uint8_t *buf, uint16_t start_idx, uint16_t dtc_idx);

#include "snap.def"
#undef DEF_SS_DID
#undef DEF_SNAP

#endif      /* OBD_P_H */
