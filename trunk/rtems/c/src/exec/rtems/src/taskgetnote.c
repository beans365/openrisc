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
 *  $Id: taskgetnote.c,v 1.2 2001-09-27 11:59:19 chris Exp $
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
 *  rtems_task_get_note
 *
 *  This directive obtains the note from the specified notepad
 *  of the specified thread.
 *
 *  Input parameters:
 *    id      - thread id
 *    notepad - notepad number
 *    note    - pointer to note
 *
 *  Output parameters:
 *    note              - filled in if successful
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

rtems_status_code rtems_task_get_note(
  Objects_Id  id,
  unsigned32  notepad,
  unsigned32 *note
)
{
  register Thread_Control *the_thread;
  Objects_Locations        location;
  RTEMS_API_Control       *api;

  /*
   *  NOTE:  There is no check for < RTEMS_NOTEPAD_FIRST because that would
   *         be checking an unsigned number for being negative.
   */

  if ( notepad > RTEMS_NOTEPAD_LAST )
    return RTEMS_INVALID_NUMBER;

  /*
   *  Optimize the most likely case to avoid the Thread_Dispatch.
   */

  if ( _Objects_Are_ids_equal( id, OBJECTS_ID_OF_SELF ) ||
       _Objects_Are_ids_equal( id, _Thread_Executing->Object.id ) ) {
      api = _Thread_Executing->API_Extensions[ THREAD_API_RTEMS ];
      *note = api->Notepads[ notepad ];
      return RTEMS_SUCCESSFUL;
  }

  the_thread = _Thread_Get( id, &location );
  switch ( location ) {

    case OBJECTS_REMOTE:
#if defined(RTEMS_MULTIPROCESSING)
      _Thread_Executing->Wait.return_argument = note;

      return _RTEMS_tasks_MP_Send_request_packet(
        RTEMS_TASKS_MP_GET_NOTE_REQUEST,
        id,
        0,          /* Not used */
        notepad,
        0           /* Not used */
      );
#endif

    case OBJECTS_ERROR:
      return RTEMS_INVALID_ID;

    case OBJECTS_LOCAL:
      api = the_thread->API_Extensions[ THREAD_API_RTEMS ];
      *note = api->Notepads[ notepad ];
      _Thread_Enable_dispatch();
      return RTEMS_SUCCESSFUL;
  }

  return RTEMS_INTERNAL_ERROR;   /* unreached - only to remove warnings */
}
