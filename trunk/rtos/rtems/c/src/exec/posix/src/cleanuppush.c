/*
 *  $Id: cleanuppush.c,v 1.2 2001-09-27 11:59:17 chris Exp $
 */

#include <pthread.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/score/chain.h>
#include <rtems/score/isr.h>
#include <rtems/score/thread.h>
#include <rtems/score/wkspace.h>
#include <rtems/posix/cancel.h>
#include <rtems/posix/pthread.h>
#include <rtems/posix/threadsup.h>

/*PAGE
 *
 *  18.2.3.1 Establishing Cancellation Handlers, P1003.1c/Draft 10, p. 184
 */

void pthread_cleanup_push(
  void   (*routine)( void * ),
  void    *arg
)
{
  POSIX_Cancel_Handler_control      *handler;
  Chain_Control                     *handler_stack;
  POSIX_API_Control                 *thread_support;
  
  if ( !routine )
    return;          /* XXX what to do really? */ 

  handler = _Workspace_Allocate( sizeof( POSIX_Cancel_Handler_control ) );

  if ( !handler )
    return;          /* XXX what to do really? */ 

  thread_support = _Thread_Executing->API_Extensions[ THREAD_API_POSIX ];

  handler_stack = &thread_support->Cancellation_Handlers;

  handler->routine = routine;
  handler->arg = arg;

  _Chain_Append( handler_stack, &handler->Node );
}
