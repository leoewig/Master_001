/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  danomem_types.h
 *
 * description:  
 * Dan-O-Mem provides read and write access to all available memory.
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
#if !defined(DANOMEM_TYPES_H_)
#define DANOMEM_TYPES_H_

/**
 * Dan-O-Mem enumerators
 *
 * @file
 * @ingroup danomem
 *
 * $Id: danomem_types.h 10338 2013-08-30 16:18:35Z pr4 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 * Dan-O-Mem interface
 *
 * Overview
 * ========
 * Dan-O-Mem provides read and write access to all available memory.
 *
 * How to Use
 * ==========
 * Use Dan-O-Mem 2.0 Protocol specification described bellow to request read or write
 * operation.
 *
 * For mmc, mmc_test_harness and prodder
 * -------------------------------------
 * - Include mmc_danomem.h
 * - Call danomem() periodically
 *
 * For vtbm, vtbm_test_harness and vrodder
 * ---------------------------------------
 * - Include vtbm_danomem.h
 * - Write CAN reception parser on desired bus and call danomem() with #ican_msg_s type
 * pointer to received ican message containing danomem data, if CAN ID matches DANOMEM_WRITE_REQ,
 * or DANOMEM_READ_REQ. See an example bellow.
 *
 * @code
 * struct ican_rxtxq_s *rxqp_command = &ican_s12_rxq[Can_command];
 * struct ican_msg_s *msgp;
 *
 * if ( ! QUEUE_EMPTY(rxqp_command) )
 * {
 *      msgp = OLDEST_MESSAGE(rxqp_command);
 *      switch (msgp->id)
 *      {
 *          case DANOMEM_WRITE_REQ:
 *          case DANOMEM_READ_REQ:
 *              danomem(msgp);
 *              break;
 *          default:
 *              ... process message with other IDs
 *              break;
 *      }
 *      REMOVE_MESSAGE(rxqp_command);
 * }
 * @endcode
 *
 * Dan-O-Mem 2.0 Protocol specification
 * ====================================
 *
 * Prodder and Vrodder target messages
 * -----------------------------------
 *
 * | Name          | Direction | Message ID | DLC | Byte 0      | Byte 1 - 3 | Byte 4 - 7 |
 * |:-------------:|:---------:|:----------:|:---:|:-----------:|:----------:|:----------:|
 * | Write Request | PC -> PRD | 0x7E0      | 8   | Type        | Address    | Data       |
 * | Write Response| PRD -> PC | 0x7E1      | 8   | Type / Error| Address    | Data       |
 * | Read Request  | PC -> PRD | 0x7E2      | 8   | Type        | Address    | Don't care |
 * | Read Response | PRD -> PC | 0x7E3      | 8   | Type / Error| Address    | Data       |
 *
 * MMC, VTBM, MMC test harness and VTBM test harness messages
 * ----------------------------------------------------------
 *
 * | Name          | Direction | Message ID | DLC | Byte 0      | Byte 1 - 3 | Byte 4 - 7 |
 * |:-------------:|:---------:|:----------:|:---:|:-----------:|:----------:|:----------:|
 * | Write Request | PC -> MMC | 0x7E4      | 8   | Type        | Address    | Data       |
 * | Write Response| MMC -> PC | 0x7E5      | 8   | Type / Error| Address    | Data       |
 * | Read Request  | PC -> MMC | 0x7E6      | 8   | Type        | Address    | Don't care |
 * | Read Response | MMC -> PC | 0x7E7      | 8   | Type / Error| Address    | Data       |
 *
 * Data field format
 * -----------------
 *
 * Data field meaning and format depends on the type. Available types and according
 * data field are listed in following table.
 *
 * | Type          | Byte 4 | Byte 5 | Byte 6 | Byte 7 |
 * |:-------------:|:------:|:------:|:------:|:------:|
 * | 8             | B0     | -      | -      | -      |
 * | 16            | B1     | B0     | -      | -      |
 * | 24            | B2     | B1     | B0     | -      |
 * | 32            | B3     | B2     | B1     | B0     |
 *
 * Other type values are invalid and will cause Dan-O-Mem to report an error
 * (value 0x80 in Byte 0 of the response) and an operation will not take place.
 *
 * @defgroup danomem Dan-O-Mem
 * @ingroup platform
 */

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
/**
 * Dan-O-Mem result type
 */
typedef enum
{
    DM_res_ok, /**< Operation successful */
    DM_res_err, /**< Operation failed */
    DM_res_pend
} danomem_res_t;

/**
 * Dan-O-Mem data types
 */
typedef enum
{
    DM_type_8 = 8u, /**< one byte of data */
    DM_type_16 = 16u, /**< two bytes of data */
    DM_type_24 = 24u, /**< three bytes of data */
    DM_type_32 = 32u /**< four bytes of data */
} danomem_type_t;

/**
 * Dan-O-Mem error codes
 */
typedef enum
{
    DM_error_wrong_type = 0x80u /**< Wrong type */
} danomem_error_t;

#endif /* DANOMEM_TYPES_H_ */
