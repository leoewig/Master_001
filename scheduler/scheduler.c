/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name: scheduler.c
 *
 * description:   
 * Implementation for the scheduler
 *
 * 
 *
 * 
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li           initial version
 *
 *****************************************************************************/


/*************************************************************************//**
 *
 * Implementation for the scheduler
 *
 *
 * @file
 * @ingroup Scheduler
 *
 *****************************************************************************
 *
 * Task Scheduler
 *
 *****************************************************************************/

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "scheduler.h"     /* public header for this module */
#include "hardware.h"      /* interrupt helpers */

/******************************************************************************
 * PRIVATE MACRO DEFINITIONS
 *****************************************************************************/

/**************************************************************************//**
 * Macro which is used to abstract the initiation of a context switch
 *****************************************************************************/
#define SCHEDULER_INITIATE_CONTEXT_SWITCH() do { (void)ASM("SYS"); } while( 0 )
/**************************************************************************//**
 * Determine whether or not the scheduler is enabled (i.e. not locked).
 *****************************************************************************/
#define IS_SCHEDULER_ENABLED() (scheduler_lock_depth == 0U)

 /*
  * GPAGE model logic: the code must know the GPAGE model used by the compiler
  * (i.e. whether compiled with +modc, +modd or +modg) - unless it's a hosted
  * test build
  */
#ifdef HOSTED
#define GPAGE_MODG      /* doesn't matter which option we choose */
#else
#if (defined GPAGE_MODC + defined GPAGE_MODD + defined GPAGE_MODG) != 1
#error precisely one of GPAGE_MODC, GPAGE_MODD and GPAGE_MODG must be defined
#endif
#endif
/**************************************************************************//**
 *
 *  Function to determine the highest priority task which is ready to execute.
 *
 *****************************************************************************/
PRIVATE task_priority_level_t
scheduler_get_waiting_task( void );

/**************************************************************************//**
 *
 *  Function to clear an event associated with a task
 *
 *  @param[in]      p_level - The priority level of the task from which the
 *                  event should be cleared
 *
 *  @par Description:
 *                  This function either clears the event currently associated
 *                  with a task (if there is one), or removes one event from
 *                  the event queue if queueing is enabled.
 *****************************************************************************/
PRIVATE void
scheduler_task_unset_event( const task_priority_level_t p_level );

/**************************************************************************//**
 *
 *  Function which wraps a task with the neccessary scheduler administration
 *
 *  @param[in]      p_level - The priority level of the task to be executed
 *
 *  @par Description:
 *                  This function calls the task function then catches the
 *                  return ('task exit'), cleans up, then initiates a context
 *                  switch to invoke the next waiting task
 *
 *                  This function MUST be in fixed page memory as it's entered
 *                  on return from the scheduler interrupt which does not set
 *                  PPAGE based on the location of this function
 *****************************************************************************/
PRIVATE NCODE void
scheduler_task_wrapper( task_priority_level_t p_level );

/******************************************************************************
 * PRIVATE TYPE DEFINITIONS
 *****************************************************************************/

/**************************************************************************//**
 * Type used for looping through a task list.  Deliberately of a larger type
 *  than task_count_t
 *****************************************************************************/
typedef uint16_t task_counter_t;

/**************************************************************************//**
 * Type used for counting the number of events associated with a task
 *****************************************************************************/
typedef uint8_t event_count_t;

/**************************************************************************//**
 * Type used to indicate the operational status of the scheduler
 *****************************************************************************/
typedef enum {
    /** The scheduler is ininitialised and not ready to be started */
    SCHEDULER_STATUS_UNINITIALISED = 0,
    /** The scheduler is initialised and ready to be started */
    SCHEDULER_STATUS_INITIALISED,
    /** The scheduler is initialised and executing tasks */
    SCHEDULER_STATUS_RUNNING,
    /** The scheduler is initialised but in the process of shutting down */
    SCHEDULER_STATUS_SHUTDOWN
} scheduler_status_t;

/**************************************************************************//**
 * Type used to store run-time information relating to a task.
 *****************************************************************************/
struct task_record_s {
    /** The current status of the task.  See task_status_t for more information
     */
    task_status_t  task_status;
    /** The number of events in the task's event queue */
    event_count_t  event_count;
    /** Counter of the number of times this task has been switched to */
    uint32_t       context_switch_count;
#if defined( SCHEDULER_ENABLE_TIMING_STATS )
    /** The CPU time used by the function in terms of timer ticks */
    ts_ticks_t        total_cpu_time_used;
#endif
    /** Flag whether or not the task has had its stack initialised or not */
    bool_t         task_initialised;
};

/******************************************************************************
 * PRIVATE DATA
 *****************************************************************************/

PRIVATE scheduler_status_t    scheduler_status = SCHEDULER_STATUS_UNINITIALISED;
/**<
 * @a2l{LongIdentifier, The overall operating status of the scheduler}
 */

PRIVATE task_priority_level_t           scheduler_current_task_level;
/**<
 * @a2l{LongIdentifier, The priority level of the currently executing task}
 */

#if defined( SCHEDULER_DEBUG )
PRIVATE task_priority_level_t           scheduler_previous_task_level;
#endif

PRIVATE task_count_t                    scheduler_task_count;
/**<
 * @a2l{LongIdentifier, The number of tasks the scheduler is configured for}
 */

/**************************************************************************//**
 * Pointer to the task definitions - the constant section of record required
 * for each task.  See task_control for the dynamic part.
 *****************************************************************************/
PRIVATE const struct task_definition_s* scheduler_task_table;

PRIVATE task_priority_level_t           scheduler_waiting_task_level;
/**<
 * @a2l{LongIdentifier, The priority level of the highest priority waiting task}
 */

PRIVATE uint8_t                         scheduler_lock_depth;
/**<
 * @a2l{conversion, uint8_t}
 * @a2l{LongIdentifier, Depth to which the scheduler is locked.  See scheduler_lock()
and scheduler_unlock() documentation for more details.}
 */

/**************************************************************************//**
 * Stored stack-pointer at the point of the first context switch (i.e. the time
 * the first task was switched to.  Needed for shutting the scheduler down.
 * This is a local variable but is not declared as such as it's used in
 *  inlined assembler code
 *****************************************************************************/
/**
 * @lint Investigate : scheduler_sp_start is used in an _asm statement, this is ignored by Lint
 * hence Lint believes that scheduler_sp_start is not used, Suppressing this
 * then makes Lint believe that it could be made static, suppress that as well
 * not referenced\n
 * lint -esym(714)\n
 * @lint Investigate : external 'scheduler_sp_start' could be made static\n
 * lint -esym(765)
 */
/*lint -esym(714,scheduler_sp_start) */
/*lint -esym(765,scheduler_sp_start) */
      uint16_t                        scheduler_sp_start;

/**************************************************************************//**
 * Dynamic data relating to tasks (as opposed to the static data pointed to by
 * scheduler_task_table
 *****************************************************************************/
PRIVATE struct task_record_s task_control[ SCHEDULER_MAX_TASK_COUNT ];

/** @polyspace:begin<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes */
/**************************************************************************//**
 * Pointer to the out-going task's record during a context switch
 * Used by scheduler_switch_context to avoid using stack.
 *****************************************************************************/
PRIVATE struct task_record_s*     old_task_record;

/**************************************************************************//**
 * Pointer to the incoming task's record during a context switch
 * Used by scheduler_switch_context to avoid using stack.
 *****************************************************************************/
PRIVATE struct task_record_s*     new_task_record;
/** @polyspace:end<MISRA-C:8.7:Not a defect:Justify with annotations> Declared at file scope for unit test purposes */

#if defined( SCHEDULER_ENABLE_TIMING_STATS )
/**************************************************************************//**
 * Snapshot of the current time at a context switch point
 * Used by scheduler_switch_context to avoid using stack.
 *****************************************************************************/
PRIVATE ts_ticks_t                  time_now;
/**************************************************************************//**
 * Time of the last context switch
 *****************************************************************************/
PRIVATE ts_ticks_t                  task_last_switch_time;
#endif

#ifndef HOSTED
#pragma []
#pragma {}
#endif

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/

/* documentation with prototype in header file */
fn_scheduler_init_return_t
scheduler_init( const struct task_definition_s* const p_task_definitions,
                const        task_count_t             p_task_count ) {
    fn_scheduler_init_return_t ret_val = SCHEDULER_INIT_OK;

    if( p_task_count == 0U ) {
        ret_val = SCHEDULER_INIT_INSUFFICIENT_TASKS;
    } else if( scheduler_status == SCHEDULER_STATUS_RUNNING ) {
        ret_val = SCHEDULER_INIT_ALREADY_RUNNING;
    } else if( p_task_count > SCHEDULER_MAX_TASK_COUNT ) {
        ret_val = SCHEDULER_INIT_TOO_MANY_TASKS;
    } else {
        /* Store these for later use */
        scheduler_task_count = p_task_count;
        scheduler_task_table = p_task_definitions;

        scheduler_status = SCHEDULER_STATUS_INITIALISED;
    }

    return ret_val;
}

/* documentation with prototype in header file */
fn_scheduler_start_return_t scheduler_start( void ) {
    fn_scheduler_start_return_t ret_val = SCHEDULER_START_OK;

    if( scheduler_status == SCHEDULER_STATUS_UNINITIALISED ) {
        ret_val = SCHEDULER_START_NOT_INITIALISED;
    } else if( scheduler_status != SCHEDULER_STATUS_INITIALISED ) {
        ret_val = SCHEDULER_START_ALREADY_RUNNING;
    } else {
        task_counter_t task_loop;

        scheduler_lock_depth = 0U;
        scheduler_waiting_task_level = 0U;

        /* This is used by scheduler_task_set_event, so we initialise it here,
           but then re-assign it if necessary */
        scheduler_current_task_level = 0U;

        /* Initialise the task_control values */
        for( task_loop = 0U;
             task_loop < scheduler_task_count;
             task_loop++ ) {
            task_control[ task_loop ].task_status          = TASK_STATUS_READY;
            task_control[ task_loop ].event_count          = 0U;
            task_control[ task_loop ].context_switch_count = 0U;
            task_control[ task_loop ].task_initialised     = FALSE;
        }

        /* Lock the scheduler while we're meddling with internal variables */
        (void)scheduler_lock();
        /* Try and kick off the idle task */
        /** @polyspace<MISRA-C:12.6:Not a defect:Justify with annotations> Construct is MISRA compliant */
        /** @polyspace<MISRA-C:13.2:Not a defect:Justify with annotations> Construct is MISRA compliant */
        if( !scheduler_task_set_event( SCHEDULER_LOWEST_PRIORITY_TASK ) ) {
            /* No need to set scheduler_waiting_task_level here, it's done
               as part of the scheduler_unlock() call */

            /* Use value > scheduler_task_count to indicate no task currently
               running */
            scheduler_current_task_level = UINT8_MAX;
            scheduler_status =             SCHEDULER_STATUS_RUNNING;
        } else {
            ret_val = SCHEDULER_START_UNABLE_TO_START_IDLE_TASK;
        }
        /* This will initiate a context switch if one is due */
        (void)scheduler_unlock();
    }

    return ret_val;
}


/* documentation with prototype in header file */
RAW_INTERRUPT void
scheduler_switch_context( void ) {
    /* IMPORTANT: Don't add any local stack variables!
                  The stack overhead of this function has been measured (zero)
                  and is used in the code below - altering this overhead would
                  cause this function to misbehave.  Also, be careful adding
                  function calls. */

    if( scheduler_status == SCHEDULER_STATUS_SHUTDOWN ) {
        /** @polyspace<MISRA-C:2.1:Low:Investigate> Should this assembler be encapsulated? */
        /** @polyspace<MISRA-C:14.2:Low:Justify with annotations> Assembler code, therefore no C-side effects */
        (void) ASM("XREF _scheduler_sp_start\nLDS _scheduler_sp_start");
        scheduler_status = SCHEDULER_STATUS_INITIALISED;
    }
    /* Check that a context switch is actually needed
       - If the waiting task isn't the same as the current task, or if they are
          the same task but the task is not initialised
       - If the scheduler's locked we never perform a context switch
    */
    else if( IS_SCHEDULER_ENABLED() &&
        (( scheduler_current_task_level != scheduler_waiting_task_level ) ||
         (task_control[ scheduler_waiting_task_level ].task_initialised == FALSE ))) {

#if defined( SCHEDULER_ENABLE_TIMING_STATS )
        time_now =ts_get_ticks();
#endif
        old_task_record = &(task_control[ scheduler_current_task_level ]);
        new_task_record = &(task_control[ scheduler_waiting_task_level ]);

        /* Check the current task is a vaid one - the first time this is called,
        ||  following task_init, scheduler_current_task will not be valid */
        if( scheduler_current_task_level < scheduler_task_count ) {
#if defined( SCHEDULER_ENABLE_TIMING_STATS )
            old_task_record->total_cpu_time_used += time_now - task_last_switch_time;
#endif
            /* If the outgoing task is still intialised, then it mustn't have
               terminated */
            /** @polyspace<MISRA-C:13.2:Not a defect:Justify with annotations> Construct is MISRA compliant */
            if( old_task_record->task_initialised ) {

                /* Perform the remainder of the basic context switch - the ISR
                   prologue has already pushed the following (ref CPU12/CPU12X
                   Reference Manual v01.03):

                   -----------8<-------------------------------------------------
                   Memory Location   CPU12 Registers
                            SP + 8   RTNH : RTNL
                            SP + 6     YH : YL
                            SP + 4     XH : XL
                            SP + 2      B : A
                                SP   CCRH : CCRL

                   After the CCR is stacked, the I bit (and the X bit, if an XIRQ
                   interrupt service request caused the interrupt) is set to
                   prevent other interrupts from disrupting the interrupt service
                   routine.
                   -----------8<-------------------------------------------------

                   We just need to push the additional things which we care about
                */
                /** @polyspace:begin<MISRA-C:2.1:Low:Investigate> Should this assembler be encapsulated? */
                /** @polyspace:begin<MISRA-C:14.2:Low:Justify with annotations> Assembler code, therefore no C-side effects */
                (void) ASM2("pshb",PPAGE);
                (void) ASM2("pshb",GPAGE);
                (void) ASM2("pshb",RPAGE);
                /** @polyspace:end<MISRA-C:14.2:Low:Justify with annotations> Assembler code, therefore no C-side effects */
                /** @polyspace:end<MISRA-C:2.1:Low:Investigate> Should this assembler be encapsulated? */

                /* Return the old task to the waiting state */
                old_task_record->task_status = TASK_STATUS_WAITING;

            } else {
                /*
                 * The task has terminated, so we need to un-wind its stack
                 * and the stack frame pushed by this SYS interrupt.  This will
                 * leave us at the stack from the previous interrupt, allowing
                 * us to restore the context and return to it.
                 * The task stack will be the two bytes containing the return
                 * address from the call to scheduler_task_wrapper()
                 * The interrupt stack will be the usual 10 bytes.
                 *
                 * Therefore we need to pop 12 bytes off the stack
                 */
                /** @polyspace<MISRA-C:2.1:Low:Investigate> Should this assembler be encapsulated? */
                /** @polyspace<MISRA-C:14.2:Low:Justify with annotations> Assembler code, therefore no C-side effects */
                (void)  ASM("leas 12,s");

                /* Depending on whether there are any events still outstanding
                   for this task, reduce the status appropriately */
                if( old_task_record->event_count > 0U ) {
                    old_task_record->task_status = TASK_STATUS_WAITING;
                } else {
                    old_task_record->task_status = TASK_STATUS_READY;
                }
            }
        } else {
            /* Store the current stack-pointer - to be used when shutting down
               the scheduler */
            /** @polyspace<MISRA-C:2.1:Low:Investigate> Should this assembler be encapsulated? */
            /** @polyspace<MISRA-C:14.2:Low:Justify with annotations> Assembler code, therefore no C-side effects */
            (void) ASM("XREF _scheduler_sp_start\nSTS _scheduler_sp_start");
        }

#if defined( SCHEDULER_DEBUG )
        scheduler_previous_task_level = scheduler_current_task_level;
#endif

        /* Switch the new task to be the current one */
        scheduler_current_task_level = scheduler_waiting_task_level;

#if defined( SCHEDULER_ENABLE_TIMING_STATS )
        task_last_switch_time = time_now;
#endif

        /* Set the new task to the running state */
        new_task_record->task_status = TASK_STATUS_RUNNING;

        new_task_record->context_switch_count++;

        /* Does the task already have a stack set up? */
        /** @polyspace<MISRA-C:13.2:Not a defect:Justify with annotations> Construct is MISRA compliant */
        if( new_task_record->task_initialised ) {
            /* Pop registers which we care about off the stack.  This list
               must match that which is pushed in the list above (but in reverse
               order). */
#if ! defined (POLYSPACE)
            RPAGE=(uint8_t) ASM("pulb");
            GPAGE=(uint8_t) ASM("pulb");
            PPAGE=(uint8_t) ASM("pulb");
#else /* ! defined (POLYSPACE) */
            RPAGE=0u;
            GPAGE=0u;
            PPAGE=0u;
#endif /* ! defined (POLYSPACE) */
        } else {
            /* Push a fake stack frame for the RTI instruction to restore
               context from */

            /* Push the task wrapper address into the RTN address */
            /** @polyspace:begin<MISRA-C:2.1:Low:Investigate> Should this assembler be encapsulated? */
            /** @polyspace:begin<MISRA-C:14.2:Low:Justify with annotations> Assembler code, therefore no C-side effects */
            (void) ASM2("pshd",scheduler_task_wrapper);
            /* X and Y are unimportant so just incorporate the next four
             * bytes into the stack */
            (void)  ASM("leas -4,s");
            /* D is set to the task level (i.e. the parameter to
               scheduler_task_wrapper) */
            (void) ASM2("pshd",(uint16_t)(((uint16_t)scheduler_current_task_level) << 8U ));
            /* CCR is set to all 0s apart from the X-Bit. Clearing the X-Bit enables XIRQ and thus
             * should only be cleared if XIRQ is required. */
            (void) ASM2("pshd",XIRQ_EN_IN_CCR);
            /** @polyspace:end<MISRA-C:14.2:Low:Justify with annotations> Assembler code, therefore no C-side effects */
            /** @polyspace:end<MISRA-C:2.1:Low:Investigate> Should this assembler be encapsulated? */

            new_task_record->task_initialised = TRUE;
        }
    } else {
        /* No context change required */
    }

    /* ReTurn from Interrupt */
}

/* documentation with prototype in header file */
fn_scheduler_stop_return_t
scheduler_stop( void ) {
    fn_scheduler_stop_return_t ret_val = SCHEDULER_STOP_OK;

    /* Check that current status is condusive with the scheduler being asked to
       stop */
    if( scheduler_status == SCHEDULER_STATUS_RUNNING ) {
        DISABLE_INTERRUPTS();
        scheduler_status =             SCHEDULER_STATUS_SHUTDOWN;
        /** @polyspace<MISRA-C:2.1:Low:Investigate> Should this assembler be encapsulated? */
        /** @polyspace<MISRA-C:14.2:Low:Justify with annotations> Assembler code, therefore no C-side effects */
        SCHEDULER_INITIATE_CONTEXT_SWITCH();
        /* We should never reach this point - the task this function was called
           from will be dead after the context switch */
    } else {
        ret_val = SCHEDULER_STOP_NOT_RUNNING;
    }
    return ret_val;
}

/* documentation with prototype in header file */
bool_t
scheduler_lock(void) {
    bool_t ret_val = TRUE;
    uint8_t CCR_reg;

    /* Critical section - scheduler_lock_depth is shared */
    SAVE_INTERRUPTS( CCR_reg );

    if( scheduler_lock_depth < UINT8_MAX ) {
        scheduler_lock_depth++;
        ret_val = FALSE;
    }

    /* End critical section */
    RESTORE_INTERRUPTS( CCR_reg );

    return( ret_val );
}

/* documentation with prototype in header file */
bool_t
scheduler_unlock(void) {
    bool_t ret_val = FALSE;
    uint8_t CCR_reg;

    /* Critical section - scheduler_lock_depth is shared */
    SAVE_INTERRUPTS( CCR_reg );

    /* If the locking depth is one, we un-lock the scheduler */
    if( scheduler_lock_depth == 1U ) {
        /* Not locked any more */
        scheduler_lock_depth = 0U;

        /* Find the next waiting task */
        scheduler_waiting_task_level = scheduler_get_waiting_task();

        /* Is a new task waiting? */
        if( scheduler_waiting_task_level != scheduler_current_task_level ) {
            /* End critical section */
            RESTORE_INTERRUPTS( CCR_reg );
            /* Execute system call to do context switch */
            /** @polyspace<MISRA-C:2.1:Low:Investigate> Should this assembler be encapsulated? */
            /** @polyspace<MISRA-C:14.2:Low:Justify with annotations> Assembler code, therefore no C-side effects */
            SCHEDULER_INITIATE_CONTEXT_SWITCH();
        } else {
            /* End critical section */
            RESTORE_INTERRUPTS( CCR_reg );
        }
    }
    /* Locking depth > 1, we just un-wind a level */
    else if( scheduler_lock_depth > 1U ) {
        scheduler_lock_depth--;
        /* End critical section */
        RESTORE_INTERRUPTS( CCR_reg );
    } else {
        /* Scheduler unlock was called when the scheduler was already
           unlocked. */
        ret_val = TRUE;
    }

    return( ret_val );
}


/* documentation with prototype in header file */
bool_t
scheduler_task_set_event( const task_priority_level_t p_level ) {
    /* Sanity check the event level */
    if( p_level <= SCHEDULER_MAX_PRIORITY_LEVEL ) {
        /* If the event causes a new task, higher priority task to be waiting ... */
        /** @polyspace<MISRA-C:13.2:Not a defect:Justify with annotations> Construct is MISRA compliant */
        if( scheduler_task_set_event_no_reschedule( p_level ) ) {
            /* Re-schedule */
            /** @polyspace<MISRA-C:2.1:Low:Investigate> Should this assembler be encapsulated? */
            /** @polyspace<MISRA-C:14.2:Low:Justify with annotations> Assembler code, therefore no C-side effects */
            SCHEDULER_INITIATE_CONTEXT_SWITCH();
        }
    }
    /** @todo sort out return value */
    return( FALSE );
}

/* documentation with prototype in header file */
task_status_t
scheduler_get_task_status( const task_priority_level_t p_level ) {
    task_status_t ret_val = TASK_STATUS_UNKNOWN;

    if( p_level <= SCHEDULER_MAX_PRIORITY_LEVEL ) {
        ret_val = task_control[ p_level ].task_status;
    }

    return( ret_val );
}

/* documentation with prototype in header file */
task_priority_level_t
scheduler_get_current_task( void ) {
    task_priority_level_t ret_val = 0U;

    ret_val = scheduler_current_task_level;

    return( ret_val );
}

ts_ticks_t
scheduler_get_last_duration( const task_priority_level_t p_level ) {
    ts_ticks_t ret_val = 0U;
    if( p_level <= SCHEDULER_MAX_PRIORITY_LEVEL ) {
#if defined( SCHEDULER_ENABLE_TIMING_STATS )
        ret_val = task_control[ p_level ].total_cpu_time_used;
#endif
    }
    return( ret_val );
}

/* documentation with prototype in header file */
bool_t
scheduler_task_set_event_no_reschedule( const task_priority_level_t p_event ) {
    bool_t ret_val = FALSE;

    /* Check that the task is enabled before we modify anything */
    if( task_control[ p_event ].task_status != TASK_STATUS_DISABLED ) {
        uint8_t CCR_reg;

        /* Critical section: dealing with shared data */
        SAVE_INTERRUPTS( CCR_reg );

        /* If the task has event queueing enabled and there are already events
        || in the queue */
        if((( (uint16_t)scheduler_task_table[ p_event ].task_options & (uint16_t)TASK_OPTIONS_ENABLE_EVENT_QUEUEING ) != 0u) &&
           ( task_control[ p_event ].event_count > 0U )) {

            /** @todo Check for overflow */
            /* Increment the number of events pending */
            task_control[ p_event ].event_count++;

            /* Task should still be in waiting state if there are events
            || pending */
        } else {
            task_control[ p_event ].event_count = 1U;

            /* If the task is in the waiting state, it can progress to the waiting
            || state */
            if( task_control[ p_event ].task_status == TASK_STATUS_READY ) {
                task_control[ p_event ].task_status = TASK_STATUS_WAITING;
            }
        }

        /* If the task we've just set as waiting is of a higher priority than the
        || current one, and it's in the waiting state */
        if(( p_event > scheduler_waiting_task_level ) &&
           ( task_control[ p_event ].task_status == TASK_STATUS_WAITING )) {
            scheduler_waiting_task_level = p_event;
            /* Only return that a context change should be made if the
             * scheduler's enabled */
            if( IS_SCHEDULER_ENABLED() ) {
                ret_val = TRUE;
            }
        }

        RESTORE_INTERRUPTS( CCR_reg );
    }

    return( ret_val );
}

/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/

/* documentation with prototype ealier in this file */
PRIVATE NCODE void
scheduler_task_wrapper( task_priority_level_t p_level ) {
    /* THIS FUNCTION MUST NOT USE STACK SPACE.

       The call to SCHEDULER_INITIATE_CONTEXT_SWITCH() means that the function
       will never get to the stage where it unwinds stack
    */
    /** @polyspace:begin<MISRA-C:2.1:Low:Investigate> Should this assembler be encapsulated? */
#if defined GPAGE_MODC
    /** @polyspace:begin<MISRA-C:14.2:Low:Justify with annotations> Assembler code, therefore no C-side effects */
    /* functions will expect GPAGE to point to the efficient constant region */
    (void)  ASM("xref __sgconst");
    (void)  ASM("movb  #phigh(__sgconst),_GPAGE");
    /** @polyspace:end<MISRA-C:14.2:Low:Justify with annotations> Assembler code, therefore no C-side effects */
#elif defined GPAGE_MODD
    /** @polyspace:begin<MISRA-C:14.2:Low:Justify with annotations> Assembler code, therefore no C-side effects */
    /* functions will expect GPAGE to point to the efficient data region */
    (void)  ASM("xref __sgdata");
    (void)  ASM("movb  #phigh(__sgdata),_GPAGE");
    /** @polyspace:end<MISRA-C:14.2:Low:Justify with annotations> Assembler code, therefore no C-side effects */
#else
    /* functions will set GPAGE before using it */
#endif
    /** @polyspace:end<MISRA-C:2.1:Low:Investigate> Should this assembler be encapsulated? */
    (scheduler_task_table[ p_level ].task_function)( p_level );

    /* We don't want to be interrupted from this point on, as if we get
     * interrupted it could leave the event & task_initialised variables in an
     * inconsistent state.  Note that there's no corresponding ENABLE_INTERRUPTS
     * for this disable, as when we'll never return to this context once we
     * execute the system call below */
    DISABLE_INTERRUPTS();

    /* Flag that the task is done */
    task_control[ p_level ].task_initialised = FALSE;

    /* Prevent the lowest priority task from being deactivated */
    if( p_level > SCHEDULER_LOWEST_PRIORITY_TASK ) {
        scheduler_task_unset_event( p_level );
    }
    /* Find the next waiting task */
    scheduler_waiting_task_level = scheduler_get_waiting_task();

    /* Switch to it */
    /** @polyspace<MISRA-C:2.1:Low:Investigate> Should this assembler be encapsulated? */
    /** @polyspace<MISRA-C:14.2:Low:Justify with annotations> Assembler code, therefore no C-side effects */
    SCHEDULER_INITIATE_CONTEXT_SWITCH();
}

/* documentation with prototype ealier in this file */
PRIVATE task_priority_level_t
scheduler_get_waiting_task( void ) {
    task_priority_level_t ret_val = 0U;
    task_counter_t loop;

    for( loop = SCHEDULER_MAX_PRIORITY_LEVEL;
         loop <= SCHEDULER_MAX_PRIORITY_LEVEL;
         loop-- ) {
        if( task_control[ loop ].event_count > 0U ) {
            /** @todo We make the assumption here that if the task has an event
               associated with it, it's READY (i.e. not disabled) */
            ret_val = (task_priority_level_t)loop;
            break;
        }
    }

    return( ret_val );
}

/* documentation with prototype ealier in this file */
PRIVATE void
scheduler_task_unset_event( const task_priority_level_t p_level ) {
    uint8_t CCR_reg;

    /* Critical section: dealing with shared data */
    SAVE_INTERRUPTS( CCR_reg );

    /* If the task has event queueing enabled, and there are events still queued */
    if((( (uint16_t)scheduler_task_table[ p_level ].task_options & (uint16_t)TASK_OPTIONS_ENABLE_EVENT_QUEUEING ) != 0u) &&
       ( task_control[ p_level ].event_count > 1U )) {
        /* Decrement the number of events remaining */
        task_control[ p_level ].event_count--;
    } else {
        /** @todo prevent a basic task calling this bit on itself */
        task_control[ p_level ].event_count = 0U;
    }

    RESTORE_INTERRUPTS( CCR_reg );
}
