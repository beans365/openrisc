/*
 *  write() - POSIX 1003.1b 6.4.2 - Write to a File
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: write.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include "libio_.h"


/*
 * write
 * 
 * This routine writes count bytes from from buffer pointed to by buffer
 * to the file associated with the open file descriptor, fildes.
 */

ssize_t write(
  int         fd,
  const void *buffer,
  size_t      count
)
{
  rtems_status_code  rc;
  rtems_libio_t     *iop;

  rtems_libio_check_fd( fd );
  iop = rtems_libio_iop( fd );
  rtems_libio_check_is_open(iop);
  rtems_libio_check_buffer( buffer );
  rtems_libio_check_count( count );
  rtems_libio_check_permissions( iop, LIBIO_FLAGS_WRITE );

  /*
   *  Now process the write() request.
   */

  if ( !iop->handlers->write )
    set_errno_and_return_minus_one( ENOTSUP );

  rc = (*iop->handlers->write)( iop, buffer, count );

  if ( rc > 0 )
    iop->offset += rc;

  return rc;
}

/*
 *  _write_r
 *
 *  This is the Newlib dependent reentrant version of write().
 */

#if defined(RTEMS_NEWLIB)

#include <reent.h>

long _write_r(
  struct _reent *ptr,
  int            fd,
  const void    *buf,
  size_t         nbytes
)
{
  return write( fd, buf, nbytes );
}
#endif
