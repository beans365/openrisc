/*
 *  Message Queue Manager
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: msgqdelete.c,v 1.2 2001-09-27 11:59:19 chris Exp $
 */

#include <rtems/system.h>
#include <rtems/score/sysstate.h>
#include <rtems/score/chain.h>
#include <rtems/score/isr.h>
#include <rtems/score/coremsg.h>
#include <rtems/score/object.h>
#include <rtems/score/states.h>
#include <rtems/score/thread.h>
#include <rtems/score/wkspace.h>
#if defined(RTEMS_MULTIPROCESSING)
#include <rtems/score/mpci.h>
#endif
#include <rtems/rtems/status.h>
#include <rtems/rtems/attr.h>
#include <rtems/rtems/message.h>
#include <rtems/rtems/options.h>
#include <rtems/rtems/support.h>

/*PAGE
 *
 *  rtems_message_queue_delete
 *
 *  This directive allows a thread to delete the message queue specified
 *  by the given queue identifier.
 *
 *  Input parameters:
 *    id - queue id
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

rtems_status_code rtems_message_queue_delete(
  Objects_Id id
)
{
  register Message_queue_Control *the_message_queue;
  Objects_Locations               location;

  the_message_queue = _Message_queue_Get( id, &location );
  switch ( location ) {

    case OBJECTS_REMOTE:
#if defined(RTEMS_MULTIPROCESSING)
      _Thread_Dispatch();
      return RTEMS_ILLEGAL_ON_REMOTE_OBJECT;
#endif

    case OBJECTS_ERROR:
      return RTEMS_INVALID_ID;

    case OBJECTS_LOCAL:
      _Objects_Close( &_Message_queue_Information,
                      &the_message_queue->Object );

      _CORE_message_queue_Close(
        &the_message_queue->message_queue,
#if defined(RTEMS_MULTIPROCESSING)
        _Message_queue_MP_Send_object_was_deleted,
#else
        NULL,
#endif
        CORE_MESSAGE_QUEUE_STATUS_WAS_DELETED
      );

      _Message_queue_Free( the_message_queue );

#if defined(RTEMS_MULTIPROCESSING)
      if ( _Attributes_Is_global( the_message_queue->attribute_set ) ) {
        _Objects_MP_Close(
          &_Message_queue_Information,
          the_message_queue->Object.id
        );

        _Message_queue_MP_Send_process_packet(
          MESSAGE_QUEUE_MP_ANNOUNCE_DELETE,
          the_message_queue->Object.id,
          0,                                 /* Not used */
          0
        );
      }
#endif

      _Thread_Enable_dispatch();
      return RTEMS_SUCCESSFUL;
  }

  return RTEMS_INTERNAL_ERROR;   /* unreached - only to remove warnings */
}
