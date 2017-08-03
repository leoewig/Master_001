/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   cgs_ccc.h
 *
 * description:  
 * Public header for Charge Current Control feature (cgs_ccc)
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
/*************************************************************************//**
 *
 * Public header for Charge Current Control feature (cgs_ccc)
 *
 *
 *****************************************************************************
 *
 * This header defines the public interface to the cgs module
 *
 *****************************************************************************/

/**
 *
 * @defgroup chargingoverview CGS Charging Overview
 */

/**
 * This module covers Charger Co-ordination and Charging Strategy.
 *
 * @defgroup cgs_ccc CGS Charger Current Control
 * @ingroup chargingoverview
 *
 *
 * # Charger Current Control Feature #
 *
 * The aim of this feature is to provide charger control in order to charge the
 * pack, including determination of whether charging is possible.
 *
 * ## Approach used #
 * The charging current is controlled by an integral controller.
 * In order to avoid massive overshoot of the intergral whilst the voltage is 
 * reaching the target a proportional control is used to calculate a limit for
 * the current (making this a rather unconventional PI controller).
 *
 * ## Limitations #
 *
 * ## Integration #
 *
 *  ## Validity Flags #
 * This component is purely a calculation engine. If there are faults that 
 * need to be dealt with, they should be addressed elsewhere.
 *
 * External variables used by cgs_chg:
 *
 * | External variable                           | Validity flag                   | Where checked  |
 * | --------------------------------------------|---------------------------------|----------------|
 * | mdm_cell_v_max_value     | mdm_cell_v_valid | cgs_capacity() |
 *
 * External variables used by cgs_cco:
 *
 * | External variable                           | Validity flag                   | Where checked  |
 * | --------------------------------------------|---------------------------------|----------------|
 * | dio_ign_dbn | no validity flag | - |
 *
 *
 */

#if !defined(CGS_CCC_H)
#define      CGS_CCC_H

/******************************************************************************
 * MACROS
 *****************************************************************************/

#define CGS_CCC_V_TARGET() ((pack_v_t)mulu16(cgs_cv_up_cell_v_lim,\
            (uint16_t)mdm_cellcount_num,\
            CELL_V_BP , COUNT_BP, PACK_V_BP))

/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "nvs_cfg.h"
#include "mmc.h"

#include "cgs_types.h"
#include "ctc_types.h"

/******************************************************************************/

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/
extern ts_time_t      cgs_cv_update_start;
extern CAL ts_time_t  cgs_cv_update_period_ms;
extern CAL cell_v_t   cgs_cv_up_cell_v_lim;
extern CAL cell_v_t   cgs_cv_v_reduce;
/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
void cgs_ccc_reset( void );
void cgs_cc_cv_update(current_t *isetpoint_p, pack_v_t  *vsetpoint_p );


#endif      /* CGS_CCC_H */

