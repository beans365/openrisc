/*
 *  Reserve enough resources to open every physical device once.
 *
 * $Id: termiosreserveresources.c,v 1.2 2001-09-27 12:01:15 chris Exp $
 */

#include <rtems.h>

#if 0
static int first_time;   /* assumed to be zeroed by BSS initialization */
#endif

void rtems_termios_reserve_resources (
  rtems_configuration_table *configuration,
  rtems_unsigned32           number_of_devices
)
{
#if 0
        rtems_api_configuration_table *rtems_config;


        if (!configuration)
                rtems_fatal_error_occurred (0xFFF0F001);
        rtems_config = configuration->RTEMS_api_configuration;
        if (!rtems_config)
                rtems_fatal_error_occurred (0xFFF0F002);
        if (!first_time)
                rtems_config->maximum_semaphores += 1;
        first_time = 1;
        rtems_config->maximum_semaphores += (4 * number_of_devices);
#endif
}

