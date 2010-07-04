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
 *  $Id: taskstart.c,v 1.2 2001-09-27 11:59:19 chris Exp $
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
 *  rtems_task_start
 *
 *  This directive readies the thread identified by the "id"
 *  based on its current priorty, to await execution.  A thread
 *  can be started only from the dormant state.
 *
 *  Input parameters:
 *    id          - thread id
 *    entry_point - start execution address of thread
 *    argument    - thread argument
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

rtems_status_code rtems_task_start(
  rtems_id         id,
  rtems_task_entry entry_point,
  unsigned32       argument
)
{
  register Thread_Control *the_thread;
  Objects_Locations        location;

  if ( entry_point == NULL )
    return RTEMS_INVALID_ADDRESS;

  the_thread = _Thread_Get( id, &location );
  switch ( location ) {

    case OBJECTS_REMOTE:
#if defined(RTEMS_MULTIPROCESSING)
      _Thread_Dispatch();
      return RTEMS_ILLEGAL_ON_REMOTE_OBJECT;
#endif

    case OBJECTS_ERROR:
      return RTEMS_INVALID_ID;

    case OBJECTS_LOCAL:
      if ( _Thread_Start(
             the_thread, THREAD_START_NUMERIC, entry_point, NULL, argument ) ) {
        _Thread_Enable_dispatch();
        return RTEMS_SUCCESSFUL;
      }
      _Thread_Enable_dispatch();
      return RTEMS_INCORRECT_STATE;
  }

  return RTEMS_INTERNAL_ERROR;   /* unreached - only to remove warnings */
}
