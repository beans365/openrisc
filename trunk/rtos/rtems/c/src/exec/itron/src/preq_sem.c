/*
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: preq_sem.c,v 1.2 2001-09-27 11:59:13 chris Exp $
 */

#include <itron.h>

#include <rtems/itron/semaphore.h>
#include <rtems/itron/task.h>
#include <rtems/score/tod.h>

/*
 *  preq_sem - Poll and Request Semaphore
 *
 *  This function implements the ITRON 3.0 preq_sem() service.
 */

ER preq_sem(
  ID semid
)
{ 
  return twai_sem( semid, TMO_POL );
}
