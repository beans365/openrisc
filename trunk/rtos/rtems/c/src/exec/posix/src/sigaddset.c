/*
 *  3.3.3 Manipulate Signal Sets, P1003.1b-1993, p. 69
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: sigaddset.c,v 1.2 2001-09-27 11:59:17 chris Exp $
 */


#include <pthread.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/posix/pthread.h>
#include <rtems/posix/psignal.h>
#include <rtems/posix/seterr.h>

int sigaddset(
  sigset_t   *set,
  int         signo
)
{
  if ( !set )
    set_errno_and_return_minus_one( EINVAL );

  if ( !signo )
    return 0;

  if ( !is_valid_signo(signo) )
    set_errno_and_return_minus_one( EINVAL );

  *set |= signo_to_mask(signo);
  return 0;
}
