/*
 *  $Id: psignal.h,v 1.2 2001-09-27 11:59:14 chris Exp $
 */

#ifndef __RTEMS_POSIX_SIGNALS_h
#define __RTEMS_POSIX_SIGNALS_h

#include <rtems/posix/pthread.h>

/*
 *  Currently 32 signals numbered 1-32 are defined
 */

#define SIGNAL_EMPTY_MASK  0x00000000
#define SIGNAL_ALL_MASK    0xffffffff

#define signo_to_mask( _sig ) (1 << ((_sig) - 1))

#define is_valid_signo( _sig ) \
  ((_sig) >= 1 && (_sig) <= 32 )

#define _States_Is_interruptible_signal( _states ) \
  ( ((_states) & \
    (STATES_WAITING_FOR_SIGNAL|STATES_INTERRUPTIBLE_BY_SIGNAL)) == \
      (STATES_WAITING_FOR_SIGNAL|STATES_INTERRUPTIBLE_BY_SIGNAL))

#define SIGACTION_TERMINATE \
  { 0, SIGNAL_ALL_MASK, {_POSIX_signals_Abnormal_termination_handler} }
#define SIGACTION_IGNORE \
  { 0, SIGNAL_ALL_MASK, {SIG_IGN} }
#define SIGACTION_STOP \
  { 0, SIGNAL_ALL_MASK, {_POSIX_signals_Stop_handler} }
#define SIGACTION_CONTINUE \
  { 0, SIGNAL_ALL_MASK, {_POSIX_signals_Continue_handler} }

#define SIG_ARRAY_MAX  (SIGRTMAX + 1)

/*
 *  Variables
 */

extern sigset_t  _POSIX_signals_Pending;

extern struct sigaction _POSIX_signals_Default_vectors[ SIG_ARRAY_MAX ];

extern struct sigaction _POSIX_signals_Vectors[ SIG_ARRAY_MAX ];

extern Watchdog_Control _POSIX_signals_Alarm_timer;

extern Thread_queue_Control _POSIX_signals_Wait_queue;

extern Chain_Control _POSIX_signals_Inactive_siginfo;

extern Chain_Control _POSIX_signals_Siginfo[ SIG_ARRAY_MAX ];

/*
 *  POSIX internal siginfo structure
 */

typedef struct {
  Chain_Node  Node;
  siginfo_t   Info;
}  POSIX_signals_Siginfo_node;

/*
 *  Internal routines
 */

void _POSIX_signals_Manager_Initialization(
  int  maximum_queued_signals
);

void _POSIX_signals_Post_switch_extension(
  Thread_Control  *the_thread
);

boolean _POSIX_signals_Unblock_thread( 
  Thread_Control  *the_thread,
  int              signo,
  siginfo_t       *info
);

boolean _POSIX_signals_Check_signal(
  POSIX_API_Control  *api,
  int                 signo,
  boolean             is_global
);

boolean _POSIX_signals_Clear_signals(
  POSIX_API_Control  *api,
  int                 signo,
  siginfo_t          *info,
  boolean             is_global,
  boolean             check_blocked
);

int killinfo(
  pid_t               pid,
  int                 sig,
  const union sigval *value
);

void _POSIX_signals_Set_process_signals(
  sigset_t   mask
);

void _POSIX_signals_Clear_process_signals(
  sigset_t   mask
);

/*
 *  Default signal handlers
 */

#define _POSIX_signals_Stop_handler NULL
#define _POSIX_signals_Continue_handler NULL

void _POSIX_signals_Abnormal_termination_handler( int signo );

#endif
/* end of file */
