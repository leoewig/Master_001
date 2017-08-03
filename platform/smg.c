/**
 * @file
 *
 * Routines for controlling the Supply Monitoring feature.
 *
 * $Id: smg.c 10412 2013-09-10 10:29:35Z tg10 $
 * Supply Monitoring Implementation
 */
/**
 * @ingroup SMG
 * @{
 * @file
 * @}
 */
/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "fm.h"
#include "ts.h"
//#include "ctc.h"
//#include "ctc_p.h"
#include "hardware.h"
#include "dio_cfg.h"
//#include "vmi.h"
//#include "vmi_p.h"
#include "smg.h"
#include "smg_p.h"


/******************************************************************************
 * CALIBRATABLE PARAMETERS
 *****************************************************************************/

#undef MEMORY_LOCATION
#define MEMORY_LOCATION CALIBRATION_MEMORY
#include "memory_layout.h"

CAL ts_time_t smg_isa_sensor_on_time_threshold = TIME(200);
/**<
 * @a2l{conversion,ts_time_t}
 * @a2l{min,0}
 * @a2l{max,10000}
 * @a2l{longidentifier,ON time settle period of the ISA current sensor power supply voltage}
 * @a2l{annotation,REQ_BMS_SMON_019}
 */

CAL ts_time_t smg_isa_sensor_off_time_threshold = TIME(500);
/**<
 * @a2l{conversion,ts_time_t}
 * @a2l{min,0}
 * @a2l{max,10000}
 * @a2l{longidentifier,OFF time settle period of the ISA current sensor power supply voltage}
 * @a2l{annotation,REQ_BMS_SMON_020}
 */

CAL supply_v_t smg_isa_sensor_sply_tol = SUPPLY_V(4.1);
/**<
 * @a2l{conversion,supply_v_t}
 * @a2l{min,0}
 * @a2l{max,5}
 * @a2l{longidentifier,Tolerance for the current sensor supply operating level.
                       Contributes to:
                           Fm_plat_i_sensor_supply. }
 * This tolerance applies to both high voltage level and the low voltage level.
 * @a2l{annotation,REQ_BMS_SMON_021}
 */

CAL ts_time_t smg_vtbm_can_on_time_threshold = TIME(200);
/**<
 * @a2l{conversion,ts_time_t}
 * @a2l{min,0}
 * @a2l{max,10000}
 * @a2l{longidentifier,ON time settle period of the VTBM CAN power supply voltage}
 * @a2l{annotation,REQ_BMS_SMON_021}
 */

CAL ts_time_t smg_vtbm_can_off_time_threshold = TIME(1800);
/**<
 * @a2l{conversion,ts_time_t}
 * @a2l{min,0}
 * @a2l{max,10000}
 * @a2l{longidentifier,OFF time settle period of the VTBM CAN power supply voltage}
 * @a2l{annotation,REQ_BMS_SMON_022}
 */

CAL supply_v_t smg_vtbm_can_5v_tol = SUPPLY_V(0.2);
/**<
 * @a2l{conversion,supply_v_t}
 * @a2l{min,0}
 * @a2l{max,5}
 * @a2l{longidentifier,Tolerance for VTBM CAN 5V supply operating level.
                       Contributes to:
                           Fm_plat_vtbm_can_supply. }
 * This tolerance applies to both high voltage level and the low voltage level.
 * @a2l{annotation,REQ_BMS_SMON_032}
 */

CAL supply_v_t smg_vtbm_can_gnd_tol = SUPPLY_V(0.5);
/**<
 * @a2l{conversion,supply_v_t}
 * @a2l{min,0}
 * @a2l{max,5}
 * @a2l{longidentifier,Tolerance for VTBM CAN ground supply operating level.
                       Contributes to:
                           Fm_plat_vtbm_can_ground. }
 * This tolerance applies to both high voltage level and the low voltage level.
 * @a2l{annotation,REQ_BMS_SMON_033}
 */

CAL supply_v_t smg_vpos_tol = SUPPLY_V(1.2);
/**<
 * @a2l{conversion,supply_v_t}
 * @a2l{min,0}
 * @a2l{max,5}
 * @a2l{longidentifier,Tolerance for the VPOS supply operating level.
                       Contributes to:
                           Fm_plat_vpos_12v_supply. }
 * This tolerance applies to both high voltage level and the low voltage level.
 * @a2l{annotation,REQ_BMS_SMON_034}
 */

CAL supply_v_t smg_vneg_tol = SUPPLY_V(2.0);
/**<
 * @a2l{conversion,supply_v_t}
 * @a2l{min,0}
 * @a2l{max,5}
 * @a2l{longidentifier,Tolerance for the VNEG supply operating level.
                       Contributes to:
                           Fm_plat_vneg_12v_supply. }
 * This tolerance applies to both high voltage level and the low voltage level.
 * @a2l{annotation,REQ_BMS_SMON_035}
 */

#undef MEMORY_LOCATION
#define MEMORY_LOCATION S12_MEMORY
#include "memory_layout.h"

/******************************************************************************
 * PRIVATE MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

/******************************************************************************
 * PRIVATE CONSTANTS DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

//PRIVATE CONST enum vmi_supply_status_e * smg_supply_status[smg_max_devs] =
//{
// //   &vmi_isa_12v_status,      /* smg_isa_sensor   */
// //   &vmi_vtbm_can_5v_status,  /* smg_vtbm_can_5v  */
//};
/**<
 * Array of pointers to the error status variables for the supplies
 */

PRIVATE CONST bool_t * smg_supply_enabled[smg_max_devs] =
{
    &do_rb_isa_enable,       /* smg_isa_sensor   */
    &do_rb_vtbm_can_enbl_hs, /* smg_vtbm_can_5v  */
};
/**<
 * Array of pointers to the error status variables for the supplies
 */

PRIVATE bool_t smg_supply_enabled_prev[smg_max_devs] =
{
    FALSE, /* smg_isa_sensor   */
    FALSE, /* smg_vtbm_can_5v  */
};
/**<
 * @a2l{conversion, bool_t}
 * @a2l{LongIdentifier, Previous value of smg_supply_enabled}
 */

PRIVATE CONST Fm_fault_list_e smg_supply_fault_type[smg_max_devs] =
{
    Fm_plat_i_sensor_supply, /* smg_isa_sensor   */
    Fm_plat_vtbm_can_supply, /* smg_vtbm_can_5v  */
};
/**<
 * Array of pointers to the error status variables for the supplies
 */

PRIVATE ts_time_t smg_on_timers[smg_max_devs] =
{
    TIME(0), /* smg_isa_sensor   */
    TIME(0), /* smg_vtbm_can_5v  */
};
/**<
 * @a2l{conversion, ts_time_t}
 * @a2l{LongIdentifier, Indicates the duration of the ON time settle period for the devices's supply voltage}
 */

PRIVATE ts_time_t smg_off_timers[smg_max_devs] =
{
    TIME(0), /* smg_isa_sensor   */
    TIME(0), /* smg_vtbm_can_5v  */
};
/**<
 * @a2l{conversion, ts_time_t}
 * @a2l{LongIdentifier, Indicates the duration of the OFF time settle period for the devices's supply voltage}
 */

/** Array of pointers to the ON-timer thresholds */
PRIVATE CAL ts_time_t * smg_on_thresholds[smg_max_devs] =
{
    &smg_isa_sensor_on_time_threshold, /* smg_isa_sensor   */
    &smg_vtbm_can_on_time_threshold,   /* smg_vtbm_can_5v  */
};

/** Array of pointers to the OFF-timer thresholds */
PRIVATE CAL ts_time_t * smg_off_thresholds[smg_max_devs] =
{
    &smg_isa_sensor_off_time_threshold, /* smg_isa_sensor   */
    &smg_vtbm_can_off_time_threshold,   /* smg_vtbm_can_5v  */
};

/******************************************************************************
 * VARIABLES DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * PRIVATE FUNCTION DEFINITIONS
 *****************************************************************************/

/**
 * Check whether any of the vehicle HV bus contactors have been commanded
 * closed by the BCM (MMC).
 *
 * @retval FALSE  None of the vehicle HV bus contactors have been
 *                commanded closed by the BCM.
 * @retval TRUE   Any of the vehicle HV bus contactors have been
 *                commanded closed by the BCM.
 */
PRIVATE FCODE bool_t is_any_veh_bus_cont_cmded_close_by_bcm(void)
{
    bool_t retval;

    if ((do_rb_veh_dc_pos_hs_mmc && do_rb_veh_dc_pos_ls_mmc)
    ||  (do_rb_veh_dc_neg_hs_mmc && do_rb_veh_dc_neg_ls_mmc))
    {
        retval = TRUE;
    }
    else
    {
        retval = FALSE;
    }

    return retval;
}

/**
 * Check whether any of the charger HV bus contactors have been commanded
 * closed by the BCM (MMC).
 *
 *  @retval FALSE  None of the charger HV bus contactors have been
 *                 commanded closed by the BCM.
 *  @retval TRUE   Any of the charger HV bus contactors have been
 *                 commanded closed by the BCM.
 */
PRIVATE FCODE bool_t charger_contactors_closed(void)
{
    bool_t retval;

    if ((do_rb_chg_dc_pos_hs_mmc && do_rb_chg_dc_pos_ls_mmc)
    ||  (do_rb_chg_dc_neg_hs_mmc && do_rb_chg_dc_neg_ls_mmc))
    {
        retval = TRUE;
    }
    else
    {
        retval = FALSE;
    }

    return retval;
}

/**
 * Reads the inputs and triggers the monitoring functionality for the power
 * supplies that are enabled.
 *
 * @param[in] idx  - Index of the monitored device.
 *
 * This function reads the input status of the monitored device. If the input
 * status is enabled,
 * - it reads the calibration data to get the ON/OFF settling times.
 * - runs the respective timer for the specified ON/OFF duration.
 * - it then checks whether the monitoring conditions for the fault
 *   have been met. These are:
 *  - the the ON/OFF timer has elapsed
 *  - the supply is enabled
 *  - the HVIL supply is either within limits or not required for this supplty
 */
PRIVATE FCODE void smg_mon_supply_on_events(smg_devices_e idx)
{
   // bool_t                  fault_present = FALSE;
   // bool_t                  mon_present   = FALSE;
   // Fm_fault_subtype_list_e subtype       = Fm_general_failure_information;
   //
   // if (*smg_supply_enabled[idx] == TRUE)
   // {
   //     if (*smg_supply_enabled[idx] != smg_supply_enabled_prev[idx])
   //     {
   //         /* Enable line has been toggled, kick-off the ON timer */
   //         smg_on_timers[idx] = ts_get_ms();
   //
   //         /* Update previous status */
   //         smg_supply_enabled_prev[idx] = *smg_supply_enabled[idx];
   //     }
   //
   //     /* Now check monitoring conditions */
   //     if (ts_is_elapsed_ms(ts_get_ms(), smg_on_timers[idx], *smg_on_thresholds[idx]))
   //     {
   //         mon_present = TRUE;
   //     }
   //
   //     /* Now check for the presence of the fault */
   //     if (*smg_supply_status[idx] == Vmi_supply_high)
   //     {
   //         /* Above tolerence */
   //         subtype       = Fm_max_v;
   //         fault_present = TRUE;
   //     }
   //
   //     if (*smg_supply_status[idx] == Vmi_supply_low)
   //     {
   //         /* Below tolerence */
   //         subtype       = Fm_min_v;
   //         fault_present = TRUE;
   //     }
   //
   //     if (*smg_supply_status[idx] == Vmi_supply_sensor_fault)
   //     {
   //         /* Fault with the diagnostic feedback */
   //         subtype       = Fm_invalid;
   //         fault_present = TRUE;
   //     }
   // }
   //
   // (void) fm_update_fault_filter(
   //            smg_supply_fault_type[idx],
   //            subtype,
   //            fault_present,
   //            mon_present);
}

/**
 * Reads the inputs and triggers the monitoring functionality for the power
 * supplies that are disabled.
 *
 * @param[in] idx  - Index of the monitored device.
 *
 * This function reads the input status of the monitored device. If the input
 * status is disabled,
 * - it reads the calibration data to get the ON/OFF settling times.
 * - runs the respective timer for the specified ON/OFF duration.
 * - it then checks whether the monitoring conditions for the fault
 *   have been met. These are:
 *  - the the ON/OFF timer has elapsed
 *  - the supply is enabled
 *  - the HVIL supply is either within limits or not required for this supplty
 */
PRIVATE FCODE void smg_mon_supply_off_events(smg_devices_e idx)
{
//    bool_t                  fault_present = FALSE;
//    bool_t                  mon_present   = FALSE;
//    Fm_fault_subtype_list_e subtype       = Fm_general_failure_information;
//
//    if (*smg_supply_enabled[idx] == FALSE)
//    {
//        if (*smg_supply_enabled[idx] != smg_supply_enabled_prev[idx])
//        {
//            /* Enable line has been toggled, kick-off the OFF timer */
//            smg_off_timers[idx] =  ts_get_ms();
//
//            /* Update previous status */
//            smg_supply_enabled_prev[idx] = *smg_supply_enabled[idx];
//        }
//
//        /* Now check monitoring conditions */
//        if (ts_is_elapsed_ms(ts_get_ms(), smg_off_timers[idx], *smg_off_thresholds[idx]))
//        {
//            mon_present = TRUE;
//        }
//
//        /* Now check for the presence of the fault */
//        if (*smg_supply_status[idx] == Vmi_supply_high)
//        {
//            /* Above tolerance */
//            subtype       = Fm_unexpected_v;
//            fault_present = TRUE;
//        }
//    }
//
//    (void) fm_update_fault_filter(
//               smg_supply_fault_type[idx],
//               subtype,
//               fault_present,
//               mon_present);
}

/**
 * Diagnose the status of contactors on a HV bus.
 *
 * @param[in] mon_present  Indicates whether the monitoring conditions
 *                         have been satisfied
 * @param[in] exp_cont_psn  Expected position of the HV bus contactors.
 * @param[in] fault_type  Fault type to be registered, incase the expected
 *               position of the HV bus contactors is faulty.
 *
 * This function diagnoses the status of the HV bus (for e.g. vehicle HV bus,
 * charger HV bus) contactors and sets a fault, incase the expected position of
 * the HV bus contactors is faulty.
 *
 * - If is_cont_cmded_close_by_bcm is TRUE check the HVIL status.
 * - If it is not intact,
 *   - if exp_cont_psn is Ctc_cont_open, register the fault fault_type with the
 *     subtype Fm_unable_to_close.
 *   - if exp_cont_psn is Ctc_cont_closed, register the fault fault_type with the
 *     subtype Fm_interlock_broken.
 * - If it is intact, clear the fault fault_type.
 */
//PRIVATE FCODE void smg_fault_filter_12v_power(
//                       bool_t                mon_present,
//                       enum Ctc_cont_state_e exp_cont_psn,
//                       Fm_fault_list_e       fault_type)
//{
//    bool_t                  fault_present = FALSE;
//    Fm_fault_subtype_list_e subtype       = Fm_general_failure_information;
//
//    if (mon_present == TRUE)
//    {
//        if (vmi_12v_power_status != Vmi_supply_ok)
//        {
//            if (exp_cont_psn == Ctc_cont_open)
//            {
//                subtype       = Fm_unable_to_close;
//                fault_present = TRUE;
//            }
//            else
//            {
//                if (exp_cont_psn == Ctc_cont_closed)
//                {
//                    subtype       = Fm_interlock_broken;
//                    fault_present = TRUE;
//                }
//            }
//        }
//    }
//
//    (void) fm_update_fault_filter(
//               fault_type,
//               subtype,
//               fault_present,
//               mon_present);
//}

/**
 * Monitor the HVIL status. This function calls smg_fault_filter_12v_power()
 * to diagnose the vehicle HV bus contactors and the charger HV bus
 * contactors.
 */
PRIVATE FCODE void mon_12v_power_status(void)
{
    /* diagnose the vehicle HV bus contactors */
//    //smg_fault_filter_12v_power(TRUE,                        ctc_cont_veh_psn_expect, Fm_veh_interlock_error);
//	smg_fault_filter_12v_power(TRUE,                        ctc_dclink_contactors_closed, Fm_veh_interlock_error);
//
//    /* diagnose the charger HV bus contactors */
//    //smg_fault_filter_12v_power(charger_contactors_closed(), ctc_cont_chg_psn_expect, Fm_charger_interlock_error);
//	smg_fault_filter_12v_power(charger_contactors_closed(), ctc_charger_contactors_closed, Fm_charger_interlock_error);
}

/**
 * Diagnose the status of contactors on a HV bus.
 *
 * This function diagnoses the status of the HV bus (for e.g. vehicle HV bus,
 * charger HV bus) contactors and sets a fault, incase the expected position of
 * the HV bus contactors is faulty.
 *
 * - If is_cont_cmded_close_by_bcm is TRUE check the HVIL status.
 * - If it is not intact,
 *   - if exp_cont_psn is Ctc_cont_open, register the fault fault_type with the
 *     subtype Fm_unable_to_close.
 *   - if exp_cont_psn is Ctc_cont_closed, register the fault fault_type with the
 *     subtype Fm_interlock_broken.
 * - If it is intact, clear the fault fault_type.
 */
PRIVATE FCODE void smg_fault_filter_vtbm_can_gnd()
{
//    bool_t                  fault_present = FALSE;
//    bool_t                  mon_present   = TRUE;
//    Fm_fault_subtype_list_e subtype       = Fm_general_failure_information;
//
//    if (vmi_vtbm_can_gnd_status == Vmi_supply_high)
//    {
//        /* Above tolerance */
//        subtype       = Fm_unexpected_v;
//        fault_present = TRUE;
//    }
//
//   // (void) fm_update_fault_filter(
//  //             Fm_plat_vtbm_can_ground,
//   //            subtype,
//   //            fault_present,
   //            mon_present);
}

/**
 * Diagnose the status of contactors on a HV bus.
 *
 * This function diagnoses the status of the HV bus (for e.g. vehicle HV bus,
 * charger HV bus) contactors and sets a fault, incase the expected position of
 * the HV bus contactors is faulty.
 *
 * - If is_cont_cmded_close_by_bcm is TRUE check the HVIL status.
 * - If it is not intact,
 *   - if exp_cont_psn is Ctc_cont_open, register the fault fault_type with the
 *     subtype Fm_unable_to_close.
 *   - if exp_cont_psn is Ctc_cont_closed, register the fault fault_type with the
 *     subtype Fm_interlock_broken.
 * - If it is intact, clear the fault fault_type.
 */
PRIVATE FCODE void smg_fault_filter_vpos()
{
//   bool_t                  fault_present = FALSE;
//   Fm_fault_subtype_list_e subtype       = Fm_general_failure_information;
//
//   if (vmi_vpos_status == Vmi_supply_high)
//   {
//       /* Above tolerance */
//       subtype       = Fm_max_v;
//       fault_present = TRUE;
//   }
//
//   if (vmi_vpos_status == Vmi_supply_low)
//   {
//       /* Below tolerance */
//       subtype       = Fm_min_v;
//       fault_present = TRUE;
//   }
//
//   (void) fm_update_fault_filter(
//              Fm_plat_vpos_12v_supply,
//              subtype,
//              fault_present,
//              vmi_mon_cond_vpos);
}

/**
 * Diagnose the status of contactors on a HV bus.
 *
 * This function diagnoses the status of the HV bus (for e.g. vehicle HV bus,
 * charger HV bus) contactors and sets a fault, incase the expected position of
 * the HV bus contactors is faulty.
 *
 * - If is_cont_cmded_close_by_bcm is TRUE check the HVIL status.
 * - If it is not intact,
 *   - if exp_cont_psn is Ctc_cont_open, register the fault fault_type with the
 *     subtype Fm_unable_to_close.
 *   - if exp_cont_psn is Ctc_cont_closed, register the fault fault_type with the
 *     subtype Fm_interlock_broken.
 * - If it is intact, clear the fault fault_type.
 */
PRIVATE FCODE void smg_fault_filter_vneg()
{
//    bool_t                  fault_present = FALSE;
//    Fm_fault_subtype_list_e subtype       = Fm_general_failure_information;
//
//    if (vmi_vneg_status == Vmi_supply_high)
//    {
//        /* Above tolerance */
//        subtype       = Fm_max_v;
//        fault_present = TRUE;
//    }
//
//    if (vmi_vneg_status == Vmi_supply_low)
//    {
//        /* Below tolerance */
//        subtype       = Fm_min_v;
//        fault_present = TRUE;
//    }
//
//    (void) fm_update_fault_filter(
//               Fm_plat_vneg_12v_supply,
//               subtype,
//               fault_present,
//               vmi_mon_cond_vneg);
}

/*****************************************************************************
 * PUBLIC FUNCTION DEFINITIONS
 *****************************************************************************/

/**
 * Reads the inputs and monitors the power supplies based on the inputs
 *
 * Description:
 * - This function reads the input voltage level of all the devices that are
 *   monitored. This function is called periodically by the scheduler.
 * - To monitor the HVIL status, it calls mon_12v_power_status().
 * - For monitoring the rest of the devices, it calls the functions
 *   smg_mon_supply_on_events() (to monitor the input power supplies that
 *   are enabled) and smg_mon_supply_off_events() (to monitor the input power
 *   supplies that are disabled).
 */
FCODE void smg_read_inputs(void)
{
//    mon_12v_power_status();
//
//    smg_fault_filter_vpos();
//    smg_fault_filter_vneg();
//    smg_fault_filter_vtbm_can_gnd();
//
//    smg_mon_supply_on_events (smg_isa_sensor);
//    smg_mon_supply_off_events(smg_isa_sensor);
//
//    smg_mon_supply_on_events (smg_vtbm_can_5v);
//    smg_mon_supply_off_events(smg_vtbm_can_5v);
}
