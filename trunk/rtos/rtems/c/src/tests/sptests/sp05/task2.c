/*  Task_2
 *
 *  This routine serves as a test task.  Makes sure a task can suspend
 *  itself.
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
 *  $Id: task2.c,v 1.2 2001-09-27 12:02:31 chris Exp $
 */

#include "system.h"

rtems_task Task_2(
  rtems_task_argument argument
)
{
  rtems_status_code status;

  puts( "TA2 - rtems_task_suspend - suspend self" );
  status = rtems_task_suspend( RTEMS_SELF );
  directive_failed( status, "rtems_task_suspend of TA2" );

  while( FOREVER ) {
    puts( "TA2 - rtems_task_wake_after - sleep 1 second" );
    status = rtems_task_wake_after( 1*TICKS_PER_SECOND );
    directive_failed( status, "rtems_task_wake_after of TA2" );
  }
}
