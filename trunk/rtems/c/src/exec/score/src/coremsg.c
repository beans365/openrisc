/*
 *  CORE Message Queue Handler
 *
 *  DESCRIPTION:
 *
 *  This package is the implementation of the CORE Message Queue Handler.
 *  This core object provides task synchronization and communication functions
 *  via messages passed to queue objects.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: coremsg.c,v 1.2 2001-09-27 11:59:34 chris Exp $
 */

#include <rtems/system.h>
#include <rtems/score/chain.h>
#include <rtems/score/isr.h>
#include <rtems/score/object.h>
#include <rtems/score/coremsg.h>
#include <rtems/score/states.h>
#include <rtems/score/thread.h>
#include <rtems/score/wkspace.h>
#if defined(RTEMS_MULTIPROCESSING)
#include <rtems/score/mpci.h>
#endif

/*PAGE
 *
 *  _CORE_message_queue_Initialize
 *
 *  This routine initializes a newly created message queue based on the
 *  specified data.
 *
 *  Input parameters:
 *    the_message_queue            - the message queue to initialize
 *    the_class                    - the API specific object class
 *    the_message_queue_attributes - the message queue's attributes
 *    maximum_pending_messages     - maximum message and reserved buffer count
 *    maximum_message_size         - maximum size of each message
 *    proxy_extract_callout        - remote extract support
 *
 *  Output parameters:
 *    TRUE   - if the message queue is initialized
 *    FALSE  - if the message queue is NOT initialized
 */

boolean _CORE_message_queue_Initialize(
  CORE_message_queue_Control    *the_message_queue,
  Objects_Classes                the_class,
  CORE_message_queue_Attributes *the_message_queue_attributes,
  unsigned32                     maximum_pending_messages,
  unsigned32                     maximum_message_size,
  Thread_queue_Extract_callout   proxy_extract_callout
)
{
  unsigned32 message_buffering_required;
  unsigned32 allocated_message_size;

  the_message_queue->maximum_pending_messages   = maximum_pending_messages;
  the_message_queue->number_of_pending_messages = 0;
  the_message_queue->maximum_message_size       = maximum_message_size;
  _CORE_message_queue_Set_notify( the_message_queue, NULL, NULL );
 
  /*
   * round size up to multiple of a ptr for chain init
   */
 
  allocated_message_size = maximum_message_size;
  if (allocated_message_size & (sizeof(unsigned32) - 1)) {
      allocated_message_size += sizeof(unsigned32);
      allocated_message_size &= ~(sizeof(unsigned32) - 1);
  }
   
  message_buffering_required = maximum_pending_messages *
       (allocated_message_size + sizeof(CORE_message_queue_Buffer_control));
 
  the_message_queue->message_buffers = (CORE_message_queue_Buffer *) 
     _Workspace_Allocate( message_buffering_required );
 
  if (the_message_queue->message_buffers == 0)
    return FALSE;
 
  _Chain_Initialize (
    &the_message_queue->Inactive_messages,
    the_message_queue->message_buffers,
    maximum_pending_messages,
    allocated_message_size + sizeof( CORE_message_queue_Buffer_control )
  );
 
  _Chain_Initialize_empty( &the_message_queue->Pending_messages );
 
  _Thread_queue_Initialize(
    &the_message_queue->Wait_queue,
    the_class,
    _CORE_message_queue_Is_priority( the_message_queue_attributes ) ?
       THREAD_QUEUE_DISCIPLINE_PRIORITY : THREAD_QUEUE_DISCIPLINE_FIFO,
    STATES_WAITING_FOR_MESSAGE,
    proxy_extract_callout,
    CORE_MESSAGE_QUEUE_STATUS_TIMEOUT
  );

  return TRUE;
}
