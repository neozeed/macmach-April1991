/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	XXX_pdu.c,v $
 * Revision 1.1  90/02/19  18:07:29  bww
 * 	Mach Release 2.5
 * 	[90/02/19  18:07:07  bww]
 * 
 * Revision 1.2  89/05/05  18:39:20  mrt
 * 	Cleanup for Mach 2.5
 * 
 */
/*  
 * Abstract:
 *	Write around for the obsolete call pager_data_unavailable
 */

#include <mach.h>

#undef pager_data_unavailable

kern_return_t pager_data_unavailable(
	paging_request, offset, size)
		vm_pager_request_t paging_request;
		vm_offset_t offset;
		vm_size_t size;
{
	return(memory_object_data_unavailable(paging_request, offset, size));
}
