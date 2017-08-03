/*****************************************************************************
 *
 * Copyright (c) Linkedriving Tech 2015 - All rights reserved 
 *
 * file name:    scheduler.h
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
/*************************************************************************//*!
 *
 * @file
 * @ingroup Scheduler
 * Public header for Scheduler
 *
 *
 *****************************************************************************
 *
 * This header defines the public interface to the Scheduler module
 *
 *****************************************************************************/

/**
 * The Scheduler module implements a relatively simple multi-task pre-emptive
 *  scheduler, with a loose base on the OSEK scheduler definition.  A common
 *  stack is used by all tasks.
 *
 * All tasks run outside of an interrupt context.
 *
 * The core scheduler does not take care of the chronogology of scheduling,
 *  but treats all tasks as being event-based and concentrates on the task
 *  of switching context from one task to another
 *
 * Within the scheduler, a task is always running, thus it is common that the
 *  lowest priority task be defined as the 'idle' task.  It should always be
 *  enabled and not exit.  All other tasks should be 'run-to-completion' type
 *  rather than long-running, otherwise they will block lower priority tasks
 *  for the duration of their execution
 *
 * The scheduler will always run the highest priority task, except when
 *  re-scheduling has been disabled via the scheduler_lock() function.
 *  Upon the scheduler being unlocked a scheduling point will be introduced.
 *
 * The scheduler is based around the use of an interrupt to trigger a
 *  context switch, then making use of the CPU's support for storing and
 *  retrieving context.  The SYS interrupt has been chosen for this purpose,
 *  though other mechanisms such as the SWI would be equally suitable.
 *  The basic principle for switching contexts is:
 *
 *  - Context 1 calls scheduler_task_set_event()
 *  - SWI
 *    - Interrupt context
 *    - Scheduler context switch
 *    - Interrupt Return
 *  - Context 2 Start
 *  - Context 2 Complete
 *  - SWI
 *    - Interrupt context
 *    - Scheduler context switch
 *    - Interrupt Return
 *  - Context 1, scheduler_task_set_event()
 *
 * Each task function is automatically wrapped in a container function which
 *  performs some scheduler administration and 'catches' the completion of
 *  the task, invoking a scheduling point to switch to the next appropriate
 *  task.  The current scheduler implementation stores PPAGE, RPAGE and
 *  GPAGE as part of the application context, but does NOT store EPAGE.
 *
 *
 * @par Usage
 * <ul>
 * <li> The scheduler must be initialised via the scheduler_init() function
 * <li> The scheduler_start() function is used to being scheduling functions
 * <li> The scheduler_task_set_event() function is used to trigger tasks while
 *     the scheduler is running
 * <li> scheduler_lock() and scheduler_unlock() are used to prevent and
 *    re-enable context switches.
 * <li> scheduler_stop() may be used in order to terminate the scheduler
 * </ul>
 *
 * @par Scheduling from within interrupts
 *
 * If it is necessary to set an event from within an interrupt context, it is
 * necessary to:
 * <ul>
 * <li> Use the scheduler_task_set_event_no_reschedule() in place of
 *     scheduler_task_set_event()
 * <li> Call SCHEDULER_DO_CONTEXT_SWITCH_ISR() as the last statement in the
 *     interrupt function
 * <li> The ISR <b> must </b> be defined with the \@svpage keyword
 * <li> The ISR function must <b> NOT </b> have any stack overhead other than
 *      page saving (i.e. no local variables)
 * </ul>
 *
 * This is due to the fact that the SYS interrupt is not I-maskable, hence
 *  calling scheduler_task_set_event() may result in a SYS interrupt being
 *  invoked, leading to nesting of interrupts
 *
 * @par Build-time options
 * <ul>
 * <li> SCHEDULER_DEBUG : Enables some extra variables which may prove useful
 *                      when debugging
 * <li> SCHEDULER_ENABLE_TIMING_STATS : Enables the measuring of task execution
 *                                    time and other useful chronometric
 *                                    stats.  Not enabled by default as has
 *                                    associated overhead.
 * <li> Plus precisely one of:
 * <ul>
 * <li> GPAGE_MODC : Code will be compiled with +modc
 * <li> GPAGE_MODD : Code will be compiled with +modd
 * <li> GPAGE_MODG : Code will be compiled with +modg
 * </ul>
 * </ul>
 *
 * @par Example
 * @code
 *    // The number of tasks employed by the application
 *    #define TASK_COUNT (3U)
 *
 *    // Priority level of the foreground task
 *    #define FG_TASK_LEVEL (2U)
 *
 *    // Priority level of the background task
 *    #define BG_TASK_LEVEL (1U)
 *
 *    SCHEDULER_TASK_DECL( task_idle ) {
 *        while (TRUE) {
 *            ServiceCOP();
 *            if( trigger ) {
 *                scheduler_task_set_event( FG_TASK_LEVEL );
 *            }
 *        }
 *    }
 *
 *    SCHEDULER_TASK_DECL( task_bg ) {
 *       ...
 *    }
 *
 *    SCHEDULER_TASK_DECL( task_fg ) {
 *       ...
 *       scheduler_task_set_event( BG_TASK_LEVEL );
 *       ...
 *    }
 *
 *    INTERRUPT void isr_fn(void) {
 *        (void)scheduler_task_set_event_no_reschedule( FG_TASK_LEVEL );
 *        SCHEDULER_DO_CONTEXT_SWITCH_ISR();
 *    }
 *
 *    // Task definition structure
 *    PRIVATE const struct task_definition_s tasks[ TASK_COUNT ] = {
 *        { task_idle, TASK_OPTIONS_NONE },
 *        { task_bg,   TASK_OPTIONS_NONE },
 *        { task_fg,   TASK_OPTIONS_ENABLE_EVENT_QUEUEING }
 *    };
 *
 *    void prog_main(void) {
 *
 *        // Initialise the scheduler
 *        scheduler_init(tasks, TASK_COUNT);
 *
 *        // Start the scheduler running - this will kick off the idle task
 *        // automatically and won't return until scheduler_stop() is called
 *        scheduler_start();
 *    }
 *
 * @endcode
 * @defgroup Scheduler Task Scheduler
 */

#if !defined(SCHEDULER_H)
#define      SCHEDULER_H

/******************************************************************************
 * INCLUDES
 *****************************************************************************/

#include "basic_types.h"
#include <ts.h>
#include "scheduler_types.h"

/**************************************************************************//**
 * Type used to define a task.  The data in this structure is intended to be
 *  constant.
 *****************************************************************************/
struct task_definition_s {
    /** Pointer to the function which implements the task */
    task_pointer_t task_function;
    /** Task-specific options.  See the members of the task_options_t for
        details */
    task_options_t task_options;
};

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/

/**************************************************************************//**
 *
 *  @brief          Initialises the scheduler module.  Should be called once
 *                  prior to calling scheduler_start()
 *
 *  @param[in]      p_task_definitions - An array of task definitions.  Must
 *                  be at least one entry long.  Following the call to
 *                  scheduler_init, the scheduler will continue to refer
 *                  to the array pointed to by this parameter, so it must
 *                  continue to exist, unchanged in memory.  Note that
 *                  there is a limit on the number of elements which may be
 *                  contained within this array - see SCHEDULER_MAX_TASK_COUNT
 *  @param[in]      p_task_count - The size of the array pointed to by the
 *                  p_task_definitions array
 *
 *  @return         Status code indicating the success/failure of the function
 *                  call.  See the enumerated type for more information
 *
 *****************************************************************************/
extern fn_scheduler_init_return_t
scheduler_init( const struct task_definition_s* const p_task_definitions,
                const        task_count_t             p_task_count );

/**************************************************************************//**
 *
 *  @brief          Starts scheduling tasks.
 *
 *  @return         Status code indicating the success/failure of the function
 *                  call.  See the enumerated type for more information
 *
 *  @par Description:
 *                  scheduler_init() must have been called prior to calling
 *                  this function.  This function should only be called once.
 *                  Upon calling scheduler_start(), the lowest priority task
 *                  (SCHEDULER_LOWEST_PRIORITY_TASK) will be set to be active
 *                  and execution of that task will begin immediately.
 *                  The function call will not return until/unless
 *                  scheduler_stop() is called.
 *
 *****************************************************************************/
extern fn_scheduler_start_return_t
scheduler_start( void );

/**************************************************************************//**
 *
 *  @brief          Used to switch task contexts
 *
 *  @par Description:
 *                  This function must be hooked up to the SYS interrupt in
 *                  such a way that no stack is employed between the interrupt
 *                  being received and this function being started.  This
 *                  function will take care of the interrupt return.  This
 *                  function should not be called directly from the
 *                  application code - scheduler_task_set_event() should be
 *                  used in order to schedule a task
 *                  When triggered, the function will evaluate the current
 *                  status of all the tasks and will switch context to the
 *                  highest priority task which is waiting to execute
 *
 *****************************************************************************/
extern void RAW_INTERRUPT
scheduler_switch_context( void );

/**************************************************************************//**
 *
 *  @brief          Stops scheduling tasks.
 *
 *  @return         Status code indicating the success/failure of the function
 *                  call.  See the enumerated type for more information
 *
 *  @par Description:
 *                  scheduler_stop() may be used to effectively shut down the
 *                  scheduler.  All tasks will be terminated, the scheduler
 *                  will return to an uninitialised state and execution will
 *                  continue from the point after the call to
 *                  scheduler_start() was made, hence the function call will
 *                  only return to the context which called it in the case
 *                  of a failure.
 *
 *****************************************************************************/
extern fn_scheduler_stop_return_t
scheduler_stop( void );

/**************************************************************************//**
 *
 *  @brief          Stop scheduler points
 *
 *  @return         Status code indicating the success/failure of the function
 *                  call.  FALSE indicates that there were no problems and the
 *                  call was successful.  An unsuccessful return indication
 *                  could result from the depth limit being exceeded.
 *
 *  @par Description:
 *                  scheduler_lock() provides an effective mechanism to prevent
 *                  the current task being pre-empted without needing to
 *                  globally disable interrupts.  Once called, all future
 *                  scheduling points will be ignored until the scheduler is
 *                  unlocked via a call to scheduler_unlock().  Any events
 *                  which would trigger a task will be tracked and upon the
 *                  scheduler being unlocked a scheduling point will be
 *                  introduced and these events will be resolved.
 *                  scheduler_lock() employs depth counting, hence nested
 *                  calls will be resolved - a call to scheduler_unlock() will
 *                  not necessarily unlock the scheduler right away - this will
 *                  only occurr when the same number of unlocking calls have
 *                  been made as there were locking calls.
 *
 *****************************************************************************/
extern bool_t
scheduler_lock( void );

/**************************************************************************//**
 *
 *  @brief          Re-start scheduler points
 *
 *  @return         Status code indicating the success/failure of the function
 *                  call.  FALSE indicates that there were no problems and the
 *                  call was successful.  An unsuccessful return indication
 *                  could be due to the scheduler already being in an unlocked
 *                  state
 *
 *  @par Description:
 *                  scheduler_unlock() should be called at some point following
 *                  a call to scheduler_lock()  in order to re-enable scheduling
 *                  Please see scheduler_lock() for more complete notes
 *                  relating to this pair of functions
 *
 *****************************************************************************/
extern bool_t
scheduler_unlock( void );

/**************************************************************************//**
 *
 *  @brief          Set a task event, requesting execution of the task and then
 *                   invoke a scheduling point
 *
 *  @param[in]      p_level - The priority level of the task which should be
 *                  triggered
 *
 *  @return         Status code indicating the success/failure of the function
 *                  call.  FALSE indicates that there were no problems and the
 *                  call was successful.  An unsuccessful return indication
 *                  could be due to the event queue for that task being full
 *
 *  @par Description:
 *                  The task associated with p_level will be flagged as being
 *                  ready to execute.  If event queueing is enabled for the
 *                  associated task, mutliple calls to this function with
 *                  the same value of p_level will be queued.  See
 *                  TASK_OPTIONS_ENABLE_EVENT_QUEUEING for more information on
 *                  how event queueing works.
 *                  Actions taken:
 *                  - If scheduler is locked, or p_level is <= current_level
 *                    then the task will be set to the waiting status if it
 *                    is not already and execution continues in the current
 *                    task context
 *                  - If p_level is > current level, initiate a context switch
 *
 *                  This function should not be used from within interrupts
 *                  for the reason outlined in this module's documentation
 *
 *****************************************************************************/
extern bool_t
scheduler_task_set_event( const task_priority_level_t p_level );

/**************************************************************************//**
 *
 *  @brief          Set a task event, requesting execution of the task, but
 *                   without a subsequent scheduling point
 *
 *  @param[in]      p_event - The priority level of the task with which the
 *                            event is associated
 *
 *  @par Description:
 *                  Literally identical to scheduler_task_set_event() when
 *                  the scheduler is locked.
 *                  This function is broken out from task_set_event so that
 *                  it's possible to trigger an event without also triggering
 *                  a scheduling point.
 *                  It is intended that this function be used from within
 *                  interrupts, or where it is desirable to remain in the
 *                  current context for some reason.
 *****************************************************************************/
extern bool_t
scheduler_task_set_event_no_reschedule( const task_priority_level_t p_event );

/**************************************************************************//**
 *
 *  @brief          Return the status of the specified task
 *
 *  @param[in]      p_level - The priority level of the task for which the
 *                  status is required
 *
 *  @return         The status of the specified task
 *
 *****************************************************************************/
extern task_status_t
scheduler_get_task_status( const task_priority_level_t p_level );

/**************************************************************************//**
 *
 *  @brief          Return the priority level of the currently executing
 *                   task
 *
 *  @return         The priority level of the currently executing task
 *
 *****************************************************************************/
extern task_priority_level_t
scheduler_get_current_task( void );

/**************************************************************************//**
 *
 *  @brief          Return the duration of the last execution of the specified
 *                  task
 *
 *  @param[in]      p_level - The priority level of the task for which the
 *                  duration is required
 *
 *  @return         The duration of the last execution of the specified task.
 *                  Will be set to zero if the task has not yet executed.
 *                  The relationship between the value returned and SI time
 *                  units is dependent upon the configuration of the timer
 *                  module.  See ts.h
 *                  THIS FUNCTION WILL RETURN ZERO IF THE SCHEDULER HAS
 *                  NOT BEEN BUILT WITH THE SCHEDULER_ENABLE_TIMING_STATS
 *                  BUILD-TIME OPTION
 *
 *****************************************************************************/
extern ts_ticks_t
scheduler_get_last_duration( const task_priority_level_t p_level );

#endif      /* SCHEDULER_H */

