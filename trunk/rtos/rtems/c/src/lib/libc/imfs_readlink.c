/*
 *  IMFS_readlink
 *
 *  The following rouine puts the symblic links destination name into 
 *  buff.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: imfs_readlink.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <errno.h>
#include "imfs.h"
#include "libio_.h"

int IMFS_readlink(
  rtems_filesystem_location_info_t  *loc,
 char                               *buf,         /* OUT */
 size_t                             bufsize    
)
{
  IMFS_jnode_t      *node;
  int                i;
  
  node = loc->node_access;

  if ( node->type != IMFS_SYM_LINK )
    set_errno_and_return_minus_one( EINVAL );

  for( i=0; ((i<bufsize) && (node->info.sym_link.name[i] != '\0')); i++ )
    buf[i] = node->info.sym_link.name[i];

  return i;
}
