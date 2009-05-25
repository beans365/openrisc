/*
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: eventflags.h,v 1.2 2001-09-27 11:59:12 chris Exp $
 */

#ifndef __RTEMS_ITRON_EVENTFLAGS_h_
#define __RTEMS_ITRON_EVENTFLAGS_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/itron/object.h>

/*
 *  The following defines the control block used to manage each event flag.
 */

typedef struct {
  ITRON_Objects_Control   Object;
  unsigned32              XXX_more_stuff_goes_here;
}   ITRON_Eventflags_Control;

/*
 *  The following defines the information control block used to manage
 *  this class of objects.
 */

ITRON_EXTERN Objects_Information  _ITRON_Eventflags_Information;

/*
 *  _ITRON_Eventflags_Manager_initialization
 *
 *  DESCRIPTION:
 *
 *  This routine performs the initialization necessary for this manager.
 */

void _ITRON_Eventflags_Manager_initialization(
  unsigned32 maximum_eventflags
);

/*
 *  XXX insert private stuff here
 */

#include <rtems/itron/eventflags.inl>

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */

