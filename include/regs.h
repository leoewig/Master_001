/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    regs.h
 *
 * description:  S12 Register Interface Header File
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
#if !defined (REGS_H)
#define REGS_H
/**
 * @file
 * @ingroup regs
 *
 * S12 Register Interface Header File
 *
 * The file provided by Cosmic to interface to the registers, actually defines
 * a new variable for each register, instead of providing an external variable
 * for each register. This causes Lint to report numerous (and unnecessary)
 * "variable already defined" errors.
 *
 * This file (and an equivalent C-file) is used in place of ioxep100.h. The
 * associated C-file, includes this file and the original ioxep100.h, so that
 * only one copy of the registers is defined.
 */
/**
 * @defgroup regs Processor registers
 * @ingroup hardware
 */
/* Copyright (c) Linkedriving Tech 2012 */

/*	PIM Module
 */
extern volatile uint8_t  PORTA;	/**< port A */
extern volatile uint8_t  PORTB;	/**< port B */
extern volatile uint8_t  DDRA;	/**< data direction port A */
extern volatile uint8_t  DDRB;	/**< data direction port B */
extern volatile uint8_t  PORTC;	/**< port C */
extern volatile uint8_t  PORTD;	/**< port C */
extern volatile uint8_t  DDRC;	/**< data direction port A */
extern volatile uint8_t  DDRD;	/**< data direction port B */
extern volatile uint8_t  PORTE;	/**< port E */
extern volatile uint8_t  DDRE;	/**< data direction port E */
extern volatile uint8_t  PUCR;	/**< pull-up control register */
extern volatile uint8_t  RDRIV;	/**< reduced drive of I/O lines */
extern volatile uint8_t  EBICTL0;	/**< external bus interface control 0 */
extern volatile uint8_t  EBICTL1;	/**< external bus interface control 1 */

/*	MMC Module
 */
extern volatile uint8_t  MMCCTL0;	/**< mapping control register 0 */
extern volatile uint8_t  MODE;	/**< mode register */
extern volatile uint8_t  GPAGE;	/**< global page register */
extern volatile uint8_t  DIRECT;	/**< direct page register */
extern volatile uint8_t  MMCCTL1;	/**< mapping control register 1 */
extern volatile uint8_t  PPAGE;	/**< program page register */
extern volatile uint8_t  RPAGE;	/**< ram page register */
extern volatile uint8_t  EPAGE;	/**< eeprom page register */
extern volatile uint16_t PARTID;	/**< part ID register */
extern volatile uint8_t  PARTIDH;	/**< part ID register high */
extern volatile uint8_t  PARTIDL;	/**< part ID register low */

/*	IRQ Module
 */
extern volatile uint8_t  ECLKCTL;	/**< eclk control register */
extern volatile uint8_t  IRQCR;	/**< interrupt control register */

/*	DBG Module
 */
extern volatile uint8_t  DBGC1;	/**< debug control register 1 */
extern volatile uint8_t  DBGSR;	/**< debug status register */
extern volatile uint8_t  DBGTCR;	/**< debug trace control register */
extern volatile uint8_t  DBGC2;	/**< debug control register 2 */
extern volatile uint16_t DBGTB;	/**< debug trace buffer register */
extern volatile uint8_t  DBGTBH;	/**< debug trace buffer register high */
extern volatile uint8_t  DBGTBL;	/**< debug trace buffer register low */
extern volatile uint8_t  DBGCNT;	/**< debug count register */
extern volatile uint8_t  DBGSCRX;	/**< debug state control register */
extern volatile uint8_t  DBGXCTL;	/**< debug comparator control register */
extern volatile uint8_t  DBGXAH;	/**< debug comparator address high */
extern volatile uint8_t  DBGXAM;	/**< debug comparator address medium */
extern volatile uint8_t  DBGXAL;	/**< debug comparator address low */
extern volatile uint16_t DBGXD;	/**< debug comparator data */
extern volatile uint16_t DBGXDM;	/**< debug comparator data mask */

/*	PIM Module
 */
extern volatile uint8_t  PORTK;	/**< port K data register */
extern volatile uint8_t  DDRK;	/**< port K data direction */

/*	CRG Module
 */
extern volatile uint8_t  SYNR;	/**< synthesizer register */
extern volatile uint8_t  REFDV;	/**< reference divider register */
extern volatile uint8_t  POSTDIV;	/**< post divider register */
extern volatile uint8_t  CRGFLG;	/**< clock generator flag register */
extern volatile uint8_t  CRGINT;	/**< clock interrupt enable */
extern volatile uint8_t  CLKSEL;	/**< clock select register */
extern volatile uint8_t  PLLCTL;	/**< PLL control register */
extern volatile uint8_t  RTICTL;	/**< clock real time control reg. */
extern volatile uint8_t  COPCTL;	/**< COP control register */
extern volatile uint8_t  FORBYP;	/**< clock force and bypass reg. */
extern volatile uint8_t  CTCTL;	/**< clock test control register */
extern volatile uint8_t  ARMCOP;	/**< COP arm/reset register */

/*	Enhanced Capture Timer Module
 */
extern volatile uint8_t  TIOS;	/**< timer select register */
extern volatile uint8_t  CFORC;	/**< compare force register */
extern volatile uint8_t  OC7M;	/**< oc7 mask register */
extern volatile uint8_t  OC7D;	/**< oc7 data register */
extern volatile uint16_t TCNT;	/**< timer counter */
extern volatile uint8_t  TSCR1;	/**< system control register 1 */
extern volatile uint8_t  TTOV;	/**< toggle on overflow register */
extern volatile uint8_t  TCTL1;	/**< control register 1 */
extern volatile uint8_t  TCTL2;	/**< control register 2 */
extern volatile uint8_t  TCTL3;	/**< control register 3 */
extern volatile uint8_t  TCTL4;	/**< control register 4 */
extern volatile uint8_t  TIE;	/**< interrupt enable register */
extern volatile uint8_t  TSCR2;	/**< system control register 2 */
extern volatile uint8_t  TFLG1;	/**< interrupt flag register 1 */
extern volatile uint8_t  TFLG2;	/**< interrupt flag register 2 */
extern volatile uint16_t TC0;	/**< capture/compare register 0 */
extern volatile uint16_t TC1;	/**< capture/compare register 0 */
extern volatile uint16_t TC2;	/**< capture/compare register 0 */
extern volatile uint16_t TC3;	/**< capture/compare register 0 */
extern volatile uint16_t TC4;	/**< capture/compare register 0 */
extern volatile uint16_t TC5;	/**< capture/compare register 0 */
extern volatile uint16_t TC6;	/**< capture/compare register 0 */
extern volatile uint16_t TC7;	/**< capture/compare register 0 */
extern volatile uint8_t  PACTL;	/**< pulse accumulator A control */
extern volatile uint8_t  PAFLG;	/**< pulse accumulator A flag */
extern volatile uint8_t  PACN3;	/**< pulse accumulator A3 count */
extern volatile uint8_t  PACN2;	/**< pulse accumulator A2 count */
extern volatile uint8_t  PACN1;	/**< pulse accumulator A1 count */
extern volatile uint8_t  PACN0;	/**< pulse accumulator A0 count */
extern volatile uint8_t  MCCTL;	/**< modulus counter control reg */
extern volatile uint8_t  MCFLG;	/**< modulus counter flag reg */
extern volatile uint8_t  ICPAR;	/**< input control pulse acc reg */
extern volatile uint8_t  DLYCT;	/**< delay counter control reg */
extern volatile uint8_t  ICOVW;	/**< input control overwrite reg */
extern volatile uint8_t  ICSYS;	/**< input control system reg */
extern volatile uint8_t  OCPD;	/**< output compare pin disconnect */
extern volatile uint8_t  TIMTST;	/**< timer test register */
extern volatile uint8_t  PTPSR;	/**< precision prescaler select reg */
extern volatile uint8_t  PTMCPSR;	/**< precision modulus prescale reg */
extern volatile uint8_t  PBCTL;	/**< pulse accumulator B control */
extern volatile uint8_t  PBFLG;	/**< pulse accumulator B flag */
extern volatile uint8_t  PA3H;	/**< pulse accumulator B3 count */
extern volatile uint8_t  PA2H;	/**< pulse accumulator B2 count */
extern volatile uint8_t  PA1H;	/**< pulse accumulator B1 count */
extern volatile uint8_t  PA0H;	/**< pulse accumulator B0 count */
extern volatile uint16_t MCCNT;	/**< modulus counter count reg */
extern volatile uint16_t TC0H;	/**< timer input capture hold 0 */
extern volatile uint16_t TC1H;	/**< timer input capture hold 1 */
extern volatile uint16_t TC2H;	/**< timer input capture hold 2 */
extern volatile uint16_t TC3H;	/**< timer input capture hold 3 */

/*	ATD1 Module
 */
extern volatile uint8_t  ATD1CTL0;	/**< ATD1 control register 0 */
extern volatile uint8_t  ATD1CTL1;	/**< ATD1 control register 1 */
extern volatile uint8_t  ATD1CTL2;	/**< ATD1 control register 2 */
extern volatile uint8_t  ATD1CTL3;	/**< ATD1 control register 3 */
extern volatile uint8_t  ATD1CTL4;	/**< ATD1 control register 4 */
extern volatile uint8_t  ATD1CTL5;	/**< ATD1 control register 5 */
extern volatile uint8_t  ATD1STAT0;	/**< ATD1 status register 0 */
extern volatile uint16_t ATD1CMPE;	/**< ATD1 compare enable */
extern volatile uint16_t ATD1STAT2;	/**< ATD1 status register 2 */
extern volatile uint16_t ATD1DIEN;	/**< ATD1 interrupt enable */
extern volatile uint16_t ATD1CMPHT;	/**< ATD1 compare higher than */
extern volatile uint16_t ATD1DR[16];	/**< ATD1 result */
extern volatile uint16_t ATD1DR0;	/**< ATD1 result 0 */
extern volatile uint16_t ATD1DR1;	/**< ATD1 result 1 */
extern volatile uint16_t ATD1DR2;	/**< ATD1 result 2 */
extern volatile uint16_t ATD1DR3;	/**< ATD1 result 3 */
extern volatile uint16_t ATD1DR4;	/**< ATD1 result 4 */
extern volatile uint16_t ATD1DR5;	/**< ATD1 result 5 */
extern volatile uint16_t ATD1DR6;	/**< ATD1 result 6 */
extern volatile uint16_t ATD1DR7;	/**< ATD1 result 7 */
extern volatile uint16_t ATD1DR8;	/**< ATD1 result 8 */
extern volatile uint16_t ATD1DR9;	/**< ATD1 result 9 */
extern volatile uint16_t ATD1DR10;	/**< ATD1 result 10 */
extern volatile uint16_t ATD1DR11;	/**< ATD1 result 11 */
extern volatile uint16_t ATD1DR12;	/**< ATD1 result 12 */
extern volatile uint16_t ATD1DR13;	/**< ATD1 result 13 */
extern volatile uint16_t ATD1DR14;	/**< ATD1 result 14 */
extern volatile uint16_t ATD1DR15;	/**< ATD1 result 15 */

/*	I2C1 Module
 */
extern volatile uint8_t  IB1AD;	/**< I2C 1 address register */
extern volatile uint8_t  IB1FD;	/**< I2C 1 freqency divider reg */
extern volatile uint8_t  IB1CR;	/**< I2C 1 control register */
extern volatile uint8_t  IB1SR;	/**< I2C 1 status register */
extern volatile uint8_t  IB1DR;	/**< I2C 1 data register */
extern volatile uint8_t  IB1CR2;	/**< I2C 1 control register 2 */

/*	SCI2 Module
 *
 * The following registers are accessible if the AMAP bit in the SCI2SR2
 * register is set to zero
 */
extern volatile uint16_t SCI2BD;	/**< SCI 2 baud rate */
extern volatile uint8_t  SCI2BDH;	/**< SCI 2 baud rate high */
extern volatile uint8_t  SCI2BDL;	/**< SCI 2 baud rate low */
extern volatile uint8_t  SCI2CR1;	/**< SCI 2 control register 1 */
/*
 * The following registers are accessible if the AMAP bit in the SCI2SR2
 * register is set to one
 */
extern volatile uint8_t  SCI2ASR1;	/**< SCI 2 alt status reg 1 */
extern volatile uint8_t  SCI2ACR1;	/**< SCI 2 alt control reg 1 */
extern volatile uint8_t  SCI2ACR2;	/**< SCI 2 alt control reg 2 */
/*
 * The following registers are always accessible
 */
extern volatile uint8_t  SCI2CR2;	/**< SCI 2 control register 2 */
extern volatile uint8_t  SCI2SR1;	/**< SCI 2 status register 1 */
extern volatile uint8_t  SCI2SR2;	/**< SCI 2 status register 2 */
extern volatile uint16_t SCI2DR;	/**< SCI 2 data register */
extern volatile uint8_t  SCI2DRH;	/**< SCI 2 data register high */
extern volatile uint8_t  SCI2DRL;	/**< SCI 2 data register low */

/*	SCI3 Module
 *
 * The following registers are accessible if the AMAP bit in the SCI3SR2
 * register is set to zero
 */
extern volatile uint16_t SCI3BD;	/**< SCI 3 baud rate */
extern volatile uint8_t  SCI3BDH;	/**< SCI 3 baud rate high */
extern volatile uint8_t  SCI3BDL;	/**< SCI 3 baud rate low */
extern volatile uint8_t  SCI3CR1;	/**< SCI 3 control register 1 */
/*
 * The following registers are accessible if the AMAP bit in the SCI3SR2
 * register is set to one
 */
extern volatile uint8_t  SCI3ASR1;	/**< SCI 3 alt status reg 1 */
extern volatile uint8_t  SCI3ACR1;	/**< SCI 3 alt control reg 1 */
extern volatile uint8_t  SCI3ACR2;	/**< SCI 3 alt control reg 2 */
/*
 * The following registers are always accessible
 */
extern volatile uint8_t  SCI3CR2;	/**< SCI 3 control register 2 */
extern volatile uint8_t  SCI3SR1;	/**< SCI 3 status register 1 */
extern volatile uint8_t  SCI3SR2;	/**< SCI 3 status register 2 */
extern volatile uint16_t SCI3DR;	/**< SCI 3 data register */
extern volatile uint8_t  SCI3DRH;	/**< SCI 3 data register high */
extern volatile uint8_t  SCI3DRL;	/**< SCI 3 data register low */

/*	SCI0 Module
 *
 * The following registers are accessible if the AMAP bit in the SCI0SR2
 * register is set to zero
 */
extern volatile uint16_t SCI0BD;	/**< SCI 0 baud rate */
extern volatile uint8_t  SCI0BDH;	/**< SCI 0 baud rate high */
extern volatile uint8_t  SCI0BDL;	/**< SCI 0 baud rate low */
extern volatile uint8_t  SCI0CR1;	/**< SCI 0 control register 1 */
/*
 * The following registers are accessible if the AMAP bit in the SCI0SR2
 * register is set to one
 */
extern volatile uint8_t  SCI0ASR1;	/**< SCI 0 alt status reg 1 */
extern volatile uint8_t  SCI0ACR1;	/**< SCI 0 alt control reg 1 */
extern volatile uint8_t  SCI0ACR2;	/**< SCI 0 alt control reg 2 */
/*
 * The following registers are always accessible
 */
extern volatile uint8_t  SCI0CR2;	/**< SCI 0 control register 2 */
extern volatile uint8_t  SCI0SR1;	/**< SCI 0 status register 1 */
extern volatile uint8_t  SCI0SR2;	/**< SCI 0 status register 2 */
extern volatile uint16_t SCI0DR;	/**< SCI 0 data register */
extern volatile uint8_t  SCI0DRH;	/**< SCI 0 data register high */
extern volatile uint8_t  SCI0DRL;	/**< SCI 0 data register low */

/*	SCI1 Module
 *
 * The following registers are accessible if the AMAP bit in the SCI1SR2
 * register is set to zero
 */
extern volatile uint16_t SCI1BD;	/**< SCI 1 baud rate */
extern volatile uint8_t  SCI1BDH;	/**< SCI 1 baud rate high */
extern volatile uint8_t  SCI1BDL;	/**< SCI 1 baud rate low */
extern volatile uint8_t  SCI1CR1;	/**< SCI 1 control register 1 */
/*
 * The following registers are accessible if the AMAP bit in the SCI1SR2
 * register is set to one
 */
extern volatile uint8_t  SCI1ASR1;	/**< SCI 1 alt status reg 1 */
extern volatile uint8_t  SCI1ACR1;	/**< SCI 1 alt control reg 1 */
extern volatile uint8_t  SCI1ACR2;	/**< SCI 1 alt control reg 2 */
/*
 * The following registers are always accessible
 */
extern volatile uint8_t  SCI1CR2;	/**< SCI 1 control register 2 */
extern volatile uint8_t  SCI1SR1;	/**< SCI 1 status register 1 */
extern volatile uint8_t  SCI1SR2;	/**< SCI 1 status register 2 */
extern volatile uint16_t SCI1DR;	/**< SCI 1 data register */
extern volatile uint8_t  SCI1DRH;	/**< SCI 1 data register high */
extern volatile uint8_t  SCI1DRL;	/**< SCI 1 data register low */

/*	SPI0 Module
 */
extern volatile uint8_t  SPI0CR1;	/**< SPI 0 control register 1 */
extern volatile uint8_t  SPI0CR2;	/**< SPI 0 control register 2 */
extern volatile uint8_t  SPI0BR;	/**< SPI 0 baud rate register */
extern volatile uint8_t  SPI0SR;	/**< SPI 0 status register */
extern volatile uint16_t SPI0DR;	/**< SPI 0 data register */
extern volatile uint8_t  SPI0DRH;	/**< SPI 0 data register high */
extern volatile uint8_t  SPI0DRL;	/**< SPI 0 data register low */

/*	I2C0 Module
 */
extern volatile uint8_t  IB0AD;	/**< I2C 0 address register */
extern volatile uint8_t  IB0FD;	/**< I2C 0 freqency divider reg */
extern volatile uint8_t  IB0CR;	/**< I2C 0 control register */
extern volatile uint8_t  IB0SR;	/**< I2C 0 status register */
extern volatile uint8_t  IB0DR;	/**< I2C 0 data register */
extern volatile uint8_t  IB0CR2;	/**< I2C 0 control register 2 */

/*	SPI1 Module
 */
extern volatile uint8_t  SPI1CR1;	/**< SPI 1 control register 1 */
extern volatile uint8_t  SPI1CR2;	/**< SPI 1 control register 2 */
extern volatile uint8_t  SPI1BR;	/**< SPI 1 baud rate register */
extern volatile uint8_t  SPI1SR;	/**< SPI 1 status register */
extern volatile uint16_t SPI1DR;	/**< SPI 1 data register */
extern volatile uint8_t  SPI1DRH;	/**< SPI 1 data register high */
extern volatile uint8_t  SPI1DRL;	/**< SPI 1 data register low */

/*	SPI2 Module
 */
extern volatile uint8_t  SPI2CR1;	/**< SPI 2 control register 1 */
extern volatile uint8_t  SPI2CR2;	/**< SPI 2 control register 2 */
extern volatile uint8_t  SPI2BR;	/**< SPI 2 baud rate register */
extern volatile uint8_t  SPI2SR;	/**< SPI 2 status register */
extern volatile uint16_t SPI2DR;	/**< SPI 2 data register */
extern volatile uint8_t  SPI2DRH;	/**< SPI 2 data register high */
extern volatile uint8_t  SPI2DRL;	/**< SPI 2 data register low */

/*	NVM Control Module
 */
extern volatile uint8_t  FCLKDIV;	/**< flash clock divider */
extern volatile uint8_t  FSEC;	/**< flash security register */
extern volatile uint8_t  FCCOBIX;	/**< flash operation index register */
extern volatile uint8_t  FECCRIX;	/**< flash ecc index register */
extern volatile uint8_t  FCNFG;	/**< flash configuration register */
extern volatile uint8_t  FERCNFG;	/**< flash error config register */
extern volatile uint8_t  FSTAT;	/**< flash status register */
extern volatile uint8_t  FERSTAT;	/**< flash error status register */
extern volatile uint8_t  FPROT;	/**< flash protection register */
extern volatile uint8_t  EPROT;	/**< eeprom protection register */
extern volatile uint16_t FCCOB;	/**< flash command object */
extern volatile uint16_t ETAG;	/**< eeprom tag counter register */
extern volatile uint16_t FEECR;	/**< flash address register */
extern volatile uint8_t  FOPT;	/**< flash option register */

/*	MPU Module
 */
extern volatile uint8_t  MPUFLG;	/**< MPU flag register */
extern volatile uint8_t  MPUASTAT0;	/**< MPU address status register 0 */
extern volatile uint8_t  MPUASTAT1;	/**< MPU address status register 1 */
extern volatile uint8_t  MPUASTAT2;	/**< MPU address status register 2 */
extern volatile uint8_t  MPUSEL;	/**< MPU descriptor select register */
extern volatile uint8_t  MPUDESC0;	/**< MPU descriptor register 0 */
extern volatile uint8_t  MPUDESC1;	/**< MPU descriptor register 1 */
extern volatile uint8_t  MPUDESC2;	/**< MPU descriptor register 2 */
extern volatile uint8_t  MPUDESC3;	/**< MPU descriptor register 3 */
extern volatile uint8_t  MPUDESC4;	/**< MPU descriptor register 4 */
extern volatile uint8_t  MPUDESC5;	/**< MPU descriptor register 5 */

/*	INT module
 */
extern volatile uint8_t  IVBR;	/**< vector base register */
extern volatile uint8_t  INT_XGPRIO;	/**< interrupt priority config. */
extern volatile uint8_t  INT_CFADDR;	/**< interrupt priority config. */
extern volatile uint8_t  INT_CFDATA[8];	/**< interrupt request config. */

/*	SCI4 Module
 *
 * The following registers are accessible if the AMAP bit in the SCI4SR2
 * register is set to zero
 */
extern volatile uint16_t SCI4BD;	/**< SCI 4 baud rate */
extern volatile uint8_t  SCI4BDH;	/**< SCI 4 baud rate high */
extern volatile uint8_t  SCI4BDL;	/**< SCI 4 baud rate low */
extern volatile uint8_t  SCI4CR1;	/**< SCI 4 control register 1 */
/*
 * The following registers are accessible if the AMAP bit in the SCI4SR2
 * register is set to one
 */
extern volatile uint8_t  SCI4ASR1;	/**< SCI 4 alt status reg 1 */
extern volatile uint8_t  SCI4ACR1;	/**< SCI 4 alt control reg 1 */
extern volatile uint8_t  SCI4ACR2;	/**< SCI 4 alt control reg 2 */
/*
 * The following registers are always accessible
 */
extern volatile uint8_t  SCI4CR2;	/**< SCI 4 control register 2 */
extern volatile uint8_t  SCI4SR1;	/**< SCI 4 status register 1 */
extern volatile uint8_t  SCI4SR2;	/**< SCI 4 status register 2 */
extern volatile uint16_t SCI4DR;	/**< SCI 4 data register */
extern volatile uint8_t  SCI4DRH;	/**< SCI 4 data register high */
extern volatile uint8_t  SCI4DRL;	/**< SCI 4 data register low */

/*	SCI5 Module
 *
 * The following registers are accessible if the AMAP bit in the SCI5SR2
 * register is set to zero
 */
extern volatile uint16_t SCI5BD;	/**< SCI 5 baud rate */
extern volatile uint8_t  SCI5BDH;	/**< SCI 5 baud rate high */
extern volatile uint8_t  SCI5BDL;	/**< SCI 5 baud rate low */
extern volatile uint8_t  SCI5CR1;	/**< SCI 5 control register 1 */
/*
 * The following registers are accessible if the AMAP bit in the SCI5SR2
 * register is set to one
 */
extern volatile uint8_t  SCI5ASR1;	/**< SCI 5 alt status reg 1 */
extern volatile uint8_t  SCI5ACR1;	/**< SCI 5 alt control reg 1 */
extern volatile uint8_t  SCI5ACR2;	/**< SCI 5 alt control reg 2 */
/*
 * The following registers are always accessible
 */
extern volatile uint8_t  SCI5CR2;	/**< SCI 5 control register 2 */
extern volatile uint8_t  SCI5SR1;	/**< SCI 5 status register 1 */
extern volatile uint8_t  SCI5SR2;	/**< SCI 5 status register 2 */
extern volatile uint16_t SCI5DR;	/**< SCI 5 data register */
extern volatile uint8_t  SCI5DRH;	/**< SCI 5 data register high */
extern volatile uint8_t  SCI5DRL;	/**< SCI 5 data register low */

/*	CAN0 Module
 */
extern volatile uint8_t  CAN0CTL0;	/**< CAN0 control register 0 */
extern volatile uint8_t  CAN0CTL1;	/**< CAN0 control register 1 */
extern volatile uint8_t  CAN0BTR0;	/**< CAN0 bus timing register 0 */
extern volatile uint8_t  CAN0BTR1;	/**< CAN0 bus timing register 1 */
extern volatile uint8_t  CAN0RFLG;	/**< CAN0 receiver flag register */
extern volatile uint8_t  CAN0RIER;	/**< CAN0 receiver interrupt reg */
extern volatile uint8_t  CAN0TFLG;	/**< CAN0 transmitter flag reg */
extern volatile uint8_t  CAN0TIER;	/**< CAN0 transmitter control reg */
extern volatile uint8_t  CAN0TARQ;	/**< CAN0 transmitter abort req. */
extern volatile uint8_t  CAN0TAAK;	/**< CAN0 transmitter abort ack. */
extern volatile uint8_t  CAN0TBSEL;	/**< CAN0 transmit buffer selection */
extern volatile uint8_t  CAN0IDAC;	/**< CAN0 identifier acceptance */
extern volatile uint8_t  CAN0MISC;	/**< CAN0 micellaneous register */
extern volatile uint8_t  CAN0RXERR;	/**< CAN0 receive error counter */
extern volatile uint8_t  CAN0TXERR;	/**< CAN0 transmit error counter */
extern volatile uint8_t  CAN0IDAR0;	/**< CAN0 id acceptance reg 0 */
extern volatile uint8_t  CAN0IDAR1;	/**< CAN0 id acceptance reg 1 */
extern volatile uint8_t  CAN0IDAR2;	/**< CAN0 id acceptance reg 2 */
extern volatile uint8_t  CAN0IDAR3;	/**< CAN0 id acceptance reg 3 */
extern volatile uint8_t  CAN0IDMR0;	/**< CAN0 id mask register 0 */
extern volatile uint8_t  CAN0IDMR1;	/**< CAN0 id mask register 1 */
extern volatile uint8_t  CAN0IDMR2;	/**< CAN0 id mask register 2 */
extern volatile uint8_t  CAN0IDMR3;	/**< CAN0 id mask register 3 */
extern volatile uint8_t  CAN0IDAR4;	/**< CAN0 id acceptance reg 4 */
extern volatile uint8_t  CAN0IDAR5;	/**< CAN0 id acceptance reg 5 */
extern volatile uint8_t  CAN0IDAR6;	/**< CAN0 id acceptance reg 6 */
extern volatile uint8_t  CAN0IDAR7;	/**< CAN0 id acceptance reg 7 */
extern volatile uint8_t  CAN0IDMR4;	/**< CAN0 id mask register 4 */
extern volatile uint8_t  CAN0IDMR5;	/**< CAN0 id mask register 5 */
extern volatile uint8_t  CAN0IDMR6;	/**< CAN0 id mask register 6 */
extern volatile uint8_t  CAN0IDMR7;	/**< CAN0 id mask register 7 */
extern volatile uint8_t  CAN0RXFG[16];	/**< CAN0 receive buffer */
extern volatile uint8_t  CAN0TXFG[16];	/**< CAN0 transmit buffer */

/*	CAN1 Module
 */
extern volatile uint8_t  CAN1CTL0;	/**< CAN1 control register 0 */
extern volatile uint8_t  CAN1CTL1;	/**< CAN1 control register 1 */
extern volatile uint8_t  CAN1BTR0;	/**< CAN1 bus timing register 0 */
extern volatile uint8_t  CAN1BTR1;	/**< CAN1 bus timing register 1 */
extern volatile uint8_t  CAN1RFLG;	/**< CAN1 receiver flag register */
extern volatile uint8_t  CAN1RIER;	/**< CAN1 receiver interrupt reg */
extern volatile uint8_t  CAN1TFLG;	/**< CAN1 transmitter flag reg */
extern volatile uint8_t  CAN1TIER;	/**< CAN1 transmitter control reg */
extern volatile uint8_t  CAN1TARQ;	/**< CAN1 transmitter abort req. */
extern volatile uint8_t  CAN1TAAK;	/**< CAN1 transmitter abort ack. */
extern volatile uint8_t  CAN1TBSEL;	/**< CAN1 transmit buffer selection */
extern volatile uint8_t  CAN1IDAC;	/**< CAN1 identifier acceptance */
extern volatile uint8_t  CAN1MISC;	/**< CAN1 micellaneous register */
extern volatile uint8_t  CAN1RXERR;	/**< CAN1 transmitter control reg */
extern volatile uint8_t  CAN1TXERR;	/**< CAN1 transmit error counter */
extern volatile uint8_t  CAN1IDAR0;	/**< CAN1 id acceptance reg 0 */
extern volatile uint8_t  CAN1IDAR1;	/**< CAN1 id acceptance reg 1 */
extern volatile uint8_t  CAN1IDAR2;	/**< CAN1 id acceptance reg 2 */
extern volatile uint8_t  CAN1IDAR3;	/**< CAN1 id acceptance reg 3 */
extern volatile uint8_t  CAN1IDMR0;	/**< CAN1 id mask register 0 */
extern volatile uint8_t  CAN1IDMR1;	/**< CAN1 id mask register 1 */
extern volatile uint8_t  CAN1IDMR2;	/**< CAN1 id mask register 2 */
extern volatile uint8_t  CAN1IDMR3;	/**< CAN1 id mask register 3 */
extern volatile uint8_t  CAN1IDAR4;	/**< CAN1 id acceptance reg 4 */
extern volatile uint8_t  CAN1IDAR5;	/**< CAN1 id acceptance reg 5 */
extern volatile uint8_t  CAN1IDAR6;	/**< CAN1 id acceptance reg 6 */
extern volatile uint8_t  CAN1IDAR7;	/**< CAN1 id acceptance reg 7 */
extern volatile uint8_t  CAN1IDMR4;	/**< CAN1 id mask register 4 */
extern volatile uint8_t  CAN1IDMR5;	/**< CAN1 id mask register 5 */
extern volatile uint8_t  CAN1IDMR6;	/**< CAN1 id mask register 6 */
extern volatile uint8_t  CAN1IDMR7;	/**< CAN1 id mask register 7 */
extern volatile uint8_t  CAN1RXFG[16];	/**< CAN1 receive buffer */
extern volatile uint8_t  CAN1TXFG[16];	/**< CAN1 transmit buffer */

/*	CAN2 Module
 */
extern volatile uint8_t  CAN2CTL0;	/**< CAN2 control register 0 */
extern volatile uint8_t  CAN2CTL1;	/**< CAN2 control register 1 */
extern volatile uint8_t  CAN2BTR0;	/**< CAN2 bus timing register 0 */
extern volatile uint8_t  CAN2BTR1;	/**< CAN2 bus timing register 1 */
extern volatile uint8_t  CAN2RFLG;	/**< CAN2 receiver flag register */
extern volatile uint8_t  CAN2RIER;	/**< CAN2 receiver interrupt reg */
extern volatile uint8_t  CAN2TFLG;	/**< CAN2 transmitter flag reg */
extern volatile uint8_t  CAN2TIER;	/**< CAN2 transmitter control reg */
extern volatile uint8_t  CAN2TARQ;	/**< CAN2 transmitter abort req. */
extern volatile uint8_t  CAN2TAAK;	/**< CAN2 transmitter abort ack. */
extern volatile uint8_t  CAN2TBSEL;	/**< CAN2 transmit buffer selection */
extern volatile uint8_t  CAN2IDAC;	/**< CAN2 identifier acceptance */
extern volatile uint8_t  CAN2MISC;	/**< CAN2 micellaneous register */
extern volatile uint8_t  CAN2RXERR;	/**< CAN2 transmitter control reg */
extern volatile uint8_t  CAN2TXERR;	/**< CAN2 transmit error counter */
extern volatile uint8_t  CAN2IDAR0;	/**< CAN2 id acceptance reg 0 */
extern volatile uint8_t  CAN2IDAR1;	/**< CAN2 id acceptance reg 1 */
extern volatile uint8_t  CAN2IDAR2;	/**< CAN2 id acceptance reg 2 */
extern volatile uint8_t  CAN2IDAR3;	/**< CAN2 id acceptance reg 3 */
extern volatile uint8_t  CAN2IDMR0;	/**< CAN2 id mask register 0 */
extern volatile uint8_t  CAN2IDMR1;	/**< CAN2 id mask register 1 */
extern volatile uint8_t  CAN2IDMR2;	/**< CAN2 id mask register 2 */
extern volatile uint8_t  CAN2IDMR3;	/**< CAN2 id mask register 3 */
extern volatile uint8_t  CAN2IDAR4;	/**< CAN2 id acceptance reg 4 */
extern volatile uint8_t  CAN2IDAR5;	/**< CAN2 id acceptance reg 5 */
extern volatile uint8_t  CAN2IDAR6;	/**< CAN2 id acceptance reg 6 */
extern volatile uint8_t  CAN2IDAR7;	/**< CAN2 id acceptance reg 7 */
extern volatile uint8_t  CAN2IDMR4;	/**< CAN2 id mask register 4 */
extern volatile uint8_t  CAN2IDMR5;	/**< CAN2 id mask register 5 */
extern volatile uint8_t  CAN2IDMR6;	/**< CAN2 id mask register 6 */
extern volatile uint8_t  CAN2IDMR7;	/**< CAN2 id mask register 7 */
extern volatile uint8_t  CAN2RXFG[16];	/**< CAN2 receive buffer */
extern volatile uint8_t  CAN2TXFG[16];	/**< CAN2 transmit buffer */

/*	CAN3 Module
 */
extern volatile uint8_t  CAN3CTL0;	/**< CAN3 control register 0 */
extern volatile uint8_t  CAN3CTL1;	/**< CAN3 control register 1 */
extern volatile uint8_t  CAN3BTR0;	/**< CAN3 bus timing register 0 */
extern volatile uint8_t  CAN3BTR1;	/**< CAN3 bus timing register 1 */
extern volatile uint8_t  CAN3RFLG;	/**< CAN3 receiver flag register */
extern volatile uint8_t  CAN3RIER;	/**< CAN3 receiver interrupt reg */
extern volatile uint8_t  CAN3TFLG;	/**< CAN3 transmitter flag reg */
extern volatile uint8_t  CAN3TIER;	/**< CAN3 transmitter control reg */
extern volatile uint8_t  CAN3TARQ;	/**< CAN3 transmitter abort req. */
extern volatile uint8_t  CAN3TAAK;	/**< CAN3 transmitter abort ack. */
extern volatile uint8_t  CAN3TBSEL;	/**< CAN3 transmit buffer selection */
extern volatile uint8_t  CAN3IDAC;	/**< CAN3 identifier acceptance */
extern volatile uint8_t  CAN3MISC;	/**< CAN3 micellaneous register */
extern volatile uint8_t  CAN3RXERR;	/**< CAN3 transmitter control reg */
extern volatile uint8_t  CAN3TXERR;	/**< CAN3 transmit error counter */
extern volatile uint8_t  CAN3IDAR0;	/**< CAN3 id acceptance reg 0 */
extern volatile uint8_t  CAN3IDAR1;	/**< CAN3 id acceptance reg 1 */
extern volatile uint8_t  CAN3IDAR2;	/**< CAN3 id acceptance reg 2 */
extern volatile uint8_t  CAN3IDAR3;	/**< CAN3 id acceptance reg 3 */
extern volatile uint8_t  CAN3IDMR0;	/**< CAN3 id mask register 0 */
extern volatile uint8_t  CAN3IDMR1;	/**< CAN3 id mask register 1 */
extern volatile uint8_t  CAN3IDMR2;	/**< CAN3 id mask register 2 */
extern volatile uint8_t  CAN3IDMR3;	/**< CAN3 id mask register 3 */
extern volatile uint8_t  CAN3IDAR4;	/**< CAN3 id acceptance reg 4 */
extern volatile uint8_t  CAN3IDAR5;	/**< CAN3 id acceptance reg 5 */
extern volatile uint8_t  CAN3IDAR6;	/**< CAN3 id acceptance reg 6 */
extern volatile uint8_t  CAN3IDAR7;	/**< CAN3 id acceptance reg 7 */
extern volatile uint8_t  CAN3IDMR4;	/**< CAN3 id mask register 4 */
extern volatile uint8_t  CAN3IDMR5;	/**< CAN3 id mask register 5 */
extern volatile uint8_t  CAN3IDMR6;	/**< CAN3 id mask register 6 */
extern volatile uint8_t  CAN3IDMR7;	/**< CAN3 id mask register 7 */
extern volatile uint8_t  CAN3RXFG[16];	/**< CAN3 receive buffer */
extern volatile uint8_t  CAN3TXFG[16];	/**< CAN3 transmit buffer */

/*	Port T Module
 */
extern volatile uint8_t  PTT;	/**< port T data register */
extern volatile uint8_t  PTIT;	/**< port T input register */
extern volatile uint8_t  DDRT;	/**< port T data direction */
extern volatile uint8_t  RDRT;	/**< port T reduce drive */
extern volatile uint8_t  PERT;	/**< port T pull enable */
extern volatile uint8_t  PPST;	/**< port T polarity select */

/*	Port S Module
 */
extern volatile uint8_t  PTS;	/**< port S data register */
extern volatile uint8_t  PTIS;	/**< port S input register */
extern volatile uint8_t  DDRS;	/**< port S data direction */
extern volatile uint8_t  RDRS;	/**< port S reduce drive */
extern volatile uint8_t  PERS;	/**< port S pull enable */
extern volatile uint8_t  PPSS;	/**< port S polarity select */
extern volatile uint8_t  WOMS;	/**< port S wired-or mode */

/*	Port M Module
 */
extern volatile uint8_t  PTM;	/**< port M data register */
extern volatile uint8_t  PTIM;	/**< port M input register */
extern volatile uint8_t  DDRM;	/**< port M data direction */
extern volatile uint8_t  RDRM;	/**< port M reduce drive */
extern volatile uint8_t  PERM;	/**< port M pull enable */
extern volatile uint8_t  PPSM;	/**< port M polarity select */
extern volatile uint8_t  WOMM;	/**< port M wired-or mode */
extern volatile uint8_t  MODRR;	/**< module routing register */

/*	Port P Module
 */
extern volatile uint8_t  PTP;	/**< port P data register */
extern volatile uint8_t  PTIP;	/**< port P input register */
extern volatile uint8_t  DDRP;	/**< port P data direction */
extern volatile uint8_t  RDRP;	/**< port P reduce drive */
extern volatile uint8_t  PERP;	/**< port P pull enable */
extern volatile uint8_t  PPSP;	/**< port P polarity select */
extern volatile uint8_t  PIEP;	/**< port P interrupt enable */
extern volatile uint8_t  PIFP;	/**< port P interrupt flag */

/*	Port H Module
 */
extern volatile uint8_t  PTH;	/**< port H data register */
extern volatile uint8_t  PTIH;	/**< port H input register */
extern volatile uint8_t  DDRH;	/**< port H data direction */
extern volatile uint8_t  RDRH;	/**< port H reduce drive */
extern volatile uint8_t  PERH;	/**< port H pull enable */
extern volatile uint8_t  PPSH;	/**< port H polarity select */
extern volatile uint8_t  PIEH;	/**< port H interrupt enable */
extern volatile uint8_t  PIFH;	/**< port H interrupt flag */

/*	Port J Module
 */
extern volatile uint8_t  PTJ;	/**< port J data register */
extern volatile uint8_t  PTIJ;	/**< port J input register */
extern volatile uint8_t  DDRJ;	/**< port J data direction */
extern volatile uint8_t  RDRJ;	/**< port J reduce drive */
extern volatile uint8_t  PERJ;	/**< port J pull enable */
extern volatile uint8_t  PPSJ;	/**< port J polarity select */
extern volatile uint8_t  PIEJ;	/**< port J interrupt enable */
extern volatile uint8_t  PIFJ;	/**< port J interrupt flag */

/*	Port AD0 Module
 */
extern volatile uint8_t  PT0AD0;	/**< port AD0 data register 0 */
extern volatile uint8_t  PT1AD0;	/**< port AD0 data register 1 */
extern volatile uint8_t  DDR0AD0;	/**< port AD0 data direction 0 */
extern volatile uint8_t  DDR1AD0;	/**< port AD0 data direction 1 */
extern volatile uint8_t  RDR0AD0;	/**< port AD0 reduce drive 0 */
extern volatile uint8_t  RDR1AD0;	/**< port AD0 reduce drive 1 */
extern volatile uint8_t  PER0AD0;	/**< port AD0 pull enable 0 */
extern volatile uint8_t  PER1AD0;	/**< port AD0 pull enable 1 */

/*	Port AD1 Module
 */
extern volatile uint8_t  PT0AD1;	/**< port AD1 data register 0 */
extern volatile uint8_t  PT1AD1;	/**< port AD1 data register 1 */
extern volatile uint8_t  DDR0AD1;	/**< port AD1 data direction 0 */
extern volatile uint8_t  DDR1AD1;	/**< port AD1 data direction 1 */
extern volatile uint8_t  RDR0AD1;	/**< port AD1 reduce drive 0 */
extern volatile uint8_t  RDR1AD1;	/**< port AD1 reduce drive 1 */
extern volatile uint8_t  PER0AD1;	/**< port AD1 pull enable 0 */
extern volatile uint8_t  PER1AD1;	/**< port AD1 pull enable 1 */

/*	CAN4 Module
 */
extern volatile uint8_t  CAN4CTL0;	/**< CAN4 control register 0 */
extern volatile uint8_t  CAN4CTL1;	/**< CAN4 control register 1 */
extern volatile uint8_t  CAN4BTR0;	/**< CAN4 bus timing register 0 */
extern volatile uint8_t  CAN4BTR1;	/**< CAN4 bus timing register 1 */
extern volatile uint8_t  CAN4RFLG;	/**< CAN4 receiver flag register */
extern volatile uint8_t  CAN4RIER;	/**< CAN4 receiver interrupt reg */
extern volatile uint8_t  CAN4TFLG;	/**< CAN4 transmitter flag reg */
extern volatile uint8_t  CAN4TIER;	/**< CAN4 transmitter control reg */
extern volatile uint8_t  CAN4TARQ;	/**< CAN4 transmitter abort req. */
extern volatile uint8_t  CAN4TAAK;	/**< CAN4 transmitter abort ack. */
extern volatile uint8_t  CAN4TBSEL;	/**< CAN4 transmit buffer selection */
extern volatile uint8_t  CAN4IDAC;	/**< CAN4 identifier acceptance */
extern volatile uint8_t  CAN4MISC;	/**< CAN4 micellaneous register */
extern volatile uint8_t  CAN4RXERR;	/**< CAN4 transmitter control reg */
extern volatile uint8_t  CAN4TXERR;	/**< CAN4 transmit error counter */
extern volatile uint8_t  CAN4IDAR0;	/**< CAN4 id acceptance reg 0 */
extern volatile uint8_t  CAN4IDAR1;	/**< CAN4 id acceptance reg 1 */
extern volatile uint8_t  CAN4IDAR2;	/**< CAN4 id acceptance reg 2 */
extern volatile uint8_t  CAN4IDAR3;	/**< CAN4 id acceptance reg 3 */
extern volatile uint8_t  CAN4IDMR0;	/**< CAN4 id mask register 0 */
extern volatile uint8_t  CAN4IDMR1;	/**< CAN4 id mask register 1 */
extern volatile uint8_t  CAN4IDMR2;	/**< CAN4 id mask register 2 */
extern volatile uint8_t  CAN4IDMR3;	/**< CAN4 id mask register 3 */
extern volatile uint8_t  CAN4IDAR4;	/**< CAN4 id acceptance reg 4 */
extern volatile uint8_t  CAN4IDAR5;	/**< CAN4 id acceptance reg 5 */
extern volatile uint8_t  CAN4IDAR6;	/**< CAN4 id acceptance reg 6 */
extern volatile uint8_t  CAN4IDAR7;	/**< CAN4 id acceptance reg 7 */
extern volatile uint8_t  CAN4IDMR4;	/**< CAN4 id mask register 4 */
extern volatile uint8_t  CAN4IDMR5;	/**< CAN4 id mask register 5 */
extern volatile uint8_t  CAN4IDMR6;	/**< CAN4 id mask register 6 */
extern volatile uint8_t  CAN4IDMR7;	/**< CAN4 id mask register 7 */
extern volatile uint8_t  CAN4RXFG[16];	/**< CAN4 receive buffer */
extern volatile uint8_t  CAN4TXFG[16];	/**< CAN4 transmit buffer */

/*	ATD0 Module
 */
extern volatile uint8_t  ATD0CTL0;	/**< ATD0 control register 0 */
extern volatile uint8_t  ATD0CTL1;	/**< ATD0 control register 1 */
extern volatile uint8_t  ATD0CTL2;	/**< ATD0 control register 2 */
extern volatile uint8_t  ATD0CTL3;	/**< ATD0 control register 3 */
extern volatile uint8_t  ATD0CTL4;	/**< ATD0 control register 4 */
extern volatile uint8_t  ATD0CTL5;	/**< ATD0 control register 5 */
extern volatile uint8_t  ATD0STAT0;	/**< ATD0 status register 0 */
extern volatile uint16_t ATD0CMPE;	/**< ATD0 compare enable */
extern volatile uint16_t ATD0STAT2;	/**< ATD0 status register 2 */
extern volatile uint16_t ATD0DIEN;	/**< ATD0 interrupt enable */
extern volatile uint16_t ATD0CMPHT;	/**< ATD0 compare higher than */
extern volatile uint16_t ATD0DR[16];	/**< ATD0 result */
extern volatile uint16_t ATD0DR0;	/**< ATD0 result 0 */
extern volatile uint16_t ATD0DR1;	/**< ATD0 result 1 */
extern volatile uint16_t ATD0DR2;	/**< ATD0 result 2 */
extern volatile uint16_t ATD0DR3;	/**< ATD0 result 3 */
extern volatile uint16_t ATD0DR4;	/**< ATD0 result 4 */
extern volatile uint16_t ATD0DR5;	/**< ATD0 result 5 */
extern volatile uint16_t ATD0DR6;	/**< ATD0 result 6 */
extern volatile uint16_t ATD0DR7;	/**< ATD0 result 7 */
extern volatile uint16_t ATD0DR8;	/**< ATD0 result 8 */
extern volatile uint16_t ATD0DR9;	/**< ATD0 result 9 */
extern volatile uint16_t ATD0DR10;	/**< ATD0 result 10 */
extern volatile uint16_t ATD0DR11;	/**< ATD0 result 11 */
extern volatile uint16_t ATD0DR12;	/**< ATD0 result 12 */
extern volatile uint16_t ATD0DR13;	/**< ATD0 result 13 */
extern volatile uint16_t ATD0DR14;	/**< ATD0 result 14 */
extern volatile uint16_t ATD0DR15;	/**< ATD0 result 15 */

/*	VOLTAGE Module
 */
extern volatile uint8_t  VREGHTCL;	/**< VREG test control register */
extern volatile uint8_t  VREGCTRL;	/**< VREG control register */
extern volatile uint8_t  VREGAPICL;	/**< VREG periodic interrupt control */
extern volatile uint8_t  VREGAPITR;	/**< VREG periodic interrupt trimming */
extern volatile uint16_t VREGAPIR;	/**< VREG periodic interrupt rate */
extern volatile uint8_t  VREGAPIRH;	/**< VREG periodic interrupt rate high */
extern volatile uint8_t  VREGAPIRL;	/**< VREG periodic interrupt rate low */
extern volatile uint8_t  VREGHTTR;	/**< VREG test register */

/*	PWM Module
 */
extern volatile uint8_t  PWME;	/**< PWM Enable */
extern volatile uint8_t  PWMPOL;	/**< PWM Clock Polarity */
extern volatile uint8_t  PWMCLK;	/**< PWM Clocks */
extern volatile uint8_t  PWMPRCLK;	/**< PWM prescale clock select */
extern volatile uint8_t  PWMCAE;	/**< PWM center align enable */
extern volatile uint8_t  PWMCTL;	/**< PWM Control Register */
extern volatile uint8_t  PWMTST;	/**< PWM Test Register */
extern volatile uint8_t  PWMPRSC;	/**< PWM Test Register */
extern volatile uint8_t  PWMSCLA;	/**< PWM scale A */
extern volatile uint8_t  PWMSCLB;	/**< PWM scale B */
extern volatile uint8_t  PWMSCNTA;	/**< PWM Test Register A */
extern volatile uint8_t  PWMSCNTB;	/**< PWM Test Register B */
extern volatile uint8_t  PWMCNT0;	/**< PWM Channel Counter 0 */
extern volatile uint8_t  PWMCNT1;	/**< PWM Channel Counter 1 */
extern volatile uint8_t  PWMCNT2;	/**< PWM Channel Counter 2 */
extern volatile uint8_t  PWMCNT3;	/**< PWM Channel Counter 3 */
extern volatile uint8_t  PWMCNT4;	/**< PWM Channel Counter 4 */
extern volatile uint8_t  PWMCNT5;	/**< PWM Channel Counter 5 */
extern volatile uint8_t  PWMCNT6;	/**< PWM Channel Counter 6 */
extern volatile uint8_t  PWMCNT7;	/**< PWM Channel Counter 7 */
extern volatile uint8_t  PWMPER0;	/**< PWM Channel Period 0 */
extern volatile uint8_t  PWMPER1;	/**< PWM Channel Period 1 */
extern volatile uint8_t  PWMPER2;	/**< PWM Channel Period 2 */
extern volatile uint8_t  PWMPER3;	/**< PWM Channel Period 3 */
extern volatile uint8_t  PWMPER4;	/**< PWM Channel Period 4 */
extern volatile uint8_t  PWMPER5;	/**< PWM Channel Period 5 */
extern volatile uint8_t  PWMPER6;	/**< PWM Channel Period 6 */
extern volatile uint8_t  PWMPER7;	/**< PWM Channel Period 7 */
extern volatile uint8_t  PWMDTY0;	/**< PWM Channel Duty 0 */
extern volatile uint8_t  PWMDTY1;	/**< PWM Channel Duty 1 */
extern volatile uint8_t  PWMDTY2;	/**< PWM Channel Duty 2 */
extern volatile uint8_t  PWMDTY3;	/**< PWM Channel Duty 3 */
extern volatile uint8_t  PWMDTY4;	/**< PWM Channel Duty 4 */
extern volatile uint8_t  PWMDTY5;	/**< PWM Channel Duty 5 */
extern volatile uint8_t  PWMDTY6;	/**< PWM Channel Duty 6 */
extern volatile uint8_t  PWMDTY7;	/**< PWM Channel Duty 7 */
extern volatile uint8_t  PWMSDN;	/**< PWM Shutdown register */

/*	SCI6 Module
 *
 * The following registers are accessible if the AMAP bit in the SCI6SR2
 * register is set to zero
 */
extern volatile uint16_t SCI6BD;	/**< SCI 6 baud rate */
extern volatile uint8_t  SCI6BDH;	/**< SCI 6 baud rate high */
extern volatile uint8_t  SCI6BDL;	/**< SCI 6 baud rate low */
extern volatile uint8_t  SCI6CR1;	/**< SCI 6 control register 1 */
/*
 * The following registers are accessible if the AMAP bit in the SCI6SR2
 * register is set to one
 */
extern volatile uint8_t  SCI6ASR1;	/**< SCI 6 alt status reg 1 */
extern volatile uint8_t  SCI6ACR1;	/**< SCI 6 alt control reg 1 */
extern volatile uint8_t  SCI6ACR2;	/**< SCI 6 alt control reg 2 */
/*
 * The following registers are always accessible
 */
extern volatile uint8_t  SCI6CR2;	/**< SCI 6 control register 2 */
extern volatile uint8_t  SCI6SR1;	/**< SCI 6 status register 1 */
extern volatile uint8_t  SCI6SR2;	/**< SCI 6 status register 2 */
extern volatile uint16_t SCI6DR;	/**< SCI 6 data register */
extern volatile uint8_t  SCI6DRH;	/**< SCI 6 data register high */
extern volatile uint8_t  SCI6DRL;	/**< SCI 6 data register low */

/*	SCI7 Module
 *
 * The following registers are accessible if the AMAP bit in the SCI7SR2
 * register is set to zero
 */
extern volatile uint16_t SCI7BD;	/**< SCI 7 baud rate */
extern volatile uint8_t  SCI7BDH;	/**< SCI 7 baud rate high */
extern volatile uint8_t  SCI7BDL;	/**< SCI 7 baud rate low */
extern volatile uint8_t  SCI7CR1;	/**< SCI 7 control register 1 */
/*
 * The following registers are accessible if the AMAP bit in the SCI7SR2
 * register is set to one
 */
extern volatile uint8_t  SCI7ASR1;	/**< SCI 7 alt status reg 1 */
extern volatile uint8_t  SCI7ACR1;	/**< SCI 7 alt control reg 1 */
extern volatile uint8_t  SCI7ACR2;	/**< SCI 7 alt control reg 2 */
/*
 * The following registers are always accessible
 */
extern volatile uint8_t  SCI7CR2;	/**< SCI 7 control register 2 */
extern volatile uint8_t  SCI7SR1;	/**< SCI 7 status register 1 */
extern volatile uint8_t  SCI7SR2;	/**< SCI 7 status register 2 */
extern volatile uint16_t SCI7DR;	/**< SCI 7 data register */
extern volatile uint8_t  SCI7DRH;	/**< SCI 7 data register high */
extern volatile uint8_t  SCI7DRL;	/**< SCI 7 data register low */

/*	PIT Module
 */
extern volatile uint8_t  PITCFLMT;	/**< PIT control micro timer register */
extern volatile uint8_t  PITFLT;	/**< PIT force load timer register */
extern volatile uint8_t  PITCE;	/**< PIT channel enable register */
extern volatile uint8_t  PITMUX;	/**< PIT multiplex register */
extern volatile uint8_t  PITINTE;	/**< PIT interrupt enable register */
extern volatile uint8_t  PITTF;	/**< PIT time-out flag register */
extern volatile uint8_t  PITMTLD0;	/**< PIT micro timer load reg. 0 */
extern volatile uint8_t  PITMTLD1;	/**< PIT micro timer load reg. 1 */
extern volatile uint16_t PITLD0;	/**< PIT load register 0 */
extern volatile uint16_t PITCNT0;	/**< PIT count register 0 */
extern volatile uint16_t PITLD1;	/**< PIT load register 1 */
extern volatile uint16_t PITCNT1;	/**< PIT count register 1 */
extern volatile uint16_t PITLD2;	/**< PIT load register 2 */
extern volatile uint16_t PITCNT2;	/**< PIT count register 2 */
extern volatile uint16_t PITLD3;	/**< PIT load register 3 */
extern volatile uint16_t PITCNT3;	/**< PIT count register 3 */
extern volatile uint16_t PITLD4;	/**< PIT load register 4 */
extern volatile uint16_t PITCNT4;	/**< PIT count register 4 */
extern volatile uint16_t PITLD5;	/**< PIT load register 5 */
extern volatile uint16_t PITCNT5;	/**< PIT count register 5 */
extern volatile uint16_t PITLD6;	/**< PIT load register 6 */
extern volatile uint16_t PITCNT6;	/**< PIT count register 6 */
extern volatile uint16_t PITLD7;	/**< PIT load register 7 */
extern volatile uint16_t PITCNT7;	/**< PIT count register 7 */

/*	Port R Module
 */
extern volatile uint8_t  PTR;	/**< port R data register */
extern volatile uint8_t  PTIR;	/**< port R input register */
extern volatile uint8_t  DDRR;	/**< port R data direction */
extern volatile uint8_t  RDRR;	/**< port R reduce drive */
extern volatile uint8_t  PERR;	/**< port R pull enable */
extern volatile uint8_t  PPSR;	/**< port R polarity select */
extern volatile uint8_t  PTRRR;	/**< port R routing register */

/*	Port L Module
 */
extern volatile uint8_t  PTL;	/**< port L data register */
extern volatile uint8_t  PTIL;	/**< port L input register */
extern volatile uint8_t  DDRL;	/**< port L data direction */
extern volatile uint8_t  RDRL;	/**< port L reduce drive */
extern volatile uint8_t  PERL;	/**< port L pull enable */
extern volatile uint8_t  PPSL;	/**< port L polarity select */
extern volatile uint8_t  WOML;	/**< port L wired-or mode */
extern volatile uint8_t  PTLRR;	/**< port L routing register */

/*	Port F Module
 */
extern volatile uint8_t  PTF;	/**< port F data register */
extern volatile uint8_t  PTIF;	/**< port F input register */
extern volatile uint8_t  DDRF;	/**< port F data direction */
extern volatile uint8_t  RDRF;	/**< port F reduce drive */
extern volatile uint8_t  PERF;	/**< port F pull enable */
extern volatile uint8_t  PPSF;	/**< port F polarity select */
extern volatile uint8_t  PTFRR;	/**< port F routing register */

/*	XGATE Module
 */
extern volatile uint16_t XGMCTL;	/**< XGATE module control reg. */
extern volatile uint8_t  XGCHID;	/**< XGATE channel ID register */
extern volatile uint8_t  XGCHPL;	/**< XGATE channel priority level */
extern volatile uint8_t  XGISPSEL;	/**< XGATE initial stack ptr and XGVBR select */
extern volatile uint16_t XGISP74;	/**< XGATE initial SP for priorites 7-4 when XGISPSEL=2 */
extern volatile uint16_t XGISP31;	/**< XGATE initial SP for priorites 3-1 when XGISPSEL=1 */
extern volatile uint16_t XGVBR;	/**< XGATE vector base register when XGISPSEL=0 */
extern volatile uint8_t  XGVBRH;	/**< XGATE vector base reg. high */
extern volatile uint8_t  XGVBRL;	/**< XGATE vector base reg. low */
extern volatile uint16_t XGIF[8];	/**< XGATE interrupt flag vector */
extern volatile uint16_t XGSWT;	/**< 16-bit XGATE software trigger reg. */
extern volatile uint8_t  XGSWTM;	/**< XGATE software trigger mask reg. */
extern volatile uint8_t  XGSWTB;	/**< XGATE software trigger bits reg. */
extern volatile uint16_t XGSEM;	/**< XGATE semaphore register */
extern volatile uint8_t  XGCCR;	/**< XGATE condition code reg. */
extern volatile uint16_t XGPC;	/**< XGATE program counter reg. */
extern volatile uint16_t XGR1;	/**< XGATE register 1 */
extern volatile uint16_t XGR2;	/**< XGATE register 2 */
extern volatile uint16_t XGR3;	/**< XGATE register 3 */
extern volatile uint16_t XGR4;	/**< XGATE register 4 */
extern volatile uint16_t XGR5;	/**< XGATE register 5 */
extern volatile uint16_t XGR6;	/**< XGATE register 6 */
extern volatile uint16_t XGR7;	/**< XGATE register 7 */

/*	TIM Module
 */
extern volatile uint8_t  T_TIOS;	/**< timer select register */
extern volatile uint8_t  T_CFORC;	/**< compare force register */
extern volatile uint8_t  T_OC7M;	/**< oc7 mask register */
extern volatile uint8_t  T_OC7D;	/**< oc7 data register */
extern volatile uint16_t T_TCNT;	/**< timer counter */
extern volatile uint8_t  T_TSCR1;	/**< system control register 1 */
extern volatile uint8_t  T_TTOV;	/**< toggle on overflow register */
extern volatile uint8_t  T_TCTL1;	/**< control register 1 */
extern volatile uint8_t  T_TCTL2;	/**< control register 2 */
extern volatile uint8_t  T_TCTL3;	/**< control register 3 */
extern volatile uint8_t  T_TCTL4;	/**< control register 4 */
extern volatile uint8_t  T_TIE;	/**< interrupt enable register */
extern volatile uint8_t  T_TSCR2;	/**< system control register 2 */
extern volatile uint8_t  T_TFLG1;	/**< interrupt flag register 1 */
extern volatile uint8_t  T_TFLG2;	/**< interrupt flag register 2 */
extern volatile uint16_t T_TC0;	/**< capture/compare register 0 */
extern volatile uint16_t T_TC1;	/**< capture/compare register 0 */
extern volatile uint16_t T_TC2;	/**< capture/compare register 0 */
extern volatile uint16_t T_TC3;	/**< capture/compare register 0 */
extern volatile uint16_t T_TC4;	/**< capture/compare register 0 */
extern volatile uint16_t T_TC5;	/**< capture/compare register 0 */
extern volatile uint16_t T_TC6;	/**< capture/compare register 0 */
extern volatile uint16_t T_TC7;	/**< capture/compare register 0 */
extern volatile uint8_t  T_PACTL;	/**< pulse accumulator A control */
extern volatile uint8_t  T_PAFLG;	/**< pulse accumulator A flag */
extern volatile uint16_t T_PACNT;	/**< pulse accumulator A count */
extern volatile uint8_t  T_OCPD;	/**< output compare pin disconnect */
extern volatile uint8_t  T_PTPSR;	/**< precision prescaler select reg */

#endif /* #if !defined (REGS_H) */
