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
 *  $Id: timer01.c,v 1.2 2001-09-27 12:02:25 chris Exp $
 */

#include <time.h>
 
#ifndef _POSIX_TIMERS
#error "rtems is supposed to have timer_create"
#endif

void test( void )
{
  clockid_t        clock_id = 0;
  timer_t          timerid = 0;
  struct sigevent  evp;
  int              result;

  evp.sigev_notify = SIGEV_NONE;
  evp.sigev_notify = SIGEV_SIGNAL;
  evp.sigev_notify = SIGEV_THREAD;
  evp.sigev_signo = SIGALRM;
  evp.sigev_value.sival_int = 0;
  evp.sigev_value.sival_ptr = NULL;
  
  result = timer_create( clock_id, &evp, &timerid );
}
