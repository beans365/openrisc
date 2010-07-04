/*
 *  This test file is used to verify that the header files associated with
 *  invoking this function are correct.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: proc09.c,v 1.2 2001-09-27 12:02:25 chris Exp $
 */

#include <sys/types.h>
 
void test( void )
{
  gid_t grouplist[ 20 ];
  int   gidsetsize;
  int   result;

  gidsetsize = 20;

  result = getgroups( gidsetsize, grouplist );
}
