/*  Task_1_through_3
 *
 *  This routine serves as a test task.  It verifies the basic task
 *  switching capabilities of the executive.
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
 *  $Id: task.c,v 1.2 2001-09-27 12:02:23 chris Exp $
 */

#include "system.h"
#include <signal.h>

void *Task_1_through_3(
  void *argument
)
{
  int seconds;
  int i;
  int status;

  for ( i=0 ; i<5 ; i++ ) {
    print_current_time( "Task1: ", "" );
    status = pthread_kill( Init_id, SIGUSR1 );
    assert( !status );

    seconds = sleep( 1 );
    assert( !seconds );
  }
  puts( "*** END OF POSIX TEST 2 ***" );
  exit( 0 );

  return NULL; /* just so the compiler thinks we returned something */
}
