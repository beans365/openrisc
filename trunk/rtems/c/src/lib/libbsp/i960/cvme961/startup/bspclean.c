/*
 *  This routine is used to return control to the NINDY monitor
 *  and is automatically invoked at shutdown.
 *
 *  NOTES:  DOES NOT RETURN!!!
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: bspclean.c,v 1.2 2001-09-27 11:59:57 chris Exp $
 */

#include <rtems.h>
#include "bsp.h"

void bsp_cleanup( void )
{
  extern void start( void  );

  register volatile void *start_addr = 0;

  asm volatile( "mov   0,g0;"
                "fmark ;"
                "syncf ;"
                ".word    0xfeedface ; "
                "bx       (%0)" : "=r" (start_addr) : "0" (start_addr) );
 /*  The constant 0xfeedface is a magic word for break which
  *  is defined by NINDY.  The branch extended restarts the
  *  application if the user types "go".
  */
}
