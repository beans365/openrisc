/*
 *  mount()
 *
 *  XXX
 *
 *  XXX make sure no required ops are NULL 
 *  XXX make sure no optional ops you are using are NULL 
 *  XXX unmount should be required.
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: mount.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <chain.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "libio_.h"

Chain_Control rtems_filesystem_mount_table_control;

#include "imfs.h"

/* XXX this structure should be in an IMFS specific file */
/* XXX this structure should use real constants */

rtems_filesystem_limits_and_options_t IMFS_LIMITS_AND_OPTIONS = {
   5,	/* link_max */
   6,	/* max_canon */
   7,	/* max_input */
   IMFS_NAME_MAX,	/* name_max */
   255,	/* path_max */
   2,	/* pipe_buf */
   1,	/* posix_async_io */
   2,	/* posix_chown_restrictions */
   3,	/* posix_no_trunc */
   4,	/* posix_prio_io */
   5,	/* posix_sync_io */
   6	/* posix_vdisable */
};

/*
 *  XXX
 */

int search_mt_for_mount_point(
  rtems_filesystem_location_info_t *location_of_mount_point
);

int init_fs_mount_table( void );


/*
 *  XXX
 */

#define FOUND      0
#define NOT_FOUND -1

/*
 *  mount
 *
 *  This routine will attempt to mount a new file system at the specified 
 *  mount point. A series of tests will be run to determine if any of the 
 *  following reasons exist to prevent the mount operation:
 *
 * 	1) The file system type or options are not valid
 *	2) No new file system root node is specified
 * 	3) The selected file system has already been mounted
 * 	4) The mount point exists with the proper permissions to allow mounting
 *	5) The selected mount point already has a file system mounted to it
 *
 */

int mount(
  rtems_filesystem_mount_table_entry_t **mt_entry,
  rtems_filesystem_operations_table    *fs_ops,
  rtems_filesystem_options_t            options,
  char                                 *device,
  char                                 *mount_point
)
{
  rtems_filesystem_location_info_t      loc;
  rtems_filesystem_mount_table_entry_t *temp_mt_entry;
  rtems_filesystem_location_info_t     *loc_to_free = NULL;

/* XXX add code to check for required operations */

  /*
   *  Is there a file system operations table?
   */

  if ( fs_ops == NULL ) {
    errno = EINVAL;
    return -1;
  }

  /*
   *  Are the file system options valid?
   */

  if ( options != RTEMS_FILESYSTEM_READ_ONLY && 
       options != RTEMS_FILESYSTEM_READ_WRITE ) {
    errno = EINVAL;
    return -1;
  }

  /*
   * Allocate a mount table entry 
   */

   temp_mt_entry = malloc( sizeof(rtems_filesystem_mount_table_entry_t) );

   if ( !temp_mt_entry ) {
     errno = ENOMEM;
     return -1;
   }

   temp_mt_entry->mt_fs_root.mt_entry = temp_mt_entry;
   temp_mt_entry->options = options;
   if ( device )
     strcpy( temp_mt_entry->dev, device );
   else
     temp_mt_entry->dev = 0;

  /*
   *  The mount_point should be a directory with read/write/execute
   *  permissions in the existing tree. 
   */

  if ( mount_point ) {

    if ( rtems_filesystem_evaluate_path( 
            mount_point, RTEMS_LIBIO_PERMS_RWX, &loc, TRUE ) == -1 )
      goto cleanup_and_bail;

    /* 
     *  Test to see if it is a directory
     */

    loc_to_free = &loc;
    if ( loc.ops->node_type( &loc ) != RTEMS_FILESYSTEM_DIRECTORY ) {
      errno = ENOTDIR;
      goto cleanup_and_bail;
    }

    /*
     *  You can only mount one file system onto a single mount point.
     */

    if ( search_mt_for_mount_point( &loc ) == FOUND ) {
      errno = EBUSY;
      goto cleanup_and_bail;
    }
 
    /*
     *  This must be a good mount point, so move the location information
     *  into the allocated mount entry.  Note:  the information that
     *  may have been allocated in loc should not be sent to freenode 
     *  until the system is unmounted.  It may be needed to correctly
     *  traverse the tree.
     */

    temp_mt_entry->mt_point_node.node_access = loc.node_access;
    temp_mt_entry->mt_point_node.handlers = loc.handlers;
    temp_mt_entry->mt_point_node.ops = loc.ops;
    temp_mt_entry->mt_point_node.mt_entry = loc.mt_entry;

    /* 
     *  This link to the parent is only done when we are dealing with system 
     *  below the base file system 
     */

    if ( !loc.ops->mount ){
      errno = ENOTSUP;
      goto cleanup_and_bail;
    }

    if ( loc.ops->mount( temp_mt_entry ) ) {
      goto cleanup_and_bail;
    }
  } else {

    /* 
     *  This is a mount of the base file system --> The 
     *  mt_point_node.node_access will be set to null to indicate that this 
     *  is the root of the entire file system.
     */

    temp_mt_entry->mt_fs_root.node_access = NULL;
    temp_mt_entry->mt_fs_root.handlers = NULL;
    temp_mt_entry->mt_fs_root.ops = NULL;

    temp_mt_entry->mt_point_node.node_access = NULL;
    temp_mt_entry->mt_point_node.handlers = NULL;
    temp_mt_entry->mt_point_node.ops = NULL;
    temp_mt_entry->mt_point_node.mt_entry = NULL;
  }

  if ( !fs_ops->fsmount_me ) {
    errno = ENOTSUP;
    goto cleanup_and_bail;
  }

  if ( fs_ops->fsmount_me( temp_mt_entry ) )
    goto cleanup_and_bail;

  /*
   *  Add the mount table entry to the mount table chain 
   */

  Chain_Append( &rtems_filesystem_mount_table_control, &temp_mt_entry->Node );

  *mt_entry = temp_mt_entry;

  return 0;

cleanup_and_bail:

  free( temp_mt_entry );
  
  if ( loc_to_free )
    rtems_filesystem_freenode( loc_to_free );

  return -1;
}



/*
 *  init_fs_mount_table
 *
 *  This routine will initialize the chain control element that manages the
 *  mount table chain.
 */

int init_fs_mount_table()
{
  Chain_Initialize_empty ( &rtems_filesystem_mount_table_control );
  return 0;
}


/*
 *  search_mt_for_mount_point
 *
 *  This routine will run through the entries that currently exist in the
 *  mount table chain. For each entry in the mount table chain it will
 *  compare the mount tables mt_point_node to the node describing the selected
 *  mount point.. If any of the mount table file system mount point nodes
 *  match the new file system selected mount point node, we are attempting
 *  to mount the new file system onto a node that already has a file system
 *  mounted to it. This is not a permitted operation.
 */

int search_mt_for_mount_point(
  rtems_filesystem_location_info_t *location_of_mount_point
)
{
  Chain_Node                           *the_node;
  rtems_filesystem_mount_table_entry_t *the_mount_entry;

  for ( the_node = rtems_filesystem_mount_table_control.first;
        !Chain_Is_tail( &rtems_filesystem_mount_table_control, the_node );
        the_node = the_node->next ) {

     the_mount_entry = (rtems_filesystem_mount_table_entry_t *) the_node;
     if ( the_mount_entry->mt_point_node.node_access  == 
             location_of_mount_point->node_access )
        return FOUND;
  }
  return NOT_FOUND;
}

