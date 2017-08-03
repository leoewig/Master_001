/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     cal_offline.c 
 *
 * description:   MMC offline calibration data
 *
 *
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           MMC offline calibration data
 *
 *****************************************************************************/


/**
 * Upper clip limit for calibrated temperature offset */
#define CAL_MAX_TEMP_OFFSET TEMP(3)

/** Lower clip limit for calibrated temperature offset */
#define CAL_MIN_TEMP_OFFSET TEMP(-3)

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "mmc.h"
#include "fm.h"
#include "cal_offline.h"
#include "cal_offline_p.h"
#include "crc.h"
#include "rtc.h"


/******************************************************************************
 * PUBLIC DATA
 *****************************************************************************/

/**
 * MMC offline calibration data structure mapped to absolute location.
 * @note the initialiser won't be present in the S-record because output
 * to .offlinecal is inhibited by mmc.lkf.  This is necessary to prevent
 * overwriting a cal when reprogramming a module with a new application.
 */

#ifndef HOSTED
#pragma section @gpage @far const {offlinecal}
#endif

//FROM cal_pack_offline_t  cal_offline_data = CAL_OFFLINE_DUMMY_INITIALISER;

#ifndef HOSTED
#pragma section @gpage @far const []
#endif


//cal_off_data_status_t cal_offline_data1_status  = cal_off_data_not_checked_yet;


//cal_off_data_status_t cal_offline_data2_status  = cal_off_data_not_checked_yet;

/** cell manufacturing date in seconds relative to the RTC reference date/time 
 */
uint32_t    cal_off_cell_man_timestamp          = 0u;

/** flag indicating whether the cell manufacturing date represented by 
 *  cal_off_cell_man_timestamp is valid or not 
 */
bool_t      cal_off_cell_man_timestamp_valid    = FALSE;

/** flag indicating whether the pack serial number data is valid or not 
 */
bool_t      cal_off_pack_sn_valid               = FALSE;


/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/



/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/**
 * Read offline calibration data from dedicated memory location and check its
 * validity
 */
void cal_offline_init(void)
{
//    bool_t fault_present = FALSE;
//    Fm_fault_subtype_list_e subtype = Fm_general_failure_information;
//
//    cal_offline_check_data1();
//    cal_offline_check_data2();
//
//    cal_off_set_cell_manufacture_date();
//    cal_off_pack_sn_valid = (bool_t)(cal_offline_data2_status == cal_off_data_ok);
//    
//    /* dataset 1 takes priority as all units must support it */
//    if (cal_off_data_ok != cal_offline_data1_status)
//    {
//        subtype = Fm_posn_1;
//        fault_present = TRUE;
//    }
//    else if (cal_off_data_ok != cal_offline_data2_status)
//    {
//        subtype = Fm_posn_2;
//        fault_present = TRUE;
//    }
//    else
//    {
//        /* MISRA */
//    }
//
//    (void) fm_update_fault_filter(Fm_plat_mmc_missing_offline_cal,
//                                  subtype,
//                                  fault_present,
//                                  TRUE);
}


/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/**
 * Verify the validity of the oflline calibration dataset 1.
 */
PRIVATE void cal_offline_check_data1(void)
{
//   uint16_t cal_offline_crc;
//   FROM cal_pack_offline_set1_t *data1p = &(cal_offline_data.cal_set1);
//
//   cal_offline_data1_status = cal_off_data_ok;
//
//   if ((data1p->validity_marker_1 != PACK_OFFLINECAL1_VALIDITY1)
//   ||  (data1p->validity_marker_2 != PACK_OFFLINECAL1_VALIDITY2))
//   {
//       /* validity marked is of wrong value */
//       cal_offline_data1_status = cal_off_data_wrong_format;
//   }
//
//   if ((cal_offline_data1_status == cal_off_data_ok)
//       && (data1p->data.layout_version != PACK_OFFLINECAL1_VERSION))
//   {
//       /* unsupported data layout version */
//       cal_offline_data1_status = cal_off_data_wrong_format;
//   }
//
//   if ((cal_offline_data1_status == cal_off_data_ok) && (data1p->data.padding != CAL_PADDING))
//   {
//       /* padding is of wrong value */
//       cal_offline_data1_status = cal_off_data_wrong_format;
//   }
//
//   if (cal_offline_data1_status == cal_off_data_ok)
//   {
//       cal_offline_crc = crc16(CRC16_SEED, (const FPTR uint8_t *) &(data1p->data),
//               sizeof(data1p->data) );
//       if ( data1p->checksum != cal_offline_crc )
//       {
//           cal_offline_data1_status = cal_off_data_invalid_crc;
//       }
//   }
//
//   /* Block is confirmed correct format and not corrupt, check payload is
//    * within acceptable limits */
//   if (cal_offline_data1_status == cal_off_data_ok)
//   {
//       cal_offline_data1_status = cal_offline_vtbm_t_range_check();
//   }
}

/**
 * Verify the validity of the oflline calibration dataset 2.
 */
PRIVATE void cal_offline_check_data2(void)
{
//    uint16_t cal_offline_crc;
//    FROM cal_pack_offline_set2_t *data2p = &(cal_offline_data.cal_set2);
//
//    cal_offline_data2_status = cal_off_data_ok;
//    
//    if ((data2p->validity_marker_1 != PACK_OFFLINECAL2_VALIDITY1)
//    ||  (data2p->validity_marker_2 != PACK_OFFLINECAL2_VALIDITY2))
//    {
//        /* validity marked is of wrong value */
//        cal_offline_data2_status = cal_off_data_wrong_format;
//    }
//
//    if ((cal_offline_data2_status == cal_off_data_ok)
//        && (data2p->data.layout_version != PACK_OFFLINECAL2_VERSION))
//    {
//        /* unsupported data layout version */
//        cal_offline_data2_status = cal_off_data_wrong_format;
//    }
//
//    if ((cal_offline_data2_status == cal_off_data_ok) && (data2p->data.padding != CAL_PADDING))
//    {
//        /* padding is of wrong value */
//        cal_offline_data2_status = cal_off_data_wrong_format;
//    }
//
//    if (cal_offline_data2_status == cal_off_data_ok)
//    {
//        cal_offline_crc = crc16(CRC16_SEED, (const FPTR uint8_t *) &(data2p->data),
//                sizeof(data2p->data) );
//               
//        if ( data2p->checksum != cal_offline_crc )
//        {
//            cal_offline_data2_status = cal_off_data_invalid_crc;
//        }
//    }
}

/**
 * Check the temperature range of all the VTBM data in the cal block */
PRIVATE cal_off_data_status_t cal_offline_vtbm_t_range_check(void)
{
//    uint8_t vn,nc;
//    temperature_t offset;
//    cal_off_data_status_t res = cal_off_data_ok;
//
//    for (vn=0u; vn<MAX_NUM_VTBM; ++vn)
//    {
//        for (nc=0u; nc< MAX_VTBM_CELLS; ++nc)
//        {
//            offset = cal_offline_data.cal_set1.data.cell_t_offset[vn][nc];
//            /* If any offset is above or below the limit,
//            * consider the whole block invalid */
//            if ((offset > CAL_MAX_TEMP_OFFSET)
//            ||  (offset < CAL_MIN_TEMP_OFFSET))
//            {
//                res = cal_off_data_bad_range;
//            }
//        }
//    }
//    return res;
}

/**
 * Converts the offline calibration date to RTC time if data is available and
 * in range.
 */
PRIVATE void cal_off_set_cell_manufacture_date(void)
{
//    cal_off_cell_man_timestamp_valid = FALSE;
//
//    if (cal_offline_data2_status == cal_off_data_ok)
//    {
//        uint16_t yr  = cal_offline_data.cal_set2.data.yr_of_cell_manufacture;
//        uint8_t  mth = cal_offline_data.cal_set2.data.mth_of_cell_manufacture;
//        uint8_t  day = cal_offline_data.cal_set2.data.day_of_cell_manufacture;
//        
//        /* RTC year is represented as number of years since 2000 
//         * The oldest allowed date is 1/1/2012 */
//        if ((yr >= 2012u) && (yr <= (2000u + RTC_MAX_YEARS))
//        &&  (mth >= 1u) && (mth <= 12u)
//        &&  (day >= 1u) && (day <= 31u))
//        {
//            rtc_time_stamp_t date_time = { 0u };
//            date_time.bdays = day;
//            date_time.bmonths = mth;
//            date_time.byears = (uint8_t)(yr - 2000u);
//            cal_off_cell_man_timestamp = rtc_convert_date_to_rtc_time(&date_time);
//            cal_off_cell_man_timestamp_valid = TRUE;
//        }
//    }
}
