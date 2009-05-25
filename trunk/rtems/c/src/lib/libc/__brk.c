/*
 *  RTEMS "Broken" __brk Implementation
 *
 *  NOTE: sbrk() is provided by each BSP.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: __brk.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <rtems.h>
#if !defined(RTEMS_UNIX)

#include <errno.h>

int __brk(
  const void *endds
)
{
  errno = EINVAL;
  return -1;
}
#endif
