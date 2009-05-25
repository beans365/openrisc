/*  Shm_setvec
 *
 *  This driver routine sets the SHM interrupt vector to point to the
 *  driver's SHM interrupt service routine.
 *
 *  Input parameters:  NONE
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
 *  $Id: mpisr.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <bsp.h>
#include <shm_driver.h>

void Shm_setvec( void )
{
  int vector;

  vector = _CPU_SHM_Get_vector();

  if ( vector )
    set_vector( Shm_isr, vector, 1 );
}
