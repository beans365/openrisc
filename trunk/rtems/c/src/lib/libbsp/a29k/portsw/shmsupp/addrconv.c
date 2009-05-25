/*  Shm_Convert_address
 *
 *  No address range conversion is required.
 *
 *  Input parameters:
 *    address - address to convert
 *
 *  Output parameters:
 *    returns - converted address
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: addrconv.c,v 1.2 2001-09-27 11:59:40 chris Exp $
 */

#include <rtems.h>
#include <bsp.h>
#include <shm_driver.h>

#ifndef lint
static char _sccsid[] = "@(#)addrconv.c 04/08/96     1.1\n";
#endif

void *Shm_Convert_address(
  void *address
)
{
  return ( address );
}
