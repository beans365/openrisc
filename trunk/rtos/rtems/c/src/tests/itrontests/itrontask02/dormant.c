/*  Dormant
 *
 *  This routine serves as two test tasks.
 *  It has one dormant and one sleeping tasks.
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
 *  $Id: dormant.c,v 1.2 2001-09-27 12:02:11 chris Exp $
 */

#include <assert.h>
#include "system.h"

void Dormant_task()
{
  puts( "DORMANT - ext_tsk - going to DORMANT state" );
  ext_tsk( );

  puts( "ERROR==>ext_tsk of DORMANT returned" );
  assert(0);
}


void Non_Dormant_task()
{
  ER       status;

  while (TRUE) {
    puts( "NON-DORMANT - Sleep for 2 minutes" );
    status = rtems_task_wake_after( 120*TICKS_PER_SECOND );
    directive_failed( status, "rtems_task_wake_after" );
  }
}  

