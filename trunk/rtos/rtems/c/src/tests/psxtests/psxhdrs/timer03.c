/*
 *  This test file is used to verify that the header files associated with
 *  invoking this function are correct.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: timer03.c,v 1.2 2001-09-27 12:02:25 chris Exp $
 */

#include <time.h>
 
#ifndef _POSIX_TIMERS
#error "rtems is supposed to have timer_settime"
#endif

void test( void )
{
  timer_t           timerid = 0;
  int               flags;
  struct itimerspec value;
  struct itimerspec ovalue;
  int               result;

  value.it_interval.tv_sec = 0;
  value.it_interval.tv_nsec = 0;
  value.it_value.tv_sec = 0;
  value.it_value.tv_nsec = 0;

  flags = TIMER_ABSTIME;
  flags = CLOCK_REALTIME;

  result = timer_settime( timerid, flags, &value, &ovalue );
}
