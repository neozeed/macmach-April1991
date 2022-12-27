/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	slotmgr_CallDriver.s,v $
 * Revision 2.2  90/08/30  11:47:23  bohman
 * 	Created.
 * 	[90/08/29  13:01:49  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2slotmgr/slotmgr_CallDriver.s
 *	Author: David E. Bohman II (CMU macmach)
 */

	.text
	.globl	__CallDriver

/*
 * MacOS Driver call
 * linkage.
 */
__CallDriver:
	movl	sp@(4),d0
	movl	sp@(8),a0
	movl	sp@(12),a1

	moveml	d2-d7/a2-a6,sp@-
	movl	d0,a2
	jsr	a2@
	moveml	sp@+,d2-d7/a2-a6

	rts
