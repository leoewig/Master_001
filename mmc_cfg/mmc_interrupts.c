/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_interrupts.c
 *
 * description:  
 * C structure to implement the MMC interrupt jump table.
 *
 * We have converted the jump table into C to let us use header files for
 * function prototypes which gives us some benefit from near/far type checking.
 * (Sadly Cosmic doesn't check for @ interrupt agreement)
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/

#include "basic_types.h"
#include "crtsi2.h"
#include "hardware.h"   /* for BREAK_IF_DEBUGGING */
#include "main_isr.h"
#include "mmc_ican.h"
#include "scheduler.h"
#include "pit.h"
#include "gate.h"
#include "i2c.h"
#include "exc.h"
#include "ts.h"
#include "flash_fault.h"
//#include "SM1.h"
#include "ect.h"
#include "lin.h"
#include "sci.h"
#include "mmc_aps.h"
#include "smm.h"

/*
 * CANbedded dummy handler declarations for POLYSPACE checking
 */
#if defined HOSTED || defined POLYSPACE
void CanTxInterrupt_0(void);
void CanRxInterrupt_0(void);
void CanErrorInterrupt_0(void);
void CanTxInterrupt_1(void);
void CanRxInterrupt_1(void);
void CanErrorInterrupt_1(void);
void CanTxInterrupt_3(void);
void CanRxInterrupt_3(void);
void CanErrorInterrupt_3(void);
#endif /* POLYSPACE */

#if defined(__CSMC__) && (!defined(_XGATE_))
#define JMP_INSTRUCTION     6u  /* Extended mode JMP instruction */
#elif defined(HOSTED) || defined (_lint) || defined (POLYSPACE)
/* hosted simulation won't be using the jump table so any value is fine */
#define JMP_INSTRUCTION     6u
#else
#error  Jump table not defined for target processor/compiler
#endif

#define JUMP(isr)       {JMP_INSTRUCTION,(isr)}

/*
 * locate the jump table at an address known to the bootloader by putting
 * in the jtable section
 */
#ifndef HOSTED
#pragma section const {jtable}
#endif

/**
 * C structure array to generate a list of "jmp function" assembler instructions
 * The interrupt vector table in the bootblock points to the corresponding
 * entry in this table
 *
 * N.B. Functions added in this table MUST be declared with the INTERRUPT
 * macro (which expands to @ interrupt @ near) and placed in unpaged Flash.
 */
/**
 * @lint Investigate : Symbols not referenced\n
 * lint -esym(528,jtable)
 */
/*lint -esym(528,jtable) */

/**
 * @lint Investigate : Structure member not referenced:\n
 * -esym(754,jtable_s::jmp_instruction)
 */
/*lint -esym(754,jtable_s::jmp_instruction) */

/**
 * @lint Investigate : Structure member not referenced:\n
 * -esym(754,jtable_s::isr)
 */
/*lint -esym(754,jtable_s::isr) */
/** @polyspace:begin<MISRA-C:16.9:Low:Investigate> Could code be changed to fix this? */
PRIVATE const struct jtable_s    {
    uint8_t jmp_instruction; INTERRUPT void (*isr)(void);
} jtable[] =
{
    JUMP(isr_spurious_unhandled),   /* 0xFF10:  Spurious interrupt. */
    JUMP(scheduler_switch_context), /* 0xFF12:  System Call (SYS). */
    JUMP(isr_mpu_err_unhandled),/* 0xFF14:  MPU Access Error. This is
                   non-maskable but we don't enable the MPU so
                   will not be generated */
    JUMP(isr_xgate_err),    /* 0xFF16:  XGATE software error interrupt */
    JUMP(isr_reserved_vector),      /* 0xFF18:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF1A:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF1C:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF1E:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF20:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF22:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF24:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF26:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF28:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF2A:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF2C:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF2E:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF30:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF32:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF34:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF36:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF38:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF3A:  Reserved. */
    JUMP(isr_atd1cmp_unhandled),    /* 0xFF3C:  ATD1 Compare Interrupt. */
    JUMP(isr_atd0cmp_unhandled),    /* 0xFF3E:  ATD0 Compare Interrupt. */
    JUMP(isr_tim_accedge_unhandled),    /* 0xFF40:  TIM Pulse accumulator input
                       edge. */
    JUMP(isr_tim_accover_unhandled),    /* 0xFF42:  TIM Pulse accumulator A
                       overflow. */
    JUMP(isr_tim_over_unhandled),   /* 0xFF44:  TIM timer overflow. */
    JUMP(isr_tim7_unhandled),       /* 0xFF46:  TIM timer channel 7. */
    JUMP(isr_tim6_unhandled),       /* 0xFF48:  TIM timer channel 6. */
    JUMP(isr_tim5_unhandled),       /* 0xFF4A:  TIM timer channel 5. */
    JUMP(isr_tim4_unhandled),       /* 0xFF4C:  TIM timer channel 4. */
    JUMP(isr_tim3_unhandled),       /* 0xFF4E:  TIM timer channel 3. */
    JUMP(isr_tim2_unhandled),       /* 0xFF50:  TIM timer channel 2. */
    JUMP(isr_tim1_unhandled),       /* 0xFF52:  TIM timer channel 1. */
    JUMP(isr_tim0_unhandled),       /* 0xFF54:  TIM timer channel 0. */
    JUMP(isr_xgate_unexpected_interrupt), /* 0xFF56:  SCI7. Hi-jacked to allow
           us to report unexpected XGATE interrupts on the S12.
           Do not change without also changing
           CHANNEL_FOR_UNHANDLED_XGATE_INTERRUPT */
    JUMP(isr_pit7_unhandled),   /* 0xFF58: Periodic interrupt timer channel 7.*/
    JUMP(isr_pit6_unhandled),   /* 0xFF5A: Periodic interrupt timer channel 6.*/
    JUMP(isr_pit5_unhandled),   /* 0xFF5C: Periodic interrupt timer channel 5.*/
    JUMP(isr_pit4_unhandled),   /* 0xFF5E: Periodic interrupt timer channel 4.*/
    JUMP(isr_reserved_vector),      /* 0xFF60:  Reserved. */
    JUMP(isr_reserved_vector),      /* 0xFF62:  Reserved. */
    JUMP(isr_xgate7_unhandled),     /* 0xFF64:  XGATE software trigger 7. */
    JUMP(isr_xgate6_unhandled),     /* 0xFF66:  XGATE software trigger 6. */
    JUMP(isr_xgate5_unhandled),     /* 0xFF68:  XGATE software trigger 5. */
    JUMP(isr_xgate4_unhandled),     /* 0xFF6A:  XGATE software trigger 4. */
    JUMP(isr_xgate3_unhandled),     /* 0xFF6C:  XGATE software trigger 3. */
    JUMP(isr_xgate2_unhandled),     /* 0xFF6E:  XGATE software trigger 2. */
    JUMP(isr_xgate1_unhandled),     /* 0xFF70:  XGATE software trigger 1. */
    JUMP(isr_xgate0_unhandled), /* 0xFF72:  XGATE software trigger 0. */
    JUMP(ts_pit_interrupt),   /* 0xFF74: Periodic interrupt timer channel 3.*/
    JUMP(isr_pit2_unhandled),/* 0xFF76: Periodic interrupt timer channel 2*/
    JUMP(ad_pit_interrupt),/* 0xFF78: Periodic interrupt timer channel 1*/
    JUMP(isr_pit_fgloop),   /* 0xFF7A: Periodic interrupt timer channel 0.*/
    JUMP(isr_high_temp_unhandled),  /* 0xFF7C:  High Temperature Interrupt. */
    JUMP(isr_api_unhandled),    /* 0xFF7E: Autonomous periodical interrupt (API). */
    JUMP(isr_lvi_unhandled),    /* 0xFF80:  Low-voltage interrupt (LVI) */
    JUMP(isr_iic1_bus_unhandled),   /* 0xFF82:  IIC1 bus. */
    JUMP(isr_xgate_internal_error), /* 0xFF84:  SCI5. Hi-jacked to allow
           us to report XGATE internal errors to the S12.
           Do not change without also changing
           CHANNEL_FOR_XGATE_INTERNAL_ERROR */
    JUMP(isr_sci4_unhandled),       /* 0xFF86:  SCI4. */
    JUMP(isr_sci3_unhandled),       /* 0xFF88:  SCI3. */
    JUMP(isr_sci2_unhandled),       /* 0xFF8A:  SCI2. */
    JUMP(isr_pwm_emergency_unhandled),  /* 0xFF8C:  PWM emergency shutdown. */
    JUMP(isr_portp_unhandled),  /* 0xFF8E:  Port P. */
    JUMP(isr_CAN4_TransmitInterrupt_unhandled), /* 0xFF90:  CAN4 transmit. */
    JUMP(isr_CAN4_ReceiveInterrupt_unhandled),  /* 0xFF92:  CAN4 receive. */
    JUMP(isr_CAN4_ErrorInterrupt_unhandled),    /* 0xFF94:  CAN4 errors. */
    JUMP(isr_CAN4_WakeupInterrupt_unhandled),   /* 0xFF96:  CAN4 wakeup. */
    JUMP(CanTxInterrupt_3),         /* 0xFF98:  CAN3 transmit. */
    JUMP(CanRxInterrupt_3),         /* 0xFF9A:  CAN3 receive. */
    JUMP(CanErrorInterrupt_3),      /* 0xFF9C:  CAN3 errors. */
    JUMP(isr_CAN3_WakeupInterrupt_unhandled),   /* 0xFF9E:  CAN3 wakeup. */
    JUMP(isr_internalcan_TransmitInterrupt),    /* 0xFFA0:  CAN2 transmit. */
    JUMP(isr_internalcan_ReceiveInterrupt),     /* 0xFFA2:  CAN2 receive. */
    JUMP(isr_internalcan_ErrorInterrupt),    /* 0xFFA4:  CAN2 errors. */
    JUMP(isr_CAN2_WakeupInterrupt_unhandled),   /* 0xFFA6:  CAN2 wakeup. */
    JUMP(CanTxInterrupt_1),     /* 0xFFA8:  CAN1 transmit. */
    JUMP(CanRxInterrupt_1),     /* 0xFFAA:  CAN1 receive. */
    JUMP(CanErrorInterrupt_1),  /* 0xFFAC:  CAN1 errors. */
    JUMP(isr_CAN1_WakeupInterrupt_unhandled), /* 0xFFAE:  CAN1 wakeup. */
    JUMP(CanTxInterrupt_0),     /* 0xFFB0:  CAN0 transmit. CANbedded */
    JUMP(CanRxInterrupt_0),     /* 0xFFB2:  CAN0 receive. CANbedded */
    JUMP(CanErrorInterrupt_0),  /* 0xFFB4:  CAN0 errors. CANbedded */
    JUMP(isr_CAN0_WakeupInterrupt_unhandled), /* 0xFFB6:  CAN0 wakeup. */
    JUMP(isr_flash_cmd_unhandled),  /* 0xFFB8:  FLASH. */
    JUMP(isr_flash_fault),      /* 0xFFBA:  FLASH Fault Detect. */
    JUMP(isr_spi2_unhandled),       /* 0xFFBC:  SPI2. */
    JUMP(isr_spi1_unhandled),       /* 0xFFBE:  SPI1. */
    JUMP(I2C_Interrupt),                /* 0xFFC0:  IIC0 bus. */
    JUMP(isr_sci6_unhandled),           /* 0xFFC2:  SCI6. */
    JUMP(isr_CRG_Self_Clock_Mode_unhandled), /* 0xFFC4:  CRG self clock mode. */
    JUMP(isr_CRG_PLL_Lock_unhandled),       /* 0xFFC6:  CRG PLL Lock. */
    JUMP(isr_PulseAccumulatorB_Overflow_unhandled),
                /* 0xFFC8:  Pulse accumulator B overflow. */
    JUMP(isr_ECT_Modulus_unhandled), /* 0xFFCA:  Modulus down counter underflow. */
    JUMP(isr_porth_unhandled),  /* 0xFFCC:  Port H. */
    JUMP(isr_portj_unhandled),  /* 0xFFCE:  Port J. */
    JUMP(isr_atd1_unhandled),           /* 0xFFD0:  ATD1. */
    JUMP(isr_atd0_unhandled),           /* 0xFFD2:  ATD0. */
    JUMP(LIN2_Interrupt),           /* 0xFFD4:  SCI1. */
    JUMP(LIN1_Interrupt),             /* 0xFFD6:  SCI0. */
    JUMP(isr_spi0_unhandled),           /* 0xFFD8:  SPI0. */
    JUMP(isr_PulseAccumulatorInputEdge_unhandled), /* 0xFFDA:  Pulse
                          accumulator input edge. */
    JUMP(isr_PulseAccumulatorA_Overflow_unhandled), /* 0xFFDC:  Pulse
                          accumulator A overflow. */
    JUMP(ECT_Overflow_Interrupt),   /* 0xFFDE:  ECT Timer overflow. */
    JUMP(isr_ECT_Channel7_unhandled),    /* 0xFFE0:  ECT Channel 7. */
    JUMP(isr_ECT_Channel6_unhandled),    /* 0xFFE2:  ECT Channel 6. */
    JUMP(isr_ECT_Channel5_unhandled),    /* 0xFFE4:  ECT Channel 5. */
    JUMP(isr_ECT_Channel4_unhandled),    /* 0xFFE6:  ECT Channel 4. */
    JUMP(ECT_Channel3_Interrupt),    /* 0xFFE8:  ECT Channel 3. */
    JUMP(ECT_Channel2_Interrupt),    /* 0xFFEA:  ECT Channel 2. */
    JUMP(s12_ECT_Channel1_Interrupt),    /* 0xFFEC:  ECT Channel 1. */
    JUMP(isr_ECT_Channel0_unhandled),    /* 0xFFEE:  ECT Channel 0. */
    JUMP(isr_RealTimeInterrupt_unhandled),   /* 0xFFF0: Real time interrupt. */
    JUMP(isr_irq_unhandled),        /* 0xFFF2:  IRQ. */
    JUMP(isr_xirq_unhandled),       /* 0xFFF4:  XIRQ. */
    JUMP(isr_software_interrupt),       /* 0xFFF6:  SWI. */
    JUMP(isr_illegal_instruction),/* 0xFFF8:  Unimplemented instruction trap. */

    /*
     * The remaining vectors are Reset vectors. In the normal case, control is
     * eventually transferred from the relevant vector in high memory, but
     * boot-time code gets first dibs and may decide not to pass control to the
     * application.
     */

    JUMP(app_reset),            /* 0xFFFA: COP failure reset. */
    JUMP(app_reset),            /* 0xFFFC: Clock monitor fail reset. */
    JUMP(app_reset),            /* 0xFFFE: Reset - Startup code. */
};
/** @polyspace:end<MISRA-C:16.9:Low:Investigate> Could code be changed to fix this? */

#ifndef HOSTED
#pragma section const {} /* stop locating constant data in jtable section */
#endif
