/*
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	XXX_pdp.c,v $
 * Revision 1.1  90/02/19  18:07:21  bww
 * 	Mach Release 2.5
 * 	[90/02/19  18:07:07  bww]
 * 
 * Revision 1.2  89/05/05  18:38:56  mrt
 * 	Cleanup for Mach 2.5
 * 
 */
/*  
 * Abstract:
 *	Write around for the obsolete call pager_data_provided
 */

#include <mach.h>

#undef pager_data_provided

kern_return_t pager_data_provided(
	paging_request, offset, data, dataCnt, lock_value)
		vm_pager_request_t paging_request;
		vm_offset_t offset;
		pointer_t data;
		unsigned int dataCnt;
		vm_prot_t lock_value;
{
	return(memory_object_data_provided(
	paging_request, offset, data, dataCnt, lock_value));
}
