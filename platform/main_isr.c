/**
 * Non-maskable and 'unused' interrupt service routines
 *
 * @file
 * @ingroup interrupts
 *
 * Conditional compilation
 * -----------------------
 * This file covers both the MMC and VTBM with `ifdef VTBM` used to define
 * interrupts which are unhandled on the VTBM but used on the MMC
 * $Id: main_isr.c 6174 2013-01-15 15:40:12Z gvs $
 */
/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#include "basic_types.h"
#include "hardware.h"
#include "main_isr.h"
#include "isr.h"


#define         APP_FATAL             INTERNAL_ERROR
/*
 * Non-maskable interrupts
 */

/**
 * @lint -esym(551,isr_error_ppage) not accessed - this is a debug variable
 */
/*lint -esym(551,isr_error_ppage) */
PRIVATE uint8_t isr_error_ppage; /**< PPAGE at time of error */

/**
 * @lint -esym(551,isr_error_address) not accessed - this is a debug variable
 */
/*lint -esym(551,isr_error_address) */
PRIVATE uint16_t isr_error_address; /**< logical address at time of error */

/**
 * interrupt handler for reserved interrupt vectors
 * If this is ever called, it's a serious problem because it should be
 * impossible for the hardware to generate the interrupt
 *
 * @todo unify with error handling strategy
 */
INTERRUPT void isr_reserved_vector(void)
{
    BREAK_IF_DEBUGGING ;
    APP_FATAL();    /* label in crtsi2.s which resets stack, restarts
               application  and will re-enable interrupts in
               due course */
}
/**
 * interrupt handler for illegal instruction execution
 *
 * If this is ever called, it's a serious problem because our code is
 * corrupt.
 *
 * @todo unify with error handling strategy
 *
 * Implementation
 * --------------
 * Function must use statics rather than automatics because we are
 * extracting info from the stack frame itself.
 */
INTERRUPT void isr_illegal_instruction(void)
{
    CAPTURE_ADDRESS(isr_error_ppage,isr_error_address);

    BREAK_IF_DEBUGGING ;
    APP_FATAL();    /* label in crtsi2.s which resets stack, restarts
               application  and will re-enable interrupts in
               due course */
}
/**
 * Interrupt handler for XGATE errors (illegal instruction or
 * illegal access)
 *
 * If this is ever called, it's a serious problem because our XGATE code
 * is corrupt
 *
 * @todo unify with error handling strategy
 *
 * Implementation
 * --------------
 * Function must use statics rather than automatics because we are
 * extracting info from the stack frame itself.
 */
INTERRUPT void isr_xgate_err(void)
{
    /*
     * get the address, but since the error was on the XGATE, it's probably
     * not very useful
     */
    CAPTURE_ADDRESS(isr_error_ppage,isr_error_address);
    BREAK_IF_DEBUGGING ;
    APP_FATAL();    /* label in crtsi2.s which resets stack, restarts
               application  and will re-enable interrupts in
               due course */
}

/**
 * interrupt handler for SWI (software interrupt) instruction
 *
 * We don't call SWI on purpose so this interrupt shows something is
 * wrong
 *
 * @todo unify with error handling strategy
 *
 * Implementation
 * --------------
 * Function must use statics rather than automatics because we are
 * extracting info from the stack frame itself.
 */
INTERRUPT void isr_software_interrupt(void)
{
    CAPTURE_ADDRESS(isr_error_ppage,isr_error_address);

    BREAK_IF_DEBUGGING ;
    APP_FATAL();    /* label in crtsi2.s which resets stack, restarts
               application  and will re-enable interrupts in
               due course */
}

#ifdef VTBM
/**
 * interrupt handler for SYS (system call) instruction
 *
 * We don't call SYS on purpose so this interrupt shows something is
 * wrong
 *
 * @todo unify with VTBM error handling strategy
 *
 * Implementation
 * --------------
 * Function must use statics rather than automatics because we are
 * extracting info from the stack frame itself.
 */
INTERRUPT void isr_sys_interrupt(void)
{
    CAPTURE_ADDRESS(isr_error_ppage,isr_error_address);

    BREAK_IF_DEBUGGING ;
    APP_FATAL();    /* label in crtsi2.s which resets stack, restarts
               application  and will re-enable interrupts in
               due course */
}
#endif

/************************************************************************************************************************************************************
 * Unhandled interrupts
 ************************************************************************************************************************************************************/

/**
 * called when one of the *_unhandled interrupts occur
 *
 * If the debugger is attached, debug mode is triggered, otherwise
 * a call is made to log the interrupt
 *
 * To find the source, look up the interrupt which corresponds to vect_offset
 * @param[in] vect_offset The offset from Vector Base at which this interrupt
 * is located.
 */
FCODE void sch_isr_Unhandled (uint8_t vect_offset)
{
    BREAK_IF_DEBUGGING;
    /** @todo unhandled interrupt should set a DTC */
   (void) set_interrupt_level(vect_offset, 0u, INT_CPU_DEST);
}

/*
 * Interrupt service routines are located in an unpaged area of ROM
 */
/************************************************************************************************************************************************************
 * CAN 4
 ************************************************************************************************************************************************************/

INTERRUPT void isr_CAN4_WakeupInterrupt_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_CAN4_WakeupInterrupt);
}

INTERRUPT void isr_CAN4_ErrorInterrupt_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_CAN4_ErrorInterrupt);
}

INTERRUPT void isr_CAN4_ReceiveInterrupt_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_CAN4_RxInterrupt);
}

INTERRUPT void isr_CAN4_TransmitInterrupt_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_CAN4_TxInterrupt);
}

/************************************************************************************************************************************************************
 * CAN 3
 ************************************************************************************************************************************************************/

INTERRUPT void isr_CAN3_WakeupInterrupt_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_CAN3_WakeupInterrupt);
}

/************************************************************************************************************************************************************
 * CAN 2
 ************************************************************************************************************************************************************/

INTERRUPT void isr_CAN2_WakeupInterrupt_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_CAN2_WakeupInterrupt);
}

INTERRUPT void isr_CAN2_ErrorInterrupt_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_CAN2_ErrorInterrupt);
}

//#if defined(VTBM) || defined (VRODDER)
INTERRUPT void isr_CAN2_ReceiveInterrupt_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_CAN2_RxInterrupt);
}

INTERRUPT void isr_CAN2_TransmitInterrupt_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_CAN2_TxInterrupt);
}
//#endif

/************************************************************************************************************************************************************
 * CAN 1
 ************************************************************************************************************************************************************/

INTERRUPT void isr_CAN1_WakeupInterrupt_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_CAN1_WakeupInterrupt);
}


#if defined(VTBM) || defined (VRODDER)
INTERRUPT void isr_CAN1_ErrorInterrupt_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_CAN1_ErrorInterrupt);
}

INTERRUPT void isr_CAN1_ReceiveInterrupt_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_CAN1_RxInterrupt);
}

INTERRUPT void isr_CAN1_TransmitInterrupt_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_CAN1_TxInterrupt);
}
#endif


/************************************************************************************************************************************************************
 * CAN 0
 ************************************************************************************************************************************************************/

INTERRUPT void isr_CAN0_WakeupInterrupt_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_CAN0_WakeupInterrupt);
}

/************************************************************************************************************************************************************
 * ECT
 ************************************************************************************************************************************************************/

INTERRUPT void isr_ECT_Overflow_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_ECT_Overflow);
}
INTERRUPT void isr_ECT_Channel0_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_ECT_Channel0Interrupt);
}
INTERRUPT void isr_ECT_Channel1_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_ECT_Channel1Interrupt);
}
INTERRUPT void isr_ECT_Channel2_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_ECT_Channel2Interrupt);
}
INTERRUPT void isr_ECT_Channel3_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_ECT_Channel3Interrupt);
}
INTERRUPT void isr_ECT_Channel4_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_ECT_Channel4Interrupt);
}
INTERRUPT void isr_ECT_Channel5_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_ECT_Channel5Interrupt);
}
INTERRUPT void isr_ECT_Channel6_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_ECT_Channel6Interrupt);
}
INTERRUPT void isr_ECT_Channel7_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_ECT_Channel7Interrupt);
}
INTERRUPT void isr_ECT_Modulus_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_ECT_ModulusInterrupt);
}

INTERRUPT void isr_PulseAccumulatorA_Overflow_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_PulseAccumulatorA_Overflow);
}

INTERRUPT void isr_PulseAccumulatorB_Overflow_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_PulseAccumulatorB_Overflow);
}

INTERRUPT void isr_PulseAccumulatorInputEdge_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_PulseAccumulatorInputEdge);
}


/************************************************************************************************************************************************************
 * PIM
 ************************************************************************************************************************************************************/

/*!
 * @brief      PortH Interrupt
 * @return     Nothing
 */
INTERRUPT void isr_porth_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_PortJ_Interrupt);
}

/*!
 * @brief      PortJ Interrupt
 * @return     Nothing
 */
INTERRUPT void isr_portj_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_PortJ_Interrupt);
}

/*!
 * @brief      PortP Interrupt
 * @return     Nothing
 */
INTERRUPT void isr_portp_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_PortP_Interrupt);
}

/************************************************************************************************************************************************************
 * CRG
 ************************************************************************************************************************************************************/

/*!
 * @brief      Real Time Interrupt
 * @return     Nothing
 */
INTERRUPT void isr_RealTimeInterrupt_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_RealTimeInterrupt);

}

INTERRUPT void isr_CRG_PLL_Lock_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_CRG_PLL_Lock);
}

INTERRUPT void isr_CRG_Self_Clock_Mode_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_CRG_Self_Clock_Mode);
}

/************************************************************************************************************************************************************
 * SPI
 ************************************************************************************************************************************************************/

INTERRUPT void isr_spi0_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_SPI0);
}

INTERRUPT void isr_spi1_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_SPI1);
}

INTERRUPT void isr_spi2_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_SPI2);
}

/************************************************************************************************************************************************************
 * SCI
 ************************************************************************************************************************************************************/

INTERRUPT void isr_sci0_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_SCI0);
}

INTERRUPT void isr_sci1_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_SCI1);
}

INTERRUPT void isr_sci2_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_SCI2);
}

INTERRUPT void isr_sci3_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_SCI3);
}

INTERRUPT void isr_sci4_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_SCI4);
}

#ifdef VTBM
INTERRUPT void isr_sci5_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_SCI5);
}
#endif

INTERRUPT void isr_sci6_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_SCI6);
}

#ifdef VTBM
INTERRUPT void isr_sci7_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_SCI7);
}
#endif

/************************************************************************************************************************************************************
 * ATD
 ************************************************************************************************************************************************************/

INTERRUPT void isr_atd0_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_ATD0);
}

INTERRUPT void isr_atd0cmp_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_ATD0CMP);
}

INTERRUPT void isr_atd1_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_ATD1);
}

INTERRUPT void isr_atd1cmp_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_ATD1CMP);
}

/************************************************************************************************************************************************************
 * IIC
 ************************************************************************************************************************************************************/
#if defined(VTBM) || defined (VRODDER)
INTERRUPT void isr_iic0_bus_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_IIC0_Bus);
}
#endif
INTERRUPT void isr_iic1_bus_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_IIC1_Bus);
}

/************************************************************************************************************************************************************
 * PWM
 ************************************************************************************************************************************************************/

INTERRUPT void isr_pwm_emergency_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_PWMEmergencyShutdown);
}

/************************************************************************************************************************************************************
 * IRQ
 ************************************************************************************************************************************************************/

INTERRUPT void isr_irq_unhandled (void)
{
   /* NB The IRQ is cleared by the associated RTI instruction */
   sch_isr_Unhandled(VECT_OFFSET_IRQ_Interrupt);
}

/************************************************************************************************************************************************************
 * XIRQ
 ************************************************************************************************************************************************************/

INTERRUPT void isr_xirq_unhandled (void)
{
   /* NB The XIRQ is cleared by the associated RTI instruction */
   sch_isr_Unhandled(VECT_OFFSET_XIRQ_Interrupt);
}


/************************************************************************************************************************************************************
 * PIT
 ************************************************************************************************************************************************************/

#ifdef VTBM
INTERRUPT void isr_pit0_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_PIT0);
}
#endif

INTERRUPT void isr_pit1_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_PIT1);
}

INTERRUPT void isr_pit2_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_PIT2);
}

#ifdef VTBM
INTERRUPT void isr_pit3_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_PIT3);
}
#endif

INTERRUPT void isr_pit4_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_PIT4);
}

INTERRUPT void isr_pit5_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_PIT5);
}

INTERRUPT void isr_pit6_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_PIT6);
}

INTERRUPT void isr_pit7_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_PIT7);
}

/************************************************************************************************************************************************************
 * TIM
 ************************************************************************************************************************************************************/

INTERRUPT void isr_tim0_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_TIM0);
}

INTERRUPT void isr_tim1_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_TIM1);
}

INTERRUPT void isr_tim2_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_TIM2);
}

INTERRUPT void isr_tim3_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_TIM3);
}

INTERRUPT void isr_tim4_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_TIM4);
}

INTERRUPT void isr_tim5_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_TIM5);
}

INTERRUPT void isr_tim6_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_TIM6);
}

INTERRUPT void isr_tim7_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_TIM7);
}

INTERRUPT void isr_tim_over_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_TIM_Over);
}

INTERRUPT void isr_tim_accedge_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_TIM_AccEdge);
}

INTERRUPT void isr_tim_accover_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_TIM_AccOver);
}

/************************************************************************************************************************************************************
 * XGATE
 ************************************************************************************************************************************************************/

INTERRUPT void isr_xgate0_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_XGATE0);
}

INTERRUPT void isr_xgate1_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_XGATE1);
}

INTERRUPT void isr_xgate2_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_XGATE2);
}

INTERRUPT void isr_xgate3_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_XGATE3);
}

INTERRUPT void isr_xgate4_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_XGATE4);
}

INTERRUPT void isr_xgate5_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_XGATE5);
}

INTERRUPT void isr_xgate6_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_XGATE6);
}

INTERRUPT void isr_xgate7_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_XGATE7);
}

/************************************************************************************************************************************************************
 * Voltage Regulator
 ************************************************************************************************************************************************************/

INTERRUPT void isr_high_temp_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_HIGH_TEMP);
}

INTERRUPT void isr_api_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_API);
}

INTERRUPT void isr_lvi_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_LVI);
}

/************************************************************************************************************************************************************
 * Spurious interrupt
 ************************************************************************************************************************************************************/

/**
 * spurious interrupt handler
 *
 * Spurious interrupts are generated when an interrupt flag is cleared between
 * the interrupt being generated and its ISR being called, presumably because
 * either:
 * 	i. a higher priority interrupt gets in first and clears the
 * 	   lower-priority interrupt flag
 * 	ii. interrupts are disabled and the flag gets cleared before interrupts
 * 	   are re-enabled.
 * In either case, it is probably not what you meant to happen!
 */
INTERRUPT void isr_spurious_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_SPURIOUS);
}

/************************************************************************************************************************************************************
 * MPU Error
 ************************************************************************************************************************************************************/

/**
 * Interrupt handler for memory protection unit.
 * MPU interrupts are non-maskable but, since we don't currently enable the
 * MPU, it is reasonable to treat the interrupt in the same way as all other
 * unhandled interrupts
 */
INTERRUPT void isr_mpu_err_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_MPU_Err);
}

/************************************************************************************************************************************************************
 * FLASH
 ************************************************************************************************************************************************************/

INTERRUPT void isr_flash_cmd_unhandled (void)
{
   sch_isr_Unhandled(VECT_OFFSET_FLASH);
}

/**
 * interrupt caused by a Flash read error
 *
 * @todo shouldn't we be handling this?
 */
INTERRUPT void isr_flash_fault_unhandled (void)
{
  sch_isr_Unhandled(VECT_OFFSET_FLASH_Fault);
}

