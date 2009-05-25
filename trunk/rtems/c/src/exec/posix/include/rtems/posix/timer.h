/*
 *  $Id: timer.h,v 1.2 2001-09-27 11:59:14 chris Exp $
 */

#ifndef __RTEMS_POSIX_TIMERS_h
#define __RTEMS_POSIX_TIMERS_h

/* ************
 * Constants
 * ************/

#define STATE_FREE_C        0x01 /* Free position of the table of timers   */
#define STATE_CREATE_NEW_C  0x02 /* Created timer but not running          */
#define STATE_CREATE_RUN_C  0x03 /* Created timer and running              */
#define STATE_CREATE_STOP_C 0x04 /* Created, ran and stopped timer         */
#define MAX_NSEC_C    1000000000 /* Maximum number of nsec allowed         */
#define MIN_NSEC_C             0 /* Minimum number of nsec allowew         */
#define TIMER_RELATIVE_C       0 /* Indicates that the fire time is
                                  * relative to the current one            */
#define SEC_TO_TICKS_C _TOD_Ticks_per_second /* Number of ticks in a second*/
#define NSEC_PER_SEC_C 1000000000 /* Nanoseconds in a second               */

#define NO_MORE_TIMERS_C      11 /* There is not available timers          */
#define BAD_TIMER_C           11 /* The timer does not exist in the table  */

#define SECONDS_PER_YEAR_C    ( 360 * 24 * 60 * 60 )
#define SECONDS_PER_MONTH_C    ( 30 * 24 * 60 * 60 )
#define SECONDS_PER_DAY_C           ( 24 * 60 * 60 )
#define SECONDS_PER_HOUR_C               ( 60 * 60 )
#define SECONDS_PER_MINUTE_C                  ( 60 )


/*
 * Data for a timer
 */

typedef struct {
  pthread_t         thread_id;  /* Thread identifier                     */
  char              state;      /* State of the timer                    */
  struct sigevent   inf;        /* Information associated to the timer   */
  timer_t           timer_id;   /* Created timer identifier              */
  struct itimerspec timer_data; /* Timing data of the timer              */
  unsigned32        ticks;      /* Number of ticks of the initialization */
  unsigned32        overrun;    /* Number of expirations of the timer    */
  rtems_time_of_day time;       /* Time in which the timer was started   */
} timer_alive_t;

/*
 * Array of Timers
 */

extern int timer_max;
extern timer_alive_t *timer_struct;

#endif
/* end of include file */

