/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	slotmgr_SCallIntr.s,v $
 * Revision 2.2  90/08/30  11:47:27  bohman
 * 	Created.
 * 	[90/08/29  13:02:31  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2slotmgr/slotmgr_SCallIntr.s
 *	Author: David E. Bohman II (CMU macmach)
 */

	.text
	.globl	_SCallIntr

/*
 * Slot interrupt handler
 * linkage.
 */
_SCallIntr:
	movl	sp@(4),a0
	movl	sp@(8),a1

	moveml	d2-d3/a2-a3,sp@-
	jsr	a0@
	moveml	sp@+,d2-d3/a2-a3

	rts
