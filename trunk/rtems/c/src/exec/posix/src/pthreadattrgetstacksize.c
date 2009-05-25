/*
 *  16.1.1 Thread Creation Attributes, P1003.1c/Draft 10, p, 140
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: pthreadattrgetstacksize.c,v 1.2 2001-09-27 11:59:17 chris Exp $
 */


#include <pthread.h>
#include <errno.h>

int pthread_attr_getstacksize(
  const pthread_attr_t  *attr,
  size_t                *stacksize
)
{
  if ( !attr || !attr->is_initialized || !stacksize )
    return EINVAL;

  *stacksize = attr->stacksize;
  return 0;
}
