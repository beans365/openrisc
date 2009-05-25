/*  options.inl
 *
 *  This file contains the static inline implementation of the inlined
 *  routines from the Options Handler.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: options.inl,v 1.2 2001-09-27 11:59:18 chris Exp $
 */

#ifndef __OPTIONS_inl
#define __OPTIONS_inl

/*PAGE
 *
 *  _Options_Is_no_wait
 *
 *  DESCRIPTION:
 *
 *  This function returns TRUE if the RTEMS_NO_WAIT option is enabled in
 *  option_set, and FALSE otherwise.
 */

RTEMS_INLINE_ROUTINE boolean _Options_Is_no_wait (
  rtems_option option_set
)
{
   return (option_set & RTEMS_NO_WAIT);
}

/*PAGE
 *
 *  _Options_Is_any
 *
 *  DESCRIPTION:
 *
 *  This function returns TRUE if the RTEMS_EVENT_ANY option is enabled in
 *  OPTION_SET, and FALSE otherwise.
 */

RTEMS_INLINE_ROUTINE boolean _Options_Is_any (
  rtems_option option_set
)
{
   return (option_set & RTEMS_EVENT_ANY);
}

#endif
/* end of include file */
