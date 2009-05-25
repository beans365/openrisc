/*
 *  RTEMS driver for TULIP based Ethernet Controller
 *
 *  Copyright (C) 1999 Emmanuel Raguet. raguet@crf.canon.fr
 *
 *  The license and distribution terms for this file may be
 *  found in found in the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 * $Id: dec21140.c,v 1.2 2001-09-27 12:01:41 chris Exp $
 *
 * ------------------------------------------------------------------------
 * [22.05.2000,StWi/CWA] added support for the DEC/Intel 21143 chip
 *
 * The 21143 support is (for now) only available for the __i386 target,
 * because that's the only testing platform I have. It should (to my best
 * knowledge) work in the same way for the "__PPC" target, but someone
 * should test this first before it's put into the code. Thanks go to
 * Andrew Klossner who provided the vital information about the
 * Intel 21143 chip.
 * (FWIW: I tested this driver using a Kingston KNE100TX with 21143PD chip)
 *
 * The driver will automatically detect whether there is a 21140 or 21143
 * network card in the system and activate support accordingly. It will
 * look for the 21140 first. If the 21140 is not found the driver will
 * look for the 21143.
 * ------------------------------------------------------------------------
 */

#include <rtems.h>

/*
 *  This driver only supports architectures with the new style
 *  exception processing.  The following checks try to keep this
 *  from being compiled on systems which can't support this driver.
 */

#if defined(__i386)
  #define DEC21140_SUPPORTED
#endif

#if defined(__PPC) && (defined(mpc604) || defined(mpc750))
  #define DEC21140_SUPPORTED
#endif

#if defined(DEC21140_SUPPORTED)
#include <bsp.h>
#if defined(i386)
#include <pcibios.h>
#endif
#if defined(__PPC)
#include <bsp/pci.h>
#include <libcpu/byteorder.h>
#include <libcpu/io.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <rtems/error.h>
#include <rtems/rtems_bsdnet.h>

#include <libcpu/cpu.h>

#include <sys/param.h>
#include <sys/mbuf.h>

#include <sys/socket.h>
#include <sys/sockio.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
 
#if defined(i386)
#include <irq.h>
#endif
#if defined(__PPC)
#include <bsp/irq.h>
#endif

#ifdef malloc
#undef malloc
#endif
#ifdef free
#undef free 
#endif

#define DEC_DEBUG

/* note: the 21143 isn't really a DEC, it's an Intel chip */
#define PCI_INVALID_VENDORDEVICEID	0xffffffff
#define PCI_VENDOR_ID_DEC 0x1011
#define PCI_DEVICE_ID_DEC_21140 0x0009
#define PCI_DEVICE_ID_DEC_21143 0x0019

#define IO_MASK  0x3
#define MEM_MASK  0xF

/* command and status registers, 32-bit access, only if IO-ACCESS */
#define ioCSR0  0x00	/* bus mode register */
#define ioCSR1  0x08	/* transmit poll demand */
#define ioCSR2  0x10	/* receive poll demand */
#define ioCSR3  0x18	/* receive list base address */
#define ioCSR4  0x20	/* transmit list base address */
#define ioCSR5  0x28	/* status register */
#define ioCSR6  0x30	/* operation mode register */
#define ioCSR7  0x38	/* interrupt mask register */
#define ioCSR8  0x40	/* missed frame counter */
#define ioCSR9  0x48	/* Ethernet ROM register */
#define ioCSR10 0x50	/* reserved */
#define ioCSR11 0x58	/* full-duplex register */
#define ioCSR12 0x60	/* SIA status register */
#define ioCSR13 0x68
#define ioCSR14 0x70
#define ioCSR15 0x78	/* SIA general register */

/* command and status registers, 32-bit access, only if MEMORY-ACCESS */
#define memCSR0  0x00	/* bus mode register */
#define memCSR1  0x02	/* transmit poll demand */
#define memCSR2  0x04	/* receive poll demand */
#define memCSR3  0x06	/* receive list base address */
#define memCSR4  0x08	/* transmit list base address */
#define memCSR5  0x0A	/* status register */
#define memCSR6  0x0C	/* operation mode register */
#define memCSR7  0x0E	/* interrupt mask register */
#define memCSR8  0x10	/* missed frame counter */
#define memCSR9  0x12	/* Ethernet ROM register */
#define memCSR10 0x14	/* reserved */
#define memCSR11 0x16	/* full-duplex register */
#define memCSR12 0x18	/* SIA status register */
#define memCSR13 0x1A
#define memCSR14 0x1C
#define memCSR15 0x1E	/* SIA general register */

#define DEC_REGISTER_SIZE    0x100   /* to reserve virtual memory */

#define RESET_CHIP   0x00000001
#if defined(__PPC)
#define CSR0_MODE    0x0030e002   /* 01b08000 */
#else
#define CSR0_MODE    0x0020e002   /* 01b08000 */
#endif
#define ROM_ADDRESS  0x00004800
#define CSR6_INIT    0x022cc000   /* 022c0000 020c0000 */  
#define CSR6_TX      0x00002000   
#define CSR6_TXRX    0x00002002   
#define IT_SETUP     0x000100c0   /* 000100e0 */
#define CLEAR_IT     0xFFFFFFFF   
#define NO_IT        0x00000000   

#define NRXBUFS 32	/* number of receive buffers */
#define NTXBUFS 16	/* number of transmit buffers */

/* message descriptor entry */
struct MD {
    /* used by hardware */
    volatile unsigned32 status;
    volatile unsigned32 counts;
    unsigned32 buf1, buf2;  
    /* used by software */
    volatile struct mbuf *m;
    volatile struct MD *next;
};

/*
 * Number of DECs supported by this driver
 */
#define NDECDRIVER	1

/*
 * Receive buffer size -- Allow for a full ethernet packet including CRC
 */
#define RBUF_SIZE	1536

#define	ET_MINLEN 60		/* minimum message length */

/*
 * RTEMS event used by interrupt handler to signal driver tasks.
 * This must not be any of the events used by the network task synchronization.
 */
#define INTERRUPT_EVENT	RTEMS_EVENT_1

/*
 * RTEMS event used to start transmit daemon.
 * This must not be the same as INTERRUPT_EVENT.
 */
#define START_TRANSMIT_EVENT	RTEMS_EVENT_2

#if defined(__PPC)
#define phys_to_bus(address) ((unsigned int)((address)) + PREP_PCI_DRAM_OFFSET)
#define bus_to_phys(address) ((unsigned int)((address)) - PREP_PCI_DRAM_OFFSET)
#define CPU_CACHE_ALIGNMENT_FOR_BUFFER PPC_CACHE_ALIGNMENT
#else
extern void Wait_X_ms( unsigned int timeToWait );
#define phys_to_bus(address) ((unsigned int) ((address)))
#define bus_to_phys(address) ((unsigned int) ((address)))
#define delay_in_bus_cycles(cycle) Wait_X_ms( cycle/100 )
#define CPU_CACHE_ALIGNMENT_FOR_BUFFER PG_SIZE

inline void st_le32(volatile unsigned32 *addr, unsigned32 value)
{
  *(addr)=value ;
}

inline unsigned32 ld_le32(volatile unsigned32 *addr)
{
  return(*addr);
}

#endif

#if (MCLBYTES < RBUF_SIZE)
# error "Driver must have MCLBYTES > RBUF_SIZE"
#endif

/*
 * Per-device data
 */
 struct dec21140_softc {

   struct arpcom			arpcom;

   rtems_irq_connect_data	irqInfo;

   volatile struct MD		*MDbase;
   volatile unsigned char	*bufferBase;
   int				acceptBroadcast;
   rtems_id			rxDaemonTid;
   rtems_id			txDaemonTid;
   
   volatile struct MD   *TxMD;
   volatile struct MD   *SentTxMD;
   int         PendingTxCount;
   int         TxSuspended;

  unsigned int 			port;
  volatile unsigned int		*base;
   
  /*
   * Statistics
   */
  unsigned long	rxInterrupts;
  unsigned long	rxNotFirst;
  unsigned long	rxNotLast;
  unsigned long	rxGiant;
  unsigned long	rxNonOctet;
  unsigned long	rxRunt;
  unsigned long	rxBadCRC;
  unsigned long	rxOverrun;
  unsigned long	rxCollision;
  
  unsigned long	txInterrupts;
  unsigned long	txDeferred;
  unsigned long	txHeartbeat;
  unsigned long	txLateCollision;
  unsigned long	txRetryLimit;
  unsigned long	txUnderrun;
  unsigned long	txLostCarrier;
  unsigned long	txRawWait;
};

static struct dec21140_softc dec21140_softc[NDECDRIVER];

/*
 * DEC21140 interrupt handler
 */
static rtems_isr
dec21140Enet_interrupt_handler (rtems_vector_number v)
{
    volatile unsigned32 *tbase;
    unsigned32 status;
    struct dec21140_softc *sc;

    sc = &dec21140_softc[0];
    tbase = (unsigned32 *)(sc->base) ;

    /*
     * Read status
     */
    status = ld_le32(tbase+memCSR5);
    st_le32((tbase+memCSR5), status); /* clear the bits we've read */

    /*
     * Frame received?
     */
    if (status & 0x000000c0){
      sc->rxInterrupts++;
      rtems_event_send (sc->rxDaemonTid, INTERRUPT_EVENT);
    }
}

static void nopOn(const rtems_irq_connect_data* notUsed)
{
  /*
   * code should be moved from dec21140Enet_initialize_hardware
   * to this location
   */
}

static int dec21140IsOn(const rtems_irq_connect_data* irq)
{
  return BSP_irq_enabled_at_i8259s (irq->name);
}


/*
 * This routine reads a word (16 bits) from the serial EEPROM.
 */
/*  EEPROM_Ctrl bits. */
#define EE_SHIFT_CLK		0x02	/* EEPROM shift clock. */
#define EE_CS			0x01	/* EEPROM chip select. */
#define EE_DATA_WRITE		0x04	/* EEPROM chip data in. */
#define EE_WRITE_0		0x01
#define EE_WRITE_1		0x05
#define EE_DATA_READ		0x08	/* EEPROM chip data out. */
#define EE_ENB			(0x4800 | EE_CS)

/* The EEPROM commands include the alway-set leading bit. */
#define EE_WRITE_CMD	(5 << 6)
#define EE_READ_CMD	(6 << 6)
#define EE_ERASE_CMD	(7 << 6)

static int eeget16(volatile unsigned int *ioaddr, int location)
{
	int i;
	unsigned short retval = 0;
	int read_cmd = location | EE_READ_CMD;
	
	st_le32(ioaddr, EE_ENB & ~EE_CS);
	st_le32(ioaddr, EE_ENB);
	
	/* Shift the read command bits out. */
	for (i = 10; i >= 0; i--) {
		short dataval = (read_cmd & (1 << i)) ? EE_DATA_WRITE : 0;
		st_le32(ioaddr, EE_ENB | dataval);
		delay_in_bus_cycles(200);
		st_le32(ioaddr, EE_ENB | dataval | EE_SHIFT_CLK);
		delay_in_bus_cycles(200);
		st_le32(ioaddr, EE_ENB | dataval); /* Finish EEPROM a clock tick. */
		delay_in_bus_cycles(200);
	}
	st_le32(ioaddr, EE_ENB);
	
	for (i = 16; i > 0; i--) {
		st_le32(ioaddr, EE_ENB | EE_SHIFT_CLK);
		delay_in_bus_cycles(200);
		retval = (retval << 1) | ((ld_le32(ioaddr) & EE_DATA_READ) ? 1 : 0);
		st_le32(ioaddr, EE_ENB);
		delay_in_bus_cycles(200);
	}

	/* Terminate the EEPROM access. */
	st_le32(ioaddr, EE_ENB & ~EE_CS);
	return ( ((retval<<8)&0xff00) | ((retval>>8)&0xff) );
}

/*
 * Initialize the ethernet hardware
 */
static void
dec21140Enet_initialize_hardware (struct dec21140_softc *sc)
{
  rtems_status_code st;
  volatile unsigned int *tbase;
  int i;
  volatile unsigned char *cp, *setup_frm, *eaddrs; 
  volatile unsigned char *buffer;
  volatile struct MD *rmd;

  tbase = sc->base;

  /*
   * WARNING : First write in CSR6
   *           Then Reset the chip ( 1 in CSR0)
   */
  st_le32( (tbase+memCSR6), CSR6_INIT);  
  st_le32( (tbase+memCSR0), RESET_CHIP);  
  delay_in_bus_cycles(200);

  /*
   * Init CSR0
   */
  st_le32( (tbase+memCSR0), CSR0_MODE);  

#ifdef DEC_DEBUG
  printk("DC2114x %x:%x:%x:%x:%x:%x IRQ %d IO %x M %x .........\n",
	 sc->arpcom.ac_enaddr[0], sc->arpcom.ac_enaddr[1],
	 sc->arpcom.ac_enaddr[2], sc->arpcom.ac_enaddr[3],
	 sc->arpcom.ac_enaddr[4], sc->arpcom.ac_enaddr[5],
	 sc->irqInfo.name, sc->port, (unsigned) sc->base);
#endif
  
  /*
   * Init RX ring
   */
  cp = (volatile unsigned char *)malloc(((NRXBUFS+NTXBUFS)*sizeof(struct MD))
					+ (NTXBUFS*RBUF_SIZE)
					+ CPU_CACHE_ALIGNMENT_FOR_BUFFER);
  sc->bufferBase = cp;
  cp += (CPU_CACHE_ALIGNMENT_FOR_BUFFER - (int)cp)
         & (CPU_CACHE_ALIGNMENT_FOR_BUFFER - 1);
#if defined(__i386)
#ifdef PCI_BRIDGE_DOES_NOT_ENSURE_CACHE_COHERENCY_FOR_DMA 
  if (_CPU_is_paging_enabled())
    _CPU_change_memory_mapping_attribute
                   (NULL, cp,
		    ((NRXBUFS+NTXBUFS)*sizeof(struct MD))
		    + (NTXBUFS*RBUF_SIZE),
		    PTE_CACHE_DISABLE | PTE_WRITABLE);
#endif
#endif
  rmd = (volatile struct MD*)cp;
  sc->MDbase = rmd;
  buffer = cp + ((NRXBUFS+NTXBUFS)*sizeof(struct MD));
  st_le32( (tbase+memCSR3), (long)(phys_to_bus((long)(sc->MDbase))));
  for (i=0 ; i<NRXBUFS; i++){
    struct mbuf *m;
    
    /* allocate an mbuf for each receive descriptor */
    MGETHDR (m, M_WAIT, MT_DATA);
    MCLGET (m, M_WAIT);
    m->m_pkthdr.rcvif = &sc->arpcom.ac_if;
    rmd->m = m;

    rmd->buf2   = phys_to_bus(rmd+1);
    rmd->buf1   = phys_to_bus(mtod(m, void *));  
    rmd->counts = 0xfdc00000 | (RBUF_SIZE);
    rmd->status = 0x80000000;
    rmd->next   = rmd + 1;
    rmd++;
  }
  /*
   * mark last RX buffer.
   */
  sc->MDbase [NRXBUFS-1].counts = 0xfec00000 | (RBUF_SIZE);
  sc->MDbase [NRXBUFS-1].next   = sc->MDbase;

  /*
   * Init TX ring
   */
  st_le32( (tbase+memCSR4), (long)(phys_to_bus((long)(rmd))) );
  for (i=0 ; i<NTXBUFS; i++){
    (rmd+i)->buf2   = phys_to_bus(rmd+i+1);
    (rmd+i)->buf1   = phys_to_bus(buffer + (i*RBUF_SIZE));
    (rmd+i)->counts = 0x01000000;
    (rmd+i)->status = 0x0;
    (rmd+i)->next   = rmd+i+1;
    (rmd+i)->m      = 0;
  } 

  /*
   * mark last TX buffer.
   */
  (rmd+NTXBUFS-1)->buf2   = phys_to_bus(rmd);
  (rmd+NTXBUFS-1)->next   = rmd;
  
  /*
   * Set up interrupts
   */
  sc->irqInfo.hdl = (rtems_irq_hdl)dec21140Enet_interrupt_handler;
  sc->irqInfo.on  = nopOn;
  sc->irqInfo.off = nopOn;
  sc->irqInfo.isOn = dec21140IsOn;  
  st = BSP_install_rtems_irq_handler (&sc->irqInfo);
  if (!st)
    rtems_panic ("Can't attach DEC21140 interrupt handler for irq %d\n",
		  sc->irqInfo.name);

  st_le32( (tbase+memCSR7), NO_IT); 

  /*
   * Build setup frame
   */
  setup_frm = (volatile unsigned char *)(bus_to_phys(rmd->buf1));
  eaddrs = (char *)(sc->arpcom.ac_enaddr);
  /* Fill the buffer with our physical address. */
  for (i = 1; i < 16; i++) {
	*setup_frm++ = eaddrs[0];
	*setup_frm++ = eaddrs[1];
	*setup_frm++ = eaddrs[0];
	*setup_frm++ = eaddrs[1];
	*setup_frm++ = eaddrs[2];
	*setup_frm++ = eaddrs[3];
	*setup_frm++ = eaddrs[2];
	*setup_frm++ = eaddrs[3];
	*setup_frm++ = eaddrs[4];
	*setup_frm++ = eaddrs[5];
	*setup_frm++ = eaddrs[4];
	*setup_frm++ = eaddrs[5];
  }
  /* Add the broadcast address when doing perfect filtering */
  memset((void*) setup_frm, 0xff, 12);
  rmd->counts = 0x09000000 | 192 ;
  rmd->status = 0x80000000;
  st_le32( (tbase+memCSR6), CSR6_INIT | CSR6_TX);
  st_le32( (tbase+memCSR1), 1);
  while (rmd->status != 0x7fffffff);
  rmd->counts = 0x01000000;    
  sc->TxMD = rmd+1;
  
  /*
   * Enable RX and TX
   */
  st_le32( (tbase+memCSR5), IT_SETUP);
  st_le32( (tbase+memCSR7), IT_SETUP); 
  st_le32( (unsigned int*)(tbase+memCSR6), CSR6_INIT | CSR6_TXRX);
}

static void
dec21140_rxDaemon (void *arg)
{
  volatile unsigned int *tbase;
  struct ether_header *eh;
  struct dec21140_softc *dp = (struct dec21140_softc *)&dec21140_softc[0];
  struct ifnet *ifp = &dp->arpcom.ac_if;
  struct mbuf *m;
  volatile struct MD *rmd;
  unsigned int len;
  rtems_event_set events;
  
  tbase = dec21140_softc[0].base ;
  rmd = dec21140_softc[0].MDbase;

  for (;;){

    rtems_bsdnet_event_receive (INTERRUPT_EVENT,
				RTEMS_WAIT|RTEMS_EVENT_ANY,
				RTEMS_NO_TIMEOUT,
				&events);
    
    while((rmd->status & 0x80000000) == 0){
      /* pass on the packet in the mbuf */
      len = (rmd->status >> 16) & 0x7ff;
      m = (struct mbuf *)(rmd->m);
      m->m_len = m->m_pkthdr.len = len - sizeof(struct ether_header);
      eh = mtod (m, struct ether_header *);
      m->m_data += sizeof(struct ether_header);
      ether_input (ifp, eh, m);
       
      /* get a new mbuf for the 21140 */
      MGETHDR (m, M_WAIT, MT_DATA);
      MCLGET (m, M_WAIT);
      m->m_pkthdr.rcvif = ifp;
      rmd->m = m;
      rmd->buf1 = phys_to_bus(mtod(m, void *));  

      rmd->status = 0x80000000;
      
      rmd=rmd->next;
    }
  }	
}

static void
sendpacket (struct ifnet *ifp, struct mbuf *m)
{
  struct dec21140_softc *dp = ifp->if_softc;
  volatile struct MD *tmd;
  volatile unsigned char *temp;
  struct mbuf *n;
  unsigned int len;
  volatile unsigned int *tbase;

  tbase = dp->base;
  /*
   * Waiting for Transmitter ready
   */	
  tmd = dec21140_softc[0].TxMD;
  n = m;

  while ((tmd->status & 0x80000000) != 0){
    tmd=tmd->next;
    }

  len = 0;
  temp = (volatile unsigned char *)(bus_to_phys(tmd->buf1));
  
  for (;;){
    len += m->m_len;
    memcpy((void*) temp, (char *)m->m_data, m->m_len);
    temp += m->m_len ;
    if ((m = m->m_next) == NULL)
      break;
  }

  if (len < ET_MINLEN) len = ET_MINLEN;
  tmd->counts =  0xe1000000 | (len & 0x7ff);  
  tmd->status = 0x80000000;

  st_le32( (tbase+memCSR1), 0x1);

  m_freem(n);
  dec21140_softc[0].TxMD = tmd->next;
}

/*
 * Driver transmit daemon
 */
void
dec21140_txDaemon (void *arg)
{
  struct dec21140_softc *sc = (struct dec21140_softc *)arg;
  struct ifnet *ifp = &sc->arpcom.ac_if;
  struct mbuf *m;
  rtems_event_set events;

  for (;;) {
    /*
     * Wait for packet
     */

    rtems_bsdnet_event_receive (START_TRANSMIT_EVENT, RTEMS_EVENT_ANY | RTEMS_WAIT, RTEMS_NO_TIMEOUT, &events);

    /*
     * Send packets till queue is empty
     */
    for (;;) {
      /*
       * Get the next mbuf chain to transmit.
       */
      IF_DEQUEUE(&ifp->if_snd, m);
      if (!m)
	break;
      sendpacket (ifp, m);
    }
    ifp->if_flags &= ~IFF_OACTIVE;
  }
}	


static void
dec21140_start (struct ifnet *ifp)
{
	struct dec21140_softc *sc = ifp->if_softc;

	rtems_event_send (sc->txDaemonTid, START_TRANSMIT_EVENT);
	ifp->if_flags |= IFF_OACTIVE;
}

/*
 * Initialize and start the device
 */
static void
dec21140_init (void *arg)
{
  struct dec21140_softc *sc = arg;
  struct ifnet *ifp = &sc->arpcom.ac_if;

  if (sc->txDaemonTid == 0) {
    
    /*
     * Set up DEC21140 hardware
     */
    dec21140Enet_initialize_hardware (sc);
    
    /*
     * Start driver tasks
     */
    sc->rxDaemonTid = rtems_bsdnet_newproc ("DCrx", 4096,
					    dec21140_rxDaemon, sc);
    sc->txDaemonTid = rtems_bsdnet_newproc ("DCtx", 4096,
					    dec21140_txDaemon, sc);
  }

  /*
   * Tell the world that we're running.
   */
  ifp->if_flags |= IFF_RUNNING;

}

/*
 * Stop the device
 */
static void
dec21140_stop (struct dec21140_softc *sc)
{
  volatile unsigned int *tbase;
  struct ifnet *ifp = &sc->arpcom.ac_if;

  ifp->if_flags &= ~IFF_RUNNING;

  /*
   * Stop the transmitter
   */
  tbase=dec21140_softc[0].base ;
  st_le32( (tbase+memCSR7), NO_IT);
  st_le32( (tbase+memCSR6), CSR6_INIT);
  free((void*)sc->bufferBase);
}


/*
 * Show interface statistics
 */
static void
dec21140_stats (struct dec21140_softc *sc)
{
	printf ("      Rx Interrupts:%-8lu", sc->rxInterrupts);
	printf ("       Not First:%-8lu", sc->rxNotFirst);
	printf ("        Not Last:%-8lu\n", sc->rxNotLast);
	printf ("              Giant:%-8lu", sc->rxGiant);
	printf ("            Runt:%-8lu", sc->rxRunt);
	printf ("       Non-octet:%-8lu\n", sc->rxNonOctet);
	printf ("            Bad CRC:%-8lu", sc->rxBadCRC);
	printf ("         Overrun:%-8lu", sc->rxOverrun);
	printf ("       Collision:%-8lu\n", sc->rxCollision);

	printf ("      Tx Interrupts:%-8lu", sc->txInterrupts);
	printf ("        Deferred:%-8lu", sc->txDeferred);
	printf (" Missed Hearbeat:%-8lu\n", sc->txHeartbeat);
	printf ("         No Carrier:%-8lu", sc->txLostCarrier);
	printf ("Retransmit Limit:%-8lu", sc->txRetryLimit);
	printf ("  Late Collision:%-8lu\n", sc->txLateCollision);
	printf ("           Underrun:%-8lu", sc->txUnderrun);
	printf (" Raw output wait:%-8lu\n", sc->txRawWait);
}

/*
 * Driver ioctl handler
 */
static int
dec21140_ioctl (struct ifnet *ifp, int command, caddr_t data)
{
	struct dec21140_softc *sc = ifp->if_softc;
	int error = 0;

	switch (command) {
	case SIOCGIFADDR:
	case SIOCSIFADDR:
		ether_ioctl (ifp, command, data);
		break;

	case SIOCSIFFLAGS:
		switch (ifp->if_flags & (IFF_UP | IFF_RUNNING)) {
		case IFF_RUNNING:
			dec21140_stop (sc);
			break;

		case IFF_UP:
			dec21140_init (sc);
			break;

		case IFF_UP | IFF_RUNNING:
			dec21140_stop (sc);
			dec21140_init (sc);
			break;

		default:
			break;
		}
		break;

	case SIO_RTEMS_SHOW_STATS:
		dec21140_stats (sc);
		break;
		
	/*
	 * FIXME: All sorts of multicast commands need to be added here!
	 */
	default:
		error = EINVAL;
		break;
	}

	return error;
}

/*
 * Attach an DEC21140 driver to the system
 */
int
rtems_dec21140_driver_attach (struct rtems_bsdnet_ifconfig *config)
{
	struct dec21140_softc *sc;
	struct ifnet *ifp;
	int mtu;
	int i;
    int deviceId = PCI_DEVICE_ID_DEC_21140; /* network card device ID */
	
	/*
	 * First, find a DEC board
	 */
#if defined(__i386)
	int signature;
	int value;
	char interrupt;
	int diag;

	if (pcib_init() == PCIB_ERR_NOTPRESENT)
	  rtems_panic("PCI BIOS not found !!");
	
	/*
	 * Try to find the network card on the PCI bus. Probe for a DEC 21140
     * card first. If not found probe the bus for a DEC/Intel 21143 card.
	 */
    deviceId = PCI_DEVICE_ID_DEC_21140;
    diag = pcib_find_by_devid( PCI_VENDOR_ID_DEC, deviceId,
                               0, &signature);
    if ( diag == PCIB_ERR_SUCCESS)
      printk( "DEC 21140 PCI network card found\n" );
    else
    {
      deviceId = PCI_DEVICE_ID_DEC_21143;
      diag = pcib_find_by_devid( PCI_VENDOR_ID_DEC, deviceId,
                                 0, &signature);
      if ( diag == PCIB_ERR_SUCCESS)
        printk( "DEC/Intel 21143 PCI network card found\n" );
      else
        rtems_panic("DEC PCI network card not found !!\n");
    }
#endif	
#if defined(__PPC)
	unsigned char ucSlotNumber, ucFnNumber;
	unsigned int  ulDeviceID, lvalue, tmp;	
	unsigned char cvalue;

	for(ucSlotNumber=0;ucSlotNumber<PCI_MAX_DEVICES;ucSlotNumber++) {
	  for(ucFnNumber=0;ucFnNumber<PCI_MAX_FUNCTIONS;ucFnNumber++) {
	    (void)pci_read_config_dword(0,
					ucSlotNumber,
					ucFnNumber,
					PCI_VENDOR_ID,
					&ulDeviceID);
	    if(ulDeviceID==PCI_INVALID_VENDORDEVICEID) {
	      /*
	       * This slot is empty
	       */
	      continue;
	    }
	    if (ulDeviceID == ((PCI_DEVICE_ID_DEC_21140<<16) + PCI_VENDOR_ID_DEC))
	      break;
	  }
	  if (ulDeviceID == ((PCI_DEVICE_ID_DEC_21140<<16) + PCI_VENDOR_ID_DEC)){
	    printk("DEC Adapter found !!\n");
	    break;
	  }
	}
	
	if(ulDeviceID==PCI_INVALID_VENDORDEVICEID)
	  rtems_panic("DEC PCI board not found !!\n");
#endif  
	/*
	 * Find a free driver
	 */
	for (i = 0 ; i < NDECDRIVER ; i++) {
		sc = &dec21140_softc[i];
		ifp = &sc->arpcom.ac_if;
		if (ifp->if_softc == NULL)
			break;
	}
	if (i >= NDECDRIVER) {
		printk ("Too many DEC drivers.\n");
		return 0;
	}

	/*
	 * Process options
	 */
#if defined(__i386)

    /* the 21143 chip must be enabled before it can be accessed */
    if ( deviceId == PCI_DEVICE_ID_DEC_21143 )
      pcib_conf_write32( signature, 0x40, 0 );

	pcib_conf_read32(signature, 16, &value);
	sc->port = value & ~IO_MASK;
        
	pcib_conf_read32(signature, 20, &value);
	if (_CPU_is_paging_enabled())
	  _CPU_map_phys_address((void **) &(sc->base),
				(void *)(value & ~MEM_MASK),
				DEC_REGISTER_SIZE ,
				PTE_CACHE_DISABLE | PTE_WRITABLE);
	else
	  sc->base = (unsigned int *)(value & ~MEM_MASK);
	
	pcib_conf_read8(signature, 60, &interrupt);
	  sc->irqInfo.name = (rtems_irq_symbolic_name)interrupt;
#endif
#if defined(__PPC)
	(void)pci_read_config_dword(0,
				    ucSlotNumber,
				    ucFnNumber,
				    PCI_BASE_ADDRESS_0,
				    &lvalue);

	sc->port = lvalue & (unsigned int)(~IO_MASK);
        
	(void)pci_read_config_dword(0,
				    ucSlotNumber,
				    ucFnNumber,
				    PCI_BASE_ADDRESS_1  ,
				    &lvalue);


	tmp = (unsigned int)(lvalue & (unsigned int)(~MEM_MASK)) 
	  + (unsigned int)PREP_ISA_MEM_BASE;
	sc->base = (unsigned int *)(tmp);

	(void)pci_read_config_byte(0,
				   ucSlotNumber,
				   ucFnNumber,
				   PCI_INTERRUPT_LINE,
				   &cvalue);
	sc->irqInfo.name = (rtems_irq_symbolic_name)cvalue;
#endif
 	if (config->hardware_address) {
	  memcpy (sc->arpcom.ac_enaddr, config->hardware_address,
		  ETHER_ADDR_LEN);
	}
	else {
	  union {char c[64]; unsigned short s[32];} rombuf;
	  int i;

	  for (i=0; i<32; i++){
	    rombuf.s[i] = eeget16(sc->base+memCSR9, i);
	  }
#if defined(__i386)
	  for (i=0 ; i<(ETHER_ADDR_LEN/2); i++){
	    sc->arpcom.ac_enaddr[2*i]   = rombuf.c[20+2*i+1];
	    sc->arpcom.ac_enaddr[2*i+1] = rombuf.c[20+2*i];
	  }  
#endif
#if defined(__PPC)
	  memcpy (sc->arpcom.ac_enaddr, rombuf.c+20, ETHER_ADDR_LEN);
#endif
	}

	if (config->mtu)
		mtu = config->mtu;
	else
		mtu = ETHERMTU;

	sc->acceptBroadcast = !config->ignore_broadcast;

	/*
	 * Set up network interface values
	 */
	ifp->if_softc = sc;
	ifp->if_unit = i + 1;
	ifp->if_name = "dc";
	ifp->if_mtu = mtu;
	ifp->if_init = dec21140_init;
	ifp->if_ioctl = dec21140_ioctl;
	ifp->if_start = dec21140_start;
	ifp->if_output = ether_output;
	ifp->if_flags = IFF_BROADCAST | IFF_SIMPLEX;
	if (ifp->if_snd.ifq_maxlen == 0)
		ifp->if_snd.ifq_maxlen = ifqmaxlen;

 	/*
	 * Attach the interface
	 */
	if_attach (ifp);
	ether_ifattach (ifp);

        printk( "DC2114x : driver has been attached\n" );
	return 1;
};
#endif /* DEC21140_SUPPORTED */

