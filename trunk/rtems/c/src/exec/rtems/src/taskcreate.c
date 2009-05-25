/*
 *  RTEMS Task Manager
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: taskcreate.c,v 1.2 2001-09-27 11:59:19 chris Exp $
 */

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/rtems/modes.h>
#include <rtems/score/object.h>
#include <rtems/score/stack.h>
#include <rtems/score/states.h>
#include <rtems/rtems/tasks.h>
#include <rtems/score/thread.h>
#include <rtems/score/threadq.h>
#include <rtems/score/tod.h>
#include <rtems/score/userext.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/apiext.h>
#include <rtems/score/sysstate.h>

/*PAGE
 *
 *  rtems_task_create
 *
 *  This directive creates a thread by allocating and initializing a
 *  thread control block and a stack.  The newly created thread is
 *  placed in the dormant state.
 *
 *  Input parameters:
 *    name             - user defined thread name
 *    initial_priority - thread priority
 *    stack_size       - stack size in bytes
 *    initial_modes    - initial thread mode
 *    attribute_set    - thread attributes
 *    id               - pointer to thread id
 *
 *  Output parameters:
 *    id                - thread id
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

rtems_status_code rtems_task_create(
  rtems_name           name,
  rtems_task_priority  initial_priority,
  unsigned32           stack_size,
  rtems_mode           initial_modes,
  rtems_attribute      attribute_set,
  Objects_Id          *id
)
{
  register Thread_Control *the_thread;
  boolean                  is_fp;
#if defined(RTEMS_MULTIPROCESSING)
  Objects_MP_Control      *the_global_object = NULL;
  boolean                  is_global;
#endif
  boolean                  status;
  rtems_attribute          the_attribute_set;
  Priority_Control         core_priority;
  RTEMS_API_Control       *api;
  ASR_Information         *asr;
 

  if ( !rtems_is_name_valid( name ) )
    return RTEMS_INVALID_NAME;

  /* 
   *  Core Thread Initialize insures we get the minimum amount of
   *  stack space.
   */

#if 0
  if ( !_Stack_Is_enough( stack_size ) )
    return RTEMS_INVALID_SIZE;
#endif

  /*
   *  Fix the attribute set to match the attributes which
   *  this processor (1) requires and (2) is able to support.
   *  First add in the required flags for attribute_set
   *  Typically this might include FP if the platform
   *  or application required all tasks to be fp aware.
   *  Then turn off the requested bits which are not supported.
   */

  the_attribute_set = _Attributes_Set( attribute_set, ATTRIBUTES_REQUIRED );
  the_attribute_set =
    _Attributes_Clear( the_attribute_set, ATTRIBUTES_NOT_SUPPORTED );

  if ( _Attributes_Is_floating_point( the_attribute_set ) )
    is_fp = TRUE;
  else
    is_fp = FALSE;

  /*
   *  Validate the RTEMS API priority and convert it to the core priority range.
   */

  if ( !_Attributes_Is_system_task( the_attribute_set ) ) {
    if ( !_RTEMS_tasks_Priority_is_valid( initial_priority ) )
      return RTEMS_INVALID_PRIORITY;
  }

  core_priority = _RTEMS_tasks_Priority_to_Core( initial_priority );

#if defined(RTEMS_MULTIPROCESSING)
  if ( _Attributes_Is_global( the_attribute_set ) ) {

    is_global = TRUE;

    if ( !_System_state_Is_multiprocessing )
      return RTEMS_MP_NOT_CONFIGURED;

  } else
    is_global = FALSE;
#endif

  /*
   *  Make sure system is MP if this task is global
   */

  /*
   *  Disable dispatch for protection
   */ 

  _Thread_Disable_dispatch();

  /*
   *  Allocate the thread control block and -- if the task is global --
   *  allocate a global object control block.
   *
   *  NOTE:  This routine does not use the combined allocate and open
   *         global object routine because this results in a lack of
   *         control over when memory is allocated and can be freed in
   *         the event of an error.
   */

  the_thread = _RTEMS_tasks_Allocate();

  if ( !the_thread ) {
    _Thread_Enable_dispatch();
    return RTEMS_TOO_MANY;
  }

#if defined(RTEMS_MULTIPROCESSING)
  if ( is_global ) {
    the_global_object = _Objects_MP_Allocate_global_object();

    if ( _Objects_MP_Is_null_global_object( the_global_object ) ) {
      _RTEMS_tasks_Free( the_thread );
      _Thread_Enable_dispatch();
      return RTEMS_TOO_MANY;
    }
  }
#endif

  /*
   *  Initialize the core thread for this task.
   */

  status = _Thread_Initialize(
    &_RTEMS_tasks_Information,
    the_thread, 
    NULL,
    stack_size,
    is_fp,
    core_priority,
    _Modes_Is_preempt(initial_modes)   ? TRUE : FALSE,
    _Modes_Is_timeslice(initial_modes) ?
      THREAD_CPU_BUDGET_ALGORITHM_RESET_TIMESLICE :
      THREAD_CPU_BUDGET_ALGORITHM_NONE,
    NULL,        /* no budget algorithm callout */
    _Modes_Get_interrupt_level(initial_modes),
    &name
  );

  if ( !status ) {
#if defined(RTEMS_MULTIPROCESSING)
    if ( is_global )
      _Objects_MP_Free_global_object( the_global_object );
#endif
    _RTEMS_tasks_Free( the_thread );
    _Thread_Enable_dispatch();
    return RTEMS_UNSATISFIED;
  }

  api = the_thread->API_Extensions[ THREAD_API_RTEMS ];
  asr = &api->Signal;
 
  asr->is_enabled = _Modes_Is_asr_disabled(initial_modes) ? FALSE : TRUE;

  *id = the_thread->Object.id;

#if defined(RTEMS_MULTIPROCESSING)
  if ( is_global ) {

    the_thread->is_global = TRUE;

    _Objects_MP_Open(
      &_RTEMS_tasks_Information,
      the_global_object,
      name,
      the_thread->Object.id
    );

    _RTEMS_tasks_MP_Send_process_packet(
      RTEMS_TASKS_MP_ANNOUNCE_CREATE,
      the_thread->Object.id,
      name
    );

   }
#endif

  _Thread_Enable_dispatch();
  return RTEMS_SUCCESSFUL;
}
