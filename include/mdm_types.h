/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mdm_types.h 
 *
 * description:  MDM public header file
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
#if !defined(MDM_TYPES_H)
#define      MDM_TYPES_H
/**
 *
 * MDM public header file
 *
 * @file
 * @ingroup mdm
 */
#include "nvs_cfg.h"
#include "tmm.h"
//#include "vio.h"
#include "lin_types.h"
#include "tmm_seq.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/** Size of non volatile data used by MDM */
//#define MDM_NVS_USED (sizeof(nvs_header_s) + sizeof(mdm_nvs_data_s))
#define MDM_NVS_USED (sizeof(nvs_header_s) + sizeof(mdm_nvs_data_s) + sizeof(tmm_seq_data_s)*3)


/**
 * Size of vectors stored in eeprom. These are simply defined as '3' in the
 * RTW generated code, so it is hard to say what they represent. */
#define MDM_EE_TRIPLE   3u

/**
 * Simple max macro */
#define MMAX(a,b) ((b>a)?b:a)

/**
 * Simple min macro */
#define MMIN(a,b) ((b<a)?b:a)

/**
 * The RTW code uses a convention where a triple of min/max/mean is 
 * transferred as a vector.
 * @note this doesn't apply to all triples (some are short/medium/long),
 * but those which are min/max/min MUST obey this ordering convention */

/** The position of the minimum in a triple */
#define MDM_MIN_IDX 0
/** The position of the maximum in a triple */
#define MDM_MAX_IDX 1
/** The position of the mean in a triple */
#define MDM_MEAN_IDX 2


#define MDM_FREEZE_VAL1 0xEDB5u
#define MDM_FREEZE_VAL2 0x5E16u



/* LOG EEPROM data reset acknowledged flag mask in mdm_ram_image.data.soc_flags */
#define MDM_RESET_ACK_MASK      (0x02u)

typedef enum MdmBgShutdown_e
{
    MdmBgNotShutdown,
    MdmBgShutdownPend,
    MdmBgShutdownDone,
    MdmBgShutdownFailed
} MdmBgShutdown_t;


/**
 * Layout version number to ensure incompatible data isn't loaded after
 * reflashing
 */
#define MDM_NVS_LAYOUT_VERSION  6u

typedef struct mdm_nvs_data_tag
{
//	int16_t 	angle_upper_endstop;
//	int16_t 	angle_lower_endstop;
//	uint16_t	sent_upper_endstop;
//	uint16_t	sent_lower_endstop;
//	uint16_t 	can_msg_id[TMM_CAN_MAX];
	uint8_t     can_BTR0;
//	sent_CRC_version_e sent_CRC_version;
//	sent_pause_pulse_e sent_pause_pulse;
//	sent_serial_message_e sent_serial_message;
//	bool_t sent_dis_rollint_cntr;
//	uint16_t    still_time;
} mdm_nvs_data_s;


/** @lint
 * Error 92: Negative array dimension or bit field length (-2)
 * Not actually an error, the structure fits in Cosmic, which uses smaller enums
 * It is however using a non-portable compiler extension. The array is near-full
 * and the mismatch of enum sizes pushes it over the limit for Lint.
 * Cosmic does give an error if the structure doesn't fit, so there is no
 * correctness issue in turning it off, just a portability issue.
 */


typedef enum {
  EE_READ_PENDING = 0,                 /* Default value */
  EE_READ_SUCCESSFUL,
  EE_READ_COMPROMISED,
  EE_READ_UNSUCCESSFUL
} ee_readstatus_e;

typedef struct mdm_nvs_wrapper_tag
{
    nvs_header_s header;    /**< Feature Information Structure.*/
    lin_seq_data_s data[LIN_STEPS_MAX];     /**< Module NV data-set. */
    tmm_seq_data_s data_FunA[TMM_SEQ_STEP_MAX];
    tmm_seq_data_s data_FunB[TMM_SEQ_STEP_MAX];
    tmm_seq_data_s data_FunC[TMM_SEQ_STEP_MAX];


    /*lint -e{92}
     * The array size is checked by the compiler. Cosmic uses smaller enums and
     * there is no way of telling Lint to use the same size, hence a diagnostic
     **/
    uint8_t reserved_bytes[(int16_t)MDM_MAX_NVS_BYTES - (int16_t)MDM_NVS_USED];
                                            /**< Feature Reserved_bytes.*/
} mdm_nvs_wrapper_s;

#endif      /* MDM_TYPES_H */
