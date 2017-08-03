/**
 * @file
 * @ingroup spi
 *
 * Definitions for SPI bus master transfers
 *
 * $Id: spimaster.c 9774 2013-08-01 08:31:44Z tg10 $
 */
/* Copyright (c) Linkedriving Tech 2012 */
#include "basic_types.h"
#include "hardware.h"
#include "spi.h"
#include "spi_p.h"

/*
 * use macros for function names so that we can keep them different on XGATE
 * and S12 cores
 */
#if defined _XGATE_
#define SPI_TRANSFER                    (xspi_transfer)
#define SPI_TRANSFER_PARTIAL            (xspi_transfer_partial)
#define SPI_TRANSFER_INTERNAL           (xspi_transfer_internal)
#define SPI_TRANSFER16                  (xspi_transfer16)
#else
#define SPI_TRANSFER                    (spi_transfer)
#define SPI_TRANSFER_PARTIAL            (spi_transfer_partial)
#define SPI_TRANSFER_INTERNAL           (spi_transfer_internal)
#define SPI_TRANSFER16                  (spi_transfer16)
#endif

/**
 * Wait for space in the transmit buffer
 *
 * @param SR spi status register (SPIxSR) for the SPI port
 * @param CR1 spi control register (SPIxCR1) for the SPI port
 *
 * This loop _will_ terminate provided the SPI port is enabled so no
 * watchdog or timeout protection other than to throw an internal error
 * if it turns out we have to wait but spi _isn't_ enabled.
 */
#define WAIT_FOR_TRANSMIT_READY(SR,CR1) \
                            do { \
                                while (((SR) & TRANSMIT_EMPTY_FLAG) == 0u) \
                                { \
                                  /* since we've been made to wait */ \
                                  /* make sure we really are enabled */ \
                                  ASSERT((CR1) & SPICR1_ENABLE); \
                                } \
                            } while (0)
/**
 * Wait for a byte to be received
 *
 * @param SR spi status register (SPIxSR) for the SPI port
 * @param CR1 spi control register (SPIxCR1) for the SPI port
 *
 * This loop will terminate provided the SPI port hasn't been mysteriously
 * disabled so only timeout protection is to throw an INTERNAL_ERROR if
 * we have to wait and find that controller isn't enabled.
 */
#define WAIT_FOR_DATA_RECEIVED(SR,CR1)  \
                            do { \
                                while (((SR) & DATA_RECEIVED_FLAG) == 0u) \
                                { \
                                  /* since we've been made to wait */ \
                                  /* make sure we really are enabled */ \
                                  ASSERT((CR1) & SPICR1_ENABLE); \
                                } \
                            } while (0)

/*
 * static functions
 */
/**
 * Perform a 16 bit SPI transfer
 *
 * All parameters as for spi_transfer()/xspi_transfer()
 *
 * Slave select logic will be performed by the caller, either spi_transfer()
 * or xspi_transfer()
 */
PRIVATE void SPI_TRANSFER16(enum spi_channel_e channel, FPTR uint8_t *readp,
                    FPTR const uint8_t *writep, uint16_t nbytes);

/**
 * function called by both spi_transfer() and spi_transfer_partial() to do the
 * work
 *
 * All arguments are as for caller apart from first flag which indicates whether
 * a partial transfer or not
 */
PRIVATE void SPI_TRANSFER_INTERNAL(bool_t is_partial,
        enum spi_channel_e channel,
        FPTR uint8_t *readp, FPTR const uint8_t *writep, uint16_t nbytes);

void SPI_TRANSFER(enum spi_channel_e channel, FPTR uint8_t *readp,
                    FPTR const uint8_t *writep, uint16_t nbytes)
{
    SPI_TRANSFER_INTERNAL(FALSE, channel, readp, writep, nbytes);
}


void SPI_TRANSFER_PARTIAL(enum spi_channel_e channel, FPTR uint8_t *readp,
                    FPTR const uint8_t *writep, uint16_t nbytes)
{
    SPI_TRANSFER_INTERNAL(TRUE, channel, readp, writep, nbytes);
}


PRIVATE void SPI_TRANSFER_INTERNAL(bool_t is_partial,
        enum spi_channel_e channel,
        FPTR uint8_t *readp, FPTR const uint8_t *writep, uint16_t nbytes)
{
    FROM struct spi_cfg_s *cfg = &spi_cfg[channel];
    struct spi_regs_s *hwp = spi_cfg[channel].hwp;
    uint16_t ccr;       /* saved value of CCR/XGMCTL for save/restore
                           interrupts */
    bool_t reading; /* TRUE if input data is to be saved, FALSE if it will
                       be discarded */
    bool_t writing; /* TRUE if data to output has been supplied, FALSE if
                       dummy data should be transmitted */

    /*
     * send SS low if it is under our control
     */
    switch (cfg->ss)
    {
        case Spi_ss_explicit:
            /* protect port read-modify-write from interrupts on this core */
            CORE_SAVE_INTERRUPTS(ccr);
            *cfg->ss_port &= (uint8_t) ~cfg->ss_port_mask;
            CORE_RESTORE_INTERRUPTS(ccr);
            break;
        case Spi_ss_auto:
            if (is_partial)
            {
                /*
                 * partial transfers not allowed for auto slave-select
                 * control because the hardware will de-select SS automatically
                 */
                INTERNAL_ERROR();
                /*
                 * if debugger not attached, will do an implicit-control
                 * transfer anyway
                 */
            }
            break;
        case Spi_ss_na:
            /* do not manipulate slave-select */
            break;
        default:
            /* unknown enum member */
            INTERNAL_ERROR();
    }

        reading = (readp != FNULL);
        writing = (writep != FNULL);

    /*
     * Enable transmit buffer, if spi is set to bidirectional pinmode
     */
    if((cfg->pinmode == Spi_pinmode_bidirectional) && (writing))
    {
        hwp->cr2 |= SPICR2_BIDIROE;
    }

    if (cfg->width == Spi_width_16bit)
    {
        SPI_TRANSFER16(channel,readp,writep,nbytes);
    }
    else
    {
        while (nbytes--)
        {
            /* wait for the transmit buffer to come free */
            WAIT_FOR_TRANSMIT_READY(hwp->sr,hwp->cr1);
            /*
             * buffer empty so write the next byte,
             * or 0 if we're just reading
             */
            hwp->dr.b.l = (writing) ? *writep++ : 0u;

            /* now wait for the data to go */
            WAIT_FOR_DATA_RECEIVED(hwp->sr,hwp->cr1);
            /*
             * data received so read it or throw it away
             */
            if (reading)
            {
                *readp++ = hwp->dr.b.l; /* put into caller's buffer */
            }
            else
            {
                (void) hwp->dr.b.l;     /* throw it away */
            }
        }
    }

    /*
     * Disable transmit buffer, if spi is set to the bidirectional mode
     */
    if(cfg->pinmode == Spi_pinmode_bidirectional)
    {
        hwp->cr2 &= (uint8_t)~(uint8_t)SPICR2_BIDIROE;
    }

    /*
     * send SS high again unless this is a partial transfer
     */
    if ((cfg->ss == Spi_ss_explicit) && !is_partial)
    {
        /* protect port read-modify-write from interrupts on this core */
        CORE_SAVE_INTERRUPTS(ccr);
        *cfg->ss_port |= cfg->ss_port_mask;
        CORE_RESTORE_INTERRUPTS(ccr);
    }
}

PRIVATE void SPI_TRANSFER16(enum spi_channel_e channel, FPTR uint8_t *readp,
                    FPTR const uint8_t *writep, uint16_t nbytes)
{
    struct spi_regs_s *hwp = spi_cfg[channel].hwp;
    bool_t reading;
    bool_t writing;
    uint16_t ntransfers;
    bool_t odd_byte;

    ntransfers = nbytes/2u;
    odd_byte   = ((nbytes % 2u) == 1u);

    reading = (readp != FNULL);
    writing = (writep != FNULL);

    while (ntransfers--)
    {
        /* wait for the transmit buffer to come free */
        WAIT_FOR_TRANSMIT_READY(hwp->sr,hwp->cr1);
        /*
         * buffer empty so write the next byte,
         * or 0 if we're just reading
         */
        if (writing)
        {
            /**
             * @lint Suspicious pointer-to-pointer conversion (area too small) *
             * lint -e{826}
             * Data is written on a word-by-word basis rather than a byte-by-byte basis 
             * for efficiency
             */
            /*lint -e{826} */
            hwp->dr.w = *(FPTR const uint16_t *) writep;
            writep += 2u;
        }
        else
        {
            hwp->dr.w = 0u;
        }

        WAIT_FOR_DATA_RECEIVED(hwp->sr,hwp->cr1);
        /*
         * data received so read it or throuw it away
         */
        if (reading)
        {
            /**
             * @lint Suspicious pointer-to-pointer conversion (area too small) *
             * lint -e{826}
             * Data is written on a word-by-word basis rather than a byte-by-byte basis 
             * for efficiency
             */
            /*lint -e{826} */
            * (FPTR uint16_t *)readp = hwp->dr.w; /* put into caller's buffer*/
            readp += 2u;
        }
        else
        {
            (void) hwp->dr.w; /* throw it away */
        }
    }

    if (odd_byte)
    {
        /* got a tiresome final byte to read/write */
        /* wait for the transmit buffer to come free */
        WAIT_FOR_TRANSMIT_READY(hwp->sr,hwp->cr1);
        if (writing)
        {
            hwp->dr.b.h = * writep;
            hwp->dr.b.l = 0u;           /* send 0 as the last byte */
        }
        WAIT_FOR_DATA_RECEIVED(hwp->sr,hwp->cr1);
        if (reading)
        {
            *readp = hwp->dr.b.h;
        }
        (void) hwp->dr.b.l; /* throw away the last byte and free the buffer */
    }
}
