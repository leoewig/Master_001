/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    lin_types.h
 *
 * description:  
 *  			lin types configure header.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Mar-23  Jun Wu           initial version
 *
 *****************************************************************************/
#if !defined(LIN_TYPES_H)
#define      LIN_TYPES_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/ 
#define LIN_STEPS_MAX 10u
/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
typedef enum
{
  time_based = 0u,
  temp_based,
  voltage_based,
  seq_mode_max
} lin_seq_mode_e;


typedef struct
{
  uint16_t            LIN_STEP_ID;
  bool_t              FLAG_LIN_STEP_END;
  lin_seq_mode_e      LIN_SEQ_MODE;
  uint8_t             LIN_POWER_SUPPLY;
  uint8_t             LIN_STEP_DURATION;
  uint8_t             BRV_POSI_SET;
  int16_t             LIN_TEMP_MIN;
  int16_t             LIN_TEMP_MAX;
  int8_t              LIN_ANA_MIN;
  int8_t              LIN_ANA_MAX;
} lin_seq_data_s;


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/ 


#endif /* LIN_TYPES_H */
