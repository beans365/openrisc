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
 *  $Id: timer06.c,v 1.2 2001-09-27 12:02:25 chris Exp $
 */

#include <time.h>
 
#ifndef _POSIX_TIMERS
#error "rtems is supposed to have nanosleep"
#endif

void test( void )
{
  struct timespec rqtp; 
  struct timespec rmtp; 
  int             result;

  rqtp.tv_sec = 0;
  rqtp.tv_nsec = 0;
  result = nanosleep( &rqtp, &rmtp );
}
