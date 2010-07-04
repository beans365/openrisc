/*  Init
 *
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
 *  $Id: init.c,v 1.2 2001-09-27 12:02:28 chris Exp $
 */

#include <bsp.h>

rtems_task Init(
  rtems_task_argument ignored
)
{
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_USE_MINIIMFS_AS_BASE_FILESYSTEM

#define CONFIGURE_MAXIMUM_TASKS            1

#define CONFIGURE_INIT

#include <confdefs.h>

/* global variables */

