/*  Task_3
 *
 *  This routine serves as a test task.  It simply sleeps for 5 seconds
 *  and then deletes itself.
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
 *  $Id: task3.c,v 1.2 2001-09-27 12:02:11 chris Exp $
 */

#include "system.h"

void Task_3()
{
  rtems_status_code status;

  /*
   * XXX - Convert Later.
   */

  puts( "TA3 - rtems_task_wake_after - sleep 5 seconds" );
  status = rtems_task_wake_after( 5*TICKS_PER_SECOND );
  directive_failed( status, "rtems_task_wake_after in TA3" );


  puts( "TA3 - exd_tsk - exit and delete self" );
  exd_tsk();
  directive_failed( 0, "exd_tsk" );
}
