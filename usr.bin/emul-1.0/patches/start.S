/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: emul/patches/start.s
 *	Author: David E. Bohman II (CMU macmach)
 */

	.data
	.space (8192)
	.text
	.globl	_start
	.globl	_done
_start:
	movl	a0,_memtop
	tstl	_mach_init_routine
	beq	0f
	movl	_mach_init_routine,a0
	jsr	a0@

0:
	jsr	_main

_done:	movl	0x40800000,sp
	movl	0x40800004,a0
	jmp	a0@
