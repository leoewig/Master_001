/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:      exc_canbedded_callbacks.c 
 *
 * description:    Callback functions called by Vector code
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li          Callback functions called by Vector code
 *
 *****************************************************************************/

#include "basic_types.h"
#include "hardware.h"
#include "dio_cfg.h"
#include "dio_veh_can_disable.h"
#include "exc.h"


/**
 * Called when CCL wants to transition to standby.
 *
 * @param sleepResult   one of kCanFailed, kCanOK or kCanNotSupported
 * @retval kCclNoRepeatCanSleep (no need to try again: we don't support
 *                              standby mode)
 */
vuint8 CCL_API_CALLBACK_TYPE ApplCclCanStandby(CCL_CHANNEL_CCLTYPE_FIRST vuint8 sleepResult)
{
    /** @lint channel argument (in macro #CCL_CHANNEL_CCLTYPE_FIRST) not
     * accessed */
    /*lint --e{715} */

    /*
     * if we've changed the logic to support a sleep mode,
     * we should reconsider this logic so assert that sleep is not supported
     */
    ASSERT(sleepResult == kCanNotSupported);

    return (vuint8) kCclNoRepeatCanSleep;
}

/**
 * Called before communication is started.  See CCL Technical reference.
 *
 * We do nothing.
 */
void CCL_API_CALLBACK_TYPE ApplCclComStart(CanChannelHandle channel)
{
    /** @lint channel argument not accessed */
    /*lint --e{715} */

    /* do nothing */
    return;
}

/**
 * Called after communication is stopped.  See CCL Technical reference.
 *
 * We do nothing.
 */
void CCL_API_CALLBACK_TYPE ApplCclComStop(CanChannelHandle channel)
{
    /** @lint channel argument not accessed */
    /*lint --e{715} */

    /* do nothing */
    return;
}

/**
 * Called after communication is suspended.  See CCL Technical reference.
 *
 * We do nothing.
 */
void CCL_API_CALLBACK_TYPE ApplCclComWait(CanChannelHandle channel)
{
    /** @lint channel argument not accessed */
    /*lint --e{715} */

    /* do nothing */
    return;
}

/**
 * Called before communication is resumed.  See CCL Technical reference.
 *
 * We do nothing.
 */
void CCL_API_CALLBACK_TYPE ApplCclComResume(CanChannelHandle channel)
{
    /** @lint channel argument not accessed */
    /*lint --e{715} */

    /* do nothing */
    return;
}

/**
 * Called at start of bus off recovery phase.  See CCL Technical reference.
 *
 * We do nothing.
 */
void CCL_API_CALLBACK_TYPE ApplCclBusOffStart(CanChannelHandle channel)
{
    /** @lint channel argument not accessed */
    /*lint --e{715} */

    /* do nothing */
    return;
}

/**
 * Called at end of bus off recovery phase.  See CCL Technical reference.
 *
 * We do nothing.
 */
void CCL_API_CALLBACK_TYPE ApplCclBusOffEnd(CanChannelHandle channel)
{
    /** @lint channel argument not accessed */
    /*lint --e{715} */

    /* do nothing */
    return;
}

/**
 * Called during CclInit() to re-initialise ISO TP and diagnostics (because
 * CCL doesn't know which CAN channels need re-initialising)
 */
void CCL_API_CALLBACK_TYPE ApplCclInit(CanChannelHandle channel)
{
    /** @lint channel argument not accessed */
    /*lint --e{715} */

    /** @todo re-initialise ISO TP and Diagnostics */
    return;
}

/**
 * Called to initialise transciever.
 *
 * We do nothing.
 */
void CCL_API_CALLBACK_TYPE ApplCclInitTrcv(CanChannelHandle channel)
{
    /** @lint channel argument not accessed */
    /*lint --e{715} */

    /* do nothing */
    return;
}

/**
 * Called to wake-up transceiver.
 *
 * We do nothing for ISA bus but switch off the transceiver for
 * the vehicle and charger buses.
 */
void CCL_API_CALLBACK_TYPE ApplCclWakeUpTrcv(CanChannelHandle channel)
{
    /* TODO: Doesn't do anything with charger bus?? */
#ifdef MMC
    uint8_t ccr;
    if (channel == (CanChannelHandle) CCL_vehicle_bus)
    {
        SAVE_INTERRUPTS(ccr);
        ENABLE_VEHICLE_CAN_TRANSCEIVER;
        RESTORE_INTERRUPTS(ccr);
    }
#else
    /** @lint channel argument not accessed */
    /*lint --e{715} */

    /* do nothing */
#endif
    return;
}

/**
 * Called to put transceiver into sleep
 *
 * We do nothing.
 */
void CCL_API_CALLBACK_TYPE ApplCclSleepTrcv(CanChannelHandle channel)
{
    /** @lint channel argument not accessed */
    /*lint --e{715} */

    /* do nothing */
    return;
}

/**
 * Called to put transceiver into standby
 *
 * We do nothing for charger or ISA buses but switch off the transceiver for
 * the vehicle bus.
 */
void CCL_API_CALLBACK_TYPE ApplCclStandbyTrcv(CanChannelHandle channel)
{
#ifdef MMC
    uint8_t ccr;
    if (channel == (CanChannelHandle) CCL_vehicle_bus)
    {
        SAVE_INTERRUPTS(ccr);
        DISABLE_VEHICLE_CAN_TRANSCEIVER;
        RESTORE_INTERRUPTS(ccr);
    }
#else
    /** @lint channel argument not accessed */
    /*lint --e{715} */

    /* do nothing */
#endif
    return;
}

/**
 * called to report bus off event.
 *
 * Recovery will be handled by the CCL.  We do nothing.
 */
C_CALLBACK_1 void C_CALLBACK_2 ApplCanBusOff(CAN_CHANNEL_CANTYPE_ONLY)
{
    /** @lint channel argument not accessed */
    /*lint --e{715} */

    return;
}

/**
 * Fatal error occurred in CANbedded
 */
void CCL_API_CALLBACK_TYPE ApplCclFatalError(void)
{
    /**
     * @lint 529 - symbols not referenced
     * We take copies of the error information to help with debugging
     * so switch off the symbols not referenced warning
     */
    /*lint --e{529} symbols not subsequently referenced */
    uint16_t error_code = cclErrorCode;
    uint16_t error_line = cclErrorLine;
    uint16_t component_id = cclComponentID;
    uint8_t channel_number = cclChannelNumber;

    /** @todo log the fault */
    INTERNAL_ERROR();
    return;
}

#if defined PRODDER || defined PLATFORM_TEST
/* Prodder doesn't use OSEK network management so need this callback */
void ApplCclNmStateChange(CCL_CHANNEL_CCLTYPE_FIRST vuint8 activeState, vuint8 targetState)
{
    /* do nothing */
    return;
}
#endif

