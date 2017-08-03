/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2017 - All rights reserved 
 *
 * file name:    tmm_seq.h
 *
 * description:  
 *  			tmm sequence management header.
 * 
 *
 * revision history:
 *
 * Date         Author           Description
 * 2017-Apr-21  Jun Wu           initial version
 *
 *****************************************************************************/
#if !defined(TMM_SEQ_H)
#define      TMM_SEQ_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#include "smm_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/ 
#define	TMM_SEQ_STEP_MAX	70u

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
typedef struct
{
  uint16_t       TMM_SEQ_TIME;
  int16_t        TMM_SEQ_ANGLE;
  uint8_t		 TMM_SEQ_SPEED;
  bool_t	     FLAG_SEQ_END;
} tmm_seq_data_s;


typedef enum
{
	no_fun,
	fun_A,
	fun_B,
	fun_C
} tmm_seq_fun_sel_e;




/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
//extern tmm_seq_data_s TMM_Seq_Data[TMM_SEQ_STEP_MAX];

extern bool_t Flag_TMM_Func_B_En[sent_channel_max];
extern bool_t Flag_TMM_Seq_Single[sent_channel_max];

extern tmm_seq_fun_sel_e TMM_Fun_Sel[sent_channel_max];
extern tmm_seq_data_s TMM_Seq_Data_A[TMM_SEQ_STEP_MAX];
extern tmm_seq_data_s TMM_Seq_Data_B[TMM_SEQ_STEP_MAX];
extern tmm_seq_data_s TMM_Seq_Data_C[TMM_SEQ_STEP_MAX];
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/ 
void tmm_seq_update(void);

#endif /* TMM_SEQ_H */
