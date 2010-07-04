/*
 *  General Serial I/O functions.
 *
 * This file contains the functions for performing serial I/O.
 * The actual system calls (console_*) should be in the BSP part
 * of the source tree. That way different BSPs can use whichever
 * SMCs and SCCs they want. Originally, all the stuff was in
 * this file, and it caused problems with one BSP using SCC2 
 * as /dev/console, others using SMC1 for /dev/console, etc.
 *
 *  On-chip resources used:
 *   resource   minor                note
 *    SMC1       0
 *    SMC2       1
 *    SCC1       2                   (shared with ethernet driver)
 *    SCC2       3
 *    SCC3       4
 *    SCC4       5
 *    BRG1
 *    BRG2
 *    BRG3
 *    BRG4
 *  Author: Jay Monkman (jmonkman@frasca.com)
 *  Copyright (C) 1998 by Frasca International, Inc.
 *
 *  Derived from c/src/lib/libbsp/m68k/gen360/console/console.c:
 *
 *  Author:
 *    W. Eric Norum
 *    Saskatchewan Accelerator Laboratory
 *    University of Saskatchewan
 *    Saskatoon, Saskatchewan, CANADA
 *    eric@skatter.usask.ca
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id: console-generic.c,v 1.2 2001-09-27 12:01:29 chris Exp $
 */

#include <rtems.h>
#include <rtems/libio.h>
#include <mpc860.h>
#include <mpc860/console.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define NIFACES 6        /* number of console devices (serial ports) */

static Buf_t *rxBufList[NIFACES];
static Buf_t *rxBufListTail[NIFACES];

/*
 * Interrupt-driven input buffer
 */
#define RXBUFSIZE       16


/*
 * I/O buffers and pointers to buffer descriptors
 */
static volatile char txBuf[NIFACES];

static volatile m860BufferDescriptor_t *RxBd[NIFACES], *TxBd[NIFACES];

/*
 * Device-specific routines
 */
static int m860_get_brg_cd(int);
unsigned char m860_get_brg_clk(int);
unsigned char m860_get_brg_clk(int);


/*
 * Compute baud-rate-generator configuration register value
 */
static int
m860_get_brg_cd (int baud)
{
  int divisor;
  int div16 = 0;
  
  divisor = ((rtems_cpu_configuration_get_clock_speed() / 16) +
                 (baud / 2)) / baud;
  if (divisor > 4096) {
    div16 = 1;
    divisor = (divisor + 8) / 16;
  }
  return M860_BRG_EN | M860_BRG_EXTC_BRGCLK | 
    ((divisor - 1) << 1) | div16;
}


/* this function will fail if more that 4 baud rates have been selected */
/*  at any time since the OS started. It needs to be fixed. FIXME */
unsigned char m860_get_brg_clk(int baud)
{
  static int  brg_spd[4];
  static char brg_used[4];
  int i;

  /* first try to find a BRG that is already at the right speed */
  for (i=0; i<4; i++) {
    if (brg_spd[i] == baud) {
      break;
    }
  }

  if (i==4) { /* I guess we didn't find one */
    for (i=0; i<4; i++) {
      if (brg_used[i] == 0) {
        break;
      }
    }
  }
  if (i != 4) {
    brg_used[i]++;
    brg_spd[i]=baud;
    switch (i) {
    case 0:
      m860.brgc1 = M860_BRG_RST;
      m860.brgc1 = m860_get_brg_cd(baud);
      break;
    case 1:
      m860.brgc2 = M860_BRG_RST;
      m860.brgc2 = m860_get_brg_cd(baud);
      break;
    case 2:
      m860.brgc3 = M860_BRG_RST;
      m860.brgc3 = m860_get_brg_cd(baud);
      break;
    case 3:
      m860.brgc4 = M860_BRG_RST;
      m860.brgc4 = m860_get_brg_cd(baud);
      break;
    }
    return i;
  }

  else 
    return 0xff;
}

/*
 * Hardware-dependent portion of tcsetattr().
 */
int
m860_smc_set_attributes (int minor, const struct termios *t)
{
  /*
   * minor must be 0 or 1
   */
  int baud;
  int brg;
  switch (t->c_cflag & CBAUD) {
  default:      baud = -1;      break;
  case B50:     baud = 50;      break;
  case B75:     baud = 75;      break;
  case B110:    baud = 110;     break;
  case B134:    baud = 134;     break;
  case B150:    baud = 150;     break;
  case B200:    baud = 200;     break;
  case B300:    baud = 300;     break;
  case B600:    baud = 600;     break;
  case B1200:   baud = 1200;    break;
  case B1800:   baud = 1800;    break;
  case B2400:   baud = 2400;    break;
  case B4800:   baud = 4800;    break;
  case B9600:   baud = 9600;    break;
  case B19200:  baud = 19200;   break;
  case B38400:  baud = 38400;   break;
  case B57600:  baud = 57600;   break;
  case B115200: baud = 115200;  break;
  case B230400: baud = 230400;  break;
  case B460800: baud = 460800;  break;
  }
  if (baud > 0) {
    brg = m860_get_brg_clk(baud);     /* 4 BRGs, 6 serial ports - hopefully */ 
                                      /*  at least 2 ports will be the same */
    m860.simode |= brg << (12 + ((minor) * 16));
  }
  return 0;
}

int
m860_scc_set_attributes (int minor, const struct termios *t)
{
  /*
   * minor must be 2, 3, 4 or 5
   */
  int baud;
  int brg;
  switch (t->c_cflag & CBAUD) {
  default:      baud = -1;      break;
  case B50:     baud = 50;      break;
  case B75:     baud = 75;      break;
  case B110:    baud = 110;     break;
  case B134:    baud = 134;     break;
  case B150:    baud = 150;     break;
  case B200:    baud = 200;     break;
  case B300:    baud = 300;     break;
  case B600:    baud = 600;     break;
  case B1200:   baud = 1200;    break;
  case B1800:   baud = 1800;    break;
  case B2400:   baud = 2400;    break;
  case B4800:   baud = 4800;    break;
  case B9600:   baud = 9600;    break;
  case B19200:  baud = 19200;   break;
  case B38400:  baud = 38400;   break;
  case B57600:  baud = 57600;   break;
  case B115200: baud = 115200;  break;
  case B230400: baud = 230400;  break;
  case B460800: baud = 460800;  break;
  }
  if (baud > 0) {
    brg = m860_get_brg_clk(baud);     /* 4 BRGs, 5 serial ports - hopefully */ 
                                      /*  at least 2 ports will be the same */
    m860.sicr |= (brg << (3 + ((minor-2) * 8))) | 
                 (brg << ((minor-2) * 8));
  }
  return 0;
}

void
m860_scc_initialize (int port)  /* port is the SCC # (i.e. 1, 2, 3 or 4) */
{
  unsigned char                brg;
  volatile m860SCCparms_t     *sccparms;
  volatile m860SCCRegisters_t *sccregs;

  /*
   * Allocate buffer descriptors
   */
  RxBd[port+1] = M860AllocateBufferDescriptors(1);
  TxBd[port+1] = M860AllocateBufferDescriptors(1);
  
  /*
   * Configure ports A and B to enable TXDx and RXDx pins
   */
  m860.papar |=  (0xC << ((port-2) * 2));
  m860.padir &= ~(0xC << ((port-2) * 2));
  m860.pbdir |=  (0x04 << (port-2));
  m860.paodr &= ~(0x8 << ((port-2) * 2));
  m860.pbdat &= ~(0x04 << (port-2));
  
  /* SCC2 is the only one with handshaking lines */
  /*
    if (port == 2) {
    m860.pcpar |= (0x02);
    m860.pcpar &= ~(0xc0);
    m860.pcdir &= ~(0xc2);
    m860.pcso  |= (0xc0);
    }
  */
  
  brg = m860_get_brg_clk(9600);     /* 4 BRGs, 5 serial ports - hopefully */ 
                                    /*  at least 2 ports will be the same */
  
  /*
   * Set up SDMA 
   */
  m860.sdcr = 0x01; /* as recommended p 16-80, sec 16.10.2.1 MPC860UM/AD */
  

  m860.sicr &= ~(0xff << ((port-1) * 8));
  m860.sicr |= (brg << (3 + ((port-1) * 8))) | (brg << ((port-1) * 8));
  
  /*
   * Set up SMC1 parameter RAM common to all protocols
   */
  if (port == 1) {
    sccparms = (m860SCCparms_t*)&m860.scc1p;
    sccregs = &m860.scc1;
  }
  else if (port == 2) {
    sccparms = &m860.scc2p;
    sccregs = &m860.scc2;
  }
  else if (port == 3) {
    sccparms = &m860.scc3p;
    sccregs = &m860.scc3;
  }
  else {
    sccparms = &m860.scc4p;
    sccregs = &m860.scc4;
  }
  
  sccparms->rbase = (char *)RxBd[port+1] - (char *)&m860;
  sccparms->tbase = (char *)TxBd[port+1] - (char *)&m860;
  
  if (port == 1)
    M860ExecuteRISC (M860_CR_OP_INIT_RX_TX | M860_CR_CHAN_SCC1);
  else if (port == 2)
    M860ExecuteRISC (M860_CR_OP_INIT_RX_TX | M860_CR_CHAN_SCC2);
  else if (port == 3)
    M860ExecuteRISC (M860_CR_OP_INIT_RX_TX | M860_CR_CHAN_SCC3);
  else if (port == 4)
    M860ExecuteRISC (M860_CR_OP_INIT_RX_TX | M860_CR_CHAN_SCC4);
  
  sccparms->rfcr = M860_RFCR_MOT | M860_RFCR_DMA_SPACE(0);
  sccparms->tfcr = M860_TFCR_MOT | M860_TFCR_DMA_SPACE(0);
  sccparms->mrblr = RXBUFSIZE;
  
  sccparms->un.uart.max_idl = 10;
  sccparms->un.uart.brklen = 0;
  sccparms->un.uart.brkec = 0;
  sccparms->un.uart.brkcr = 1;
  
  sccparms->un.uart.parec = 0;
  sccparms->un.uart.frmec = 0;
  sccparms->un.uart.nosec = 0;
  
  sccparms->un.uart.uaddr[0] = 0;
  sccparms->un.uart.uaddr[1] = 0;
  sccparms->un.uart.toseq  = 0;
  
  sccparms->un.uart.character[0] = 0x8000;
  sccparms->un.uart.character[1] = 0x8000;
  sccparms->un.uart.character[2] = 0x8000;
  sccparms->un.uart.character[3] = 0x8000;
  sccparms->un.uart.character[4] = 0x8000;
  sccparms->un.uart.character[5] = 0x8000;
  sccparms->un.uart.character[6] = 0x8000;
  sccparms->un.uart.character[7] = 0x8000;
  
  sccparms->un.uart.rccm = 0xc0ff;
  
  /*
   * Set up the Receive Buffer Descriptor
   */
  RxBd[port+1]->status = M860_BD_EMPTY | M860_BD_WRAP | 
    M860_BD_INTERRUPT;
  RxBd[port+1]->length = 0;
  RxBd[port+1]->buffer = malloc(RXBUFSIZE);
  
  /*
   * Setup the Transmit Buffer Descriptor
   */
  TxBd[port+1]->status = M860_BD_WRAP;
  
  /*
   * Set up SCCx general and protocol-specific mode registers
   */
  sccregs->scce = 0xffff;
  sccregs->sccm = 0x0000;
  sccregs->gsmr_h = 0x00000020;
  sccregs->gsmr_l = 0x00028004;
  sccregs->psmr   = 0x3000;
  sccregs->gsmr_l = 0x00028034;
}

void
m860_smc_initialize (int port)  /* port is the SMC number (i.e. 1 or 2) */
{
  unsigned char brg;

  /*
   * Allocate buffer descriptors
   */
  RxBd[port-1] = M860AllocateBufferDescriptors (1);
  TxBd[port-1] = M860AllocateBufferDescriptors (1);

  /*
   * Configure port B pins to enable SMTXDx and SMRXDx pins
   */
  m860.pbpar |=  (0xC0 << ((port-1) * 4));
  m860.pbdir &= ~(0xC0 << ((port-1) * 4));
  m860.pbdir |=  (0x01 << (port-1));
  m860.pbodr &= ~(0xC0 << ((port-1) * 4));
  m860.pbdat &= ~(0x01 << (port-1));
  
  /*
   * Set up BRG1 (9,600 baud)
   */
  brg = m860_get_brg_clk(9600);     /* 4 BRGs, 5 serial ports - hopefully */ 
                                    /*  at least 2 ports will be the same */

  /*
   * Put SMC in NMSI mode, connect SMC to BRG
   */
  m860.simode &= ~(0x7000 << ((port-1) * 16));
  m860.simode |= brg << (12 + ((port-1) * 16));

  /*
   * Set up SMC1 parameter RAM common to all protocols
   */
  if (port == 1) {
    m860.smc1p.rbase = (char *)RxBd[port-1] - (char *)&m860;
    m860.smc1p.tbase = (char *)TxBd[port-1] - (char *)&m860;
    m860.smc1p.rfcr = M860_RFCR_MOT | M860_RFCR_DMA_SPACE(0);
    m860.smc1p.tfcr = M860_TFCR_MOT | M860_TFCR_DMA_SPACE(0);
    m860.smc1p.mrblr = RXBUFSIZE;
    
    /*
     * Set up SMC1 parameter RAM UART-specific parameters
     */
    m860.smc1p.un.uart.max_idl = 10;
    m860.smc1p.un.uart.brklen = 0;
    m860.smc1p.un.uart.brkec = 0;
    m860.smc1p.un.uart.brkcr = 0;
    
  }
  else {
    m860.smc2p.rbase = (char *)RxBd[port-1] - (char *)&m860;
    m860.smc2p.tbase = (char *)TxBd[port-1] - (char *)&m860;
    m860.smc2p.rfcr = M860_RFCR_MOT | M860_RFCR_DMA_SPACE(0);
    m860.smc2p.tfcr = M860_TFCR_MOT | M860_TFCR_DMA_SPACE(0);
    m860.smc2p.mrblr = RXBUFSIZE;
    
    /*
     * Set up SMC2 parameter RAM UART-specific parameters
     */
    m860.smc2p.un.uart.max_idl = 10;
    m860.smc2p.un.uart.brklen = 0;
    m860.smc2p.un.uart.brkec = 0;
    m860.smc2p.un.uart.brkcr = 0;
  }      
  
  /*
   * Set up the Receive Buffer Descriptor
   */
  RxBd[port-1]->status = M860_BD_EMPTY | M860_BD_WRAP | 
                                  M860_BD_INTERRUPT;
  RxBd[port-1]->length = 0;
  RxBd[port-1]->buffer = malloc(RXBUFSIZE);
         
  /*
   * Setup the Transmit Buffer Descriptor
   */
  TxBd[port-1]->status = M860_BD_WRAP;
  
  /*
   * Set up SMCx general and protocol-specific mode registers
   */
  if (port == 1) {
    m860.smc1.smce = ~0;        /* Clear any pending events */
    m860.smc1.smcm = 0; /* Mask all interrupt/event sources */
    m860.smc1.smcmr = M860_SMCMR_CLEN(9) | M860_SMCMR_SM_UART;
    
    /*
     * Send "Init parameters" command
     */
    M860ExecuteRISC (M860_CR_OP_INIT_RX_TX | M860_CR_CHAN_SMC1);
    
    /*
     * Enable receiver and transmitter
     */
    m860.smc1.smcmr |= M860_SMCMR_TEN | M860_SMCMR_REN;
  }
  else {
    m860.smc2.smce = ~0;        /* Clear any pending events */
    m860.smc2.smcm = 0; /* Mask all interrupt/event sources */
    m860.smc2.smcmr = M860_SMCMR_CLEN(9) | M860_SMCMR_SM_UART;
    
    /*
     * Send "Init parameters" command
     */
    M860ExecuteRISC (M860_CR_OP_INIT_RX_TX | M860_CR_CHAN_SMC2);
    
    /*
     * Enable receiver and transmitter
     */
    m860.smc2.smcmr |= M860_SMCMR_TEN | M860_SMCMR_REN;
  }
}

int
m860_char_poll_read (int minor)
{
  unsigned char c;
  rtems_unsigned32 level;

  _CPU_ISR_Disable(level);  
  if (RxBd[minor]->status & M860_BD_EMPTY) {
    _CPU_ISR_Enable(level);
    return -1;
  }
  c = ((char *)RxBd[minor]->buffer)[0];
  RxBd[minor]->status = M860_BD_EMPTY | M860_BD_WRAP;
  _CPU_ISR_Enable(level);
  return c;
}

int
m860_char_poll_write (int minor, const char *buf, int len)
{
  while (len--) {
    while (TxBd[minor]->status & M860_BD_READY)
      continue;
    txBuf[minor] = *buf++;
    TxBd[minor]->buffer = &txBuf[minor];
    TxBd[minor]->length = 1;
    TxBd[minor]->status = M860_BD_READY | M860_BD_WRAP;
  }
  return 0;
}

/*
 * Interrupt handler
 */
rtems_isr
m860_scc1_console_interrupt_handler (rtems_vector_number v)
{
  /*
   * Buffer received?
   */
  if ((m860.scc1.sccm & 0x1) && (m860.scc1.scce & 0x1)) {
    m860.scc1.scce = 0x1;
    /*    m860.scc1.sccm &= ~0x1;*/
    
    while ((RxBd[SCC1_MINOR]->status & M860_BD_EMPTY) == 0) {
      rxBufListTail[SCC1_MINOR]->next = malloc(sizeof(Buf_t));
      if (rxBufListTail[SCC1_MINOR]->next) {
        rxBufListTail[SCC1_MINOR] = rxBufListTail[SCC1_MINOR]->next;
        rxBufListTail[SCC1_MINOR]->buf = RxBd[SCC1_MINOR]->buffer;
        rxBufListTail[SCC1_MINOR]->len = RxBd[SCC1_MINOR]->length;
        rxBufListTail[SCC1_MINOR]->pos = 0;
        rxBufListTail[SCC1_MINOR]->next = 0;

        RxBd[SCC1_MINOR]->buffer = malloc(RXBUFSIZE);
      }
      RxBd[SCC1_MINOR]->status = M860_BD_EMPTY | M860_BD_WRAP | 
                                 M860_BD_INTERRUPT;
    }
  }
  
  /*
   * Buffer transmitted?
   */
#if 0
  if (m860.smc1.smce & 0x2) {
    m860.smc1.smce = 0x2;
    if ((smcTxBd->status & M360_BD_READY) == 0)
      rtems_termios_dequeue_characters (smc1ttyp, smcTxBd->length);
  }
#endif
  m860.cisr = 1UL << 30;  /* Clear SCC1 interrupt-in-service bit */
}

rtems_isr
m860_scc2_console_interrupt_handler (rtems_vector_number v)
{
  /*
   * Buffer received?
   */
  if ((m860.scc2.sccm & 0x1) && (m860.scc2.scce & 0x1)) {
    m860.scc2.scce = 0x1;
    /*    m860.scc2.sccm &= ~0x1;*/
    
    while ((RxBd[SCC2_MINOR]->status & M860_BD_EMPTY) == 0) {
      rxBufListTail[SCC2_MINOR]->next = malloc(sizeof(Buf_t));
      if (rxBufListTail[SCC2_MINOR]->next) {
        rxBufListTail[SCC2_MINOR] = rxBufListTail[SCC2_MINOR]->next;
        rxBufListTail[SCC2_MINOR]->buf = RxBd[SCC2_MINOR]->buffer;
        rxBufListTail[SCC2_MINOR]->len = RxBd[SCC2_MINOR]->length;
        rxBufListTail[SCC2_MINOR]->pos = 0;
        rxBufListTail[SCC2_MINOR]->next = 0;

        RxBd[SCC2_MINOR]->buffer = malloc(RXBUFSIZE);
      }
      RxBd[SCC2_MINOR]->status = M860_BD_EMPTY | M860_BD_WRAP | 
                                 M860_BD_INTERRUPT;
    }
  }
  
  /*
   * Buffer transmitted?
   */
#if 0
  if (m860.smc1.smce & 0x2) {
    m860.smc1.smce = 0x2;
    if ((smcTxBd->status & M360_BD_READY) == 0)
      rtems_termios_dequeue_characters (smc1ttyp, smcTxBd->length);
  }
#endif
  m860.cisr = 1UL << 29;  /* Clear SCC2 interrupt-in-service bit */
}

rtems_isr
m860_scc3_console_interrupt_handler (rtems_vector_number v)
{
  /*
   * Buffer received?
   */
  if ((m860.scc3.sccm & 0x1) && (m860.scc3.scce & 0x1)) {
    m860.scc3.scce = 0x1;
    /*    m860.scc3.sccm &= ~0x1;*/
    
    while ((RxBd[SCC3_MINOR]->status & M860_BD_EMPTY) == 0) {
      rxBufListTail[SCC3_MINOR]->next = malloc(sizeof(Buf_t));
      if (rxBufListTail[SCC3_MINOR]->next) {
        rxBufListTail[SCC3_MINOR] = rxBufListTail[SCC3_MINOR]->next;
        rxBufListTail[SCC3_MINOR]->buf = RxBd[SCC3_MINOR]->buffer;
        rxBufListTail[SCC3_MINOR]->len = RxBd[SCC3_MINOR]->length;
        rxBufListTail[SCC3_MINOR]->pos = 0;
        rxBufListTail[SCC3_MINOR]->next = 0;

        RxBd[SCC3_MINOR]->buffer = malloc(RXBUFSIZE);
      }
      RxBd[SCC3_MINOR]->status = M860_BD_EMPTY | M860_BD_WRAP | 
                                 M860_BD_INTERRUPT;
    }
  }
  
  /*
   * Buffer transmitted?
   */
#if 0
  if (m860.smc1.smce & 0x2) {
    m860.smc1.smce = 0x2;
    if ((smcTxBd->status & M360_BD_READY) == 0)
      rtems_termios_dequeue_characters (smc1ttyp, smcTxBd->length);
  }
#endif
  m860.cisr = 1UL << 28;  /* Clear SCC3 interrupt-in-service bit */
}

rtems_isr
m860_scc4_console_interrupt_handler (rtems_vector_number v)
{
  /*
   * Buffer received?
   */
  if ((m860.scc4.sccm & 0x1) && (m860.scc4.scce & 0x1)) {
    m860.scc4.scce = 0x1;
    /*    m860.scc4.sccm &= ~0x1;*/
    
    while ((RxBd[SCC4_MINOR]->status & M860_BD_EMPTY) == 0) {
      rxBufListTail[SCC4_MINOR]->next = malloc(sizeof(Buf_t));
      if (rxBufListTail[SCC4_MINOR]->next) {
        rxBufListTail[SCC4_MINOR] = rxBufListTail[SCC4_MINOR]->next;
        rxBufListTail[SCC4_MINOR]->buf = RxBd[SCC4_MINOR]->buffer;
        rxBufListTail[SCC4_MINOR]->len = RxBd[SCC4_MINOR]->length;
        rxBufListTail[SCC4_MINOR]->pos = 0;
        rxBufListTail[SCC4_MINOR]->next = 0;

        RxBd[SCC4_MINOR]->buffer = malloc(RXBUFSIZE);
      }
      RxBd[SCC4_MINOR]->status = M860_BD_EMPTY | M860_BD_WRAP | 
                                 M860_BD_INTERRUPT;
    }
  }
  
  /*
   * Buffer transmitted?
   */
#if 0
  if (m860.smc1.smce & 0x2) {
    m860.smc1.smce = 0x2;
    if ((smcTxBd->status & M360_BD_READY) == 0)
      rtems_termios_dequeue_characters (smc1ttyp, smcTxBd->length);
  }
#endif
  m860.cisr = 1UL << 27;  /* Clear SCC4 interrupt-in-service bit */
}

rtems_isr
m860_smc1_console_interrupt_handler (rtems_vector_number v)
{
  /*
   * Buffer received?
   */
  if (m860.smc1.smce & 0x1) {
    m860.smc1.smce = 0x1;
    /*    m860.scc2.sccm &= ~0x1;*/
    
    while ((RxBd[SMC1_MINOR]->status & M860_BD_EMPTY) == 0) {
      rxBufListTail[SMC1_MINOR]->next = malloc(sizeof(Buf_t));
      if (rxBufListTail[SMC1_MINOR]->next) {
        rxBufListTail[SMC1_MINOR] = rxBufListTail[SMC1_MINOR]->next;
        rxBufListTail[SMC1_MINOR]->buf = RxBd[SMC1_MINOR]->buffer;
        rxBufListTail[SMC1_MINOR]->len = RxBd[SMC1_MINOR]->length;
        rxBufListTail[SMC1_MINOR]->pos = 0;
        rxBufListTail[SMC1_MINOR]->next = 0;

        RxBd[SMC1_MINOR]->buffer = malloc(RXBUFSIZE);
      }
      RxBd[SMC1_MINOR]->status = M860_BD_EMPTY | M860_BD_WRAP | 
                                 M860_BD_INTERRUPT;
    }
  }
  
  /*
   * Buffer transmitted?
   */
#if 0
  if (m860.smc1.smce & 0x2) {
    m860.smc1.smce = 0x2;
    if ((smcTxBd->status & M360_BD_READY) == 0)
      rtems_termios_dequeue_characters (smc1ttyp, smcTxBd->length);
  }
#endif
  m860.cisr = 1UL << 4;  /* Clear SMC1 interrupt-in-service bit */
}

rtems_isr
m860_smc2_console_interrupt_handler (rtems_vector_number v)
{
  /*
   * Buffer received?
   */
  if (m860.smc2.smce & 0x1) {
    m860.smc2.smce = 0x1;
    
    while ((RxBd[SMC2_MINOR]->status & M860_BD_EMPTY) == 0) {
      rxBufListTail[SMC2_MINOR]->next = malloc(sizeof(Buf_t));
      if (rxBufListTail[SMC2_MINOR]->next) {
        rxBufListTail[SMC2_MINOR] = rxBufListTail[SMC2_MINOR]->next;
        rxBufListTail[SMC2_MINOR]->buf = RxBd[SMC2_MINOR]->buffer;
        rxBufListTail[SMC2_MINOR]->len = RxBd[SMC2_MINOR]->length;
        rxBufListTail[SMC2_MINOR]->pos = 0;
        rxBufListTail[SMC2_MINOR]->next = 0;

        RxBd[SMC2_MINOR]->buffer = malloc(RXBUFSIZE);
      }
      RxBd[SMC2_MINOR]->status = M860_BD_EMPTY | M860_BD_WRAP | 
                                 M860_BD_INTERRUPT;
    }
  }
  
  /*
   * Buffer transmitted?
   */
#if 0
  if (m860.smc1.smce & 0x2) {
    m860.smc1.smce = 0x2;
    if ((smcTxBd->status & M360_BD_READY) == 0)
      rtems_termios_dequeue_characters (smc1ttyp, smcTxBd->length);
  }
#endif
  m860.cisr = 1UL << 3;  /* Clear SMC2 interrupt-in-service bit */
}


int
m860_buf_poll_read (int minor, char **buf)
{
  int len;

  if (RxBd[minor]->status & M860_BD_EMPTY)
    return -1;
  
  RxBd[minor]->buffer = malloc(RXBUFSIZE); /* I hope this succeeds ... */
  len = RxBd[minor]->length;
  RxBd[minor]->status = M860_BD_EMPTY | M860_BD_WRAP;
  
  return len;
}

int
m860_buf_poll_write (int minor, char *buf, int len)
{
  static char *last_buf[6];

  while (TxBd[minor]->status & M860_BD_READY)
    continue;
  if (last_buf[minor])
    free(last_buf[minor]);
  last_buf[minor] = buf;
  TxBd[minor]->buffer = buf;
  TxBd[minor]->length = len;
  TxBd[minor]->status = M860_BD_READY | M860_BD_WRAP;
  return 0;
}

void m860_console_initialize(void)
{
  int i;

  for (i=0; i < NIFACES; i++) {
    rxBufList[i] = malloc(sizeof(Buf_t));
    rxBufListTail[i] = rxBufList[i];
    rxBufList[i]->buf  = 0;
    rxBufList[i]->len  = 0;
    rxBufList[i]->pos  = 0;
    rxBufList[i]->next = 0;
  }
}

rtems_device_driver m860_console_read(rtems_device_major_number major,
                                      rtems_device_minor_number minor,
                                      void *arg)
{
  rtems_libio_rw_args_t *rw_args;
  char *buffer;
  int maximum;
  int count;
  Buf_t *tmp_buf;
  rtems_unsigned32 level;

  /*
   * Set up interrupts
   * FIXME: DANGER: WARNING:
   * CICR and SIMASK must be set in any module that uses
   *   the CPM. Currently those are console-generic.c and
   *   network.c. If the registers are not set the same
   *   in both places, strange things may happen.
   *   If they are only set in one place, then an application
   *   that used the other module won't work correctly.
   *   Put this comment in each module that sets these 2 registers
   */
  m860.cicr = 0x00e43e80;   /* SCaP=SCC1, SCbP=SCC2, SCcP=SCC3, 
			       SCdP=SCC4, IRL=1, HP=SCC1, IEN=1 */
  m860.simask |= M860_SIMASK_LVM1; 

  rw_args = (rtems_libio_rw_args_t *) arg;
  buffer = rw_args->buffer;
  maximum = rw_args->count;
  count = 0;

  while (count == 0) {
    if (rxBufList[minor]->len) {
      while ((count < maximum) && 
             (rxBufList[minor]->pos < rxBufList[minor]->len)) {
        buffer[count++] = rxBufList[minor]->buf[rxBufList[minor]->pos++];
      }
      _CPU_ISR_Disable(level);
      if (rxBufList[minor]->pos == rxBufList[minor]->len) {
        if (rxBufList[minor]->next) {
          tmp_buf=rxBufList[minor]->next;
          free ((void *) rxBufList[minor]->buf);
          free ((void *) rxBufList[minor]);
          rxBufList[minor]=tmp_buf;
        }
        else {
          free(rxBufList[minor]->buf);
          rxBufList[minor]->buf=0;
          rxBufList[minor]->len=0;
          rxBufList[minor]->pos=0;
        }
      }
      _CPU_ISR_Enable(level);
    }
    else
      if(rxBufList[minor]->next && !rxBufList[minor]->len) {
        tmp_buf = rxBufList[minor];
        rxBufList[minor] = rxBufList[minor]->next;
        free(tmp_buf);
      }
    /*      sleep(1);*/
  }
  rw_args->bytes_moved = count;
  return (count >= 0) ? RTEMS_SUCCESSFUL : RTEMS_UNSATISFIED;
}
  
rtems_device_driver m860_console_write(rtems_device_major_number major,
                                       rtems_device_minor_number minor,
                                       void *arg)
{
  int count;
  int maximum;
  rtems_libio_rw_args_t *rw_args;
  char *in_buffer;
  char *out_buffer;
  int n;

  /*
   * Set up interrupts
   * FIXME: DANGER: WARNING:
   * CICR and SIMASK must be set in any module that uses
   *   the CPM. Currently those are console-generic.c and
   *   network.c. If the registers are not set the same
   *   in both places, strange things may happen.
   *   If they are only set in one place, then an application
   *   that used the other module won't work correctly.
   *   Put this comment in each module that sets these 2 registers
   */
#if 0
    m860.cicr = 0x00e43e80;   /* SCaP=SCC1, SCbP=SCC2, SCcP=SCC3, 
			       SCdP=SCC4, IRL=1, HP=SCC1, IEN=1 */
    m860.simask |= M860_SIMASK_LVM1;
#endif

  rw_args = (rtems_libio_rw_args_t *) arg;

  in_buffer = rw_args->buffer;
  maximum = rw_args->count;

  out_buffer = malloc(maximum*2);  /* This is wasteful, but it won't */
                                   /*  be too small */

  if (!out_buffer) {
    rw_args->bytes_moved = 0;
    return RTEMS_NO_MEMORY;
  }
  n=0;
  for (count = 0; count < maximum; count++) {
    if ( in_buffer[ count ] == '\n') {
      out_buffer[count + n] = '\r';
      n++;
    }
    out_buffer[count + n] = in_buffer[count];
  }
  m860_buf_poll_write(minor, out_buffer, maximum+n);
  rw_args->bytes_moved = maximum;
  return RTEMS_SUCCESSFUL;
}


/*
 *  How to use the console. 
 *   In your BSP, have the following functions:
 *
 *  rtems_device_driver console_initialize(rtems_device_major_number  major,
 *                                         rtems_device_minor_number  minor,
 *                                         void *arg)
 *  rtems_device_driver console_open(rtems_device_major_number major,
 *                                   rtems_device_minor_number minor,
 *                                   void *arg)
 *  rtems_device_driver console_close(rtems_device_major_number major,
 *                                    rtems_device_minor_number minor,
 *                                    void *arg)
 *  rtems_device_driver console_read(rtems_device_major_number major,
 *                                   rtems_device_minor_number minor,
 *                                   void *arg)
 *  rtems_device_driver console_write(rtems_device_major_number major,
 *                                    rtems_device_minor_number minor,
 *                                    void *arg)
 *  rtems_device_driver console_control(rtems_device_major_number major,
 *                                      rtems_device_minor_number minor,
 *                                      void *arg)
 *
 */
