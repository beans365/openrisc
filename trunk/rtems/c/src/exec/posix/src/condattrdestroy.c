/*
 *  $Id: condattrdestroy.c,v 1.2 2001-09-27 11:59:17 chris Exp $
 */

#include <pthread.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/score/object.h>
#include <rtems/score/states.h>
#include <rtems/score/watchdog.h>
#include <rtems/posix/cond.h>
#include <rtems/posix/time.h>
#include <rtems/posix/mutex.h>

/*PAGE
 *
 *  11.4.1 Condition Variable Initialization Attributes, 
 *            P1003.1c/Draft 10, p. 96
 */
 
int pthread_condattr_destroy(
  pthread_condattr_t *attr
)
{
  if ( !attr || attr->is_initialized == FALSE )
    return EINVAL;

  attr->is_initialized = FALSE;
  return 0;
}
