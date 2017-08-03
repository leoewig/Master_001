/**
 * @file
 * @ingroup s12_xgate
 *
 * XGATE vector table
 *
 * $Id: xgate_vector.c 2140 2012-05-18 14:19:01Z tg10 $
 */
#define	_XGATE_
#include "hardware.h"
#include "gate.h"

#define UNDEFD  {x_unhandled_isr, (void *)0}

#pragma section const {xvectors}
/*  gate table
 */
/**
 * @lint Investigate : Symbols not referenced\n
 * lint -esym(528,xvector)\n
 * @lint Investigate : Structure members not referenced:\n
 * lint -esym(754,gate_s::isr) \n
 * lint -esym(754,gate_s::data)
 */
/*lint -esym(528,xvector) */
/*lint -esym(754,gate_s::isr) */
/*lint -esym(754,gate_s::data) */
static const struct gate_s
{
  INTERRUPT void (*isr)(); /**< XGATE interrupt service routine \n
  				(arguments deliberately unspecified as they
				will vary) */
  void *data;		/**< Pointer to function arguments \n
			     (or can optionally be interpreted as a uint16_t
			      value)  */
} xvector[] = {
    UNDEFD,         /* 0x00 spurious interrupt */
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD,
    UNDEFD, UNDEFD, UNDEFD, UNDEFD, UNDEFD, UNDEFD, UNDEFD, UNDEFD,
    UNDEFD, UNDEFD, UNDEFD, UNDEFD, UNDEFD, UNDEFD, UNDEFD, UNDEFD,
    UNDEFD, UNDEFD, UNDEFD, UNDEFD,
    UNDEFD,         /* 0x2c periodic interrupt timer 7 */
    UNDEFD,         /* 0x2d periodic interrupt timer 6 */
    UNDEFD,         /* 0x2e periodic interrupt timer 5 */
    UNDEFD,         /* 0x2f periodic interrupt timer 4 */
    UNDEFD,         /* 0x30 TODO check: xram access violation */
    UNDEFD,         /* 0x31 TODO check: xgate software error  */
    UNDEFD,         /* 0x32 xgate software trigger 7 */
    UNDEFD,         /* 0x33 xgate software trigger 6 */
    UNDEFD,         /* 0x34 xgate software trigger 5 */
    UNDEFD,         /* 0x35 xgate software trigger 4 */
    UNDEFD,         /* 0x36 xgate software trigger 3 */
    UNDEFD,         /* 0x37 xgate software trigger 2 */
    UNDEFD,         /* 0x38 xgate software trigger 1 */
    UNDEFD,         /* 0x39 xgate software trigger 0 */
    UNDEFD,         /* 0x3a periodic interrupt timer 3 */
    UNDEFD,	/* 0x3b periodic interrupt timer 2 */
    UNDEFD,	/* 0x3c periodic interrupt timer 1 */
    UNDEFD,          /* 0x3d periodic interrupt timer 0 */
    UNDEFD,         /* 0x3e reserved */
    UNDEFD,         /* 0x3f autonomous periodical interrupt */
    UNDEFD,         /* 0x40 low voltage interrupt */
    UNDEFD,         /* 0x41 IIC1 */
    UNDEFD,         /* 0x42 SCI5 */
    UNDEFD,         /* 0x43 SCI4 */
    UNDEFD,         /* 0x44 SCI3 */
    UNDEFD,         /* 0x45 SCI2 */
    UNDEFD,         /* 0x46 PWM emergency shutdown */
    UNDEFD,         /* 0x47 port P interrupt */
    UNDEFD,         /* 0x48 MSCAN 4 transmit */
    UNDEFD,         /* 0x49 MSCAN 4 receive */
    UNDEFD,         /* 0x4a MSCAN 4 errors */
    UNDEFD,         /* 0x4b MSCAN 4 wake-up */
    UNDEFD,         /* 0x4c MSCAN 3 transmit */
    UNDEFD,         /* 0x4d MSCAN 3 receive */
    UNDEFD,         /* 0x4e MSCAN 3 errors */
    UNDEFD,         /* 0x4f MSCAN 3 wake-up */
    UNDEFD,          /* 0x58 MSCAN 2 transmit */
    UNDEFD,          /* 0x59 MSCAN 2 receive */
    UNDEFD,         /* 0x52 MSCAN 2 errors */
    UNDEFD,         /* 0x53 MSCAN 2 wake-up */
    UNDEFD,         /* 0x54 MSCAN 1 transmit */
    UNDEFD,         /* 0x55 MSCAN 1 receive */
    UNDEFD,         /* 0x56 MSCAN 1 errors */
    UNDEFD,         /* 0x57 MSCAN 1 wake-up */
    UNDEFD,         /* 0x58 MSCAN 0 transmit */
    UNDEFD,         /* 0x59 MSCAN 0 receive */
    UNDEFD,         /* 0x5a MSCAN 0 errors */
    UNDEFD,         /* 0x5b MSCAN 0 wake-up */
    UNDEFD,         /* 0x5c flash */
    UNDEFD,         /* 0x5d eeprom */
    UNDEFD,         /* 0x5e SPI2 */
    UNDEFD,         /* 0x5f SPI1 */
    UNDEFD,         /* 0x60 IIC0 */
    UNDEFD,         /* 0x61 reserved */
    UNDEFD,         /* 0x62 CRG self clock mode */
    UNDEFD,         /* 0x63 CRG PLL lock */
    UNDEFD,         /* 0x64 pulse accumulator B overflow */
    UNDEFD,         /* 0x65 modulus down counter underflow */
    UNDEFD,         /* 0x66 port H */
    UNDEFD,         /* 0x67 port J */
    UNDEFD,         /* 0x68 ATD1 */
    UNDEFD,         /* 0x69 ATD0 */
    UNDEFD,         /* 0x6a SCI1 */
    UNDEFD,         /* 0x6b SCI0 */
    UNDEFD,         /* 0x6c SPI0 */
    UNDEFD,         /* 0x6d pulse accumulator input edge */
    UNDEFD,         /* 0x6e pulse accumulator A overflow */
    UNDEFD,         /* 0x6f enhanced capture timer overflow */
    UNDEFD,         /* 0x70 enhanced capture timer channel 7 */
    UNDEFD,         /* 0x71 enhanced capture timer channel 6 */
    UNDEFD,         /* 0x72 enhanced capture timer channel 5 */
    UNDEFD,         /* 0x73 enhanced capture timer channel 4 */
    UNDEFD,         /* 0x74 enhanced capture timer channel 3 */
    UNDEFD,         /* 0x75 enhanced capture timer channel 2 */
    UNDEFD,         /* 0x76 enhanced capture timer channel 1 */
    UNDEFD,         /* 0x77 enhanced capture timer channel 0 */
    UNDEFD,         /* 0x78 real time interrupt */
    };
#pragma section const {}
