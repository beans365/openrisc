/*
 *  RTEMS API Support
 *
 *  NOTE:
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: rtemsapi.h,v 1.2 2001-09-27 11:59:18 chris Exp $
 */
 
#ifndef __RTEMS_API_h
#define __RTEMS_API_h
 
#include <rtems/config.h>

/*PAGE
 *
 *  _RTEMS_API_Initialize
 *
 *  This routine initializes the RTEMS API by invoking the initialization
 *  routine for each RTEMS manager with the appropriate parameters
 *  from the configuration_table.
 */
 
void _RTEMS_API_Initialize(
  rtems_configuration_table *configuration_table
);

#endif
/* end of include file */
