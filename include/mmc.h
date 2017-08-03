/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc.h
 *
 * description:  Header file for MMC-application-specific data types etc
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(MMC_H)
#define      MMC_H
/**
 *
 * Header file for MMC-application-specific data types etc.
 *
 * These are shared between MMC features that may need to include these
 * definitions within their own header file.
 * MMC features that need these definitions can pull in this header instead of
 * basic_types.h
 *
 * @file
 * @ingroup util
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "basic_types.h"
#include "ts.h"
#include "fixedpoint.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/
#define OHM_BP             10           /**< bin point for ohm_t */
#define OHM_PER_VOLT_BP     4           /**< bin point for ohm_per_volt_t */
#define MILLIOHM_BP        10           /**< bin point for milliohm_t */
#define CURRENT_BP          0           /**< bin point for current_t */
#define ENERGY_BP           0           /**< bin point for energy_t */
#define ENERGY_WH_BP        6           /**< bin point for energy_wh_t */
#define CYCLE_ENERGY_WH_BP  13          /**< bin point for cycle_energy_wh_t */
#define HIGH_RES_CHARGE_BP  0           /**< bin point for high_res_charge_t */
#define CHARGE_BP           3           /**< bin point for charge_t */
#define PACK_V_BP           4           /**< bin point for pack_v_t */
#define SUPPLY_V_BP        10           /**< bin point for supply_v_t */
#define OBD_TEMPERATURE_BP  7           /**< bin point for obd_temperature_t */
#define POWER_BP            0           /**< bin point for power_t
                                          @todo value not yet known*/
#define TIME_BP          0           /**< bin point for ts_time_t */
#define TICKS_BP         0           /**< bin point for ts_ticks_t */
#define PERCENT_BP       8           /**< bin point for numbers wanting a
                                          range of 0 to 100 */
#define COUNT_BP         0           /**< bin point for cellcount_t etc. */
#define FRACTION_BP      15          /**< bin point for fraction_t */
#define SPEED_BP         8           /**< bin point for speed_t */
#define FLOW_LPM_BP      2           /**< bp for flow_t litres/min */
#define SECONDS_X10_BP   0           /**< bin point for seconds_x10_t */
#define FREQUENCY_BP     7

#define OHM_MAX          UINT16_MAX   /**< maximum value for ohm_t */
#define OHM_MIN          UINT16_MIN   /**< minimum value for ohm_t */
#define MILLIOHM_MAX     UINT16_MAX   /**< maximum value for milliohm_t */
#define MILLIOHM_MIN     UINT16_MIN   /**< minimum value for milliohm_t */
#define OHM_PER_VOLT_MAX INT32_MAX    /**< maximum value for ohm_per_volr_t */
#define OHM_PER_VOLT_MIN INT32_MIN    /**< minimum value for ohm_per_volr_t */
#define CURRENT_MAX      INT32_MAX    /**< maximum value for current_t */
#define CURRENT_MIN      INT32_MIN    /**< minimum value for current_t */
#define ENERGY_MAX       UINT32_MAX   /**< maximum value for energy_t */
#define ENERGY_WH_MAX    INT32_MAX    /**< maximum value for energy_wh_t */
#define HIGH_RES_CHARGE_MAX    INT32_MAX   /**< maximum value for high_res_charge_t */
#define HIGH_RES_CHARGE_MIN    INT32_MIN   /**< minimum value for high_res_charge_t */
#define CHARGE_MAX       INT32_MAX   /**< maximum value for charge_t */
#define CHARGE_MIN       INT32_MIN   /**< minimum value for charge_t */
#define PACK_V_MAX       SINT16_MAX  /**< maximum value for pack_v_t */
#define PACK_V_MIN       SINT16_MIN  /**< minimum value for pack_v_t */
#define SUPPLY_V_MAX     INT16_MAX  /**< maximum value for supply_v_t */
#define SUPPLY_V_MIN     INT16_MIN  /**< minimum value for supply_v_t */
#define POWER_MAX        UINT16_MAX  /**< maximum value for power_t */

/** @note The Maximum and Minimum values for ts_time_t #TS_TIME_T_MAX
 * and #TS_TIME_T_MIN are defined in ts_types.h */

#define PACK_V(v)          BP_SCALE( (v),            pack_v_t,          PACK_V_BP ) /**< Convert a float into pack_v_t */
#define SUPPLY_V(v)        BP_SCALE( (v),            supply_v_t,        SUPPLY_V_BP ) /**< Convert a float into supply_v_t */
#define FRACTION(x)        BP_SCALE( (x),            fraction_t,        FRACTION_BP ) /**< Convert a float into fraction_t */
#define SECONDS_X10(x)     BP_SCALE( (seconds_x10_t) ((x) / 10.0f), seconds_x10_t, SECONDS_X10_BP)                          /**< Convert a float into seconds_x10_t */
#define OHM(x)             BP_SCALE( (x),            ohm_t,             OHM_BP )      /**< Convert a float (in ohm) into ohm_t */
#define OHM_TO_MILLIOHM(x) BP_SCALE(((x * 1000.0f)), milliohm_t,        MILLIOHM_BP ) /**< Convert a float (in ohm) into mohm_t */
#define OHM_PER_VOLT(x)    BP_SCALE( (x),            ohm_per_volt_t,    OHM_PER_VOLT_BP )      /**< Convert a float (in ohm) into ohm_t */
/*lint -emacro(960,AMPS) Violates MISRA 2004 Required Rule 10.4, Prohibited
 * cast of complex floating-point expression: Integer versus Floating-point */
#define AMPS(x)           ((current_t) ((x) * 1000.0f))                               /**< Convert a float (in amps) into current_t (in mA) */
#define CHARGE(x)          BP_SCALE( (x),            charge_t,          CHARGE_BP)   /**< Convert a float into charge_t */
#define HIGH_RES_CHARGE(x) BP_SCALE( (x),            high_res_charge_t, HIGH_RES_CHARGE_BP)  /**< Convert a float into high_res_charge_t */
#define TIME(x)            BP_SCALE( (x),            ts_time_t,         TIME_BP) /**< Convert a float into ts_time_t */
#define PERCENT(p)         BP_SCALE( (p),            percent_t,         PERCENT_BP) /**< macro to set a fixed-point percentage from a floating point number */
#define KM_P_HR(x)         BP_SCALE( (x),            speed_t,           SPEED_BP)
#define FLOW_LPM(x)        BP_SCALE( (x),            flow_t,            FLOW_LPM_BP)
#define ENERGY_WH(x)       BP_SCALE( (x),            energy_wh_t,       ENERGY_WH_BP ) /**< Convert a float into energy_wh_t */
#define CYCLE_ENERGY_WH(x) BP_SCALE( (x),            cycle_energy_wh_t, CYCLE_ENERGY_WH_BP ) /**< Convert a float into cycle_energy_wh_t */
#define OBD_TEMP(x)        BP_SCALE( (x),            obd_temperature_t, OBD_TEMPERATURE_BP ) /**< Convert a float into obd_temperature_t */
#define FREQ(x)            BP_SCALE( (x),            frequency_t,       FREQUENCY_BP )      /**< Convert a float into frequency_t */

/** Constant for milliamps in an amp */
#define MILLIAMP_IN_AMP   ((int32_t)          1000)

/**
 * Number of PIT interrupts between ADC reads. Currently, based upon 50us interrupt,
 * will read ADCs every 10ms.
 */
#define AI_PIT_INTERRUPT_COUNT 10u

/** Milliseconds in an MMC Foreground slice */
#define MMC_FG_MS       (AI_PIT_INTERRUPT_COUNT * MMC_FG_PIT_MS)

/** Milliseconds in an MMC Foreground slice */
#define MMC_FG_PIT_MS       1u

/** Milliseconds in an MMC background slice */
#define MMC_BG_MS       250u

/** Macro to set validity flag to "old" & prevent multiple instances of the same code */
#define CLEAR_VALIDITY(flag) ((flag) = ((flag) == New_data_available) ? Old_data_available : (flag))

/** Macro to set validity flag to "new" & prevent multiple instances of the same code */
#define SET_VALIDITY(flag)   ((flag) = New_data_available)

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
/**
 * Available top-level states from the MS feature. */
 enum ms_state_e
{
    Ms_state_low_power,   /**< BMS uninitialised  */
    Ms_state_init,        /**< BMS initialised    */
    Ms_state_standby,     /**< BMS standby */
    Ms_state_veh_closing, /**< BMS closing vehicle contactors */
    Ms_state_veh_opening, /**< BMS opening vehicle conactors */
    Ms_state_veh_active,  /**< BMS in vehicle active mode */
    Ms_state_charger,     /**< BMS in charger mode */
    Ms_state_shutdown ,   /**< BMS in shutdown mode */
    Ms_state_error,
    /* The values above this point are hard-coded in the network catalogue and
     * should not be altered unless the corresponding changes are agreed
     * for the dbcs */
    Ms_max_states
};

/**
 * Possible values of BMS Status */
enum bms_status_e
{
    NBMSStatus_Initialisation           = 0,
    NBMSStatus_Standby                  = 1,
    NBMSStatus_Running__no_warning_     = 2,
    NBMSStatus_Running__B3_warning_     = 3,
    NBMSStatus_Running__B4_warning_     = 4,
    NBMSStatus_Running__B5_warning_     = 5,
    NBMSStatus_Running__B6_warning_     = 6,
    NBMSStatus_Running__B7_warning_     = 7,
    NBMSStatus_Not_running__B8_warning_ = 8,
    NBMSStatus_Precharge                = 9,
    NBMSStatus_Shutting_down            = 10,
    NBMSStatus_Cell_balancing_only      = 11,
    NBMSStatus_External_charging        = 12,
    NBMSStatus_B1_Fault                 = 13,
    NBMSStatus_B2_Fault                 = 14,
    NBMSStatus_Not_running__B9_warning_ = 15,
    NBMSStatus_Default                  = 31
};

/**
 * Data type used to represent percentages */
typedef uint16_t        percent_t;

/**
 * Data type used to represent pack voltages
 * It has a binary point of #PACK_V_BP  */
typedef int16_t        pack_v_t;

/**
 * Data type used to represent supply voltages
 * It has a binary point of #SUPPLY_V_BP  */
typedef int16_t         supply_v_t;

/**
 * Data type used to represent resistance (ohm)
 * It has a binary point of #OHM_BP  */
typedef uint32_t        ohm_t;

/**
 * Data type used to represent resistance (mohm)
 * It has a binary point of #MILLIOHM_BP  */
typedef uint16_t         milliohm_t;

/**
 * Data type used to represent resistance per volt (ohm_per_volt)
 * It has a binary point of #OHM_PER_VOLT_BP  */
typedef int32_t          ohm_per_volt_t;

/**
 * Data type used to represent current (mA) */
typedef int32_t         current_t;

/**
 * Data type used to represent energy (mJ) */
typedef uint32_t         energy_t;

/**
 * Data type used to represent total energy throughput (Wh) */
typedef uint32_t         energy_wh_t;

/**
 * Data type used to represent energy throughput in a single cycle (Wh) */
typedef uint32_t         cycle_energy_wh_t;

/**
 * Data type used to represent high resolution charge (uC - microCoulombs) */
typedef int32_t         high_res_charge_t;


/**
 * Data type used to represent power (W) */
typedef uint16_t        power_t;

/**
 * Datatype used to count the cells in the pack, this needs to be increased to
 * a uint16_t if more than 255 cells must be supported. */
typedef uint8_t         cellcount_t;

/**
 * Data type used to represent fractions.
 **/
typedef uint16_t         fraction_t;

/**
 * Data type used to represent debug seconds data.
 **/
typedef uint16_t         seconds_x10_t;

/**
  * Data type used to represent speeds in terms of km/h
 **/
typedef uint32_t         speed_t;

/** Coolant flow in l/min */
typedef uint16_t        flow_t;

/** Temperature used for OBD (requires larger range than temperature_t) */
typedef int16_t        obd_temperature_t;

/**
 * Data type used to represent charge (Coulombs) */
typedef int32_t         charge_t;

/**
 * Data type used to represent frequency.
 **/
typedef uint32_t        frequency_t;

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

#endif      /* MMC_H */
