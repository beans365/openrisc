/*
 *  tcsetattr() - POSIX 1003.1b 7.2.1 - Get and Set State
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: tcsetattr.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <rtems.h>
#if defined(RTEMS_NEWLIB)

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <termios.h>
/* #include <sys/ioctl.h> */

int ioctl();

#include <rtems/libio.h>
#include "libio_.h"

int tcsetattr(
  int             fd,
  int             opt,
  struct termios *tp
)
{
  switch (opt) {
  default:
    set_errno_and_return_minus_one( ENOTSUP );

  case TCSADRAIN:
    if (ioctl( fd, RTEMS_IO_TCDRAIN, NULL ) < 0)
    	return -1;
    /*
     * Fall through to....
     */
  case TCSANOW:
    return ioctl( fd, RTEMS_IO_SET_ATTRIBUTES, tp );
  }
}
#endif
