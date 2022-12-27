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
 * $Log:	bitmap_inline.c,v $
 * Revision 2.2  90/08/30  11:00:44  bohman
 * 	Created.
 * 	[90/08/29  11:28:26  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/bitmap_inline.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MACHINE_BITMAP_INLINE_C_
#define	_MACHINE_BITMAP_INLINE_C_

/*
 * Character bitmap allocation routines (inline expansions).
 *
 * The bit offsets are numbered as follows:
 *
 *	7 6 5 4 3 2 1 0
 *	M	      L
 *	S	      S
 *	B	      B
 */

/*
 * Initialize a bitmap.
 */
static inline
void
init_bitmap_byte(bp)
unsigned char *bp;
{
    asm("st	%0@" : : "a" (bp));
}

/*
 * Allocate an element in a bitmap.
 * returns -1 if no free elements,
 * otherwise the bit offset.
 */
static inline
int
alloc_bitmap_byte(bp)
unsigned char *bp;
{
    int bit;

    asm("bfffo	%1@{#0:#8},%0" : "=d" (bit) : "a" (bp));

    if (bit == 8)
	return (-1);

    asm("bfclr	%0@{%1:#1}" : : "a" (bp), "d" (bit));

    return (7 - bit);
}

/*
 * Free an element in a bitmap.
 * NB: the element number is
 * not checked for range errors.
 */
static inline
int
free_bitmap_byte(bp, bit)
unsigned char *bp;
int bit;
{
    bit = 7 - bit;

    asm("bfset	%0@{%1:#1}" : : "a" (bp), "d" (bit));

    return (0);
}

#endif	_MACHINE_BITMAP_INLINE_C_
