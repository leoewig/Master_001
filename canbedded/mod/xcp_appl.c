

/*****************************************************************************
| Project Name:   XCP application callback function template
|    File Name:   _xcp_appl.c
|
|  Description: 
|    Template for the XCP application callback functions.
|    This is just an example and may be modified .
|
|  Contains:
|    - Examples for FLASH programming, EEPROM programming, CalPage switching
|
|-----------------------------------------------------------------------------
|               D E M O
|-----------------------------------------------------------------------------
|
|       Please note, that the demo and example programs 
|       only show special aspects of the software. 
|       With regard to the fact that these programs are meant 
|       for demonstration purposes only,
|       Vector Informatik's liability shall be expressly excluded in cases 
|       of ordinary negligence, to the extent admissible by law or statute.
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
| Hr           Andreas Herkommer         Vector Informatik GmbH
| Svh          Sven Hesselmann           Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
|  Date       Version  Author  Description
| ----------  -------  ------  -----------------------------------------------
| 2004-12-30  1.00.00   Tri    ESCAN00010708: Create template _xcp_appl.c
| 2005-02-21  1.01.00   Tri    Rename XcpApplInit to ApplXcpInitTemplate
| 2005-05-19  1.02.00   Tri    Support data paging on Star12X
| 2005-12-14  1.03.00   Tri    ESCAN00013899: Data acquisition not possible during cold start
| 2006-03-17  1.04.00   Tri    Compiler switch for paged memory model added
| 2006-05-30  1.05.00   Tri    Demo disclaimer added
| 2007-01-18  1.06.00   Tri    ESCAN00018808: Support data paging on Star12X / Cosmic
| 2008-04-03  1.07.00   Hr     ESCAN00025754: Missing function stubs
| 2008-07-21  1.08.00   Hr     Added new callbacks
| 2008-09-10  1.09.00   Svh    Added new callback ApplXcpReadChecksumValue
|***************************************************************************/


/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "XcpProf.h"
#include "hardware.h"
#include "cosmic_pointers.h"
#include "cal.h"
#include "flash_eee.h"
#include "ixcp.h"       /* for ixcp_prog_allowed() */
#include "vprg.h"       /* for vprg_xcp_user_command() */
#include "ms.h"


/***************************************************************************/
/* Version check                                                           */
/***************************************************************************/
#if ( CP_XCP_VERSION < 0x0103 )
  #error "XCP Protocol Layer and XCP template are inconsistent!"
#endif
#if ( CP_XCP_VERSION < 0x00 )
 #error "XCP Protocol Layer and XCP on CAN Transport Layer are not consistent!"
#endif


/*****************************************************************************/
/* Constants                                                                 */
/*****************************************************************************/

#if defined ( XCP_ENABLE_VECTOR_MAPNAMES )
V_MEMROM0 static MEMORY_ROM vuint8 xcpMapNames[14] = { '1','2','3','4','5','6','7','8','9','0','A','B','C','D' };
#endif


/*****************************************************************************/
/* Local data definitions                                                    */
/*****************************************************************************/

#if defined ( XCP_ENABLE_CALIBRATION_PAGE )
static vuint8 xcpCalPage; /* 0-Ram,1-Flash */
#endif

#if defined ( XCP_ENABLE_SEED_KEY )
vuint8 xcpResource;
#endif



/*****************************************************************************/
/*                                                                           */
/* Implementation                                                            */
/*                                                                           */
/*****************************************************************************/

#if defined ( XCP_ENABLE_SEED_KEY ) || defined ( XCP_ENABLE_CALIBRATION_PAGE )

void ApplXcpInitTemplate( void )
{
  #if defined ( XCP_ENABLE_SEED_KEY )
  /* Part of the example implementation for Seed & Key. */
  xcpResource = 0;
  #endif


  #if defined ( XCP_ENABLE_CALIBRATION_PAGE )
  /* Part of the example implementation for page switching */
  xcpCalPage = 0;                         /* 0-Ram,1-Flash */
  #endif
}

#endif



#if defined (XCP_ENABLE_DAQ)

  #if defined (XCP_ENABLE_DAQ_RESUME)
/*****************************************************************************/
/* DAQ list resume feature                                                   */
/*****************************************************************************/

void ApplXcpDaqResumeStore( MEMORY_ROM tXcpDaq * daq )
{
  /* something has to be done here */
}


void ApplXcpDaqResumeClear( void )
{
  /* something has to be done here */
}


vuint8 ApplXcpDaqResume( tXcpDaq * daq )
{
  /* something has to be done here */
  return (vuint8)0u;
}

vuint8 ApplXcpCalResumeStore( void )
{
  /* something has to be done here */
  return (vuint8)0u;
}

  #endif /* XCP_ENABLE_DAQ_RESUME */


  #if defined ( XCP_ENABLE_DAQ_TIMESTAMP )
/****************************************************************************/
/* DAQ Timestamp                                                            */
/****************************************************************************/

XcpDaqTimestampType ApplXcpGetTimestamp( void )
{
  return (XcpDaqTimestampType)0u;
}
  #endif /* XCP_ENABLE_DAQ_TIMESTAMP */

#endif /* XCP_ENABLE_DAQ */



#if defined ( XCP_ENABLE_CALIBRATION_PAGE )

/*****************************************************************************/
/* Page switching                                                            */
/* This is just an example how the page switching could be implemented.      */
/*****************************************************************************/

/*****************************************************************************
| NAME:             ApplXcpGetCalPage
| CALLED BY:        XcpCommand
| PRECONDITIONS:    RAM / ROM switching enabled
| INPUT PARAMETERS: segment : 
|                   mode    : 
| RETURN VALUES:    
| DESCRIPTION:      
******************************************************************************/
vuint8 ApplXcpGetCalPage( vuint8 segment, vuint8 mode ) 
{
  return (cal_get_cal_page() == Cal_page_flash) ? 0u : 1u;
}

/*****************************************************************************
| NAME:             ApplXcpSetCalPage
| CALLED BY:        XcpCommand
| PRECONDITIONS:    RAM / ROM switching enabled
| INPUT PARAMETERS: segment : 
|                   page    :
|                   mode    : CAL_ECU and/or CAL_XCP or CAL_ALL
| RETURN VALUES:    
| DESCRIPTION:      
******************************************************************************/
vuint8 ApplXcpSetCalPage( vuint8 segment, vuint8 page, vuint8 mode ) 
{
  cal_status_e cal_status;
  vuint8 canbedded_status;

  if (segment!=0)
  {
    return CRC_OUT_OF_RANGE; /* Only one segment supported */
  }
  if (page>1)
  {
    return CRC_PAGE_NOT_VALID; 
  }
  if ((mode&0x03)!=0x03)
  {
    return CRC_PAGE_MODE_NOT_VALID; /* Only one segment supported */
  }

  cal_status = cal_set_cal_page(Cal_slave_canbedded,
                                (page==0u) ? Cal_page_flash : Cal_page_ram);

  switch (cal_status)
  {
      case Cal_success:
          canbedded_status = (vuint8) 0;
          break;
      case Cal_pending:
          canbedded_status = XCP_CMD_PENDING;
          break;
      default:
          /* something went worng - but don't know what */
          canbedded_status = CRC_GENERIC;
          break;
  }
  return canbedded_status;
}


  #if defined ( XCP_ENABLE_PAGE_COPY )
/*****************************************************************************
| NAME:             ApplXcpCopyCalPage
| CALLED BY:        XcpCommand
| PRECONDITIONS:    RAM / ROM switching enabled
| INPUT PARAMETERS: srcSeg   : 
|                   srcPage  :
|                   destSeg  : 
|                   destPage :
| RETURN VALUES:    
| DESCRIPTION:      
******************************************************************************/
vuint8 ApplXcpCopyCalPage(vuint8 srcSeg, vuint8 srcPage, vuint8 destSeg, vuint8 destPage)
{
    cal_status_e cal_status;
    vuint8 canbedded_status;

  if ((srcSeg != 0u) || (destSeg != 0u))
  {
      canbedded_status = CRC_SEGMENT_NOT_VALID;
  }
  else if ((srcPage > 1u) || (destPage > 1u) || (srcPage == destPage))
  {
      canbedded_status = CRC_PAGE_NOT_VALID;
  }
  else if (destPage == 0u)
  {
      canbedded_status = CRC_WRITE_PROTECTED;
  }
  else
  {
      /* correct request to copy from flash to EEPROM */
      cal_status = cal_copy_flash_cal(Cal_slave_canbedded);
      switch (cal_status)
      {
          case Cal_success:
              canbedded_status = 0;
              break;
          case Cal_pending:
              canbedded_status = XCP_CMD_PENDING;
              break;
          case Cal_failure:
          default:
              /* something went wrong - don't know what */
              canbedded_status = CRC_GENERIC;
      }
  }
  return canbedded_status;
}
  #endif

#endif /* XCP_ENABLE_CALIBRATION_PAGE */


#if defined ( XCP_ENABLE_PAGE_FREEZE )
/*****************************************************************************
| NAME:             ApplXcpSetFreezeMode
| CALLED BY:        XcpCommand
| PRECONDITIONS:    RAM / ROM switching enabled
| INPUT PARAMETERS: segment : 
|                   mode    :
| RETURN VALUES:    
| DESCRIPTION:      
******************************************************************************/
void ApplXcpSetFreezeMode( vuint8 segment, vuint8 mode )
{
  /* something has to be done here */
}

/*****************************************************************************
| NAME:             ApplXcpGetFreezeMode
| CALLED BY:        XcpCommand
| PRECONDITIONS:    RAM / ROM switching enabled
| INPUT PARAMETERS: segment : 
| RETURN VALUES: mode    
| DESCRIPTION:      
******************************************************************************/
uint8 ApplXcpGetFreezeMode( vuint8 segment )
{
  /* something has to be done here */

  return (vuint8)0u;
}
#endif /* XCP_ENABLE_PAGE_FREEZE */



#if defined ( XCP_ENABLE_PROGRAM )

/*****************************************************************************/
/* Flash Programming                                                         */
/* Used only if Flash Programming is required                                */
/*****************************************************************************/

/*****************************************************************************
| NAME:             ApplXcpProgramStart
| CALLED BY:        XcpCommand
| PRECONDITIONS:    flash programming required
| INPUT PARAMETERS: none
| RETURN VALUES:    either doesn't return at all (because the bootloader is
|                   invoked, or returns XCP_CMD_ERROR if conditions are not
|                   correct for reprogramming.
| DESCRIPTION:      Prepare for Flash programming by saving a reprogram
|                   request to internal emulated EEPROM and resetting
******************************************************************************/
vuint8 ApplXcpProgramStart( void )
{
    bool_t programming_allowed;
    vuint8 response=XCP_CMD_ERROR;

    programming_allowed = ixcp_prog_allowed(Reprogram_xcp);

    if (programming_allowed)
    {
        if (ResetReqAccepted!=ms_request_reset(Reset_program_xcp_vehicle))
        {
            programming_allowed =FALSE;
        }
    }

    if (programming_allowed)
    {
        response=XCP_CMD_PENDING;
    }

    /* 
     * if programming is not allowed return XCP_CMD_ERROR
     * XcpCommand() converts this to a GENERIC error response (which is
     * correct according to the XCP spec) 
     */
    return response;
}


/*****************************************************************************
| NAME:             ApplXcpFlashClear
| CALLED BY:        XcpCommand
| PRECONDITIONS:    flash programming required
| INPUT PARAMETERS: address : pointer to memory address
|                   size    : memory size
| RETURN VALUES:    XCP_CMD_ERROR
| DESCRIPTION:      It is an error to receive a PROGRAM_CLEAR request here
|                   because it must have arrived before a PROGRAM_START
******************************************************************************/
vuint8 ApplXcpFlashClear( MTABYTEPTR address, vuint32 size )
{
  /* something has to be done here */

  return (vuint8)XCP_CMD_OK;
}

/*****************************************************************************
| NAME:             ApplXcpFlashProgram
| CALLED BY:        XcpCommand
| PRECONDITIONS:    flash programming required
| INPUT PARAMETERS: data    : pointer to data
|                   address : pointer to memory address
|                   size    : memory size
| RETURN VALUES:    XCP_CMD_ERROR
| DESCRIPTION:      It is an error to receive a PROGRAM request here
|                   because it must have arrived before a PROGRAM_START
******************************************************************************/
vuint8 ApplXcpFlashProgram( MEMORY_ROM BYTEPTR data, MTABYTEPTR address, vuint8 size )
{
  /* something has to be done here */

  return (vuint8)XCP_CMD_ERROR;
}


/*****************************************************************************
| NAME:             ApplXcpReset
| CALLED BY:        XcpCommand
| PRECONDITIONS:    flash programming required
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      We don't expect to encounter a PROGRAM_RESET without
|                   previously entering the bootblock via a PROGRAM_START but
|                   the XCP standard does say it can be used for other purposes
|                   so we'll support it just in case.
******************************************************************************/
void ApplXcpReset( void )
{
    ms_request_reset(Reset_controlled);
}

#endif /* XCP_ENABLE_PROGRAM */



/*****************************************************************************/
/* Platform and implementation specific functions for XCP                    */
/*****************************************************************************/

/*****************************************************************************
| NAME:             ApplXcpGetPointer
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: addr_ext : 8  bit address extension
|                   addr     : any of the following:
|                       A 24-bit global address (if addr_ext == 0)
|                       A 16-bit local address (if addr_ext == 0xff)
|                       The low 16-bits of a 24-bit global address (otherwise)
| RETURN VALUES:    Pointer to the address specified by the parameters. 
| DESCRIPTION:      Convert a memory address from XCP format 8/32bit into a C
|                   pointer.
|                   Used for memory transfers like DOWNLOAD, UPLOAD (MTA).
|                   CANbedded XCP slave uses the convention that an extension
|                   of 0xff means a near variable so these have to be
|                   converted into a global address.
|                   The default version in this file produced the wrong address
|                   for locally addressed RAM variables
******************************************************************************/
#if defined ( ApplXcpGetPointer )
 /* defined as macro */
#else
MTABYTEPTR ApplXcpGetPointer( vuint8 addr_ext, vuint32 addr )
{
    // Here I am
    vuint8 *local_ptr;
    FPTR vuint8 *global_ptr;
    vuint32 global_address;

    if (addr_ext == (vuint8) 0xffu)
    {
        /*
         * We need to convert a local 16-bit address to a global address.
         * Cosmic has a library routine for this, we just need to encourage
         * its use by turning the 16-bit local address into a near pointer and
         * then casting that to a far pointer.
         */
        local_ptr = (vuint8 *) addr;    /* interpret the address as a near
                                           pointer) */
        global_ptr = (FPTR vuint8 *) local_ptr; /* convert to a far pointer */
        global_address = cosmic_global_address_from_fptr(global_ptr);
                                        /* and convert back to an address */
    }
    else
    {
        /* put the 24-bit address together */
        global_address = (((vuint32) addr_ext) << 16u)  | addr;
    }

    return global_address;
}

#endif /* ApplXcpGetPointer */


#if defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL ) 

/*****************************************************************************
| NAME:             ApplXcpRead
| CALLED BY:        XCP Protocol Layer
| PRECONDITIONS:    none
| INPUT PARAMETERS: addr : 32 bit address
| RETURN VALUES:    Value specified by the address parameters. 
| DESCRIPTION:      Read a single byte from the memory.
******************************************************************************/
vuint8 ApplXcpRead( vuint32 addr )
{
  #if defined ( C_COMP_COSMIC_MCS12X_MSCAN12 )
  /* X: Higher Word Address */
  /* D: Lower Word Address */

  /* Return: B */
    #asm
      tfr   ccrw,y
      sei
      exg   d,x
      ldaa  $10
      stab  $10
      gldab 0,x
      staa  $10
      tfr   y,ccrw
    #endasm
  #else
  return *((vuint8*)addr);
  #endif
}  

/*****************************************************************************
| NAME:             ApplXcpWrite
| CALLED BY:        XCP Protocol Layer
| PRECONDITIONS:    none
| INPUT PARAMETERS: addr : 32 bit address
|                   data : data to be written to memory
| RETURN VALUES:    none
| DESCRIPTION:      Write a single byte to RAM.
******************************************************************************/
void XCP_FAR ApplXcpWrite( vuint32 addr, vuint8 data )
{
  #if defined ( C_COMP_COSMIC_MCS12X_MSCAN12 )
  /* X: Higher Word Address */
  /* D: Lower Word Address */
  /* Stack: Data */
    #asm
      tfr   ccrw,y
      sei
      exg   d,x
      ldaa  $10       
      stab  $10
    #if defined ( __CSMC__ ) && ( (__CSMC__ & 0x01) == 0x01 )
      ldab  3,s
    #else
      ldab  4,s
    #endif
      gstab 0,x
      staa  $10
      tfr   y,ccrw
    #endasm
  #else
  *((vuint8*)addr) = data;
  #endif
}

  #if defined ( XCP_ENABLE_CHECKSUM )
     #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD22 ) || \
         ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD24 ) || \
         ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD44 )
/*****************************************************************************
| NAME:             ApplXcpReadChecksumValue
| CALLED BY:        XCP Protocol Layer
| PRECONDITIONS:    none
| INPUT PARAMETERS: addr : 32 bit address
| RETURN VALUES:    Pointer to the address specified by the parameters. 
| DESCRIPTION:      Read and return memory with size of tXcpChecksumAddType.
******************************************************************************/
tXcpChecksumAddType ApplXcpReadChecksumValue( vuint32 addr )
{
  return *((tXcpChecksumAddType*)addr);
}  
    #endif
  #endif
#endif


#if defined ( XCP_ENABLE_WRITE_PROTECTION )
/*****************************************************************************
| NAME:             ApplXcpCheckWriteAccess
| CALLED BY:        XcpWriteMta
| PRECONDITIONS:    none
| INPUT PARAMETERS: address : address
|                   size    : size of memroy area
| RETURN VALUES:    XCP_CMD_DENIED : if access is denied
|                   XCP_CMD_OK     : if access is granted
| DESCRIPTION:      Check addresses for valid write access
|                   Used only, if write protection of memory areas is required
******************************************************************************/
  #if defined ( ApplXcpCheckWriteAccess ) /* defined as macro */
  #else

vuint8 ApplXcpCheckWriteAccess( MEMORY_ROM MTABYTEPTR address, vuint8 size )
{
  /* Protect variable xcp */
  vuint8 xcp;

  if (address+size>=(MTABYTEPTR)&xcp && address<(MTABYTEPTR)&xcp+sizeof(xcp))
  {
    return (vuint8)XCP_CMD_DENIED;
  }

  return (vuint8)XCP_CMD_OK;
}
  #endif  /* ApplXcpCheckWriteAccess */

#endif /* XCP_ENABLE_WRITE_PROTECTION */

#if defined ( XCP_ENABLE_READ_PROTECTION )
/*****************************************************************************
| NAME:             ApplXcpCheckReadAccess
| CALLED BY:        XcpReadMta
| PRECONDITIONS:    none
| INPUT PARAMETERS: address : address
|                   size    : size of memroy area
| RETURN VALUES:    XCP_CMD_DENIED : if access is denied
|                   XCP_CMD_OK     : if access is granted
| DESCRIPTION:      Check addresses for valid write access
|                   Used only, if write protection of memory areas is required
******************************************************************************/
  #if defined ( ApplXcpCheckReadAccess ) /* defined as macro */
  #else

uint8 ApplXcpCheckReadAccess( MEMORY_ROM MTABYTEPTR address, vuint8 size )
{
  /* Protect variable xcp */
  vuint8 xcp;

  if (address+size>=(MTABYTEPTR)&xcp && address<(MTABYTEPTR)&xcp+sizeof(xcp))
  {
    return (vuint8)XCP_CMD_DENIED;
  }

  return (vuint8)XCP_CMD_OK;
}
  #endif  /* ApplXcpCheckReadAccess */
#endif /* XCP_ENABLE_READ_PROTECTION */


#if defined ( XCP_ENABLE_BOOTLOADER_DOWNLOAD )

/*****************************************************************************
| NAME:             ApplXcpDisableNormalOperation
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: a    : address
|                   size : number of bytes
| RETURN VALUES:    XCP_CMD_OK or XCP_CMD_DENIED
| DESCRIPTION:      Initiate Flash Download
|                   Used only if Download of the Flash Kernel is required
******************************************************************************/
vuint8 ApplXcpDisableNormalOperation( MTABYTEPTR a, vuint16 size )
{
  /*
    CANape attempts to download the flash kernel to XCP_RAM.
    Address is a
    Size is size bytes
  */

  /* ... */

  /* return XCP_CMD_DENIED;    Refused */
  return (vuint8)XCP_CMD_OK;     /* Ok */
}


/*****************************************************************************
| NAME:             ApplXcpStartBootLoader
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    
| DESCRIPTION:      
******************************************************************************/
vuint8 ApplXcpStartBootLoader( void )
{

  /* ... */

  return (vuint8)XCP_CMD_DENIED;
}

#endif /* XCP_ENABLE_BOOTLOADER_DOWNLOAD */


#if defined ( XCP_ENABLE_WRITE_EEPROM )
/*****************************************************************************
| NAME:             ApplXcpCheckWriteEEPROM
| CALLED BY:        XcpWriteMta
| PRECONDITIONS:    none
| INPUT PARAMETERS: addr : address
|                   size : number of bytes
|                   data : pointer to data
| RETURN VALUES:    XCP_CMD_OK      : EEPROM written
|                   XCP_CMD_DENIED  : This is not EEPROM
|                   XCP_CMD_PENDING : EEPROM write in progress, call XcpSendCrm when done
| DESCRIPTION:      EEPROM Programming
******************************************************************************/
vuint8 ApplXcpCheckWriteEEPROM( MTABYTEPTR addr, vuint8 size, MEMORY_ROM BYTEPTR data )
{
    cal_status_e cal_status;
    vuint8 canbedded_status;
    FPTR uint8_t *dest_p;

    dest_p = cosmic_fptr_from_global_address(addr);

    cal_status = cal_write(Cal_slave_canbedded,dest_p,size,data);

    switch (cal_status)
    {
        case Cal_pending:
            canbedded_status = XCP_CMD_PENDING;
            break;
        case Cal_success:
            canbedded_status = XCP_CMD_OK;
            break;
        case Cal_failure:
        default:
            /* 
             * CANbedded interface only allows XCP_CMD_SYNTAX as a possible
             * error code here.  XCP_CMD_DENIED would actually lead to memory
             * being written directly in XcpWriteMta, so we can't use that.
             */
            canbedded_status = XCP_CMD_SYNTAX;
            break;
    }
  return canbedded_status;
}
#endif

#if defined ( XCP_ENABLE_READ_EEPROM )
/*****************************************************************************
| NAME:             ApplXcpCheckReadEEPROM
| CALLED BY:        XcpReadMta
| PRECONDITIONS:    none
| INPUT PARAMETERS: addr : address
|                   size : number of bytes
|                   data : pointer to data
| RETURN VALUES:    0 (FALSE) : This is not EEPROM
|                   1 (TRUE)  : EEPROM read
| DESCRIPTION:      Read EEPROM
******************************************************************************/
vuint8 ApplXcpCheckReadEEPROM( MTABYTEPTR addr, vuint8 size, BYTEPTR data )
{
  /* something has to be done here */

  return (vuint8)XCP_CMD_DENIED;
}
#endif



/*****************************************************************************/
/* Seed & Key                                                                */
/* This is just an example how seed & key could be implemented.              */
/*****************************************************************************/
#if defined ( XCP_ENABLE_SEED_KEY )

/*****************************************************************************
| NAME:             ApplXcpGetSeed
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: resource : resource
|                              (either RM_CAL_PAG or RM_DAQ or RM_STIM or RM_PGM)
|                   seed     : pointer to the generated seed.
| RETURN VALUES:    the length of the seed.
| DESCRIPTION:      Generates a seed.
|                   Attention: The seed has a maximum length of MAX_CTO-2 bytes.
******************************************************************************/
vuint8 ApplXcpGetSeed( MEMORY_ROM vuint8 resource, vuint8 *seed )
{
  /* Store resource mask */
  xcpResource = resource;

  /* Generate a seed */
  seed[0] = 0;
  seed[1] = 1;
  seed[2] = 2;
  seed[3] = 3;
  seed[4] = 4;
  seed[5] = 5;

  return (vuint8)6u;
}

/*****************************************************************************
| NAME:             ApplXcpUnlock
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: key    : pointer to the key 
|                   length : length of the key
| RETURN VALUES:    0 : if the key is not valid
|                   RM_CAL_PAG, RM_DAQ, RM_STIM, RM_PGM : for valid keys
| DESCRIPTION:      Check key and return the resource that has to be unlocked.
******************************************************************************/
vuint8 ApplXcpUnlock( MEMORY_ROM vuint8 *key, MEMORY_ROM vuint8 length )
{
  /*
    Ckeck the key
    key[0],key[1],key[2],key[3],key[4],key[5]
  */

  return xcpResource;
}

#endif /* XCP_ENABLE_SEED_KEY */


/****************************************************************************/
/* GET_ID service                                                           */
/****************************************************************************/
#if defined ( XCP_ENABLE_VECTOR_MAPNAMES )

/*****************************************************************************
| NAME:             ApplXcpGetIdData
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: pData : pointer to pointer of the MAP file names
| RETURN VALUES:    length of the MAP file names
| DESCRIPTION:      Returns a pointer to a pointer of MAP file names
******************************************************************************/
vuint32 ApplXcpGetIdData( MTABYTEPTR *pData)
{
    *pData = (MTABYTEPTR)xcpMapNames;

    return (vuint32)sizeof(xcpMapNames);
}

#endif /* XCP_ENABLE_VECTOR_MAPNAMES */


/****************************************************************************/
/* User defined service                                                     */
/****************************************************************************/
#if defined ( XCP_ENABLE_USER_COMMAND )

/**
 * Function to handle "User Command" requests.  Currently just return
 * XCP_CMD_SYNTAX, which will lead to an error being reported
 */
vuint8 ApplXcpUserService( MEMORY_ROM BYTEPTR pCmd )
{

  /* something has to be done here */


  vprg_xcp_user_command(pCmd);
  /* vprg will send the dto so tell CANbedded to send nothing */
  return (vuint8)XCP_CMD_PENDING;
}

#endif


/****************************************************************************/
/* Open Command Interface                                                   */
/****************************************************************************/
#if defined ( XCP_ENABLE_OPENCMDIF )

vuint8 ApplXcpOpenCmdIf( MEMORY_ROM BYTEPTR pCmd, BYTEPTR pRes, BYTEPTR pLength )
{
  if(0xC1 == pCmd[0])
  {
    /* Implementation for XCP command 0xC1 has to be done here */
    pRes[1]  = 0x11; /* Set response */
    pRes[2]  = 0x22;
    *pLength = 3; /* Set response length */
    return (vuint8)XCP_CMD_OK;
  }
  return (vuint8)XCP_CMD_UNKNOWN;
}

#endif





/****************************************************************************/
/* ApplXcpSendStall                                                             */
/****************************************************************************/

#if defined ( XCP_ENABLE_SEND_EVENT ) || defined ( XCP_ENABLE_SERV_TEXT_PUTCHAR )
  #if defined ( ApplXcpSendStall )
 /* ApplXcpSendStall is already defined or a macro */
  #else
vuint8 ApplXcpSendStall( void )
{
  /* something has to be done here */

  return( (vuint8) 1u );
}
  #endif
#endif

/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
