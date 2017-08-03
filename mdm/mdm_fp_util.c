/**
 *
 * MDM stand-alone Floating point utilites
 * The functions are not MDM-specific as such, but only MDM makes use of them
 * They apply to the RTW datatypes.
 *
 * There are two sorts of routines in this file:
 * Totally generic floating point checks and checked conversions from floats
 * for MMC data types.
 *
 * @file
 * @ingroup mdm
 * $Id: mdm_fp_util.c 11629 2014-01-20 18:03:58Z pr4 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#include "basic_types.h"
#include "mdm.h"
#include "utils.h"

/******************************************************************************
 * MACROS
 *****************************************************************************/

/** Mask of the float sign bit is given by
 * FP_SIGN_MASK     0x80000000u
 * if needed */
/** Mask of the float exponent bit */
#define FP_EXPONENT_MASK 0x7f800000u
/*8 Mask of the float mantissa field */
#define FP_MANTISSA_MASK 0x007FFFFFu
/** interest mask for the detection of an inf */
#define FP_INF_INTR_MASK (FP_EXPONENT_MASK|FP_MANTISSA_MASK)

/**
 * Minimum used for percent conversiont e.g. SOC */
#define MDM_PCT_MIN 0.0f
/**
 * Max used for percent conversiont e.g. SOC */
#define MDM_PCT_MAX 100.0f

/* Max used for percent conversion of SOH */
#define MDM_PCT_SOH_MAX 200.0f

/**
 * Maximum cell voltage conversion */
#define MDM_CELLV_MAX   5.0f
/**
 * Min converted cell voltage */
#define MDM_CELLV_MIN   0.0f

/**
 * The limit of 125 degrees C is sourced from the cell management interface document */
#define MDM_TEMP_MAX    125.0f
/**
 * The limit of -40 deg C is sourced from the cell management interface document */
#define MDM_TEMP_MIN    -40.0f

/**
 * Maximum current */
#define MDM_CURR_MAX 1600.0f
/**
 * Minimum current
 * @note all the currents output from MDM are *positive* irrespective of
 * whether they are flowing in or out, the flow direction is determined by the
 * variable*/
#define MDM_CURR_MIN 0.0f
/**
 * Maximum pack voltage */
#define MDM_PACKV_MAX 1000.0f
/**
 * Minimum pack voltage */
#define MDM_PACKV_MIN 0.0f

/**
 * Maximum resistance (ohm) */
#define MDM_OHM_MAX 64.0f
/**
 * Minimum resistance (ohm) */
#define MDM_OHM_MIN 0.0f

/**
 * Maximum resistance (milliohm) */
#define MDM_MILLIOHM_MAX 0.064f
/**
 * Minimum resistance (milliohm) */
#define MDM_MILLIOHM_MIN 0.0f

/**
 * Maximum time */
#define MDM_SECONDS_X10_MAX 655360.0f
/**
 * Minimum time */
#define MDM_SECONDS_X10_MIN 0.0f

/**
 * Clip a float to lie within the range specified by the min and max values
 * value passed in must be finite */
#define MDM_FLOAT_CLIP( value_, minval_, maxval_) ( \
    ( (value_) > (maxval_) ) \
    ? (maxval_) \
    : ( ( (value_) < (minval_) ) \
        ? (minval_) \
        : (value_) \
      ) \
)

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

STATIC_ASSERT( MDM_PACKV_MAX<=PACKV_TO_FLOAT(INT16_MAX),
    Overflows_packv);

STATIC_ASSERT( MDM_TEMP_MAX<=TEMP_TO_FLOAT(TEMPERATURE_MAX),
    Overflows_temp_max);

STATIC_ASSERT( MDM_TEMP_MIN>=TEMP_TO_FLOAT(TEMPERATURE_MIN),
    Overflows_temp_min);

STATIC_ASSERT( MDM_CELLV_MAX<=CELLV_TO_FLOAT(CELL_V_MAX),
    Overflows_cellv_max);

STATIC_ASSERT( MDM_CELLV_MIN<=CELLV_TO_FLOAT(0),
    Overflows_cellv_min);

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/
/**
 * Test whether a 32-bit float value is finite
 * @retval TRUE if value is finite
 * @retval FALSE is value is INF or NAN */
bool_t is_finite(real32_T value)
{
    bool_t result;
    uint32_t intval;
    /** @lint 740 Unusual pointer cast */
    /*lint -e{740} */
    intval = *((uint32_t*)&value);
    result = (bool_t)((intval & FP_EXPONENT_MASK) != FP_EXPONENT_MASK);
    return result;
}

/**
 * Test whether a value is non-finite
 * @retval TRUE if value is INF or NAN
 * @retval FALSE otherwise */
bool_t is_not_finite(real32_T value)
{
    bool_t result;
    uint32_t intval;
    /** @lint 740 Unusual pointer cast */
    /*lint -e{740} */
    intval = *((uint32_t*)&value);
    result = (bool_t)((intval & FP_EXPONENT_MASK) == FP_EXPONENT_MASK);
    return result;
}

/**
 * Test whether a value is infinite
 * @retval TRUE if value is INF
 * @retval FALSE otherwise */

bool_t is_infinite(real32_T value)
{
    bool_t result;
    uint32_t intval;
    /** @lint 740 Unusual pointer cast */
    /*lint -e{740} */
    intval = *((uint32_t*)&value);
    result = (bool_t)((intval & FP_INF_INTR_MASK ) == FP_EXPONENT_MASK);
    return result;
}

/**
 * Test whether a value is a NaN
 * @retval TRUE if value is NaN
 * @retval FALSE otherwise */
bool_t is_nan(real32_T value)
{
    bool_t result;
    uint32_t intval;
    /** @lint 740 Unusual pointer cast (incompatible indirect types) */
    /*lint -e{740} */
    intval = *((uint32_t*)&value);
    result = (bool_t)(
           ( (intval & FP_EXPONENT_MASK) == FP_EXPONENT_MASK)
           && ( (intval & FP_MANTISSA_MASK) != 0u )
           );
    return result;
}

/**
 * Clip a floating point nuber to lie in the range of a percent_t */
percent_t mdm_percent_clip(real32_T value)
{
    real32_T clip_val;
    if ( is_not_finite(value) )
    {
        /* Number is a NaN or inf */
        clip_val=MDM_PCT_MIN;
    }
    else
    {
        clip_val=MDM_FLOAT_CLIP(value,MDM_PCT_MIN,MDM_PCT_MAX);
    }
    return PERCENT(clip_val);
}

/**
 * Clip a floating point number representing SOH to fit into a percent_t
 * SOH values can go up to 200% */
percent_t mdm_soh_percent_clip(real32_T value)
{
    real32_T clip_val;
    if ( is_not_finite(value) )
    {
        /* Number is a NaN or inf */
        clip_val=MDM_PCT_MIN;
    }
    else
    {
        clip_val=MDM_FLOAT_CLIP(value,MDM_PCT_MIN,MDM_PCT_SOH_MAX);
    }
    return PERCENT(clip_val);
}

/**
 * Check a real number for validity as a cell_v_t and clip it into range */
cell_v_t mdm_cellv_clip(real32_T value)
{
    real32_T clip_val;
   if ( is_not_finite(value) )
    {
        /* Number is a NaN or inf */
        clip_val=MDM_CELLV_MAX;
    }
    else
    {
        clip_val=MDM_FLOAT_CLIP(value,MDM_CELLV_MIN,MDM_CELLV_MAX);
    }
    return CELL_V(clip_val);
}

/**
 * Check a real number for validity as a temperature_t and clip it into range*/
temperature_t mdm_temp_clip(real32_T value)
{
    real32_T clip_val;
    if ( is_not_finite(value) )
    {
        /* Number is a NaN or inf */
        clip_val=MDM_TEMP_MAX;
    }
    else
    {
        clip_val=MDM_FLOAT_CLIP(value,MDM_TEMP_MIN,MDM_TEMP_MAX);
    }
    return TEMP(clip_val);
}

/**
 * Check a real number for validity as a current_t and clip it into range */
current_t mdm_curr_clip(real32_T value)
{
    real32_T clip_val;
    if ( is_not_finite(value) )
    {
        /* Number is a NaN or inf */
        clip_val=MDM_CURR_MAX;
    }
    else
    {
        clip_val=MDM_FLOAT_CLIP(value,MDM_CURR_MIN,MDM_CURR_MAX);
    }
    return AMPS(clip_val);
}

/**
 * Check a real number for validity as a pack_v_t and clip it into range */
pack_v_t mdm_packv_clip(real32_T value)
{
    real32_T clip_val;
   if ( is_not_finite(value) )
    {
        /* Number is a NaN or inf */
        clip_val=MDM_PACKV_MAX;
    }
    else
    {
        clip_val=MDM_FLOAT_CLIP(value,MDM_PACKV_MIN,MDM_PACKV_MAX);
    }
    return PACK_V(clip_val);
}

/**
 * Check a real number for validity as an ohm_t and clip it into range */
ohm_t mdm_ohm_clip(real32_T value)
{
    real32_T clip_val;
   if ( is_not_finite(value) )
    {
        /* Number is a NaN or inf */
        clip_val=MDM_OHM_MAX;
    }
    else
    {
        clip_val=MDM_FLOAT_CLIP(value,MDM_OHM_MIN,MDM_OHM_MAX);
    }
    return OHM(clip_val);
}

/**
 * Check a real number for validity as an milliohm_t and clip it into range */
milliohm_t mdm_milliohm_clip(real32_T value)
{
    real32_T clip_val;
   if ( is_not_finite(value) )
    {
        /* Number is a NaN or inf */
        clip_val=MDM_MILLIOHM_MAX;
    }
    else
    {
        clip_val=MDM_FLOAT_CLIP(value,MDM_MILLIOHM_MIN,MDM_MILLIOHM_MAX);
    }
    return OHM_TO_MILLIOHM(clip_val);
}

/**
 * Check a real number for validity as a seconds_x10_t and clip it into range */
seconds_x10_t mdm_seconds_x10_clip(real32_T value)
{
    real32_T clip_val;
   if ( is_not_finite(value) )
    {
        /* Number is a NaN or inf */
        clip_val=MDM_SECONDS_X10_MAX;
    }
    else
    {
        clip_val=MDM_FLOAT_CLIP(value,MDM_SECONDS_X10_MIN,MDM_SECONDS_X10_MAX);
    }
    return SECONDS_X10(clip_val);
}

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
