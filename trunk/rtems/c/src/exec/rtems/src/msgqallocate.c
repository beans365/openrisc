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
 *  $Id: msgqallocate.c,v 1.2 2001-09-27 11:59:19 chris Exp $
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
 *  _Message_queue_Allocate
 *
 *  Allocate a message queue and the space for its messages
 * 
 *  Input parameters:
 *    the_message_queue - the message queue to allocate message buffers
 *    count             - maximum message and reserved buffer count
 *    max_message_size  - maximum size of each message
 *
 *  Output parameters:
 *    the_message_queue - set if successful, NULL otherwise
 */

Message_queue_Control *_Message_queue_Allocate (
  unsigned32           count,
  unsigned32           max_message_size
)
{
  return 
    (Message_queue_Control *)_Objects_Allocate(&_Message_queue_Information);

}
