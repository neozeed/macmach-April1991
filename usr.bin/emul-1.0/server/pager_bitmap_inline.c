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
 *	File: emul/server/pager_bitmap_inline.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_PAGER_BITMAP_INLINE_C_
#define	_PAGER_BITMAP_INLINE_C_

static inline
boolean_t
page_check_written(bp, page)
bitmap_ptr_t	bp;
unsigned 	page;
{
    boolean_t	x;

    if (bp == 0)
	x = FALSE;
    else
	asm("bfextu	%1{%2:#1},%0" : "=d" (x) : "m" (*bp), "d" (page));

    return (x);
}

static inline
void
page_set_written(bp, page)
bitmap_ptr_t	bp;
unsigned	page;
{
    if (bp != 0)
	asm("bfset %0{%1:#1}" : "=m" (*bp) : "d" (page));
}

#endif	_PAGER_BITMAP_INLINE_C_
