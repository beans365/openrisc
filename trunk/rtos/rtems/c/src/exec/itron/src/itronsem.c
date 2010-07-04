/*
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: itronsem.c,v 1.2 2001-09-27 11:59:13 chris Exp $
 */

#include <itron.h>

#include <rtems/itron/semaphore.h>
#include <rtems/itron/task.h>
#include <rtems/score/tod.h>

/*
 *  _ITRON_Semaphore_Manager_initialization
 *
 *  DESCRIPTION:
 *
 *  This routine initializes all semaphore manager related data structures.
 *
 *  Input parameters:
 *    maximum_semaphores - maximum configured semaphores
 *
 *  Output parameters:  NONE
 */

void _ITRON_Semaphore_Manager_initialization(
  unsigned32 maximum_semaphores
)
{
  _Objects_Initialize_information(
    &_ITRON_Semaphore_Information,     /* object information table */
    OBJECTS_ITRON_SEMAPHORES,          /* object class */
    FALSE,                             /* TRUE if this is a global */
                                       /*   object class */
    maximum_semaphores,                /* maximum objects of this class */
    sizeof( ITRON_Semaphore_Control ), /* size of this object's control block */
    FALSE,                             /* TRUE if names for this object */
                                       /*   are strings */
    ITRON_MAXIMUM_NAME_LENGTH,         /* maximum length of each object's */
                                       /*   name */
    FALSE                              /* TRUE if this class is threads */
  );

  /*
   *  Register the MP Process Packet routine.
   *
   *  NOTE: No MP Support YET in RTEMS ITRON implementation.
   */

}

