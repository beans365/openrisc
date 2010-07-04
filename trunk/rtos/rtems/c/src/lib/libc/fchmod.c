/*
 *  fchmod() - POSIX 1003.1b 5.6.4 - Change File Modes
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: fchmod.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#include <rtems.h>
#include <rtems/libio.h>
#include "libio_.h"

int fchmod(
  int       fd,
  mode_t    mode
)
{
  rtems_libio_t *iop;
  
  rtems_libio_check_fd( fd );
  iop = rtems_libio_iop( fd );
  rtems_libio_check_is_open(iop);

  /*
   *  Now process the fchmod().
   */

  rtems_libio_check_permissions( iop, LIBIO_FLAGS_WRITE );

  if ( !iop->handlers->fchmod )
    set_errno_and_return_minus_one( ENOTSUP );

  return (*iop->pathinfo.handlers->fchmod)( &iop->pathinfo, mode );
}

