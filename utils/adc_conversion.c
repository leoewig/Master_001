/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name: adc_conversion.c
 *
 * description:   
 * ADC Conversion utility routines
 *
 * 
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/

/******************************************************************************
 * INCLUDES
 *****************************************************************************/


#include "basic_types.h"
#include "mmc.h"
#include "ai_types.h"
#include "utils.h"
#include "vmi_types.h"
#include "adc_conversion.h"
#include "interp.h"

typedef int32_t conv_v_t;
typedef int32_t conv_supply_v_t;

#define CONV_V_BP 8
#define CONV_SUPPLY_V_BP 4
#define CONV_SUPPLY_V(v)     BP_SCALE((v), conv_supply_v_t, CONV_SUPPLY_V_BP) /**< Convert a float into conv_supply_v_t */

PRIVATE conv_supply_v_t vmi_supply_voltages_gain[Vmi_last_supply] =
{
#ifdef RUB_010
    CONV_SUPPLY_V(133.00/33.00), /* ISA */
    CONV_SUPPLY_V( 12.74/ 2.74), /* VTBM CAN 5V */
    CONV_SUPPLY_V( 12.74/ 2.74), /* VTBM CAN GND */
    CONV_SUPPLY_V(133.00/33.00), /* VPOS */
    CONV_SUPPLY_V(  6.00/ 1.00), /* VNEG */
    CONV_SUPPLY_V(  6.00/ 1.00), /* 12V POWER */
    CONV_SUPPLY_V(  6.00/ 1.00), /* 12V LOGIC */
#else
    CONV_SUPPLY_V(133.00/33.00), /* ISA */
    CONV_SUPPLY_V( 24.70/20.00), /* VTBM CAN 5V */
    CONV_SUPPLY_V( 24.70/20.00), /* VTBM CAN GND */
    CONV_SUPPLY_V(133.00/33.00), /* VPOS */
    CONV_SUPPLY_V(  6.00/ 1.00), /* VNEG */
    CONV_SUPPLY_V(  6.00/ 1.00), /* 12V POWER */
    CONV_SUPPLY_V(  6.00/ 1.00), /* 12V LOGIC */
#endif
};

PRIVATE supply_v_t vmi_supply_voltages_offset[Vmi_last_supply] =
{
    SUPPLY_V(  0.0), /* ISA */
    SUPPLY_V(  0.0), /* VTBM CAN 5V */
    SUPPLY_V(  0.0), /* VTBM CAN GND */
    SUPPLY_V(  0.0), /* VPOS */
    SUPPLY_V(-25.0), /* VNEG */
    SUPPLY_V(  0.0), /* 12V POWER */
    SUPPLY_V(  0.75),/* 12V LOGIC */
};

/**
 * Converts a given pack_v_t into the positive and negative voltage required to produce that
 * voltage for the high resolution overall voltage. NB the 2 rails will be
 * balanced
 */
void adc_voltage_split (pack_v_t pack_v_request, pack_v_t *v_pack_pos, pack_v_t *v_pack_neg)
{
    pack_v_t v_pack;
    conv_v_t v_temp;

    v_temp = shift_s32((int32_t) pack_v_request, PACK_V_BP,    CONV_V_BP);
    v_temp = divs32    (v_temp, (int32_t) 2,     CONV_V_BP, 0, CONV_V_BP);
    v_pack = narrow_s16(v_temp,                  CONV_V_BP,    PACK_V_BP);

    *v_pack_pos = v_pack;
    *v_pack_neg = -v_pack;
}

uint16_t adc_supply_v_to_c (enum vmi_supply_e supply, supply_v_t supply_v_request)
{
    supply_v_t vmi_request;
    supply_v_t v_adc;

    uint16_t adc;

    /* v_adc = (vmi_request / gain) - offset */
    vmi_request = (supply_v_t) subs32((int32_t) supply_v_request, (int32_t) vmi_supply_voltages_offset[supply], SUPPLY_V_BP,      SUPPLY_V_BP, SUPPLY_V_BP);
    v_adc       = (supply_v_t) divs32((int32_t) vmi_request,                vmi_supply_voltages_gain  [supply], SUPPLY_V_BP, CONV_SUPPLY_V_BP, SUPPLY_V_BP);

    /* The voltage at the ADC cannot be negative */
    if (v_adc < 0)
    {
        v_adc = 0;
    }

    adc = V_TO_C(BP_TO_FLOAT(v_adc, SUPPLY_V_BP));

    /* Now do some range checking & limiting to valid ADC values */
    if (adc > 4095u)
    {
        adc = 4095u;
    }

    return adc ;
}

/**
 * documentation with prototype in header file
 *
 * NB This routine has been lifted from the MMC code
*/
uint16 adc_conversion_interp_1d_s16_u16 (sint16 u,
                          const FPTR sint16 *uaxis,
                          const FPTR uint16 *yaxis,
                          uint8 n_pts) {

    uint16 y = yaxis[0];

    if (u < uaxis[0]) {
        /* u off bottom of table */
        y = yaxis[0];
    } else if (u >= uaxis[n_pts-1u]) {
        /* u off top of table */
        y = yaxis[n_pts-1u];
    } else {
        /* u within table */
        uint8 i;
        bool done = FALSE;

        /* Note that this i is off by one from the one in the design */
        /**
         * @lint Deviation Required : Prohibited Implicit Conversion, this is trusted code so suppress
         * lint -esym(960, 10.1)
         */
        /*lint -esym(960, 10.1) */
        /** @polyspace<MISRA-C:12.5:Not a defect:No action planned> Trusted existing code, no change planned */
        /** @polyspace<MISRA-C:12.6:Not a defect:No action planned> Construct is Misra compliant */
        /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
        for (i=1; !done && (i < n_pts); i++) {
        /*lint +esym(960, 10.1) */
            /* Valid indices into uaxis[] and yaxis[]: i-1 and i only */
            if ((u == uaxis[i]) &&
                (uaxis[i-1u] == uaxis[i])) {
                /*
                 * Handle steps in table. The result given is the y-value
                 * with the smallest possible index.
                 */
                y = yaxis[i-1u];
                done = TRUE;
            } else if (u < uaxis[i]) {
                /*                                 (   u - u     )
                 *                                 (        i-1  )
                 * Calculate y = y    + (y  - y   )(-------------)
                 *                i-1     i    i-1 (  u  - u     )
                 *                                 (   i    i-1  )
                 *
                 * or y = y_(i-1) + (Dy)(du/Du)
                 *
                 * Division performed last in order to minimise error in
                 * calculation: y_(i-1) + ((Dy)(du))/Du
                 *
                 * Dy stored as separate sign because the range of Dy exceeds
                 * that of a sint16. However, the _absolute_ difference between
                 * two sint16s can be represented in a uint16.
                 */
                uint16 ydiff;   /* "|Dy|" */
                bool ydneg;     /* "sign(Dy)" */
                uint32 numer;   /* numerator "(|Dy|*du)" */
                uint16 frac;    /* product term "(|Dy|*du)/Du" */
		uint16 du, Du;

		if (yaxis[i] >= yaxis[i-1u]) {
                    ydneg = FALSE;
                    /**
                     * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                     * lint -esym(960, 10.3)
                     */
                    /*lint -esym(960, 10.3) */
                    /** @polyspace<MISRA-C:10.3:Not a defect:Justify with annotations> Trusted existing code. no change planned */
                    ydiff = (uint16)(yaxis[i] - yaxis[i-1u]);
                    /*lint +esym(960, 10.3) */
                } else {
                    ydneg = TRUE;
                    /**
                     * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                     * lint -esym(960, 10.3)
                     */
                    /*lint -esym(960, 10.3) */
                    /** @polyspace<MISRA-C:10.3:Not a defect:Justify with annotations> Trusted existing code. no change planned */
                    ydiff = (uint16)(yaxis[i-1u] - yaxis[i]);
                    /*lint +esym(960, 10.3) */
                }

		/* We know that uaxis[i] > u > uaxis[i-1u] so there's no problem
		 * with casting these to unsigned. */
                /**
                 * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                 * lint -esym(960, 10.3)
                 */
                /*lint -esym(960, 10.3) */
                /** @polyspace<MISRA-C:10.3:Not a defect:Justify with annotations> Trusted existing code. no change planned */
		du = (uint16)(u-uaxis[i-1u]);
                /*lint +esym(960, 10.3) */
                /**
                 * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                 * lint -esym(960, 10.3)
                 */
                /*lint -esym(960, 10.3) */
                /** @polyspace<MISRA-C:10.3:Not a defect:Justify with annotations> Trusted existing code. no change planned */
		Du = (uint16)(uaxis[i] - uaxis[i-1u]);
                /**
                 * @lint Deviation Required : Prohibited cast of complex integer expression, this is trusted code so suppress
                 * lint -esym(960, 10.3)
                 */
                /*lint -esym(960, 10.3) */
                /** @polyspace<MISRA-C:10.3:Low:Investigate> Could code be changed to fix this? */
                /* This generates EMUL (u16*u16->u32) instruction */
                numer = (uint32) ydiff * (uint32)du;
                /*lint +esym(960, 10.3) */

                /* u32/u16 division. Cast to uint16 causes compiler to
                 * generate EDIV instruction. */
                /* (We know that frac will fit in a uint16) */
                /**
                 * @lint Deviation Required : Prohibited Implicit Conversion, this is trusted code so suppress
                 * lint -esym(960, 10.1)
                 */
                /*lint -esym(960, 10.1) */
                frac = (uint16)(numer / Du);

                /** @polyspace<MISRA-C:13.2:Not a defect:No action planned> Construct is Misra compliant */
		if (ydneg) {
		    /* If Dy was negative, we know that
		       yaxis[i] < yaxis[i-1u]-frac < yaxis[i-1u], so
		       yaxis[i-1u]-frac will fit into a sint16. */
                    y = yaxis[i-1u] - (uint16)frac;
                } else {
		    /* If Dy was positive, we know that
		       yaxis[i] > yaxis[i-1u]+frac > yaxis[i-1u], so
		       yaxis[i-1u]+frac will fit into a sint16. */
                    y = yaxis[i-1u] + (uint16)frac;
                }
                done = TRUE;
            } else {
                /* else clause can never be reached; no appropriate
                 * action to take */
                ;
            }
        }
    }

    return y;
}

PRIVATE uint16_t lookup_hr_pos_counts [2] =
{
    0,
    4095
};

PRIVATE pack_v_t lookup_hr_pos_volts [2] =
{
    PACK_V(0),
    PACK_V(1007.004)
};


PRIVATE pack_v_t lookup_hr_iso_pos_volts [2] =
{
    PACK_V(0),
    PACK_V(1000.0)
};


/**
 * Pack Voltages corresponding to ADC counts with iso det enabled
 * on vehicle pos rail.
 * other axis is lookup_hr_iso_pos_volts */
PRIVATE uint16_t lookup_hr_iso_pos_counts [2] =
{
    0,
    85
};


/**
 * Converts a given pack_v_t into the ADC counts required to produce that
 * voltage for the high resolution positive rail
 */
uint16_t adc_hi_pos_v_to_c (pack_v_t pack_v_request)
{
    uint16_t adc_pos;

    adc_pos = adc_conversion_interp_1d_s16_u16(pack_v_request, (const FPTR sint16 *)lookup_hr_pos_volts, (const FPTR uint16 *)lookup_hr_pos_counts, 2);

    return adc_pos;
}

uint16_t adc_hi_pos_iso_v_to_c (pack_v_t pack_v_request)
{
    uint16_t adc_pos;

    adc_pos = adc_conversion_interp_1d_s16_u16(pack_v_request, (const FPTR sint16 *)lookup_hr_iso_pos_volts, (const FPTR uint16 *)lookup_hr_iso_pos_counts, 2);

    return adc_pos;
}


uint16_t adc_hi_pos_sw_v_to_c (pack_v_t pack_v_request, bool_t iso_enabled)
{
    if (iso_enabled) {
        return adc_hi_pos_iso_v_to_c(pack_v_request);
    } else {
        return adc_hi_pos_v_to_c(pack_v_request);
    }
}


PRIVATE uint16_t lookup_hr_neg_counts [2] =
{
    0,
    4095
};

PRIVATE pack_v_t lookup_hr_neg_volts [2] =
{
    PACK_V(-1000.0),
    PACK_V(5)
};

/**
 * Converts a given pack_v_t into the ADC counts required to produce that
 * voltage for the high resolution negative rail
 */
uint16_t adc_hi_neg_v_to_c (pack_v_t pack_v_request)
{
    uint16_t adc_neg;

    adc_neg = adc_conversion_interp_1d_s16_u16(pack_v_request, (const FPTR sint16 *)lookup_hr_neg_volts, (const FPTR uint16 *)lookup_hr_neg_counts, 2);

    return adc_neg;
}

/**
 * Converts a given pack_v_t into the ADC counts required to produce that
 * voltage for the high resolution overall voltage. NB the 2 rails will be
 * balanced
 */
void adc_hi_v_to_c (pack_v_t pack_v_request, uint16_t *adc_counts_pos, uint16_t *adc_counts_neg)
{
    pack_v_t v_pack_pos;
    pack_v_t v_pack_neg;

    uint16_t adc_pos;
    uint16_t adc_neg;

    adc_voltage_split (pack_v_request, &v_pack_pos, &v_pack_neg);

    adc_pos = adc_hi_pos_v_to_c(v_pack_pos);
    adc_neg = adc_hi_neg_v_to_c(v_pack_neg);

    *adc_counts_pos = adc_pos;
    *adc_counts_neg = adc_neg;
}

PRIVATE uint16_t lookup_lr_pos_counts [2] =
{
    0,
    4095
};

PRIVATE pack_v_t lookup_lr_pos_volts [2] =
{
    PACK_V(-652.597),
    PACK_V((5 * 260.219) - 652.597)
};

/**
 * Converts a given pack_v_t into the ADC counts required to produce that
 * voltage for the high resolution positive rail
 */
uint16_t adc_lo_pos_v_to_c (pack_v_t pack_v_request)
{
    uint16_t adc_pos;

    adc_pos = adc_conversion_interp_1d_s16_u16(pack_v_request, (const FPTR sint16 *)lookup_lr_pos_volts, (const FPTR uint16 *)lookup_lr_pos_counts, 2);

    return adc_pos;
}

PRIVATE uint16_t lookup_lr_neg_counts [2] =
{
    0,
    4095
};

PRIVATE pack_v_t lookup_lr_neg_volts [2] =
{
    PACK_V(-656.677),
    PACK_V((5 * 262.985) - 656.677)
};

/**
 * Converts a given pack_v_t into the ADC counts required to produce that
 * voltage for the high resolution negative rail
 */
uint16_t adc_lo_neg_v_to_c (pack_v_t pack_v_request)
{
    uint16_t adc_neg;

    adc_neg = adc_conversion_interp_1d_s16_u16(pack_v_request, (const FPTR sint16 *)lookup_lr_neg_volts, (const FPTR uint16 *)lookup_lr_neg_counts, 2);

    return adc_neg;
}

/**
 * Converts a given pack_v_t into the ADC counts required to produce that
 * voltage for the high resolution overall voltage. NB the 2 rails will be
 * balanced
 */
void adc_lo_v_to_c (pack_v_t pack_v_request, uint16_t *adc_counts_pos, uint16_t *adc_counts_neg)
{
    pack_v_t v_pack_pos;
    pack_v_t v_pack_neg;

    uint16_t adc_pos;
    uint16_t adc_neg;

    adc_voltage_split (pack_v_request, &v_pack_pos, &v_pack_neg);

    adc_pos = adc_lo_pos_v_to_c(v_pack_pos);
    adc_neg = adc_lo_neg_v_to_c(v_pack_neg);

    *adc_counts_pos = adc_pos;
    *adc_counts_neg = adc_neg;
}

/*****************************************************************
 * Temperature Conversion Routines
 ****************************************************************/

/*****************************************************************************
 * static data
 *****************************************************************************/

/** From the HW/SW Specification NB This should be the same table as used by the TMI feature */
CONST temperature_t lookup_bosch_temperature[LOOKUP_SIZE_BOSCH]=
{
    TEMP(-50),
    TEMP(-49),
    TEMP(-48),
    TEMP(-47),
    TEMP(-46),
    TEMP(-45),
    TEMP(-44),
    TEMP(-43),
    TEMP(-42),
    TEMP(-41),
    TEMP(-40),
    TEMP(-39),
    TEMP(-38),
    TEMP(-37),
    TEMP(-36),
    TEMP(-35),
    TEMP(-34),
    TEMP(-33),
    TEMP(-32),
    TEMP(-31),
    TEMP(-30),
    TEMP(-29),
    TEMP(-28),
    TEMP(-27),
    TEMP(-26),
    TEMP(-25),
    TEMP(-24),
    TEMP(-23),
    TEMP(-22),
    TEMP(-21),
    TEMP(-20),
    TEMP(-19),
    TEMP(-18),
    TEMP(-17),
    TEMP(-16),
    TEMP(-15),
    TEMP(-14),
    TEMP(-13),
    TEMP(-12),
    TEMP(-11),
    TEMP(-10),
    TEMP( -9),
    TEMP( -8),
    TEMP( -7),
    TEMP( -6),
    TEMP( -5),
    TEMP( -4),
    TEMP( -3),
    TEMP( -2),
    TEMP( -1),
    TEMP(  0),
    TEMP(  1),
    TEMP(  2),
    TEMP(  3),
    TEMP(  4),
    TEMP(  5),
    TEMP(  6),
    TEMP(  7),
    TEMP(  8),
    TEMP(  9),
    TEMP( 10),
    TEMP( 11),
    TEMP( 12),
    TEMP( 13),
    TEMP( 14),
    TEMP( 15),
    TEMP( 16),
    TEMP( 17),
    TEMP( 18),
    TEMP( 19),
    TEMP( 20),
    TEMP( 21),
    TEMP( 22),
    TEMP( 23),
    TEMP( 24),
    TEMP( 25),
    TEMP( 26),
    TEMP( 27),
    TEMP( 28),
    TEMP( 29),
    TEMP( 30),
    TEMP( 31),
    TEMP( 32),
    TEMP( 33),
    TEMP( 34),
    TEMP( 35),
    TEMP( 36),
    TEMP( 37),
    TEMP( 38),
    TEMP( 39),
    TEMP( 40),
    TEMP( 41),
    TEMP( 42),
    TEMP( 43),
    TEMP( 44),
    TEMP( 45),
    TEMP( 46),
    TEMP( 47),
    TEMP( 48),
    TEMP( 49),
    TEMP( 50),
    TEMP( 51),
    TEMP( 52),
    TEMP( 53),
    TEMP( 54),
    TEMP( 55),
    TEMP( 56),
    TEMP( 57),
    TEMP( 58),
    TEMP( 59),
    TEMP( 60),
    TEMP( 61),
    TEMP( 62),
    TEMP( 63),
    TEMP( 64),
    TEMP( 65),
    TEMP( 66),
    TEMP( 67),
    TEMP( 68),
    TEMP( 69),
    TEMP( 70),
    TEMP( 71),
    TEMP( 72),
    TEMP( 73),
    TEMP( 74),
    TEMP( 75),
    TEMP( 76),
    TEMP( 77),
    TEMP( 78),
    TEMP( 79),
    TEMP( 80),
    TEMP( 81),
    TEMP( 82),
    TEMP( 83),
    TEMP( 84),
    TEMP( 85),
    TEMP( 86),
    TEMP( 87),
    TEMP( 88),
    TEMP( 89),
    TEMP( 90),
    TEMP( 91),
    TEMP( 92),
    TEMP( 93),
    TEMP( 94),
    TEMP( 95),
    TEMP( 96),
    TEMP( 97),
    TEMP( 98),
    TEMP( 99),
    TEMP(100),
    TEMP(101),
    TEMP(102),
    TEMP(103),
    TEMP(104),
    TEMP(105),
    TEMP(106),
    TEMP(107),
    TEMP(108),
    TEMP(109),
    TEMP(110),
    TEMP(111),
    TEMP(112),
    TEMP(113),
    TEMP(114),
    TEMP(115),
    TEMP(116),
    TEMP(117),
    TEMP(118),
    TEMP(119),
    TEMP(120),
    TEMP(121),
    TEMP(122),
    TEMP(123),
    TEMP(124),
    TEMP(125),
};

/** From the HW/SW Specification NB This should be the same table as used by the TMI feature */
CONST uint16_t lookup_bosch_counts[LOOKUP_SIZE_BOSCH]=
{
    4002,
    3996,
    3990,
    3984,
    3977,
    3969,
    3962,
    3954,
    3945,
    3936,
    3927,
    3917,
    3907,
    3897,
    3885,
    3874,
    3861,
    3849,
    3835,
    3822,
    3807,
    3792,
    3776,
    3760,
    3743,
    3725,
    3707,
    3688,
    3669,
    3648,
    3627,
    3605,
    3583,
    3560,
    3536,
    3511,
    3486,
    3460,
    3433,
    3405,
    3377,
    3348,
    3318,
    3288,
    3257,
    3225,
    3193,
    3160,
    3126,
    3091,
    3057,
    3021,
    2985,
    2948,
    2911,
    2874,
    2836,
    2798,
    2759,
    2720,
    2680,
    2641,
    2601,
    2561,
    2520,
    2480,
    2439,
    2398,
    2358,
    2317,
    2276,
    2236,
    2195,
    2155,
    2114,
    2074,
    2034,
    1995,
    1955,
    1916,
    1878,
    1839,
    1801,
    1764,
    1726,
    1690,
    1653,
    1617,
    1582,
    1547,
    1512,
    1479,
    1445,
    1412,
    1380,
    1348,
    1317,
    1286,
    1256,
    1227,
    1198,
    1169,
    1141,
    1114,
    1087,
    1061,
    1035,
    1010,
     986,
     962,
     938,
     915,
     893,
     871,
     850,
     829,
     808,
     788,
     769,
     750,
     732,
     714,
     696,
     679,
     662,
     646,
     630,
     614,
     599,
     584,
     570,
     556,
     543,
     529,
     516,
     504,
     491,
     480,
     468,
     457,
     445,
     435,
     424,
     414,
     404,
     395,
     385,
     376,
     367,
     358,
     350,
     342,
     334,
     326,
     318,
     311,
     304,
     297,
     290,
     283,
     277,
     270,
     264,
     258,
     252,
     247,
     241,
     236,
     230,
     225,
     220,
     215,
     211,
     206,
     201,
     197,
};

/** From the HW/SW Specification NB This should be the same table as used by the TMI feature */
CONST temperature_t lookup_bosch_temperature_rub10[LOOKUP_SIZE_BOSCH]=
{
    TEMP(-50),
    TEMP(-49),
    TEMP(-48),
    TEMP(-47),
    TEMP(-45),
    TEMP(-46),
    TEMP(-44),
    TEMP(-43),
    TEMP(-42),
    TEMP(-41),
    TEMP(-40),
    TEMP(-39),
    TEMP(-38),
    TEMP(-37),
    TEMP(-36),
    TEMP(-35),
    TEMP(-34),
    TEMP(-33),
    TEMP(-32),
    TEMP(-31),
    TEMP(-30),
    TEMP(-29),
    TEMP(-28),
    TEMP(-27),
    TEMP(-26),
    TEMP(-25),
    TEMP(-24),
    TEMP(-23),
    TEMP(-22),
    TEMP(-21),
    TEMP(-20),
    TEMP(-19),
    TEMP(-18),
    TEMP(-17),
    TEMP(-16),
    TEMP(-15),
    TEMP(-14),
    TEMP(-13),
    TEMP(-12),
    TEMP(-11),
    TEMP(-10),
    TEMP( -9),
    TEMP( -8),
    TEMP( -7),
    TEMP( -6),
    TEMP( -5),
    TEMP( -4),
    TEMP( -3),
    TEMP( -2),
    TEMP( -1),
    TEMP(  0),
    TEMP(  1),
    TEMP(  2),
    TEMP(  3),
    TEMP(  4),
    TEMP(  5),
    TEMP(  6),
    TEMP(  7),
    TEMP(  8),
    TEMP(  9),
    TEMP( 10),
    TEMP( 11),
    TEMP( 12),
    TEMP( 13),
    TEMP( 14),
    TEMP( 15),
    TEMP( 16),
    TEMP( 17),
    TEMP( 18),
    TEMP( 19),
    TEMP( 20),
    TEMP( 21),
    TEMP( 22),
    TEMP( 23),
    TEMP( 24),
    TEMP( 25),
    TEMP( 26),
    TEMP( 27),
    TEMP( 28),
    TEMP( 29),
    TEMP( 30),
    TEMP( 31),
    TEMP( 32),
    TEMP( 33),
    TEMP( 34),
    TEMP( 35),
    TEMP( 36),
    TEMP( 37),
    TEMP( 38),
    TEMP( 39),
    TEMP( 40),
    TEMP( 41),
    TEMP( 42),
    TEMP( 43),
    TEMP( 44),
    TEMP( 45),
    TEMP( 46),
    TEMP( 47),
    TEMP( 48),
    TEMP( 49),
    TEMP( 50),
    TEMP( 51),
    TEMP( 52),
    TEMP( 53),
    TEMP( 54),
    TEMP( 55),
    TEMP( 56),
    TEMP( 57),
    TEMP( 58),
    TEMP( 59),
    TEMP( 60),
    TEMP( 61),
    TEMP( 62),
    TEMP( 63),
    TEMP( 64),
    TEMP( 65),
    TEMP( 66),
    TEMP( 67),
    TEMP( 68),
    TEMP( 69),
    TEMP( 70),
    TEMP( 71),
    TEMP( 72),
    TEMP( 73),
    TEMP( 74),
    TEMP( 75),
    TEMP( 76),
    TEMP( 77),
    TEMP( 78),
    TEMP( 79),
    TEMP( 80),
    TEMP( 81),
    TEMP( 82),
    TEMP( 83),
    TEMP( 84),
    TEMP( 85),
    TEMP( 86),
    TEMP( 87),
    TEMP( 88),
    TEMP( 89),
    TEMP( 90),
    TEMP( 91),
    TEMP( 92),
    TEMP( 93),
    TEMP( 94),
    TEMP( 95),
    TEMP( 96),
    TEMP( 97),
    TEMP( 98),
    TEMP( 99),
    TEMP(100),
    TEMP(101),
    TEMP(102),
    TEMP(103),
    TEMP(104),
    TEMP(105),
    TEMP(106),
    TEMP(107),
    TEMP(108),
    TEMP(109),
    TEMP(110),
    TEMP(111),
    TEMP(112),
    TEMP(113),
    TEMP(114),
    TEMP(115),
    TEMP(116),
    TEMP(117),
    TEMP(118),
    TEMP(119),
    TEMP(120),
    TEMP(122),
    TEMP(121),
    TEMP(123),
    TEMP(124),
    TEMP(125),
};

/** From the HW/SW Specification NB This should be the same table as used by the TMI feature */
CONST uint16_t lookup_bosch_counts_rub10[LOOKUP_SIZE_BOSCH]=
{
 /* ADC     Temperature  */
    623u,   /* -50       */
    622u,   /* -49       */
    621u,   /* -48       */
    620u,   /* -47       */
    619u,   /* -45       */
    619u,   /* -46       */
    617u,   /* -44       */
    616u,   /* -43       */
    615u,   /* -42       */
    614u,   /* -41       */
    613u,   /* -40       */
    611u,   /* -39       */
    610u,   /* -38       */
    608u,   /* -37       */
    607u,   /* -36       */
    605u,   /* -35       */
    603u,   /* -34       */
    602u,   /* -33       */
    600u,   /* -32       */
    598u,   /* -31       */
    596u,   /* -30       */
    594u,   /* -29       */
    592u,   /* -28       */
    589u,   /* -27       */
    587u,   /* -26       */
    584u,   /* -25       */
    582u,   /* -24       */
    579u,   /* -23       */
    576u,   /* -22       */
    573u,   /* -21       */
    571u,   /* -20       */
    567u,   /* -19       */
    564u,   /* -18       */
    561u,   /* -17       */
    557u,   /* -16       */
    554u,   /* -15       */
    550u,   /* -14       */
    547u,   /* -13       */
    543u,   /* -12       */
    539u,   /* -11       */
    535u,   /* -10       */
    531u,   /*  -9       */
    526u,   /*  -8       */
    522u,   /*  -7       */
    518u,   /*  -6       */
    513u,   /*  -5       */
    508u,   /*  -4       */
    503u,   /*  -3       */
    499u,   /*  -2       */
    493u,   /*  -1       */
    488u,   /*   0       */
    483u,   /*   1       */
    478u,   /*   2       */
    473u,   /*   3       */
    467u,   /*   4       */
    462u,   /*   5       */
    456u,   /*   6       */
    450u,   /*   7       */
    444u,   /*   8       */
    439u,   /*   9       */
    433u,   /*  10       */
    427u,   /*  11       */
    421u,   /*  12       */
    415u,   /*  13       */
    409u,   /*  14       */
    403u,   /*  15       */
    397u,   /*  16       */
    390u,   /*  17       */
    384u,   /*  18       */
    378u,   /*  19       */
    372u,   /*  20       */
    366u,   /*  21       */
    359u,   /*  22       */
    353u,   /*  23       */
    347u,   /*  24       */
    341u,   /*  25       */
    335u,   /*  26       */
    328u,   /*  27       */
    322u,   /*  28       */
    316u,   /*  29       */
    310u,   /*  30       */
    304u,   /*  31       */
    298u,   /*  32       */
    292u,   /*  33       */
    286u,   /*  34       */
    281u,   /*  35       */
    275u,   /*  36       */
    269u,   /*  37       */
    264u,   /*  38       */
    258u,   /*  39       */
    252u,   /*  40       */
    247u,   /*  41       */
    242u,   /*  42       */
    237u,   /*  43       */
    231u,   /*  44       */
    226u,   /*  45       */
    221u,   /*  46       */
    216u,   /*  47       */
    211u,   /*  48       */
    207u,   /*  49       */
    202u,   /*  50       */
    197u,   /*  51       */
    193u,   /*  52       */
    188u,   /*  53       */
    184u,   /*  54       */
    180u,   /*  55       */
    175u,   /*  56       */
    171u,   /*  57       */
    167u,   /*  58       */
    163u,   /*  59       */
    159u,   /*  60       */
    156u,   /*  61       */
    152u,   /*  62       */
    148u,   /*  63       */
    145u,   /*  64       */
    141u,   /*  65       */
    138u,   /*  66       */
    134u,   /*  67       */
    131u,   /*  68       */
    128u,   /*  69       */
    125u,   /*  70       */
    122u,   /*  71       */
    119u,   /*  72       */
    116u,   /*  73       */
    113u,   /*  74       */
    111u,   /*  75       */
    108u,   /*  76       */
    105u,   /*  77       */
    103u,   /*  78       */
    100u,   /*  79       */
     98u,   /*  80       */
     95u,   /*  81       */
     93u,   /*  82       */
     91u,   /*  83       */
     89u,   /*  84       */
     87u,   /*  85       */
     85u,   /*  86       */
     83u,   /*  87       */
     81u,   /*  88       */
     79u,   /*  89       */
     77u,   /*  90       */
     75u,   /*  91       */
     73u,   /*  92       */
     71u,   /*  93       */
     70u,   /*  94       */
     68u,   /*  95       */
     67u,   /*  96       */
     65u,   /*  97       */
     63u,   /*  98       */
     62u,   /*  99       */
     60u,   /* 100       */
     59u,   /* 101       */
     58u,   /* 102       */
     56u,   /* 103       */
     55u,   /* 104       */
     54u,   /* 105       */
     52u,   /* 106       */
     51u,   /* 107       */
     50u,   /* 108       */
     49u,   /* 109       */
     48u,   /* 110       */
     47u,   /* 111       */
     46u,   /* 112       */
     45u,   /* 113       */
     44u,   /* 114       */
     43u,   /* 115       */
     42u,   /* 116       */
     41u,   /* 117       */
     40u,   /* 118       */
     39u,   /* 119       */
     38u,   /* 120       */
     37u,   /* 122       */
     37u,   /* 121       */
     36u,   /* 123       */
     35u,   /* 124       */
     34u,   /* 125       */
};

/** From the HW/SW Specification NB This should be the same table as used by the TMI feature */
CONST temperature_t lookup_pt1000_temperature[LOOKUP_SIZE_PT]=
{
    TEMP(-40.0),
    TEMP(-39.0),
    TEMP(-38.0),
    TEMP(-37.0),
    TEMP(-36.0),
    TEMP(-35.0),
    TEMP(-34.0),
    TEMP(-33.0),
    TEMP(-32.0),
    TEMP(-31.0),
    TEMP(-30.0),
    TEMP(-29.0),
    TEMP(-28.0),
    TEMP(-27.0),
    TEMP(-26.0),
    TEMP(-25.0),
    TEMP(-24.0),
    TEMP(-23.0),
    TEMP(-22.0),
    TEMP(-21.0),
    TEMP(-20.0),
    TEMP(-19.0),
    TEMP(-18.0),
    TEMP(-17.0),
    TEMP(-16.0),
    TEMP(-15.0),
    TEMP(-14.0),
    TEMP(-13.0),
    TEMP(-12.0),
    TEMP(-11.0),
    TEMP(-10.0),
    TEMP( -9.0),
    TEMP( -8.0),
    TEMP( -7.0),
    TEMP( -6.0),
    TEMP( -5.0),
    TEMP( -4.0),
    TEMP( -3.0),
    TEMP( -2.0),
    TEMP( -1.0),
    TEMP(  0.0),
    TEMP(  1.0),
    TEMP(  2.0),
    TEMP(  3.0),
    TEMP(  4.0),
    TEMP(  5.0),
    TEMP(  6.0),
    TEMP(  7.0),
    TEMP(  8.0),
    TEMP(  9.0),
    TEMP( 10.0),
    TEMP( 11.0),
    TEMP( 12.0),
    TEMP( 13.0),
    TEMP( 14.0),
    TEMP( 15.0),
    TEMP( 16.0),
    TEMP( 17.0),
    TEMP( 18.0),
    TEMP( 19.0),
    TEMP( 20.0),
    TEMP( 21.0),
    TEMP( 22.0),
    TEMP( 23.0),
    TEMP( 24.0),
    TEMP( 25.0),
    TEMP( 26.0),
    TEMP( 27.0),
    TEMP( 28.0),
    TEMP( 29.0),
    TEMP( 30.0),
    TEMP( 31.0),
    TEMP( 32.0),
    TEMP( 33.0),
    TEMP( 34.0),
    TEMP( 35.0),
    TEMP( 36.0),
    TEMP( 37.0),
    TEMP( 38.0),
    TEMP( 39.0),
    TEMP( 40.0),
    TEMP( 41.0),
    TEMP( 42.0),
    TEMP( 43.0),
    TEMP( 44.0),
    TEMP( 45.0),
    TEMP( 46.0),
    TEMP( 47.0),
    TEMP( 48.0),
    TEMP( 49.0),
    TEMP( 50.0),
    TEMP( 51.0),
    TEMP( 52.0),
    TEMP( 53.0),
    TEMP( 54.0),
    TEMP( 55.0),
    TEMP( 56.0),
    TEMP( 57.0),
    TEMP( 58.0),
    TEMP( 59.0),
    TEMP( 60.0),
    TEMP( 61.0),
    TEMP( 62.0),
    TEMP( 63.0),
    TEMP( 64.0),
    TEMP( 65.0),
    TEMP( 66.0),
    TEMP( 67.0),
    TEMP( 68.0),
    TEMP( 69.0),
    TEMP( 70.0),
    TEMP( 71.0),
    TEMP( 72.0),
    TEMP( 73.0),
    TEMP( 74.0),
    TEMP( 75.0),
    TEMP( 76.0),
    TEMP( 77.0),
    TEMP( 78.0),
    TEMP( 79.0),
    TEMP( 80.0),
    TEMP( 81.0),
    TEMP( 82.0),
    TEMP( 83.0),
    TEMP( 84.0),
    TEMP( 85.0),
};

/** From the HW/SW Specification NB This should be the same table as used by the TMI feature */
CONST uint16_t lookup_pt1000_counts[LOOKUP_SIZE_PT]=
{
 /* ADC     Voltage    Temperature  */
    V_TO_C(1.770), /*  -40.0 */
    V_TO_C(1.779), /*  -39.0 */
    V_TO_C(1.787), /*  -38.0 */
    V_TO_C(1.795), /*  -37.0 */
    V_TO_C(1.804), /*  -36.0 */
    V_TO_C(1.812), /*  -35.0 */
    V_TO_C(1.820), /*  -34.0 */
    V_TO_C(1.829), /*  -33.0 */
    V_TO_C(1.837), /*  -32.0 */
    V_TO_C(1.845), /*  -31.0 */
    V_TO_C(1.853), /*  -30.0 */
    V_TO_C(1.862), /*  -29.0 */
    V_TO_C(1.870), /*  -28.0 */
    V_TO_C(1.878), /*  -27.0 */
    V_TO_C(1.887), /*  -26.0 */
    V_TO_C(1.895), /*  -25.0 */
    V_TO_C(1.903), /*  -24.0 */
    V_TO_C(1.911), /*  -23.0 */
    V_TO_C(1.920), /*  -22.0 */
    V_TO_C(1.928), /*  -21.0 */
    V_TO_C(1.936), /*  -20.0 */
    V_TO_C(1.944), /*  -19.0 */
    V_TO_C(1.953), /*  -18.0 */
    V_TO_C(1.961), /*  -17.0 */
    V_TO_C(1.969), /*  -16.0 */
    V_TO_C(1.977), /*  -15.0 */
    V_TO_C(1.986), /*  -14.0 */
    V_TO_C(1.994), /*  -13.0 */
    V_TO_C(2.002), /*  -12.0 */
    V_TO_C(2.010), /*  -11.0 */
    V_TO_C(2.019), /*  -10.0 */
    V_TO_C(2.027), /*   -9.0 */
    V_TO_C(2.035), /*   -8.0 */
    V_TO_C(2.043), /*   -7.0 */
    V_TO_C(2.051), /*   -6.0 */
    V_TO_C(2.060), /*   -5.0 */
    V_TO_C(2.068), /*   -4.0 */
    V_TO_C(2.076), /*   -3.0 */
    V_TO_C(2.084), /*   -2.0 */
    V_TO_C(2.093), /*   -1.0 */
    V_TO_C(2.101), /*    0.0 */
    V_TO_C(2.109), /*    1.0 */
    V_TO_C(2.117), /*    2.0 */
    V_TO_C(2.125), /*    3.0 */
    V_TO_C(2.134), /*    4.0 */
    V_TO_C(2.142), /*    5.0 */
    V_TO_C(2.150), /*    6.0 */
    V_TO_C(2.158), /*    7.0 */
    V_TO_C(2.166), /*    8.0 */
    V_TO_C(2.175), /*    9.0 */
    V_TO_C(2.183), /*   10.0 */
    V_TO_C(2.191), /*   11.0 */
    V_TO_C(2.199), /*   12.0 */
    V_TO_C(2.207), /*   13.0 */
    V_TO_C(2.216), /*   14.0 */
    V_TO_C(2.224), /*   15.0 */
    V_TO_C(2.232), /*   16.0 */
    V_TO_C(2.240), /*   17.0 */
    V_TO_C(2.248), /*   18.0 */
    V_TO_C(2.256), /*   19.0 */
    V_TO_C(2.264), /*   20.0 */
    V_TO_C(2.273), /*   21.0 */
    V_TO_C(2.281), /*   22.0 */
    V_TO_C(2.289), /*   23.0 */
    V_TO_C(2.297), /*   24.0 */
    V_TO_C(2.305), /*   25.0 */
    V_TO_C(2.313), /*   26.0 */
    V_TO_C(2.322), /*   27.0 */
    V_TO_C(2.330), /*   28.0 */
    V_TO_C(2.338), /*   29.0 */
    V_TO_C(2.346), /*   30.0 */
    V_TO_C(2.354), /*   31.0 */
    V_TO_C(2.362), /*   32.0 */
    V_TO_C(2.370), /*   33.0 */
    V_TO_C(2.379), /*   34.0 */
    V_TO_C(2.387), /*   35.0 */
    V_TO_C(2.395), /*   36.0 */
    V_TO_C(2.403), /*   37.0 */
    V_TO_C(2.411), /*   38.0 */
    V_TO_C(2.419), /*   39.0 */
    V_TO_C(2.427), /*   40.0 */
    V_TO_C(2.435), /*   41.0 */
    V_TO_C(2.443), /*   42.0 */
    V_TO_C(2.452), /*   43.0 */
    V_TO_C(2.460), /*   44.0 */
    V_TO_C(2.468), /*   45.0 */
    V_TO_C(2.476), /*   46.0 */
    V_TO_C(2.484), /*   47.0 */
    V_TO_C(2.492), /*   48.0 */
    V_TO_C(2.500), /*   49.0 */
    V_TO_C(2.508), /*   50.0 */
    V_TO_C(2.516), /*   51.0 */
    V_TO_C(2.525), /*   52.0 */
    V_TO_C(2.533), /*   53.0 */
    V_TO_C(2.541), /*   54.0 */
    V_TO_C(2.549), /*   55.0 */
    V_TO_C(2.557), /*   56.0 */
    V_TO_C(2.565), /*   57.0 */
    V_TO_C(2.573), /*   58.0 */
    V_TO_C(2.581), /*   59.0 */
    V_TO_C(2.589), /*   60.0 */
    V_TO_C(2.597), /*   61.0 */
    V_TO_C(2.605), /*   62.0 */
    V_TO_C(2.613), /*   63.0 */
    V_TO_C(2.621), /*   64.0 */
    V_TO_C(2.629), /*   65.0 */
    V_TO_C(2.637), /*   66.0 */
    V_TO_C(2.646), /*   67.0 */
    V_TO_C(2.654), /*   68.0 */
    V_TO_C(2.662), /*   69.0 */
    V_TO_C(2.670), /*   70.0 */
    V_TO_C(2.678), /*   71.0 */
    V_TO_C(2.686), /*   72.0 */
    V_TO_C(2.694), /*   73.0 */
    V_TO_C(2.702), /*   74.0 */
    V_TO_C(2.710), /*   75.0 */
    V_TO_C(2.718), /*   76.0 */
    V_TO_C(2.726), /*   77.0 */
    V_TO_C(2.734), /*   78.0 */
    V_TO_C(2.742), /*   79.0 */
    V_TO_C(2.750), /*   80.0 */
    V_TO_C(2.758), /*   81.0 */
    V_TO_C(2.766), /*   82.0 */
    V_TO_C(2.774), /*   83.0 */
    V_TO_C(2.782), /*   84.0 */
    V_TO_C(2.790), /*   85.0 */
};

/** From the HW/SW Specification NB This should be the same table as used by the TMI feature */
CONST temperature_t lookup_ncp_temperature[LOOKUP_SIZE_NCP]=
{
    TEMP(-40.0),
    TEMP(-39.0),
    TEMP(-38.0),
    TEMP(-37.0),
    TEMP(-36.0),
    TEMP(-35.0),
    TEMP(-34.0),
    TEMP(-33.0),
    TEMP(-32.0),
    TEMP(-31.0),
    TEMP(-30.0),
    TEMP(-29.0),
    TEMP(-28.0),
    TEMP(-27.0),
    TEMP(-26.0),
    TEMP(-25.0),
    TEMP(-24.0),
    TEMP(-23.0),
    TEMP(-22.0),
    TEMP(-21.0),
    TEMP(-20.0),
    TEMP(-19.0),
    TEMP(-18.0),
    TEMP(-17.0),
    TEMP(-16.0),
    TEMP(-15.0),
    TEMP(-14.0),
    TEMP(-13.0),
    TEMP(-12.0),
    TEMP(-11.0),
    TEMP(-10.0),
    TEMP( -9.0),
    TEMP( -8.0),
    TEMP( -7.0),
    TEMP( -6.0),
    TEMP( -5.0),
    TEMP( -4.0),
    TEMP( -3.0),
    TEMP( -2.0),
    TEMP( -1.0),
    TEMP(  0.0),
    TEMP(  1.0),
    TEMP(  2.0),
    TEMP(  3.0),
    TEMP(  4.0),
    TEMP(  5.0),
    TEMP(  6.0),
    TEMP(  7.0),
    TEMP(  8.0),
    TEMP(  9.0),
    TEMP( 10.0),
    TEMP( 11.0),
    TEMP( 12.0),
    TEMP( 13.0),
    TEMP( 14.0),
    TEMP( 15.0),
    TEMP( 16.0),
    TEMP( 17.0),
    TEMP( 18.0),
    TEMP( 19.0),
    TEMP( 20.0),
    TEMP( 21.0),
    TEMP( 22.0),
    TEMP( 23.0),
    TEMP( 24.0),
    TEMP( 25.0),
    TEMP( 26.0),
    TEMP( 27.0),
    TEMP( 28.0),
    TEMP( 29.0),
    TEMP( 30.0),
    TEMP( 31.0),
    TEMP( 32.0),
    TEMP( 33.0),
    TEMP( 34.0),
    TEMP( 35.0),
    TEMP( 36.0),
    TEMP( 37.0),
    TEMP( 38.0),
    TEMP( 39.0),
    TEMP( 40.0),
    TEMP( 41.0),
    TEMP( 42.0),
    TEMP( 43.0),
    TEMP( 44.0),
    TEMP( 45.0),
    TEMP( 46.0),
    TEMP( 47.0),
    TEMP( 48.0),
    TEMP( 49.0),
    TEMP( 50.0),
    TEMP( 51.0),
    TEMP( 52.0),
    TEMP( 53.0),
    TEMP( 54.0),
    TEMP( 55.0),
    TEMP( 56.0),
    TEMP( 57.0),
    TEMP( 58.0),
    TEMP( 59.0),
    TEMP( 60.0),
    TEMP( 61.0),
    TEMP( 62.0),
    TEMP( 63.0),
    TEMP( 64.0),
    TEMP( 65.0),
    TEMP( 66.0),
    TEMP( 67.0),
    TEMP( 68.0),
    TEMP( 69.0),
    TEMP( 70.0),
    TEMP( 71.0),
    TEMP( 72.0),
    TEMP( 73.0),
    TEMP( 74.0),
    TEMP( 75.0),
    TEMP( 76.0),
    TEMP( 77.0),
    TEMP( 78.0),
    TEMP( 79.0),
    TEMP( 80.0),
    TEMP( 81.0),
    TEMP( 82.0),
    TEMP( 83.0),
    TEMP( 84.0),
    TEMP( 85.0),
};

/** From the HW/SW Specification NB This should be the same table as used by the TMI feature */
CONST uint16_t lookup_ncp_counts[LOOKUP_SIZE_NCP]=
{
   V_TO_C(4.691), /* 40.0 */
   V_TO_C(4.671), /* 39.0 */
   V_TO_C(4.650), /* 38.0 */
   V_TO_C(4.628), /* 37.0 */
   V_TO_C(4.605), /* 36.0 */
   V_TO_C(4.581), /* 35.0 */
   V_TO_C(4.555), /* 34.0 */
   V_TO_C(4.529), /* 33.0 */
   V_TO_C(4.501), /* 32.0 */
   V_TO_C(4.473), /* 31.0 */
   V_TO_C(4.443), /* 30.0 */
   V_TO_C(4.412), /* 29.0 */
   V_TO_C(4.379), /* 28.0 */
   V_TO_C(4.346), /* 27.0 */
   V_TO_C(4.311), /* 26.0 */
   V_TO_C(4.274), /* 25.0 */
   V_TO_C(4.237), /* 24.0 */
   V_TO_C(4.198), /* 23.0 */
   V_TO_C(4.159), /* 22.0 */
   V_TO_C(4.117), /* 21.0 */
   V_TO_C(4.075), /* 20.0 */
   V_TO_C(4.032), /* 19.0 */
   V_TO_C(3.987), /* 18.0 */
   V_TO_C(3.941), /* 17.0 */
   V_TO_C(3.894), /* 16.0 */
   V_TO_C(3.846), /* 15.0 */
   V_TO_C(3.797), /* 14.0 */
   V_TO_C(3.747), /* 13.0 */
   V_TO_C(3.696), /* 12.0 */
   V_TO_C(3.643), /* 11.0 */
   V_TO_C(3.591), /* 10.0 */
   V_TO_C(3.537), /* -9.0 */
   V_TO_C(3.482), /* -8.0 */
   V_TO_C(3.427), /* -7.0 */
   V_TO_C(3.371), /* -6.0 */
   V_TO_C(3.315), /* -5.0 */
   V_TO_C(3.258), /* -4.0 */
   V_TO_C(3.200), /* -3.0 */
   V_TO_C(3.143), /* -2.0 */
   V_TO_C(3.085), /* -1.0 */
   V_TO_C(3.026), /*  0.0 */
   V_TO_C(2.968), /*  1.0 */
   V_TO_C(2.909), /*  2.0 */
   V_TO_C(2.850), /*  3.0 */
   V_TO_C(2.792), /*  4.0 */
   V_TO_C(2.733), /*  5.0 */
   V_TO_C(2.675), /*  6.0 */
   V_TO_C(2.617), /*  7.0 */
   V_TO_C(2.559), /*  8.0 */
   V_TO_C(2.501), /*  9.0 */
   V_TO_C(2.444), /* 10.0 */
   V_TO_C(2.388), /* 11.0 */
   V_TO_C(2.331), /* 12.0 */
   V_TO_C(2.276), /* 13.0 */
   V_TO_C(2.221), /* 14.0 */
   V_TO_C(2.167), /* 15.0 */
   V_TO_C(2.113), /* 16.0 */
   V_TO_C(2.060), /* 17.0 */
   V_TO_C(2.008), /* 18.0 */
   V_TO_C(1.957), /* 19.0 */
   V_TO_C(1.906), /* 20.0 */
   V_TO_C(1.856), /* 21.0 */
   V_TO_C(1.808), /* 22.0 */
   V_TO_C(1.760), /* 23.0 */
   V_TO_C(1.713), /* 24.0 */
   V_TO_C(1.667), /* 25.0 */
   V_TO_C(1.622), /* 26.0 */
   V_TO_C(1.577), /* 27.0 */
   V_TO_C(1.534), /* 28.0 */
   V_TO_C(1.492), /* 29.0 */
   V_TO_C(1.450), /* 30.0 */
   V_TO_C(1.410), /* 31.0 */
   V_TO_C(1.370), /* 32.0 */
   V_TO_C(1.332), /* 33.0 */
   V_TO_C(1.294), /* 34.0 */
   V_TO_C(1.258), /* 35.0 */
   V_TO_C(1.222), /* 36.0 */
   V_TO_C(1.187), /* 37.0 */
   V_TO_C(1.153), /* 38.0 */
   V_TO_C(1.120), /* 39.0 */
   V_TO_C(1.088), /* 40.0 */
   V_TO_C(1.057), /* 41.0 */
   V_TO_C(1.026), /* 42.0 */
   V_TO_C(0.997), /* 43.0 */
   V_TO_C(0.968), /* 44.0 */
   V_TO_C(0.940), /* 45.0 */
   V_TO_C(0.913), /* 46.0 */
   V_TO_C(0.887), /* 47.0 */
   V_TO_C(0.861), /* 48.0 */
   V_TO_C(0.836), /* 49.0 */
   V_TO_C(0.812), /* 50.0 */
   V_TO_C(0.789), /* 51.0 */
   V_TO_C(0.766), /* 52.0 */
   V_TO_C(0.744), /* 53.0 */
   V_TO_C(0.723), /* 54.0 */
   V_TO_C(0.702), /* 55.0 */
   V_TO_C(0.682), /* 56.0 */
   V_TO_C(0.662), /* 57.0 */
   V_TO_C(0.643), /* 58.0 */
   V_TO_C(0.625), /* 59.0 */
   V_TO_C(0.607), /* 60.0 */
   V_TO_C(0.590), /* 61.0 */
   V_TO_C(0.573), /* 62.0 */
   V_TO_C(0.557), /* 63.0 */
   V_TO_C(0.541), /* 64.0 */
   V_TO_C(0.526), /* 65.0 */
   V_TO_C(0.511), /* 66.0 */
   V_TO_C(0.497), /* 67.0 */
   V_TO_C(0.483), /* 68.0 */
   V_TO_C(0.469), /* 69.0 */
   V_TO_C(0.456), /* 70.0 */
   V_TO_C(0.444), /* 71.0 */
   V_TO_C(0.431), /* 72.0 */
   V_TO_C(0.419), /* 73.0 */
   V_TO_C(0.408), /* 74.0 */
   V_TO_C(0.397), /* 75.0 */
   V_TO_C(0.386), /* 76.0 */
   V_TO_C(0.375), /* 77.0 */
   V_TO_C(0.365), /* 78.0 */
   V_TO_C(0.355), /* 79.0 */
   V_TO_C(0.346), /* 80.0 */
   V_TO_C(0.336), /* 81.0 */
   V_TO_C(0.328), /* 82.0 */
   V_TO_C(0.319), /* 83.0 */
   V_TO_C(0.310), /* 84.0 */
   V_TO_C(0.302), /* 85.0 */
};

/*****************************************************************************
 * Private functions
 *****************************************************************************/

/*****************************************************************************
 * Public functions
 *****************************************************************************/

/**
 * Convert a given temperature_t into the equivalent ADC counts required to produce that temperature.
 */
uint16_t TemperatureToAdcCounts(enum tmi_sensor_brand_e brand, bool_t reduced_scaling, temperature_t temp_in)
{
    uint16_t      counts;
    temperature_t lower_limit;
    temperature_t upper_limit;
    uint8_t       table_size;

    CONST FPTR temperature_t * table_temperature;
    CONST FPTR uint16_t      * table_counts;

    switch (brand)
    {
    case Tmi_sensor_Bosch:
    case Tmi_sensor_none:
    default:
        if (!reduced_scaling)
        {
            lower_limit       = lookup_bosch_temperature[0];
            upper_limit       = lookup_bosch_temperature[LOOKUP_SIZE_BOSCH - 1];
            table_temperature = lookup_bosch_temperature;
            table_counts      = lookup_bosch_counts;
            table_size        = LOOKUP_SIZE_BOSCH;
        }
        else
        {
            lower_limit       = lookup_bosch_temperature_rub10[0];
            upper_limit       = lookup_bosch_temperature_rub10[LOOKUP_SIZE_BOSCH - 1];
            table_temperature = lookup_bosch_temperature_rub10;
            table_counts      = lookup_bosch_counts_rub10;
            table_size        = LOOKUP_SIZE_BOSCH;
        }
        break;

    case Tmi_sensor_PT1000:
        lower_limit       = lookup_pt1000_temperature[0];
        upper_limit       = lookup_pt1000_temperature[LOOKUP_SIZE_PT - 1];
        table_temperature = lookup_pt1000_temperature;
        table_counts      = lookup_pt1000_counts;
        table_size        = LOOKUP_SIZE_PT;
        break;

    case Tmi_sensor_NCP:
        lower_limit       = lookup_ncp_temperature[0];
        upper_limit       = lookup_ncp_temperature[LOOKUP_SIZE_NCP - 1];
        table_temperature = lookup_ncp_temperature;
        table_counts      = lookup_ncp_counts;
        table_size        = LOOKUP_SIZE_NCP;
        break;
    }

    if (temp_in < lower_limit)
    {
        /* Use minimum value to 'ensure' that DACs provide a relevant output */
        counts = 0;
    }
    else
    {
        if (temp_in > upper_limit)
        {
            /* Use maximum value to 'ensure' that DACs provide a relevant output */
            counts = MAX_ADC_COUNTS;
        }
        else
        {
            counts = adc_conversion_interp_1d_s16_u16 (
                temp_in,
                table_temperature,
                table_counts,
                table_size);
        }
    }

    /* Now do some range checking & limiting to valid ADC values */
    if (counts > 4095)
    {
        counts = 4095;
    }

    return counts;
}

uint16_t adc_current_to_c(current_t current)
{
#define OM_A (   5880L*1000L)
#define OM_B (  12000L*1000L)
#define OM_C (    160L)

    uint16_t counts;
     int32_t temp;

    if (current > 0)
    {
        temp = divs32(OM_A, (int32_t) current, 0, CURRENT_BP, 0);
        temp = subs32(temp, OM_C,           0, 0,          0);
        temp = divs32(OM_B, temp,           0, 0,          0);
        temp = clips32(temp, (int32_t) 0, (int32_t) 4095);

        /* NB temp can be cast to uint16_t as a result of above clipping */
        counts = narrow_u16((uint32_t) temp, 0, 0);
    }
    else
    {
        counts = 0;
    }

    /* Now do some range checking & limiting to valid ADC values */
    if (counts > 4095)
    {
        counts = 4095;
    }

    return counts;
}
