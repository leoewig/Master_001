/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    bal_types.h
 *
 * description:  
 * Header file that contains the type definitions of the Balancing feature.
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
#if !defined(BAL_TYPES_H)
#define      BAL_TYPES_H
#include "nvs_cfg.h"
#include "ts_types.h"         /* for ts_time_t */

/**
 *
 * Header file that contains the type definitions of the Balancing feature.
 *
 * @file
 * @ingroup bal
 */

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/**
 * macro to streamline data accesses in balancing code
 */
#define BAL_NV(FIELD)               (umc_ram_image.data.FIELD)

#define BAL_NV_FLAGS BAL_NV(bal_flags)

/**
 * determine if balancing status will allow balancing to occur
 */
#define BALANCING_RUNNING(status)       \
                                    (((status) == Bal_status_chg_maintenance) \
                                     || ((status) == Bal_status_chg_correction))

/** Indication that rebalancing is active and should be reasserted */
#define BAL_FLG_PLUGCHRG_REQ 0x01u
/** Set if the upper estimate was the latest */
#define BAL_FLG_UP_LATEST    0x02u
/* Return the status of the plugchrg_req flag */
#define BAL_TEST_PLUGCHRG_REQ() TEST_MASK(BAL_NV_FLAGS,BAL_FLG_PLUGCHRG_REQ)
/* Clear the request to plug in the charger */
#define BAL_CLEAR_PLUGCHRG_REQ() do { \
    CLEAR_MASK8(BAL_NV_FLAGS,BAL_FLG_PLUGCHRG_REQ);\
} while (0)

/** Assert the request in NV for charger plug-inc */
#define BAL_SET_PLUGCHRG_REQ() do { \
    SET_MASK8(BAL_NV_FLAGS,BAL_FLG_PLUGCHRG_REQ);\
} while (0)

/** Convert a position into its NV-flag equivalent and set it in the NV image */
#define BAL_SET_LATEST_SPAN_POS(pos) do {\
    if (BalEstUpper==pos) { \
        SET_MASK8(BAL_NV_FLAGS,BAL_FLG_UP_LATEST); \
    } else { \
        CLEAR_MASK8(BAL_NV_FLAGS,BAL_FLG_UP_LATEST); \
    } \
} while(0)

/** Extract position enums from the stored NV flags
 * @param[out] latest_ position (BalEstPos_e ) of the latest reading
 * @param[out] alt_ position (BalEstPos_e) of the alternative reading
 **/
#define GET_SPAN_POS(latest_,alt_) do{\
 if (TEST_MASK(BAL_NV_FLAGS,BAL_FLG_UP_LATEST)){\
        latest_=BalEstUpper;\
        alt_=BalEstLower;\
    } else { \
        latest_=BalEstLower;\
        alt_=BalEstUpper;\
    } \
} while (0)

/** Mark the high-V span reading as the latest */
#define SET_UPPER_LATEST() SET_MASK8(BAL_NV_FLAGS,BAL_FLG_UP_LATEST)
/** Mark the low-V span reading as the latest */
#define SET_LOWER_LATEST() CLEAR_MASK8(BAL_NV_FLAGS,BAL_FLG_UP_LATEST)
/** Yield true if the span_t passed in is inside a sensible range 
 * @param[in] s a span_t to inspect */
#define IS_VALID_SPAN(s) ((s)<SPAN_PCT(MAX_FLT_SPAN))

/** Given a span position, yield the alternate position */
#define BAL_ALT_SPAN(s) (BalEstUpper==(s)?BalEstLower:BalEstUpper)

/** The largest (as a float) span that can be stored */
#define MAX_FLT_SPAN 100.0f
/** The smallest (as a float) span that can be stored */
#define MIN_FLT_SPAN 0.0f

/****************************************************************************
 * Cell  SOC span type
 * Used to represent an % range (of cell SOC) within a single byte for
 * compact EE storage
 ***************************************************************************/

/** Single byte span type - used for efficient storage of cell data spans in
 * EE */
typedef uint8_t span_t;

#define SPAN_PCT_BP 1
/** Turn a span into a binary pointed representation */
#define SPAN_PCT(pct) BP_SCALE( (pct), span_t, SPAN_PCT_BP)

#define INVALID_SPAN 0xFFu

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/**
 * Enumeration to denote the balancing status.
 * @note changes to this enum must be mirrored in the ASAP2 COMPU_VTAB
 * in bal.a2l
 */
typedef enum {
  Bal_status_uninitialised,
  Bal_status_disabled, /**< Balancing is disabled by calibration. */
  Bal_status_halted, /**< Balancing is prevented from running.
                           See #bal_halted_reason to find out why */
  Bal_status_chg_maintenance, /**< A charge estimate has not been carried out
                             within the time given by #umc_phase_period_ms,
                            so balancing is limited to preventing further
                            drift between VTBMs */
  Bal_status_chg_correction  /**< a recent charge estimate is available so
                               balancing has full authority to correct for
                               charge imbalances in the pack */

} Bal_status_e;

/**
 * Debug enum to indicate reason for balancing being halted
 * @note changes to this enum must be mirrored in the ASAP2 COMPU_VTAB
 * in bal.a2l
 */
typedef enum {
    Bal_halted_none,            /**< balancing isn't halted */
    Bal_halted_cm_inhibit,      /**< Cell management doesn't want balancing to
                                  affect voltage readings */
    Bal_halted_v_invalid,       /**< a complete set of cell voltages is
                                  unavailable */
    Bal_halted_t_invalid,       /**< a complete set of cell temperatures is
                                  unavailable */
    Bal_halted_t_min,           /**< the coldest cell is too cold */
    Bal_halted_12v_low,         /**< the 12V-nominal supply voltage is too low*/
    Bal_halted_by_mainstate,    /**< Main state says no balancing, including
                                  because the no-balancing failure function is
                                  active */
    Bal_halted_by_failure_func /**< A failure has occured which inhibits
                                 balancing */
} bal_halted_reason_e;

/** Enum denoting index of estimates. This is used to index the bal span NV 
 * data. Changing the enum values would cause the NV to be incorrectly decoded
 */
enum BalEstPos_e
{
    BalEstLower,
    BalEstUpper,
    /** Counter, not a valid argument */ 
    BalEstCount


};

#endif      /* BAL_TYPES_H */
