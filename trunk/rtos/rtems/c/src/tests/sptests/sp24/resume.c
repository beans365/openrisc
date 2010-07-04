/*  Resume_task
 *
 *  This subprogram is scheduled as a timer service routine.  When
 *  it fires it resumes the task which is mapped to this timer.
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
 *  $Id: resume.c,v 1.2 2001-09-27 12:02:36 chris Exp $
 */

#include "system.h"

rtems_timer_service_routine Resume_task(
  rtems_id  timer_id,
  void     *ignored_address
)
{
  rtems_id          task_to_resume;
  rtems_status_code status;

  task_to_resume = Task_id[ rtems_get_index( timer_id ) ];
  status = rtems_task_resume( task_to_resume );
  directive_failed_with_level( status, "rtems_task_resume", 1 );
}
