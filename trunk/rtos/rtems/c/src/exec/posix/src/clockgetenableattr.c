/*
 *  $Id: clockgetenableattr.c,v 1.2 2001-09-27 11:59:17 chris Exp $
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
 *  20.1.5 CPU-time Clock Attribute Access, P1003.4b/D8, p. 58
 */

int clock_getenable_attr(
  clockid_t    clock_id,
  int         *attr
)
{
  return POSIX_NOT_IMPLEMENTED();
}
