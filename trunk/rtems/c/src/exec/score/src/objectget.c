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
 *  $Id: objectget.c,v 1.2 2001-09-27 11:59:34 chris Exp $
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
 * _Objects_Get
 *
 * This routine sets the object pointer for the given
 * object id based on the given object information structure.
 *
 * Input parameters:
 *   information - pointer to entry in table for this class
 *   id          - object id to search for
 *   location    - address of where to store the location
 *
 * Output parameters:
 *   returns  - address of object if local
 *   location - one of the following:
 *                  OBJECTS_ERROR  - invalid object ID
 *                  OBJECTS_REMOTE - remote object
 *                  OBJECTS_LOCAL  - local object
 */

Objects_Control *_Objects_Get(
  Objects_Information *information,
  Objects_Id           id,
  Objects_Locations   *location
)
{
  Objects_Control *the_object;
  unsigned32       index;

  index = _Objects_Get_index( id );

  if ( information->maximum >= index ) {
    _Thread_Disable_dispatch();
    if ( (the_object = _Objects_Get_local_object( information, index )) != NULL ) {
      *location = OBJECTS_LOCAL;
      return( the_object );
    }
    _Thread_Enable_dispatch();
    *location = OBJECTS_ERROR;
    return( NULL );
  }
  *location = OBJECTS_ERROR;
#if defined(RTEMS_MULTIPROCESSING)
  _Objects_MP_Is_remote(
    information,
    _Objects_Build_id( information->the_class, _Objects_Local_node, index ),
    location,
    &the_object
  );
  return the_object;
#else
  return NULL;
#endif
}
