/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   sec.h
 *
 * description:   Header for security seed and key generation
 *
 *
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
#ifndef SEC_H
#define SEC_H
/**
 * Header for security seed and key generation
 *
 * @file
 * @ingroup sec
 *
 */
/**
 * Seed and key generation for application and FBL
 *
 * @defgroup sec SEC Security Seed and Key
 */
/* Copyright (c) Linkedriving Tech 2013 */

/*
 * NB Prototypes are declared for both the app and FBL so that the prodder
 * can call them both
 */
/**
 * Generate a 32-bit seed from a 16-bit 'random' number such as a timer
 */
uint32_t sec_seed_gen_app(uint16_t random16);
/**
 * Generate a 32-bit seed from a 16-bit 'random' number such as a timer
 */
uint32_t sec_seed_gen_fbl(uint16_t random16);

/**
 * Generate a 32-bit key from a 16-bit seed using the same algorithm as a
 * customer scan tool
 */
uint32_t sec_key_gen_app(uint32_t seed);
/**
 * Generate a 32-bit key from a 16-bit seed using the same algorithm as a
 * customer scan tool
 */
uint32_t sec_key_gen_fbl(uint32_t seed);

#endif
