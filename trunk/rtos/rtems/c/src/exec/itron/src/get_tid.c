/*
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: get_tid.c,v 1.2 2001-09-27 11:59:13 chris Exp $
 */

#include <itron.h>

#include <rtems/score/thread.h>
#include <rtems/score/userext.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/apiext.h>
#include <rtems/score/sysstate.h>

#include <rtems/itron/task.h>

/*
 *  get_tid - Get Task Identifier
 */

ER get_tid(
  ID *p_tskid
)
{
  /*
   *  This does not support multiprocessing.  The id handling will have
   *  to be enhanced to support multiprocessing.
   */
   
  *p_tskid = _Objects_Get_index( _Thread_Executing->Object.id );
  return E_OK;
}

