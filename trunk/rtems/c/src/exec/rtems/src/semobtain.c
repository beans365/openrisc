/*
 *  Semaphore Manager
 *
 *  DESCRIPTION:
 *
 *  This package is the implementation of the Semaphore Manager.
 *  This manager utilizes standard Dijkstra counting semaphores to provide
 *  synchronization and mutual exclusion capabilities.
 *
 *  Directives provided are:
 *
 *     + create a semaphore
 *     + get an ID of a semaphore
 *     + delete a semaphore
 *     + acquire a semaphore
 *     + release a semaphore
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: semobtain.c,v 1.2 2001-09-27 11:59:19 chris Exp $
 */

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/rtems/attr.h>
#include <rtems/score/isr.h>
#include <rtems/score/object.h>
#include <rtems/rtems/options.h>
#include <rtems/rtems/sem.h>
#include <rtems/score/coremutex.h>
#include <rtems/score/coresem.h>
#include <rtems/score/states.h>
#include <rtems/score/thread.h>
#include <rtems/score/threadq.h>
#if defined(RTEMS_MULTIPROCESSING)
#include <rtems/score/mpci.h>
#endif
#include <rtems/score/sysstate.h>

#include <rtems/score/interr.h>

/*PAGE
 *
 *  rtems_semaphore_obtain
 *
 *  This directive allows a thread to acquire a semaphore.
 *
 *  Input parameters:
 *    id         - semaphore id
 *    option_set - wait option
 *    timeout    - number of ticks to wait (0 means wait forever)
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

rtems_status_code rtems_semaphore_obtain(
  Objects_Id      id,
  unsigned32      option_set,
  rtems_interval  timeout
)
{
  register Semaphore_Control *the_semaphore;
  Objects_Locations           location;
  boolean                     wait;

  the_semaphore = _Semaphore_Get( id, &location );
  switch ( location ) {
    case OBJECTS_REMOTE:
#if defined(RTEMS_MULTIPROCESSING)
      return _Semaphore_MP_Send_request_packet(
          SEMAPHORE_MP_OBTAIN_REQUEST,
          id,
          option_set,
          timeout
      );
#endif

    case OBJECTS_ERROR:
      return RTEMS_INVALID_ID;

    case OBJECTS_LOCAL:
      if ( _Options_Is_no_wait( option_set ) )
        wait = FALSE;
      else
        wait = TRUE;

      if ( !_Attributes_Is_counting_semaphore(the_semaphore->attribute_set) ) {
        _CORE_mutex_Seize(
          &the_semaphore->Core_control.mutex,
          id, 
          wait,
          timeout
        );
        _Thread_Enable_dispatch();
        return _Semaphore_Translate_core_mutex_return_code( 
                  _Thread_Executing->Wait.return_code );
      } else {
        _CORE_semaphore_Seize(
          &the_semaphore->Core_control.semaphore,
          id,
          wait,
          timeout
        );
        _Thread_Enable_dispatch();
        return _Semaphore_Translate_core_semaphore_return_code( 
                  _Thread_Executing->Wait.return_code );
      }
  }

  return RTEMS_INTERNAL_ERROR;   /* unreached - only to remove warnings */
}
