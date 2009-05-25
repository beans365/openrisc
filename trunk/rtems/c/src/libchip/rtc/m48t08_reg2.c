/*
 *  This file contains a typical set of register access routines which may be
 *  used with the m48t08 chip if accesses to the chip are as follows:
 *
 *    + registers are accessed as bytes
 *    + registers are on 16-bit boundaries
 *
 *  COPYRIGHT (c) 1989-1997.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: m48t08_reg2.c,v 1.2 2001-09-27 12:01:42 chris Exp $
 */

#define _M48T08_MULTIPLIER 2
#define _M48T08_NAME(_X) _X##_2
#define _M48T08_TYPE unsigned8

#include "m48t08_reg.c"

