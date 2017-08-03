/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    build_idents.h 
 *
 * description:  
 * This file is a central location for ID codes used to identufy a S.U.T
 * in its RESET CAN Frame sent on the Control Bus
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
#if ! defined BUILD_IDENTS_H
#define BUILD_IDENTS_H

/**
 *
 * This file is a central location for ID codes used to identufy a S.U.T
 * in its RESET CAN Frame sent on the Control Bus
 *
 * @file
 * @ingroup mmc mmc_test_harness harness_pc_interface vtbm mmc_test_harness
 */


/** codes transmitted in RESET CAN frame to identify a S.U.T */

#define BUILD_IDENT_MMC                (0x80u)
#define BUILD_IDENT_MMC_TEST_HARNESS   (0x81u)
#define BUILD_IDENT_VTBM               (0x82u)
#define BUILD_IDENT_VTBM_TEST_HARNESS  (0x83u)


#endif /* ! defined BUILD_IDENTS_H */
