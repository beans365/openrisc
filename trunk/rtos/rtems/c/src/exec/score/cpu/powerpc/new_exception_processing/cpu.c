/*
 *  PowerPC CPU Dependent Source
 *
 *  Author:	Andrew Bray <andy@i-cubed.co.uk>
 *
 *  COPYRIGHT (c) 1995 by i-cubed ltd.
 *
 *  To anyone who acknowledges that this file is provided "AS IS"
 *  without any express or implied warranty:
 *      permission to use, copy, modify, and distribute this file
 *      for any purpose is hereby granted without fee, provided that
 *      the above copyright notice and this notice appears in all
 *      copies, and that the name of i-cubed limited not be used in
 *      advertising or publicity pertaining to distribution of the
 *      software without specific, written prior permission.
 *      i-cubed limited makes no representations about the suitability
 *      of this software for any purpose.
 *
 *  Derived from c/src/exec/cpu/no_cpu/cpu.c:
 *
 *  COPYRIGHT (c) 1989-1997.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may be found in
 *  the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: cpu.c,v 1.2 2001-09-27 11:59:29 chris Exp $
 */

#include <rtems/system.h>
#include <rtems/score/isr.h>
#include <rtems/score/context.h>
#include <rtems/score/thread.h>
#include <rtems/score/interr.h>


/*  _CPU_Initialize
 *
 *  This routine performs processor dependent initialization.
 *
 *  INPUT PARAMETERS:
 *    cpu_table       - CPU table to initialize
 *    thread_dispatch - address of disptaching routine
 */

void _CPU_Initialize(
  rtems_cpu_table  *cpu_table,
  void      (*thread_dispatch)      /* ignored on this CPU */
)
{
  _CPU_Table = *cpu_table;
}

/*PAGE
 *
 *  _CPU_Context_Initialize
 */

void _CPU_Context_Initialize(
  Context_Control  *the_context,
  unsigned32       *stack_base,
  unsigned32        size,
  unsigned32        new_level,
  void             *entry_point,
  boolean           is_fp
)
{
  unsigned32 msr_value;
  unsigned32 sp;

  sp = (unsigned32)stack_base + size - CPU_MINIMUM_STACK_FRAME_SIZE;
  *((unsigned32 *)sp) = 0;
  the_context->gpr1 = sp;
   
  _CPU_MSR_GET( msr_value );

  if (!(new_level & CPU_MODES_INTERRUPT_MASK)) {
    msr_value |= MSR_EE;
  }
  else {
    msr_value &= ~MSR_EE;
  }

  the_context->msr = msr_value;

  /*
   *  The FP bit of the MSR should only be enabled if this is a floating
   *  point task.  Unfortunately, the vfprintf_r routine in newlib 
   *  ends up pushing a floating point register regardless of whether or
   *  not a floating point number is being printed.  Serious restructuring
   *  of vfprintf.c will be required to avoid this behavior.  At this
   *  time (7 July 1997), this restructuring is not being done.
   */

  /*if ( is_fp ) */
    the_context->msr |= PPC_MSR_FP;

  the_context->pc = (unsigned32)entry_point;
}



/*PAGE
 *
 *  _CPU_Install_interrupt_stack
 */

void _CPU_Install_interrupt_stack( void )
{
}




