/*
 *  Signal Manager
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: signalsend.c,v 1.2 2001-09-27 11:59:19 chris Exp $
 */

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/asr.h>
#include <rtems/score/isr.h>
#include <rtems/rtems/modes.h>
#include <rtems/rtems/signal.h>
#include <rtems/score/thread.h>
#include <rtems/rtems/tasks.h>

/*PAGE
 *
 *  rtems_signal_send
 *
 *  This directive allows a thread to send signals to a thread.
 *
 *  Input parameters:
 *    id         - thread id
 *    signal_set - signal set
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - if successful
 *    error code - if unsuccessful
 */

rtems_status_code rtems_signal_send(
  Objects_Id             id,
  rtems_signal_set signal_set
)
{
  register Thread_Control *the_thread;
  Objects_Locations        location;
  RTEMS_API_Control       *api;
  ASR_Information         *asr;

  the_thread = _Thread_Get( id, &location );
  switch ( location ) {

    case OBJECTS_REMOTE:
#if defined(RTEMS_MULTIPROCESSING)
      return _Signal_MP_Send_request_packet(
        SIGNAL_MP_SEND_REQUEST,
        id,
        signal_set
      );
#endif

    case OBJECTS_ERROR:
      return RTEMS_INVALID_ID;

    case OBJECTS_LOCAL:
      api = the_thread->API_Extensions[ THREAD_API_RTEMS ];
      asr = &api->Signal;

      if ( ! _ASR_Is_null_handler( asr->handler ) ) {
        if ( asr->is_enabled ) {
          _ASR_Post_signals( signal_set, &asr->signals_posted );

          the_thread->do_post_task_switch_extension = TRUE;

          if ( _ISR_Is_in_progress() && _Thread_Is_executing( the_thread ) )
            _ISR_Signals_to_thread_executing = TRUE;
        } else {
          _ASR_Post_signals( signal_set, &asr->signals_pending );
        }
        _Thread_Enable_dispatch();
        return RTEMS_SUCCESSFUL;
      }
      _Thread_Enable_dispatch();
      return RTEMS_NOT_DEFINED;
  }

  return RTEMS_INTERNAL_ERROR;   /* unreached - only to remove warnings */
}
