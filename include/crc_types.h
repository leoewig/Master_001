/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  crc_types.h
 *
 * description:  
 *  Public types and macros used by crc utility
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
#if !defined(CRC_TYPES_H_)
#define CRC_TYPES_H_

/**
 * Public types and macros used by crc utility
 *
 * @file
 * @ingroup crc
 *
 */


/******************************************************************************
 * INCLUDES
 *****************************************************************************/

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/
#define CRC16_SEED 0xffffU /**< Macro to specify the initial CRC-CCIT seed. */
#define CRC32_SEED 0xffffffffU /**< Macro to specify the initial CRC32 seed. */

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
typedef uint16_t    crc16_t; /**< CRC16-CCIT type */
typedef uint32_t    crc32_t; /**< CRC-32 type */

#endif /* CRC_TYPES_H_ */
