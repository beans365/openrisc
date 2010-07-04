/*
 *  Routines to access a host errno
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: hosterr.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <rtems.h>
#if defined(RTEMS_UNIX)
#include <errno.h>

int host_errno( void );

/*
 *  fix_syscall_errno
 *
 *  copy host errno, if any to thread aware errno, if any
 */

void fix_syscall_errno( void )
{
  errno = host_errno();
}

/*
 *  host_errno
 *
 *  Get the host system errno, if any
 *  When using newlib (or possibly other libc's) on top of UNIX
 *  the errno returned by system calls may be unavailable due
 *  to trickery of making errno thread aware.
 *  This provides a kludge of getting at it.
 */

#undef errno
extern int errno;
int host_errno(void)
{
  return errno;
}
#endif
