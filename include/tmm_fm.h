/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    tmm_fm.h
 *
 * description:  
 *  			tmm failure management.
 * 
 *
 * revision history:
 *
 * Date         Author           Description
 * 2016-May-18  Jun Wu           initial version
 *
 *****************************************************************************/
#if !defined(TMM_FM_H)
#define      TMM_FM_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#include "smm.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/ 
#define Over_Current_Max  400                /* 4 seconds */

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
typedef struct 
{
	bool_t ECUT_FLAG_1;
	bool_t ECUT_FLAG_2;
	bool_t ECUT_FLAG_3;
	bool_t ECUT_FLAG_4;
} tmm_ecut_flag_s;


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
//extern uint16_t Err_SENT_TimeOut_10ms;



extern bool_t FLAG_TOUT_DIG_RAW[sent_channel_max];
extern bool_t ErrGradBegr[sent_channel_max];
extern tmm_ecut_flag_s TMM_ECUT_FLAG[sent_channel_max];
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/ 
void tmm_fm_update(void);
void tmm_fm_update_test(void);

#endif /* TMM_FM_H */
