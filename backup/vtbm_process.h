/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    vtbm_process.h
 *
 * description:  process vtbm data
 * 
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Sep-17 Jun Wu           initial version
 *
 *****************************************************************************/


#ifndef __vtbm_process_H
#define __vtbm_process_H
/* MODULE Events */

#include "basic_types.h"
#include "mdm.h"

extern temperature_t mdm_temp_avg;

void voltage_process_measurement(void);
void temp_process_measurement(void);
void vtbm_voltage_process(void);
void vtbm_store_cellvolt(void);
void vtbm_set_volt_minmax(void);
void vtbm_set_deltas(void);
pack_v_t vtbm_set_cellsum(void);
void vtbm_temp_process(void);
void vtbm_store_celltemp(void);
void vtbm_set_temp_minmax(void);
PRIVATE int16_t add_s16s16(int16_t lhs,int16_t rhs);
void vtbm_set_tempavg(void);
void set_minmax(void);
void volt_raw_value_process(uint8_t voltage_filt_cnt);

/*
** ===================================================================
**     Event       :  SM1_OnFreeTxBuf (module Events)
**
**     From bean   :  SM1 [SynchroMaster]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**         This event is available only when the <Interrupt
**         service/event> property is enabled and the <Output buffer
**         size> property is set to non-zero value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/



#endif /* __vtbm_process_H*/

/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 3.00 [04.12]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
