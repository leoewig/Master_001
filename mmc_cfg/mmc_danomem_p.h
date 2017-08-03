/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_danomem_p.h
 *
 * description:  
 * Dan-O-Mem private header for mmc, mmc test harness and prodder targets
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/
#if !defined(MMC_DANOMEM_P_H_)
#define MMC_DANOMEM_P_H_



/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/*
 * CANbedded signal name mappings
 */
#if !defined PRODDER
#define RDREQ_TYPE      DMsut_rdreq_type
#define RDREQ_ADDRESS   DMsut_rdreq_address

#define RDRSP_TYPE      DMsut_rdrsp_type
#define RDRSP_ADDRESS   DMsut_rdrsp_address
#define RDRSP_DATA      DMsut_rdrsp_data

#define WRREQ_TYPE      DMsut_wrreq_type
#define WRREQ_ADDRESS   DMsut_wrreq_address
#define WRREQ_DATA      DMsut_wrreq_data

#define WRRSP_TYPE      DMsut_wrrsp_type
#define WRRSP_ADDRESS   DMsut_wrrsp_address
#define WRRSP_DATA      DMsut_wrrsp_data

#else

#define RDREQ_TYPE      DMprd_rdreq_type
#define RDREQ_ADDRESS   DMprd_rdreq_address

#define RDRSP_TYPE      DMprd_rdrsp_type
#define RDRSP_ADDRESS   DMprd_rdrsp_address
#define RDRSP_DATA      DMprd_rdrsp_data

#define WRREQ_TYPE      DMprd_wrreq_type
#define WRREQ_ADDRESS   DMprd_wrreq_address
#define WRREQ_DATA      DMprd_wrreq_data

#define WRRSP_TYPE      DMprd_wrrsp_type
#define WRRSP_ADDRESS   DMprd_wrrsp_address
#define WRRSP_DATA      DMprd_wrrsp_data

#endif

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
PRIVATE void danomem_write_request(void);
PRIVATE void danomem_read_request(void);

#endif /* MMC_DANOMEM_P_H_ */
