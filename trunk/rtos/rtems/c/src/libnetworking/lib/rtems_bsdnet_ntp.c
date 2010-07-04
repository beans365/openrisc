/*
 * Synchronize with an NTP server
 *
 * This program may be distributed and used for any purpose.
 * I ask only that you:
 *      1. Leave this author information intact.
 *      2. Document any changes you make.
 *
 * W. Eric Norum
 * Canadian Light Source
 * University of Saskatchewan
 * Saskatoon, Saskatchewan, CANADA
 * eric@cls.usask.ca
 *
 *  $Id: rtems_bsdnet_ntp.c,v 1.2 2001-09-27 12:01:52 chris Exp $
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <limits.h>
#include <rtems.h>
#include <rtems/rtems_bsdnet.h>
#include <rtems/error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
 * RTEMS base: 1988, January 1
 *  UNIX base: 1970, January 1
 *   NTP base: 1900, January 1
 */
#define UNIX_BASE_TO_NTP_BASE (((70UL*365UL)+17UL) * (24*60*60))

struct timestamp {
	rtems_unsigned32	integer;
	rtems_unsigned32	fraction;
};

struct ntpPacketSmall {
	rtems_unsigned8		li_vn_mode;
	rtems_unsigned8		stratum;
	rtems_signed8		poll_interval;
	rtems_signed8		precision;
	rtems_signed32		root_delay;
	rtems_signed32		root_dispersion;
	char			reference_identifier[4];
	struct timestamp	reference_timestamp;
	struct timestamp	originate_timestamp;
	struct timestamp	receive_timestamp;
	struct timestamp	transmit_timestamp;
};

struct ntpPacket {
	struct ntpPacketSmall	ntp;
	char			authenticator[96];
};

static int
processPacket (struct ntpPacketSmall *p)
{
	time_t tbuf;
	struct tm *lt;
	rtems_time_of_day rt;
	rtems_interval ticks_per_second;

	if (((p->li_vn_mode & (0x7 << 3)) != (3 << 3))
	 || ((p->transmit_timestamp.integer == 0) && (p->transmit_timestamp.fraction == 0)))
		return 0;
	rtems_clock_get (RTEMS_CLOCK_GET_TICKS_PER_SECOND, &ticks_per_second);
	tbuf = ntohl (p->transmit_timestamp.integer) - UNIX_BASE_TO_NTP_BASE - rtems_bsdnet_timeoffset;
	lt = gmtime (&tbuf);
	rt.year = lt->tm_year + 1900;
	rt.month = lt->tm_mon + 1;
	rt.day = lt->tm_mday;
	rt.hour = lt->tm_hour;
	rt.minute = lt->tm_min;
	rt.second = lt->tm_sec;
	rt.ticks = ntohl (p->transmit_timestamp.fraction) / (ULONG_MAX / ticks_per_second);
	if (rt.ticks >= ticks_per_second)
		rt.ticks = ticks_per_second - 1;
	rtems_clock_set (&rt);
	return 1;
}

int
rtems_bsdnet_synchronize_ntp (int interval, rtems_task_priority priority)
{
	int s;
	int i;
	static struct sockaddr_in myAddr, farAddr;
	int fromlen;
	struct ntpPacketSmall packet;
	struct timeval tv;

	if (interval != 0) {
		printf ("Daemon-mode note yet supported.\n");
		errno = EINVAL;
		return -1;
	}
	s = socket (AF_INET, SOCK_DGRAM, 0);
	if (s < 0) {
		printf ("Can't create socket: %s\n", strerror (errno));
		return -1;
	}
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	if (setsockopt (s, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof tv) < 0) {
		printf ("Can't set socket receive timeout: %s", strerror (errno));
		return -1;
	}
	memset (&myAddr, 0, sizeof myAddr);
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons (123);
	myAddr.sin_addr.s_addr = htonl (INADDR_ANY);
	if (bind (s, (struct sockaddr *)&myAddr, sizeof myAddr) < 0) {
		printf ("Can't bind socket: %s\n", strerror (errno));
		return -1;
	}
	for (;;) {
		/*
		 * If there's no server we just have to wait
		 * and hope that there's an NTP broadcast
		 * server out there somewhere.
		 */
		if (rtems_bsdnet_ntpserver_count > 0) {
			memset (&farAddr, 0, sizeof farAddr);
			farAddr.sin_family = AF_INET;
			farAddr.sin_port = htons (123);
			/*
			 * For now, try only the first server.
			 */
			farAddr.sin_addr = rtems_bsdnet_ntpserver[0];
			memset (&packet, 0, sizeof packet);
			packet.li_vn_mode = (3 << 3) | 3; /* NTP version 3, client */
			i = sendto (s, &packet, sizeof packet, 0, (struct sockaddr *)&farAddr, sizeof farAddr);
			if (i != sizeof packet) {
				printf ("Can't send: %s\n", strerror (errno));
				return -1;
			}
		}
		fromlen = sizeof farAddr;
		i = recvfrom (s, &packet, sizeof packet, 0, (struct sockaddr *)&farAddr, &fromlen);
		if (i == 0)
			rtems_panic ("EOF");
		if (i < 0) {
			if ((errno == EWOULDBLOCK) || (errno == EAGAIN))
				continue;
			rtems_panic ("Can't receive: %s", strerror (errno));
		}
		if (i >= sizeof packet) {
			if (processPacket (&packet))
				return 0;
		}
	}
}
