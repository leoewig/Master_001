/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   smg.h
 *
 * description:  
 * Private header file for SMG - This file defines all the parameters used
 * internally within the SMG feature.
 * Supply Monitoring Feature
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
/* finally, read through this generic design once */
#if !defined(SMG_H)
#define      SMG_H
/**
 * @file
 *
 * Header file for Supply Monitoring Feature
 *
 * $Id: smg.h 11312 2013-11-15 10:19:18Z tg10 $
 */
/**
 * Supply Monitoring Feature
 *
 * @par Requirements:
 * @par Requirement Traceability:
 *
 * This "feature" has been merged with the VMI feature since the original system design, hence
 * the requirements traceablility can be found in VMI.H.
 *
 * @par Design Details:
 * The SMG module is used by the MMC. This module provides a facility to check
 * whether the input supply voltages to the devices in the MMC are within the
 * range and flags faults if this is not the case. It basically monitors the
 * voltage on:
 * -# Current sensor power supply
 * -# VTBM CAN power supply.(both 5V & ground)
 * -# Vpos
 * -# Vneg
 * -# 12V control status(aka HVIL status)
 * -# 12V logic status
 *
 * This module uses smg_read_inputs() function to read all the required inputs
 * and to check the voltage level of the monitored devices. This function is
 * called periodically by the scheduler. The smg_read_inputs() function
 * internally calls the private functions smg_mon_supply_on_events(),
 * smg_mon_supply_off_events() and mon_12v_power_status() to monitor the power
 * supplies.
 *
 * The general principle of SMG is that, except for the HVIL
 * status, whenever the input state of a monitored device is enabled/disabled
 * the respective ON/OFF timer is run. When the timer elapses, it checks whether
 * the input supply voltage level is within the specified limits, otherwise it
 * registers the respective fault. For
 * monitoring the HVIL status, it calls the private function mon_12v_power_status(). The
 * monitoring of the HVIL status is primarily triggered by the condition "any
 * vehicle/charger HV bus contactors commanded closed by the BCM(MMC)".
 *
 * For monitoring the HVIL status, it calls mon_12v_power_status() which calls
 * smg_fault_filter_12v_power(). The function smg_fault_filter_12v_power() is
 * used to diagose the vehicle HV bus contactors and the charger HV bus
 * contactors.
 *
 * @defgroup SMG Supply Monitoring
 * @ingroup platform
 * @{
 * @file
 * @}
 */

#include "basic_types.h"
#include "fixedpoint.h"
#include "mmc.h"
#include "fm.h"
#include "smg_types.h"

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern CAL ts_time_t  smg_isa_sensor_on_time_threshold;
extern CAL ts_time_t  smg_isa_sensor_off_time_threshold;

extern CAL ts_time_t  smg_vtbm_can_on_time_threshold;
extern CAL ts_time_t  smg_vtbm_can_off_time_threshold;

extern CAL supply_v_t smg_isa_sensor_sply_tol;
extern CAL supply_v_t smg_vtbm_can_5v_tol;
extern CAL supply_v_t smg_vtbm_can_gnd_tol;
extern CAL supply_v_t smg_vpos_tol;
extern CAL supply_v_t smg_vneg_tol;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/* Description is in the source file */
FCODE void smg_read_inputs(void);

#endif