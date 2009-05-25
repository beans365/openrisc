/*
 *  3.4.2 Suspend Process Execution, P1003.1b-1993, p. 81
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: pause.c,v 1.2 2001-09-27 11:59:17 chris Exp $
 */


#include <pthread.h>
#include <signal.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/posix/pthread.h>
#include <rtems/posix/psignal.h>

/*
 *  3.4.2 Suspend Process Execution, P1003.1b-1993, p. 81
 */

int pause( void )
{
  sigset_t  all_signals;
  int       status;

  (void) sigfillset( &all_signals );

  status = sigtimedwait( &all_signals, NULL, NULL );

  return status;
}

