/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:   scheduler_types.h
 *
 * description:  Public header for Scheduler
 *
 *
 *
 * revision history:
 *
 * Date        Author           Description
 * 2015-Aug-10 Rui Li   initial version
 *                       
 *
 *****************************************************************************/
#if !defined(SCHEDULER_TYPES_H)
#define      SCHEDULER_TYPES_H
/*************************************************************************//*!
 *
 * @file
 * @ingroup Scheduler
 * Public header for Scheduler
 *
 *
 *****************************************************************************/

/******************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************/

/**************************************************************************//**
 * The maximum number of task priorities supported by this scheduler
 * configuration
 *****************************************************************************/
#define SCHEDULER_MAX_PRIORITY_LEVEL (4U)

/**************************************************************************//**
 * The maximum number of tasks which the scheduler can cope with
 *****************************************************************************/
#define SCHEDULER_MAX_TASK_COUNT (SCHEDULER_MAX_PRIORITY_LEVEL+1U)

/**************************************************************************//**
 * The priority level of the lowest priority task
 *****************************************************************************/
#define SCHEDULER_LOWEST_PRIORITY_TASK ((task_priority_level_t)0U)

/**************************************************************************//**
 * Helper macro for declaring/defining task functions.  e.g.:
 *   SCHEDULER_TASK_DECL( task_foo );
 *   SCHEDULER_TASK_DECL( task_foo ) { run_it( p_level ); }
 *****************************************************************************/
/* polyspace<MISRA-C:19.4:Low:Justify with annotations> Is it possible to make this compliant and have compilable code? */
#define SCHEDULER_TASK_DECL(_n) void (_n)( const task_priority_level_t p_level )

/**
 * Macro to invoke the scheduler at the end of an interrupt service routine
 * in which scheduler_task_set_event_no_reschedule() has been called
 *
 * @note the invoking ISR must have been declared with \@svpage and must
 * not have any local variables to ensure the stack is in a predictable state.
 *
 * @par Implementation
 * Code needs an xref because compiler will 'optimise' out the function
 * prototype if it's not called from C.
 *
 * The code relies on GPAGE/RPAGE having been pushed on the stack with the
 * sequence
 * @code
 *      ldab GPAGE
 *      ldx RPAGE
 *      pshb
 *      pshx
 * @endcode
 */
#if (!defined (POLYSPACE))
#define SCHEDULER_DO_CONTEXT_SWITCH_ISR() \
        do{ \
            (void) ASM("xref _scheduler_switch_context");\
            /* restore RPAGE and GPAGE manually from the stack */ \
            (void) ASM("pulx\n pulb\n stx RPAGE\n stab GPAGE\n"); \
            (void) ASM("jmp _scheduler_switch_context");\
        } while(0)
#else
#define SCHEDULER_DO_CONTEXT_SWITCH_ISR()
#endif

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/

/**************************************************************************//**
 * The priority of a task, with zero being the lowest priority and the maximum
 *  value being MAX_PRIORITY_LEVEL
 *****************************************************************************/
typedef uint8_t task_priority_level_t;

/**************************************************************************//**
 * Used to count tasks.  Synonymous with task_priority_level_t
 *****************************************************************************/
typedef task_priority_level_t task_count_t;

/**************************************************************************//**
 * Function type of a schedulable task.
 * There are no particular constraints relating to the task function from the
 *  perspective of the scheduler - in order to complete, the task simply
 *  returns from this functions.
 *
 * @param p_level The task priority level of the task
 *****************************************************************************/
typedef void(*task_pointer_t)( const task_priority_level_t p_level );

/**************************************************************************//**
 * Enumeration of all the potential states of a task
 *****************************************************************************/
typedef enum {
    /** The task is disabled and will not be scheduled */
    TASK_STATUS_DISABLED  = 0,
    /** The task is awaiting an event */
    TASK_STATUS_READY     = 1,
    /** The task is ready to be run */
    TASK_STATUS_WAITING   = 2,
    /** The task is currently running */
    TASK_STATUS_RUNNING   = 3,
    /** Used to indicate that the scheduler does not have that information,
        for example in the case where reference has been made to an invalid
        task */
    TASK_STATUS_UNKNOWN   = 255
} task_status_t;

/**************************************************************************//**
 * Enumeration of options relating to tasks
 *****************************************************************************/
typedef enum {
    /** No options are enabled for this task */
    TASK_OPTIONS_NONE = 0,
    /** Enable event queueing.  If event queueing is enabled, multiple calls
        to scheduler_task_set_event() which cannot be serviced immediately
        (e.g. the task is already running) will be queued, resulting in the
        task being triggered a corresponding number of times when scheduling
        allows.  If task queueing is not enabled for a task, events which
        occurr when the task is waiting or running will have no effect */
    TASK_OPTIONS_ENABLE_EVENT_QUEUEING = 1
} task_options_t;

/**************************************************************************//**
 * Type used to enumerate the return value of the scheduler_init function
 *****************************************************************************/
typedef enum {
    /** Successful */
    SCHEDULER_INIT_OK = 0,
    /** Could not process, the scheduler is already running */
    SCHEDULER_INIT_ALREADY_RUNNING,
    /** Could not process, not enough tasks defined */
    SCHEDULER_INIT_INSUFFICIENT_TASKS,
    /** Could not process, too many tasks defined.  See
        SCHEDULER_MAX_TASK_COUNT */
    SCHEDULER_INIT_TOO_MANY_TASKS
} fn_scheduler_init_return_t;

/**************************************************************************//**
 * Type used to enumerate the return value of the scheduler_start function
 *****************************************************************************/
typedef enum {
    /** Successful */
    SCHEDULER_START_OK = 0,
    /** Could not process, the scheduler is already running */
    SCHEDULER_START_ALREADY_RUNNING,
    /** Could not process, the scheduler has not been initialised via
        scheduler_init() */
    SCHEDULER_START_NOT_INITIALISED,
    /** Could not process, the scheduler has not been initialised via
        scheduler_init() */
    SCHEDULER_START_UNABLE_TO_START_IDLE_TASK
} fn_scheduler_start_return_t;

/**************************************************************************//**
 * Type used to enumerate the return value of the scheduler_stop function
 *****************************************************************************/
typedef enum {
    /** Successful */
    SCHEDULER_STOP_OK = 0,
    /** Could not process, the scheduler is not running */
    SCHEDULER_STOP_NOT_RUNNING
} fn_scheduler_stop_return_t;

#endif      /* SCHEDULER_TYPES_H */
