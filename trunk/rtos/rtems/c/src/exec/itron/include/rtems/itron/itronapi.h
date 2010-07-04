/*
 *  ITRON API Support
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: itronapi.h,v 1.2 2001-09-27 11:59:12 chris Exp $
 */
 
#ifndef __ITRON_API_h
#define __ITRON_API_h
 
#include <rtems/config.h>

/*
 *  _ITRON_API_Initialize
 *
 *  Initialize the ITRON API.
 */
 
void _ITRON_API_Initialize(
  rtems_configuration_table *configuration_table
);

#endif
/* end of include file */
