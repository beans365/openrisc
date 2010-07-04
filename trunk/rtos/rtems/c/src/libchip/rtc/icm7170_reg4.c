/*
 *  This file contains a typical set of register access routines which may be
 *  used with the icm7170 chip if accesses to the chip are as follows:
 *
 *    + registers are accessed as bytes
 *    + registers are on 32-bit boundaries
 *
 *  COPYRIGHT (c) 1989-1997.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: icm7170_reg4.c,v 1.2 2001-09-27 12:01:42 chris Exp $
 */

#define _ICM7170_MULTIPLIER 4
#define _ICM7170_NAME(_X) _X##_4
#define _ICM7170_TYPE unsigned8

#include "icm7170_reg.c"

