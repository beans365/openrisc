/*  copyrt.h
 *
 *  This include file contains the copyright notice for RTEMS
 *  which is included in every binary copy of the executive.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: copyrt.h,v 1.2 2001-09-27 11:59:32 chris Exp $
 */

#ifndef __RTEMS_COPYRIGHT_h
#define __RTEMS_COPYRIGHT_h

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SCORE_INIT

const char _Copyright_Notice[] =
"COPYRIGHT (c) 1989-1999.\n\
On-Line Applications Research Corporation (OAR).\n";

#else

extern const char _Copyright_Notice[];

#endif

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
