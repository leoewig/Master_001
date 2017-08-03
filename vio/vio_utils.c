/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      vio_utils.c
 *
 * description:  
 *  Routines to format transmitted CAN messages.
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/


#include <mmc.h>
#include "vio_p.h"      /* private header for this module */

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/*
 * PUBLIC DATA (documented in header)
 */
 //LDT
#define RPM_BP 0
/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/**
 * Utility routine to scale BMS scaled data into Vehicle CAN scaling
 *
 * @param[in] value - The data value to be converted
 * @param[in] can_sf - The number of binary points in the CAN representation
 * @param[in] can_offset - The offset to be applied to the CAN representation
 * @param[in] can_max - The maximum value allowed for the CAN data
 * @param[in] can_min - The minimum value allowed for the CAN data
 * @param[in] mmc_divisor - The scale factor for the MMC data
 * @retval The CAN representation of the data
 */
uint16_t vio_scale_down_s32(int32_t value,
                            int32_t can_sf,
                            int32_t can_offset,
                            int32_t can_min,
                            int32_t can_max,
                            int32_t mmc_divisor)
{
    /* There doesn't seem to be a precision-preserving fixed-point routine that
     * does what we want here. */
    int32_t  tmp32 = value;

    /* Clip this into the validity range for the signal */
    tmp32 = clips32(tmp32, can_min, can_max );

    /* subtracting the offset should make this a non-negative number */
    tmp32 = subs32(tmp32, can_offset, 0, 0, 0);

    /* Now apply the scale factor */
    tmp32 = divs32(tmp32, can_sf, 0, 0, 0);

    /* Remove the internal MMC scaling */
    tmp32 = divs32 (tmp32, mmc_divisor, 0, 0, 0);

    return (uint16_t)tmp32;
}

/**
 * Utility routine to scale BMS scaled data into Vehicle CAN scaling
 *
 * @param[in] value - The data value to be converted
 * @param[in] can_sf - The number of binary points in the CAN representation
 * @param[in] can_offset - The offset to be applied to the CAN representation
 * @param[in] can_max - The maximum value allowed for the CAN data
 * @param[in] can_min - The minimum value allowed for the CAN data
 * @param[in] mmc_divisor - The scale factor for the MMC data
 * @retval The CAN representation of the data
 */
int8_t vio_scale_up_s32_s8(int32_t value,
                         int32_t can_sf,
                         int32_t can_offset,
                         int32_t can_min,
                         int32_t can_max,
                         int32_t mmc_divisor)
{
    /* There doesn't seem to be a precision-preserving fixed-point routine that
     * does what we want here. */
    int32_t tmp32 = value;

    /* Clip this into the validity range for the signal */
    tmp32 = clips32(tmp32, can_min, can_max );

    /* subtracting the offset should make this a non-negative number */
    tmp32 = subs32(tmp32, can_offset, 0, 0, 0);

    /* Now apply the scale factor */
    tmp32 = muls32(tmp32, can_sf, 0, 0, 0);

    /* Remove the internal MMC scaling */
    tmp32 = divs32(tmp32, mmc_divisor, 0, 0, 0);

    return (int8_t) tmp32;
}

/**
 * Utility routine to scale BMS scaled data into Vehicle CAN scaling
 *
 * @param[in] value - The data value to be converted
 * @param[in] can_sf - The number of binary points in the CAN representation
 * @param[in] can_offset - The offset to be applied to the CAN representation
 * @param[in] can_max - The maximum value allowed for the CAN data
 * @param[in] can_min - The minimum value allowed for the CAN data
 * @param[in] mmc_divisor - The scale factor for the MMC data
 * @retval The CAN representation of the data
 */
uint16_t vio_scale_up_s32(int32_t value,
                          int32_t can_sf,
                          int32_t can_offset,
                          int32_t can_min,
                          int32_t can_max,
                          int32_t mmc_divisor)
{
    /* There doesn't seem to be a precision-preserving fixed-point routine that
     * does what we want here. */
    int32_t tmp32 = value;

    /* Clip this into the validity range for the signal */
    tmp32 = clips32(tmp32, can_min, can_max );

    /* subtracting the offset should make this a non-negative number */
    tmp32 = subs32(tmp32, can_offset, 0, 0, 0);

    /* Now apply the scale factor */
    tmp32 = muls32(tmp32, can_sf, 0, 0, 0);

    /* Remove the internal MMC scaling */
    tmp32 = divs32(tmp32, mmc_divisor, 0, 0, 0);

    return (uint16_t)tmp32;
}

/**
 * Utility routine to scale BMS scaled data into Vehicle CAN scaling
 *
 * @param[in] value - The data value to be converted
 * @param[in] can_sf - The number of binary points in the CAN representation
 * @param[in] can_offset - The offset to be applied to the CAN representation
 * @param[in] can_max - The maximum value allowed for the CAN data
 * @param[in] can_min - The minimum value allowed for the CAN data
 * @param[in] mmc_divisor - The scale factor for the MMC data
 * @retval The CAN representation of the data
 */
uint16_t vio_scale_down_u32(uint32_t value,
                            uint32_t can_sf,
                            uint32_t can_offset,
                            uint32_t can_min,
                            uint32_t can_max,
                            uint32_t mmc_divisor)
{
    /* There doesn't seem to be a precision-preserving fixed-point routine that
     * does what we want here. */
    uint32_t tmp32 = value;

    /* Clip this into the validity range for the signal */
    tmp32 = clipu32(tmp32, can_min, can_max );

    /* subtracting the offset should make this a non-negative number */
    tmp32 = subu32(tmp32, can_offset, 0, 0, 0);

    /* Now apply the scale factor */
    tmp32 = divu32(tmp32, can_sf, 0, 0, 0);

    /* Remove the internal MMC scaling */
    tmp32 = divu32 (tmp32, mmc_divisor, 0, 0, 0);

    return (uint16_t)tmp32;
}

/**
 * Utility routine to scale BMS scaled data into Vehicle CAN scaling
 *
 * @param[in] value - The data value to be converted
 * @param[in] can_sf - The number of binary points in the CAN representation
 * @param[in] can_offset - The offset to be applied to the CAN representation
 * @param[in] can_max - The maximum value allowed for the CAN data
 * @param[in] can_min - The minimum value allowed for the CAN data
 * @param[in] mmc_divisor - The scale factor for the MMC data
 * @retval The CAN representation of the data
 */
uint16_t vio_scale_up_u32(uint32_t value,
                          uint32_t can_sf,
                          uint32_t can_offset,
                          uint32_t can_min,
                          uint32_t can_max,
                          uint32_t mmc_divisor)
{
    /* There doesn't seem to be a precision-preserving fixed-point routine that
     * does what we want here. */
    uint32_t tmp32 = value;

    /* Clip this into the validity range for the signal */
    tmp32 = clipu32(tmp32, can_min, can_max );

    /* subtracting the offset should make this a non-negative number */
    tmp32 = subu32(tmp32, can_offset, 0, 0, 0);

    /* Now apply the scale factor */
    tmp32 = mulu32(tmp32, can_sf, 0, 0, 0);

    /* Remove the internal MMC scaling */
    tmp32 = divu32(tmp32, mmc_divisor, 0, 0, 0);

    return (uint16_t)tmp32;
}

uint16_t vio_scale_i_actual(current_t i_ma)
{
    return vio_scale_up_s32
       (i_ma,
        (int32_t) IACTUAL_FACTOR,
        (int32_t) IACTUAL_OFFSET * CURRENT_SF,
        IACTUAL_MIN * CURRENT_SF,
        IACTUAL_MAX * CURRENT_SF,
        MILLIAMP_IN_AMP);
}

uint8_t vio_scale_i_model(current_t i_ma)
{
    return (uint8_t) vio_scale_up_s32
       (i_ma,
        (int32_t) IMODEL_FACTOR,
        (int32_t) IMODEL_OFFSET * CURRENT_SF,
        IMODEL_MIN * CURRENT_SF,
        IMODEL_MAX * CURRENT_SF,
        MILLIAMP_IN_AMP);
}

uint16_t vio_scale_i_charge(current_t i_ma)
{
    return vio_scale_up_s32
       (i_ma,
        (int32_t) ICHARGE_FACTOR,
        (int32_t) ICHARGE_OFFSET * CURRENT_SF,
        ICHARGE_MIN * CURRENT_SF,
        ICHARGE_MAX * CURRENT_SF,
        MILLIAMP_IN_AMP);
}

uint16_t vio_scale_p_actual(current_t current, pack_v_t voltage)
{
    int32_t  power;
    uint32_t tmp;

    power = muls32(current, (int32_t) voltage, CURRENT_BP, PACK_V_BP, POWER_BP);
    tmp   = abs32(power);

    return vio_scale_down_u32
       ((uint32_t) tmp,
        (uint32_t) PACTUAL_FACTOR,
        (uint32_t) PACTUAL_OFFSET * (uint32_t) POWER_SF,
        (uint32_t) PACTUAL_MIN    * (uint32_t) POWER_SF,
        (uint32_t) PACTUAL_MAX    * (uint32_t) POWER_SF,
        (uint32_t) POWER_SF);
}

uint16_t vio_scale_p_charge(current_t current, pack_v_t voltage)
{
    int32_t power;

    power = muls32(current, (int32_t) voltage, CURRENT_BP, PACK_V_BP, POWER_BP);

    return vio_scale_down_u32
       ((uint32_t) power,
        (uint32_t) PCHARGE_FACTOR,
        (uint32_t) PCHARGE_OFFSET * (uint32_t) POWER_SF,
        (uint32_t) PCHARGE_MIN    * (uint32_t) POWER_SF,
        (uint32_t) PCHARGE_MAX    * (uint32_t) POWER_SF,
        (uint32_t) POWER_SF);
}

uint8_t vio_scale_bms_temperature(temperature_t value)
{
    return (uint8_t) vio_scale_up_s32
       ((int32_t) value,
        (int32_t) TBMS_FACTOR,
        (int32_t) TBMS_OFFSET * TEMPERATURE_SF,
        (int32_t) TBMS_MIN    * TEMPERATURE_SF,
        (int32_t) TBMS_MAX    * TEMPERATURE_SF,
        (int32_t) TEMPERATURE_SF);
}

int8_t vio_scale_model_temperature(temperature_t value)
{
    return vio_scale_up_s32_s8
       ((int32_t) value,
        (int32_t) TMODEL_FACTOR,
        (int32_t) TMODEL_OFFSET * TEMPERATURE_SF,
        (int32_t) TMODEL_MIN    * TEMPERATURE_SF,
        (int32_t) TMODEL_MAX    * TEMPERATURE_SF,
        (int32_t) TEMPERATURE_SF);
}

uint16_t vio_scale_cell_temperature(temperature_t value)
{
    return vio_scale_up_s32
       ((int32_t) value,
        (int32_t) TCELL_FACTOR,
        (int32_t) TCELL_OFFSET * TEMPERATURE_SF,
        (int32_t) TCELL_MIN    * TEMPERATURE_SF,
        (int32_t) TCELL_MAX    * TEMPERATURE_SF,
        (int32_t) TEMPERATURE_SF);
}

uint8_t vio_scale_percent_down(percent_t percent, uint16_t factor)
{
    return (uint8_t) vio_scale_down_u32
       ((uint32_t) percent,
        (uint32_t) factor,
        (uint32_t) PERCENT_OFFSET * PERCENT_SF,
        (uint32_t) PERCENT_MIN * PERCENT_SF,
        (uint32_t) PERCENT_MAX * PERCENT_SF,
        (uint32_t) PERCENT_SF);
}

/**
 * @param[in] percent the percentage to scale for output on CAN
 * @param[in] factor  the physical -> representation scaling factor for CAN 
 * @param[in] sub_offset  the value to subtract from percent to translate to 
 * the range of the CAN representation */
uint8_t vio_scale_percent_up(percent_t percent, uint16_t factor, percent_t sub_offset)
{
    return (uint8_t) vio_scale_up_u32
       ((uint32_t) percent,
        (uint32_t) factor,
        (uint32_t)sub_offset,
        ((uint32_t) PERCENT_MIN * PERCENT_SF) + sub_offset,
        ((uint32_t) PERCENT_MAX * PERCENT_SF) + sub_offset,
        (uint32_t) PERCENT_SF);
}

int16_t vio_scale_pack_voltage(pack_v_t voltage)
{
    uint32_t abs_voltage;
    bool_t   negative_voltage;
    uint16_t can_voltage_u16;
     int16_t can_voltage;

    abs_voltage = abs16(voltage);

    if (voltage >= PACK_V(0))
    {
        negative_voltage = FALSE;
    }
    else
    {
        negative_voltage = TRUE;
    }

    can_voltage_u16 = vio_scale_up_s32
       ((int32_t) abs_voltage,
        (int32_t) DCLINK_FACTOR,
        (int32_t) DCLINK_OFFSET * (int32_t) PACKV_SF,
        (int32_t) DCLINK_MIN * (int32_t) PACKV_SF,
        (int32_t) DCLINK_MAX * (int32_t) PACKV_SF,
        (int32_t) PACKV_SF);

    if (!negative_voltage)
    {
        can_voltage = (int16_t) can_voltage_u16;
    }
    else
    {
        can_voltage = -(int16_t) can_voltage_u16;
    }

    return can_voltage;
}

uint16_t vio_scale_vhybrid(pack_v_t voltage)
{
    return vio_scale_up_s32
       ((int32_t) voltage,
        (int32_t) VHYBRID_FACTOR,
        (int32_t) VHYBRID_OFFSET * (int32_t) PACKV_SF,
        (int32_t) VHYBRID_MIN * (int32_t) PACKV_SF,
        (int32_t) VHYBRID_MAX * (int32_t) PACKV_SF,
        (int32_t) PACKV_SF);
}

/**
 * Convert a #cell_v_t to the scaling needed for the vehicle bus
 * cell_v_t is bin 13 (#CELL_V_BP) volts
 * The Vehicle CAN messages are in mV with the range 0 to ~8000mV.
 * This, happily, works out as a unity conversion because cell_v_t has
 * the range 0 to ~8V
 */
//uint16_t vio_scale_cell_voltage(cell_v_t voltage)
//{
//    return voltage;
//}

/**
 * Convert a #percent_t SOCc to the scaling needed for the vehicle bus
 */
uint16_t vio_scale_soc_c(percent_t value)
{
    return (uint16_t)vio_scale_up_s32
       ((int32_t) value,
        (int32_t) SOC_C_FACTOR,
        (int32_t) SOC_C_OFFSET * (int32_t) PERCENT_SF,
        (int32_t) SOC_C_MIN    * (int32_t) PERCENT_SF,
        (int32_t) SOC_C_MAX    * (int32_t) PERCENT_SF,
        (int32_t) PERCENT_SF);
}

//NB
uint16_t vio_scale_v_pack(pack_v_t voltage)
{
    return vio_scale_up_s32
       ((int32_t) voltage,
        (int32_t) VSYS_FACTOR,       //10
        (int32_t) VSYS_OFFSET * (int32_t) PACKV_SF,
        (int32_t) VSYS_MIN    * (int32_t) PACKV_SF,
        (int32_t) VSYS_MAX    * (int32_t) PACKV_SF,      //500*16
        (int32_t) PACKV_SF);    //16
}

uint16_t vio_scale_i_bus(current_t i_ma)
{
    return vio_scale_up_s32
       (i_ma,
        (int32_t) ISYS_FACTOR,
        (int32_t) ISYS_OFFSET * CURRENT_SF,
        (int32_t) ISYS_MIN    * CURRENT_SF,
        (int32_t) ISYS_MAX    * CURRENT_SF,
        MILLIAMP_IN_AMP);
}

//uint8_t vio_scale_cell_temperature(temperature_t value)
//{
//    return (uint8_t)vio_scale_up_s32
//       ((int32_t) value,
//        (int32_t) TCELL_FACTOR,
//        (int32_t) TCELL_OFFSET * (int32_t) TEMPERATURE_SF,
//        (int32_t) TCELL_MIN    * (int32_t) TEMPERATURE_SF,
//        (int32_t) TCELL_MAX    * (int32_t) TEMPERATURE_SF,
//        (int32_t) TEMPERATURE_SF);
//}

uint8_t vio_scale_air_temperature(temperature_t value)
{
    return (uint8_t)vio_scale_up_s32
       ((int32_t) value,
        (int32_t) TAIR_FACTOR,
        (int32_t) TAIR_OFFSET * (int32_t) TEMPERATURE_SF,
        (int32_t) TAIR_MIN    * (int32_t) TEMPERATURE_SF,
        (int32_t) TAIR_MAX    * (int32_t) TEMPERATURE_SF,
        (int32_t) TEMPERATURE_SF);
}

uint16_t vio_scale_cell_voltage(cell_v_t voltage)
{
    return vio_scale_up_s32
       ((int32_t) voltage,
        (int32_t) VCELL_FACTOR,
        (int32_t) VCELL_OFFSET * (int32_t) CELLV_SF,
        (int32_t) VCELL_MIN    * (int32_t) CELLV_SF,
        (int32_t) VCELL_MAX    * (int32_t) CELLV_SF,
        (int32_t) CELLV_SF);
}

uint8_t vio_scale_soc(percent_t value)
{
    return (uint8_t)vio_scale_up_u32
       ((uint32_t) value,
        (uint32_t) SOC_FACTOR,
        (uint32_t) SOC_OFFSET * (uint32_t) PERCENT_SF,
        (uint32_t) SOC_MIN    * (uint32_t) PERCENT_SF,
        (uint32_t) SOC_MAX    * (uint32_t) PERCENT_SF,
        (uint32_t) PERCENT_SF);
}

uint8_t vio_scale_soh(percent_t value)
{
    return (uint8_t)vio_scale_up_u32
       ((uint32_t) value,
        (uint32_t) SOH_FACTOR,
        (uint32_t) SOH_OFFSET * (uint32_t) PERCENT_SF,
        (uint32_t) SOH_MIN    * (uint32_t) PERCENT_SF,
        (uint32_t) SOH_MAX    * (uint32_t) PERCENT_SF,
        (uint32_t) PERCENT_SF);
}

uint16_t vio_scale_comp_speed(rpm_t value)
{
    return vio_scale_up_u32
       ((uint32_t) value,
        (uint32_t) COMP_SPD_FACTOR,
        (uint32_t) COMP_SPD_OFFSET  * (uint32_t) RPM_SF,
        (uint32_t) COMP_SPD_MIN     * (uint32_t) RPM_SF,
        (uint32_t) COMP_SPD_MAX     * (uint32_t) RPM_SF,
        (uint32_t) RPM_SF);
}

uint16_t vio_scale_avail_energy(energy_wh_t value)
{
    return vio_scale_down_u32
       ((uint32_t) value,
        (uint32_t) EPACK_FACTOR,
        (uint32_t) EPACK_OFFSET  * (uint32_t) ENERGY_SF,
        (uint32_t) EPACK_MIN     * (uint32_t) ENERGY_SF,
        (uint32_t) EPACK_MAX     * (uint32_t) ENERGY_SF,
        (uint32_t) ENERGY_SF);
}

uint16_t vio_scale_cell_resistance(milliohm_t value)
{
    return vio_scale_down_u32
       ((uint32_t) value*100UL,
        (uint32_t) RCELL_FACTOR,
        (uint32_t) RCELL_OFFSET  * (uint32_t) MILLIOHM_SF,
        (uint32_t) RCELL_MIN     * (uint32_t) MILLIOHM_SF,
        (uint32_t) RCELL_MAX     * (uint32_t) MILLIOHM_SF,
        (uint32_t) MILLIOHM_SF);
}

/**
* Scales the received ambient temperature.
* @param[in] rx_data - received CAN data
* @param[out] t_out  - pointer to the scaled temperature output variable
*                      (written only if temperature is within range)
* @retval TRUE  if the scaled temperature is within range
* @retval FALSE if the scaled temperature is not within range
*/
bool_t vio_scale_rx_ambient_temperature(uint8_t rx_data, temperature_t* t_out)
{
    bool_t in_range;
    int32_t tmp32;

    /* upscale the received value to temperature_t */
    tmp32 = BP_SCALE(rx_data, int32_t, TEMPERATURE_BP);

    /* remove VIO scale factor */
    tmp32 = (int32_t)ndivs32(tmp32, (int16_t)AMB_T_FACT_DENOM, TEMPERATURE_BP, 0, TEMPERATURE_BP);

    /* add offset */
    tmp32 = adds32(tmp32, (int32_t)TEMP(AMB_T_OFFSET), TEMPERATURE_BP, TEMPERATURE_BP, TEMPERATURE_BP);

    /* verify range */
    in_range = (bool_t)((tmp32 >= TEMP(AMB_T_MIN))
        && (tmp32 <= TEMP(AMB_T_MAX)));

    if (in_range)
    {
        *t_out = (temperature_t)tmp32;
    }

    return in_range;
}

/**
 * Scales the received charger output voltage.
 * @param[in] rx_data - received CAN data
 * @param[out] v_out  - pointer to the scaled voltage output variable
 *                      (written only if voltage is within range)
 * @retval TRUE  if the scaled voltage is within range
 * @retval FALSE if the scaled voltage is not within range
 */
bool_t vio_scale_rx_chg_voltage(uint16_t rx_data, pack_v_t* v_out)
{
    bool_t in_range;
    int32_t tmp32;

    /* upscale the received value to the pack voltage */
    tmp32 = BP_SCALE(rx_data, int32_t, PACK_V_BP);
    
    /* remove VIO scale factor */
    tmp32 = (int32_t)ndivs32(tmp32, (int16_t)CHG_V_FACT_DENOM, PACK_V_BP, 0, PACK_V_BP);

    /* add offset */
    tmp32 = adds32(tmp32, (int32_t)PACK_V(CHG_V_OFFSET), PACK_V_BP, PACK_V_BP, PACK_V_BP);

    /* verify range */
    in_range = (bool_t)((tmp32 >= PACK_V(CHG_V_MIN)) && (tmp32 <= PACK_V(CHG_V_MAX)));

    if (in_range)
    {
        *v_out = (pack_v_t)tmp32;
    }

    return in_range;
}

/**
 * Scales the received charger output current.
 * @param[in] rx_data - received CAN data
 * @param[out] i_out  - pointer to the scaled current output variable
 *                      (written only if current is within range)
 * @retval TRUE  if the scaled current is within range
 * @retval FALSE if the scaled current is not within range
 */
bool_t vio_scale_rx_chg_current(uint16_t rx_data, current_t* i_out)
{
    bool_t in_range;
    int32_t tmp32;

    /* upscale the received value to current_t */
    tmp32 = (int32_t)rx_data * 1000;
    
    /* remove VIO scale factor */
    tmp32 = divs32(tmp32, (int32_t)CHG_I_FACT_DENOM, 0, 0, 0);

    /* add offset */
    tmp32 = adds32(tmp32, (int32_t)CHG_I_OFFSET * 1000, 0, 0, 0);

    /* verify range */
    in_range = (bool_t)((tmp32 >= ((int32_t)CHG_I_MIN * 1000)) 
                     && (tmp32 <= ((int32_t)CHG_I_MAX * 1000)));

    if (in_range)
    {
        *i_out = (current_t)tmp32;
    }

    return in_range;
}

/**
 * Scales the received charger temperature.
 * @param[in] rx_data - received CAN data
 * @param[out] t_out  - pointer to the scaled temperature output variable
 *                      (written only if temperature is within range)
 * @retval TRUE  if the scaled temperature is within range
 * @retval FALSE if the scaled temperature is not within range
 */
bool_t vio_scale_rx_chg_temperature(uint8_t rx_data, low_res_temp_t* t_out)
{
    bool_t in_range;
    int32_t tmp32;

    /* upscale the received value to low_res_temp_t */
    tmp32 = BP_SCALE(rx_data, int32_t, LOW_RES_TEMP_BP);
    
    /* remove VIO scale factor */
    tmp32 = (int32_t)ndivs32(tmp32, (int16_t)CHG_T_FACT_DENOM, LOW_RES_TEMP_BP, 0, LOW_RES_TEMP_BP);

    /* add offset */
    tmp32 = adds32(tmp32, (int32_t)LOW_RES_TEMP(CHG_T_OFFSET), LOW_RES_TEMP_BP, LOW_RES_TEMP_BP, LOW_RES_TEMP_BP);

    /* verify range */
    in_range = (bool_t)((tmp32 >= LOW_RES_TEMP(CHG_T_MIN)) 
                     && (tmp32 <= LOW_RES_TEMP(CHG_T_MAX)));

    if (in_range)
    {
        *t_out = (low_res_temp_t)tmp32;
    }

    return in_range;
}

uint8_t vio_calc_data_xor_code(const uint8_t* data, uint8_t data_len)
{
    uint8_t xor_code = 0u;
    uint8_t i;

    for (i = 0u; i < data_len; i++)
    {
        /*lint -e415 */
        xor_code ^= data[i];
    }

    return xor_code;
}

