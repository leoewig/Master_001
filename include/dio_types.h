/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  dio_types.h
 *
 * description:  
 * I/O definition for vehcile CAN transceiver disable line
 * 
 * This I/O mapping is needed by both the application and the MMC bootblock
 * so it is defined here for consistency
 *  
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#ifndef DIO_TYPES_H
#define DIO_TYPES_H   /* multiple inclusion protection */
/**
 * @file
 * @ingroup dio
 *
 * Digital I/O header file
 */
/**
 * enum for defining I/O ports
 *
 * @note To make look-ups easy, the order of definition in this enum
 * must match the order of entries in #dio_mapping[]
 */
enum dio_port_e {
    Dio_port_a,
    Dio_port_b,
    Dio_port_e,
    Dio_port_h,
    Dio_port_j,
    Dio_port_k,
    Dio_port_m,
    Dio_port_p,
    Dio_port_t,
    Dio_port_s,
    Dio_port_max
} ;

/**
 * specify whether a port is active high or low
 */
enum dio_active_e {
    Dio_active_low, /**< pin LOW equivalent to variable TRUE */
    Dio_active_high /**< pin HIGH equivalent to variable TRUE */
};

/**
 * Specify whether a line is read/written via interface functions running on
 * S12 or directly.
 */
enum dio_interface_e {
    Dio_interface_function, /**< input read or output written by
                              dio_read_inputs() or dio_write_outputs() */
    Dio_interface_direct    /**< input read or output written directly */
};

/**
 * Specify whether an input shall be enabled with pull-up, pull-down
 * or left as it is by default
 */
enum dio_pull_cfg_e {
    Dio_pull_up,            /**< Input will be enabled with pull-up resistor */
    Dio_pull_down,          /**< Input will be enabled with pull-down resistor */
    Dio_pull_none,          /**< No pull resistor will be enabled */
    Dio_pull_default        /**< Pull resistor configuration will be left default */
};



/** Mask of NECLK bit in ECLKCTL register */
#define NECLK_MASK  0x80u
/** Mask of NCLKX2 bit in ECLKCTL register */
#define NCLKX2_MASK 0x40u

/**
 * ECLK configuration can foul DIO's operation, if you use PE4 this macro must
 * be used to disable the peripheral that would override the port.
 * Unlike NECLK, NCLKX2 defaults to disabled - which it needs to be if you use
 * PE7 */
#define DIO_DISABLE_ECLK() do { ECLKCTL |= NECLK_MASK; } while (0)

#endif  /* DIO_TYPES_H */
