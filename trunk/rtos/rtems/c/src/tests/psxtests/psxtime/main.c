/*
 *  Simple test program -- simplified version of sample test hello.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: main.c,v 1.2 2001-09-27 12:02:26 chris Exp $
 */

#define TEST_INIT

#include <bsp.h>

void test_main( void );

rtems_task Init(
  rtems_task_argument ignored
)
{
  test_main();
  exit( 0 );
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS 1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT

#include <confdefs.h>

/* end of file */
