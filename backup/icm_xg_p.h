/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   icm_xg_p.h
 *
 * description: 
 * XGATE Internal Comms private header
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
#if !defined(ICM_XGATE_P_H)
#define      ICM_XGATE_P_H
/**
 *
 *  XGATE Internal Comms private header
 *
 *
 * @file
 * @ingroup XGATE
 * $Id: icm_xg_p.h 10359 2013-09-03 09:39:35Z pr4 $
 */

/* Copyright (c) Linkedriving Tech 2012 - All rights reserved */

#define CELLS_IN_PAIR  2u

#define ICM_PAIR_RXD_MASK(pair) (uint8_t)(1u<<(pair))

/**
 * Turn a pair number into a lower (local) cell number
 * @param[in] pair_num a VTBM measurement pair number, range 0 to 7 inclusive
 * @returns number in the range 0 to 14 for the first cell of the pair
 */
#define PAIR_TO_CELL(pair_num) ((uint8_t) ((pair_num)*CELLS_IN_PAIR))

#define ICM_LOCAL_TO_GLOBAL( vtbm_num_, local_cell_num_ ) \
    ( xg_gcell_offset[ (vtbm_num_) ] + (local_cell_num_) )

/**
 * Return true or false depending on whether pair_num is valid for vtbm_num
 * @param[in] pair_num a cell pair number 0 to 7
 * @param[in] vtbm_num a VTBM number 0 to MAX_NUM_VTBM-1
 */
#define VALID_CELL_PAIR(pair_num, vtbm_num) \
    ( (pair_num) < xg_vtbm_expected_cellframes[(vtbm_num)] )

/**
 * Variable for unit testing
 */

#if defined(UNIT_TEST)
extern uint16_t xg_cellframe_dup_total;
#endif /* defined UINT_TEST */

/*
 * PRIVATE function prototypes (documented in source)
 */

PRIVATE void    xg_process_cell_data(
                    const struct icm_cell_pair_data_s *datap);

PRIVATE void    xg_process_refs(const struct icm_ref_data_s *datap);

PRIVATE void     xg_unpack_celldata(const struct ican_msg_s *msgp,
                    struct icm_cell_pair_data_s *cellp);

/* Unpack reference data */
PRIVATE void    xg_unpack_refdata(const struct ican_msg_s *msgp,
                        struct icm_ref_data_s *refp);

PRIVATE bool_t xg_record_pair_received( uint8_t vtbm_num, uint8_t pair_num);


/* see body for description */
PRIVATE void    xg_decode_volt(const uint8_t* buffp, cell_v_t *val,
                        enum CellVstat_e *statusp);

/* see body for description */
PRIVATE void    xg_decode_temp(const uint8_t* buffp, uint8_t rshift,
                        temperature_t *val, enum CellTstat_e *statusp);

PRIVATE void    xg_store_cellframe( const struct icm_cell_pair_data_s * datap);

PRIVATE void    xg_process_measurement(void);

PRIVATE void xg_set_volt_minmax(FPTR  struct xg_s12_data_s * measp );

PRIVATE void xg_set_temp_minmax(FPTR  struct xg_s12_data_s * measp );

PRIVATE void    xg_default_t_hilo(FPTR  struct xg_s12_data_s * measp );

PRIVATE pack_v_t xg_calc_cell_sum(
                        cellcount_t count,
                        bool_t allow_old_values) ;

PRIVATE void    xg_store_celldata( const struct icm_cell_pair_data_s * datap,
                        FPTR struct xg_s12_data_s * outp ) ;

PRIVATE void xg_store_cellvolt( uint8_t vtbm_num,
    cellcount_t cellnum, cell_v_t volts , enum CellVstat_e vstat);

PRIVATE void xg_store_celltemp(uint8_t vtbm_num, cellcount_t cellnum,
    temperature_t temp , enum CellTstat_e tstat,temperature_t cell_offset);

PRIVATE int16_t     xg_sign_extend16(uint16_t val, uint8_t nbits);


PRIVATE enum CellTstat_e xg_status_from_temp( uint16_t tmp);

PRIVATE enum CellVstat_e xg_status_from_volt( uint16_t raw_volt);

PRIVATE bool_t xg_is_expected_ref(uint8_t  vtbm_num);

PRIVATE void xg_decode_refdig(uint8_t byteval,
    struct icm_ref_data_s *refp);

PRIVATE void xg_detect_t_fault(FPTR struct xg_s12_data_s *measp);
PRIVATE void xg_detect_v_fault(FPTR struct xg_s12_data_s *measp);

void xg_set_deltas(void);
void xg_set_validity(void);

PRIVATE data_validity_e xg_determine_validity(cellcount_t expected,
    cellcount_t received, data_validity_e previous);

PRIVATE int16_t add_s16s16(int16_t lhs,int16_t rhs);
#endif      /* ICM_XGATE_P_H */

