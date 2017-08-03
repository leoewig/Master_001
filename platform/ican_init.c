/**
 * @file
 * @ingroup ican
 *
 * Routines for initialising and controlling the CAN bus for both the
 * bootblock and the application.
 *
 * Conditional compilation
 * =======================
 * `ifdef BBLK` 
 * ------------
 * The bootblock only uses `ican_init()` and its helpers so all other
 * functions are contained within an ifndef BBLK ... endif compilation switch
 *
 * `ifdef EVB_4MHZ`
 * -----------------
 * The BMS crystal is 8MHz but an unmodified evaluation board (EVB) is only
 * 4MHz.  Therefore we have a compilation switch for setting CAN bus timing
 * parameters.  A particular annoyance is that the EVB's 4MHz crystal is too
 * slow to clock a 1Mbaud bus, so we have to use the bus clock, which isn't as
 * good according to the manual.
 *
 */

#include "basic_types.h"
#include "hardware.h"
#include "ican.h"
#include "ican_p.h"

/*
 * HW register definitions
 * With an 8MHz crystal and no prescalar, we want
 * 	1+Tseg1+Tseg2 = 8 cycles (1MBaud) or 16 cycles (500kbaud)
 * NB The value in the register is 1 less than the effective value
 */
#define	CTL0_INIT		0x01u	/**< CANCTL0 INITRQ request */
#define	CTL1_INITAK		0x01u	/**< CANCTL1 INITAK
					   (init request acknowledgement) */
#define CTL1_ENABLE		0x80u	/**< CAN enable */

#ifdef EVB_4MHZ
/* unmodified EVB hardware */
#define	CTL1_USE_BUSCLK		0x40u	/**< CLKSRC=1 to use bus clock */

#define CTL1_CLKSRC_SETTING     CTL1_USE_BUSCLK
				/**< 4MHz EVB uses the BUS clock not crystal */
#define BTR0_1MBAUD		0x44u	/**< SJW=2 cycles, prescalar = 5 */
#define BTR1_1MBAUD		0x34u	/**< Tseg2=4 cycles, Tseq1=5 cycles */
#define BTR0_500KBAUD		0x40u	/**< SJW=2 cycles, prescalar = 1 */
#define BTR1_500KBAUD		0x14u	/**< Tseg2=2 cycles, Tseq1=5 cycles */
#else
/* BMS hardware or modified EVB */
#define CTL1_CLKSRC_SETTING     0u /**< BMS uses crystal instead of bus clock */
#define BTR0_1MBAUD		0x40u	/**< SJW=2 cycles, prescalar = 1 */
#define BTR1_1MBAUD		0x14u	/**< Tseg2=2 cycles, Tseq1=5 cycles */
//#define BTR0_500KBAUD		0x80u	/**< SJW=3 cycles, prescalar = 1 */
//#define BTR1_500KBAUD		0x67u	/**< Tseg2=7 cycles, Tseq1=6 cycles */

#define BTR0_500KBAUD		0x81u	/**< SJW=3 cycles, prescalar = 1 */
#define BTR1_500KBAUD		0x67u	/**< Tseg2=7 cycles, Tseq1=6 cycles */
#endif /* EVB or BMS hardware */

/*
 * public data
 */
/**
 * structure of flags showing whether error is currently present.  These are
 * set and cleared by the driver using the macros #ICAN_SET_ERROR and
 * #ICAN_CLEAR_ERROR_PRESENT.  They can be tested in both driver and
 * application with #ICAN_ERROR_IS_PRESENT.
 *
 * @note ican_rx_hw_buffer_overflow will never be set in this structure because
 * it is a tranisent error.  It will only be found set in #ican_errors_latched
 */
struct ican_errors_s ican_errors_present[Can_n_channels]; /* will be
                                 initialised to all zeros (i.e. all FALSE) */
/**
 * structure of flags showing whether error has ever been present.  These are
 * _set only_ by the driver (with #ICAN_SET_ERROR) and cleared by the
 * application using the macro #ICAN_CLEAR_LATCH_IF_ERROR_NOT_PRESENT.
 * They can be tested with #ICAN_ERROR_IS_LATCHED.
 */
struct ican_errors_s ican_errors_latched[Can_n_channels]; /* will be
                                 initialised to all zeros (i.e. all FALSE) */

void ican_init(void)
{
    enum can_handle_e i;


    //for (i = (enum can_handle_e) 0 ; i < Can_n_channels ; i++)
    //{
    // ican_init_channel(i);
    //}

	ican_init_channel(0u);
}

/**
 * Initialises and enables a single CAN channel
 * @param       chnl    CAN channel handle, an index into the #ican_hw array
 */
PRIVATE void ican_init_channel(enum can_handle_e chnl)
{
    /* get a pointer to the hardware registers for the controller */
    volatile struct can_controller_s *hwp = ican_hw[chnl].hwp;

    /*
     * TEST_HOOK call to tell hosted test system which controller we are
     * configuring.  This will allow TEST_HOOKs below to set or clear
     * INITAK in accordance with INITRQ
     */
    TEST_HOOK(ican_hw[chnl].physical_channel_number);

    ican_chnl_start_init(hwp);
    ican_chnl_enable_and_set_clock_source(hwp);
    ican_chnl_set_speed(hwp,ican_speed[chnl]);
    ican_chnl_set_filters(hwp,&ican_filter_cfg[chnl]);
    ican_chnl_exit_init_mode(hwp);
#ifndef BBLK
    ican_chnl_enable_interrupts(hwp);
#endif
}

/**
 * Puts CAN controller into intialisation mode and sets control registers
 * to a known blank state so that subsequent functions can OR into them
 * @param       hwp     Pointer to CAN controller registers
 */
PRIVATE void ican_chnl_start_init(
                                volatile struct can_controller_s *hwp)
{
    hwp->CANCTL0 = CTL0_INIT;
    while (!(hwp->CANCTL1 & CTL1_INITAK))
    {
        /*
         * Wait for initialisation.
         * If it never comes, the COP or watchdog will reset us, but
         * note that there is no known reason for this to happen
         */
        TEST_HOOK(0);
    }
}

/**
 * exits initialisation (so controller is now running)
 * @param       hwp     Pointer to CAN controller registers
 */
PRIVATE void ican_chnl_exit_init_mode(
                                volatile struct can_controller_s *hwp)
{
    hwp->CANCTL0 = 0u;		    /* clear INITRQ */
    while (hwp->CANCTL1 & CTL1_INITAK)
    {
        /*
         * Wait for exit from initialisation mode.
         * If it never comes, the COP or watchdog will reset us, but
         * note that there is no known reason for this to happen
         */
        TEST_HOOK(0);
    }
}

/**
 * Enables the CAN channel and selects between bus clock or oscillator.
 *
 * The channel will not actually run until it is brought out of initialisation
 * mode by calling ican_chnl_exit_init_mode()
 *
 * Oscillator is preferred as the clock source but may not be an option if too
 * slow for the desired bus frequency.
 * @param       hwp     Pointer to CAN controller registers
 *
 * @note Setting the clock source and enabling are combined into a single
 * function because the CANE bit of CANCTL1 is described as 'write once'.  If
 * you do write it more than once, you might find that the CAN controller will
 * not come out of initialisation mode when you clear INITRQ.  Oddly, and
 * somewhat annoyingly, the problem only seems to show itself when the
 * debugger is _not_ attached!
 */
PRIVATE void ican_chnl_enable_and_set_clock_source(
                                volatile struct can_controller_s *hwp)
{
    /*
     * Writing to CTL1_INITAK is ignored: it will stay set regardless because
     * we will be in initialisation mode.  We choose to write a 1 to it
     * for the benefit of unit testing
     */
    hwp->CANCTL1 = (uint8_t) (CTL1_ENABLE | CTL1_CLKSRC_SETTING | CTL1_INITAK);
}

/**
 * Set CAN bus baudrate
 *
 * @param       hwp     Pointer to CAN controller registers
 * @param       speed   Desired baudrate for the bus
 */
PRIVATE void ican_chnl_set_speed(volatile struct can_controller_s *hwp,
                                        enum icanspeeds_e speed)
{
    switch (speed)
    {
        case Canspeed_500:
            /* 500 kbit/s */
            hwp->CANBTR0 = BTR0_500KBAUD;
            hwp->CANBTR1 = BTR1_500KBAUD;
            break;
        case Canspeed_1000:
            /* 1000 kbit/s */
            hwp->CANBTR0 = BTR0_1MBAUD;
            hwp->CANBTR1 = BTR1_1MBAUD;
            break;
        default:
            /* 
             * error in our configuration file, or we added a speed and
             * forgot to add it here...
             */
            INTERNAL_ERROR();
            break;
    }
}


/**
 * Set CAN bus filtering
 *
 * @param       hwp      Pointer to CAN controller registers
 * @param       filterp  Structure containing desired filter type with
 *                       mask and acceptance registers
 */
PRIVATE void ican_chnl_set_filters(
                            volatile struct can_controller_s *hwp,
                            CONST struct ican_filter_cfg_s *filterp)
{
    /*
     * set identifier acceptance control register, which controls how
     * the acceptance and mask registers are interpreted
     */
    hwp->CANIDAC = (uint8_t) filterp->filter_type;

    /* the identifier acceptance registers */
    hwp->CANIDAR01 = filterp->acceptance.w01;
    hwp->CANIDAR23 = filterp->acceptance.w23;
    hwp->CANIDAR45 = filterp->acceptance.w45;
    hwp->CANIDAR67 = filterp->acceptance.w67;

    /* the identifier acceptance mask */
    hwp->CANIDMR01 = filterp->mask.w01;
    hwp->CANIDMR23 = filterp->mask.w23;
    hwp->CANIDMR45 = filterp->mask.w45;
    hwp->CANIDMR67 = filterp->mask.w67;
}

#ifndef BBLK
/**
 * Enable the following interrupts for a CAN channel
 * - Message received (RXF)
 * - All status changes for transmit and receive
 * - Receiver detects hardware overrun (where a message had to be discarded
 *   because the receive buffers were all full)
 *
 * Transmit interrupts are disabled.
 *
 * @param       hwp      Pointer to CAN controller registers
 */
PRIVATE void ican_chnl_enable_interrupts(
                                volatile struct can_controller_s *hwp)
{
    hwp->CANRIER = CAN_CSC | CAN_TSTATE_MASK | CAN_RSTATE_MASK
                   | CAN_OVR | CAN_RXF;
    hwp->CANTIER = 0u;
}
#endif
