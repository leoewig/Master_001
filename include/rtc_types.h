/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    rtc_types.h
 *
 * description:  Header file for Real Time Clock control.
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
#if !defined(RTC_TYPES_H)
#define      RTC_TYPES_H

#include "nvs_cfg.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

#define RTC_NVS_USED (sizeof(nvs_header_s) + sizeof(rtc_nvs_data_s))

#define RTC_RESET_ALARM       0u

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * Time related constants
 *****************************************************************************/
/** Days in January  */
#define DAYS_IN_JAN     31u
/** Days in not leap year February */
#define DAYS_IN_FEB     28u
/** Days in March */
#define DAYS_IN_MAR     31u
/** Days in April */
#define DAYS_IN_APR     30u
/** Days in May */
#define DAYS_IN_MAY     31u
/** Days in June */
#define DAYS_IN_JUN     30u
/** Days in July */
#define DAYS_IN_JUL     31u
/** Days in August */
#define DAYS_IN_AUG     31u
/** Days in September */
#define DAYS_IN_SEP     30u
/** Days in October */
#define DAYS_IN_OCT     31u
/** Days in November */
#define DAYS_IN_NOV     30u
/** Days in December */
#define DAYS_IN_DEC     31u

#define RTC_MAX_YEARS   99u
#define RTC_YEAR_MOD    100u

#define MINUTES_IN_HOUR     (60u)
#define HOURS_IN_DAY        (24u)
#define MINUTES_IN_DAY      (1440u)
#define SECONDS_IN_MIN      ((uint32_t)60u)

/** Number of seconds in an hour */
#define SECONDS_IN_HOUR     ((uint32_t)3600u)
/** Number of seconds in a day */
#define SECONDS_IN_DAY      ((uint32_t)(SECONDS_IN_HOUR * 24u))
/** Number of days in a year */
#define DAYS_IN_YEAR        ((uint32_t)365u)
/** Number of seconds in a years */
#define SECONDS_IN_YEAR     ((DAYS_IN_YEAR * SECONDS_IN_DAY))
/** Number of seconds in January */
#define SECONDS_IN_JAN       (DAYS_IN_JAN * SECONDS_IN_DAY)
/** Number of seconds in February */
#define SECONDS_IN_FEB       (DAYS_IN_FEB * SECONDS_IN_DAY)
/** Number of seconds in March */
#define SECONDS_IN_MAR       (DAYS_IN_MAR * SECONDS_IN_DAY)
/** Number of seconds in April */
#define SECONDS_IN_APR       (DAYS_IN_APR * SECONDS_IN_DAY)
/** Number of seconds in May */
#define SECONDS_IN_MAY       (DAYS_IN_MAY * SECONDS_IN_DAY)
/** Number of seconds in June */
#define SECONDS_IN_JUN       (DAYS_IN_JUN * SECONDS_IN_DAY)
/** Number of seconds in July */
#define SECONDS_IN_JUL       (DAYS_IN_JUL * SECONDS_IN_DAY)
/** Number of seconds in August */
#define SECONDS_IN_AUG       (DAYS_IN_AUG * SECONDS_IN_DAY)
/** Number of seconds in September */
#define SECONDS_IN_SEP       (DAYS_IN_SEP * SECONDS_IN_DAY)
/** Number of seconds in October */
#define SECONDS_IN_OCT       (DAYS_IN_OCT * SECONDS_IN_DAY)
/** Number of seconds in November */
#define SECONDS_IN_NOV       (DAYS_IN_NOV * SECONDS_IN_DAY)
/** Number of seconds in December */
#define SECONDS_IN_DEC       (DAYS_IN_DEC * SECONDS_IN_DAY)

/**
 * The maximum number of minutes into the future that an alarm can be set
 */
#define MAX_FUTURE_ALARM    ((28u*MINUTES_IN_DAY)+(23u*MINUTES_IN_HOUR)+59u)

/**< Size in bytes of the i2c transaction buffer */
#define RTC_TRANS_BUFF_SIZE (0x0Fu)
/**
 * The rtc time type for holding the number of seconds since 1.1.2012
 */
typedef uint32_t rtc_time_t;

/**
 * RTC enumeration used to denote the status of RTC data.
 */
typedef enum
{
    Rtc_data_unknown,   /**< status of the data is unknown, e.g. it hasn't been read yet */
    Rtc_data_invalid,   /**< Data can't be read or invalid */
    Rtc_data_valid      /**< Indicates data is read, processed and available for use */
} RTC_DataValidType;

/**
 * Enumeration to identify the status of RTC communication.
 */
typedef enum
{
    Rtc_unknown,            /**< Current status of RTC device is not known                       */
    Rtc_i2c_fault,          /**< I2C fault - unable to communicate with rtc- reset rtc system    */
    Rtc_stuck_in_test,      /**< RTC device appear to be stuck in test mode- reset rtc system    */
    Rtc_clock_no_run,       /**< RTC device indicates that its clock in not running- reset rtc system */
    Rtc_low_battery,        /**< RTC indicating that there is a low battery condition on the rtc */
    Rtc_cannot_update,      /**< RTC driver indicates RTC communication but time not changing    */
    Rtc_invalid_data, 	    /**< RTC driver indicates the data returned from RTC is not in a valid format */
    Rtc_ok                  /**< Normal operation                                                */
} Rtc_status_t;

/*
 * RTC enumeratio the indicates the status of the command interface
 */
typedef enum
{
    Rtc_requested,	    /**< Request to set the time has been placed and will be processed */
    Rtc_busy	        /**< there is an operation already in progress, user must wait before trying operation again */
} Rtc_set_time_res_e;

typedef enum
{
    Rtc_invalid_request,        /**< Requested alarm time exceed maximum allowed time
                                     in the future */
    Rtc_system_not_ready,       /**< RTC system is not ready and hence alarm time in the
                                     future can't be calculated */
    Rtc_alarm_request_processed /**< Alarm request has been successfully processed */
} Rtc_set_alarm_res_e;

/**
 * rtc state machine states
 */
typedef enum
{
    Rtc_init_state,         /**< Initialization of i2c and rtc chip    */
    Rtc_idle_state,         /**< Await external requests or periodic timeout to get time */
    Rtc_process_cmd_state   /**< Process external or internal requests */
} rtc_states_t;

/**
 * Enumeration of available i2c transations supported for the pca8565
 */
typedef enum
{
    Pca_read_device, /**< read all the device and extract all information */
    Pca_set_time,
    Pca_set_alarm,
    Pca_disable_alarm,
    Pca_set_up,
    Pca_set_interrupt,
    Pca_clear_interrupt,
    Pca_max_cmds
} i2c_comms_id;


typedef struct
{
    uint8_t      bseconds;        /**< seconds (binary)    */
    uint8_t      bminutes;        /**< minutes (binary)    */
    uint8_t      bhours;          /**< hours   (binary)    */
    uint8_t      bdays;           /**< days    (binary)    */
    uint8_t      bmonths;         /**< months  (binary)    */
    uint8_t      byears;          /**< years   (binary)    */
} rtc_time_stamp_t;

/**
 * This is the main rtc structure containing the rtc current status infomation
 *  - Elaped seconds since 1st Jan 2012 #seconds,
 *  - Curent RTC status #status. see #Rtc_status_t,
 *  - Flag showing if an alarm is set #alarm_set,
 *  - Flag showing if an alarm is active #alarm_active,
 *  - variable identifing if an rtc command is currently being processed #command
 *      which is set to #Pca_max_cmds if no command is currently active,
 *  - The current time-date in binary #current,
 *  - The last alarm setting in binary #alarm.
 */
typedef struct
{
    rtc_time_t   seconds;           /**< Current time in seconds         */
    Rtc_status_t status;            /**< Current status of the rtc system*/
    bool_t       alarm_set;         /**< Is an alarm time set            */
    bool_t       alarm_active;      /**< Is the alarm currently active   */
    bool_t       alarm_irq_armed;   /**< Is the alarm interrupt output currently enabled */
    i2c_comms_id command;           /**< RTC command to process          */
    rtc_time_stamp_t alarm;         /**< Alarm time (absolute match)     */
    rtc_time_stamp_t current;       /**< Current time                    */
} rtc_data_t;


/**
 * This structure defines the bytes needed for a i2c transation;
 * Number of bytes to tx and rx and a commanded, device address.
 */
typedef struct
{
    const i2c_comms_id i2c_request_id;        /**< id of this transation */
    const uint8_t txb;                        /**< Number of bytes to tx */
    const uint8_t tx_addr;                    /**< device address to set */
    const uint8_t rxb;                        /**< Number of bytes of rx */
} i2c_req_t;

/**
 * the enum is used to control i2c transitions
 */
typedef enum
{
    pca_idle,           /**< nothing to do             */
    pca_i2c_check,      /**< check its ok to progress i2c transition */
    pca_tx,             /**< tx any bytes required     */
    pca_await_tx,       /**< await the i2c tx complete */
    pca_rx,             /**< rx any bytes required     */
    pca_await_rx        /**< await the i2c rx complete */
} rtc_i2c_trans_state;

/**
 * Enumerator denoting states of rtc device initialization state machine
 */
typedef enum
{
    Rtc_init_i2c,           /**< I2C peripheral initiazlization state */
    Rtc_setup_pca,          /**< Setting up rtc device */
    Rtc_read_check_device,  /**< Initial read and check of rtc data and registers*/
    Rtc_clear_alarm_ie_if,  /**< Clear alarm wake-up flag and disable interrupt */
    Rtc_error_occurred      /**< An error state handling state, when an error has occurred
                                 during any of above states */
} rtc_init_state_t;

#define RTC_ALARM_REQUESTER(src, inhibit)    Rtc_alarm_req_ ## src,
typedef enum
{
#include "rtc_alarm.def"
    Rtc_alarm_req_max
} rtc_alarm_req_e;
#undef RTC_ALARM_REQUESTER

/******************************************************************************
 * NVS Data Structure
 *****************************************************************************/

#define RTC_NVS_USED (sizeof(nvs_header_s) + sizeof(rtc_nvs_data_s))

/**
 * Version number for RTC NVS layout to ensure that incompatible data isn't
 * loaded after reflashing
 */
#define RTC_NVS_LAYOUT_VERSION   2u

/** Data portion of RTC feature's NVS structure. Update #RTC_NVS_LAYOUT_VERSION if changing */
typedef struct
{
    uint32_t requested_alarm_times[Rtc_alarm_req_max];  /**< Alarm time in minutes since 1.1.2012
                                                         (absolute match) for each registered
                                                         requester. Zero denotes no alarm */
} rtc_nvs_data_s;

/**
 * NVS structure type for the RTC feature
 */
typedef struct
{
    nvs_header_s header;    /**< Feature Information Structure.*/
    rtc_nvs_data_s data;     /**< Module NV data-set. */
    uint8_t reserved_bytes[(int16_t) RTC_MAX_NVS_BYTES - (int16_t) RTC_NVS_USED];
                                /**< Feature Reserved_bytes.*/
} rtc_nvs_wrapper_s;

#endif /* RTC_TYPES_H */
