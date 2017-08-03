/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  ctc_types.h
 *
 * description:  
 * Public header for the contactor control feature
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
#if !defined(CTC_TYPES_H)
#define      CTC_TYPES_H
/**
 *
 * Public header for the contactor control feature
 *
 * @file
 * @ingroup ctc
 */
#include "nvs_cfg.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

#define CTC_NVS_USED (sizeof(nvs_header_s) + sizeof(ctc_nvs_data_s))

#define CTC_PC_RES_DEC_PERIOD_MS ((ts_time_t)1000) /**< Period between
        decrementing energy from #ctc_prechg_res_mj_ohm for REQ_BMS_CTC_PREC_003 */

/** Shorthand macro for establishing the validity of the current
 *  DC link contactor position indication */
#define CTC_DCLINK_STATE_VALID \
    (bool_t)(ctc_dclink_contactors_closed_validity == New_data_available)

/** Shorthand macro for establishing the validity of the current
 *  charger contactor position indication */
#define CTC_CHARGER_STATE_VALID \
    (bool_t)(ctc_charger_contactors_closed_validity == New_data_available)




/* bit positions for contactors states */
#define PRELS      0u
#define PREHS      2u
#define VEHNEGLS   4u
#define VEHNEGHS   6u
#define VEHPOSLS   8u
#define VEHPOSHS   10u
#define CHGNEGLS   12u
#define CHGNEGHS   14u
#define CHGPOSLS   16u
#define CHGPOSHS   18u



/******************************************************************************
 * PUBLIC TYPE DEFINITIONS
 *****************************************************************************/
#include "fm_types.h"
#include "ts_types.h"


typedef CONST volatile uint16_t* fb_sig_address_t;
/**
 * CTC NVS layout version to ensure incompatible data isn't reloaded after
 * reflashing
 */
#define CTC_NVS_LAYOUT_VERSION          1u
/** CTC NVS data - update #CTC_NVS_LAYOUT_VERSION if changing */
typedef struct
{
    energy_t prechg_res_mj; /**< The amount of energy per ohm in the pre-charge
                                 resistor */
    uint32_t last_prechg_dec_update_s; /**< Time (in sec) of most recent prechg
                                            resistor energy decrement */
} ctc_nvs_data_s;

typedef struct
{
    nvs_header_s header;    /**< Feature Information Structure.*/
    ctc_nvs_data_s data;    /**< Module NV data-set. */
    uint8_t                reserved_bytes[(int16_t) CTC_MAX_NVS_BYTES
                                                - (int16_t) CTC_NVS_USED];
                                                /**< Feature Reserved_bytes.*/
} ctc_nvs_wrapper_s;


/**
 * position flags within the contactor psn variable
 */
enum Ctc_cont_state_e
{
    Ctc_cont_open        = 0u,
    Ctc_cont_closed      = 1u,
    Ctc_cont_unk         = 3u
};

#define CSET(cont, state)   ((uint32_t)(state) << (cont))
/* Note that the macro CGET requires Ctc_cont_unk to be 3 masking both contactor bit */
#define CGET(cont_bit_psn, c_state) ((enum Ctc_cont_state_e) ((c_state & ((uint32_t)Ctc_cont_unk << cont_bit_psn)) >> cont_bit_psn))

/* short hand to close */
#define PRE_LS    CSET(PRELS, Ctc_cont_closed)
#define PRE_HS    CSET(PREHS, Ctc_cont_closed)
#define VEH_NG_LS CSET(VEHNEGLS, Ctc_cont_closed)
#define VEH_NG_HS CSET(VEHNEGHS, Ctc_cont_closed)
#define VEH_PS_LS CSET(VEHPOSLS, Ctc_cont_closed)
#define VEH_PS_HS CSET(VEHPOSHS, Ctc_cont_closed)
#define CHG_NG_LS CSET(CHGNEGLS, Ctc_cont_closed)
#define CHG_NG_HS CSET(CHGNEGHS, Ctc_cont_closed)
#define CHG_PS_LS CSET(CHGPOSLS, Ctc_cont_closed)
#define CHG_PS_HS CSET(CHGPOSHS, Ctc_cont_closed)

#define CHG_NEG_CLOSED (CHG_NG_LS | CHG_NG_HS)
#define CHG_POS_CLOSED (CHG_PS_LS | CHG_PS_HS)
#define VEH_PRE_CLOSED (PRE_LS    | PRE_HS)
#define VEH_NEG_CLOSED (VEH_NG_LS | VEH_NG_HS)
#define VEH_POS_CLOSED (VEH_PS_LS | VEH_PS_HS)


/* short hand for uncommanded */
#define NO_CMD_VEH_POS  (CSET(VEHPOSLS, Ctc_cont_unk) | CSET(VEHPOSHS, Ctc_cont_unk))
#define NO_CMD_VEH_NEG  (CSET(VEHNEGLS, Ctc_cont_unk) | CSET(VEHNEGHS, Ctc_cont_unk))
#define NO_CMD_VEHICLE  (NO_CMD_VEH_NEG | NO_CMD_VEH_POS)

#define NO_CMD_CHG_POS  (CSET(CHGPOSLS, Ctc_cont_unk) | CSET(CHGPOSHS, Ctc_cont_unk))
#define NO_CMD_CHG_NEG  (CSET(CHGNEGLS, Ctc_cont_unk) | CSET(CHGNEGHS, Ctc_cont_unk))
#define NO_CMD_CHARGER  (NO_CMD_CHG_NEG | NO_CMD_CHG_POS)

#define ALL_OPEN   0u

/* The current to use for 'wear' if the reading is missed
 * This replaces the uses of 'magic number' 1500 Amps */
#define CTC_MISSED_WEAR_CURRENT AMPS(0)




/**
 * Opening and closing sequences
 *
 * Note that ctc_seq_last_measurement_fault relies on there being a Ctc_seq_complete.
 */
enum Ctc_sequence_e
{
    Ctc_seq_none,
    Ctc_seq_veh_opening,
    Ctc_seq_veh_closing,
    Ctc_seq_chg_opening,
    Ctc_seq_chg_closing,
    Ctc_seq_complete
};

enum Ctc_not_allowed_reason_e
{
    Ctc_na_none,
    Ctc_na_veh_delayed_pos_dmd,           /**< contactor has been closed recently */
    Ctc_na_veh_delayed_neg_dmd,           /**< contactor has been closed recently */
    Ctc_na_veh_delayed_prechg_dmd,        /**< contactor has been closed recently */
    Ctc_na_cont_chg_psn_expect,           /**< charger operating @mantis{4747} almost certainly will need expanding once requirements are available */
    Ctc_na_vmi_12vlogicsupply_status,     /**< 5 duff 12v logic supply           */
    Ctc_na_vmi_hvil_status,               /**< 6 duff hvil supply                */
    Ctc_na_fail_func_inhibit,             /**< 7 contactor closure is inhibited via a fault */
    Ctc_na_vmi_low_res_bus_0v,            /**< 8 bus volts detected              */
    Ctc_na_imi_hv_bus_zero_curr_status,   /**< 9 veh bus current detected        */
    Ctc_na_prechg_res_mj,                 /**< 10 cooking pre-charge resistor     */
    Ctc_na_mdm_cell_v_complete,           /**< 11 not a complete set of cell volts readings */
    Ctc_na_veh_pos_at_idle,               /**< 12 hs driver isn't idle            */
    Ctc_na_veh_neg_at_idle,               /**< 13 ''  */
    Ctc_na_prechg_at_idle,                /**< 14 ''  */
    Ctc_na_om_prechg_HS_drv_chip_fault,   /**< 15 hs driver chips are faulty      */
    Ctc_na_om_veh_pos_HS_drv_chip_fault,  /**< 16 ''  */
    Ctc_na_om_veh_neg_HS_drv_chip_fault,  /**< 17 ''  */
    Ctc_na_sequence_fault                 /**< 18 last seq caused a fault         */
};

enum Ctc_charge_not_allowed_reason_e
{
    Ctc_na_chg_chg_none,
    Ctc_na_chg_veh_cont_closed,     /**< One or more vehicle contactors are closed  */
    Ctc_na_chg_chg_cont_closed,     /**< One or more charger contactors are closed  */
    Ctc_na_chg_12V_bat_range,       /**< 12V Battery out of change                     */
    Ctc_na_chg_hvil_status,         /**< duff hvil supply                              */
    Ctc_na_chg_fail_func_inhibit,   /**< charger cont closure is inhibited via a fault */
    Ctc_na_chg_vmi_bus_zero_v,      /**< zero volts not observed on vehicle bus        */
    Ctc_na_chg_vmi_chg_bus_cell_v,  /**< charger bus not close to summed cell voltage (call 029) */
    Ctc_na_chg_imi_non_zero,        /**< imi indicates a bus current is observed    */
    Ctc_na_chg_mdm_cell_v_complete, /**< incomplete set of cell voltages            */
    Ctc_na_chg_chg_drvs_idle,       /**< charger bus high_side drivers are not at idle  */
    Ctc_na_chg_chg_drivs_chip_flt,  /**< charger bus high-side driver faults        */
    Ctc_na_chg_chg_sequence_fault  /**< last charger close sequence caused a fault */
};

/**
 * Opening and closing sequences
 *
 * Note that ctc_seq_last_measurement_fault relies on there being a Ctc_seq_complete.
 */
enum Ctc_continual_check_fail_e
{
    Ctc_contin_chk_none,             /* No check has failed since reset */
    Ctc_contin_chk_hvil_dmd_true,    /* HVIL not intact, contactor dmd true */
    Ctc_contin_chk_hvil_dmd_false,   /* HVIL not intact, contactor dmd false */
    Ctc_contin_chk_vbus_disch_to,    /* Vehicle bus discharge timeout */
    Ctc_contin_chk_cbus_disch_to     /* Charger bus discharge timeout */
};

/******************************************************************************
 * PRIVATE TYPE DEFINITIONS
 *****************************************************************************/

/** Actions for ctc_write_outputs */
enum ctc_action_steps_e
{
    Ctc_action_apply_outputs,     /**< Apply outputs defined in the sequence tbl  */
    Ctc_action_delay,             /**< Delaying for perios defined in sequnce tbl */
    Ctc_action_measure,           /**< Make the mesurement defined in sequence tbl*/
    Ctc_action_step_complete,     /**< ctc step is complete */
    Ctc_action_failed             /**< Measurement defined in seq step failed     */
};
typedef enum ctc_action_steps_e ctc_action_step_enum ;




/**
 * Measurement actions that can be utilized by a contactor sequence
 *  These reference as embedded in the sequence table
 */
enum ctc_measure_e
{
    Ctc_no_measure,               /**< No Measurement step is required                  */
    Ctc_fail_tst,                 /**< measurement always fails (force sequence change) */

    Ctc_cap_bal_delay,            /**< Neg contactor closed to balance
                                       capacitance */
    Ctc_prechg_hs_fb,             /**< Precharge HS driver feedback is OK         */
    Ctc_latch_disable,            /**< Disable the driver latch                   */
    Ctc_veh_pos_hs_fb,            /**< main Pos HS driver feedback is OK          */
    Ctc_veh_neg_hs_fb,            /**< main Neg HS driver feedback is OK          */
    Ctc_chg_pos_hs_fb,            /**< chg pos HS driver feedback is OK           */
    Ctc_chg_neg_hs_fb,            /**< chg Neg HS driver feedback is OK           */

    Ctc_prechg_hs_fault,          /**< prechg hs chip feedback detected a fault   */
    Ctc_veh_pos_hs_fault,         /**< veh pos hs chip feedback detected a fault  */
    Ctc_veh_neg_hs_fault,         /**< veh neg hs chip feedback detected a fault  */
    Ctc_chg_pos_hs_fault,         /**< chg pos hs chip feedback detected a fault  */
    Ctc_chg_neg_hs_fault,         /**< chg neg hs chip feedback detected a fault  */

    Ctc_prechg_ls_fb,             /**< Precharge LS driver feedback is OK         */
    Ctc_veh_pos_ls_fb,            /**< veh pos LS driver feedback is OK           */
    Ctc_veh_neg_ls_fb,            /**< veh neg LS driver feedback is OK           */
    Ctc_chg_pos_ls_fb,            /**< chg neg LS driver feedback is OK           */
    Ctc_chg_neg_ls_fb,            /**< chg neg LS driver feedback is OK           */

    Ctc_prechg_hs_unpwr,          /**< preChr HS driver unpowered                 */
    Ctc_prechg_hs_pwr,            /**< preChr HS driver has power                 */
    Ctc_veh_pos_hs_unpwr,         /**< main pos HS driver unpowered               */
    Ctc_veh_neg_hs_unpwr,         /**< main neg HS driver unpowered               */
    Ctc_veh_neg_hs_pwr,           /**< main neg HS driver has power               */
    Ctc_veh_pos_hs_pwr,           /**< main Pos HS driver has power               */
    Ctc_chg_pos_hs_pwr,           /**< chg Pos HS driver has power               */
    Ctc_chg_neg_hs_pwr,           /**< chg neg HS driver has power               */
    Ctc_chg_pos_hs_unpwr,         /**< chg pos HS driver unpowered               */
    Ctc_chg_neg_hs_unpwr,         /**< chg neg HS driver unpowered               */

    Ctc_plus_ve_low_res_zero,     /**< HV +ve is ~zero                            */
    Ctc_veh_bus_low_res_zero,     /**< HV Bus low res is ~zero                    */
    Ctc_neg_ve_low_res_zero,      /**< HV -ve is ~zero                            */

    Ctc_veh_bus_low_res_prechg_ok,/**< HV Bus voltage is < ctc_cal 006                          */
    Ctc_veh_bus_low_res_range_ok, /**< HV Bus volts between cal_006 and cal 007                 */
    Ctc_veh_bus_low_res_cell_chk, /**< HV bus volts between cal 006 and 100% of sum cell voltage*/
    Ctc_veh_bus_low_res_prechg_complete,/**< HV Bus volts >= (summed cell volts * cal 09)/100%  */

    Ctc_veh_bus_imi_curr_zero,    /**< imi feature confirms bus current is zero                 */
    Ctc_veh_bus_imi_curr_nzero,   /**< imi feature confirms bus current is non-zero             */
    Ctc_veh_bus_imi_valid,        /**< imi feature confirms current is valid                    */
    Ctc_veh_bus_imi_curr_ok,      /**< imi feature confirms current is with bounds              */
    Ctc_veh_bus_not_zero_pos,     /**< imi feature confirms current is non-zero and mA > 0mA    */
    Ctc_veh_bus_not_zero_neg,     /**< imi feature confirms current is non-zero and mA < 0mA    */
    Ctc_flow_i_pos,               /**< Check current flow is a positive value                   */
    Ctc_pos_ve_low_res_cell_chk,  /**< HV +ve is 5v to 105% of Ecell volts                      */
    Ctc_pos_ve_low_res_range_ok,  /**< Test VE+ volts in range                                  */
    Ctc_veh_bus_imi_curr_npos,    /**< Check that the current is either negative or a small pos */
    Ctc_gfd_incomplete_chg,       /**< Check if ground fault detection is complete on charger   */
    Ctc_gfd_fault_chg,            /**< Check if a ground fault has been detected on charger     */
    Ctc_gfd_incomplete_veh,       /**< Check if ground fault detection is complete on vehicle   */
    Ctc_gfd_fault_veh,            /**< Check if a ground fault has been detected on vehciles    */
    Ctc_chg_volts_ok,             /**< Check charger volts close to pack volts                  */

    Ctc_prechg_event_wait,        /**< Time since contactor close < #ctc_prechg_event_wait_ms   */
    Ctc_veh_pos_event_wait,       /**< Time since contactor close < #ctc_veh_v_expected_ms      */
    Ctc_prechr_detect_event_wait, /**< Time since contactor close < #ctc_prechg_det_wait_ms     */
    Ctc_report_closure,           /**< Report vehicle bus contactor closure                     */
    Ctc_report_chg_closure,       /**< Report charger bus contactors closed                     */
    Ctc_report_open,              /**< Report vehcile contactor open                            */
    Ctc_report_chg_open,          /**< Report charger bus contactors open                       */
    Ctc_veh_bus_open_curr_ok,     /**< Check current flow with main +ve/-ve open                */
    Ctc_veh_pos_closed,           /**< Check that positive contactor is closed                  */
    Ctc_veh_neg_closed,           /**< Check that negative contactor is closed                  */

    ctc_measurement_max
};
typedef enum ctc_measure_e ctc_measure_enum;

enum ctc_measure_fail_action
{
    Ctc_abort,                   /**< Abort sequence - set contactors to open*/
    Ctc_main_close,              /**< Carry out Main close sequence          */
    Ctc_main_open,               /**< Carry out Main open sequence           */
    Ctc_chg_close,               /**< Carry out charger close sequence         */
    Ctc_chg_open,                /**< Carry out charger Open sequence         */
    Ctc_loop_back1,              /**< Loop back 1 step in sequence tbl       */
    Ctc_loop_back3,              /**< Loop back 3 steps in sequence tbl      */
    Ctc_loop_back4,              /**< Loop back 4 steps in sequence tbl      */
    Ctc_jump_forward3,           /**< Jump forward 3 steps in the sequence tbl*/
    Ctc_jump_forward4,           /**< Jump forward 4 steps in the sequence tbl*/
    Ctc_continue_seq,            /**< Continue with sequence                 */
    Ctc_max_fail_action
};

/**
 * Enumeration for the possible contactor states
 */
typedef enum
{
    CTC_OPEN,

    CTC_DCLINK_CLOSE_WAIT_PRECHARGE_CLOSED,
    CTC_DCLINK_CLOSE_WAIT_POS_CLOSED,
    CTC_DCLINK_CLOSE_WAIT_POS_VOLTAGE,
    CTC_DCLINK_CLOSE_WAIT_NEG_CLOSED,
    CTC_DCLINK_CLOSE_WAIT_VOLTAGE,
    CTC_DCLINK_CLOSE_WAIT_PRECHARGE_OPEN,
    CTC_DCLINK_CLOSE_WAIT_POS_OPEN,
    CTC_DCLINK_CLOSE_WAIT_NEG_OPEN,
    CTC_DCLINK_CLOSED_WAIT_PRECHARGE_OPEN,
    CTC_DCLINK_CLOSED,

    CTC_DCLINK_OPEN_WAIT_POS_OPEN_I_NEG,
    CTC_DCLINK_OPEN_WAIT_NEG_OPEN_I_NEG,

    CTC_DCLINK_OPEN_WAIT_POS_OPEN_I_POS,
    CTC_DCLINK_OPEN_WAIT_NEG_OPEN_I_POS,

    CTC_CHARGER_CLOSE_WAIT_NEG_CLOSED,
    CTC_CHARGER_CLOSE_WAIT_PRECHARGE_CLOSED,
    CTC_CHARGER_CLOSE_WAIT_POS_CLOSED,
    CTC_CHARGER_CLOSE_WAIT_POS_VOLTAGE,
    CTC_CHARGER_CLOSE_WAIT_DCLINK_POS_CLOSED,
    CTC_CHARGER_CLOSE_WAIT_DCLINK_POS_VOLTAGE,
    CTC_CHARGER_CLOSE_WAIT_DCLINK_NEG_CLOSED,
    CTC_CHARGER_CLOSE_WAIT_DCLINK_NEG_VOLTAGE,
    CTC_CHARGER_CLOSE_WAIT_PRECHARGE_OPEN,
    CTC_CHARGER_CLOSE_WAIT_DCLINK_POS_OPEN,
    CTC_CHARGER_CLOSE_WAIT_DCLINK_NEG_OPEN,
    CTC_CHARGER_CLOSE_WAIT_POS_OPEN,
    CTC_CHARGER_CLOSE_WAIT_NEG_OPEN,
    CTC_CHARGER_CLOSED_WAIT_PRECHARGE_OPEN,
    CTC_CHARGER_CLOSED,

    CTC_CHARGER_OPEN_WAIT_DCLINK_NEG_OPEN_I_NEG,
    CTC_CHARGER_OPEN_WAIT_DCLINK_POS_OPEN_I_NEG,
    CTC_CHARGER_OPEN_WAIT_POS_OPEN_I_NEG,
    CTC_CHARGER_OPEN_WAIT_NEG_OPEN_I_NEG,

    CTC_CHARGER_OPEN_WAIT_DCLINK_NEG_OPEN_I_POS,
    CTC_CHARGER_OPEN_WAIT_DCLINK_POS_OPEN_I_POS,
    CTC_CHARGER_OPEN_WAIT_POS_OPEN_I_POS,
    CTC_CHARGER_OPEN_WAIT_NEG_OPEN_I_POS

} ctc_state_t;

/**
 * Enumeration for all possible CTC errors
 */
typedef enum
{
    CTC_FEEDBACK_DCLINK_POS,   /* Error on position feedback signal from DCLINK
                                  positive contactor */
    CTC_FEEDBACK_DCLINK_NEG,   /* Error on positition feedback signal from
                                  DCLINK negative contactor */
    CTC_FEEDBACK_CHARGER_POS,  /* Error on position feedback signal from charger
                                  positive contactor */
    CTC_FEEDBACK_CHARGER_NEG,  /* Error on position feedback signal from charger
                                  negative contactor */
    CTC_DRIVER_PRECHARGE_I,    /* Current draw by pre-charge contactor out of
                                  range */
    CTC_DRIVER_DCLINK_POS_I,   /* Current draw by DCLINK positive contactor out
                                  of range */
    CTC_DRIVER_DCLINK_NEG_I,   /* Current draw by DCLINK negative contactor out
                                  of range */
    CTC_DRIVER_CHARGER_POS_I,  /* Current draw by charger positive contactor out
                                  of range */
    CTC_DRIVER_CHARGER_NEG_I,  /* Current draw by charger negative contactor out
                                  of range */
    CTC_DRIVER_DCLINK,         /* Feedback from driver associated with DCLINK 
                                  indicates error.
                                  Low level (and more specific fault code) will
                                  be indicated by OM module errors) */
    CTC_DRIVER_CHARGER,        /* Feedback from driver associated with charger
                                  indicates error.
                                  Low level (and more specific fault code) will
                                  be indicated by OM module errors)  */
    CTC_DCLINK_POS_STUCK_CLOSED, /* Position feedback indicates the DCLINK
                                    positive contactor is unexpectedly closed */
    CTC_DCLINK_NEG_STUCK_CLOSED, /* Position feedback indicates the DCLINK
                                    negative contactor is unexpectedly closed */
    CTC_DCLINK_POS_STUCK_OPEN, /* Position feedback indicates the DCLINK
                                  positive contactor is unexpectedly open */
    CTC_DCLINK_NEG_STUCK_OPEN, /* Position feedback indicates the DCLINK
                                  negative contactor is unexpectedly open */
    CTC_CHARGER_POS_STUCK_CLOSED, /* Position feedback indicates the charger
                                     positive contactor is unexpectedly closed */
    CTC_CHARGER_NEG_STUCK_CLOSED, /* Position feedback indicates the charger
                                     negative contactor is unexpectedly closed */
    CTC_CHARGER_POS_STUCK_OPEN, /* Position feedback indicates the charger
                                   positive contactor is unexpectedly open */
    CTC_CHARGER_NEG_STUCK_OPEN, /* Position feedback indicates the charger
                                   negative contactor is unexpectedly open */
    CTC_DCLINK_V,               /* DCLINK voltage out of range during 
                                   pre-charge */
    CTC_DCLINK_I,               /* DCLINK current was non-zero at start of
                                   attempt to close contactors */
    CTC_DCLINK_PRECHARGE_I,     /* DCLINK current was out of range during
                                   precharge */
    CTC_CHARGER_V,
    CTC_CHARGER_I,               /* Charger bus current was non-zero at 
                                    start of attempt to close contactors */
    CTC_MAX_ERRORS
} ctc_error_conditions_t;

/** Enumeration of contactor state feedback (analogue) signals */
typedef enum
{
    Ctc_fb_dclink_pos,
    Ctc_fb_dclink_neg,
    Ctc_fb_charger_pos,
    Ctc_fb_charger_neg,

    Ctc_fb_sig_count
} ctc_feedback_sig_t;



/** Structure containing the scaling parameters of analogue contactor
 *  state feedback signals */
typedef struct
{
    supply_v_t gain;
    supply_v_t offset;
} ctc_fb_sig_calibration_t;


/** Structure containing the configuration of the contactor feedback 
 *  signals */
typedef struct
{
    fb_sig_address_t              signal_address;
    data_validity_e              *signal_validity;
    CAL ctc_fb_sig_calibration_t *cal;
} ctc_fb_sig_config_t;


struct ctc_sequence_step
{

    uint32_t contactor_state;
    FPTR ts_time_t * delay;             /* pointer to cal delay or null == 0ms*/
    ctc_measure_enum measurement_ref;   /* measurement required this step    */
    Fm_fault_list_e  flt_tp;            /* fault to raise if measurement fails */
    Fm_fault_subtype_list_e  flt_subtp;
    enum ctc_measure_fail_action fail_action;  /* what shall we do next of fail*/
    uint8_t fault_inhibit;
};
typedef struct ctc_sequence_step ctc_seq_step_strt ;

typedef struct ctc_debug_frame_s
{
    pack_v_t    overall_voltage;
    pack_v_t    pos_voltage;
    int16_t     latest_imi;
    uint8_t     save_seq;
    uint8_t     marker;
} ctc_debug_frame;

#endif


/* CTC_TYPES_H */
