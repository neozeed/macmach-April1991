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
 * $Log:	phys.s,v $
 * Revision 2.2  90/09/04  17:27:04  bohman
 * 	Created.
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/phys.s
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * This file contains routines used to
 * copy, zero physical pages.
 */

#include <mac2/asm.h>

#include <mach/mac2/vm_param.h>

/*
 * pmap_zero_page(phys)
 *
 * zeros a machine-independent page of PHYSICAL
 * memory.
 */
ENTRY(pmap_zero_page)
    movl    sp@(4),a0
    movl    _page_size,d1
    jmp	    _zero_mem

/*
 * pmap_copy_page(src, dst)
 *
 * copy one machine independent page of physical
 * memory to another.
 */
ENTRY(pmap_copy_page)
    movl    sp@(4),a0
    movl    sp@(8),a1
    movl    _page_size,d1
    jmp	    _copy_mem
