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
#include <signal.h>

void *Task_2(
  void *argument
)
{
  int status;

  printf( "Task_2: sending SIGUSR1\n" );
  status = pthread_kill( Init_id, SIGUSR1 );
  assert( !status );

     /* switch to Init */

  printf( "Task_2: exit\n" );
  pthread_exit( NULL );

     /* switch to Init */

  return NULL; /* just so the compiler thinks we returned something */
}
