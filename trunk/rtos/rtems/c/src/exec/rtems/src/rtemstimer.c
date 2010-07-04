/*
 *  Timer Manager
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: rtemstimer.c,v 1.2 2001-09-27 11:59:19 chris Exp $
 */

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/score/object.h>
#include <rtems/score/thread.h>
#include <rtems/rtems/timer.h>
#include <rtems/score/tod.h>
#include <rtems/score/watchdog.h>

/*PAGE
 *
 *  _Timer_Manager_initialization
 *
 *  This routine initializes all timer manager related data structures.
 *
 *  Input parameters:
 *    maximum_timers - number of timers to initialize
 *
 *  Output parameters:  NONE
 */

void _Timer_Manager_initialization(
  unsigned32 maximum_timers
)
{
  _Objects_Initialize_information(
    &_Timer_Information,
    OBJECTS_RTEMS_TIMERS,
    FALSE,
    maximum_timers,
    sizeof( Timer_Control ),
    FALSE,
    RTEMS_MAXIMUM_NAME_LENGTH,
    FALSE
  );
}
