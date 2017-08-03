
/*****************************************************************************
| Project Name:   XCP on CAN Transport Layer adaption
|    File Name:   xcp_can.c
|
|  Description: 
|    Adaption component for the interface XCP Protocol Layer
|    and XCP on CAN Transport Layer ( Vector CANbedded CAN Driver )
|
|  Limitations:
|    - Interleaved communication mode is not allowed on CAN
|    - Detection of all XCP slaves within a CAN network is not supported
|        ( GET_SLAVE_ID )
|    - Assignment of CAN identifiers to DAQ lists or STIM is not supported
|        ( GET_DAQ_ID, SET_DAQ_ID )
|    - Variable data length is not supported.
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2008 by Vector Informatik GmbH.           All rights reserved.
|
|       This software is copyright protected and 
|       proporietary to Vector Informatik GmbH.
|       Vector Informatik GmbH grants to you only
|       those rights as set out in the license conditions.
|       All other rights remain with Vector Informatik GmbH.
| 
|       Diese Software ist urheberrechtlich geschuetzt. 
|       Vector Informatik GmbH raeumt Ihnen an dieser Software nur 
|       die in den Lizenzbedingungen ausdruecklich genannten Rechte ein.
|       Alle anderen Rechte verbleiben bei Vector Informatik GmbH.
|
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     -------------------------------------
| Tri          Frank Triem               Vector Informatik GmbH
| Svh          Sven Hesselmann           Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
|  Date       Version  Author  Description
| ----------  -------  ------  -----------------------------------------------
| 2003-09-04  1.00.00   Tri    Creation based on CCP Driver adaption.
|                              MISRA rules applied.
| 2005-01-03  1.01.00   Tri    ESCAN00008009: Rename module versions according to PD_PSC_Development
|                              ESCAN00009121: Add copyright note
|                              ESCAN00009127: Remove XCP_ENABLE_SEND_BUFFER
|                              ESCAN00009141: Compiler warnings while compilation with Tasking Compiler
|                              ESCAN00008085: Apply new naming for the parameters of callbacks
|                              ESCAN00008010: Remove extern declaration for xcp struct
|                              ESCAN00009154: Update Seed & Key
|                              ESCAN00008008: Apply PSC naming convention for types and structurs
|                              ESCAN00008007: Rename the post-organified filenames to xcpProf.h and xcpProf.c
|                              ESCAN00009172: Atmega only: Compiler error due to pointer conversion from RAM to Flash
|                              ESCAN00010708: Create template _xcp_appl.c
|                              ESCAN00007209: Apply naming convention to callback functions
|                              ESCAN00009144: Minor changes
| 2005-02-28  1.02.00   Tri    ESCAN00011303: Add CQ version and comment
| 2005-05-05  1.03.00   Tri    ESCAN00007515: Warning when compiling xcp_can.c
|                              ESCAN00012311: Support CAN-Driver without transmit queue
|                              ESCAN00011772: Support multiple CAN channels
| 2006-05-30  1.04.00   Tri    ESCAN00016506: Remove ISR locks in XcpPreCopy
| 2006-10-24  1.05.00   Tri    ESCAN00018168: Minor changes and rework of consistency checks
| 2008-03-19  1.06.00   Svh    ESCAN00023519: allow global access to variable xcpChannelNumber
|                              ESCAN00023375: Grant access to the current active channel
|                              ESCAN00025417: Removed not needed variable xcpQueuedObjectDLC
|***************************************************************************/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "XcpProf.h"
#include "can_inc.h"


/***************************************************************************/
/* Version check                                                           */
/***************************************************************************/
#if ( CP_XCPONCAN_VERSION != 0x0106u )
  #error "Source and Header file are inconsistent!"
#endif
#if ( CP_XCPONCAN_RELEASE_VERSION != 0x00u )
  #error "Source and Header file are inconsistent!"
#endif


/****************************************************************************/
/* Constants                                                                */
/****************************************************************************/

/* Global constants with XCP Transport Layer for CAN main and subversion */
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kXcpOnCanMainVersion    = (vuint8)(CP_XCPONCAN_VERSION >> 8);
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kXcpOnCanSubVersion     = (vuint8)(CP_XCPONCAN_VERSION & 0x00ffu);
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kXcpOnCanReleaseVersion = (vuint8)(CP_XCPONCAN_RELEASE_VERSION);


/*****************************************************************************/
/* Default switches                                                          */
/*****************************************************************************/
#if defined ( C_ENABLE_TRANSMIT_QUEUE )
  #define XCP_DISABLE_SEND_WITHOUT_QUEUE
#else
  #define XCP_ENABLE_SEND_WITHOUT_QUEUE
#endif


/*****************************************************************************/
/* Defines                                                                   */
/*****************************************************************************/


/*****************************************************************************/
/* Constants                                                                 */
/*****************************************************************************/

#if defined ( XCP_ENABLE_SEND_WITHOUT_QUEUE )
/* States of xcpSendWithoutQueueState */
  #define kXcpNoMsgPending 0
  #define kXcpMsgPending   1
#endif


/*****************************************************************************/
/* Local data definitions                                                    */
/*****************************************************************************/
#if defined ( XCP_ENABLE_SEND_WITHOUT_QUEUE )
/* 
  State of CAN message, required to send without any queue.
  (requested to send via ApplXcpSend).
  These variables are initialized in ApplXcpInit().
*/
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 xcpSendWithoutQueueState;
#endif

#if defined ( XCP_ENABLE_MULTI_CHANNEL )
/* global variables */
/* this variable is not static so application can read it, ESCAN00023519, ESCAN00023375 */
V_MEMRAM0 V_MEMRAM1 CanChannelHandle V_MEMRAM2 xcpChannelNumber; /* ECU number of actual XCP session */
#endif


/*****************************************************************************/
/* External data                                                             */
/*****************************************************************************/
#if defined ( XCP_ENABLE_MULTI_CHANNEL )
/* Transmit handle of the XCP Slave message */
V_MEMROM0 extern V_MEMROM1 CanTransmitHandle V_MEMROM2 xcpTxHandleField[kXcpNumberOfCanChannels];

/* Tx data buffer */
V_MEMROM0 extern V_MEMROM1 TxDataPtr V_MEMROM2 xcpTxDataPtrField[kXcpNumberOfCanChannels];
#endif



/*****************************************************************************/
/* The following functions are the interface between the                     */
/* XCP Protocol Layer and the Vector CAN driver                              */
/*****************************************************************************/

/*****************************************************************************
| NAME:             XcpPreCopy
| CALLED BY:        CanHL_ReceivedRxHandle
| PRECONDITIONS:    CTO or DTO Rx CAN message received
| INPUT PARAMETERS: depending on API: 
|                   - rxStruct  : pointer to RxInfoStruct or
|                   - rxObject  : ReceiveHandle or
|                   - rxDataPtr : pointer to received data
| RETURN VALUES:    kCanNoCopyData (no handling of received message by CAN Driver)
| DESCRIPTION:      Precopy function for CTO and DTO Rx messages.
|                   This function is configured in the generation tool.
******************************************************************************/

#if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) || defined ( C_SINGLE_RECEIVE_CHANNEL )
  #if defined ( rxRegPtr )
  #else
    #define rxRegPtr (rxStruct->pChipData)
  #endif
  #define PRECOPY_PARAM_TYPE CanRxInfoStructPtr
  #define PRECOPY_PARAM      rxStruct
  #if defined ( XCP_ENABLE_VARIABLE_DLC )
    #define rxActualDLC      CanRxActualDLC( PRECOPY_PARAM )
  #endif
  #if defined ( XCP_ENABLE_MULTI_CHANNEL )
    #define rxChannelHandle  (PRECOPY_PARAM->Channel)
  #endif
#else /* if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) || defined ( C_SINGLE_RECEIVE_CHANNEL ) */
  #if defined ( C_SINGLE_RECEIVE_BUFFER )
    #if defined ( rxRegPtr )
    #else
      #define rxRegPtr ((vuint8*)CanRDSPtr)
    #endif
    #define PRECOPY_PARAM_TYPE CanReceiveHandle
    #define PRECOPY_PARAM      rxObject
    #if defined ( XCP_ENABLE_VARIABLE_DLC )
      #define rxActualDLC      CanRxActualDLC
    #endif
    #if defined ( XCP_ENABLE_MULTI_CHANNEL )
      #error "Single receive buffer API does not support XCP multiple CAN channels."
    #endif
  #endif /* if defined ( C_SINGLE_RECEIVE_BUFFER ) */
  #if defined ( C_MULTIPLE_RECEIVE_BUFFER )
    #if defined ( rxRegPtr )
    #else
      #define rxRegPtr rxDataPtr
    #endif
    #define PRECOPY_PARAM_TYPE CanChipDataPtr
    #define PRECOPY_PARAM      rxDataPtr
    #if defined ( XCP_ENABLE_VARIABLE_DLC )
      #define rxActualDLC      CanRxActualDLC
    #endif
    #if defined ( XCP_ENABLE_MULTI_CHANNEL )
      #error "Multi receive buffer API does not support XCP multiple CAN channels."
    #endif
  #endif /* if defined ( C_MULTIPLE_RECEIVE_BUFFER ) */
#endif /* if defined ( C_MULTIPLE_RECEIVE_CHANNEL ) || defined ( C_SINGLE_RECEIVE_CHANNEL ) */

vuint8 XcpPreCopy( PRECOPY_PARAM_TYPE PRECOPY_PARAM )
{
#if defined ( XCP_ENABLE_VARIABLE_DLC )
  vuint8  i;
#endif
  /* An aligned field of 8 Byte, which is passed XcpCommand.          */
  /* Deviation of MISRA rule 110 (an aligned byte array is required). */
  tXcpCto c;

#if defined ( XCP_ENABLE_MULTI_CHANNEL )
  #if defined ( XCP_ENABLE_MULTI_CONNECTION_PROTECTION )
  /* Ensure that no XCP connection is interrupt via another channel. */
  if ( ( xcpChannelNumber != rxChannelHandle ) && ( XcpGetState() == (vuint8)XCP_CONNECTED ) )
  {
    /* Do not transmit an error packet due to XcpConfirmation() will be called on the wrong channel. */
    return( (vuint8)kCanNoCopyData );
  } 
  else
  #endif
  {
    /* Store the CAN channel for the response.  */
    xcpChannelNumber = rxChannelHandle;
  }
#else
  #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  /* avoid compiler warning due to unused parameters */
  PRECOPY_PARAM = PRECOPY_PARAM;
  #endif
#endif

  /*
    XCP message has been received.
    Copy the received data from the CAN cell to an aligned buffer.
    To ensure minimal operation time, copy only the necessary number of data.
  */

#if defined ( XCP_ENABLE_VARIABLE_DLC )

  /* Copy only the received data (varaiable DLC) */
  for (i = ( (vuint8)rxActualDLC - (vuint8)1 ); i < (vuint8)(0x08); i--)
  {
    c.b[i] = rxRegPtr[i];
  }

#else

  /* DLC = 8 */
  c.b[0] = rxRegPtr[0];
  c.b[1] = rxRegPtr[1];
  c.b[2] = rxRegPtr[2];
  c.b[3] = rxRegPtr[3];
  c.b[4] = rxRegPtr[4];
  c.b[5] = rxRegPtr[5];
  c.b[6] = rxRegPtr[6];
  c.b[7] = rxRegPtr[7];

#endif

  /* Executing the command interpreter (evaluation of the protocol message) */
  XcpCommand( c.dw );

  return( (vuint8)kCanNoCopyData );
}


/*****************************************************************************
| NAME:             XcpConfirmation
| CALLED BY:        CanTransmit
| PRECONDITIONS:    CTO or DTO has been transmitted successfully.
| INPUT PARAMETERS: Handle of transmitted message.
| RETURN VALUES:    none 
| DESCRIPTION:      Confirmation function of CTO and DTO Tx messages.
|                   This function is configured in the generation tool.
******************************************************************************/
void XcpConfirmation( CanTransmitHandle txObject )
{
#if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  /* avoid compiler warning due to unused parameters */
  txObject = txObject;
#endif

  /*
    Notify the XCP protocol driver of the successful transmission of a message.
    The XCP driver will not call ApplXcpSend() again, until XcpSendCallBack has
    indicated the successful transmission of the previous message.
  */
  (void)XcpSendCallBack();         /* ESCAN00007515 */
}


/*****************************************************************************
| NAME:             ApplXcpSend
| CALLED BY:        XcpSendCrm, XcpSendEv, XcpSendDto
| PRECONDITIONS:    none
| INPUT PARAMETERS: msg : pointer to message
|                   len : length of message data
| RETURN VALUES:    none 
| DESCRIPTION:      Request for the transmission of a DTO or CTO message.
******************************************************************************/
void ApplXcpSend( vuint8 len, MEMORY_ROM BYTEPTR msg )
{
#if defined ( XCP_ENABLE_VARIABLE_DLC )
  vuint8  i;
#endif

  /*
    Copy the transmit data to the transmit buffer.
    To ensure minimal operation time, copy only the necessary number of data.
  */
#if defined ( XCP_ENABLE_VARIABLE_DLC )

  for (i = (len-(vuint8)1u); i < (vuint8)(0x08); i--)
  {
    XcpGetTransmitDataPtr()[i] = msg[i];
  }

#else

  #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  /* avoid compiler warning due to unused parameters */
  len = len;
  #endif

  /* DLC = 8 */
  XcpGetTransmitDataPtr()[0] = msg[0];
  XcpGetTransmitDataPtr()[1] = msg[1];
  XcpGetTransmitDataPtr()[2] = msg[2];
  XcpGetTransmitDataPtr()[3] = msg[3];
  XcpGetTransmitDataPtr()[4] = msg[4];
  XcpGetTransmitDataPtr()[5] = msg[5];
  XcpGetTransmitDataPtr()[6] = msg[6];
  XcpGetTransmitDataPtr()[7] = msg[7];

#endif

  /* Transmit the protocol message */
  if ( XcpTransmit() != kCanTxOk )
  {
#if defined ( XCP_ENABLE_SEND_WITHOUT_QUEUE )

    /*
      Trying to send the message directly via CanTransmit failed.
      Set flag, to start trying to send this message in ApplXcpBackground.
    */
    xcpSendWithoutQueueState = kXcpMsgPending;

#else /* if defined ( XCP_ENABLE_SEND_WITHOUT_QUEUE ) */

    /*
      Fatal Error, message cannot be sent. Should never fail.
      What happens in this case? Initialize XCP.
    */
    XcpInit();

#endif /* if defined ( XCP_ENABLE_SEND_WITHOUT_QUEUE ) */
  }
}


#if defined ( ApplXcpInit )
/* already defined as macro. */
#else
/*****************************************************************************
| NAME:             ApplXcpInit
| CALLED BY:        XcpInit
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    none 
| DESCRIPTION:      Initializations of the XCP on CAN Transport Layer.
|                   Can also be used for miscellaneous initializations.
******************************************************************************/
void ApplXcpInit( void )
{
  #if defined ( XCP_ENABLE_SEND_WITHOUT_QUEUE )
  /* Initialize variables for transmisson without any queue. */
  xcpSendWithoutQueueState = (vuint8)kXcpNoMsgPending;
  #endif
}
#endif /* #if defined ( ApplXcpInit ) */


#if defined ( ApplXcpBackground )
/* already defined as macro. */
#else
/*****************************************************************************
| NAME:             ApplXcpBackground
| CALLED BY:        XcpBackground
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    none 
| DESCRIPTION:      Perform background operations
******************************************************************************/
void ApplXcpBackground( void )
{
  #if defined ( XCP_ENABLE_SEND_WITHOUT_QUEUE )

  /* If a CAN message is pending, try to send this */
  if ( xcpSendWithoutQueueState == (vuint8)kXcpMsgPending )
  {
    /* Try to transmit the protocol message */
    if ( XcpTransmit() == (vuint8)kCanTxOk )
    {
      /* The message was sent successfully. Clear the 'message pending' bit. */
      xcpSendWithoutQueueState = (vuint8)kXcpNoMsgPending;
    }
    else
    {
      /* The message is still pending. */
    }
  }
  else
  {
    /* No message is pending. */
  }

  #endif
}
#endif



/*****************************************************************************/
/* Consistency checks ( XCP Transport Layer on CAN specific )                */
/*****************************************************************************/

/* Check configuration of Transport Layer */

#if defined ( XCP_TRANSPORT_LAYER_TYPE_CAN )
#else
  #error "Configuration for Transport Layer on CAN doesn't exist!"
#endif

/* Check consistency of variable or static data length */

#if defined ( XCP_ENABLE_VARIABLE_DLC )
  #if defined ( C_ENABLE_DLC_CHECK )
    #error "XCP consistency error: Variable data length not possible due to DLC check is enabled."
  #endif
  #if defined ( XCP_DISABLE_VARIABLE_XCP_DATA_LENGTH )
    #error "XCP consistency error: Select either variable or static data length."
  #endif
#else /* if defined ( XCP_ENABLE_VARIABLE_DLC ) */
  #if defined ( XCP_DISABLE_VARIABLE_DLC )
  #else
    #error "XCP consistency error: Usage of variable or static data length not defined."
  #endif
#endif /* if defined ( XCP_ENABLE_VARIABLE_DLC ) */

/* Check range of kXcpMaxCTO and kXcpMaxDTO */

#if (kXcpMaxCTO == 8)
#else
  #error "XCP consistency error: MAX_CTO must be 8 on CAN."
#endif
#if (kXcpMaxDTO == 8)
#else
  #error "XCP consistency error: MAX_DTO must be 8 on CAN."
#endif

/* Check consistency of DAQ header ODT */

#if defined ( XCP_ENABLE_DAQ_HDR_ODT_DAQ )
  #error "XCP consistency error: DAQ_HDR_ODT_DAQ must be disabled on CAN."
#endif


/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
