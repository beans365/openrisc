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
 *  $Id: system.h,v 1.2 2001-09-27 12:02:34 chris Exp $
 */

#include <tmacros.h>

/* functions */

rtems_task Init(
  rtems_task_argument argument
);
 
rtems_task First_FP_task(
  rtems_task_argument argument
);
 
rtems_task FP_task(
  rtems_task_argument argument
);
 
rtems_task Task_1(
  rtems_task_argument argument
);

/* configuration information */

#define CONFIGURE_INIT_TASK_ATTRIBUTES    RTEMS_FLOATING_POINT 

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             7

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS         (18 * RTEMS_MINIMUM_STACK_SIZE)

#include <confdefs.h>

/* global variables */

TEST_EXTERN rtems_id Task_id[ 7 ];     /* array of task ids */
TEST_EXTERN rtems_id Task_name[ 7 ];   /* array of task names */

TEST_EXTERN rtems_double FP_factors[ 10 ];  /* FP "uniqueness" factors */
TEST_EXTERN rtems_unsigned32 INTEGER_factors[ 10 ];  /* INT "uniqueness" factors */
/* end of include file */
