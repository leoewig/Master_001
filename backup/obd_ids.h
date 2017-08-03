/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    obd_ids.h 
 *
 * description:   
 *   Header for reading Software, Calibration and Hardware identification over UDS   
 *   
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined OBD_IDS_H
#define OBD_IDS_H
/**
 * Header for reading Software, Calibration and Hardware identification over UDS
 *
 * @file
 * @ingroup obd
 *
 */
#include "obd_ids_types.h"


/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/

/** The ECU Name reported by OBD service $09 (PID $0A)
 */
extern FROM char_t     obd_id_ecu_name[OBD_ECU_NAME_LEN+1u];

/** The hardware ID reported as DID 0xF191
 */
extern FROM char_t     obd_id_hw_id[OBD_HW_ID_LEN+1u];

/******************************************************************************
 * PUBLIC FUNCTIONS PROTOTYPES
 *****************************************************************************/

void obd_read_idblock(FPTR uint8_t *buffer, FPTR uint16_t *lengthp);
void obd_read_boot_sw_id(FPTR uint8_t *buffer, FPTR uint16_t *lengthp);
void obd_read_app_sw_id(FPTR uint8_t *buffer, FPTR uint16_t *lengthp);
void obd_read_cal_data_id(FPTR uint8_t *buffer, FPTR uint16_t *lengthp);
void obd_read_app_fingerprint(FPTR uint8_t *buffer, FPTR uint16_t *lengthp);
void obd_read_cal_fingerprint(FPTR uint8_t *buffer, FPTR uint16_t *lengthp);


#endif /* if !defined OBD_IDS_H */
