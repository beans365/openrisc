/*
 *  Clock Tick interrupt conexion code.
 *
 *  COPYRIGHT (c) 1989-1997.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may in
 *  the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  Modified to support the MPC750.
 *  Modifications Copyright (c) 1999 Eric Valette valette@crf.canon.fr
 *
 *  $Id: p_clock.c,v 1.2 2001-09-27 12:01:06 chris Exp $
 */

#include <bsp.h>
#include <bsp/irq.h>
#include <libcpu/c_clock.h>

static rtems_irq_connect_data clockIrqData = {BSP_DECREMENTER,
					      clockIsr,
					      (rtems_irq_enable)clockOn,
					      (rtems_irq_disable)clockOff,
					      (rtems_irq_is_enabled) clockIsOn};
					      

int BSP_disconnect_clock_handler (void)
{
  return BSP_remove_rtems_irq_handler (&clockIrqData);
}

int BSP_connect_clock_handler (void)
{
  return BSP_install_rtems_irq_handler (&clockIrqData);
}
