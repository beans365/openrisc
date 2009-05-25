/*  types.h
 *
 *  This include file defines the types used by the RTEMS API.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: types.h,v 1.2 2001-09-27 11:59:18 chris Exp $
 */

#ifndef __RTEMS_RTEMS_TYPES_h
#define __RTEMS_RTEMS_TYPES_h

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/score/object.h>
#include <rtems/score/priority.h>
#include <rtems/score/tod.h>
#include <rtems/score/watchdog.h>
#include <rtems/rtems/modes.h>
#if defined(RTEMS_MULTIPROCESSING)
#include <rtems/score/mpci.h>
#include <rtems/score/mppkt.h>
#endif

/*
 *  RTEMS basic type definitions
 */

typedef unsigned8       rtems_unsigned8;  /* unsigned 8-bit value  */
typedef unsigned16      rtems_unsigned16; /* unsigned 16-bit value */
typedef unsigned32      rtems_unsigned32; /* unsigned 32-bit value */

typedef signed8         rtems_signed8;    /* signed 8-bit value  */
typedef signed16        rtems_signed16;   /* signed 16-bit value */
typedef signed32        rtems_signed32;   /* signed 32-bit value */

/*
 * some C++ compilers (eg: HP's) don't do 'long long'
 */
#if defined(__GNUC__)
typedef unsigned64      rtems_unsigned64; /* unsigned 64-bit value */
typedef signed64        rtems_signed64;   /* signed 64-bit value */
#endif

typedef single_precision rtems_single;    /* single precision float */
typedef double_precision rtems_double;    /* double precision float */

typedef boolean          rtems_boolean;

typedef unsigned32       rtems_name;
typedef Objects_Id       rtems_id;

typedef Context_Control            rtems_context;
typedef Context_Control_fp         rtems_context_fp;
typedef CPU_Interrupt_frame        rtems_interrupt_frame;

/*
 *  Time related 
 */

typedef Watchdog_Interval rtems_interval;
typedef TOD_Control       rtems_time_of_day;

/*
 *  Define the type for an RTEMS API task mode.
 */
 
typedef Modes_Control rtems_mode;

/*
 *  MPCI related entries
 */

#if defined(RTEMS_MULTIPROCESSING)
typedef MP_packet_Classes          rtems_mp_packet_classes;
typedef MP_packet_Prefix           rtems_packet_prefix;

typedef MPCI_initialization_entry  rtems_mpci_initialization_entry;
typedef MPCI_get_packet_entry      rtems_mpci_get_packet_entry;
typedef MPCI_return_packet_entry   rtems_mpci_return_packet_entry;
typedef MPCI_send_entry            rtems_mpci_send_packet_entry;
typedef MPCI_receive_entry         rtems_mpci_receive_packet_entry;

typedef MPCI_Entry rtems_mpci_entry;

typedef MPCI_Control rtems_mpci_table;
#endif

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
