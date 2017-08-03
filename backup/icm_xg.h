/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   icm_xg.h
 *
 * description: 
 * Header file for the XGATE portion of the internal comms.
 * This includes decoding and processing the incoming data.
 * 
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
#if !defined(ICM_XG_H)
#define      ICM_XG_H
/**
 *
 * Header file for the XGATE portion of the internal comms.
 * This includes decoding and processing the incoming data.
 *
 * @file
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

/**
 *
 * <h2> Internal Comms XGATE functionality </h2>
 *
 * <h2> Approach used </h2>
 * An overview of the design approach.
 *
 * <h2> Something </h2>
 * Details of the design should go here, with headings as required.
 *
 * <h2> Limitations </h2>
 * This header is shared between the XGATE and S12, all variables in it must be
 * pragma aligned
 *
 * <h2> How to use </h2>
 * A description of exactly what the application and other features must do to
 * use this feature.
 *
 * <h2> Configuration </h2>
 * If the feature has configuration items then point to where they are.
 *
 * <h2> Requirement Satisfaction </h2>
 * A link to the requirements document, and optionally a list of where in the
 * design the requirements are satisfied.
 *
 * @defgroup ICM_XG Internal Comms XGATE functionality
 * @ingroup DEF icm
 *
 */

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

enum XgMeasure_e
{
    /** Not doing anything interesting */
    XgMeasureIdle,

    /** The XG as seen a transition from
    * !IcmActiveMeasure => IcmActiveMeasure
    * Once this is set, the XGATE owns the measurement buffer.*/
    XgMeasureAcq,
    /**
     * The XG has seen a full set of data and has finished all its post
     * processing. Once this is set the XG has released the measurement
     * buffer.
     * A transition away from and back to this value indicates a new
     * measurement  has been completed. */
    XgMeasureComplete,

    /**
     * The XGATE is expecting reference measurements
     */
    XgMeasureRefs,

    /**
     * The XGATE has received a complete set of reference measurements */
    XgRefsComplete,

    /**
     * The XGate has given up on filling the data buffer, and will no longer
     * write into it. */
    XgMeasureError
};

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

/* These are shared with the S12 and so must be aligned */
#ifdef __CSMC__
#pragma align on
#endif

/****************************************************************************
 * Configuration (CAL and derived)
 ***************************************************************************/

extern FRAM cellcount_t xg_gcell_offset[MAX_NUM_VTBM];

/****************************************************************************
 * Status
 ***************************************************************************/

extern FRAM volatile enum XgMeasure_e   xg_measure_state;
extern FRAM volatile bool_t             xg_receiving_ref;                
extern FRAM uint8_t                     icm_measure_rxd_ctr;
/****************************************************************************
 * Whole pack information
 ***************************************************************************/

extern FRAM volatile cellcount_t     xg_cellframe_rx_count;
extern FRAM volatile cellcount_t     xg_cellframe_expect_count;
extern FRAM volatile uint16_t    xg_invalid_cell_ctr;
extern FRAM cellcount_t         xg_expected_cells_count;

extern volatile FRAM uint16_t  xg_invalid_cell_ctr;
extern volatile FRAM uint16_t  xg_invalid_cellpair_ctr;
extern volatile FRAM uint16_t  xg_unexpect_cellsig_ctr;
extern volatile FRAM uint16_t  xg_invalid_vtbm_cellmsg_ctr;
extern volatile FRAM uint16_t       icm_invalid_ref_vtbm_ctr;
extern volatile FRAM uint16_t       icm_unexpect_ref_ctr;
extern volatile FRAM uint16_t       icm_unexpect_cellframes;

/****************************************************************************
 * Per-VTBM information
 ***************************************************************************/

extern FRAM volatile uint8_t xg_vtbm_rx_cellframe_mask[MAX_NUM_VTBM];
extern FRAM volatile uint8_t xg_vtbm_rx_cellframe_count[MAX_NUM_VTBM];
extern FRAM uint8_t          xg_vtbm_expected_cellframes[MAX_NUM_VTBM];

extern FRAM uint8_t     xg_vtbm_n_cell_v_meas[MAX_NUM_VTBM];
extern FRAM uint8_t     xg_vtbm_n_cell_t_meas[MAX_NUM_VTBM];

/*****************************************************************************
 * Received reference data
 ****************************************************************************/

extern FRAM bool_t              xg_refs_received[MAX_NUM_VTBM];
extern FRAM cell_v_t            xg_ref_active_val[MAX_NUM_VTBM];
extern FRAM enum CellVstat_e    xg_ref_active_stat[MAX_NUM_VTBM];
extern FRAM cell_v_t            xg_ref_passive_val[MAX_NUM_VTBM];
extern FRAM enum CellVstat_e    xg_ref_passive_stat[MAX_NUM_VTBM];
extern FRAM uint8_t             xg_refs_diag[MAX_NUM_VTBM];
extern FRAM cell_v_t            xg_ref_start_val[MAX_NUM_VTBM];
extern FRAM enum CellVstat_e    xg_ref_start_stat[MAX_NUM_VTBM];
extern FRAM volatile uint8_t    xg_ref_rx_count;

#ifdef __CSMC__
#pragma  align off
#endif

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

void xg_rx_celldata(const struct ican_msg_s *msgp);

void xg_rx_refdata(const struct ican_msg_s *msgp);

void xg_measure_timeout(void);

void icm_proc_prev_meas(void);

#endif      /* ICM_XG_H */
