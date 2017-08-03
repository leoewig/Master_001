/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2016 - All rights reserved 
 *
 * file name:    sci.h
 *
 * description:  
 *  			sci configure header.
 * 
 *
 * revision history:
 *
 * Date         Author           Description
 * 2016-Apr-15  Jun Wu           initial version
 *
 *****************************************************************************/
#if !defined(SCI_H)
#define      SCI_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "basic_types.h"
#include "hardware.h"
#include "regs.h"
#include "vio_types.h"

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/ 
#define TMM_Display1_MASK  0x01
#define TMM_Display2_MASK  0x02
#define TMM_Display3_MASK  0x03
#define TMM_Display4_MASK  0x04
#define TMM_Display5_MASK  0x05
#define TMM_RX_SENT_MASK   0x10
#define TMM_RX_Angle_MASK  0x20
#define TMM_RX_Config_MASK 0x30
#define TMM_RX_PID_MASK 0x40

#define Motor_Rotating_MASK  0x10
#define Motor_Direction_MASK  0x20
//#define Motor_Direction_Negative_MASK  0x40
//#define SEQ_MEAS_START_MASK 0x80
#define TMM_UME_MASK 0x40
#define TMM_LME_MASK 0x80

#define SCI_BAUD  		9600
#define SCI_INP_BUF_SIZE 8 
#define SCI_OUT_BUF_SIZE 8
#define SCICR2_TIE_MASK                0x80

#define SCISR1_PF_MASK                 1
#define SCISR1_FE_MASK                 2
#define SCISR1_NF_MASK                 4
#define SCISR1_OR_MASK                 8
#define SCISR1_IDLE_MASK               16
#define SCISR1_RDRF_MASK               32
#define SCISR1_TC_MASK                 64
#define SCISR1_TDRE_MASK               128

//#define  SCI1CR2_TIE (SCI1CR2&SCICR2_TIE_MASK)
//#define  SCI1CR2_TIE (SCI1CR2&SCICR2_TIE_MASK)


#define ERR_OK           0             /* OK */
#define ERR_SPEED        1             /* This device does not work in the active speed mode. */
#define ERR_RANGE        2             /* Parameter out of range. */
#define ERR_VALUE        3             /* Parameter of incorrect value. */
#define ERR_OVERFLOW     4             /* Timer overflow. */
#define ERR_MATH         5             /* Overflow during evaluation. */
#define ERR_ENABLED      6             /* Device is enabled. */
#define ERR_DISABLED     7             /* Device is disabled. */
#define ERR_BUSY         8             /* Device is busy. */
#define ERR_NOTAVAIL     9             /* Requested value or method not available. */
#define ERR_RXEMPTY      10            /* No data in receiver. */
#define ERR_TXFULL       11            /* Transmitter is full. */
#define ERR_BUSOFF       12            /* Bus not available. */
#define ERR_OVERRUN      13            /* Overrun error is detected. */
#define ERR_FRAMING      14            /* Framing error is detected. */
#define ERR_PARITY       15            /* Parity error is detected. */
#define ERR_NOISE        16            /* Noise error is detected. */
#define ERR_IDLE         17            /* Idle error is detected. */
#define ERR_FAULT        18            /* Fault error is detected. */
#define ERR_BREAK        19            /* Break char is received during communication. */
#define ERR_CRC          20            /* CRC error is detected. */
#define ERR_ARBITR       21            /* A node losts arbitration. This error occurs if two nodes start transmission at the same time. */
#define ERR_PROTECT      22            /* Protection error is detected. */
#define ERR_UNDERFLOW    23            /* Underflow error is detected. */
#define ERR_UNDERRUN     24            /* Underrun error is detected. */
#define ERR_COMMON       25            /* Common error of a device. */
#define ERR_LINSYNC      26            /* LIN synchronization error is detected. */


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
extern uint16_t SCI_InpLen;  
extern bool_t SCI_rx_success;
extern uint32_t SCI_rec_buf_number;
extern uint8_t SCI_rec_buf[8];
extern uint8_t SCI_Send_Buffer[8];
extern uint8_t SCI_response_buf[8];
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/ 
INTERRUPT void SCI1_Interrupt(void);
void sci_init(void);
uint8_t SCI1_RecvChar(uint8_t *Chr);
uint8_t SCI1_SendChar(uint8_t Chr);
uint8_t SCI1_RecvBlock(uint8_t *Ptr, uint16_t Size, uint16_t *Rcv);
uint8_t SCI1_SendBlock(uint8_t *Ptr, uint16_t Size, uint16_t *Snd);
uint8_t SCI1_ClearRxBuf(void);
uint8_t SCI1_ClearTxBuf(void);
uint8_t SCICalcChecksum(uint8_t *data);


void SCI1_OnError(void);
void SCI1_OnRxChar(void);
void SCI1_OnTxChar(void);
void SCI1_OnFullRxBuf(void);
void SCI1_OnFreeTxBuf(void);

void sci_rx_response(void);
void sci_write_outputs(void);

#endif /* SCI_H */
