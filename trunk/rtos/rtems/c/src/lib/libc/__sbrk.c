/*
 *  RTEMS "Broken" __sbrk Implementation
 *
 *  NOTE: sbrk is provided by the BSP.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: __sbrk.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <rtems.h>
#if !defined(RTEMS_UNIX)

#include <errno.h>

void * __sbrk(
  int incr
)
{
  errno = EINVAL;
  return (void *)0;
}
#endif
