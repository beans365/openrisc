/*
 *  pipe() - POSIX 1003.1b 6.1.1 Create an Inter-Process Channel
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: pipe.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <errno.h>

int pipe(
  int filsdes[2]
)
{
  errno = ENOSYS;
  return -1;
}
