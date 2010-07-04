/*
 *  Marginal implementations of some POSIX API routines
 *  to be used when POSIX is disabled.
 *
 *    + getpid
 *    + _getpid_r
 *    + kill
 *    + _kill_r
 *    + __kill
 *    + sleep
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: no_posix.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <rtems.h>

#include <unistd.h>

/*
 *  These are directly supported (and completely correct) in the posix api.
 */

#if !defined(RTEMS_POSIX_API)
pid_t getpid(void)
{
  return 0;
}

#if defined(RTEMS_NEWLIB)
#include <sys/reent.h>

pid_t _getpid_r(
  struct _reent *ptr
)
{
  return getpid();
}
#endif

#endif

#if !defined(RTEMS_POSIX_API)
int kill( pid_t pid, int sig )
{
  return 0;
}

int _kill_r( pid_t pid, int sig )
{
  return 0;
}
#endif

int __kill( pid_t pid, int sig )
{
  return 0;
}


/*
 *  3.4.3 Delay Process Execution, P1003.1b-1993, p. 81
 *
 *  $Id: no_posix.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */ 

#include <time.h>
#include <unistd.h> 

#include <rtems.h>

#if !defined(RTEMS_POSIX_API)
unsigned int sleep(
  unsigned int seconds
)
{
  rtems_status_code status;
  rtems_interval    ticks_per_second;
  rtems_interval    ticks;

  status = rtems_clock_get(
  RTEMS_CLOCK_GET_TICKS_PER_SECOND,
  &ticks_per_second
  );

  ticks = seconds * ticks_per_second;

  status = rtems_task_wake_after( ticks );

  /*
   *  Returns the "unslept" amount of time.  In RTEMS signals are not
   *  interruptable, so tasks really sleep all of the requested time.
   */

  return 0;
}
#endif

