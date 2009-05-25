/*  Test1
 *
 *  This test uses a hack to disable suto-extend then checks to see only the
 *  requested number of objects are allocated.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-1997.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may in
 *  the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: test1.c,v 1.2 2001-09-27 12:02:29 chris Exp $
 */

#include "system.h"
#include <rtems/score/object.h>
#include <stdio.h>

void test1()
{
  boolean           auto_extend;
  rtems_status_code result;
  rtems_unsigned32  task_count = 0;
  
  char              c1 = 'a';
  char              c2 = 'a';
  char              c3 = '0';
  char              c4 = '0';
    
  printf( "\n TEST1 : auto-extend disabled.\n" );

  /*
   * This is a major hack and only recommended for a test. Doing this
   * saves having another test.
   */

  auto_extend = _Objects_Information_table[OBJECTS_RTEMS_TASKS]->auto_extend;
  _Objects_Information_table[OBJECTS_RTEMS_TASKS]->auto_extend = FALSE;
  
  while (task_count < MAX_TASKS)
  {
    rtems_name name;

    printf(" TEST1 : creating task '%c%c%c%c', ", c1, c2, c3, c4);
    
    name = rtems_build_name(c1, c2, c3, c4);

    result = rtems_task_create(name,
                               10,
                               RTEMS_MINIMUM_STACK_SIZE,
                               RTEMS_DEFAULT_ATTRIBUTES,
                               RTEMS_LOCAL,
                               &task_id[task_count]);

    if (status_code_bad(result))
      break;
    
    printf("number = %3i, id = %08x, starting, ", task_count, task_id[task_count]);
    
    fflush(stdout);
    result = rtems_task_start(task_id[task_count],
                              test_task,
                              (rtems_task_argument) task_count);
    
    if (status_code_bad(result))
      break;
    
    /*
     *  Update the name.
     */
    
    NEXT_TASK_NAME(c1, c2, c3, c4);
    
    task_count++;
  }

  if (task_count >= MAX_TASKS)
    printf( "\nMAX_TASKS too small for work-space size, please make larger !!\n\n" );
    
  if (task_count != (TASK_ALLOCATION_SIZE - 1)) {
    printf( " FAIL1 : the number of tasks does not equal the expected size -\n"
            "           task created = %i, required number = %i\n",
            task_count, TASK_ALLOCATION_SIZE);
    exit( 1 );
  }

  destory_all_tasks("TEST1");
  
  _Objects_Information_table[OBJECTS_RTEMS_TASKS]->auto_extend = auto_extend;
  
  printf( " TEST1 : completed\n" );
}





