/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    main_isr_types.h
 *
 * description:  Definitions of S12 interrupt table offests
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
#ifndef MAIN_ISR_TYPES_H
#define MAIN_ISR_TYPES_H
/**
 * @file
 * @ingroup interrupts
 *
 * Definitions of S12 interrupt table offests
 *
 */

/** Vector address offset for SPURIOUS */
#define VECT_OFFSET_SPURIOUS                        0x10u


/** Vector address offset for SYSCAL */
#define VECT_OFFSET_SYSCAL                          0x12u


/** Vector address offset for MPU_Err */
#define VECT_OFFSET_MPU_Err                         0x14u


/** Vector address offset for XGATE_Err */
#define VECT_OFFSET_XGATE_Err                       0x16u


/** Vector address offset for ATD1CMP */
#define VECT_OFFSET_ATD1CMP                         0x3Cu


/** Vector address offset for ATD0CMP */
#define VECT_OFFSET_ATD0CMP                         0x3Eu


/** Vector address offset for TIM_AccEdge */
#define VECT_OFFSET_TIM_AccEdge                     0x40u


/** Vector address offset for TIM_AccOver */
#define VECT_OFFSET_TIM_AccOver                     0x42u


/** Vector address offset for TIM_Over */
#define VECT_OFFSET_TIM_Over                        0x44u


/** Vector address offset for TIM7 */
#define VECT_OFFSET_TIM7                            0x46u


/** Vector address offset for TIM6 */
#define VECT_OFFSET_TIM6                            0x48u


/** Vector address offset for TIM5 */
#define VECT_OFFSET_TIM5                            0x4Au


/** Vector address offset for TIM4 */
#define VECT_OFFSET_TIM4                            0x4Cu


/** Vector address offset for TIM3 */
#define VECT_OFFSET_TIM3                            0x4Eu


/** Vector address offset for TIM2 */
#define VECT_OFFSET_TIM2                            0x50u


/** Vector address offset for TIM1 */
#define VECT_OFFSET_TIM1                            0x52u


/** Vector address offset for TIM0 */
#define VECT_OFFSET_TIM0                            0x54u


/** Vector address offset for SCI7 */
#define VECT_OFFSET_SCI7                            0x56u


/** Vector address offset for PIT7 */
#define VECT_OFFSET_PIT7                            0x58u


/** Vector address offset for PIT6 */
#define VECT_OFFSET_PIT6                            0x5Au


/** Vector address offset for PIT5 */
#define VECT_OFFSET_PIT5                            0x5Cu


/** Vector address offset for PIT4 */
#define VECT_OFFSET_PIT4                            0x5Eu


/** Vector address offset for XGATE7 */
#define VECT_OFFSET_XGATE7                          0x64u


/** Vector address offset for XGATE6 */
#define VECT_OFFSET_XGATE6                          0x66u


/** Vector address offset for XGATE5 */
#define VECT_OFFSET_XGATE5                          0x68u


/** Vector address offset for XGATE4 */
#define VECT_OFFSET_XGATE4                          0x6Au


/** Vector address offset for XGATE3 */
#define VECT_OFFSET_XGATE3                          0x6Cu


/** Vector address offset for XGATE2 */
#define VECT_OFFSET_XGATE2                          0x6Eu


/** Vector address offset for XGATE1 */
#define VECT_OFFSET_XGATE1                          0x70u


/** Vector address offset for XGATE0 */
#define VECT_OFFSET_XGATE0                          0x72u


/** Vector address offset for PIT3 */
#define VECT_OFFSET_PIT3                            0x74u


/** Vector address offset for PIT2 */
#define VECT_OFFSET_PIT2                            0x76u


/** Vector address offset for PIT1 */
#define VECT_OFFSET_PIT1                            0x78u


/** Vector address offset for PIT0 */
#define VECT_OFFSET_PIT0                            0x7Au


/** Vector address offset for HIGH_TEMP */
#define VECT_OFFSET_HIGH_TEMP                       0x7Cu


/** Vector address offset for API */
#define VECT_OFFSET_API                             0x7Eu


/** Vector address offset for LVI */
#define VECT_OFFSET_LVI                             0x80u


/** Vector address offset for IIC1_Bus */
#define VECT_OFFSET_IIC1_Bus                        0x82u


/** Vector address offset for SCI5 */
#define VECT_OFFSET_SCI5                            0x84u


/** Vector address offset for SCI4 */
#define VECT_OFFSET_SCI4                            0x86u


/** Vector address offset for SCI3 */
#define VECT_OFFSET_SCI3                            0x88u


/** Vector address offset for SCI2 */
#define VECT_OFFSET_SCI2                            0x8Au


/** Vector address offset for PWMEmergencyShutdown */
#define VECT_OFFSET_PWMEmergencyShutdown            0x8Cu


/** Vector address offset for PortP_Interrupt */
#define VECT_OFFSET_PortP_Interrupt                 0x8Eu


/** Vector address offset for CAN4_TransmitInterrupt */
#define VECT_OFFSET_CAN4_TxInterrupt                0x90u


/** Vector address offset for CAN4_RxInterrupt */
#define VECT_OFFSET_CAN4_RxInterrupt                0x92u


/** Vector address offset for CAN4_ErrorInterrupt */
#define VECT_OFFSET_CAN4_ErrorInterrupt             0x94u


/** Vector address offset for CAN4_WakeupInterrupt */
#define VECT_OFFSET_CAN4_WakeupInterrupt            0x96u


/** Vector address offset for CAN3_TxInterrupt */
#define VECT_OFFSET_CAN3_TxInterrupt                0x98u


/** Vector address offset for CAN3_RxInterrupt */
#define VECT_OFFSET_CAN3_RxInterrupt                0x9Au


/** Vector address offset for CAN3_ErrorInterrupt */
#define VECT_OFFSET_CAN3_ErrorInterrupt             0x9Cu


/** Vector address offset for CAN3_WakeupInterrupt */
#define VECT_OFFSET_CAN3_WakeupInterrupt            0x9Eu


/** Vector address offset for CAN2_TxInterrupt */
#define VECT_OFFSET_CAN2_TxInterrupt                0xA0u


/** Vector address offset for CAN2_RxInterrupt */
#define VECT_OFFSET_CAN2_RxInterrupt                0xA2u


/** Vector address offset for CAN2_ErrorInterrupt */
#define VECT_OFFSET_CAN2_ErrorInterrupt             0xA4u


/** Vector address offset for CAN2_WakeupInterrupt */
#define VECT_OFFSET_CAN2_WakeupInterrupt            0xA6u


/** Vector address offset for CAN1_TxInterrupt */
#define VECT_OFFSET_CAN1_TxInterrupt                0xA8u


/** Vector address offset for CAN1_RxInterrupt */
#define VECT_OFFSET_CAN1_RxInterrupt                0xAAu


/** Vector address offset for CAN1_ErrorInterrupt */
#define VECT_OFFSET_CAN1_ErrorInterrupt             0xACu


/** Vector address offset for CAN1_WakeupInterrupt */
#define VECT_OFFSET_CAN1_WakeupInterrupt            0xAEu


/** Vector address offset for CAN0_TxInterrupt */
#define VECT_OFFSET_CAN0_TxInterrupt                0xB0u


/** Vector address offset for CAN0_RxInterrupt */
#define VECT_OFFSET_CAN0_RxInterrupt                0xB2u


/** Vector address offset for CAN0_ErrorInterrupt */
#define VECT_OFFSET_CAN0_ErrorInterrupt             0xB4u


/** Vector address offset for CAN0_WakeupInterrupt */
#define VECT_OFFSET_CAN0_WakeupInterrupt            0xB6u


/** Vector address offset for FLASH */
#define VECT_OFFSET_FLASH                           0xB8u


/** Vector address offset for FLASH_Fault */
#define VECT_OFFSET_FLASH_Fault                     0xBAu


/** Vector address offset for SPI2 */
#define VECT_OFFSET_SPI2                            0xBCu


/** Vector address offset for SPI1 */
#define VECT_OFFSET_SPI1                            0xBEu


/** Vector address offset for IIC0_Bus */
#define VECT_OFFSET_IIC0_Bus                        0xC0u


/** Vector address offset for SCI6 */
#define VECT_OFFSET_SCI6                            0xC2u


/** Vector address offset for CRG_Self_Clock_Mode */
#define VECT_OFFSET_CRG_Self_Clock_Mode             0xC4u


/** Vector address offset for CRG_PLL_Lock */
#define VECT_OFFSET_CRG_PLL_Lock                    0xC6u


/** Vector address offset for PulseAccumulatorB_Overflow */
#define VECT_OFFSET_PulseAccumulatorB_Overflow      0xC8u


/** Vector address offset for ECT_ModulusInterrupt */
#define VECT_OFFSET_ECT_ModulusInterrupt            0xCAu


/** Vector address offset for PortH_Interrupt */
#define VECT_OFFSET_PortH_Interrupt                 0xCCu


/** Vector address offset for PortJ_Interrupt */
#define VECT_OFFSET_PortJ_Interrupt                 0xCEu


/** Vector address offset for ATD1 */
#define VECT_OFFSET_ATD1                            0xD0u


/** Vector address offset for ATD0 */
#define VECT_OFFSET_ATD0                            0xD2u


/** Vector address offset for SCI1 */
#define VECT_OFFSET_SCI1                            0xD4u


/** Vector address offset for SCI0 */
#define VECT_OFFSET_SCI0                            0xD6u


/** Vector address offset for SPI0 */
#define VECT_OFFSET_SPI0                            0xD8u


/** Vector address offset for PulseAccumulatorInputEdge */
#define VECT_OFFSET_PulseAccumulatorInputEdge       0xDAu


/** Vector address offset for PulseAccumulatorA_Overflow */
#define VECT_OFFSET_PulseAccumulatorA_Overflow      0xDCu


/** Vector address offset for ECT_Overflow */
#define VECT_OFFSET_ECT_Overflow                    0xDEu


/** Vector address offset for ECT_Channel7Interrupt */
#define VECT_OFFSET_ECT_Channel7Interrupt           0xE0u


/** Vector address offset for ECT_Channel6Interrupt */
#define VECT_OFFSET_ECT_Channel6Interrupt           0xE2u


/** Vector address offset for ECT_Channel5Interrupt */
#define VECT_OFFSET_ECT_Channel5Interrupt           0xE4u


/** Vector address offset for ECT_Channel4Interrupt */
#define VECT_OFFSET_ECT_Channel4Interrupt           0xE6u


/** Vector address offset for ECT_Channel3Interrupt */
#define VECT_OFFSET_ECT_Channel3Interrupt           0xE8u


/** Vector address offset for ECT_Channel2Interrupt */
#define VECT_OFFSET_ECT_Channel2Interrupt           0xEAu


/** Vector address offset for ECT_Channel1Interrupt */
#define VECT_OFFSET_ECT_Channel1Interrupt           0xECu


/** Vector address offset for ECT_Channel0Interrupt */
#define VECT_OFFSET_ECT_Channel0Interrupt           0xEEu


/** Vector address offset for RealTimeInterrupt */
#define VECT_OFFSET_RealTimeInterrupt               0xF0u


/** Vector address offset for IRQ_Interrupt */
#define VECT_OFFSET_IRQ_Interrupt                   0xF2u


/** Vector address offset for XIRQ_Interrupt */
#define VECT_OFFSET_XIRQ_Interrupt                  0xF4u


/** Vector address offset for SW_Interrupt */
#define VECT_OFFSET_SW_Interrupt                    0xF6u


/** Vector address offset for Instr_Trap */
#define VECT_OFFSET_Instr_Trap                      0xF8u

#endif /* MAIN_ISR_TYPES_H */
