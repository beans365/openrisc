/*  macros/coremutex.h
 *
 *  This include file contains all of the inlined routines associated
 *  with core mutexes.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: coremutex.inl,v 1.2 2001-09-27 11:59:34 chris Exp $
 */

#ifndef __MACROS_CORE_MUTEX_h
#define __MACROS_CORE_MUTEX_h


/*PAGE
 *
 *  _CORE_mutex_Is_locked
 *
 */
 
#define _CORE_mutex_Is_locked( _the_mutex ) \
  ( (_the_mutex)->lock == CORE_MUTEX_LOCKED )
 
/*PAGE
 *
 *  _CORE_mutex_Is_fifo
 *
 */
 
#define _CORE_mutex_Is_fifo( _the_attribute ) \
  ( (_the_attribute)->discipline == CORE_MUTEX_DISCIPLINES_FIFO )
 
/*PAGE
 *
 *  _CORE_mutex_Is_priority
 *
 */

#define _CORE_mutex_Is_priority( _the_attribute ) \
  ( (_the_attribute)->discipline == CORE_MUTEX_DISCIPLINES_PRIORITY )

/*PAGE
 *
 *  _CORE_mutex_Is_inherit_priority
 *
 */
 
#define _CORE_mutex_Is_inherit_priority( _the_attribute ) \
  ( (_the_attribute)->discipline == \
                               CORE_MUTEX_DISCIPLINES_PRIORITY_INHERIT )
 
/*PAGE
 *
 *  _CORE_mutex_Is_priority_ceiling
 *
 */
 
#define _CORE_mutex_Is_priority_ceiling( _the_attribute )\
  ( (_the_attribute)->discipline == CORE_MUTEX_DISCIPLINES_PRIORITY_CEILING )
 
#endif
/* end of include file */
