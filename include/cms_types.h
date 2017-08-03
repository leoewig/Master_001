/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   cms_types.h
 *
 * description:  
 * Cluster messages enumeration
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
#if !defined(CMS_TYPES_H_)
#define CMS_TYPES_H_

/**
 * Cluster messages enumeration
 *
 * @file
 * @ingroup cms
 *
 */

/* Copyright (c) Linkedriving Tech 2015 - All rights resevered */


/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/**
 * Macro used for cluster message extraction from cms.def file
 */
#define CMS_TABLE(msg, id)   Cms_ ## msg = id,

/**
 * Enumeration of available cluster messages. Each message
 * defined in cms.def file with CMS_TABLE macro will be extracted
 * here.
 */
enum cms_messages_e
{
    Cms_no_message,                  /**< No cluster message request */
#include "cms.def"
    Cms_messages_max,                /**< Number of cluster messages */
    Cms_clear_all_requests           /**< Clear all cluster messages request*/
};
#undef CMS_TABLE

#endif /* CMS_TYPES_H_ */
