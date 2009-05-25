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
 *  $Id: pthread24.c,v 1.2 2001-09-27 12:02:25 chris Exp $
 */

#include <pthread.h>
#include <limits.h>  /* only for PTHREAD_STACK_MIN */

#ifndef _POSIX_THREADS
#error "rtems is supposed to have pthread_setstackaddr"
#endif

#ifndef _POSIX_THREAD_ATTR_STACKADDR
#error "rtems is supposed to have pthread_setstackaddr"
#endif

void test( void )
{
  pthread_attr_t  attribute;
  void           *stackaddr;
  int             result;

  stackaddr = NULL;

  result = pthread_attr_setstackaddr( &attribute, stackaddr );
}
