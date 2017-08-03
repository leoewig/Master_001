/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    XCP definitions
 *
 * description: XCP definitions
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
#ifndef IXCP_H
#define IXCP_H
/**
 * @file
 * @ingroup ixcp
 *
 * XCP definitions
 */
/**
 * @defgroup ixcp IXCP Internal XCP
 *
 * XCP master and slave on the internal CAN bus
 */
#include "ixcp_types.h"
#include "flash_eee_types.h"

#if defined BBLK
#include "ican.h"      /* for enum can_handle_e and CAN_MAX_DATA */
#endif

#if defined VTBM || defined BBLK_VTBM

/* VTBM needs board ID-specific initialisation */
void ixcp_init_vtbm(void);
extern uint16_t ixcp_cro_id;       /* VTBM has an ID-specific CRO ID */

#elif defined BBLK_MMC

void ixcp_init_bblk_mmc(enum can_handle_e bus);
extern uint16_t ixcp_cro_id;       /* MMC bootblock has a bus-specific CRO ID */

#endif

#define CAN_MAX_DATA        8u  /**< max length of a CAN message */

void ixcp_cro(const struct ican_msg_s *msgp);
void ixcp_send_dto(ixcp_dto_type_e dto_id,const uint8_t *dto_payload);
ixcp_dto_type_e ixcp_set_error(ixcp_error_code_e error_code,
                                uint8_t *dto_payload);
void ixcp_idle_tasks(void);

void ixcp_resume_connection(void);

#ifdef MMC
bool_t ixcp_reset_conditions_ok(void);
#endif

bool_t ixcp_prog_allowed(flash_eee_reprogramming_e reprogramming_option);
#endif /* ifndef IXCP_H */
