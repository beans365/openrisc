/*  options.inl
 *
 *  This file contains the macro implementation of the inlined
 *  routines from the Options Handler.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: options.inl,v 1.2 2001-09-27 11:59:19 chris Exp $
 */

#ifndef __OPTIONS_inl
#define __OPTIONS_inl

/*PAGE
 *
 *  _Options_Is_no_wait
 *
 */

#define _Options_Is_no_wait( _option_set ) \
   ( (_option_set) & RTEMS_NO_WAIT )

/*PAGE
 *
 *  _Options_Is_any
 *
 */

#define _Options_Is_any( _option_set ) \
   ( (_option_set) & RTEMS_EVENT_ANY )

#endif
/* end of include file */
