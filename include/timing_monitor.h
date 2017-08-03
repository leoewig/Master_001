/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    timing_monitor.h
 *
 * description:  
 * Timing monitor header
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
/**
 * Timing monitor header
 * @file
 * @ingroup timing_monitor
 *
 */
/**
 * Timing monitor
 *
 * Timing monitor provides a means to output a timing signal on the 
 * unused VTBM acquisition output.
 *
 * Use as follows:
 * + add a member to the #timing_monitor_e enumeration and update the
 *   timing_monitor_e_vtab in timing_monitor.a2l accordingly
 * + Call #TIMING_MONITOR_ON() with the new enumeration value at the point you
 *   want the signal to switch on.
 * + Call #TIMING_MONITOR_OFF() with the enumeration value at the point you
 *   want the signal to switch off.
 * + Alternatively, call #TIMING_MONITOR_TOGGLE to toggle it
 * + When running the code, calibrate #timing_monitor to the enumeration value
 *   to make the signal active.
 *
 * @note on real hardware, the VTBM acquistion signal is pulled up, so calling
 * TIMING_MONITOR_ON will pull the signal low.  However, on current prodder
 * boards, there is no pull-up so calling TIMING_MONITOR will send the signal
 * high.
 *
 * @defgroup timing_monitor Timing Monitor
 * @ingroup utils
 */
#include "timing_monitor_types.h"
extern CAL timing_monitor_e timing_monitor;
extern CAL timing_monitor_e timing_monitor2;
