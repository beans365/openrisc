/*
 *  IMFS_node_type
 *
 *  The following verifies that returns the type of node that the 
 *  loc refers to.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: imfs_ntype.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <errno.h>
#include "imfs.h"

rtems_filesystem_node_types_t IMFS_node_type(
   rtems_filesystem_location_info_t    *pathloc         /* IN */
)
{
  IMFS_jnode_t *node;

  node = pathloc->node_access;
  return node->type;
}
