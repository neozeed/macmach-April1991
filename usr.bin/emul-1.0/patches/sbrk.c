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
 *	File: emul/patches/sbrk.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Needed to satisfy external references (libmach.a)
 */

sbrk()
{
    asm("trap #15");
}

brk()
{
    asm("trap #15");
}
