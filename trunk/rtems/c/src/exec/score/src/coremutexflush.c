/*
 *  Mutex Handler
 *
 *  DESCRIPTION:
 *
 *  This package is the implementation of the Mutex Handler.
 *  This handler provides synchronization and mutual exclusion capabilities.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: coremutexflush.c,v 1.2 2001-09-27 11:59:34 chris Exp $
 */

#include <rtems/system.h>
#include <rtems/score/isr.h>
#include <rtems/score/coremutex.h>
#include <rtems/score/states.h>
#include <rtems/score/thread.h>
#include <rtems/score/threadq.h>

/*PAGE
 *
 *  _CORE_mutex_Flush
 *
 *  This function a flushes the mutex's task wait queue.
 *
 *  Input parameters:
 *    the_mutex              - the mutex to be flushed
 *    remote_extract_callout - function to invoke remotely
 *    status                 - status to pass to thread
 *
 *  Output parameters:  NONE
 */

void _CORE_mutex_Flush(
  CORE_mutex_Control         *the_mutex,
  Thread_queue_Flush_callout  remote_extract_callout,
  unsigned32                  status
)
{
  _Thread_queue_Flush(
    &the_mutex->Wait_queue,
    remote_extract_callout,
    status
  );
}
