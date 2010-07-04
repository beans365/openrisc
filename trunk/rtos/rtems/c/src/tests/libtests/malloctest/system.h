/*  system.h
 *
 *  This include file contains information that is included in every
 *  function in the test set.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: system.h,v 1.2 2001-09-27 12:02:12 chris Exp $
 */

#include <tmacros.h>

/* macros */

/* functions */

rtems_task Init(
  rtems_task_argument argument
);

rtems_task Task_1_through_5(
  rtems_task_argument argument
);

void blow_stack( void );

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define TASK_STACK_SIZE (RTEMS_MINIMUM_STACK_SIZE*3)

#define CONFIGURE_EXTRA_TASK_STACKS  ((RTEMS_MINIMUM_STACK_SIZE*2)*5)
#define CONFIGURE_MAXIMUM_TASKS               6

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#include <confdefs.h>

/* global variables */

TEST_EXTERN rtems_id   Task_id[ 6 ];         /* array of task ids */
TEST_EXTERN rtems_name Task_name[ 6 ];       /* array of task names */

/* end of include file */
