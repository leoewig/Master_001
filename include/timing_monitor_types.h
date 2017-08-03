/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    timing_monitor_types.h
 *
 * description:  
 * Timing montiro type and macro definitions
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Timing montiro type and macro definitions
 *                       
 *
 *****************************************************************************/
/**
 * Timing montiro type and macro definitions
 *
 * @file
 * @ingroup timing_monitor
 * $Id: timing_monitor_types.h 10149 2013-08-16 11:54:05Z pr4 $
 */

/**
 * enumeration to control which tining info is output on VTBM acquisisiton line
 */
typedef enum {
    Timing_monitor_none,        /**< no timing signal */
    Timing_monitor_fg,          /**< ON at start of fg loop and OFF at end */
    Timing_monitor_bg,          /**< ON at start of background and OFF at end */
    Timing_monitor_idle_loop,   /**< TOGGLE at start of each idle loop */
    Timing_monitor_icm,         /**< ON at start of ICM task, OFF at end */
    Timing_monitor_icm_rx,      /**< ON whilst processing RX queue */
    Timing_monitor_meas_proc,    /**< ON whilst processing data */
    Timing_monitor_tsdisp,       /* Toggled in the ts dispatcher */ 
        /* Prodder only */
    Timing_monitor_cmd,
    Timing_monitor_emu,
    Timing_monitor_emu2,
    Timing_monitor_cont,
    Timing_monitor_rtc_task,
    Timing_monitor_rtc_emu,
    Timing_monitor_ee_emu,
    Timing_monitor_nvs,
    Timing_monitor_psul
} timing_monitor_e;

#ifdef PRODDER
/* PP0 is spare, routed to MMC's PT4 */
#define TIMING_MONITOR_PORT     PTP
#define TIMING_MONITOR_MASK     1u

/* PP1 is spare, routed to MMC's PT5 */
#define TIMING_MONITOR2_PORT     PTP
#define TIMING_MONITOR2_MASK     2u

#else
#define TIMING_MONITOR_PORT     PORTE /**< port containing unused VTBM
                                        acquisition o/p */
#define TIMING_MONITOR_MASK     4u      /**< VTBM acquisition is bit 2 */
#endif  /* def PRODDER */

/**
 * Switch timing signal on if #timing_monitor has been calibrated to the
 * given context
 */
#define TIMING_MONITORX_ON(context,var,port,mask)      do { \
                    if ((var) == (context)) \
                    { \
                        uint8_t ccr; \
                        SAVE_INTERRUPTS(ccr); \
                        (port) |= (uint8_t) (mask); \
                        RESTORE_INTERRUPTS(ccr); \
                    } \
                } while (0)

#define TIMING_MONITOR_ON(context)  TIMING_MONITORX_ON( context,\
        timing_monitor,TIMING_MONITOR_PORT,TIMING_MONITOR_MASK)

#define TIMING_MONITOR2_ON(context)  TIMING_MONITORX_ON( context,\
        timing_monitor2,TIMING_MONITOR2_PORT,TIMING_MONITOR2_MASK)

/**
 * Switch timing signal off if #timing_monitor has been calibrated to the
 * given context
 */
#define TIMING_MONITORX_OFF(context,var,port,mask)      do { \
                    if ( (var) == (context)) \
                    { \
                        uint8_t ccr; \
                        SAVE_INTERRUPTS(ccr); \
                        (port) &= (uint8_t) ~(mask); \
                        RESTORE_INTERRUPTS(ccr); \
                    } \
                } while (0)

#define TIMING_MONITOR_OFF(context) TIMING_MONITORX_OFF(context,\
        timing_monitor,TIMING_MONITOR_PORT,TIMING_MONITOR_MASK)

#define TIMING_MONITOR2_OFF(context) TIMING_MONITORX_OFF(context,\
        timing_monitor2,TIMING_MONITOR2_PORT,TIMING_MONITOR2_MASK)

/**
 * Toggle timing signal if #timing_monitor has been calibrated to the
 * given context
 */

#define TIMING_MONITORX_TOGGLE(context,var,port,mask)  do { \
                    if ( (var) == (context)) \
                    { \
                        uint8_t ccr; \
                        SAVE_INTERRUPTS(ccr); \
                        (port) ^= (uint8_t) (mask); \
                        RESTORE_INTERRUPTS(ccr); \
                    } \
                } while (0)

#define TIMING_MONITOR_TOGGLE(context)  TIMING_MONITORX_TOGGLE(context,\
        timing_monitor,TIMING_MONITOR_PORT,TIMING_MONITOR_MASK)

#define TIMING_MONITOR2_TOGGLE(context) TIMING_MONITORX_TOGGLE(context,\
        timing_monitor2,TIMING_MONITOR2_PORT,TIMING_MONITOR2_MASK)
