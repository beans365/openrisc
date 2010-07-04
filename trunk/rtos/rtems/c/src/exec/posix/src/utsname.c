/*
 *  $Id: utsname.c,v 1.2 2001-09-27 11:59:17 chris Exp $
 */

#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <sys/utsname.h>


#include <rtems/system.h>
#include <rtems/score/object.h>
#include <rtems/sptables.h>

#include <rtems/posix/seterr.h>

/*PAGE
 *
 *  4.4.1 Get System Name, P1003.1b-1993, p. 90
 */

int uname(
  struct utsname *name
)
{
  /*  XXX: Here is what Solaris returns...
          sysname = SunOS
          nodename = node_name
          release = 5.3
          version = Generic_101318-12
          machine = sun4m
  */

  if ( !name )
    set_errno_and_return_minus_one( EFAULT );

  strcpy( name->sysname, "RTEMS" );

  sprintf( name->nodename, "Node %d", _Objects_Local_node );

  strcpy( name->release, RTEMS_VERSION );

  strcpy( name->version, "" );
 
  sprintf( name->machine, "%s/%s", CPU_NAME, CPU_MODEL_NAME );

  return 0;
}
