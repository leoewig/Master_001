/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     vprg_p.h
 *
 * description:   VTBM re-programming macros and types
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           VTBM re-programming macros and types
 *                       
 *
 *****************************************************************************/
#if !defined(VPRG_H)
#define      VPRG_H
/**
 *
 * VTBM re-programming macros and types
 *
 * @file
 * @ingroup vprg
 */

/**
 * VTBM re-programming
 *
 * # Overview
 * 
 * The VRPG feature allows a means of reprogramming VTBMs with a binary stored
 * in flash on the MMC.  There are three ways of triggering the re-programming:
 * + by manipulating VRPG calibration values
 * + by sending XCP_USER_COMMAND messages within a vehicle-bus XCP session
 * + by UDS diagnostic service request
 *
 * When VTBM re-programming is triggered by either VPRG calibration values
 * or XCP_USER_COMMAND, VTBM re-programming is a two-stage process.  First,
 * the image to be programmed must be validated and then it is programmed into
 * a selected number of VTBMs.
 * When VTBM re-programming is triggered by the UDS diagnostic service request,
 * both stages are performed by the VPRG feature. VTBM mask and an image to be be
 * programmed is configured with calibration values.
 *
 * # Layout of an image folder
 *
 * A VTBM image folder is programmed onto the MMC separately from the MMC
 * bootblock and application.  This image folder can contain multiple
 * VTBM binaries.  The image folder consists of
 * + An image folder header, specifying:
 *   + the number of images in the folder
 *   + the location within the folder of each image
 *   + the name of each image
 *   + a CRC for the header
 * + Multiple images, each one consisting of:
 *   + An image header, specifying the number of sectors of data
 *   + a CRC for the image header
 *   + Multiple sectors, each containing:
 *     + the number of bytes of data
 *     + the VTBM address to which the data should be programmed
 *     + the data itself
 *     + a CRC for the sector
 *
 * Sectors must correpsond to the flash sectors of the target processor
 * because a single erase command will be issued for each sector.
 *
 * Image folders are built with the python program `image_folder.py`.  
 * `image_folder.py` can also be used to list the images contained within an
 * S-Record file.  An image folder can be appended to to an MMC application
 * S-Record with the program `srecjoin.py`.
 *
 * # Validating the image
 *
 * Validating an image consists of checking that the CRCs of the image folder
 * header, the image header and each sector is correct.
 *
 * # Programming an image
 *
 * Only the most recently validated image can be programmed.  Programming
 * uses the XCP protocol, with the VPRG feature acting as the XCP master.
 *
 * Multiple VTBMs can be programmed in a single programming sequence.  Before
 * each VTBM is reprogrammed, VTBMs are power cycled so that VPRG can connect
 * to the VTBM bootblock's XCP slave.  This will allow a corrupt VTBM
 * application to be re-programmed.
 *
 * # Using the calibration interface
 * 
 * ## XCP connection to the MMC
 * 
 * The calibration interface can be manipulated from an XCP session on either
 * the internal or vehicle buses.  Note, however, that an XCP connection must
 * not be established to any VTBMs while reprogramming is being carried out.
 *
 * ## Calibration variables
 * The calibration interface is driven with the variables:
 * + #vprg_demand_cal which should be set as follows:
 *   + #Vprg_dmd_none when the interface is idle
 *   + #Vprg_dmd_prepare to validate and image
 *   + #Vprg_dmd_program to program an image
 *   + #Vprg_dmd_reset to reset the MMC
 *   + #Vprg_dmd_return_to_idle to recover from an error or validate another
 *     image
 *  Because calibratables are non-volatile, the contents of #vprg_demand_cal
 *  is ignored unless it has been set to #Vprg_dmd_none in the current power
 *  cycle.  The variable #vprg_via_cal_active displays whether or
 *  not #vprg_demand_cal is being ignored.
 * + #vprg_image_cal which specifies the index of the image to be validated
 *   or programmed
 * + #vprg_vtbm_mask_cal which specifies the VTBMs to be reprogrammed
 *
 * Progress is monitored by observing the variables #vprg_state
 * and #vprg_latest_error.
 *
 * # Using UDS interface
 *
 * ## Requesting VTBM reprogramming
 *
 * Reprogramming may be requested by calling vprg_uds_vtbm_reprogramming_req(). When
 * no other VTBM reprogramming operation is taking place, this will trigger
 * internal UDS demand processing state machine, which perform transitions
 * between all VTBM reprogramming states automatically.
 *
 * ## Requesting VTBM reprogramming status
 *
 * Status of the operation may be obtained by calling vprg_uds_get_status()
 *
 * ## Configuring VTBM reprogramming
 *
 * The UDS diagnostic service driven operation is configured with the
 * variables:
 * + #vprg_uds_vtbm_mask controls which VTBMs are to be programmed
 * + #vprg_uds_vtbm_image controls what image is to be programmed into
 *
 * # Using the XCP User Command interface
 * XCP provides a command code for user-specified commands.  We use this code
 * to control VTBM re-prgramming.
 *
 * The XCP User Command interface is only available within vehicle-bus XCP
 * sessions.
 *
 * The protocol used has been implemented by the bms_prog program.
 *
 * # Software design
 * 
 * VTBM reprogramming is split between regular foreground functions and two
 * idle-loop functions vprg_check_image_idle_task() and vprg_comms_idle_task().
 * The idle-loop functions have the twin benefits of increasing the speed of
 * operation and reducing the impact of timing-sensitive parts of the
 * application.
 *
 * The feature is split over three source files:
 * + vprg_state.c containing the main state machine and the interface to the
 *   application.
 * + vprg_image.c containing image validation functions
 * + vprg_comms.c containing the XCP master for VTBM re-programming
 *
 * ## Main state machine
 * The main foreground state-machine is controlled by calling vprg_update()
 * which in turn calls vprg_state_machine().  The state of the state machine
 * is given by #vprg_state.
 *
 * ## Image checking state machine
 * Upon request, an image is checked within the idle loop.  The state of image 
 * checking is given by #vprg_ci_state
 *
 * ## VTBM re-programming
 * The task of reprogramming VTBMs is split between foreground functions in
 * vprg_state.c and idle loop functions within vprg_comms.c.  The re-programming
 * states of the main state machine are as follows:
 * ### Foreground states
 * + Shutdown ICM
 *   - To cease regular communication with the VTBMs and stop processing CAN
 *     messages on the internal bus.
 * + Power cycle VTBMs
 *   - To power VTBMs off and on to allow an XCP connection to the bootloader.
 * + Communicating
 *   - When control is switched to the idle-loop state machine in vtbm_comms.c
 * + Programming complete
 *   - When all desired VTBMs have been reprogrammed.  
 *
 * ### Idle-loop states
 * + Connect
 *   - To establish the XCP connection with the next VTBM to be reprogrammed
 * + Program Start
 *   - To send a PROGRAM_START XCP message and check the response
 * + Erase
 *   - To send an erase message for each sector within the image being
 *     programmed
 * + Program
 *   - To program each sector within the image being programmed.
 * + Program Reset
 *   - To reset the VTBM when programming is complete
 * + Complete
 *   - To return control to the foreground to either power cycle the VTBMs prior
 *     to programming the next VTBM or to enter the Programming Complete state
 *
 * # Using the feature
 * The feature is installed into the application by calling vprg_update() from
 * the foreground task and vprg_idle_tasks() from the idle loop.
 *
 * Upon receipt of an XCP user command, the CANbedded XCP slave must call
 * vprg_xcp_user_command() with the contents of the message.
 *
 *
 * @defgroup vprg VTBM Re-programming
 */
/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "vprg_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * FUNCTIONS
 *****************************************************************************/

void vprg_update(void);
void vprg_idle_tasks(void);
void vprg_xcp_user_command(FPTR const uint8_t *data);
bool_t vprg_uds_vtbm_reprogramming_req(void);
void vprg_uds_get_status(bool_t *finished, uint16_t* mask, uint16_t *image,
    vprg_error_e *latest_error, vprg_ci_state_e *image_status, vprg_comms_error_e * comms_error);


/******************************************************************************
 * DATA
 *****************************************************************************/
extern bool_t vprg_reprog_request;      /**< Request from vrpg to icm to enter
                                              VTBM reprogramming mode */


#endif      /* VPRG_H */

