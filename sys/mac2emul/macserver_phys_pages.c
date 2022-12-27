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
 *	File: mac2emul/macserver_phys_pages.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include <mach/mach_types.h>

#include <vm/vm_page.h>

zone_t		macserver_phys_page_zone;

macserver_phys_pages_initialize()
{
    macserver_phys_page_zone = zinit(sizeof (struct vm_page),
				     2048*sizeof (struct vm_page),
				     0, FALSE, "macserver vm_page zone");

    if (macserver_phys_page_zone == ZONE_NULL)
	panic("zinit macserver vm_page zone");				     
}

macserver_phys_pages_insert(obj, offset, size, perm, phys, pageq)
kern_obj_t		obj;
vm_offset_t		offset;
vm_size_t		size;
vm_prot_t		perm;
vm_offset_t		phys;
register queue_t	pageq;
{
    vm_object_t		object;
    vm_page_t		pg;

    object = vm_object_lookup(obj);
    if (object == VM_OBJECT_NULL)
	return (KERN_MEMORY_FAILURE);

    offset = trunc_page(offset);
    size = round_page(size);
    phys = trunc_page(phys);

    if (mac_map_phys_check(phys, size, perm) == FALSE) {
	memory_object_data_error(object,
				 offset, size,
				 KERN_MEMORY_ERROR);
	return (KERN_SUCCESS);
    }	

    for (; size > 0;
	 size -= PAGE_SIZE, offset += PAGE_SIZE, phys += PAGE_SIZE) {

	vm_object_lock(object);
	if ((pg = vm_page_lookup(object, offset)) != VM_PAGE_NULL) {
	    vm_page_lock_queues();
	    vm_page_free(pg);
	    vm_page_unlock_queues();

	    if (pg->private)
		queue_remove(pageq, pg, vm_page_t, pageq);
	    else
		pg = VM_PAGE_NULL;
	}

	if (pg == VM_PAGE_NULL)
	    pg = (vm_page_t)zalloc(macserver_phys_page_zone);

	vm_page_init(pg, object, offset, phys);
	pg->wire_count = 1;
	pg->private = TRUE;
	PAGE_WAKEUP_DONE(pg);
	vm_object_unlock(object);

	queue_enter(pageq, pg, vm_page_t, pageq);
    }

    vm_object_deallocate(object);

    return (KERN_SUCCESS);
}

macserver_phys_pages_terminate(obj, pageq)
kern_obj_t		obj;
register queue_t	pageq;
{
    register vm_page_t		pg;
    register vm_object_t	object;

    if (obj != KERN_OBJ_NULL)
	object = vm_object_lookup(obj);
    else
	object = VM_OBJECT_NULL;

    while (!queue_empty(pageq)) {
	pg = (vm_page_t)queue_first(pageq);
	queue_remove(pageq, pg, vm_page_t, pageq);

	if (object != VM_OBJECT_NULL) {
	    vm_object_lock(object);

	    vm_page_lock_queues();
	    vm_page_free(pg);
	    vm_page_unlock_queues();

	    vm_object_unlock(object);
	}

	zfree(macserver_phys_page_zone, pg);
    }

    if (object != VM_OBJECT_NULL)
	vm_object_deallocate(object);
}
