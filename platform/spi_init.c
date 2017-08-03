/**
 * @file
 * @ingroup spi
 *
 * SPI port initialisation
 *
 * $Id: spi_init.c 6550 2013-01-29 12:03:51Z pr4 $
 */
#include "basic_types.h"
#include "hardware.h"
#include "spi.h"
#include "spi_p.h"

#ifdef _XGATE_
#error spi_init functions must be executed from the S12 and not the XGATE
#endif

/**
 * Implementation
 * --------------
 *
 * This function is intended for the S12 only since it must disable
 * interrupts to set up the slave select lines.
 */
void spi_init(void)
{
    uint8_t chan;

    for (chan = 0u ; chan < (uint8_t) Spi_n_channels ; chan++)
    {
        spi_init_channel((enum spi_channel_e) chan);
    }
}


/* documentation in header file */
void spi_enable(enum spi_channel_e channel)
{
    FROM struct spi_cfg_s *cfgp = &spi_cfg[channel];
    struct spi_regs_s *hwp = cfgp->hwp;
    uint8_t ccr;        /* register to save CCR for SAVE/RESTORE_INTERRUPTS */

    if (cfgp->ss == Spi_ss_explicit)
    {
        /*
         * don't risk any foreground interference with the read-modify-write
         * of the ports
         */
        SAVE_INTERRUPTS(ccr);
        *cfgp->ss_port_ddr |= cfgp->ss_port_mask;
        *cfgp->ss_port |= cfgp->ss_port_mask;
        RESTORE_INTERRUPTS(ccr);
    }
    hwp->cr1 |= (uint8_t) SPICR1_ENABLE;
}

/* documentation in header file */
void spi_disable(enum spi_channel_e channel)
{
    FROM struct spi_cfg_s *cfgp = &spi_cfg[channel];
    struct spi_regs_s *hwp = cfgp->hwp;
    uint8_t ccr;        /* register to save CCR for SAVE/RESTORE_INTERRUPTS */

    /* disable spi */
    hwp->cr1 &= (uint8_t) ~SPICR1_ENABLE;

    /*
     * and force SS to be an input by clearing it's bit in the data direction
     * register (avoiding read-modify-write problems by disabling interrupts).
     */
    SAVE_INTERRUPTS(ccr);
    *cfgp->ss_port_ddr &= (uint8_t) ~ cfgp->ss_port_mask ;
    RESTORE_INTERRUPTS(ccr);
}

/**
 * Initialialises a single spi channel
 *
 * The approach taken is to clear the two control registers (SPICR1 and SPICR2)
 * with spi_reset_port() and then build them up function by function.
 *
 * @param       chan    index into spi_cfg[] for the channel to be initialised
 */
PRIVATE void spi_init_channel(enum spi_channel_e chan)
{
    FROM struct spi_cfg_s *cfgp = &spi_cfg[chan]; /* ptr to channel
                                                     configuration */
    struct spi_regs_s *hwp = cfgp->hwp; /* ptr to SPI h/w registers */

    spi_reset_port(hwp);
    spi_set_master_or_slave(hwp,cfgp->master_slave);
    spi_select_pins(cfgp->pins);
    spi_set_baudrate(hwp,cfgp->speed);
    spi_set_clock_format(hwp,cfgp->cpol,cfgp->cpha);
    spi_set_buswidth(hwp,cfgp->width);
    spi_set_slave_select_logic(hwp,cfgp->ss,cfgp->master_slave);
    spi_set_bidirectional(hwp,cfgp->pinmode);

    spi_enable(chan);
}

/**
 * clear SPI control registers (SPICR1 and SPICR2) to give us a clean sheet
 * upon which to the build configuration
 *
 * @param       hwp       Pointer to SPI channel registers
 */
PRIVATE void spi_reset_port(struct spi_regs_s *hwp)
{
    hwp->cr1 = 0u;
    hwp->cr2 = 0u;
}

/**
 * select the output pins to be used by the SPI port
 *
 * @param pins  pin selection (NB enumeration values chosen so that they can
 *              be OR'ed directly into #MODRR)
 *
 * @note assumes relevant MODRR bits are clear prior to call
 */
PRIVATE void spi_select_pins(enum spi_pins_e pins)
{
    MODRR |= (uint8_t) pins;
}

/**
 * set SPI port as master or slave
 *
 * @param hwp           Pointer to SPI channel registers
 * @param master_slave  enumeration value chosen to be OR'ed directly into
 *                      SPICR1
 * @note assumes SPICR1 has been cleared by spi_reset_port() prior to call
 */
PRIVATE void spi_set_master_or_slave(struct spi_regs_s *hwp,
                                        enum spi_ms_e master_slave)
{
    hwp->cr1 |= (uint8_t) master_slave;
}

/**
 * @param hwp           Pointer to SPI channel registers
 * @param speed         baud rate (NB enumeration values chosen so that they can
 *                      be written directly to register)
 */
PRIVATE void spi_set_baudrate(struct spi_regs_s *hwp,enum spi_spd_e speed)
{
    hwp->br = (uint8_t) speed ;
}

/**
 * Set SPI channel clock phase and polarity
 *
 * @param hwp   pointer to SPI channel registers
 * @param cpol  clock polarity
 * @param cpha  clock phase
 *
 * NB enumeration values are both chosen so that they can be written directly
 * to the register.
 * @note assumes SPICR1 has been cleared by spi_reset_port() prior to call
 */
PRIVATE void spi_set_clock_format(struct spi_regs_s *hwp,enum spi_cpol_e cpol,
        enum spi_cpha_e cpha)
{
    hwp->cr1 |= (uint8_t) cpol | (uint8_t) cpha ;
}

/**
 * Set up SPI channel for 8 or 16bit transfers
 * @param hwp           Pointer to SPI channel registers
 * @param width         baud rate (NB enumeration values chosen so that they can
 *                      be written directly to register)
 * @note assumes SPICR2 has been cleared by spi_reset_port() prior to call
 */
PRIVATE void spi_set_buswidth(struct spi_regs_s *hwp,enum spi_width_e width)
{
    hwp->cr2 |= (uint8_t) width;
}

/**
 * Set up the slave select logic for a SPI port by setting the MODFEN and SSOE
 * bits.
 *
 * Logic for bus masters is as follows:
 * MODFEN | SSOE | effect
 * :----: | :--: | :-----
 *   0    |  x   | SS not used by SPI
 *   1    |  0   | SS used for mode fault detection
 *   1    |  1   | SS driven SPI
 *
 * We don't use mode-fault detection so will set both MODFEN and SSOE if we want
 * automatic control of slave select (ss_mode == Spi_ss_auto) or will leave
 * MODFEN clear if we drive SS manually through digital port
 * (ss_mode = Spi_ss_explicit).
 * MODFEN and SSOE are ignored for bus slaves so we leave them clear.
 * @note assumes SPICR1 and SPICR2 have been cleared by spi_reset_port() prior
 * to call.
 */
PRIVATE void spi_set_slave_select_logic(struct spi_regs_s *hwp,
                enum spi_ss_e ss_mode, enum spi_ms_e master_slave)
{
    if ((master_slave == Spi_ms_master) && (ss_mode == Spi_ss_auto))
    {
        /* set both MODFEN and SSOE */
        hwp->cr1 |= SPICR1_SSOE;
        hwp->cr2 |= SPICR2_MODFEN;
    }
    else
    {
        /* leave SSOE and MODFEN clear */
    }
}

/**
 * Set bidirectional or normal pin mode
 *
 * Logic for pin mode in Master mode is as follows:
 * Pin mode | SPC0 | BIDIROE | MISO | MOSI
 * :-------:|:----:|:-------:|:----:|:---
 * Normal   | 0    | X       | Master In| Master Out
 * Bidirectional| 1| 0       | MISO not used by SPI| Master In
 * Bidirectional| 1| 1       | MISO not used by SPI| Master I/O
 *
 * Logic for pin mode in Slave mode is as follows:
 * Pin mode | SPC0 | BIDIROE | MISO | MOSI
 * :-------:|:----:|:-------:|:----:|:---
 * Normal   | 0    | X       | Slave Out| Slave In
 * Bidirectional| 1| 0       | Slave In | MOSI not used by SPI
 * Bidirectional| 1| 1       | Slave I/O| MOSI not used by SPI
 *
 * Bidirectional mode is intended to be used only when there's only MO or MI
 * communication.
 */
PRIVATE void spi_set_bidirectional(struct spi_regs_s *hwp, enum spi_pinmode_e mode)
{
    if(mode == Spi_pinmode_bidirectional)
    {
        hwp->cr2 |= SPICR2_SPC0;
    }
    else
    {
        /* leave SPC0 and BIDIROE clear */
    }
}
