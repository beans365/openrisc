/*  void Shm_Convert_packet( &packet )
 *
 *  This routine is the shared memory locked queue MPCI driver routine
 *  used to convert the RTEMS's information in a packet from non-native
 *  format to processor native format.
 *
 *  Input parameters:
 *    packet  - pointer to a packet
 *
 *  Output parameters:
 *    *packet - packet in native format
 *
 *  NOTE: Message buffers are not manipulated.
 *        Endian conversion is currently the only conversion.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: cnvpkt.c,v 1.2 2001-09-27 12:01:12 chris Exp $
 */

#include <rtems.h>
#include "shm_driver.h"

void Shm_Convert_packet(
  rtems_packet_prefix *packet
)
{
  rtems_unsigned32 *pkt, i;

  pkt = (rtems_unsigned32 *) packet;
  for ( i=RTEMS_MINIMUN_HETERO_CONVERSION ; i ; i--, pkt++ )
    *pkt = CPU_swap_u32( *pkt );

  for ( i=packet->to_convert ; i ; i--, pkt++ )
    *pkt = CPU_swap_u32( *pkt );
}
