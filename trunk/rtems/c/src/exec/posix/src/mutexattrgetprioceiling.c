/*
 *  $Id: mutexattrgetprioceiling.c,v 1.2 2001-09-27 11:59:17 chris Exp $
 */

#include <assert.h>
#include <errno.h>
#include <pthread.h>

#include <rtems/system.h>
#include <rtems/score/coremutex.h>
#include <rtems/score/watchdog.h>
#if defined(RTEMS_MULTIPROCESSING)
#include <rtems/score/mpci.h>
#endif
#include <rtems/posix/mutex.h>
#include <rtems/posix/priority.h>
#include <rtems/posix/time.h>

/*PAGE
 *
 *  13.6.1 Mutex Initialization Scheduling Attributes, P1003.1c/Draft 10, p. 128
 */
 
int pthread_mutexattr_getprioceiling(
  const pthread_mutexattr_t   *attr,
  int                         *prioceiling
)
{
  if ( !attr || !attr->is_initialized || !prioceiling )
    return EINVAL;

  *prioceiling = attr->prio_ceiling;
  return 0;
}
