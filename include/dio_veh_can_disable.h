/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  dio_veh_can_disable.h
 *
 * description:  
 * I/O definition for vehcile CAN transceiver disable line
 * 
 * This I/O mapping is needed by both the application and the MMC bootblock
 * so it is defined here for consistency
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
 * I/O definition for vehcile CAN transceiver disable line
 *
 * This I/O mapping is needed by both the application and the MMC bootblock
 * so it is defined here for consistency
 *
 * @file
 * @ingroup dio
 * @ingroup mmc_cfg
 * @ingroup bblk
 *
 */

#define         VEH_CAN_DIS_PORT        PORTA
#define         VEH_CAN_DIS_DDR         DDRA
#define         VEH_CAN_DIS_PIN         1u
#define         VEH_CAN_DIS_MASK        (uint8_t) (1u << VEH_CAN_DIS_PIN)

#ifdef MMC
/*
 * define the calibration override to make sure that we don't mess with the
 * transceiver if someone else is trying to
 */
extern CAL bool_t do_or_en_veh_can_dis;

/**
 * Enable the vehicle CAN transceiver and clear the high-level dio variable
 * unless the output override calibration is enabled.
 * NB If necessary, interrupts should be saved/restored around the invocation
 * of this macro.
 */
#define         ENABLE_VEHICLE_CAN_TRANSCEIVER      \
                        do { \
                            if (!do_or_en_veh_can_dis) \
                            { \
                                do_veh_can_dis = FALSE; \
                                ENABLE_VEHICLE_CAN_TRANSCEIVER_LOWLEVEL; \
                            } \
                        } while (0)
#else
/**
 * Enable the vehicle CAN transceiver
 */
#define         ENABLE_VEHICLE_CAN_TRANSCEIVER      \
                            ENABLE_VEHICLE_CAN_TRANSCEIVER_LOWLEVEL
#endif

#ifdef MMC
/**
 * Disable the vehicle CAN transceiver and set the high-level dio variable
 * unless the output override calibration is enabled.
 * NB If necessary, interrupts should be saved/restored around the invocation
 * of this macro.
 */
#define         DISABLE_VEHICLE_CAN_TRANSCEIVER      \
                        do { \
                            if (!do_or_en_veh_can_dis) \
                            { \
                                do_veh_can_dis = TRUE; \
                                DISABLE_VEHICLE_CAN_TRANSCEIVER_LOWLEVEL; \
                            } \
                        } while (0)
#else
/**
 * Disable the vehicle CAN transceiver
 */
#define         DISABLE_VEHICLE_CAN_TRANSCEIVER      \
                            DISABLE_VEHICLE_CAN_TRANSCEIVER_LOWLEVEL
#endif

/**
 * Twiddle the bits to switch on the transceiver by setting the disable output
 * low.
 */
#define         ENABLE_VEHICLE_CAN_TRANSCEIVER_LOWLEVEL      \
                        do { \
                            VEH_CAN_DIS_DDR |= VEH_CAN_DIS_MASK; \
                            VEH_CAN_DIS_PORT &= (uint8_t) ~VEH_CAN_DIS_MASK; \
                        } while (0)

/**
 * Twiddle the bits to switch off the transceiver by setting the disable output
 * high.
 */
#define         DISABLE_VEHICLE_CAN_TRANSCEIVER_LOWLEVEL      \
                        do { \
                            VEH_CAN_DIS_DDR |= VEH_CAN_DIS_MASK; \
                            VEH_CAN_DIS_PORT |= VEH_CAN_DIS_MASK; \
                        } while (0)

/**
 * Test whether the vehcile CNA transceiver is enabled (by seeing if the
 * disable output is low)
 */
#define         VEHICLE_CAN_TRANSCEIVER_ENABLED \
                        (!(VEH_CAN_DIS_PORT & VEH_CAN_DIS_MASK))
