

/*****************************************************************************
| Project Name:   XCP Protocol Layer
|    File Name:   XcpProf.c
|
|  Description:   Implementation of the XCP Protocol Layer 
|                 XCP V1.0 slave device driver
|                 Professional Version
|
|
|     General limitations:
|
|     - Timestamp size is always WORD
|     - Daq and Event numbers are BYTE
|     - Only dynamic DAQ list allocation supported
|     - Max. checksum block size is 0xFFFF
|     - CECKSUM_TYPE CRC16, CRC32 and 'user defined' are not supported
|     - MAX_DTO is limited to max. 255
|     - The resume bits in daq lists are not set
|     - STORE_DAQ, CLEAR_DAQ and STORE_CAL will not send a event message 
|     - Entering resume mode will not send a event message
|     - Overload indication by event is not supported
|     - Page Info and Segment Info is not supported
|     - DAQ does not support address extensions
|     - DAQ-list and event channel prioritization is not supported
|     - Event channels contain one DAQ-list
|     - Event channel time unit is 1 ms
|     - ODT optimization not supported
|     - Interleaved communication mode is not supported
|     - The seed size is equal or less MAX_CTO-2
|     - The key size is equal or less MAX_CTO-2
|     - Only default programming data format is supported
|     - GET_SECTOR_INFO does not return sequence numbers
|     - PROGRAM_VERIFY and PROGRAM_FORMAT are not supported
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
| Ds           Sven Deckardt             Vector Informatik GmbH
| Eta          Edgar Tongoona            Vector Informatik GmbH
| Hr           Andreas Herkommer         Vector Informatik GmbH
| Svh          Sven Hesselmann           Vector Informatik GmbH
| Tri          Frank Triem               Vector Informatik GmbH
| Za           Rainer Zaiser             Vector Informatik GmbH
| Bwr          Brandon Root              Vector CANtech
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
|  Date       Version  Author  Description
| ----------  -------  ------  -----------------------------------------------
| 2003-03-01  0.9.00    Ds     Created  
| 2003-05-01  0.9.10    Za     Still a lot of changes, no detailled history yet
| 2003-06-31  1.0.00    Za     Released
| 2003-09-19  1.0.00    Za     XCP_DAQ_TIMESTAMP_FIXED
| 2003-10-22  1.0.01    Ds     Change #if def instruction
| 2003-10-30  1.0.02    Ds     Bugfix in the loop of the ODT Entry
| 2003-10-30  1.0.03    Ds     Rename xcp20.c to xcpProf.c 
| 2003-11-20  1.01.00   Tri    Updated to PSC standard
|                              MISRA rules applied.
|                              Minor changes.
| 2004-02-11  1.01.01   Ds,Tri Updated and adaption for CANape
|                              ESCAN00007511: Warning when compiling XCP.H
|                              ESCAN00007517: Compiler error when using a packet length of 253 bytes or greater
|                              ESCAN00007518: Seed & Key: reading of the unlock key not performed correctly
|                              ESCAN00007532: Division by zero in calculation of the queue size
|                              ESCAN00007533: Memory overflow check during memory allocation might not be detected.
| 2004-06-16  1.02.00   Tri    ESCAN00008482: Add user defined function to service GET_ID
| 2005-01-03  1.03.00   Tri,Za ESCAN00008009: Rename module versions according to PD_PSC_Development
|                              ESCAN00009121: Add copyright note
|                              ESCAN00009125: Remove defines for revision 18
|                              ESCAN00009127: Remove XCP_ENABLE_SEND_BUFFER
|                              ESCAN00009130: Add support for data stimulation (STIM)
|                              ESCAN00007824: Warning due to unreferenced label 'negative_response1'
|                              ESCAN00008012: Remove include definitions of the standard libraries within XCP Professional
|                              ESCAN00008015: Avoid usage of the test mode within XCP Professional
|                              ESCAN00008018: XCP module version in response of CC_GET_COMM_MODE_INFO corrected
|                              ESCAN00008004: Compiler error when block upload is enabled and block download is disabled
|                              ESCAN00008005: Resource bit for CAL/PAG always set in response upon CC_CONNECT
|                              ESCAN00009141: Compiler warnings while compilation with Tasking Compiler
|                              ESCAN00007823: Warning about unreferenced variable 'prescaler'
|                              ESCAN00008003: Compiler error when block download is disabled and Programming enabled
|                              ESCAN00008060: Issue in negative response of command BUILD_CHECKSUM
|                              ESCAN00008013: Checksum calculation wrong
|                              ESCAN00008072: Compiler warning in range check of defines
|                              ESCAN00007971: Implement and support 'session configuration id'
|                              ESCAN00008006: Page switching always enabled when programming is enabled.
|                              ESCAN00008010: Remove extern declaration for xcp struct
|                              ESCAN00009154: Update Seed & Key
|                              ESCAN00010703: PROGRAM size = 0 is valid
|                              ESCAN00008017: Rework of Flash Programming by Flash Kernel
|                              ESCAN00009200: Positive Response upon command PROGRAM_START not correct
|                              ESCAN00010705: Rework command PROGRAM_RESET
|                              ESCAN00010706: Return the status of event channels
|                              ESCAN00010707: Consistency check in case of no DAQ released
|                              ESCAN00008008: Apply PSC naming convention for types and structures
|                              ESCAN00009173: Consistency check for generator DLL and component
|                              ESCAN00008007: Rename the post-organified filenames to xcpProf.h and xcpProf.c
|                              ESCAN00009172: Atmega only: Compiler error due to pointer conversion from RAM to Flash
|                              ESCAN00007209: Apply naming convention to callback functions
|                              ESCAN00009144: Minor changes
| 2005-02-01  1.04.00   Tri    ESCAN00010989: Update comment for version scan
|                              ESCAN00010848: Move timestamp unit of response to GET_DAQ_EVENT_INFO in low nibble
| 2005-02-17  1.05.00   Tri    ESCAN00011210: Support GENy Fwk 1.3 DLL Versions
| 2005-02-28  1.06.00   Tri    ESCAN00010961: Include XCP Transport Layer on CAN
| 2005-05-10  1.07.00   Tri    ESCAN00011446: Tasking Compiler only: Compiler Warnings
|                              ESCAN00012314: Compatibility with CANape 5.5.x regarding timestamp unit
|                              ESCAN00012356: Support data paging on Star12X / Metrowerks
|                              ESCAN00012617: Add service to retrieve XCP state
| 2006-01-03  1.08.00   Tri    ESCAN00013899: Data acquisition not possible during cold start
|                              ESCAN00009196: PROGRAM_FORMAT is not implemented as specified
|                              ESCAN00009199: Negative Response to command PROGRAM_PREPARE is not correct
|                              ESCAN00009202: Programming Info not implemented correctly
|                              ESCAN00014313: Warning because of undefined 'kXcpStimOdtCount'
|                              ESCAN00013634: Remove kXcpPacketHeaderSize
|                              ESCAN00014710: Rework Calibration Data Page Freeze
|                              ESCAN00014712: Rework Segment Info and Page Info
|                              ESCAN00014775: Delayed EEPROM read access not possible
| 2006-03-09  1.09.00   Tri    ESCAN00013637: Support command TRANSPORT_LAYER_CMD
|                              ESCAN00015283: Start of a single DAQ list is not possible
|                              ESCAN00015607: Support XCP on FlexRay Transport Layer
| 2006-05-05  1.10.00   Tri    ESCAN00016158: Add demo disclaimer to XCP Basic
|                              ESCAN00016098: Calculation of CRC16 CCITT is not correct
| 2006-05-30  1.11.00   Tri    ESCAN00016225: Support Cp_XcpOnLin
| 2006-07-18  1.12.00   Tri    ESCAN00016955: Support AUTOSAR CRC module
|                              ESCAN00016958: Delayed EEPROM read access not possible
| 2006-10-26  1.13.00   Tri    ESCAN00017516: Support Cp_XcpOnCanAsr
|                              ESCAN00017504: Replace P_MEM_ROM by MEMORY_FAR resp. V_MEMRAM2_FAR
|                              ESCAN00017804: Multiple definitions of XCP_DISABLE_TESTMODE
|                              ESCAN00017878: Overwriting of memory during data acquisition allocation
| 2007-01-30  1.14.00   Tri    ESCAN00018808: Support data paging on Star12X / Cosmic
| 2007-04-03  1.15.00   Eta    ESCAN00018153: Overwriting of memory during data stimulation
|                       Svh    ESCAN00020126: Commands SHORT_DOWNLOAD and MODIFY_BITS must be supported
|                              ESCAN00012618: Support command MODIFY_BITS
|                              ESCAN00020127: It has to be possible to en-/disable XCP globally
|                              ESCAN00019094: Extend implementation for runtime deactivation of XCP (professional)
|                              ESCAN00020128: Add AUTOSAR based API
|                              ESCAN00018154: Support overwriting of XcpSendDto() in header
|                              ESCAN00015859: Support memory read protection
| 2007-05-25  1.16.00   Svh    ESCAN00020906: Compiler error due to incorrect pointer assignment
| 2007-07-09  1.16.01   Hr     Support AUTOSAR Memory Mapping
| 2007-12-07  1.16.02   Hr     ISS046 - Only CTOs are flushed by the Protocol Layer
| 2007-09-14  1.17.00   Svh    ESCAN00022293: Support for SET_SEGMENT_MODE and GET_SEGMENT_MODE added
| 2007-12-17  1.18.00   Svh    ESCAN00023759: Compile error for MCS12x
| 2008-03-17  1.19.00   Svh    ESCAN00021035: XcpSendCallBack() always returns 1
|                              ESCAN00024265: STIM with time stamp is not supported
|                              ESCAN00024863: Missing error code in CC_TRANSPORT_LAYER_CMD
|                              ESCAN00025020: Possible stack issues with big MAX_DTO values in case Send Queue is not used
|                              ESCAN00023570: Do not set default answer is last CRM is still pending
| 2008-04-10  1.20.00   Svh    ESCAN00025979: tXcpDto wrong structure ordering causing erroneous one-time Stim trigger
| 2008-04-30  1.21.00   Eta    ESCAN00026424: compiler error when using Cosmic with option -pck
|                       Svh    ESCAN00026491: Data inconsistency of ODTs in case of Overrun + FlexRay
| 2008-04-30  1.21.01   Svh    ESCAN00026541: compiler error due to unknown symbol
| 2008-06-04  1.21.02   Svh    ESCAN00027343: Redefinition of ApplXcpSendFlush
| 2008-07-21  1.22.00   Hr     ESCAN00022545: Memory Read Protection always returns Ok to CANape
|                              ESCAN00020637: Support different Info Time Unit for DTO handling
|                              ESCAN00017954: Support MIN_ST_PGM
|                              ESCAN00017951: Add open interface for command processing
|                              ESCAN00028579: CC_PROGRAM_START should support an application callback
|                       Svh    ESCAN00028720: Support ADDRESS_GRANULARITY WORD
| 2008-09-10  1.23.00   Svh    ESCAN00029890: Incorrect checksum calculation
|                              ESCAN00029896: Command pending for several call backs added
|                              ESCAN00029897: XcpStimEventStatus() returns 1 also if no STIM data is available
|                              ESCAN00029905: Configuration of parameter MIN_ST in response of GET_COMM_MODE_INFO command added
| 2008-10-01  1.23.01   Hr     ESCAN00030382: Compiler error due to missing ;
| 2008-10-10  1.23.02   Bwr    ESCAN00030037: Support for more than 255 ODT entries
| 2008-12-01  1.24.00   Hr     ESCAN00031342: Version information of implementation inconsistent to release version information
|                              ESCAN00031726: Add support for XcpOnTcpIpAsr
|                              ESCAN00031948: Event gets lost, if a Response is still pending
|                              ESCAN00031949: Add error check for ODT_SIZE_WORD with no DAQ_HDR_ODT_DAQ
|                       Bwr    ESCAN00030566: SET_REQUEST with incorrect mode is ignored
|                              ESCAN00030601: Support for more than 255 ODTs
|***************************************************************************/


/***************************************************************************/
/* Include files                                                           */
/***************************************************************************/

    #include "XcpProf.h"

/* Include AUTOSAR CRC module header file. */
#if defined ( XCP_ENABLE_AUTOSAR_CRC_MODULE )
  #include "Crc.h"
#endif

/* XCP DPRAM Client Mode */



/***************************************************************************/
/* Version check                                                           */
/***************************************************************************/
#if ( CP_XCP_VERSION != 0x0124u )
  #error "Source and Header file are inconsistent!"
#endif
#if ( CP_XCP_RELEASE_VERSION != 0x00u )
  #error "Source and Header file are inconsistent!"
#endif

#if defined ( VGEN_GENY )
  #if defined ( CP_XCPDLL_VERSION ) 
    #if ( CP_XCPDLL_VERSION >= 0x0209u )
    #else
      #error "XCP component version and generator version are not consistent!"
    #endif
  #else 
    #if defined ( VGEN_VERSION_CP_XCP_MAIN )
      #if ( ( VGEN_VERSION_CP_XCP_MAIN > 1 ) || ( VGEN_VERSION_CP_XCP_SUB > 5 ) )
      #else
        #error "XCP component version and generator version are not consistent!"
      #endif
    #else
      #error "XCP component version and generator version are not consistent!"
    #endif
  #endif
#endif

/* Check specifcation version of AUTOSAR CRC module. */
#if defined ( XCP_ENABLE_AUTOSAR_CRC_MODULE )
  #if ( CRC_AR_MAJOR_VERSION >= 2 )
  #else
    #error "Compatibility error: Only AUTOSAR XCP module Release 2.x.x and greater is supported!"
  #endif
#endif


/***************************************************************************/
/* Check of Transport Layer availability                                   */
/***************************************************************************/

#if defined ( XCP_TRANSPORT_LAYER_TYPE_CAN ) && defined ( XCP_TRANSPORT_LAYER_TYPE_LIN )
  #error "XCP Transport Layer Error: Only one XCP Transport Layer is supported!"
#endif
#if defined ( XCP_TRANSPORT_LAYER_TYPE_CAN ) && defined ( XCP_TRANSPORT_LAYER_TYPE_FLEXRAY )
  #error "XCP Transport Layer Error: Only one XCP Transport Layer is supported!"
#endif
#if defined ( XCP_TRANSPORT_LAYER_TYPE_LIN ) && defined ( XCP_TRANSPORT_LAYER_TYPE_FLEXRAY )
  #error "XCP Transport Layer Error: Only one XCP Transport Layer is supported!"
#endif

#if defined ( XCP_TRANSPORT_LAYER_TYPE_CAN ) && defined ( XCP_TRANSPORT_LAYER_TYPE_CAN_ASR )
  #error "XCP Transport Layer Error: Only one XCP Transport Layer is supported!"
#endif
#if defined ( XCP_TRANSPORT_LAYER_TYPE_FLEXRAY ) && defined ( XCP_TRANSPORT_LAYER_TYPE_CAN_ASR )
  #error "XCP Transport Layer Error: Only one XCP Transport Layer is supported!"
#endif
#if defined ( XCP_TRANSPORT_LAYER_TYPE_LIN ) && defined ( XCP_TRANSPORT_LAYER_TYPE_CAN_ASR )
  #error "XCP Transport Layer Error: Only one XCP Transport Layer is supported!"
#endif


/****************************************************************************/
/* Definitions                                                              */
/****************************************************************************/

/* Definition of endianess. */
#if defined ( XCP_CPUTYPE_BIGENDIAN ) || defined ( XCP_CPUTYPE_LITTLEENDIAN )
#else
  #if defined ( C_CPUTYPE_BIGENDIAN )
    #define XCP_CPUTYPE_BIGENDIAN
  #endif
  #if defined ( C_CPUTYPE_LITTLEENDIAN )
    #define XCP_CPUTYPE_LITTLEENDIAN
  #endif
  #if defined ( CPU_BYTE_ORDER )
    #if ( CPU_BYTE_ORDER == HIGH_BYTE_FIRST )
      #define XCP_CPUTYPE_BIGENDIAN
    #endif
    #if ( CPU_BYTE_ORDER == LOW_BYTE_FIRST )
      #define XCP_CPUTYPE_LITTLEENDIAN
    #endif
  #endif
#endif




/****************************************************************************/
/* Macros                                                                   */
/****************************************************************************/

/* Definition of macros that have to be used within the context of XcpCommand. */
/* They have to be declared global Due to MISRA rule 91. */

#define error(e) { err=(e); goto negative_response; }
#define check_error(e) { err=(e); if (err!=0) { goto negative_response; } }
#define error1(e,b1) { err=(e); CRM_BYTE(2)=(b1); xcp.CrmLen=3; goto negative_response1; }
#define error2(e,b1,b2) { err=(e); CRM_BYTE(2)=(b1); CRM_BYTE(3)=(b2); xcp.CrmLen=4; goto negative_response1; }

#if defined ( XCP_ENABLE_SEED_KEY )
  /* Return CRC_ACCESS_LOCKED if the resource is locked. */
  #define CheckResourceProtection(resource)   if ( (xcp.ProtectionStatus & (resource)) != 0 ) { error(CRC_ACCESS_LOCKED) }
#else
  /* The resource protection is unavailable. */
  #define CheckResourceProtection(resource)
#endif


#if defined ( XCP_ENABLE_AUTOSAR_CRC_MODULE )
  /* Macros for calculation of checksum with AUTOSAR CRC module. */

  #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_CRC16CCITT )
    #define XcpCalculateCrc(data_ptr, length, start_value)  Crc_CalculateCRC16(data_ptr, length, start_value)
  #endif

  #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_CRC32 )
    #define XcpCalculateCrc(data_ptr, length, start_value)  Crc_CalculateCRC32(data_ptr, length, start_value)
  #endif

#endif  /* defined ( XCP_ENABLE_AUTOSAR_CRC_MODULE ) */

#if defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL )
  #define XCP_WRITE_BYTE_2_ADDR(addr, data)           ApplXcpWrite( (vuint32)(addr), (vuint8)(data) )
  #define XCP_READ_BYTE_FROM_ADDR(addr)               ApplXcpRead ( (vuint32)(addr) )
  #if defined ( XCP_ENABLE_CHECKSUM )
     #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD22 ) || \
         ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD24 ) || \
         ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD44 )
      #define XCP_READ_CHECKSUMVALUE_FROM_ADDR(addr)  ApplXcpReadChecksumValue ( (vuint32)(addr) )
    #else
      #define XCP_READ_CHECKSUMVALUE_FROM_ADDR(addr)  XCP_READ_BYTE_FROM_ADDR(addr)
    #endif
  #endif
#else
  #define XCP_WRITE_BYTE_2_ADDR(addr, data)           *(addr) = (data)
  #define XCP_READ_BYTE_FROM_ADDR(addr)               *(addr)
  #if defined ( XCP_ENABLE_CHECKSUM )
    #define XCP_READ_CHECKSUMVALUE_FROM_ADDR(addr)    *((tXcpChecksumAddType*)addr)
  #endif
#endif

#if defined ( XCP_ENABLE_CONTROL )
  #define XcpPlCheckControlState( )               { if ( XcpPlControlState == kXcpControl_Disable ) { return; } }
  #define XcpPlCheckControlStateRet( ret_value )  { if ( XcpPlControlState == kXcpControl_Disable ) { return( ret_value ); } }
#else
  #define XcpPlCheckControlState( )
  #define XcpPlCheckControlStateRet( ret_value )
#endif

/****************************************************************************/
/* Constants                                                                */
/****************************************************************************/

/****************************************************************************/
/* 8 Bit Constants for export                                               */
/****************************************************************************/

/* Global constants with XCP Protocol Layer main and subversion */
V_MEMROM0 MEMORY_ROM vuint8 kXcpMainVersion    = (vuint8)(CP_XCP_VERSION >> 8);
V_MEMROM0 MEMORY_ROM vuint8 kXcpSubVersion     = (vuint8)(CP_XCP_VERSION & 0x00ff);
V_MEMROM0 MEMORY_ROM vuint8 kXcpReleaseVersion = (vuint8)(CP_XCP_RELEASE_VERSION);



/****************************************************************************/
/* Local data                                                               */
/****************************************************************************/

/* This section containes all RAM locations needed by the XCP driver */

/******************************************************************************/
/* Local Data definitions with unspecified size                               */
/******************************************************************************/

static RAM tXcpData xcp;



/******************************************************************************/
/* Local Data definitions with 8Bit size                                      */
/******************************************************************************/

#if defined ( XCP_ENABLE_CONTROL )
static vuint8 XcpPlControlState;
#endif



/***************************************************************************/
/* Prototypes for local functions                                          */
/***************************************************************************/

static vuint8 XcpWriteMta( vuint8 size, MEMORY_ROM BYTEPTR data );
static vuint8 XcpReadMta( vuint8 size, BYTEPTR data );

#if defined ( XcpMemClr )
#else
static void XcpMemClr( BYTEPTR p, vuint16 n );
#endif

#if defined ( XCP_ENABLE_SEND_EVENT ) || defined ( XCP_ENABLE_SERV_TEXT )
static void XcpSendEv( void );
#endif

#if defined ( XCP_ENABLE_SEND_QUEUE )
static vuint8 XcpSendDtoFromQueue( void );
static void XcpQueueInit( void );
#endif

#if defined ( XCP_ENABLE_DAQ )
static void XcpFreeDaq( void );
static vuint8 XcpAllocMemory( void );
static vuint8 XcpAllocDaq( vuint8 daqCount );
static vuint8 XcpAllocOdt( vuint8 daq, vuint8 odtCount );
static vuint8 XcpAllocOdtEntry( vuint8 daq, vuint8 odt, vuint8 odtEntryCount );
static void XcpStartDaq( vuint8 daq );
static void XcpStartAllSelectedDaq( void );
static void XcpStopDaq( vuint8 daq );
static void XcpStopAllSelectedDaq( void );
static void XcpStopAllDaq( void );

  #if defined ( XCP_ENABLE_STIM )
static vuint8 XcpAllocStimMemory( void );
  #endif

#endif



/******************************************************************************
*
*
*           Code Part for Cp_Xcp
*
*
*******************************************************************************/


/*****************************************************************************
| NAME:             XcpMemSet
| CALLED BY:        XcpFreeDaq
| PRECONDITIONS:    none
| INPUT PARAMETERS: p : pointer to start address.
|                   n : number of data bytes.
|                   d : data byte to initialize with.
| RETURN VALUES:    none 
| DESCRIPTION:      Initialize n bytes starting from address p with b.
******************************************************************************/
                                 
#if defined ( XcpMemSet )
 /* XcpMemSet is overwritten */
#else
void XcpMemSet( BYTEPTR p, vuint16 n, vuint8 b )
{
  for ( ; n > 0; n-- )
  {
    *p = b;
    p++;
  }
}                
#endif

/*****************************************************************************
| NAME:             XcpMemClr
| CALLED BY:        XcpFreeDaq, XcpInit
| PRECONDITIONS:    none
| INPUT PARAMETERS: p : pointer to start address.
|                   n : number of data bytes.
| RETURN VALUES:    none 
| DESCRIPTION:      Initialize n bytes starting from address p 0.
******************************************************************************/

#if defined ( XcpMemClr )
 /* XcpMemClr is overwritten */
#else
/* A macro would be more efficient. Due to MISRA rule 96 violations a function is implemented. */
static void XcpMemClr( BYTEPTR p, vuint16 n )
{
  XcpMemSet( p, n, (vuint8)0u);
}
#endif

/*****************************************************************************
| NAME:             XcpMemCpy
| CALLED BY:        XcpEvent
| PRECONDITIONS:    none
| INPUT PARAMETERS: dest : pointer to destination address.
|                   src  : pointer to source address.
|                   n    : number of data bytes to copy.
| RETURN VALUES:    none 
| DESCRIPTION:      Copy n bytes from src to dest.
|                   A maximum of 255 Bytes can be copied at once.
******************************************************************************/

/* Optimize this function 
   It is used in the inner loop of XcpEvent for data acquisition sampling 
*/

#if defined ( XcpMemCpy ) || defined ( C_COMP_COSMIC_MCS12X_MSCAN12 )
 /* XcpMemCpy is overwritten */
#else
void XcpMemCpy( DAQBYTEPTR dest, MEMORY_ROM DAQBYTEPTR src, vuint8 n )
{
  for ( ; n > 0; n-- )
  {
    XCP_WRITE_BYTE_2_ADDR( dest, *src );
    dest++;
    src++;
  }
}
#endif


/****************************************************************************/
/* Transmit                                                                 */
/****************************************************************************/


/*****************************************************************************
| NAME:             XcpSendCrm
| CALLED BY:        XcpBackground, XcpCommand, XcpSendCallBack, application
| PRECONDITIONS:    XCP is initialized and in connected state and 
|                   a command packet (CMD) has been received.
| INPUT PARAMETERS: none
| RETURN VALUES:    none 
| DESCRIPTION:      Transmission of a command response packet (RES), 
|                    or error packet (ERR) if no other packet is pending.
******************************************************************************/
void XcpSendCrm( void )
{
  /* Activation control */
  XcpPlCheckControlState()

#if defined ( XCP_ENABLE_SEND_QUEUE )

  ApplXcpInterruptDisable();

  if ( (xcp.SendStatus & (vuint8)XCP_SEND_PENDING) != 0 )
  {
    if ( (xcp.SendStatus & (vuint8)XCP_CRM_REQUEST) != 0 )
    {
      XCP_ASSERT(0);
      xcp.SessionStatus |= (vuint8)SS_ERROR; 
    }
    xcp.SendStatus |= (vuint8)XCP_CRM_REQUEST;
  } 
  else
  {
    xcp.SendStatus |= (vuint8)XCP_CRM_PENDING;
    ApplXcpSend(xcp.CrmLen,&xcp.Crm.b[0]);
  }

  ApplXcpInterruptEnable();

#else

  ApplXcpSend(xcp.CrmLen,&xcp.Crm.b[0]);

#endif

  ApplXcpSendFlush();

}

#if defined ( XCP_ENABLE_SEND_EVENT ) || defined ( XCP_ENABLE_SERV_TEXT )
/*****************************************************************************
| NAME:             XcpSendEv
| CALLED BY:        XcpSendEvent, XcpSendCallBack, XcpPutchar
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    none 
| DESCRIPTION:      Send a EV or SERV message, if no other message is pending.
******************************************************************************/
static void XcpSendEv( void )
{      
  #if defined ( XCP_ENABLE_SEND_QUEUE )

  ApplXcpInterruptDisable();

  if ( (xcp.SendStatus & (vuint8)XCP_SEND_PENDING) != 0 )
  {
    if ( (xcp.SendStatus & (vuint8)XCP_EVT_REQUEST) != 0 )
    {
      XCP_ASSERT(0);
      xcp.SessionStatus |= (vuint8)SS_ERROR; 
    }
    xcp.SendStatus |= (vuint8)XCP_EVT_REQUEST;
  }
  else
  {
    xcp.SendStatus |= (vuint8)XCP_EVT_PENDING;
    ApplXcpSend(xcp.EvLen,xcp.Ev.b);
    xcp.EvLen = 0;
  }

  ApplXcpInterruptEnable();

  #else

  ApplXcpSend(xcp.EvLen,xcp.Ev.b);
  xcp.EvLen = 0;
    
  #endif

  ApplXcpSendFlush();

}
#endif /* XCP_ENABLE_SEND_EVENT || XCP_ENABLE_SERV_TEXT */


#if defined ( XCP_ENABLE_DAQ )
/*****************************************************************************
| NAME:             XcpSendDto
| CALLED BY:        XcpSendDtoFromQueue, XcpEvent, XcpSendCallBack
| PRECONDITIONS:    none
| INPUT PARAMETERS: dto : pointer to XCP packet type definition
| RETURN VALUES:    none 
| DESCRIPTION:      Send a DTO.
******************************************************************************/
  #if defined ( XcpSendDto )
  /* XcpSendDto is redefined */
  #else
void XcpSendDto( MEMORY_ROM tXcpDto *dto )
{
#if defined ( XCP_ALLIGN_WORD )
  ApplXcpSend( dto->l, &dto->XcpDtoByteField.b[0] );
#else
  ApplXcpSend( dto->l, &dto->b[0] );
#endif
}
  #endif
#endif /* XCP_ENABLE_DAQ */


#if defined ( XCP_ENABLE_SEND_QUEUE )
/*****************************************************************************
| NAME:             XcpSendDtoFromQueue
| CALLED BY:        XcpEvent, XcpSendCallBack
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    0 : DTO has NOT been transmitted from queue. 
|                   1 : DTO has been transmitted from queue. 
| DESCRIPTION:      Send a DTO from the queue.
******************************************************************************/
static vuint8 XcpSendDtoFromQueue( void )
{
  ApplXcpInterruptDisable();
  if ( ( (xcp.SendStatus & (vuint8)XCP_SEND_PENDING) == 0 ) && ( xcp.QueueLen != 0 ))
  {
    xcp.SendStatus |= (vuint8)XCP_DTO_PENDING;
    XcpSendDto(&xcp.pQueue[xcp.QueueRp]);
    xcp.QueueRp++;
    if ( (vuint8)(xcp.QueueRp>=xcp.QueueSize) != (vuint8)0u )
    {
      xcp.QueueRp = 0;
    }
    xcp.QueueLen--;
    ApplXcpInterruptEnable();
    return (vuint8)1u;
  }
  ApplXcpInterruptEnable();
  return (vuint8)0u;
} /* Deviation of MISRA rule 82 (more than one return path). */
#endif /* XCP_ENABLE_SEND_QUEUE */


/****************************************************************************/
/* Transmit Queue */
/****************************************************************************/

#if defined ( XCP_ENABLE_SEND_QUEUE )

/*****************************************************************************
| NAME:             XcpQueueInit
| CALLED BY:        XcpFreeDaq, XcpStopDaq, XcpStopAllDaq
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    none 
| DESCRIPTION:      Initialize the transmit queue.
******************************************************************************/
static void XcpQueueInit(void)
{

  xcp.QueueLen = (vuint8)0u;
  xcp.QueueRp = (vuint8)0u;
}


#endif /* XCP_ENABLE_SEND_QUEUE */


/****************************************************************************/
/* Handle Mta (Memory-Transfer-Address) */
/****************************************************************************/

/* Assign a pointer to a Mta */
#if defined ( XcpSetMta )
#else
    #define XcpSetMta(p,e) (xcp.Mta = (p))
#endif

/*****************************************************************************
| NAME:             XcpWriteMta
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: size : number of data bytes.
|                   data : address of data.
| RETURN VALUES:    XCP_CMD_OK, XCP_CMD_DENIED
| DESCRIPTION:      Write n bytes.
|                   Copying of size bytes from data to xcp.Mta
******************************************************************************/
static vuint8 XcpWriteMta( vuint8 size, MEMORY_ROM BYTEPTR data )
{
#if defined ( XCP_ENABLE_WRITE_EEPROM ) || defined ( XCP_ENABLE_WRITE_PROTECTION )
  vuint8 r;
#endif

  /* DPRAM Client */

  /* EEPROM write access */
#if defined ( XCP_ENABLE_WRITE_EEPROM )
  r = ApplXcpCheckWriteEEPROM(xcp.Mta,size,data);
  if ( r != (vuint8)XCP_CMD_DENIED )  /* EEPROM write access */
  {
    xcp.Mta += size;
    return r;
  }
#endif

  /* Checked ram memory write access */
#if defined ( XCP_ENABLE_WRITE_PROTECTION )
  r = ApplXcpCheckWriteAccess(xcp.Mta,size);
  if ( r == (vuint8)XCP_CMD_DENIED )
  {
    xcp.Mta += size;
    return (vuint8)XCP_CMD_DENIED;
  }
#endif

  /* Standard RAM memory write access */
  while ( size > (vuint8)0u ) 
  {
    XCP_WRITE_BYTE_2_ADDR( xcp.Mta, *data );
    xcp.Mta++;
    data++;
    size--;
  }
  return (vuint8)XCP_CMD_OK;
}

/*****************************************************************************
| NAME:             XcpReadMta
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: size :
|                   data : address of data
| RETURN VALUES:    XCP_CMD_OK
| DESCRIPTION:      Read n bytes.
|                   Copying of size bytes from data to xcp.Mta
******************************************************************************/
static vuint8 XcpReadMta( vuint8 size, BYTEPTR data )
{
#if defined ( XCP_ENABLE_READ_EEPROM ) || defined ( XCP_ENABLE_READ_PROTECTION )
  vuint8 r;
#endif

  /* DPRAM Client */

  /* EEPROM read access */
#if defined ( XCP_ENABLE_READ_EEPROM )
  r = ApplXcpCheckReadEEPROM(xcp.Mta,size,data);
  if ( r != (vuint8)XCP_CMD_DENIED )
  {
    xcp.Mta += size;
    return (vuint8)r; /* ESCAN00014775, ESCAN00016958 */
  }
#endif

  /* Checked ram memory write access */
#if defined ( XCP_ENABLE_READ_PROTECTION )
  r = ApplXcpCheckReadAccess(xcp.Mta,size);
  if ( r == (vuint8)XCP_CMD_DENIED )
  {
    xcp.Mta += size;
    return (vuint8)XCP_CMD_DENIED;
  }
#endif

  /* Standard RAM memory read access */
  while (size > 0)
  {
    /* 
       Compiler bug Tasking
       *(data++) = *(xcp.Mta++);
    */
    *(data) = XCP_READ_BYTE_FROM_ADDR( xcp.Mta );
    data++;
    xcp.Mta++;
    size--;
  }
  return (vuint8)XCP_CMD_OK;
}


/****************************************************************************/
/* Data Aquisition Setup                                                    */
/****************************************************************************/


#if defined ( XCP_ENABLE_DAQ )

/*****************************************************************************
| NAME:             XcpFreeDaq
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Free all dynamic DAQ lists
******************************************************************************/
static void XcpFreeDaq( void )
{
  xcp.SessionStatus &= (vuint8)((~SS_DAQ) & 0xFFu);

  xcp.Daq.DaqCount = 0;
  xcp.Daq.OdtCount = 0;
  xcp.Daq.OdtEntryCount = 0;

  xcp.pOdt = (tXcpOdt*)0;
  xcp.pOdtEntryAddr = 0;
  xcp.pOdtEntrySize = 0;

  XcpMemClr((BYTEPTR)&xcp.Daq.u.b[0], (vuint16)kXcpDaqMemSize);  /* Deviation of MISRA rule 44. */
  #if defined ( kXcpMaxEvent ) && ! defined ( XCP_ENABLE_STIM )
    XcpMemSet( (BYTEPTR)&xcp.Daq.EventDaq[0], (vuint16)sizeof(xcp.Daq.EventDaq), (vuint8)0xFFu);  /* Deviation of MISRA rule 44. */
  #endif

  #if defined ( XCP_ENABLE_SEND_QUEUE )
  XcpQueueInit();
  #endif
}

/*****************************************************************************
| NAME:             XcpAllocMemory
| CALLED BY:        XcpAllocDaq, XcpAllocOdt, XcpAllocOdtEntry, XcpInit
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    0, CRC_MEMORY_OVERFLOW
| DESCRIPTION:      Allocate Memory for daq,odt,odtEntries and Queue
|                   according to DaqCount, OdtCount and OdtEntryCount
******************************************************************************/
static vuint8 XcpAllocMemory( void )
{
  vuint16 s;
  #if defined ( XCP_ENABLE_NO_P2INT_CAST  )
  vuint8* p;
  vuint8  i;
  #endif

  /* Check memory overflow */
  s = (vuint16)( ( xcp.Daq.DaqCount      *   (vuint8)sizeof(tXcpDaqList)                           ) + 
                 ( xcp.Daq.OdtCount      *  (vuint16)sizeof(tXcpOdt)                               ) + 
                 ( xcp.Daq.OdtEntryCount * ( (vuint8)sizeof(DAQBYTEPTR) + (vuint8)sizeof(vuint8) ) )
               );
  #if defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS )
  s += 3; /* Worst case 3 bytes needed for alignment */
  #endif
  if (s>=(vuint16)kXcpDaqMemSize) 
  {
    return (vuint8)CRC_MEMORY_OVERFLOW;
  }

  /* Force WORD alignment for ODTs */
  #if defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS )
    #if defined ( XCP_ENABLE_NO_P2INT_CAST  )
  p = (vuint8*)&xcp.Daq.u.DaqList[xcp.Daq.DaqCount];
  i = ((vuint8)p) & (vuint8)0x01u;
  p += i;
  xcp.pOdt = (tXcpOdt*)p;
    #else
      #if defined ( C_CPUTYPE_8BIT ) || defined ( C_CPUTYPE_16BIT )
  /* Allign to words */
  xcp.pOdt = (tXcpOdt*)((((vuint32)(&xcp.Daq.u.DaqList[xcp.Daq.DaqCount])) + (vuint32)1u) & (vuint32)0xFFFFFFFEu );
      #else
  /* Allign to dwords */
  xcp.pOdt = (tXcpOdt*)((((vuint32)(&xcp.Daq.u.DaqList[xcp.Daq.DaqCount])) + (vuint32)3u) & (vuint32)0xFFFFFFFCu );
      #endif
    #endif
  #else
  xcp.pOdt = (tXcpOdt*)&xcp.Daq.u.DaqList[xcp.Daq.DaqCount];
  #endif
  
  /* Force DWORD alignment for ODT entries */
  #if defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS )
    #if defined ( XCP_ENABLE_NO_P2INT_CAST  )
  p = (vuint8*)&xcp.pOdt[xcp.Daq.OdtCount];
  i = ((vuint8)p) & (vuint8)0x03u;
  i = (vuint8)4u - i;
  i &= 0x03;
  p += i;
  xcp.pOdtEntryAddr = (DAQBYTEPTR*)p;
    #else
  xcp.pOdtEntryAddr = (DAQBYTEPTR*) ((((vuint32)&xcp.pOdt[xcp.Daq.OdtCount]) + (vuint32)3u) & (vuint32)0xFFFFFFFCu );
    #endif
  #else
  xcp.pOdtEntryAddr = (DAQBYTEPTR*)&xcp.pOdt[xcp.Daq.OdtCount];
  #endif
  xcp.pOdtEntrySize = (vuint8*)&xcp.pOdtEntryAddr[xcp.Daq.OdtEntryCount];
  
  #if defined ( XCP_ENABLE_SEND_QUEUE )
  /* Force WORD alignment for the queue */
    #if defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS )
    #if defined ( XCP_ENABLE_NO_P2INT_CAST  )
  p = (vuint8*)&xcp.pOdtEntrySize[xcp.Daq.OdtEntryCount];
  i = ((vuint8)p) & (vuint8)0x01u;
  p += i;
  xcp.pQueue = (tXcpDto*)p;
    #else
      #if defined ( C_CPUTYPE_8BIT ) || defined ( C_CPUTYPE_16BIT )
  /* Allign to words */
  xcp.pQueue = (tXcpDto*)((((vuint32)(&xcp.pOdtEntrySize[xcp.Daq.OdtEntryCount])) + (vuint32)1u) & (vuint32)0xFFFFFFFEu );      
      #else
  /* Allign to dwords */
  xcp.pQueue = (tXcpDto*)((((vuint32)(&xcp.pOdtEntrySize[xcp.Daq.OdtEntryCount])) + (vuint32)3u) & (vuint32)0xFFFFFFFCu );
      #endif
    #endif
    #else
  xcp.pQueue = (tXcpDto*)&xcp.pOdtEntrySize[xcp.Daq.OdtEntryCount];
    #endif
  xcp.QueueSize = (vuint8)(( ( ((vuint16)kXcpDaqMemSize) - s) / (vuint16)sizeof(tXcpDto) )&0x00FFu);
  if (xcp.QueueSize>=128) 
  {
    xcp.QueueSize = 127;  /* Maximum size because of the modulo operation in XcpQueueWrite */
  }
    #if defined ( kXcpSendQueueMinSize )
  if (xcp.QueueSize<(vuint8)kXcpSendQueueMinSize)
  {
    return (vuint8)CRC_MEMORY_OVERFLOW;
  }
    #else
  /* At least one queue entry per odt */
  if (xcp.QueueSize<xcp.Daq.OdtCount)
  {
    return (vuint8)CRC_MEMORY_OVERFLOW;
  }
    #endif
  #endif /* XCP_ENABLE_SEND_QUEUE */


  return (vuint8)0u;
} /* Deviation of MISRA rule 82 (more than one return path). */

  #if defined ( XCP_ENABLE_STIM )
/*****************************************************************************
| NAME:             XcpAllocStimMemory
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    0, CRC_MEMORY_OVERFLOW
| DESCRIPTION:      Allocate Memory for daq,odt,odtEntries and Queue
|                   according to DaqCount, OdtCount and OdtEntryCount
******************************************************************************/
static vuint8 XcpAllocStimMemory( void )
{
  vuint8 daq;
  tXcpOdtIdx odt, n;

  n = 0;
  for (daq=0; daq<xcp.Daq.DaqCount; daq++)
  {
    for (odt=DaqListFirstOdt(daq); odt<=DaqListLastOdt(daq); odt++)
    {
      if ( (DaqListFlags(daq)&(vuint8)DAQ_FLAG_DIRECTION) != 0x00u )
      {
        if (n >= (vuint8)kXcpStimOdtCount)
        {
          return (vuint8)CRC_MEMORY_OVERFLOW;
        }
    #if defined ( XCP_ALLIGN_WORD )
        xcp.StimBuffer[n].XcpDtoByteField.b[0] = (vuint8)0xFFu; /* Invalidate this buffer */
    #else
        xcp.StimBuffer[n].b[0] = (vuint8)0xFFu; /* Invalidate this buffer */
    #endif
        DaqListOdtStimBuffer(odt) = &xcp.StimBuffer[n];
        n++;
      } 
      else
      {
        DaqListOdtStimBuffer(odt) = 0;
      }
    } /* odt */
  } /* daq */

  return (vuint8)0u;
}
  #endif


/*****************************************************************************
| NAME:             XcpAllocDaq
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: daqCount : 
| RETURN VALUES:    return value of XcpAllocMemory, CRC_SEQUENCE
| DESCRIPTION:      Allocate DAQ list
******************************************************************************/
static vuint8 XcpAllocDaq( vuint8 daqCount )
{
  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
  if ( (xcp.Daq.OdtCount!=0) || (xcp.Daq.OdtEntryCount!=0) )
  {
    return (vuint8)CRC_SEQUENCE;
  }
  #endif

  xcp.Daq.DaqCount = daqCount;
#if defined ( XCP_ALLIGN_WORD )
  xcp.Daq.DaqCount &= 0x00FFu;
#endif

  return XcpAllocMemory();
} /* deviation of MISRA rule 82 (more than one return path). */

/*****************************************************************************
| NAME:             XcpAllocOdt
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: daq      : 
|                   odtCount :
| RETURN VALUES:    return value of XcpAllocMemory,
|                   CRC_SEQUENCE, CRC_MEMORY_OVERFLOW
| DESCRIPTION:      Allocate all ODTs in a DAQ list
******************************************************************************/
static vuint8 XcpAllocOdt( vuint8 daq, vuint8 odtCount )
{
  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
  if ( (xcp.Daq.DaqCount==0) || (xcp.Daq.OdtEntryCount!=0) )
  {
    return (vuint8)CRC_SEQUENCE;
  }
  #endif

  /* Absolute ODT count must fit in a BYTE */
  #if !defined ( XCP_ENABLE_DAQ_HDR_ODT_DAQ )
  if (((vuint16)xcp.Daq.OdtCount+(vuint16)odtCount) > (vuint16)0xFBu)
  {
    return (vuint8)CRC_MEMORY_OVERFLOW;
  }
  #endif

  xcp.Daq.u.DaqList[daq].firstOdt = xcp.Daq.OdtCount;
  xcp.Daq.OdtCount += odtCount;
  #if defined ( XCP_ALLIGN_WORD )
  xcp.Daq.OdtCount &= 0x00FFu;
  #endif
  xcp.Daq.u.DaqList[daq].lastOdt = (tXcpOdtIdx)(xcp.Daq.OdtCount-1);

  return XcpAllocMemory();
} /* Deviation of MISRA rule 82 (more than one return path). */

/*****************************************************************************
| NAME:             XcpAllocOdtEntry
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: daq :
|                   odt :
|                   odtEntryCount :
| RETURN VALUES:    return value of XcpAllocMemory
| DESCRIPTION:      Allocate all ODT entries
|                   Parameter odt is relative odt number
******************************************************************************/
static vuint8 XcpAllocOdtEntry( vuint8 daq, vuint8 odt, vuint8 odtEntryCount )
{
  tXcpOdtIdx xcpFirstOdt;

  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
  if ( (xcp.Daq.DaqCount==0) || (xcp.Daq.OdtCount==0) )
  {
    return (vuint8)CRC_SEQUENCE;
  }
  #endif

  /* Absolute ODT entry count count must fit in a WORD */
  if (xcp.Daq.OdtEntryCount > (0xFFFFu - odtEntryCount))
  {
    return (vuint8)CRC_MEMORY_OVERFLOW;
  }
  xcpFirstOdt = xcp.Daq.u.DaqList[daq].firstOdt;
  xcp.pOdt[xcpFirstOdt+odt].firstOdtEntry = xcp.Daq.OdtEntryCount;
  xcp.Daq.OdtEntryCount += (vuint16)odtEntryCount;
  xcp.pOdt[xcpFirstOdt+odt].lastOdtEntry = (vuint16)(xcp.Daq.OdtEntryCount-1);

  return XcpAllocMemory();
} /* Deviation of MISRA rule 82 (more than one return path). */

/*****************************************************************************
| NAME:             XcpStartDaq
| CALLED BY:        XcpCommand, XcpStartAllSelectedDaq
| PRECONDITIONS:    none
| INPUT PARAMETERS: daq :
| RETURN VALUES:    none
| DESCRIPTION:      Start DAQ
******************************************************************************/
static void XcpStartDaq( vuint8 daq )
{
  /* Initialize the DAQ list */
  DaqListFlags(daq) |= (vuint8)DAQ_FLAG_RUNNING;
  #if defined ( XCP_ENABLE_DAQ_PRESCALER )
  DaqListCycle(daq) = 1;
  #endif

  xcp.SessionStatus |= (vuint8)SS_DAQ;
}

/*****************************************************************************
| NAME:             XcpStartAllSelectedDaq
| CALLED BY:        XcpCommand, XcpInit
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Start all selected DAQs
******************************************************************************/
static void XcpStartAllSelectedDaq(void)
{
  vuint8 daq;

  /* Start all selected DAQs */
  for (daq=0;daq<xcp.Daq.DaqCount;daq++)
  {
    if ( (DaqListFlags(daq) & (vuint8)DAQ_FLAG_SELECTED) != 0 )
    {
      XcpStartDaq(daq);
      DaqListFlags(daq) &= (vuint8)(~DAQ_FLAG_SELECTED & 0x00FFu);
    }
  }
}

/*****************************************************************************
| NAME:             XcpStopDaq
| CALLED BY:        XcpCommand, XcpStopAllSelectedDaq
| PRECONDITIONS:    none
| INPUT PARAMETERS: daq : 
| RETURN VALUES:    none
| DESCRIPTION:      Stop DAQ
******************************************************************************/
static void XcpStopDaq( vuint8 daq )
{
  vuint8 i;

  DaqListFlags(daq) = 0;

  /* Check if all DAQ lists are stopped */
  for (i=0;i<xcp.Daq.DaqCount;i++)
  {
    if ( (DaqListFlags(i) & (vuint8)DAQ_FLAG_RUNNING) != 0 )
    {
      return;
    }
  }

  xcp.SessionStatus &= (vuint8)(~SS_DAQ & 0x00FFu);

  #if defined ( XCP_ENABLE_SEND_QUEUE )
  XcpQueueInit();
  #endif
}

/*****************************************************************************
| NAME:             XcpStopAllSelectedDaq
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      Stop all selected DAQs
******************************************************************************/
static void XcpStopAllSelectedDaq(void)
{
  vuint8 daq;

  for (daq=0;daq<xcp.Daq.DaqCount;daq++)
  {
    if ( (DaqListFlags(daq) & (vuint8)DAQ_FLAG_SELECTED) != 0 )
    {
      XcpStopDaq(daq);
      DaqListFlags(daq) &= (vuint8)(~DAQ_FLAG_SELECTED & 0x00FFu);
    }
  }
}

/*****************************************************************************
| NAME:             XcpStopAllDaq
| CALLED BY:        XcpCommand, XcpDisconnect
| PRECONDITIONS:    none
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      Stop all DAQs
******************************************************************************/
static void XcpStopAllDaq( void )
{
  vuint8 daq;

  for (daq=0; daq<xcp.Daq.DaqCount; daq++)
  {
    DaqListFlags(daq) = 0;
  }

  xcp.SessionStatus &= (vuint8)(~SS_DAQ & 0x00FFu);

  #if defined ( XCP_ENABLE_SEND_QUEUE )
  XcpQueueInit();
  #endif
}


/****************************************************************************/
/* Data Aquisition Processor                                                */
/****************************************************************************/

/*****************************************************************************
| NAME:             XcpStimEventStatus
| CALLED BY:        application
| PRECONDITIONS:    The XCP is initialized and in connected state.
| INPUT PARAMETERS: event : event channel number to process
|                   action : 
| RETURN VALUES:    1 (TRUE) if STIM data is available and XcpEvent() can be called
| DESCRIPTION:      Handling of data acquisition event channel.
******************************************************************************/
  #if defined ( XCP_ENABLE_STIM )
vuint8 XcpStimEventStatus( vuint8 event , vuint8 action )
{
  vuint16 e;
  vuint8 *d, daq;
  tXcpOdtIdx odt;

  /* Activation control */
  XcpPlCheckControlStateRet((vuint8)0u)

  if ( (xcp.SessionStatus & (vuint8)SS_DAQ) == 0 )
  {
    return (vuint8)0u ;
  }

  for (daq=0; daq<xcp.Daq.DaqCount; daq++)
  {
    if ( (DaqListFlags(daq) & (vuint8)DAQ_FLAG_RUNNING) == 0 )
    {
      continue;
    }
    if ( DaqListEventChannel(daq) != event )
    {
      continue;
    }

    if ( (DaqListFlags(daq) & (vuint8)DAQ_FLAG_DIRECTION) != 0 ) /* STIM */
    {
      for (odt=DaqListFirstOdt(daq);odt<=DaqListLastOdt(daq);odt++)
      {
        e = DaqListOdtFirstEntry(odt);
        if ( OdtEntrySize(e) == (vuint8)0u )
        {
          continue;  
        }
        d = (vuint8*)DaqListOdtStimBuffer(odt);
        XCP_ASSERT(d!=0);
        if (action == (vuint8)STIM_CHECK_ODT_BUFFER)
        {
          if ( *d == (vuint8)0xFFu ) 
          {
            return (vuint8)0u; /* No STIM data available */
          } 
        }
        else /* STIM_RESET_ODT_BUFFER */
        {
          *d = (vuint8)0xFFu;
        }

      } /* odt */

    } 
  
  } /* daq */

  return (vuint8)1u;

}
  #endif

/*****************************************************************************
| NAME:             XcpEvent
| CALLED BY:        application
| PRECONDITIONS:    The XCP is initialized and in connected state.
| INPUT PARAMETERS: event : event channel number to process
| RETURN VALUES:    status code (XCP_EVENT_xxxx)
| DESCRIPTION:      Handling of data acquisition or stimulation event channel.
******************************************************************************/
vuint8 XcpEvent( vuint8 event )
{
  vuint8 status;
  vuint16 e,el;     
  vuint8  *d;       
  vuint8  n;        
  vuint8 daq;
  tXcpOdtIdx odt;
  vuint8 i;
  tXcpDto *dtop;
  #if defined ( XCP_ENABLE_SEND_QUEUE )
  #else
  static tXcpDto dto; /* ESCAN00025020 */
  #endif
  #if defined ( XCP_ENABLE_DAQ_TIMESTAMP ) && defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS )
  XcpDaqTimestampType t;
  #endif
  #if defined ( C_COMP_COSMIC_MCS12X_MSCAN12 )
  DAQBYTEPTR src;
  #endif
 
  /* Activation control */
  XcpPlCheckControlStateRet((vuint8)XCP_EVENT_NOP)

  status = (vuint8)0u;

  if ( (xcp.SessionStatus & (vuint8)SS_DAQ) == 0 )
  {
    return (vuint8)XCP_EVENT_NOP;
  }

  #if defined ( kXcpMaxEvent ) && ! defined( XCP_ENABLE_STIM )

    #if defined ( XCP_ENABLE_PARAMETER_CHECK )
  if (event >= (vuint8)kXcpMaxEvent)
  {
    return (vuint8)XCP_EVENT_NOP;
  }
    #endif

  BEGIN_PROFILE(4); /* Timingtest */
  daq = xcp.Daq.EventDaq[event];
  if ( ((daq<xcp.Daq.DaqCount) && ( (DaqListFlags(daq) & (vuint8)DAQ_FLAG_RUNNING) != 0 )) != 0 )
  {

  #else

  BEGIN_PROFILE(4); /* Timingtest */
  for (daq=0; daq<xcp.Daq.DaqCount; daq++)
  {
    if ( (DaqListFlags(daq)& (vuint8)DAQ_FLAG_RUNNING) == 0 )
    {
      continue;
    }
    if ( DaqListEventChannel(daq) != event )
    {
      continue;
    }

  #endif

  #if defined ( XCP_ENABLE_DAQ_PRESCALER )
    DaqListCycle(daq)--;
    if ( DaqListCycle(daq) == (vuint8)0 )
    {
      DaqListCycle(daq) = DaqListPrescaler(daq);
  #endif

  /* Data Stimulation (STIM) */
  #if defined ( XCP_ENABLE_STIM )

    if ( (DaqListFlags(daq)&(vuint8)DAQ_FLAG_DIRECTION) != 0 ) /* STIM */
    {
      status |= (vuint8)XCP_EVENT_STIM;
      for (odt=DaqListFirstOdt(daq);odt<=DaqListLastOdt(daq);odt++)
      {
        e = DaqListOdtFirstEntry(odt);
        if (OdtEntrySize(e)==(vuint8)0u)
        {
          goto next_odt;  
        }
        el = DaqListOdtLastEntry(odt);
        d = (vuint8*)DaqListOdtStimBuffer(odt);
        XCP_ASSERT(d!=0);
        
        if ( *d == (vuint8)0xFFu )
        {
          status |= (vuint8)XCP_EVENT_STIM_OVERRUN; /* No STIM data available */
        } 
        else 
        { 
          /* This is the inner loop, optimize here */
          *d = (vuint8)0xFFu; /* Invalidate STIM data buffer */
    #if defined ( XCP_ENABLE_DAQ_TIMESTAMP )
          if (odt == DaqListFirstOdt(daq))
          {
            /* Time stamp is only applicable in first odt of STIM */
      #if !defined ( XCP_ENABLE_DAQ_TIMESTAMP_FIXED )
            if ( (DaqListFlags(daq) & (vuint8)DAQ_FLAG_TIMESTAMP) != 0 )
      #endif
            {
              /* increase pointer to after time stamp */
              d = d + sizeof(XcpDaqTimestampType);
            }
          }
    #endif 

          while (e<=el)
          {
            n = OdtEntrySize(e);
            if ( n == (vuint8)0u )
            {
              break;
            }
    #if defined ( C_COMP_COSMIC_MCS12X_MSCAN12 )
            src = (vuint32)OdtEntryAddr(e);
            for ( ; n > 0; n-- )
            {
              d++;
              XCP_WRITE_BYTE_2_ADDR( src, *d );
              src++;
            }
    #else
            XcpMemCpy(OdtEntryAddr(e), d+1, n);
            d += n;
    #endif
            e++;
          }
        }

      } /* odt */

    } 
    else {

  #endif /* XCP_ENABLE_STIM */

      for (odt=DaqListFirstOdt(daq);odt<=DaqListLastOdt(daq);odt++)
      {

        status |= (vuint8)XCP_EVENT_DAQ;

        ApplXcpInterruptDisable(); /* The following code is not reentrant */

  #if defined ( XCP_ENABLE_SEND_QUEUE )
        /* Check if there is space in the queue for this ODT */
  #if defined ( XCP_TRANSPORT_LAYER_TYPE_FLEXRAY ) || defined ( XCP_TRANSPORT_LAYER_TYPE_FLEXRAY_ASR )
        /* avoid overwriting the last element */
        if (xcp.QueueLen>=(vuint8)(xcp.QueueSize-1))
  #else        
        if (xcp.QueueLen>=xcp.QueueSize)
  #endif
        {
          status |= (vuint8)XCP_EVENT_DAQ_OVERRUN; /* Queue overflow */
          DaqListFlags(daq) |= (vuint8)DAQ_FLAG_OVERRUN;
          goto next_odt;
        }
        dtop = &xcp.pQueue[(xcp.QueueRp+xcp.QueueLen)%xcp.QueueSize];
    #if defined ( XCP_SEND_QUEUE_SAMPLE_ODT )
        xcp.QueueLen++;
    #endif
  #else
        dtop = &dto;
  #endif /* XCP_ENABLE_SEND_QUEUE */

  #if defined ( XCP_ENABLE_DAQ_HDR_ODT_DAQ )

        /* ODT,DAQ */
    #if defined ( XCP_ALLIGN_WORD )
        dtop->EightByteField.byte_0 = odt-DaqListFirstOdt(daq); /* Relative odt number */
        dtop->EightByteField.byte_1 = daq;
    #else
        dtop->b[0] = odt-DaqListFirstOdt(daq); /* Relative odt number */
        dtop->b[1] = daq;
    #endif    
        i = 2;

  #else

        /* PID */
    #if defined ( XCP_ALLIGN_WORD )
        dtop->XcpDtoByteField.EightByteField.byte_0 = odt;
    #else
        dtop->b[0] = odt;
    #endif
        i = 1;

  #endif

        /* Use BIT7 of PID or ODT to indicate overruns */
  #if defined ( XCP_ENABLE_SEND_QUEUE )
    #if defined ( XCP_ENABLE_DAQ_OVERRUN_INDICATION )
        if ( (DaqListFlags(daq) & (vuint8)DAQ_FLAG_OVERRUN) != 0 )
        {
      #if defined ( XCP_ALLIGN_WORD )
          dtop->XcpDtoByteField.EightByteField.byte_0 |= (vuint8)0x80;
      #else
          dtop->b[0] |= (vuint8)0x80;
      #endif
          DaqListFlags(daq) &= (vuint8)(~DAQ_FLAG_OVERRUN & 0xFFu);
        }
    #endif
  #endif

        /* Timestamps */
  #if defined ( XCP_ENABLE_DAQ_TIMESTAMP )

    #if !defined ( XCP_ENABLE_DAQ_TIMESTAMP_FIXED )
        if ( (DaqListFlags(daq) & (vuint8)DAQ_FLAG_TIMESTAMP) != 0 )
        {
    #endif

          if (odt==DaqListFirstOdt(daq))
          {
    #if defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS )
            t = ApplXcpGetTimestamp();
    #endif

    #if defined ( XCP_ENABLE_DAQ_HDR_ODT_DAQ )

        #if defined ( XCP_ALLIGN_WORD )
            *(XcpDaqTimestampType*)&dtop->b[1] = ApplXcpGetTimestamp();
        #else
            *(XcpDaqTimestampType*)&dtop->b[2] = ApplXcpGetTimestamp();
        #endif
      
            i = 2 + sizeof(XcpDaqTimestampType);

    #else /* XCP_ENABLE_DAQ_HDR_ODT_DAQ */

      #if defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS )
        #if defined ( XCP_CPUTYPE_BIGENDIAN )  /* Avoid WORD access */
          #if defined ( XCP_ALLIGN_WORD )
            dtop->EightByteField.byte_1 = (vbittype)((t>>8)&0xFFu);
            dtop->EightByteField.byte_2 = (vbittype)(t&0xFFu);
          #else
            dtop->b[1] = (vuint8)(t>>8);
            dtop->b[2] = (vuint8)t;
          #endif
        #else
          #if defined ( XCP_ALLIGN_WORD )
            dtop->EightByteField.byte_1 = (vbittype)(t&0xFFu);
            dtop->EightByteField.byte_2 = (vbittype)((t>>8)&0xFFu);
          #else
            dtop->b[1] = (vuint8)t;
            dtop->b[2] = (vuint8)(t>>8);
          #endif
        #endif
      #else
            *(XcpDaqTimestampType*)&dtop->b[1] = ApplXcpGetTimestamp();
      #endif /* XCP_DISABLE_UNALIGNED_MEM_ACCESS */
            i = 3;

    #endif /* XCP_ENABLE_DAQ_HDR_ODT_DAQ */
          }

    #if !defined ( XCP_ENABLE_DAQ_TIMESTAMP_FIXED )
        }
    #endif

  #endif /* XCP_ENABLE_DAQ_TIMESTAMP */

        /* Sample data */
        /* This is the inner loop, optimize here */
        e = DaqListOdtFirstEntry(odt);
        if (OdtEntrySize(e)==0)
        {
          goto next_odt;  
        }
        el = DaqListOdtLastEntry(odt);
  #if defined ( XCP_ALLIGN_WORD )
        i = (i+1)>>1;
        d = (vuint8*)&dtop->XcpDtoByteField.b[i];
  #else
        d = (vuint8*)&dtop->b[i];
  #endif
        
        while (e<=el)
        {
          n = OdtEntrySize(e);
          if (n == 0)
          {
            break;
          }
  #if defined ( C_COMP_COSMIC_MCS12X_MSCAN12 )
          src = (vuint32)OdtEntryAddr(e);
          for ( ; n > 0; n-- )
          {
            *d = XCP_READ_BYTE_FROM_ADDR( src );
            d++;
            src++;
          }
  #else
          XcpMemCpy(d, OdtEntryAddr(e), n);
          d = &d[n]; /* d += n; Suppress warning for MISRA rule 101 (pointer arithmetic) */
  #endif
          e++;
        }
  #if defined ( XCP_ALLIGN_WORD )
        dtop->l = (vuint8)(d-(&dtop->XcpDtoByteField.b[0]) );
  #else
        dtop->l = (vuint8)(d-(&dtop->b[0]) );
  #endif
  #if defined ( XCP_ALLIGN_WORD )
        /* There are 2 bytes within one adress */
        dtop->l = dtop->l << 1;
  #endif
        XCP_ASSERT(dtop->l<=kXcpMaxDTO);

        /* Queue or transmit the DTO */
  #if defined ( XCP_ENABLE_SEND_QUEUE )
    #if defined ( XCP_SEND_QUEUE_SAMPLE_ODT )
        /* No action yet */
    #else
        if ( (xcp.SendStatus & (vuint8)XCP_SEND_PENDING) != 0 )
        {
          xcp.QueueLen++;
        }
        else
        {
          xcp.SendStatus |= (vuint8)XCP_DTO_PENDING;
          XcpSendDto(dtop);
        }
    #endif
  #else
        XcpSendDto(&dto);
  #endif /* XCP_ENABLE_SEND_QUEUE */
        next_odt:

        ApplXcpInterruptEnable();

      } /* odt */

  #if defined ( XCP_ENABLE_SEND_QUEUE ) && defined ( XCP_SEND_QUEUE_SAMPLE_ODT )
      (void)XcpSendDtoFromQueue();
  #endif

  #if defined ( XCP_ENABLE_STIM )
    }
  #endif

  #if defined ( XCP_ENABLE_DAQ_PRESCALER )
    }
  #endif
  
  } /* daq */

  END_PROFILE(4); /* Timingtest */
  return status; 
}

#endif /* XCP_ENABLE_DAQ */


/****************************************************************************/
/* Background Processing                                                    */
/* Used for Checksum Calculation                                            */
/****************************************************************************/

#if defined ( XCP_ENABLE_CHECKSUM )
  #if defined ( XCP_ENABLE_AUTOSAR_CRC_MODULE )
  #else
/* Table for CCITT checksum calculation */
    #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_CRC16CCITT )



  V_MEMROM0 MEMORY_ROM vuint16 CRC16CCITTtab[256] = {
      #if defined ( XCP_ENABLE_CRC16CCITT_REFLECTED )
    0x0000,0x1189,0x2312,0x329B,0x4624,0x57AD,0x6536,0x74BF,
    0x8C48,0x9DC1,0xAF5A,0xBED3,0xCA6C,0xDBE5,0xE97E,0xF8F7,
    0x1081,0x0108,0x3393,0x221A,0x56A5,0x472C,0x75B7,0x643E,
    0x9CC9,0x8D40,0xBFDB,0xAE52,0xDAED,0xCB64,0xF9FF,0xE876,
    0x2102,0x308B,0x0210,0x1399,0x6726,0x76AF,0x4434,0x55BD,
    0xAD4A,0xBCC3,0x8E58,0x9FD1,0xEB6E,0xFAE7,0xC87C,0xD9F5,
    0x3183,0x200A,0x1291,0x0318,0x77A7,0x662E,0x54B5,0x453C,
    0xBDCB,0xAC42,0x9ED9,0x8F50,0xFBEF,0xEA66,0xD8FD,0xC974,
    0x4204,0x538D,0x6116,0x709F,0x0420,0x15A9,0x2732,0x36BB,
    0xCE4C,0xDFC5,0xED5E,0xFCD7,0x8868,0x99E1,0xAB7A,0xBAF3,
    0x5285,0x430C,0x7197,0x601E,0x14A1,0x0528,0x37B3,0x263A,
    0xDECD,0xCF44,0xFDDF,0xEC56,0x98E9,0x8960,0xBBFB,0xAA72,
    0x6306,0x728F,0x4014,0x519D,0x2522,0x34AB,0x0630,0x17B9,
    0xEF4E,0xFEC7,0xCC5C,0xDDD5,0xA96A,0xB8E3,0x8A78,0x9BF1,
    0x7387,0x620E,0x5095,0x411C,0x35A3,0x242A,0x16B1,0x0738,
    0xFFCF,0xEE46,0xDCDD,0xCD54,0xB9EB,0xA862,0x9AF9,0x8B70,
    0x8408,0x9581,0xA71A,0xB693,0xC22C,0xD3A5,0xE13E,0xF0B7,
    0x0840,0x19C9,0x2B52,0x3ADB,0x4E64,0x5FED,0x6D76,0x7CFF,
    0x9489,0x8500,0xB79B,0xA612,0xD2AD,0xC324,0xF1BF,0xE036,
    0x18C1,0x0948,0x3BD3,0x2A5A,0x5EE5,0x4F6C,0x7DF7,0x6C7E,
    0xA50A,0xB483,0x8618,0x9791,0xE32E,0xF2A7,0xC03C,0xD1B5,
    0x2942,0x38CB,0x0A50,0x1BD9,0x6F66,0x7EEF,0x4C74,0x5DFD,
    0xB58B,0xA402,0x9699,0x8710,0xF3AF,0xE226,0xD0BD,0xC134,
    0x39C3,0x284A,0x1AD1,0x0B58,0x7FE7,0x6E6E,0x5CF5,0x4D7C,
    0xC60C,0xD785,0xE51E,0xF497,0x8028,0x91A1,0xA33A,0xB2B3,
    0x4A44,0x5BCD,0x6956,0x78DF,0x0C60,0x1DE9,0x2F72,0x3EFB,
    0xD68D,0xC704,0xF59F,0xE416,0x90A9,0x8120,0xB3BB,0xA232,
    0x5AC5,0x4B4C,0x79D7,0x685E,0x1CE1,0x0D68,0x3FF3,0x2E7A,
    0xE70E,0xF687,0xC41C,0xD595,0xA12A,0xB0A3,0x8238,0x93B1,
    0x6B46,0x7ACF,0x4854,0x59DD,0x2D62,0x3CEB,0x0E70,0x1FF9,
    0xF78F,0xE606,0xD49D,0xC514,0xB1AB,0xA022,0x92B9,0x8330,
    0x7BC7,0x6A4E,0x58D5,0x495C,0x3DE3,0x2C6A,0x1EF1,0x0F78
      #else
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7u,
    0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1efu,
    0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6u,
    0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3deu,
    0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485u,
    0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58du,
    0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4u,
    0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bcu,
    0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823u,
    0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92bu,
    0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12u,
    0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1au,
    0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41u,
    0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49u,
    0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70u,
    0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78u,
    0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16fu,
    0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067u,
    0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35eu,
    0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256u,
    0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50du,
    0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405u,
    0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73cu,
    0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634u,
    0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9abu,
    0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3u,
    0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9au,
    0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92u,
    0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9u,
    0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1u,
    0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8u,
    0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0u
      #endif /* defined ( XCP_ENABLE_CRC16CCITT_REFLECTED ) */
  };



    #endif /* ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_CRC16CCITT ) */

  #endif /*  defined ( XCP_ENABLE_AUTOSAR_CRC_MODULE ) */

  #if !defined ( kXcpChecksumBlockSize )
    #define kXcpChecksumBlockSize 256
  #endif

#endif /* defined ( XCP_ENABLE_CHECKSUM ) */


/*****************************************************************************
| NAME:             XcpBackground
| CALLED BY:        application
| PRECONDITIONS:    none
| INPUT PARAMETERS: none 
| RETURN VALUES:    0 : background calculation finished
|                   1 : background calculation still pending
| DESCRIPTION:      perform background calculation of checksum
******************************************************************************/
vuint8 XcpBackground( void )
{

  BEGIN_PROFILE(3); /* Timingtest */

  /* Activation control */
  XcpPlCheckControlStateRet((vuint8)0u)

  /* STORE_DAQ_REQ or CLEAR_DAQ_REQ pending */
#if defined ( XCP_ENABLE_DAQ )
  #if defined ( XCP_ENABLE_DAQ_RESUME )
  if ( (xcp.SessionStatus & (vuint8)SS_STORE_DAQ_REQ) != 0 )
  {
    ApplXcpDaqResumeStore( &xcp.Daq );
  #if defined ( XCP_TRANSPORT_LAYER_TYPE_FLEXRAY_ASR )
    FrXcp_DaqResumeStore();
  #endif
    xcp.SessionStatus &= (vuint8)(~SS_STORE_DAQ_REQ & 0xFFu);
    /* #### Send an event message */
  }
  if ( (xcp.SessionStatus & (vuint8)SS_CLEAR_DAQ_REQ) != 0 )
  {
    ApplXcpDaqResumeClear();
  #if defined ( XCP_TRANSPORT_LAYER_TYPE_FLEXRAY_ASR )
    FrXcp_DaqResumeClear();
  #endif
    xcp.SessionStatus &= (vuint8)(~SS_CLEAR_DAQ_REQ & 0xFFu);
    /* #### Send an event message */
  }
  if ( (xcp.SessionStatus & (vuint8)SS_STORE_CAL_REQ) != 0 )
  {
    vuint8 ret;
    /* report application to store calibration data */
    ret = ApplXcpCalResumeStore();
    if ( ret != 0 )
    {
      /* Application finished with storing, clear bit */
      xcp.SessionStatus &= (vuint8)(~SS_STORE_CAL_REQ & 0xFFu);
      /* #### Send an event message */
    }
  }
  #endif
#endif /* XCP_ENABLE_DAQ */

  /* XCP checksum calculation */
#if defined ( XCP_ENABLE_CHECKSUM )

  /*
     Checksum algorithm is not defined by the standard
     Type is defined by tXcpChecksumSumType, the maximum blocksize is 64K
  */

  /* Checksum calculation in progress */
  if ( (xcp.CheckSumSize) != (tXcpChecksumSumType)0u )
  {

    vuint16 n;

    if (xcp.CheckSumSize<=(kXcpChecksumBlockSize-1))
    {
      n = xcp.CheckSumSize;
      xcp.CheckSumSize = 0;
    }
    else
    {
      n = (vuint16)kXcpChecksumBlockSize;
      xcp.CheckSumSize -= kXcpChecksumBlockSize;
    }

  #if defined ( XCP_ENABLE_AUTOSAR_CRC_MODULE )

      /* Calculate CRC with AUTOSAR CRC module. */
      xcp.CheckSum = XcpCalculateCrc(xcp.Mta, n, xcp.CheckSum);
      xcp.Mta += n;

  #else

    do
    {
    #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_CRC16CCITT )

      #if defined ( XCP_ENABLE_CRC16CCITT_REFLECTED )
      /* CRC16 CCITT Reflected: Refin = true and refout = true. */
      xcp.CheckSum = CRC16CCITTtab[((vuint8)(xcp.CheckSum&0xFF)) ^ XCP_READ_CHECKSUMVALUE_FROM_ADDR( xcp.Mta )] ^ ((vuint8)((xcp.CheckSum>>8)&0xFF));
      #else
      /* CRC16 CCITT */
      xcp.CheckSum = CRC16CCITTtab[((vuint8)((xcp.CheckSum >> 8)&0xFF)) ^ XCP_READ_CHECKSUMVALUE_FROM_ADDR( xcp.Mta )] ^ (xcp.CheckSum << 8);
      #endif

      xcp.Mta++;
      n--;

    #else

      /* Checksum calculation method: XCP_ADD_xx */
      xcp.CheckSum += (tXcpChecksumSumType)(XCP_READ_CHECKSUMVALUE_FROM_ADDR( xcp.Mta ));
      xcp.Mta      += (vuint8)sizeof(tXcpChecksumAddType);
      n            -= (vuint8)sizeof(tXcpChecksumAddType);

    #endif
    } 
    while (n!=0);

  #endif  /* defined ( XCP_ENABLE_AUTOSAR_CRC_MODULE ) */

    /* Checksum calculation finished ? */
    if ( (xcp.CheckSumSize) != 0 )
    {
      END_PROFILE(3); /* Timingtest */
      return (vuint8)1u; /* Still pending */
    }

    CRM_BUILD_CHECKSUM_TYPE = kXcpChecksumMethod;
    CRM_BUILD_CHECKSUM_RESULT = xcp.CheckSum;
    xcp.CrmLen = CRM_BUILD_CHECKSUM_LEN;


    XcpSendCrm();

  } /* xcp.CheckSumSize */
#endif /* XCP_ENABLE_CHECKSUM */

  /* Application specific background ground loop. */ 
  ApplXcpBackground();

  END_PROFILE(3); /* Timingtest */

  return (vuint8)0u;
}


/****************************************************************************/
/* Command Processor                                                        */
/****************************************************************************/


/*****************************************************************************
| NAME:             XcpDisconnect
| CALLED BY:        XcpCommand
| PRECONDITIONS:    XCP is initialized and in connected state.
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      If the XCP slave is connected to a XCP master a call of this
|                   function discontinues the connection (transition to disconnected state).
|                   If the XCP slave is not connected this function performs no action.
******************************************************************************/
void XcpDisconnect( void )
{
  /* Activation control */
  XcpPlCheckControlState()

  xcp.SessionStatus &= (vuint8)(~SS_CONNECTED & 0xFFu);

#if defined ( XCP_ENABLE_DAQ )
  XcpStopAllDaq();
#endif
#if defined ( XCP_ENABLE_SEED_KEY )
  /* Lock all resources */
  xcp.ProtectionStatus = RM_CAL_PAG|RM_DAQ|RM_PGM|RM_STIM;
#endif

}


/*****************************************************************************
| NAME:             XcpCommand
| CALLED BY:        XcpSendCallBack, XCP Transport Layer
| PRECONDITIONS:    none
| INPUT PARAMETERS: pCmd : data of received CTO message.
| RETURN VALUES:    none
| DESCRIPTION:      
******************************************************************************/
void XcpCommand( MEMORY_ROM vuint32* pCommand )
{
  MEMORY_ROM tXcpCto* pCmd = (MEMORY_ROM tXcpCto*) pCommand;
  vuint8 err;
  #if defined ( XCP_ENABLE_STIM )
    #if !defined ( XCP_ENABLE_DAQ_HDR_ODT_DAQ )
  vuint8 pid;
    #endif
  #endif

  /* Activation control */
  XcpPlCheckControlState()

 /* XCP Data Stimulation Handler */

  #if defined ( XCP_ENABLE_STIM )

    #if defined ( XCP_ENABLE_DAQ_HDR_ODT_DAQ )
      #error "Data stimulation not implemented for XCP_ENABLE_DAQ_HDR_ODT_DAQ"
    #else
  pid = (vuint8)CRO_BYTE(0);

  if (pid < (vuint8)0xC0u)
  {
    tXcpDto *pStimBuffer;

    CheckResourceProtection( RM_STIM )      /* Check whether the resource STIM is locked. */

    if (pid<xcp.Daq.OdtCount) 
    {

      pStimBuffer = DaqListOdtStimBuffer(pid);
      if (pStimBuffer != (tXcpDto*)0) 
      {
        *pStimBuffer = *(tXcpDto*)pCmd; /* Copy to STIM buffer, this validated the buffer */ 

        /* No response, if there is a valid STIM buffer */
        return;
      }
    }

    /* Error */
    /* pid out of range or DAQ list not configured for STIM */
    error(CRC_OUT_OF_RANGE)
 
  }    

    #endif

  #endif /* XCP_ENABLE_STIM */

    
  /* XCP Command Handler */

  BEGIN_PROFILE(1); /* Timingtest */

  /* CONNECT */
  if (CRO_CMD==CC_CONNECT)
  {

    /* Prepare the default response */
    /* ESCAN00023570 */
    CRM_CMD = 0xFF; /* No Error */
    xcp.CrmLen = 1; /* Length = 1 */


    /* DPRAM */
    /* DPRAM Client */
    
    /* Reset DAQ */
    /* Do not reset DAQ if in resume mode */ 
#if defined ( XCP_ENABLE_DAQ )
    if ( (xcp.SessionStatus & (vuint8)SS_RESUME) == 0 )
    {
      XcpFreeDaq();
  #if defined ( XCP_ENABLE_SEND_QUEUE )
      xcp.SendStatus = 0; /* Clear all transmission flags */
  #endif
    }
#endif /* XCP_ENABLE_DAQ */

#if defined ( XCP_ENABLE_SEED_KEY )
    /* Lock all resources. */
    xcp.ProtectionStatus = RM_CAL_PAG|RM_DAQ|RM_PGM|RM_STIM;
#endif

    /* Reset Session Status */
    xcp.SessionStatus = SS_CONNECTED;

    xcp.CrmLen = CRM_CONNECT_LEN;

    /* Versions of the XCP Protocol Layer and Transport Layer Specifications. */
    CRM_CONNECT_TRANSPORT_VERSION = (vuint8)( (vuint16)XCP_TRANSPORT_LAYER_VERSION >> 8 );
    CRM_CONNECT_PROTOCOL_VERSION =  (vuint8)( (vuint16)XCP_VERSION >> 8 );

    CRM_CONNECT_MAX_CTO_SIZE = kXcpMaxCTO;
    CRM_CONNECT_MAX_DTO_SIZE = kXcpMaxDTO;

#if defined ( XCP_ENABLE_CALIBRATION_PAGE )
    CRM_CONNECT_RESOURCE = RM_CAL_PAG;            /* Calibration */
#else
    CRM_CONNECT_RESOURCE = 0x00;                  /* Reset resource mask */
#endif
#if defined ( XCP_ENABLE_DAQ )
    CRM_CONNECT_RESOURCE |= (vuint8)RM_DAQ;       /* Data Acquisition */
#endif
#if defined ( XCP_ENABLE_STIM )
    CRM_CONNECT_RESOURCE |= (vuint8)RM_STIM;      /* Stimulation */
#endif
#if defined ( XCP_ENABLE_PROGRAM )
    CRM_CONNECT_RESOURCE |= (vuint8)RM_PGM;       /* Flash Programming */
#endif

    CRM_CONNECT_COMM_BASIC = 0;
#if defined ( XCP_ENABLE_COMM_MODE_INFO )
    CRM_CONNECT_COMM_BASIC |= (vuint8)CMB_OPTIONAL;
#endif
#if defined ( XCP_ENABLE_BLOCK_UPLOAD )
    CRM_CONNECT_COMM_BASIC |= (vuint8)CMB_SLAVE_BLOCK_MODE;
#endif
#if defined ( XCP_ALLIGN_WORD )
    CRM_CONNECT_COMM_BASIC |= (vuint8)CMB_ADDRESS_GRANULARITY_WORD;
#endif
#if defined ( XCP_CPUTYPE_BIGENDIAN )
    CRM_CONNECT_COMM_BASIC |= (vuint8)PI_MOTOROLA;
#endif

    XCP_PRINT(("<- 0xFF version=%02Xh/%02Xh, maxcro=%02Xh, maxdto=%02Xh, resource=%02X, mode=%02X\n",
        CRM_CONNECT_PROTOCOL_VERSION,
        CRM_CONNECT_TRANSPORT_VERSION,
        CRM_CONNECT_MAX_CTO_SIZE,
        CRM_CONNECT_MAX_DTO_SIZE,
        CRM_CONNECT_RESOURCE,
        CRM_CONNECT_COMM_BASIC));

    goto positive_response;

  }

  /* Handle other commands only if connected */
  else /* CC_CONNECT */
  {
    if ( (xcp.SessionStatus & (vuint8)SS_CONNECTED) != 0 )
    {
      /* Ignore commands if the previous command sequence has not been completed */
#if defined ( XCP_ENABLE_SEND_QUEUE )
      if ( (xcp.SendStatus & (vuint8)(XCP_CRM_PENDING|XCP_CRM_REQUEST)) != 0 )
      {
        xcp.SessionStatus |= (vuint8)SS_ERROR; 
        END_PROFILE(1); /* Timingtest */

        /* No response */
        return;
      }
#endif

      /* Prepare the default response */
      /* ESCAN00023570 */
      CRM_CMD = 0xFF; /* No Error */
      xcp.CrmLen = 1; /* Length = 1 */

      switch (CRO_CMD)
      {

        case CC_SYNC:
          {
            /* Always return a negative response with the error code ERR_CMD_SYNCH. */
            xcp.CrmLen = CRM_SYNCH_LEN;
            CRM_CMD    = PID_ERR;
            CRM_ERR    = CRC_CMD_SYNCH;
           
          }
          break;


#if defined ( XCP_ENABLE_COMM_MODE_INFO )
        case CC_GET_COMM_MODE_INFO:
          {
            xcp.CrmLen = CRM_GET_COMM_MODE_INFO_LEN;
            /* Transmit the version of the XCP Protocol Layer implementation.    */
            /* The higher nibble is the main version, the lower the sub version. */
            /* The lower nibble overflows, if the sub version is greater than 15.*/
            CRM_GET_COMM_MODE_INFO_DRIVER_VERSION = (vuint8)( ((CP_XCP_VERSION & 0x0F00) >> 4) |
                                                               (CP_XCP_VERSION & 0x000F)         );
            CRM_GET_COMM_MODE_INFO_COMM_OPTIONAL = 0;
            CRM_GET_COMM_MODE_INFO_QUEUE_SIZE = 0;
  #if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
            CRM_GET_COMM_MODE_INFO_COMM_OPTIONAL |= (vuint8)CMO_MASTER_BLOCK_MODE;
            CRM_GET_COMM_MODE_INFO_MAX_BS = (vuint8)((254 / CRO_DOWNLOAD_MAX_SIZE) + 1u);
    #if defined (kXcpBlockDownloadMinSt)
            CRM_GET_COMM_MODE_INFO_MIN_ST = (vuint8)kXcpBlockDownloadMinSt;
    #else
            CRM_GET_COMM_MODE_INFO_MIN_ST = 0;
    #endif
  #else
            CRM_GET_COMM_MODE_INFO_MAX_BS = 0;
            CRM_GET_COMM_MODE_INFO_MIN_ST = 0;
  #endif


          }
          break;
#endif /* XCP_ENABLE_COMM_MODE_INFO */


          case CC_DISCONNECT:
            {
              xcp.CrmLen = CRM_DISCONNECT_LEN;
              XcpDisconnect();

            }
            break;

                       
#if defined ( kXcpStationIdLength ) || defined ( XCP_ENABLE_VECTOR_MAPNAMES )
          case CC_GET_ID:
            {
              xcp.CrmLen = CRM_GET_ID_LEN;
              CRM_GET_ID_MODE = 0;
              CRM_GET_ID_LENGTH = 0;

  #if defined ( kXcpStationIdLength )
              if ( CRO_GET_ID_TYPE == IDT_ASAM_NAME )   /* Type = ASAM MC2 */
              {
                CRM_GET_ID_LENGTH = (vuint32)kXcpStationIdLength;
                XcpSetMta( ApplXcpGetPointer(0xFF, (vuint32)(&kXcpStationId[0])), 0xFF);
              }
  #endif
  #if defined ( XCP_ENABLE_VECTOR_MAPNAMES )
              if ( CRO_GET_ID_TYPE == IDT_VECTOR_MAPNAMES )   /* Type = Vector Type */
              {
                MTABYTEPTR pData;

                CRM_GET_ID_LENGTH = ApplXcpGetIdData(&pData);
                XcpSetMta(pData,0xFF);
              }
  #endif

            }
            break;                    
#endif


          case CC_GET_STATUS: 
            {
              xcp.CrmLen = CRM_GET_STATUS_LEN;
              CRM_GET_STATUS_STATUS = xcp.SessionStatus;
#if defined ( XCP_ENABLE_SEED_KEY )
              /* Return current resource protection status. If a bit is one, the associated resource is locked. */
              CRM_GET_STATUS_PROTECTION = xcp.ProtectionStatus;
#else
              CRM_GET_STATUS_PROTECTION = 0;
#endif

#if defined ( XCP_ENABLE_DAQ_RESUME )
              /* Return the session configuration ID. */
              CRM_GET_STATUS_CONFIG_ID = xcp.Daq.DaqConfigId;
#else
              /* Session configuration ID not available. */
              CRM_GET_STATUS_CONFIG_ID = 0x00;
#endif

            }
            break;


#if defined ( XCP_ENABLE_SEED_KEY )

          case CC_GET_SEED:
            {
              /* Only seeds with a maximum length of MAX_CTO-2 are supported so far. */

              /* Check whether the first part or a remaining part of the seed is requested. */
              if (CRO_GET_SEED_MODE == 0x01)
              {
                /* Remaining parts of seeds are not supported so far. */
                error(CRC_OUT_OF_RANGE)
              }
              else
              {
  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
                /* Only one resource may be requested at one time. */
                switch (CRO_GET_SEED_RESOURCE)
                {
                  case RM_CAL_PAG: 
                    break;
                  case RM_PGM:
                    break;
                  case RM_DAQ:
                    break;
                  case RM_STIM:
                    break;
                  default:
                    error(CRC_OUT_OF_RANGE)
                }
                
  #endif                
                if ((xcp.ProtectionStatus & CRO_GET_SEED_RESOURCE) != 0)   /* locked */
                {
                   vuint8 seedLength;
                   seedLength = ApplXcpGetSeed(CRO_GET_SEED_RESOURCE, CRM_GET_SEED_DATA); 
                   if (seedLength > (vuint8)(kXcpMaxCTO-2))
                   {
                     /* A maximum seed length of MAX_CTO-2 is supported. */
                     error(CRC_OUT_OF_RANGE)
                   }
                   CRM_GET_SEED_LENGTH = seedLength;
                }
                else                       /* Unlocked */
                {
                  /* return 0 if the resource is unprotected. */
                  CRM_GET_SEED_LENGTH = 0;
                }
                xcp.CrmLen = (vuint8)CRM_GET_SEED_LEN;
              }

            }
            break;

          case CC_UNLOCK:
            {
              vuint8 resource;
              

              /* Only keys with a maximum length of MAX_CTO-2 are supported so far. */
              if (CRO_UNLOCK_LENGTH > (vuint8)(kXcpMaxCTO-2))
              {
                error(CRC_SEQUENCE)
              }
              else
              {
                resource = ApplXcpUnlock(CRO_UNLOCK_KEY, CRO_UNLOCK_LENGTH);
                if ( resource == (vuint8)0x00u )
                {
                  /* Key wrong ! */
                  /* Send ERR_ACCESS_LOCKED and go to disconnected state. */
                  XcpDisconnect();
                  error(CRC_ACCESS_LOCKED)
                }
                else
                {
                  /* unlock (reset) the appropriate resource protection mask bit.. */
                  xcp.ProtectionStatus &= (vuint8)~resource;

                  /* ..and return the current resource protection status. */
                  CRM_UNLOCK_PROTECTION = xcp.ProtectionStatus;
                  xcp.CrmLen = CRM_UNLOCK_LEN;
                }
              }

            }
            break;

#endif /* XCP_ENABLE_SEED_KEY */


#if defined ( XCP_ENABLE_CALIBRATION_PAGE )

          case CC_SET_CAL_PAGE:
            {
             /*
              * code modified by Ricardo to allow for a delayed response
              * so that data can be copied out of EEPROM
              *
              * Original code was:
              * check_error( ApplXcpSetCalPage(CRO_SET_CAL_PAGE_SEGMENT,CRO_SET_CAL_PAGE_PAGE,CRO_SET_CAL_PAGE_MODE) )
              */
              err = ApplXcpSetCalPage(CRO_SET_CAL_PAGE_SEGMENT,
                                        CRO_SET_CAL_PAGE_PAGE,
                                        CRO_SET_CAL_PAGE_MODE);
              if (err==(vuint8)XCP_CMD_PENDING) 
              {
                goto no_response;
              }

              check_error( err )
            }
            break;

          case CC_GET_CAL_PAGE:
            {


              xcp.CrmLen = CRM_GET_CAL_PAGE_LEN;
              CRM_GET_CAL_PAGE_PAGE = ApplXcpGetCalPage(CRO_GET_CAL_PAGE_SEGMENT,CRO_GET_CAL_PAGE_MODE);

            }
            break;

#endif /* XCP_ENABLE_CALIBRATION_PAGE */


#if defined ( XCP_ENABLE_PAGE_INFO )
          /* Paging Information optional */
          case CC_GET_PAG_PROCESSOR_INFO:
            {
       
              xcp.CrmLen = CRM_GET_PAG_PROCESSOR_INFO_LEN;
          
              CRM_GET_PAG_PROCESSOR_INFO_MAX_SEGMENT = (vuint8)kXcpMaxSegment;
  #if defined ( XCP_ENABLE_PAGE_FREEZE )
              CRM_GET_PAG_PROCESSOR_INFO_PROPERTIES  = 1; /* FREEZE_SUPPORTED = 1 */
  #else
              CRM_GET_PAG_PROCESSOR_INFO_PROPERTIES  = 0; /* FREEZE_SUPPORTED = 0 */
  #endif
        
        
            }
            break;  

            
#endif /* defined ( XCP_BASIC_VERSION_COMMENT ) */

          /* Paging freeze mode support */
#if defined ( XCP_ENABLE_PAGE_FREEZE )
          case CC_SET_SEGMENT_MODE:
            {

              if (CRO_SET_SEGMENT_MODE_SEGMENT > ((vuint8)(kXcpMaxSegment-1)&0xFFu))
              {
                error(CRC_OUT_OF_RANGE)
              }

              /* inform application about Set Segment Mode command */
              ApplXcpSetFreezeMode(CRO_SET_SEGMENT_MODE_SEGMENT, CRO_SET_SEGMENT_MODE_MODE);
              /* CRO_SET_SEGMENT_MODE_MODE;*/
              xcp.CrmLen = CRM_SET_SEGMENT_MODE_LEN;
   
            }
            break;

          case CC_GET_SEGMENT_MODE:
            {
          
              if (CRO_GET_SEGMENT_MODE_SEGMENT > ((vuint8)(kXcpMaxSegment-1)&0xFFu))
              {
                error(CRC_OUT_OF_RANGE)
              }

              /* request current freeze mode information from application */
              CRM_GET_SEGMENT_MODE_MODE = ApplXcpGetFreezeMode(CRO_GET_SEGMENT_MODE_SEGMENT);
              
              xcp.CrmLen = CRM_GET_SEGMENT_MODE_LEN;

            }
            break;
#endif /* XCP_ENABLE_PAGE_FREEZE */

          /* Copy cal page support */
#if defined ( XCP_ENABLE_PAGE_COPY )
          case CC_COPY_CAL_PAGE:
            {
          
              xcp.CrmLen = CRM_COPY_CAL_PAGE_LEN;
              
              err = ApplXcpCopyCalPage(CRO_COPY_CAL_PAGE_SRC_SEGMENT,CRO_COPY_CAL_PAGE_SRC_PAGE,CRO_COPY_CAL_PAGE_DEST_SEGMENT,CRO_COPY_CAL_PAGE_DEST_PAGE);

              if (err==(vuint8)XCP_CMD_PENDING) 
              {
                goto no_response;
              }

              check_error( err )

            }
            break;
#endif /* XCP_ENABLE_PAGE_COPY */


          case CC_SET_MTA:
            {
              XcpSetMta(ApplXcpGetPointer(CRO_SET_MTA_EXT,CRO_SET_MTA_ADDR),CRO_SET_MTA_EXT);

            }
            break;

#if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
          case CC_DOWNLOAD_NEXT:


            if (CRO_DOWNLOAD_SIZE!=xcp.NextDataBlockSize)
            {
              /* xcp.NextDataBlockSize = 0;*/
              error1(CRC_SEQUENCE,xcp.NextDataBlockSize)
            }
          /* |||| */

#endif /* XCP_ENABLE_BLOCK_DOWNLOAD */

          case CC_DOWNLOAD:
            {
              vuint8 size;


              CheckResourceProtection( RM_CAL_PAG )

              size = CRO_DOWNLOAD_SIZE;
#if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
              if (size>CRO_DOWNLOAD_MAX_SIZE)
              {
                size = CRO_DOWNLOAD_MAX_SIZE;
              }
              xcp.NextDataBlockSize = (vuint8)((CRO_DOWNLOAD_SIZE-size)&0xFFu);
#else
              if (size>CRO_DOWNLOAD_MAX_SIZE)
              {
                error(CRC_OUT_OF_RANGE)
              }
#endif

              err = XcpWriteMta(size,CRO_DOWNLOAD_DATA);
              if (err==(vuint8)XCP_CMD_PENDING) 
              {
                goto no_response;
              }
              if (err==(vuint8)XCP_CMD_DENIED)
              {
                error(CRC_WRITE_PROTECTED)
              }
              if (err==(vuint8)XCP_CMD_SYNTAX)
              {
                error(CRC_CMD_SYNTAX)
              }

#if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
              if ( xcp.NextDataBlockSize != 0) 
              {
                goto no_response;
              }
#endif
                    
          
            }
            break;

#if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
#else
          case CC_DOWNLOAD_MAX:
            {

              CheckResourceProtection( RM_CAL_PAG )

              err = XcpWriteMta(CRO_DOWNLOAD_MAX_MAX_SIZE,CRO_DOWNLOAD_MAX_DATA);
              if (err==(vuint8)XCP_CMD_PENDING)
              {
                return; 
              }
              if (err==(vuint8)XCP_CMD_DENIED)
              {
                error(CRC_WRITE_PROTECTED)
              }
              if (err==(vuint8)XCP_CMD_SYNTAX)
              {
                error(CRC_CMD_SYNTAX)
              }

            }
            break;
#endif /* !defined ( XCP_ENABLE_BLOCK_DOWNLOAD ) */

#if defined ( XCP_ENABLE_SHORT_DOWNLOAD ) 
          case CC_SHORT_DOWNLOAD:
            {
  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
              if (CRO_SHORT_DOWNLOAD_SIZE > (vuint8)CRM_SHORT_DOWNLOAD_MAX_SIZE)
              {
                error(CRC_OUT_OF_RANGE)
              }
  #endif
              CheckResourceProtection( RM_CAL_PAG )

              XcpSetMta(ApplXcpGetPointer(CRO_SHORT_DOWNLOAD_EXT,CRO_SHORT_DOWNLOAD_ADDR),CRO_SHORT_DOWNLOAD_EXT);
              err = XcpWriteMta(CRO_SHORT_DOWNLOAD_SIZE, CRO_SHORT_DOWNLOAD_DATA);
              if (err==(vuint8)XCP_CMD_PENDING) 
              {
                goto no_response;
              }
              if (err==(vuint8)XCP_CMD_DENIED)
              {
                error(CRC_WRITE_PROTECTED)
              }
              if (err==(vuint8)XCP_CMD_SYNTAX)
              {
                error(CRC_CMD_SYNTAX)
              }
            }
            break;
#endif /* defined ( XCP_ENABLE_SHORT_DOWNLOAD ) */

#if defined ( XCP_ENABLE_MODIFY_BITS ) 
          case CC_MODIFY_BITS:
            {
              vuint32 data;
              MTABYTEPTR Mta_backup;

              CheckResourceProtection( RM_CAL_PAG )
              Mta_backup = xcp.Mta;
              err = XcpReadMta(sizeof(data), (BYTEPTR)&data);
              xcp.Mta = Mta_backup;
              if (err==(vuint8)XCP_CMD_PENDING)
              {
                goto no_response;
              }
              if (err==(vuint8)XCP_CMD_DENIED)
              {
                error(CRC_ACCESS_DENIED)
              }

              data = ((data) & ((~((vuint32)(((vuint16)~CRO_MODIFY_BITS_AND)<<CRO_MODIFY_BITS_SHIFT))))
                                           ^((vuint32)(CRO_MODIFY_BITS_XOR<<CRO_MODIFY_BITS_SHIFT)));

              err = XcpWriteMta(sizeof(data), (MEMORY_ROM BYTEPTR)&data);
              xcp.Mta = Mta_backup;
              if (err==(vuint8)XCP_CMD_PENDING) 
              {
                goto no_response;
              }
              if (err==(vuint8)XCP_CMD_DENIED)
              {
                error(CRC_WRITE_PROTECTED)
              }
              if (err==(vuint8)XCP_CMD_SYNTAX)
              {
                error(CRC_CMD_SYNTAX)
              }
            }
            break;
#endif /* defined ( XCP_ENABLE_MODIFY_BITS ) */

          case CC_UPLOAD:
            {
              vuint8 size = CRO_UPLOAD_SIZE;


#if defined ( XCP_ENABLE_BLOCK_UPLOAD )
              if ( size > (vuint8)CRM_UPLOAD_MAX_SIZE )
              {
                xcp.SessionStatus |= (vuint8)SS_BLOCK_UPLOAD;
                xcp.NextDataBlockSize = (vuint8)((size - (vuint8)CRM_UPLOAD_MAX_SIZE)&0xFFu);
                size = CRM_UPLOAD_MAX_SIZE;
              }
#else
              if ( size > (vuint8)CRM_UPLOAD_MAX_SIZE )
              {
                error(CRC_OUT_OF_RANGE)
              }
#endif
              err = XcpReadMta(size,CRM_UPLOAD_DATA);
              xcp.CrmLen = (vuint8)((CRM_UPLOAD_LEN+size)&0xFFu);
              if (err==(vuint8)XCP_CMD_PENDING)
              {
                goto no_response;
              }
              if (err==(vuint8)XCP_CMD_DENIED)
              {
                error(CRC_ACCESS_DENIED)
              }

            }
            break;

          case CC_SHORT_UPLOAD:
            {

#if defined ( XCP_ENABLE_PARAMETER_CHECK )
              if (CRO_SHORT_UPLOAD_SIZE > (vuint8)CRM_SHORT_UPLOAD_MAX_SIZE)
              {
                error(CRC_OUT_OF_RANGE)
              }
#endif
              XcpSetMta(ApplXcpGetPointer(CRO_SHORT_UPLOAD_EXT,CRO_SHORT_UPLOAD_ADDR),CRO_SHORT_UPLOAD_EXT);
              err = XcpReadMta(CRO_SHORT_UPLOAD_SIZE,CRM_SHORT_UPLOAD_DATA);
              xcp.CrmLen = (vuint8)((CRM_SHORT_UPLOAD_LEN+CRO_SHORT_UPLOAD_SIZE)&0xFFu);
              if (err==(vuint8)XCP_CMD_PENDING)
              {
                goto no_response; /* ESCAN00014775 */
              }
              if (err==(vuint8)XCP_CMD_DENIED)
              {
                error(CRC_ACCESS_DENIED)
              }

            }
            break;


#if defined ( XCP_ENABLE_CHECKSUM )

          case CC_BUILD_CHECKSUM: /* Build Checksum */
            {
              vuint32 s;


               /* Initialization of checksum calculation. */
  #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_CRC16CCITT ) || \
      ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_CRC32 )
              xcp.CheckSum = (tXcpChecksumSumType)0xFFFFFFFFu;
  #else
              xcp.CheckSum = (tXcpChecksumSumType)0u;
  #endif
              s = CRO_BUILD_CHECKSUM_SIZE;
              xcp.CheckSumSize = (vuint16)s;
              /* The blocksize is limited to WORD length. */
              /* If the blocksize exceeds the allowed maximum transmit negative response. */
              if ( (s & (vuint32)0xffff0000u) != (vuint32)0u )
              {
                CRM_BUILD_CHECKSUM_RESULT = 0xFFFFu; /* Range, max. 64K-1 */
                err = CRC_OUT_OF_RANGE;
                xcp.CrmLen = CRM_BUILD_CHECKSUM_LEN; 
                goto negative_response1;
              } 
              else
              {
              
  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
                /* Paramteer check whether the block size is modulo 2 for ADD_22, ADD_24 */
                /* and ADD_44 (modulo 4).                                                */  
    #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD22 ) || \
        ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD24 ) || \
        ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD44 )        
                if ( (s % (vuint32)sizeof(tXcpChecksumAddType)) != (vuint32)0u )
                {
                  error(CRC_OUT_OF_RANGE)
                }
                else
    #endif    
  #endif
                {
                  goto no_response; /* Checksum calculation will be performed by XcpBackground() */
                }
              }
            }
            /* break; never reached */

#endif /* XCP_ENABLE_CHECKSUM */


#if defined ( XCP_ENABLE_DAQ )

  #if defined ( XCP_ENABLE_DAQ_PROCESSOR_INFO )

          case CC_GET_DAQ_PROCESSOR_INFO:
            {

              xcp.CrmLen = CRM_GET_DAQ_PROCESSOR_INFO_LEN;
              CRM_GET_DAQ_PROCESSOR_INFO_MIN_DAQ = 0;          
              CRM_GET_DAQ_PROCESSOR_INFO_MAX_DAQ = xcp.Daq.DaqCount; /* dynamic or static */          
    #if defined ( kXcpMaxEvent )
              CRM_GET_DAQ_PROCESSOR_INFO_MAX_EVENT = kXcpMaxEvent;
    #else
              CRM_GET_DAQ_PROCESSOR_INFO_MAX_EVENT = 0; /* Unknown */    
    #endif
    #if defined ( XCP_ENABLE_DAQ_HDR_ODT_DAQ )
              /* DTO identification field type: Relative ODT number, absolute list number (BYTE) */
              CRM_GET_DAQ_PROCESSOR_INFO_DAQ_KEY_BYTE = DAQ_HDR_ODT_DAQB;
    #else
              /* DTO identification field type: Absolute ODT number */
              CRM_GET_DAQ_PROCESSOR_INFO_DAQ_KEY_BYTE = DAQ_HDR_PID;
    #endif
              CRM_GET_DAQ_PROCESSOR_INFO_PROPERTIES = (vuint8)( DAQ_PROPERTY_CONFIG_TYPE
    #if defined ( XCP_ENABLE_DAQ_TIMESTAMP )
                | DAQ_PROPERTY_TIMESTAMP
    #endif
    #if defined ( XCP_ENABLE_DAQ_PRESCALER )
                | DAQ_PROPERTY_PRESCALER
    #endif
    #if defined ( XCP_ENABLE_DAQ_RESUME )
                | DAQ_PROPERTY_RESUME
    #endif
    #if defined ( XCP_ENABLE_DAQ_OVERRUN_INDICATION ) /* DAQ_PROPERTY_OVERLOAD_INDICATION */
                | DAQ_OVERLOAD_INDICATION_PID
    #endif
                );

            }
            break;

  #endif /* XCP_ENABLE_DAQ_PROCESSOR_INFO */

  #if defined ( XCP_ENABLE_DAQ_RESOLUTION_INFO )

            case CC_GET_DAQ_RESOLUTION_INFO:
              {

                xcp.CrmLen = CRM_GET_DAQ_RESOLUTION_INFO_LEN;
                CRM_GET_DAQ_RESOLUTION_INFO_GRANULARITY_DAQ = 1;
                CRM_GET_DAQ_RESOLUTION_INFO_GRANULARITY_STIM = 1;
                CRM_GET_DAQ_RESOLUTION_INFO_MAX_SIZE_DAQ  = XCP_MAX_ODT_ENTRY_SIZE;
                CRM_GET_DAQ_RESOLUTION_INFO_MAX_SIZE_STIM = XCP_MAX_ODT_ENTRY_SIZE;
    #if defined ( XCP_ENABLE_DAQ_TIMESTAMP )
                CRM_GET_DAQ_RESOLUTION_INFO_TIMESTAMP_MODE = kXcpDaqTimestampUnit | (vuint8)sizeof(XcpDaqTimestampType)
      #if defined ( XCP_ENABLE_DAQ_TIMESTAMP_FIXED )
                | DAQ_TIMESTAMP_FIXED
      #endif
                ;
                CRM_GET_DAQ_RESOLUTION_INFO_TIMESTAMP_TICKS = kXcpDaqTimestampTicksPerUnit;  /* BCD coded */
    #else
                CRM_GET_DAQ_RESOLUTION_INFO_TIMESTAMP_MODE = 0;
                CRM_GET_DAQ_RESOLUTION_INFO_TIMESTAMP_TICKS = 0;
    #endif /* XCP_ENABLE_DAQ_TIMESTAMP */
          
              }
              break;
  #endif /* XCP_ENABLE_DAQ_RESOLUTION_INFO */

  #if defined ( XCP_ENABLE_DAQ_EVENT_INFO ) && defined ( kXcpMaxEvent )
            case CC_GET_DAQ_EVENT_INFO:
              {
                vuint8 event = (vuint8)CRO_GET_DAQ_EVENT_INFO_EVENT;


    #if defined ( XCP_ENABLE_PARAMETER_CHECK )
                if (event >= (vuint8)kXcpMaxEvent )
                {
                  error(CRC_OUT_OF_RANGE)
                } 
    #endif

                xcp.CrmLen = CRM_GET_DAQ_EVENT_INFO_LEN;
                CRM_GET_DAQ_EVENT_INFO_PROPERTIES = kXcpEventDirection[event];
                CRM_GET_DAQ_EVENT_INFO_MAX_DAQ_LIST = 1; /* Only one DAQ-List available per event channel */
                CRM_GET_DAQ_EVENT_INFO_NAME_LENGTH = kXcpEventNameLength[event];
                CRM_GET_DAQ_EVENT_INFO_TIME_CYCLE = kXcpEventCycle[event];
    #if defined ( XCP_ENABLE_CANAPE_5_5_X_SUPPORT )
                CRM_GET_DAQ_EVENT_INFO_TIME_UNIT = kXcpEventUnit[event];
    #else
                CRM_GET_DAQ_EVENT_INFO_TIME_UNIT = kXcpEventUnit[event]>>4;
    #endif
                CRM_GET_DAQ_EVENT_INFO_PRIORITY = 0;     /* Event channel prioritization is not supported. */
                XcpSetMta( ApplXcpGetPointer( 0xFF, (vuint32)kXcpEventName[event]), 0xFF );

              }
              break;
  #endif /* XCP_ENABLE_DAQ_EVENT_INFO && kXcpMaxEvent */

  #if defined ( XCP_ENABLE_DAQ_RESUME )
          case CC_SET_REQUEST:
            {

              if ( (CRO_SET_REQUEST_MODE & (SS_STORE_DAQ_REQ | SS_CLEAR_DAQ_REQ | SS_STORE_CAL_REQ)) != 0 )
              {
                if ( (CRO_SET_REQUEST_MODE & SS_STORE_DAQ_REQ) != 0 )
                {
                  xcp.Daq.DaqConfigId = CRO_SET_REQUEST_CONFIG_ID;
                  xcp.SessionStatus |= (vuint8)SS_STORE_DAQ_REQ;
                }
                if ( (CRO_SET_REQUEST_MODE & SS_CLEAR_DAQ_REQ) != 0 )
                {
                  xcp.SessionStatus |= (vuint8)SS_CLEAR_DAQ_REQ;
                }
                if ( (CRO_SET_REQUEST_MODE & SS_STORE_CAL_REQ) != 0 )
                {
                  xcp.SessionStatus |= (vuint8)SS_STORE_CAL_REQ;
                }
              }
              else
              {
                error(CRC_OUT_OF_RANGE)
              }

              xcp.CrmLen = CRM_SET_REQUEST_LEN;


            }
            break;
  #endif /* XCP_ENABLE_DAQ_RESUME */

          case CC_FREE_DAQ:
            {

              CheckResourceProtection( RM_DAQ )

              XcpFreeDaq();

            }
            break;

          case CC_ALLOC_DAQ:
            {
              vuint8 count = (vuint8)CRO_ALLOC_DAQ_COUNT;
 

              check_error( XcpAllocDaq(count) )

            }
            break;

          case CC_ALLOC_ODT:
            {
              vuint8 daq = (vuint8)CRO_ALLOC_ODT_DAQ;
              vuint8 count = CRO_ALLOC_ODT_COUNT;
 

  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
              if (daq>=xcp.Daq.DaqCount)
              {
                error(CRC_OUT_OF_RANGE)
              }
  #endif

              check_error( XcpAllocOdt(daq, count) )

            }
            break;

          case CC_ALLOC_ODT_ENTRY:
            {
              vuint8 daq = (vuint8)CRO_ALLOC_ODT_ENTRY_DAQ;
              vuint8 odt = CRO_ALLOC_ODT_ENTRY_ODT;
              vuint8 count = CRO_ALLOC_ODT_ENTRY_COUNT;
 

  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
              if ( (daq>=xcp.Daq.DaqCount) || (odt>=DaqListOdtCount(daq)) )
              {
                error(CRC_OUT_OF_RANGE)
              }
  #endif

              check_error( XcpAllocOdtEntry(daq, odt, count) )

            }
            break;

          case CC_GET_DAQ_LIST_MODE:
            {
              vuint8 daq = (vuint8)CRO_GET_DAQ_LIST_MODE_DAQ;


  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
              if (daq>=xcp.Daq.DaqCount)
              {
                error(CRC_OUT_OF_RANGE)
              }
  #endif
          
              xcp.CrmLen = CRM_GET_DAQ_LIST_MODE_LEN;
              CRM_GET_DAQ_LIST_MODE_MODE = DaqListFlags(daq);
  #if defined ( XCP_ENABLE_DAQ_PRESCALER )
              CRM_GET_DAQ_LIST_MODE_PRESCALER = DaqListPrescaler(daq);
  #else
              CRM_GET_DAQ_LIST_MODE_PRESCALER = 1;
  #endif
  #if defined ( kXcpMaxEvent )
              CRM_GET_DAQ_LIST_MODE_EVENTCHANNEL = 0; /* #### Lookup in EventDaq[]*/
  #else
              CRM_GET_DAQ_LIST_MODE_EVENTCHANNEL = DaqListEventChannel(daq);
  #endif
              CRM_GET_DAQ_LIST_MODE_PRIORITY = 0;  /* DAQ-list prioritization is not supported. */

            }
            break;

          case CC_SET_DAQ_LIST_MODE:
            {
              vuint8 daq = (vuint8)CRO_SET_DAQ_LIST_MODE_DAQ;
  #if defined ( XCP_ENABLE_DAQ_PRESCALER ) || ( !defined ( XCP_ENABLE_DAQ_PRESCALER ) && defined ( XCP_ENABLE_PARAMETER_CHECK ) )
              vuint8 xcpPrescaler = CRO_SET_DAQ_LIST_MODE_PRESCALER;
  #endif
              vuint8 event = (vuint8)(CRO_SET_DAQ_LIST_MODE_EVENTCHANNEL&0xFFu);
 

  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
              if (daq>=xcp.Daq.DaqCount)
              {
                error(CRC_OUT_OF_RANGE)
              } 
    #if defined ( kXcpMaxEvent )
              if (event >= (vuint8)kXcpMaxEvent)
              {
                error(CRC_OUT_OF_RANGE)
              } 
    #endif
    #if !defined ( XCP_ENABLE_DAQ_PRESCALER )
              if (xcpPrescaler!=1)
              {
                error(CRC_OUT_OF_RANGE)
              }
    #endif
              if (CRO_SET_DAQ_LIST_MODE_PRIORITY!=0)   /* Priorization is not supported */
              {
                error(CRC_OUT_OF_RANGE)
              } 
  #endif

  #if defined ( XCP_ENABLE_DAQ_PRESCALER )
              if (xcpPrescaler==0)
              {
                xcpPrescaler = 1;
              }
              DaqListPrescaler(daq) = xcpPrescaler;
  #endif
  #if defined ( kXcpMaxEvent ) && ! defined ( XCP_ENABLE_STIM )
              xcp.Daq.EventDaq[event] = daq;
  #else
              DaqListEventChannel(daq) = event;
  #endif
              DaqListFlags(daq) = CRO_SET_DAQ_LIST_MODE_MODE;

  #if defined ( XCP_ENABLE_STIM )
              check_error( XcpAllocStimMemory() )
  #endif

              break;
            }


          case CC_SET_DAQ_PTR:
            {
              vuint8 daq = (vuint8) (CRO_SET_DAQ_PTR_DAQ&0xFFu);
              vuint8 odt = CRO_SET_DAQ_PTR_ODT;
              vuint8 idx = CRO_SET_DAQ_PTR_IDX;
              tXcpOdtIdx odt0 = (tXcpOdtIdx)(DaqListFirstOdt(daq)+odt); /* Absolute odt number */


  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
              if ( (daq>=xcp.Daq.DaqCount) || (odt>=DaqListOdtCount(daq)) || (idx>=DaqListOdtEntryCount(odt0)) )
              {
                error(CRC_OUT_OF_RANGE)
              } 
  #endif

              xcp.CrmLen = CRM_SET_DAQ_PTR_LEN;
              xcp.DaqListPtr = DaqListOdtFirstEntry(odt0)+idx;
            }
            break;

          case CC_WRITE_DAQ: /* Write DAQ entry */
            {

              CheckResourceProtection( RM_DAQ )

  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
              if ( ((vuint8)CRO_WRITE_DAQ_SIZE==(vuint8)0u ) || (CRO_WRITE_DAQ_SIZE > (vuint8)XCP_MAX_ODT_ENTRY_SIZE) )
              {
                error(CRC_OUT_OF_RANGE)
              }  
  #endif

              xcp.CrmLen = CRM_WRITE_DAQ_LEN;
              OdtEntrySize(xcp.DaqListPtr) = CRO_WRITE_DAQ_SIZE;
              OdtEntryAddr(xcp.DaqListPtr) = (DAQBYTEPTR)ApplXcpGetPointer(CRO_WRITE_DAQ_EXT,CRO_WRITE_DAQ_ADDR);
          
              xcp.DaqListPtr++; /* Autoincrement */
          
            }
            break;

          case CC_START_STOP_DAQ_LIST:
            {
              vuint8 daq = (vuint8)(CRO_START_STOP_DAQ&0xFFu);

              CheckResourceProtection( RM_DAQ )

  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
              if (daq>=xcp.Daq.DaqCount)
              {
                error(CRC_OUT_OF_RANGE)
              } 
  #endif

              if ( (CRO_START_STOP_MODE==1 ) || (CRO_START_STOP_MODE==2) )
              {
                DaqListFlags(daq) |= (vuint8)DAQ_FLAG_SELECTED;
                if ( CRO_START_STOP_MODE == (vuint8)1u )
                {
                  XcpStartDaq(daq);
                }
                xcp.CrmLen = CRM_START_STOP_LEN;
                CRM_START_STOP_FIRST_PID = DaqListFirstPid(daq);
              } 
              else
              {
                XcpStopDaq(daq);
              }

            }
            break;

          case CC_START_STOP_SYNCH:
            {

              CheckResourceProtection( RM_DAQ )

              if (CRO_START_STOP_MODE==2) /* stop selected */
              {
                XcpStopAllSelectedDaq();
              } 
              else
              {
                if (CRO_START_STOP_MODE==1) /* start selected */
                {
                  XcpStartAllSelectedDaq();
                }
                else
                {
                  /* CRO_START_STOP_MODE==0 : stop all */
                  XcpStopAllDaq();
                }
              }
            }
            break;

  #if defined ( XCP_ENABLE_DAQ_TIMESTAMP )
          case CC_GET_DAQ_CLOCK:
            {
              xcp.CrmLen = CRM_GET_DAQ_CLOCK_LEN;
              CRM_GET_DAQ_CLOCK_TIME = (vuint32)ApplXcpGetTimestamp();

            }
            break;
  #endif

#endif /* XCP_ENABLE_DAQ */


          /* Flash Programming Kernel Download */
#if defined ( XCP_ENABLE_BOOTLOADER_DOWNLOAD )

          case CC_PROGRAM_PREPARE: /* Prepare flash kernel download */
            {
              XCP_PRINT(("-> PROGRAM_PREPARE\n"));

              if ( ApplXcpDisableNormalOperation(xcp.Mta,CRO_PROGRAM_PREPARE_SIZE) == (vuint8)XCP_CMD_DENIED )
              {
                error(CRC_GENERIC)  /* ESCAN00009199 */
              }
              xcp.CrmLen = CRM_PROGRAM_PREPARE_LEN;

              XCP_PRINT(("<- 0xFF\n"));
            }
            break;

#endif /* XCP_ENABLE_BOOTLOADER_DOWNLOAD */

 
           /* Flash Programming direct and Kernel */
#if defined ( XCP_ENABLE_PROGRAM ) || defined ( XCP_ENABLE_BOOTLOADER_DOWNLOAD )

           case CC_PROGRAM_START:   /* Start flash kernel */
             {
               XCP_PRINT(("-> PROGRAM_START\n"));

               CheckResourceProtection( RM_PGM )

               xcp.CrmLen = CRM_PROGRAM_START_LEN;                           
               CRM_PROGRAM_MAX_CTO_PGM = kXcpMaxCTO;                      
               CRM_PROGRAM_COMM_MODE_PGM = (vuint8)0u;
               #if defined ( XCP_ENABLE_BLOCK_UPLOAD )
                 CRM_PROGRAM_COMM_MODE_PGM |= (vuint8)CMB_SLAVE_BLOCK_MODE;
               #endif
               #if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
                 CRM_PROGRAM_COMM_MODE_PGM |= (vuint8)CMO_MASTER_BLOCK_MODE;
                 CRM_PROGRAM_MAX_BS_PGM = (vuint8)((254 / CRO_PROGRAM_MAX_SIZE) + (vuint8)1u);
                 CRM_PROGRAM_MIN_ST_PGM = (vuint8)kXcpProgramMinStPgm;
               #else
                 CRM_PROGRAM_MAX_BS_PGM = (vuint8)0u;
                 CRM_PROGRAM_MIN_ST_PGM = (vuint8)0u;
               #endif 
               CRM_PROGRAM_QUEUE_SIZE_PGM = (vuint8)0u; /* Interleaved mode is not implemented */                      

  #if defined ( XCP_ENABLE_BOOTLOADER_DOWNLOAD )
               if ( ApplXcpStartBootLoader() == (vuint8)0u ) /* Never returns */
               {
                 error(CRC_GENERIC);
               }
  #else
               err = ApplXcpProgramStart();
               if (err == XCP_CMD_PENDING)
               {
                 goto no_response;
               }
               if ( err != XCP_CMD_OK )
               {
                 error(CRC_GENERIC)
               }
  #endif

               XCP_PRINT(("<- 0xFF\n"));
             }
             break;
#endif
      
          /* Flash Programming  */
#if defined ( XCP_ENABLE_PROGRAM )

          /* Sector info*/
  #if defined ( XCP_ENABLE_PROGRAM_INFO )
          case CC_GET_SECTOR_INFO:
            {
              vuint8 sector = CRO_PROGRAM_GET_SECTOR_INFO_NUMBER;  
          
              XCP_PRINT(("-> GET_SECTOR_INFO sector=%u\n",sector));
           
    #if defined ( XCP_ENABLE_PARAMETER_CHECK )
              if (sector>=kXcpProgramMaxSector)
              {
                error(CRC_OUT_OF_RANGE)
              } 
    #endif


              /* The programming method is per default Sequential Programming (ESCAN00009202) */
              CRM_PROGRAM_GET_SECTOR_PGM_METHOD = (vuint8)0x00;

              /* Mode 0: get start address of sector */
              if (CRO_PROGRAM_GET_SECTOR_INFO_MODE == 0)
              {
                CRM_PROGRAM_GET_SECTOR_SECTOR_INFO = kXcpProgramSectorStart[sector];
                XCP_PRINT(("<- 0xFF address=%u\n",CRM_PROGRAM_GET_SECTOR_SECTOR_INFO));
              }

              /* Mode 1: get length of sector */
              if (CRO_PROGRAM_GET_SECTOR_INFO_MODE == 1)
              {
                CRM_PROGRAM_GET_SECTOR_SECTOR_INFO = (kXcpProgramSectorEnd[sector]-kXcpProgramSectorStart[sector])+(vuint32)1u;
                XCP_PRINT(("<- 0xFF length=%u\n",CRM_PROGRAM_GET_SECTOR_SECTOR_INFO));
              }

              xcp.CrmLen = CRM_PROGRAM_GET_SECTOR_INFO_LEN;
            }
            break;


          case CC_GET_PGM_PROCESSOR_INFO:
            {
              XCP_PRINT(("-> GET_PGM_PROCESSOR_INFO\n"));
          
              xcp.CrmLen = CRM_GET_PGM_PROCESSOR_INFO_LEN;
              /* PGM_PROPERTIES: only the default programming format is supported (ESCAN00009202) */
              CRM_GET_PGM_PROCESSOR_INFO_PROPERTIES = (vuint8)( PGM_ACCESS_ABSOLUTE | PGM_COMPRESSION_NONE | PGM_ENCRYPTION_NONE | PGM_NON_SEQ_NONE );
              /* MAX_SECTOR */
              CRM_GET_PGM_PROCESSOR_INFO_MAX_SECTOR = kXcpProgramMaxSector;
          
              XCP_PRINT(("<- 0xFF MAX_SECTOR =%u\n",CRM_GET_PGM_PROCESSOR_INFO_MAX_SECTOR));
            }
            break;

  #endif /* XCP_ENABLE_PROGRAM_INFO */

          case CC_PROGRAM_RESET: /* disconnect without response */
            {
              XCP_PRINT(("-> PROGRAM_RESET\n"));
              XcpDisconnect();
              ApplXcpReset(); /* May not return */
              XCP_PRINT(("<- 0xFF\n"));
            } 
            break;


      case CC_PROGRAM_CLEAR:
            {
              XCP_PRINT(("-> PROGRAM_CLEAR\n"));

              CheckResourceProtection( RM_PGM )

              /* Only the absolute access mode (default access mode) is available. */
              xcp.CrmLen =  CRM_PROGRAM_CLEAR_LEN;
              
              err = ApplXcpFlashClear(xcp.Mta, CRO_PROGRAM_CLEAR_SIZE);
              if (err == XCP_CMD_PENDING)
              {
                goto no_response; /* FLASH clear pending */
              }
              if (err != XCP_CMD_OK)
              {
                error(CRC_GENERIC)
              }
          
              XCP_PRINT(("<- 0xFF\n"));
            }
            break;

  #if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
            case CC_PROGRAM_NEXT:

          
              if (CRO_PROGRAM_NEXT_SIZE!=xcp.NextDataBlockSize)
              {
                error1(CRC_SEQUENCE,xcp.NextDataBlockSize)
              }
              /* |||| */
  #endif

          case CC_PROGRAM:
            {
              vuint8 size = CRO_PROGRAM_SIZE;
              vuint8 r;


              CheckResourceProtection( RM_PGM )

  #if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
              xcp.NextDataBlockSize = size;
              if (size>(vuint8)CRO_PROGRAM_MAX_SIZE)
              {
                size = CRO_PROGRAM_MAX_SIZE;
              }
              xcp.NextDataBlockSize -= size;
  #else
              if (size>(vuint8)CRO_PROGRAM_MAX_SIZE)
              {
                error(CRC_OUT_OF_RANGE)
              }
  #endif

              r = ApplXcpFlashProgram(CRO_PROGRAM_DATA,xcp.Mta,size);
              xcp.Mta += size;
              xcp.CrmLen = CRM_PROGRAM_LEN;
              if (r==XCP_CMD_PENDING)
              {
                goto no_response; /* FLASH write pending */
              }
              if (r!=XCP_CMD_OK)
              {
                error(CRC_GENERIC)
              }

  #if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
              if ( xcp.NextDataBlockSize != 0 )
              {
                goto no_response;
              }
  #endif
              XCP_PRINT(("<- 0xFF\n"));
            }
            break;

  #if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
  #else
      
          case CC_PROGRAM_MAX:
            {
              vuint8 r;
  

              CheckResourceProtection( RM_PGM )

              r = ApplXcpFlashProgram(CRO_PROGRAM_MAX_DATA,xcp.Mta,CRO_PROGRAM_MAX_MAX_SIZE);
              xcp.Mta += CRO_PROGRAM_MAX_MAX_SIZE;
              if (r==XCP_CMD_PENDING)
              {
                goto no_response; /* FLASH write pending */
              }
              if (r!=XCP_CMD_OK)
              {
                error(CRC_GENERIC)
              }

              XCP_PRINT(("<- 0xFF\n"));
            }
            break;

  #endif /* !defined ( XCP_ENABLE_BLOCK_DOWNLOAD ) */ 
#endif /* XCP_ENABLE_PROGRAM */


#if defined ( XCP_ENABLE_USER_COMMAND ) || defined ( XCP_ENABLE_WRITE_DAQ_MULTIPLE )
          case CC_USER_CMD:
            {


  #if defined ( XCP_ENABLE_WRITE_DAQ_MULTIPLE )
            /* Write multiple DAQ entries */
              if( CRO_WRITE_DAQ_MULTIPLE_COMMAND == CC_WRITE_DAQ_MULTIPLE )
              {
                vuint8 NoDAQ;

                CheckResourceProtection( RM_DAQ )

                for(NoDAQ = 0; NoDAQ < CRO_WRITE_DAQ_MULTIPLE_NODAQ; NoDAQ++)
                {

    #if defined ( XCP_ENABLE_PARAMETER_CHECK )
                  if ( ((vuint8)CRO_WRITE_DAQ_MULIPLE_SIZE(NoDAQ)==(vuint8)0u )
                     || (CRO_WRITE_DAQ_MULIPLE_SIZE(NoDAQ) > (vuint8)XCP_MAX_ODT_ENTRY_SIZE)
                     || (CRO_WRITE_DAQ_MULTIPLE_NODAQ > (vuint8)((kXcpMaxCTO-3u) / 8u)) )
                  {
                    error(CRC_OUT_OF_RANGE)
                  }
    #endif
                  OdtEntrySize(xcp.DaqListPtr) = CRO_WRITE_DAQ_MULIPLE_SIZE(NoDAQ);
                  OdtEntryAddr(xcp.DaqListPtr) = (DAQBYTEPTR)ApplXcpGetPointer(CRO_WRITE_DAQ_MULIPLE_EXT(NoDAQ),CRO_WRITE_DAQ_MULIPLE_ADDR(NoDAQ));
                  xcp.DaqListPtr++; /* Autoincrement */
                }
                xcp.CrmLen = CRM_WRITE_DAQ_LEN;
              }
              else
  #endif /* XCP_ENABLE_WRITE_DAQ_MULTIPLE */
              {
          
  #if defined ( XCP_ENABLE_USER_COMMAND )
                err = ApplXcpUserService( (MEMORY_ROM BYTEPTR) &CRO_WORD(0) );
                
                if (err==(vuint8)XCP_CMD_PENDING)
                {
                  goto no_response; 
                }
                if (err==(vuint8)XCP_CMD_SYNTAX)
                {
                  error(CRC_CMD_SYNTAX)
                }
  #endif
              }


            }
            break;
#endif
         

#if defined ( XCP_ENABLE_TL_COMMAND )
          case CC_TRANSPORT_LAYER_CMD:
            {
              err = ApplXcpTLService( &CRO_BYTE(0) );
              if (err==(vuint8)XCP_CMD_PENDING)
              {
                goto no_response; 
              }
              if (err==(vuint8)XCP_CMD_SYNTAX)
              {
                error(CRC_CMD_SYNTAX)
              }
              else
              if (err==(vuint8)XCP_CMD_BUSY)
              {
                error(CRC_CMD_BUSY)
              }
              else
              if (err==(vuint8)XCP_CMD_UNKNOWN)
              {
                error(CRC_CMD_UNKNOWN)
              }
              else
              if (err==(vuint8)XCP_CMD_OUT_OF_RANGE)
              {
                error(CRC_OUT_OF_RANGE)
              }
              else
              if (err==(vuint8)XCP_CMD_DENIED)
              {
                error(CRC_WRITE_PROTECTED)
              }
              else
              {
                ; /* Concluding else, nothing to do */
              }
            }
            break;
#endif
         
         
          default: /* unknown */
            {
#if defined ( XCP_ENABLE_OPENCMDIF )
              err = ApplXcpOpenCmdIf( pCmd->b, xcp.Crm.b, &xcp.CrmLen );
              if (err==(vuint8)XCP_CMD_PENDING)
              {
                goto no_response; 
              }
              if (err!=(vuint8)XCP_CMD_OK)
              {
                error(CRC_CMD_UNKNOWN)
              }
#else
              error(CRC_CMD_UNKNOWN)
#endif
            }

      } /* switch */

      goto positive_response;
    }

    /* Not connected */
    else
    {
      goto no_response;
    }
  } /* CC_CONNECT */

negative_response:
  xcp.CrmLen = 2;

#if defined ( XCP_ENABLE_CHECKSUM ) || defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
negative_response1:
#endif
  CRM_CMD = (vuint8)PID_ERR;
  CRM_ERR = (vuint8)err;
 
positive_response:
  XcpSendCrm();

no_response:
  END_PROFILE(1); /* Timingtest */
  return;
}


/****************************************************************************/
/* Send notification callback                                               */
/****************************************************************************/


/*****************************************************************************
| NAME:             XcpSendCallBack
| CALLED BY:        XCP Transport Layer
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    0 : if the XCP Protocol Layer is idle (no transmit messages are pending)
| DESCRIPTION:      Notifies the XCP Protocol Layer about the successful
|                   transmission of a XCP packet.
******************************************************************************/
vuint8 XcpSendCallBack( void )
{
  BEGIN_PROFILE(2); /* Timingtest */

  /* Activation control */
  XcpPlCheckControlStateRet((vuint8)1u)

#if defined ( XCP_ENABLE_DAQ ) && defined ( XCP_ENABLE_SEND_QUEUE )

  /* Clear all pending flags */
  /* A pending flag indicates that ApplXcpSend() is in progress */
  xcp.SendStatus &= (vuint8)(~XCP_SEND_PENDING & 0xFFu);

  /* Now check if there is another transmit request */
 
  /* Send a RES or ERR (CRM) message */
  if ( (xcp.SendStatus & (vuint8)XCP_CRM_REQUEST) != 0 )
  {
    xcp.SendStatus &= (vuint8)(~XCP_CRM_REQUEST & 0xFFu);
    XcpSendCrm();
    END_PROFILE(2); /* Timingtest */
    return (vuint8)0x01u;
  }

  /* Send a EV or SERV message */
  #if defined ( XCP_ENABLE_SEND_EVENT ) || defined ( XCP_ENABLE_SERV_TEXT )
  if ( (xcp.SendStatus & (vuint8)XCP_EVT_REQUEST) != 0 )
  {
    xcp.SendStatus &= (vuint8)(~XCP_EVT_REQUEST & 0xFFu);
    XcpSendEv();
    END_PROFILE(2); /* Timingtest */
    return (vuint8)0x01u;
  }
  #endif

  /* Send a DAQ message from the queue or from the buffer */
  if ( (xcp.SessionStatus & (vuint8)SS_DAQ) != 0 )
  {
    if ( XcpSendDtoFromQueue() != 0 )
    {
      END_PROFILE(2); /* Timingtest */
      return (vuint8)0x01u;
    }
  }
#endif /* XCP_ENABLE_DAQ && XCP_ENABLE_SEND_QUEUE */

  /* Continue a pending block upload command */
#if defined ( XCP_ENABLE_BLOCK_UPLOAD )
  if ( (xcp.SessionStatus & (vuint8)SS_BLOCK_UPLOAD) != 0 )
  {
#if ! defined ( XCP_ALLIGN_WORD )
    union
    {
      vuint8  b[4];
      vuint32 dw;
    } cmd;          /* Deviation of MISRA rule 110 (an aligned byte array is required). */
    cmd.b[0] = CC_UPLOAD;
    cmd.b[1] = xcp.NextDataBlockSize;
    xcp.SessionStatus &= (vuint8)(~SS_BLOCK_UPLOAD & 0xFFu);
    XcpCommand( &cmd.dw );
    END_PROFILE(2); /* Timingtest */
#else
    tXcpCto cmd;
    cmd.EightByteField.byte_0 = CC_UPLOAD;
    cmd.EightByteField.byte_1 = xcp.NextDataBlockSize;
    xcp.SessionStatus &= (vuint8)(~SS_BLOCK_UPLOAD & 0xFFu);
    XcpCommand( &cmd.dw[0] );
    END_PROFILE(2); /* Timingtest */
#endif

    return (vuint8)0x01u;
  }
#endif

  END_PROFILE(2); /* Timingtest */
  return (vuint8)0x00u;
}


/****************************************************************************/
/* Initialization / de-initialization                                       */
/****************************************************************************/


/*****************************************************************************
| NAME:             XcpInit
| CALLED BY:        application
| PRECONDITIONS:    the data link layer has to be initialized.
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Initialization of the XCP Protocol Layer
|                   Application specific initialization
|                    ( e.g. Vector XCP on CAN Transport Layer )
******************************************************************************/
void XcpInit( void )
{
#if defined ( XCP_ENABLE_CONTROL )
  XcpPlControlState = kXcpControl_Enable;
#endif

  /* Application specific initialization function. */
  ApplXcpInit();

  /* Initialize all XCP variables to zero */
  XcpMemClr((BYTEPTR)&xcp,(vuint16)sizeof(xcp));

  /* Initialize the RAM interface */

  /* Initialize the session status (ESCAN00013899) */
  xcp.SessionStatus = (vuint8)0u;

  #if defined ( XCP_ENABLE_SEND_QUEUE)
  /* Initialize the transmit queue (ESCAN00013899) */
  xcp.SendStatus = (vuint8)0u;
  #endif

  /* Resume DAQ */
#if defined ( XCP_ENABLE_DAQ )
  #if defined ( XCP_ENABLE_DAQ_RESUME )
  if ( ApplXcpDaqResume( &xcp.Daq ) != 0 )
  {
    /* Adjust the pointers */
    XcpAllocMemory(); 
    /* Goto temporary disconnected mode and start all selected DAQ lists */
    xcp.SessionStatus |= (vuint8)SS_RESUME;
    /* Select all DAQ lists. */
    XcpStartAllSelectedDaq();

    /* Send an event message to indicate resume mode */
    /* #### */


  }
  #endif /* XCP_ENABLE_DAQ_RESUME */
#endif /* XCP_ENABLE_DAQ */
}

/*****************************************************************************
| NAME:             XcpExit
| CALLED BY:        application
| PRECONDITIONS:    The XCP Protocol Layer has to be initialized.
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      De-initialization of the XCP Protocol Layer.
******************************************************************************/
void XcpExit( void )
{
  /* Activation control */
  XcpPlCheckControlState()

  /* Deinitialize the RAM interface */
}


/****************************************************************************/
/* Print via SERV/SERV_TEXT                                                */
/****************************************************************************/


#if defined ( XCP_ENABLE_SERV_TEXT )
  #if defined ( XCP_ENABLE_SERV_TEXT_PUTCHAR )

/*****************************************************************************
| NAME:             XcpPutChar
| CALLED BY:        application, XcpPrint
| PRECONDITIONS:    XCP is initialized and in connected state.
| INPUT PARAMETERS: c : character
| RETURN VALUES:    none
| DESCRIPTION:      Put a char into a service request packet (SERV).
******************************************************************************/
void XcpPutchar( MEMORY_ROM vuint8 c )
{
  /* Activation control */
  XcpPlCheckControlState()

  /* Check for stall condition */         
    #if defined ( XCP_ENABLE_SEND_QUEUE )

  while ( (xcp.SendStatus & (vuint8)XCP_EVT_REQUEST) != 0 )
  {
    if ( ApplXcpSendStall() == 0 )
    {
      return; /* Abort */
    }
  }

    #endif

  /* If xcp.EvLen!=0 there is a pending text message*/
  if (xcp.EvLen<2)
  {
    xcp.EvLen = 2;
  }

#if defined ( XCP_ALLIGN_WORD )
  xcp.Ev.b[(xcp.EvLen)>>1] = c;
  xcp.EvLen += 2;
#else
  xcp.Ev.b[xcp.EvLen] = c;
  xcp.EvLen++;
#endif

  if ( (xcp.EvLen>=(vuint8)kXcpMaxCTO) || (c==(vuint8)0x00u) )  /* Flush*/
  { 
    EV_BYTE(0) = 0xFC; /* SERV */
    EV_BYTE(1) = 0x01; /* SERV_TEXT*/
    XcpSendEv();
  }

}

    #if defined ( XCP_ENABLE_SERV_TEXT_PRINTF )

/*****************************************************************************
| NAME:             XcpPrint
| CALLED BY:        application
| PRECONDITIONS:    XCP is initialized and in connected state.
| INPUT PARAMETERS: *str : pointer to a string
| RETURN VALUES:    none
| DESCRIPTION:      Transmission of a service request packet (SERV).
******************************************************************************/
void XcpPrint( MEMORY_ROM vuint8 *str )
{
  /* Activation control */
  XcpPlCheckControlState()

  /* Transmit the text message. */
  while ( *str != 0x00 )
  {
    XcpPutchar(*str);
    str++;
  }
  /* Transmit the terminating 0x00. */
  XcpPutchar( (vuint8)0x00u );
}

                           
    #endif /* XCP_ENABLE_SERV_TEXT_PRINTF */
  #endif /* XCP_ENABLE_SERV_TEXT_PUTCHAR */
#endif /* XCP_ENABLE_SERV_TEXT */

                            
#if defined ( XCP_ENABLE_SEND_EVENT )

/*****************************************************************************
| NAME:             XcpSendEvent
| CALLED BY:        application
| PRECONDITIONS:    none
| INPUT PARAMETERS: evc : event code 
|                   c   : pointer to event data
|                   len : event data length
| RETURN VALUES:    none
| DESCRIPTION:      Transmission of an event packet (EV).
******************************************************************************/
void XcpSendEvent( vuint8 evc, MEMORY_ROM BYTEPTR c, vuint8 len)
{
  vuint8 i;

  /* Activation control */
  XcpPlCheckControlState()

  /* Check for stall condition */         
#if defined ( XCP_ENABLE_SEND_QUEUE )
  while ( (xcp.SendStatus & (vuint8)XCP_EVT_REQUEST) != (vuint8)0u )
  {
    if (!ApplXcpSendStall())
    {
      return; /* Abort */
    }
  }
#endif

  EV_BYTE(0)  = PID_EV; /* Event*/
  EV_BYTE(1)  = evc;  /* Event Code*/
  xcp.EvLen   = 2;
 
  if (len <= (vuint8)(kXcpMaxCTO-2) )
  {
    if (c != 0x00u)
    {
      for (i = 0; i < len; i++)
      {
#if defined ( XCP_ALLIGN_WORD )
        xcp.Ev.b[(xcp.EvLen)>>1]  = c[i];
        xcp.EvLen += 2;
        i++;
#else
        xcp.Ev.b[xcp.EvLen]       = c[i];
        xcp.EvLen++;
#endif
      }
    } 
    else
    {
      xcp.EvLen += len;
    }
  }


  XcpSendEv();
}

#endif /* XCP_ENABLE_SEND_EVENT */

#if defined ( XCP_ENABLE_GET_CONNECTION_STATE )
/*****************************************************************************
| NAME:             XcpGetState
| CALLED BY:        XcpPreCopy
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    XCP_CONNECTED    : XCP is connected
|                   XCP_DISCONNECTED : XCP is disconnected
| DESCRIPTION:      Get the connection state of the XCP Protocol Layer
******************************************************************************/
vuint8 XcpGetState( void )
{
  return ((xcp.SessionStatus & (vuint8)SS_CONNECTED) > (vuint8)0u) ? XCP_CONNECTED : XCP_DISCONNECTED;
}
#endif

#if defined ( XCP_ENABLE_GET_XCP_DATA_POINTER )
/*****************************************************************************
| NAME:             XcpGetXcpDataPointer
| CALLED BY:        Application
| PRECONDITIONS:    none
| INPUT PARAMETERS: tXcpData ** pXcpData: Pointer to Pointer that is set to xcp
| RETURN VALUES:    none
| DESCRIPTION:      Get the pointer to the internal xcp structure
******************************************************************************/
void XcpGetXcpDataPointer( RAM tXcpData ** pXcpData )
{
  *pXcpData = &xcp;
}
#endif

#if defined ( XCP_ENABLE_CONTROL )
/*****************************************************************************
| NAME:             XcpControl
| CALLED BY:        Application
| PRECONDITIONS:    none
| INPUT PARAMETERS: command: en- or disable the modul
| RETURN VALUES:    none
|                   none
| DESCRIPTION:      En- or disable the Xcp Protocol Layer globally
******************************************************************************/
void XcpControl( vuint8 command )
{
  XcpPlControlState = command;
}
#endif

#if defined ( XCP_ENABLE_VERSION_INFO_API )
/**************************************************************************************************
   Function name    : XcpGetVersionInfo
   ------------------------------------------------------------------------------------------------
   Description      : Returns version information of module
   ------------------------------------------------------------------------------------------------
   Called by        : -
   ------------------------------------------------------------------------------------------------
   Parameter        : Pointer to location at which version information shall be stored at
   ------------------------------------------------------------------------------------------------
   Returncode       : void
   ------------------------------------------------------------------------------------------------
   Misc             : -
**************************************************************************************************/
void XcpGetVersionInfo(Std_VersionInfoType *XcpVerInfoPtr)
{
  /* Since this service only access non-volatile data and no channel parameter is passed,
     checking of the channel handle and initialization is omitted. */
  XcpVerInfoPtr->vendorID = XCP_VENDOR_ID;
  XcpVerInfoPtr->moduleID = XCP_MODULE_ID;
  XcpVerInfoPtr->sw_major_version = (CP_XCP_VERSION >> 8u);
  XcpVerInfoPtr->sw_minor_version = (CP_XCP_VERSION & 0xff);
  XcpVerInfoPtr->sw_patch_version = CP_XCP_RELEASE_VERSION;
}
#endif /* XCP_ENABLE_VERSION_INFO_API */


/****************************************************************************/
/* Test                                                                     */
/* Some screen output functions for test and diagnostics                    */
/****************************************************************************/





/*******************************************************************************
  Consistency checks
*******************************************************************************/

/* Check definition of endianess of CPU */

#if defined ( XCP_CPUTYPE_LITTLEENDIAN ) || defined ( XCP_CPUTYPE_BIGENDIAN )
#else
  #error "Please define XCP_CPUTYPE_LITTLEENDIAN or XCP_CPUTYPE_BIGENDIAN."
#endif

/* Check consistency of alignment switch */

#if defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS )
  #if defined ( XCP_TRANSPORT_LAYER_TYPE_CAN_ASR )
    #if ( CPU_TYPE == CPU_TYPE_8 )
      #error "XCP consistency error: Forcing alignment for 8-bit microcontrollers is useless."
    #endif
  #else
    #if defined ( C_CPUTYPE_8BIT )
      #error "XCP consistency error: Forcing alignment for 8-bit microcontrollers is useless."
    #endif
  #endif
  #if defined ( XCP_ENABLE_UNALIGNED_MEM_ACCESS )
    #error "XCP consistency error: Select either alignment or no alignment."
  #endif
#else /* if defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS ) */
  #if defined ( XCP_ENABLE_UNALIGNED_MEM_ACCESS )
  #else
    #error "XCP consistency error: Usage of alignment not specified."
  #endif
#endif /* if defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS ) */


/*******************************************************************************
* Organi check
*******************************************************************************/







/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
