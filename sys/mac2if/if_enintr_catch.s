/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	if_enintr_catch.s,v $
 * Revision 2.2  90/08/30  11:09:00  bohman
 * 	Created.
 * 	[90/08/29  12:36:21  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2if/if_enintr_catch.s
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Linkage routine between
 * slot manager and ethernet
 * driver.
 */

	.text
	.globl	_enintr_catch

_enintr_catch:
	movl	a1,sp@-
	jsr	_enintr
	addql	#4,sp
	moveq	#1,d0
	rts
