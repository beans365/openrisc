/*
 *  $Id: clockgetcpuclockid.c,v 1.2 2001-09-27 11:59:17 chris Exp $
 */

#include <assert.h>
#include <time.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/score/isr.h>
#include <rtems/score/thread.h>
#include <rtems/score/tod.h>

#include <rtems/posix/seterr.h>
#include <rtems/posix/time.h>

/*PAGE
 *
 *  20.1.3 Accessing a Process CPU-time CLock, P1003.4b/D8, p. 55
 */

int clock_getcpuclockid(
  pid_t      pid,
  clockid_t *clock_id
)
{
  return POSIX_NOT_IMPLEMENTED();
}
