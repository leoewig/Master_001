/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   smg_types.h
 *
 * description:  
 * Header file that contains the types of the Supply Monitoring Feature.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
#if !defined( SMG_TYPES_H )
#define       SMG_TYPES_H

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/**
 * Event-driven devices to be monitored by SMG
 */
typedef enum
{
    smg_isa_sensor,   /**< ISA current sensor power supply */
    smg_vtbm_can_5v,  /**< VTBM CAN power supply */
    smg_max_devs      /**< Keep this as the last element */

} smg_devices_e;


#endif /* SMG_TYPES_H */
