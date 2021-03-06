/*
 * This is based on the mvme-crt0.S file from libgloss/rs6000.
 * crt0.S -- startup file for PowerPC systems.
 *
 * Copyright (c) 1995 Cygnus Support
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 *
 *  $Id: start.S,v 1.2 2001-09-27 12:01:03 chris Exp $
 */
#include <rtems/score/targopts.h>
#include "ppc-asm.h"

	.file	"start.s"
	.section ".got2","aw"
	.align	2

.LCTOC1 = .+32768

	.extern	FUNC_NAME(atexit)
	.globl	FUNC_NAME(__atexit)
	.section ".sdata","aw"
	.align	2
FUNC_NAME(__atexit):			/* tell C's eabi-ctor's we have an atexit function */
	.long	FUNC_NAME(atexit)@fixup	/* and that it is to register __do_global_dtors */

	.section ".fixup","aw"
	.align	2
	.long	FUNC_NAME(__atexit)

	.section ".got2","aw"
.Ltable = .-.LCTOC1
	.long	.LCTOC1			/* address we think .LCTOC1 is loaded at */

.Lbss_start = .-.LCTOC1
	.long	_edata                 /* includes sbss and bss */

.Lend = .-.LCTOC1
	.long	_end

.Lstack = .-.LCTOC1			/* stack address if set by user */
	.long	__stack

	.text
.Lptr:
	.long .LCTOC1-.Laddr

	.globl	_start
	.type	_start,@function
_start:
	b	past_constants
					/* Set MSR */
	.long	_etext                  /* end of code space */
	.long	_edata                  /* end of code and data space */

past_constants:
	lis     r5,0
	mr      r4,r5
	ori     r4,r4,0x0000            /* 0x2030  */
        mtmsr   r4
	
#if (SCORE603E_GENERATION == 1)
	lis     r4,0
	mtspr   530,r4                  /* Set IBAT1U */
	mtspr   531,r4                  /* Set IBAT1L */
	mtspr   534,r4                  /* Set IBAT3U */
	mtspr   535,r4                  /* Set IBAT3L */
	mtspr   538,r4                  /* Set DBAT1U */
	mtspr   539,r4                  /* Set DBAT1L */
	lis     r4,0
	ori     r4,r4,0x1fff
	mtspr   528,r4                  /* Set IBAT0U */
	mtspr   536,r4                  /* Set DBAT0U */
	lis     r4,0
	ori     r4,r4,0x0002	
	mtspr   529,r4                  /* Set IBAT0L */
	mtspr   537,r4                  /* Set DBAT0L */
	lis     r4,-4096                /*  0xf000    */
	ori     r4,r4,8191              /*  0x1fff    */
	mtspr   532,r4                  /* Set IBAT2U */
	mtspr   540,r4                  /* Set DBAT2U */
	lis     r4,-4096                /*  0xf000    */
	ori     r4,r4,1	
	mtspr   533,r4                  /* Set IBAT2L */
	mtspr   541,r4                  /* Set DBAT2L */
	lis     r4,-32768               /*  0x8000    */
	ori     r4,r4,8191              /*  0x1fff    */
	mtspr   542,r4                  /* Set DBAT3U */
	lis     r4,-32768               /*  0x8000    */
	ori     r4,r4,0x003a
	mtspr   543,r4                  /* Set DBAT3L */

#elif (SCORE603E_GENERATION == 2)
/* XXX FILL THIS IN WHEN I GET HELLO TO COME UP. */
	
#else
#error "Unknown Generation of  Score603e"
#endif

	bl	.Laddr			/* get current address */
.Laddr:
	mflr	r4			/* real address of .Laddr */
	lwz	r5,(.Lptr-.Laddr)(r4)	/* linker generated address of .LCTOC1 */
	add	r5,r5,r4		/* correct to real pointer */
	lwz	r4,.Ltable(r5)		/* get linker's idea of where .Laddr is */
	subf	r4,r4,r5		/* calculate difference between where linked and current */

	/* clear bss */
	lwz	r6,.Lbss_start(r5)	/* calculate beginning of the BSS */
	lwz	r7,.Lend(r5)		/* calculate end of the BSS */
	add	r6,r6,r4		/* adjust pointers */
	add	r7,r7,r4

	cmplw	1,r6,r7
	bc	4,4,.Ldone

	subf	r8,r6,r7		/* number of bytes to zero */
	srwi	r9,r8,2			/* number of words to zero */
	mtctr	r9
	li	r0,0			/* zero to clear memory */
	addi	r6,r6,-4		/* adjust so we can use stwu */
.Lloop:
	stwu	r0,4(r6)		/* zero bss */
	bdnz	.Lloop

.Ldone:

	lwz	r0,.Lstack(r5)		/* stack address or 0 */
	cmplwi	1,r0,0			/* equal to 0? */
	bc	12,6,.Lnostack		/* use default stack if == 0 */
	mr	sp,r0			/* use user defined stack */

.Lnostack:
	/* set up initial stack frame */
	addi	sp,sp,-4		/* make sure we don't overwrite debug mem */
	lis	r0,0
	stw	r0,0(sp)		/* clear back chain */
	stwu	sp,-56(sp)		/* push another stack frame */

	/* Let her rip */
	bl	FUNC_NAME(boot_card)

	/* return value from boot_card is argument to exit */
	bl	FUNC_NAME(exit)
	trap
.Lstart:
	.size	_start,.Lstart-_start
