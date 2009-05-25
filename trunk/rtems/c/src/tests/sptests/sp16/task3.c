/*  Task_3
 *
 *  This routine serves as a test task.  It competes with the other tasks
 *  for region resources.
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
 *  $Id: task3.c,v 1.2 2001-09-27 12:02:33 chris Exp $
 */

#include "system.h"

rtems_task Task_3(
  rtems_task_argument argument
)
{
  rtems_status_code  status;
  void              *segment_address_1;
  void              *segment_address_2;

  puts(
    "TA3 - rtems_region_get_segment - wait on 3968 byte segment from region 2"
  );
  status = rtems_region_get_segment(
    Region_id[ 2 ],
    3968,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &segment_address_1
  );
  directive_failed( status, "rtems_region_get_segment" );
  puts_nocr( "TA3 - got segment from region 2 - " );
  Put_address_from_area_2( segment_address_1 );
  new_line;
  directive_failed( status, "rtems_region_return_segment" );

  puts( "TA3 - rtems_region_get_segment - wait on 2K segment from region 3" );
  status = rtems_region_get_segment(
    Region_id[ 3 ],
    2048,
    RTEMS_DEFAULT_OPTIONS,
    RTEMS_NO_TIMEOUT,
    &segment_address_2
  );
  directive_failed( status, "rtems_region_get_segment" );
}
