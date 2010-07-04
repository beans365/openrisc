/*
 *  $Id: console.h,v 1.2 2001-09-27 12:01:29 chris Exp $
 */

#ifndef _M860_CONSOLE_H_
#define _M860_CONSOLE_H_

#include <rtems/libio.h>

int m860_smc_set_attributes(int, const struct termios*);
int m860_scc_set_attributes(int, const struct termios*);
void m860_scc_initialize(int);
void m860_smc_initialize(int);
int m860_char_poll_read(int);
int m860_char_poll_write(int, const char*, int);
rtems_isr m860_scc1_console_interrupt_handler(rtems_vector_number);
rtems_isr m860_scc2_console_interrupt_handler(rtems_vector_number);
rtems_isr m860_scc3_console_interrupt_handler(rtems_vector_number);
rtems_isr m860_scc4_console_interrupt_handler(rtems_vector_number);
rtems_isr m860_smc1_console_interrupt_handler(rtems_vector_number);
rtems_isr m860_smc2_console_interrupt_handler(rtems_vector_number);
int m860_buf_poll_read(int, char**);
int m860_buf_poll_write(int, char*, int);
void m860_console_initialize(void);
rtems_device_driver m860_console_read(rtems_device_major_number,
				      rtems_device_minor_number,
				      void*);
rtems_device_driver m860_console_write(rtems_device_major_number,
				       rtems_device_minor_number,
				       void*);


typedef struct Buf_t_ {
  struct Buf_t_ *next;
  volatile char *buf;
  volatile int   len;
  int   pos;
} Buf_t;

#define SMC1_MINOR    0
#define SMC2_MINOR    1
#define SCC1_MINOR    2
#define SCC2_MINOR    3
#define SCC3_MINOR    4
#define SCC4_MINOR    5


#endif
