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
 * $Log:	pmap_kmap.h,v $
 * Revision 2.2  90/08/30  11:02:44  bohman
 * 	Created.
 * 	[90/08/29  11:43:50  bohman]
 * 
 */

/*
 *	Apple Macintosh II Mach (macmach)
 *
 *	File: mac2/pmap.c
 *	Author: David E. Bohman II (CMU macmach)
 */

#ifndef	_MAC2_PMAP_KMAP_H_
#define _MAC2_PMAP_KMAP_H_

#include <cputypes.h>

/*
 * Describe special mappings
 * that must be entered as
 * virt == phys in the kernel
 * pmap at startup.
 */

#define	PMAP_ROM_MAP_ADDR	((vm_offset_t)0x40000000)
#define PMAP_ROM_MAP_SIZE	((vm_size_t)(16*1024*1024))
#define PMAP_ROM_MAP_PROT	(VM_PROT_READ|VM_PROT_EXECUTE)

#define PMAP_IO_MAP_ADDR	((vm_offset_t)0x50000000)
#define PMAP_IOX_MAP_ADDR	((vm_offset_t)0x50f00000)
#define PMAP_IO_MAP_SIZE	((vm_size_t)(256*1024))
#define PMAP_IO_MAP_PROT	(VM_PROT_WRITE|VM_PROT_READ)

#define PMAP_SLOT_MAP_ADDR	((vm_offset_t)0xf9000000)
#define PMAP_SLOT_MAP_SIZE	((vm_size_t)(6*16*1024*1024))
#define PMAP_SLOT_MAP_PROT	(VM_PROT_WRITE|VM_PROT_READ)

#endif	_MAC2_PMAP_KMAP_H_
