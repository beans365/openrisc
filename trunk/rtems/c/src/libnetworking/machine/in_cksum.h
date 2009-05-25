/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	from tahoe:	in_cksum.c	1.2	86/01/05
 *	from:		@(#)in_cksum.c	1.3 (Berkeley) 1/19/91
 *	from: Id: in_cksum.c,v 1.8 1995/12/03 18:35:19 bde Exp
 *	$Id: in_cksum.h,v 1.2 2001-09-27 12:01:53 chris Exp $
 */

#ifndef _MACHINE_IN_CKSUM_H_
#define	_MACHINE_IN_CKSUM_H_	1

#include <sys/cdefs.h>

/*
 * It it useful to have an Internet checksum routine which is inlineable
 * and optimized specifically for the task of computing IP header checksums
 * in the normal case (where there are no options and the header length is
 * therefore always exactly five 32-bit words.
 */

/*
 *  Optimized version for the i386 family
 */

#if (defined(__GNUC__) && defined(__i386__))

static __inline u_int
in_cksum_hdr(const struct ip *ip)
{
	register u_int sum = 0;
		    
#define ADD(n)	\
    __asm__ volatile ("addl " #n "(%2), %0" : "=r" (sum) : "0" (sum), "r" (ip))
#define ADDC(n)	\
    __asm__ volatile ("adcl " #n "(%2), %0" : "=r" (sum) : "0" (sum), "r" (ip))
#define MOP	\
    __asm__ volatile ("adcl         $0, %0" : "=r" (sum) : "0" (sum))

	ADD(0);
	ADDC(4);
	ADDC(8);
	ADDC(12);
	ADDC(16);
	MOP;
	sum = (sum & 0xffff) + (sum >> 16);
	if (sum > 0xffff)
		sum -= 0xffff;

	return ~sum & 0xffff;
}

static __inline void
in_cksum_update(struct ip *ip)
{
	int __tmpsum;
	__tmpsum = (int)ntohs(ip->ip_sum) + 256;
	ip->ip_sum = htons(__tmpsum + (__tmpsum >> 16));
}

/*
 *  Optimized version for the MC68xxx and Coldfire families
 */

#elif (defined(__GNUC__) && (defined(__mc68000__) || defined(__m68k__)))

static __inline__ u_int
in_cksum_hdr(const struct ip *ip)
{
	register u_int *ap = (u_int *)ip;
	register u_int sum = *ap++;
	register u_int tmp;
		    
	__asm__("addl  %2@+,%0\n\t"
	        "movel %2@+,%1\n\t"
	        "addxl %1,%0\n\t"
	        "movel %2@+,%1\n\t"
	        "addxl %1,%0\n\t"
	        "movel %2@,%1\n\t"
	        "addxl %1,%0\n\t"
	        "moveq #0,%1\n\t"
	        "addxl %1,%0\n" :
		"=d" (sum), "=d" (tmp), "=a" (ap) :
		"0" (sum), "2" (ap));
	sum = (sum & 0xffff) + (sum >> 16);
	if (sum > 0xffff)
		sum -= 0xffff;
	return ~sum & 0xffff;
}

/*
 *  Optimized version for the PowerPC family
 */

#elif (defined(__GNUC__) && (defined(__PPC__) || defined(__ppc__)))

static __inline u_int
in_cksum_hdr(const struct ip *ip)
{
        register u_int sum = 0;
        register u_int tmp;

#define ADD(n) \
	 __asm__ volatile ("addc  %0,%0,%2" : "=r" (sum) : "0" (sum), "r" (n))
#define ADDC(n) \
	__asm__ volatile ("adde  %0,%0,%2" : "=r" (sum) : "0" (sum), "r" (n))
#define MOP     \
	__asm__ volatile ("addic %0,%0,0"  : "=r" (sum) : "0" (sum))

        tmp = *(((u_int *) ip));      ADD(tmp);
        tmp = *(((u_int *) ip) + 1);  ADDC(tmp);
        tmp = *(((u_int *) ip) + 2);  ADDC(tmp);
        tmp = *(((u_int *) ip) + 3);  ADDC(tmp);
        tmp = *(((u_int *) ip) + 4);  ADDC(tmp);
        tmp = 0;                      ADDC(tmp);
        sum = (sum & 0xffff) + (sum >> 16);
        if (sum > 0xffff)
                sum -= 0xffff;

        return ~sum & 0xffff;
}

static __inline void
in_cksum_update(struct ip *ip)
{
        int __tmpsum;
        __tmpsum = (int)ntohs(ip->ip_sum) + 256;
        ip->ip_sum = htons(__tmpsum + (__tmpsum >> 16));
}

/*
 *  SPARC Version
 */

#elif (defined(__GNUC__) && defined(sparc))

static __inline u_int
in_cksum_hdr(const struct ip *ip)
{
   register u_int sum = 0;
   register u_int tmp_o2;
   register u_int tmp_o3;

   __asm__ volatile ("
     ld [%0], %1 ; \
     ld [%0+4], %2 ; \
     addcc %1, %2, %1 ; \
     ld [%0+8], %2 ; \
     addxcc %1, %2, %1 ; \
     ld [%0+12], %2 ; \
     addxcc %1, %2, %1 ; \
     ld [%0+16], %2 ; \
     addxcc %1, %2, %1 ; \
     set 0x0ffff, %3 ; \
     srl %1, 16, %2 ; \
     and %1, %3, %1 ; \
     addx %1, %2, %1 ; \
     srl %1, 16, %1 ; \
     add %1, %%g0, %1 ; \
     neg %1 ; \
     and %1, %3, %1 ; \
    " : "=r" (ip), "=r" (sum), "=r" (tmp_o2), "=r" (tmp_o3)
      : "0" (ip), "1" (sum)
  );
  return sum;
}

#define	in_cksum_update(ip) \
	do { \
		int __tmpsum; \
		__tmpsum = (int)ntohs(ip->ip_sum) + 256; \
		ip->ip_sum = htons(__tmpsum + (__tmpsum >> 16)); \
	} while(0)
/*
 *  Here is the generic, portable, inefficient algorithm.
 */

#else
u_int in_cksum_hdr __P((const struct ip *));
#define	in_cksum_update(ip) \
	do { \
		int __tmpsum; \
		__tmpsum = (int)ntohs(ip->ip_sum) + 256; \
		ip->ip_sum = htons(__tmpsum + (__tmpsum >> 16)); \
	} while(0)

#endif

#endif /* _MACHINE_IN_CKSUM_H_ */
