/*
 *  mkdir() - POSIX 1003.1b 5.4.1 - Make a Directory
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: mkdir.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int mkdir(
  const char *pathname,
  mode_t      mode
)
{
  return mknod( pathname, mode | S_IFDIR, 0LL);
}

