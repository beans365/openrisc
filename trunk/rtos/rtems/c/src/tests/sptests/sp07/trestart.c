/*  Task_restart_extension
 *
 *  This routine is the trestart user extension.
 *
 *  Input parameters:
 *    unused          - pointer to currently running TCB
 *    restarted_task  - pointer to new TCB being started
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
 *  $Id: trestart.c,v 1.2 2001-09-27 12:02:31 chris Exp $
 */

#include "system.h"

void Task_restart_extension(
  rtems_tcb *unused,
  rtems_tcb *restarted_task
)
{
  if ( task_number( restarted_task->Object.id ) > 0 ) {
    puts_nocr( "TASK_RESTART - " );
    put_name( Task_name[ task_number( restarted_task->Object.id ) ], FALSE );
    puts( " - restarted." );
  }
}
