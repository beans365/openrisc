/*
 *  $Id: semaphorenametoid.c,v 1.2 2001-09-27 11:59:17 chris Exp $
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
 *  _POSIX_Semaphore_Name_to_id
 *
 *  Look up the specified name and attempt to locate the id
 *  for the associated semaphore.
 */

int _POSIX_Semaphore_Name_to_id(
  const char          *name,
  Objects_Id          *id
)
{
  Objects_Name_to_id_errors  status;

   if ( !name )
     return EINVAL;

  if ( !name[0] )
    return EINVAL;

  status = _Objects_Name_to_id( 
    &_POSIX_Semaphore_Information, (char *)name, 0, id );

  if ( status == OBJECTS_SUCCESSFUL )
    return 0;

  return ENOENT;
}

