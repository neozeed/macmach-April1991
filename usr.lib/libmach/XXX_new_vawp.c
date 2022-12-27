/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	XXX_new_vawp.c,v $
 * Revision 1.1  90/02/19  18:07:16  bww
 * 	Mach Release 2.5
 * 	[90/02/19  18:07:07  bww]
 * 
 * Revision 1.3  89/05/05  18:38:49  mrt
 * 	Cleanup for Mach 2.5
 * 
 */
/*  
 * Abstract:
 *	Write around for the obsolete call vm_allocate_with_pager
 */

#include <mach.h>

#undef vm_allocate_with_pager

kern_return_t vm_allocate_with_pager(
	target_task, address, size, anywhere, memory_object, offset)
		vm_task_t target_task;
		vm_address_t *address;
		vm_size_t size;
		boolean_t anywhere;
		memory_object_t memory_object;
		vm_offset_t offset;
{
	return(vm_map(target_task, address, size, 0, anywhere,
		memory_object, offset, FALSE,
		VM_PROT_ALL, VM_PROT_ALL, VM_INHERIT_COPY));
}
