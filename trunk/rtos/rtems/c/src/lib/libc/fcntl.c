/*
 *   fcntl() - POSIX 1003.1b 6.5.2 - File Control
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: fcntl.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <rtems.h>
#include "libio_.h"

int fcntl(
  int fd,
  int cmd,
  ...
)
{
  va_list        ap;
  rtems_libio_t *iop;
  rtems_libio_t *diop;
  int            fd2;
  int            flags;
  int            mask;
  int            ret = 0;
  
  va_start( ap, cmd );

  rtems_libio_check_fd( fd );
  iop = rtems_libio_iop( fd );
  rtems_libio_check_is_open(iop);

  /*
   *  Now process the fcntl().
   */

  /*
   *  This switch should contain all the cases from POSIX.
   */

  switch ( cmd ) {
    case F_DUPFD:        /* dup */
      fd2 = va_arg( ap, int );
      if ( fd2 )
        diop = rtems_libio_iop( fd2 );
      else {
        /* allocate a file control block */
        diop = rtems_libio_allocate();
        if ( diop == 0 ) {
          ret = -1;
          break;
        }
      }

      diop->handlers   = iop->handlers;
      diop->file_info  = iop->file_info;
      diop->flags      = iop->flags;
      diop->pathinfo   = iop->pathinfo;
      ret = (int) (diop - rtems_libio_iops);
      break;

    case F_GETFD:        /* get f_flags */
      ret = ((iop->flags & LIBIO_FLAGS_CLOSE_ON_EXEC) != 0);
      break;

    case F_SETFD:        /* set f_flags */
      /*
       *  Interpret the third argument as the "close on exec()" flag.
       *  If this argument is 1, then the file descriptor is to be closed
       *  if a new process is exec()'ed.  Since RTEMS does not support
       *  processes, then we can ignore this one except to make 
       *  F_GETFD work.
       */

      if ( va_arg( ap, int ) )
        iop->flags |= LIBIO_FLAGS_CLOSE_ON_EXEC;
      else
        iop->flags &= ~LIBIO_FLAGS_CLOSE_ON_EXEC;
      break;

    case F_GETFL:        /* more flags (cloexec) */
      ret = rtems_libio_to_fcntl_flags( iop->flags );
      break;

    case F_SETFL:
      flags = rtems_libio_fcntl_flags( va_arg( ap, int ) );
      mask = LIBIO_FLAGS_NO_DELAY | LIBIO_FLAGS_APPEND;

      /*
       *  XXX If we are turning on append, should we seek to the end?
       */

      iop->flags = (iop->flags & ~mask) | (flags & mask);
      break;

    case F_GETLK:
      errno = ENOTSUP;
      ret = -1;
      break;

    case F_SETLK:
      errno = ENOTSUP;
      ret = -1;
      break;

    case F_SETLKW:
      errno = ENOTSUP;
      ret = -1;
      break;

    case F_SETOWN:       /*  for sockets. */
      errno = ENOTSUP;
      ret = -1;
      break;

    case F_GETOWN:       /*  for sockets. */
      errno = ENOTSUP;
      ret = -1;
      break;

    default:
      errno = EINVAL;
      ret = -1;
      break;
  }
  if ((ret >= 0) && iop->handlers->fcntl) {
    int err = (*iop->handlers->fcntl)( cmd, iop );
    if (err) {
      errno = err;
      ret = -1;
    }
  }
  return ret;
}
