/**
 *
 * Digital I/O types
 *
 * @file
 * @ingroup dio
 * $Id: dio_types_cust.h 873 2015-03-16 11:19:41Z j_bailey $
 */

/* Copyright (c) Ricardo plc 2015 - All rights reserved */

#ifndef DIO_TYPES_CUST_H
#define DIO_TYPES_CUST_H   /* multiple inclusion protection */

#include "mmc.h"

typedef struct
{
    data_validity_e validity;
    percent_t       duty;
    frequency_t     freq;
} Dio_pin_signal_data_t;

#endif  /* DIO_TYPES_CUST_H */
