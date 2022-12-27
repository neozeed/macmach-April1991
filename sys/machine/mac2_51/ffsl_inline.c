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
 * $Log:	ffsl_inline.c,v $
 * Revision 2.2  90/08/30  11:01:18  bohman
 * 	Created.
 * 	[90/08/29  11:34:28  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/ffsl_inline.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef _MACHINE_FFSL_INLINE_C_
#define _MACHINE_FFSL_INLINE_C_

/*
 * Return the bit position
 * of the most significant
 * set bit in the long word,
 * or -1.
 */
static inline
int
ffsl(l)
unsigned long l;
{
    int bit;

    asm("bfffo %1{#0:#0},%0; bne 0f; moveq #-1,%0; 0:" :
	"=d" (bit) :
	"d" (l));

    if (bit < 0)
	return (bit);

    return (31 - bit);
}

#endif _MACHINE_FFSL_INLINE_C_
