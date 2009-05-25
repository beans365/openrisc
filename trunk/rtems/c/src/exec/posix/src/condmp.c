/*
 *  $Id: condmp.c,v 1.2 2001-09-27 11:59:17 chris Exp $
 */

#include <pthread.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/score/object.h>
#include <rtems/score/states.h>
#include <rtems/score/watchdog.h>
#include <rtems/posix/cond.h>
#include <rtems/posix/time.h>
#include <rtems/posix/mutex.h>

/*
 *  TEMPORARY
 */

#if defined(RTEMS_MULTIPROCESSING)
void _POSIX_Condition_variables_MP_Send_process_packet (
  POSIX_Condition_variables_MP_Remote_operations  operation,
  Objects_Id                        condition_variables_id,
  Objects_Name                      name,
  Objects_Id                        proxy_id
)
{
  (void) POSIX_MP_NOT_IMPLEMENTED();
}

void _POSIX_Condition_variables_MP_Send_extract_proxy(
  Thread_Control *the_thread
)
{
  (void) POSIX_MP_NOT_IMPLEMENTED();
}
#endif

/*
 *  END OF TEMPORARY
 */

