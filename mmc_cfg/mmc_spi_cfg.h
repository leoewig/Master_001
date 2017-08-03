/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_spi_cfg.h
 *
 * description:  
 * SPI bus assignments for the MMC
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/
#ifndef MMC_SPI_DEF_H
#define MMC_SPI_DEF_H
/**
 * @file
 * @ingroup mmc_cfg
 * @ingroup spi
 *
 * SPI bus assignments for the MMC
 *
 * @note this file will be copied by the build system to spi_cfg.h
 *
 */
/* Copyright (c) Linkedriving Tech 2012 */

/**
 * enumeration of SPI channels.
 *
 * @note The value of the enum must correspond to the
 * order of definition in #spi_cfg[]
 */
enum spi_channel_e {
   Spi_EEPROM=0,
   SPI_DAC1,
   SPI_DAC2,
   Spi_n_channels      /* must be last, do not change */
} ;

extern FROM struct spi_cfg_s spi_cfg[Spi_n_channels];

#endif /* MMC_SPI_DEF_H */

