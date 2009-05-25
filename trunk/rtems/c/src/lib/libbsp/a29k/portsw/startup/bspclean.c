/*  bsp_cleanup()
 *
 *  This routine normally is part of start.s and usually returns
 *  control to a monitor.
 *
 *  INPUT:  NONE
 *
 *  OUTPUT: NONE
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: bspclean.c,v 1.2 2001-09-27 11:59:42 chris Exp $
 */

#include <rtems.h>
#include <bsp.h>

#ifndef lint
static char _sccsid[] = "@(#)bspclean.c 04/08/96     1.1\n";
#endif

void bsp_cleanup( void )
{
}
