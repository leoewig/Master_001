/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  dio_chg_can_disable.h
 *
 * description:  
 * I/O definition for charger CAN transceiver disable line
 * 
 *  
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
/**
 * I/O definition for charger CAN transceiver disable line
 *
 * @file
 * @ingroup dio
 * @ingroup mmc_cfg
 *
 */

#define         CHG_CAN_DIS_PORT        PORTA
#define         CHG_CAN_DIS_DDR         DDRA
#define         CHG_CAN_DIS_PIN         2u
#define         CHG_CAN_DIS_MASK        (uint8_t) (1u << CHG_CAN_DIS_PIN)

#ifdef MMC
/*
 * define the calibration override to make sure that we don't mess with the
 * transceiver if someone else is trying to
 */
extern CAL bool_t do_or_en_chg_can_dis;

/**
 * Enable the charger CAN transceiver and clear the high-level dio variable
 * unless the output override calibration is enabled.
 * NB If necessary, interrupts should be saved/restored around the invocation
 * of this macro.
 */
#define         ENABLE_CHARGER_CAN_TRANSCEIVER      \
                        do { \
                            if (!do_or_en_chg_can_dis) \
                            { \
                                do_chg_can_dis = FALSE; \
                                ENABLE_CHARGER_CAN_TRANSCEIVER_LOWLEVEL; \
                            } \
                        } while (0)
#else
/**
 * Enable the charger CAN transceiver
 */
#define         ENABLE_CHARGER_CAN_TRANSCEIVER      \
                            ENABLE_CHARGER_CAN_TRANSCEIVER_LOWLEVEL
#endif

#ifdef MMC
/**
 * Disable the charger CAN transceiver and set the high-level dio variable
 * unless the output override calibration is enabled.
 * NB If necessary, interrupts should be saved/restored around the invocation
 * of this macro.
 */
#define         DISABLE_CHARGER_CAN_TRANSCEIVER      \
                        do { \
                            if (!do_or_en_chg_can_dis) \
                            { \
                                do_chg_can_dis = TRUE; \
                                DISABLE_CHARGER_CAN_TRANSCEIVER_LOWLEVEL; \
                            } \
                        } while (0)
#else
/**
 * Disable the charger CAN transceiver
 */
#define         DISABLE_CHARGER_CAN_TRANSCEIVER      \
                            DISABLE_CHARGER_CAN_TRANSCEIVER_LOWLEVEL
#endif

/**
 * Twiddle the bits to switch on the transceiver by setting the disable output
 * low.
 */
#define         ENABLE_CHARGER_CAN_TRANSCEIVER_LOWLEVEL      \
                        do { \
                            CHG_CAN_DIS_DDR |= CHG_CAN_DIS_MASK; \
                            CHG_CAN_DIS_PORT &= (uint8_t) ~CHG_CAN_DIS_MASK; \
                        } while (0)

/**
 * Twiddle the bits to switch off the transceiver by setting the disable output
 * high.
 */
#define         DISABLE_CHARGER_CAN_TRANSCEIVER_LOWLEVEL      \
                        do { \
                            CHG_CAN_DIS_DDR |= CHG_CAN_DIS_MASK; \
                            CHG_CAN_DIS_PORT |= CHG_CAN_DIS_MASK; \
                        } while (0)

/**
 * Test whether the charger CNA transceiver is enabled (by seeing if the
 * disable output is low)
 */
#define         CHARGER_CAN_TRANSCEIVER_ENABLED \
                        (!(CHG_CAN_DIS_PORT & CHG_CAN_DIS_MASK))
