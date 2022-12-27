/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	XXX_plr.c,v $
 * Revision 1.1  90/02/19  18:07:33  bww
 * 	Mach Release 2.5
 * 	[90/02/19  18:07:07  bww]
 * 
 * Revision 1.2  89/05/05  18:39:27  mrt
 * 	Cleanup for Mach 2.5
 * 
 */
/*  
 * Abstract:
 *	Write around for the obsolete pager lock calls
 */

#include <mach.h>


#undef pager_lock_request

kern_return_t pager_lock_request(
	paging_request, offset, size, should_clean, should_flush, lock_value, reply_to)
		vm_pager_request_t paging_request;
		vm_offset_t offset;
		vm_size_t size;
		boolean_t should_clean;
		boolean_t should_flush;
		vm_prot_t lock_value;
		port_t reply_to;
{
	return(memory_object_lock_request(
	paging_request, offset, size, should_clean, should_flush, lock_value, reply_to));
}
