/*
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: mc68681.h,v 1.2 2001-09-27 12:01:42 chris Exp $
 */

#ifndef _MC68681_H_
#define _MC68681_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  These are just used in the interface between this driver and
 *  the read/write register routines when accessing the first
 *  control port.
 */

#define MC68681_STATUS          1
#define MC68681_RX_BUFFER       3

#define MC68681_MODE            0
#define MC68681_CLOCK_SELECT    1
#define MC68681_COMMAND         2
#define MC68681_TX_BUFFER       3

/*
 *  Data Port bit map configuration
 *
 *   D0      : Baud Rate Set Selection
 *   D1 - D2 : Extended Baud Rate Setting
 */

#define MC68681_DATA_BAUD_RATE_SET_1      0  /* ACR[7] = 0 */
#define MC68681_DATA_BAUD_RATE_SET_2      1  /* ACR[7] = 1 */

#define MC68681_XBRG_IGNORED              (0 << 1)
#define MC68681_XBRG_ENABLED              (1 << 1)
#define MC68681_XBRG_DISABLED             (2 << 1)
#define MC68681_XBRG_MASK                 (3 << 1)

/*
 *  Custom baud rate table information
 */

typedef unsigned char mc68681_baud_t;
typedef mc68681_baud_t mc68681_baud_table_t[RTEMS_TERMIOS_NUMBER_BAUD_RATES];

#define MC68681_BAUD_NOT_VALID 0xFF

extern mc68681_baud_t 
    mc68681_baud_rate_table[4][RTEMS_TERMIOS_NUMBER_BAUD_RATES];


/*
 * Driver function table
 */

extern console_fns mc68681_fns;
extern console_fns mc68681_fns_polled;

/*
 * Default register access routines
 */

unsigned8 mc68681_get_register(     /* registers are at 1 byte boundaries */
  unsigned32  ulCtrlPort,           /*   and accessed as bytes            */
  unsigned8   ucRegNum
);

void  mc68681_set_register(
  unsigned32  ulCtrlPort,
  unsigned8   ucRegNum,
  unsigned8   ucData
);

unsigned8 mc68681_get_register_2(   /* registers are at 2 byte boundaries */
  unsigned32  ulCtrlPort,           /*   and accessed as bytes            */
  unsigned8   ucRegNum
);

void  mc68681_set_register_2(
  unsigned32  ulCtrlPort,
  unsigned8   ucRegNum,
  unsigned8   ucData
);

unsigned8 mc68681_get_register_4(   /* registers are at 4 byte boundaries */
  unsigned32  ulCtrlPort,           /*   and accessed as bytes            */
  unsigned8   ucRegNum
);

void  mc68681_set_register_4(
  unsigned32  ulCtrlPort,
  unsigned8   ucRegNum,
  unsigned8   ucData
);

unsigned8 mc68681_get_register_8(   /* registers are at 8 byte boundaries */
  unsigned32  ulCtrlPort,           /*   and accessed as bytes            */
  unsigned8   ucRegNum
);

void  mc68681_set_register_8(
  unsigned32  ulCtrlPort,
  unsigned8   ucRegNum,
  unsigned8   ucData
);


#ifdef __cplusplus
}
#endif

#endif /* _MC68681_H_ */
