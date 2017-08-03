/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    adc_conversion.h
 *
 * description:  Public header for ADC Conversion utility routines
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Public header for ADC Conversion utility routines
 *                       
 *
 *****************************************************************************/

#if !defined(ADC_CONVERSION_H_)
#define ADC_CONVERSION_H_

/**
 *
 * @file
 * @ingroup utils
 *
 * Public header for ADC Conversion utility routines
 *
 */


/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "mmc.h"
#include "tmi_types.h"
#include "vmi_types.h"

/******************************************************************************
 * PRIVATE MACRO DEFINITIONS
 *****************************************************************************/

#define ADC_COUNTS_TEMPERATURE(brand, scaled, temp) (TemperatureToAdcCounts((brand), scaled, BP_SCALE((temp), temperature_t, TEMPERATURE_BP)))

/******************************************************************************
 * PRIVATE TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *****************************************************************************/
extern uint16 adc_conversion_interp_1d_s16_u16 (sint16 u,
                          const FPTR sint16 *uaxis,
                          const FPTR uint16 *yaxis,
                          uint8 n_pts);

extern void     adc_voltage_split (pack_v_t pack_v_request, pack_v_t *v_pack_pos, pack_v_t *v_pack_neg);

extern uint16_t adc_hi_pos_v_to_c (pack_v_t pack_v_request);
extern uint16_t adc_hi_pos_iso_v_to_c (pack_v_t pack_v_request);
extern uint16_t adc_hi_pos_sw_v_to_c (pack_v_t pack_v_request, bool_t iso_enabled);
extern uint16_t adc_hi_neg_v_to_c (pack_v_t pack_v_request);
extern void     adc_hi_v_to_c     (pack_v_t pack_v_request, uint16_t *adc_counts_pos, uint16_t *adc_counts_neg);
/**@todo A_HV_VEHICLE_LORES_POS with ISO_DET_ENABLED not implemented */
extern uint16_t adc_lo_pos_v_to_c (pack_v_t pack_v_request);
extern uint16_t adc_lo_neg_v_to_c (pack_v_t pack_v_request);
extern void     adc_lo_v_to_c     (pack_v_t pack_v_request, uint16_t *adc_counts_pos, uint16_t *adc_counts_neg);

extern uint16_t adc_supply_v_to_c (enum vmi_supply_e supply, supply_v_t supply_v_request);

extern uint16_t TemperatureToAdcCounts(enum tmi_sensor_brand_e brand, bool_t scaled, temperature_t temp_in);

extern uint16_t adc_current_to_c(current_t current);

 /******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

#endif /* ADC_CONVERSION_H_ */
