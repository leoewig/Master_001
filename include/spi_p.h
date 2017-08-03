/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   spi_p.h
 *
 * description:  
 * Private header for SPI driver
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
/**
 * @file
 * @ingroup spi
 *
 * Private header for SPI driver
 *
 * This header defines the memory-mapped structures which drive the SPI
 * hardware and declares the structure used to hold SPI config info
 *
 */
/*
 * need to include register location header ioxep100.h rather than regs.h
 * because need address information for structure location definitions
 * spi0, spi1 and spi2 below
 */

#include "spi_types.h"

/**
 * structure to overlay the SPI registers
 */
struct spi_regs_s {
    uint8_t cr1;        /**< Control Register 1 */
    uint8_t cr2;        /**< Control Register 2 */
    uint8_t br;         /**< Baud rate register */
    volatile uint8_t sr;    /**< status register */
    volatile union {
    uint16_t w;     /**< 16-bit access */
    struct {
        uint8_t h;          /**< high byte */
        uint8_t l;          /**< low byte */
    } b;            /**< 8-bit access */
    } dr;           /**< data register */
};

/**
 * @misra
 * MISRA 18.2 deviation - ID TBD
 * Object assigned to overlapping object.
 */

/**
 * @lint
 * Definition of SPI registers leads to multiple copies of the same variable
 * name being declared at the same address, suppress:\n
 * -esym(14,spi0,spi1,spi2)
 */
/*lint -esym(14,spi0,spi1,spi2) */

/*
 * locate structures over the SPI registers
 */

struct spi_regs_s ABS_LOCATE(spi0,SPI0CR1);   /**< SPI 0 registers */
struct spi_regs_s ABS_LOCATE(spi1,SPI1CR1);   /**< SPI 1 registers */
struct spi_regs_s ABS_LOCATE(spi2,SPI2CR1);   /**< SPI 2 registers */

/**
 * structure used to define SPI configuration
 *
 * @note hwp, pins, ss_port, monitor_port, ss_port_ddr and ss_port_mask are all
 * mutually dependent so all five fields must be edited together to ensure
 * self-consistency. (With only 3 SPI ports it's just not worth trying to do it
 * another way)
 */

#pragma align on
struct spi_cfg_s {
    struct spi_regs_s *hwp;     /**< ptr to controller h/w registers */
    enum spi_pins_e pins;       /**< port to pins mapping */
    enum spi_ms_e master_slave;     /**< configure as master or slave */
    enum spi_spd_e speed;       /**< baud rate */
    enum spi_width_e width;     /**< 8/16 bit transfers */
    enum spi_cpol_e cpol;       /**< clock polarity */
    enum spi_cpha_e cpha;       /**< clock phase */
    enum spi_ss_e ss;           /**< Slave select line control */
    enum spi_pinmode_e pinmode; /**< Normal or bidirectional pin mode */
    volatile uint8_t *ss_port;      /**< ptr to slave select I/O port
                      (ignored if controller is a slave or
                      if SS control is automattic) */
    volatile uint8_t *ss_monitor;      /**< ptr to slave select input
                      monitor port (ignored if controller is a slave or
                      if SS control is automattic) */
    volatile uint8_t *ss_port_ddr;  /**< ptr to slave select I/O port data
                      direction register (ignored if controller is a slave or
                      if SS control is automattic) */
    uint8_t ss_port_mask;       /**< mask for ss_port/ss_monitor/ss_port_ddr
                      (ignored if controller is a slave or if SS control
                      is automattic) */
};
#pragma align off

PRIVATE void spi_init_channel(enum spi_channel_e chan);
PRIVATE void spi_reset_port(struct spi_regs_s *hwp);
PRIVATE void spi_select_pins(enum spi_pins_e pins);
PRIVATE void spi_set_master_or_slave(struct spi_regs_s *hwp,
                                        enum spi_ms_e master_slave);
PRIVATE void spi_set_baudrate(struct spi_regs_s *hwp,enum spi_spd_e speed);
PRIVATE void spi_set_clock_format(struct spi_regs_s *hwp,enum spi_cpol_e cpol,
                                    enum spi_cpha_e cpha);
PRIVATE void spi_set_buswidth(struct spi_regs_s *hwp,enum spi_width_e width);
PRIVATE void spi_set_slave_select_logic(struct spi_regs_s *hwp,
                        enum spi_ss_e ss_mode, enum spi_ms_e master_slave);
PRIVATE void spi_set_bidirectional(struct spi_regs_s *hwp,
                        enum spi_pinmode_e mode);

