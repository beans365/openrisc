/*
 *  3.3.9 Queue a Signal to a Process, P1003.1b-1993, p. 78
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: sigqueue.c,v 1.2 2001-09-27 11:59:17 chris Exp $
 */


#include <pthread.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/posix/pthread.h>
#include <rtems/posix/psignal.h>

int sigqueue(
  pid_t               pid,
  int                 signo,
  const union sigval  value
)
{
  return killinfo( pid, signo, &value );
}

