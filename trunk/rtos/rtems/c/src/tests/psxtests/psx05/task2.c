/*  Task_2
 *
 *  This routine serves as a test task.
 *
 *  Input parameters:
 *    argument - task argument
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
 *  $Id: task2.c,v 1.2 2001-09-27 12:02:24 chris Exp $
 */

#include "system.h"
#include <errno.h>

void *Task_2(
  void *argument
)
{
  int status;

  printf( "Task 2: pthread_mutex_lock unavailable (inherit case)\n" );
  status = pthread_mutex_lock( &Mutex2_id );
  if ( status )
    printf( "status =%d\n", status );
  assert( !status );
  printf( "Task 2: mutex acquired\n" );

     /* switch to init */

  printf( "Task 2: unlock Mutex 2\n" );
  status = pthread_mutex_unlock( &Mutex2_id );
  assert( !status );

  printf( "Task 2: exit\n" );
  pthread_exit( NULL );

     /* switch to Init */

  return NULL; /* just so the compiler thinks we returned something */
}
