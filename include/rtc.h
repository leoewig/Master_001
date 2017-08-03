/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    rtc.h 
 *
 * description:   Header file for Real Time Clock control.
 *
 *
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/

/**
 * @file
 * @ingroup rtc
 *
 * Header file for Real Time Clock control.
 *
 */
 /**
 * RTC Driver Interface
 *
 * @par A brief Introduction about the Real Time Clock:
 * The clock/date provides seconds, minutes, hours, day, date, month, and year
 * information. The real-time clock (RTC) is a low-power battery backed
 * clock/date device.
 * Programmable alarms are available for minute, hour and day (number in month).
 * Data is transferred to and from the RTC via I2C bus.
 * @par
 * Various other information is available from the RTC:
 * For example, the intgrity of the clock data and alarm status information.
 * This information is collected from the device and reflected at the RTC interface.
 *
 * @par Approach used:
 *  The RTC module provides the number of second since midnight on the 1st Jan 2012,
 *  by regulary requesting the data from the rtc chip and calculating the seconds
 *  since that date. In addition the rtc module provides methods to;
 *  - # set an alarm a number of minutes into the future for each feature listed
 *  in rtc_alarm.def file
 *  - # disable a previously set alarm,
 *  - # detect if alarm is active,
 *  - # establish the number of second to the next alarm for each feature listed
 *  in rtc_alarm.def file
 *  - # establish the current status of the rtc sub-system.
 *  - # dismiss an alarm of each feature listed
 *  in rtc_alarm.def file
 * @par Integration
 *  - In order to initialize the RTC sub-system a call needs to be make to rtc_init().
 *  - A call must be then made to rtc_update() periodically to run the RTC state machine.
 *  - Provide rtc_alarm.def configuration file and configure the build system to
 *    copy it to the build directory, renaming it to rtc_alarm.def. For the main application,
 *    this file is called mmc_rtc_alarm.def. User shall register all features
 *    (or requesters, since each is just a symbolic name to be used by alarm set,
 *    get and dismiss function to identify each alarm) in rtc_alarm.def file
 *    using RTC_ALARM_REQUESTER macro. The first argument shall be a symbolic name
 *    of requester (intended to be a name of a feature) and the second argument shall
 *    be inhibit enumeration from the ms.def file, which identifies a feature
 *    requesting an alarm. This will be used upon shutdown request to determine
 *    whether an alarm requesting feature is ready to shutdown and hence won't place
 *    any more alarm request. This method shall ensure that the rtc feature is
 *    the last one allowing the system shutdown in order to not miss any alarm requests.
 *    All alarm requests are then to be processed, the earlier one deduced and the rtc
 *    chip itself configured accordingly.
 *
 * @par Interface
 *  The following functions are provided to interface to the RTC sub-system;
 *  - # rtc_get_seconds()       - This function provide the number of elapsed seconds since
 *                                midnight on the 1st January 2012
 *  - # rtc_get_status()        - Returns the status of the RTC indicating if the data from
 *                                the RTC is valid or otherwise.
 *  - # rtc_set_alarm()         - Provides a mechanism for the application to set an RTC
 *                                alarm in minutes from the current time for each requester
 *                                (independently) registered in rtc_alarm.def file
 *  - # rtc_get_alarm()         - Returns number of seconds in future the alarm of queried
 *                                requester is set for
 *  - # rtc_is_alarm_active()   - returns TRUE if an RTC alarm of a requester specified with
 *                                the function argument is active at the moment
 *  - # rtc_dismiss_alarm()     - dismiss active alarm of a requester specified with
 *                                the function argument
 *  - # rtc_has_been_woken_by() - returns TRUE if requester specified with function argument
 *                                is responsible for waking up the system
 *
 * The RTC driver module interacts with the RTC device, in this case PCA8565 using
 * the I2C bus protocol.
 *
 * @par Limitation:
 * The RTC driver has a limitation that the function rtc_update() needs to be called
 * multiple times before it can read the time from the RTC device. However the user
 * can poll the status of the rtc system to ensure that the information received is
 * correct. See the interface descriptions of access functions for more details.
 *
 * @par Requirement:
 *
 * | Requirement     | Satisfied By             | Unit Tested By    | PAT Tested By              |
 * |-----------------|--------------------------|-------------------|----------------------------|
 * | REQ_BMS_RTC_001 | @ref rtc                 | test_rtc_update() | pat_rtc_basic()         |
 * | REQ_BMS_RTC_002 | rtc_get_seconds(), rtc_calc_seconds() | test_rtc_calc_seconds() | pat_rtc_provide_seconds() |
 * | REQ_BMS_RTC_009 | rtc_get_status()         | test_rtc_update() | pat_rtc_recovery_invalid_data() |
 * | REQ_BMS_RTC_010 | rtc_update_fault_filter(), rtc_handle_i2c_error() | - | pat_rtc_recovery_i2c_fault()|
 * | REQ_BMS_RTC_011 | rtc_check_and_update_chip_alarm(), rtc_check_and_update_time() | test_rtc_check_and_update_time(), test_rtc_check_and_update_chip_alarm() | pat_rtc_recovery_invalid_data() |
 * | REQ_BMS_RTC_012 | rtc_check_battery_ok()   | test_rtc_check_battery_ok() | pat_rtc_recovery_low_voltage() |
 * | REQ_BMS_RTC_003 | rtc_update()             | test_rtc_update() | pat_rtc_recovery_invalid_data(), pat_rtc_recovery_frozen_clock(), pat_rtc_recovery_i2c_fault(), pat_rtc_recovery_low_voltage() |
 * | REQ_BMS_RTC_004 | rtc_set_alarm(), rtc_update() | test_rtc_set_alarm() | pat_rtc_single_umc_alarm() |
 * | REQ_BMS_RTC_005 | rtc_set_alarm()          | test_rtc_set_alarm() | pat_rtc_single_umc_alarm() |
 * | REQ_BMS_RTC_017 | rtc_update_shutdown()    | -                 | pat_rtc_shutdown() |
 * | REQ_BMS_RTC_018 | rtc_update_shutdown()    | -                 | pat_rtc_shutdown() |
 * | REQ_BMS_RTC_006 | rtc_is_alarm_active(), rtc_get_alarm() | test_rtc_is_alarm_active(), test_rtc_get_alarm() | pat_rtc_single_umc_alarm() |
 * | REQ_BMS_RTC_007 | rtc_check_clock_running(), rtc_check_stuck_in_test() | test_rtc_check_clock_running(), test_rtc_check_stuck_in_test() | pat_rtc_recovery_frozen_clock(), pat_rtc_recovery_stuck_in_test_mode() |
 * | REQ_BMS_RTC_014 | rtc_update_fault_filter()| -                 | pat_rtc_recovery_i2c_fault()|
 * | REQ_BMS_RTC_015 | rtc_init_hardware()      | -                 | pat_rtc_recovery_frozen_clock(), pat_rtc_recovery_stuck_in_test_mode() |
 * | REQ_BMS_RTC_016 | rtc_update_fault_filter()| -                 | pat_rtc_recovery_i2c_fault(), pat_rtc_recovery_invalid_data(), pat_rtc_recovery_low_voltage()|
 * | REQ_BMS_RTC_008 | rtc_update_fault_filter()| -                 | pat_rtc_recovery_i2c_fault(), pat_rtc_recovery_frozen_clock(), pat_rtc_recovery_stuck_in_test_mode()|
 *
 *
 * @defgroup rtc Real Time Clock Driver
 * @ingroup platform
 */
#if !defined(RTC_H)
#define      RTC_H

#include "mmc.h"
#include "rtc_types.h"

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
extern FRAM rtc_nvs_wrapper_s rtc_ram_image;


/******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 *****************************************************************************/
void rtc_init( void );
void rtc_update( void );
uint32_t rtc_get_seconds( void );
RTC_DataValidType rtc_get_status(void);
Rtc_set_alarm_res_e rtc_set_alarm(rtc_alarm_req_e requester, uint16_t alarm_minutes);
bool_t rtc_has_been_woken_by(rtc_alarm_req_e requester);
rtc_time_t rtc_convert_date_to_rtc_time(const rtc_time_stamp_t* date_time);
bool_t rtc_is_alarm_active(rtc_alarm_req_e requester);
void rtc_dismiss_alarm(rtc_alarm_req_e requester);
uint32_t rtc_get_alarm(rtc_alarm_req_e requester);
Rtc_status_t rtc_ss_data_status(void);

extern bool_t rtc_wake_ups_sources_deduced;

extern const rtc_time_stamp_t rtc_ref_time;

/******************************************************************************
 * TEST HOOKS
 *****************************************************************************/

//#if defined(UNIT_TEST) || defined (ON_TARGET_TEST)
Rtc_set_time_res_e rtc_set_time(rtc_time_stamp_t * set);
void rtc_set_seconds(uint32_t value);
void rtc_set_status(Rtc_status_t new_status);
//#endif

#endif /* RTC_H */
