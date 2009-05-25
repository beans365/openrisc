/*
 *  $Id: conddefaultattributes.c,v 1.2 2001-09-27 11:59:17 chris Exp $
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

/*PAGE
 *
 *  The default condition variable attributes structure.
 */
 
const pthread_condattr_t _POSIX_Condition_variables_Default_attributes = {
  TRUE,                      /* is_initialized */
  PTHREAD_PROCESS_PRIVATE    /* process_shared */
};
