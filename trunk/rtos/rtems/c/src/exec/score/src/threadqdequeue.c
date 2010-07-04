/*
 *  Thread Queue Handler
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: threadqdequeue.c,v 1.2 2001-09-27 11:59:34 chris Exp $
 */

#include <rtems/system.h>
#include <rtems/score/chain.h>
#include <rtems/score/isr.h>
#include <rtems/score/object.h>
#include <rtems/score/states.h>
#include <rtems/score/thread.h>
#include <rtems/score/threadq.h>
#include <rtems/score/tqdata.h>

/*PAGE
 *
 *  _Thread_queue_Dequeue
 *
 *  This routine removes a thread from the specified threadq.  If the
 *  threadq discipline is FIFO, it unblocks a thread, and cancels its
 *  timeout timer.  Priority discipline is processed elsewhere.
 *
 *  Input parameters:
 *    the_thread_queue - pointer to threadq
 *
 *  Output parameters:
 *    returns - thread dequeued or NULL
 *
 *  INTERRUPT LATENCY:
 *    check sync
 */

Thread_Control *_Thread_queue_Dequeue(
  Thread_queue_Control *the_thread_queue
)
{
  Thread_Control *the_thread;

  switch ( the_thread_queue->discipline ) {
    case THREAD_QUEUE_DISCIPLINE_FIFO:
      the_thread = _Thread_queue_Dequeue_fifo( the_thread_queue );
      break;
    case THREAD_QUEUE_DISCIPLINE_PRIORITY:
      the_thread = _Thread_queue_Dequeue_priority( the_thread_queue );
      break;
    default:              /* this is only to prevent warnings */
      the_thread = NULL;
      break;
  }

  return( the_thread );
}

