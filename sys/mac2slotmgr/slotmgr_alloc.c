/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	slotmgr_alloc.c,v $
 * Revision 2.2  90/08/30  11:47:39  bohman
 * 	Created.
 * 	[90/08/29  13:03:44  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2slotmgr/slotmgr_alloc.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Slot Manager
 * MACH kernel memory allocation.
 * See "Inside Macintosh II".
 */

#include <mach/mach_types.h>

#define MEMBLK_MAGIC	((unsigned long)0x45673210)

vm_offset_t
NewPtr(size)
register unsigned long size;
{
    register vm_offset_t p;

    size += sizeof (unsigned long) + sizeof (MEMBLK_MAGIC);
    p = kalloc(size);
    if (p == 0)
	return (0);

    *((unsigned long *)p) = size;

    *((unsigned long *)(p + sizeof (unsigned long))) = MEMBLK_MAGIC;

    return (p + sizeof (unsigned long) + sizeof (MEMBLK_MAGIC));
}

DisposPtr(p)
register vm_offset_t p;
{
    p -= sizeof (MEMBLK_MAGIC);
    if (*((unsigned long *)p) != MEMBLK_MAGIC) {
	printf("DisposPtr: bad magic: block not freed\n");
	return;
    }
    p -= sizeof (unsigned long);
    kfree(p, *(unsigned long *)p);
}

vm_offset_t
NewHandle(size)
register unsigned long size;
{
    register vm_offset_t p;

    p = NewPtr(size + sizeof (vm_offset_t));
    if (p == 0)
	return (0);

    *((vm_offset_t *)p) = (p + sizeof (vm_offset_t));

    return (p);
}

DisposHandle(h)
register vm_offset_t h;
{
    DisposPtr(h);
}

vm_offset_t
RecoverHandle(p)
vm_offset_t p;
{
    return (p - sizeof (vm_offset_t));
}

