/*
 *  pathconf() - POSIX 1003.1b - 5.7.1 - Configurable Pathname Varables
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: pathconf.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

long pathconf(
  const char *path,
  int         name
)
{
  int status;
  int fd;

  fd = open( path, O_RDONLY );
  if ( fd == -1 )
    return -1;

  status = fpathconf( fd, name );

  (void) close( fd );

  return status;
}
