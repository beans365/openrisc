/*
 *  3.3.2 Send a Signal to a Process, P1003.1b-1993, p. 68
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: kill.c,v 1.2 2001-09-27 11:59:17 chris Exp $
 */


#include <pthread.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/posix/pthread.h>
#include <rtems/posix/psignal.h>

int kill(
  pid_t pid,
  int   sig
)
{
  return killinfo( pid, sig, NULL );
}

/*
 *  _kill_r
 *
 *  This is the Newlib dependent reentrant version of kill().
 */

#if defined(RTEMS_NEWLIB)

#include <reent.h>

int _kill_r(
  struct _reent *ptr,
  pid_t          pid,
  int            sig
)
{
  return kill( pid, sig );
}
#endif

