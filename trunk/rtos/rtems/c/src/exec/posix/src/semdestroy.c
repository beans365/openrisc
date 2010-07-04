/*
 *  $Id: semdestroy.c,v 1.2 2001-09-27 11:59:17 chris Exp $
 */

#include <stdarg.h>

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <limits.h>

#include <rtems/system.h>
#include <rtems/score/object.h>
#include <rtems/posix/semaphore.h>
#include <rtems/posix/time.h>
#include <rtems/posix/seterr.h>

/*PAGE
 *
 *  11.2.2 Destroy an Unnamed Semaphore, P1003.1b-1993, p.220
 */

int sem_destroy(
  sem_t *sem
)
{
  register POSIX_Semaphore_Control *the_semaphore;
  Objects_Locations                 location;
 
  the_semaphore = _POSIX_Semaphore_Get( sem, &location );
  switch ( location ) {
    case OBJECTS_ERROR:
      set_errno_and_return_minus_one( EINVAL );
    case OBJECTS_REMOTE:
      _Thread_Dispatch();
      return POSIX_MP_NOT_IMPLEMENTED();
      set_errno_and_return_minus_one( EINVAL );
    case OBJECTS_LOCAL:
      /*
       *  Undefined operation on a named semaphore.
       */

      if ( the_semaphore->named == TRUE ) {
        _Thread_Enable_dispatch();
        set_errno_and_return_minus_one( EINVAL );
      }
 
      _POSIX_Semaphore_Delete( the_semaphore );
      _Thread_Enable_dispatch();
      return 0;
  }
  return POSIX_BOTTOM_REACHED();
}
