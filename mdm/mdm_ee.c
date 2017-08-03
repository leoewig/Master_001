/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mdm_ee.c
 *
 * description:  Measurement Data Manager eeprom read and write support
 * 
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/


#include "mmc.h"        /* MMC header */

#include "mdm_types.h"     /* mdm types header */
#include "mdm_ee.h"        /* public header for this module */
#include "hardware.h"       /* for TEST_HOOK */
#include "mdm_ee_p.h"
#include "mmc_rtw.h"
#include "utils.h"
#include "nvs.h"
#include "mdm.h"
#include "lin.h"
#include "tmm_seq.h"
#include <string.h>





/******************************************************************************
 * decalaration
 *****************************************************************************/

/******************************************************************************
 * MACROS
 *****************************************************************************/

/**
 * The size of an array, to be used only with variables declared as arrays, not
 * pointers.
 **/
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

/******************************************************************************
 * COMPILE-TIME CHECKS
 *****************************************************************************/


/******************************************************************************
 * Non-volatile data
 *****************************************************************************/

	
FRAM mdm_nvs_wrapper_s mdm_ram_image ={
        NVS_HEADER_INIT(MDM_NVS_LAYOUT_VERSION),
        {
        	0u,
        },
        {
        	0u,
        },
        {
          0u,
        },
        {
          0u,
        },
   		NVS_RESERVED_BYTES_INIT
    };

//FRAM mdm_nvs_wrapper_s mdm_ram_image ={
//        NVS_HEADER_INIT(MDM_NVS_LAYOUT_VERSION),
//        {
//          0u,
//        },
//        {
//          0u,
//        },
//      NVS_RESERVED_BYTES_INIT
//    };
	

/******************************************************************************
 * S12 PRIVATE DATA
 *****************************************************************************/
PRIVATE uint16_t mdm_ctr_reqEEWrite = 0U;
/**<
 * @a2l{LongIdentifier,MDM's version of the eeprom write request counter}
 */

/** @lint -esym(551,mdm_ee_image_writes_ctr) variable is for diagnostics and
 * testing*/

/*lint -esym(551,mdm_ee_image_writes_ctr) */

/**
 * @a2l{LongIdentifier,A counter incremented when MDM requests a live write} */
PRIVATE uint16_t mdm_ee_image_writes_ctr;

PRIVATE uint8_t mdm_ee_force_write_prev=0u;

#define MEMORY_LOCATION CALIBRATION_MEMORY_S12
#include "memory_layout.h"

PRIVATE CAL uint8_t mdm_ee_force_write=0u;

#undef MEMORY_LOCATION
#define MEMORY_LOCATION S12_MEMORY
#include "memory_layout.h"

/******************************************************************************
 * S12 PUBLIC DATA
 *****************************************************************************/
ee_readstatus_e mdm_ee_readstatus = EE_READ_PENDING; 

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/**
 * Inspect the restored EEPROM image and signal the appropriate value to the
 * autogen code.
 *
 * | Restore state      | Becomes              |
 * |--------------------|----------------------|
 * | New_data_available | EE_READ_SUCCESSFUL   |
 * | Old_data_available | EE_READ_SUCCESSFUL   |
 * | No_data_available  | EE_READ_UNSUCCESSFUL |
 *
 * @pre Must be called after EEPROM restoration has finished and before
 * cm_ref_initialize()*/
void  mdm_ee_init(void)
{
//	can_message_e msg;
	
   switch (NVS_RESTORE_STATUS(mdm_ram_image))
   {
   case New_data_available:
       /* all is well: the latest data has been restored */
       mdm_ee_readstatus = EE_READ_SUCCESSFUL;
       mdm_read_image();
       break;
  
   case Old_data_available:
       /* data has been restored from EEPROM but might not be the latest */
       mdm_ee_readstatus = EE_READ_COMPROMISED;
       mdm_read_image();
       break;
  
   case No_data_available:
   default:
       /* No data found: ram_image will contain the static initialiser.
       * Do not use the data, the Cell management will initialise
       * variables to the values it wants */
       mdm_ee_readstatus = EE_READ_UNSUCCESSFUL;
	   mdm_read_image();
	   
	   //memcpy(TMM_CAN_MSG,TMM_CAN_MSG_Default, TMM_CAN_MAX);       //too long
	   //for(msg=Rx_TMM_Sent; msg < TMM_CAN_MAX; msg++)
		//	{
		//		TMM_CAN_MSG[msg] = TMM_CAN_MSG_Default[msg];
		//	}
       break;
   }

  // mdm_ee_image_writes_ctr=0u;
  // mdm_ee_force_write_prev=mdm_ee_force_write;
}

/**
 * Restore data from the eeprom mirror to the cell management variables.
 * This function should be called after the NVS feature has successfully
 * written data into this feature's RAM image.*/
PRIVATE void mdm_read_image(void)
{
      mdm_read_tmm_seq_image();
      mdm_read_lin_seq_image();
}


/**
 * Copy the scalar items from the EEPROM image #mdm_ram_image into the cell
 * management inputs.
 **/
PRIVATE void mdm_read_tmm_seq_image(void)
{ 
    uint16_t step_id;

   for(step_id = 0u; step_id < TMM_SEQ_STEP_MAX; step_id++)
   {
        TMM_Seq_Data_A[step_id].TMM_SEQ_TIME  = mdm_ram_image.data_FunA[step_id].TMM_SEQ_TIME;
        TMM_Seq_Data_A[step_id].TMM_SEQ_ANGLE = mdm_ram_image.data_FunA[step_id].TMM_SEQ_ANGLE;
        TMM_Seq_Data_A[step_id].TMM_SEQ_SPEED = mdm_ram_image.data_FunA[step_id].TMM_SEQ_SPEED;
        TMM_Seq_Data_A[step_id].FLAG_SEQ_END  = mdm_ram_image.data_FunA[step_id].FLAG_SEQ_END;

        TMM_Seq_Data_B[step_id].TMM_SEQ_TIME  = mdm_ram_image.data_FunB[step_id].TMM_SEQ_TIME;
        TMM_Seq_Data_B[step_id].TMM_SEQ_ANGLE = mdm_ram_image.data_FunB[step_id].TMM_SEQ_ANGLE;
        TMM_Seq_Data_B[step_id].TMM_SEQ_SPEED = mdm_ram_image.data_FunB[step_id].TMM_SEQ_SPEED;
        TMM_Seq_Data_B[step_id].FLAG_SEQ_END  = mdm_ram_image.data_FunB[step_id].FLAG_SEQ_END;

        TMM_Seq_Data_C[step_id].TMM_SEQ_TIME  = mdm_ram_image.data_FunC[step_id].TMM_SEQ_TIME;
        TMM_Seq_Data_C[step_id].TMM_SEQ_ANGLE = mdm_ram_image.data_FunC[step_id].TMM_SEQ_ANGLE;
        TMM_Seq_Data_C[step_id].TMM_SEQ_SPEED = mdm_ram_image.data_FunC[step_id].TMM_SEQ_SPEED;
        TMM_Seq_Data_C[step_id].FLAG_SEQ_END  = mdm_ram_image.data_FunC[step_id].FLAG_SEQ_END;  
   
   }
}

/**
 * Copy the vector items from the EEPROM image into the cell management inputs
 **/
PRIVATE void  mdm_read_lin_seq_image(void)
{
   uint16_t step_id;
   for(step_id = 0u; step_id < LIN_STEPS_MAX; step_id++)
   {
   		LIN_Seq_Data[step_id].LIN_STEP_ID =  mdm_ram_image.data[step_id].LIN_STEP_ID;
   		LIN_Seq_Data[step_id].FLAG_LIN_STEP_END = mdm_ram_image.data[step_id].FLAG_LIN_STEP_END;
		LIN_Seq_Data[step_id].LIN_SEQ_MODE = mdm_ram_image.data[step_id].LIN_SEQ_MODE;
		LIN_Seq_Data[step_id].LIN_POWER_SUPPLY = mdm_ram_image.data[step_id].LIN_POWER_SUPPLY;
		LIN_Seq_Data[step_id].LIN_STEP_DURATION = mdm_ram_image.data[step_id].LIN_STEP_DURATION;
		LIN_Seq_Data[step_id].BRV_POSI_SET = mdm_ram_image.data[step_id].BRV_POSI_SET;
		LIN_Seq_Data[step_id].LIN_TEMP_MIN = mdm_ram_image.data[step_id].LIN_TEMP_MIN;
		LIN_Seq_Data[step_id].LIN_TEMP_MAX = mdm_ram_image.data[step_id].LIN_TEMP_MAX;
		LIN_Seq_Data[step_id].LIN_ANA_MIN = mdm_ram_image.data[step_id].LIN_ANA_MIN;
		LIN_Seq_Data[step_id].LIN_ANA_MAX = mdm_ram_image.data[step_id].LIN_ANA_MAX;
	}

}

/**
 * Check whether an eeprom write is due, this is determined by seeing if the
 * write request counter has changed.
 * If the counter from CM is changed:
 * - update our own counter to match it
 * - copy the outputs from Cell management into the array
 * - tell NVS to write the data
 */
void mdm_ee_update(void)
{
//	
//   //if (ccd_ctr_reqEEWrite!=mdm_ctr_reqEEWrite)
//   //{
//   //    mdm_ctr_reqEEWrite=ccd_ctr_reqEEWrite;
//   //    mdm_write_valid_image();
//   //}
//	mdm_write_image();
//	
//	if (mod_ctr_reqEEWrite!=mdm_ctr_reqEEWrite)
//   {
//       mdm_ctr_reqEEWrite=mod_ctr_reqEEWrite;
//       //mdm_write_valid_image();
//		mdm_write_image();
//   }
//   else if (mdm_ee_force_write!=mdm_ee_force_write_prev)
//   {
//       mdm_ee_force_write_prev = mdm_ee_force_write;
//       mdm_write_image();
//   }
//   else
//   {
//       /* nothing to do */
//   }
}

/**
 * If the cell management indicates the outputs are valid (determined by
 * @ref pid_flg_seOutputsValid ):
 * - copy values from the cell management outputs into the MDM eeprom RAM image
 * - tell the NVS feature that there is data to write.
 * A checkpoint is requested instead of a 'store' so that the data will be 
 * considered 'old' if it is read back without a further write.
 *
 **/
void mdm_write_valid_image(void)
{
    if (mdm_populate_valid_image())
    {
        nvs_request_checkpoint(Nvs_Mdm_feature);
    }
}
/**
 * Populate and write an image, whether it was valid or not
 */
PRIVATE void mdm_write_image(void)
{
 //   mdm_populate_image();
 //   nvs_request_checkpoint(Nvs_Mdm_feature);
}

/**
 * Update the data to be written to MDM ee */
void mdm_populate_image(void)
{
      mdm_ee_image_writes_ctr++;
      mdm_write_tmm_seq_image();
      mdm_write_lin_seq_image();
}

/**
 * Populate the NVS eeprom image from CM's data, if the data indicates it is
 * valid.
 * @retval TRUE if the image was copied
 * @retval FALSE if the image was not copied
 **/
bool_t mdm_populate_valid_image(void)
{
 //  bool_t image_copied=FALSE;
 //  if (pid_flg_seOutputsValid)
 //  {
 //      mdm_populate_image();
 //      image_copied=TRUE;
 //  }
 //  return image_copied;
}

/**
 * Transfer scalar items from the cell management variables to the EEPROM image
 */
PRIVATE void mdm_write_tmm_seq_image(void)
{
     uint16_t step_id;

   for(step_id = 0u; step_id < TMM_SEQ_STEP_MAX; step_id++)
   {
        mdm_ram_image.data_FunA[step_id].TMM_SEQ_TIME  = TMM_Seq_Data_A[step_id].TMM_SEQ_TIME;
        mdm_ram_image.data_FunA[step_id].TMM_SEQ_ANGLE = TMM_Seq_Data_A[step_id].TMM_SEQ_ANGLE; 
        mdm_ram_image.data_FunA[step_id].TMM_SEQ_SPEED = TMM_Seq_Data_A[step_id].TMM_SEQ_SPEED; 
        mdm_ram_image.data_FunA[step_id].FLAG_SEQ_END  = TMM_Seq_Data_A[step_id].FLAG_SEQ_END;   

        mdm_ram_image.data_FunB[step_id].TMM_SEQ_TIME  = TMM_Seq_Data_B[step_id].TMM_SEQ_TIME;
        mdm_ram_image.data_FunB[step_id].TMM_SEQ_ANGLE = TMM_Seq_Data_B[step_id].TMM_SEQ_ANGLE; 
        mdm_ram_image.data_FunB[step_id].TMM_SEQ_SPEED = TMM_Seq_Data_B[step_id].TMM_SEQ_SPEED; 
        mdm_ram_image.data_FunB[step_id].FLAG_SEQ_END  = TMM_Seq_Data_B[step_id].FLAG_SEQ_END;   

        mdm_ram_image.data_FunC[step_id].TMM_SEQ_TIME  = TMM_Seq_Data_C[step_id].TMM_SEQ_TIME;
        mdm_ram_image.data_FunC[step_id].TMM_SEQ_ANGLE = TMM_Seq_Data_C[step_id].TMM_SEQ_ANGLE; 
        mdm_ram_image.data_FunC[step_id].TMM_SEQ_SPEED = TMM_Seq_Data_C[step_id].TMM_SEQ_SPEED; 
        mdm_ram_image.data_FunC[step_id].FLAG_SEQ_END  = TMM_Seq_Data_C[step_id].FLAG_SEQ_END;      
   }
}

/**
 * Transfer vector items from the cell management variables to the EEPROM image
 */
PRIVATE void mdm_write_lin_seq_image(void)
{
	uint16_t step_id;

   for(step_id = 0u; step_id < LIN_STEPS_MAX; step_id++)
   {
   		mdm_ram_image.data[step_id].LIN_STEP_ID =  LIN_Seq_Data[step_id].LIN_STEP_ID;
   		mdm_ram_image.data[step_id].FLAG_LIN_STEP_END = LIN_Seq_Data[step_id].FLAG_LIN_STEP_END;
		mdm_ram_image.data[step_id].LIN_SEQ_MODE = LIN_Seq_Data[step_id].LIN_SEQ_MODE;
		mdm_ram_image.data[step_id].LIN_POWER_SUPPLY = LIN_Seq_Data[step_id].LIN_POWER_SUPPLY;
		mdm_ram_image.data[step_id].LIN_STEP_DURATION = LIN_Seq_Data[step_id].LIN_STEP_DURATION;
		mdm_ram_image.data[step_id].BRV_POSI_SET = LIN_Seq_Data[step_id].BRV_POSI_SET;
		mdm_ram_image.data[step_id].LIN_TEMP_MIN = LIN_Seq_Data[step_id].LIN_TEMP_MIN;
		mdm_ram_image.data[step_id].LIN_TEMP_MAX = LIN_Seq_Data[step_id].LIN_TEMP_MAX;
		mdm_ram_image.data[step_id].LIN_ANA_MIN = LIN_Seq_Data[step_id].LIN_ANA_MIN;
		mdm_ram_image.data[step_id].LIN_ANA_MAX = LIN_Seq_Data[step_id].LIN_ANA_MAX;
	}
}

