/*
 *  Reserve resources for console driver.
 *
 *  This is in a separate file to minimize the amount of baggage
 *  pulled in when not using the console device driver.
 *
 *  Author:
 *    W. Eric Norum
 *    Saskatchewan Accelerator Laboratory
 *    University of Saskatchewan
 *    Saskatoon, Saskatchewan, CANADA
 *    eric@skatter.usask.ca
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: consolereserveresources.c,v 1.2 2001-09-27 12:00:08 chris Exp $
 */

#include <termios.h>
#include <bsp.h>
#include <rtems/libio.h>
#include "m68360.h"

int _console_reserve_resources_removed;
