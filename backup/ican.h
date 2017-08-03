/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    ican.h
 *
 * description:  Header file for internal CAN bus driver
 * 
 * 
 *  
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial Version
 *                       
 *
 *****************************************************************************/
/**
 * @file
 * @ingroup ican
 * Header file for internal CAN bus driver
 *
 */
#ifndef ICAN_H
#define ICAN_H
#include "hardware.h"   /* for the addresses of the CAN h/w registers */
#include "ican_cfg.h"    /* for the can_handle_e enumeration */
/**
 * @defgroup ican ICAN Internal CAN bus driver
 * @ingroup platform
 *
 * Internal CAN Bus Driver
 *
 * Introduction
 * ------------
 *
 * This module controls the CAN hardware for multiple CAN controllers.  It
 * is intended to be flexible and lightweight but does _not_ try to
 * do everything for you and is not integrated with a CAN message database.
 * Message receipt in particular will require extensive hand-coding.
 *
 * Every application using this feature will need a customisation module
 * to configure each CAN bus to be controlled by the feature and to perform
 * application-specific tasks such as failure management.
 * See [MMC ICAN Customisation](group__mmc_ican.html) for an example.  All
 * configuration is performed by the customisation module.
 *
 * Approach used
 * -------------
 *
 * The software defines one transmit and two receive circular buffers for each
 * configured CAN controller.  These buffers form the interface between
 * application code and the interrupt service routines which interact with the
 * hardware.  The size of each buffer is configurable and should depend on the
 * bus speed, the bus loading and the frequency with which the application
 * writes or reads the buffers.
 *
 * Data integrity is ensured by the design requirement that only one context
 * manipulates the end of a given buffer.
 *
 * Message transmission
 * --------------------
 *
 * Messages are put in the transmit circular buffer by calling icantx().
 * icantx() copies the message data to the buffer and then enables
 * the CAN transmit interrupt.
 *
 * As soon as a h/w transmit buffer is available (which will often
 * be immediately), the CAN transmit ISR will be triggered.  The ISR copies
 * the data from the circular buffer to the hardware and initiates transmission.
 * If the circular buffer is now empty, the transmit interrupt is disabled.
 *
 * Message priority
 * ----------------
 *
 * Each CAN controller has three hardware transmit buffers which can all be
 * loaded with messages for transmission.  To ensure messages are sent out in
 * the desired order, the hardware associates an 8-bit priority with each
 * buffered message.  This driver transmits messages in the order in which
 * icantx() is called by assigning ever-decreasing priorities to each message.
 * Priorities are reset to 0 (the highest priority) whenever the transmit
 * hardware has no messages queued.  If the priority reaches 254 (almost the
 * lowest priority) then no further messages are loaded into the hardware
 * transmit buffers until all three buffers empty (and the priorities can be
 * reset).  See icantx_from_isr().
 *
 * Message reception
 * -----------------
 *
 * @note Message reception must be customised in the ICAN
 * Customisation module (e.g. [mmc_ican](group__mmc_ican.html)).
 *
 * The receive ISR examines the message to determine whether the message is
 * to be decoded by the S12 or the XGATE.  The message is then copied into a
 * receive circular buffer.  If the buffer is full, the message will be lost.
 *
 * Message decoding does not occur in the ISR: it can either be left to the
 * next loop of the application or the ISR could trigger a lower-priority task
 * to perform the decoding.
 *
 * Decoding functions process messages in the receive circular buffer using the
 * macros:
 * - #QUEUE_EMPTY to see whether there are messages to be decoded
 * - #OLDEST_MESSAGE to read the oldest message in the buffer
 * - #REMOVE_MESSAGE to discard the oldest message in the buffer
 *
 * A decoder loop for the internal bus S12 messages might look like this:
 * ~~~c
 * struct ican_rxtxq_s *rxqp = &ican_s12_rxq[Can_internal];
 * struct ican_msg_s *msgp;
 *
 * while (!QUEUE_EMPTY(rxqp))
 * {
 *     msgp = OLDEST_MESSAGE(rxqp);
 *     switch (msgp->id)
 *     {
 *         case MSG_X_ID:
 *             ... decode message X (data is at msgp->data) ...
 *         case MSG_Y_ID:
 *             ... decode message Y ...
 *         ...
 *         default:
 *             ... unknown message error? ...
 *     }
 *     REMOVE_MESSAGE(rxqp);    ... discard message now it's done with ...
 * }
 * ~~~
 *
 *
 * Configuration
 * -------------
 *
 * 1. Bus configuration is performed by an application-specific module.
 *    See [MMC ICAN Customisation](group__mmc_ican.html) for an example.
 *
 * Interrupt, XGATE and Application safety
 * ---------------------------------------
 *
 * The circular buffers provide the means of ensuring data integrity in the
 * face of multiple execution contexts because only one context will write
 * a given end of the circular buffer.  For example, S12 application code will
 * call icantx() which will add data to the buffer and then advance the 'in'
 * index.  Once 'in' has been moved, the transmit interrupt will see the
 * data and can access it via the 'out' index.
 *
 * It is a requirement of the application that for a given controller, messages
 * are only transmitted (by calling icantx()) from a single context and only
 * decoded from a single context.
 *
 * Error and Status Reporting
 * --------------------------
 *
 * The #ican_errors_present and #ican_errors_latched arrays contain
 * #ican_errors_s structures for each configured controller.  The ican driver
 * sets the corresponding flag in _both_ ican_errors_present and
 * ican_errors_latched when a fault is detected.  If the fault goes away,
 * only the flag in ican_errors_present is cleared.  It is up to the application
 * to clear the ican_errors_latched flags as required.
 *
 * Requirements Trace
 * ==================
 *
 * Requirements tracing is covered in the ICAN application customisation
 * feature, e.g. [MMC Ican](group__mmc__ican.html)
 *
 */

#define CAN_MAX_DATA        8u  /**< max length of a CAN message */
#define STD_ID_SHIFT        5u  /**< shift of 11-bit identifier in
                       CAN buffer */
#define EXACT_MATCH_16BIT_MASK  0x0007u /**< mask for an exact match filter
                       (lowest 3bits must be set to do not
                        care) */
#define WIDE_OPEN_16BIT_MASK    0xffffu /**< mask to allow all messages in */

/*
 * data structures
 */
/**
 * Structure containing a standard ID CAN message which is either ready to be
 * transmitted or has been received
 */
struct ican_msg_s {
    uint16_t id;
    uint8_t data[CAN_MAX_DATA];
    uint8_t dlc;
};



/**
 * Structure describing a circular buffer of CAN messages
 *
 * Messages are put in to the buffer at the index stored in `in` and taken out
 * from the index stored in `out`.  Indices are used instead of pointers in
 * deference to our friends at misra.
 *
 * All manipulation should be via these macros,
 * - #QUEUE_FULL
 * - #QUEUE_EMPTY
 * - #REMOVE_MESSAGE
 * - #MESSAGE_ADDED
 */

struct ican_rxtxq_s {
    uint8_t sz;     /**< size of message buffer array */
    uint8_t out;    /**< index of the oldest message in the buffer */
    uint8_t in;     /**< index of next empty message where one can be added */
    uint8_t priority;   /**< transmit only: priority of next message to be
                                sent - managed by the transmit ISR */
    struct ican_msg_s *msgs; /**< pointer to circular buffer of queued
                                messages */
};

/**
 * array of Receive message queues, one per controller
 */
extern struct ican_rxtxq_s ican_s12_rxq[Can_n_channels];

/*
 * macros to manipulate the circular buffer
 */

/**
 * helper macro for #REMOVE_MESSAGE and #MESSAGE_ADDED to move an index
 * up the list, wrapping if necessary.
 * @param size          number of elements in the circular buffer
 * @param[in,out] idx   the index to be incremented
 * @note only used by the above macros and not invoked directly from code
 * @note *idx is modified by this macro*
 * Implementation
 * --------------
 * `idx` is an index into an array from 0 to (`size` -1).  The macro increments
 * `idx` and then checks if it has gone outside the array.
 * If it has `(idx >- size)`, `idx` is set to the start of the array.
 */
#define     MOVE_CIRC_BUF(size,idx) do { \
                            (idx)++; \
                            if ((idx) >= (size)) \
                            { \
                            (idx) = 0u; \
                            } \
                        } while (0)

/**
 * Pointer to space to store a new message
 * @param qp    a pointer to the #ican_rxtxq_s message queue
 * @returns     a pointer to a #ican_msg_s structure
 * @note This macro must not be invoked if `QUEUE_FULL(qp) == TRUE`
 * @note This macro is used by icantx() and by the CAN receive ISRs so should
 * not be needed in application code.
 * Implementation
 * --------------
 * The next message buffer to be written to is at `msgs[in]`, so the macro
 * returns a pointer to this message buffer.  The calling code will then
 * store information in the buffer (id, dlc, message data) before
 * calling #MESSAGE_ADDED() to make the data avaliable.
 */
#define     NEW_MESSAGE(qp)     &((qp)->msgs[(qp)->in])
/**
 * Pointer to oldest message in the buffer, i.e. the next one to be processed
 * and removed.
 * @param qp    a pointer to the #ican_rxtxq_s message queue.  To get the next
 *              message in the receive queue for the NAME bus, this should be
 *              `&can_rxq[Can_NAME]`
 * @returns     a pointer to a #ican_msg_s structure
 * @note This macro must not be invoked if `QUEUE_EMPTY(qp) == TRUE`
 *
 * Implementation
 * --------------
 * The oldest unprocessed message in the circular buffer will be at `msgs[out]`
 * so the macro returns a pointer to it.
 */
#define     OLDEST_MESSAGE(qp)  &((qp)->msgs[(qp)->out])
/**
 * Remove the oldest message from the buffer.
 * @note Must only be invoked if there is data in the queue (i.e. only
 *              if `QUEUE_EMPTY(qp) != TRUE`)
 * @param qp    a pointer to the ican_rxtxq_s message queue.  To remove a
 *              message in the receive queue for the NAME bus after decoding
 *              it, this should be `&can_rxq[Can_NAME]`
 *
 * Implementation
 * --------------
 * The macro advances the out index, which will make the message buffer it
 * was pointing at available for a new message to be written.
 */
#define     REMOVE_MESSAGE(qp)  MOVE_CIRC_BUF((qp)->sz,(qp)->out)

/**
 * Adjust the buffer after a new message has been stored.
 * @param qp    a pointer to the ican_rxtxq_s message queue
 * @note Data must not be added, and this macro must not be invoked, if
 *      `QUEUE_FULL(qp) == TRUE`
 * @note This macro is used by icantx() and by the CAN receive ISRs so should
 * not normally be needed in application code.
 *
 * Implementation
 * --------------
 * This macro is only called after a message has been stored by writing through
 * the pointer returned by #NEW_MESSAGE().  The macro moves the in index
 * so that the data can be seen within the circular buffer.
 *
 */
#define     MESSAGE_ADDED(qp)   MOVE_CIRC_BUF((qp)->sz,(qp)->in)


/**
 * See whether there are any messages in the buffer
 * @param qp    a pointer to the #ican_rxtxq_s message queue.  To check
 *              the receive queue for the NAME bus, this should be
 *              `&can_rxq[Can_NAME]`
 * @retval TRUE there are no messages in the buffer
 * @retval FALSE there is at least one message in the buffer
 *
 * Implementation
 * --------------
 * The in index refers to the next buffer to be written so, if `out == in`,
 * there must be no data in the buffer.
 */
#define     QUEUE_EMPTY(qp) ((qp)->in == (qp)->out)
/**
 * See if the buffer is full (so no new messages can be added)
 * @note This macro is used by icantx() and by the CAN receive ISRs so should
 * not normally be needed in application code.
 * @param qp  a pointer to the rxtxq_s message queue
 * @retval TRUE the buffer is full so no more messages can be queued
 * @retval FALSE there is room for at least one more message in the buffer
 *
 * Implementation
 * --------------
 * We keep one message buffer between `out` and `in` empty (otherwise we
 * couldn't distinguish an empty buffer from a full one) so the buffer is full
 * if `in` is one less than `out`.  We need the ?: operator to handle when `in`
 * is about to wrap from the end of the circular buffer to the start
 */
#define     QUEUE_FULL(qp)  (((qp)->out == 0u) ? \
        ((qp)->in == ((qp)->sz - 1u)) : ((qp)->in == ((qp)->out - 1u)))

/** clear TXEIE[2:0] */
#define     DISABLE_CAN_TX_INTERRUPTS(HWP)  ((HWP)->CANTIER = 0u)
/** set TXEIE[2:0] */
#define     ENABLE_CAN_TX_INTERRUPTS(HWP)   ((HWP)->CANTIER = 7u)

/**
 * Structure containing error flags for a single CAN controller.
 * A structure is used instead of bit masks in a single byte because we need
 * to allow atomic setting and clearing from different contexts or cores
 */
struct ican_errors_s {
    bool_t ican_bus_off;        /**< controller has gone bus-off */
    bool_t ican_tx_warning;     /**< Transmit error counter has exceeded 96 */
    bool_t ican_tx_error;       /**< Transmit error counter has exceeded 127 */
    bool_t ican_rx_warning;     /**< Receive error counter has exceeded 96 */
    bool_t ican_rx_error;       /**< Receive error counter has exceeded 127 */
    bool_t ican_tx_buffer_overflow; /**< transmit message couldn't be queued
                                      because software buffer was full */
    bool_t ican_rx_hw_buffer_overflow; /**< receive message was lost because
                                         hardware buffers over ran */
    bool_t ican_rx_s12_sw_buffer_overflow; /**< receive message was lost because
                         there was no room for it in the S12 software buffer */
    bool_t ican_rx_xg_sw_buffer_overflow; /**< receive message was lost because
                     there was no room for it in the XGATE software buffer */
};

extern struct ican_errors_s ican_errors_present[Can_n_channels];
extern struct ican_errors_s ican_errors_latched[Can_n_channels];


/**
 * Report a CAN driver error
 *
 * Sets the error in both the #ican_errors_present and #ican_errors_latched
 * structure arrays.
 * @param channel_handle        Channel name from the #can_handle_e enumeration
 * @param ican_error            Type of error (from the list of members of
 *                              the #ican_errors_s structure
 */
#define         ICAN_SET_ERROR(channel_handle,ican_error)       \
                do { \
                      ican_errors_present[channel_handle].ican_error = TRUE; \
                      ican_errors_latched[channel_handle].ican_error = TRUE; \
                } while (0)
/**
 * Clear a CAN driver error in the #ican_errors_present structure array.
 *
 * Should be invoked only by the ican driver, not application code.
 * @param channel_handle        Channel name from the #can_handle_e enumeration
 * @param ican_error            Type of error (from the list of members of
 *                              the #ican_errors_s structure
 */
#define         ICAN_CLEAR_ERROR_PRESENT(channel_handle,ican_error)       \
                      (ican_errors_present[channel_handle].ican_error = FALSE)

/**
 * Clear a CAN driver error in the #ican_errors_latched structure array
 *
 * Is not invoked by the ican driver, but is intended for use by application
 * code, where it forms part of the #ICAN_CLEAR_LATCH_IF_ERROR_NOT_PRESENT macro
 * @param channel_handle        Channel name from the #can_handle_e enumeration
 * @param ican_error            Type of error (from the list of members of
 *                              the #ican_errors_s structure
 */
#define         ICAN_CLEAR_ERROR_LATCHED(channel_handle,ican_error)       \
                      (ican_errors_latched[channel_handle].ican_error = FALSE)

/**
 * Clear a latch if the error is no longer present.
 *
 * There is a risk that an error will be set from an interrupt (or another
 * core) after the !PRESENT test but before the latch is cleared.  However,
 * since we only clear the latch if it is already set, the effect of this
 * will be negligable: we will not miss an isolated fault.
 *
 * Disabling interrupts will not help because the error might be set on the
 * XGATE while this code is running on the S12.
 */
#define     ICAN_CLEAR_LATCH_IF_ERROR_NOT_PRESENT(channel_handle,ican_error) \
                do { \
                    if (ICAN_ERROR_IS_LATCHED(channel_handle,ican_error) \
                        && !ICAN_ERROR_IS_PRESENT(channel_handle,ican_error)) \
                    { \
                        ICAN_CLEAR_ERROR_LATCHED(channel_handle,ican_error); \
                    } \
                } while (0)


/**
 * Test for presence of a CAN driver error in the #ican_errors_present
 * structure array.
 *
 * @param channel_handle        Channel name from the #can_handle_e enumeration
 * @param ican_error            Type of error (from the list of members of
 *                              the #ican_errors_s structure
 * @retval      TRUE    error is present
 * @retval      FALSE   error is absent
 */
#define         ICAN_ERROR_IS_PRESENT(channel_handle,ican_error)       \
                              (ican_errors_present[channel_handle].ican_error)

/**
 * Test for presence of a CAN driver error in the #ican_errors_latched
 * structure array.
 *
 * @param channel_handle        Channel name from the #can_handle_e enumeration
 * @param ican_error            Type of error (from the list of members of
 *                              the #ican_errors_s structure
 * @retval      TRUE    error is present
 * @retval      FALSE   error is absent
 */
#define         ICAN_ERROR_IS_LATCHED(channel_handle,ican_error)       \
                              (ican_errors_latched[channel_handle].ican_error)


/*
 * function prototypes
 */

/**
 * Queues a message for transmission by the ISR provided the controller is not
 * bus off and there is space in the transmit software buffer.
 *
 * @param id    CAN message ID
 * @param chnl  CAN channel to be used
 * @param dlc   message data length count
 * @param data[] pointer to message data, which is copied into a local buffer
 * @retval   TRUE the message could be queued
 * @retval   FALSE controller bus-off or queue full so the message not queued
 *
 * If the message is queued successfully, this function enables the CAN
 * transmit interrupt to actually send the message.
 *
 * Implementation note
 * -------------------
 * The #ican_errors_present array is used to check for bus-offness rather than
 * checking the hardware directly.  This will be set and cleared by an
 * interrupt so there will be no practical difference.
 */
bool_t icantx(uint16_t id, enum can_handle_e chnl, uint8_t dlc,
    FPTR const uint8_t data[]);

bool_t ican_tx_impaired(enum can_handle_e chnl);
bool_t ican_rx_impaired(enum can_handle_e chnl);

#endif /* #ifndef ICAN_H */
