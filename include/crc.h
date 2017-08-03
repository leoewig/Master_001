/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:  crc.h
 *
 * description:  
 *   This utility calculates a CRC checksum of a given area of memory.
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
#if !defined(CRC_H)
#define      CRC_H
/**
 * This utility calculates a CRC checksum of a given area of memory.
 *
 * @file
 * @ingroup crc
 *
 */



/**
 * <h1>CRC utility </h1>
 *
 * <h2> CRC Overview </h2>
 * The basic idea of CRC (cyclic redundancy checksum) algorithms is simply to treat the message
 * as an enormous binary number, to divide it by another fixed binary number and to make
 * the remainder from this division the checksum. Upon receipt of the message, the receiver
 * can perform the same division and compare the remainder with the "checksum"
 * (transmitted remainder).
 *
 * This utility provides functions calculating CRC checksums of a given block of memory
 * using look-up table based method in sake of computation speed.
 *
 * <h2> Supported CRC algorithms </h2>
 * Currently, this utility supports CRC-CCIT and CRC-32 algorithms. Since there's
 * no clear standard definition of each algorithm, except polynomial representing
 * a divisor to be used, each algorithm may be fully defined by following parameters:
 * - Width: Width of the algorithm expressed in bits. Note that width is always
 * one less than width of the polynomial used
 * - Polynomial: Polynomial used for CRC calculation, i.e. the divisor. Representation
 * used here suggests, that the top bit of the polynomial may be omitted, while not
 * losing any information. For example x^16 + x^12 + x^5 + 1 polynomial may
 * be fully represented by 0x1021 binary number.
 * - Initial value: This parameter specifies the initial value of the register
 * when the algorithm starts
 * - Reflected input: This boolean specifies whether the algorithm shall reflect
 * input data, i.e. if FALSE, it process MSB bit of input data first. If it's TRUE,
 * it process LSB bit of data first.
 * - Reflected output: This boolean specifies whether the calculated value shall be
 * reflected or not
 * - XOR Out: A value to be XORed with calculated value
 *
 * <h3> CRC-CCIT </h3>
 * - Width: 16 bits
 * - Polynomial: 0x1021
 * - Initial seed: 0xffff
 * - Reflected on input: No
 * - Reflected on output: No
 * - Output XORed with value: 0x0000
 *
 * <h3> CRC-32 </h3>
 * - Width: 32 bits
 * - Polynomial: 0x04c11db7
 * - Initial seed: 0xffffffff
 * - Reflected on input: Yes
 * - Reflected on output: Yes
 * - Output XORed with value: 0xffffffff
 *
 * <h2> Limitations </h2>
 * - The CRC calculation will not handle memory regions spanning both paged and
 * non-paged memory.
 * - Maximal length of memory region to be calculated is limited to 65535 bytes
 *
 * <h2> How to use </h2>
 * The first (or only) call to crc16() or crc32() should specify the seed as #CRC16_SEED,
 * resp. #CRC32_SEED, subsequent calls should specify the previously returned value
 * as the next seed.
 *
 * <h2> Requirement Satisfaction </h2>
 * The requirements document for the feature can be found in
 * <a href="http://viewvc/viewvc.cgi/trunk/docs/reqs/generic/RU_BMS_Support.doc?root=Q52717_NightWing&view=co">RU BMS Support</a>.
 *
 * All requirements are satisfied by this module.
 *
 * Requirements are satisfied as follows:
 *
 * | Requirement          | Satisfied By | Tested By |
 * | ---------------------|--------------|-----------|
 * | REQ_BMS_SUP_CRC_001  | crc16()      | test_crc16_seed(), test_crc16_start(), test_crc16_start_address(), test_crc16_length() |
 * | REQ_BMS_SUP_CRC_002  | crc16()      | test_crc16_table() |
 * | REQ_BMS_SUP_CRC_003  | crc16()      | test_crc16_table() |
 * | REQ_BMS_SUP_CRC_004  | crc16()      | _not tested_ |
 * | REQ_BMS_SUP_CRC_005  | crc16()      | test_crc16_seed(), test_crc16_start(), test_crc16_start_address(), test_crc16_length() |
 * | REQ_BMS_SUP_CRC_006  | crc16()      | test_crc16_seed() |
 * | REQ_BMS_SUP_CRC_007  | crc32()      | test_crc32_seed(), test_crc32_start(), test_crc32_start_address(), test_crc32_length() |
 * | REQ_BMS_SUP_CRC_008  | crc32()      | test_crc32_table() |
 * | REQ_BMS_SUP_CRC_009  | crc32()      | test_crc32_table() |
 * | REQ_BMS_SUP_CRC_010  | crc32()      | _not tested_ |
 * | REQ_BMS_SUP_CRC_011  | crc32()      | test_crc32_seed(), test_crc32_start(), test_crc32_start_address(), test_crc32_length() |
 * | REQ_BMS_SUP_CRC_012  | crc32()      | test_crc32_seed() |
 *
 * @untestable
 * - REQ_BMS_SUP_CRC_004, REQ_BMS_SUP_CRC_010 - both crc16() and crc32() are found reentrant
 * by code inspection
 *
 * @defgroup crc Cyclic Redundancy Check
 * @ingroup utils
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "crc_types.h"

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/* documentation in source file */
uint16_t crc16 (uint16_t crc_in, FPTR const uint8_t* addr, uint16_t size);
uint32_t crc32 (uint32_t crc_in, FPTR const uint8_t* data_p, uint16_t size);

#endif      /* CRC_H */
