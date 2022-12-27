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
 *	File: mac2emul/macmemory.c
 *	Author: David E. Bohman II (CMU macmach)
 */

/*
 * mac memory management.
 */

#include <sys/param.h>
#include <sys/user.h>

#include <kern/task.h>
#include <vm/vm_map.h>

#include <mach/vm_param.h>

#include <mac2slotmgr/Types.h>
#include <mac2slotmgr/Slots.h>
#include <mac2slotmgr/slotmgr.h>

#include <mac2emul/macdefs.h>
#include <mac2emul/macinternal.h>

mac_set_addr_mode(to32bit)
boolean_t	to32bit;
{
    register struct pcb	*pcb = current_thread_pcb();
    register struct mac_internal *mac = pcb->pcb_emul;

    if (pcb->pcb_flags&MAC_24BIT) {
	if (to32bit == TRUE) {
	    pmap_change(vm_map_pmap(mac->task_compat_map),
			vm_map_pmap(current_task()->map));
	    pcb->pcb_flags &= ~MAC_24BIT;
	}
    }
    else {
	if (to32bit == FALSE) {
	    pmap_change(vm_map_pmap(current_task()->map),
			vm_map_pmap(mac->task_compat_map));
	    pcb->pcb_flags |= MAC_24BIT;
	}
    }
}

kern_return_t
mac_compat_map_init(thread)
thread_t	thread;
{
    pmap_t		pmap = pmap_create(0);
    vm_map_t		task_compat_map;
    struct mac_internal	*mac = thread->pcb->pcb_emul;

    if (pmap == PMAP_NULL)
	return (KERN_RESOURCE_SHORTAGE);

    pmap_set_24bit(pmap);

    task_compat_map = vm_map_create(pmap,
				    VM_MIN_ADDRESS,
				    VM_MAX_ADDRESS&0x00ffffff,
				    TRUE);

    if (task_compat_map == VM_MAP_NULL)
	return (KERN_RESOURCE_SHORTAGE);

    mac->task_compat_map = task_compat_map;

    return (KERN_SUCCESS);
}

void
mac_compat_map_cleanup(thread)
thread_t	thread;
{
    struct pcb *pcb = thread->pcb;
    struct mac_internal	*mac = pcb->pcb_emul;
    vm_map_t		map;

    map = mac->task_compat_map;
    mac->task_compat_map = VM_MAP_NULL;

    if (map != VM_MAP_NULL)
	vm_map_deallocate(map);
}

vm_map_t
mac_task_compat_map(thread)
thread_t	thread;
{
    return ((thread)->pcb->pcb_emul->task_compat_map);
}

struct mac_phys_region {
    vm_offset_t	    phys;
    vm_size_t	    size;
    vm_prot_t	    perm;
} mac_phys_region[] = {
    { 0x40800000, (512*1024),
	  VM_PROT_EXECUTE|VM_PROT_READ },	/* ROM */
    { 0x50f04000, (8*1024),
	  VM_PROT_READ|VM_PROT_WRITE },		/* SCC */
    { 0x50f14000, (8*1024),
	  VM_PROT_READ|VM_PROT_WRITE },		/* ASC */
    { 0x50f16000, (8*1024),
	  VM_PROT_READ|VM_PROT_WRITE },		/* IWM */
    { 0xf9000000, (6*16*1024*1024),
	  VM_PROT_READ|VM_PROT_WRITE },		/* NUBUS */
#define MAC_PHYS_SLOT_REGION	4
    { 0 }
};

boolean_t
mac_map_phys_check(phys, size, perm)
vm_offset_t	phys;
vm_size_t	size;
vm_prot_t	perm;
{
    register struct mac_phys_region	*p;

    for (p = mac_phys_region; ; p++) {
	if (p->phys == 0)
	    return (FALSE);

	if (phys < p->phys)
	    return (FALSE);

	if (phys < (p->phys + p->size))
	    break;
    }

    if ((perm & p->perm) != perm)
	return (FALSE);

    if (p == &mac_phys_region[MAC_PHYS_SLOT_REGION]) {
	register 	slot;
	int		slot_lo, slot_hi;
#define slot_ptr_to_slot(p)	(((vm_offset_t)(p) & 0x0f000000) >> 24)

	slot_lo = slot_ptr_to_slot(phys);
	slot_hi = slot_ptr_to_slot(phys+size-1);

	for (slot = slot_lo; slot <= slot_hi; slot++) 
	    if ((slot_to_slotdata_ptr(slot)->SFlags&SLOT_MAPPABLE) == 0)
		return (FALSE);
    }

    return (TRUE);
}
