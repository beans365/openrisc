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
 *  $Id: timerfireafter.c,v 1.2 2001-09-27 11:59:19 chris Exp $
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
 *  rtems_timer_fire_after
 *
 *  This directive allows a thread to start a timer.
 *
 *  Input parameters:
 *    id      - timer id
 *    ticks   - interval until routine is fired
 *    routine - routine to schedule
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code        - if unsuccessful
 */

rtems_status_code rtems_timer_fire_after(
  Objects_Id                         id,
  rtems_interval                     ticks,
  rtems_timer_service_routine_entry  routine,
  void                              *user_data
)
{
  Timer_Control      *the_timer;
  Objects_Locations   location;

  if ( ticks == 0 )
    return RTEMS_INVALID_NUMBER;

  the_timer = _Timer_Get( id, &location );
  switch ( location ) {
    case OBJECTS_REMOTE:            /* should never return this */
      return RTEMS_INTERNAL_ERROR;

    case OBJECTS_ERROR:
      return RTEMS_INVALID_ID;

    case OBJECTS_LOCAL:
      (void) _Watchdog_Remove( &the_timer->Ticker );
      the_timer->the_class = TIMER_INTERVAL;
      _Watchdog_Initialize( &the_timer->Ticker, routine, id, user_data );
      _Watchdog_Insert_ticks( &the_timer->Ticker, ticks );
      _Thread_Enable_dispatch();
      return RTEMS_SUCCESSFUL;
  }

  return RTEMS_INTERNAL_ERROR;   /* unreached - only to remove warnings */
}
