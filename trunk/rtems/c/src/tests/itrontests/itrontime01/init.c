/*  Init
 *
 *  This routine is the initialization task for this test program.
 *  It is called from init_exec and has the responsibility for creating
 *  and starting the tasks that make up the test.  If the time of day
 *  clock is required for the test, it should also be set to a known
 *  value by this function.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: init.c,v 1.2 2001-09-27 12:02:11 chris Exp $
 */

#define TEST_INIT
#include "system.h"
#include <stdio.h>
#include <time.h>

/*
 * get the current time in second since epoch (1970), change it since 1985,
 * compute the time in millisecond 
 *
 */
 
int time_sec_epoch( void ) {

int s_time,t_time,r_time;
long m_sec;

t_time = time(NULL);
s_time = (1985-1970)*365*24*60*60;
r_time = t_time - s_time;
m_sec = r_time*1000;

printf( "Time from 1970: %d\n", t_time );
printf( "Time between 1970 and 1985: %d\n", s_time );
printf( "Time from 1985: %d\n", r_time );
printf("milliseconds: %ld\n",m_sec);

return 0;
} 



void ITRON_Init( void )
{
  printf( "\n\n*** ITRON TIME TEST 01 ***\n" );
  printf( "doesn't test anything yet\n" );
  printf( "*** END OF ITRON TIME TEST 01 ***\n" );
  exit( 0 );
}


