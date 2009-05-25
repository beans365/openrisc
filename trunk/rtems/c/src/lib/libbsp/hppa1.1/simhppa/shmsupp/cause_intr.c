/*  void Shm_Cause_interrupt_pxfl( node )
 *
 *  This routine is the shared memory driver routine which
 *  generates interrupts to other CPUs.
 *
 *  Input parameters:
 *    node          - destination of this packet (0 = broadcast)
 *
 *  Output parameters: NONE
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: cause_intr.c,v 1.2 2001-09-27 11:59:45 chris Exp $
 */

#include <stdio.h>

#include <rtems.h>
#include <rtems/error.h>

#include <shm_driver.h>

void Shm_Cause_interrupt_pxfl(
  rtems_unsigned32 node
)
{
  Shm_Interrupt_information *intr;
  rtems_unsigned32 *u32;
  rtems_unsigned32  value;

  intr = &Shm_Interrupt_table[node];
  value = intr->value;

  switch ( intr->length ) {
    case NO_INTERRUPT:
       break;

    case LONG:
      u32   = (rtems_unsigned32 *)intr->address;
      HPPA_ASM_STWAS( value, 0, u32 );
      break;
    default:
      fprintf( stderr, "Shm_Cause_interrupt_pxfl: Unsupported length!!!\n" );
  }
}
