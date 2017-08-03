/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_spi_cfg.c
 *
 * description:  
 * SPI bus assignments for the MMC
 *
 * The assignments are brought out to this file to facilitate changes
 * without having to edit the functional platform code
 *
 * @note This file will be copied by the build system to spi_cfg.c
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/

#include "hardware.h"
#include "spi.h"
#include "spi_p.h"

/**
 * SPI bus configuration
 *
 * See #spi_cfg_s for the meaning of the fields.
 *
 * @note the order of entries must agree with the spi_channel_e enumeration
 * in spi_cfg.h
 */
FROM struct spi_cfg_s spi_cfg[Spi_n_channels] =
{
   // /* PSU latch on SPI 0 (pins PS[4..6]) */
   // { &spi0, Spi_pins_spi0_ps4567, Spi_ms_master, Spi_spd_48p8khz,
   //     Spi_width_8bit, Spi_cpol_0, Spi_cpha_1,
   //     Spi_ss_na, 
   //     Spi_pinmode_normal,
   //     &PTS, &PTIS, &DDRS, 0x80u },
   //

    /* EEPROM on SPI 0 (pins PS[4..6]) */
    { &spi0, Spi_pins_spi0_ps4567, Spi_ms_master, Spi_spd_4p167mhz,
        Spi_width_8bit, Spi_cpol_0, Spi_cpha_0,
        Spi_ss_explicit, 
        Spi_pinmode_normal,
        &PTS, &PTIS, &DDRS, 0x80u },

	
	     /* VTBM 6804 on SPI 1 (pins PH[0..3]) */	
    // { &spi1, Spi_pins_spi1_ph0123, Spi_ms_master, Spi_spd_4p167mhz,
    //     Spi_width_8bit, Spi_cpol_0, Spi_cpha_0,
    //     Spi_ss_explicit, /* manual control to allow multi-byte transfers */
    //     Spi_pinmode_normal,
    //     &PTH, &PTIH, &DDRH, 0x08u },

    /* DAC1 on SPI 1 (pins PH[0..3]) */ 
    { &spi1, Spi_pins_spi1_ph0123, Spi_ms_master, Spi_spd_4p167mhz,
        Spi_width_8bit, Spi_cpol_0, Spi_cpha_1,
        Spi_ss_explicit, /* manual control to allow multi-byte transfers */
        Spi_pinmode_normal,
        &PTH, &PTIH, &DDRH, 0x08u },
   
		/* DAC2 on SPI 2 (pins PH[4:7])*/
    { &spi2, Spi_pins_spi2_ph4567, Spi_ms_master, Spi_spd_4p167mhz,
        Spi_width_8bit, Spi_cpol_0, Spi_cpha_1,
        Spi_ss_explicit, /* manual control to allow multi-byte transfers */
        Spi_pinmode_normal,
        &PTH, &PTIH, &DDRH, 0x80u },
};
