/*
 *  16.1.4 Detaching a Thread, P1003.1c/Draft 10, p. 149
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: pthreaddetach.c,v 1.2 2001-09-27 11:59:17 chris Exp $
 */


#include <pthread.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/score/thread.h>
#include <rtems/posix/pthread.h>

int pthread_detach(
  pthread_t   thread
)
{
  register Thread_Control *the_thread;
  POSIX_API_Control       *api;
  Objects_Locations        location;

  the_thread = _POSIX_Threads_Get( thread, &location );
  switch ( location ) {
    case OBJECTS_ERROR:
    case OBJECTS_REMOTE:
      return ESRCH;
    case OBJECTS_LOCAL:

      api = the_thread->API_Extensions[ THREAD_API_POSIX ];
      api->detachstate = PTHREAD_CREATE_DETACHED;
      _Thread_Enable_dispatch();
      return 0;
  }

  return POSIX_BOTTOM_REACHED();
}

