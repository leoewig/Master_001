/** ###################################################################
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : PE_Types.H
**     Project   : Project
**     Processor : MC9S12XEP100MAL
**     Beantype  : PE_Types
**     Version   : Driver 01.12
**     Compiler  : CodeWarrior HCS12X C Compiler
**     Date/Time : 2015/9/6, 10:13
**     Abstract  :
**         PE_Types.h - contains definitions of basic types,
**         register access macros and hardware specific macros
**         which can be used in user application.
**     Settings  :
**     Contents  :
**         No public methods
**
**     (c) Copyright UNIS, a.s. 1997-2008
**     UNIS, a.s.
**     Jundrovska 33
**     624 00 Brno
**     Czech Republic
**     http      : www.processorexpert.com
**     mail      : info@processorexpert.com
** ###################################################################*/

#ifndef __PE_Types_H
#define __PE_Types_H

#include "basic_types.h"

#ifndef FALSE
  #define  FALSE  0
#endif
#ifndef TRUE
  #define  TRUE   1
#endif

#define ASM(asm_string)                 _asm(asm_string)
#define IMASK_IN_CCR    0x10u   /**< mask for INTERRUPT MASK bit in CCR */

/* Freescale types */
typedef unsigned char       VUINT8;
typedef signed char         VINT8;
typedef unsigned short int  VUINT16;
typedef signed short int    VINT16;
typedef unsigned long int   VUINT32;

/* Aditional standard ANSI C types */
/*#ifndef int8_t
typedef signed char int8_t;
#endif
#ifndef int16_t
typedef signed int   int16_t;
#endif
#ifndef int32_t
typedef signed long int    int32_t;
#endif

#ifndef uint8_t
typedef unsigned char       uint8_t;
#endif
#ifndef uint16_t
typedef unsigned int  uint16_t;
#endif
#ifndef uint32_t
typedef unsigned long int   uint32_t;
#endif*/

/*Types definition*/
//typedef unsigned char bool;
typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long dword;
typedef unsigned long dlong[2];
typedef void (*tIntFunc)(void);
typedef uint8_t TPE_ErrCode;

/**************************************************/
/* PE register access macros                      */
/**************************************************/
#define setRegBit(reg, bit)                                     (reg |= reg##_##bit##_##MASK)
#define clrRegBit(reg, bit)                                     (reg &= ~reg##_##bit##_##MASK)
#define getRegBit(reg, bit)                                     (reg & reg##_##bit##_##MASK)
#define setReg(reg, val)                                        (reg = (word)(val))
#define getReg(reg)                                             (reg)
#define setRegBits(reg, mask)                                   (reg |= (word)(mask))
#define getRegBits(reg, mask)                                   (reg & (word)(mask))
#define clrRegBits(reg, mask)                                   (reg &= (word)(~(mask)))
#define setRegBitGroup(reg, bits, val)                          (reg = (word)((reg & ~reg##_##bits##_##MASK) | ((val) << reg##_##bits##_##BITNUM)))
#define getRegBitGroup(reg, bits)                               ((reg & reg##_##bits##_##MASK) >> reg##_##bits##_##BITNUM)
#define setRegMask(reg, maskAnd, maskOr)                        (reg = (word)((getReg(reg) & ~(maskAnd)) | (maskOr)))
#define setRegBitVal(reg, bit, val)                             ((val) == 0 ? (reg &= ~reg##_##bit##_##MASK) : (reg |= reg##_##bit##_##MASK))
#define changeRegBits(reg, mask)                                (reg ^= (mask))
#define changeRegBit(reg, bit)                                  (reg ^= reg##_##bit##_##MASK)

/******************************************************************/
/* Uniform multiplatform peripheral access macros - 16 bit access */
/******************************************************************/
#define setReg16Bit(RegName, BitName)                           (RegName |= RegName##_##BitName##_##MASK)
#define clrReg16Bit(RegName, BitName)                           (RegName &= ~RegName##_##BitName##_##MASK)
#define invertReg16Bit(RegName, BitName)                        (RegName ^= RegName##_##BitName##_##MASK)
#define testReg16Bit(RegName, BitName)                          (RegName & RegName##_##BitName##_##MASK)

/* Whole peripheral register access macros */
#define setReg16(RegName, val)                                  (RegName = (word)(val))
#define getReg16(RegName)                                       (RegName)

/* Bits peripheral register access macros */
#define testReg16Bits(RegName, GetMask)                         (RegName & (GetMask))
#define clrReg16Bits(RegName, ClrMask)                          (RegName &= ~(word)(ClrMask))
#define setReg16Bits(RegName, SetMask)                          (RegName |= (word)(SetMask))
#define invertReg16Bits(RegName, InvMask)                       (RegName ^= (word)(InvMask))
#define clrSetReg16Bits(RegName, ClrMask, SetMask)              (RegName = (RegName & (~(word)(ClrMask))) | (word)(SetMask))
#define seqClrSetReg16Bits(RegName, BitsMask, BitsVal)          (RegName &= ~(~(word)(BitsVal) & (word)(BitsMask)),\
                                                                RegName |= (word)(BitsVal) & (word)(BitsMask) )
#define seqSetClrReg16Bits(RegName, BitsMask, BitsVal)          (RegName |= (word)(BitsVal) & (word)(BitsMask),\
                                                                RegName &= ~(~(word)(BitsVal) & (word)(BitsMask)) )
#define seqResetSetReg16Bits(RegName, BitsMask, BitsVal)        (RegName &= ~(word)(BitsMask),\
                                                                RegName |= (word)(BitsVal) & (word)(BitsMask) )
#define clrReg16BitsByOne(RegName, ClrMask, BitsMask)           (RegName &= (word)(ClrMask) & (word)(BitsMask))

/* Bit group peripheral register access macros */
#define testReg16BitGroup(RegName, GroupName)                   (RegName & RegName##_##GroupName##_##MASK)
#define getReg16BitGroupVal(RegName, GroupName)                 ((RegName & RegName##_##GroupName##_##MASK) >> RegName##_##GroupName##_##BITNUM)
#define setReg16BitGroupVal(RegName, GroupName, GroupVal)       (RegName = (RegName & ~RegName##_##GroupName##_##MASK) | ((GroupVal) << RegName##_##GroupName##_##BITNUM))
#define seqClrSetReg16BitGroupVal(RegName,GroupName,GroupVal)   (RegName &= ~(~((GroupVal) << RegName##_##GroupName##_##BITNUM) & RegName##_##GroupName##_##MASK),\
                                                                RegName |= ((GroupVal) << RegName##_##GroupName##_##BITNUM) & RegName##_##GroupName##_##MASK )
#define seqSetClrReg16BitGroupVal(RegName,GroupName,GroupVal)   (RegName |= ((GroupVal) << RegName##_##GroupName##_##BITNUM) & RegName##_##GroupName##_##MASK,\
                                                                RegName &= ~(~((GroupVal) << RegName##_##GroupName##_##BITNUM) & RegName##_##GroupName##_##MASK) )
#define seqResetSetReg16BitGroupVal(RegName,GroupName,GroupVal) (RegName &= ~RegName##_##GroupName##_##MASK,\
                                                                RegName |= ((GroupVal) << RegName##_##GroupName##_##BITNUM) & RegName##_##GroupName##_##MASK )

/*****************************************************************/
/* Uniform multiplatform peripheral access macros - 8 bit access */
/*****************************************************************/
#define setReg8Bit(RegName, BitName)                             (RegName |= RegName##_##BitName##_##MASK)
#define clrReg8Bit(RegName, BitName)                             (RegName &= ~RegName##_##BitName##_##MASK)
#define invertReg8Bit(RegName, BitName)                          (RegName ^= RegName##_##BitName##_##MASK)
#define testReg8Bit(RegName, BitName)                            (RegName & RegName##_##BitName##_##MASK)

/* Whole peripheral register access macros */
#define setReg8(RegName, val)                                    (RegName = (byte)(val))
#define getReg8(RegName)                                         (RegName)

/* Bits peripheral register access macros */
#define testReg8Bits(RegName, GetMask)                           (RegName & (GetMask))
#define clrReg8Bits(RegName, ClrMask)                            (RegName &= ~(byte)(ClrMask))
#define setReg8Bits(RegName, SetMask)                            (RegName |= (byte)(SetMask))
#define invertReg8Bits(RegName, InvMask)                         (RegName ^= (byte)(InvMask))
#define clrSetReg8Bits(RegName, ClrMask, SetMask)                (RegName = (RegName & (~(byte)(ClrMask))) | (byte)(SetMask))
#define seqClrSetReg8Bits(RegName, BitsMask, BitsVal)            (RegName &= ~(~(byte)(BitsVal) & (byte)(BitsMask)),\
                                                                 RegName |= (byte)(BitsVal) & (byte)(BitsMask) )
#define seqSetClrReg8Bits(RegName, BitsMask, BitsVal)            (RegName |= (byte)(BitsVal) & (byte)(BitsMask),\
                                                                 RegName &= ~(~(byte)(BitsVal) & (byte)(BitsMask)) )
#define seqResetSetReg8Bits(RegName, BitsMask, BitsVal)          (RegName &= ~(byte)(BitsMask),\
                                                                 RegName |= (byte)(BitsVal) & (byte)(BitsMask) )
#define clrReg8BitsByOne(RegName, ClrMask, BitsMask)             (RegName &= (byte)(ClrMask) & (byte)(BitsMask))

/* Bit group peripheral register access macros */
#define testReg8BitGroup(RegName, GroupName)                     (RegName & RegName##_##GroupName##_##MASK)
#define getReg8BitGroupVal(RegName, GroupName)                   ((RegName & RegName##_##GroupName##_##MASK) >> RegName##_##GroupName##_##BITNUM)
#define setReg8BitGroupVal(RegName, GroupName, GroupVal)         (RegName = (RegName & ~RegName##_##GroupName##_##MASK) | ((GroupVal) << RegName##_##GroupName##_##BITNUM))
#define seqClrSetReg8BitGroupVal(RegName,GroupName,GroupVal)     (RegName &= ~(~((GroupVal) << RegName##_##GroupName##_##BITNUM) & RegName##_##GroupName##_##MASK),\
                                                                 RegName |= ((GroupVal) << RegName##_##GroupName##_##BITNUM) & RegName##_##GroupName##_##MASK )
#define seqSetClrReg8BitGroupVal(RegName,GroupName,GroupVal)     (RegName |= ((GroupVal) << RegName##_##GroupName##_##BITNUM) & RegName##_##GroupName##_##MASK,\
                                                                 RegName &= ~(~((GroupVal) << RegName##_##GroupName##_##BITNUM) & RegName##_##GroupName##_##MASK) )
#define seqResetSetReg8BitGroupVal(RegName,GroupName,GroupVal)   (RegName &= ~RegName##_##GroupName##_##MASK,\
                                                                 RegName |= ((GroupVal) << RegName##_##GroupName##_##BITNUM) & RegName##_##GroupName##_##MASK )

/* register access macros */

#define in16(var,l,h)  (var = ((word)(l)) | (((word)(h)) << 8))
#define out16(l,h,val) (l = (byte)val, h = (byte)(val >> 8))

#define output(P, V) (P = (V))
#define input(P) (P)

#define __DI()  { ASM("sei"); }      /* Disable global interrupts  */
#define __EI()  { ASM("cli"); }      /* Enable global interrupts */
#define EnterCritical()     { ASM("pshc"); ASM("sei"); ASM("movb 1,SP+,CCR"); } /* This macro is used by Processor Expert. It saves CCR register and disable global interrupts. */
#define ExitCritical()  { ASM("movb CCR, 1,-SP"); ASM("pulc"); } /* This macro is used by Processor Expert. It restores CCR register saved in SaveStatusReg(). */
/* obsolete definition for backward compatibility */
#define SaveStatusReg()     EnterCritical()
#define RestoreStatusReg()  ExitCritical()

//#define ENABLE_INTERRUPTS() \
//            do { \
//                  (void) ASM("CLI"); \
//            } while(0)
//#define DISABLE_INTERRUPTS() \
//            do { \
//                 (void) ASM("SEI"); \
//            } while(0)
//#define SAVE_INTERRUPTS(x)  do {                    \
//                    /* save CCR image and then */   \
//                        /* disable interrupts */        \
//                    (x) =(uint8_t) IMASK_IN_CCR;    \
//                    DISABLE_INTERRUPTS();       \
//                } while (0)
//#define RESTORE_INTERRUPTS(x) do {                  \
//                    /* if CCR image shows I bit clear */\
//                    /* re-enable interrupts */      \
//                    if (((x) & IMASK_IN_CCR) == 0u) \
//                    {                   \
//                    ENABLE_INTERRUPTS();        \
//                    }                   \
//                } while (0)
#define ISR(x) INTERRUPT void x(void)


typedef struct {          /* Image */
  word width;             /* Image width in pixels */
  word height;            /* Image height in pixels */
  byte *pixmap;           /* Image pixel bitmap */
  word size;              /* Image size in bytes */
  int8_t *name;           /* Image name */
} TIMAGE;
typedef TIMAGE* PIMAGE ; /* Pointer to image */

/* 16-bit register (big endian) */
typedef union {
   word w;
   struct {
     byte high,low;
   } b;
} TWREG;

#endif /* __PE_Types_H */
/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 3.00 [04.12]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/
