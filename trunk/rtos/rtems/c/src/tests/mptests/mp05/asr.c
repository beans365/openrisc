/*  Process_asr
 *
 *  This routine performs the processing for task 1's RTEMS_ASR.  It is called
 *  by an assembly routine which saves the necessary registers.
 *
 *  Input parameters:
 *    signal - signal set
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
 *  $Id: asr.c,v 1.2 2001-09-27 12:02:18 chris Exp $
 */

#include "system.h"

rtems_asr Process_asr(
    rtems_signal_set signal
)
{
  if ( signal != expected_signal ) {
     printf(
       "ERROR: I was expecting signal 0x%.8x got 0x%.8x\n",
       expected_signal,
       signal
     );
     rtems_fatal_error_occurred( 0xf0000 );
  }
  signal_caught = 1;
}
