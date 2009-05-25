/*
 *  truncate() - Truncate a File to the Specified Length
 *
 *  This routine is not defined in the POSIX 1003.1b standard but is
 *  commonly supported on most UNIX and POSIX systems.  It is provided
 *  for compatibility.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: truncate.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

int truncate(
  const char  *path,
  off_t        length
)
{
  int status;
  int fd;

  fd = open( path, O_WRONLY );
  if ( fd == -1 )
    return -1;

  status = ftruncate( fd, length );

  (void) close( fd );

  return status;
}

