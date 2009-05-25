/*  Receive_messages
 *
 *  This routine receives and prints three messages.
 *  an error condition.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: recvmsg.c,v 1.2 2001-09-27 12:02:21 chris Exp $
 */

#include "system.h"

void Receive_messages()
{
 rtems_status_code status;
 rtems_unsigned32  index;
 rtems_unsigned32  size;
 char              receive_buffer[16];

 for ( index=1 ; index <=3 ; index++ ) {
   puts( "Receiving message ..." );
   status = rtems_message_queue_receive(
     Queue_id[ 1 ],
     (long (*)[4])receive_buffer,
     &size,
     RTEMS_DEFAULT_OPTIONS,
     RTEMS_NO_TIMEOUT
   );
   directive_failed( status, "rtems_message_queue_receive" );
   puts_nocr( "Received : ");
   puts( receive_buffer );
 }

  puts( "Receiver delaying for a second" );
  status = rtems_task_wake_after( TICKS_PER_SECOND );
  directive_failed( status, "rtems_task_wake_after" );
}
