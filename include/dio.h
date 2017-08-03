/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  dio.h
 *
 * description:  
 * The aim of the DIO feature is to provide an efficient mechanism for
 * reading and writing digital inputs and outputs.
 * 
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
#ifndef DIO_H
#define DIO_H   /* multiple includion protection */
/**
 * @file
 * @ingroup dio
 *
 * Digital I/O header file
 */
/**
 * Digital Input/Output
 *
 * Design goals
 * ------------
 *
 * The aim of the DIO feature is to provide an efficient mechanism for
 * reading and writing digital inputs and outputs.
 *
 *
 * Approach used
 * -------------
 *
 * Digital inputs and outputs are configured in the #dio_input_cfg[]
 * and #dio_output_cfg[] arrays which are defined in an application-specific
 * C files (e.g. mmc_dio_cfg.c).
 *
 * The arrays specify the I/O line (port and pin number), an application
 * boolean associated with the line, whether the line is active high or low and
 * whether it is accessed by this module or directly.
 *
 * For the outputs, the configuration also includes application booleans:
 *   - to indicate the most recent demand.
 *   - to indicate the value read back from the output pin.
 *   - to control whether the output level should be overridden.
 *   - to control the level that the output should set to when overridden.
 *
 * Indications are updated each time the outputs are written.
 *
 * Application booleans are defined in the configuration C file and declared
 * in an application-specific header file (e.g. mmc_dio_cfg.h).
 * The configuration files are copied by the build system to dio_cfg.c and
 * dio_cfg.h in the build directory.
 *
 * An init function, dio_init(), configures all the channels regardless of
 * whether they are read/written by this module or directly.
 *
 * Inputs using this module are read by calling dio_read_inputs().  Outputs
 * using this module are written by calling dio_write_outputs().
 *
 * The expectation is that dio_read_inputs() will be called at the start of
 * each foreground loop and dio_write_outputs() will be called at the end of
 * each loop.
 *

 * Data integrity
 * --------------
 *
 * Writing digital outputs requires a read-modify-write of the output port
 * which risks corruption if the port is written from another task between the
 * read and write (i.e. the other task's changes could be lost).  [See also the
 * note on using bset/bclr below]
 *
 * dio_write_outputs() protects against this by disabling interrupts around
 * the read-modify-write.  The application must ensure by design the following:
 * - corruption of XGATE writes are not possible
 * - Outputs written directly are not corrupted by a call to dio_write_outputs()
 *   in a higher-priority task.
 *
 * No precautions are needed for digital inputs since reading the port is an
 * atomic operation.
 *
 * dio_read_inputs() and dio_write_outputs() are reentrant although care has
 * to be taken to make sure that output booleans in low priority tasks are not
 * written with intermediate values.  For example the following would be unsafe
 * if dio_write_outputs() were also called from a higher priority task.
 * ~~~c
 * portvar = FALSE; / * set default value  UNSAFE! * /
 * if (...some condition...)
 * {
 *     portvar = TRUE; / * override default  UNSAFE! * /
 * }
 * dio_write_outputs();
 * ~~~
 *
 * ### Atomic operations with bset/bclr
 *
 * We do not use the S12 core's ability to set or clear bits with the `bset`
 * and `bclr` instructions.  These instructions require the bitmask to be
 * immediate data and so do not fit with configuration array approach.
 * Instead, it would be necessary to \`\#define\` the bitmask for each port and
 * pass it into an `_asm()` macro.  This is possible but messy (# and ##
 * preprocessing required) and requires more application information to
 * be hardcoded in the driver.
 *
 * The XGATE cannot set or clear port bits atomically.
 *
 * Driving ports directly
 * ----------------------
 *
 * It may be necessary to drive ports directly instead of using
 * dio_read_inputs() and dio_write_outputs().  among the reasons for this are:
 * 1. The output is a debug line specifically for debugging XGATE code
 * 2. The output is on the same port as a SPI slave-select line that must
 *     be explicitly controlled by a SPI driver on the XGATE.  To write a port
 *     safely from both cores would require semaphores so it is best avoided.
 * 3. A subset of ports must be written quickly and repeatedly (e.g. driving a
 *    multiplexor multiple times) making repeated calls to dio_write_outputs()
 *    undesirable.
 *
 * A port is indicated as being driven directly by setting the `interface`
 * member of the configuration array to #Dio_interface_direct.
 *
 * This feature will initialise ports directly-driven ports but does not read
 * or write them: that must be done manually by application code.  However, the
 * configuration arrays #dio_input_cfg[], #dio_output_cfg[] and #dio_mapping[]
 * are exported so can be used by the application.
 *
 * Limitations
 * -----------
 *
 * The module has the following limitations:
 * - The code does not currently support the ADC port digital I/O. (See note
 *   with dio_init())
 *
 * Integration
 * -----------
 *
 * To use this feature, you must:
 *   1. Write a configuration source file, e.g. mmc_dio_cfg.c, which specifies
 *      inputs, outputs, their associated application booleans and pull resistor
 *      configuration.
 *   2. Configure the build system to copy the header file into the build
 *      directory with the name dio_cfg.c (see mmc.mk for an example of how this
 *      is done).
 *   3. Include dio.h and call dio_init() once from initialisation code.
 *   4. Call dio_read_inputs() at the start of each loop.
 *   5. Call dio_write_outputs() at the end of each loop.
 *
 *
 * Requirements
 * ------------
 *
 * @mantis{3620} Input filtering
 *
 *
 *  The requirements are tested by the following TUTs:
 *
 *    - base_set_all_low()
 *    - base_P2S_walk1s()
 *    - base_S2P_walk1s()
 *    - base_set_all_high()
 *    - base_P2S_walk0s()
 *    - base_S2P_walk0s()
 *    - base_set_all_low()
 *    - dio_init()
 *    - dio_set_all_low()
 *    - dio_P2S_walk1s()
 *    - dio_set_all_high()
 *    - dio_P2S_walk0s()
 *
 * | Requirement            | Satisfied By | HUT | PAT |
 * | -----------------------|--------------|----------------|----------|
 * | REQ_BMS_SUP_DIG_001    | dio_cfg.c    | test_dio_inputs(), test_dio_outputs() |  |
 * | REQ_BMS_SUP_DIG_002    | dio_cfg.c    | test_dio_init_pull_resistors() |  |
 * | REQ_BMS_SUP_DIG_003    | dio_read_inputs()    | test_dio_inputs() |  |
 * | REQ_BMS_SUP_DIG_004    | dio_cfg.c, dio_write_outputs() | test_dio_outputs() |  |
 * | REQ_BMS_SUP_DIG_005    | dio_cfg.c, dio_write_outputs() | test_dio_outputs__active_low() |  |
 * | REQ_BMS_SUP_DIG_006    | dio_cfg.c, dio_write_outputs() | test_dio_outputs__override() |  |
 * | REQ_BMS_SUP_DIG_007    | dio_cfg.c    | _not tested_ |  |
 * | REQ_BMS_SUP_DIG_008    | dio_cfg.c    | _not tested_ |  |
 * | REQ_BMS_SUP_DIG_009    | dio_write_output() | test_dio_outputs__active_low() |  |
 * | REQ_BMS_SUP_DIG_010    | dio_read_inputs(), dio_write_outputs()|  |  |
 *
 * @defgroup dio DIO Digital I/O
 * @ingroup platform
 */

#include "dio_types.h"

#define DEF_DIG_OP(var_base,port,bit,orientation,operation) \
    { &do_## var_base, &do_dmd_## var_base, &do_rb_## var_base, &do_or_en_## \
    var_base, &do_or_val_## var_base, port, bit, orientation, operation }

/**
 * Configuration structure for digital inputs
 */
struct dio_in_cfg_s {
    bool_t *var;    /**< application variable in which to store input */
    enum dio_port_e port;    /**< I/O port */
    uint8_t bitnum;        /**< pin number of I/O port (range 0 to 7) */
    enum dio_active_e active_high_low;
    enum dio_interface_e interface;    /**< direct or via dio_read_inputs()
                                                dio_write_outputs() */
};

/**
 * Configuration structure for digital outputs
 */
struct dio_out_cfg_s {
    bool_t *var;    /**< application variable in which to set output */
    bool_t *dmd; /**< application variable in which to store actual level requested */
    bool_t *read_back;  /**< application variable in which to store value
              read back from pin */
    CAL bool_t *override_en;  /**< calibration in which to set if override is
                                                                      enabled */
    CAL bool_t *override_val;  /**< calibration in which to set override value */
    enum dio_port_e port;    /**< I/O port */
    uint8_t bitnum;        /**< pin number of I/O port (range 0 to 7) */
    enum dio_active_e active_high_low;
    enum dio_interface_e interface;    /**< direct or via dio_read_inputs()
                                                dio_write_outputs() */
};

/**
 * Pull-resistor configuration structure
 */
struct dio_pull_cfg_s {
    enum dio_port_e port;           /**< I/O port */
    uint8_t bitmask;                /**< pin bit mask of I/O port to be configured */
    enum dio_pull_cfg_e pull_cfg;   /**< pull resistor type configuration */
};

/**
 * Specify whether a port's pull resistor is configurable
 * per port or per pin basis
 */
enum dio_pull_per_e
{
    Dio_pull_per_port,  /**< this port's pull resistors are configurable per port only */
    Dio_pull_per_pin    /**< this port's pull resistors are configurable per pin */
};

/**
 * structure to provide the mapping between port names and h/w registers.
 *
 * There are separate input port and output port entries to account for the
 * different styles of digital port used.
 */
struct dio_hw_mapping_s {
    volatile uint8_t *output_portp;     /**< ptr to register to write outputs */
    volatile uint8_t *input_portp;      /**< ptr to register to read inputs */
    volatile uint8_t *ddrp;             /**< ptr to data direction register */
    volatile uint8_t *pull_cfg;         /**< ptr to pull resistor enable/disable register */
    volatile uint8_t *pull_polarity;    /**< ptr to pull resistor polarity register, may be NULL */
    enum dio_pull_per_e  pull_per;          /**< pull resistor configuration per port or per pin */
    uint8_t pull_per_port_mask;         /**< bitmask to global pull resistor enable/ disable
                                             register if pull_per is Dio_pull_per_port */
};


/**
 * Mapping between ports defined in #dio_port_e and h/w registers
 *
 * @note the order of entries in this array must match the order of
 * definition in the #dio_port_e enumeration
 *
 * @todo locate in shared CPU/XGATE flash
 *
 */
extern CONST struct dio_hw_mapping_s dio_mapping[Dio_port_max];


/*
 * declarations for dio_cfg.c
 */

/** @polyspace<MISRA-C:8.12:Low:Investigate> Is there any way to specify this and still have a useable confuration system? */
/**
 * Configuration array for digital inputs
 */
extern CONST struct dio_in_cfg_s dio_input_cfg[];

/** @polyspace<MISRA-C:8.12:Low:Investigate> Is there any way to specify this and still have a useable confuration system? */
/**
 * Configuration array for digital outputs (excluding SPI SS lines)
 */
extern CONST struct dio_out_cfg_s dio_output_cfg[];

/** @polyspace<MISRA-C:8.12:Low:Investigate> Is there any way to specify this and still have a useable confuration system? */
/**
 * Configuration array for pull resistor configuration
 */
extern CONST struct dio_pull_cfg_s dio_pull_cfg[];

/**
 * number of digital inputs described in dio_input_cfg
 */
extern CONST uint8_t dio_n_inputs;

/**
 * number of digital outputs described in dio_output_cfg
 */
extern CONST uint8_t dio_n_outputs;

/**
 * number of pull resistor described in dio_pull_cfg
 */
extern CONST uint8_t dio_n_pull_cfgs;

/*
 * function prototypes for dio.c
 */
/**
 * Setup digital inputs and outputs.
 *
 * Should be called once before any calls to dio_read_inputs() or
 * dio_read_outputs().  All initialisation is assumed to be done by the CPU so
 * both CPU and XGATE channels are set up.
 */
void dio_init(void);

/**
 * Read CPU digital inputs
 *
 * Reads all CPU input channels configured in #dio_input_cfg and stores the
 * resutls in the associated booleans.
 *
 * XGATE digital inputs are not read.
 */
void dio_read_inputs(void);

/**
 * Write CPU digital outputs
 *
 * Writes all CPU output channels configured in #dio_input_cfg with the values
 * contained in the associated booleans.
 *
 * The application booleans that indicate the most recent demand are updated.
 *
 * After writing, the output channels are read back and application booleans
 * updated with the results.
 *
 * XGATE digital outputs are not written.
 */
void dio_write_outputs(void);
#endif  /* ifndef DIO_H */
