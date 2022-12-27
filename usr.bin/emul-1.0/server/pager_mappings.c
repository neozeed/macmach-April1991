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
 *	File: emul/server/pager_mappings.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#include "server_defs.h"
#include "pager_defs.h"

#include "mappings.h"

setup_mappings()
{
    map_ram();

    map_patches();

    map_special();
}

map_ram()
{
    printf("memtop %x\n", memtop);
    /*
     * Create a new region for the
     * system RAM.  Map it into the
     * task.
     */
    pager_new_region(0, memtop,
		     VM_PROT_READ|VM_PROT_WRITE,
		     0,
		     PAGING_OBJ_TYPE_NULL, 0,
		     0,
		     PAGING_OBJ_TYPE_FILE, pagingfile);

    pager_map_region(0, memtop,
		     VM_PROT_READ|VM_PROT_WRITE,
		     0,
		     VM_INHERIT_COPY);

    pager_map_compat_region(0, memtop > 0x800000? 0x800000: memtop,
			    VM_PROT_READ|VM_PROT_WRITE,
			    0,
			    VM_INHERIT_NONE);
}

map_patches()
{
    register vm_offset_t	offset;

    if (read(file, &filehdr, sizeof (struct exec)) < 0)
	unix_error_exit("read header");

    if (filehdr.a_entry != EMUL_PATCHES_START)
	other_error_exit("patches entry wrong");

    /*
     * Text region
     */
    offset = EMUL_PATCHES_START;
    pager_new_region(offset,
		     round_page(filehdr.a_text),
		     VM_PROT_READ,
		     sizeof (filehdr),
		     PAGING_OBJ_TYPE_FILE, file,
		     offset,
		     PAGING_OBJ_TYPE_FILE, pagingfile);

    pager_map_region(offset,
		     round_page(filehdr.a_text),
		     VM_PROT_READ,
		     offset,
		     VM_INHERIT_NONE);

    pager_map_compat_region(offset,
			    round_page(filehdr.a_text),
			    VM_PROT_READ,
			    offset & 0x00ffffff,
			    VM_INHERIT_NONE);

    /*
     * Data region (and part of BSS XXX)
     */
    offset += round_page(filehdr.a_text);
    pager_new_region(offset,
		     round_page(filehdr.a_data),
		     VM_PROT_READ|VM_PROT_WRITE,
		     sizeof (filehdr) + round_page(filehdr.a_text),
		     PAGING_OBJ_TYPE_FILE, file,
		     offset,
		     PAGING_OBJ_TYPE_FILE, pagingfile);

    pager_map_region(offset,
		     round_page(filehdr.a_data),
		     VM_PROT_READ|VM_PROT_WRITE,
		     offset,
		     VM_INHERIT_NONE);

    pager_map_compat_region(offset,
			    round_page(filehdr.a_data),
			    VM_PROT_READ|VM_PROT_WRITE,
			    offset & 0x00ffffff,
			    VM_INHERIT_NONE);

    /*
     * Rest is zero fill
     */
    offset += round_page(filehdr.a_data);
    pager_new_region(offset,
		     EMUL_PATCHES_START + EMUL_PATCHES_SIZE - offset,
		     VM_PROT_READ|VM_PROT_WRITE,
		     0,
		     PAGING_OBJ_TYPE_NULL, 0,
		     offset,
		     PAGING_OBJ_TYPE_FILE, pagingfile);

    pager_map_region(offset,
		     EMUL_PATCHES_START + EMUL_PATCHES_SIZE - offset,
		     VM_PROT_READ|VM_PROT_WRITE,
		     offset,
		     VM_INHERIT_NONE);

    pager_map_compat_region(offset,
			    EMUL_PATCHES_START + EMUL_PATCHES_SIZE - offset,
			    VM_PROT_READ|VM_PROT_WRITE,
			    offset & 0x00ffffff,
			    VM_INHERIT_NONE);
}

map_special()
{
#define pager_special_region(X, prot, max_prot) \
    {							\
	pager_new_region(X##_ADDR,			\
			 X##_SIZE,			\
			 (max_prot),			\
			 0,				\
			 PAGING_OBJ_TYPE_PHYS, 0,	\
			 0,				\
			 PAGING_OBJ_TYPE_NULL, 0);	\
\
	pager_map_region(X##_ADDR,			\
			 X##_SIZE,			\
			 (prot),			\
			 X##_ADDR,			\
			 VM_INHERIT_NONE);		\
\
	pager_map_compat_region(X##_ADDR,		\
				X##_SIZE,		\
				(prot),			\
				X##_COMPAT_ADDR,	\
				VM_INHERIT_NONE);	\
    }

    pager_special_region(ROM, VM_PROT_READ, VM_PROT_READ);

    {
	register slot;

	for (slot = SLOT_NUM_LOW; slot <= SLOT_NUM_HIGH; slot++) {
	    pager_new_region(SLOT_ADDR(slot),
			     SLOT_SIZE,
			     VM_PROT_READ|VM_PROT_WRITE,
			     0,
			     PAGING_OBJ_TYPE_PHYS, 0,
			     0,
			     PAGING_OBJ_TYPE_NULL, 0);

	    pager_map_region(SLOT_ADDR(slot),
			     SLOT_SIZE,
			     VM_PROT_READ|VM_PROT_WRITE,
			     SLOT_ADDR(slot),
			     VM_INHERIT_NONE);

	    pager_map_compat_region(SLOT_ADDR(slot),
				    SLOT_COMPAT_SIZE,
				    VM_PROT_READ|VM_PROT_WRITE,
				    SLOT_COMPAT_ADDR(slot),
				    VM_INHERIT_NONE);
	}
    }

    pager_special_region(SCC,
			 VM_PROT_READ|VM_PROT_WRITE,
			 VM_PROT_READ|VM_PROT_WRITE);

    pager_special_region(SND,
 			 VM_PROT_READ|VM_PROT_WRITE,
			 VM_PROT_READ|VM_PROT_WRITE);

    pager_special_region(IWM,
			 VM_PROT_READ|VM_PROT_WRITE,
			 VM_PROT_READ|VM_PROT_WRITE);
}
