/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  crc_p.h
 *
 * description:  
 *   Private header for crc utility.
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
#if !defined(CRC_P_H)
#define      CRC__P_H
/**
 * Private header for crc utility.
 *
 * @file
 * @ingroup crc
 *
 */

/* Copyright (c) Linkedriving Tech 2015 - All rights resevered */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
#if defined UNIT_TEST
extern uint16_t crc16_table[];
extern uint32_t crc32_table[];
#endif
/******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 *****************************************************************************/

#endif      /* CRC_H */
