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
 *  $Id: partgetbuffer.c,v 1.2 2001-09-27 11:59:19 chris Exp $
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
 *  rtems_partition_get_buffer
 *
 *  This directive will obtain a buffer from a buffer partition.
 *
 *  Input parameters:
 *    id     - partition id
 *    buffer - pointer to buffer address
 *
 *  Output parameters:
 *    buffer            - pointer to buffer address filled in
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

rtems_status_code rtems_partition_get_buffer(
  Objects_Id   id,
  void       **buffer
)
{
  register Partition_Control *the_partition;
  Objects_Locations           location;
  void                       *the_buffer;

  the_partition = _Partition_Get( id, &location );
  switch ( location ) {
    case OBJECTS_REMOTE:
#if defined(RTEMS_MULTIPROCESSING)
      _Thread_Executing->Wait.return_argument = buffer;
      return(
        _Partition_MP_Send_request_packet(
          PARTITION_MP_GET_BUFFER_REQUEST,
          id,
          0           /* Not used */
        )
      );
#endif

    case OBJECTS_ERROR:
      return RTEMS_INVALID_ID;

    case OBJECTS_LOCAL:
      the_buffer = _Partition_Allocate_buffer( the_partition );
      if ( the_buffer ) {
        the_partition->number_of_used_blocks += 1;
        _Thread_Enable_dispatch();
        *buffer = the_buffer;
        return RTEMS_SUCCESSFUL;
      }
      _Thread_Enable_dispatch();
      return RTEMS_UNSATISFIED;
  }

  return RTEMS_INTERNAL_ERROR;   /* unreached - only to remove warnings */
}
