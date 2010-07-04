/*  fatal.h
 *
 *  Copyright (c) 1998, National Research Council of Canada
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: fatal.h,v 1.2 2001-09-27 12:00:19 chris Exp $
 */
 
#include <rtems/score/interr.h>
#include <rtems/score/userext.h>

User_extensions_routine bsp_fatal_error_occurred(
  Internal_errors_Source  the_source,
  rtems_boolean           is_internal,
  rtems_unsigned32        the_error
);
