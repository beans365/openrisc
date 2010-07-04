/*
 *  $Id: psignal.c,v 1.2 2001-09-27 11:59:17 chris Exp $
 */

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>

#include <rtems/system.h>
#include <rtems/score/isr.h>
#include <rtems/score/thread.h>
#include <rtems/score/tqdata.h>
#include <rtems/score/wkspace.h>
#include <rtems/posix/seterr.h>
#include <rtems/posix/threadsup.h>
#include <rtems/posix/psignal.h>
#include <rtems/posix/pthread.h>
#include <rtems/posix/time.h>
#include <stdio.h>


/*
 *  Currently 32 signals numbered 1-32 are defined
 */

#define SIGNAL_EMPTY_MASK  0x00000000
#define SIGNAL_ALL_MASK    0xffffffff

#define signo_to_mask( _sig ) (1 << ((_sig) - 1))

#define is_valid_signo( _sig ) \
  ((_sig) >= 1 && (_sig) <= 32 )

/*** PROCESS WIDE STUFF ****/

sigset_t  _POSIX_signals_Pending;

void _POSIX_signals_Abnormal_termination_handler( int signo )
{
  exit( 1 );
}

#define SIG_ARRAY_MAX  (SIGRTMAX + 1)
struct sigaction _POSIX_signals_Default_vectors[ SIG_ARRAY_MAX ] = {
  /* NO SIGNAL 0 */  SIGACTION_IGNORE,
  /* SIGHUP    1 */  SIGACTION_TERMINATE,
  /* SIGINT    2 */  SIGACTION_TERMINATE,
  /* SIGQUIT   3 */  SIGACTION_TERMINATE,
  /* SIGILL    4 */  SIGACTION_TERMINATE,
  /* SIGTRAP   5 */  SIGACTION_TERMINATE,
  /* SIGIOT    6 */  SIGACTION_TERMINATE,
  /* SIGABRT   6     SIGACTION_TERMINATE, -- alias for SIGIOT */
  /* SIGEMT    7 */  SIGACTION_TERMINATE,
  /* SIGFPE    8 */  SIGACTION_TERMINATE,
  /* SIGKILL   9 */  SIGACTION_TERMINATE,
  /* SIGBUS   10 */  SIGACTION_TERMINATE,
  /* SIGSEGV  11 */  SIGACTION_TERMINATE,
  /* SIGSYS   12 */  SIGACTION_TERMINATE,
  /* SIGPIPE  13 */  SIGACTION_TERMINATE,
  /* SIGALRM  14 */  SIGACTION_TERMINATE,
  /* SIGTERM  15 */  SIGACTION_TERMINATE,
  /* SIGUSR1  16 */  SIGACTION_TERMINATE,
  /* SIGUSR2  17 */  SIGACTION_TERMINATE,
  /* SIGRTMIN 18 */  SIGACTION_TERMINATE,
  /* SIGRT    19 */  SIGACTION_TERMINATE,
  /* SIGRT    20 */  SIGACTION_TERMINATE,
  /* SIGRT    21 */  SIGACTION_TERMINATE,
  /* SIGRT    22 */  SIGACTION_TERMINATE,
  /* SIGRT    23 */  SIGACTION_TERMINATE,
  /* SIGRT    24 */  SIGACTION_TERMINATE,
  /* SIGRT    25 */  SIGACTION_TERMINATE,
  /* SIGRT    26 */  SIGACTION_TERMINATE,
  /* SIGRT    27 */  SIGACTION_TERMINATE,
  /* SIGRT    28 */  SIGACTION_TERMINATE,
  /* SIGRT    29 */  SIGACTION_TERMINATE,
  /* SIGRT    30 */  SIGACTION_TERMINATE,
  /* SIGRT    31 */  SIGACTION_TERMINATE,
  /* SIGRTMAX 32 */  SIGACTION_TERMINATE
};

struct sigaction _POSIX_signals_Vectors[ SIG_ARRAY_MAX ];

Watchdog_Control _POSIX_signals_Alarm_timer;

Thread_queue_Control _POSIX_signals_Wait_queue;

Chain_Control _POSIX_signals_Inactive_siginfo;
Chain_Control _POSIX_signals_Siginfo[ SIG_ARRAY_MAX ];

/*PAGE
 *
 *  XXX - move these
 */

#define _States_Is_interruptible_signal( _states ) \
  ( ((_states) & \
    (STATES_WAITING_FOR_SIGNAL|STATES_INTERRUPTIBLE_BY_SIGNAL)) == \
      (STATES_WAITING_FOR_SIGNAL|STATES_INTERRUPTIBLE_BY_SIGNAL))

/*
 *  _POSIX_signals_Post_switch_extension
 */

void _POSIX_signals_Post_switch_extension(
  Thread_Control  *the_thread
)
{
  POSIX_API_Control  *api;
  int                 signo;
  ISR_Level           level;

  api = the_thread->API_Extensions[ THREAD_API_POSIX ];

  /*
   *  If we invoke any user code, there is the possibility that
   *  a new signal has been posted that we should process so we
   *  restart the loop if a signal handler was invoked.
   *
   *  The first thing done is to check there are any signals to be 
   *  processed at all.  No point in doing this loop otherwise.
   */

restart:
  _ISR_Disable( level );
    if ( !(~api->signals_blocked & 
          (api->signals_pending | _POSIX_signals_Pending)) ) {
     _ISR_Enable( level );
     return;
   }
  _ISR_Enable( level );

  for ( signo = SIGRTMIN ; signo <= SIGRTMAX ; signo++ ) {

    if ( _POSIX_signals_Check_signal( api, signo, FALSE ) )
      goto restart;

    if ( _POSIX_signals_Check_signal( api, signo, TRUE ) )
      goto restart;

  }

/* XXX - add __SIGFIRSTNOTRT or something like that to newlib siginfo.h */

  for ( signo = SIGHUP ; signo <= __SIGLASTNOTRT ; signo++ ) {

    if ( _POSIX_signals_Check_signal( api, signo, FALSE ) )
      goto restart;
 
    if ( _POSIX_signals_Check_signal( api, signo, TRUE ) )
      goto restart;

  }

}

/*PAGE
 *
 *  _POSIX_signals_Alarm_TSR
 */
 
void _POSIX_signals_Alarm_TSR(
  Objects_Id      id,
  void           *argument
)
{
  int status;

  status = kill( getpid(), SIGALRM );
  /* XXX can't print from an ISR, should this be fatal? */
  assert( !status );
}

/*PAGE
 *
 *  _POSIX_signals_Manager_Initialization
 */

void _POSIX_signals_Manager_Initialization(
  int  maximum_queued_signals
)
{
  unsigned32 signo;

  /*
   *  Insure we have the same number of vectors and default vector entries
   */

  assert(
   sizeof(_POSIX_signals_Vectors) == sizeof(_POSIX_signals_Default_vectors)
  );

  memcpy( 
    _POSIX_signals_Vectors,
    _POSIX_signals_Default_vectors,
    sizeof( _POSIX_signals_Vectors )
  );
  
  /*
   *  Initialize the set of pending signals for the entire process
   */

  sigemptyset( &_POSIX_signals_Pending );

  /*
   *  Initialize the timer used to implement alarm().
   */

  _Watchdog_Initialize(
    &_POSIX_signals_Alarm_timer,
    _POSIX_signals_Alarm_TSR,
    0,
    NULL
  );
 
  /*
   *  Initialize the queue we use to block for signals
   */
 
  _Thread_queue_Initialize(
    &_POSIX_signals_Wait_queue,
    OBJECTS_NO_CLASS,
    THREAD_QUEUE_DISCIPLINE_PRIORITY,
    STATES_WAITING_FOR_SIGNAL | STATES_INTERRUPTIBLE_BY_SIGNAL,
    NULL,
    EAGAIN
  );

  /* XXX status codes */

  /* 
   *  Allocate the siginfo pools.
   */

  for ( signo=1 ; signo<= SIGRTMAX ; signo++ )
    _Chain_Initialize_empty( &_POSIX_signals_Siginfo[ signo ] );

  _Chain_Initialize(
    &_POSIX_signals_Inactive_siginfo,
    _Workspace_Allocate_or_fatal_error(
      maximum_queued_signals * sizeof( POSIX_signals_Siginfo_node )
    ),
    maximum_queued_signals,
    sizeof( POSIX_signals_Siginfo_node )
  );
}
