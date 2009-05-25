/*
 *  Object Handler
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: objectcopynamestring.c,v 1.2 2001-09-27 11:59:34 chris Exp $
 */

#include <rtems/system.h>
#include <rtems/score/address.h>
#include <rtems/score/chain.h>
#include <rtems/score/object.h>
#if defined(RTEMS_MULTIPROCESSING)
#include <rtems/score/objectmp.h>
#endif
#include <rtems/score/thread.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/sysstate.h>
#include <rtems/score/isr.h>

/*PAGE
 *
 *  _Objects_Copy_name_string
 *
 *  XXX
 */
 
void _Objects_Copy_name_string(
  void       *source,
  void       *destination
)
{
  unsigned8 *source_p = (unsigned8 *) source;
  unsigned8 *destination_p = (unsigned8 *) destination;
 
  *destination_p = '\0';
  if ( source_p ) {
    do {
      *destination_p++ = *source_p;
    } while ( *source_p++ );
  }
}
