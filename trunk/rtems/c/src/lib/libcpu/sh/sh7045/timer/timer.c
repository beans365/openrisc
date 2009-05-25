/*
 *  timer for the Hitachi SH 704X 
 *
 *  This file manages the benchmark timer used by the RTEMS Timing Test
 *  Suite.  Each measured time period is demarcated by calls to
 *  Timer_initialize() and Read_timer().  Read_timer() usually returns
 *  the number of microseconds since Timer_initialize() exitted.
 *
 *  NOTE: It is important that the timer start/stop overhead be
 *        determined when porting or modifying this code.
 *
 *  Authors: Ralf Corsepius (corsepiu@faw.uni-ulm.de) and
 *           Bernd Becker (becker@faw.uni-ulm.de)
 *
 *  COPYRIGHT (c) 1997-1998, FAW Ulm, Germany
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  COPYRIGHT (c) 1998.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: timer.c,v 1.2 2001-09-27 12:01:39 chris Exp $
 */

#include <rtems.h>

#include <rtems/score/sh_io.h>
#include <rtems/score/iosh7045.h>

/*
 *  We use a Phi/4 timer
 */
#define SCALE (Timer_MHZ/4)

#define MTU1_STARTMASK 	0xfd
#define MTU1_SYNCMASK 	0xfd
#define MTU1_MODEMASK 	0xc0
#define MTU1_TCRMASK 	0x01
#define MTU1_TIORMASK 	0x88
#define MTU1_STAT_MASK 	0xf8
#define MTU1_TIERMASK 	0xfc
#define IPRC_MTU1_MASK	0xfff0

#ifndef MTU1_PRIO
#define MTU1_PRIO 15
#endif

#define MTU1_VECTOR 86

rtems_isr timerisr();

static rtems_unsigned32 Timer_interrupts;

rtems_boolean Timer_driver_Find_average_overhead;

static rtems_unsigned32 Timer_MHZ ;

void Timer_initialize( void )
{
  rtems_unsigned8  temp8;
  rtems_unsigned16 temp16;
  rtems_unsigned32 level;
  rtems_isr	   *ignored;

  Timer_MHZ = rtems_cpu_configuration_get_clicks_per_second() / 1000000 ;
  
  /*
   *  Timer has never overflowed.  This may not be necessary on some
   *  implemenations of timer but ....
   */

  Timer_interrupts /* .i */ = 0;
  _CPU_ISR_Disable( level);

  /*
   *  Somehow start the timer
   */
  /* stop Timer 1  */
  temp8 = read8( MTU_TSTR) & MTU1_STARTMASK;
  write8( temp8, MTU_TSTR);

  /* initialize counter 1 */
  write16( 0, MTU_TCNT1);

  /* Timer 1 is independent of other timers */
  temp8 = read8( MTU_TSYR) & MTU1_SYNCMASK;
  write8( temp8, MTU_TSYR);

  /* Timer 1, normal mode */
  temp8 = read8( MTU_TMDR1) & MTU1_MODEMASK;
  write8( temp8, MTU_TMDR1);

  /* x0000000
   * |||||+++--- Internal Clock
   * |||++------ Count on rising edge
   * |++-------- disable TCNT clear
   * +---------- don`t care
   */
  write8( MTU1_TCRMASK, MTU_TCR1);

  /* gra and grb are not used */
  write8( MTU1_TIORMASK, MTU_TIOR1);

  /* reset all status flags */
  temp8 = read8( MTU_TSR1) & MTU1_STAT_MASK;
  write8( temp8, MTU_TSR1);

  /* enable overflow interrupt */
  write8( MTU1_TIERMASK, MTU_TIER1);

  /* set interrupt priority */
  temp16 = read16( INTC_IPRC) & IPRC_MTU1_MASK;
  temp16 |= MTU1_PRIO;
  write16( temp16, INTC_IPRC);

  /* initialize ISR */
  _CPU_ISR_install_raw_handler( MTU1_VECTOR, timerisr, &ignored );
  _CPU_ISR_Enable( level);

  /* start timer 1 */
  temp8 = read8( MTU_TSTR) | ~MTU1_STARTMASK;
  write8( temp8, MTU_TSTR);
}

/*
 *  The following controls the behavior of Read_timer().
 *
 *  AVG_OVERHEAD is the overhead for starting and stopping the timer.  It
 *  is usually deducted from the number returned.
 *
 *  LEAST_VALID is the lowest number this routine should trust.  Numbers
 *  below this are "noise" and zero is returned.
 */

#define AVG_OVERHEAD      1  /* It typically takes X.X microseconds */
                             /* (Y countdowns) to start/stop the timer. */
                             /* This value is in microseconds. */
#define LEAST_VALID       0 /* 20 */ /* Don't trust a clicks value lower than this */

int Read_timer( void )
{
  rtems_unsigned32 clicks;
  rtems_unsigned32 total ;
  /*
   *  Read the timer and see how many clicks it has been since we started.
   */
  

  clicks = read16( MTU_TCNT1);   /* XXX: read some HW here */
  
  /*
   *  Total is calculated by taking into account the number of timer overflow
   *  interrupts since the timer was initialized and clicks since the last
   *  interrupts.
   */

  total = clicks + Timer_interrupts * 65536 ;

  if ( Timer_driver_Find_average_overhead )
    return total / SCALE;          /* in XXX microsecond units */
  else 
  {
    if ( total < LEAST_VALID )
      return 0;            /* below timer resolution */
  /*
   *  Somehow convert total into microseconds
   */
    return (total / SCALE - AVG_OVERHEAD) ;
  }
}

/*
 *  Empty function call used in loops to measure basic cost of looping
 *  in Timing Test Suite.
 */

rtems_status_code Empty_function( void )
{
  return RTEMS_SUCCESSFUL;
}

void Set_find_average_overhead(
  rtems_boolean find_flag
)
{
  Timer_driver_Find_average_overhead = find_flag;
}

/* Timer 1 is used */

#pragma interrupt
void timerisr( void )
{
  unsigned8 temp8;

  /* reset the flags of the status register */
  temp8 = read8( MTU_TSR1) & MTU1_STAT_MASK;
  write8( temp8, MTU_TSR1);

  Timer_interrupts += 1;
}
