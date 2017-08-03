/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     main_isr.h 
 *
 * description: 
 *
 * Declarations for non-maskable and unhandled interrupt service routines
 *
 * Conditional compilation
 * -----------------------
 * This file covers both the MMC and VTBM with `ifdef VTBM` used to define
 * interrupts which are unhandled on the VTBM but used on the MMC   
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
 * @file
 * @ingroup interrupts
 *
 * Declarations for non-maskable and unhandled interrupt service routines
 *
 * Conditional compilation
 * -----------------------
 * This file covers both the MMC and VTBM with `ifdef VTBM` used to define
 * interrupts which are unhandled on the VTBM but used on the MMC
 *
 */

#ifndef MAIN_ISR_H
#define MAIN_ISR_H

#include "main_isr_types.h"

/*
 * system error interrupts
 */
INTERRUPT void isr_reserved_vector(void);
INTERRUPT void isr_xgate_err(void);
INTERRUPT void isr_software_interrupt(void);
INTERRUPT void isr_illegal_instruction(void);
#ifdef VTBM
INTERRUPT void isr_sys_interrupt(void);
#endif

/************************************************************************************************************************************************************
 * CAN 4
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_CAN4_WakeupInterrupt_unhandled (void);
extern INTERRUPT void isr_CAN4_ErrorInterrupt_unhandled (void);
extern INTERRUPT void isr_CAN4_ReceiveInterrupt_unhandled (void);
extern INTERRUPT void isr_CAN4_TransmitInterrupt_unhandled (void);

/************************************************************************************************************************************************************
 * CAN 3
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_CAN3_WakeupInterrupt_unhandled (void);

/************************************************************************************************************************************************************
 * CAN 2
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_CAN2_WakeupInterrupt_unhandled (void);
extern INTERRUPT void isr_CAN2_ErrorInterrupt_unhandled (void);
extern INTERRUPT void isr_CAN2_ReceiveInterrupt_unhandled (void);
extern INTERRUPT void isr_CAN2_TransmitInterrupt_unhandled (void);


/************************************************************************************************************************************************************
 * CAN 1
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_CAN1_WakeupInterrupt_unhandled (void);
#if defined(VTBM) || defined (VRODDER)
extern INTERRUPT void isr_CAN1_ErrorInterrupt_unhandled (void);
extern INTERRUPT void isr_CAN1_ReceiveInterrupt_unhandled (void);
extern INTERRUPT void isr_CAN1_TransmitInterrupt_unhandled (void);
#endif

/************************************************************************************************************************************************************
 * CAN 0
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_CAN0_WakeupInterrupt_unhandled (void);

/************************************************************************************************************************************************************
 * ECT
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_ECT_Overflow_unhandled (void);
extern INTERRUPT void isr_ECT_Channel0_unhandled (void);
extern INTERRUPT void isr_ECT_Channel1_unhandled (void);
extern INTERRUPT void isr_ECT_Channel2_unhandled (void);
extern INTERRUPT void isr_ECT_Channel3_unhandled (void);
extern INTERRUPT void isr_ECT_Channel4_unhandled (void);
extern INTERRUPT void isr_ECT_Channel5_unhandled (void);
extern INTERRUPT void isr_ECT_Channel6_unhandled (void);
extern INTERRUPT void isr_ECT_Channel7_unhandled (void);
extern INTERRUPT void isr_ECT_Modulus_unhandled (void);

extern INTERRUPT void isr_PulseAccumulatorA_Overflow_unhandled (void);
extern INTERRUPT void isr_PulseAccumulatorB_Overflow_unhandled (void);
extern INTERRUPT void isr_PulseAccumulatorInputEdge_unhandled (void);

/************************************************************************************************************************************************************
 * PIM
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_porth_unhandled (void);
extern INTERRUPT void isr_portj_unhandled (void);
extern INTERRUPT void isr_portp_unhandled (void);

/************************************************************************************************************************************************************
 * CRG
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_RealTimeInterrupt_unhandled (void);
extern INTERRUPT void isr_CRG_PLL_Lock_unhandled (void);
extern INTERRUPT void isr_CRG_Self_Clock_Mode_unhandled (void);

/************************************************************************************************************************************************************
 * SPI
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_spi0_unhandled (void);
extern INTERRUPT void isr_spi1_unhandled (void);
extern INTERRUPT void isr_spi2_unhandled (void);

/************************************************************************************************************************************************************
 * SCI
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_sci0_unhandled (void);
extern INTERRUPT void isr_sci1_unhandled (void);
extern INTERRUPT void isr_sci2_unhandled (void);
extern INTERRUPT void isr_sci3_unhandled (void);
extern INTERRUPT void isr_sci4_unhandled (void);
#ifdef VTBM
extern INTERRUPT void isr_sci5_unhandled (void);
#endif
extern INTERRUPT void isr_sci6_unhandled (void);
#ifdef VTBM
extern INTERRUPT void isr_sci7_unhandled (void);
#endif

/************************************************************************************************************************************************************
 * ATD
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_atd0_unhandled (void);
extern INTERRUPT void isr_atd0cmp_unhandled (void);
extern INTERRUPT void isr_atd1_unhandled (void);
extern INTERRUPT void isr_atd1cmp_unhandled (void);

/************************************************************************************************************************************************************
 * IIC
 ************************************************************************************************************************************************************/
#if defined(VTBM) || defined (VRODDER)
extern INTERRUPT void isr_iic0_bus_unhandled(void);
#endif
extern INTERRUPT void isr_iic1_bus_unhandled(void);

/************************************************************************************************************************************************************
 * PWM
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_pwm_emergency_unhandled (void);

/************************************************************************************************************************************************************
 * IRQ
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_irq_unhandled (void);

/************************************************************************************************************************************************************
 * XIRQ
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_xirq_unhandled (void);

/************************************************************************************************************************************************************
 * PIT
 ************************************************************************************************************************************************************/
#ifdef VTBM
extern INTERRUPT void isr_pit0_unhandled (void);
#endif
extern INTERRUPT void isr_pit1_unhandled (void);
extern INTERRUPT void isr_pit2_unhandled (void);
extern INTERRUPT void isr_pit3_unhandled (void);
extern INTERRUPT void isr_pit4_unhandled (void);
extern INTERRUPT void isr_pit5_unhandled (void);
extern INTERRUPT void isr_pit6_unhandled (void);
extern INTERRUPT void isr_pit7_unhandled (void);

/************************************************************************************************************************************************************
 * TIM
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_tim0_unhandled (void);
extern INTERRUPT void isr_tim1_unhandled (void);
extern INTERRUPT void isr_tim2_unhandled (void);
extern INTERRUPT void isr_tim3_unhandled (void);
extern INTERRUPT void isr_tim4_unhandled (void);
extern INTERRUPT void isr_tim5_unhandled (void);
extern INTERRUPT void isr_tim6_unhandled (void);
extern INTERRUPT void isr_tim7_unhandled (void);
extern INTERRUPT void isr_tim_over_unhandled (void);
extern INTERRUPT void isr_tim_accedge_unhandled (void);
extern INTERRUPT void isr_tim_accover_unhandled (void);

/************************************************************************************************************************************************************
 * XGATE
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_xgate0_unhandled (void);
extern INTERRUPT void isr_xgate1_unhandled (void);
extern INTERRUPT void isr_xgate2_unhandled (void);
extern INTERRUPT void isr_xgate3_unhandled (void);
extern INTERRUPT void isr_xgate4_unhandled (void);
extern INTERRUPT void isr_xgate5_unhandled (void);
extern INTERRUPT void isr_xgate6_unhandled (void);
extern INTERRUPT void isr_xgate7_unhandled (void);

/************************************************************************************************************************************************************
 * Voltage Regulator
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_high_temp_unhandled (void);
extern INTERRUPT void isr_api_unhandled (void);
extern INTERRUPT void isr_lvi_unhandled (void);

/************************************************************************************************************************************************************
 * Spurious interrupt
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_spurious_unhandled (void);

/************************************************************************************************************************************************************
 * MPU Error
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_mpu_err_unhandled (void);

/************************************************************************************************************************************************************
 * FLASH
 ************************************************************************************************************************************************************/
extern INTERRUPT void isr_flash_cmd_unhandled (void);
extern INTERRUPT void isr_flash_fault_unhandled (void);

#endif /* MAIN_ISR_H */
