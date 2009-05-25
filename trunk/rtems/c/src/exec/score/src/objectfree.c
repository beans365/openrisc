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
 *  $Id: objectfree.c,v 1.2 2001-09-27 11:59:34 chris Exp $
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
 *  _Objects_Free
 *
 *  DESCRIPTION:
 *
 *  This function frees a object control block to the
 *  inactive chain of free object control blocks.
 */

void _Objects_Free(
  Objects_Information *information,
  Objects_Control     *the_object
)
{
  unsigned32  allocation_size = information->allocation_size;

  _Chain_Append( &information->Inactive, &the_object->Node );

  if ( information->auto_extend ) {
    unsigned32  block;
    
    block = 
      _Objects_Get_index( the_object->id ) - _Objects_Get_index( information->minimum_id );
    block /= information->allocation_size;
      
    information->inactive_per_block[ block ]++;
    information->inactive++;
  
    /*
     *  Check if the threshold level has been met of
     *  1.5 x allocation_size are free.
     */

    if ( information->inactive > ( allocation_size + ( allocation_size >> 1 ) ) ) {
      _Objects_Shrink_information( information );
    }
  }
}
