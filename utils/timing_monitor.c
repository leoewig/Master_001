/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:        timing_monitor.c
 *
 * description:  Timing monitor utility
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
/**
 * Timing monitor utility
 * @file
 * @ingroup timing_monitor
 */
#include "mmc.h"
#include "timing_monitor.h"

/* 
 * make timing_monitor S12-only because safely writing to the I/O port
 * from the XGATE without interfering with the S12 would be impossible without
 * littering the S12 code with semaphores.
 */
#define MEMORY_LOCATION S12_MEMORY
#include "memory_layout.h"

/**
 * Calibratibale to cause a timing signal to be output on the VTBM acquisition
 * output.  See #TIMING_MONITOR_ON and #TIMING_MONITOR_OFF
 */
CAL timing_monitor_e timing_monitor = Timing_monitor_none;
CAL timing_monitor_e timing_monitor2 = Timing_monitor_none;
/**< @a2l{conversion,timing_monitor_e} */
