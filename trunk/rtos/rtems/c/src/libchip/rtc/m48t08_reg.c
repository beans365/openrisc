/*
 *  This file contains a typical set of register access routines which may be
 *  used with the m48t08 chip if accesses to the chip are as follows:
 *
 *    + registers are accessed as bytes
 *    + registers are only byte-aligned (no address gaps)
 *
 *  COPYRIGHT (c) 1989-1997.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: m48t08_reg.c,v 1.2 2001-09-27 12:01:42 chris Exp $
 */

#include <rtems.h>

#ifndef _M48T08_MULTIPLIER
#define _M48T08_MULTIPLIER 1
#define _M48T08_NAME(_X) _X
#define _M48T08_TYPE unsigned8
#endif

#define CALCULATE_REGISTER_ADDRESS( _base, _reg ) \
  (_M48T08_TYPE *)((_base) + ((_reg) * _M48T08_MULTIPLIER ))

/* 
 *  M48T08 Get Register Routine
 */

unsigned32 _M48T08_NAME(m48t08_get_register)(
  unsigned32  ulCtrlPort,
  unsigned8   ucRegNum
)
{
  _M48T08_TYPE *port;

  port = CALCULATE_REGISTER_ADDRESS( ulCtrlPort, ucRegNum );

  return *port;
}

/*
 *  M48T08 Set Register Routine
 */

void  _M48T08_NAME(m48t08_set_register)(
  unsigned32  ulCtrlPort,
  unsigned8   ucRegNum,
  unsigned32  ucData
)
{
  _M48T08_TYPE *port;

  port = CALCULATE_REGISTER_ADDRESS( ulCtrlPort, ucRegNum );

  *port = ucData;
}
