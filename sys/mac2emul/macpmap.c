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
 *	File: mac2emul/macpmap.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * mac pmap support.
 */

#include <sys/param.h>

#include <kern/task.h>
#include <kern/thread.h>

#include <vm/vm_map.h>

#include <mac2/pmmu_inline.c>

pmap_set_24bit(pmap)
register pmap_t	pmap;
{
    if (pmap->alt_root == 0) {
	pt_root_alloc(&pmap->alt_ptrt, &pmap->alt_root);
	pmap_do_24bit_map(pmap);
    }

    pmap->flags |= PMAP_F_24BIT;
}

pmap_change(old_pmap, new_pmap)
register pmap_t	old_pmap, new_pmap;
{
    PMMU_RP_reg	rp;

    old_pmap->cpus_using = FALSE;

    rp.limit = PMMU_RP_LIMIT;
    rp.valid = PMMU_VALID_RP;
    if (new_pmap->flags&PMAP_F_24BIT)
	rp.phys = (unsigned long)new_pmap->alt_root;
    else
	rp.phys = (unsigned long)new_pmap->root;
    pmmu_set_crp(&rp);

    flush_cpu_caches();

    new_pmap->cpus_using = TRUE;
}
