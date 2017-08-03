/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    mdm.c
 *
 * description:  Measurement Data Manager implementation
 * 
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2016-Apr-28 Jun Wu          initial version
 *
 *****************************************************************************/
/**
 *
 *
 * @file
 * @ingroup mdm
 */

#include "fm.h"         /* for failure function contributors */
#include "hardware.h"   /* For interrupt protection */
#include "mmc.h"        /* MMC header */
#include "mmc_aps.h"    /* For task manipulation */
#include "utils.h"      /* For ZERO_ARRAY */
#include "mdm.h"        /* public header for this module */
#include "mdm_ee.h"     /* header for MDM eeprom component */
#include "mdm_bg.h"     /* Header for data accessed by background functions only */
#include "mdm_p.h"
#include "rtc.h"        /* for RTC time */
#include "dio_cfg.h"    
#include "ms.h"         /* For shutdown flags */
#include "nvs.h"
#include "vio.h"       
#include "mdm_types.h"
#include "vio_p.h"

#include "smm.h"
#include "tmm.h"
#include "ai.h"
#include "LDT_TMM_Model.h"



PRIVATE void mdm_trsf_bcm_temp(void);
PRIVATE void mdm_trsf_curr_integ(void);
PRIVATE void mdm_trsf_v_buff(void);
PRIVATE void mdm_trsf_curr_buff(void);


/******************************************************************************
 * MACROS
 *****************************************************************************/

/******************************************************************************
 * XGATE PRIVATE DATA
 *****************************************************************************/

/******************************************************************************
 * Dummy 'A2L' variables
 *****************************************************************************/


/******************************************************************************
 * Mapped temperature inputs
 *****************************************************************************/


/******************************************************************************
 * Background calculation buffer
 *****************************************************************************/

PRIVATE FRAM  struct bg_fg_data_s  mdm_bg_fg_bank0;
PRIVATE FRAM  struct bg_fg_data_s  mdm_bg_fg_bank1;

/******************************************************************************
 * Counters and flags for controlling handover
 *****************************************************************************/

/*****************************************************************************
 * Performance measurement
 ****************************************************************************/


/*****************************************************************************
 * RTC signals
 ****************************************************************************/
PRIVATE uint32_t        mdm_rtc_time_s  = 0u;


PRIVATE data_validity_e mdm_rtc_status= No_data_available;

static bool mdm_seen_good_vt = FALSE;
PRIVATE bool_t mdm_curr_valid_seen=FALSE;
PRIVATE ts_ticks_t mdm_max_ticks=0u;
PRIVATE ts_ticks_t mdm_delta_ticks=0u;
PRIVATE volatile bool_t  mdm_bg_running=FALSE;
PRIVATE uint8_t mdm_bg_ran_count=0u;


/*****************************************************************************
 * Voltage buffer signals
 ****************************************************************************/

/*****************************************************************************
 * Current buffer signals
 ****************************************************************************/

/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/
int16_t mdm_TMM_angle;


uint8_t mdm_bg_op_read_ctr=0u;

uint16_t TMM_Run_Ctr = 0u;

/******************************************************************************
 * S12 PUBLIC DATA
 *****************************************************************************/
#undef MEMORY_LOCATION
#define MEMORY_LOCATION S12_MEMORY
#include "memory_layout.h"
/**
 * Buffer being actively used by bg task */
FPTR struct bg_fg_data_s        *mdm_bg_write_p=&mdm_bg_fg_bank0 ;

/**
 * Buffer that has been filled with values used by bg task */
FPTR CONST struct bg_fg_data_s *mdm_bg_output_p=&mdm_bg_fg_bank1;


/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/**
 *  Initialise MDM data structures
 */
void mdm_init(void)
{

	LDT_TMM_Model_initialize();                    //TMM App initialize
    
}

/**
 * Prepare data for execution of background task features.
 * The values are extracted in the background task.
 *
 * This function runs in the background task before other features and performs
 * data shuffling any additional conversions needed by the background features.
 *
 **/
void mdm_prep_bg_inputs(void)
{
	//mdm_in_Temp_Pt100 = Temp_Pt100/10;
	
	//mdm_in_TMM_Spare_1 = TMM_Spare_1;
	mdm_in_TMM_Matrix_Num = TMM_Matrix_Num;
	mdm_in_TMM_Run_Cnt = TMM_Run_Ctr;
}

/**
 * - Invoke the TMM management
 *
 * This function must be called before mdm_post_bg_outputs()
 */
void mdm_TMM_task(void)
{
	bool_t TMM_ran = FALSE;

	//if(TMM_Scaling_State == scaling_to_Matrix)                   /* customer add his own conditions */
	//{
	//	mdm_prep_bg_inputs();
	//	
	//	LDT_TMM_Model_step();  /* Auto-generated TMM management function */
//
	//	TMM_ran = TRUE;
	//	TMM_Run_Ctr++;
//
	//	if(TMM_Run_Ctr > 200u)                  /* for cycle run */
	//		{
	//		TMM_Run_Ctr = 0u;
	//		}
	//}
	//else
	//{
	//	TMM_Run_Ctr = 0u;
	//}
		
        

  
    /* The output is transferred anyway, if TMM has not run this will output
     * TMM's default values */
    mdm_post_bg_outputs(TMM_ran);

}

/**
 * Extract any remaining outputs (if any) from the background strategy components
 * mdm_TMM_task() and fill the exchanged data structure with them.
 *
 * @note this should be called at the end of the background task invocation. */
void mdm_post_bg_outputs(bool_t tmm_ran)
{
    /* Signal that the background has run one more time */
    mdm_bg_ran_count++;
    /* And flag it as not running */
    mdm_bg_running=FALSE;
	
	if((tmm_ran)&&(mdm_out_Flag_TMM_Control))
  {
		//TMM_Angle_Tar = mdm_out_TMM_Angle_Tar;
		//TMM_Speed_Tar = mdm_out_TMM_Speed_Tar;	
		//Flag_TMM_Angle_Control = mdm_out_Flag_TMM_Control; 
  }
}


/*****************************************************************************
 * PRIVATE functions
 ****************************************************************************/
