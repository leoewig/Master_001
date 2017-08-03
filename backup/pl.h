/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    pl.h 
 *
 * description:  Public header file for the Pack Layout feature.
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   Initial Version
 *                       
 *
 *****************************************************************************/
#if !defined(PL_H)
#define      PL_H
/**
 *
 * Public header file for the Pack Layout feature.
 *
 * @file
 * @ingroup pl
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 * Topology of VTBMs and cells
 *
 * The Pack Layout feature verifies the battery pack configuration. This feature
 * verifies the following battery pack configuration parameters,
 *  - Number of VTBMs in the battery pack (from #pl_num_vtbms)
 *  - Total number of cells in the battery pack (from #pl_num_cells_on_vtbm)
 *  - VTBM power supply configuration (from #pl_vtbm_pwr_src)
 *  and calculates the cell index of the first cell on each vtbm
 *  (#pl_first_cell_on_vtbm)
 *
 * ## Run-time checks
 * The run-time checks are done in pl_init(). This function is called during the
 * system initialisation before any other MMC application feature is initialised.
 * The pl_init() function calls,
 *  - pl_verify_num_vtbms() to verify the number of VTBMs
 *  - pl_verify_num_cells() to verify the total number of cells
 *  - pl_verify_vtbm_pwr_sply_cfg() to verify the VTBM power supply configuration
 *  - pl_calculate_cell_start_indices() to calculate the first cell attached to each
 *    VTBM.
 *
 * #### Verifying the total number of VTBMS (pl_verify_num_vtbms())
 * pl_num_vtbms is checked to lie between 2 and #MAX_NUM_VTBM inclusive.
 * If the check fails, the bad_pack_layout fault is raised.
 *
 * #### Verifying the total number of cells (pl_verify_num_cells())
 * The sum of the entries in in #pl_num_cells_on_vtbm is calculated and
 * checked to be equal to #cmc_num_batteryPackCells and equal to or less
 * than #MAX_NUM_CELLS.
 * If the check fails, the bad_pack_laoyout fault is raised.
 *
 * #### Verifying VTBM power supply configuration (pl_verify_vtbm_pwr_sply_cfg())
 * The power supply configuration #pl_vtbm_pwr_src is checked to ensure
 * that all entries are less than #pl_num_vtbms and every VTBM is referenced
 * precisely once.
 *
 * ### Calculating the index of the first cell on each VTBM (pl_calculate_cell_start_indices())
 * The PL feature, calculates the cell start index for
 * each VTBM in the pack. The index of the first cell on each VTBM is placed
 * in the array #pl_first_cell_on_vtbm.  The index of the first
 * cell on vtbm0 is 0. The index of the first cell on rest of the VTBMs is
 * calculated as,
 *
 * pl_first_cell_on_vtbm[N] = pl_first_cell_on_vtbm[N-1] + pl_num_cells_on_vtbm[N-1]
 * ## How to use
 * The PL feature is S12 code and is called from a single task.
 *
 * The pl_init() function should be called before the balancing feature is
 * initialised.
 *
 * ## Requirement Satisfaction
 * The requirements for this feature can be found in the
 * <a href="http://viewvc/viewvc.cgi/trunk/docs/reqs/generic/RU_BMS_Balancing.doc?root=Q52717_NightWing&view=co">
 * Balancing Requirements Section 2 Pack Layout Requirements</a> document.
 *
 * @todo: fix the requirements trace
 *
 * | Requirement    | Satisfied By           | Unit Tested By | PAT Tested by |
 * | ---------------|------------------------|----------------|---------------|
 * | REQ_BMS_PL_001 | #pl_num_vtbms          | | |
 * | REQ_BMS_PL_009 | n/a                    | | |
 * | REQ_BMS_PL_010 | n/a                    | | |
 * | REQ_BMS_PL_002 | #pl_num_cells_on_vtbm  | | |
 * | REQ_BMS_PL_004 | #pl_vtbm_pwr_src | | |
 * | REQ_BMS_PL_006 | #pl_init()             | | |
 * | REQ_BMS_PL_008 | #pl_vtbm_pwr_src | | |
 *
 * @defgroup pl PL Pack Layout
 *
 *

 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "pl_types.h"
#include "mmc.h"
#include "mdm_types.h"  /* for MAX_NUM_VTBM */


/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern CAL uint8_t pl_num_vtbms;
extern CAL cellcount_t pl_num_cells_on_vtbm[MAX_NUM_VTBM];
extern CAL uint8_t pl_vtbm_pwr_src[MAX_NUM_VTBM];

extern uint8_t pl_vtbm_pwr_sink[MAX_NUM_VTBM];
extern cellcount_t pl_first_cell_on_vtbm[MAX_NUM_VTBM];
extern bool_t pl_bad_pack_layout;
extern pl_vtbm_ps_cfg_err_e pl_vtbm_ps_cfg_status;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/* Documented in source */
void pl_init(void);

#endif      /* PL_H */
