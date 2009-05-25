/*
 *  mkfifo() - POSIX 1003.1b 5.4.1 - Make a FIFO Special File
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: mkfifo.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int mkfifo(
  const  char *path,
  mode_t       mode
)
{
  return mknod( path, mode | S_IFIFO, 0LL );
}
