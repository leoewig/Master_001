/**
 * @file
 *
 * S12 interface to XGATE
 *
 * $Id: gate.c 9626 2013-07-22 14:04:38Z tg10 $
 */
/**
 * @ingroup s12_xgate
 * @{
 * @file
 * @}
 */
#include "basic_types.h"
#include "hardware.h"
#include "isr.h"
#include "gate.h"

/*
 * .lkf file symbols
 */
#if !defined (_lint) && !defined(HOSTED)
/**
 * Top of low-priority stack defined in .lkf file
 * (in XGATE address space)
 */
extern uint16_t xstack_lp_xg_addr;
/**
 * Top of high-priority stack defined in .lkf file
 * (in XGATE address space)
 */
extern uint16_t xstack_hp_xg_addr;
/**
 * start of xvector array
 * (in XGATE address space)
 */
extern uint16_t xvectors_xg_addr;

#else
/* For Lint purposes only, declare dummy variables for the symbols defined in
 * the linker file*/
/**
 * @lint Investigate : Symbol not accessed, these are symbols from the linker file and hence
 * cannot be changed\n
 * lint -esym(551,xstack_lp_xg_addr)\n
 * lint -esym(551,xstack_hp_xg_addr)\n
 * lint -esym(551,xvectors_xg_addr)\n
 * lint -esym(551,exbss)
 */
/*lint -esym(551,xstack_lp_xg_addr) */
/*lint -esym(551,xstack_hp_xg_addr) */
/*lint -esym(551,xvectors_xg_addr) */
/*lint -esym(551,exbss) */
static const uint16_t xstack_lp_xg_addr = 0u;
static const uint16_t xstack_hp_xg_addr = 0u;
static const uint16_t xvectors_xg_addr  = 0u;
#endif

/**
 * enum of options for the XGATE Initial Stack Pointer Select Register
 *
 * Values chosen to writable straight into XGISPSEL
 */
enum xgispsel_e {
    Xgispsel_xgvbr = 0u,	/**< Map the Vector base register */
    Xgispsel_xgisp31 = 1u,	/**< Map the low-priority thread stack */
    Xgispsel_xgisp74 = 2u	/**< Map the high-priority thread stack */
};


FRAM uint16_t xg_unhandled_chnl; /* set by XGATE unexpected interrupt handler
				   to the channel causing the interrupt.
				   Defined here so that non-XGATE applications
				   can be linked */

FRAM uint16_t xg_internal_error_pc;/* set by XGATE before triggering the
                                      isr_xgate_internal_error on the S12
				   Defined here so that non-XGATE applications
				   can be linked */

/* documentated in header */
void gate_init(void)
{
    if ((XGCHID == 0u) && ((XGMCTL & XGE) == 0u))
    {
        /**
         * @lint Investigate : Casting addresses to an unsigned integers\n
         * lint --e{923} (warning inhibited within this code block)
         */
        /*lint --e{923} */

	/*
	 * new events are disabled (XGMCTL.XGE == 0) and no thread is running
	 * (XGCHID == 0) so we can initialise base vector and stack pointers
	 *
	 * XGVBR, XGISP31 and XGISP74 are all mapped to the same address so
	 * we have to map each one in turn (via XGISPSEL) before writing them
	 */
            /* map the vector base register */
	XGISPSEL = (uint8_t) Xgispsel_xgvbr;
            /* point to the vector array */
	XGVBR = (uint16_t) &xvectors_xg_addr;

            /* map the initial low priority stack ptr*/
	XGISPSEL = (uint8_t) Xgispsel_xgisp31;
            /* point to the top of the low priority stack */
	XGISP31 = (uint16_t) &xstack_lp_xg_addr;

            /* map the initial high priority stack p */
	XGISPSEL = (uint8_t) Xgispsel_xgisp74;
	    /* point to the top of the high priority stack */
	XGISP74 = (uint16_t) &xstack_hp_xg_addr;

        /** @todo initialise the XGATE stack with a pattern */
    }

    /*
     * Changing bits in XGMCTL requires a corresponding mask bit to be written
     * at the same time.
     *
     * Set the following:
     * XGE:	Enable interrupt requests to the XGATE
     * XGIE:	Enable interrupt requests from the XGATE to the S12
     * XGFRZ:	XGATE stops when the S12 is in BDM mode
     * XGSWEF:	Clear any software errors (XGSWEF is set-to-clear)
     * And clear:
     * XGFACT:	Don't signal that XGATE is always active
     */
    XGMCTL = (XGEM | XGE) | (XGFRZM | XGFRZ) | (XGIEM | XGIE)
		| (XGSWEFM | XGSWEF) | XGFACTM;
}

/* documentated in header */
void gate_stop(void)
{
    do {
	/* clear XGE, XGIE and XGATE software error if set */
	XGMCTL = XGEM | XGIEM | (XGSWEFM | XGSWEF) ;
    } while (XGCHID != 0u);
}

/* documented in header */
void gate_init_maskable_error_interrupts(void)
{
    /*
     * Enable isr_xgate_unexpected_interrupt() and isr_xgate_internal_error()
     * with the maximum priority.
     * Must be specified as XGATE interrupts to enable execution on S12
     * via the SIF mechanism!
     * S12 offsets are twice the XGATE channel number, hence the *2
     */
    (void) set_interrupt_level(CHANNEL_FOR_UNHANDLED_XGATE_INTERRUPT*2u,
	    INT_LVL_MAX, INT_XGATE_DEST);
    (void) set_interrupt_level(CHANNEL_FOR_XGATE_INTERNAL_ERROR*2u,
	    INT_LVL_MAX, INT_XGATE_DEST);
}

/* documented in header */
/**
 * @todo Add DTC setting
 */
INTERRUPT void isr_xgate_unexpected_interrupt(void)
{
    uint8_t offset;

    offset = (uint8_t) (xg_unhandled_chnl * 2u); /* vector address offsets are
					    twice the XGATE channel number */
    /** @polyspace<MISRA-C:2.1:Not a defect:Justify with annotations> For test/debug purposes */
    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test/debug purposes */
    BREAK_IF_DEBUGGING;
    /*
     * The XGATE received an interrupt you weren't expecting.  Lookup the
     * S12 offset (offset) or XGATE channel (xg_unhandled_chnl) to find out
     * which interrupt it was.  Use Table 1-14 Interrupt Vector Locations in
     * the S12X manual.
     */
    sch_isr_Unhandled(offset);

    /* finally, acknowledge the XGATE interrupt */
    CLEAR_XGIF_FOR_CHANNEL(CHANNEL_FOR_UNHANDLED_XGATE_INTERRUPT);
}

/* documented in header */
/**
 * @todo Add DTC setting
 */
INTERRUPT void isr_xgate_internal_error(void)
{
    /**
     * @lint Investigate : Symbol not accessed\n
     * lint !e550
     */
    /*lint -esym(550,xgate_pc) */
    uint16_t xgate_pc;

    xgate_pc = xg_internal_error_pc; /* Program counter when INTERNAL_ERROR()
                                        invoked */
    /** @polyspace<MISRA-C:2.1:Not a defect:Justify with annotations> For test/debug purposes */
    /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test/debug purposes */
    BREAK_IF_DEBUGGING;

    /*
     * if you're reading this comment from the debugger, it's bad news, I'm
     * afraid.  XGATE code has called the INTERNAL_ERROR macro.  To find out
     * where, grep the absolute listing files (*.la) for the hex address in
     * xg_internal_error_pc.  It should point to a "tfr R2,PC" instruction.
     * Then open up the listing to see the C source line it corresponds to.
     */
    /* finally, acknowledge the XGATE interrupt */
    CLEAR_XGIF_FOR_CHANNEL(CHANNEL_FOR_XGATE_INTERNAL_ERROR);
}
