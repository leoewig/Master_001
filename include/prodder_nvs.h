/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   prodder_nvs.h 
 *
 * description:  NVS access from the prodder
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           Initial version
 *                       
 *
 *****************************************************************************/
#ifndef PRODDER_NVS_H
#define PRODDER_NVS_H
/**
 * NVS access from the prodder
 *
 * @file
 * @ingroup prodder
 *
 */
typedef enum {
    Prod_nvs_idle,
    Prod_nvs_read_feature,
    Prod_nvs_write_feature
} PROD_nvs_command_code_e ;

void prodder_nvs_machine(void);
#endif
