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
 * $Log:	ufs_machdep.c,v $
 * Revision 2.2  90/08/30  11:03:35  bohman
 * 	Created.
 * 	[90/08/29  12:00:54  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/ufs_machdep.c
 */

/*
 * Copyright (c) 1983, 1988 Regents of the University of California.
 * All rights reserved.
 *
 *	@(#)ufs_machdep.c	7.2 (Berkeley) 6/29/88
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/user.h>
#include <sys/buf.h>
#include <sys/conf.h>
#include <sys/proc.h>
#include <sys/vm.h>

/*
 * Machine dependent handling of the buffer cache.
 */

/*
 * Expand or contract the actual memory allocated to a buffer.
 * If no memory is available, release buffer and take error exit
 */
allocbuf(tp, size)
	register struct buf *tp;
	int size;
{
	register struct buf *bp, *ep;
	int sizealloc, take;

	sizealloc = roundup(size, CLBYTES);
	/*
	 * Buffer size does not change
	 */
	if (sizealloc == tp->b_bufsize)
		goto out;
	/*
	 * Buffer size is shrinking.
	 * Place excess space in a buffer header taken from the
	 * BQ_EMPTY buffer list and placed on the "most free" list.
	 * If no extra buffer headers are available, leave the
	 * extra space in the present buffer.
	 *
	 * assert(MAXBSIZE == PAGE_SIZE)
	 */
	if (sizealloc < tp->b_bufsize)
	    goto out;

	/*
	 * More buffer space is needed. Get it out of buffers on
	 * the "most free" list, placing the empty headers on the
	 * BQ_EMPTY buffer header list.
	 *
	 * assert(MAXBSIZE == PAGE_SIZE)
	 */
	brelse(tp);
	return (0);

out:
	tp->b_bcount = size;
	return (1);
}

/*
 * Release space associated with a buffer.
 */
bfree(bp)
	struct buf *bp;
{

	bp->b_bcount = 0;
}
