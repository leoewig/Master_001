/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    mmc_danomem.c
 *
 * description:  
 *  Dan-O-Mem communication parser for mmc, mmc test harness and prodder targets
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/


/******************************************************************************
 * INCLUDES
 *****************************************************************************/

/**
 * @lint use of 'undef' is discouraged\n
 */
/*lint -e{960}*/
#ifdef UNIT_TEST
#undef UNIT_TEST
#endif  /* UNIT_TEST */

#include "basic_types.h"
#include "hardware.h"
#include "mmc_danomem.h"
#include "mmc_danomem_p.h"
#include "exc.h"

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/**
 * Function to send ID frame from a S.U.T build
 */

#if ! defined PRODDER       /* This Frame Is ONLY In A S.U.T ! */

/** see DoxyGen for ConvertPackedMonthName() in danomem.cpp */

void SendSutIdFrame(uint8_t p_U8_build_id, const char_t* p_pS_date_string,
    const char_t* p_pS_time_string, uint8_t p_U8_reset_reason)
{
    CAN_TX_SIG(ID_year, ascii2bin(p_pS_date_string[9], p_pS_date_string[10]));
    CAN_TX_SIG(ID_month, ascii2bin(p_pS_date_string[1], p_pS_date_string[2]));
    CAN_TX_SIG(ID_date, ascii2bin(p_pS_date_string[4], p_pS_date_string[5]));
    CAN_TX_SIG(ID_hour, ascii2bin(p_pS_time_string[0], p_pS_time_string[1]));
    CAN_TX_SIG(ID_minute, ascii2bin(p_pS_time_string[3], p_pS_time_string[4]));
    CAN_TX_SIG(ID_second, ascii2bin(p_pS_time_string[6], p_pS_time_string[7]));
    CAN_TX_SIG(ID_reason, p_U8_reset_reason);
    CAN_TX_SIG(ID_build, p_U8_build_id);
}
#endif

/**
 * Dan-O-Mem request parser using CANbedded
 */
void danomem(void)
{
    /**
     * @lint Investigate : Signed versus Unsigned\n
     * lint --e{960}
     */
    /*lint --e{960} */
    if (CAN_RX_SIG_INDICATION(WRREQ_TYPE))
    {
        CAN_RX_SIG_INDICATION_CLR(WRREQ_TYPE);
        /* write request received */
        danomem_write_request();
    }

    if (CAN_RX_SIG_INDICATION(RDREQ_TYPE))
    {
        CAN_RX_SIG_INDICATION_CLR(RDREQ_TYPE);
        /* read request received */
        danomem_read_request();
    }
}

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/**
 * If the response is marked as 'pending' a slower task will be processing the
 * request, rather than force other features to remember this information, it
 * is kept here. If there is a possibility of the task completion interrupting
 * the routine that services danomem, this implementation should change to pass
 * these variables to the task called */

PRIVATE bool_t          dm_pending=FALSE;
PRIVATE uint8_t         dm_pend_req=0u;
PRIVATE uint8_t         dm_pend_sent=0u;
PRIVATE volatile FPTR void       *dm_pend_addr_p;
PRIVATE uint32_t        dm_pend_data;
PRIVATE uint8_t         dm_pend_type;

DEBUGVAR(dm_pend_sent)
DEBUGVAR(dm_pend_req)

/**
 * Dan-O-Mem write request parser using CANbedded
 */
PRIVATE void danomem_write_request(void)
{
    volatile FPTR void *addr_p;
    uint32_t data;
    danomem_type_t type;
    uint8_t res;
    danomem_res_t write_res;


    /* Parse incoming CAN messages */

    type = (danomem_type_t) CAN_RX_SIG(WRREQ_TYPE);

    /**
     * @lint Investigate : cast from unsigned long to pointer\n
     * lint -e{923}
     */
    /*lint -e{923} */
    addr_p = (volatile FPTR void *) CAN_RX_SIG(WRREQ_ADDRESS);
    data = CAN_RX_SIG(WRREQ_DATA);
    /* perform write and read it back */
    res = (uint8_t) type;

    write_res=danomem_write_data(type, (FPTR void *) addr_p, data);
#ifdef PLATFORM_TEST
    /* Unit tests can't handle pending messages, so translate the pending
     * response to OK before continuing. UNIT_TEST is undef'd above though
     * so we look at PLATFORM_TEST instead */
    if ( DM_res_pend==write_res)
    {
        write_res = DM_res_ok;
    }
#endif /* PLATFORM_TEST */

    if (DM_res_pend==write_res)
    {
        /* remember data for pending response, if there is a possibility
         * that the 'pending' response might be sent BEFORE
         * danomem_write_data() returns, then this data needs to be stored
         * before calling the function, rather than afterwards */
        dm_pend_req++;
        dm_pend_addr_p = addr_p;
        dm_pend_data = data;
        dm_pend_type = res;
        dm_pending=TRUE;
    }
    else if (write_res == DM_res_err)
    {
        res = (uint8_t) DM_error_wrong_type;
    }
    else
    {
        /* res is the type, as above */
    }
  
    if (danomem_read_data(type, (FPTR void *) addr_p, &data) == DM_res_err)
    {
        res = (uint8_t) DM_error_wrong_type;
    }
    
    if (DM_res_pend!=write_res)
    {
        CAN_TX_SIG(WRRSP_TYPE, res);
        /**
         * @lint Investigate : cast from pointer to unsigned long\n
         * lint -e{923}
         */
        /*lint -e{923} */
        CAN_TX_SIG(WRRSP_ADDRESS, (uint32_t) addr_p);
        /* message sent on write of data */
        CAN_TX_SIG(WRRSP_DATA, data);
    }
}

/**
 * Dan-O-Mem read request parser using CANbedded
 */
PRIVATE void danomem_read_request(void)
{
    volatile FPTR void *addr_p;
    uint32_t data = 0u;
    danomem_type_t type;

    /* Parse incoming CAN messages */

    type = (danomem_type_t) CAN_RX_SIG(RDREQ_TYPE);

    /**
     * @lint Investigate :  cast from unsigned long to pointer\n
     * lint -e{923}
     */
    /*lint -e{923} */
    addr_p = (volatile FPTR void *) CAN_RX_SIG(RDREQ_ADDRESS);
    /* read data */
    if (danomem_read_data(type, addr_p, &data) == DM_res_ok)
    {
        CAN_TX_SIG(RDRSP_TYPE, (uint8_t) type);
    }
    else
    {
        CAN_TX_SIG(RDRSP_TYPE, (uint8_t) DM_error_wrong_type);
    }
    /**
     * @lint Investigate : cast from pointer to unsigned long\n
     * -e{923}
     */
    /*lint -e{923} */
    CAN_TX_SIG(RDRSP_ADDRESS, (uint32_t) addr_p);
    /* message sent on write of data */
    CAN_TX_SIG(RDRSP_DATA, data);
}

/**
 * Request transmission of the data in the pending_xxx variables
 * The CAL feature calls this function on response to any operation,
 * including ones that don't get fed through it, so danomem needs to remember
 * whether it has a pending response or not. */
void danomem_send_pending(void)
{
    uint8_t ccr;
    /* This can be called from lower priority tasks, so protect against
     * interrupts */
    SAVE_INTERRUPTS(ccr);

    if (dm_pending)
    {
        dm_pend_sent++;
        CAN_TX_SIG(WRRSP_TYPE, dm_pend_type);
        /**
         * @lint Investigate : cast from pointer to unsigned long\n
         * lint -e{923}
         */
        /*lint -e{923} */
        CAN_TX_SIG(WRRSP_ADDRESS, (uint32_t) dm_pend_addr_p);
        CAN_TX_SIG(WRRSP_DATA, dm_pend_data);
        dm_pending=FALSE;
    }
    RESTORE_INTERRUPTS(ccr);
}
