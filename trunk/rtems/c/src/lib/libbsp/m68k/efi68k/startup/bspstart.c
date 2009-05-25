/*
 *  This routine starts the application.  It includes application,
 *  board, and monitor specific initialization and configuration.
 *  The generic CPU dependent initialization has been performed
 *  before this routine is invoked.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: bspstart.c,v 1.2 2001-09-27 12:00:03 chris Exp $
 */

#include <bsp.h>
#include <rtems/libio.h>
#include <libcsupport.h>

#include <string.h>
 
/*
 *  The original table from the application and our copy of it with
 *  some changes.
 */

extern rtems_configuration_table  Configuration;
rtems_configuration_table         BSP_Configuration;

rtems_cpu_table Cpu_table;

char *rtems_progname;
 
rtems_unsigned32 Timer_interrupts;

extern void set_debug_traps(void);
extern void breakpoint(void);

/*
 *  Use the shared implementations of the following routines
 */
 
void bsp_postdriver_hook(void);
void bsp_libc_init( void *, unsigned32, int );
void bsp_pretasking_hook(void);               /* m68k version */

/*
 *  bsp_start
 *
 *  This routine does the bulk of the system initialization.
 */

void bsp_start( void )
{
  void                 *vbr;
  extern void          *_WorkspaceBase;
  extern void          *_RamSize;
  extern unsigned long  _M68k_Ramsize;

  _M68k_Ramsize = (unsigned long)&_RamSize;		/* RAM size set in linker script */

/*   set_debug_traps();    */
/*   breakpoint(); */

  /*
   *  we only use a hook to get the C library initialized.
   */

  Cpu_table.pretasking_hook = bsp_pretasking_hook;
  Cpu_table.postdriver_hook = bsp_postdriver_hook;

  m68k_get_vbr( vbr );
  Cpu_table.interrupt_vector_table = vbr;

  BSP_Configuration.work_space_start = (void *) &_WorkspaceBase;

  /* Clock_exit is done as an atexit() function */
}

