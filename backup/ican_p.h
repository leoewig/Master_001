/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   ican_p.h
 *
 * description:   
 *  Private header for Internal CAN driver
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(ICAN_P_H)
#define      ICAN_P_H
/**
 *
 * Private header for Internal CAN driver
 *
 * @file
 * @ingroup ican
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */
#include "ican.h"

/**
 * lowest possible priority for a message as stored in
 * can_controller_s::CANRXFG.priority
 */
#define         CAN_MINIMUM_PRIORITY    255u

/*
 * register bits required by more than one file
 */
#define         CAN_RXF         1u /**< mask for CANRFLG.RXF/CANRIER.RXFIE 
                                        (message recieved) */
#define         CAN_OVR         2u /**< mask for CANRFLG.OVRIF/CANRIER.OVRIE
                                        (receive overrun detected) */
#define         CAN_TSTATE_MASK      0x0cu /**< mask for CARNRFLG.TSTAT[01] and
                                CANRIER.TSTATE[01] (transmitter status bits) */
#define         CAN_RSTATE_MASK      0x30u /**< mask for CARNRFLG.RSTAT[01] and
                                CANRIER.RSTATE[01] (receiver status bits) */
#define         CAN_CSC         0x40u /**< mask for CANRFLG.CSCIF/CANRIER.CSCIE
                                        (CAN status change) */

#define         CAN_TSTATE_IS_BUSOFF    CAN_TSTATE_MASK  /**< if both TSTAT bits
                                        are set in CANRFLG then controller is
                                        bus-off */
#define         CAN_TSTATE_WARNING      0x04u   /**< CANRFLG.TSTAT = 01,
                                                indicating tx warning */
#define         CAN_TSTATE_ERROR        0x08u   /**< CANRFLG.TSTAT = 10,
                                                indicating tx error */
#define         CAN_RSTATE_WARNING      0x10u   /**< CANRFLG.RSTAT = 01,
                                                indicating rx warning */
#define         CAN_RSTATE_ERROR        0x20u   /**< CANRFLG.RSTAT = 10,
                                                indicating rx error */
#define         CAN_BUSOFF_INTERRUPT_ENABLE     4u      /***< Setting for
                                        CANRIER.TSTATE[01] to enable only
                                        bus-off entry/exit state-change 
                                        interrupts */

/*
 * h/w access macros
 */
/**
 * Test for the message receive flag
 */
#define         CAN_MESSAGE_RECEIVED(hwp)       ((hwp)->CANRFLG & CAN_RXF)

/**
 * Free a receive buffer by clearing the RXF bit.
 * RXF is set-to-clear
 */
#define         CAN_RELEASE_RX_BUFFER(hwp)      do { \
                                                    (hwp)->CANRFLG = CAN_RXF; \
                                                } while (0)

/**
 * Test whether any of the transmit buffers have a message to send
 */
#define         CAN_TRANSMIT_PENDING(hwp)  \
                                            (((hwp)->CANTFLG & 0x7u) != 0x7u)
                                        
enum icanspeeds_e {
    Canspeed_500, /**< 500 kbit/s */
    Canspeed_1000 /**< 1 Mbit/s */
} ; /**< supported bus speeds */

enum ican_filter_type_e {
    Canfilter_2_32bit = 0u,
    Canfilter_4_16bit = 0x10u,
    Canfilter_8_8bit = 0x20u,
    Canfilter_closed = 0x30u
} ; /**< supported filtering modes for CAN controllers
      (values equal to setting of CANIDAC register) */

struct ican_filter_regs_s {
    uint16_t w01;
    uint16_t w23;
    uint16_t w45;
    uint16_t w67;
} ; /**< structure reflecting layout of CANIDAR and CANIDMR acceptance and
         mask registers */

struct ican_filter_cfg_s {
        enum ican_filter_type_e filter_type;
        struct ican_filter_regs_s acceptance;   /**< CANIDAR 0,1 2,3 4,5 6,7 */
        struct ican_filter_regs_s mask;         /**< CANIDMR 0,1 2,3 4,5 6,7 */
}; /**< structure to contain filter configuration for a bus */

/**
 * C structure which maps preceisely over the CAN h/w registers
 */
struct can_controller_s {
    uint8_t CANCTL0;    /**< control register 0 */
    uint8_t CANCTL1;    /**< control register 1 */
    uint8_t CANBTR0;    /**< bus timing register 0 */
    uint8_t CANBTR1;    /**< bus timing register 1 */
    uint8_t CANRFLG;    /**< receiver flag register */
    uint8_t CANRIER;    /**< receiver interrupt reg */
    uint8_t CANTFLG;    /**< transmitter flag reg */
    uint8_t CANTIER;    /**< transmitter control reg */
    uint8_t CANTARQ;    /**< transmitter abort req. */
    uint8_t CANTAAK;    /**< transmitter abort ack. */
    uint8_t CANTBSEL;   /**< transmit buffer selection */
    uint8_t CANIDAC;    /**< identifier acceptance */
    uint8_t reserved;   /**< not used */
    uint8_t CANMISC;    /**< micellaneous register */
    uint8_t CANRXERR;   /**< receive error counter */
    uint8_t CANTXERR;   /**< transmit error counter */
    uint16_t CANIDAR01; /**< id acceptance regs 0 and 1 */
    uint16_t CANIDAR23; /**< id acceptance reg 2 and 3 */
    uint16_t CANIDMR01; /**< id mask register 0 and 1 */
    uint16_t CANIDMR23; /**< id mask register 2 and 3*/
    uint16_t CANIDAR45; /**< id acceptance reg 4 and 5 */
    uint16_t CANIDAR67; /**< id acceptance reg 6 and 7 */
    uint16_t CANIDMR45; /**< id mask register 4 and 5 */
    uint16_t CANIDMR67; /**< id mask register 6 and 7 */
    /**
     * layout of transmit/receive buffers allowing 16-bit access to identifiers
     */
    struct canbuf_s {
        uint16_t id01;                  /**< ID0, ID1 */
        uint16_t id23;                  /**< ID2, ID3 */
        uint8_t data[CAN_MAX_DATA];     /**< message contents */
        uint8_t dlr;                    /**< data length register */
        uint8_t priority;               /**< transmission priority */
        uint16_t timestamp;             /**< optional timestamp */
    } CANRXFG;
    struct canbuf_s CANTXFG;
};
#if !defined POLYSPACE && !defined HOSTED
/**
 * @lint
 * Definition of CAN registers leads to multiple copies of same variable being
 * declared.  Lint doesn't like this but Cosmic doesn't give us a good
 * alternative:\n
 * -esym(14,ican_channel0_regs,ican_channel1_regs)\n
 * -esym(14,ican_channel2_regs,ican_channel3_regs)
 */
/*lint -esym(14,ican_channel0_regs,ican_channel1_regs) */
/*lint -esym(14,ican_channel2_regs,ican_channel3_regs) */
volatile struct can_controller_s ican_channel0_regs @&CAN0CTL0;
volatile struct can_controller_s ican_channel1_regs @&CAN1CTL0;
volatile struct can_controller_s ican_channel2_regs @&CAN2CTL0;
volatile struct can_controller_s ican_channel3_regs @&CAN3CTL0;
#else
extern volatile struct can_controller_s ican_channel0_regs;
extern volatile struct can_controller_s ican_channel1_regs;
extern volatile struct can_controller_s ican_channel2_regs;
extern volatile struct can_controller_s ican_channel3_regs;
#endif

extern CONST struct ican_hw_s {
    uint8_t physical_channel_number ;
    volatile struct can_controller_s *hwp ;
} ican_hw[Can_n_channels] ;

extern enum icanspeeds_e ican_speed[Can_n_channels] ;

extern struct ican_rxtxq_s ican_txq[Can_n_channels] ;

extern struct ican_rxtxq_s ican_s12_rxq[Can_n_channels] ;

extern struct ican_rxtxq_s ican_xgate_rxq[Can_n_channels] ;

extern struct ican_filter_cfg_s ican_filter_cfg[Can_n_channels] ;

/*
 * functions shared between files in the ICAN feature (i.e. not private)
 */
void ican_tx_from_isr(volatile struct can_controller_s *hwp,
                    struct ican_rxtxq_s *rxqp);
void ican_error_from_isr(volatile struct can_controller_s *hwp,
                    enum can_handle_e chnl);

/**
 * sets up bus speed, filtering and interrupts and then enables each bus
 */
void ican_init(void);

/*
 * PRIVATE function prototypes (documented in source)
 */

PRIVATE  void ican_init_channel(enum can_handle_e chnl);
PRIVATE  void ican_chnl_enable_interrupts(
                                volatile struct can_controller_s *hwp);
PRIVATE  void ican_chnl_start_init(
                                volatile struct can_controller_s *hwp);
PRIVATE  void ican_chnl_exit_init_mode(
                                volatile struct can_controller_s *hwp);
PRIVATE  void ican_chnl_enable_and_set_clock_source(
                                volatile struct can_controller_s *hwp);
PRIVATE  void ican_chnl_set_speed(
                volatile struct can_controller_s *hwp,enum icanspeeds_e speed);
PRIVATE  void ican_chnl_set_filters(
                            volatile struct can_controller_s *hwp,
                            CONST struct ican_filter_cfg_s *filterp);

#endif      /* ICAN_P_H */

