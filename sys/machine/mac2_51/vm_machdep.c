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
 * $Log:	vm_machdep.c,v $
 * Revision 2.2  90/08/30  11:03:41  bohman
 * 	Created.
 * 	[90/08/29  12:01:49  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/vm_machdep.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * Machine dependent virtual memory support.
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/buf.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/vm.h>

#include <machine/cpu.h>
#include <machine/reg.h>
#include <machine/pmap.h>
#include <vm/vm_page.h>
#include <vm/vm_object.h>
#include <vm/vm_kern.h>

/*
 * Check kernel accessibility to a given address.
 */
kernacc(vaddr, count, access)
register unsigned long vaddr, count;
register access;
{
    register pp_t *pp;

    count = btoc((int)vaddr + count) - btop(vaddr);

    while (count--) {
	pp = pmap_pp(kernel_pmap, vaddr);
	if (pp == PP_NULL)
	    return (0);
	if (access == B_READ) {
	    if (pp->valid != PMMU_VALID_PP)
		return (0);
	} else {
	    if ((pp->valid != PMMU_VALID_PP) || (pp->prot == PMMU_WP))
		return (0);
	}
	vaddr += NBPG;
    }
    return (1);
}
