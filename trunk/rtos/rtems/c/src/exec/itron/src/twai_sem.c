/*
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: twai_sem.c,v 1.2 2001-09-27 11:59:13 chris Exp $
 */

#include <itron.h>

#include <rtems/itron/semaphore.h>
#include <rtems/itron/task.h>
#include <rtems/score/tod.h>

/*
 *  twai_sem - Wait on Semaphore with Timeout
 *
 *  This function implements the ITRON 3.0 twai_sem() service.
 */

ER twai_sem(
  ID semid,
  TMO tmout
)
{
  ITRON_Semaphore_Control *the_semaphore;
  Objects_Locations        location;
  Watchdog_Interval        interval;
  boolean                  wait;
  CORE_semaphore_Status    status;
  
  interval = 0;
  if ( tmout == TMO_POL ) {
    wait = FALSE;
  } else {
    wait = TRUE;
    if ( tmout != TMO_FEVR )
      interval = TOD_MILLISECONDS_TO_TICKS(tmout);
  }

  if ( wait && _ITRON_Is_in_non_task_state() )
    return E_CTX;
  
  the_semaphore = _ITRON_Semaphore_Get( semid, &location );
  switch ( location ) {
    case OBJECTS_REMOTE:               /* Multiprocessing not supported */
    case OBJECTS_ERROR:
      return _ITRON_Semaphore_Clarify_get_id_error( semid );

    case OBJECTS_LOCAL:
      _CORE_semaphore_Seize(
        &the_semaphore->semaphore,
        the_semaphore->Object.id,
        wait,                           /* wait for a timeout */
        interval                        /* timeout value */
      );
      _Thread_Enable_dispatch();
      status = (CORE_semaphore_Status) _Thread_Executing->Wait.return_code;
      return _ITRON_Semaphore_Translate_core_semaphore_return_code( status );
  }
  return E_OK;
}
