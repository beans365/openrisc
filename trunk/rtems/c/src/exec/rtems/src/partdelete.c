/*
 *  Partition Manager
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: partdelete.c,v 1.2 2001-09-27 11:59:19 chris Exp $
 */

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/score/address.h>
#include <rtems/score/object.h>
#include <rtems/rtems/part.h>
#include <rtems/score/thread.h>
#include <rtems/score/sysstate.h>

/*PAGE
 *
 *  rtems_partition_delete
 *
 *  This directive allows a thread to delete a partition specified by
 *  the partition identifier, provided that none of its buffers are
 *  still allocated.
 *
 *  Input parameters:
 *    id - partition id
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

rtems_status_code rtems_partition_delete(
  Objects_Id id
)
{
  register Partition_Control *the_partition;
  Objects_Locations                  location;

  the_partition = _Partition_Get( id, &location );
  switch ( location ) {
    case OBJECTS_REMOTE:
#if defined(RTEMS_MULTIPROCESSING)
      _Thread_Dispatch();
      return RTEMS_ILLEGAL_ON_REMOTE_OBJECT;
#endif

    case OBJECTS_ERROR:
      return RTEMS_INVALID_ID;

    case OBJECTS_LOCAL:
      if ( the_partition->number_of_used_blocks == 0 ) {
        _Objects_Close( &_Partition_Information, &the_partition->Object );
        _Partition_Free( the_partition );
#if defined(RTEMS_MULTIPROCESSING)
        if ( _Attributes_Is_global( the_partition->attribute_set ) ) {

          _Objects_MP_Close(
            &_Partition_Information,
            the_partition->Object.id
          );

          _Partition_MP_Send_process_packet(
            PARTITION_MP_ANNOUNCE_DELETE,
            the_partition->Object.id,
            0,                         /* Not used */
            0                          /* Not used */
          );
        }
#endif

        _Thread_Enable_dispatch();
        return RTEMS_SUCCESSFUL;
      }
      _Thread_Enable_dispatch();
      return RTEMS_RESOURCE_IN_USE;
  }

  return RTEMS_INTERNAL_ERROR;   /* unreached - only to remove warnings */
}
