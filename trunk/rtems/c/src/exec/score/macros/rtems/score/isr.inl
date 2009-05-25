/*  isr.inl
 *
 *  This include file contains the macro implementation of all
 *  inlined routines in the Interrupt Handler.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: isr.inl,v 1.2 2001-09-27 11:59:34 chris Exp $
 */

#ifndef __ISR_inl
#define __ISR_inl

/*PAGE
 *
 *  _ISR_Is_in_progress
 *
 */

#define _ISR_Is_in_progress() \
        (_ISR_Nest_level != 0)

/*PAGE
 *
 *  _ISR_Is_vector_number_valid
 *
 */

#define _ISR_Is_vector_number_valid( _vector ) \
  ( (_vector) <= CPU_INTERRUPT_MAXIMUM_VECTOR_NUMBER )

/*PAGE
 *
 *  _ISR_Is_valid_user_handler
 *
 */

#define _ISR_Is_valid_user_handler( _handler ) \
  ((_handler) != NULL)

#endif
/* end of include file */
