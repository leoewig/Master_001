/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    rtc_types.h
 *
 * description:   Private header file for Real Time Clock control.
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
 * Private header file for Real Time Clock control.
 *
 */

#if !defined(RTC_P_H)
#define      RTC_P_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "i2c_types.h"
#include "rtc_types.h"
#include "ts_types.h"

/******************************************************************************
 * PRIVATE MACRO DEFINITIONS
 *****************************************************************************/
/**
 * This set of macros can be but into a seperate header file
 * They are set up for the PCA8565
 */

/** I2C address of PCA8565 RTC chip */
#define RTC_I2C_ADDRESS (0x51u)
/** RTC chip identifier needed by the I2C driver */
#define PCA8565_DEVICE_ID   I2C_RTC_USER

/** Definition of start addresses of data */
#define PCA8565_DATA_START_ADDRESS  (0u)
/* #define PCA8565_CTR_1       (PCA8565_DATA_START_ADDRESS)    */
/* #define PCA8565_CTR_2       (PCA8565_DATA_START_ADDRESS+1u) */
#define PCA8565_TIME_START  (PCA8565_DATA_START_ADDRESS+PCA_SEC)
/* #define PCA8565_ALARM       (PCA8565_DATA_START_ADDRESS+0u) */

/* defines some specific bit meanings in PCS8565 */
/* bit values for control reg 1        */
#define PCA_CTR1_CKL_STOP     (0x20u)
#define PCA_CTR1_TEST1        (0x80u)

/* bit values for control reg 2 */
#define PCA_CTR2_AF     (0x08u)

/* bit values for control reg 2 */
#define PCA_CTR2_AIE    (0x02u)


/* battery fail but located in seconds */
#define PCA_CTR1_BAT_FAIL     (0x80u)

/* top bit of each alarm register - 0 == enabled */
#define PCA_ALARM_DISABLE    (0x80u)
#define PCA_ALARM_ENABLE     (0x00u)

#define PCA_IRQ_ENABLE       (0x02u)

#define PCA_IRQ_CLEAR        (0x00u)


/* bit values for clock out reg */
#define PCA_CLK_OUT_DISABLE   (0x00u)

/* bit values for timer control reg */
#define PCA_TIM_CTR_DISABLE   (0x00u)

/* These define the location of the PCA data in the Rx buffer */
#define PCA_CTR1           (0u)
#define PCA_CTR2           (1u)
#define PCA_SEC            (2u)
#define PCA_MIN            (3u)
#define PCA_HOUR           (4u)
#define PCA_DAY            (5u)
#define PCA_MONTH          (7u)
#define PCA_YEAR           (8u)
#define PCA_ALARM_MIN      (9u)
#define PCA_ALARM_HOUR     (10u)
#define PCA_ALARM_DAY      (11u)
#define PCA_ALARM_WEEKDAY  (12u)
#define PCA_CLKOUT_CONTROL (13u)
/* #define PCA_TIMER_CTR      (14u) keep lint happy */
/* #define PCA_TIMER          (15u) keep lint happy */

/* define valid sections of PCS8565 bcd time registers */
#define BCD_SEC_MASK  (0x7Fu)
#define BCD_MINU_MASK (0x7Fu)
#define BCD_HOUR_MASK (0x3Fu)
#define BCD_DAY_MASK  (0x3Fu)
#define BCD_MNTH_MASK (0x1Fu)
#define BCD_YEAR_MASK (0xFFu)

/* define min max data ranges for time */
#define SEC_MIN    0u
#define SEC_MAX    59u
#define MINU_MIN   0u
#define MINU_MAX   59u
#define HOURS_MIN  0u
#define HOURS_MAX  24u
#define DAYS_MIN   1u
#define DAYS_MAX   31u
#define MNTH_MIN   1u
#define MNTH_MAX   12u
#define YEAR_MIN   0u
#define YEAR_MAX   99u

#define RTC_I2C_STATE_TIMEOUT_MS    (ts_time_t) 30

/******************************************************************************
 * PRIVATE CONSTANTS DEFINITIONS
 *****************************************************************************/

/** No.of bytes of data to transmit to set an address */
#define RTC_REQUEST_LEN     (1U)
/** No. of bytes to Tx for setting clk out and timer ctl registers */
#define RTC_RESET_INIT      (RTC_REQUEST_LEN+2u)
/** No. of bytes to ready to capture control registers */
#define RTC_CONFIG_LEN        (0x02u)
/** No. of bytes to ready to capture entire dat and state info */
#define RTC_READ_LEN        (0x0fu)
/** no. of data bytes Txed to set the time */
#define RTC_TIME_SET_LEN    (8u)
/** No. of data bytes TXed to set an alarm */
#define RTC_ALARM_SET_LEN   (5u)
/** No. of data bytes TXed to set an interrupt request */
#define RTC_IRQ_SET_LEN   (2u)
/** No. of data bytes TXed to clear an interrupt request */
#define RTC_IRQ_CLEAR_LEN   (2u)
/**
 * period between clock updates in msec */
#define RTC_TIMER_UPDATE_RATE_MS        ((ts_time_t)250u)
/**
 * period between rtc initialisation attempts */
#define RTC_I2C_INITIALISATION_MS       ((ts_time_t)100u)
/**
 * period in msec above which rtc will detect the seconds are not being updated */
#define RTC_UNCHANGING_LIMIT_MS         (2000u)
/**
 * Starts the i2c state machine processing a command */
#define RTC_START_I2C_STATE_MACHINE         \
    do                                      \
    {                                       \
        rtc_i2c_state = pca_i2c_check;      \
        rtc_i2c_ts = ts_get_ms();           \
    } while (0)

/**
 * Convert a binary number to a bdc number */
#define BIN_TO_BCD(bin)     ((uint8_t)(((bin)/10u)<<4u)+((bin)%10u))
/**
 * provide a days worth of seconds if its a leap year */
#define ADD_LEAP_DAY(year)  (((year % 4u)>0u) ? 0u : SECONDS_IN_DAY)
/**
 * check whether a year is a leap year */
#define IS_LEAP_YEAR(year)  (((year % 4u)>0u) ? FALSE : TRUE)

/**
 * Value used for mark the seconds as an invalid reading
 */
#define RTC_INVALID_SECONDS 0xffu

#if defined(HOSTED_UNIT_TEST)

extern rtc_states_t  rtc_state;
extern rtc_data_t    rtc_data;
extern i2c_comms_id rtc_init_state;
extern rtc_time_stamp_t rtc_alarm_to_set;

/* extern i2c_comms_id rtc_init_state;*/
/* extern PRIVATE i2c_comms_id rtc_init_state;*/
extern rtc_i2c_trans_state rtc_i2c_state;
extern bool_t rtc_i2c_fault_occurred;

extern rtc_time_t previous_second;
extern uint16_t rtc_unchanging_ms;

extern uint8_t i2c_rx_buff[RTC_TRANS_BUFF_SIZE];
extern uint8_t i2c_tx_buff[RTC_TRANS_BUFF_SIZE];

extern bool_t rtc_wake_up_sources[Rtc_alarm_req_max];
extern bool_t rtc_alarm_active[Rtc_alarm_req_max];
extern bool_t rtc_unknown_alarm_wake_up;
extern bool_t rtc_wake_ups_sources_deduced;

extern ts_time_t rtc_ts;

#endif

PRIVATE bool_t rtc_command(i2c_comms_id cmd);
PRIVATE void rtc_handle_response(i2c_comms_id cmd);
PRIVATE bool_t rtc_init_hardware(void);
PRIVATE rtc_i2c_trans_state rtc_handle_i2c_error(I2C_ProgressType i2c_err, rtc_i2c_trans_state current_state);
PRIVATE FPTR uint8_t* rtc_create_tx_buffer(i2c_comms_id cmd);
PRIVATE bool_t rtc_status_ok(void);
PRIVATE bool_t rtc_readable(void);
PRIVATE void rtc_check_clock_running(void);
PRIVATE void rtc_check_battery_ok(void);
PRIVATE rtc_time_t rtc_calc_seconds(void);
PRIVATE rtc_time_t rtc_seconds_this_year(const rtc_time_stamp_t* time_stamp);
PRIVATE rtc_time_t rtc_seconds_complete_years(const rtc_time_stamp_t* start, const rtc_time_stamp_t* end);
PRIVATE rtc_time_t rtc_calc_time_difference(const rtc_time_stamp_t* start, const rtc_time_stamp_t* end);
PRIVATE void rtc_check_and_update_time(void);
PRIVATE void rtc_check_and_update_alarm(void);
PRIVATE void rtc_update_alarm_active(void);
PRIVATE void rtc_calc_alarm_time(rtc_time_t alarm_seconds);
PRIVATE void rtc_seconds_updated(void);
PRIVATE void rtc_setup_init_ok(rtc_init_state_t next_state);
PRIVATE rtc_i2c_trans_state rtc_await_completion(I2C_ProgressType i2c_ret, rtc_i2c_trans_state next_step);
PRIVATE void rtc_update_fault_filter(void);
PRIVATE void rtc_check_stuck_in_test(void);
PRIVATE bool_t rtc_number_of_days_valid(rtc_time_stamp_t const * time);
PRIVATE void rtc_update_shutdown(void);
PRIVATE void rtc_deduce_wake_up_sources(void);
PRIVATE bool_t rtc_process_alarms(void);
PRIVATE bool_t rtc_all_alarm_features_allowed_to_shdn(void);
PRIVATE void rtc_reset_alarm_active_flags(void);
PRIVATE bool_t rtc_set_up_I2C(void);
PRIVATE bool_t rtc_cmds_blocked(void);
PRIVATE void rtc_check_and_update_chip_alarm(void);

#endif /* RTC_P_H */
