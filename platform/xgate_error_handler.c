/**
 * @file
 * @ingroup s12_xgate
 *
 * XGATE vector table
 *
 * $Id: xgate_error_handler.c 4157 2012-10-05 09:06:07Z ae4 $
 */
#define _XGATE_
#include "hardware.h"
#include "gate.h"


/*
 *  Handler for unexpected XGATE interrupts.
 *
 *  Documented in header
 */
INTERRUPT void x_unhandled_isr( void )
{
    uint16_t offset;
    /*
     * store the channel number so that it can be seen from the S12 when we
     * call the error handler
     */
    xg_unhandled_chnl = XGCHID;
    offset = xg_unhandled_chnl * 2u;    /* Vector offsets are twice the channel
                       number (because 2 bytes per channel)
                       */
    INT_CFADDR = (uint8_t) offset & 0xf0u ;    /* map the interrupt into INT_CFDATA[] */
    INT_CFDATA[(offset & 0xfu) >> 1u] = 0u;  /* disable by setting
                           priority to 0 in INT_CFDATA[]*/

#if ! defined (POLYSPACE)
    _sif(CHANNEL_FOR_UNHANDLED_XGATE_INTERRUPT);
#endif
}
