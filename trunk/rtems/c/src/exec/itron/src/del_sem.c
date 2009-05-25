/*
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: del_sem.c,v 1.2 2001-09-27 11:59:13 chris Exp $
 */

#include <itron.h>

#include <rtems/itron/semaphore.h>
#include <rtems/itron/task.h>
#include <rtems/score/tod.h>

/*
 *  del_sem - Delete Semaphore
 *
 *  This function implements the ITRON 3.0 del_sem() service.
 */

ER del_sem(
  ID semid
)
{
  ITRON_Semaphore_Control *the_semaphore;
  Objects_Locations        location;
  
  the_semaphore = _ITRON_Semaphore_Get( semid, &location );
  switch ( location ) {
    case OBJECTS_REMOTE:               /* Multiprocessing not supported */
    case OBJECTS_ERROR:
      return _ITRON_Semaphore_Clarify_get_id_error( semid );
 
    case OBJECTS_LOCAL:
      _CORE_semaphore_Flush(
        &the_semaphore->semaphore,
        NULL,                          /* Multiprocessing not supported */
        CORE_SEMAPHORE_WAS_DELETED
      );

      _ITRON_Objects_Close(
        &_ITRON_Semaphore_Information,
        &the_semaphore->Object
      );

      _ITRON_Semaphore_Free( the_semaphore );

  /*
   *  If multiprocessing were supported, this is where we would announce
   *  the destruction of the semaphore to the rest of the system.
   */

#if defined(RTEMS_MULTIPROCESSING)
#endif

    _Thread_Enable_dispatch();
    return E_OK;

  }
  return E_OK;
}
