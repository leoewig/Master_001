/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   hardware.h
 *
 * description:  Hardware specific macros
 * 
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
 * @file
 * @ingroup utils
 *
 * Hardware specific macros
 *
 */

#ifndef  HARDWARE_H
#define HARDWARE_H

#include "basic_types.h"


/**
 * Wrapper for `_asm()` inline assembler taking a single string argument.
 *
 * The Cosmic compiler treats the return type of `_asm()` as a special case.
 * If the Cosmic C calling convention is adhered to, i.e. return value
 * is in the `d` register (or `b` register if only a byte), then the return
 * value can be defined with a cast and assigned to regular C variables.
 * For example,
 * ~~~c
 * uint8_t byte_off_stack;
 * uint16_t word_off_stack;
 * ...
 * byte_off_stack = (uint8_t) ASM("pulb");
 * word_off_stack = (uint16_t) ASM("puld");
 * ~~~
 *
 * @lint Special treatment for embedded assembler wrappers #ASM and #ASM2 <br>
 * -emacro(718,ASM,ASM2)    _asm() is undeclared <br>
 * -emacro(746,ASM,ASM2)    call to _asm() made without a protoype <br>
 * -emacro(515,ASM,ASM2)    _asm() called with different argument counts <br>
 */
/*lint -emacro(718,ASM,ASM2) -emacro(746,ASM,ASM2) */
/*lint -emacro(515,ASM,ASM2) */
#ifndef HOSTED
#define ASM(asm_string)                 _asm(asm_string)
#else
/* on PC, make ASM always just return zero */
#define ASM(asm_string)                 0
#endif


/**
 * Wrapper for `_asm()` inline assembler taking both a string and a parameter
 * argument.
 * See #ASM for the the use of casts to specify the return type and for lint
 * treatment.
 * @param asm_string    String containing assembly code
 * @param argument      Argument using the regular C calling convention: a byte
 *                      will be copied to `b` and a word to `d`
 *
 * For example,
 * ~~~c
 * uint8_t byte_for_stack;
 * uint16_t word_for_stack;
 * ...
 * (void) ASM2("pshb",byte_for_stack);
 * (void) ASM2("pshd",word_for_stack);
 * ~~~
 */
#ifndef HOSTED
#define ASM2(asm_string,argument)      _asm(asm_string,argument)
/* on PC, make ASM2 always just return zero */
#else
#define ASM2(asm_string,argument)                 0
#endif

/*
 * includes for the target processor
 */

#include "regs.h"

#ifdef _XGATE_
#include "gate.h"       /* XGATE control register defiitions */
#include "xprocessor.h"     /* Definitions for Cosmic XGATE utilities */
#endif
/*
 * Interrupt macro definitions.  These macros depend on the parser (Cosmic,
 * Developer Studio,lint, polyspace) and the target (XGATE, S12).
 * Doxygen descriptions come after the definitions to make it easier to
 * include definition only when the macro is defined
 */


/**
 * COP Period
 *
 * Set RSBCK=1 : COP and RTI stop in BDM
 * CR[2:0]=3   : Wdog period = 2^18/OSCCLK seconds 
 * For BCM/VTBM hardware, OSCCLK is 8MHz, giving timeout of 0.032s
 */
#define COP_PERIOD_32MS         0x43u

/**
 * XIRQ on PE[0]
 * This is a non-maskable interrupt, enabled by clearing the X-Bit in CCR
 * once the input level has settled. 
 */
#define XIRQ_ENABLED        FALSE
#define XMASK_IN_CCR        0x40u   /**< mask for X Bit in CCR */
#define XIRQ_EN_IN_CCR      (XMASK_IN_CCR & ((uint8_t) ~(uint8_t)((uint8_t)XIRQ_ENABLED << 6u)))


#ifdef HOSTED
/*
 * don't want any of these macros to do anything apart from access the
 * parameter in *SAVE_INTERRUPTS()
 */
/* XGATE save/restore defined for both micros */

#ifndef _XGATE_
/* S12 code running on host */
#define     ABS_LOCATE(name,location) name
#define     ENABLE_INTERRUPTS()       /* nothing */
#define     DISABLE_INTERRUPTS()      /* nothing */
#define     SAVE_INTERRUPTS(x)      (x = 0)
#define     RESTORE_INTERRUPTS(x)       /* nothing */
#define     CORE_SAVE_INTERRUPTS(x)     SAVE_INTERRUPTS(x)
#define     CORE_RESTORE_INTERRUPTS(x)  RESTORE_INTERRUPTS(x)
#else
/* XGATE code running on host */
#define     XGENABLE_INTERRUPTS()       /* nothing */
#define     XGDISABLE_INTERRUPTS()      /* nothing */
#define     XGSAVE_INTERRUPTS(x)        (x = 0)
#define     XGRESTORE_INTERRUPTS(x)     /* nothing */
#define     CORE_SAVE_INTERRUPTS(x)     XGSAVE_INTERRUPTS(x)
#define     CORE_RESTORE_INTERRUPTS(x)  XGRESTORE_INTERRUPTS(x)
#endif

#else   /* not hosted */
#if defined(_lint)
#define ABS_LOCATE(name,location) (name)
#define ENABLE_INTERRUPTS()
#define DISABLE_INTERRUPTS()
#define IMASK_IN_CCR    0x10u   /**< mask for INTERRUPT MASK bit in CCR */
#define SAVE_INTERRUPTS(x)  do {                    \
                    /* save CCR image and then */   \
                        /* disable interrupts */        \
                    (x) =(uint8_t) IMASK_IN_CCR;    \
                    DISABLE_INTERRUPTS();       \
                } while (0)
#define RESTORE_INTERRUPTS(x) do {                  \
                    /* if CCR image shows I bit clear */\
                    /* re-enable interrupts */      \
                    if (((x) & IMASK_IN_CCR) == 0u) \
                    {                   \
                    ENABLE_INTERRUPTS();        \
                    }                   \
                } while (0)
#define CORE_SAVE_INTERRUPTS(x)     SAVE_INTERRUPTS(x)
#define CORE_RESTORE_INTERRUPTS(x)  RESTORE_INTERRUPTS(x)
#elif defined (POLYSPACE)
#define ABS_LOCATE(name,location) (name)
#define ENABLE_INTERRUPTS()
#define DISABLE_INTERRUPTS()
#define SAVE_INTERRUPTS(x)
#define RESTORE_INTERRUPTS(x)
#define CORE_SAVE_INTERRUPTS(x)
#define CORE_RESTORE_INTERRUPTS(x)
#elif !defined (_XGATE_)
/* S12 code */
#define ABS_LOCATE(name,location) name @location
#define ENABLE_INTERRUPTS() \
            do { \
                  (void) ASM("CLI"); \
            } while(0)
#define DISABLE_INTERRUPTS() \
            do { \
                 (void) ASM("SEI"); \
            } while(0)
#define IMASK_IN_CCR    0x10u   /**< mask for INTERRUPT MASK bit in CCR */
#define SAVE_INTERRUPTS(x)  do {                    \
                    /* save CCR image and then */   \
                        /* disable interrupts */        \
                    (x) =(uint8_t) ASM("TFR CCR,B");   \
                    DISABLE_INTERRUPTS();       \
                } while (0)

#define RESTORE_INTERRUPTS(x)   do {                    \
                    /* if CCR image shows I bit clear */\
                    /* re-enable interrupts */      \
                    if (((x) & IMASK_IN_CCR) == 0u) \
                    {                   \
                    ENABLE_INTERRUPTS();        \
                    }                   \
                } while (0)
#define CORE_SAVE_INTERRUPTS(x)     SAVE_INTERRUPTS(x)
#define CORE_RESTORE_INTERRUPTS(x)  RESTORE_INTERRUPTS(x)
#else
/* XGATE code */
#define ABS_LOCATE(name,location) name @location
#define XGENABLE_INTERRUPTS()   (XGMCTL = (XGEM | XGE))

#define XGDISABLE_INTERRUPTS()  (XGMCTL = XGEM)

#define XGSAVE_INTERRUPTS(x)    do {                    \
                    /* save XGMCTL image and then */    \
                        /* disable interrupts */        \
                    (x) = XGMCTL;           \
                    XGDISABLE_INTERRUPTS();     \
                } while (0)

#define XGRESTORE_INTERRUPTS(x)  do {                   \
                    /* if XGMCTL image shows XGE bit set */\
                    /* re-enable interrupts */      \
                    if ((x) & XGE)      \
                    {                   \
                    XGENABLE_INTERRUPTS();      \
                    }                   \
                } while (0)

#define CORE_SAVE_INTERRUPTS(x)     XGSAVE_INTERRUPTS(x)
#define CORE_RESTORE_INTERRUPTS(x)  XGRESTORE_INTERRUPTS(x)
#endif
#endif


/*
 * def NAME used for Doxygen macro comments because each macro is created
 * differently according to the parser
 */
#ifndef _XGATE_
/**
 * @def ENABLE_INTERRUPTS
 * enable interrupts on the S12 core <b>(not defined for XGATE code)</b>
 */
/**
 * @def DISABLE_INTERRUPTS
 * disable interrupts on the S12 core <b>(not defined for XGATE code)</b>
 */
/**
 * @def SAVE_INTERRUPTS
 * Save current state of S12 interrupts then disable them <b>(not defined for
 * XGATE code)</b>
 *
 * @param[out]  x   Variable in which CCR (which includes the I bit)
 *          is saved.  Can be uint8_t or bigger
 */

/**
 * @def RESTORE_INTERRUPTS
 * Restore interrupts to state previously saved by #SAVE_INTERRUPTS
 *              <b>(not defined for XGATE code)</b>
 * @param[in]   x   Variable set by previous call to #SAVE_INTERRUPTS
 */
#else
/**
 * @def XGENABLE_INTERRUPTS
 * Enable XGATE interrupts by setting XGE and the XGE write-enable bit XGEM
 *              <b>(not defined for S12 code)</b>
 */
/**
 * @def XGDISABLE_INTERRUPTS
 * Disable XGATE interrupts by clearing XGE while setting the XGE write-enable
 * bit XGEM
 *              <b>(not defined for S12 code)</b>
 */
/**
 * @def XGSAVE_INTERRUPTS
 * Save current state of XGATE interrupts and then disable them
 *              <b>(not defined for S12 code)</b>
 * @param[out]  x   Variable in which XGMCTL (which includes the XGE bit)
 *          is saved.  Can be uint16_t or bigger
 */
/**
 * @def XGRESTORE_INTERRUPTS
 * Restore interrupts to state previously saved by #XGSAVE_INTERRUPTS
 *              <b>(not defined for S12 code)</b>
 * @param[in]   x   State of XGMCTL saved by previous call to
 *          #XGSAVE_INTERRUPTS
 */
#endif /* ifndef _XGATE_ */
/**
 * @def CORE_SAVE_INTERRUPTS
 * Save current state of interrupts on the core on which code is executing.
 * Will expand to either SAVE_INTERRUPTS or XGSAVE_INTERRUPTS
 * @param[out]  x   Variable in which state of interrupt is saved.
 *          is saved.  Can be uint16_t or bigger.
 */
/**
 * @def CORE_RESTORE_INTERRUPTS
 * Restore interrupts to state previously saved by #CORE_SAVE_INTERRUPTS
 * Will expand to either RESTORE_INTERRUPTS or XGRESTORE_INTERRUPTS
 * @param[in]   x   State of interrupts saved by previous call to
 *          #CORE_SAVE_INTERRUPTS
 */

/*
 * debugging support
 */
#ifndef _XGATE_
/**
 * @def BREAK_IF_DEBUGGING
 * On S12, will stop the program in the debugger if it's attached but do
 * nothing otherwise.  Will do the same in Developer's Studio. <b>Not available
 * in XGATE code</b>
 */
#endif

/**
 * @def TEST_HOOK(N)
 * Provide a call out to test code when running on the host.  Ignored on the
 * target.  Useful to test code which loops waiting for h/w registers to change.
 * @param   N   A unique number for the current function to allow
 *          multiple calls to be disambiguated
 */
#ifdef HOSTED
#define BREAK_IF_DEBUGGING  _asm { int 3 } /* magical old-timers' DOS
                          interrupt that still works.  Note that we must call
                         _asm() and not the wrapper ASM(), because that gets
                         nobbled in HOSTED builds */
extern void test_hook(char *filename,char *function_name,uint16_t n);

#define TEST_HOOK(N)    test_hook(__FILE__,__FUNCTION__,N)
#elif defined(_lint) || defined(POLYSPACE)
#define BREAK_IF_DEBUGGING      /* do nothing */
#define TEST_HOOK(N)            /* do nothing */
#elif !defined(_XGATE_)
#define BREAK_IF_DEBUGGING      (void) ASM("BGND")
#define TEST_HOOK(N)            /* do nothing */
#else
/* XGATE code.  Can't enter the debugger so warn the developer by making
 * the compiler barf
 */
#define BREAK_IF_DEBUGGING  BREAK-IF-DEBUGGING not available on XGATE
#define TEST_HOOK(N)            /* do nothing */
#endif

/******************************************************************************
 *  Definition of INTERNAL_ERROR
 ******************************************************************************/

#if defined(POLYSPACE)
#define INTERNAL_ERROR()       (5/0)   /* Cause PolySpace to show red if this is called */
#define RESET_S12()
#elif defined(_lint)
/* For Lint, INTERNAL error should not have an infinite loop in order to avoid
 * complaints about unreachable code. */
#define INTERNAL_ERROR()
#define RESET_S12()
#else /* defined POLYSPACE */

/* Not Polyspace and not Lint */
/**
 * @todo  Add DTC setting to INTERNAL_ERROR
 */

#ifndef _XGATE_

#if defined MMC || defined VTBM
/* log MMC or VTBM faults to Emulated EEPROM */
void flash_eee_log_internal_error(FPTR const char_t *location);

/*
 * Usual stringification jiggery-pokery to get __LINE__ expanded before it
 * is stringified
 */
#define LOG_INTERNAL_ERROR(file,line)   LOG_INTERNAL_ERROR2(file,line)
#define LOG_INTERNAL_ERROR2(file,line)  \
                                        flash_eee_log_internal_error(file #line)
#else
#define LOG_INTERNAL_ERROR(file,line)   /* nothing */
#endif

/**
 * Macro to indicate an _impossible_ situation which implies a flaw in
 * our logic.
 * First of all, log the location of the fault to emulated EEPROM and then
 * stop in the debugger if it's attached or reset the micro.
 */
#define INTERNAL_ERROR()          do{ \
                                    LOG_INTERNAL_ERROR(__FILE__,__LINE__); \
                                    BREAK_IF_DEBUGGING; \
                                    RESET_S12();\
                                  } while(0)
/**
 * Reset the S12 micro by writing an illegal value to the COP (which induces 
 * an immediate reset).
 * We try to ensure the COP is enabled by writing COPCTL.
 * If COPCTL has been previously written, this will have no effect.  If
 * the previous write left COPCTL disarmed (CR[2:0]=0), the infinite
 * loop means the external watchdog will get us eventually.  Note that
 * interrupts are disabled in case we call it from an idle task (below the
 * priority where the external dog is kicked).
 */

#define RESET_S12()             do{ \
                                    DISABLE_INTERRUPTS(); \
                                    COPCTL = COP_PERIOD_32MS; \
                                    ARMCOP = 0u; \
                                } while(1)

#else /* ndef _XGATE_ */

/*
 * on the XGATE save the program counter and trigger an interrupt on the S12.
 * Oddly, TFR Rd,PC puts PC+4 into Rd so we might as well subtract the 4 to get
 * the actual program counter.
 */
#define INTERNAL_ERROR()          do { \
                                xg_internal_error_pc = (uint16_t) ASM("TFR R2,PC") - 4u;\
                                _sif(CHANNEL_FOR_XGATE_INTERNAL_ERROR);\
                            } while (0)
#endif /* ndef _XGATE_ */

#endif /* defined POLYSPACE */


#ifdef HOSTED
#else
#define TEST_HOOK(N)    /* do nothing */
#endif /* HOSTED */

#if !defined PROD_APP_TEST 
#define ASSERT(expr)            do { \
                                    if (!(expr)) \
                                    { \
                                        INTERNAL_ERROR(); \
                                    } \
                                } while (0)
#else
#define ASSERT(expr)            do { \
                                } while (0)
#endif

#if ! defined POLYSPACE
/**
 * Macro to grab the source address when an unexpected interrupt occurs.
 * Useful for s/w triggered interrupts such as illegal instruction or SWI
 *
 * @param[out] p current value of PPAGE
 * @param[out] a Return address on stack
 *
 * Stack On Entry:
 *  Address | Data
 *  --------|-----------
 *  SP + 0  | CCR (High)
 *  SP + 1  | CCR (Low)
 *  SP + 2  | B
 *  SP + 3  | A
 *  SP + 4  | X (High)
 *  SP + 5  | X (Low)
 *  SP + 6  | Y (High)
 *  SP + 7  | Y (Low)
 *  SP + 8  | Return Address (High)
 *  SP + 9  | Return Address (Low)
 *
 *  @note Macro will not work if the invoking function has its own automatic
 *  data so parameters passed to this macro _must_ be static.
 */
#define CAPTURE_ADDRESS(p,a) \
       do { \
              /* Capture return address for debug purposes */ \
        (void) ASM ("xref _PPAGE"); \
        (p) = (uint8_t) ASM("ldab _PPAGE"); \
        (a) = (uint16_t) ASM("ldd  8,sp") ; \
       } while(0)
#else /* ! defined POLYSPACE */
#define CAPTURE_ADDRESS(p,a) /* nothing */
#endif /* ! defined POLYSPACE */


#endif  /* HARDWARE_H */
