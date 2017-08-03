/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   spi_types.h
 *
 * description:  
 * Declarations required by both SPI masters and SPI slaves
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
#ifndef SPI_TYPES_H
#define SPI_TYPES_H
/**
 * @file
 * @ingroup spi
 *
 * Declarations required by both SPI masters and SPI slaves
 *
 */

/*
 * SPIxCR1 bits
 */
#define         SPICR1_ENABLE           0x40u /**< SPE bit of SPIxCR1 enables
                                                the SPI port */

/** mask for SSOE bit of SPIxCR1.  See spi_set_slave_select_logic().  */
#define         SPICR1_SSOE             0x02u
/** mask for MODFEN bit of SPIxCR2.  See spi_set_slave_select_logic() */
#define         SPICR2_MODFEN           0x10u
/** mask for SPC0 bit of SPIxCR2.  See spi_set_pinmode_logic() */
#define         SPICR2_SPC0             0x01u
/** mask for BIDIROE bit of SPIxCR2.  See spi_set_pinmode_logic() */
#define         SPICR2_BIDIROE          0x08u

/*
 * SPIxSR bits
 */
#define TRANSMIT_EMPTY_FLAG             0x20u   /**< SPIxSR bit showing
                                                  transmitter can be reloaded */
#define DATA_RECEIVED_FLAG              0x80u   /** SPIxSR bit showing data has
                                                  arrived */

/*
 * enums used in SPI configuration
 */
/** Master or slave selection (values chosen allow ORing into SPIxCR1.MSTR */
enum spi_ms_e {
    Spi_ms_slave=0u,        /**< controller is SPI slave */
    Spi_ms_master=0x10u     /**< controller is SPI master */
};

/**
 * SPI baudrate
 *
 * if the required speed is not here, add a new member to the list with the
 * value equal to that required in the baud-rate register
 * (see table 21-7 in MC9S12XEP100 ref manual for available speeds but
 *  note that table is for 25MHz bus but we have a 50MHz bus))
 * @note This enumeration assumes a 50MHx bus frequency
 */
enum spi_spd_e {
    Spi_spd_12p5mhz=1u,     /**< 12.5MHz - the closest to 20MHz we can get*/
    Spi_spd_5mhz=0x40u,     /**< 5MHz */
    Spi_spd_4p167mhz=0x21u, /**< 4.16667 MHz bus - thought this was the nearest
                              to 5MHz till I finally spotted 2.5MHz in the
                              table */
    Spi_spd_48p8khz=0x76    /**< 48.8khz */
};

/** SPI bus width (values chosen allow ORing into SPIxCR2.XFRW */
enum spi_width_e {
    Spi_width_8bit=0u,      /**< all transfers 8 bits */
    Spi_width_16bit=0x40u   /**< all transfers 16 bits */
};

/** SPI slave-select (SS) control */
enum spi_ss_e {
    Spi_ss_na,      /**< not applicable (use if controller is a slave) */
    Spi_ss_explicit,    /**< spi_transfer() must drive SS line low at start of
                transfer at high at the end */
    Spi_ss_auto     /**< SS line automatically driven by SPI h/w */
};

/** SPI pin mode control */
enum spi_pinmode_e {
    Spi_pinmode_normal,       /**< Normal mode (MOSI and MISO used) */
    Spi_pinmode_bidirectional /**< Bidirectional mode (MISO not used when in Master mode,
                                 MOSI not used when in Slave mode) */
};

/** SPI clock polarity (values chosen to OR into SPIxCR1 */
enum spi_cpol_e {
    Spi_cpol_0 = 0u,    /**< SPI clock active high */
    Spi_cpol_1 = 0x08u  /**< SPI clock active low */
};

/** SPI clock phase (values chosen to OR into SPIxCR1 */
enum spi_cpha_e {
    Spi_cpha_0 = 0u,    /**< sample data on odd edges */
    Spi_cpha_1 = 0x04u  /**< sample data on even edges */
};

/** SPI port to pin mapping (values chosen to OR into MODRR)
 * numbers in the name represent pins for MISO, MOSI, SCK, SS respectively */
enum spi_pins_e {
    Spi_pins_spi0_ps4567=0u,    /**< SPI0 on PS[4:7] */
    Spi_pins_spi0_pm2453=0x10u, /**< SPI0 on PM[2:5] (note irregular order) */
    Spi_pins_spi1_pp0123=0u,    /**< SPI1 on PP[0:3] */
    Spi_pins_spi1_ph0123=0x20u, /**< SPI1 on PH[0:3] */
    Spi_pins_spi2_pp4576=0u,    /**< SPI2 on PP[4:7] (note irregular order) */
    Spi_pins_spi2_ph4567=0x40u  /**< SPI2 on PH[4:7] */
};

#endif /* SPI_TYPES_H */
