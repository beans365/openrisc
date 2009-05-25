/*
 *  rtems_task_variable_delete - Delete a per-task variable
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: taskvariabledelete.c,v 1.2 2001-09-27 11:59:19 chris Exp $
 */

#include <rtems/system.h>
#include <rtems/rtems/tasks.h>
#include <rtems/score/wkspace.h>

/*
 *  rtems_task_variable_delete
 *
 *  This directive removes a task variable. 
 */

rtems_status_code rtems_task_variable_delete(
  rtems_id  tid,
  void    **ptr
)
{
  Thread_Control        *the_thread;
  Objects_Locations      location;
  rtems_task_variable_t *tvp, *prev;

  prev = NULL;

  the_thread = _Thread_Get (tid, &location);
  switch (location) {
  case OBJECTS_REMOTE:
#if defined(RTEMS_MULTIPROCESSING)
    _Thread_Dispatch();
    return RTEMS_ILLEGAL_ON_REMOTE_OBJECT;
#endif

  case OBJECTS_ERROR:
    return RTEMS_INVALID_ID;

  default:
    return RTEMS_INTERNAL_ERROR;

  case OBJECTS_LOCAL:
    tvp = the_thread->task_variables;
    while (tvp) {
      if (tvp->ptr == ptr) {
        if (prev) prev->next = tvp->next;
        else      the_thread->task_variables = tvp->next;
	if (_Thread_Is_executing (the_thread))
          *tvp->ptr = tvp->gval;
        _Thread_Enable_dispatch();
        _Workspace_Free(tvp);
        return RTEMS_SUCCESSFUL;
      }
      prev = tvp;
      tvp = tvp->next;
    }
    _Thread_Enable_dispatch();
    return RTEMS_INVALID_ADDRESS;
  }

  return RTEMS_INTERNAL_ERROR;   /* unreached - only to remove warnings */
}
