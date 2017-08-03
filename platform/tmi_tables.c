/**
 *
 * Routines for reading the temperature measurement input signals
 *
 * @file
 * @ingroup tmi
 * $Id: tmi_tables.c 10412 2013-09-10 10:29:35Z tg10 $
 */

/* Copyright (c) Linkedriving Tech 2011 - All rights reserved */

#include "basic_types.h"
#include "hardware.h"
#include "mmc.h"          /* access to the MMC specific data types     */
#include "tmi.h"          /* public header for this module             */
#include "tmi_p.h"        /* private header for this module            */
#include "ai_types.h"     /* access to the A/D conversion macros       */

/******************************************************************************
 * CALIBRATION DATA
 *****************************************************************************/

#define MEMORY_LOCATION CALIBRATION_MEMORY_S12
#include "memory_layout.h"

/**
 * Lookup table, containing the counts axis values, to convert ADC counts to
 * degrees C for the Bosch sensor.
 * Data has been extracted from the BCM HW/SW Specification and re-engineered
 * to convert from ADC counts to temperature instead from voltage to temperature
 *
 * @a2l{type,xaxis}
 * @a2l{min,0}
 * @a2l{max,2047}
 * @a2l{units,ADC counts}
 * @a2l{longidentifier,The lookup table of raw ADC counts.
                       Contributes to:
                           Fm_plat_temp1_input
                           Fm_plat_temp2_input
                           Fm_plat_temp3_input
                           Fm_plat_temp4_input. }
 * @a2l{annotation,The raw temperature ADC counts}
 */
FROM uint16_t tmi_lookup_bosch_counts[LOOKUP_SIZE_BOSCH]=
{
// /* ADC     Temperature  */
     197u,  /* 125       */
//     201u,  /* 124       */
//     206u,  /* 123       */
//     211u,  /* 122       */
//     215u,  /* 121       */
//     220u,  /* 120       */
//     225u,  /* 119       */
//     230u,  /* 118       */
//     236u,  /* 117       */
//     241u,  /* 116       */
//     247u,  /* 115       */
//     252u,  /* 114       */
//     258u,  /* 113       */
//     264u,  /* 112       */
//     270u,  /* 111       */
//     277u,  /* 110       */
//     283u,  /* 109       */
//     290u,  /* 108       */
//     297u,  /* 107       */
//     304u,  /* 106       */
//     311u,  /* 105       */
//     318u,  /* 104       */
//     326u,  /* 103       */
//     334u,  /* 102       */
//     342u,  /* 101       */
//     350u,  /* 100       */
//     358u,  /*  99       */
//     367u,  /*  98       */
//     376u,  /*  97       */
//     385u,  /*  96       */
//     395u,  /*  95       */
//     404u,  /*  94       */
//     414u,  /*  93       */
//     424u,  /*  92       */
//     435u,  /*  91       */
//     445u,  /*  90       */
//     457u,  /*  89       */
//     468u,  /*  88       */
//     480u,  /*  87       */
//     491u,  /*  86       */
//     504u,  /*  85       */
//     516u,  /*  84       */
//     529u,  /*  83       */
//     543u,  /*  82       */
//     556u,  /*  81       */
//     570u,  /*  80       */
//     584u,  /*  79       */
//     599u,  /*  78       */
//     614u,  /*  77       */
//     630u,  /*  76       */
//     646u,  /*  75       */
//     662u,  /*  74       */
//     679u,  /*  73       */
//     696u,  /*  72       */
//     714u,  /*  71       */
//     732u,  /*  70       */
//     750u,  /*  69       */
//     769u,  /*  68       */
//     788u,  /*  67       */
//     808u,  /*  66       */
//     829u,  /*  65       */
//     850u,  /*  64       */
//     871u,  /*  63       */
//     893u,  /*  62       */
//     915u,  /*  61       */
//     938u,  /*  60       */
//     962u,  /*  59       */
//     986u,  /*  58       */
//    1010u,  /*  57       */
//    1035u,  /*  56       */
//    1061u,  /*  55       */
//    1087u,  /*  54       */
//    1114u,  /*  53       */
//    1141u,  /*  52       */
//    1169u,  /*  51       */
//    1198u,  /*  50       */
//    1227u,  /*  49       */
//    1256u,  /*  48       */
//    1286u,  /*  47       */
//    1317u,  /*  46       */
//    1348u,  /*  45       */
//    1380u,  /*  44       */
//    1412u,  /*  43       */
//    1445u,  /*  42       */
//    1479u,  /*  41       */
//    1512u,  /*  40       */
//    1547u,  /*  39       */
//    1582u,  /*  38       */
//    1617u,  /*  37       */
//    1653u,  /*  36       */
//    1690u,  /*  35       */
//    1726u,  /*  34       */
//    1764u,  /*  33       */
//    1801u,  /*  32       */
//    1839u,  /*  31       */
//    1878u,  /*  30       */
//    1916u,  /*  29       */
//    1955u,  /*  28       */
//    1995u,  /*  27       */
//    2034u,  /*  26       */
//    2074u,  /*  25       */
//    2114u,  /*  24       */
//    2155u,  /*  23       */
//    2195u,  /*  22       */
//    2236u,  /*  21       */
//    2276u,  /*  20       */
//    2317u,  /*  19       */
//    2358u,  /*  18       */
//    2398u,  /*  17       */
//    2439u,  /*  16       */
//    2480u,  /*  15       */
//    2520u,  /*  14       */
//    2561u,  /*  13       */
//    2601u,  /*  12       */
//    2641u,  /*  11       */
//    2680u,  /*  10       */
//    2720u,  /*   9       */
//    2759u,  /*   8       */
//    2798u,  /*   7       */
//    2836u,  /*   6       */
//    2874u,  /*   5       */
//    2911u,  /*   4       */
//    2948u,  /*   3       */
//    2985u,  /*   2       */
//    3021u,  /*   1       */
//    3057u,  /*   0       */
//    3091u,  /*  -1       */
//    3126u,  /*  -2       */
//    3160u,  /*  -3       */
//    3193u,  /*  -4       */
//    3225u,  /*  -5       */
//    3257u,  /*  -6       */
//    3288u,  /*  -7       */
//    3318u,  /*  -8       */
//    3348u,  /*  -9       */
//    3377u,  /* -10       */
//    3405u,  /* -11       */
//    3433u,  /* -12       */
//    3460u,  /* -13       */
//    3486u,  /* -14       */
//    3511u,  /* -15       */
//    3536u,  /* -16       */
//    3560u,  /* -17       */
//    3583u,  /* -18       */
//    3605u,  /* -19       */
//    3627u,  /* -20       */
//    3648u,  /* -21       */
//    3669u,  /* -22       */
//    3688u,  /* -23       */
//    3707u,  /* -24       */
//    3725u,  /* -25       */
//    3743u,  /* -26       */
//    3760u,  /* -27       */
//    3776u,  /* -28       */
//    3792u,  /* -29       */
//    3807u,  /* -30       */
//    3822u,  /* -31       */
//    3835u,  /* -32       */
//    3849u,  /* -33       */
//    3861u,  /* -34       */
//    3874u,  /* -35       */
//    3885u,  /* -36       */
//    3897u,  /* -37       */
//    3907u,  /* -38       */
//    3917u,  /* -39       */
//    3927u,  /* -40       */
//    3936u,  /* -41       */
//    3945u,  /* -42       */
//    3954u,  /* -43       */
//    3962u,  /* -44       */
//    3969u,  /* -45       */
//    3977u,  /* -46       */
//    3984u,  /* -47       */
//    3990u,  /* -48       */
//    3996u,  /* -49       */
//    4002u,  /* -50       */
};

/**
 * Lookup table, containing the temperature axis values, to convert ADC counts to
 * degrees C for the Bosch  sensor.
 * Data has been extracted from the BCM HW/SW Specification and re-engineered
 * to convert from ADC counts to temperature instead from voltage to temperature
 *
 * @a2l{min,-100.0}
 * @a2l{max,150.0}
 * @a2l{conversion,temperature_t}
 * @a2l{LongIdentifier,The actual temperature associated with the input ADC count}
 * @a2l{type,curve}
 * @a2l{xaxis,tmi_lookup_bosch_counts}
 */
FROM temperature_t tmi_lookup_bosch_temperature[LOOKUP_SIZE_BOSCH]=
{
    TEMP(125),
//    TEMP(124),
//    TEMP(123),
//    TEMP(122),
//    TEMP(121),
//    TEMP(120),
//    TEMP(119),
//    TEMP(118),
//    TEMP(117),
//    TEMP(116),
//    TEMP(115),
//    TEMP(114),
//    TEMP(113),
//    TEMP(112),
//    TEMP(111),
//    TEMP(110),
//    TEMP(109),
//    TEMP(108),
//    TEMP(107),
//    TEMP(106),
//    TEMP(105),
//    TEMP(104),
//    TEMP(103),
//    TEMP(102),
//    TEMP(101),
//    TEMP(100),
//    TEMP( 99),
//    TEMP( 98),
//    TEMP( 97),
//    TEMP( 96),
//    TEMP( 95),
//    TEMP( 94),
//    TEMP( 93),
//    TEMP( 92),
//    TEMP( 91),
//    TEMP( 90),
//    TEMP( 89),
//    TEMP( 88),
//    TEMP( 87),
//    TEMP( 86),
//    TEMP( 85),
//    TEMP( 84),
//    TEMP( 83),
//    TEMP( 82),
//    TEMP( 81),
//    TEMP( 80),
//    TEMP( 79),
//    TEMP( 78),
//    TEMP( 77),
//    TEMP( 76),
//    TEMP( 75),
//    TEMP( 74),
//    TEMP( 73),
//    TEMP( 72),
//    TEMP( 71),
//    TEMP( 70),
//    TEMP( 69),
//    TEMP( 68),
//    TEMP( 67),
//    TEMP( 66),
//    TEMP( 65),
//    TEMP( 64),
//    TEMP( 63),
//    TEMP( 62),
//    TEMP( 61),
//    TEMP( 60),
//    TEMP( 59),
//    TEMP( 58),
//    TEMP( 57),
//    TEMP( 56),
//    TEMP( 55),
//    TEMP( 54),
//    TEMP( 53),
//    TEMP( 52),
//    TEMP( 51),
//    TEMP( 50),
//    TEMP( 49),
//    TEMP( 48),
//    TEMP( 47),
//    TEMP( 46),
//    TEMP( 45),
//    TEMP( 44),
//    TEMP( 43),
//    TEMP( 42),
//    TEMP( 41),
//    TEMP( 40),
//    TEMP( 39),
//    TEMP( 38),
//    TEMP( 37),
//    TEMP( 36),
//    TEMP( 35),
//    TEMP( 34),
//    TEMP( 33),
//    TEMP( 32),
//    TEMP( 31),
//    TEMP( 30),
//    TEMP( 29),
//    TEMP( 28),
//    TEMP( 27),
//    TEMP( 26),
//    TEMP( 25),
//    TEMP( 24),
//    TEMP( 23),
//    TEMP( 22),
//    TEMP( 21),
//    TEMP( 20),
//    TEMP( 19),
//    TEMP( 18),
//    TEMP( 17),
//    TEMP( 16),
//    TEMP( 15),
//    TEMP( 14),
//    TEMP( 13),
//    TEMP( 12),
//    TEMP( 11),
//    TEMP( 10),
//    TEMP(  9),
//    TEMP(  8),
//    TEMP(  7),
//    TEMP(  6),
//    TEMP(  5),
//    TEMP(  4),
//    TEMP(  3),
//    TEMP(  2),
//    TEMP(  1),
//    TEMP(  0),
//    TEMP( -1),
//    TEMP( -2),
//    TEMP( -3),
//    TEMP( -4),
//    TEMP( -5),
//    TEMP( -6),
//    TEMP( -7),
//    TEMP( -8),
//    TEMP( -9),
//    TEMP(-10),
//    TEMP(-11),
//    TEMP(-12),
//    TEMP(-13),
//    TEMP(-14),
//    TEMP(-15),
//    TEMP(-16),
//    TEMP(-17),
//    TEMP(-18),
//    TEMP(-19),
//    TEMP(-20),
//    TEMP(-21),
//    TEMP(-22),
//    TEMP(-23),
//    TEMP(-24),
//    TEMP(-25),
//    TEMP(-26),
//    TEMP(-27),
//    TEMP(-28),
//    TEMP(-29),
//    TEMP(-30),
//    TEMP(-31),
//    TEMP(-32),
//    TEMP(-33),
//    TEMP(-34),
//    TEMP(-35),
//    TEMP(-36),
//    TEMP(-37),
//    TEMP(-38),
//    TEMP(-39),
//    TEMP(-40),
//    TEMP(-41),
//    TEMP(-42),
//    TEMP(-43),
//    TEMP(-44),
//    TEMP(-45),
//    TEMP(-46),
//    TEMP(-47),
//    TEMP(-48),
//    TEMP(-49),
//    TEMP(-50),
};

/**
 * Lookup table, containing the counts axis values, to convert ADC counts to
 * degrees C for the Bosch sensor.
 * Data has been extracted from the BCM HW/SW Specification and re-engineered
 * to convert from ADC counts to temperature instead from voltage to temperature
 *
 * @a2l{type,xaxis}
 * @a2l{min,0}
 * @a2l{max,2047}
 * @a2l{units,ADC counts}
 * @a2l{longidentifier,The lookup table of raw ADC counts.
                       Contributes to:
                           Fm_plat_temp1_input
                           Fm_plat_temp2_input
                           Fm_plat_temp3_input
                           Fm_plat_temp4_input. }
 * @a2l{annotation,The raw temperature ADC counts}
 */
FROM uint16_t tmi_lookup_bosch_counts_rub010[LOOKUP_SIZE_BOSCH]=
{
// /* ADC     Temperature  */
     34u,   /* 125       */
//     35u,   /* 124       */
//     36u,   /* 123       */
//     37u,   /* 121       */
//     37u,   /* 122       */
//     38u,   /* 120       */
//     39u,   /* 119       */
//     40u,   /* 118       */
//     41u,   /* 117       */
//     42u,   /* 116       */
//     43u,   /* 115       */
//     44u,   /* 114       */
//     45u,   /* 113       */
//     46u,   /* 112       */
//     47u,   /* 111       */
//     48u,   /* 110       */
//     49u,   /* 109       */
//     50u,   /* 108       */
//     51u,   /* 107       */
//     52u,   /* 106       */
//     54u,   /* 105       */
//     55u,   /* 104       */
//     56u,   /* 103       */
//     58u,   /* 102       */
//     59u,   /* 101       */
//     60u,   /* 100       */
//     62u,   /*  99       */
//     63u,   /*  98       */
//     65u,   /*  97       */
//     67u,   /*  96       */
//     68u,   /*  95       */
//     70u,   /*  94       */
//     71u,   /*  93       */
//     73u,   /*  92       */
//     75u,   /*  91       */
//     77u,   /*  90       */
//     79u,   /*  89       */
//     81u,   /*  88       */
//     83u,   /*  87       */
//     85u,   /*  86       */
//     87u,   /*  85       */
//     89u,   /*  84       */
//     91u,   /*  83       */
//     93u,   /*  82       */
//     95u,   /*  81       */
//     98u,   /*  80       */
//    100u,   /*  79       */
//    103u,   /*  78       */
//    105u,   /*  77       */
//    108u,   /*  76       */
//    111u,   /*  75       */
//    113u,   /*  74       */
//    116u,   /*  73       */
//    119u,   /*  72       */
//    122u,   /*  71       */
//    125u,   /*  70       */
//    128u,   /*  69       */
//    131u,   /*  68       */
//    134u,   /*  67       */
//    138u,   /*  66       */
//    141u,   /*  65       */
//    145u,   /*  64       */
//    148u,   /*  63       */
//    152u,   /*  62       */
//    156u,   /*  61       */
//    159u,   /*  60       */
//    163u,   /*  59       */
//    167u,   /*  58       */
//    171u,   /*  57       */
//    175u,   /*  56       */
//    180u,   /*  55       */
//    184u,   /*  54       */
//    188u,   /*  53       */
//    193u,   /*  52       */
//    197u,   /*  51       */
//    202u,   /*  50       */
//    207u,   /*  49       */
//    211u,   /*  48       */
//    216u,   /*  47       */
//    221u,   /*  46       */
//    226u,   /*  45       */
//    231u,   /*  44       */
//    237u,   /*  43       */
//    242u,   /*  42       */
//    247u,   /*  41       */
//    252u,   /*  40       */
//    258u,   /*  39       */
//    264u,   /*  38       */
//    269u,   /*  37       */
//    275u,   /*  36       */
//    281u,   /*  35       */
//    286u,   /*  34       */
//    292u,   /*  33       */
//    298u,   /*  32       */
//    304u,   /*  31       */
//    310u,   /*  30       */
//    316u,   /*  29       */
//    322u,   /*  28       */
//    328u,   /*  27       */
//    335u,   /*  26       */
//    341u,   /*  25       */
//    347u,   /*  24       */
//    353u,   /*  23       */
//    359u,   /*  22       */
//    366u,   /*  21       */
//    372u,   /*  20       */
//    378u,   /*  19       */
//    384u,   /*  18       */
//    390u,   /*  17       */
//    397u,   /*  16       */
//    403u,   /*  15       */
//    409u,   /*  14       */
//    415u,   /*  13       */
//    421u,   /*  12       */
//    427u,   /*  11       */
//    433u,   /*  10       */
//    439u,   /*   9       */
//    444u,   /*   8       */
//    450u,   /*   7       */
//    456u,   /*   6       */
//    462u,   /*   5       */
//    467u,   /*   4       */
//    473u,   /*   3       */
//    478u,   /*   2       */
//    483u,   /*   1       */
//    488u,   /*   0       */
//    493u,   /*  -1       */
//    499u,   /*  -2       */
//    503u,   /*  -3       */
//    508u,   /*  -4       */
//    513u,   /*  -5       */
//    518u,   /*  -6       */
//    522u,   /*  -7       */
//    526u,   /*  -8       */
//    531u,   /*  -9       */
//    535u,   /* -10       */
//    539u,   /* -11       */
//    543u,   /* -12       */
//    547u,   /* -13       */
//    550u,   /* -14       */
//    554u,   /* -15       */
//    557u,   /* -16       */
//    561u,   /* -17       */
//    564u,   /* -18       */
//    567u,   /* -19       */
//    571u,   /* -20       */
//    573u,   /* -21       */
//    576u,   /* -22       */
//    579u,   /* -23       */
//    582u,   /* -24       */
//    584u,   /* -25       */
//    587u,   /* -26       */
//    589u,   /* -27       */
//    592u,   /* -28       */
//    594u,   /* -29       */
//    596u,   /* -30       */
//    598u,   /* -31       */
//    600u,   /* -32       */
//    602u,   /* -33       */
//    603u,   /* -34       */
//    605u,   /* -35       */
//    607u,   /* -36       */
//    608u,   /* -37       */
//    610u,   /* -38       */
//    611u,   /* -39       */
//    613u,   /* -40       */
//    614u,   /* -41       */
//    615u,   /* -42       */
//    616u,   /* -43       */
//    617u,   /* -44       */
//    619u,   /* -46       */
//    619u,   /* -45       */
//    620u,   /* -47       */
//    621u,   /* -48       */
//    622u,   /* -49       */
//    623u,   /* -50       */
};

/**
 * Lookup table, containing the temperature axis values, to convert ADC counts to
 * degrees C for the Bosch  sensor.
 * Data has been extracted from the BCM HW/SW Specification and re-engineered
 * to convert from ADC counts to temperature instead from voltage to temperature
 *
 * @a2l{min,-100.0}
 * @a2l{max,150.0}
 * @a2l{conversion,temperature_t}
 * @a2l{LongIdentifier,The actual temperature associated with the input ADC count}
 * @a2l{type,curve}
 * @a2l{xaxis,tmi_lookup_bosch_counts}
 */
FROM temperature_t tmi_lookup_bosch_temperature_rub010[LOOKUP_SIZE_BOSCH]=
{
    TEMP(125),
//    TEMP(124),
//    TEMP(123),
//    TEMP(121),
//    TEMP(122),
//    TEMP(120),
//    TEMP(119),
//    TEMP(118),
//    TEMP(117),
//    TEMP(116),
//    TEMP(115),
//    TEMP(114),
//    TEMP(113),
//    TEMP(112),
//    TEMP(111),
//    TEMP(110),
//    TEMP(109),
//    TEMP(108),
//    TEMP(107),
//    TEMP(106),
//    TEMP(105),
//    TEMP(104),
//    TEMP(103),
//    TEMP(102),
//    TEMP(101),
//    TEMP(100),
//    TEMP( 99),
//    TEMP( 98),
//    TEMP( 97),
//    TEMP( 96),
//    TEMP( 95),
//    TEMP( 94),
//    TEMP( 93),
//    TEMP( 92),
//    TEMP( 91),
//    TEMP( 90),
//    TEMP( 89),
//    TEMP( 88),
//    TEMP( 87),
//    TEMP( 86),
//    TEMP( 85),
//    TEMP( 84),
//    TEMP( 83),
//    TEMP( 82),
//    TEMP( 81),
//    TEMP( 80),
//    TEMP( 79),
//    TEMP( 78),
//    TEMP( 77),
//    TEMP( 76),
//    TEMP( 75),
//    TEMP( 74),
//    TEMP( 73),
//    TEMP( 72),
//    TEMP( 71),
//    TEMP( 70),
//    TEMP( 69),
//    TEMP( 68),
//    TEMP( 67),
//    TEMP( 66),
//    TEMP( 65),
//    TEMP( 64),
//    TEMP( 63),
//    TEMP( 62),
//    TEMP( 61),
//    TEMP( 60),
//    TEMP( 59),
//    TEMP( 58),
//    TEMP( 57),
//    TEMP( 56),
//    TEMP( 55),
//    TEMP( 54),
//    TEMP( 53),
//    TEMP( 52),
//    TEMP( 51),
//    TEMP( 50),
//    TEMP( 49),
//    TEMP( 48),
//    TEMP( 47),
//    TEMP( 46),
//    TEMP( 45),
//    TEMP( 44),
//    TEMP( 43),
//    TEMP( 42),
//    TEMP( 41),
//    TEMP( 40),
//    TEMP( 39),
//    TEMP( 38),
//    TEMP( 37),
//    TEMP( 36),
//    TEMP( 35),
//    TEMP( 34),
//    TEMP( 33),
//    TEMP( 32),
//    TEMP( 31),
//    TEMP( 30),
//    TEMP( 29),
//    TEMP( 28),
//    TEMP( 27),
//    TEMP( 26),
//    TEMP( 25),
//    TEMP( 24),
//    TEMP( 23),
//    TEMP( 22),
//    TEMP( 21),
//    TEMP( 20),
//    TEMP( 19),
//    TEMP( 18),
//    TEMP( 17),
//    TEMP( 16),
//    TEMP( 15),
//    TEMP( 14),
//    TEMP( 13),
//    TEMP( 12),
//    TEMP( 11),
//    TEMP( 10),
//    TEMP(  9),
//    TEMP(  8),
//    TEMP(  7),
//    TEMP(  6),
//    TEMP(  5),
//    TEMP(  4),
//    TEMP(  3),
//    TEMP(  2),
//    TEMP(  1),
//    TEMP(  0),
//    TEMP( -1),
//    TEMP( -2),
//    TEMP( -3),
//    TEMP( -4),
//    TEMP( -5),
//    TEMP( -6),
//    TEMP( -7),
//    TEMP( -8),
//    TEMP( -9),
//    TEMP(-10),
//    TEMP(-11),
//    TEMP(-12),
//    TEMP(-13),
//    TEMP(-14),
//    TEMP(-15),
//    TEMP(-16),
//    TEMP(-17),
//    TEMP(-18),
//    TEMP(-19),
//    TEMP(-20),
//    TEMP(-21),
//    TEMP(-22),
//    TEMP(-23),
//    TEMP(-24),
//    TEMP(-25),
//    TEMP(-26),
//    TEMP(-27),
//    TEMP(-28),
//    TEMP(-29),
//    TEMP(-30),
//    TEMP(-31),
//    TEMP(-32),
//    TEMP(-33),
//    TEMP(-34),
//    TEMP(-35),
//    TEMP(-36),
//    TEMP(-37),
//    TEMP(-38),
//    TEMP(-39),
//    TEMP(-40),
//    TEMP(-41),
//    TEMP(-42),
//    TEMP(-43),
//    TEMP(-44),
//    TEMP(-46),
//    TEMP(-45),
//    TEMP(-47),
//    TEMP(-48),
//    TEMP(-49),
//    TEMP(-50),
};

/**
 * Lookup table, containing the counts axis values, to convert ADC counts to
 * degrees C for the PT1000 sensor.
 * Data has been extracted from the BCM HW/SW Specification and re-engineered
 * to convert from ADC counts to temperature instead from voltage to temperature
 *
 * @a2l{type,xaxis}
 * @a2l{min,0}
 * @a2l{max,2047}
 * @a2l{units,ADC counts}
 * @a2l{longidentifier,The lookup table of raw ADC counts.
                       Contributes to:
                           Fm_plat_temp1_input
                           Fm_plat_temp2_input
                           Fm_plat_temp3_input
                           Fm_plat_temp4_input. }
 * @a2l{annotation,The raw temperature ADC counts}
 */
FROM uint32_t tmi_lookup_pt1000_counts[LOOKUP_SIZE_PT]=
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
	V_TO_C(2.798), /* 86.0  */ 
	V_TO_C(2.806), /* 87.0  */ 
	V_TO_C(2.814), /* 88.0  */ 
	V_TO_C(2.822), /* 89.0  */ 
	V_TO_C(2.830), /* 90.0  */ 
	V_TO_C(2.838), /* 91.0  */ 
	V_TO_C(2.846), /* 92.0  */ 
	V_TO_C(2.854), /* 93.0  */ 
	V_TO_C(2.862), /* 94.0  */ 
	V_TO_C(2.870), /* 95.0  */ 
	V_TO_C(2.878), /* 96.0  */ 
	V_TO_C(2.886), /* 97.0  */ 
	V_TO_C(2.894), /* 98.0  */ 
	V_TO_C(2.902), /* 99.0  */ 
	V_TO_C(2.910), /* 100.0 */ 
	V_TO_C(2.918), /* 101.0 */ 
	V_TO_C(2.926), /* 102.0 */ 
	V_TO_C(2.934), /* 103.0 */ 
	V_TO_C(2.942), /* 104.0 */ 
	V_TO_C(2.950), /* 105.0 */ 
	V_TO_C(2.958), /* 106.0 */ 
	V_TO_C(2.965), /* 107.0 */ 
	V_TO_C(2.973), /* 108.0 */ 
	V_TO_C(2.981), /* 109.0 */ 
	V_TO_C(2.989), /* 110.0 */ 
	V_TO_C(2.997), /* 111.0 */ 
	V_TO_C(3.005), /* 112.0 */ 
	V_TO_C(3.013), /* 113.0 */ 
	V_TO_C(3.021), /* 114.0 */ 
	V_TO_C(3.029), /* 115.0 */ 
	V_TO_C(3.037), /* 116.0 */ 
	V_TO_C(3.045), /* 117.0 */ 
	V_TO_C(3.053), /* 118.0 */ 
	V_TO_C(3.061), /* 119.0 */ 
	V_TO_C(3.069), /* 120.0 */ 
	V_TO_C(3.076), /* 121.0 */ 
	V_TO_C(3.084), /* 122.0 */ 
	V_TO_C(3.092), /* 123.0 */ 
	V_TO_C(3.100), /* 124.0 */ 
	V_TO_C(3.108), /* 125.0 */ 
	V_TO_C(3.116), /* 126.0 */ 
	V_TO_C(3.124), /* 127.0 */ 
	V_TO_C(3.132), /* 128.0 */ 
	V_TO_C(3.140), /* 129.0 */ 
	V_TO_C(3.148), /* 130.0 */ 
	V_TO_C(3.156), /* 131.0 */ 
	V_TO_C(3.163), /* 132.0 */ 
	V_TO_C(3.171), /* 133.0 */ 
	V_TO_C(3.179), /* 134.0 */ 
	V_TO_C(3.187), /* 135.0 */ 
	V_TO_C(3.195), /* 136.0 */ 
	V_TO_C(3.203), /* 137.0 */ 
	V_TO_C(3.211), /* 138.0 */ 
	V_TO_C(3.219), /* 139.0 */ 
	V_TO_C(3.226), /* 140.0 */ 
	V_TO_C(3.234), /* 141.0 */ 
	V_TO_C(3.242), /* 142.0 */ 
	V_TO_C(3.250), /* 143.0 */ 
	V_TO_C(3.258), /* 144.0 */ 
	V_TO_C(3.266), /* 145.0 */ 
	V_TO_C(3.274), /* 146.0 */ 
	V_TO_C(3.281), /* 147.0 */ 
	V_TO_C(3.289), /* 148.0 */ 
	V_TO_C(3.297), /* 149.0 */ 
	V_TO_C(3.305)  /* 150.0 */ 
};

/**
 * Lookup table, containing the temperature axis values, to convert ADC counts to
 * degrees C for the PT1000 sensor.
 * Data has been extracted from the BCM HW/SW Specification and re-engineered
 * to convert from ADC counts to temperature instead from voltage to temperature
 *
 * @a2l{min,-100.0}
 * @a2l{max,150.0}
 * @a2l{conversion,temperature_t}
 * @a2l{LongIdentifier,The actual temperature associated with the input ADC count}
 * @a2l{type,curve}
 * @a2l{xaxis,tmi_lookup_pt1000_counts}
 */
FROM int32_t tmi_lookup_pt1000_temperature[LOOKUP_SIZE_PT]=
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
	TEMP( 86.0 ),
	TEMP( 87.0 ),
	TEMP( 88.0 ),
	TEMP( 89.0 ),
	TEMP( 90.0 ),
	TEMP( 91.0 ),
	TEMP( 92.0 ),
	TEMP( 93.0 ),
	TEMP( 94.0 ),
	TEMP( 95.0 ),
	TEMP( 96.0 ),
	TEMP( 97.0 ),
	TEMP( 98.0 ),
	TEMP( 99.0 ),
	TEMP( 100.0), 
	TEMP( 101.0), 
	TEMP( 102.0), 
	TEMP( 103.0), 
	TEMP( 104.0), 
	TEMP( 105.0), 
	TEMP( 106.0), 
	TEMP( 107.0), 
	TEMP( 108.0), 
	TEMP( 109.0), 
	TEMP( 110.0), 
	TEMP( 111.0), 
	TEMP( 112.0), 
	TEMP( 113.0), 
	TEMP( 114.0), 
	TEMP( 115.0), 
	TEMP( 116.0), 
	TEMP( 117.0), 
	TEMP( 118.0), 
	TEMP( 119.0), 
	TEMP( 120.0), 
	TEMP( 121.0), 
	TEMP( 122.0), 
	TEMP( 123.0), 
	TEMP( 124.0), 
	TEMP( 125.0), 
	TEMP( 126.0), 
	TEMP( 127.0), 
	TEMP( 128.0), 
	TEMP( 129.0), 
	TEMP( 130.0), 
	TEMP( 131.0), 
	TEMP( 132.0), 
	TEMP( 133.0), 
	TEMP( 134.0), 
	TEMP( 135.0), 
	TEMP( 136.0), 
	TEMP( 137.0), 
	TEMP( 138.0), 
	TEMP( 139.0), 
	TEMP( 140.0), 
	TEMP( 141.0), 
	TEMP( 142.0), 
	TEMP( 143.0), 
	TEMP( 144.0), 
	TEMP( 145.0), 
	TEMP( 146.0), 
	TEMP( 147.0), 
	TEMP( 148.0), 
	TEMP( 149.0), 
	TEMP( 150.0), 
};

/**
 * Lookup table, containing the counts axis values, to convert ADC counts to
 * degrees C for the NCP sensor.
 * Data has been extracted from the BCM HW/SW Specification and re-engineered
 * to convert from ADC counts to temperature instead from voltage to temperature
 *
 * @a2l{min,0}
 * @a2l{max,2047}
 * @a2l{longidentifier,The lookup table of raw ADC counts.
                       Contributes to:
                           Fm_plat_temp1_input
                           Fm_plat_temp2_input
                           Fm_plat_temp3_input
                           Fm_plat_temp4_input. }
 * @a2l{type,xaxis}
 */
FROM uint16_t tmi_lookup_ncp_counts[LOOKUP_SIZE_NCP]=
{
// /* ADC     Voltage    Temperature  */
   V_TO_C(0.302), /* 85.0 */
//   V_TO_C(0.310), /* 84.0 */
//   V_TO_C(0.319), /* 83.0 */
//   V_TO_C(0.328), /* 82.0 */
//   V_TO_C(0.336), /* 81.0 */
//   V_TO_C(0.346), /* 80.0 */
//   V_TO_C(0.355), /* 79.0 */
//   V_TO_C(0.365), /* 78.0 */
//   V_TO_C(0.375), /* 77.0 */
//   V_TO_C(0.386), /* 76.0 */
//   V_TO_C(0.397), /* 75.0 */
//   V_TO_C(0.408), /* 74.0 */
//   V_TO_C(0.419), /* 73.0 */
//   V_TO_C(0.431), /* 72.0 */
//   V_TO_C(0.444), /* 71.0 */
//   V_TO_C(0.456), /* 70.0 */
//   V_TO_C(0.469), /* 69.0 */
//   V_TO_C(0.483), /* 68.0 */
//   V_TO_C(0.497), /* 67.0 */
//   V_TO_C(0.511), /* 66.0 */
//   V_TO_C(0.526), /* 65.0 */
//   V_TO_C(0.541), /* 64.0 */
//   V_TO_C(0.557), /* 63.0 */
//   V_TO_C(0.573), /* 62.0 */
//   V_TO_C(0.590), /* 61.0 */
//   V_TO_C(0.607), /* 60.0 */
//   V_TO_C(0.625), /* 59.0 */
//   V_TO_C(0.643), /* 58.0 */
//   V_TO_C(0.662), /* 57.0 */
//   V_TO_C(0.682), /* 56.0 */
//   V_TO_C(0.702), /* 55.0 */
//   V_TO_C(0.723), /* 54.0 */
//   V_TO_C(0.744), /* 53.0 */
//   V_TO_C(0.766), /* 52.0 */
//   V_TO_C(0.789), /* 51.0 */
//   V_TO_C(0.812), /* 50.0 */
//   V_TO_C(0.836), /* 49.0 */
//   V_TO_C(0.861), /* 48.0 */
//   V_TO_C(0.887), /* 47.0 */
//   V_TO_C(0.913), /* 46.0 */
//   V_TO_C(0.940), /* 45.0 */
//   V_TO_C(0.968), /* 44.0 */
//   V_TO_C(0.997), /* 43.0 */
//   V_TO_C(1.026), /* 42.0 */
//   V_TO_C(1.057), /* 41.0 */
//   V_TO_C(1.088), /* 40.0 */
//   V_TO_C(1.120), /* 39.0 */
//   V_TO_C(1.153), /* 38.0 */
//   V_TO_C(1.187), /* 37.0 */
//   V_TO_C(1.222), /* 36.0 */
//   V_TO_C(1.258), /* 35.0 */
//   V_TO_C(1.294), /* 34.0 */
//   V_TO_C(1.332), /* 33.0 */
//   V_TO_C(1.370), /* 32.0 */
//   V_TO_C(1.410), /* 31.0 */
//   V_TO_C(1.450), /* 30.0 */
//   V_TO_C(1.492), /* 29.0 */
//   V_TO_C(1.534), /* 28.0 */
//   V_TO_C(1.577), /* 27.0 */
//   V_TO_C(1.622), /* 26.0 */
//   V_TO_C(1.667), /* 25.0 */
//   V_TO_C(1.713), /* 24.0 */
//   V_TO_C(1.760), /* 23.0 */
//   V_TO_C(1.808), /* 22.0 */
//   V_TO_C(1.856), /* 21.0 */
//   V_TO_C(1.906), /* 20.0 */
//   V_TO_C(1.957), /* 19.0 */
//   V_TO_C(2.008), /* 18.0 */
//   V_TO_C(2.060), /* 17.0 */
//   V_TO_C(2.113), /* 16.0 */
//   V_TO_C(2.167), /* 15.0 */
//   V_TO_C(2.221), /* 14.0 */
//   V_TO_C(2.276), /* 13.0 */
//   V_TO_C(2.331), /* 12.0 */
//   V_TO_C(2.388), /* 11.0 */
//   V_TO_C(2.444), /* 10.0 */
//   V_TO_C(2.501), /*  9.0 */
//   V_TO_C(2.559), /*  8.0 */
//   V_TO_C(2.617), /*  7.0 */
//   V_TO_C(2.675), /*  6.0 */
//   V_TO_C(2.733), /*  5.0 */
//   V_TO_C(2.792), /*  4.0 */
//   V_TO_C(2.850), /*  3.0 */
//   V_TO_C(2.909), /*  2.0 */
//   V_TO_C(2.968), /*  1.0 */
//   V_TO_C(3.026), /*  0.0 */
//   V_TO_C(3.085), /* -1.0 */
//   V_TO_C(3.143), /* -2.0 */
//   V_TO_C(3.200), /* -3.0 */
//   V_TO_C(3.258), /* -4.0 */
//   V_TO_C(3.315), /* -5.0 */
//   V_TO_C(3.371), /* -6.0 */
//   V_TO_C(3.427), /* -7.0 */
//   V_TO_C(3.482), /* -8.0 */
//   V_TO_C(3.537), /* -9.0 */
//   V_TO_C(3.591), /* 10.0 */
//   V_TO_C(3.643), /* 11.0 */
//   V_TO_C(3.696), /* 12.0 */
//   V_TO_C(3.747), /* 13.0 */
//   V_TO_C(3.797), /* 14.0 */
//   V_TO_C(3.846), /* 15.0 */
//   V_TO_C(3.894), /* 16.0 */
//   V_TO_C(3.941), /* 17.0 */
//   V_TO_C(3.987), /* 18.0 */
//   V_TO_C(4.032), /* 19.0 */
//   V_TO_C(4.075), /* 20.0 */
//   V_TO_C(4.117), /* 21.0 */
//   V_TO_C(4.159), /* 22.0 */
//   V_TO_C(4.198), /* 23.0 */
//   V_TO_C(4.237), /* 24.0 */
//   V_TO_C(4.274), /* 25.0 */
//   V_TO_C(4.311), /* 26.0 */
//   V_TO_C(4.346), /* 27.0 */
//   V_TO_C(4.379), /* 28.0 */
//   V_TO_C(4.412), /* 29.0 */
//   V_TO_C(4.443), /* 30.0 */
//   V_TO_C(4.473), /* 31.0 */
//   V_TO_C(4.501), /* 32.0 */
//   V_TO_C(4.529), /* 33.0 */
//   V_TO_C(4.555), /* 34.0 */
//   V_TO_C(4.581), /* 35.0 */
//   V_TO_C(4.605), /* 36.0 */
//   V_TO_C(4.628), /* 37.0 */
//   V_TO_C(4.650), /* 38.0 */
//   V_TO_C(4.671), /* 39.0 */
//   V_TO_C(4.691), /* 40.0 */
};

/**
 * Lookup table, containing the temperature axis values, to convert ADC counts to
 * degrees C for the NCP sensor.
 * Data has been extracted from the BCM HW/SW Specification and re-engineered
 * to convert from ADC counts to temperature instead from voltage to temperature
 *
 * @a2l{conversion,temperature_t}
 * @a2l{min,-100.0}
 * @a2l{max,150.0}
 * @a2l{LongIdentifier,The actual temperature associated with the input ADC count.}
 * @a2l{type,curve}
 * @a2l{xaxis,tmi_lookup_ncp_counts}
 */
FROM temperature_t tmi_lookup_ncp_temperature[LOOKUP_SIZE_NCP]=
{
    TEMP( 85.0),
//    TEMP( 84.0),
//    TEMP( 83.0),
//    TEMP( 82.0),
//    TEMP( 81.0),
//    TEMP( 80.0),
//    TEMP( 79.0),
//    TEMP( 78.0),
//    TEMP( 77.0),
//    TEMP( 76.0),
//    TEMP( 75.0),
//    TEMP( 74.0),
//    TEMP( 73.0),
//    TEMP( 72.0),
//    TEMP( 71.0),
//    TEMP( 70.0),
//    TEMP( 69.0),
//    TEMP( 68.0),
//    TEMP( 67.0),
//    TEMP( 66.0),
//    TEMP( 65.0),
//    TEMP( 64.0),
//    TEMP( 63.0),
//    TEMP( 62.0),
//    TEMP( 61.0),
//    TEMP( 60.0),
//    TEMP( 59.0),
//    TEMP( 58.0),
//    TEMP( 57.0),
//    TEMP( 56.0),
//    TEMP( 55.0),
//    TEMP( 54.0),
//    TEMP( 53.0),
//    TEMP( 52.0),
//    TEMP( 51.0),
//    TEMP( 50.0),
//    TEMP( 49.0),
//    TEMP( 48.0),
//    TEMP( 47.0),
//    TEMP( 46.0),
//    TEMP( 45.0),
//    TEMP( 44.0),
//    TEMP( 43.0),
//    TEMP( 42.0),
//    TEMP( 41.0),
//    TEMP( 40.0),
//    TEMP( 39.0),
//    TEMP( 38.0),
//    TEMP( 37.0),
//    TEMP( 36.0),
//    TEMP( 35.0),
//    TEMP( 34.0),
//    TEMP( 33.0),
//    TEMP( 32.0),
//    TEMP( 31.0),
//    TEMP( 30.0),
//    TEMP( 29.0),
//    TEMP( 28.0),
//    TEMP( 27.0),
//    TEMP( 26.0),
//    TEMP( 25.0),
//    TEMP( 24.0),
//    TEMP( 23.0),
//    TEMP( 22.0),
//    TEMP( 21.0),
//    TEMP( 20.0),
//    TEMP( 19.0),
//    TEMP( 18.0),
//    TEMP( 17.0),
//    TEMP( 16.0),
//    TEMP( 15.0),
//    TEMP( 14.0),
//    TEMP( 13.0),
//    TEMP( 12.0),
//    TEMP( 11.0),
//    TEMP( 10.0),
//    TEMP(  9.0),
//    TEMP(  8.0),
//    TEMP(  7.0),
//    TEMP(  6.0),
//    TEMP(  5.0),
//    TEMP(  4.0),
//    TEMP(  3.0),
//    TEMP(  2.0),
//    TEMP(  1.0),
//    TEMP(  0.0),
//    TEMP( -1.0),
//    TEMP( -2.0),
//    TEMP( -3.0),
//    TEMP( -4.0),
//    TEMP( -5.0),
//    TEMP( -6.0),
//    TEMP( -7.0),
//    TEMP( -8.0),
//    TEMP( -9.0),
//    TEMP(-10.0),
//    TEMP(-11.0),
//    TEMP(-12.0),
//    TEMP(-13.0),
//    TEMP(-14.0),
//    TEMP(-15.0),
//    TEMP(-16.0),
//    TEMP(-17.0),
//    TEMP(-18.0),
//    TEMP(-19.0),
//    TEMP(-20.0),
//    TEMP(-21.0),
//    TEMP(-22.0),
//    TEMP(-23.0),
//    TEMP(-24.0),
//    TEMP(-25.0),
//    TEMP(-26.0),
//    TEMP(-27.0),
//    TEMP(-28.0),
//    TEMP(-29.0),
//    TEMP(-30.0),
//    TEMP(-31.0),
//    TEMP(-32.0),
//    TEMP(-33.0),
//    TEMP(-34.0),
//    TEMP(-35.0),
//    TEMP(-36.0),
//    TEMP(-37.0),
//    TEMP(-38.0),
//    TEMP(-39.0),
//    TEMP(-40.0),
};

#define MEMORY_LOCATION GENERAL_MEMORY
#include "memory_layout.h"
