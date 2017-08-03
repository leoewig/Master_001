/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:     vprg_p.h
 *
 * description:   VTBM re-programming private header
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           VTBM re-programming private header
 *                       
 *
 *****************************************************************************/
#if !defined(VPRG_P_H)
#define      VPRG_P_H


/******************************************************************************
 * INCLUDES
 *****************************************************************************/
#include "ts_types.h"   /* for ts_time_t */

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/


/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/*
 * public functions interfacing the vprg modules
 */

/* vprg_image.c */
void vprg_check_image_idle_task(void);
void vprg_check_image_request(image_index_t image_index);
vprg_ci_state_e vprg_check_image_status(void);
FPTR image_header_s *vprg_get_image_header(image_index_t image);
FPTR image_sector_header_s *vprg_first_sector( 
                            FPTR const image_header_s *image_header_p);
FPTR image_sector_header_s *vprg_next_sector( 
                            FPTR const image_sector_header_s *image_sector_p);

/* vprg_comms.c */
void vprg_comms_idle_task(void);
void vprg_comms_set_demand(image_index_t image_index,
                                vprg_vtbm_mask_t vtbm_mask);
vprg_state_e vprg_comms_request(void);
bool_t vprg_comms_all_vtbms_programmed(void);
vprg_vtbm_mask_t vprg_vtbm_id_to_mask(uint8_t i);
vprg_comms_state_e vprg_comms_status(void);


/*
 * vprg_state.c private functions
 */
PRIVATE void vprg_update_demand(vprg_demand_e *demand_p,image_index_t *image_p,
                                        vprg_vtbm_mask_t *vtbm_mask_p);
PRIVATE void vprg_update_cal_demand(vprg_demand_e *demand_p,
        image_index_t *image_p,vprg_vtbm_mask_t *vtbm_mask_p);
PRIVATE void vprg_update_xcp_usercmd_demand(vprg_demand_e *demand_p,
        image_index_t *image_p,vprg_vtbm_mask_t *vtbm_mask_p);
PRIVATE void vprg_update_uds_demand(vprg_demand_e *demand_p,
        image_index_t *image_p,vprg_vtbm_mask_t *vtbm_mask_p);
PRIVATE void vprg_resolve_demand(
                            vprg_demand_e cal_demand, image_index_t cal_image,
                                vprg_vtbm_mask_t cal_vtbm_mask,
                            vprg_demand_e xcp_demand, image_index_t xcp_image,
                                vprg_vtbm_mask_t xcp_vtbm_mask,
                            vprg_demand_e uds_demand, image_index_t uds_image,
                                vprg_vtbm_mask_t uds_vtbm_mask,
                            vprg_demand_e *demand_p, image_index_t *image_p,
                                vprg_vtbm_mask_t *vtbm_mask_p);
PRIVATE vprg_demand_e vprg_validate_demand(vprg_demand_e unvalidated_demand);
PRIVATE vprg_state_e vprg_state_machine(vprg_state_e current_state,
                            vprg_demand_e demand,image_index_t image,
                                vprg_vtbm_mask_t vtbm_mask);
PRIVATE vprg_state_e vprg_idle_enter(void);
PRIVATE vprg_state_e vprg_idle_during(vprg_demand_e demand,image_index_t image);
PRIVATE vprg_state_e vprg_checking_image_enter(image_index_t image_index);
PRIVATE vprg_state_e vprg_checking_image_during(void);
PRIVATE vprg_state_e vprg_ready_to_program_enter(void);
PRIVATE vprg_state_e vprg_ready_to_program_during(vprg_demand_e demand,
                            image_index_t image, vprg_vtbm_mask_t vtbm_mask);
PRIVATE vprg_state_e vprg_check_demand_and_start_programming(
                    image_index_t image_index, vprg_vtbm_mask_t vtbm_mask);
PRIVATE vprg_state_e vprg_shutdown_icm_enter(void);
PRIVATE vprg_state_e vprg_shutdown_icm_during(void);
PRIVATE vprg_state_e vprg_cycle_vtbms_enter(void);
PRIVATE vprg_state_e vprg_cycle_vtbms_during(void);
PRIVATE vprg_state_e vprg_communicating_enter(void);
PRIVATE vprg_state_e vprg_communicating_during(void);
PRIVATE vprg_state_e vprg_programming_complete_enter(void);
PRIVATE vprg_state_e vprg_programming_complete_during(vprg_demand_e demand);
PRIVATE vprg_state_e vprg_error_during(vprg_demand_e demand);

PRIVATE vprg_state_e vprg_error_enter(vprg_error_e error);
PRIVATE void vprg_report_fault(vprg_error_e error);
PRIVATE void vprg_send_dto(vprg_state_e new_state);

/*
 * vprg_image.c private functions
 */
PRIVATE vprg_ci_state_e vprg_check_folder(image_index_t image);
PRIVATE bool_t vprg_check_folder_header(void);
PRIVATE bool_t vprg_check_folder_header_string(void);
PRIVATE bool_t vprg_check_folder_header_contents(void);
PRIVATE bool_t vprg_check_folder_header_crc(void);
PRIVATE bool_t vprg_check_image_index(image_index_t image);
PRIVATE bool_t vprg_check_image(image_index_t image);
PRIVATE uint32_t vprg_get_image_offset(image_index_t image);
PRIVATE bool_t vprg_check_image_header(
                                    FPTR const image_header_s *image_header_p);
PRIVATE bool_t vprg_check_image_sectors(
                                    FPTR const image_header_s *image_header_p);
PRIVATE bool_t vprg_check_image_header_string(
                                    FPTR const image_header_s *image_header_p);
PRIVATE bool_t vprg_check_image_crc(FPTR const image_header_s *image_header_p);
PRIVATE bool_t vprg_check_image_sectors(
                                FPTR const image_header_s *image_header_p);
PRIVATE bool_t vprg_check_sector(
                        FPTR const image_sector_header_s *image_sector_p);
PRIVATE bool_t vprg_check_sector_length(
                        FPTR const image_sector_header_s *image_sector_p);
PRIVATE uint16_t vprg_sector_length(
                        FPTR const image_sector_header_s *image_sector_p);
PRIVATE bool_t vprg_check_sector_header_string(
                            FPTR const image_sector_header_s *image_sector_p);
PRIVATE bool_t vprg_check_sector_crc(
                            FPTR const image_sector_header_s *image_sector_p);
/*
 * vprg_comms.c private functions
 */
PRIVATE vprg_comms_state_e vprg_comms_connect(void);
PRIVATE vprg_comms_state_e vprg_comms_program_start(void);
PRIVATE vprg_comms_state_e vprg_comms_erase(void);
PRIVATE vprg_comms_state_e vprg_comms_program(void);
PRIVATE vprg_comms_state_e vprg_comms_program_reset(void);
PRIVATE bool_t vprg_calc_next_vtbm_id(void);
PRIVATE vprg_comms_error_e vprg_send_connect(void);
PRIVATE vprg_comms_error_e vprg_send_program_start(void);
PRIVATE vprg_comms_error_e vprg_cro_tx_dto_rx(FPTR const uint8_t *tx_data,
                                ts_time_t timeout_ms);
PRIVATE vprg_comms_error_e vprg_cro_tx_dto_rx_with_retries(
                                FPTR const uint8_t *tx_data,
                                ts_time_t timeout_ms, uint8_t max_attempts);
PRIVATE bool_t vprg_poll_for_dto(void);
PRIVATE bool_t vprg_erase_sector(FPTR const image_sector_header_s *sector_p);
PRIVATE bool_t vprg_program_and_check_sector(
                                FPTR const image_sector_header_s *sector_p);
PRIVATE vprg_comms_error_e vprg_program_sector(
                                FPTR const image_sector_header_s *sector_p);
PRIVATE bool_t vprg_check_sector_programming(
                                FPTR const image_sector_header_s *sector_p);

PRIVATE vprg_comms_error_e vprg_send_erase(uint32_t address, uint32_t length);
PRIVATE vprg_comms_error_e vprg_send_set_mta(uint32_t address);
PRIVATE vprg_comms_error_e vprg_send_program(uint32_t address,
                                    FPTR const uint8_t *data, uint8_t nbytes);
PRIVATE vprg_comms_error_e vprg_send_program_max(uint32_t address,
                                    FPTR const uint8_t *data);
PRIVATE vprg_comms_error_e vprg_send_build_checksum(uint32_t address,
                                    uint32_t length,uint16_t *received_crc_p);
PRIVATE void vprg_send_program_reset(void);

/******************************************************************************
 * DATA DECLARATIONS
 *****************************************************************************/

extern image_index_t vprg_last_checked_image;
extern vprg_image_error_e vprg_image_error;
extern vprg_comms_error_e vprg_comms_error;

#ifdef UNIT_TEST

/*
 * vprg_state.c
 */
extern CAL vprg_demand_e vprg_demand_cal;
extern CAL image_index_t vprg_image_cal;
extern vprg_demand_e vprg_demand;
extern vprg_demand_e vprg_demand_cal_last;
extern vprg_state_e vprg_state;
extern vprg_error_e vprg_latest_error;
extern bool_t vprg_via_cal_active;

/*
 * vprg_image.c
 */
extern image_index_t vprg_image_to_check;
extern FROM image_folder_header_s image_folder;
extern vprg_ci_state_e vprg_ci_state;

/*
 * vprg_comms.c
 */
extern uint16_t vprg_dto_id;
extern uint8_t vprg_dto_data[];
extern uint8_t vprg_connect_attempts;

#endif /* UNIT_TEST */
  
#endif      /* VPRG_P_H */

