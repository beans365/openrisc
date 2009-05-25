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
 *  $Id: tasksuspend.c,v 1.2 2001-09-27 11:59:19 chris Exp $
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
 *  rtems_task_suspend
 *
 *  This directive will place the specified thread in the "suspended"
 *  state.  Note that the suspended state can be in addition to
 *  other waiting states.
 *
 *  Input parameters:
 *    id - thread id
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

rtems_status_code rtems_task_suspend(
  Objects_Id id
)
{
  register Thread_Control *the_thread;
  Objects_Locations               location;

  the_thread = _Thread_Get( id, &location );
  switch ( location ) {

    case OBJECTS_REMOTE:
#if defined(RTEMS_MULTIPROCESSING)
      return _RTEMS_tasks_MP_Send_request_packet(
        RTEMS_TASKS_MP_SUSPEND_REQUEST,
        id,
        0,          /* Not used */
        0,          /* Not used */
        0           /* Not used */
      );
#endif

    case OBJECTS_ERROR:
      return RTEMS_INVALID_ID;

    case OBJECTS_LOCAL:
      if ( !_States_Is_suspended( the_thread->current_state ) ) {
        _Thread_Suspend( the_thread );
        _Thread_Enable_dispatch();
        return RTEMS_SUCCESSFUL;
      }
      _Thread_Enable_dispatch();
      return RTEMS_ALREADY_SUSPENDED;
  }

  return RTEMS_INTERNAL_ERROR;   /* unreached - only to remove warnings */
}
