/*
 *  $Id: clockgetres.c,v 1.2 2001-09-27 11:59:17 chris Exp $
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
 *  14.2.1 Clocks, P1003.1b-1993, p. 263
 */

int clock_getres(
  clockid_t        clock_id,
  struct timespec *res
)
{
  if ( !res )
    set_errno_and_return_minus_one( EINVAL );
 
  switch ( clock_id ) {
 
    /*
     *  All time in rtems is based on the same clock tick.
     */

    case CLOCK_REALTIME:
    case CLOCK_PROCESS_CPUTIME:
    case CLOCK_THREAD_CPUTIME:
      if ( res )
        _POSIX_Interval_to_timespec( _TOD_Microseconds_per_tick, res ); 
      break;
 
    default:
      set_errno_and_return_minus_one( EINVAL );
 
  }
  return 0;
}
