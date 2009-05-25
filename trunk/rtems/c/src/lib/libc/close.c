/*
 *  close() - POSIX 1003.1b 6.3.1 - Close a File
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: close.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include "libio_.h"

int close(
  int  fd
)
{
  rtems_libio_t      *iop;
  rtems_status_code   rc;

  rtems_libio_check_fd(fd);
  iop = rtems_libio_iop(fd);
  rtems_libio_check_is_open(iop);

  rc = RTEMS_SUCCESSFUL;
  if ( iop->handlers->close )
    rc = (*iop->handlers->close)( iop );

  rtems_libio_free( iop );

  return rc;
}

/*
 *  _close_r
 *
 *  This is the Newlib dependent reentrant version of close().
 */

#if defined(RTEMS_NEWLIB)

#include <reent.h>

int _close_r(
  struct _reent *ptr,
  int            fd
)
{
  return close( fd );
}
#endif
