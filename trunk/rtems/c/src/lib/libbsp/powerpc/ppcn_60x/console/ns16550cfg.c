/*  nc16550cfg.c
 *
 *  This include file contains all console driver definations for the nc16550
 *
 *  COPYRIGHT (c) 1998 by Radstone Technology
 *
 *
 * THIS FILE IS PROVIDED TO YOU, THE USER, "AS IS", WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE RISK
 * AS TO THE QUALITY AND PERFORMANCE OF ALL CODE IN THIS FILE IS WITH YOU.
 *
 * You are hereby granted permission to use, copy, modify, and distribute
 * this file, provided that this notice, plus the above copyright notice
 * and disclaimer, appears in all copies. Radstone Technology will provide
 * no support for this code.
 *
 *  COPYRIGHT (c) 1989-1997.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may in
 *  the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: ns16550cfg.c,v 1.2 2001-09-27 12:00:49 chris Exp $
 */

#include <rtems.h>
#include <bsp.h>

unsigned8 Read_ns16550_register(
  unsigned32  ulCtrlPort,
  unsigned8   ucRegNum
)
{
  unsigned char *p = (unsigned char *)ulCtrlPort;
  unsigned char ucData;

  inport_byte( &p[ucRegNum], ucData );
  return ucData;
}

void  Write_ns16550_register(
  unsigned32  ulCtrlPort,
  unsigned8   ucRegNum,
  unsigned8   ucData
)
{
  unsigned char *p = (unsigned char *)ulCtrlPort;

  outport_byte( &p[ucRegNum], ucData );
}
