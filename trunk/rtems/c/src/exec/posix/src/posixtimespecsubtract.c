/*
 *  $Id: posixtimespecsubtract.c,v 1.2 2001-09-27 11:59:17 chris Exp $
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
 *  _POSIX_Timespec_subtract
 */

void _POSIX_Timespec_subtract(
  const struct timespec *the_start,
  const struct timespec *end,
  struct timespec *result
)
{
  struct timespec  start_struct = *the_start;
  struct timespec *start = &start_struct;
  unsigned int nsecs_per_sec = TOD_NANOSECONDS_PER_SECOND;
 
  if (end->tv_nsec < start->tv_nsec) {
    int seconds = (start->tv_nsec - end->tv_nsec) / nsecs_per_sec + 1;
    start->tv_nsec -= nsecs_per_sec * seconds;
    start->tv_sec += seconds;
  }
 
  if (end->tv_nsec - start->tv_nsec > nsecs_per_sec) {
    int seconds = (start->tv_nsec - end->tv_nsec) / nsecs_per_sec;
    start->tv_nsec += nsecs_per_sec * seconds;
    start->tv_sec -= seconds;
  }
 
  result->tv_sec  = end->tv_sec - start->tv_sec;
  result->tv_nsec = end->tv_nsec - start->tv_nsec;
}
