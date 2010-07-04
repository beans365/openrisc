/*
 *  Interrupt Manager
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: intrcatch.c,v 1.2 2001-09-27 11:59:19 chris Exp $
 */

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/score/isr.h>
#include <rtems/rtems/intr.h>

/*  rtems_interrupt_catch
 *
 *  This directive allows a thread to specify what action to take when
 *  catching signals.
 *
 *  Input parameters:
 *    new_isr_handler - address of interrupt service routine (isr)
 *    vector          - interrupt vector number
 *    old_isr_handler - address at which to store previous ISR address
 *
 *  Output parameters:
 *    RTEMS_SUCCESSFUL - always succeeds
 *    *old_isr_handler  - previous ISR address
 */

rtems_status_code rtems_interrupt_catch(
  rtems_isr_entry      new_isr_handler,
  rtems_vector_number  vector,
  rtems_isr_entry     *old_isr_handler
)
{
  if ( !_ISR_Is_vector_number_valid( vector ) )
    return RTEMS_INVALID_NUMBER;

  if ( !_ISR_Is_valid_user_handler( (void *) new_isr_handler ) )
    return RTEMS_INVALID_ADDRESS;

  if ( !_ISR_Is_valid_user_handler( (void *) old_isr_handler ) )
    return RTEMS_INVALID_ADDRESS;

  _ISR_Install_vector(
    vector, (proc_ptr)new_isr_handler, (proc_ptr *)old_isr_handler );

  return RTEMS_SUCCESSFUL;
}
