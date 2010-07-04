/*  Application_task
 *
 *  This routine is as an example of an application task which
 *  prints a message including its RTEMS task id.  This task
 *  then invokes exit to return to the monitor.
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
 *  $Id: apptask.c,v 1.2 2001-09-27 12:02:28 chris Exp $
 */

#include "system.h"
#include <stdio.h>

rtems_task Application_task(
  rtems_task_argument argument
)
{
  rtems_id          tid;
  rtems_status_code status;

  status = rtems_task_ident( RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &tid );

  printf( "Application task was invoked with argument (%d) "
          "and has id of 0x%x\n", argument, tid );

  printf( "*** END OF SAMPLE SINGLE PROCESSOR APPLICATION ***\n" );
  exit( 0 );
}
