/*
 *  COPYRIGHT (c) 1997.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may in
 *  the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: dumpbuf.c,v 1.2 2001-09-27 12:01:42 chris Exp $
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <rtems/dumpbuf.h>
 
/*
 *  Put the body below Dump_Buffer so it won't get inlined.
 */

static inline void Dump_Line(
  unsigned char *buffer, 
  int            length
);

void Dump_Buffer(
  unsigned char *buffer,
  int            length
)
{
 
  int i, mod, max;
 
  if ( !length ) return;
 
  mod = length % 16;
 
  max = length - mod;
 
  for ( i=0 ; i<max ; i+=16 )
    Dump_Line( &buffer[ i ], 16 );
 
  if ( mod )
    Dump_Line( &buffer[ max ], mod );
}

static inline void Dump_Line(
  unsigned char *buffer,
  int            length
)
{
 
  int  i;
  char line_buffer[120];
 
  line_buffer[0] = '\0';
 
  for( i=0 ; i<length ; i++ )
    sprintf( line_buffer, "%s%02x ", line_buffer, buffer[ i ] );
 
  for( ; i<16 ; i++ )
    strcat( line_buffer, "   " );
 
  strcat( line_buffer, "|" );
  for( i=0 ; i<length ; i++ )
    sprintf( line_buffer, "%s%c", line_buffer,
             isprint( buffer[ i ] ) ? buffer[ i ] : '.' );
 
  for( ; i<16 ; i++ )
    strcat( line_buffer, " " );
 
  strcat( line_buffer, "|\n" );
 
  printf( line_buffer );
}
