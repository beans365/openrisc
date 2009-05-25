/*  timesys.h
 *
 *  This header file contains the global variables for the Time
 *  suite.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: timesys.h,v 1.2 2001-09-27 12:02:39 chris Exp $
 */

#include <tmacros.h>

/*
 *   How many times a particular operation is performed while timed.
 */

#ifndef OPERATION_COUNT 
#define OPERATION_COUNT 100
#endif

/* functions */

#define put_time( _message, _total_time, \
                  _iterations, _loop_overhead, _overhead ) \
    printf( \
      "%s %d\n", \
      (_message), \
      (((_total_time) - (_loop_overhead)) / (_iterations)) - (_overhead) \
    )

#if  defined(STACK_CHECKER_ON) || defined(RTEMS_DEBUG)
#define Print_Warning() \
  do { \
    puts( \
      "\n" \
      "THE TIMES REPORTED BY THIS TEST INCLUDE DEBUG CODE!\n" \
      "\n" \
    ); \
  } while (0)

#else
#define Print_Warning()
#endif

/* variables */

TEST_EXTERN volatile unsigned32 end_time;   /* ending time variable */
TEST_EXTERN volatile unsigned32 overhead;   /* loop overhead variable */

/* end of include file */
