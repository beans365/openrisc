/*
 *  NOTE:  The structure of the routines is identical to that of POSIX
 *         Message_queues to leave the option of having unnamed message
 *         queues at a future date.  They are currently not part of the
 *         POSIX standard but unnamed message_queues are.  This is also 
 *         the reason for the apparently unnecessary tracking of 
 *         the process_shared attribute.  [In addition to the fact that
 *         it would be trivial to add pshared to the mq_attr structure
 *         and have process private message queues.]
 *
 *         This code ignores the O_RDONLY/O_WRONLY/O_RDWR flag at open
 *         time.
 *
 *  $Id: mqueuegetattr.c,v 1.2 2001-09-27 11:59:17 chris Exp $
 */

#include <stdarg.h>

#include <pthread.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>

#include <rtems/system.h>
#include <rtems/score/watchdog.h>
#include <rtems/posix/seterr.h>
#include <rtems/posix/mqueue.h>
#include <rtems/posix/time.h>

/*PAGE
 *
 *  15.2.8 Get Message Queue Attributes, P1003.1b-1993, p. 283
 */

int mq_getattr(
  mqd_t           mqdes,
  struct mq_attr *mqstat
)
{
  register POSIX_Message_queue_Control *the_mq;
  Objects_Locations                     location;
  CORE_message_queue_Attributes        *the_mq_attr;
 
  if ( !mqstat )
    set_errno_and_return_minus_one( EINVAL );

  the_mq = _POSIX_Message_queue_Get( mqdes, &location );
  switch ( location ) {
    case OBJECTS_ERROR:
      set_errno_and_return_minus_one( EBADF );
    case OBJECTS_REMOTE:
      _Thread_Dispatch();
      return POSIX_MP_NOT_IMPLEMENTED();
      set_errno_and_return_minus_one( EINVAL );
    case OBJECTS_LOCAL:
      /*
       *  Return the old values.
       */
 
      /* XXX this is the same stuff as is in mq_setattr... and probably */
      /* XXX should be in an inlined private routine */
 
      the_mq_attr = &the_mq->Message_queue.Attributes;
 
      mqstat->mq_flags   = the_mq->oflag;
      mqstat->mq_msgsize = the_mq->Message_queue.maximum_message_size;
      mqstat->mq_maxmsg  = the_mq->Message_queue.maximum_pending_messages;
      mqstat->mq_curmsgs = the_mq->Message_queue.number_of_pending_messages;
 
      _Thread_Enable_dispatch();
      return 0;
  }
  return POSIX_BOTTOM_REACHED();
}

