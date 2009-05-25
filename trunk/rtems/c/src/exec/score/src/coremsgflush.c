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
 *  $Id: coremsgflush.c,v 1.2 2001-09-27 11:59:34 chris Exp $
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
 *  _CORE_message_queue_Flush
 *
 *  This function flushes the message_queue's pending message queue.  The
 *  number of messages flushed from the queue is returned.
 *
 *  Input parameters:
 *    the_message_queue - the message_queue to be flushed
 *
 *  Output parameters:
 *    returns - the number of messages flushed from the queue
 */
 
unsigned32 _CORE_message_queue_Flush(
  CORE_message_queue_Control *the_message_queue
)
{
  if ( the_message_queue->number_of_pending_messages != 0 )
    return _CORE_message_queue_Flush_support( the_message_queue );
  else
    return 0;
}

