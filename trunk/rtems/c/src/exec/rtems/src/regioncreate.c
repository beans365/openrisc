/*
 *  Region Manager
 *
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: regioncreate.c,v 1.2 2001-09-27 11:59:19 chris Exp $
 */

#include <rtems/system.h>
#include <rtems/rtems/status.h>
#include <rtems/rtems/support.h>
#include <rtems/score/object.h>
#include <rtems/rtems/options.h>
#include <rtems/rtems/region.h>
#include <rtems/score/states.h>
#include <rtems/score/thread.h>

/*PAGE
 *
 *  rtems_region_create
 *
 *  This directive creates a region of physical contiguous memory area
 *  from which variable sized segments can be allocated.
 *
 *  Input parameters:
 *    name             - user defined region name
 *    starting_address - physical start address of region
 *    length           - physical length in bytes
 *    page_size        - page size in bytes
 *    attribute_set    - region attributes
 *    id               - address of region id to set
 *
 *  Output parameters:
 *    id       - region id
 *    RTEMS_SUCCESSFUL - if successful
 *    error code - if unsuccessful
 */

rtems_status_code rtems_region_create(
  rtems_name          name,
  void               *starting_address,
  unsigned32          length,
  unsigned32          page_size,
  rtems_attribute  attribute_set,
  Objects_Id         *id
)
{
  Region_Control *the_region;

  if ( !rtems_is_name_valid( name ) )
    return RTEMS_INVALID_NAME;

  if ( !_Addresses_Is_aligned( starting_address ) )
    return RTEMS_INVALID_ADDRESS;

  _Thread_Disable_dispatch();             /* to prevent deletion */

  the_region = _Region_Allocate();

  if ( !the_region ) {
    _Thread_Enable_dispatch();
    return RTEMS_TOO_MANY;
  }

  the_region->maximum_segment_size =
    _Heap_Initialize(&the_region->Memory, starting_address, length, page_size);

  if ( !the_region->maximum_segment_size ) {
    _Region_Free( the_region );
    _Thread_Enable_dispatch();
    return RTEMS_INVALID_SIZE;
  }

  the_region->starting_address      = starting_address;
  the_region->length                = length;
  the_region->page_size             = page_size;
  the_region->attribute_set         = attribute_set;
  the_region->number_of_used_blocks = 0;

  _Thread_queue_Initialize(
    &the_region->Wait_queue,
    OBJECTS_RTEMS_REGIONS,
    _Attributes_Is_priority( attribute_set ) ? 
       THREAD_QUEUE_DISCIPLINE_PRIORITY : THREAD_QUEUE_DISCIPLINE_FIFO,
    STATES_WAITING_FOR_SEGMENT,
#if defined(RTEMS_MULTIPROCESSING)
    _Region_MP_Send_extract_proxy,
#else
    NULL,
#endif
    RTEMS_TIMEOUT
  );

  _Objects_Open( &_Region_Information, &the_region->Object, &name );

  *id = the_region->Object.id;
  _Thread_Enable_dispatch();
  return RTEMS_SUCCESSFUL;
}
