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
 *  $Id: time13.c,v 1.2 2001-09-27 12:02:25 chris Exp $
 */

#include <time.h>
 
void test( void )
{
  time_t     time;
  struct tm *timestruct_pointer;
  struct tm  timestruct;
  
  timestruct_pointer = localtime_r( &time, &timestruct );
}
